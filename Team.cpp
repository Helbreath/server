#include "Team.h"

extern void PutLogList(char* cMsg);
extern char G_cTxt[512];

Team::Team()
{
	onesec = timeGetTime();
	bteam = false;
	bend = false;
	endcount = 0;
	pjoin = 0;
	iteam = -1;
	ekxkill = 10;
	checkstop = 300;
	rew = 50;
	for (int i = 0; i < 4; i++)
	{
		team[i].kills = 0;
		//team[i].maxkills = 100;
	}
}

Team::~Team()
{

}


void Team::TeamTimer()
{
	auto g = G_pGame;

	auto dwTime = timeGetTime();

	if ((dwTime - onesec) > 1 _s) {

		if (bend)
		{
			endcount++;

			if (endcount == 10)
			{
				DisableEvent();

			}
		}

		for (int i = 0; i < DEF_MAXCLIENTS; i++)
		{
			auto pi = g->m_pClientList[i];
			if (!pi) continue;
			CheckAfk(i);
		}

		onesec = dwTime;
	}
}

void Team::Join(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;
	if (p->IsInMap("team")) return;
	if (!bteam) return;
	short x, y;

	if (!p->IsTeamPlayer())
	{

		if (iteam == 3) iteam = -1;
		iteam++;
		p->iteam = iteam;

	}

	switch (iteam)
	{
	case 0: x = 36; y = 33; break;
	case 1: x = 76; y = 31; break;
	case 2: x = 36; y = 70; break;
	case 3: x = 62; y = 70; break;
	default: return; break;
	}

	if (!MakeItems(client, iteam)) return;
	EquipCape(client);
	EquipBoots(client);
	NotPoints(client);
	g->RequestTeleportHandler(client, "2", "team", x, y);
}

bool Team::MakeItems(int client, short steam)
{
	char color;
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return false;

	if (p->GetItemCount() > 48)
	{
		g->ShowClientMsg(client, "You need 2 free space in bag for this event.");
		return false;
	}

	switch (steam)
	{
	case 0: color = 14; break; //red
	case 1: color = 10; break; //blue 
	case 2: color = 5; break; //green
	case 3: color = 13; break; //yellow
	default: return false; break;
	}

	CreateCape(client, "Cape+1", color);
	CreateBoots(client, "LongBoots", color);
	return true;
}

void Team::Kill(int iattacker, int itarget)
{
	if (bend) return;
	auto g = G_pGame;
	auto att = g->m_pClientList[iattacker];
	auto tar = g->m_pClientList[itarget];
	if (!att || !tar) return;

	att->m_iEnemyKillCount += 5;
	if (att->m_iEnemyKillCount > att->m_iMaxEK)
	{
		att->m_iMaxEK = att->m_iEnemyKillCount;
	}
	//att->Send(iattacker, DEF_NOTIFY_ENEMYKILLREWARD, itarget);
	g->SendNotifyMsg(0, iattacker, DEF_NOTIFY_ENEMYKILLS, att->m_iEnemyKillCount, att->m_iMaxEK, 0, 0);
	g->calcularTop15HB(iattacker);
	team[att->iteam].kills++;
	NotifyPoints();
	//RequestRevive(itarget);
	End(att->iteam);
}

