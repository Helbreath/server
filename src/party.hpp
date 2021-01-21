
#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <memory>
#include "defines.hpp"

namespace hbx
{

class client;
class guild;

class party
{
public:
	party(uint32_t id);
	~party();

	uint16_t get_count() const { return uint16_t(_players.size()); }
	uint16_t get_alive_count() const;
	client * GetMembers() const;
	client * GetHuntmasters(guild * guild_);
	client * GetRaidmasters(guild * guild_);
	void add_player(client * player);
	bool withdraw_player(client * player);
	void notify_member_list(client * player) const;
	void update_member_coords(client * player) const;
	void update_member_map(client * player) const;
	void update_guild_bonuses();

	void ping_map(const client * pinger, int16_t x, int16_t y, uint32_t pinguid) const;

	uint32_t GetID()	const { return _id; }
private:
	void _RemovePlayer(client * player);

	std::map<guild *, std::vector<std::shared_ptr<client>>> _gldHuntmasters;
	std::map<guild *, std::vector<std::shared_ptr<client>>> _gldRaidmasters;

	std::vector<std::shared_ptr<client>> _players;
	uint32_t _id;
};

}
