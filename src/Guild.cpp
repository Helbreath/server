
#include "Guild.h"
#include "Client.h"


Guild::Guild()
{
}


Guild::~Guild()
{
	for (auto member : memberlist)
		delete member;
}

bool Guild::Save()
{
	return false;
}

bool Guild::PromoteUser(const std::shared_ptr<Client> & client, std::string target)
{
	if (!CanPromote(client))
		return false;
	stGuildMember * player = GetMember(target);
	stGuildMember * invoker = GetMember(client->name);
	if (!player)
	{
		//member is not in the guild
		//TODO: send message of failure
		return false;
	}
	if (player->player != nullptr)
	{
		uint8_t _ot = player->pRank->order;
		uint8_t _it = invoker->pRank->order;
		if (_ot-1 < _it)
		{
			//rank being promoted to is still under invoker's

			//perform promote
			for (auto rank : ranklist)
			{
				if (rank->rank == _ot - 1)
				{
					//
					player->pRank = rank;
					player->rank = rank->rank;
					player->player->guildRank = rank->rank;
					//TODO: send message of success to guild
					return true;
				}
			}
		}
		else
		{
			//cannot promote to your rank or higher
			//TODO: send message of failure
			return false;
		}
	}
	else
	{
		//player is not online - only allow promotion of online players?
		//TODO: send message of failure
		return false;
	}
	return false;
}

bool Guild::DemoteUser(const std::shared_ptr<Client> & client, std::string character)
{
	if (!CanPromote(client))
		return false;
	return false;
}

bool Guild::AddUser(const std::shared_ptr<Client> & client)
{
	//user already in a guild - should never trigger here on live
	if (client->guild != nullptr)
	{
		__debugbreak();
		return false;
	}


	client->guildGUID = uid;
	client->guildRank = 1;

	stGuildMember * newmember = new stGuildMember;
	newmember->lastonline = 0;
	newmember->level = client->level;
	newmember->name = client->name;
	newmember->player = client;
	//newmember->playerclass = client->playerclass;
	newmember->pRank = _defaultrank;
	newmember->rank = newmember->pRank->rank;
	memberlist.push_back(newmember);


	//send message to guild?


	//update client with new guild params here? or in server?
	return true;
}

void Guild::SendChatMessage(std::string & message, uint8_t order)
{
	for (auto member : memberlist)
	{
		if (member->pRank->order < order)
		{
			//member.player->Notify(nullptr, );//TODO: send chat message
		}
	}
}

Guild::stGuildMember * Guild::GetMember(std::string name)
{
	for (auto member : memberlist)
	{
		if (member->player->name == name)
		{
			return member;
		}
	}
	return nullptr;
}

//TODO: perhaps make this 'easier' in the future? inline functions should suffice for now
inline bool Guild::CanInvite(const std::shared_ptr<Client> & client)
{
	for (auto & permission : permissionlist)
		if ((permission.rank == client->guildRank) && (permission.canInvite))
			return true;
	return false;
}

inline bool Guild::CanKick(const std::shared_ptr<Client> & client)
{
	for (auto & permission : permissionlist)
		if ((permission.rank == client->guildRank) && (permission.canKick))
			return true;
	return false;
}

inline bool Guild::CanChangeMotd(const std::shared_ptr<Client> & client)
{
	for (auto & permission : permissionlist)
		if ((permission.rank == client->guildRank) && (permission.canChangeMotd))
			return true;
	return false;
}

inline bool Guild::CanSetTeleport(const std::shared_ptr<Client> & client)
{
	for (auto & permission : permissionlist)
		if ((permission.rank == client->guildRank) && (permission.canSetTeleport))
			return true;
	return false;
}

inline bool Guild::CanSummon(const std::shared_ptr<Client> & client)
{
	for (auto & permission : permissionlist)
		if ((permission.rank == client->guildRank) && (permission.canSummon))
			return true;
	return false;
}

inline bool Guild::CanPromote(const std::shared_ptr<Client> & client)
{
	for (auto & permission : permissionlist)
		if ((permission.rank == client->guildRank) && (permission.canPromote))
			return true;
	return false;
}

inline bool Guild::CanDemote(const std::shared_ptr<Client> & client)
{
	for (auto & permission : permissionlist)
		if ((permission.rank == client->guildRank) && (permission.canDemote))
			return true;
	return false;
}

bool Guild::SetDefaultRank(uint8_t newrank)
{
	for (auto rank : ranklist)
	{
		if (rank->rank == newrank)
		{//rank exists
			_defaultrank = rank;
			return true;
		}
	}
	//rank does not exist
	return false;
}

