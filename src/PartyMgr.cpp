
#include "PartyMgr.h"


PartyMgr::PartyMgr(void)
{
	_idCnt = unixtime();
}

PartyMgr::~PartyMgr(void)
{
}

Party * PartyMgr::CreateParty(Client * player, Client * player2)
{
// 	CParty * party = new CParty( _GetNewID() );
// 
// 	_parties[ party->GetID() ] = party;
// 
// 	if(player)
// 	{
// 		ASSERT(player->m_pXSock);
// 		party->AddPlayer( player );
// 	}
// 
// 	if(player2)
// 	{
// 		ASSERT(player->m_pXSock);
// 		party->AddPlayer( player2 );
// 	}
// 
// 	return party;
 	return 0;
}

void PartyMgr::DeleteParty(uint32_t id)
{
// 	PartyMapCIter iter = _parties.find( id );
// 	if(iter != _parties.end())
// 	{
// 		delete (*iter).second;
// 		_parties.erase( iter );
// 	}
}

void PartyMgr::RemoveFromParty(Client * player)
{
// 	if(!player->GetParty()) 
// 		return;
// 
// 	PartyMapCIter iter = _parties.find( player->GetParty()->GetID() );
// 	if(iter != _parties.end() && (*iter).second->WithdrawPlayer( player ) )
// 	{
// 		delete (*iter).second;
// 		_parties.erase( iter );
// 	}
}