/*
void Team::RequestRevive(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;

	p->m_bIsBeingResurrected = true;
	c_cmd->SendCommand(client, "/revive", 0, 0, 0, 0);
}

void Team::Revive(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;

	if (!p->m_bIsBeingResurrected) return;

	wsprintf(G_cTxt, "(!) Resurrect Player! %s", p->m_cCharName);
	PutLogList(G_cTxt);

	p->m_bIsKilled = false;
	p->m_iHP = g->iGetMaxHP(client) / 2;
	p->m_iMP = ((p->m_iMag * 2) + (p->m_iLevel / 2)) + p->m_iInt / 2;
	p->m_iSP = (p->m_iStr * 2) + (p->m_iLevel / 2);
	p->m_iHungerStatus = 100;
	p->m_bIsBeingResurrected = false;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;

	g->RequestTeleportHandler(client, "2", p->m_cMapName, -1, -1);
}

void Team::DontRevive(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;

	if (!p->m_bIsBeingResurrected) return;

	wsprintf(G_cTxt, "(!) Resurrect Player! %s", p->m_cCharName);
	PutLogList(G_cTxt);

	p->m_bIsKilled = false;
	p->m_iHP = g->iGetMaxHP(client) / 2;
	p->m_iMP = ((p->m_iMag * 2) + (p->m_iLevel / 2)) + p->m_iInt / 2;
	p->m_iSP = (p->m_iStr * 2) + (p->m_iLevel / 2);
	p->m_iHungerStatus = 100;
	p->m_bIsBeingResurrected = false;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
	p->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 0;

	if (p->IsLocation("elvine"))
		g->RequestTeleportHandler(client, "2", "elvine", -1, -1, true);
	else
		g->RequestTeleportHandler(client, "2", "aresden", -1, -1, true);
}
*/

void Team::NotPoints(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;

	//for (int i = 0; i < 4; i++)
	//{
		//c_cmd->SendCommand(client, "/teamkills", i, team[i].kills, 0, 0);
		//c_cmd->SendCommand(client, "/teammaxkills", team[i].maxkills, 0, 0, 0);
		G_pGame->SendNotifyMsg(0, client, DEF_NOTIFY_TEAMARENA, team[0].kills
			, team[1].kills, team[2].kills, 0, team[3].kills
			, 0, 0);
	//}

}

void Team::NotifyPoints()
{
	auto g = G_pGame;
	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = g->m_pClientList[i];
		if (!pi) continue;
		/*c_cmd->SendCommand(i, "/teamkills", 0, team[0].kills, 0, 0);
		c_cmd->SendCommand(i, "/teamkills", 1, team[1].kills, 0, 0);
		c_cmd->SendCommand(i, "/teamkills", 2, team[2].kills, 0, 0);
		c_cmd->SendCommand(i, "/teamkills", 3, team[3].kills, 0, 0);*/
		G_pGame->SendNotifyMsg(0, i, DEF_NOTIFY_TEAMARENA, team[0].kills
			, team[1].kills, team[2].kills, 0, team[3].kills
			, 0, 0);
	}
}

void Team::End(int iteam)
{
	auto g = G_pGame;
	if (team[iteam].kills == 200)
	{
		for (int i = 0; i < DEF_MAXCLIENTS; i++)
		{
			auto pi = g->m_pClientList[i];
			if (!pi) continue;

			switch (iteam)
			{
			case 0: wsprintf(G_cTxt, "Red Team Wins!"); break;
			case 1: wsprintf(G_cTxt, "Blue Team Wins!"); break;
			case 2: wsprintf(G_cTxt, "Green Team Wins!"); break;
			case 3: wsprintf(G_cTxt, "Yellow Team Wins!"); break;
			}

			g->SendAlertMsg(i, G_cTxt);
		}
		Reward(iteam);
		bend = true;
	}
}

void Team::Reward(int iteam)
{
	auto g = G_pGame;
	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = g->m_pClientList[i];
		if (!pi) continue;
		if (string(pi->m_cMapName) != "team") continue;
		if (pi->iteam != iteam) continue;

		//pi->m_iTokens += rew;

		pi->m_iEnemyKillCount += 150;
		if (pi->m_iEnemyKillCount > pi->m_iMaxEK)
		{
			pi->m_iMaxEK = pi->m_iEnemyKillCount;
		}
		g->SendNotifyMsg(0, i, DEF_NOTIFY_ENEMYKILLS, pi->m_iEnemyKillCount, pi->m_iMaxEK, 0, 0);
		g->calcularTop15HB(i);
		g->ShowClientMsg(i, "You've received 150 EKs for been in the winner team!");
	}
}

