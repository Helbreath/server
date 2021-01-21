
#include "party.hpp"
#include "client.hpp"

namespace hbx
{

party::party(uint32_t id) : _id(id)
{
}

party::~party()
{
	// 	while(Client * member = GetMembers())
	// 	{
	// 		member->Notify(NULL, NOTIFY_PARTY, 2, 0, NULL, NULL);
	// 		member->SetParty( NULL );
	// 	}
}

void party::add_player(client * player)
{
	// 	_players.push_front( player );
	// 
	// 	NotifyMemberList( player );
	// 	//NotifyPartyStatus( player );
	// 	player->SetParty( this );
	// 
	// 	while(Client * member = GetMembers())
	// 	{
	// 		member->Notify(NULL, NOTIFY_PARTY, 4, 1, NULL, player->m_cCharName);
	// 
	// 		if(member == player || player->m_cMapIndex != member->m_cMapIndex)
	// 			continue;
	// 
	// 		member->Notify( player->m_handle, NOTIFY_PARTY_COORDS, 
	// 			player->m_sX, player->m_sY, NULL, player->m_cCharName );
	// 	}
	// 
	// 	UpdateGuildBonuses();
}

bool party::withdraw_player(client * player)
{
	// 	if (!player) return _players.empty(); // Added Check xRisenx
	// 
	// 	while(Client * member = GetMembers())
	// 	{
	// 		member->Notify(NULL, NOTIFY_PARTY, 6, 1, NULL, player->m_cCharName);
	// 	}
	// 
	// 	_RemovePlayer(player);
	// 
	// 	if(_players.size() == 1)
	// 	{
	// 		ClientListIter iter = _players.begin();
	// 		Client * solePlayer = *iter;
	// 		_RemovePlayer(solePlayer);
	// 
	// 		solePlayer->Notify(NULL, NOTIFY_PARTY, 2, 0, NULL, NULL);
	// 	} else {
	// 		UpdateGuildBonuses();
	// 	}
	// 
	// 	return _players.empty();
	return false;
}

void party::_RemovePlayer(client * player)
{
	// 	_players.remove( player );
	// 	player->SetParty( NULL );
}

void party::notify_member_list(client * player) const
{
	// 	char * index = packet;
	// 
	// 	while(CClient * member = GetMembers())
	// 	{
	// 		if(member == player)
	// 			continue;
	// 		Push(index, member->m_cCharName, 11);
	// 	}
	// 	player->Notify(NULL, NOTIFY_PARTY, 5, 1, _players.size() - 1, packet);
}

void party::update_member_coords(client * player) const
{
	// 	if(player->m_partyCoordSteps >= 4)
	// 	{
	// 		player->m_partyCoordSteps = 0;
	// 
	// 		while(CClient * member = GetMembers())
	// 		{
	// 			if(member == player || player->m_cMapIndex != member->m_cMapIndex)
	// 				continue;
	// 
	// 			member->Notify(player->m_handle, NOTIFY_PARTY_COORDS, 
	// 				player->m_sX, player->m_sY, NULL, player->m_cCharName);
	// 		}
	// 	}
	// 
	// 	player->m_partyCoordSteps++;
}

void party::update_member_map(client * player) const
{
	// 	while(Client * member = GetMembers())
	// 	{
	// 		if(member == player)
	// 			continue;
	// 
	// 		// debug tests
	// 		ASSERT(member->m_pXSock);
	// 		if(!member->m_pXSock)
	// 		{
	// 			sprintf(g_cTxt, "m_pXSock == NULL Player(%s) Party Size(%u) SameParty(%u) PartyStatus(%u) Map(%s %u-%u)", 
	// 				member->m_cCharName, GetPlayerCnt(), member->GetParty() == this, member->GetPartyStatus(), 
	// 				member->m_cMapName, member->m_sX, member->m_sY);
	// 			ERR(g_cTxt);
	// 			continue;
	// 		}
	// 
	// 		if(player->m_cMapIndex == member->m_cMapIndex)
	// 		{
	// 			player->Notify(member->m_handle, NOTIFY_PARTY_COORDS, 
	// 				member->m_sX, member->m_sY, NULL, member->m_cCharName);
	// 			member->Notify(player->m_handle, NOTIFY_PARTY_COORDS, 
	// 				player->m_sX, player->m_sY, NULL, player->m_cCharName);
	// 		}
	// 		else
	// 		{
	// 			member->Notify(player->m_handle, NOTIFY_PARTY_COORDS, 0, 0, NULL, player->m_cCharName);
	// 		}
	// 	}
}

void party::update_guild_bonuses()
{
	// 	_gldHuntmasters.clear();
	// 	_gldRaidmasters.clear();
	// 
	// 	while(CClient * player = GetMembers())
	// 	{
	// 		if(!player->m_guild) continue;
	// 
	// 		switch( player->m_iGuildRank )
	// 		{
	// 		case GUILDRANK_RAIDMASTER:
	// 			_gldRaidmasters[ player->m_guild ].push_front( player );
	// 			break;
	// 
	// 		case GUILDRANK_HUNTMASTER:
	// 			_gldHuntmasters[ player->m_guild ].push_front( player );
	// 			break;
	// 
	// 		default:
	// 			continue;
	// 		}
	// 	}
}

uint16_t party::get_alive_count() const
{
	// 	uint32_t n = 0;
	// 
	// 	for(ClientListCIter iter = _players.begin();
	// 		iter != _players.end();
	// 		++iter)
	// 	{
	// 		if( !(*iter)->IsDead() )
	// 			n++;
	// 	}
	// 
	// 	return n; 
	return 0;
}

client * party::GetMembers() const
{
	// 	static bool getting = false;
	// 	static ClientListCIter iter;
	// 
	// 	if(getting)
	// 	{
	// 		if(iter == _players.end())
	// 		{
	// 			getting = false;
	// 			return NULL;
	// 		}
	// 
	// 		return (*iter++);
	// 	}
	// 	else
	// 	{
	// 		iter = _players.begin();
	// 
	// 		if(iter == _players.end())
	// 			return NULL;
	// 
	// 		getting = true;
	// 		return (*iter++);
	// 	}
	return nullptr;
}

client * party::GetHuntmasters(guild * guild_)
{
	// 	static bool getting = false;
	// 	static ClientListCIter iter, end;
	// 
	// 	if(_gldHuntmasters.count(guild) == 0)
	// 		return NULL;
	// 
	// 	if(getting)
	// 	{
	// 		if(iter == end)
	// 		{
	// 			getting = false;
	// 			return NULL;
	// 		}
	// 
	// 		return (*iter++);
	// 	}
	// 	else
	// 	{
	// 		iter = _gldHuntmasters[guild].begin();
	// 		end = _gldHuntmasters[guild].end();
	// 
	// 		if(iter == end)
	// 			return NULL;
	// 
	// 		getting = true;
	// 		return (*iter++);
	// 	}
	return nullptr;
}

client * party::GetRaidmasters(guild * guild_)
{
	// 	static bool getting = false;
	// 	static ClientListCIter iter, end;
	// 
	// 	if(_gldRaidmasters.count(guild) == 0)
	// 		return NULL;
	// 
	// 	if(getting)
	// 	{
	// 		if(iter == end)
	// 		{
	// 			getting = false;
	// 			return NULL;
	// 		}
	// 
	// 		return (*iter++);
	// 	}
	// 	else
	// 	{
	// 		iter = _gldRaidmasters[guild].begin();
	// 		end = _gldRaidmasters[guild].end();
	// 
	// 		if(iter == end)
	// 			return NULL;
	// 
	// 		getting = true;
	// 		return (*iter++);
	// 	}
	return nullptr;
}

void party::ping_map(const client * pinger, int16_t x, int16_t y, uint32_t pinguid) const
{
	// 	foreach(it, _players)
	// 	{
	// 		Client * player = *it;
	// 		if(player == pinger || player->m_cMapIndex != pinger->m_cMapIndex || player->m_pinguid == pinguid)
	// 			continue;
	// 
	// 		player->Notify( NULL, NOTIFY_PINGMAP, PINGMAP_PARTY, x, y); 
	// 		player->m_pinguid = pinguid;
	// 	}
}

}
