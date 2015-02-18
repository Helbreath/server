#pragma once

#include "Client.h"

#define GUILDPARTYBONUS_RANGE_X		FLAGRANGE_X
#define GUILDPARTYBONUS_RANGE_Y		FLAGRANGE_Y

class Party
{
public:
	Party(uint32_t id);
	~Party();

	uint8_t GetPlayerCnt()	const { return uint8_t(_players.size()); }
	uint8_t GetAlivePlayerCnt()	const;
	Client * GetMembers() const;
	Client * GetHuntmasters(Guild * guild);
	Client * GetRaidmasters(Guild * guild);
	void AddPlayer(Client * player);
	bool WithdrawPlayer(Client * player);
	void NotifyMemberList(Client * player) const;
	void UpdateMemberCoords(Client * player) const;
	void UpdateMemberMap(Client * player) const;
	void UpdateGuildBonuses();

	void PingMap(const Client * pinger, int16_t x, int16_t y, uint32_t pinguid) const;

	uint32_t GetID()	const { return _id; }
private:
	void _RemovePlayer(Client * player);

	std::map< Guild*, std::list<shared_ptr<Client>> > _gldHuntmasters;
	std::map< Guild*, std::list<shared_ptr<Client>> > _gldRaidmasters;

	std::list<shared_ptr<Client>> _players;
	uint32_t _id;
};
