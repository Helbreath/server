
#include "guild.hpp"
#include "client.hpp"


namespace hbx
{

guild::guild()
{
}


guild::~guild()
{
}

bool guild::Save()
{
	return false;
}

bool guild::PromoteUser(const std::shared_ptr<client> & client, std::string target)
{
	if (!CanPromote(client))
		return false;
	std::shared_ptr<stGuildMember> player = GetMember(target);
	std::shared_ptr<stGuildMember> invoker = GetMember(client->name);
	if ((player == nullptr) || (invoker == nullptr))
	{
		//member or invoker is not in the guild
		//TODO: send message of failure
		return false;
	}
	if (player->player != nullptr)
	{
		uint8_t _ot = player->rank->order;
		uint8_t _it = invoker->rank->order;
		if (_ot - 1 < _it)
		{
			//rank being promoted to is still under invoker's

			//perform promote
			for (const auto & rank : ranklist)
			{
				if (rank->rank == _ot - 1)
				{
					player->rank = rank;
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

bool guild::DemoteUser(const std::shared_ptr<client> & client, std::string character)
{
	if (!CanPromote(client))
		return false;
	return false;
}

bool guild::AddUser(const std::shared_ptr<client> & client)
{
	//user already in a guild - should never trigger here on live
	if (client->guild_ != nullptr)
	{
		return false;
	}


	std::shared_ptr<stGuildMember> newmember = std::make_shared<stGuildMember>();
	newmember->lastonline = 0;
	newmember->level = client->level;
	newmember->name = client->name;
	newmember->player = client;
	//newmember->playerclass = client->playerclass;
	newmember->rank = _defaultrank;
	memberlist.push_back(newmember);


	//send message to guild?


	//update client with new guild params here? or in server?
	return true;
}

void guild::SendChatMessage(std::string & message, uint8_t order)
{
	for (const auto & member : memberlist)
	{
		if (member->rank->order < order)
		{
			//member.player->Notify(nullptr, );//TODO: send chat message
		}
	}
}

std::shared_ptr<stGuildMember> guild::GetMember(std::string name)
{
	for (const auto & member : memberlist)
	{
		if (member->player->name == name)
		{
			return member;
		}
	}
	return nullptr;
}

//TODO: perhaps make this 'easier' in the future? inline functions should suffice for now
inline bool guild::CanInvite(const std::shared_ptr<client> & client)
{
    return client->guildRank->permission.canInvite;
}

inline bool guild::CanKick(const std::shared_ptr<client> & client)
{
    return client->guildRank->permission.canKick;
}

inline bool guild::CanChangeMotd(const std::shared_ptr<client> & client)
{
    return client->guildRank->permission.canChangeMotd;
}

inline bool guild::CanSetTeleport(const std::shared_ptr<client> & client)
{
    return client->guildRank->permission.canSetTeleport;
}

inline bool guild::CanSummon(const std::shared_ptr<client> & client)
{
    return client->guildRank->permission.canSummon;
}

inline bool guild::CanPromote(const std::shared_ptr<client> & client)
{
    return client->guildRank->permission.canPromote;
}

inline bool guild::CanDemote(const std::shared_ptr<client> & client)
{
    return client->guildRank->permission.canDemote;
}

bool guild::SetDefaultRank(uint8_t newrank)
{
	for (const auto & rank : ranklist)
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

}