void Team::EnableEvent()
{
	auto g = G_pGame;
	bend = false;
	endcount = 0;
	pjoin = 0;
	iteam = -1;
	for (int i = 0; i < 4; i++)
	{
		team[i].kills = 0;
		//team[i].maxkills = 200;
	}

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = g->m_pClientList[i];
		if (!pi) continue;
		g->SendAlertMsg(i, "Event Team Arena Enabled");
	}
	
}

void Team::DisableEvent()
{
	auto g = G_pGame;
	bend = false;
	endcount = 0;
	pjoin = 0;
	iteam = -1;
	for (int i = 0; i < 4; i++)
	{
		team[i].kills = 0;
	}

	for (int i = 0; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = g->m_pClientList[i];
		if (!pi) continue;
		g->SendAlertMsg(i, "Event Team Arena Disabled");
	}
}

void Team::CreateCape(int client, char* itemname, char color)
{
	auto g = G_pGame;
	char   seps[] = "= \t\n";
	char* cp, cData[256];
	class  CStrTok* pStrTok;
	class  CItem* pItem;
	short* sp;
	int    iRet, iEraseReq;
	DWORD* dwp;
	WORD* wp;

	auto p = g->m_pClientList[client];
	if (!p) return;

	pItem = new class CItem;

	if (g->_bInitItemAttr(pItem, itemname) == false) {
		delete pItem;
		return;
	}

	ZeroMemory(cData, sizeof(cData));

	pItem->m_cItemColor = color;
	pItem->teamcape = true;

	if (g->_bAddClientItemList(client, pItem, &iEraseReq) == true) {

		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;

		cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

		*cp = 1;
		cp++;

		memcpy(cp, pItem->m_cName, 20);
		cp += 20;

		dwp = (DWORD*)cp;
		*dwp = pItem->m_dwCount;
		cp += 4;

		*cp = pItem->m_cItemType;
		cp++;

		*cp = pItem->m_cEquipPos;
		cp++;

		*cp = (char)0;
		cp++;

		sp = (short*)cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;

		*cp = pItem->m_cGenderLimit;
		cp++;

		wp = (WORD*)cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;

		wp = (WORD*)cp;
		*wp = pItem->m_wWeight;
		cp += 2;

		sp = (short*)cp;
		*sp = pItem->m_sSprite;
		cp += 2;

		sp = (short*)cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;

		*cp = pItem->m_cItemColor;
		cp++;

		*cp = (char)pItem->m_sItemSpecEffectValue2;
		cp++;

		dwp = (DWORD*)cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;

		*cp = (char)pItem->m_sItemSpecEffectValue3;
		cp++;



		if (iEraseReq == 1) {
			delete pItem;
			pItem = 0;
		}

		iRet = p->m_pXSock->iSendMsg(cData, 54);


		return;
	}
	else {

		delete pItem;
		return;
	}
	if (pStrTok != 0) delete pStrTok;

}

