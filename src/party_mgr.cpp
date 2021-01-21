
#include "party_mgr.hpp"
#include "client.hpp"
#include "party.hpp"
#include "utility.hpp"

namespace hbx
{

party_mgr::party_mgr(void)
{
    _idCnt = utility::current_time();
}

party_mgr::~party_mgr(void)
{
}

party * party_mgr::create_party(client * player, client * player2)
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

void party_mgr::delete_party(uint32_t id)
{
    // 	PartyMapCIter iter = _parties.find( id );
    // 	if(iter != _parties.end())
    // 	{
    // 		delete (*iter).second;
    // 		_parties.erase( iter );
    // 	}
}

void party_mgr::remove_from_party(client * player)
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

}
