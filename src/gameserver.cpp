

#include "gserver.hpp"
#include "client.hpp"
#include "map.hpp"
#include "itembag.hpp"
#include "build_item.hpp"
#include "chat_handler.hpp"
#include "delay_event.hpp"
#include "dropmanager.hpp"
#include "fish.hpp"
#include "mineral.hpp"
#include "guild.hpp"
#include "inventory.hpp"
#include "inventory_mgr.hpp"
#include "magic.hpp"
#include "npc.hpp"
#include "party.hpp"
#include "party_mgr.hpp"
#include "skill.hpp"
#include "potion.hpp"
#include "streams.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "utility.hpp"
#include "defines.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace hbx
{

bool gserver::Init(std::string config)
{
// 	configfile = config;
// 	try
// 	{
// 		using Poco::JSON::Parser;
// 		using Poco::Dynamic::Var;
// 		using Poco::JSON::Object;
// 		using Poco::JSON::Array;
// 
// 		Parser parser;
// 
// 		{
// 			std::ifstream config(configfile.c_str());
// 			Var parsed = parser.parse(config);
// 			Var parsedResult = parser.result();
// 
// 			logger->information(Poco::format("Loading %s", configfile));
// 
// 			Object::Ptr configOptions = parsedResult.extract<Object::Ptr>();
// 			if (configOptions)
// 			{
// 				worldname = configOptions->get("worldname").convert<string>();
// 				logger->information(Poco::format("worldname: %s", worldname));
// 
// 				sqlhost = configOptions->get("sqlhost").convert<string>();
// 				logger->information(Poco::format("sqlhost: %s", sqlhost));
// 
// 				sqluser = configOptions->get("sqluser").convert<string>();
// 				logger->information(Poco::format("sqluser: %s", sqluser));
// 
// 				sqlpass = configOptions->get("sqlpass").convert<string>();
// 				logger->information("sqlpass set");
// 
// 				sqldb = configOptions->get("sqldb").convert<string>();
// 				logger->information(Poco::format("sqldb: %s", sqldb));
// 
// 				if (configOptions->has("maps"))
// 				{
// 					Array::Ptr maps = configOptions->getArray("maps");
// 					for (int i = 0; i < maps->size(); ++i)
// 					{
// 						string mapname = maps->get(i).convert<string>();
// 						map * map = new map(this);
// 						try
// 						{
// 							if (!map_->bInit(mapname))
// 								return false;
// 						}
// 						catch (...)
// 						{
// 							logger->fatal(Poco::format("Failed to load map: %s", mapname));
// 							return false;
// 						}
// 						maplist.push_back(map);
// 					}
// 				}
// 			}
// 		}
// 
// 		{
// 			std::ifstream config("npc.json");
// 			parser.reset();
// 			Var parsed = parser.parse(config);
// 			Var parsedResult = parser.result();
// 
// 			logger->information("Loading npc.json");
// 
// 			Object::Ptr configOptions = parsedResult.extract<Object::Ptr>();
// 			if (configOptions)
// 			{
// 				if (configOptions->has("npcs"))
// 				{
// 					Array::Ptr npcs = configOptions->getArray("npcs");
// 					for (int i = 0; i < npcs->size(); ++i)
// 					{
// 						Array::Ptr npc = npcs->getArray(i);
// 
// 						
// 
// 						Npc * newnpc = new Npc(0, 0);
// 						newnpc->name = npc->get(0).convert<string>();
// 						newnpc->SetType((uint8_t)npc->get(1).convert<uint8_t>());
// 						newnpc->SetTypeOriginal(newnpc->get_type());
// 						newnpc->m_iHitDice = npc->get(2).convert<uint32_t>();
// 						newnpc->m_iDefenseRatio = npc->get(3).convert<uint32_t>();
// 						newnpc->m_iHitRatio = npc->get(4).convert<uint32_t>();
// 						newnpc->m_iMinBravery = npc->get(5).convert<uint32_t>();
// 						newnpc->m_iExpDice = npc->get(6).convert<uint32_t>();
// 						newnpc->m_cAttackDiceThrow = npc->get(7).convert<uint8_t>();
// 						newnpc->m_cAttackDiceRange = npc->get(8).convert<uint8_t>();
// 						newnpc->m_cSize = npc->get(9).convert<int8_t>();
// 						newnpc->side = (Side)npc->get(10).convert<int8_t>();
// 						newnpc->m_cActionLimit = npc->get(11).convert<int8_t>();
// 						newnpc->timeActionInterval = npc->get(12).convert<uint64_t>();
// 						newnpc->m_cResistMagic = npc->get(13).convert<int16_t>();
// 						newnpc->m_cMagicLevel = npc->get(14).convert<int8_t>();
// 						newnpc->m_cDayOfWeekLimit = npc->get(15).convert<int8_t>();
// 						newnpc->m_cChatMsgPresence = npc->get(16).convert<int8_t>();
// 						newnpc->m_cTargetSearchRange = npc->get(17).convert<int8_t>();
// 						newnpc->timeRegen = npc->get(18).convert<uint64_t>();
// 						newnpc->element = (Element)npc->get(19).convert<uint8_t>();
// 						newnpc->absDamage = npc->get(20).convert<uint32_t>();
// 						newnpc->m_iMaxMana = npc->get(21).convert<uint32_t>();
// 						newnpc->m_iMagicHitRatio = npc->get(22).convert<uint32_t>();
// 						newnpc->attackRange = npc->get(23).convert<uint32_t>();
// 						newnpc->goldDropValue = npc->get(24).convert<uint64_t>() * GOLDDROPMULTIPLIER;
// 
// 						if (!m_npcConfigList[newnpc->get_type()])
// 							m_npcConfigList[newnpc->get_type()] = newnpc;
// 						else
// 							delete newnpc;
// 					}
// 				}
// 			}
// 		}
// 
// 		{
// 			std::ifstream config("item.json");
// 			parser.reset();
// 			Var parsed = parser.parse(config);
// 			Var parsedResult = parser.result();
// 
// 			logger->information("Loading item.json");
// 
// 			Object::Ptr configOptions = parsedResult.extract<Object::Ptr>();
// 			if (configOptions)
// 			{
// 				if (configOptions->has("items"))
// 				{
// 					Array::Ptr items = configOptions->getArray("items");
// 					for (int i = 0; i < items->size(); ++i)
// 					{
// 						Array::Ptr item = items->getArray(i);
// 
// 						item * newitem = new item();
// 						
// 						try {
// 							newitem_->m_sIDnum = item_->get(0).convert<int64_t>();
// 							newitem_->name = item_->get(1).convert<string>();
// 							newitem_->m_cItemType = item_->get(2).convert<int8_t>();
// 							newitem_->m_cEquipPos = item_->get(3).convert<int8_t>();
// 							newitem_->itemEffectType = item_->get(4).convert<int16_t>();
// 							newitem_->itemEffectV1 = item_->get(5).convert<int16_t>();
// 							newitem_->itemEffectV2 = item_->get(6).convert<int16_t>();
// 							newitem_->itemEffectV3 = item_->get(7).convert<int16_t>();
// 							newitem_->itemEffectV4 = item_->get(8).convert<int16_t>();
// 							newitem_->itemEffectV5 = item_->get(9).convert<int16_t>();
// 							newitem_->itemEffectV6 = item_->get(10).convert<int16_t>();
// 							newitem_->maxDurability = item_->get(11).convert<int32_t>();
// 							newitem_->specialEffect = item_->get(12).convert<int16_t>();
// 							newitem_->spriteID = item_->get(13).convert<int16_t>();
// 							newitem_->spriteFrame = item_->get(14).convert<int16_t>();
// 							newitem_->price = item_->get(15).convert<int32_t>();
// 							newitem_->weight = item_->get(16).convert<uint16_t>();
// 							newitem_->appearanceValue = item_->get(17).convert<int8_t>();
// 							newitem_->swingSpeed = item_->get(18).convert<int8_t>();
// 							newitem_->levelLimit = item_->get(19).convert<int16_t>();
// 							newitem_->genderLimit = item_->get(20).convert<int8_t>();
// 							newitem_->specialEffectV1 = item_->get(21).convert<int16_t>();
// 							newitem_->specialEffectV2 = item_->get(22).convert<int16_t>();
// 							newitem_->relatedSkill = item_->get(23).convert<int16_t>();
// 							newitem_->category = item_->get(24).convert<int8_t>();
// 							newitem_->color = item_->get(25).convert<uint32_t>();
// 						}
// 						catch (...)
// 						{
// 							log->info(Poco::format("e: item: %s", newitem_name));
// 						}
// 
// 						if (!m_pItemConfigList[newitem_->m_sIDnum])
// 							m_pItemConfigList[newitem_->m_sIDnum] = newitem;
// 						else
// 							delete newitem;
// 					}
// 				}
// 			}
// 		}
// 
// 		{
// 			std::ifstream config("magic.json");
// 			parser.reset();
// 			Var parsed = parser.parse(config);
// 			Var parsedResult = parser.result();
// 
// 			logger->information("Loading magic.json");
// 
// 			Object::Ptr configOptions = parsedResult.extract<Object::Ptr>();
// 			if (configOptions)
// 			{
// 				if (configOptions->has("magic"))
// 				{
// 					Array::Ptr magics = configOptions->getArray("magic");
// 					for (int i = 0; i < magics->size(); ++i)
// 					{
// 						Array::Ptr magic = magics->getArray(i);
// 
// 						Magic * newmagic = new Magic();
// 						try {
// 							newmagic->num = magic->get(0).convert<uint16_t>();
// 							newmagic->name = magic->get(1).convert<string>();
// 							newmagic->magicType = magic->get(2).convert<uint16_t>();
// 							newmagic->delayTime = magic->get(3).convert<uint64_t>();
// 							newmagic->lastTime = magic->get(4).convert<uint64_t>();
// 							newmagic->manaCost = magic->get(5).convert<uint16_t>();
// 							newmagic->m_hRange = magic->get(6).convert<uint8_t>();
// 							newmagic->m_vRange = magic->get(7).convert<uint8_t>();
// 							newmagic->m_sValue[0] = magic->get(8).convert<uint16_t>();
// 							newmagic->m_sValue[1] = magic->get(9).convert<uint16_t>();
// 							newmagic->m_sValue[2] = magic->get(10).convert<uint16_t>();
// 							newmagic->m_sValue[3] = magic->get(11).convert<uint16_t>();
// 							newmagic->m_sValue[4] = magic->get(12).convert<uint16_t>();
// 							newmagic->m_sValue[5] = magic->get(13).convert<uint16_t>();
// 							newmagic->m_sValue[6] = magic->get(14).convert<uint16_t>();
// 							newmagic->m_sValue[7] = magic->get(15).convert<uint16_t>();
// 							newmagic->m_sValue[8] = magic->get(16).convert<uint16_t>();
// 							newmagic->m_sIntLimit = magic->get(17).convert<uint16_t>();
// 							newmagic->goldCost = magic->get(18).convert<int32_t>();
// 							newmagic->category = magic->get(19).convert<uint8_t>();
// 							newmagic->element = (Element)magic->get(20).convert<uint8_t>();
// 						}
// 						catch (...)
// 						{
// 							log->info(Poco::format("e: magic: %s", newmagicname));
// 						}
// 
// 						if (!m_pMagicConfigList[newmagic->num])
// 							m_pMagicConfigList[newmagic->num] = newmagic;
// 						else
// 							delete newmagic;
// 					}
// 				}
// 			}
// 		}

	for (int i = 1; i < 300; i++)
		m_iLevelExpTable[i] = iGetLevelExp(i);

	m_iLimitedUserExp = m_iLevelExpTable[LEVELLIMIT+1]; 
	m_iLevelExp51     = m_iLevelExpTable[51]; 


	return true;
}

npc * gserver::GetNpcByName(std::string name)
{
	for (int x = 0; x < MAXNPCTYPES; x++)
	{
		if (m_npcConfigList[x])
		{
			npc * npc_ = m_npcConfigList[x];
			if (npc_->name == name)
			{
				return npc_;
			}
		}
	}
	return nullptr;
}

item * gserver::GetItemByName(std::string name)
{
	if (name == "nothing") return 0;
	for (int x = 0; x < MAXITEMTYPES; x++)
	{
		item * item_ = m_pItemConfigList[x];
		if (item_ && item_->name == name) {
			return item_;
		}
	}
	log->info("Could not find item {}", name);
	return nullptr;
}

uint64_t gserver::iGetLevelExp(uint16_t iLevel)
{
	uint64_t iRet;

	if (iLevel == 0) return 0;
	iRet = iGetLevelExp(iLevel - 1) + int64_t(iLevel) * int64_t( 50 + (iLevel * (iLevel / 17) * (iLevel / 17) ) );

	return iRet;
}

// void gserver::ChatThread()
// {
// 	struct timespec req={0};
// 	req.tv_sec = 0;
// 	req.tv_nsec = 1000000L;//1ms
// 	while (get_status() == server_status::online)
// 	{
// 		if (!chatpipe.empty())
// 		{
// 			//stuff to do!
// 			mutchat.lock();
// 			std::shared_ptr<MsgQueueEntry> msg = chatpipe.front();
// 			chatpipe.pop_front();
// 			mutchat.unlock();
// 
// 			stream_read sr(msg->data, msg->size);
// 
// 			uint32_t msgid = sr.read_int32();
// 
// 			if (msgid == MSGID_COMMAND_CHECKCONNECTION)
// 			{//connection timeout check in chat to ease packet load from actionthread
// 				//TODO: add check for this packet not being received as often as it should. to speed hack freely you could remove this packet from being sent entirely
// 				//TODO: fix client side times
// 				//TODO: cheaters shouldn't be "instantly banned to bi", but instead maybe tile locked unable to react for a few seconds
// // 				uint64_t timereceived, timenow, timegapclient_, timegapserver;
// // 				timenow = utility::current_time();
// // 				timereceived = sr.read_int32();
// // 
// // 				if (msg->client_->m_dwInitCCTimeRcv == NULL) {
// // 					msg->client_->m_dwInitCCTimeRcv = timereceived;
// // 					msg->client_->m_dwInitCCTime = timenow;
// // 				}
// // 				else {
// // 					timegapclient = (timereceived - msg->client_->m_dwInitCCTimeRcv);
// // 					timegapserver = (timenow - msg->client_->m_dwInitCCTime);
// // 
// // 					if (timegapclient < timegapserver) return;
// // 
// // 					if(msg->client_->m_timeHackTurn == 0)
// // 					{
// // 						msg->client_->m_timeHack = timegapclient - timegapserver;
// // 						msg->client_->m_timeHackDif = 0;
// // 					}else{
// // 						msg->client_->m_timeHackDif += (timegapclient - timegapserver) - msg->client_->m_timeHack;
// // 					}
// // 
// // 					if(msg->client_->m_timeHackTurn == 3 &&
// // 						msg->client_->m_timeHackDif > 1400*4)
// // 					{
// // 						string logmsg = Poco::format("(!) Client(%s) Timeout/Hack(1400+) %?d Time gap %?d", msg->client_->m_cCharName, 
// // 							msg->client_->m_timeHackDif/4,   timegapclient - timegapserver);
// // 						poco_error(*logger, logmsg);
// // 						poco_error(*fileLogger, logmsg);
// // 
// // 						if(msg->client_->m_timeHackDif > 2500*4)
// // 						{
// // 							string logmsg = Poco::format("(!) Client(%s) Speed hacker timeout(3000+) %?d Time gap %?d. Banned to BI", msg->client_->m_cCharName, 
// // 								msg->client_->m_timeHackDif/4,   timegapclient - timegapserver);
// // 							poco_error(*logger, logmsg);
// // 							poco_error(*fileLogger, logmsg);
// // 							//DeleteClient(iClientH, true, true, true, true); // "official" code. why delete when you can just teleport them instantly?
// // 							//perhaps delete if you want them to suffer the consequences of speed hacking as they stay in game for up to 10 seconds
// // 							//on disconnect. that might be the only reason
// // 							//TODO: add teleport or delete code later - for now, delete
// // 							gate->mutclientlist.lock_shared();
// // 							DeleteClient(msg->client_, true, false);
// // 							gate->mutclientlist.unlock_shared();
// // 							return;
// // 						}
// // 					}
// // 
// // 					++msg->client_->m_timeHackTurn %= 4;
// // 				}
// 
// 			}
// 			else if (msgid == MSGID_COMMAND_CHATMSG)
// 			{
// 				if (msg->client_->m_iTimeLeft_ShutUp > 0)
// 					continue;
// 				if (msg->client_->map_->flags.chatDisabled && !msg->client_->IsGM())
// 					continue;
//                 msg->client_->map_->IncPlayerActivity(msg->client.get());
// 
// 				try
// 				{
// 					//sr.read_int32();
// 					sr.read_int16();//command
// 					sr.read_int16();//X
// 					sr.read_int16();//Y
// 					std::string charname = sr.read_string();
// 					std::string message = sr.read_string();
// 
// 					chathandler->parse_chat(msg->client_, message);
// 				}
// 				catch (int test)
// 				{
// 					//-193 data length error
// 					logger->error(Poco::format("ChatThread() : %?d", test));
// 				}
// 			}
// 		}
// 		nanosleep(&req,0);
// 	}
// 	logger->information("ChatThread() complete.");
// 	//mysql_thread_end();
// }

// void gserver::TimerThread()
// {
// 	struct timespec req = { 0 };
// 	req.tv_sec = 0;
// 	req.tv_nsec = 1000000L;//1ms
// 
// 	uint64_t t1htimer;
// 	uint64_t t30mintimer;
// 	uint64_t t6mintimer;
// 	uint64_t t5mintimer;
// 	uint64_t t3mintimer;
// 	uint64_t t1mintimer;
// 	uint64_t t30sectimer;
// 	uint64_t t5sectimer;
// 	uint64_t t1sectimer;
// 	uint64_t t100msectimer;
// 	uint64_t ltime;
// 
// 	t1htimer = t30mintimer = t6mintimer = t5mintimer = t3mintimer = t1mintimer = t30sectimer = t5sectimer = t1sectimer = t100msectimer = utility::current_time();
// 
// 	while (serverstatus == SERVERSTATUS_ONLINE)
// 	{
// 		try
// 		{
// 			ltime = ltime = utility::current_time();
// 
// 			if (t100msectimer < ltime)
// 			{
// 				//run delay events
// 				DelayEventProcessor();
// 
// 				t100msectimer += 100;
// 			}
// 			if (t1sectimer < ltime)
// 			{
// 				t1sectimer += 1000;
// 			}
// 			if (t5sectimer < ltime)
// 			{
// 				t5sectimer += 5000;
// 			}
// 			if (t30sectimer < ltime)
// 			{
// 				t30sectimer += 30000;
// 			}
// 			if (t1mintimer < ltime)
// 			{
// 				t1mintimer += 60000;
// 			}
// 			if (t3mintimer < ltime)
// 			{
// 				t3mintimer += 180000;
// 			}
// 			if (t6mintimer < ltime)
// 			{
// 				t6mintimer += 360000;
// 			}
// 			if (t1htimer < ltime)
// 			{
// 
// 				t1htimer += 3600000;
// 			}
// 			nanosleep(&req, 0);
// 		}
// 		catch (Poco::Data::MySQL::MySQLException * e)
// 		{
// 			log->critical(Poco::format("TimerThread() SQL Exception: %s", edisplayText()));
// 		}
// 		catch (...)
// 		{
// 			logger->fatal("Unspecified gserver::TimerThread() Exception.");
// 			return;
// 		}
// 	}
// 	logger->information("TimerThread() complete.");
// 
// 	mysql_thread_end();
// }

void gserver::RequestTeleportHandler(std::shared_ptr<client> client_, char teleportType, string cMapName /*= ""*/, int dX /*= -1*/, int dY /*= -1*/)
{
	uint16_t sX, sY;
	bool bRet, bIsLockedMapNotify;
	bool setRecallTime = true;
	string DestMapName, TempMapName;
	uint16_t destx, desty;
	uint8_t direction, iMapSide;
	client * iExH;
	map * pmap;

	if (!client_) return;
	if (!client_->m_bIsInitComplete) return;
	if (client_->dead_) return;
	if (client_->m_bIsOnWaitingProcess) return;

	if ((teleportType == 1 || teleportType == 3) && (client_->IsInFoeMap() || (client_->map_->flags.apocalypseMap && client_->map_->dynamicGateType != 4)))
	{
		client_->Notify(0, NOTIFY_CANNOTRECALL, 1);
		return;
	}

	sX = client_->x;
	sY = client_->y;

	bRet = client_->map_->bSearchTeleportDest(sX, sY, cMapName, &destx, &desty, &direction);

	if (!bRet && teleportType == 4)
	{
		SendObjectMotionRejectMsg(client_);
		SendNotifyMsg(0, client_, NOTIFY_TELEPORT_REJECTED,sX,sY,0);
		return;
	}

	bIsLockedMapNotify = false;

	if (client_->exchangeMode)
	{
		iExH = client_->exchangePlayer;
		//_ClearExchangeStatus(iExH)
		//_ClearExchangeStatus(client);
	}
	//Since we moved this to the NPC class... Maybe move it back to gserver :/
	//RemoveFromTarget(client);

	client_->map_->ClearOwner(sX, sY);

	SendEventToNearClient_TypeA(client_->shared_from_this(), MSGID_MOTION_EVENT_REJECT, 0, 0, 0);

	iMapSide = iGetMapLocationSide(cMapName);
	if ((client_->lockedMapName == "NONE") && (client_->lockedMapTime > 0))
	{
		uint32_t tmp_mapSide = iMapSide;
		if (tmp_mapSide >= 11) tmp_mapSide -= 10;

		if (tmp_mapSide == 0 || client_->side != tmp_mapSide)
		{
			destx = -1;
			desty = -1;
			bIsLockedMapNotify = true;
			cMapName = client_->lockedMapName;
		}
	}


	if (client_->map_->flags.permIllusionOn)
	{
		client_->remove_magic_effect(MAGICTYPE_CONFUSE);
	}

	pmap = get_map(cMapName);
	
	if(bRet)
	{
		if(pmap == 0 || (bRet && iMapSide && iMapSide <= 10 && (client_->map_->location_name == client_->nation) != 0 && !client_->IsGM() && !client_->is_neutral() ))
		{
			cMapName = client_->map_name;
			destx = client_->x;
			desty = client_->y;
			setRecallTime = false;
		}
	}

	if (bRet && cMapName.length() == 0)
	{
		for (const auto & map_ : maps)
		{
			if (map_->name == cMapName)
			{
				client_->x   = destx;
				client_->y   = desty;
				client_->direction = direction;
				client_->map_ = map_.get();
				client_->map_name = map_->name;
				PlayerMapEntry(client_->sft(), setRecallTime);
				if (bIsLockedMapNotify == true) SendNotifyMsg(0, client_, NOTIFY_LOCKEDMAP, client_->lockedMapTime, 0, 0, client_->lockedMapName);
				return;
			}
		}
		//used to server change here
		return;
	}
	else {
		switch (teleportType)
		{
		case 0: // Forced Recall. 
		case 1: // Normal Recall 
			//if (memcmp(m_pMapList[ player->m_cMapIndex ]->m_cName, "resurr", 6) == 0) return;
			if (client_->level > 80)
				TempMapName, sideMap[ client_->side ];
			else
				TempMapName = sideMapFarm[ client_->side ];

			if ((client_->lockedMapName == "NONE") && (client_->lockedMapTime > 0))
			{
				bIsLockedMapNotify = true;
				TempMapName = client_->lockedMapName;
			}
			
			for (const auto & map_ : maps)
			{
				if (map_->name == TempMapName, 10)
				{
					map_->GetInitialPoint(client_->x, client_->y, client_->nation);
					client_->map_ = map_.get();
					client_->map_name = map_->name;
					PlayerMapEntry(client_->sft(), setRecallTime);
					if (bIsLockedMapNotify == true) SendNotifyMsg(0, client_, NOTIFY_LOCKEDMAP, client_->lockedMapTime, 0, 0, client_->lockedMapName);
					return;
				}
			}
			//used to server change here
			return;

		case 2: // Forced teleport  
		case 3: // Player requested teleport 
			if ((client_->lockedMapName == "NONE") && (client_->lockedMapTime > 0) && (cMapName == "resurr"))
			{
				dX = -1;
				dY = -1;
				bIsLockedMapNotify = true;
				TempMapName = client_->lockedMapName;
			}
			else {
				TempMapName = cMapName;
			}

			pmap = get_map(TempMapName);
			if (pmap == nullptr)
			{
				//all else has failed. no map can be found to send the player to - would normally be a server transfer
				//with the login server either giving them the hg with the map, or saying that the game server is not online
				//for this failsafe, just send them to their respective town
				
				//no error checks because this should never ever trigger
				if (client_->nation.substr(0, 3) == "are")
				{
					pmap = get_map("aresden");
				}
				else if (client_->nation.substr(0, 3) == "elv")
				{
					pmap = get_map("elvine");
				}
				if (pmap == nullptr)
				{
					//map still does not exist - //TODO: log error in future, for now respawn on same map
					client_->map_->GetInitialPoint(client_->x, client_->y, client_->nation);
					client_->x = 50;// dX; //-1;	  			
					client_->y = 50;// dY; //-1;	 
					PlayerMapEntry(client_->sft(), setRecallTime);
					if (bIsLockedMapNotify == true) SendNotifyMsg(0, client_, NOTIFY_LOCKEDMAP, client_->lockedMapTime, 0, 0, client_->lockedMapName);
					return;
				}
				pmap->GetInitialPoint(client_->x, client_->y, client_->nation);
				client_->map_ = pmap;
				client_->map_name = "aresden";

				client_->x   = dX; //-1;	  			
				client_->y   = dY; //-1;	  

				PlayerMapEntry(client_->sft(), setRecallTime);
				if (bIsLockedMapNotify == true) SendNotifyMsg(0, client_, NOTIFY_LOCKEDMAP, client_->lockedMapTime, 0, 0, client_->lockedMapName);
				return;
			}

			client_->map_ = pmap;

			if (dX == -1 || dY == -1)
				pmap->GetInitialPoint(client_->x, client_->y, client_->nation);
			else 
			{
				client_->x   = dX;
				client_->y   = dY;
			}

			client_->map_name = pmap->name;
			break;
		}
	}
	PlayerMapEntry(client_->sft(), setRecallTime);
	if (bIsLockedMapNotify == true) SendNotifyMsg(0, client_, NOTIFY_LOCKEDMAP, client_->lockedMapTime, 0, 0, client_->lockedMapName);
}

void gserver::PlayerMapEntry(std::shared_ptr<client> client_, bool setRecallTime)
{
	if (!client_) return;

	iSetSide(client_.get());

	stream_write sw;

	sw.write_int32(MSGID_RESPONSE_INITDATA);
	sw.write_int16(MSGTYPE_CONFIRM);

	if (client_->observerMode == false)
		client_->map_->bGetEmptyPosition(client_->x, client_->y, client_);
	else client_->map_->GetInitialPoint(client_->x, client_->y, client_->nation);

    uint16_t maxx = moveLocationX[client_->resolution];
    uint16_t maxy = moveLocationY[client_->resolution];

	sw.write_int16(client_->handle);
    sw.write_int16(maxx);
    sw.write_int16(maxy);
	//sw.write_int16(client_->x - 14 - 5);
	//sw.write_int16(client_->y - 12 - 5);
    sw.write_int16(client_->x);
    sw.write_int16(client_->y);

	sw.write_int16(client_->get_type());

	sw.write_int16(client_->m_sAppr1);
	sw.write_int16(client_->m_sAppr2);
	sw.write_int16(client_->m_sAppr3);
	sw.write_int16(client_->m_sAppr4);
	sw.write_int32(client_->m_iApprColor);
	sw.write_int16(client_->m_sHeadApprValue);
	sw.write_int16(client_->m_sBodyApprValue);
	sw.write_int16(client_->m_sArmApprValue);
	sw.write_int16(client_->m_sLegApprValue);

	if (client_->playerKillCount > 0)
		client_->set_status_flag(STATUS_PK, true);

	sw.write_int32(client_->status);

	sw.write_string(client_->map_name, 10);
	sw.write_string(client_->map_->location_name, 10);

	if (client_->map_->fixedDay == true) 
		sw.write_int8(1);
	else
		sw.write_int8(m_cDayOrNight);

	if (client_->map_->fixedDay) 
		sw.write_int8(WEATHER_SUNNY);
	else 
		sw.write_int8(client_->map_->weather);

	sw.write_int32(client_->contribution);

	if(!client_->observerMode && !client_->IsEthereal())
	{
		client_->map_->SetOwner(client_, client_->x, client_->y);
	}

	sw.write_int8((client_->observerMode)?1:0);
	sw.write_int32(client_->m_reputation);
	sw.write_int32(uint32_t(client_->health));
	sw.write_int32(client_->m_iLucky);
	if (client_->m_bIsOnShop && m_iCrusadeWinnerSide == client_->side)//crusade shop price change
		sw.write_int8(-10);
	else
		sw.write_int8(0);

	uint64_t oldpos = sw.position;
	uint16_t total = 0;

	sw.write_int16(0);//total placeholder

    tile * pTileSrc = client_->map_->GetTile(client_->x - maxx / 2, client_->y - maxy / 2);

    for (int y = 0; y < maxy; ++y)
    {
        for (int x = 0; x < maxx; ++x)
        {
            if (WriteTileData(sw, client_, x, y))
                total++;
        }
    }
	char * temp = sw.data + oldpos;
	*((short *)temp) = total;

	client_->write(sw);
/*

    int ytiles = client_->screenheight_v / 32;
    int xtiles = client_->screenwidth_v / 32;
    for (int y = client_->y - ytiles / 2; y < client_->y + ytiles / 2; ++y)
    {
        for (int x = client_->x - xtiles / 2; x < client_->x + xtiles / 2; ++x)
        {
            stream_write sw;
            sw.write_int32(MSGID_MODIFYTILE);
            if (WriteTileData(sw, client_, x, y))
            {
                client_->write(sw);
            }
        }
    }*/

	//	SendEventToNearClient_TypeA(client_, MSGID_MOTION_EVENT_CONFIRM, NULL, NULL, NULL);

// 	if(m_astoria.get() && strcmp(player->m_cMapName, "astoria") == 0)
// 	{
// 		const Casualties * stats = m_astoria->GetStats();
// 		player->Notify(NULL, NOTIFY_CASUALTIES, 
// 			stats[ARESDEN].deaths, stats[ELVINE].deaths,NULL,/*, stats[ISTRIA].deaths,*/ NULL, // Commented out 3rd faction xRisenx
// 			stats[ARESDEN].kills,stats[ELVINE].kills, NULL/*, stats[ISTRIA].kills*/); // Commented out 3rd faction xRisenx
// 
// 		switch(m_astoria->GetEventget_type())
// 		{
// 		case ET_CAPTURE:
// 			UpdateRelicPos(iClientH);
// 			break;
// 
// 		case ET_DESTROY_SHIELD:
// 			player->Notify(NULL, NOTIFY_SHIELDHP, ARESDEN, m_astoria->GetShieldHP(ARESDEN));
// 			player->Notify(NULL, NOTIFY_SHIELDHP, ELVINE, m_astoria->GetShieldHP(ELVINE));
// 			break;
// 		}
// 	}

// 	if (m_bHeldenianMode)
// 		UpdateHeldenianStatus();

	SendEventToNearClient_TypeA(client_, MSGID_MOTION_EVENT_CONFIRM, 0, 0, 0);

/*
	if(setRecallTime)
	{
		player->m_bIsWarLocation = false;
		player->m_iTimeLeft_ForceRecall = 0;

		if(player->IsNeutral())
		{
			if(m_bIsCrusadeMode && !player->IsGM() && 
				((strcmp(m_pMapList[player->m_cMapIndex]->m_cName, sideMap[ARESDEN]) == 0) || 
				 (strcmp(m_pMapList[player->m_cMapIndex]->m_cName, sideMap[ELVINE]) == 0)  / *|| 
				 (strcmp(m_pMapList[player->m_cMapIndex]->m_cName, sideMap[ISTRIA]) == 0)* /))  // Commented out 3rd faction xRisenx
			{
				player->m_dwWarBeginTime = utility::current_time();
				player->m_bIsWarLocation = true;
				player->m_iTimeLeft_ForceRecall = 1; 
			}
		}
		else if (player->IsInFoeMap() && !player->IsGM()) 
		{
			player->m_dwWarBeginTime = utility::current_time();
			player->m_bIsWarLocation = true;

			SetForceRecallTime(iClientH) ;
		}

		if (player->m_iTimeLeft_ForceRecall > 0) {
			SendNotifyMsg(NULL, iClientH, NOTIFY_FORCERECALLTIME, player->m_iTimeLeft_ForceRecall, NULL, NULL, NULL);
		}
	}

	iMapSide = iGetMapLocationSide(m_pMapList[player->m_cMapIndex]->m_cName);

	if (iMapSide >= 11) iTmpMapSide = iMapSide - 10 ;
	else iTmpMapSide = iMapSide ;

	player->m_bIsInBuilding = false ;

	if ((player->m_side != iTmpMapSide) && (iMapSide != 0) ) {
		if ( (iMapSide <= 10) && !player->IsGM() && !player->IsNeutral() )
		{
			player->m_dwWarBeginTime = utility::current_time();
			player->m_bIsWarLocation = true;
			player->m_iTimeLeft_ForceRecall = 1 ; 
			player->m_bIsInBuilding = true ;
		}
	}
	else if (player->IsInJail() && !player->IsGM()) {
		player->m_bIsWarLocation = true;
		player->m_dwWarBeginTime = utility::current_time();

		if (player->m_iTimeLeft_ForceRecall == 0) {
			player->m_iTimeLeft_ForceRecall = 20*5 ; 
		} else if (player->m_iTimeLeft_ForceRecall > 20*5) {
			player->m_iTimeLeft_ForceRecall = 20*5 ;
		}
	}

	_CheckQuestEnvironment(iClientH);

	if (m_astoria.get() && player->m_cMapIndex == m_iAstoriaMapIndex) {
		UpdateRelicPos(iClientH);
	}

	if (player->m_iSpecialAbilityTime == 0) {
		SendNotifyMsg(NULL, iClientH, NOTIFY_SPECIALABILITYENABLED, NULL, NULL, NULL, NULL);
	}

	if (m_bHeldenianMode)
	{
		SendNotifyMsg(NULL, iClientH, NOTIFY_HELDENIANSTART,  NULL , NULL, NULL, NULL);
	}

	if (m_bIsCrusadeMode == true) {
		if (player->m_dwCrusadeGUID == 0) {
			player->m_iCrusadeDuty = 0;
			player->m_iConstructionPoint = 0;
			player->m_dwCrusadeGUID = m_dwCrusadeGUID;
		}
		else if (player->m_dwCrusadeGUID != m_dwCrusadeGUID) {
			player->m_iCrusadeDuty       = 0;
			player->m_iConstructionPoint = 0;
			player->m_iWarContribution   = 0;
			player->m_dwCrusadeGUID = m_dwCrusadeGUID;
			SendNotifyMsg(NULL, iClientH, NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, NULL, 0, NULL, -1);
		}
		SendNotifyMsg(NULL, iClientH, NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, player->m_iCrusadeDuty, NULL, NULL);
	}
	else {
		if (player->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			player->m_iCrusadeDuty = 0;
			player->m_iConstructionPoint = 0;
		}
		else if ((player->m_dwCrusadeGUID != NULL) && (player->m_dwCrusadeGUID != m_dwCrusadeGUID)) {
			SendNotifyMsg(NULL, iClientH, NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, NULL, 0, NULL, -1);
			player->m_iWarContribution   = 0;
			player->m_dwCrusadeGUID = 0;
		}
	}

	if (m_pMapList[player->m_cMapIndex]->m_bIsFightZone) { //Send all map restrictions
		if (m_pMapList[player->m_cMapIndex]->m_isPartyDisabled && !player->IsGM() && player->GetParty()) 
			partyMgr.RemoveFromParty( player );
		if (m_pMapList[player->m_cMapIndex]->m_isShieldDisabled)
			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTSHIELD, true, NULL, NULL, NULL, NULL);
		if (m_pMapList[player->m_cMapIndex]->m_isArmorDisabled){
			if (!m_pClientList[iClientH]->IsGM() && !m_pClientList[iClientH]->is_dead()){
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_HEAD ] != -1){
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_HEAD, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_HEAD ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_HEAD ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BODY ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_BODY, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BODY ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BODY ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_ARMS ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_ARMS, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_ARMS ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_ARMS ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BOOTS ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_BOOTS, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BOOTS ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_BOOTS ], false);
				}
				if ( m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_PANTS ] != -1) {
					SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMRELEASED, EQUIPPOS_PANTS, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_PANTS ], NULL, NULL);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[ EQUIPPOS_PANTS ], false);
				}
				SendEventToNearClient_TypeA(iClientH, OWNERTYPE_PLAYER, MSGID_MOTION_NULL, NULL, NULL, NULL);
			}

			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTARMOR, true, NULL, NULL, NULL, NULL);
		}
		if (m_pMapList[player->m_cMapIndex]->m_isPermIllusionOn){
			if (!player->IsGM()){
				for (i = 1; i < MAXCLIENTS; i++)
					if(m_pClientList[i] != NULL && m_pClientList[i]->IsGM() && m_pClientList[i]->m_cMapIndex == player->m_cMapIndex) break;

				if (i != MAXCLIENTS){
					SendNotifyMsg(NULL, iClientH, NOTIFY_MAGICEFFECTON, MAGICTYPE_CONFUSE, 3, i, NULL);
					player->m_cMagicEffectStatus[ MAGICTYPE_CONFUSE ] = 3;
					player->SetStatusFlag(STATUS_ILLUSION, true);
				}
			}
			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTILLUSION, true, NULL, NULL, NULL, NULL);
		}

		if (m_pMapList[player->m_cMapIndex]->m_isChatDisabled)
			SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTCHAT, m_pMapList[player->m_cMapIndex]->m_isChatDisabled, NULL, NULL, NULL, NULL);

		for (i = 0; i < MAXMAGICTYPE; i++)
			if (m_pMapList[player->m_cMapIndex]->m_magicLimited[i])
				SendNotifyMsg(NULL, iClientH, NOTIFY_EVENTSPELL, true, i, NULL, NULL, NULL);

		poco_warning(*logger, Poco::format("Char(%s)-Enter(%s) Observer(%?d)", (string)player->m_cCharName, (string)player->m_cMapName, player->m_bIsObserverMode));
	}

	if (player->GetParty())
	{
		player->GetParty()->UpdateMemberMap(player);
	}

	if(player->m_guild)
		player->m_guild->MemberMapChanged(player);

	SendNotifyMsg(NULL, iClientH, NOTIFY_SAFEATTACKMODE, NULL, NULL, NULL, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_DOWNSKILLINDEXSET, player->m_iDownSkillIndex, NULL, NULL, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_ITEMPOSLIST, NULL, NULL, NULL, NULL);

	SendNotifyMsg(NULL, iClientH, NOTIFY_CONSTRUCTIONPOINT, player->m_iConstructionPoint, player->m_iWarContribution, 1, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_GIZONITEMUPGRADELEFT, player->m_iGizonItemUpgradeLeft, NULL, NULL, NULL);
	SendNotifyMsg(NULL, iClientH, NOTIFY_SUPERATTACKLEFT, NULL, NULL, NULL, NULL); // Added Show Criticals Left xRisenx
	Notify_ApocalypseGateState(iClientH);

	player->m_nextRecallPoint = 0;
	if (player->m_iQuest) {
		cQuestRemain = (m_pQuestConfigList[player->m_iQuest]->m_iMaxCount - player->m_iCurQuestCount);
		SendNotifyMsg(NULL, iClientH, NOTIFY_QUESTCOUNTER, cQuestRemain, NULL, NULL, NULL);
		_bCheckIsQuestCompleted(iClientH);
	}*/
}

int gserver::iGetMapLocationSide(string MapName)
{
	if(MapName.length() == 0) return 0;

	if(MapName[ MapName.length() - 2] == '_')
	{
		switch(MapName[ MapName.length() - 1])
		{
			case '1':
				return faction::aresden;
			case '2':
				return faction::elvine;
		}
	}

	
	if (MapName == sideMap[faction::aresden]) return faction::aresden + 10 ;
	if (MapName == sideMap[faction::elvine])  return faction::elvine + 10 ;
	if (MapName == "arebrk11") return faction::aresden + 10 ;
	if (MapName == "elvbrk11") return faction::elvine + 10 ;
	if (MapName == "arefarm") return faction::aresden;
	if (MapName == "elvfarm") return faction::elvine;
	if (MapName == "arewrhus") return faction::aresden;
	if (MapName == "elvwrhus") return faction::elvine;

	/*  memcmp 를하기 때문에 앞에서 걸린다.
	if (memcmp(pMapName, "gshop_1f") == 0) return 1;
	if (memcmp(pMapName, "bsmith_1f") == 0) return 1;
	if (memcmp(pMapName, "wrhus_1f") == 0) return 1;

	if (memcmp(pMapName, "gshop_2f") == 0) return 2;
	if (memcmp(pMapName, "bsmith_2f") == 0) return 2;
	if (memcmp(pMapName, "wrhus_2f") == 0) return 2; */


	return 0;
}

void gserver::RequestHuntmode(std::shared_ptr<client> client_)
{
	if (client_ == 0) return;

	if (client_->playerKillCount > 0 ) return;
	if (client_->m_bIsInitComplete == false) return;
	if (client_->map_name.substr(0, 8) == "cityhall") return;
	
	if ((client_->level > DEF_LIMITHUNTERLEVEL) && (client_->civilian == false) ) return;


	if (client_->nation == "aresden")
	{
		client_->nation = "arehunter";
		client_->civilian = true;
	}
	else if (client_->nation == "elvine")
	{
		client_->nation = "elvhunter";
		client_->civilian = true;
	}
	else if (client_->nation == "arehunter")
	{
		client_->nation = "aresden";
		client_->civilian = false;
	}	
    else if (client_->nation == "elvhunter")
    {
		client_->nation = "elvine";
		client_->civilian = false;
	}

//	SendNotifyMsg(0, 0, NOTIFY_RESPONSE_HUNTMODE, 0, 0, 0, client_->m_cLocation);

	SendEventToNearClient_TypeA(client_, MSGID_MOTION_NULL, 0, 0, 0);
}

void gserver::SendNotifyMsg(std::shared_ptr<client> from, std::shared_ptr<client> to, uint16_t wMsgType, uint64_t sV1 /*= 0*/, uint64_t sV2 /*= 0*/, uint64_t sV3 /*= 0*/, string pString /*= ""*/,
							uint64_t sV4 /*= 0*/, uint64_t sV5 /*= 0*/, uint64_t sV6 /*= 0*/, uint64_t sV7 /*= 0*/, uint64_t sV8 /*= 0*/, uint64_t sV9 /*= 0*/, string pString2 /*= ""*/)
{
	stream_write sw;

	if (!to) return; // from may not be set.. only like 2 packets in the ENTIRE GAME use it. mostly trade requests. to HAS to be set

	sw.write_int32(MSGID_NOTIFY);
	sw.write_int16(wMsgType);


	switch (wMsgType)
	{
	case NOTIFY_PINGMAP:
		sw.write_int8(uint8_t(sV1));
		sw.write_int16(uint16_t(sV2));
		sw.write_int16(uint16_t(sV3));
		to->write(sw);
		break;

	case NOTIFY_LOCKEDMAP:
		sw.write_int16(uint16_t(sV1));
		sw.write_string(pString, 10);
		to->write(sw);
		break;

		//Single Int
	case NOTIFY_REQDEF:
	case NOTIFY_SLATE_CREATESUCCESS:
		sw.write_int32(uint32_t(sV1));
		to->write(sw);
		break;
	case NOTIFY_RESPONSE_HUNTMODE:
		sw.write_string(pString, 10);
		to->write(sw);
		break;
	case NOTIFY_PARTY:
		switch (sV1) {
		case 4:
		case 6:
			sw.write_int16(uint16_t(sV1));
			sw.write_int16(uint16_t(sV2));
			sw.write_int16(uint16_t(sV3));
			sw.write_string(pString, 10);
			to->write(sw);
			break;

		case 5:
			sw.write_int16(uint16_t(sV1));
			sw.write_int16(uint16_t(sV2));
			sw.write_int16(uint16_t(sV3));
			sw.write_string(pString, sV3*11);
			to->write(sw);
			break;

		default:
			sw.write_int16(uint16_t(sV1));
			sw.write_int16(uint16_t(sV2));
			sw.write_int16(uint16_t(sV3));
			sw.write_int16(uint16_t(sV4));
			to->write(sw);
			break;
		}
		break;

	case NOTIFY_PARTY_COORDS:
		sw.write_string(pString, 10);
		sw.write_int16(uint16_t(sV1));
		sw.write_int16(uint16_t(sV2));
		to->write(sw);
		break;

		//Single Short
	case NOTIFY_CANNOTCONSTRUCT:
	case NOTIFY_METEORSTRIKECOMING:
	case NOTIFY_METEORSTRIKEHIT:
	case NOTIFY_SPECIALABILITYENABLED:
	case NOTIFY_FORCEDISCONN:
	case NOTIFY_OBSERVERMODE:
	case NOTIFY_QUESTCOMPLETED:
	case NOTIFY_QUESTABORTED:
	case NOTIFY_SLATE_STATUS:
	case NOTIFY_ITEMUPGRADEFAIL:
	case NOTIFY_SKILLUSINGEND:
	case NOTIFY_FORCERECALLTIME:
		sw.write_int16(uint16_t(sV1));
		to->write(sw);
		break;

		//Double Short
	case NOTIFY_TELEPORT_REJECTED:
	case NOTIFY_SKILL:
	case NOTIFY_ITEMRELEASED:
	case NOTIFY_ITEMLIFESPANEND:
		sw.write_int16(uint16_t(sV1));
		sw.write_int16(uint16_t(sV2));
		to->write(sw);
		break;

		//empty packets (just ID)
	case NOTIFY_SLATE_EXP:
	case NOTIFY_SLATE_MANA:
	case NOTIFY_SLATE_INVINCIBLE:
	case NOTIFY_SLATE_CREATEFAIL:
	case NOTIFY_RESURRECTPLAYER:
	case NOTIFY_EVENTRESET:
	case NOTIFY_CRAFTING_SUCCESS:
	case NOTIFY_PORTIONSUCCESS:
	case NOTIFY_LOWPORTIONSKILL:
	case NOTIFY_PORTIONFAIL:
	case NOTIFY_NOMATCHINGPORTION:
	case NOTIFY_CRAFTING_FAIL:
	case NOTIFY_NOMATCHINGCRAFTING:
	case NOTIFY_NO_CRAFT_CONTRIB:
	case NOTIFY_ADMINUSERLEVELLOW:
		to->write(sw);
		break;
	case NOTIFY_ITEMPOSLIST:
/*
 		for (ItemWrap * var : to->itemList)
 		{
			sw.write_int16(var->_item_->x);
			sw.write_int16(var->_item_->y);
 		}
		to->write(sw);*/
 		break;
	case NOTIFY_WHISPERMODEON:
	case NOTIFY_WHISPERMODEOFF:
	case NOTIFY_PLAYERNOTONGAME:
		sw.write_string(pString, 10);
		sw.write_string("          ", 10);
		to->write(sw);
		break;

	case NOTIFY_NOTICEMSG:
		sw.write_string(pString, pString.length());
		sw.write_int8(0);
		to->write(sw);
		break;
	case NOTIFY_EXP:
		sw.write_int32(uint32_t(to->experience));//TODO: make 64bit client side still?
		sw.write_int32(to->m_reputation);
		to->write(sw);
		break;

	case NOTIFY_HP:
		sw.write_int32(uint32_t(to->health));
		sw.write_int32(uint32_t(to->mana));
		to->write(sw);
		break;

	case NOTIFY_MP:
		sw.write_int32(uint32_t(to->mana));
		to->write(sw);
		break;

	case NOTIFY_SP:
		sw.write_int32(to->stamina);
		to->write(sw);
		break;

	case NOTIFY_CHARISMA:
		sw.write_int32(to->m_iCharisma);
		to->write(sw);
		break;

	case NOTIFY_SETTING_SUCCESS:
	case NOTIFY_LEVELUP:
		sw.write_int32(to->level);
		sw.write_int32(to->GetStr());
		sw.write_int32(to->GetVit());
		sw.write_int32(to->GetDex());
		sw.write_int32(to->GetInt());
		sw.write_int32(to->GetMag());
		sw.write_int32(to->GetChr());
		to->write(sw);
		break;

	case NOTIFY_CHANGETITLE:
		sw.write_string(pString, 11);
		to->write(sw);
		break;

	case NOTIFY_REQTITLEANSWER:
		sw.write_int32(sV1);//TitleType
		sw.write_int32(sV2);//ReturnID
		sw.write_int32(sV3);//CrusadeJob
		sw.write_string(pString);//ActiveTitle
		to->write(sw);
		break;

	case NOTIFY_KILLED:

		sw.write_string(pString, 20);//ActiveTitle

		to->write(sw);
		break;

	case NOTIFY_REQGUILDNAMEANSWER:

		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_string(pString);

		to->write(sw);
		break;


	case NOTIFY_GRANDMAGICRESULT:

		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_int16(sV3);
		sw.write_string(pString, 10);
		sw.write_int16(sV4);

		if (sV9 > 0)  {
			sw.write_string(pString2, (sV9 + 1) * 2);
		}
		else
		{
			sw.write_int16(0);
		}

		to->write(sw);
		break;


	case NOTIFY_MAPSTATUSNEXT:
	case NOTIFY_MAPSTATUSLAST:
		sw.write_string(pString, sV1);
		to->write(sw);
		break;

	case NOTIFY_BUILDITEMSUCCESS:
	case NOTIFY_BUILDITEMFAIL:
		if (sV1 >= 0) {
			sw.write_int16(sV1);
		}
		else {
			sw.write_int16(sV1 + 10000);
		}

		sw.write_int16(sV2);

		to->write(sw);
		break;

	case NOTIFY_QUESTREWARD:
		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_int32(sV3);
		sw.write_string(pString, 20);
		sw.write_int32(sV4);

		to->write(sw);
		break;

	case NOTIFY_QUESTCONTENTS:
		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_int16(sV3);
		sw.write_int16(sV4);
		sw.write_int16(sV5);
		sw.write_int16(sV6);
		sw.write_int16(sV7);
		sw.write_int16(sV8);
		sw.write_int16(sV9);
		if (pString2.length() > 0) sw.write_string(pString2, 20);

		to->write(sw);
		break;

	case NOTIFY_GIZONITEMUPGRADELEFT:
	case NOTIFY_ITEMATTRIBUTECHANGE:
		sw.write_int16(sV1);
		sw.write_int32(sV2);
		sw.write_int32(sV3);
		sw.write_int32(sV4);

		to->write(sw);
		break;


	case NOTIFY_GIZONEITEMCHANGE:
		sw.write_int16(sV1);
		sw.write_int8(sV2);
		sw.write_int16(sV3);
		sw.write_int16(sV4);
		sw.write_int16(sV5);
		sw.write_int8(sV6);
		sw.write_int8(sV7);
		sw.write_int32(sV8);
		sw.write_string(pString, 20);

		to->write(sw);
		break;

	case NOTIFY_ITEMCOLORCHANGE:
		sw.write_int16(sV1);
		sw.write_int16(sV2);

		to->write(sw);
		break;
	case NOTIFY_ENEMYKILLS:
		sw.write_int32(sV1);
		sw.write_int32(sV2);

		to->write(sw);
		break;

	case NOTIFY_CRUSADE:
		sw.write_int32(sV1);
		sw.write_int32(sV2);
		sw.write_int32(sV3);
		sw.write_int32(sV4);

		to->write(sw);
		break;

	case NOTIFY_CONSTRUCTIONPOINT:
	case NOTIFY_SPECIALABILITYSTATUS:
	case NOTIFY_DAMAGEMOVE:
		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_int16(sV3);

		to->write(sw);
		break;

	case NOTIFY_NPCTALK:
		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_int16(sV3);
		sw.write_int16(sV4);
		sw.write_int16(sV5);
		sw.write_int16(sV6);
		sw.write_int16(sV7);
		sw.write_int16(sV8);
		sw.write_int16(sV9);
		if (pString.length() > 0) sw.write_string(pString, 20);
		if (pString2.length() > 0) sw.write_string(pString2, 20);

		to->write(sw);
		break;

	case NOTIFY_SUPERATTACKLEFT:
		sw.write_int16(to->superAttack);

		to->write(sw);
		break;

	case NOTIFY_SAFEATTACKMODE:
		sw.write_int8(to->safeAttackMode);

		to->write(sw);
		break;

	case NOTIFY_SERVERSHUTDOWN:
	case NOTIFY_GLOBALATTACKMODE:
	case NOTIFY_WEATHERCHANGE:
		sw.write_int8(sV1);

		to->write(sw);
		break;

	case NOTIFY_CANNOTRATING:
		sw.write_int16(sV1);

		to->write(sw);
		break;

	case NOTIFY_RATINGPLAYER:
		sw.write_int8(sV1);
		sw.write_string(pString, 10);
		sw.write_int32(to->m_reputation);

		to->write(sw);
		break;

	case NOTIFY_PLAYERSHUTUP:
		sw.write_int16(sV1);
		sw.write_string(pString, 10);

		to->write(sw);
		break;

	case NOTIFY_PLAYERPROFILE:
		if (pString.length() > 100) {
			sw.write_string(pString, 100);
		}
		else {
			sw.write_string(pString, pString.length());
		}
		sw.write_int8(0);

		to->write(sw);
		break;

	case NOTIFY_REPAIRITEMPRICE:
	case NOTIFY_SELLITEMPRICE:
		sw.write_int32(sV1);
		sw.write_int32(sV2);
		sw.write_int32(sV3);
		sw.write_int32(sV4);
		sw.write_string(pString, 20);

		to->write(sw);
		break;

	case NOTIFY_CANNOTREPAIRITEM:
	case NOTIFY_CANNOTSELLITEM:
		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_string(pString, 20);

		to->write(sw);
		break;


	case NOTIFY_TOTALUSERS:
		sw.write_int16(m_iTotalGameServerClients);

		to->write(sw);
		break;

	case NOTIFY_MAGICEFFECTOFF:
	case NOTIFY_MAGICEFFECTON:
		sw.write_int16(sV1);
		sw.write_int32(sV2);
		sw.write_int32(sV3);

		to->write(sw);
		break;

	case NOTIFY_SETITEMCOUNT:
		sw.write_int16(sV1);
		sw.write_int32(sV2);
		sw.write_int8(sV3);

		to->write(sw);
		break;

	case NOTIFY_ITEMDEPLETED_ERASEITEM:
		sw.write_int16(sV1);
		sw.write_int16(sV2);

		to->write(sw);
		break;

	case NOTIFY_DROPITEMFIN_COUNTCHANGED:
	case NOTIFY_DROPITEMFIN_ERASEITEM:
		sw.write_int16(sV1);
		sw.write_int32(sV2);

		to->write(sw);
		break;

	case NOTIFY_CANNOTGIVEITEM:
	case NOTIFY_GIVEITEMFIN_COUNTCHANGED:
	case NOTIFY_GIVEITEMFIN_ERASEITEM:
		sw.write_int16(sV1);
		sw.write_int32(sV2);
		sw.write_string(pString, 20);

		to->write(sw);
		break;

	case NOTIFY_ENEMYKILLREWARD:
		sw.write_int32(to->experience);
		sw.write_int32(to->enemyKillCount);
		sw.write_string(pString, 10);
		sw.write_string(pString2, 20);
		sw.write_int16(sV1);//Guild Rank
		sw.write_int16(sV2);//War Contribution

		to->write(sw);
		break;

	case NOTIFY_PKCAPTURED:
		sw.write_int16(sV1);
		sw.write_int16(sV2);
		sw.write_string(pString, 10);
		sw.write_int32(to->m_iRewardGold);
		sw.write_int32(to->experience);

		to->write(sw);
		break;

	case NOTIFY_PKPENALTY:
		sw.write_int32(to->experience);
		sw.write_int32(to->GetStr());
		sw.write_int32(to->GetVit());
		sw.write_int32(to->GetDex());
		sw.write_int32(to->GetInt());
		sw.write_int32(to->GetMag());
		sw.write_int32(to->GetChr());
		sw.write_int32(to->playerKillCount);

		to->write(sw);
		break;

	case NOTIFY_TRAVELERLIMITEDLEVEL:
	case NOTIFY_LIMITEDLEVEL:
		sw.write_int32(to->experience);

		to->write(sw);
		break;

	case NOTIFY_CANNOTRECALL:
	case NOTIFY_CANNOTCRUSADE:
		sw.write_int8(sV1);

		to->write(sw);
		break;

	case NOTIFY_SPELL_SKILL:
		for (int i = 0; i < MAXMAGICTYPE; i++) {
			sw.write_int8(to->magicMastery[i]);
		}

		for (int i = 0; i < MAXSKILLTYPE; i++) {
			sw.write_int8(to->skillMastery[i]);
		}

		to->write(sw);
		break;

// 	case NOTIFY_UPDATETITLELIST:
// 
// 		//for ()
// 		int iNumPoints = (uint32_t)sV1;
// 		int iNextRankPoints = (uint32_t)sV2;
// 		int iRank = (uint32_t)sV3;
// 		int iTitleIndex = (uint32_t)sV4;
// 		char cActiveTitle = (char)sV5;
// 
// 		memset(cp, 0, 21);
// 		memcpy(cp, pString, strlen(pString) + 1);
// 		cp += 21;
// 
// 		ip = (int *)cp;
// 		*ip = iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iTitleIndex;
// 		cp += 4;
// 
// 		*cp = cActiveTitle;
// 		cp++;
// 
// 		/ *ip = (int *)cp;
// 		*ip = player->iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iTitleIndex;
// 		cp += 4;
// 
// 		*cp = player->cActiveTitle;
// 		cp++; */
// 
// 			iRet = player->socket->write(cData, 44);
// 		break;
// 
// 	case NOTIFY_TITLELEVELUP:
// 	{
// 		int iNumPoints = (uint32_t)sV1;
// 		int iNextRankPoints = (uint32_t)sV2;
// 		int iRank = (uint32_t)sV3;
// 		int iTitleIndex = (uint32_t)sV4;
// 
// 		memset(cp, 0, 21);
// 		memcpy(cp, pString, strlen(pString) + 1);
// 		cp += 21;
// 
// 		ip = (int *)cp;
// 		*ip = iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = iTitleIndex;
// 		cp += 4;
// 
// 		/ *ip = (int *)cp;
// 		*ip = player->iNumPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iNextRankPoints;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iRank;
// 		cp += 4;
// 
// 		ip = (int *)cp;
// 		*ip = player->iTitleIndex;
// 		cp += 4; */
// 
// 			iRet = player->socket->write(cData, 43);
// 		break;
// 	}
// 		SendNotifyMsg(nullptr, client_, NOTIFY_REQTITLEANSWER, findclient_->TitleType, ReturnID, iCrusadeJob, findclient_->ActiveTitle);

	}
/*
	switch (wMsgType)
	{
	case NOTIFY_DONATEEVENTINFO:
		Push(cp, m_donateEventPot);
		
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_ADMININFO:
		Push(cp, (uint32_t)player->m_iAdminUserLevel);
		Push(cp, player->m_GMFlags);
		
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_MAILRECEIVED:
	case NOTIFY_GUILDBOARDPOST:
		Push(cp, pString);

		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_MONSTERCOUNT:
		Push(cp, (uint16_t)sV1);
		npc = (CNpc*)sV2;
		if(npc)
		{
			Push(cp, (uint16_t)npc->m_sX);
			Push(cp, (uint16_t)npc->m_sY);
		} else {
			Push(cp, (uint16_t)-1);
			Push(cp, (uint16_t)-1);
		}
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_GUILDCONTRIBUTERSP:
		Push(cp, (uint8_t)sV1);
		Push(cp, sV2);

		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_ITEMTOGUILDBANK:
		pItem = (CItem*)sV1;
		if (!pItem) return;

		Push(cp, sV1);
		Push(cp, sV2); // warehouseVer

		WriteItemData(cp, pItem);

		Push(cp, (uint16_t)pItem->m_sItemEffectValue2);
		Push(cp, pItem->m_dwAttribute);
		Push(cp, (uint8_t)pItem->m_sItemSpecEffectValue2);
		
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}
		iRet = player->socket->write(cData, cp - cData);
		break;

	case NOTIFY_SOCCERVICTORY: 
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_SOCCER:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;
		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_SOCCERBEGIN:					
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_HELDENIANVICTORY: //0x0BEB
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		iRet = player->socket->write(cData, 8);
		break;
	
	case NOTIFY_HELDENIANSTART:
	case NOTIFY_HELDENIANEND:					
		iRet = player->socket->write(cData, 6);
		break;
	case NOTIFY_HELDENIANCOUNT:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV3;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV4;		
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV5;		
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV6;
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV7;		
		cp += 2;
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV8;		
		cp += 2;
		iRet = player->socket->write(cData, 22);
		break;

	case NOTIFY_APOCGATECLOSE:
	case NOTIFY_APOCGATEOPEN:
		ip  = (int *)cp;
		*ip = sV1;
		cp += 4;
		ip  = (int *)cp;
		*ip = sV2;
		cp += 4;
		memcpy(cp,pString,10);
		cp += 10;
		iRet = player->socket->write(cData, 24);
		break;

	case NOTIFY_ABADDONKILLED:
		memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
		cp += 10;
		iRet = player->socket->write(cData, 16);
		break;

	case NOTIFY_APOCGATESTARTMSG:
	case NOTIFY_APOCGATEENDMSG:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_LGNPTS:
		dwp = (uint32*)cp;
		*dwp = sV1;

		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_TCLOC:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		memcpy(cp, pString, 10);
		cp += 10;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV4;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV5;
		cp += 2;

		memcpy(cp, pString2, 10);
		cp += 10;

		iRet = player->socket->write(cData, 34);
		break;



	case NOTIFY_QUESTCOUNTER:
		ip  = (int *)cp;
		*ip = sV1;
		cp += 4;

		iRet = player->socket->write(cData, 24);
		break;











	case NOTIFY_ENERGYSPHERECREATED:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short *)cp;
		*sp = (short)sV2;
		cp += 2;
		
		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 30);
		break;

	case NOTIFY_ITEMSOCKETCHANGE:
		Push(cp, (uint16_t)sV1);

		pItem = (CItem*)sV2;
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}

		iRet = player->socket->write(cData, cp - index);
		break;

	case NOTIFY_NOMOREAGRICULTURE:	   
	case NOTIFY_AGRICULTURESKILLLIMIT: 
	case NOTIFY_AGRICULTURENOAREA:     
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_NOMORECRUSADESTRUCTURE:
	case NOTIFY_EXCHANGEITEMCOMPLETE:
	case NOTIFY_CANCELEXCHANGEITEM:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_SETEXCHANGEITEM:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;
		
		pItem = (CItem*)sV2;
		
		Push(cp, (uint16_t)pItem->m_sSprite);
		Push(cp, (uint16_t)pItem->m_sSpriteFrame);
		Push(cp, sV3); // amount
		Push(cp, (uint8_t)pItem->m_cItemColor);
		Push(cp, (uint16_t)pItem->m_wCurLifeSpan);
		Push(cp, (uint16_t)pItem->m_wMaxLifeSpan);
		Push(cp, (uint16_t)(pItem->m_sItemSpecEffectValue2 + 100));
		Push(cp, pItem->m_cName, 20);

		Push(cp, m_pClientList[iFromH]->m_cCharName, 10);
		Push(cp, pItem->m_dwAttribute);
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}
		iRet = player->socket->write(cData, 57 + MAXITEMSOCKETS);
		break;

	case NOTIFY_OPENEXCHANGEWINDOW:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		pItem = (CItem*)sV2;
		
		Push(cp, (uint16_t)pItem->m_sSprite);
		Push(cp, (uint16_t)pItem->m_sSpriteFrame);
		Push(cp, sV3); // amount
		Push(cp, (uint8_t)pItem->m_cItemColor);
		Push(cp, (uint16_t)pItem->m_wCurLifeSpan);
		Push(cp, (uint16_t)pItem->m_wMaxLifeSpan);
		Push(cp, (uint16_t)(pItem->m_sItemSpecEffectValue2 + 100));
		Push(cp, pItem->m_cName, 20);

		Push(cp, m_pClientList[iFromH]->m_cCharName, 10);
		Push(cp, pItem->m_dwAttribute);
		for(i = 0; i < MAXITEMSOCKETS; i++)
		{
			Push(cp, pItem->m_sockets[i]);
		}

		iRet = player->socket->write(cData, 57 + MAXITEMSOCKETS);
		break;

	case NOTIFY_NOTFLAGSPOT:
		iRet = player->socket->write(cData, 6);
		break;



	case NOTIFY_DOWNSKILLINDEXSET:
		sp = (short *)cp;
		*sp = (short)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_ADMINIFO:
		switch (sV1) {
		case 1:

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_iHP;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_iDefenseRatio;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_bIsSummoned;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_cActionLimit;
			cp += 4;

			ip  = (int *)cp;
			*ip	= m_pNpcList[sV2]->m_iHitDice;
			cp += 4;

			dwp  = (uint32_t *)cp; 
			*dwp = m_pNpcList[sV2]->m_dwDeadTime;
			cp += 4;

			dwp  = (uint32_t *)cp; 
			*dwp = m_pNpcList[sV2]->m_dwRegenTime;
			cp += 4;

			ip  = (int *)cp; 
			*ip	= (int)m_pNpcList[sV2]->m_bIsKilled;
			cp += 4;

			iRet = player->socket->write(cData, 26 + 12);
			break;
		}
		break;


	case NOTIFY_EVENTILLUSION:
	case NOTIFY_EVENTTP:
		*cp = (char)sV1;
		cp++;
		*cp = (char)sV2;
		cp++;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_EVENTSPELL:
		*cp = sV1;
		cp++;
		*cp = sV2;
		cp++;
		*cp = player->IsGM();
		cp++;
		iRet = player->socket->write(cData, 9);
		break;

	case NOTIFY_EVENTARMOR:
	case NOTIFY_EVENTSHIELD:
	case NOTIFY_EVENTCHAT:
	case NOTIFY_EVENTPARTY:
		*cp = sV1;
		cp++;
		*cp = player->IsGM();
		cp++;
		iRet = player->socket->write(cData, 8);
		break;



	case NOTIFY_QUERY_JOINPARTY:
	case NOTIFY_IPACCOUNTINFO:
		strcpy(cp, pString);
		cp += strlen(pString);

		iRet = player->socket->write(cData, 6 + strlen(pString) + 1);
		break;

	case NOTIFY_REWARDGOLD:
		dwp = (uint32_t *)cp;
		*dwp = player->m_iRewardGold;
		cp += 4;

		iRet = player->socket->write(cData, 10);
		break;



	case NOTIFY_MONSTEREVENT_POSITION:
		*cp = (char)sV3;
		cp++;

		sp = (short*)cp;
		*sp = (short)sV1;
		cp += 2;

		sp = (short*)cp;
		*sp = (short)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 11);
		break;


	case NOTIFY_FISHCANCELED:
	case NOTIFY_FISHSUCCESS:
	case NOTIFY_FISHFAIL:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_DEBUGMSG:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_FISHCHANCE:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_ENERGYSPHEREGOALIN:
	case NOTIFY_EVENTFISHMODE:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2;
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV3;
		cp += 2;

		memcpy(cp, pString, 20);
		cp += 20;

		iRet = player->socket->write(cData, 32);
		break;



	case NOTIFY_TIMECHANGE:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_TOBERECALLED:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_HUNGER:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;
#ifdef MonsterBar
	case NOTIFY_NPCBAR:
		*cp = (char)sV1;
		cp++;

		iRet = m_pClientList[iToH]->socket->write(cData, 7);
		break;

	case NOTIFY_NPCBAR2:
		*cp = (char)sV1;
		cp++;

		iRet = m_pClientList[iToH]->socket->write(cData, 7);
		break;
#endif


	case NOTIFY_PLAYERONGAME:
	case NOTIFY_FRIENDONGAME:
		memcpy(cp, pString, 10);
		cp += 10;
		if (pString != NULL) {
			memcpy(cp, pString2, 14);
			cp += 14;
		}
		iRet = player->socket->write(cData, 31);
		break;



	case NOTIFY_ITEMSOLD:
	case NOTIFY_ITEMREPAIRED:
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV1;
		cp += 4;
		dwp = (uint32_t *)cp;
		*dwp = (uint32_t)sV2;
		cp += 4;

		iRet = player->socket->write(cData, 14);
		break;




		//(word)sV1, sV2
	case NOTIFY_SHOWMAP:
		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV1;
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV2;
		cp += 2;

		iRet = player->socket->write(cData, 10);
		break;

		//(word)sV1


		// (char)sV1
	case NOTIFY_EVENTSTART:
	case NOTIFY_EVENTSTARTING:
	case NOTIFY_EVENTSTARTING2:
	case NOTIFY_EVENTSTARTING3:
	case NOTIFY_RELICINALTAR:
		*cp = (char)sV1;
		cp++;

		iRet = player->socket->write(cData, 7);
		break;

	case NOTIFY_EVENTWINNER:
		// event
		*cp = (char)sV1;
		cp++;

		// winner
		*cp = (char)sV2;
		cp++;

		iRet = player->socket->write(cData, 8);
		break;

	case NOTIFY_CASUALTIES:
		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV1; // Aresden deaths
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV2; // Elvine
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV4; // Aresden kills
		cp += 2;

		wp  = (uint16_t *)cp;  
		*wp = (uint16_t)sV5; // Elvine
		cp += 2;

		iRet = player->socket->write(cData, 18);
		break;

	case NOTIFY_SHIELDHP: // Shield xRisenx
		Push(cp, (uint8_t)sV1);
		//Push(cp, (uint16_t)sV2);
		Push(cp, (uint32_t)sV2);
		
		iRet = player->socket->write(cData, 11); // 9->11
		break;

	case NOTIFY_RELICPOSITION:
		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV1; //Relic X
		cp += 2;

		wp  = (uint16_t *)cp;
		*wp = (uint16_t)sV2; //Relic Y
		cp += 2;
		iRet = player->socket->write(cData, 10);
		break;

	case NOTIFY_RELICGRABBED:
		memcpy(cp, pString, 10);
		cp += 10;
		iRet = player->socket->write(cData, 17);
		break;



	case NOTIFY_CANNOTITEMTOBANK:
	case NOTIFY_SETTING_FAILED:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_SERVERCHANGE:
		memcpy(cp, player->m_cMapName, 10);
		cp += 10;

		if (m_iGameServerMode == 1) // LAN
			memcpy(cp, m_cGameServerAddrExternal, 15);
		else // INTERNET
			memcpy(cp, m_cLogServerAddr, 15);
		cp += 15;

		ip = (int *)cp;
		*ip = m_iLogServerPort;
		cp += 4;

		iRet = player->socket->write(cData, 16 +19);
		break;

	case NOTIFY_QUERY_DISMISSGUILDREQPERMISSION:
	case NOTIFY_QUERY_JOINGUILDREQPERMISSION:
	case NOTIFY_CANNOTJOINMOREGUILDSMAN:

		memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
		cp += 10;

		iRet = player->socket->write(cData, 16);
		break;

	case COMMONTYPE_JOINGUILDAPPROVE:
		if (m_pClientList[iFromH] != NULL)
			memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
		else memcpy(cp, "?", 1);
		cp += 20;

		sp  = (short *)cp;
		*sp = GUILDSTARTRANK;
		cp += 2;

		iRet = player->socket->write(cData, 28);
		break;

	case COMMONTYPE_JOINGUILDREJECT:
	case COMMONTYPE_DISMISSGUILDAPPROVE:
	case COMMONTYPE_DISMISSGUILDREJECT:
		if (m_pClientList[iFromH] != NULL)
			memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
		else memcpy(cp, "?", 1);
		cp += 20;

		sp  = (short *)cp;
		*sp = GUILDSTARTRANK;
		cp += 2;

		iRet = player->socket->write(cData, 28);
		break;

	case NOTIFY_GUILDDISBANDED:

		memcpy(cp, pString, 20);
		cp += 20;

		memcpy(cp, player->m_cLocation, 10);
		cp += 10;

		iRet = player->socket->write(cData, 36);
		break;

	case NOTIFY_FIGHTZONERESERVE:
		ip = (int *)cp;
		*ip = (int )sV1;
		cp += 4;

		iRet = player->socket->write(cData, 10);
		break; 

	case NOTIFY_NOGUILDMASTERLEVEL:
		iRet = player->socket->write(cData, 6);
		break;

	case NOTIFY_CANNOTBANGUILDMAN:
		iRet = player->socket->write(cData, 6);
		break;



	case NOTIFY_REPAIRALLPRICE:
		Push(cp, (uint32_t)sV1);
		iRet = player->socket->write(cData, 10);
		break;

#ifdef TitleHG
	case NOTIFY_CHANGETITLE:
		memcpy(cp, pString, 11);
		cp += 11;
		iRet = player->socket->write(cData, 17);
		break;

	case NOTIFY_REQTITLEANSWER: 
		{
			int iNumPoints = (uint32_t)sV1;
			int iReturnID = (uint32_t)sV2;
			int iCrusadeJob = (uint32_t)sV3;
			int iRank = (uint32_t)sV4;

			ip = (int *)cp;
			*ip = iReturnID;
			cp += 4;

			ip = (int *)cp;
			*ip = iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = iCrusadeJob;
			cp += 4;

			/ *ip = (int *)cp;
			*ip = player->iReturnID;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iCrusadeJob;
			cp += 4;* /

			memset(cp, 0, 21);
			memcpy(cp, pString, strlen(pString)+1);
			cp += 21;

			iRet = player->socket->write(cData, 43);
			break;
		}

	case NOTIFY_ADDTITLE: 
		{
			int iNumPoints = (uint32_t)sV1;
			int iNextRankPoints = (uint32_t)sV2;
			int iRank = (uint32_t)sV3;
			int iTitleIndex = (uint32_t)sV4;
			char cActiveTitle = (char)sV5;

			memset(cp, 0, 21);
			memcpy(cp, pString, strlen(pString)+1);
			cp += 21;

			ip = (int *)cp;
			*ip = iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = iTitleIndex;
			cp += 4;

			*cp = cActiveTitle;
			cp++;

			/ *ip = (int *)cp;
			*ip = player->iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iTitleIndex;
			cp += 4;

			*cp = player->cActiveTitle;
			cp++;* /

			iRet = player->socket->write(cData, 44);
			break;
		}

	case NOTIFY_TITLELEVELUP:
		{
			int iNumPoints = (uint32_t)sV1;
			int iNextRankPoints = (uint32_t)sV2;
			int iRank = (uint32_t)sV3;
			int iTitleIndex = (uint32_t)sV4;
			
			memset(cp, 0, 21);
			memcpy(cp, pString, strlen(pString)+1);
			cp += 21;

			ip = (int *)cp;
			*ip = iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = iTitleIndex;
			cp += 4;

			/ *ip = (int *)cp;
			*ip = player->iNumPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iNextRankPoints;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iRank;
			cp += 4;

			ip = (int *)cp;
			*ip = player->iTitleIndex;
			cp += 4;* /

			iRet = player->socket->write(cData, 43);
			break;
		}
#endif
		// Gladiator Arena xRisenx
		/ *case NOTIFY_ARENASTATUS:
			ip  = (int *)cp;
			*ip = sV1;
		    cp += 4;

		    ip  = (int *)cp;
		    *ip = sV2;
		    cp += 4;

		    ip  = (int *)cp;
		    *ip = sV3;
		    cp += 4;

		    memcpy(cp,pString,21);
		    cp += 21;

		    iRet = m_pClientList[iToH]->socket->write(cData,39);
			iRet = player->socket->write(cData,39);
		    break;* /
		// Gladiator Arena xRisenx
		// Monster kill event xRisenx
		case NOTIFY_NPCHUNTON:
		case NOTIFY_NPCHUNTOFF:
		iRet = player->socket->write(cData, 6);
		break;
		case NOTIFY_NPCHUNTWINNER:
		ip = (int *)cp;
		*ip = player->m_iContribution;
		cp += 4;
		ip = (int *)cp;
		*ip = player->m_reputation;
		cp += 4;
		iRet = player->socket->write(cData, 14);
		break;
		// Monster kill event xRisenx

	}
	}*/
}

uint8_t gserver::iSetSide(client * client_)
{
	if (client_ == 0) return 0;
	
	client_->side = faction::neutral; 
	client_->m_bIsOnTown = false;
	client_->civilian = false;
	client_->neutral = false;

	if (client_->nation.length() == 0 || client_->nation[0] == '\0')
	{
		//fail safe set to trav?
		client_->nation = "NONE";
	}
	if (client_->nation[0] == '1')
		client_->nation = "aresden";

	if (client_->nation.length() <= 3)
	{
		//something borked. check if "are" or "elv" matches
		if (client_->nation[0] == 'a')
		{
			client_->nation = "aresden";
		}
		else if (client_->nation[0] == 'e')
		{
			client_->nation = "elvine";
		}
	}

	if (client_->nation.substr(3, 6) == "hunter")
	{
		client_->civilian = true;
	}

	if (client_->map_name == "bisle")
	{
		client_->civilian = false;
	}

	if (client_->nation.substr(0, 3) == "are")
	{
		client_->side = faction::aresden;
		if (client_->map_name == sideMap[faction::aresden])
		{
			client_->m_bIsOnTown = true;
		}
	}
	else if (client_->nation.substr(0, 3) == "are")
	{
		client_->side = faction::elvine;
		if (client_->map_name == sideMap[faction::elvine])
		{
			client_->m_bIsOnTown = true;
		}
	}
	else
	{
		client_->side = faction::neutral;
		client_->m_bIsOnTown = true;
		if (client_->map_name == sideMap[faction::neutral])
		{
			client_->m_bIsOnTown = true;
		}
	}

	client_->set_side_flag((faction) client_->side);

	if (client_->map_name == sideMap[faction::neutral])
	{
		client_->neutral = true;
	}

	if ((client_->map_name.substr(0, 6) == "bsmith") ||
		(client_->map_name.substr(0, 7) == "gldhall") ||
		(client_->map_name.substr(0, 5) == "gshop") ||
		(client_->map_name.substr(0, 5) == "cityhall") )
		client_->m_bIsOnShop = true;
	else
		client_->m_bIsOnShop = false;


	if ((client_->map_name.substr(0, 5) == "wrhus") ||
		(client_->map_name.substr(3, 5) == "wrhus") ||
		(client_->map_name.substr(0, 5) == "cityhall"))
		client_->m_bIsOnWarehouse = true;
	else
		client_->m_bIsOnWarehouse = false;


	if((client_->map_name.substr(0, 6) == "wzdtwr") ||
		(client_->map_name.substr(0, 6) == "cityhall") )
		client_->m_bIsOnTower = true;
	else
		client_->m_bIsOnTower = false;

	return client_->side;
}

item * gserver::_iGetArrowItemIndex(client * client_)
{
	if (client_ == nullptr) return nullptr;

// 	for (std::vector<ItemWrap*>::iterator iter = client_->itemList.begin(); iter != client_->itemList.end(); ++iter)
// 		if ((*iter)->_item && ((*iter)->_item_->m_cItemType == ITEMTYPE_ARROW) && ((*iter)->_item_->count > 0))
// 			return (*iter)->_item;

	return nullptr;
}

void gserver::InitPlayerData(std::shared_ptr<client> client_)
{
    log->info("InitPlayerData()");
	if (!client_ || !client_->map_)
	{
        log->error("InitPlayerData() client or map nullptr");
		return;
    }
	try
	{
		{
			//initial stuff. no point sending char data if they aren't allowed to login, or the data needs adjusting
			if ((client_->x == -1) || (client_->y == -1))
			{
				if (!client_->map_->GetInitialPoint(client_->x, client_->y, client_->nation))
				{
					DeleteClient(client_, false, true);
					return;
				}
			}

			//if m_bIsInitComplete is already set to true, it means the object is being resumed so use their old spot instead of finding a new one
			//calling SetOwner further below should have no negative effect since it's effectively just replacing the var with the same value
			if (!client_->m_bIsInitComplete)
				client_->map_->bGetEmptyPosition(client_->x, client_->y, client_);

			if(client_->playerKillCount > 0)
				client_->set_status_flag(STATUS_PK, true);

            iSetSide(client_.get());

			client_->m_iNextLevelExp = m_iLevelExpTable[client_->level + 1];

// 			CalcTotalItemEffect(iClientH, -1, true); //false
 			iCalcTotalWeight(client_);
// 			_SendQuestContents(iClientH);
// 
// 			player->ValidateSkills(true);
// 			CheckMagicInt(iClientH);
// 			

			//SendNotifyMsg(NULL, iClientH, NOTIFY_REQDEF, m_pClientList[iClientH]->m_iDefenseRatio, NULL, NULL, NULL);
// 			if ((cGuildStatus == 0) && (memcmp(player->m_cGuildName, "NONE", 4) != 0)) {
// 				ZeroMemory(player->m_cGuildName, sizeof(player->m_cGuildName));
// 				strcpy(player->m_cGuildName, "NONE");
// 				player->m_iGuildRank = GUILDRANK_NONE;
// 				player->m_iGuildGUID = -1;
// 				player->m_guild = NULL;
// 
// 				//SendNotifyMsg(NULL, iClientH, NOTIFY_GUILDDISBANDED, NULL, NULL, NULL, player->m_cGuildName);
// 			}

			//client_->Equipped.Arrow = _iGetArrowItemIndex(client.get());
			//CalcTotalItemEffect(iClientH, -1, false);

		}
		{
			stream_write sw;
			sw.write_int32(MSGID_RESPONSE_INITPLAYER);
			sw.write_int16(MSGTYPE_CONFIRM);

			client_->write(sw);
		}
		{
			stream_write sw;
			sw.write_int32(MSGID_PLAYERCHARACTERCONTENTS);
			sw.write_int16(MSGTYPE_CONFIRM);

			sw.write_int32(uint32_t(client_->health));
			sw.write_int32(uint32_t(client_->mana));
			sw.write_int32(uint32_t(client_->stamina));

			sw.write_int32(client_->m_iDefenseRatio);
			sw.write_int32(client_->m_iHitRatio);
			sw.write_int32(client_->level);

			sw.write_int32(client_->GetStr());
			sw.write_int32(client_->GetInt());
			sw.write_int32(client_->GetVit());
			sw.write_int32(client_->GetDex());
			sw.write_int32(client_->GetMag());
			sw.write_int32(client_->GetChr());

			sw.write_int16(client_->levelUpPool);
			sw.write_int64(client_->experience);
			sw.write_int32(client_->enemyKillCount);
			sw.write_int32(client_->playerKillCount);
			sw.write_int32(client_->m_iRewardGold);

			sw.write_string(client_->nation, 10);
			if (client_->guild_)
				sw.write_string(client_->guild_->name, 20);
			else
				sw.write_string("NONE", 20);
			sw.write_int32(-1);//TODO: remove
			sw.write_int8(client_->superAttack);
			sw.write_int32(client_->arenaNumber);
			sw.write_int32(client_->m_iEnemyKillTotalCount);

			client_->write(sw);
		}
		{
			stream_write sw = stream_write();

			sw.write_int32(MSGID_PLAYERITEMLISTCONTENTS);
			sw.write_int16(0);

            auto writeitem = [](stream_write & sw, std::shared_ptr<item> item_)
            {
                if (!item_->initialized)
                {
                    sw.write_int8(0);
                    return;
                }
                sw.write_int8(1);
                sw.write_string(item_->name);
                sw.write_int16(item_->m_dwCount);
                sw.write_int8(item_->m_cItemType);
                sw.write_int32(item_->m_cItemColor);
                sw.write_int16(item_->m_wMaxLifeSpan);
                sw.write_int32(item_->m_wCurLifeSpan);
                sw.write_int16(item_->m_sItemSpecEffectValue1);
                sw.write_int16(item_->m_sItemSpecEffectValue2);
                sw.write_int16(item_->m_sItemSpecEffectValue3);
                sw.write_int8(item_->m_cGenderLimit);
                sw.write_int16(item_->m_sItemEffectValue1);
                sw.write_int16(item_->m_sItemEffectValue2);
                sw.write_int16(item_->m_sItemEffectValue3);
                sw.write_int16(item_->m_sItemEffectValue4);
                sw.write_int16(item_->m_sItemEffectValue5);
                sw.write_int16(item_->m_sItemEffectValue6);
                sw.write_int16(item_->m_sLevelLimit);
                sw.write_int32(item_->m_dwAttribute);
                sw.write_int16(item_->GetWeight());
                sw.write_int16(item_->m_sSprite);
                sw.write_int16(item_->m_sSpriteFrame);
                sw.write_int64(item_->uid);
            };

            sw.write_int8(client_->itemBagList.size());
            for (const std::shared_ptr<itembag> & ib : client_->itemBagList)
            {
                sw.write_int8(ib->itemList.size()); 
                for (const std::shared_ptr<item> & item_ : ib->itemList)
                {
                    writeitem(sw, item_);
                }
            }

            sw.write_int8(client_->itemBagBankList.size());
            for (const std::shared_ptr<itembag> & ib : client_->itemBagBankList)
            {
                sw.write_int8(ib->itemList.size());
                for (const std::shared_ptr<item> & item_ : ib->itemList)
                {
                    writeitem(sw, item_);
                }
            }

            //TODO: send this during connect? or on bank access? profile this on alpha to see how often each are used
//             sw.write_int8(client_->itemBagGuildBankList.size());
//             for (ItemBag & ib : client_->itemBagGuildBankList)
//             {
//                 sw.write_int8(ib.itemList.size());
//                 for (Item & item : ib.itemList)
//                 {
//                     writeitem(sw, item);
//                 }
//             }

			//magic skill mastery. why it's in items...

			for (uint8_t i = 0; i < MAXMAGICTYPE; ++i)
			{
				sw.write_int8(client_->magicMastery[i]);
			}
			for (uint8_t i = 0; i < MAXSKILLTYPE; ++i)
			{
				sw.write_int8(client_->skillMastery[i]);
			}

			client_->write(sw);
		}
		{
			stream_write sw = stream_write();

			sw.write_int32(MSGID_RESPONSE_INITDATA);
			sw.write_int16(MSGTYPE_CONFIRM);

            sw.write_int32(uint32_t(client_->charid));

            uint16_t maxx = moveLocationX[client_->resolution];
            uint16_t maxy = moveLocationY[client_->resolution];

            sw.write_int16(maxx);
            sw.write_int16(maxy);
            sw.write_int16(client_->x);
            sw.write_int16(client_->y);
            sw.write_int16(client_->get_type());
			sw.write_int16(client_->m_sAppr1);
			sw.write_int16(client_->m_sAppr2);
			sw.write_int16(client_->m_sAppr3);
			sw.write_int16(client_->m_sAppr4);
			sw.write_int32(client_->m_iApprColor);
			sw.write_int16(client_->m_sHeadApprValue);
			sw.write_int16(client_->m_sBodyApprValue);
			sw.write_int16(client_->m_sArmApprValue);
			sw.write_int16(client_->m_sLegApprValue);

			if (client_->playerKillCount > 0)
				client_->set_status_flag(STATUS_PK, true);

			sw.write_int32(client_->status);
			sw.write_string(client_->map_->name, 10);
			sw.write_string(client_->map_->location_name, 10);//map location side name


			if (client_->map_->fixedDay) //day(1) or night(0)?
				sw.write_int8(1);
			else
				sw.write_int8(m_cDayOrNight);

			if (client_->map_->fixedDay) //sunny(0) or raining(1-3) or snowing(4-6)?
				sw.write_int8(WEATHER_SUNNY);
			else 
				sw.write_int8(client_->map_->weather);

			sw.write_int32(client_->contribution);

			if(!client_->observerMode && !client_->IsEthereal())
			{
				client_->map_->SetOwner(client_, client_->x, client_->y);
			}

			sw.write_int8((client_->observerMode)?1:0);
			sw.write_int32(client_->m_reputation);
			sw.write_int64(uint32_t(client_->health));
			sw.write_int32(client_->m_iLucky);

			if (client_->m_bIsOnShop && m_iCrusadeWinnerSide == client_->side)//crusade shop price change
				sw.write_int8(-10);
			else
				sw.write_int8(0);

			uint64_t oldpos = sw.position;
			uint16_t total = 0;

			sw.write_int16(0);//total placeholder

            tile * pTileSrc = client_->map_->GetTile(client_->x - maxx/2, client_->y - maxy/2);
            
            for (int y = 0; y < maxy; ++y)
			{
				for (int x = 0; x < maxx; ++x)
				{
					if (WriteTileData(sw, client_, x, y))
						total++;
				}
			}
			char * temp = sw.data + oldpos;
			*((short *)temp) = total;
			client_->m_bIsInitComplete = true;

			client_->write(sw);

			SendEventToNearClient_TypeA(client_, MSGID_MOTION_EVENT_CONFIRM, 0, 0, 0);
		}
	}
	catch(int32_t reason)
	{
		if (reason == -192)
		{
			log->error("(data == 0)");
		}
		else if (reason == -193)
		{
			log->error("gserver: (position+a > size)");
		}
	}
	catch (std::exception& e)
	{
		log->error("InitPlayerData() Exception: {}", e.what());
	}
}

void gserver::ClientMotionHandler(std::shared_ptr<client> client_, stream_read & sr)
{
	if (!client_->m_bIsInitComplete) return;
	if (client_->dead_) return;
	if (client_->observerMode) return;

	sr.position = 0;

	uint32_t msgid = sr.read_int32();

	uint16_t dX, sX = sr.read_int16();
	uint16_t dY, sY = sr.read_int16();

	uint8_t direction = sr.read_int8();

	int64_t clienttime = sr.read_int64();

    int16_t iv1 = sr.read_int16();

	int16_t magictype;
	int16_t attacktype;
	int8_t iRet;
    uint64_t targetid;
	if (msgid == MSGID_MOTION_MAGIC)
		magictype = sr.read_int16();
	else if (msgid == MSGID_MOTION_ATTACK || msgid == MSGID_MOTION_ATTACKMOVE)
	{
        dX = iv1;
		dY = sr.read_int16();
		attacktype = sr.read_int16();
        targetid = sr.read_int16();//might need to expand to at least 32bit client side
	}

	switch (msgid)
	{
        case MSGID_MOTION_STOP:
            iRet = iClientMotion_Stop_Handler(client_, sX, sY, direction);
            if (iRet == 1)
			{
                SendEventToNearClient_TypeA(client_, msgid, 0, 0, 0);
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(client_);
            break;

        case MSGID_MOTION_RUN:
        case MSGID_MOTION_MOVE:
            if (!bCheckClientMoveFrequency(client_, (msgid == MSGID_MOTION_RUN)))
                iRet = 3;
            else
                iRet = iClientMotion_Move_Handler(client_, sX, sY, direction, (msgid == MSGID_MOTION_RUN));

            if (iRet == 1)
			{
                SendEventToNearClient_TypeA(client_, msgid, 0, 0, 0);
            }
            else if (iRet == 2 || iRet == 3)
            {
                SendObjectMotionRejectMsg(client_);
            }

            if ((client_->health <= 0)) client_->KilledHandler(nullptr, 1);
            break;

        case MSGID_MOTION_DAMAGEMOVE:
            iRet = iClientMotion_Move_Handler(client_, sX, sY, direction, false);
            if (iRet == 1)
			{
                SendEventToNearClient_TypeA(client_, msgid, client_->m_iLastDamage, 0, 0);
            }
            if ((client_ != nullptr) && (client_->health <= 0)) client_->KilledHandler(nullptr, 1);
            break;

        case MSGID_MOTION_ATTACKMOVE:
            iRet = iClientMotion_Move_Handler(client_, sX, sY, direction, false);
            if ((iRet == 1) && (client_ != nullptr))
			{
                SendEventToNearClient_TypeA(client_, msgid, 0, 0, 0);

                attacktype = 1;
                iClientMotion_Attack_Handler(client_, client_->x, client_->y, dX, dY, attacktype, direction, targetid, false, true);
            }
            if ((client_ != nullptr) && (client_->health <= 0)) client_->KilledHandler(nullptr, 1);

            bCheckClientAttackFrequency(client_);
            break;

        case MSGID_MOTION_ATTACK:
            _CheckAttackType(client_.get(), attacktype);

            if (!bCheckClientAttackFrequency(client_))
                iRet = 2;
            else
                iRet = iClientMotion_Attack_Handler(client_, sX, sY, dX, dY, attacktype, direction, targetid);
            if (iRet == 1)
			{
                if (attacktype >= 20)
				{
                    client_->superAttack--;

                    if (client_->superAttack < 0) client_->superAttack = 0;
                }

                SendEventToNearClient_TypeA(client_, msgid, dX, dY, attacktype);
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(client_);

            break;

        case MSGID_MOTION_GETITEM:
            iRet = iClientMotion_GetItem_Handler(client_, sX, sY, direction);
            if (iRet == 1)
			{
                SendEventToNearClient_TypeA(client_, msgid, 0, 0, 0);
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(client_);
            break;

        case MSGID_MOTION_MAGIC:
            iRet = iClientMotion_Magic_Handler(client_, sX, sY, direction);

            if (iRet == 1)
			{
                SendEventToNearClient_TypeA(client_, msgid, magictype, (short)10, 0);
                client_->m_hasPrecasted = true;
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(client_);
            break;
	}
}

void gserver::SendObjectMotionRejectMsg(std::shared_ptr<client> client_)
{
	if(!client_) return;
	if(client_->rejectedMove) return;

	client_->rejectedMove = true;
	client_->m_resetMoveFreq = true; 

	stream_write sw(12);

	sw.write_int32(MSGID_RESPONSE_MOTION);
	sw.write_int16(OBJECTMOTION_REJECT);
	sw.write_int16(client_->x);
	sw.write_int16(client_->y);

	client_->write(sw);
}

int gserver::iClientMotion_Stop_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir)
{
	std::shared_ptr<unit> owner;

	if (!client_) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (client_->dead_ == true) return 0;
	if (client_->m_bIsInitComplete == false) return 0;


	if ((sX != client_->x) || (sY != client_->y)) return 2;


	if (client_->skillInUse[19] == true)
	{
		owner = client_->map_->GetOwner(sX, sY);
		if (owner != nullptr) {
			//TODO: pretend corpse and someone is on top of you - make it so you appear on a tile nearby instead
			DeleteClient(client_, true, false);
			return 0;
		}
		client_->map_->ClearOwner(sX, sY);
		client_->map_->ClearDeadOwner(sX, sY);
		client_->map_->SetOwner(client_, sX, sY);
	}

	ClearSkillUsingStatus(client_);

	if (client_->map_ != nullptr) {
		client_->map_->IncPlayerActivity(client_.get());
	}

	client_->direction = cDir;

	stream_write sw(10);

	sw.write_int32(MSGID_RESPONSE_MOTION);
	sw.write_int32(OBJECTMOTION_CONFIRM);

	client_->write(sw);

	return 1;
}

// Calculates the number of players within a given radius.
int gserver::getPlayerNum(map * pMap, short dX, short dY, char cRadius)
{
	int x, y, ret;
	tile * pTile;

	//if ((cMapIndex < 0) || (cMapIndex > MAXMAPS)) return 0;
	//if (maplist[cMapIndex] == NULL) return 0;

	ret = 0;
	for (x = dX - cRadius; x <= dX + cRadius; x++)
    {
        for (y = dY - cRadius; y <= dY + cRadius; y++)
        {
            if ((x < 0) || (x >= pMap->sizeX) ||
                (y < 0) || (y >= pMap->sizeY))
            {

            }
            else
            {
                pTile = pMap->GetTile(x, y);
                //pTile = (class tile *)(pMap->_tile + x + y*pMap->sizeY);
                if ((pTile->owner != nullptr) && (pTile->m_cOwnerType == ot_player))
                    ret++;
            }
        }
    }

	return ret;
}

void gserver::SendEventToNearClient_TypeA(std::shared_ptr<unit> owner, uint32_t msgid, uint32_t sV1, uint32_t sV2, uint32_t sV3)
{
	uint32_t * pstatus, status = 0, statusdummy = 0;
	bool    bOwnerSend;

	pstatus = &statusdummy;

	stream_write sw;

	sw.write_int32(msgid);

	if (owner->is_player())
	{
		std::shared_ptr<client> player = std::static_pointer_cast<client>(owner);

		switch (msgid) {
		case MSGID_MOTION_NULL:
		case MSGID_MOTION_DAMAGE:
		case MSGID_MOTION_DYING:
			bOwnerSend = true;
			break;
		default:
			bOwnerSend = false;
			break;
		}

		switch (msgid) 
		{
		case MSGID_MOTION_MAGIC:
		case MSGID_MOTION_DAMAGE:
		case MSGID_MOTION_DAMAGEMOVE:
		case MSGID_MOTION_DYING:
            sw.write_int16(uint16_t(owner->handle + 30000));
            sw.write_int8(owner->get_owner_type());
			sw.write_int8(owner->direction);
			sw.write_int16(sV1);
			sw.write_int8(uint8_t(sV2));

			if(msgid == MSGID_MOTION_DYING)
			{
				sw.write_int16(player->x);
				sw.write_int16(player->y);
			}
			break;

		case MSGID_MOTION_ATTACK:
		case MSGID_MOTION_ATTACKMOVE:
            sw.write_int16(uint16_t(owner->handle + 30000));
            sw.write_int8(owner->get_owner_type());
            sw.write_int8(owner->direction);
			sw.write_int16(player->x);
			sw.write_int16(player->y);
			sw.write_int8(uint8_t(sV3));
			break;

		case MSGID_MOTION_EVENT_CONFIRM:
		case MSGID_MOTION_EVENT_REJECT:
		case MSGID_MOTION_NULL:
		default:
            sw.write_int16(uint16_t(owner->handle + 30000));
            sw.write_int8(owner->get_owner_type());
            sw.write_int16(player->x);
			sw.write_int16(player->y);
			sw.write_int16(player->get_type());
			sw.write_int8(owner->direction);
			sw.write_string(player->name, 10);

			sw.write_int16(player->m_sAppr1);
			sw.write_int16(player->m_sAppr2);
			sw.write_int16(player->m_sAppr3);
			sw.write_int16(player->m_sAppr4);
			sw.write_int32(player->m_iApprColor);

			sw.write_int16(player->m_sHeadApprValue);
			sw.write_int16(player->m_sBodyApprValue);
			sw.write_int16(player->m_sArmApprValue);
			sw.write_int16(player->m_sLegApprValue);

			pstatus = (uint32_t*)(sw.data+sw.position);

			sw.write_int32(player->status);

			sw.write_int8((uint8_t)((msgid != MSGID_MOTION_NULL || !player->dead_) ? 0 : 1));
			break;
		}

		if(bOwnerSend)
		{
			player->write(sw);
		}

		if(player->IsEthereal())
		{
			return;
		}
		{
			std::unique_lock<std::mutex> l(cl_m);
			for (const std::shared_ptr<client> & client_ : clients)
			{
				if ((client_->handle != player->handle) && (client_->m_bIsInitComplete) && (player->map_ == client_->map_)
					&& ((client_->x > player->x - 15) && (client_->x < player->x + 15)//screen res location
					&& (client_->y > player->y - 13) && (client_->y < player->y + 13)))
				{
                    if (_bGetIsPlayerHostile(player, client_) && client_->m_iAdminUserLevel == 0)
						*pstatus = player->status & STATUS_ENEMYFLAGS;
					// 				else
					// 					*pstatus = status;
					client_->write(sw);
				}
			}
		}
	}
	else
 {
		std::shared_ptr<npc> npc_ = std::static_pointer_cast<npc>(owner);

		switch (msgid)
        {
        case MSGID_MOTION_MAGIC:
        case MSGID_MOTION_DAMAGE:
        case MSGID_MOTION_DAMAGEMOVE:
        case MSGID_MOTION_DYING:
            sw.write_int16(uint16_t(owner->handle + 30000));
            sw.write_int8(owner->get_owner_type());
            sw.write_int8(uint8_t(npc_->direction));
			sw.write_int16(uint16_t(sV1));
			sw.write_int8(uint8_t(sV2));

            if(msgid == MSGID_MOTION_DYING)
            {
				sw.write_int16(uint16_t(npc_->x));
				sw.write_int16(uint16_t(npc_->y));
            }
            break;

        case MSGID_MOTION_ATTACK:
        case MSGID_MOTION_ATTACKMOVE:
            sw.write_int16(uint16_t(owner->handle + 30000));
            sw.write_int8(owner->get_owner_type());
            sw.write_int8(uint8_t(npc_->direction));
			sw.write_int16(uint16_t(npc_->x));
			sw.write_int16(uint16_t(npc_->y));
			sw.write_int16(uint16_t(sV3));
            break;

		case MSGTYPE_CONFIRM:
		case MSGTYPE_REJECT:
		case OBJECTNULLACTION:
		default:
            sw.write_int16(uint16_t(owner->handle + 30000));
            sw.write_int8(owner->get_owner_type());
            sw.write_int16(uint16_t(npc_->x));
			sw.write_int16(uint16_t(npc_->y));
			sw.write_int16(uint16_t(npc_->get_type()));
			sw.write_int8(uint8_t(npc_->direction));
			sw.write_int16(uint16_t(npc_->appr2));

			pstatus = (uint32_t*)(sw.data+sw.position);
			sw.write_int32(uint32_t(npc_->status));

			sw.write_int8((uint8_t)((msgid != MSGID_MOTION_NULL || !npc_->dead_) ? 0 : 1));
			break;
		}


		{
            std::unique_lock<std::mutex> l(cl_m);
			for (const std::shared_ptr<client> & client_ : clients)
			{
				if (/*(client_->m_handle != npc->m_handle) && */(client_->m_bIsInitComplete) && (npc_->map_ == client_->map_)
					&& ((client_->x > npc_->x - 15) && (client_->x < npc_->x + 15)//screen res location
					&& (client_->y > npc_->y - 12) && (client_->y < npc_->y + 12)))
				{
					// 				if(_bGetIsPlayerHostile(player,client.get()) && client_->m_iAdminUserLevel == 0)//can add flag modifiers here
					// 					*pstatus = status & STATUS_ENEMYFLAGS;
					// 				else
					// 					*pstatus = status;
					client_->write(sw);
				}
			}
		}
	}
}

void gserver::SendEventToNearClient_TypeB(uint32_t msgid, uint16_t msgtype, map * mapIndex, uint16_t sX, uint16_t sY, uint32_t sV1, uint32_t sV2, uint32_t sV3, uint32_t sV4)
{
	int i = 0, iRet = 0, iShortCutIndex;
	char * cp = 0, cData[100] = {};
	uint32_t * dwp = 0, dwTime;
	uint16_t * wp = 0;
	short * sp = 0;
	bool bFlag;
	char  cKey ;
	stream_write sw;

	cKey = (char)(rand() % 255) +1; 

	sw.write_int32(msgid);
	sw.write_int16(msgtype);

	sw.write_int16(sX);
	sw.write_int16(sY);

	sw.write_int16(sV1);
	sw.write_int16(sV2);
	sw.write_int16(sV3);
	sw.write_int16(sV4);

	dwTime = utility::current_time();

	bFlag = true;
	iShortCutIndex = 0;



	std::unique_lock<std::mutex> l(cl_m);
	for (std::shared_ptr<client> client_ : clients)
	{
		// fix this map_ * == int
		if ((client_->m_bIsInitComplete) && (client_->map_ == mapIndex)
			&& ((client_->x > sX - 15) && (client_->x < sX + 15)//screen res location
			&& (client_->y > sY - 13) && (client_->y < sY + 13)))
		{
			client_->write(sw);
		}
	}
}

int gserver::iClientMotion_GetItem_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir)
{
	uint32_t * dwp = 0;
	uint16_t * wp = 0;
	short sRemainItemSprite, sRemainItemSpriteFrame;
	char  cData[100] = {};
	uint32_t RemainItemColor;
	int   iRet = 0, iEraseReq;
	item * pItem;
	stream_write sw;

	if ((cDir <= 0) || (cDir > 8))       return 0;
	if (client_->dead_ == true) return 0;
	if (client_->m_bIsInitComplete == false) return 0;


	if ((sX != client_->x) || (sY != client_->y)) return 2;

	if (client_->map_ != nullptr) {
		client_->map_->IncPlayerActivity(client_.get());
	}

	if(client_->skillInUse[19]) {
		client_->map_->ClearOwner(sX, sY);
		client_->map_->SetOwner(client_, sX, sY);
	}

	ClearSkillUsingStatus(client_);

	pItem = client_->map_->pGetItem(sX, sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &RemainItemColor); 
	if (pItem != 0) {

		if(!pItem->m_disabled && _bAddClientItemList(client_, pItem, &iEraseReq)) {

			//_bItemLog(ITEMLOG_GET, iClientH, (int) -1, pItem);

			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_SETITEM, client_->map_,
				client_->x, client_->y,
				sRemainItemSprite, sRemainItemSpriteFrame, RemainItemColor);

			SendItemNotifyMsg(client_, NOTIFY_ITEMOBTAINED, pItem, 0);
			if (iEraseReq == 1) delete pItem;
		}
		else 
		{

			client_->map_->bSetItem(sX, sY, pItem);

			SendItemNotifyMsg(client_, NOTIFY_CANNOTCARRYMOREITEM, 0, 0);
		}
	}

	sw.write_int32(MSGID_RESPONSE_MOTION);
	sw.write_int16(OBJECTMOTION_CONFIRM);

	client_->write(sw);

	return 1;
}

int gserver::iClientMotion_Attack_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint16_t dX, uint16_t dY, int16_t wType, uint8_t cDir, uint64_t wTargetObjectID, bool bResponse /*= true*/, bool bIsDash /*= false*/)
{
    //TODO: fix
/*
	char cData[100];
	uint64_t dwTime;
	int32_t iRet, iExp, tdX = 0, tdY = 0;
	int16_t sOwner=0, sAbsX, sAbsY =0;
	char    cOwnerType, tgtDist = 1;
	bool    bNearAttack = false;
	int32_t iErr = 0, tX = 0, tY = 0, i = 1;
	int16_t sItemIndex;
	stream_write sw;
	
	if (client == nullptr) return 0;
	if ((cDir <= 0) || (cDir > 8))       return 0;
	if (client_->m_bIsInitComplete == false) return 0;
	if (client_->dead_ == true) return 0;

	dwTime = utility::current_time();
	client_->m_dwLastActionTime = dwTime;

	std::shared_ptr<client> target;
	std::shared_ptr<npc> npc;
	shared_ptr<Unit> maptarget = client_->map_->GetOwner(dX, dY);

    if (maptarget == nullptr)
    {
        //attacking air //TODO: can they?
        return;
    }
    
    if (maptarget->is_player())
		target = GetClient(wTargetObjectID);
	else if (maptarget->is_npc())
		npc = client_->map_->GetNpc(wTargetObjectID);
	else
	{
		sw.write_int32(MSGID_RESPONSE_MOTION);
		sw.write_int16(OBJECTMOTION_REJECT);
		client_->socket->write(sw);
		return 0;
	}

#ifndef NO_MSGSPEEDCHECK
	client_->m_iAttackMsgRecvCount++;
	if (client_->m_iAttackMsgRecvCount >= 2)
	{
		if (client_->m_dwAttackLAT != 0)
		{			//68059813-68020153=39660
			if ((dwTime - client_->m_dwAttackLAT) < 12) {
				log->info(Poco::format("(!) Speed hack suspect(%s) - attack-lat(%?d)", client_->name, dwTime - client_m_dwAttackLAT));
				DeleteClient(client_->shared_from_this(), true, true);
				return 0;
			}
		}
		client_->m_dwAttackLAT = dwTime;
		client_->m_iAttackMsgRecvCount = 0;
	}
#endif

    //
    if (wType == OWNERTYPE_PLAYER)
    {
		if (wTargetObjectID < MAXCLIENTS)
        {
			if (target != nullptr)
            {
				tdX = target->x;
				tdY = target->y;
				tgtDist = 1;
			}
		}
		else if ((wTargetObjectID > 10000) && (wTargetObjectID < (10000 + MAXNPCS)))
        {
			if (npc != nullptr)
            {
				tdX = npc->x;
				tdY = npc->y;
				tgtDist = (npc->m_cSize < 2) ? 1 : npc->m_cSize;
			}
		}
		maptarget = client_->map_->GetOwner(dX, dY);
		if (maptarget != nullptr)
        {
			if (maptarget->tag == (wTargetObjectID - 10000))
            {
				dX = tdX = maptarget->x;
				dY = tdY = maptarget->y;
				bNearAttack = false;
			}
			else if ((tdX == dX) && (tdY == dY))
            {
				bNearAttack = false;
			}
			else if ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1))
            {
				dX = tdX;
				dY = tdY;
				bNearAttack = true;
			}
		}
	}
	if ((dX < 0) || (dX >= client_->map_->sizeX) ||
		(dY < 0) || (dY >= client_->map_->sizeY)) return 0;


	if ((sX != client_->x) || (sY != client_->y)) return 2;


	if (client_->map != nullptr) {
		client_->map_->IncPlayerActivity(client.get());
	}

	sAbsX = abs(sX - dX);
	sAbsY = abs(sY - dY);
	if ((wType != 2) && (wType < 20)) {
		if (client_->Equipped.RightHand.initialized) {
			if (client_->Equipped.RightHand.m_sIDnum == ITEM_STORMBRINGER) {
				if (sAbsX >(3 + tgtDist) || sAbsY > (3 + tgtDist)) wType = 0;
			}
			else{
				if (sAbsX > tgtDist || sAbsY > tgtDist) wType = 0;
			}
		}
		else{
			if (sAbsX > tgtDist || sAbsY > tgtDist) wType = 0;
		}
	}

	if (client_->skillInUse[19]) {
		client_->map_->ClearOwner(sX, sY);
		client_->map_->SetOwner(client_, sX, sY);
	}

	ClearSkillUsingStatus(client.get());

	client_->direction = cDir;

	iExp = 0;

	maptarget = client_->map_->GetOwner(dX, dY);

	if (maptarget != nullptr) {
		if ((wType != 0) && ((dwTime - client_->m_dwRecentAttackTime) > 100)) {
			if (client_->m_bIsInBuilding == false)
            {
				if (client_->Equipped.RightHand.initialized && client_->Equipped.RightHand.itemEffectType == ITEMEFFECTTYPE_ATTACK_MAGICITEM)
                {
					short sType;
					if (wType >= 20)
                    {
						sType = client_->Equipped.RightHand.itemEffectV3;
						//PlayerMagicHandler(iClientH, dX, dY, sType, TRUE);
					}
					else
                    {
						sType = client_->Equipped.RightHand.itemEffectV2;
						//PlayerMagicHandler(iClientH, dX, dY, sType, TRUE);
					}
				}
				if (client_->Equipped.RightHand.initialized)
                {
					if (client_->Equipped.RightHand.m_sIDnum == ITEM_DIRECTION_BOW)
                    {
						sAbsX = abs(sX - dX);
						sAbsY = abs(sY - dY);
						tgtDist = sAbsX > sAbsY ? sAbsX : sAbsY;
						for (i = 1; i < tgtDist; i++)
                        {
							if (utility::gen_rand(1, 4) <= 3)
                            {
								iErr = 0;
								CMisc::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
								maptarget = client_->map_->GetOwner(tX, tY);
								if (maptarget->get_owner_type() != OWNERTYPE_NONE)
									iExp += CalculateAttackEffect(maptarget, client_, tX, tY, wType, bNearAttack, bIsDash);
							}
						}

						maptarget = client_->map_->GetOwner(dX, dY);
						if (iExp == 0 && maptarget->is_npc() && utility::gen_rand(1, 2) == 1)
                        {
							iExp += CalculateAttackEffect(maptarget, client_, dX, dY, wType, bNearAttack, bIsDash);
						}
						iExp += CalculateAttackEffect(maptarget, client_, dX, dY, wType, bNearAttack, bIsDash);
					}
					else if (client_->Equipped.TwoHand.m_sIDnum == ITEM_FIRE_BOW)
                    {
						if ((client_->m_sAppr2 & 0xF000) != 0)
                        {

							if (!client_->map_->iGetAttribute(sX, sY, 0x00000006) && (abs(dX - sX) > 1 || abs(dY - sY) > 1))
								iAddDynamicObjectList(client_, DYNAMICOBJECT_FIRE, client_->map, dX, dY, (utility::gen_rand(1, 7) + 3) * 1000, 8);

							iExp += CalculateAttackEffect(maptarget, client_, dX, dY, wType, bNearAttack, bIsDash);
						}
					}
					else
                    {
						iExp += CalculateAttackEffect(maptarget, client_, dX, dY, wType, bNearAttack, bIsDash);
					}
				}
				else
                {
					iExp += CalculateAttackEffect(maptarget, client_, dX, dY, wType, bNearAttack, bIsDash);
				}
			}
			else
            {
				iExp += CalculateAttackEffect(maptarget, client), dX, dY, wType, bNearAttack, bIsDash);
			}
			client_->m_dwRecentAttackTime = dwTime;
		}
	}
	else _CheckMiningAction(client.get(), dX, dY);


	if (iExp != 0) 	GetExp(client.get(), iExp, true);


	if (bResponse == true)
	{
		sw.write_int32(MSGID_RESPONSE_MOTION);
		sw.write_int16(OBJECTMOTION_ATTACK_CONFIRM);

		client_->socket->write(sw);
	}

	maptarget = client_->map_->GetOwner(dX, dY);
	if (maptarget == nullptr){ return 0;  std::cout << "Error Client Motion Handler Error code 1" << std::endl; }
	switch (maptarget->get_owner_type())
	{
		case OWNERTYPE_PLAYER:
			break;

		case OWNERTYPE_NPC:
		{
			/ *Npc * npc = static_cast<Npc*>(maptarget);
			Client* client2;
			client2 = GetClient(client_->m_handle);
			SendNotifyMsg(NULL, GetClient(client_->m_handle), NOTIFY_NPCBAR, (npc->m_iHP * 100) / (npc->m_iHitDice * 6 + npc->m_iHitDice), npc->m_iMP, NULL, NULL);
			if (npc->m_iMP != 0)
				SendNotifyMsg(nullptr, client.get(), NOTIFY_NPCBAR2, (npc->m_iMP * 100) / npc->m_iMaxMana, 0, 0, npc->m_cNpcName);
			else
				SendNotifyMsg(nullptr, client.get(), NOTIFY_NPCBAR2, 0, 0, 0, npc->m_cNpcName);
			break;* /
		}
	}*/
	return 1;
}

int gserver::iClientMotion_Move_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir, bool bIsRun)
{
	uint64_t dwTime;
	short dX, dY, sDOtype;
	int   iDamage;
	bool  bRet;

	if (!client_) return 0;
	if ((cDir <= 0) || (cDir > 8)) return 0;
	if (client_->dead_ == true) return 0;
	if (client_->m_bIsInitComplete == false) return 0;


	if ((sX != client_->x) || (sY != client_->y)) return 2;

	dwTime = utility::current_time();
	client_->m_dwLastActionTime = dwTime;

#ifndef NO_MSGSPEEDCHECK
	if (bIsRun == false)
	{
		client_->m_iMoveMsgRecvCount++;

		if (client_->m_iMoveMsgRecvCount >= 7)
		{
			if (client_->m_dwMoveLAT != 0)
			{
			
				if ((dwTime - client_->m_dwMoveLAT) < (72*8*7 -3000))
				{
					log->info("(!) Speed hack suspect({}) - move-lat({})", client_->name, dwTime - client_->m_dwMoveLAT); 
					DeleteClient(client_, true, false);
					return 0;
				}
			}
			client_->m_dwMoveLAT = dwTime;
			client_->m_iMoveMsgRecvCount = 0;
		}
	}
	else
	{
		client_->m_iRunMsgRecvCount++;

		if (client_->m_iRunMsgRecvCount >= 7)
		{
			if (client_->m_dwRunLAT != 0)
			{

				if ((dwTime - client_->m_dwRunLAT) < (43*8*7 -1500))
				{
					DeleteClient(client_, true, false);
					return 0;
				}
			}
			client_->m_dwRunLAT	= dwTime;
			client_->m_iRunMsgRecvCount = 0;
		}
	}
#endif


	if (client_->map_ != nullptr)
	{
		client_->map_->IncPlayerActivity(client_.get());
	}

	//ClearSkillUsingStatus(iClientH);

	dX = client_->x;
	dY = client_->y;

	switch (cDir)
	{
        case 1:	dY--; break;
        case 2:	dX++; dY--;	break;
        case 3:	dX++; break;
        case 4:	dX++; dY++;	break;
        case 5: dY++; break;
        case 6:	dX--; dY++;	break;
        case 7:	dX--; break;
        case 8:	dX--; dY--;	break;
	}

	item * pTopItem = 0; // 2.172

	bRet = client_->map_->bGetMoveable(dX, dY, &sDOtype, pTopItem);

	if (client_->magic_effect_status[ MAGICTYPE_HOLDOBJECT ] != 0)
		bRet = false;

	if (bRet == true)
	{

		//if (client_->m_iQuest != NULL) _bCheckIsQuestCompleted(iClientH);

		client_->map_->ClearOwner(client_->x, client_->y);

		client_->x   = dX;
		client_->y   = dY;
		client_->direction = cDir;

		if(!client_->IsEthereal())
		{
			client_->map_->SetOwner(client_, dX, dY);
		}

		if( client_->GetParty())
		{
			client_->GetParty()->update_member_coords(client_.get());
		}

// 		if (m_astoria.get() && m_astoria->GetEventget_type() == ET_CAPTURE && 
// 			m_astoria->GetRelicHolder() == m_pClientList[iClientH])
// 		{
// 			m_astoria->m_relicHolderSteps++;
// 			if (m_astoria->m_relicHolderSteps > 4){
// 				m_astoria->m_relicHolderSteps = 0;
// 				UpdateRelicPos();
// 			}
// 		}

		if (sDOtype == DYNAMICOBJECT_SPIKE) {
			if ((client_->neutral == true) && ((client_->m_sAppr2 & 0xF000) == 0)) {

			}
			else {
				//iDamage = utility::gen_rand(2,4);
				iDamage = 175; // Player Damage Spike Field xRisenx

				if (!client_->IsInvincible()){
					if (client_->health <= iDamage)
						client_->health = 0;
					else
						client_->health -= iDamage;
					client_->m_lastDamageTime = dwTime;
				}
			}
		}

		/*

		short sRemainItemSprite, sRemainItemSpriteFrame;
		char cRemainItemColor;

		switch (pTopItem->m_sIDnum) {
		case 540: 
		if (m_pClientList[iClientH]->m_side == 2) {

		if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS] != -1) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]] != NULL) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]]->m_sItemEffectType == ITEMEFFECTTYPE_DEFENSE_ANTIMINE)) {

		}
		else {


		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, (61+100), m_pClientList[iClientH]->m_sType);
		}


		pTopItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);

		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
		sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);
		delete pTopItem;
		}
		break;
		case 541: 			if (m_pClientList[iClientH]->m_side == 1) {

		if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS] != -1) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]] != NULL) && 
		(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[EQUIPPOS_BOOTS]]->m_sItemEffectType == ITEMEFFECTTYPE_DEFENSE_ANTIMINE)) {

		}
		else {


		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, (61+100), m_pClientList[iClientH]->m_sType);
		}


		pTopItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);

		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
		m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
		sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);
		delete pTopItem;
		}
		break;
		}
		//
		*/

		stream_write sw(100);

		sw.write_int32(MSGID_RESPONSE_MOTION);
		sw.write_int16(OBJECTMOVE_CONFIRM);
		sw.write_int16(dX - 12);//800x600
		//sw.write_int16(dX - 10);
		sw.write_int16(dY - 9);//800x600
		//sw.write_int16(dY - 7);
		sw.write_int8(cDir);

		if (bIsRun == true)
		{
			if (client_->stamina > 0)
			{
				if (client_->m_iTimeLeft_FirmStamina == 0)
				{
					client_->stamina--;
					sw.write_int8(1);
				}
				else
					sw.write_int8(0);
			}
			else {
				if (client_->m_iTimeLeft_FirmStamina == 0) {
					client_->stamina--;
					sw.write_int8(1);
				}
				else
					sw.write_int8(0);

				if (client_->stamina < -10)
				{
					client_->stamina = 0;
					DeleteClient(client_);
					return 0;
				}
			}
		}
		else
			sw.write_int8(0);

		sw.write_int32(uint32_t(client_->health));

		int ix, iy, iSize = 2, iIndex = 0;
		tile * pTileSrc;
		short total = 0;
		uint64_t oldpos = sw.position;
		sw.write_int16(0);//total placeholder

		pTileSrc = client_->map_->GetTile(dX - 12, dY - 9);

		while (1)
		{
			ix = _tmp_iMoveLocX[cDir][iIndex];
			iy = _tmp_iMoveLocY[cDir][iIndex];
			if ((ix == -1) || (iy == -1)) break;
			iIndex++;

			if (WriteTileData(sw, client_, ix, iy))
				total++;
		}

		
		char * temp = sw.data + oldpos;
		*((short *)temp) = total;

//#ifdef 800x600
		//iSize = iComposeMoveMapData((short)(dX - 12), (short)(dY - 9), iClientH, cDir, cp);
//#else
		//iSize = iComposeMoveMapData((short)(dX - 10), (short)(dY - 7), iClientH, cDir, cp);
//#endif
		client_->write(sw);


        //TODO: this should only update the edges
/*
        int ytiles = client_->screenheight_v / 32;
        int xtiles = client_->screenwidth_v / 32;
        for (int y = client_->y - ytiles / 2; y < client_->y + ytiles / 2; ++y)
        {
            for (int x = client_->x - xtiles / 2; x < client_->x + xtiles / 2; ++x)
            {
                stream_write sw;
                sw.write_int32(MSGID_MODIFYTILE);
                if (WriteTileData(sw, client_, x, y))
                {
                    client_->write(sw);
                }
            }
        }*/

	}
	else
 {
		client_->rejectedMove = true; 
		client_->m_resetMoveFreq = true; 

		stream_write sw(50);

		sw.write_int32(MSGID_RESPONSE_MOTION);
		sw.write_int16(OBJECTMOVE_REJECT);
        sw.write_int16(uint16_t(client_->handle + 30000));
        sw.write_int8(client_->get_owner_type());
        sw.write_int16(client_->x);
		sw.write_int16(client_->y);
		sw.write_int16(client_->get_type());
		sw.write_int8(cDir);
		sw.write_string(client_->name, 10);
		sw.write_int16(client_->m_sAppr1);
		sw.write_int16(client_->m_sAppr2);
		sw.write_int16(client_->m_sAppr3);
		sw.write_int16(client_->m_sAppr4);
		sw.write_int32(client_->m_iApprColor);
		sw.write_int16(client_->m_sHeadApprValue);
		sw.write_int16(client_->m_sBodyApprValue);
		sw.write_int16(client_->m_sArmApprValue);
		sw.write_int16(client_->m_sLegApprValue);
		sw.write_int32(client_->status);

		client_->write(sw);

		std::shared_ptr<unit> owner = client_->map_->GetOwner(dX, dY);
		if(owner)
		{
			if(owner->is_player())
				RequestFullObjectData(client_, owner);
		}

		return 0;
	}

	return 1;
}

int gserver::iClientMotion_Magic_Handler(std::shared_ptr<client> client_, uint16_t sX, uint16_t sY, uint8_t cDir)
{
	if ((!client_) || (client_->is_dead()) || (!client_->m_bIsInitComplete)) return 0;

	if ((sX != client_->x) || (sY != client_->y)) return 2;

	if (!client_->map_) return 0;

	client_->map_->IncPlayerActivity(client_.get());

	if (client_->skillInUse[19])//TODO: which skill was this again?
	{
		client_->map_->ClearOwner(sX, sY);
		client_->map_->SetOwner(client_, sX, sY);
	}

	ClearSkillUsingStatus(client_);

	client_->remove_magic_effect(MAGICTYPE_INVISIBILITY);

	client_->direction = cDir;

	stream_write sw;

	sw.write_int32(MSGID_RESPONSE_MOTION);
	sw.write_int16(OBJECTMOTION_CONFIRM);

	client_->write(sw);

	return 1;
}

void gserver::ClientCommonHandler(std::shared_ptr<client> client_, stream_read & sr)
{
	if (!client_->m_bIsInitComplete) return;
	if (client_->dead_) return;

	uint32_t msgid = sr.read_int32();
	uint16_t command = sr.read_int16();

// 	uint16_t sX = sr.read_int16();
// 	uint16_t sY = sr.read_int16();
// 
// 	uint8_t direction = sr.read_int8();
// 
// 	int32_t iV1 = sr.read_int32();
// 	int32_t iV2 = sr.read_int32();
// 	int32_t iV3 = sr.read_int32();
// 
// 	string stringmsg = sr.read_string(30);
// 
// 	int32_t iV4 = sr.read_int32();

	switch(command)
	{
// 			case COMMONTYPE_REQ_CREATESLATE:
// 		ReqCreateSlateHandler(iClientH, pData);
// 		break;
// 
// 	case COMMONTYPE_REQGUILDNAME:
// 		RequestGuildNameHandler(iClientH, iV1, iV2);
// 		break;
// 
// 	case COMMONTYPE_UPGRADEITEM:
// 		RequestItemUpgradeHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_ACCEPTJOINPARTY:
// 		RequestAcceptJoinPartyHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_SETGUILDTELEPORTLOC:
// 		RequestSetGuildTeleportLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, pString);
// 		break;
// 
// 	case COMMONTYPE_SETGUILDCONSTRUCTLOC:
// 		RequestSetGuildConstructLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, pString);
// 		break;
// 
// 	case COMMONTYPE_GUILDTELEPORT:
// 		RequestGuildTeleportHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_SUMMONWARUNIT:
// 		RequestSummonWarUnitHandler(iClientH, sX, sY, iV1, iV2, iV3);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_MAPSTATUS:
// 		MapStatusHandler(iClientH, iV1, pString);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_SELECTCRUSADEDUTY:
// 		SelectCrusadeDutyHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_CANCELQUEST:
// 		CancelQuestHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_ACTIVATESPECABLTY:
// 		ActivateSpecialAbilityHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQUEST_JOINPARTY:
// 		JoinPartyHandler(iClientH, iV1, pString);
// 		break;
// 
// 	case COMMONTYPE_GETMAGICABILITY:
// 		GetMagicAbilityHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_BUILDITEM:
// 		BuildItemHandler(iClientH, pData);
// 		break;
// 
// 	case COMMONTYPE_CANCELEXCHANGEITEM:
// 		CancelExchangeItem(iClientH);
// 		break;
// 
// 	case COMMONTYPE_CONFIRMEXCHANGEITEM:
// 		ConfirmExchangeItem(iClientH);
// 		break;
// 
// 	case COMMONTYPE_SETEXCHANGEITEM:
// 		SetExchangeItem(iClientH, iV1, iV2);
// 		break;
// 
// 	case COMMONTYPE_REQ_GETHEROMANTLE:
// 		GetHeroMantleHandler(iClientH, iV1, pString);
// 		break;
// 		// EK Trade xRisenx
// 	/*case COMMONTYPE_REQ_GETTRADEEKITEM:
//         GetTradeEKItemHandler(iClientH, iV1, pString);  
//         break;*/
// 		// EK Trade xRisenx
// 		// Ball Points xRisenx
// 	/*case COMMONTYPE_REQ_GETTRADEBALLITEM:    
//         GetTradeBALLItemHandler(iClientH, iV1, pString);  
//         break;*/
// 		// Ball Points xRisenx
// 	case COMMONTYPE_REQ_GETOCCUPYFLAG:
// 		GetOccupyFlagHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_SETDOWNSKILLINDEX:
// 		SetDownSkillIndexHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_TALKTONPC:
// 		NpcTalkHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQ_CREATEPOTION:
// 		ReqCreatePortionHandler(iClientH, pData);
// 		break;
// 
// 	case COMMONTYPE_REQ_GETFISHTHISTIME:
// 		ReqGetFishThisTimeHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_REPAIRITEMCONFIRM:
// 		ReqRepairItemCofirmHandler(iClientH, iV1, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_REPAIRITEM:
// 		ReqRepairItemHandler(iClientH, iV1, iV2, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_SELLITEMCONFIRM:
// 		ReqSellItemConfirmHandler(iClientH, iV1, iV2, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_SELLITEM:
// 		ReqSellItemHandler(iClientH, iV1, iV2, iV3, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_USESKILL:
// 		UseSkillHandler(iClientH, iV1, iV2, iV3);
// 		break;

	case COMMONTYPE_REQ_USEITEM:
	{
		uint16_t sX = sr.read_int16();
		uint16_t sY = sr.read_int16();

		uint8_t direction = sr.read_int8();

		int32_t iV1 = sr.read_int32();
		int32_t iV2 = sr.read_int32();
		int32_t iV3 = sr.read_int32();
		int32_t iV4 = sr.read_int32();
		UseItemHandler(client_.get(), iV1, iV2, iV3, iV4);
	}
		break;

// 	case COMMONTYPE_REQ_GETREWARDMONEY:
// 		GetRewardMoneyHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_ITEMDROP:
// 		DropItemHandler(iClientH, iV1, iV2, pString, true);
// 		break;
// 
// 	case COMMONTYPE_EQUIPITEM:
// 		bEquipItemHandler(iClientH, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQ_PURCHASEITEM:
// 		RequestPurchaseItemHandler(iClientH, pString, iV1);
// 		break;
// 
// 	case COMMONTYPE_REQ_STUDYMAGIC:
// 		RequestStudyMagicHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_REQ_TRAINSKILL:
// 		//RequestTrainSkillHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_GIVEITEMTOCHAR:
// 		GiveItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
// 		break;
// 
// 	case COMMONTYPE_EXCHANGEITEMTOCHAR:
// 		ExchangeItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
// 		break;
// 
// 	case COMMONTYPE_JOINGUILDAPPROVE:
// 		JoinGuildApproveHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_JOINGUILDREJECT:
// 		JoinGuildRejectHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_DISMISSGUILDAPPROVE:
// 		DismissGuildApproveHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_DISMISSGUILDREJECT:
// 		DismissGuildRejectHandler(iClientH, pString);
// 		break;
// 
// 	case COMMONTYPE_RELEASEITEM:
// 		ReleaseItemHandler(iClientH, iV1, true);
// 		break;
// 
	case COMMONTYPE_TOGGLECOMBATMODE:
		ToggleCombatModeHandler(client_);
		break;

	case COMMONTYPE_MAGIC:
	{
		uint16_t sX = sr.read_int16();
		uint16_t sY = sr.read_int16();

		uint8_t direction = sr.read_int8();

		int32_t iV1 = sr.read_int32();
		int32_t iV2 = sr.read_int32();
		int32_t iV3 = sr.read_int32();

		//string stringmsg = sr.read_string(30);

		//int32_t iV4 = sr.read_int32();
		//if (client_->m_hasPrecasted || client_->IsGM())
			PlayerMagicHandler(client_, iV1, iV2, (iV3 - 100));
	}
		break;
// 
// 	case COMMONTYPE_TOGGLESAFEATTACKMODE:
// 		ToggleSafeAttackModeHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET:
// 		GetFightzoneTicketHandler(iClientH);
// 		break;
// 
// 	case COMMONTYPE_CRAFTITEM:
// 		ReqCreateCraftingHandler(iClientH, pData);
// 		break; 
// 
	case COMMONTYPE_REQTITLE:
		RequestTitleHandler(client_, sr);
		break;
// 
// 	case COMMONTYPE_REQ_REPAIRALL:
// 		ReqRepairAll(iClientH);
// 		break;
// 
// 	case COMMONTYPE_REQ_REPAIRALLCONFIRM:
// 		ReqRepairAllConfirmHandler(iClientH, iV1);
// 		break;
	default:
		log->error("Unknown common packet received from client - 0x{x}", (uint32_t)command);
		break;
	}
}

int  _tmp_iMCProb[] = { 0, 300, 250, 200, 150, 100, 80, 70, 60, 50, 40 };
int  _tmp_iMLevelPenalty[] = { 0, 5, 5, 8, 8, 10, 14, 28, 32, 36, 40 };
void gserver::PlayerMagicHandler(std::shared_ptr<client> client_, uint16_t dX, uint16_t dY, int16_t sType, bool bItemEffect /*= false*/, int32_t iV1 /*= 0*/)
{
	short  sX, sY, sMagicCircle, rx = 0, ry = 0, sRemainItemSprite = 0, sRemainItemSpriteFrame = 0, sLevelMagic;
	char   cData[120] = {}, cDir = 0, cName[11] = {}, cNpcWaypoint[11], cName_Master[11] = {}, cRemainItemColor = 0;
	string cNpcName;
	double dV1, dV2, dV3, dV4;
	int    i, iErr, iRet = 0, ix, iy, iResult, iDiceRes, iNamingValue = 0, iFollowersNum, iEraseReq = 0, weatherBonus;
	int    tX, tY, iManaCost, iMagicAttr, iItemID;
	const int crossPnts[5][2] = { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
	std::shared_ptr<unit> owner;
	item * pItem;
	uint32_t * dwp = 0;
	uint64_t dwTime = utility::current_time();
	uint16_t  * wp = 0, wWeaponType;
	magic * spell = m_pMagicConfigList[sType];

	if (spell == nullptr)
		return;

	weatherBonus = 0;

#ifdef DEF_GUILDWARMODE
	switch (sType) {
        case 31:
        case 40:
        case 41:
        case 46:
        case 54:
        case 55:
        case 73:
            return;
	}
#endif

	if (client_ == nullptr) return;

	if (!client_->m_bIsInitComplete) return;

	if (client_->map_ == nullptr) return;

	if ((dX < 0) || (dX >= client_->map_->sizeX) || (dY < 0) || (dY >= client_->map_->sizeY)) return;

	if (abs(dX - client_->x) > 13 || abs(dY - client_->y) > 13)
	{
		log->error("Magic casted out of range - Player: {} : ({},{}) -> ({},{})", client_->name, client_->x, client_->y, dX, dY);
		return;
	}

	if ((sType < 0) || (sType >= 100)) return;
	if ((bItemEffect == false) && (client_->magicMastery[sType] != 1)) return;

	if (client_->map_->flags.attackEnabled == false) return;

	//check if staff is equipped (can only cast with staff or empty hands)
	if (client_->itemEquipStatus[EQUIPPOS_RHAND] != -1)
	{
		wWeaponType = ((client_->m_sAppr2 & 0x0FF0) >> 4);
		if ((wWeaponType < 35) || (wWeaponType > 38))
		return;
	}

	if ((client_->itemEquipStatus[EQUIPPOS_LHAND] != -1) ||
		(client_->itemEquipStatus[EQUIPPOS_TWOHAND] != -1)) return;

	//if ((dwTime - m_pClientList[iClientH]->m_dwRecentAttackTime) < 100) return; 
	client_->m_dwRecentAttackTime = dwTime;
	client_->m_dwLastActionTime = dwTime;

	sX = client_->x;
	sY = client_->y;

	//         1      2     3     4     5	 6     7	 8	  9    10
	//        300%	250%  200%  150%  100%  80%   70%   60%  50%   40%
	sMagicCircle = (sType / 10) + 1;
	if (client_->skillMastery[4] == 0)
		dV1 = 1.0f;
	else dV1 = (double)client_->skillMastery[4];

	if (bItemEffect == true) dV1 = (double)100.0f;

	dV2 = (double)(dV1 / 100.0f);
	dV3 = (double)_tmp_iMCProb[sMagicCircle];

	dV1 = dV2 * dV3;
	iResult = (int)dV1;

	if (client_->GetInt() > 50)
		iResult += (client_->GetInt() - 50) / 2;

	sLevelMagic = (client_->level / 10);
	if (sMagicCircle != sLevelMagic)
	{
		if (sMagicCircle > sLevelMagic)
		{
			dV1 = (double)(client_->level - sLevelMagic * 10);
			dV2 = (double)abs(sMagicCircle - sLevelMagic)*_tmp_iMLevelPenalty[sMagicCircle];
			dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
			dV4 = (dV1 / dV3)*dV2;

			iResult -= abs(abs(sMagicCircle - sLevelMagic)*_tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
		}
		else
		{
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}

	switch (client_->map_->weather)
	{
		case 0: break;
		case 1: iResult = iResult - (iResult / 24); break;
		case 2:	iResult = iResult - (iResult / 12); break;
		case 3: iResult = iResult - (iResult / 5);  break;
		default: break;
	}

	if (client_->m_iSpecialWeaponEffectType == 10)
	{
		dV1 = (double)iResult;
		dV2 = (double)(client_->m_iSpecialWeaponEffectValue * 3);
		dV3 = dV1 + dV2;
		iResult = (int)dV3;
	}

	if (iResult <= 0) iResult = 1;

	iResult = 100;

	//iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus);

	iManaCost = spell->manaCost;//Value1 ??
	if ((client_->safeAttackMode == true) && (client_->map_->flags.fightZone == false))
	{
		iManaCost += (iManaCost / 2) - (iManaCost / 10);
	}

	if (client_->manaSave > 0)
	{
		dV2 = double(client_->manaSave) / 100.0f;
		dV3 = iManaCost;
		dV1 = dV2 * dV3;
		dV2 = dV3 - dV1;
		iManaCost = dV2;
		if (iManaCost <= 0) iManaCost = 1;
	}

	if (iResult < 100)
	{
		iDiceRes = utility::gen_rand(1, 100);
		if (iResult < iDiceRes)
		{
			SendEventToNearClient_TypeA(client_, MSGID_MOTION_DAMAGE, 0, -1, 0);
			return;
		}
	}
	if (((client_->m_iHungerStatus <= 10) || (client_->stamina <= 0)) && (utility::gen_rand(1, 1000) <= 100))
	{
		SendEventToNearClient_TypeA(client_, MSGID_MOTION_DAMAGE, 0, -1, 0);
		return;
	}

	if (client_->mana < iManaCost)
	{
		return;
	}

	iResult = client_->skillMastery[4];
	if (client_->GetMag() > 50) iResult += (client_->GetMag() - 50);

	sLevelMagic = (client_->level / 10);
	if (sMagicCircle != sLevelMagic)
	{
		if (sMagicCircle > sLevelMagic)
		{
			dV1 = double(client_->level - sLevelMagic * 10);
			dV2 = double(abs(sMagicCircle - sLevelMagic)*_tmp_iMLevelPenalty[sMagicCircle]);
			dV3 = double(abs(sMagicCircle - sLevelMagic) * 10);
			dV4 = (dV1 / dV3)*dV2;

			iResult -= abs(abs(sMagicCircle - sLevelMagic)*_tmp_iMLevelPenalty[sMagicCircle] - int(dV4));
		}
		else
		{
			iResult += 5 * abs(sMagicCircle - sLevelMagic);
		}
	}

	iResult += client_->m_iAddAR;

	if (iResult <= 0) iResult = 1;
	if (sType >= 80) iResult += 10000;

	if (spell->category == 1)
		if (client_->map_->iGetAttribute(sX, sY, 0x00000005) != 0) return;

	iMagicAttr = 0;
	//iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;

	if ((client_->status & 0x10) != 0)
	{
		client_->set_status_flag(STATUS_INVISIBILITY, false);

		RemoveFromDelayEventList(client_, MAGICTYPE_INVISIBILITY);
	}

	owner = client_->map_->GetOwner(dX, dY);

	//old MAGIC_NOEFFECT

	client_->mana -= iManaCost;
	if (client_->mana < 0)
		client_->mana = 0;

	client_->CalculateSSN_SkillIndex(SKILL_MAGIC, 1);

	SendNotifyMsg(nullptr, client_, NOTIFY_MP, 0, 0, 0);

	SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_MAGIC, client_->map_,
		client_->x, client_->y, dX, dY, (sType + 100), client_->get_type());


	if ((owner != nullptr) && (m_bIsCrusadeMode == false) && (owner->is_player()))
	{
        std::shared_ptr<client> player = std::static_pointer_cast<client>(owner);
		if (((client_->civilian == true) ||
			(player->civilian == true)) && (client_->side != owner->side))
			return;

		if ((client_->civilian == true) && (player->civilian == false))
		{
			switch (spell->magicType)
			{
			case MAGICTYPE_SPDOWN_AREA:
			case MAGICTYPE_SUMMON:
			case MAGICTYPE_PROTECT:
			case MAGICTYPE_HOLDOBJECT:
			case MAGICTYPE_INVISIBILITY:
			case MAGICTYPE_BERSERK:
			case MAGICTYPE_POISON:
			case MAGICTYPE_HPUP_SPOT:
				return;
			}
		}
	}



	if (spell->delayTime == 0)
	{
		switch (spell->magicType)
		{
		case MAGICTYPE_POLYMORPH:
		{
			owner = client_->map_->GetOwner(dX, dY);
			if (owner == nullptr) return;
			if (owner->CheckResistMagic(client_->direction, iResult) == false)
			{
				if (owner->is_player())
				{
                    std::shared_ptr<client> temp = std::static_pointer_cast<client>(owner);
					if (temp->magic_effect_status[MAGICTYPE_POLYMORPH] != 0) return;
					temp->magic_effect_status[MAGICTYPE_POLYMORPH] = spell->m_sValue[MAGICV_TYPE];
					temp->set_type_original(temp->get_type());
					temp->set_type(18);
					SendEventToNearClient_TypeA(temp, MSGID_MOTION_NULL, 0, 0, 0);
				}
				else if (owner->is_npc())
				{
                    std::shared_ptr<npc> temp = std::static_pointer_cast<npc>(owner);
					if (temp->magic_effect_status[MAGICTYPE_POLYMORPH] != 0) return;
					temp->magic_effect_status[MAGICTYPE_POLYMORPH] = spell->m_sValue[MAGICV_TYPE];
					temp->set_type_original(temp->get_type());
					temp->set_type(18);
					SendEventToNearClient_TypeA(temp, MSGID_MOTION_NULL, 0, 0, 0);
				}

				RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_POLYMORPH, dwTime + uint64_t(spell->lastTime * 1000),
					owner, owner->map_, 0, 0, 0, spell->m_sValue[MAGICV_TYPE], 0);

				if (owner->is_player())
					SendNotifyMsg(nullptr, std::static_pointer_cast<client>(owner), NOTIFY_MAGICEFFECTON, MAGICTYPE_POLYMORPH, spell->m_sValue[MAGICV_TYPE], 0);
			}
			break;
		}

		case MAGICTYPE_DAMAGE_SPOT:
		{
			owner = client_->map_->GetOwner(dX, dY);
			if (owner == nullptr) return;
			if (owner->CheckResistMagic(client_->direction, iResult) == false)
				Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);

			owner = client_->map_->GetDeadOwner(dX, dY);//pretend corpse skill check
			if (owner == nullptr) return;
			if ((owner->is_player()) && (owner->health > 0))
			{
				if (owner->CheckResistMagic(client_->direction, iResult) == false)
					Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
			}
			break;
		}

		case MAGICTYPE_DAMAGE_SPOT_SPDOWN:
		{
			owner = client_->map_->GetOwner(dX, dY);
			if (owner == nullptr) return;
			if (owner->CheckResistMagic(client_->direction, iResult) == false)
			{
				Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
				Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
			}

			owner = client_->map_->GetDeadOwner(dX, dY);
			if (owner == nullptr) return;
			if ((owner->is_player()) && (owner->health > 0))
			{
				if (owner->CheckResistMagic(client_->direction, iResult) == false)
				{
					Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
					Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
				}
			}
		}

		case MAGICTYPE_DAMAGE_LINEAR_SPDOWN:
		{
			sX = client_->x;
			sY = client_->y;

			for (i = 2; i < 13; i++)
			{
				iErr = 0;
				utility::GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
				for (int pNo = 0; pNo < 5; pNo++)
				{
					owner = client_->map_->GetOwner(tX + crossPnts[pNo][0], tY + crossPnts[pNo][1]);
					if (owner == nullptr) return;
					if (owner->CheckResistMagic(client_->direction, iResult) == false)
					{
						Effect_Damage_Spot_DamageMove(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + weatherBonus, true, spell->element, spell);
						if (!owner->is_dead() && owner->CheckResistMagic(client_->direction, iResult) == false)
						{
							Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
							Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
						}
					}

					owner = client_->map_->GetDeadOwner(tX + crossPnts[pNo][0], tY + crossPnts[pNo][1]);
					if (owner == nullptr) return;
					if (owner->CheckResistMagic(client_->direction, iResult) == false)
					{
						Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + weatherBonus, true, spell->element, spell);
						if (!owner->is_dead() && owner->CheckResistMagic(client_->direction, iResult) == false)
						{
							Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
							Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
						}
					}
				}
				if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
			}


			for (iy = dY - spell->m_vRange; iy <= dY + spell->m_vRange; iy++)
			{
				for (ix = dX - spell->m_hRange; ix <= dX + spell->m_hRange; ix++)
				{
					owner = client_->map_->GetOwner(ix, iy);
					if (owner == nullptr) return;
					if (owner->CheckResistMagic(client_->direction, iResult) == false)
					{
						Effect_Damage_Spot_DamageMove(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + weatherBonus, true, spell->element, spell);
						if (!owner->is_dead() && owner->CheckResistMagic(client_->direction, iResult) == false)
						{
							Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
							Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
						}
					}

					owner = client_->map_->GetDeadOwner(ix, iy);
					if (owner == nullptr) return;
					if (owner->CheckResistMagic(client_->direction, iResult) == false)
					{
						Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + weatherBonus, true, spell->element, spell);
						if (!owner->is_dead() && owner->CheckResistMagic(client_->direction, iResult) == false)
						{
							Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
							Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
						}
					}
				}
			}

			owner = client_->map_->GetOwner(dX, dY);
			if (owner == nullptr) return;
			if (owner->CheckResistMagic(client_->direction, iResult) == false)
			{
				Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
				if (!owner->is_dead() && owner->CheckResistMagic(client_->direction, iResult) == false)
				{
					Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
					Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
				}
			}

			owner = client_->map_->GetDeadOwner(dX, dY);
			if (owner == nullptr) return;
			if (owner->CheckResistMagic(client_->direction, iResult) == false)
			{
				Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS] + weatherBonus, true, spell->element, spell);
				if (!owner->is_dead() && owner->CheckResistMagic(client_->direction, iResult) == false)
				{
					Effect_Damage_Spot(client_, owner, spell->m_sValue[MAGICV_THROW], spell->m_sValue[MAGICV_RANGE], spell->m_sValue[MAGICV_BONUS] + weatherBonus, true, spell->element, spell);
					Effect_SpDown_Spot(client_, owner, spell->m_sValue[MAGICV_LINEARTHROW], spell->m_sValue[MAGICV_LINEARRANGE], spell->m_sValue[MAGICV_LINEARBONUS]);
				}
			}
			break;
		}

		case MAGICTYPE_HPUP_SPOT:
		{
			owner = client_->map_->GetOwner(dX, dY);
			Effect_HpUp_Spot(client_, owner, m_pMagicConfigList[sType]->m_sValue[0], m_pMagicConfigList[sType]->m_sValue[1], m_pMagicConfigList[sType]->m_sValue[2]);
			break;
		}

		case MAGICTYPE_TELEPORT:
		{
			owner = client_->map_->GetOwner(dX, dY);

			switch (m_pMagicConfigList[sType]->m_sValue[3]) {
			case 1:
				if ((owner->is_player()) && (client_ == owner)) {
					//RequestTeleportHandler(client.get(), "1   ");
					RequestTeleportHandler(client_, 1);
				}
				break;
			}
			break;
		}

		case MAGICTYPE_CREATE:
		{
			if (client_->map_->bGetIsMoveAllowedTile(dX, dY) == false)
				return;

			pItem = new item;

			switch (m_pMagicConfigList[sType]->m_sValue[0]) {//4?
			case 1:
				if (utility::gen_rand(1, 2) == 1)
					iItemID = 99;
				else iItemID = 98;
				break;
			}

			//pItem->InitItemAttr(iItemID, m_pItemConfigList);

			pItem->m_sTouchEffectType = ITET_ID;
			pItem->m_sTouchEffectValue1 = utility::gen_rand(1, 100000);
			pItem->m_sTouchEffectValue2 = utility::gen_rand(1, 100000);
			pItem->m_sTouchEffectValue3 = utility::current_time();

			client_->map_->bSetItem(dX, dY, pItem);

			//_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem);

			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, client_->map_,
				dX, dY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
			break;
		}

		case MAGICTYPE_CANCELLATION:
		{
			break;
		}

		case MAGICTYPE_INHIBITION:
		{
			break;
		}

		case MAGICTYPE_DAMAGE_LINEAR:
		{
			break;
		}

		case MAGICTYPE_FSW_LINEAR:
		{
			break;
		}

		case MAGICTYPE_ICE_LINEAR:
		{
			break;
		}

		case MAGICTYPE_MB_LINEAR:
		{
			break;
		}

		case MAGICTYPE_TREMOR:
		{
			break;
		}

		case MAGICTYPE_DAMAGE_AREA:
		{
			break;
		}

		case MAGICTYPE_DAMAGE_AREA_NOSPOT:
		{
			break;
		}

		case MAGICTYPE_DAMAGE_AREA_MOVE:
		{
			break;
		}

		case MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN:
		{
			break;
		}

		case MAGICTYPE_SPDOWN_AREA:
		{
			break;
		}

		case MAGICTYPE_SPUP_AREA:
		{
			break;
		}

		case MAGICTYPE_SUMMON:
		{
			if (client_->map_->flags.fightZone) return;
			if ((m_bHeldenianMode) && (client_->map_->flags.heldenianMap)) return;

			owner = client_->map_->GetOwner(dX, dY);
			if (owner == nullptr) return;
			if (owner->is_player() && client_->side == owner->side)
			{
                //TODO: fix summon spell
				iFollowersNum = 0;//iGetFollowerNumber(sOwnerH, cOwnerType);

				if (iFollowersNum >= (client_->skillMastery[SKILL_MAGIC] / 20) && !client_->IsGM()) break;

				switch (iV1)
                {
				    case 0:
					    iResult = utility::gen_rand(1, client_->skillMastery[SKILL_MAGIC] / 10);

					    if (iResult < client_->skillMastery[SKILL_MAGIC] / 20)
						    iResult = client_->skillMastery[SKILL_MAGIC] / 20;

					    switch (iResult) {
					        case 1: cNpcName = "Slime"; break;
					        case 2: cNpcName = "Giant-Ant"; break;
					        case 3: cNpcName = "Amphis"; break;
					        case 4: cNpcName = "Orc"; break;
						        //case 5: strcpy(cNpcName, "Skeleton"); break;
					        case 5: cNpcName = "Clay-Golem"; break;
					        case 6: cNpcName = "Stone-Golem"; break;
					        case 7: cNpcName = "Orc-Mage"; break;
					        case 8: cNpcName = "Hellbound"; break;
					        case 9: cNpcName = "Cyclops"; break;
					    }
					    break;

				    case 1: cNpcName = "Orc"; break;
					    //case 2: strcpy(cNpcName, "Skeleton"); break;
				    case 2: cNpcName = "Clay-Golem"; break;
				    case 3: cNpcName = "Stone-Golem"; break;
				    case 4: cNpcName = "Hellbound"; break;
				    case 5: cNpcName = "Cyclops"; break;
				    case 6: cNpcName = "Troll"; break;
				    case 7: cNpcName = "Orge"; break;
				}
                std::shared_ptr<npc> npc = owner->map_->CreateNpc(cNpcName, 0, MOVETYPE_RANDOM, &dX, &dY, client_->side, cNpcWaypoint, 0, 0, false, true);
				if (npc != nullptr)
					npc->Follow(std::static_pointer_cast<client>(owner));
			}
			break;
		}

// 		case MAGICTYPE_PROTECT:
// 			target = m_pMapList[caster->m_cMapIndex]->GetOwner(dX, dY);
// 
// 			if (!target) goto MAGIC_NOEFFECT;
// 			if (target->is_npc())
// 			{
// 				targetNpc = (CNpc*)target;
// 				if (targetNpc->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
// 			}
// 
// 			switch (target->m_cMagicEffectStatus[MAGICTYPE_PROTECT])
// 			{
// 			case MAGICPROTECT_PFM:
// 				if (spell->m_sValue[MAGICV_TYPE] == MAGICPROTECT_AMP/* || MAGICPROTECT_GPFM*/)
// 				{
// 					RemoveFromDelayEventList(target, MAGICTYPE_PROTECT);
// 					break;
// 				}
// 				else
// 					goto MAGIC_NOEFFECT;
// 
// 			case MAGICPROTECT_DS:
// 				if (spell->m_sValue[MAGICV_TYPE] == MAGICPROTECT_GDS)
// 				{
// 					RemoveFromDelayEventList(target, MAGICTYPE_PROTECT);
// 					break;
// 				}
// 				else
// 					goto MAGIC_NOEFFECT;
// 
// 			case NULL:
// 				break;
// 
// 			default:
// 				goto MAGIC_NOEFFECT;
// 			}
// 
// 			target->m_cMagicEffectStatus[MAGICTYPE_PROTECT] = (char)spell->m_sValue[MAGICV_TYPE];
// 			switch (spell->m_sValue[MAGICV_TYPE]){
// 			case MAGICPROTECT_PFA:
// 				target->SetStatusFlag(STATUS_PFA, true);
// 				break;
// 			case MAGICPROTECT_PFM:
// 				//case MAGICPROTECT_GPFM:
// 			case MAGICPROTECT_AMP:
// 				target->SetStatusFlag(STATUS_PFM, true);
// 				break;
// 			case MAGICPROTECT_DS:
// 			case MAGICPROTECT_GDS:
// 				target->SetStatusFlag(STATUS_DEFENSESHIELD, true);
// 				break;
// 			}
// 
// 
// 			RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_PROTECT, dwTime + (spell->m_dwLastTime * 1000),
// 				target->m_handle, target->m_ownerType, NULL, NULL, NULL, spell->m_sValue[MAGICV_TYPE], NULL, NULL);
// 
// 			if (target->is_player())
// 			{
// 				targetPlr = (CClient*)target;
// 				targetPlr->Notify(NULL, NOTIFY_MAGICEFFECTON, MAGICTYPE_PROTECT, spell->m_sValue[MAGICV_TYPE], NULL, NULL);
// 			}
// 			break;
// 
// 		case MAGICTYPE_HOLDOBJECT:
// 			m_pMapList[caster->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
// 			if (CheckResistingMagicSuccess(caster->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
// 
// 				switch (cOwnerType)
// 				{
// 				case OWNERTYPE_PLAYER:
// 					if (m_pClientList[sOwnerH] == NULL) goto MAGIC_NOEFFECT;
// 
// 					if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] != 0) goto MAGIC_NOEFFECT;
// 
// 					if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
// 					if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(dX, dY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
// 
// 					if (caster->GetParty() && caster->GetParty() == m_pClientList[sOwnerH]->GetParty()) goto MAGIC_NOEFFECT;
// 
// 					if (m_pClientList[sOwnerH]->m_iAddPR >= 500) goto MAGIC_NOEFFECT;
// 
// 					if (caster->m_side == m_pClientList[sOwnerH]->m_side &&
// 						m_bIsCrusadeMode == false && caster->m_iAdminUserLevel == 0 &&
// 						(
// 						strcmp(m_pMapList[caster->m_cMapIndex]->m_cName, sideMap[ARESDEN]) == 0 ||
// 						strcmp(m_pMapList[caster->m_cMapIndex]->m_cName, sideMap[ELVINE]) == 0 ||
// 						//strcmp(m_pMapList[caster->m_cMapIndex]->m_cName, sideMap[ISTRIA]) == 0 || // Commented out 3rd faction xRisenx
// 						strcmp(m_pMapList[caster->m_cMapIndex]->m_cName, sideMap[NEUTRAL]) == 0 ||
// 						strcmp(m_pMapList[caster->m_cMapIndex]->m_cName, sideMapFarm[ARESDEN]) == 0 ||
// 						strcmp(m_pMapList[caster->m_cMapIndex]->m_cName, sideMapFarm[ELVINE]) == 0 //||
// 						//strcmp(m_pMapList[caster->m_cMapIndex]->m_cName, sideMapFarm[ISTRIA]) == 0 // Commented out 3rd faction xRisenx
// 						))
// 					{
// 						goto MAGIC_NOEFFECT;
// 					}
// 
// 					m_pClientList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] = (char)spell->m_sValue[MAGICV_TYPE];
// 					break;
// 
// 				case OWNERTYPE_NPC:
// 					if (m_pNpcList[sOwnerH] == NULL) goto MAGIC_NOEFFECT;
// 					if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto MAGIC_NOEFFECT;
// 					if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] != 0) goto MAGIC_NOEFFECT;
// 					m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] = (char)spell->m_sValue[MAGICV_TYPE];
// 					break;
// 				}
// 
// 				RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_HOLDOBJECT, dwTime + (spell->m_dwLastTime * 1000),
// 					sOwnerH, cOwnerType, NULL, NULL, NULL, spell->m_sValue[MAGICV_TYPE], NULL, NULL);
// 
// 				if (cOwnerType == OWNERTYPE_PLAYER)
// 					SendNotifyMsg(NULL, sOwnerH, NOTIFY_MAGICEFFECTON, MAGICTYPE_HOLDOBJECT, spell->m_sValue[MAGICV_TYPE], NULL, NULL);
// 			}
// 			break;
// 
// 		case MAGICTYPE_INVISIBILITY:
// 			switch (spell->m_sValue[MAGICV_TYPE])
// 			{
// 			case 1:
// 
// 				m_pMapList[caster->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
// 
// 				switch (cOwnerType) {
// 				case OWNERTYPE_PLAYER:
// 					if (m_pClientList[sOwnerH] == NULL) goto MAGIC_NOEFFECT;
// 					if (m_pClientList[sOwnerH]->IsInvisible()) goto MAGIC_NOEFFECT;
// 					if (m_astoria.get() && m_astoria->GetRelicHolder() == m_pClientList[sOwnerH]) goto MAGIC_NOEFFECT;
// 
// 					//	if (caster->IsNeutral()) goto MAGIC_NOEFFECT;
// 
// 					m_pClientList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_INVISIBILITY] = (char)spell->m_sValue[MAGICV_TYPE];
// 					m_pClientList[sOwnerH]->SetStatusFlag(STATUS_INVISIBILITY, true);
// 					RemoveFromTarget(sOwnerH, OWNERTYPE_PLAYER, MAGICTYPE_INVISIBILITY);
// 					break;
// 
// 				case OWNERTYPE_NPC:
// 					if (m_pNpcList[sOwnerH] == NULL) goto MAGIC_NOEFFECT;
// 					if (m_pNpcList[sOwnerH]->IsInvisible()) goto MAGIC_NOEFFECT;
// 
// 					if (m_pNpcList[sOwnerH]->m_cActionLimit == 0) {
// 						m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_INVISIBILITY] = (char)spell->m_sValue[MAGICV_TYPE];
// 						m_pNpcList[sOwnerH]->SetStatusFlag(STATUS_INVISIBILITY, true);
// 						RemoveFromTarget(sOwnerH, OWNERTYPE_NPC, MAGICTYPE_INVISIBILITY);
// 					}
// 					break;
// 				}
// 
// 				RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_INVISIBILITY, dwTime + (spell->m_dwLastTime * 1000),
// 					sOwnerH, cOwnerType, NULL, NULL, NULL, spell->m_sValue[MAGICV_TYPE], NULL, NULL);
// 
// 				if (cOwnerType == OWNERTYPE_PLAYER)
// 					SendNotifyMsg(NULL, sOwnerH, NOTIFY_MAGICEFFECTON, MAGICTYPE_INVISIBILITY, spell->m_sValue[MAGICV_TYPE], NULL, NULL);
// 				break;
// 
// 			case 2:
// 
// 				//	if (caster->IsNeutral()) goto MAGIC_NOEFFECT;
// 
// 				for (ix = dX - 8; ix <= dX + 8; ix++)//xRisenx TODO: resolution?
// 					for (iy = dY - 8; iy <= dY + 8; iy++) {
// 					m_pMapList[caster->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
// 					if (sOwnerH != NULL) {
// 						switch (cOwnerType)
// 						{
// 						case OWNERTYPE_PLAYER:
// 							if (m_pClientList[sOwnerH] == NULL) goto MAGIC_NOEFFECT;
// 
// 							if (m_pClientList[sOwnerH]->IsInvisible()) {
// 								m_pClientList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_INVISIBILITY] = NULL;
// 								m_pClientList[sOwnerH]->SetStatusFlag(STATUS_INVISIBILITY, false);
// 								RemoveFromDelayEventList(sOwnerH, cOwnerType, MAGICTYPE_INVISIBILITY);
// 							}
// 							break;
// 
// 						case OWNERTYPE_NPC:
// 							if (m_pNpcList[sOwnerH] == NULL) goto MAGIC_NOEFFECT;
// 							if (m_pNpcList[sOwnerH]->IsInvisible()) {
// 								m_pNpcList[sOwnerH]->m_cMagicEffectStatus[MAGICTYPE_INVISIBILITY] = NULL;
// 								m_pNpcList[sOwnerH]->SetStatusFlag(STATUS_INVISIBILITY, false);
// 								RemoveFromDelayEventList(sOwnerH, cOwnerType, MAGICTYPE_INVISIBILITY);
// 							}
// 							break;
// 						}
// 					}
// 					}
// 				break;
// 			}
// 			break;

		default:
			break;
		}
	}
	else {

	}
}

std::shared_ptr<client> gserver::GetClient(uint64_t ObjectID)
{
	for (std::shared_ptr<client> client_ : clients)
	{
		if (client_->handle == ObjectID)
			return client_;
	}
	return nullptr;
}

void gserver::RequestTitleHandler(std::shared_ptr<client> client_, stream_read & sr)
{
	int * ip = nullptr;
	char * cp = nullptr;
	short * sp = nullptr;

	if (client_ == nullptr) return;

/*
	sr.position += 6 + 2 + 2 + 1;

	int32_t ObjectID = sr.read_int32();
	int32_t ReturnID = sr.read_int32();

	std::shared_ptr<client> findclient = GetClient(ObjectID);

	if (findclient == nullptr) return;

	int iCrusadeJob = 0;
	if (m_bIsCrusadeMode) {
		iCrusadeJob = findclient_->crusadeDuty;
	}*/

	//SendNotifyMsg(nullptr, client_, NOTIFY_REQTITLEANSWER, findclient_->TitleType, ReturnID, iCrusadeJob, findclient_->ActiveTitle);
}

void gserver::ToggleCombatModeHandler(std::shared_ptr<client> client_)
{
	short sAppr2;

	if (client_ == nullptr) return;
	if (client_->m_bIsInitComplete == false) return;
	if (client_->dead_ == true) return;
	if (client_->skillInUse[19] == true) return;

	sAppr2 = (short)((client_->m_sAppr2 & 0xF000) >> 12);

	client_->m_bIsAttackModeChange = true;


	if (sAppr2 == 0) {
		client_->m_sAppr2 = (0xF000 | client_->m_sAppr2);
	}
	else {
		client_->m_sAppr2 = (0x0FFF & client_->m_sAppr2);
	}

	SendEventToNearClient_TypeA(client_, MSGID_MOTION_NULL, 0, 0, 0);

}

bool gserver::_bAddClientItemList(std::shared_ptr<client> client_, item * pItem, int * pDelReq)
{
	int i = 0;

	if (pItem == 0) return false;


	if ((pItem->m_cItemType == ITEMTYPE_CONSUME) || (pItem->m_cItemType == ITEMTYPE_ARROW))
	{
		if ((client_->m_iCurWeightLoad + pItem->GetWeight(pItem->m_dwCount)) > _iCalcMaxLoad(client_.get()))
			return false;
	}
	else
	{
		if ((client_->m_iCurWeightLoad + pItem->GetWeight(1)) > _iCalcMaxLoad(client_.get()))
			return false;
	}

/*
	if ((pItem->m_cItemType == ITEMTYPE_CONSUME) || (pItem->m_cItemType == ITEMTYPE_ARROW)) {
		for (i = 0; i < client_->itemList.size(); i++)
			if ( (client_->itemList[i]->_item != nullptr) && 
				(client_->itemList[i]->_item_->name == pItem->name) )
            {

					client_->itemList[i]->_item_->count += pItem->count;
					//delete pItem;
					*pDelReq = 1;

					iCalcTotalWeight(client);

					return true;
			}
	}

	for (i = 0; i < client_->itemList.size(); i++)
		if (client_->itemList[i]->_item == 0) {

			client_->itemList[i]->_item = pItem;

			client_->itemList[i]->_item_->x = 40;
			client_->itemList[i]->_item_->y = 30;

			*pDelReq = 0;

			if (pItem->m_cItemType == ITEMTYPE_ARROW)
				client_->Equipped.Arrow = _iGetArrowItemIndex(client);

			iCalcTotalWeight(client);

			return true;
		}*/

	return false;
}

int gserver::_iCalcMaxLoad(client * client_)
{
	return (client_->GetStr() * 500 + client_->level * 2000); // Changed max weight from 5 to 20 per level xRisenx
}

int gserver::iCalcTotalWeight(std::shared_ptr<client> client_)
{
	client_->UpdateWeight();

	return client_->GetWeight();
}

void gserver::DropItemHandler(std::shared_ptr<client> client_, short sItemIndex, int iAmount, string pItemName, bool bByPlayer)
{
/*
	Item * pItem;
	Client * player = client.get();
	if (!player || player->m_bIsOnWaitingProcess || !player->m_bIsInitComplete) return;

	if (sItemIndex < 0 || sItemIndex >= client_->itemList.size()) return;

	Item * &itemDrop = player->itemList[sItemIndex]->_item;
	if (!itemDrop || itemDrop->m_disabled) return;
	if (iAmount != -1 && iAmount < 0) return;

	if( (itemDrop->m_cItemType == ITEMTYPE_CONSUME || 
		itemDrop->m_cItemType == ITEMTYPE_ARROW) &&
		iAmount == -1) 
		iAmount = itemDrop->count;


	if(itemDrop->name != pItemName) return;
	if(client_->map_->iCheckItem(player->x, player->y) == ITEM_RELIC)
		return;

	if ( ( (itemDrop->m_cItemType == ITEMTYPE_CONSUME) ||
		(itemDrop->m_cItemType == ITEMTYPE_ARROW) ) &&
		(((int)itemDrop->count - iAmount) > 0) ) {
			pItem = new Item;
			if (pItem->InitItemAttr(itemDrop->name, m_pItemConfigList) == false) {

				delete pItem;
				return;
			}
			else {
				if (iAmount <= 0) {

					delete pItem;
					return;
				}
				pItem->count = iAmount;
			}


			if (iAmount > itemDrop->count) {
				delete pItem;
				return;
			}

			itemDrop->count -= iAmount;

			client_->SetItemCount(sItemIndex, itemDrop->count);

			player->map_->bSetItem(player->x, player->y, pItem);

			//_bItemLog(ITEMLOG_DROP, iClientH, (int) -1, pItem, !bByPlayer);

			//AddGroundItem(pItem, player->m_sX, player->m_sY, player->pMap, TILECLEANTIMEPLAYER);

			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, player->map,
				player->x, player->y,  
				pItem->spriteID, pItem->spriteFrame, pItem->color); 

			SendNotifyMsg(nullptr, client.get(), NOTIFY_DROPITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0);
	}
	else {

		//ReleaseItemHandler(iClientH, sItemIndex, true);

		if ( player->itemList[sItemIndex]->_item_->equipped == true)
			SendNotifyMsg(nullptr, client.get(), NOTIFY_ITEMRELEASED, itemDrop->m_cEquipPos, sItemIndex, 0);

		if ((itemDrop->itemEffectType == ITEMEFFECTTYPE_ALTERITEMDROP) && 
			(itemDrop->currentDurability == 0)) {
				delete itemDrop;
				itemDrop = nullptr;
		}
		else {
			if(itemDrop->m_sIDnum != ITEM_RELIC/ * || (m_astoria.get() && m_astoria->IsRelicGenuine(itemDrop))* /)
			{
/ *
				if(itemDrop->m_sIDnum == ITEM_RELIC)
				{
					m_astoria->PlayerDropRelic(player);		
					UpdateRelicPos();
				}
				else* /
					AddGroundItem(itemDrop, player->x, player->y, player->map, TILECLEANTIMEPLAYER);

				player->map_->bSetItem(player->x, player->y, 
					itemDrop);

				//_bItemLog(ITEMLOG_DROP, iClientH, (int) -1, itemDrop, !bByPlayer);

				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, player->map,
					player->x, player->y,  
					itemDrop->spriteID, 
					itemDrop->spriteFrame, 
					itemDrop->color);
			}/ *else{
				// delete the fake relic
				delete itemDrop;
			}* /
		}

		itemDrop = 0;
		player->itemList[sItemIndex]->_item_->equipped = false;

		SendNotifyMsg(0, player, NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0);

		player->Equipped.Arrow = _iGetArrowItemIndex(player);
	}

	iCalcTotalWeight(client.get());
	SendNotifyMsg(0, player, NOTIFY_REQDEF, player->m_iDefenseRatio, 0, 0);*/
}

void gserver::AddGroundItem(item * pItem, uint16_t x, uint16_t y, map * mapIndex, uint64_t dwTime)
{//TODO: move to Map class and revise a bit
// 	for(int i = 0; i < MAXGROUNDITEMS; i++)
// 	{
// 		if(m_stGroundNpcItem[i].bEmpty)
// 		{
// 			m_stGroundNpcItem[i].bEmpty = false;
// 			m_stGroundNpcItem[i].dropTime = utility::current_time() + dwTime;
// 			m_stGroundNpcItem[i].item = pItem;
// 			m_stGroundNpcItem[i].sx = x;
// 			m_stGroundNpcItem[i].sy = y;
// 			m_stGroundNpcItem[i].cMapIndex = mapIndex;
// 			return;
// 		}
// 	}
// 	PutLogList("WARNING: Ground NPC item list is full, item will not be cleared.");
}

void gserver::SendItemNotifyMsg(std::shared_ptr<client> client_, uint16_t msgtype, item *pItem, int iV1)
{
	stream_write sw;

	sw.write_int32(MSGID_NOTIFY);
	sw.write_int16(msgtype);

	switch (msgtype)
	{
	case NOTIFY_ITEMOBTAINED:
		WriteItemData(sw, pItem);

		sw.write_int8((char)pItem->m_sSpecialEffectValue2);
		sw.write_int32(pItem->m_dwAttribute);

		for(int j = 0; j < MAXITEMSOCKETS; j++)
		{
			sw.write_int8(pItem->m_sockets[j]);
		}

		client_->write(sw);
		break;

	case NOTIFY_ITEMPURCHASED:
		WriteItemData(sw, pItem);

		sw.write_int16(iV1); // (iCost - iDiscountCost);

		client_->write(sw);
		break;

	case NOTIFY_CANNOTCARRYMOREITEM:
		client_->write(sw);
		break;
	}
}

void gserver::WriteItemData(stream_write & sw, item * pItem) const
{
	sw.write_string(pItem->name, 20);
	sw.write_int32(pItem->m_dwCount);
	sw.write_int8(pItem->m_cItemType);
	sw.write_int8(pItem->m_cEquipPos);
	sw.write_int8(0);
	sw.write_int16(pItem->m_sLevelLimit);
	sw.write_int8(pItem->m_cGenderLimit);
	sw.write_int16(pItem->m_wCurLifeSpan);
	sw.write_int16(pItem->GetWeight());
	sw.write_int16(pItem->m_sSprite);
	sw.write_int16( pItem->m_sSpriteFrame);
	sw.write_int32(pItem->m_cItemColor);
}

void gserver::ItemDepleteHandler(std::shared_ptr<client> client_, item * pItem, bool bIsUseItemResult, bool bIsLog /*= true*/, bool notify /*= true*/)
{
	if (client_->m_bIsInitComplete == false) return;

/*
	if ((bIsLog == true) && (client_->m_pItemList[sItemIndex]->_item_->m_cItemType != ITEMTYPE_CONSUME) 
		&& (client_->m_pItemList[sItemIndex]->_item_->m_cItemType != ITEMTYPE_EAT) 
		&& (client_->m_pItemList[sItemIndex]->_item_->m_cItemType != ITEMTYPE_USE_DEPLETE) 
		&& (client_->m_pItemList[sItemIndex]->_item_->m_cItemType != ITEMTYPE_USE_DEPLETE_DEST)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_cItemType != ITEMTYPE_MATERIAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_ISMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_SCANMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_MHMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_MSMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_COTGMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_SOTGMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_MBMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_FSWMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_LCMANUAL)
		&& (client_->m_pItemList[sItemIndex]->_item_->m_sIDnum != ITEM_BSWMANUAL))
	{
	//	_bItemLog(ITEMLOG_DEPLETE, client,(int) -1, client_->m_pItemList[sItemIndex]);
	}*/

	//TODO: need to unequip the item if it's equipped
	//ReleaseItemHandler(client_, sItemIndex, true);

	if(notify)
		SendNotifyMsg(nullptr, client_, NOTIFY_ITEMDEPLETED_ERASEITEM, pItem->m_slot, (int)bIsUseItemResult, 0);

/*
	for (ItemWrap * pitemw : client_->itemList)
	{
		if (pitemw->_item == pItem)
		{
			pitemw->DeleteItem();
			break;
		}
	}*/

	//client_->m_pItemList[sItemIndex]->_item_->equipped = false;

	// !!! BUG POINT

	//client_->Equipped.Arrow = _iGetArrowItemIndex(client);

	iCalcTotalWeight(client_);
}

bool gserver::WriteTileData(stream_write & sw, std::shared_ptr<client> player, uint16_t ix, uint16_t iy) {
	unsigned char ucHeader = 0;

	tile * pTile = player->map_->GetTile(ix, iy);

    if (pTile->owner || pTile->deadowner || !pTile->items.empty() || pTile->m_sDynamicObjectType!= 0) {
        sw.write_int16(ix);
        sw.write_int16(iy);

        if (pTile->owner) {
            ucHeader = ucHeader | 0x01;
        }
        if (pTile->deadowner) {
            ucHeader = ucHeader | 0x02;
        }
        if (!pTile->items.empty()) {
            ucHeader = ucHeader | 0x04;
        }
        if (pTile->m_sDynamicObjectType != 0) {
            ucHeader = ucHeader | 0x08;
        }

        if ((ucHeader & 0x01) != 0 || (ucHeader & 0x02) != 0)
		{
			std::shared_ptr<unit> owner;

            if ((ucHeader & 0x01) != 0)
			{
                owner = pTile->owner;
            }

            if ((ucHeader & 0x02) != 0)
			{
                owner = pTile->deadowner;
            }

            switch (pTile->m_cOwnerType) {
                case ot_player:

                    sw.write_int64(owner->handle);
                    sw.write_int8(owner->get_owner_type());
                    sw.write_int16(owner->get_type());
                    sw.write_int8(owner->direction);
                    sw.write_int16(owner->appr1);
                    sw.write_int16(owner->appr2);
                    sw.write_int16(owner->appr3);
                    sw.write_int16(owner->appr4);
                    sw.write_int32(owner->appr_color);
                    sw.write_int16(owner->head_appr);
                    sw.write_int16(owner->body_appr);
                    sw.write_int16(owner->arm_appr);
                    sw.write_int16(owner->leg_appr);

                    if (_bGetIsPlayerHostile(player, std::static_pointer_cast<client>(owner))) {
                        sw.write_int32(STATUS_ENEMYFLAGS & owner->status);
                    }
                    else {
                        sw.write_int32(owner->status);
                    }
                    sw.write_string(owner->name, 10);
                    break;
                case ot_npc:
                    sw.write_int64(owner->handle);
                    sw.write_int8(owner->get_owner_type());
                    sw.write_int16(owner->get_type());
                    sw.write_int8(owner->direction);
                    sw.write_int16(owner->appr2);
                    sw.write_int32(owner->status);
                    break;
            }
        }

        if (!pTile->items.empty()) {
            sw.write_int16(pTile->items[0]->m_sSprite);
            sw.write_int16(pTile->items[0]->m_sSpriteFrame);
            sw.write_int32(pTile->items[0]->m_cItemColor);
        }

        if (pTile->m_sDynamicObjectType != 0) {
            sw.write_int16(pTile->m_wDynamicObjectID);
            sw.write_int16(pTile->m_sDynamicObjectType);
        }

        return true;
    }
    return false;
}

void gserver::RequestFullObjectData(std::shared_ptr<client> client_, std::shared_ptr<unit> target)
{
	uint32_t temp;

	if (client_ == 0) return;
	if (client_->m_bIsInitComplete == false) return;

	if (target != 0)
	{
		if (target->is_player())
		{
			stream_write sw(50);

			std::shared_ptr<client> object = std::static_pointer_cast<client>(target);

			sw.write_int32(MSGID_MOTION_STOP);
            sw.write_int16(uint16_t(object->handle));
            sw.write_int8(object->get_owner_type());
            sw.write_int16(object->x);
			sw.write_int16(object->y);
			sw.write_int16(object->get_type());
			sw.write_int8(object->direction);
			sw.write_string(object->name, 10);
			sw.write_int16(object->m_sAppr1);
			sw.write_int16(object->m_sAppr2);
			sw.write_int16(object->m_sAppr3);
			sw.write_int16(object->m_sAppr4);
			sw.write_int32(object->m_iApprColor);
			sw.write_int16(object->m_sHeadApprValue);
			sw.write_int16(object->m_sBodyApprValue);
			sw.write_int16(object->m_sArmApprValue);
			sw.write_int16(object->m_sLegApprValue);


			temp = object->status;
			if(_bGetIsPlayerHostile(client_, object) && client_->m_iAdminUserLevel == 0)
				temp &= STATUS_ENEMYFLAGS;

			sw.write_int32(temp);
			sw.write_int8(uint8_t(object->dead_ ? true : false));

			client_->write(sw);
		}
		else
		{
            if (target->handle == 0) return;

            std::shared_ptr<npc> object = std::static_pointer_cast<npc>(target);
			if(!object) 
				return;

			stream_write sw(50);

			sw.write_int32(MSGID_MOTION_STOP);
            sw.write_int16(uint16_t(object->handle + 10000));
            sw.write_int8(object->get_owner_type());
            sw.write_int16(object->x);
			sw.write_int16(object->y);
			sw.write_int16(object->get_type());
			sw.write_int8(object->direction);
			sw.write_int16(object->appr2);
			sw.write_int32(object->status);
			sw.write_int8(uint8_t(object->dead_ ? true : false));

			client_->write(sw);
		}
	}
}

bool gserver::_bGetIsPlayerHostile(std::shared_ptr<client> player, std::shared_ptr<client> target)
{
	if (player) return false;
	if (target) return false;

	if (player == target) return false;

	if (player->is_neutral()) {
		if (target->playerKillCount != 0) 
			return true;
		else return false;
	}
	else {
		if (player->side != target->side) {
			return true;
		}
		else {
			if (target->playerKillCount != 0) 
				return true;
			else return false;
		}
	}

	return false;
}

// bool gserver::LoadCharacterData(std::shared_ptr<client> client_)
// {
// 
// 
//     client_->handle = rs.value("id").convert<uint64_t>();
// 	client_->mapName = rs.value("maploc").convert<string>();
// 	client_->x = rs.value("locx").convert<int16_t>();
// 	client_->y = rs.value("locy").convert<int16_t>();
// 	client_->gender = rs.value("gender").convert<uint8_t>();
// 	client_->colorSkin = rs.value("skin").convert<uint8_t>();
// 	client_->m_cHairStyle = rs.value("hairstyle").convert<uint8_t>();
// 	client_->colorHair = rs.value("haircolor").convert<uint32_t>();
// 	client_->colorUnderwear = rs.value("underwear").convert<uint32_t>();
// 	string temp = rs.value("magicmastery").convert<string>();
// 	for (int i = 0; i < temp.length(); ++i)
// 	{
// 		client_->magicMastery[i] = (temp[i] == '1') ? 1 : 0;
// 	}
// 	//memcpy(client_->m_cMagicMastery, rs.value("magicmastery").convert<string>().c_str(), rs.value("magicmastery").convert<string>().length());//fix. each should be 0x00, not (char)'0'
// 	string side = rs.value("nation").convert<string>();
// 	/*if (side == "NONE")
// 		side = Side::NEUTRAL;
// 		else if (side == "Aresden")
// 		side = Side::ARESDEN;
// 		else if (side == "Elvine")
// 		side = Side::ELVINE;*/
// 	client_->nation = side;
// 	//client_->m_cMapName = rs.value("BlockDate").convert<string>();
// 	client_->lockedMapName = rs.value("lockmapname").convert<string>();
// 	client_->lockedMapTime = rs.value("lockmaptime").convert<uint64_t>();
// 	client_->profile = rs.value("profile").convert<string>();
// 	client_->health = rs.value("hp").convert<uint64_t>();
// 	client_->mana = rs.value("mp").convert<uint64_t>();
// 	client_->stamina = rs.value("sp").convert<uint64_t>();
// 	client_->enemyKillCount = rs.value("ek").convert<int32_t>();
// 	client_->playerKillCount = rs.value("pk").convert<int32_t>();
// 	client_->level = rs.value("level").convert<uint32_t>();
// 	client_->experience = rs.value("exp").convert<uint64_t>();
// 	client_->SetStr(rs.value("strength").convert<int32_t>(), false);
// 	client_->SetVit(rs.value("vitality").convert<int32_t>());
// 	client_->SetDex(rs.value("dexterity").convert<int32_t>());
// 	client_->SetInt(rs.value("intelligence").convert<int32_t>(), false);
// 	client_->SetMag(rs.value("magic").convert<int32_t>());
// 	client_->SetChr(rs.value("charisma").convert<int32_t>());
// 	client_->m_iLuck = rs.value("luck").convert<uint16_t>();
// 	client_->m_iRewardGold = rs.value("rewardgold").convert<int32_t>();
// 	client_->m_iHungerStatus = rs.value("hunger").convert<int32_t>();
// 	client_->m_iAdminUserLevel = rs.value("adminlevel").convert<int32_t>();
// 	client_->m_iTimeLeft_ShutUp = rs.value("leftshutuptime").convert<uint64_t>();
// 	client_->m_iTimeLeft_Rating = rs.value("leftpoptime").convert<uint64_t>();
// 	client_->m_reputation = rs.value("popularity").convert<int32_t>();
// 	//client_->guild = rs.value("guildid").convert<uint64_t>();//TODO: guild search
// 	//client_->m_iDownSkillIndex = rs.value("DownSkillID").convert<int16_t>();//use?
// 	client_->charid = rs.value("id").convert<uint64_t>();
// 	client_->m_sCharIDnum1 = rs.value("id1").convert<int16_t>();
// 	client_->m_sCharIDnum2 = rs.value("id2").convert<int16_t>();
// 	client_->m_sCharIDnum3 = rs.value("id3").convert<int16_t>();
// 	client_->m_iQuest = rs.value("questnum").convert<int32_t>();
// 	client_->m_iCurQuestCount = rs.value("questcount").convert<int32_t>();
// 	client_->m_iQuestRewardType = rs.value("questrewardtype").convert<int32_t>();
// 	client_->m_iQuestRewardAmount = rs.value("questrewardamount").convert<int32_t>();
// 	client_->contribution = rs.value("contribution").convert<int32_t>();
// 	client_->m_iQuestID = rs.value("questid").convert<int32_t>();
// 	client_->m_bIsQuestCompleted = rs.value("questcompleted").convert<bool>();
// 	client_->m_iTimeLeft_ForceRecall = rs.value("leftforcerecalltime").convert<uint64_t>();
// 	client_->m_iTimeLeft_FirmStamina = rs.value("leftfirmstaminatime").convert<uint64_t>();
// 	client_->m_iSpecialEventID = rs.value("eventid").convert<int32_t>();
// 	client_->superAttack = rs.value("leftsac").convert<int32_t>();
// 	client_->arenaNumber = rs.value("fightnum").convert<int32_t>();
// 	client_->arenaReserveTime = rs.value("fightdate").convert<uint64_t>();
// 	client_->arenaTicketNumber = rs.value("fightticket").convert<int32_t>();
// 	client_->m_iSpecialAbilityTime = rs.value("leftspectime").convert<uint64_t>();
// 	client_->crusadeContribution = rs.value("warcon").convert<int32_t>();
// 	client_->crusadeDuty = rs.value("crujob").convert<int32_t>();
// 	client_->crusadePoint = rs.value("cruconstructpoint").convert<int32_t>();
// 	client_->crusadeGUID = rs.value("cruid").convert<uint64_t>();
// 	client_->deadPenaltyTime = rs.value("leftdeadpenaltytime").convert<uint64_t>();
// 	uint64_t partyid = rs.value("partyid").convert<uint64_t>();
// 	// 					if (partyid && partyMgr.PartyExists(partyid))
// 	// 					{
// 	// 						client_->SetParty(partyMgr.GetParty(partyid));
// 	// 
// 	// 					}
// 	client_->m_iGizonItemUpgradeLeft = rs.value("gizonitemupgradeleft").convert<int32_t>();
// 	client_->m_elo = rs.value("elo").convert<int32_t>();
// 	client_->m_iEnemyKillTotalCount = rs.value("totalek").convert<int32_t>();
// 	//client_->m_iLucky = rs.value("Lucky").convert<uint32_t>();//not in DB
// 	//client_->m_iMonsterCount = rs.value("MonsterCount").convert<uint32_t>();//not in DB
// 
// 
// 	if (client_->gender == MALE) client_->SetType(1);
// 	else if (client_->gender == FEMALE) client_->SetType(4);
// 	client_->SetType(client_->get_type() + client_->colorSkin - 1);
// 	client_->m_sAppr1 = (client_->m_cHairStyle << 8) | (client_->colorHair << 4) | client_->colorUnderwear;
// }
// 		
// 
// {
// 	Session ses(sqlpool->get());
// 	Statement select(ses);
// 	select << "SELECT currency_id,count FROM char_currency WHERE char_id=?;", use(client_->charid), now;
// 	RecordSet rs(select);
// 
// 	uint32_t rowcount = rs.rowCount();
// 
// 	if (rowcount > 0)
// 	{
// 		rs.moveFirst();
// 		Client::stCurrency currency;
// 
//         do
// 		{
// 			currency._id = rs.value("currency_id").convert<int64_t>();
// 			currency._count = rs.value("count").convert<int64_t>();
// 			client_->currency.push_back(currency);
//         } while (rs.moveNext());
//     }
// }
// 
// {
//     Session ses(sqlpool->get());
//     Statement select(ses);
//     select << "SELECT * FROM item_bag WHERE char_id=?;", use(client_->charid), now;
//     RecordSet rs(select);
// 
//     uint32_t rowcount = rs.rowCount();
// 
//     if (rowcount > 0)
//     {
//         rs.moveFirst();
// 
//         do
//         {
//             shared_ptr<ItemBag> ib = boost::make_shared<ItemBag>();
//             ib->itemId = rs.value("item_id").convert<uint64_t>();
//             ib->slot = rs.value("bag_slot").convert<uint16_t>();
//             ib->unique_id = rs.value("uid").convert<uint64_t>();
//             ib->maxItems = rs.value("size").convert<uint64_t>();
//             client_->itemBagList.push_back(ib);
//         } while (rs.moveNext());
//     }
// }
// 
// {
//     Session ses(sqlpool->get());
//     Statement select(ses);
//     select << "SELECT * FROM item WHERE char_id=?;", use(client_->charid), now;
//     RecordSet rs(select);
// 
//     uint32_t rowcount = rs.rowCount();
// 
//     if (rowcount > 0)
//     {
//         rs.moveFirst();
// 
//         do
//         {
//             std::map<uint8_t, shared_ptr<ItemBag>> ibmap;
// 
//             std::shared_ptr<item> item_ = boost::make_shared<item>();
//             item_->uid = rs.value("item_id").convert<int64_t>();
//             item_->name = rs.value("name").convert<string>();
//             item_->count = rs.value("count").convert<uint32_t>();
//             item_->m_cItemType = rs.value("type").convert<int8_t>();
//             item_->color = rs.value("color").convert<uint32_t>();
//             item_->effectV1 = rs.value("effect1").convert<int16_t>();
//             item_->effectV2 = rs.value("effect2").convert<int16_t>();
//             item_->effectV3 = rs.value("effect3").convert<int16_t>();
//             item_->currentDurability = rs.value("durability").convert<int32_t>();
//             item_->m_dwAttribute = rs.value("attribute").convert<uint32_t>();
//             item_->m_sockets[0] = rs.value("socket1").convert<uint8_t>();
//             item_->m_sockets[1] = rs.value("socket2").convert<uint8_t>();
//             item_->m_sockets[2] = rs.value("socket3").convert<uint8_t>();
//             item_->m_sockets[3] = rs.value("socket4").convert<uint8_t>();
//             item_->equipped = rs.value("equipped").convert<int64_t>();
//             item_->initialized = true;
//             int bagid = rs.value("bag_id").convert<int32_t>();
//             if (item_->equipped > 0)
//             {
//                 //item is equipped
//                 client_->equippedItems[item_->equipped] = item;
//             }
//             else
//             {
//                 if (client_->itemBagList[bagid] != nullptr)
//                 {
//                     client_->itemBagList[bagid]->itemList.push_back(item);
//                 }
//                 else
//                 {
//                     //TODO: catch this
//                 }
//             }
// return true;
// }

bool gserver::bCheckClientAttackFrequency(std::shared_ptr<client> client_)
{
	uint64_t dwTimeGap;
	uint64_t dwTime = utility::current_time();

	if (client_ == nullptr) return false;

	if (client_->IsGM())
		return true;

	if (client_->m_dwAttackFreqTime == 0)
		client_->m_dwAttackFreqTime = dwTime;
	else {
		dwTimeGap = dwTime - client_->m_dwAttackFreqTime;
		client_->m_dwAttackFreqTime = dwTime;

// #ifndef NO_MSGSPEEDCHECK_ATTACK
// 		if (dwTimeGap < 320) {
// 			log->info(Poco::format("(!) Speed hack suspect(%s) - attack(%?d)", client_name, dwTimeGap));
// 			return false;
// 		}
// 		else if (dwTimeGap < 240) {
// 			log->info(Poco::format("(!) Speed hack suspect(%s) - attack(%?d). Disconnected", client_name, dwTimeGap));
// 			DeleteClient(client_->shared_from_this(), true, true);
// 			return false;
// 		}
// #endif
	}

	return true;
}

//TODO: not all magic will have a cast time
bool gserver::bCheckClientMagicFrequency(client * client_)
{
	uint64_t dwTimeGap;
	uint64_t dwTime = utility::current_time();

	if (client_ == nullptr) return false;

	if (client_->IsGM())
		return true;

	if (client_->m_dwMagicFreqTime == 0)
		client_->m_dwMagicFreqTime = dwTime;
	else {
		dwTimeGap = dwTime - client_->m_dwMagicFreqTime;
		client_->m_dwMagicFreqTime = dwTime;

#ifndef NO_MSGSPEEDCHECK_MAGIC
// 		if (dwTimeGap < 1200) {
// 			log->info(Poco::format("(-~-HACKING-~-) Speed hacker detected(%s) - magic(%?d). Disconnected", client_name, dwTimeGap));
// 			DeleteClient(client_->shared_from_this(), true, true);
// 			return false;
// 		}
// 		else if (dwTimeGap < 1600) {
// 			log->info(Poco::format("(-~-HACKING-~-) Speed hack suspect(%s) - magic(%?d)", client_name, dwTimeGap));
// 			return false;
// 		}
#endif
	}

	return true;
}

bool gserver::bCheckClientMoveFrequency(std::shared_ptr<client> client_, bool running)
{
	uint64_t dwTimeGap;
	uint64_t dwTime = utility::current_time();

	if (!client_) return false;

	if (client_->IsGM())
		return true;

	if (client_->m_dwMoveFreqTime == 0)
		client_->m_dwMoveFreqTime = dwTime;
	else {
		if (client_->m_resetMoveFreq == true) {
			client_->m_dwMoveFreqTime = 0;
			client_->m_resetMoveFreq = false;
			return true;
		}

		if (client_->m_bIsAttackModeChange == true) {
			client_->m_dwMoveFreqTime = 0;
			client_->m_bIsAttackModeChange = false;
			return true;
		}

		dwTimeGap = dwTime - client_->m_dwMoveFreqTime;
		client_->m_dwMoveFreqTime = dwTime;

		if (running)
		{
			if (dwTimeGap < 30) dwTimeGap = 300;
			client_->m_runTime[client_->m_runTurn] = dwTimeGap;

// #ifdef DOSPEEDCHECK
// 			if (client_->m_iAdminUserLevel == 0) {
// 				uint64_t sum = 0;
// 				for (int i = 0; i < SPEEDCHECKTURNS; i++)
// 					sum += client_->m_runTime[i];
// 
// 				if (sum < 210 * SPEEDCHECKTURNS)
// 				{
// 					log->info(Poco::format("(-~-HACKING-~-) Speed hacker detected(%s) - run-avg(%?d). BI banned", client_name, sum / SPEEDCHECKTURNS));
// 
// 					DeleteClient(client_->shared_from_this(), true, true);
// 					return false;
// 				}
// 				else if (sum < 230 * SPEEDCHECKTURNS)
// 				{
// 					log->info(Poco::format("(-~-HACKING-~-) Speed hack suspect(%s) - run-avg(%?d)", client_name, sum / SPEEDCHECKTURNS));
// 
// 					++client_->m_runTurn %= SPEEDCHECKTURNS;
// 					return false;
// 				}
// 				++client_->m_runTurn %= SPEEDCHECKTURNS;
// 			}
// #endif
		}
		else{
			if (dwTimeGap < 30) dwTimeGap = 540;
			client_->m_moveTime[client_->m_moveTurn] = dwTimeGap;
// #ifdef DOSPEEDCHECK
// 			if (client_->m_iAdminUserLevel == 0) {
// 				uint64_t sum = 0;
// 				for (int i = 0; i < SPEEDCHECKTURNS; i++)
// 					sum += client_->m_moveTime[i];
// 
// 				/*if(sum < 330*SPEEDCHECKTURNS)
// 				{
// 				wsprintf(g_cTxt, "(-~-HACKING-~-) Speed hacker detected(%s) - move-avg(%i). BI banned", player->m_cCharName, sum/SPEEDCHECKTURNS);
// 				PutLogList(g_cTxt);
// 
// 				DeleteClient(iClientH, true, true, true, true);
// 				return false;
// 				}else */if (sum < 350 * SPEEDCHECKTURNS)
// 				{
// 					log->info(Poco::format("(-~-HACKING-~-) Speed hack suspect(%s) - move-avg(%?d)", client_name, sum / SPEEDCHECKTURNS));
// 
// 					++client_->m_moveTurn %= SPEEDCHECKTURNS;
// 					return false;
// 				}
// 				++client_->m_moveTurn %= SPEEDCHECKTURNS;
// 			}
// #endif
		}
	}

	return true;
}

void gserver::_CheckAttackType(client * client_, int16_t & spType)
{
	uint16_t wType;

	if (client_ == nullptr) return;
	wType = ((client_->m_sAppr2 & 0x0FF0) >> 4);

	switch (spType) {
	case 2:
		if (client_->Equipped.Arrow.initialized) spType = 0;
		if (wType < 40) spType = 1;
		break;

	case 20:
		if (client_->superAttack <= 0)  spType = 1;
		if (client_->skillMastery[SKILL_HANDATTACK] < 100) spType = 1;
		break;

	case 21:

		if (client_->superAttack <= 0)  spType = 1;
		if (client_->skillMastery[SKILL_SHORTSWORD] < 100) spType = 1;
		break;

	case 22:

		if (client_->superAttack <= 0)  spType = 1;
		if (client_->skillMastery[SKILL_FENCING] < 100) spType = 1;
		break;

	case 23:
		if (client_->superAttack <= 0)   spType = 1;
		if (client_->skillMastery[SKILL_LONGSWORD] < 100) spType = 1;
		break;

	case 24:
		if (client_->superAttack <= 0)  spType = 1;
		if (client_->skillMastery[SKILL_AXE] < 100) spType = 1;
		break;

	case 25:
		if (client_->superAttack <= 0)  spType = 2;
		if (client_->skillMastery[SKILL_ARCHERY] < 100) spType = 2;
		if (client_->Equipped.Arrow.initialized)      spType = 0;
		if (wType < 40) spType = 1;
		break;
	case 26:

		if (client_->superAttack <= 0)  spType = 1;
		if (client_->skillMastery[SKILL_HAMMER] < 100) spType = 1;
		break;

	case 27:

		if (client_->superAttack <= 0)  spType = 1;
		if (client_->skillMastery[SKILL_STAFF] < 100) spType = 1;
		break;
	}
}

void gserver::ClearSkillUsingStatus(std::shared_ptr<client> client_)
{
	int i;

	if (client_ == nullptr) return;

	//m_pClientList[iClientH]->m_hasPrecasted = false;
	for (i = 0; i < MAXSKILLTYPE; i++) {
		client_->skillInUse[i] = false;
		client_->skillInUseTime[i] = 0;
	}

	//TODO: fishing
// 	if (client_->m_iAllocatedFish != 0) {
// 		if (m_pFish[client_->m_iAllocatedFish] != 0)
// 			m_pFish[client_->m_iAllocatedFish]->m_sEngagingCount--;
// 
// 		client_->m_iAllocatedFish = NULL;
// 		SendNotifyMsg(nullptr, client_, NOTIFY_FISHCANCELED, 0, 0, 0);
// 	}
}

void gserver::UseItemHandler(client * client_, short sItemIndex, short dX, short dY, short sDestItemID)
{
/*
	int iTemp, iMax, iV1, iV2, iV3, iSEV1, iEffectResult = 0;
	uint64_t dwTime, dwGUID;
	short sTemp, sTmpType, sTmpAppr1;
	bool depleteItem = true;
	dwTime = utility::current_time();

	if (!client || client_->is_dead() || !client_->m_bIsInitComplete) return;

	if (sItemIndex < 0 || sItemIndex >= MAXITEMS) return;

	Item * item = client_->itemList[sItemIndex]->_item;

	if (!item || item_->m_disabled) return;

	if ((item_->m_cItemType == ITEMTYPE_USE_DEPLETE) ||
		(item_->m_cItemType == ITEMTYPE_USE_PERM) ||
		(item_->m_cItemType == ITEMTYPE_ARROW) ||
		(item_->m_cItemType == ITEMTYPE_EAT) ||
		(item_->m_cItemType == ITEMTYPE_USE_SKILL) ||
		(item_->m_cItemType == ITEMTYPE_USE_DEPLETE_DEST)) {
	}
	else return;

	if ((item_->m_cItemType == ITEMTYPE_USE_DEPLETE) || (item_->m_cItemType == ITEMTYPE_EAT))
	{
		switch (item_->itemEffectType)
		{
		case ITEMEFFECTTYPE_HPSTOCK:
			iV1 = item_->itemEffectV1;
			iV2 = item_->itemEffectV2;
			iV3 = item_->itemEffectV3;

			client_->m_iHPstock += utility::gen_rand(iV1, iV2) + iV3;
			if (client_->m_iHPstock < 0)   client_->m_iHPstock = 0;
			if (client_->m_iHPstock > 500) client_->m_iHPstock = 500;

			client_->m_iHungerStatus += utility::gen_rand(iV1, iV2) + iV3;
			if (client_->m_iHungerStatus > 100) client_->m_iHungerStatus = 100;
			if (client_->m_iHungerStatus < 0)   client_->m_iHungerStatus = 0;
			break;
		}

		if (depleteItem)
			ItemDepleteHandler(client_, item, true);
		else
			SendNotifyMsg(nullptr, client_, NOTIFY_SETITEMCOUNT, sItemIndex, item_->count, true);

		switch (iEffectResult) {
		case 1:
			SendNotifyMsg(nullptr, client_, NOTIFY_HP);
			break;
		case 2:
			SendNotifyMsg(nullptr, client_, NOTIFY_MP);
			break;
		case 3:
			SendNotifyMsg(nullptr, client_, NOTIFY_SP);
			break;
		case 4: // Invincible
			SendNotifyMsg(nullptr, client_, NOTIFY_SLATE_INVINCIBLE);
			break;
		case 5: // Mana
			SendNotifyMsg(nullptr, client_, NOTIFY_SLATE_MANA);
			break;
		case 6: // EXP
			SendNotifyMsg(nullptr, client_, NOTIFY_SLATE_EXP);
			break;
		case 7: // Added Faster Criticals
			SendNotifyMsg(nullptr, client_, NOTIFY_SUPERATTACKLEFT);
			break;
		default:
			break;
		}
	}*/

}

void gserver::Effect_Damage_Spot(std::shared_ptr<unit> attacker, std::shared_ptr<unit> target, short sV1, short sV2, short sV3, bool exp, Element element /*= ELEMENT_NONE*/, magic * spell /*= nullptr*/)
{
	int32_t damage = utility::gen_rand(sV1, sV2) + sV3;
	int64_t dwtime = utility::current_time();
	if (damage < 0) damage = 0;

	if (attacker->is_dead() || target->is_dead()) return;

	if (attacker->is_player())
	{
		std::shared_ptr<client> player = std::static_pointer_cast<client>(attacker);
		if (!m_bIsCrusadeMode && player->civilian && target->is_player()) return;//no damage to civilians outside of crusade (default)
		//TODO: player damage calculations here
	}
	else
	{
		//npc
		//TODO: for sake of testing

	}

	if (target->is_player())
	{
        std::shared_ptr<client> ptarget = std::static_pointer_cast<client>(target);
		if (!ptarget->m_bIsInitComplete) return; //cannot attack players not fully loaded in
		if (ptarget->is_dead()) return;

		if (ptarget->status & STATUS_REDSLATE) return;//invincibility
	
		//players can only kill a non-hostile during crusade or if they are a criminal
		if (attacker->is_player())
		{
            std::shared_ptr<client> pattacker = std::static_pointer_cast<client>(attacker);
			if (ptarget->playerKillCount == 0)//target is not a criminal
			{
				if (!m_bIsCrusadeMode && ptarget->civilian) return;//crusade mode is off and you are a civilian aka protected from PK
				if (pattacker->is_neutral()) return;//attacker has no town affiliation
			}
			if (pattacker->safeAttackMode)
			{
				int iSideCondition = pattacker->GetPlayerRelationship(ptarget);
				if (iSideCondition != 7 && iSideCondition != 2 && iSideCondition != 6)
				{
					if (attacker->map_->flags.fightZone)
					{
                        if (pattacker->guild_ == ptarget->guild_)
							return;
					}
				}
			}
			if (ptarget->map_->iGetAttribute(target->x, target->y, 0x00000005) != 0) return;
		}

		ClearSkillUsingStatus(ptarget);

		//TODO: elemental absorption code that was never actually fully implemented
		/*switch (attr)
		{
		case 1:
			if (m_pClientList[sTargetH]->m_iAddAbsEarth != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsEarth;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;

		case 2:
			if (m_pClientList[sTargetH]->m_iAddAbsAir != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsAir;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;

		case 3:
			if (m_pClientList[sTargetH]->m_iAddAbsFire != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsFire;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;

		case 4:
			if (m_pClientList[sTargetH]->m_iAddAbsWater != 0) {
				dTmp1 = (double)iDamage;
				dTmp2 = (double)m_pClientList[sTargetH]->m_iAddAbsWater;
				dTmp3 = (dTmp2 / 100.0f)*dTmp1;
				iDamage = iDamage - (int)(dTmp3);
				if (iDamage < 0) iDamage = 0;
			}
			break;
		}*/

		damage -= utility::gen_rand(1, ptarget->GetVit() / 10) - 1;
		if (damage < 0) damage = 0;

		ptarget->health -= damage;

		if (attacker && attacker->is_player())
		{
			SendNotifyMsg(nullptr, std::static_pointer_cast<client>(attacker), NOTIFY_NOTICEMSG, 0, 0, 0, fmt::format("You did ({}) to character ({})", damage, ptarget->name));
		}

		ptarget->m_lastDamageTime = dwtime;
		if (ptarget->health <= 0)
		{
			ptarget->KilledHandler(attacker, damage);
		}
		else
		{
			if (damage > 0)
			{
				SendNotifyMsg(nullptr, ptarget, NOTIFY_HP, 0, 0, 0);
				SendEventToNearClient_TypeA(ptarget, MSGID_MOTION_DAMAGE, damage, 0, 0);
			}
		}
	}
	else
	{
	}
}

void gserver::Effect_HpUp_Spot(std::shared_ptr<unit> caster, std::shared_ptr<unit> target, short sV1, short sV2, short sV3)
{
	uint64_t time = utility::current_time();

	if (caster == nullptr || target == nullptr)
		return;

	uint32_t hp = utility::gen_rand(sV1, sV2) + sV3;

	if (caster->is_dead() || target->is_dead()) return;

	if (target->is_player())
	{
		std::shared_ptr<client> ptarget = std::static_pointer_cast<client>(target);

        //int maxhp = (3 * player->GetVit()) + (2 * player->level) + (player->GetStr() / 2);//official
		int maxhp = 8 * (ptarget->GetVit() + ptarget->level) + 2 * (ptarget->GetInt() + ptarget->GetStr());//"new"

		if (ptarget->m_iSideEffect_MaxHPdown != 0)
			maxhp = maxhp - (maxhp / ptarget->m_iSideEffect_MaxHPdown);

		if (ptarget->health < maxhp)
			ptarget->health += hp;

		if (ptarget->health > maxhp) ptarget->health = maxhp;
		if (ptarget->health <= 0) ptarget->health = 1;

		SendNotifyMsg(nullptr, ptarget, NOTIFY_HP, 0, 0, 0);
	}
	else if (target->is_npc())
	{
        std::shared_ptr<npc> ntarget = std::static_pointer_cast<npc>(target);

		if (ntarget->health <= 0) return;

		int maxhp = (ntarget->m_iHitDice * 4);

		if (ntarget->health < maxhp)
			ntarget->health += hp;

		if (ntarget->health > maxhp) ntarget->health = maxhp;
		if (ntarget->health <= 0) ntarget->health = 1;
	}
}

void gserver::Effect_SpDown_Spot(std::shared_ptr<unit> caster, std::shared_ptr<unit> target, short sV1, short sV2, short sV3)
{
	int iSP, iMaxSP, iSideCondition;

	if (caster == nullptr || target == nullptr)
		return;

	if (caster->is_dead() || target->is_dead()) return;

	iSP = utility::gen_rand(sV1, sV2) + sV3;

	if (target->is_player())
	{
        std::shared_ptr<client> ptarget = std::static_pointer_cast<client>(target);

		if (ptarget->IsInvincible()) return;
		if (ptarget->get_status_flag(STATUS_REDSLATE) != 0) return;

		if (caster->is_player())
		{
            std::shared_ptr<client> pcaster = std::static_pointer_cast<client>(caster);
            
            if (pcaster->GetParty() == ptarget->GetParty())
				return;

			if (pcaster->safeAttackMode)
			{
				iSideCondition = pcaster->GetPlayerRelationship(ptarget);
				if (iSideCondition != 7 && iSideCondition != 2 && iSideCondition != 6)
				{
					if (pcaster->map_->flags.fightZone)
					{
						if (pcaster->guild_ == ptarget->guild_)
							return;
					}
					else
						return;
				}
			}
		}

		iMaxSP = ptarget->GetMaxSP();
		if (ptarget->stamina > 0)
		{
			if (ptarget->m_iTimeLeft_FirmStamina == 0)
			{
				ptarget->stamina -= iSP;
				if (ptarget->stamina < 0) ptarget->stamina = 0;
				SendNotifyMsg(nullptr, ptarget, NOTIFY_SP, 0, 0, 0);
			}
		}
	}
}

void gserver::Effect_SpUp_Spot(std::shared_ptr<unit> caster, std::shared_ptr<unit> target, short sV1, short sV2, short sV3)
{
	int iSP, iMaxSP;
	uint64_t time = utility::current_time();
	if (caster == nullptr || target == nullptr)
		return;

	if (caster->is_dead() || target->is_dead()) return;

	iSP = utility::gen_rand(sV1, sV2) + sV3;

	if (target->is_player())
	{
        std::shared_ptr<client> ptarget = std::static_pointer_cast<client>(target);
		
		if (ptarget->is_dead()) return;

		iMaxSP = ptarget->GetMaxSP();
		if (ptarget->stamina < iMaxSP)
        {
            ptarget->stamina += iSP;

			if (ptarget->stamina > iMaxSP)
                ptarget->stamina = iMaxSP;

			SendNotifyMsg(nullptr, ptarget, NOTIFY_SP, 0, 0, 0);
		}
	}
}

void gserver::Effect_Damage_Spot_DamageMove(std::shared_ptr<unit> attacker, std::shared_ptr<unit> target, short sV1, short sV2, short sV3, bool exp, Element element /*= ELEMENT_NONE*/, magic * spell /*= nullptr*/)
{

}

void gserver::CalculateSSN_ItemIndex(client * client_, item * Weapon, int iValue)
{
	short sSkillIndex;

	if (client_ == nullptr) return;
	if (client_->m_bIsInitComplete == false) return;
	if (Weapon == nullptr) return;
	if (client_->dead_ == true) return;

	sSkillIndex = Weapon->m_sRelatedSkill;

	client_->CalculateSSN_SkillIndex(sSkillIndex, iValue);

}

int32_t gserver::CalculateAttackEffect(std::shared_ptr<unit> target, std::shared_ptr<unit> attacker, int tdX, int tdY, int iAttackMode, bool bNearAttack /*= false*/, bool bIsDash /*= false*/)
{
	int    iAP_SM=0, iAP_L=0, iAttackerHitRatio=0, iTargetDefenseRatio=0, iDestHitRatio=0, iResult=0, iAP_Abs_Armor=0, iAP_Abs_Shield=0;
	char   cAttackerDir=0, cAttackerSide=0, cTargetDir=0, cProtect=0;
	string AttackerName;
	short  sWeaponIndex=0, sAttackerWeapon=0, dX=0, dY=0, sX=0, sY=0, sAtkX=0, sAtkY=0, sTgtX=0, sTgtY=0;
	uint64_t  dwTime = utility::current_time();
	uint16_t   wWeaponType=0;
	double dTmp1=0, dTmp2=0, dTmp3=0;
	bool   bKilled = false;
	bool   bNormalMissileAttack = false;
	bool   bIsAttackerBerserk=false;
	int32_t iKilledDice, iDamage, iExp, iWepLifeOff = 0, iSideCondition, /*iMaxSuperAttack,*/ iWeaponSkill, iComboBonus, iTemp, iTemp2 = 0;
	int32_t iAttackerHP, iMoveDamage, iRepDamage;
	char   cAttackerSA;
	int32_t iAttackerSAvalue, iHitPoint;
	char   cDamageMoveDir;
	int32_t iPartyID, iConstructionPoint, iWarContribution, tX = 0, tY = 0, iDst1 = 0, iDst2 = 0;
	int32_t	iSkillLV = 0, iCropLimit = 0;
	int16_t sItemIndex = 0;

    std::shared_ptr<client> cattacker = std::static_pointer_cast<client>(attacker);
    std::shared_ptr<client> ctarget = std::static_pointer_cast<client>(target);
    std::shared_ptr<npc> nattacker = std::static_pointer_cast<npc>(attacker);
    std::shared_ptr<npc> ntarget = std::static_pointer_cast<npc>(target);

	iExp = 0;
	iPartyID = 0;

	cAttackerSA = 0;
	iAttackerSAvalue = 0;
	wWeaponType = 0;

	if ((target == nullptr) || (attacker == nullptr))
		return 0;

	if (attacker->map_->flags.attackEnabled == false) return 0;

	if (target->dead_ == true) return 0;


	if (attacker->is_player())
	{
		if ((cattacker->status & STATUS_INVISIBILITY) != 0) {
			cattacker->set_magic_flag(MAGICTYPE_INVISIBILITY, false);

			RemoveFromDelayEventList(attacker, MAGICTYPE_INVISIBILITY);
			cattacker->magic_effect_status[MAGICTYPE_INVISIBILITY] = 0;
		}

		if ((cattacker->m_sAppr2 & 0xF000) == 0) return 0;

		iAP_SM = 0;
		iAP_L = 0;

		wWeaponType = ((cattacker->m_sAppr2 & 0x0FF0) >> 4);
		if ((wWeaponType == 35 && cattacker->Equipped.TwoHand.initialized) && (cattacker->Equipped.TwoHand.m_sItemEffectType == ITEMEFFECTTYPE_ATTACK_MAGICITEM))
        {
			return 0;
		}
		cAttackerSide = cattacker->side;

		if (wWeaponType == 0)
		{
			iAP_SM = iAP_L = utility::gen_rand(1, (cattacker->GetStr() / 12));
			if (iAP_SM <= 0) iAP_SM = 1;
			if (iAP_L <= 0) iAP_L = 1;
			iAttackerHitRatio = cattacker->m_iHitRatio + cattacker->skillMastery[SKILL_HANDATTACK];

			cattacker->m_sUsingWeaponSkill = SKILL_HANDATTACK;

			if (cattacker->HasPartyRaidBonus()) //TODO: party bonus for hand attack?
			{
				iAttackerHitRatio += 40;
				iAP_SM += 10;
				iAP_L += 10;
			}
		}
		else if ((wWeaponType >= 1) && (wWeaponType < 40))
		{

			iAP_SM = utility::gen_rand(cattacker->m_cAttackDiceThrow_SM, cattacker->m_cAttackDiceRange_SM);
			iAP_L = utility::gen_rand(cattacker->m_cAttackDiceThrow_L, cattacker->m_cAttackDiceRange_L);


			iAP_SM += cattacker->m_cAttackBonus_SM;
			iAP_L += cattacker->m_cAttackBonus_L;

			if (cattacker->HasPartyRaidBonus()) //TODO: party bonus?
			{
				//iAttackerHitRatio += 100;
				iAP_SM += 50;
				iAP_L += 50;
			}

			iAttackerHitRatio = cattacker->m_iHitRatio;


			dTmp1 = (double)iAP_SM;
			if (cattacker->GetStr() <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double)cattacker->GetStr();

			dTmp2 = dTmp2 / 5.0f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iAP_SM = (int)(dTmp3 + 0.5f);

			dTmp1 = (double)iAP_L;
			if (cattacker->GetStr() <= 0)
				dTmp2 = 1.0f;
			else dTmp2 = (double)cattacker->GetStr();

			dTmp2 = dTmp2 / 5.0f;
			dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
			iAP_L = (int)(dTmp3 + 0.5f);
			//
		}
		else if (wWeaponType >= 40)
		{
			iAP_SM = utility::gen_rand(cattacker->m_cAttackDiceThrow_SM, cattacker->m_cAttackDiceRange_SM);
			iAP_L = utility::gen_rand(cattacker->m_cAttackDiceThrow_L, cattacker->m_cAttackDiceRange_L);


			iAP_SM += cattacker->m_cAttackBonus_SM;
			iAP_L += cattacker->m_cAttackBonus_L;

			iAttackerHitRatio = cattacker->m_iHitRatio;
			bNormalMissileAttack = true;


			iAP_SM += utility::gen_rand(1, (cattacker->GetStr() / 20));
			iAP_L += utility::gen_rand(1, (cattacker->GetStr() / 20));
			//
		}

		iAttackerHitRatio += 50;
		if (iAP_SM <= 0) iAP_SM = 1;
		if (iAP_L <= 0) iAP_L = 1;

		if (cattacker->m_iCustomItemValue_Attack != 0)
		{

			if ((cattacker->m_iMinAP_SM != 0) && (iAP_SM < cattacker->m_iMinAP_SM))
			{
				iAP_SM = cattacker->m_iMinAP_SM;
			}
			if ((cattacker->m_iMinAP_L != 0) && (iAP_L < cattacker->m_iMinAP_L))
			{
				iAP_L = cattacker->m_iMinAP_L;
			}

			if ((cattacker->m_iMaxAP_SM != 0) && (iAP_SM > cattacker->m_iMaxAP_SM))
			{
				iAP_SM = cattacker->m_iMaxAP_SM;
			}
			if ((cattacker->m_iMaxAP_L != 0) && (iAP_L > cattacker->m_iMaxAP_L))
			{
				iAP_L = cattacker->m_iMaxAP_L;
			}
		}
		//TODO: fantasy "monster event"
		if (cattacker->m_heroArmourBonus == 1)
		{
			// 			if (cattacker->m_iMonsterCount >= 10)
			// 			{
			// 				cattacker->m_iMonsterCount / 10;
			// 				iAP_SM += cattacker->m_iMonsterCount / 10;
			// 				iAP_L += cattacker->m_iMonsterCount / 10;
			// 			}
			iAttackerHitRatio += 100;
			iAP_SM += 5;
			iAP_L += 5;
		}

		//if (g_game->m_pClientList[sAttackerH]->m_iMonsterCount > 0) 
		//	{
		//       iAP_SM = g_game->m_pClientList[sAttackerH]->m_iMonsterCount/20; // Change from 10 to 20.
		//	iAP_L = g_game->m_pClientList[sAttackerH]->m_iMonsterCount/20; // Change from 10 to 20.
		//       iAP_SM += 5;  // Warrior
		//       iAP_L += 5;    // Warrior
		//	}

		//TODO: Do something with this?
		// New monster event xRisenx
		if (cattacker->m_heroArmourBonus == 1)
		{ // Warrior Hero Set +50 Damage Per Hit xRisenx
			iAttackerHitRatio += 125;  // Warrior Hero Set +125 Hit Rate Per Hit xRisenx
			iAP_SM += 50;
			iAP_L += 50;
		}

		// New Ancient Warrior Hero Set xRisenx
		if (cattacker->m_AheroArmourBonus == 1)
		{ // Warrior Ancient Hero Set +125 Damage Per Hit xRisenx
			iAttackerHitRatio += 200;  // Warrior Ancient Hero Set +200 Hit Rate Per Hit xRisenx
			iAP_SM += 125;
			iAP_L += 125;
		}
		// New Legendary Warrior Hero Set xRisenx
		if (cattacker->m_LheroArmourBonus == 1)
		{ // Warrior Legendary Hero Set +300 Damage Per Hit xRisenx
			iAttackerHitRatio += 300;  // Warrior Legendary Hero Set +300 Hit Rate Per Hit xRisenx
			iAP_SM += 300;
			iAP_L += 300;
		}

		if (cattacker->Equipped.TwoHand.initialized)
		{
			iTemp = cattacker->Equipped.TwoHand.m_sIDnum;
			if ((iTemp == ITEM_KLONESSAXE || iTemp == ITEM_KLONESSBLADE) && cattacker->m_reputation > 0)
			{
				iRepDamage = cattacker->m_reputation / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
			else if ((iTemp == ITEM_DKLONESSAXE || iTemp == ITEM_DKLONESSBLADE) && cattacker->m_reputation < 0)
			{
				iRepDamage = abs(cattacker->m_reputation) / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
			else if ((cattacker->Equipped.TwoHand.m_sIDnum == ITEM_DARKEXECUTOR) && (m_cDayOrNight == 2))
			{
				iAP_SM += 80;
				iAP_L += 80;
			}
			else if ((cattacker->Equipped.TwoHand.m_sIDnum == ITEM_LIGHTINGBLADE) && (m_cDayOrNight == 1))
			{
				iAP_SM += 80;
				iAP_L += 80;
			}
		}

		if (cattacker->Equipped.RightHand.initialized)
		{
			iTemp = cattacker->Equipped.RightHand.m_sIDnum;
			if ((iTemp == ITEM_KLONESSESTERK) && cattacker->m_reputation > 0)
			{
				iRepDamage = cattacker->m_reputation / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
		}

		if (cattacker->Equipped.Necklace.initialized)
		{
			iTemp = cattacker->Equipped.Necklace.m_sIDnum;
			if ((iTemp == ITEM_NECKLACEOFKLONESS) && cattacker->m_reputation > 0)
			{
				iRepDamage = cattacker->m_reputation / 100;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
			if ((iTemp == ITEM_NECKLACEOFDKLONESS) && cattacker->m_reputation < 0)
			{
				iRepDamage = abs(cattacker->m_reputation) / 100;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
		}

		if (cattacker->Equipped.RightHand.initialized)
		{
			iTemp = cattacker->Equipped.RightHand.m_sIDnum;
			if ((iTemp == ITEM_DKLONESSESTERK) && cattacker->m_reputation < 0)
			{
				iRepDamage = abs(cattacker->m_reputation) / 50;
				if (iRepDamage < 5) iRepDamage = 5;
				iAP_SM += iRepDamage;
				iAP_L += iRepDamage;
			}
		}

		cAttackerDir = cattacker->direction;
		AttackerName = cattacker->name;

		bIsAttackerBerserk = cattacker->is_berserked();

		if ((cattacker->superAttack > 0) && (iAttackMode >= 20)) {

			dTmp1 = (double)iAP_SM;
			dTmp2 = (double)cattacker->level;
			dTmp3 = dTmp2 / 100.0f;
			dTmp2 = dTmp1 * dTmp3;
			iTemp = (int)(dTmp2 + 0.5f);
			iAP_SM += iTemp;

			dTmp1 = (double)iAP_L;
			dTmp2 = (double)cattacker->level;
			dTmp3 = dTmp2 / 100.0f;
			dTmp2 = dTmp1 * dTmp3;
			iTemp = (int)(dTmp2 + 0.5f);
			iAP_L += iTemp;

			switch (cattacker->m_sUsingWeaponSkill)
			{
			case SKILL_ARCHERY:
				iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10);
				//iAttackerHitRatio  += 30;
				break;
			case SKILL_SHORTSWORD:
				iAP_SM *= 2; iAP_L *= 2;
				break;
			case SKILL_LONGSWORD:
				iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10);
				iAttackerHitRatio += 45;
				break;
			case SKILL_AXE:
				iAP_SM += (iAP_SM / 3); iAP_L += (iAP_L / 3);
				iAttackerHitRatio += 30;
				break;
			case SKILL_HAMMER:
				iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5);
				iAttackerHitRatio += 15;
				break;
			case SKILL_STAFF:
				iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5);
				iAttackerHitRatio += 50;
				break;
			}


			iAttackerHitRatio += 5; // Hit rate bonus from using Critcals xRisenx
			iAttackerHitRatio += cattacker->m_iCustomItemValue_Attack;
		}


		if (bIsDash == true) {

			iAttackerHitRatio += 20;

			switch (cattacker->m_sUsingWeaponSkill) {
			case SKILL_LONGSWORD:
				iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10);
				iAttackerHitRatio += 45;
				break;
			case SKILL_AXE:
				iAP_SM += (iAP_SM / 3); iAP_L += (iAP_L / 3);
				iAttackerHitRatio += 30;
				break;
			case SKILL_HAMMER:
				iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5);
				iAttackerHitRatio += 15;
				break;
			}
		}

		iAttackerHP = cattacker->health;

		iAttackerHitRatio += cattacker->m_iAddAR;

		sAtkX = cattacker->x;
		sAtkY = cattacker->y;

		if (cattacker->GetParty())
			iPartyID = cattacker->GetParty()->GetID();
	}
	else if (attacker->is_npc())
	{
		if ((nattacker->status & STATUS_INVISIBILITY) != 0) {
			nattacker->set_magic_flag(MAGICTYPE_INVISIBILITY, false);

			RemoveFromDelayEventList(attacker, MAGICTYPE_INVISIBILITY);
			nattacker->magic_effect_status[MAGICTYPE_INVISIBILITY] = 0;
		}


		cAttackerSide = nattacker->side;

		iAP_SM = 0;
		iAP_L = 0;

		// Crusade
		if (nattacker->m_cAttackDiceThrow != 0)
			iAP_L = iAP_SM = utility::gen_rand(nattacker->m_cAttackDiceThrow, nattacker->m_cAttackDiceRange);

		iAttackerHitRatio = nattacker->m_iHitRatio;

		cAttackerDir = nattacker->direction;
		AttackerName = nattacker->name;

		if (nattacker->is_berserked())
			bIsAttackerBerserk = true;
		else bIsAttackerBerserk = false;


		iAttackerHP = nattacker->health;


		cAttackerSA = nattacker->specialAbility;

		sAtkX = cattacker->x;
		sAtkY = cattacker->y;
	}

	switch (target->get_owner_type())
	{
	case ot_player:
	{
#ifdef SAMESIDETOWNPROTECT

		if ((g_clientList[sTargetH]->m_iPKCount == 0) && (cAttackerType == OWNERTYPE_PLAYER) && (g_clientList[sTargetH]->m_side == cAttackerSide) && (g_clientList[sTargetH]->m_iIsOnTown == NONPK)) return 0;
#endif 
#ifdef BEGINNERTOWNPROTECT
		if ((cAttackerType == OWNERTYPE_PLAYER) && (g_clientList[sTargetH]->m_bIsNeutral == true)
			&& (g_clientList[sTargetH]->m_iPKCount == 0) && (g_clientList[sTargetH]->m_iIsOnTown == NONPK)) return 0;
#endif

		//if ((g_clientList[sTargetH]->m_sX != tdX) || (g_clientList[sTargetH]->m_sY != tdY)) return 0; // Fix for cannot hit moving targets with meele weps xRisenx // Anti Hack vs Criting hack

		if (ctarget->IsInvincible()) return 0;

		if ((attacker->is_player()) && (cattacker->neutral == true) && (ctarget->playerKillCount == 0)) return 0;

		if (ctarget->GetParty() && ctarget->GetParty()->GetID() == iPartyID)
			return 0;

		if ((ctarget->status & STATUS_REDSLATE) != 0) return 0;

		cTargetDir = ctarget->direction;
		iTargetDefenseRatio = ctarget->m_iDefenseRatio;

		if ((attacker->is_player()) && (cattacker->safeAttackMode == true)) {
			iSideCondition = cattacker->GetPlayerRelationship(ctarget);
			if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6)) {

				iAP_SM = iAP_SM / 2;
				iAP_L = iAP_L / 2;
			}
			else {

				if (attacker->map_->flags.fightZone == true) {
					if (cattacker->guild_ == ctarget->guild_) return 0;
					else {

						iAP_SM = iAP_SM / 2;
						iAP_L = iAP_L / 2;
					}
				}
				else return 0;
			}
		}

		if (attacker->is_npc() && ctarget->HasPartyHuntBonus())
		{
			iAP_SM *= 0.85;
			iAP_L *= 0.85;
		}

		iTargetDefenseRatio += ctarget->m_iAddDR;

		sTgtX = ctarget->x;
		sTgtY = ctarget->y;
	}
		break;
	case ot_npc:
	{
		if (ntarget->health == 0) return 0;

		//if ((g_npcList[sTargetH]->m_sX != tdX) || (g_npcList[sTargetH]->m_sY != tdY)) return 0; // Fix for cannot hit moving targets with meele weps xRisenx // Anti Hack vs Criting hack

		cTargetDir = ntarget->direction;
		iTargetDefenseRatio = ntarget->m_iDefenseRatio;


		if (attacker->is_player())
		{
			switch (ntarget->get_type())
			{
			case NPC_ESG:
			case NPC_GMG:
			case NPC_AS:
				if ((cattacker->side == faction::neutral) || (ntarget->side == attacker->side)) return 0;
				break;
			}

			if ((wWeaponType == 25) && (ntarget->m_cActionLimit == 5) && (ntarget->buildCount > 0)) {

				if (cattacker->crusadeDuty != 2 && (cattacker->m_iAdminUserLevel == 0)) break;

				switch (ctarget->get_type())
				{
				case NPC_AGT:
				case NPC_CGT:
				case NPC_MS:
				case NPC_DT:
					if (cattacker->IsGM()) ntarget->buildCount = 1;
					switch (ntarget->buildCount)
					{
					case 1:
						ctarget->m_sAppr2 = 0;
						SendEventToNearClient_TypeA(ntarget, MSGID_MOTION_NULL, 0, 0, 0);

						if (m_bIsCrusadeMode)
						{
							switch (ntarget->get_type())
							{
							case NPC_AGT: iConstructionPoint = 700; iWarContribution = 700; break;
							case NPC_CGT: iConstructionPoint = 700; iWarContribution = 700; break;
							case NPC_MS: iConstructionPoint = 500; iWarContribution = 500; break;
							case NPC_DT: iConstructionPoint = 500; iWarContribution = 500; break;
							}

							cattacker->crusadeContribution += iWarContribution;
							if (cattacker->crusadeContribution > MAXWARCONTRIBUTION)
								cattacker->crusadeContribution = MAXWARCONTRIBUTION;

							SendNotifyMsg(nullptr,cattacker, NOTIFY_CONSTRUCTIONPOINT, cattacker->crusadePoint, cattacker->crusadeContribution, 0);
						}
						break;
					case 5:
						ntarget->appr2 = 1;
						SendEventToNearClient_TypeA(ntarget, MSGID_MOTION_NULL, 0, 0, 0);
						break;
					case 10:
						ntarget->appr2 = 2;
						SendEventToNearClient_TypeA(ntarget, MSGID_MOTION_NULL, 0, 0, 0);
						break;
					}
					break;
				}

				ntarget->buildCount--;
				if (ntarget->buildCount <= 0) {
					ntarget->buildCount = 0;
				}
				return 0;
			}

			//#ifdef TitleHG
			//			if ((wWeaponType == 27) && (g_npcList[sTargetH]->m_iNpcCrops != 0) && (g_npcList[sTargetH]->m_cActionLimit == 5) && (g_npcList[sTargetH]->m_iBuildCount > 0)) {
			//
			//				//iSkillLV = g_clientList[sAttackerH]->m_cSkillMastery[SKILL_FARMING];
			//				iSkillLV = (g_clientList[sAttackerH]->m_stTitles[TITLE_INDEX_CRAFT_FARMING].m_iRank);
			//				iCropLimit = g_npcList[sTargetH]->m_iCropsSkillLV;
			////#ifdef TitleHG
			//				//cFarmingSkill = (g_game->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_CRAFT_FARMING].m_iRank);
			////#endif
			//
			//				//if(20 > iSkillLV) return 0;  
			//
			//				//if(g_clientList[sAttackerH]->m_iLevel <  20 ) return 0 ;
			//
			//
			//				switch (g_npcList[sTargetH]->m_sType) {
			//				case NPC_CROPS:
			//					switch (g_npcList[sTargetH]->m_iBuildCount) {
			//					case 1: 
			//						g_npcList[sTargetH]->m_sAppr2 = g_npcList[sTargetH]->m_iNpcCrops << 8 | 3;
			//						//						g_npcList[sTargetH]->m_sAppr2 = (char)0;
			//
			//						//if(iSkillLV <= iCropLimit + 10)
			//							//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			//						if(iSkillLV <= iCropLimit + 10)
			//							g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, 1);
			//						g_game->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_NULL, NULL, NULL, NULL);
			////#ifdef TitleHG
			//						//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			////#endif
			//						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
			//						//{
			//							g_game->bCropsItemDrop(sAttackerH,sTargetH,true);
			//						//}
			//						g_game->DeleteNpc(sTargetH);
			//
			//						break;
			//					case 8: 		
			//						g_npcList[sTargetH]->m_sAppr2 = g_npcList[sTargetH]->m_iNpcCrops << 8 | 3;
			//						//						g_npcList[sTargetH]->m_sAppr2 = (char)3;
			//
			//						//if(iSkillLV <= iCropLimit + 10)
			//							//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			//						g_game->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_NULL, NULL, NULL, NULL);
			////#ifdef TitleHG
			//						if(iSkillLV <= iCropLimit + 10)
			//							g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, 1);
			//						//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			////#endif
			//						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
			//						//{
			//							//if(iSkillLV <= iCropLimit + 10)
			//								//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			////#ifdef TitleHG
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, cFarmingSkill <= iCropLimit + 10);
			////#endif
			//							g_game->bCropsItemDrop(sAttackerH,sTargetH);
			//						//}
			//						break;
			//
			//					case 18: 
			//						g_npcList[sTargetH]->m_sAppr2 = g_npcList[sTargetH]->m_iNpcCrops << 8 | 2;
			//						//						g_npcList[sTargetH]->m_sAppr2 = (char)2;
			//
			//						//if(iSkillLV <= iCropLimit + 10)
			//							//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			//						if(iSkillLV <= iCropLimit + 10)
			//							g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, 1);
			//						g_game->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_NULL, NULL, NULL, NULL);
			////#ifdef TitleHG
			//						//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			////#endif
			//						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
			//						//{
			//							//if(iSkillLV <= iCropLimit + 10)
			//								//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
			////#ifdef TitleHG
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);
			//							//g_game->increaseTitlePoints(sAttackerH, TITLE_INDEX_CRAFT_FARMING, cFarmingSkill <= iCropLimit + 10);
			////#endif
			//							g_game->bCropsItemDrop(sAttackerH,sTargetH);
			//						//}
			//						break;
			//					}
			//					break;
			//				}
			//
			//				if(g_npcList[sTargetH] == NULL)
			//				{
			//					return 0;
			//				}
			//
			//				/*if(probabilityTable(iSkillLV,iCropLimit,1) > 0 
			//					|| g_npcList[sTargetH]->m_iBuildCount == 1
			//					|| g_npcList[sTargetH]->m_iBuildCount == 8
			//					|| g_npcList[sTargetH]->m_iBuildCount == 18) {
			//
			//						iTemp = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];
			//
			//						if ((iTemp != -1) || (g_clientList[sAttackerH]->m_pItemList[iTemp] != NULL)) {
			//
			//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)
			//								g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan--;
			//
			//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
			//
			//								g_game->SendNotifyMsg(NULL, sAttackerH, NOTIFY_ITEMLIFESPANEND, g_clientList[sAttackerH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
			//
			//								g_game->ReleaseItemHandler(sAttackerH, iTemp, true);  
			//							}
			//						}
			//
			//
			//						g_npcList[sTargetH]->m_iBuildCount--;
			//				}*/
			//
			//				if (g_npcList[sTargetH]->m_iBuildCount <= 0) {
			//					g_npcList[sTargetH]->m_iBuildCount = 0;
			//				}
			//
			//				return 0;
			//			}
			//			if (g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1 &&
			//				g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ] != NULL &&
			//				g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] ]->m_sIDnum == ITEM_DEMONSLAYER &&
			//				g_npcList[sTargetH]->m_sType == NPC_DEMON) {
			//				iAP_L += 10;
			//			}
			//		}
			//
			//		sTgtX = g_npcList[sTargetH]->m_sX;
			//		sTgtY = g_npcList[sTargetH]->m_sY;
			//		break;
			//	}
			//#else

//TODO: Farming?
/*
			if ((wWeaponType == 27) && (ctarget->m_iNpcCrops != 0) && (ctarget->m_cActionLimit == 5) && (ctarget->m_iBuildCount > 0))
			{

				iSkillLV = (static_cast<Client*>(attacker)->m_stTitles[TITLE_INDEX_CRAFT_FARMING].m_iRank);
				//iSkillLV = g_clientList[sAttackerH]->m_cSkillMastery[SKILL_FARMING];
				iCropLimit = ctarget->m_iCropsSkillLV;

				//if(20 > iSkillLV) return 0;  

				//if(g_clientList[sAttackerH]->m_iLevel <  20 ) return 0 ; 


				switch (ctarget->m_sType) {
				case NPC_CROPS:
					switch (ctarget->m_iBuildCount) {
					case 1:
						ctarget->m_sAppr2 = ctarget->m_iNpcCrops << 8 | 3;
						//						g_npcList[sTargetH]->m_sAppr2 = (char)0;

						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);

						increaseTitlePoints(static_cast<Client*>(attacker), TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);

						SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_NULL, 0, 0, 0);


						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						//{
						bCropsItemDrop(static_cast<Client*>(attacker), ctarget, true);
						//}
						DeleteNpc(ctarget);

						break;
					case 8:
						ctarget->m_sAppr2 = ctarget->m_iNpcCrops << 8 | 3;
						//						g_npcList[sTargetH]->m_sAppr2 = (char)3;

						increaseTitlePoints(static_cast<Client*>(attacker), TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);

						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);

						SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_NULL, 0, 0, 0);


						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						//{
						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
						bCropsItemDrop(static_cast<Client*>(attacker), ctarget);
						//}
						break;

					case 18:
						ctarget->m_sAppr2 = ctarget->m_iNpcCrops << 8 | 2;
						//						g_npcList[sTargetH]->m_sAppr2 = (char)2;

						increaseTitlePoints(static_cast<Client*>(attacker), TITLE_INDEX_CRAFT_FARMING, iSkillLV <= iCropLimit + 10);

						//if(iSkillLV <= iCropLimit + 10)
						//CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);

						SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_NULL, 0, 0, 0);


						//if(probabilityTable(iSkillLV,iCropLimit,2) > 0)
						//{
						//if(iSkillLV <= iCropLimit + 10)
						//	CalculateSSN_SkillIndex(sAttackerH, SKILL_FARMING, 1);
						bCropsItemDrop(static_cast<Client*>(attacker), ctarget);
						//}
						break;
					}
					break;
				}

				if (ctarget == nullptr)
				{
					return 0;
				}

				//				if(probabilityTable(iSkillLV,iCropLimit,1) > 0 
				//					|| g_npcList[sTargetH]->m_iBuildCount == 1
				//					|| g_npcList[sTargetH]->m_iBuildCount == 8
				//					|| g_npcList[sTargetH]->m_iBuildCount == 18) {
				//
				//						iTemp = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];
				//
				//						if ((iTemp != -1) || (g_clientList[sAttackerH]->m_pItemList[iTemp] != NULL)) {
				//
				//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)
				//								g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan--;
				//
				//							if (g_clientList[sAttackerH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				//
				//								g_game->SendNotifyMsg(NULL, sAttackerH, NOTIFY_ITEMLIFESPANEND, g_clientList[sAttackerH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				//
				//								g_game->ReleaseItemHandler(sAttackerH, iTemp, true);  
				//							}
				//						}
				////#endif
				//
				//						g_npcList[sTargetH]->m_iBuildCount--;
				//				}

				ctarget->m_iBuildCount--;
				if (ctarget->m_iBuildCount <= 0) {
					ctarget->m_iBuildCount = 0;
				}

				return 0;
			}*/
			if (cattacker->itemEquipStatus[EQUIPPOS_TWOHAND] != -1 &&
				cattacker->Equipped.TwoHand.initialized &&
				cattacker->Equipped.TwoHand.m_sIDnum == ITEM_DEMONSLAYER &&
				ntarget->get_type() == NPC_DEMON) {
				iAP_L += 10;
			}
		}

		sTgtX = ntarget->x;
		sTgtY = ntarget->y;
	}
		break;
	}

	if ((attacker->is_player()) && (target->is_player())) {

		sX = attacker->x;
		sY = attacker->y;

		dX = target->x;
		dY = target->y;

		if (target->map_->iGetAttribute(sX, sY, 0x00000006) != 0) return 0;
		if (target->map_->iGetAttribute(dX, dY, 0x00000006) != 0) return 0;
	}


	if (attacker->is_player()) {
		if (cattacker->GetDex() > 50) {
			iAttackerHitRatio += (cattacker->GetDex() - 50);
		}
	}

	if (wWeaponType >= 40) {
		switch (attacker->map_->weather) {
		case WEATHER_LIGHTRAIN:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 20); break;
		case WEATHER_MEDIUMRAIN:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 10); break;
		case WEATHER_HEAVYRAIN:	iAttackerHitRatio = iAttackerHitRatio - (iAttackerHitRatio / 4);  break;
		}
	}
	if (iAttackerHitRatio < 0)   iAttackerHitRatio = 0;

	//TODO: use Unit* base, save a switch call
	switch (target->get_owner_type())
	{
	case ot_player:
		cProtect = ctarget->magic_effect_status[MAGICTYPE_PROTECT];
		break;
	case ot_npc:
		cProtect = ntarget->magic_effect_status[MAGICTYPE_PROTECT];
		break;
	}



	if (attacker->is_player())
	{
		if (cattacker->Equipped.TwoHand.initialized)
		{
			/*if (cattacker->Equipped.TwoHand.itemEffectType == ITEMEFFECTTYPE_ATTACK_ARROW)
			{
				if (cattacker->Equipped.Arrow.initialized)
				{
					return 0;
				}
				else
				{
					cattacker->Equipped.Arrow.count--;
					if (cattacker->Equipped.Arrow.count <= 0) {

						ItemDepleteHandler(cattacker, &cattacker->Equipped.Arrow, false);

						cattacker->Equipped.Arrow = _iGetArrowItemIndex(cattacker);
					}
					else {
						SendNotifyMsg(nullptr, cattacker, NOTIFY_SETITEMCOUNT, cattacker->Equipped.Arrow.m_slot, cattacker->Equipped.Arrow->count, (char)false);
						iCalcTotalWeight(cattacker);
					}
				}
				if (cProtect == MAGICPROTECT_PFA) return 0;
			}
			else*/
            {
				switch (cProtect)
				{
				case MAGICPROTECT_DS:
					iTargetDefenseRatio += 40;
					break;
				case MAGICPROTECT_GDS:
					iTargetDefenseRatio += 100;
					break;
				}
				if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
			}
		}
	}
	else {
		switch (cProtect)
		{
		case MAGICPROTECT_PFA:
			switch (nattacker->get_type())
			{
                case NPC_DARK_ELF:
                case NPC_AGT:
                    if (abs(sTgtX - nattacker->x) >= 1 || abs(sTgtY - nattacker->y) >= 1)
                        return 0;
                    break;
			}
			break;
		case MAGICPROTECT_DS: iTargetDefenseRatio += 40;  break;
		case MAGICPROTECT_GDS: iTargetDefenseRatio += 100; break;
		}
		if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
	}

	if (cAttackerDir == cTargetDir) iTargetDefenseRatio = iTargetDefenseRatio / 2;

	if (iTargetDefenseRatio < 1)   iTargetDefenseRatio = 1;

	dTmp1 = (double)(iAttackerHitRatio);
	dTmp2 = (double)(iTargetDefenseRatio);

	dTmp3 = (dTmp1 / dTmp2) * HITRATIOFACTOR;
	iDestHitRatio = (int)(dTmp3);

	if (iDestHitRatio < MINIMUMHITRATIO) iDestHitRatio = MINIMUMHITRATIO;

	if (iDestHitRatio > MAXIMUMHITRATIO) iDestHitRatio = MAXIMUMHITRATIO;

	if ((bIsAttackerBerserk == true) && (iAttackMode < 20))
    {
		if (attacker->is_player())
        {
			iAP_SM = iAP_SM * 4;
			iAP_L = iAP_L * 4;
		}
		else{
			iAP_SM = iAP_SM * 3.6;
			iAP_L = iAP_L  * 3.6;
		}
	}

	if (attacker->is_player())
    {
		iAP_SM += cattacker->m_iAddPhysicalDamage;
		iAP_L += cattacker->m_iAddPhysicalDamage;
	}

	if (bNearAttack == true)
    {
		iAP_SM = iAP_SM / 2;
		iAP_L = iAP_L / 2;
	}

	if (target->is_player())
    {
		iAP_SM -= (utility::gen_rand(1, ctarget->m_iVit / 10) - 1);
		iAP_L -= (utility::gen_rand(1, ctarget->m_iVit / 10) - 1);
	}

	/*if (cAttackerType == OWNERTYPE_PLAYER) {
	short weaponStatus = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1 ?
	g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] : g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];

	if (weaponStatus != -1 &&	g_clientList[sAttackerH]->m_pItemList[ weaponStatus ] != NULL) {
	ItemID weapon = g_clientList[sAttackerH]->m_pItemList[ weaponStatus ]->m_sIDnum;

	if((g_game->m_cDayOrNight == 2 && weapon == ITEM_DARKEXECUTOR) ||
	(g_game->m_cDayOrNight == 1 && weapon == ITEM_LIGHTINGBLADE)){
	iAP_SM += 8;
	iAP_L  += 8;
	}*/

	//if ((g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sIDnum == 849) || // KlonessBlade 
	//	(g_game->m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 850)) { // KlonessAxe
	//	if (g_game->m_pClientList[sAttackerH]->m_reputation > 0) {
	//		iRepDamage = g_game->m_pClientList[sAttackerH]->m_reputation/100;
	//		if (iRepDamage < 5) iRepDamage = 5;
	//			iAP_SM += iRepDamage;
	//			iAP_L += iRepDamage;
	//	}
	//	if (cTargetType == OWNERTYPE_PLAYER) {
	//		if (g_game->m_pClientList[sTargetH] == NULL) return 0;
	//		if (g_game->m_pClientList[sTargetH]->m_reputation < 0) {
	//			iRepDamage = (abs(g_game->m_pClientList[sTargetH]->m_reputation)/10);
	//			if (iRepDamage > 10) iRepDamage = 10;
	//			iAP_SM += iRepDamage;
	//			iAP_L += iRepDamage;
	//		}
	//	}
	//}
	/*if (cTargetType == OWNERTYPE_PLAYER &&
	(weapon == ITEM_KLONESSAXE ||	weapon == ITEM_KLONESSBLADE || weapon == ITEM_KLONESSESTERK) &&
	g_clientList[sAttackerH]->m_reputation > 0)
	{
	iRepDamage = g_clientList[sAttackerH]->m_reputation/100;
	if (iRepDamage < 5) iRepDamage = 5;
	iDamage += iRepDamage;
	}*/
	//}

	/*if (cTargetType == OWNERTYPE_PLAYER && g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_NECK] != -1 &&
	g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_NECK] ] != NULL &&
	g_clientList[sAttackerH]->m_pItemList[ g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_NECK] ]->m_sIDnum == ITEM_NECKLACEOFKLONESS &&
	g_clientList[sAttackerH]->m_reputation > g_clientList[sTargetH]->m_reputation)
	{
	iTemp = (g_clientList[sAttackerH]->m_reputation - g_clientList[sTargetH]->m_reputation)/22;
	iAP_SM += iTemp > 15 ? 15 : iTemp;
	}*/
	//}

	if (attacker->is_player())
    {
		if (iAP_SM <= 1) iAP_SM = 1;
		if (iAP_L <= 1) iAP_L = 1;
	}
	else {
		if (iAP_SM <= 0) iAP_SM = 0;
		if (iAP_L <= 0) iAP_L = 0;
	}

	iResult = utility::gen_rand(1, 100);

	if (iResult <= iDestHitRatio)
    {
		if (attacker->is_player())
		{
			if (((cattacker->m_iHungerStatus <= 10) || (cattacker->stamina <= 0)) &&
				(utility::gen_rand(1, 10) == 5)) return false;

			cattacker->m_iComboAttackCount++;
			if (cattacker->m_iComboAttackCount < 0) cattacker->m_iComboAttackCount = 0;
			if (cattacker->m_iComboAttackCount > 4) cattacker->m_iComboAttackCount = 1;

			iWeaponSkill = _iGetWeaponSkillType(cattacker);
			iComboBonus = iGetComboAttackBonus(iWeaponSkill, cattacker->m_iComboAttackCount);


			if ((cattacker->m_iComboAttackCount > 1) && (cattacker->m_iAddCD != 0))
				iComboBonus += cattacker->m_iAddCD;

			iAP_SM += iComboBonus;
			iAP_L += iComboBonus;


			switch (cattacker->m_iSpecialWeaponEffectType)
			{
			case ITEMSTAT_CRITICAL:
				if ((cattacker->superAttack > 0) && (iAttackMode >= 20)) {
					iAP_SM += cattacker->m_iSpecialWeaponEffectValue * 10; // *10 xRisenx
					iAP_L += cattacker->m_iSpecialWeaponEffectValue * 10; // *10 xRisenx
				}
				break;

			case ITEMSTAT_POISONING:
				cAttackerSA = 61;
				iAttackerSAvalue = cattacker->m_iSpecialWeaponEffectValue * 50; // *50 xRisenx
				break;

			case ITEMSTAT_RIGHTEOUS:
				cAttackerSA = 62;
				break;
			}

			if (cattacker->map_->flags.fightZone == true)
			{
				iAP_SM += iAP_SM / 3;
				iAP_L += iAP_L / 3;
			}

			if (cattacker->map_->flags.apocalypseMap)
			{
				iAP_SM *= 1.2;
				iAP_L *= 1.2;
			}

			if (cattacker->CheckNearbyFlags())
			{
				iAP_SM *= 1.3;
				iAP_L *= 1.3;
			}

			if ((target->is_player()) && (m_bIsCrusadeMode == true) && (cattacker->crusadeDuty == 1))
			{

				if (cattacker->level <= 80)
				{
					iAP_SM += iAP_SM;
					iAP_L += iAP_L;
				}

				else if (cattacker->level <= 100)
				{
					iAP_SM += (iAP_SM * 7) / 10;
					iAP_L += (iAP_L * 7) / 10;
				}
				else
				{
					iAP_SM += iAP_SM / 3;;
					iAP_L += iAP_L / 3;
				}
			}
		}

		switch (target->get_owner_type()/*target->m_sType*/)
		{
		case ot_player:
			ClearSkillUsingStatus(ctarget);

			if ((dwTime -ctarget->m_dwTime) < RAGPROTECTIONTIME)
			{
				return 0;
			}
			else
			{

				//if(cAttackerSA == 62 && g_clientList[sTargetH]->m_reputation < g_clientList[sAttackerH]->m_reputation)
				if (cAttackerSA == 62 && cattacker->m_reputation > 0)
				{
					iRepDamage = cattacker->m_reputation / 60;
					if (iRepDamage < 5) iRepDamage = 5;
					iAP_SM += iRepDamage;
					iAP_L += iRepDamage;
					/*iTemp = (g_clientList[sAttackerH]->m_reputation - g_clientList[sTargetH]->m_reputation) / 20;
					if (iTemp > 10) iTemp = 10;
					iAP_SM += iTemp;*/
				}


				iAP_Abs_Armor = 0;
				iAP_Abs_Shield = 0;

				iTemp = utility::gen_rand(1, 10000);
				if ((iTemp >= 1) && (iTemp < 5000))           iHitPoint = 1; // BODY
				else if ((iTemp >= 5000) && (iTemp < 7500))   iHitPoint = 2; // PANTS + LEGGINGS
				else if ((iTemp >= 7500) && (iTemp < 9000))   iHitPoint = 3; // ARMS
				else if ((iTemp >= 9000) && (iTemp <= 10000)) iHitPoint = 4; // HEAD

				//TODO: fix item stats
				switch (iHitPoint)
				{
				case 1:
					if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BODY] > 0)
					{

						if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BODY] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BODY];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 2:
					if ((ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] +
						ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BOOTS]) > 0)
					{

						if ((ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] +
							ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BOOTS]) >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)(ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_PANTS] + ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_BOOTS]);
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 3:
					if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS] > 0)
					{

						if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_ARMS];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;

				case 4:
					if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD] > 0)
					{

						if (ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD] >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Armor[EQUIPPOS_HEAD];
						dTmp2 = (double)iAP_SM;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Armor = (int)dTmp3;
					}
					break;
				}

				bool shield = false;
				if (cAttackerDir != cTargetDir && ctarget->m_iDamageAbsorption_Shield > 0)
				{
					if (utility::gen_rand(1, 100) <= (ctarget->skillMastery[SKILL_SHIELD]))
					{
						shield = true;

						ctarget->CalculateSSN_SkillIndex(SKILL_SHIELD, 1);

						if (ctarget->m_iDamageAbsorption_Shield >= 80)
							dTmp1 = 80.0f;
						else dTmp1 = (double)ctarget->m_iDamageAbsorption_Shield;
						dTmp2 = (double)iAP_SM - iAP_Abs_Armor;
						dTmp3 = (dTmp1 / 100.0f) * dTmp2;

						iAP_Abs_Shield = (int)dTmp3;

						iTemp = ctarget->itemEquipStatus[EQUIPPOS_LHAND];
						if (!ctarget->is_neutral())
						{
							EnduStrippingDamage(target, attacker, iTemp, 2000);
						}
					}
				}

				iAP_SM = iAP_SM - (iAP_Abs_Armor + iAP_Abs_Shield);
				if (iAP_SM <= 0) iAP_SM = 1;


				if ((attacker->is_player()) && (cattacker->m_bIsSpecialAbilityEnabled == true))
				{
					switch (cattacker->m_iSpecialAbilityType)
					{
					case 0: break;
					case 1:
						iTemp = (ctarget->health / 2);
						if (iTemp > iAP_SM) iAP_SM = iTemp;
						if (iAP_SM <= 0) iAP_SM = 1;
						break;

					case 2:
						if (ctarget->magic_effect_status[MAGICTYPE_ICE] == 0) {
							ctarget->magic_effect_status[MAGICTYPE_ICE] = 1;
							ctarget->set_status_flag(STATUS_FROZEN, true);
							RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_ICE, dwTime + (30 * 1000),
								target, nullptr, 0, 0, 1, 0, 0);

							SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTON, MAGICTYPE_ICE, 1, 0, 0);
						}
						break;

					case 3:
						if (ctarget->magic_effect_status[MAGICTYPE_HOLDOBJECT] == 0) {
							ctarget->magic_effect_status[MAGICTYPE_HOLDOBJECT] = 2;

							RegisterDelayEvent(DELAYEVENTTYPE_MAGICRELEASE, MAGICTYPE_HOLDOBJECT, dwTime + (10 * 1000),
								ctarget, nullptr, 0, 0, 10, 0, 0);

							SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTON, MAGICTYPE_HOLDOBJECT, 10, 0, 0);
						}
						break;

					case 4:
						iAP_SM = (ctarget->health);
						break;

					case 5:
						ctarget->health += iAP_SM;
						if (ctarget->GetMaxHP() < ctarget->health)
							ctarget->health = ctarget->GetMaxHP();
						SendNotifyMsg(nullptr, ctarget, NOTIFY_HP, 0, 0, 0);
						break;
					}
				}


				if ((attacker->is_player()) && (ctarget->m_bIsSpecialAbilityEnabled == true)) {
					switch (ctarget->m_iSpecialAbilityType) {
					case 50:
						if (cattacker->Equipped.TwoHand.initialized)
							cattacker->Equipped.TwoHand.m_wCurLifeSpan = 0;
						else if (cattacker->Equipped.RightHand.initialized)
							cattacker->Equipped.RightHand.m_wCurLifeSpan = 0;
						break;

					case 51:
						if (iHitPoint == ctarget->m_iSpecialAbilityEquipPos)
							iAP_SM = 0;
						break;

					case 52:
						iAP_SM = 0;
						break;
					}
				}

				if ((ctarget->m_bIsLuckyEffect == true) &&
					(utility::gen_rand(1, 10) < 5) && (ctarget->health <= iAP_SM)) {
					iAP_SM = 1;
				}

				switch (iHitPoint) {
				case 1:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_BODY];
					if (iTemp == -1)
						iTemp = ctarget->itemEquipStatus[EQUIPPOS_FULLBODY];

					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);
					break;

				case 2:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_PANTS];
					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);

					iTemp = ctarget->itemEquipStatus[EQUIPPOS_BOOTS];
					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);
					break;

				case 3:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_ARMS];
					EnduStrippingDamage(target, attacker, iTemp, 2000, shield);
					break;

				case 4:
					iTemp = ctarget->itemEquipStatus[EQUIPPOS_HEAD];
					EnduStrippingDamage(target, attacker, iTemp, 3000, shield);
					break;
				}


				if (cAttackerSA == 2 && ctarget->magic_effect_status[MAGICTYPE_PROTECT])
				{
					SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTOFF, MAGICTYPE_PROTECT, ctarget->magic_effect_status[MAGICTYPE_PROTECT], 0);
					switch (ctarget->magic_effect_status[MAGICTYPE_PROTECT])
					{
					case MAGICPROTECT_PFA:
						ctarget->set_status_flag(STATUS_PFA, false);
						break;
					case MAGICPROTECT_PFM:
						//case MAGICPROTECT_GPFM:
					case MAGICPROTECT_AMP:
						ctarget->set_status_flag(STATUS_PFM, false);
						break;
					case MAGICPROTECT_DS:
					case MAGICPROTECT_GDS:
						ctarget->set_status_flag(STATUS_DEFENSESHIELD, false);
						break;
					}
					ctarget->magic_effect_status[MAGICTYPE_PROTECT] = 0;
					RemoveFromDelayEventList(target, MAGICTYPE_PROTECT);
				}


				if ((ctarget->IsPoisoned() == false) &&//TODO: overwrite poisons? At the moment, a stronger poison gets ignored if any is applied prior.
					((cAttackerSA == 5) || (cAttackerSA == 6) || (cAttackerSA == 61)))
				{
					if (target->CheckResistPoison() == false) {

						if (cAttackerSA == 5)	ctarget->SetPoison(15, dwTime);
						else if (cAttackerSA == 6)  ctarget->SetPoison(40, dwTime);
						else if (cAttackerSA == 61) ctarget->SetPoison(iAttackerSAvalue, dwTime);

						ctarget->set_status_flag(STATUS_POISON, true);
						SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTON, MAGICTYPE_POISON, ctarget->PoisonLevel(), 0);
					}
				}
#ifdef Showdmg
				gserver->m_pClientList[sTargetH]->m_iHP -= iAP_SM;
				if (gserver->m_pClientList[sAttackerH] != 0)
				{
					if (gserver->m_pClientList[sAttackerH]->iDmgShowon == 1)
					{
						ZeroMemory(cDamageMod, sizeof(cDamageMod));
						//wsprintf(cDamageMod ,"You did [%d] Damage to Character [%s] Remaining Life [%d]", iAP_SM,g_game->m_pClientList[sTargetH]->m_cCharName,g_game->m_pClientList[sTargetH]->m_iHP);
						wsprintf(cDamageMod, "You did [%d] Damage to Character [%s]", iAP_SM, gserver->m_pClientList[sTargetH]->m_cCharName);
						gserver->SendNotifyMsg(0, sAttackerH, NOTIFY_NOTICEMSG, 0, 0, 0, cDamageMod);
					}
				}
#else
				if (ctarget->health <= iAP_SM)
					ctarget->health = 0;
				else
					ctarget->health -= iAP_SM;
#endif
				ctarget->m_lastDamageTime = dwTime;
				if (ctarget->health == 0) {

					if (attacker->is_player())
						bAnalyzeCriminalAction(cattacker, ctarget->x, ctarget->y);

					ctarget->KilledHandler(attacker, iAP_SM);
					bKilled = true;
					iKilledDice = ctarget->level;

				}
				else
				{
					if (iAP_SM > 0)
					{

						if (ctarget->m_iAddTransMana > 0)
						{
							dTmp1 = (double)ctarget->m_iAddTransMana;
							dTmp2 = (double)iAP_SM;
							dTmp3 = (dTmp1 / 100.0f)*dTmp2;


							iTemp = (3 * ctarget->GetMag()) + (2 * ctarget->level) + (2 * ctarget->GetInt());
							ctarget->mana += (int)dTmp3;
							if (ctarget->mana > iTemp) ctarget->mana = iTemp;
						}


						if (ctarget->m_iAddChargeCritical > 0)
						{
							if (utility::gen_rand(1, 100) < (ctarget->m_iAddChargeCritical))
							{
								/*iMaxSuperAttack = (g_clientList[sTargetH]->m_iLevel / 10);
								if (g_clientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) g_clientList[sTargetH]->m_iSuperAttackLeft++;
								g_game->SendNotifyMsg(0, sTargetH, NOTIFY_SUPERATTACKLEFT, 0, 0, 0, 0);*/
								if (ctarget->superAttack < MAXSUPERATTACK)
								{
									ctarget->superAttack++;
									SendNotifyMsg(nullptr, ctarget, NOTIFY_SUPERATTACKLEFT, 0, 0, 0);
								}
							}
						}
						SendNotifyMsg(nullptr, ctarget, NOTIFY_HP, 0, 0, 0);

						if (attacker->is_player())
							sAttackerWeapon = ((cattacker->m_sAppr2 & 0x0FF0) >> 4);
						else sAttackerWeapon = 1;


						if ((attacker->is_player()) && (cattacker->map_->flags.fightZone == true))
							iMoveDamage = 125; // Damage you take before you start fly when it xRisenx
						else iMoveDamage = 100;

						if (iAP_SM >= iMoveDamage)
						{

							if (sTgtX == sAtkX)
							{
								if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE;
								else if (sTgtY > sAtkY) cDamageMoveDir = 5;
								else if (sTgtY < sAtkY) cDamageMoveDir = 1;
							}
							else if (sTgtX > sAtkX)
							{
								if (sTgtY == sAtkY)     cDamageMoveDir = 3;
								else if (sTgtY > sAtkY) cDamageMoveDir = 4;
								else if (sTgtY < sAtkY) cDamageMoveDir = 2;
							}
							else if (sTgtX < sAtkX)
							{
								if (sTgtY == sAtkY)     cDamageMoveDir = 7;
								else if (sTgtY > sAtkY) cDamageMoveDir = 6;
								else if (sTgtY < sAtkY) cDamageMoveDir = 8;
							}


							ctarget->m_iLastDamage = iAP_SM;

							SendNotifyMsg(nullptr, ctarget, NOTIFY_DAMAGEMOVE, cDamageMoveDir, iAP_SM, sAttackerWeapon);
						}
						else
						{
						CAE_SKIPDAMAGEMOVE:;
							int iProb;


							if (attacker->is_player())
							{
								switch (cattacker->m_sUsingWeaponSkill) {
								case SKILL_ARCHERY: iProb = 3500; break;
								case SKILL_LONGSWORD: iProb = 1000; break;
								case SKILL_FENCING: iProb = 2900; break;
								case SKILL_AXE: iProb = 2500; break;
								case SKILL_HAMMER: iProb = 2000; break;
								case SKILL_STAFF: iProb = 2000; break;
								default: iProb = 1; break;
								}
							}
							else iProb = 1;

							if (utility::gen_rand(1, 10000) >= iProb)
								SendEventToNearClient_TypeA(ctarget, MSGID_MOTION_DAMAGE, iAP_SM, sAttackerWeapon, 0);
						}

						if (ctarget->magic_effect_status[MAGICTYPE_HOLDOBJECT] == 1) {

							// 1: Hold-Person 
							// 2: Paralyze
							SendNotifyMsg(nullptr, ctarget, NOTIFY_MAGICEFFECTOFF, MAGICTYPE_HOLDOBJECT, ctarget->magic_effect_status[MAGICTYPE_HOLDOBJECT], 0);

							ctarget->magic_effect_status[MAGICTYPE_HOLDOBJECT] = 0;
							RemoveFromDelayEventList(target, MAGICTYPE_HOLDOBJECT);
						}

						ctarget->superAttackMax++;
						if (ctarget->superAttackMax > 14)
						{
							ctarget->superAttackMax = 0;
							/*iMaxSuperAttack = (g_clientList[sTargetH]->m_iLevel / 10);
							if (g_clientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) g_clientList[sTargetH]->m_iSuperAttackLeft++;
							g_game->SendNotifyMsg(0, sTargetH, NOTIFY_SUPERATTACKLEFT, 0, 0, 0);*/
							if (ctarget->superAttack < MAXSUPERATTACK)
							{
								ctarget->superAttack++;
								SendNotifyMsg(nullptr, ctarget, NOTIFY_SUPERATTACKLEFT, 0, 0, 0);
							}
						}
					}
				}
			}
			break;

		case ot_npc:

			if (ntarget->m_cBehavior == BEHAVIOR_DEAD) return 0;
			if (ntarget->dead_ == true) return 0;

			if (m_bIsCrusadeMode == true)
			{
				if (cAttackerSide == ntarget->side)
				{
					switch (ntarget->get_type())
					{
                        case NPC_ESG:
                        case NPC_GMG:
                        case NPC_LWB:
                        case NPC_GHK:
                        case NPC_GHKABS:
                        case NPC_TK:
                        case NPC_BG:
                        case NPC_CP:
                            return 0;

                        default: break;
					}
				}
				else
				{
					switch (ntarget->get_type())
					{
					case NPC_GMG:
						if (cAttackerSide != 0)
						{
							ntarget->m_iV1 += iAP_L;
							if (ntarget->m_iV1 > 300)
							{

								ntarget->m_iV1 -= 300;
								m_mana[ntarget->side] -= 5;
								if (m_mana[ntarget->side] <= 0) m_mana[ntarget->side] = 0;
								//testcode
								//wsprintf(g_cTxt, "ManaStock down: %d", m_mana[g_npcList[sTargetH]->m_side]);
								//PutLogList(g_cTxt);
							}
						}
						break;
					}
				}
			}

			switch (ntarget->m_cActionLimit)
			{
                case 1:
                case 2:
                case 6:
                    return 0;
			}

			if (ntarget->m_cSize == 0)
				iDamage = iAP_SM;
			else iDamage = iAP_L;

			if (ntarget->absDamage < 0)
			{
				dTmp1 = (double)iDamage;
				dTmp2 = (double)(abs(ntarget->absDamage)) / 100.0f;
				dTmp3 = dTmp1 * dTmp2;
				dTmp2 = dTmp1 - dTmp3;
				iDamage = (int)dTmp2;
				if (iDamage < 0) iDamage = 1;
			}


			if (cAttackerSA == 2 && ntarget->magic_effect_status[MAGICTYPE_PROTECT])
			{
				switch (ntarget->magic_effect_status[MAGICTYPE_PROTECT])
				{
					case MAGICPROTECT_PFA:
						ntarget->set_status_flag(STATUS_PFA, false);
						break;
					case MAGICPROTECT_PFM:
						//case MAGICPROTECT_GPFM:
					case MAGICPROTECT_AMP:
						ntarget->set_status_flag(STATUS_PFM, false);
						break;
					case MAGICPROTECT_DS:
					case MAGICPROTECT_GDS:
						ntarget->set_status_flag(STATUS_DEFENSESHIELD, false);
						break;
				}
				ntarget->magic_effect_status[MAGICTYPE_PROTECT] = 0;
				RemoveFromDelayEventList(target, MAGICTYPE_PROTECT);
			}

			// Hero Damage Bonus 1% Per Rank xRisenx
			/*if (cAttackerType == OWNERTYPE_PLAYER && cTargetType == OWNERTYPE_PLAYER)
			{
			int iTitlesAddedDmg = 0;
			iTitlesAddedDmg = ((g_game->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_EK].m_iRank * iDamage)/100);
			}*/
			//#ifdef TitleHG
			//increase damage according to the player's title rank...
			//	note only physical damage PvE can be altered here. PvP, magical is elsewhere.

/*
			if (attacker->m_sType == OWNERTYPE_PLAYER) // well this still makes a correct check that attacker is not NPC. before we ever need to in the title function.. go pro. and it fixed it lol... well this is the fix we had prepared to put into the title code anyway. since attack checks this for us before any call it makes our fix redundant. useless.
			{
				int iTitlesAddedDmg = 0;
				switch (gserver->m_pNpcList[sTargetH]->m_sType) {
				case 10:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SLIME].m_iRank * iDamage) / 20); break;
				case 16:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTANT].m_iRank * iDamage) / 20); break;
				case 22:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_AMPHIS].m_iRank * iDamage) / 20); break;
				case 14:
					if (strcmp(gserver->m_pNpcList[sTargetH]->m_cNpcName, "Orc") == 0) {
						iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ORC].m_iRank * iDamage) / 20);
					}
					else if (strcmp(gserver->m_pNpcList[sTargetH]->m_cNpcName, "Orc-Mage") == 0) {
						iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ORCMAGE].m_iRank * iDamage) / 20);
					}
					break;
				case 18:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ZOMBIE].m_iRank * iDamage) / 20); break;
				case 11:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SKELETON].m_iRank * iDamage) / 20); break;
				case 17:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SCORPION].m_iRank * iDamage) / 20); break;
				case 23:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CLAYGOLEM].m_iRank * iDamage) / 20); break;
				case 12:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_STONEGOLEM].m_iRank * iDamage) / 20); break;
				case 27:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_HELHOUND].m_iRank * iDamage) / 20); break;
				case 57:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTFROG].m_iRank * iDamage) / 20); break;
				case 61:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_RUDOLF].m_iRank * iDamage) / 20); break;
				case 28:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_TROLL].m_iRank * iDamage) / 20); break;
				case 13:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CYCLOPS].m_iRank * iDamage) / 20); break;
				case 65:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ICEGOLEM].m_iRank * iDamage) / 20); break;
				case 53:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_BEHOLDER].m_iRank * iDamage) / 20); break;
				case 60:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CANNIBALPLANT].m_iRank * iDamage) / 20); break;
				case 29:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_OGRE].m_iRank * iDamage) / 20); break;
				case 62:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DIREBOAR].m_iRank * iDamage) / 20); break;
				case 80:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_TENTOCLE].m_iRank * iDamage) / 20); break;
				case 74:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTCRAYFISH].m_iRank * iDamage) / 20); break;
				case 58:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_MOUNTAINGIANT].m_iRank * iDamage) / 20); break;
				case 76:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTPLANT].m_iRank * iDamage) / 20); break;
				case 30:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_LICHE].m_iRank * iDamage) / 20); break;
				case 48:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_STALKER].m_iRank * iDamage) / 20); break;
				case 33:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_WEREWOLF].m_iRank * iDamage) / 20); break;
				case 54:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DARKELF].m_iRank * iDamage) / 20); break;
				case 63:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_FROST].m_iRank * iDamage) / 20); break;
				case 72:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CLAWTURTLE].m_iRank * iDamage) / 20); break;
				case 59:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ETTIN].m_iRank * iDamage) / 20); break;
				case 31:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DEMON].m_iRank * iDamage) / 20); break;
				case 32:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_UNICORN].m_iRank * iDamage) / 20); break;
				case 52:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GARGOYLE].m_iRank * iDamage) / 20); break;
				case 75:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GIANTLIZARD].m_iRank * iDamage) / 20); break;
				case 79:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_NIZIE].m_iRank * iDamage) / 20); break;
				case 49:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_HELLCLAW].m_iRank * iDamage) / 20); break;
				case 50:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_TIGERWORM].m_iRank * iDamage) / 20); break;
				case 66:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_WYVERN].m_iRank * iDamage) / 20); break;
				case 73:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_FIREWYVERN].m_iRank * iDamage) / 20); break;
				case 81:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ABBADON].m_iRank * iDamage) / 20); break;
				case 71:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_CENTAURUS].m_iRank * iDamage) / 20); break;
				case 77:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_MASTERMAGEORC].m_iRank * iDamage) / 20); break;
				case 78:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_MINOTAUR].m_iRank * iDamage) / 20); break;
				case 70:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_BARLOG].m_iRank * iDamage) / 20); break;
				case 104:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DARKUNICORN].m_iRank * iDamage) / 20); break;
				case 105:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_BLACKDEMON].m_iRank * iDamage) / 20); break;
				case 106:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_EARTHWYVERN].m_iRank * iDamage) / 20); break;
				case 107:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_WINDWYVERN].m_iRank * iDamage) / 20); break;
				case 108:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_DARKWYVERN].m_iRank * iDamage) / 20); break;
				case 109:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_GRASSWYVERN].m_iRank * iDamage) / 20); break;
				case 110:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SKYWYVERN].m_iRank * iDamage) / 20); break;
				case 111:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_SEAWYVERN].m_iRank * iDamage) / 20); break;
				case 112:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_METALWYVERN].m_iRank * iDamage) / 20); break;
				case 113:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_VOIDWYVERN].m_iRank * iDamage) / 20); break;
				case 114:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ETERNALDRAGON].m_iRank * iDamage) / 20); break;
				case 122:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDTROLL].m_iRank * iDamage) / 20); break;
				case 123:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDCYCLOPS].m_iRank * iDamage) / 20); break;
				case 124:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDSTALKER].m_iRank * iDamage) / 20); break;
				case 125:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDGARGOYLE].m_iRank * iDamage) / 20); break;
				case 126:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDHELLCLAW].m_iRank * iDamage) / 20); break;
				case 127:  iTitlesAddedDmg = ((gserver->m_pClientList[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_ENRAGEDTIGERWORM].m_iRank * iDamage) / 20); break; // if it had any value allocated, the trace would not hide it. i say his design fails to initialize a value to m_iRank in some cases. the variable has been declared but never assigned. might be always worked without any crash or error for over 3 months lol....... DO NOT MAKE ANOTHER EXCUSE FOR THIS NOOBS CODE! I SEE NOTHING BUT BUGS HERE MAN same.. Lets burn ze source :D i'LL BRING THE PETROL yes
					//case xxxxx:  iTitlesAddedDmg = ((g_game->[sAttackerH]->m_stTitles[TITLE_INDEX_MOB_].m_iRank * iDamage)/20); break;
				default: break;
				}
				if (iTitlesAddedDmg > 0)//just in case
					iDamage += iTitlesAddedDmg;

				if (gserver->m_pClientList[sAttackerH] != NULL){
					if (gserver->m_pClientList[sAttackerH]->iDmgShowon == 2){
						char cTitlesMsg[256];
						ZeroMemory(cTitlesMsg, sizeof(cTitlesMsg));
						wsprintf(cTitlesMsg, "StartDmg (%d),  TBonus (%d),  Total (%d),  Remaining Life (%d)", (iDamage - iTitlesAddedDmg), iTitlesAddedDmg, iDamage, gserver->m_pNpcList[sTargetH]->m_iHP);
						gserver->SendNotifyMsg(NULL, sAttackerH, NOTIFY_NOTICEMSG, NULL, NULL, NULL, cTitlesMsg);
					}
				}


				//int iTitlesAddedDmg = g_game->getTitlesMobBonusDamage(sAttackerH, sTargetH, iDamage);
				//iDamage += iTitlesAddedDmg;
			} // Alll that i marked now was commented before .. as i changed it to this but i moved back to this when it crashes as i think this was the error.. and it might have been because i remember i had this bug before and then i added this
*/



			//#endif
#ifdef Showdmg
			//switch (g_game->m_pNpcList[sTargetH]->m_cActionLimit) {
			//case 0:
			//case 3:
			//case 5:
			g_npcList[sTargetH]->ReduceHP(iDamage);
			//{
			//	//g_game->m_pNpcList[sTargetH]->m_iHP -= iDamage;
			//if (g_game->m_pClientList[sAttackerH] != NULL){
			//if (g_game->m_pClientList[sAttackerH]->iDmgShowon == 1){
			//ZeroMemory(cDamageMod, sizeof(cDamageMod));
			//wsprintf(cDamageMod ,"You did [%d] Damage to Npc [%s] Remaining Life [%d]", iDamage,g_game->m_pNpcList[sTargetH]->m_cNpcName,g_game->m_pNpcList[sTargetH]->m_iHP);
			//g_game->SendNotifyMsg(NULL, sAttackerH, NOTIFY_NOTICEMSG, NULL, NULL, NULL, cDamageMod);
			//}
			//}
			//break;
			//}
			//}
#else
			ntarget->ReduceHP(5000/*iDamage*/);
			SendEventToNearClient_TypeA(target, MSGID_MOTION_DAMAGE, iDamage, sAttackerWeapon, 0);
			
#endif
			if (ntarget->health == 0)
            {
                std::shared_ptr<npc> npc_ = std::static_pointer_cast<npc>(target);
                ntarget->health = 0;
				if (attacker->is_player())
					target->map_->NpcKilledHandler(attacker, npc_, iDamage);
				else
					target->map_->NpcKilledHandler(attacker, npc_, iDamage);

				bKilled = true;
                iKilledDice = npc_->m_iHitDice;
			}
			/*
			else {

				if ((g_npcList[sTargetH]->m_sType != 21) && (g_npcList[sTargetH]->m_sType != 55) && (g_npcList[sTargetH]->m_sType != 56)
					&& (g_npcList[sTargetH]->m_side == cAttackerSide)) goto CAE_SKIPCOUNTERATTACK;


				if (g_npcList[sTargetH]->m_cActionLimit != 0) goto CAE_SKIPCOUNTERATTACK;
				if (g_npcList[sTargetH]->m_bIsPermAttackMode == true) goto CAE_SKIPCOUNTERATTACK;
				if ((g_npcList[sTargetH]->m_bIsSummoned == true) &&
					(g_npcList[sTargetH]->m_iSummonControlMode == 1)) goto CAE_SKIPCOUNTERATTACK;

				if (g_npcList[sTargetH]->m_sType == NPC_CP) goto CAE_SKIPCOUNTERATTACK;

				if (utility::gen_rand(1, 3) == 2) {

					if (g_npcList[sTargetH]->m_cBehavior == BEHAVIOR_ATTACK) {
						tX = tY = 0;
						switch (g_npcList[sTargetH]->m_cTargetType) {
						case OWNERTYPE_PLAYER:
							if (g_clientList[g_npcList[sTargetH]->m_iTargetIndex] != NULL) {
								tX = g_clientList[g_npcList[sTargetH]->m_iTargetIndex]->m_sX;
								tY = g_clientList[g_npcList[sTargetH]->m_iTargetIndex]->m_sY;
							}
							break;

						case OWNERTYPE_NPC:
							if (g_npcList[g_npcList[sTargetH]->m_iTargetIndex] != NULL) {
								tX = g_npcList[g_npcList[sTargetH]->m_iTargetIndex]->m_sX;
								tY = g_npcList[g_npcList[sTargetH]->m_iTargetIndex]->m_sY;
							}
							break;
						}
						iDst1 = (g_npcList[sTargetH]->m_sX - tX)*(g_npcList[sTargetH]->m_sX - tX) + (g_npcList[sTargetH]->m_sY - tY)*(g_npcList[sTargetH]->m_sY - tY);

						tX = tY = 0;
						switch (cAttackerType) {
						case OWNERTYPE_PLAYER:
							if (g_clientList[sAttackerH] != NULL) {
								tX = g_clientList[sAttackerH]->m_sX;
								tY = g_clientList[sAttackerH]->m_sY;
							}
							break;

						case OWNERTYPE_NPC:
							if (g_npcList[sAttackerH] != NULL) {
								tX = g_npcList[sAttackerH]->m_sX;
								tY = g_npcList[sAttackerH]->m_sY;
							}
							break;
						}


						iDst2 = (g_npcList[sTargetH]->m_sX - tX)*(g_npcList[sTargetH]->m_sX - tX) + (g_npcList[sTargetH]->m_sY - tY)*(g_npcList[sTargetH]->m_sY - tY);

						if (iDst2 <= iDst1) {

							g_npcList[sTargetH]->m_cBehavior = BEHAVIOR_ATTACK;
							g_npcList[sTargetH]->m_sBehaviorTurnCount = 0;
							g_npcList[sTargetH]->m_iTargetIndex = sAttackerH;
							g_npcList[sTargetH]->m_cTargetType = cAttackerType;
						}
					}
					else {
						g_npcList[sTargetH]->m_cBehavior = BEHAVIOR_ATTACK;
						g_npcList[sTargetH]->m_sBehaviorTurnCount = 0;
						g_npcList[sTargetH]->m_iTargetIndex = sAttackerH;
						g_npcList[sTargetH]->m_cTargetType = cAttackerType;
					}
				}

			CAE_SKIPCOUNTERATTACK:;

				// Stun
				if ((utility::gen_rand(1, (g_npcList[sTargetH]->dwGoldDropValue / 55) + 3) == 1) &&
					(g_npcList[sTargetH]->m_cActionLimit == 0))
					g_npcList[sTargetH]->m_dwTime = dwTime;


				if (cAttackerType == OWNERTYPE_PLAYER)
					sAttackerWeapon = ((g_clientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
				else sAttackerWeapon = 1;

				if ((wWeaponType < 40) && (g_npcList[sTargetH]->m_cActionLimit == 4)) {

					if (sTgtX == sAtkX) {
						if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE2;
						else if (sTgtY > sAtkY) cDamageMoveDir = 5;
						else if (sTgtY < sAtkY) cDamageMoveDir = 1;
					}
					else if (sTgtX > sAtkX) {
						if (sTgtY == sAtkY)     cDamageMoveDir = 3;
						else if (sTgtY > sAtkY) cDamageMoveDir = 4;
						else if (sTgtY < sAtkY) cDamageMoveDir = 2;
					}
					else if (sTgtX < sAtkX) {
						if (sTgtY == sAtkY)     cDamageMoveDir = 7;
						else if (sTgtY > sAtkY) cDamageMoveDir = 6;
						else if (sTgtY < sAtkY) cDamageMoveDir = 8;
					}

					dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) {

						cDamageMoveDir = utility::gen_rand(1, 8);
						dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

						if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) goto CAE_SKIPDAMAGEMOVE2;
					}

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->ClearOwner(/ *5,* / sTargetH, OWNERTYPE_NPC, g_npcList[sTargetH]->m_sX, g_npcList[sTargetH]->m_sY);

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, OWNERTYPE_NPC, dX, dY);
					g_npcList[sTargetH]->m_sX = dX;
					g_npcList[sTargetH]->m_sY = dY;
					g_npcList[sTargetH]->m_cDir = cDamageMoveDir;

					gserver->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_MOVE, NULL, NULL, NULL);

					dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
					dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

					if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) {

						cDamageMoveDir = utility::gen_rand(1, 8);
						dX = g_npcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
						dY = g_npcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

						if (g_mapList[g_npcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, NULL) == false) goto CAE_SKIPDAMAGEMOVE2;
					}

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->ClearOwner(/ *5,* / sTargetH, OWNERTYPE_NPC, g_npcList[sTargetH]->m_sX, g_npcList[sTargetH]->m_sY);

					g_mapList[g_npcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, OWNERTYPE_NPC, dX, dY);
					g_npcList[sTargetH]->m_sX = dX;
					g_npcList[sTargetH]->m_sY = dY;
					g_npcList[sTargetH]->m_cDir = cDamageMoveDir;

					gserver->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_MOVE, NULL, NULL, NULL);

					if (gserver->bCheckEnergySphereDestination(sTargetH, sAttackerH, cAttackerType) == true) {
						if (cAttackerType == OWNERTYPE_PLAYER) {
							iExp = (g_npcList[sTargetH]->m_iExp / 3);
							if (g_npcList[sTargetH]->m_iNoDieRemainExp > 0)
								iExp += g_npcList[sTargetH]->m_iNoDieRemainExp;


							if (g_clientList[sAttackerH]->m_iAddExp != NULL) {
								dTmp1 = (double)g_clientList[sAttackerH]->m_iAddExp;
								dTmp2 = (double)iExp;
								dTmp3 = (dTmp1 / 100.0f)*dTmp2;
								iExp += (int)dTmp3;
							}

							// Crusade
							if ((gserver->m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;

							//g_clientList[sAttackerH]->m_iExpStock += iExp;
							gserver->GetExp(sAttackerH, iExp);


							gserver->DeleteNpc(sTargetH);
							return false;
						}
					}

				CAE_SKIPDAMAGEMOVE2:;
				}
				else {
					gserver->SendEventToNearClient_TypeA(sTargetH, OWNERTYPE_NPC, MSGID_MOTION_DAMAGE, iDamage, sAttackerWeapon, NULL);
				}

				if (g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] == 1) {

					// 1: Hold-Person 
					g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] = NULL;
					gserver->RemoveFromDelayEventList(sTargetH, OWNERTYPE_NPC, MAGICTYPE_HOLDOBJECT);
				}
				else if (g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] == 2) {
					// 2: Paralize
					if ((g_npcList[sTargetH]->m_iHitDice > 50) && (utility::gen_rand(1, 10) == 5)) {
						g_npcList[sTargetH]->m_cMagicEffectStatus[MAGICTYPE_HOLDOBJECT] = NULL;
						gserver->RemoveFromDelayEventList(sTargetH, OWNERTYPE_NPC, MAGICTYPE_HOLDOBJECT);
					}
				}


				if ((g_npcList[sTargetH]->m_iNoDieRemainExp > 0) && (g_npcList[sTargetH]->m_bIsSummoned != true) &&
					(cAttackerType == OWNERTYPE_PLAYER) && (g_clientList[sAttackerH] != NULL)) {


					if (g_npcList[sTargetH]->m_iNoDieRemainExp > iDamage) {
						iExp = iDamage;
						g_npcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
					}
					else {
						iExp = g_npcList[sTargetH]->m_iNoDieRemainExp;
						g_npcList[sTargetH]->m_iNoDieRemainExp = 0;
					}


					if (g_clientList[sAttackerH]->m_iAddExp != NULL) {
						dTmp1 = (double)g_clientList[sAttackerH]->m_iAddExp;
						dTmp2 = (double)iExp;
						dTmp3 = (dTmp1 / 100.0f)*dTmp2;
						iExp += (int)dTmp3;
					}

					if (gserver->m_bIsCrusadeMode == true) iExp = iExp / 3;


					if (g_clientList[sAttackerH]->m_iLevel > 100) {
						switch (g_npcList[sTargetH]->m_sType) {
						case 55:
						case 56:
							iExp = 0;
							break;
						default: break;
						}
					}

				}
			}*/
			break;
		}

		//CAE_SKIPCOUNTERATTACK:;

/*
		if (cAttackerType == OWNERTYPE_PLAYER) {

			if (g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND] != -1)
				sWeaponIndex = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
			else sWeaponIndex = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_RHAND];

			if (sWeaponIndex != -1) {
				if ((g_clientList[sAttackerH]->m_pItemList[sWeaponIndex] != NULL) &&
					(g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_sIDnum != ITEM_PICKAXE)) {
					if (bKilled == false)
						CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, 1);
					else {
						if (g_clientList[sAttackerH]->m_iHP <= 3)
							CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, utility::gen_rand(1, iKilledDice) * 2);
						else CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, utility::gen_rand(1, iKilledDice));
					}
				}

				if ((g_clientList[sAttackerH]->m_pItemList[sWeaponIndex] != NULL) &&
					(g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wMaxLifeSpan != 0))
				{
					iWepLifeOff = 1;
					if ((wWeaponType >= 1) && (wWeaponType < 40)) {
						switch (g_mapList[g_clientList[sAttackerH]->m_cMapIndex]->m_weather)
						{
						case WEATHER_LIGHTRAIN:		if (utility::gen_rand(1, 3) == 1) iWepLifeOff++; break;
						case WEATHER_MEDIUMRAIN:	if (utility::gen_rand(1, 2) == 1) iWepLifeOff += utility::gen_rand(1, 2); break;
						case WEATHER_HEAVYRAIN:		if (utility::gen_rand(1, 2) == 1) iWepLifeOff += utility::gen_rand(1, 3); break;
						}
					}

					if (!g_clientList[sAttackerH]->IsNeutral()) {
						if (g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan < iWepLifeOff)
							g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
						else g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan -= iWepLifeOff;
					}

					if (g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan == 0) {

						gserver->SendNotifyMsg(NULL, sAttackerH, NOTIFY_ITEMLIFESPANEND, g_clientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_cEquipPos, sWeaponIndex, NULL, NULL);

						gserver->ReleaseItemHandler(sAttackerH, sWeaponIndex, true);
					}
				}
			}
			else {
				if (wWeaponType == 0) {
					CalculateSSN_SkillIndex(sAttackerH, SKILL_HANDATTACK, 1);
				}
			}
		}*/
	}
	else {
/*
		if (cAttackerType == OWNERTYPE_PLAYER) {
			g_clientList[sAttackerH]->m_iComboAttackCount = 0;
		}*/
	}

	return iExp;
}

void gserver::EnduStrippingDamage(std::shared_ptr<unit> target, std::shared_ptr<unit> attacker, int item, int higherStripEndu, bool shield /*= false*/)
{
	int equiptItem = 0, enduDamage = 1;
	float stripChance = 0;

/*
	if (g_clientList[sTargetH] == NULL) return;
	if (cAttackerType == OWNERTYPE_PLAYER && g_clientList[sAttackerH] == NULL) return;
	if (item == -1 || g_clientList[sTargetH]->m_pItemList[item] == NULL) return;

	if (g_clientList[sTargetH]->m_bIsSpecialAbilityEnabled == true &&
		g_clientList[sTargetH]->m_iSpecialAbilityType == 52) { //Mer shield active
		return;
	}

	if (cAttackerType == OWNERTYPE_PLAYER &&
		(g_clientList[sTargetH]->m_side != g_clientList[sAttackerH]->m_side || g_mapList[g_clientList[sAttackerH]->m_cMapIndex]->m_bIsFightZone))
	{
		switch (g_clientList[sAttackerH]->m_sUsingWeaponSkill)
		{
		case SKILL_HAMMER:
			stripChance = 0.25;
			enduDamage = 10;
			equiptItem = g_clientList[sAttackerH]->m_sItemEquipmentStatus[EQUIPPOS_TWOHAND];
			if (equiptItem == -1 || g_clientList[sAttackerH]->m_pItemList[equiptItem] == NULL) break;

			switch (g_clientList[sAttackerH]->m_pItemList[equiptItem]->m_sIDnum)
			{ // Reworked Stripping on Hammers
			case ITEM_DAMEHAMMER:
				stripChance = 0.20f;
				enduDamage = 200;
				break;
			case ITEM_MASTERHAMMER:
				stripChance = 0.25f;
				enduDamage = 350;
				break;
			case ITEM_LEGENDHAMMER:
				stripChance = 0.45f;
				enduDamage = 600;
				break;
			case ITEM_GBHAMMER:
				stripChance = 0.55f;
				enduDamage = 950;
				break;
				/ *case ITEM_AGBHAMMER:
				stripChance = 0.60f;
				enduDamage = 1000;
				break;* /
			}
			break;

		case SKILL_AXE:
			enduDamage = 8; // Changed DMG to end on armors from 5 to 8 xRisenx
			break;

		default:
			enduDamage = 1;
			break;
		}
	}


	if (shield)
	{
		enduDamage = ceil(enduDamage * 0.2);
	}
	else if (g_clientList[sTargetH]->m_pItemList[item]->m_cCategory == 5) // shields
	{
		enduDamage = ceil(enduDamage * 0.8);
	}

	if (!g_clientList[sTargetH]->IsNeutral() &&
		g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan > 0 &&
		memcmp(g_clientList[sTargetH]->m_pItemList[item]->m_cName, "Fantasy", 6) != 0) {
		g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan -= enduDamage;
	}

	if (g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan <= 0) {
		g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan = 0;
		gserver->SendNotifyMsg(NULL, sTargetH, NOTIFY_ITEMLIFESPANEND, g_clientList[sTargetH]->m_pItemList[item]->m_cEquipPos, item, NULL, NULL);
		gserver->ReleaseItemHandler(sTargetH, item, true);
	}
	else if (stripChance > 0)
	{
		switch (g_clientList[sTargetH]->m_pItemList[item]->m_sIDnum)
		{
		case ITEM_MERIENPLATEMAIL_M:
		case ITEM_MERIENPLATEMAIL_W:
			return;
		}

		//if ( g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan <  higherStripEndu )
		if (g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan <  25000)
			stripChance *= utility::gen_rand(6, (g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan - g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan));
		else
			stripChance *= utility::gen_rand(4, (g_clientList[sTargetH]->m_pItemList[item]->m_wMaxLifeSpan - g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan));

		if (stripChance >  g_clientList[sTargetH]->m_pItemList[item]->m_wCurLifeSpan) {
			gserver->ReleaseItemHandler(sTargetH, item, true);
			gserver->SendNotifyMsg(NULL, sTargetH, NOTIFY_ITEMRELEASED, g_clientList[sTargetH]->m_pItemList[item]->m_cEquipPos, item, NULL, NULL);
		}
	}*/
}

int gserver::_iCalcSkillSSNpoint(int iLevel)
{
	int iRet = 0;

	if (iLevel < 1) return 1;

	if (iLevel <= 50) // Skill Gain Ratio xRisenx
		iRet = iLevel;
	else if (iLevel > 95)
		iRet = (iLevel * iLevel) / 10;
	else
		iRet = iLevel;

	return iRet;
}

void gserver::_CheckMiningAction(client * client_, int dX, int dY)
{
/*
	int16_t sType;
	uint64_t dwRegisterTime;
	int32_t   iDynamicIndex, iSkillLevel, iResult;
	int32_t   iMineralID = ITEM_COAL;
	Item * pItem;
	uint16_t  wWeaponType;

	if (client == nullptr)  return;


	client_->RemoveMagicEffect(MAGICTYPE_INVISIBILITY);


	client_->pMap->bGetDynamicObject(dX, dY, &sType, &dwRegisterTime, &iDynamicIndex);

	switch (sType) {
	case DYNAMICOBJECT_MINERAL1:
	case DYNAMICOBJECT_MINERAL2:

		wWeaponType = ((client_->m_sAppr2 & 0x0FF0) >> 4);
		if (wWeaponType == 25) {
		}
		else return;

		if ((client_->m_sAppr2 & 0xF000) == 0) return;

#ifdef TitleHG
		iSkillLevel = (client_->m_stTitles[TITLE_INDEX_CRAFT_MINING].m_iRank * 3) + client_->m_iAddMining; // New Skill % To Items xRisenx

#else
		iSkillLevel = client_->m_cSkillMastery[SKILL_MINING];
		if (iSkillLevel == 0) break;

		if (m_pDynamicObjectList[iDynamicIndex] == NULL) break;
#endif

		iSkillLevel -= m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iDifficulty;
#ifdef TitleHG
		if (iSkillLevel < 4) iSkillLevel = 4;
#else
		if (iSkillLevel <= 0) iSkillLevel = 1;
#endif
		iResult = utility::gen_rand(1, 100);
		if (iResult <= iSkillLevel) {
#ifdef TitleHG
			increaseTitlePoints(iClientH, TITLE_INDEX_CRAFT_MINING, 1);
#else
			CalculateSSN_SkillIndex(client_, SKILL_MINING, 1);
#endif

			switch (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_cType) {
			case 1:
				switch (utility::gen_rand(1, 6)) {
				case 1:
				case 2:
				case 3:
				case 4:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 5:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 6:
					iMineralID = ITEM_BLONDESTONE; // BlondeStone
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				}
				break;

			case 2:
				switch (utility::gen_rand(1, 7)) {
				case 1:
				case 2:
				case 3:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 4:
				case 5:
				case 6:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 7:
					if (utility::gen_rand(1, 3) == 2) {
						iMineralID = ITEM_SILVERNUGGET; // SilverNugget
						GetExp(iClientH, utility::gen_rand(1, 4));
					}
					else {
						iMineralID = ITEM_BLONDESTONE; // BlondeStone
						GetExp(iClientH, utility::gen_rand(1, 3));
					}
					break;
				}
				break;

			case 3:
				switch (utility::gen_rand(1, 6)) {
				case 1:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 6:
					if (utility::gen_rand(1, 8) == 3) {
						if (utility::gen_rand(1, 2) == 1) {
							iMineralID = ITEM_SILVERNUGGET; // SilverNugget
							GetExp(iClientH, utility::gen_rand(1, 4));
						}
						else {
							iMineralID = ITEM_BLONDESTONE; // BlondeStone
							GetExp(iClientH, utility::gen_rand(1, 3));
						}
						break;
					}
					else {
						iMineralID = ITEM_IRONORE; // IronOre
						GetExp(iClientH, utility::gen_rand(1, 3));
					}
					break;
				}
				break;

			case 4:
				switch (utility::gen_rand(1, 6)) {
				case 1:
				case 2:
					iMineralID = ITEM_COAL; // Coal
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 3:
					if (utility::gen_rand(1, 3) == 2) {
						iMineralID = ITEM_SILVERNUGGET; // SilverNugget
						GetExp(iClientH, utility::gen_rand(1, 4));
					}
					else{
						iMineralID = ITEM_COAL; // Coal
						GetExp(iClientH, utility::gen_rand(1, 3));
					}
					break;
				case 4:
				case 5:
					iMineralID = ITEM_IRONORE; // IronOre
					GetExp(iClientH, utility::gen_rand(1, 3));
					break;
				case 6:
					if (utility::gen_rand(1, 3) == 3)
					{
						// 0.33333
						if (utility::gen_rand(1, 2) == 1)
						{
							// 0.5
							if (utility::gen_rand(1, 2) == 1)
							{
								// 0.5
								iMineralID = ITEM_MITHRAL;
								GetExp(iClientH, utility::gen_rand(1, 30));
							}
							else {
								if (utility::gen_rand(1, 100) <= 75)
								{
									// 0.75
									iMineralID = ITEM_GOLDNUGGET;
									GetExp(iClientH, utility::gen_rand(1, 5));
								}
								else {
									// 0.25
									iResult = utility::gen_rand(1, 10000);
									if (iResult <= 1600)
									{
										iResult = utility::gen_rand(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_ARMORGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_ARMORGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_ARMORGEM21;
									}
									else if (iResult <= 3200)
									{
										iResult = utility::gen_rand(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_MINDGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_MINDGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_MINDGEM21;
									}
									else if (iResult <= 6300)
									{
										iResult = utility::gen_rand(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_BLOODGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_BLOODGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_BLOODGEM21;
									}
									else if (iResult <= 9400)
									{
										iResult = utility::gen_rand(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_REJUGEM7;
										else if (iResult <= 9980)
											iMineralID = ITEM_REJUGEM14;
										else if (iResult <= 10000)
											iMineralID = ITEM_REJUGEM21;
									}
									else if (iResult <= 9700)
									{
										iResult = utility::gen_rand(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_TACTICALGEM3;
										else if (iResult <= 9980)
											iMineralID = ITEM_TACTICALGEM5;
										else if (iResult <= 10000)
											iMineralID = ITEM_TACTICALGEM7;
									}
									else if (iResult <= 9999)
									{
										iResult = utility::gen_rand(1, 10000);
										if (iResult <= 7000)
											iMineralID = ITEM_ENCHANTEDGEM2;
										else if (iResult <= 9980)
											iMineralID = ITEM_ENCHANTEDGEM4;
										else if (iResult <= 10000)
											iMineralID = ITEM_ENCHANTEDGEM6;
									}
									else if (iResult <= 10000)
									{
										iMineralID = ITEM_VORTEXGEM;
									}

									GetExp(iClientH, utility::gen_rand(1, 60));
								}
							}
							break;
						}
						else {
							// 0.5
							iMineralID = ITEM_SILVERNUGGET; // SilverNugget
							GetExp(iClientH, utility::gen_rand(1, 4));
						}
						break;
					}
					else
					{
						// 0.77777
						if (utility::gen_rand(1, 2) == 1)
						{
							iMineralID = ITEM_GOLDNUGGET; // GoldNugget
							GetExp(iClientH, utility::gen_rand(1, 5));
						}
						else {
							iMineralID = ITEM_IRONORE;  // IronOre
							GetExp(iClientH, utility::gen_rand(1, 3));
						}
						break;
					}
					break;
				}
				break;

			case 5:
				switch (utility::gen_rand(1, 19)) {
				case 3:
					iMineralID = ITEM_SAPPHIRE; // Sapphire
					GetExp(iClientH, utility::gen_rand(2, 3));
					break;
				default:
					iMineralID = ITEM_CRYSTAL; // Crystal
					GetExp(iClientH, utility::gen_rand(2, 3));
					break;
				}
				break;

			case 6:
				switch (utility::gen_rand(1, 5)) {
				case 1:
					if (utility::gen_rand(1, 6) == 3) {
						iMineralID = ITEM_EMERALD; // Emerald 
						GetExp(iClientH, utility::gen_rand(2, 4));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal
						GetExp(iClientH, utility::gen_rand(2, 3));
					}
					break;
				case 2:
					if (utility::gen_rand(1, 6) == 3) {
						iMineralID = ITEM_SAPPHIRE; // Saphire
						GetExp(iClientH, utility::gen_rand(2, 4));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal
						GetExp(iClientH, utility::gen_rand(2, 3));
					}
					break;
				case 3:
					if (utility::gen_rand(1, 6) == 3) {
						iMineralID = ITEM_RUBY; // Ruby
						GetExp(iClientH, utility::gen_rand(2, 4));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal 
						GetExp(iClientH, utility::gen_rand(2, 3));
					}
					break;
				case 4:
					iMineralID = ITEM_CRYSTAL; // Crystal 
					GetExp(iClientH, utility::gen_rand(2, 3));
					break;
				case 5:
					if (utility::gen_rand(1, 12) == 3) {
						iMineralID = ITEM_DIAMOND; // Diamond
						GetExp(iClientH, utility::gen_rand(2, 5));
					}
					else {
						iMineralID = ITEM_CRYSTAL; // Crystal
						GetExp(iClientH, utility::gen_rand(2, 3));
					}
					break;
				}
				break;

			}

			pItem = new class CItem;
			if (pItem->InitItemAttr(iMineralID) == false) {
				delete pItem;
			}
			else {

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
					m_pClientList[iClientH]->m_sY, pItem);

				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
			}

			m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain--;
			if (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain <= 0) {
				bDeleteMineral(m_pDynamicObjectList[iDynamicIndex]->m_iV1);
				delete m_pDynamicObjectList[iDynamicIndex];
				m_pDynamicObjectList[iDynamicIndex] = NULL;
			}
		}
		break;

	default:
		break;
	}*/
}

bool gserver::bDeleteMineral(int iIndex)
{
/*
	int iDynamicIndex;
	uint32_t dwTime;

	dwTime = utility::current_time();

	if (m_pMineral[iIndex] == NULL) return false;
	iDynamicIndex = m_pMineral[iIndex]->m_sDynamicObjectHandle;
	if (m_pDynamicObjectList[iDynamicIndex] == NULL) return false;

	SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex,
		m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY,
		m_pDynamicObjectList[iDynamicIndex]->m_sType, iDynamicIndex, NULL);
	m_pMapList[m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex]->SetDynamicObject(NULL, NULL, m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, dwTime);
	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->SetTempMoveAllowedFlag(m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, true);

	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->m_iCurMineral--;

	delete m_pMineral[iIndex];
	m_pMineral[iIndex] = NULL;*/

	return true;
}

void gserver::GetExp(std::shared_ptr<client> client_, uint64_t iExp, bool bIsAttackerOwn /*= false*/)
{
	double dV1, dV2, dV3;
	int iUnitValue;
	uint64_t dwTime = utility::current_time();

	if (client_ == nullptr) return;

	if (client_->level <= 80) {
		dV1 = (double)(80 - client_->level);
		dV2 = dV1 * 0.025f;
		dV3 = (double)iExp;
		dV1 = (dV2 + 1.025f)*dV3;
		iExp = (int)dV1;
	}
	else
	{
		if ((client_->level >= 100) && (client_->map_->name == "arefarm")
			|| (client_->map_->name == "elvfarm"))
		{
			iExp = (iExp / 10);
		}
		else if ((client_->map_->name == "arefarm")
			|| (client_->map_->name == "elvfarm"))
		{
			iExp = (iExp * 1 / 4);
		}
	}

	party * party = client_->GetParty();

	if (dwTime - client_->m_dwLastActionTime >= 5*60*1000)
		return;

	if (party)
	{
		if (iExp >= 10 && party->get_count() > 1)
		{
			int iPartyTotalMember = party->get_alive_count();

			dV1 = (double)iExp;

			if (iPartyTotalMember == 1)	dV2 = dV1;
			else dV2 = (dV1 * ((std::log((float)(iPartyTotalMember + 2) * 9) / 9) + 0.65)) / iPartyTotalMember;

			dV3 = dV2 + 0.5f;
			iUnitValue = (int)dV3;

			//TODO: turn into for ( : )
			while (client * member = party->GetMembers())
			{
				if (member->is_dead())
					continue;

				uint32_t exp = 0;
				if (member->level == PLAYERMAXLEVEL)
					exp += (iUnitValue / 3);
				else
					exp += iUnitValue;

				if ((member->status & STATUS_GREENSLATE) != 0)
					exp *= 3;

				member->m_iExpStock += exp;
			}

			if ((client_->status & STATUS_GREENSLATE) != 0) iUnitValue *= 3;
			if (client_->level == PLAYERMAXLEVEL)
				iUnitValue /= 3;
			if ((bIsAttackerOwn == true) && (iPartyTotalMember > 1))
				client_->m_iExpStock += (int)(iUnitValue / 10);

		}
		else
		{
			if ((client_->status & STATUS_GREENSLATE) != 0) iExp *= 3;
			if (client_->level == PLAYERMAXLEVEL)
				iExp /= 3;
			client_->m_iExpStock += iExp;
		}
	} // if
	else
	{
		if ((client_->status & STATUS_GREENSLATE) != 0) iExp *= 3;
		if (client_->level == PLAYERMAXLEVEL)
			iExp /= 3;
		client_->m_iExpStock += iExp;
	}

	// Xp System xRisenx
	if (client_->level >= 1 && client_->level < 205)
		iExp *= 100;
	client_->m_iExpStock += iExp;
	//else if (m_pClientList[iClientH]->m_iLevel >= 50 && m_pClientList[iClientH]->m_iLevel <= 99)
	//	iExp *= 3;
	//else if (m_pClientList[iClientH]->m_iLevel >= 100 && m_pClientList[iClientH]->m_iLevel <= 149)
	//	iExp *= 4;
	//else if (m_pClientList[iClientH]->m_iLevel >= 150 && m_pClientList[iClientH]->m_iLevel < 175)
	//	iExp *= 5;
	//if (m_pClientList[iClientH]->m_iLevel >= 1 && m_pClientList[iClientH]->m_iLevel < 200)
	//	iExp *= 17; // Raised a little xRisenx


}

int gserver::iAddDynamicObjectList(unit * owner, short sType, map * pmap, short sX, short sY, uint32_t dwLastTime, int iV1)
{
/*
	int i;
	short sPreType;
	uint32_t dwTime, dwRegisterTime;

	m_pMapList[cMapIndex]->bGetDynamicObject(sX, sY, &sPreType, &dwRegisterTime);
	if (sPreType != NULL) return NULL;

	switch (sType) {
		case DYNAMICOBJECT_FIRE:
			if (m_pMapList[cMapIndex]->bGetIsMoveAllowedTile(sX, sY) == false)
				return NULL;

			if (dwLastTime != NULL)  {
				switch (m_pMapList[cMapIndex]->m_weather) {
					case WEATHER_LIGHTRAIN:		dwLastTime = dwLastTime - (dwLastTime / 2);       break;
					case WEATHER_MEDIUMRAIN:	dwLastTime = (dwLastTime / 2) - (dwLastTime / 3); break;
					case WEATHER_HEAVYRAIN:		dwLastTime = (dwLastTime / 3) - (dwLastTime / 4); break;
				}

				if (dwLastTime == NULL) dwLastTime = 1000;
			}
			break;

		case DYNAMICOBJECT_FISHOBJECT:
		case DYNAMICOBJECT_FISH:
			if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == false)
				return NULL;
			break;

		case DYNAMICOBJECT_MINERAL1:
		case DYNAMICOBJECT_MINERAL2:
			if (m_pMapList[cMapIndex]->bGetMoveable(sX, sY) == false)
				return NULL;
			m_pMapList[cMapIndex]->SetTempMoveAllowedFlag(sX, sY, false);
			break;
		case DYNAMICOBJECT_ARESDENFLAG:
		case DYNAMICOBJECT_ELVINEFLAG:
			m_pMapList[cMapIndex]->SetTempMoveAllowedFlag(sX, sY, false);
			break;
	}


	for (i = 1; i < MAXDYNAMICOBJECTS; i++)
	{
		if (m_pDynamicObjectList[i] == NULL)
		{
			dwTime = utility::current_time();

			if (dwLastTime != NULL)
				dwLastTime += (utility::gen_rand(1, 4) * 1000);

			// VAMP - here the dynamic object is given the caster's ownerId and ownerType
			m_pDynamicObjectList[i] = new DynamicObject(sOwner, cOwnerType, sType, cMapIndex, sX, sY, dwTime, dwLastTime, iV1);
			m_pMapList[cMapIndex]->SetDynamicObject(i, sType, sX, sY, dwTime);
			SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, MSGTYPE_CONFIRM, cMapIndex, sX, sY, sType, i, NULL);

			return i;
		}
	}
*/

	return 0;
}

void gserver::CheckDynamicObjectList()
{
/*
	int i;
	uint32_t dwTime = utility::current_time(), dwRegisterTime;
	short sType;

	for (i = 1; i < MAXDYNAMICOBJECTS; i++) {
		if (m_pDynamicObjectList[i] && m_pDynamicObjectList[i]->m_dwLastTime != 0)
		{

			switch (m_pDynamicObjectList[i]->m_sType)
			{
				case DYNAMICOBJECT_FIRE:

					switch (m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_weather)
					{
						case WEATHER_LIGHTRAIN:
						case WEATHER_MEDIUMRAIN:
						case WEATHER_HEAVYRAIN:
							m_pDynamicObjectList[i]->m_dwLastTime = m_pDynamicObjectList[i]->m_dwLastTime -
								(m_pDynamicObjectList[i]->m_dwLastTime / 10) * m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_weather;
							break;
					}
					break;
			}

			if (dwTime - m_pDynamicObjectList[i]->m_dwRegisterTime >= m_pDynamicObjectList[i]->m_dwLastTime)
			{
				m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->bGetDynamicObject(m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, &sType, &dwRegisterTime);

				if (dwRegisterTime == m_pDynamicObjectList[i]->m_dwRegisterTime) {
					SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, MSGTYPE_REJECT, m_pDynamicObjectList[i]->m_cMapIndex, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, m_pDynamicObjectList[i]->m_sType, i, NULL);
					m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->SetDynamicObject(NULL, NULL, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, dwTime);
				}

				switch (sType)
				{
					case DYNAMICOBJECT_FISHOBJECT:
					case DYNAMICOBJECT_FISH:
						bDeleteFish(m_pDynamicObjectList[i]->m_sOwner, 2);
						break;
				}

				delete m_pDynamicObjectList[i];
				m_pDynamicObjectList[i] = NULL;
			}
		}
	}*/
}

bool gserver::bAnalyzeCriminalAction(std::shared_ptr<client> client_, short dX, short dY, bool bIsCheck /*= false*/)
{
	uint16_t tX, tY;
	string cNpcName;
	char  cNpcWaypoint[11];

	if (client_ == nullptr) return false;
	if (client_->m_bIsInitComplete == false) return false;
	if (m_bIsCrusadeMode == true) return false;
	if (client_->map_->flags.fightZone) return false;

	std::shared_ptr<unit> target = client_->map_->GetOwner(dX, dY);

	if ((target->is_player()) && (target != nullptr))
	{
		if (_bGetIsPlayerHostile(client_, std::static_pointer_cast<client>(target)) != true)
		{
			if (bIsCheck == true) return true;

			if (client_->map_->name == sideMap[faction::aresden])
				cNpcName = "Guard-Aresden";
			else if (client_->map_->name == sideMap[faction::elvine])
				cNpcName = "Guard-Elvine";
			else  if (client_->map_->name == sideMap[faction::neutral])
				cNpcName = "Guard-Neutral";
			else
				return false;

			memset(cNpcWaypoint, 0, sizeof(cNpcWaypoint));

			tX = (int)client_->x;
			tY = (int)client_->y;

			std::shared_ptr<npc> guard = client_->map_->CreateNpc(cNpcName, 0, MOVETYPE_RANDOM,
									 &tX, &tY, (faction)-1, cNpcWaypoint, 0, 0, false, true);

			if (guard)
			{
				guard->SetTarget(client_->shared_from_this());
			}
		}
	}

	return false;
}

int gserver::_iGetWeaponSkillType(std::shared_ptr<client> client_)
{
	uint16_t wWeaponType;

	if (client_ == nullptr) return 0;

	wWeaponType = ((client_->m_sAppr2 & 0x0FF0) >> 4);

	if (wWeaponType == 0) {
		return 5;
	}
	else if ((wWeaponType >= 1) && (wWeaponType <= 2)) {

		return 7;
	}
	else if ((wWeaponType > 2) && (wWeaponType < 20)) {
		if (wWeaponType == 7)
			return 9;
		else return 8;
	}
	else if ((wWeaponType >= 20) && (wWeaponType < 30)) {
		return 10;
	}
	else if ((wWeaponType >= 30) && (wWeaponType < 35)) {

		return 14;
	}
	else if ((wWeaponType >= 35) && (wWeaponType < 40)) {

		return 21;
	}
	else if (wWeaponType >= 40) {
		return 6;
	}

	return 1;
}

//                             1 2 3 
static int ___iCAB5[] = { 0, 0, 0, 1, 2 };
static int ___iCAB6[] = { 0, 0, 0, 0, 0 };
static int ___iCAB7[] = { 0, 0, 1, 2, 3 };
static int ___iCAB8[] = { 0, 0, 1, 3, 5 };
static int ___iCAB9[] = { 0, 0, 2, 4, 8 };
static int ___iCAB10[] = { 0, 0, 1, 2, 3 };

int16_t gserver::iGetComboAttackBonus(int16_t iSkill, int16_t iComboCount)
{
	if (iComboCount <= 1) return 0;
	if (iComboCount > 6) return 0;
	switch (iSkill) {
		case 5: 		return ___iCAB5[iComboCount];
			break;
		case 6: 		return ___iCAB6[iComboCount];
			break;
		case 7:
			return ___iCAB7[iComboCount];
			break;
		case 8: 		return ___iCAB8[iComboCount];
			break;
		case 9:
			return ___iCAB9[iComboCount];
			break;
		case 10: 		return ___iCAB10[iComboCount];
			break;
		case 14:
			return ___iCAB6[iComboCount];
			break;
		case 21:
			return ___iCAB10[iComboCount];
			break;
	}

	return 0;
}

void gserver::RequestRestartHandler(std::shared_ptr<client> player)
{
	if (!player->dead_) return;

	//perform buff/debuff removals for non-persistents
	//player->OnDeath(); //or something similar
	if (player->magic_effect_status[MAGICTYPE_CONFUSE] != 0)
		player->remove_magic_effect(MAGICTYPE_CONFUSE);


	if (player->IsGM())
	{
		//GM respawns in place by default
		//do nothing
	}
	else
	{
		//Player is in enemy town and is not an admin, send to jail and lock for 3 minutes
		if (player->is_ares())
		{
			if (player->map_name == sideMap[faction::elvine])
			{
				player->lockedMapName = sideMapJail[faction::elvine];
				player->lockedMapTime = lockedMapTimeDefault;
				player->map_name = sideMapJail[faction::elvine];
			}
			else if (player->level < farmRestartLimit)
			{
				player->map_name = sideMapFarm[faction::aresden];
			}
		}
		else if (player->is_elv())
		{
			if (player->map_name == sideMap[faction::aresden])
			{
				player->lockedMapName = sideMapJail[faction::aresden];
				player->lockedMapTime = lockedMapTimeDefault;
				player->map_name = sideMapJail[faction::aresden];
			}
			else if (player->level < farmRestartLimit)
			{
				player->map_name = sideMapFarm[faction::elvine];
			}
		}
		else
		{
			player->map_name = sideMap[faction::neutral];
		}
	}


	player->dead_ = false;
	player->health = player->GetMaxHP();
	player->m_iHungerStatus = 100;

	player->map_->ClearDeadOwner(player->x, player->y);

	RequestTeleportHandler(player, 2, player->map_name);
}

void gserver::RemoveFromDelayEventList(std::shared_ptr<unit> unit, int32_t iEffectType)
{
// 	delayMutex.lock();
// 	std::list<std::shared_ptr<delay_event>> removedelaylist;
// 
// 	for (const std::shared_ptr<delay_event> & delayevent : DelayEventList)
// 	{
// 		if (iEffectType != 0)
// 		{
// 			if ((delayevent->target->handle == unit->handle) && (delayevent->target->get_type() == unit->get_type()) && (delayevent->m_iEffectType == iEffectType))
// 			{
// 				removedelaylist.push_back(delayevent);
// 			}
// 		}
// 		else
// 		{
// 			if ((delayevent->target->handle == unit->handle) && (delayevent->target->get_type() == unit->get_type()))
// 			{
// 				removedelaylist.push_back(delayevent);
// 			}
// 		}
// 	}
// 
// 	for (const std::shared_ptr<delay_event> & delayevent : removedelaylist)
// 	{
// 		DelayEventList.remove(delayevent);
// 	}
// 	delayMutex.unlock();
}

bool gserver::RegisterDelayEvent(int iDelayType, int iEffectType, uint64_t dwLastTime, std::shared_ptr<unit> unit, map * pmap, int dX, int dY, int iV1, int iV2, int iV3)
{
	std::shared_ptr<delay_event> delayevent(new delay_event);


	delayevent->m_iDelayType = iDelayType;
	delayevent->m_iEffectType = iEffectType;

	delayevent->m_pMap = pmap;
	delayevent->m_dX = dX;
	delayevent->m_dY = dY;

	delayevent->target = unit;
	delayevent->m_iV1 = iV1;
	delayevent->m_iV2 = iV2;
	delayevent->m_iV3 = iV3;

	delayevent->m_dwTriggerTime = dwLastTime;

	delayMutex.lock();
	DelayEventList.push_back(delayevent);
	delayMutex.unlock();

	return true;
}

//NOTE: if this becomes a bit bloaty, move Unit timers to their respective classes
//needs live testing to see if this would cause any issues with potentially thousands of delay events
//constantly firing and being checked every ~100ms
void gserver::DelayEventProcessor()
{
// 	uint64_t _time = utility::current_time();
// 	delayMutex.lock();
// 	std::vector<std::shared_ptr<delay_event>> tempDelayEventList = DelayEventList;
// 	for (const auto & delayevent : tempDelayEventList)
// 	{
// 		if (delayevent->m_dwTriggerTime < _time)
// 		{
// 			switch (delayevent->m_iDelayType)
// 			{
// 			case DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT:
// 				//CalcMeteorStrikeEffectHandler(delayevent->map);//good ol' HB handlers for handling handlers
// 				break;
// 
// 			case DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE:
// 				//DoMeteorStrikeDamageHandler(delayevent->map);
// 				break;
// 
// 			case DELAYEVENTTYPE_METEORSTRIKE:
// 				//MeteorStrikeHandler(delayevent->map);
// 				break;
// 
// 			case DELAYEVENTTYPE_USEITEM_SKILL:
// 				if (delayevent->target != nullptr && delayevent->target->is_player())
// 				{
// 					int skillnum = delayevent->m_iEffectType;
// 
// 					std::shared_ptr<client> player = std::static_pointer_cast<client>(delayevent->target);
// 
// 					if (player->skillInUse[skillnum] == false || player->skillInUseTime[skillnum] != delayevent->m_iV2) break;
// 
// 					player->skillInUse[skillnum] = false;
// 					player->skillInUseTime[skillnum] = 0;
// 
// 					int32_t result = CalculateUseSkillItemEffect(player, delayevent->m_iV1, skillnum, player->map_, delayevent->m_dX, delayevent->m_dY);
// 
// 					SendNotifyMsg(nullptr, player, NOTIFY_SKILLUSINGEND, result);
// 				}
// 				break;
// 
// 			case DELAYEVENTTYPE_DAMAGEOBJECT:
// 				break;
// 			case DELAYEVENTTYPE_MAGICRELEASE:
// 				break;
// 			}
// 
// 			DelayEventList.remove(delayevent);
// 		}
// 	}
// 	delayMutex.unlock();
}

int32_t gserver::CalculateUseSkillItemEffect(std::shared_ptr<client> player, int16_t skillvalue, int skillnum, map * map_, int32_t x, int32_t y)
{
// 	int32_t dX, dY, itemid = 0;
// 	if (player == nullptr) return 0;
// 	if (player->map_ != map_) return 0;
// 	if (skillnum == 0) return 0;
// 	if (skillvalue == 0) return 0;
// 	if (m_pSkillConfigList[skillnum] == nullptr) return 0;
// 	dX = player->x;
// 	dY = player->y;
// 
// 	int result = utility::gen_rand(1, 105);
// 	if (skillvalue <= result) return 0;
// 
// 	switch (m_pSkillConfigList[skillnum]->m_sType)
// 	{
// 	case SKILLEFFECTTYPE_TAMING:
// 		//_TamingHandler(player, skillnum, map, dX, dY);//??
// 		break;
// 	case SKILLEFFECTTYPE_GET:
// 		switch (m_pSkillConfigList[skillnum]->m_sValue1)
// 		{
// 			case 1:
// 				itemid = 99;
// 				break;
// 			case 2:
// 			{
// 				if (!map_->bGetIsWater(dX, dY)) return 0;
// 				int fish = 0;// iCheckFish(player, map, dX, dY);
// 				if (fish == 0) itemid = 100;
// 
// 				if (itemid != 0)
// 				{
// 					if (itemid == 100)
// 					{
// 						SendNotifyMsg(nullptr, player, NOTIFY_FISHSUCCESS);
// 						GetExp(player, utility::gen_rand(2, 5));
// 
// 						item * item_ = new item();
// 						if (item_->InitItemAttr(itemid, m_pItemConfigList))
// 						{
// 							map_->bSetItem(dX, dY, item_);
// 							SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, COMMONTYPE_ITEMDROP, map_, dX, dY, item_->m_sSprite, item_->m_sSpriteFrame, item_->m_cItemColor);
// 						}
// 					}
// 				}
// 			}
// 		}
// 		break;
// 	}
	return 1;
}

}