void Team::CreateBoots(int client, char* itemname, char color)
{
	auto g = G_pGame;
	char   seps[] = "= \t\n";
	char* cp, cData[256];
	class  CStrTok* pStrTok;
	class  CItem* pItem;
	short* sp;
	int    iRet, iEraseReq;
	DWORD* dwp;
	WORD* wp;

	auto p = g->m_pClientList[client];
	if (!p) return;

	pItem = new class CItem;

	if (g->_bInitItemAttr(pItem, itemname) == false) {
		delete pItem;
		return;
	}

	ZeroMemory(cData, sizeof(cData));

	pItem->m_cItemColor = color;
	pItem->teamboots = true;

	if (g->_bAddClientItemList(client, pItem, &iEraseReq) == true) {

		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;

		cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

		*cp = 1;
		cp++;

		memcpy(cp, pItem->m_cName, 20);
		cp += 20;

		dwp = (DWORD*)cp;
		*dwp = pItem->m_dwCount;
		cp += 4;

		*cp = pItem->m_cItemType;
		cp++;

		*cp = pItem->m_cEquipPos;
		cp++;

		*cp = (char)0;
		cp++;

		sp = (short*)cp;
		*sp = pItem->m_sLevelLimit;
		cp += 2;

		*cp = pItem->m_cGenderLimit;
		cp++;

		wp = (WORD*)cp;
		*wp = pItem->m_wCurLifeSpan;
		cp += 2;

		wp = (WORD*)cp;
		*wp = pItem->m_wWeight;
		cp += 2;

		sp = (short*)cp;
		*sp = pItem->m_sSprite;
		cp += 2;

		sp = (short*)cp;
		*sp = pItem->m_sSpriteFrame;
		cp += 2;

		*cp = pItem->m_cItemColor;
		cp++;

		*cp = (char)pItem->m_sItemSpecEffectValue2;
		cp++;

		dwp = (DWORD*)cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;

		*cp = (char)pItem->m_sItemSpecEffectValue3;
		cp++;



		if (iEraseReq == 1) {
			delete pItem;
			pItem = 0;
		}

		iRet = p->m_pXSock->iSendMsg(cData, 54);


		return;
	}
	else {

		delete pItem;
		return;
	}
	if (pStrTok != 0) delete pStrTok;

}

void Team::CheckAfk(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;
	if (!p->IsInMap("team")) return;
	if (p->m_iAdminUserLevel != 0) return;

	p->stoptime++;

	if (p->stoptime == checkstop)
	{
		if (p->IsLocation("elvine"))
			g->RequestTeleportHandler(client, "2", "elvine", -1, -1, true);
		else
			g->RequestTeleportHandler(client, "2", "aresden", -1, -1, true);

		p->stoptime = 0;
	}
}

void Team::EquipCape(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;

	int index = -1;
	for (int it = 0; it < DEF_MAXITEMS; it++)
	{
		CItem* item = g->m_pClientList[client]->m_pItemList[it];
		if (!item) continue;

		if (item->teamcape)
		{
			index = it;
			break;
		}

	}
	if (index != -1)
	{
		g->bEquipItemHandler(client, index, true);
		g->SendNotifyMsg(0, client, NOTIFY_EQUIPITEM2, index, 0, 0, 0, 0, 0, 0);
	}
}

void Team::EquipBoots(int client)
{
	auto g = G_pGame;
	auto p = g->m_pClientList[client];
	if (!p) return;

	int index = -1;
	for (int it = 0; it < DEF_MAXITEMS; it++)
	{
		CItem* item = g->m_pClientList[client]->m_pItemList[it];
		if (!item) continue;

		if (item->teamboots)
		{
			index = it;
			break;
		}

	}
	if (index != -1)
	{
		g->bEquipItemHandler(client, index, true);
		g->SendNotifyMsg(0, client, NOTIFY_EQUIPITEM2, index, 0, 0, 0, 0, 0, 0);
	}
}



void Team::DeleteCape(int iClientH) {
	if (!G_pGame->m_pClientList[iClientH]) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		CItem* it = G_pGame->m_pClientList[iClientH]->m_pItemList[i];
		if (!it)
			continue;

		if (!it->teamcape)
			continue;

		G_pGame->ItemDepleteHandler(iClientH, i, false, true);
	}
}


void Team::DeleteBoots(int iClientH) {
	if (!G_pGame->m_pClientList[iClientH]) return;

	for (int i = 0; i < DEF_MAXITEMS; i++)
	{
		CItem* it = G_pGame->m_pClientList[iClientH]->m_pItemList[i];
		if (!it)
			continue;

		if (!it->teamboots)
			continue;

		G_pGame->ItemDepleteHandler(iClientH, i, false, true);
	}
}