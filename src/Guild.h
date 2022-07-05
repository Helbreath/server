#pragma once

#include "common.h"

class Client;

class Guild
{
public:
	Guild();
	~Guild();

	struct stGuildRank
	{
		uint8_t rank;//rankid applied to members
		uint8_t order;//order in the guild list
		std::string name;
	};
	struct stGuildMember
	{
		stGuildMember() { pRank = 0; playerclass = 0; level = 0; lastonline = 0; rank = 0; }
		std::string name;
		uint8_t playerclass;
		uint16_t level;
		uint64_t lastonline;
		std::shared_ptr<Client> player;
		uint8_t rank;
		Guild::stGuildRank * pRank;
	};
	struct stGuildMemberPermissions
	{
		uint8_t rank;
		bool canInvite;
		bool canKick;
		bool canChangeMotd;
		bool canSetTeleport;//crusade teleport location
		bool canSummon;//unsure
		bool canPromote;
		bool canDemote;
	};


	bool Save();
	bool PromoteUser(const std::shared_ptr<Client> & client, std::string character);
	bool DemoteUser(const std::shared_ptr<Client> & client, std::string character);
	bool AddUser(const std::shared_ptr<Client> & client);
	void SendChatMessage(std::string & message, uint8_t order = 0);
	bool CanInvite(const std::shared_ptr<Client> & client);
	bool CanKick(const std::shared_ptr<Client> & client);
	bool CanChangeMotd(const std::shared_ptr<Client> & client);
	bool CanSetTeleport(const std::shared_ptr<Client> & client);
	bool CanSummon(const std::shared_ptr<Client> & client);
	bool CanPromote(const std::shared_ptr<Client> & client);
	bool CanDemote(const std::shared_ptr<Client> & client);
	bool SetDefaultRank(uint8_t newrank);

	stGuildMember * GetMember(std::string name);

	uint64_t uid;
	std::string name;
	std::string leader;

	std::vector<stGuildMemberPermissions> permissionlist;
	std::vector<stGuildMember*> memberlist;
	std::vector<stGuildRank*> ranklist;

private:
	stGuildRank * _defaultrank;
};

