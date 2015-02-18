#pragma once

#include "Client.h"
#include "Party.h"
#include <unordered_map>

class PartyMgr
{
public:
	PartyMgr(void);
	~PartyMgr(void);

	Party * CreateParty(Client * player = 0, Client * player2 = 0);
	Party * GetParty(uint32_t id)		{ return _parties[id]; }
	void RemoveFromParty(Client * player);
	void DeleteParty(uint32_t id);

	bool PartyExists(uint32_t id)	const { return ( _parties.find( id ) != _parties.end() ); }
protected:
	std::unordered_map< uint32_t, Party* > _parties;

	uint64_t _idCnt;
	uint64_t _GetNewID() { return ++_idCnt; }
};
