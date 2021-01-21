#pragma once

#include "itembag.hpp"

namespace hbx
{

class client;

struct stGuildMemberPermissions
{
    bool canInvite = false;
    bool canKick = false;
    bool canChangeMotd = false;
    bool canSetTeleport = false;//crusade teleport location
    bool canSummon = false;//unsure
    bool canPromote = false;
    bool canDemote = false;
};
struct stGuildRank
{
    uint8_t rank;//rankid applied to members
    uint8_t order;//order in the guild list
    std::string name;
    stGuildMemberPermissions permission;
};
struct stGuildMember
{
    stGuildMember() { playerclass = 0; level = 0; lastonline = 0; }
    std::string name;
    uint8_t playerclass;
    uint16_t level;
    uint64_t lastonline;
    std::shared_ptr<client> player;
    std::shared_ptr<stGuildRank> rank;
};

class guild
{
public:
	guild();
	~guild();

	bool Save();
	bool PromoteUser(const std::shared_ptr<client> & client_, std::string character);
	bool DemoteUser(const std::shared_ptr<client> & client_, std::string character);
	bool AddUser(const std::shared_ptr<client> & client_);
	void SendChatMessage(std::string & message, uint8_t order = 0);
	bool CanInvite(const std::shared_ptr<client> & client_);
	bool CanKick(const std::shared_ptr<client> & client_);
	bool CanChangeMotd(const std::shared_ptr<client> & client_);
	bool CanSetTeleport(const std::shared_ptr<client> & client_);
	bool CanSummon(const std::shared_ptr<client> & client_);
	bool CanPromote(const std::shared_ptr<client> & client_);
	bool CanDemote(const std::shared_ptr<client> & client_);
	bool SetDefaultRank(uint8_t newrank);

	std::shared_ptr<stGuildMember> GetMember(std::string name);

	uint64_t uid;
	std::string name;
	std::string leader;

	//std::vector<stGuildMemberPermissions> permissionlist;
	std::vector<std::shared_ptr<stGuildMember>> memberlist;
	std::vector<std::shared_ptr<stGuildRank>> ranklist;

	std::vector<itembag> itemBagList;

private:
	std::shared_ptr<stGuildRank> _defaultrank;
};

}
