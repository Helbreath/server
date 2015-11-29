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
		string name;
	};
	struct stGuildMember
	{
		stGuildMember() { pRank = 0; playerclass = 0; level = 0; lastonline = 0; rank = 0; }
		string name;
		uint8_t playerclass;
		uint16_t level;
		uint64_t lastonline;
		shared_ptr<Client> player;
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
	bool PromoteUser(const shared_ptr<Client> & client, string character);
	bool DemoteUser(const shared_ptr<Client> & client, string character);
	bool AddUser(const shared_ptr<Client> & client);
	void SendChatMessage(string & message, uint8_t order = 0);
	bool CanInvite(const shared_ptr<Client> & client);
	bool CanKick(const shared_ptr<Client> & client);
	bool CanChangeMotd(const shared_ptr<Client> & client);
	bool CanSetTeleport(const shared_ptr<Client> & client);
	bool CanSummon(const shared_ptr<Client> & client);
	bool CanPromote(const shared_ptr<Client> & client);
	bool CanDemote(const shared_ptr<Client> & client);
	bool SetDefaultRank(uint8_t newrank);

	stGuildMember * GetMember(string name);

	uint64_t uid;
	string name;
	string leader;

	std::vector<stGuildMemberPermissions> permissionlist;
	std::vector<stGuildMember*> memberlist;
	std::vector<stGuildRank*> ranklist;

private:
	stGuildRank * _defaultrank;
};

