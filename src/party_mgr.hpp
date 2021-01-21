#pragma once

#include <unordered_map>
#include <cstdint>

namespace hbx
{

class client;
class party;

class party_mgr
{
public:
	party_mgr(void);
	~party_mgr(void);

	party * create_party(client * player = 0, client * player2 = 0);
	party * GetParty(uint32_t id) { return _parties[id]; }
	void remove_from_party(client * player);
	void delete_party(uint32_t id);

	bool PartyExists(uint32_t id)	const { return (_parties.find(id) != _parties.end()); }
protected:
	std::unordered_map<uint32_t, party *> _parties;

	uint64_t _idCnt;
	uint64_t _GetNewID() { return ++_idCnt; }
};

}
