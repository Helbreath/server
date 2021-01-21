
#include "chat_handler.hpp"
#include "defines.hpp"
#include "client.hpp"
#include "gserver.hpp"
#include "item.hpp"
#include "guild.hpp"
#include "map.hpp"
#include "npc.hpp"
#include "net/netmessages.hpp"
#include "streams.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace hbx
{

chat_handler::chat_handler(gserver * gs_)
	: gs(gs_)
{

}

void chat_handler::parse_chat(const std::shared_ptr<client> & client_, std::string message)
{
	if (client_->m_iAdminUserLevel > 0)
	{
		//TODO: log gm chat
	}

	// string_view?
	char sendmode = 0;

	if (message.length() > 0)
	{
		switch (message[0])
		{
			case '$':
				message = message.substr(1, message.length() - 1);
				sendmode = CHAT_PARTY;
				break;
			case '@':
			case '^':
				message = message.substr(1, message.length() - 1);
				if (!client_->guild_)
					break;
				sendmode = CHAT_GUILD;
				break;
			case '!':
				message = message.substr(1, message.length() - 1);
				if (/*client->m_iLevel <= 10 ||*/ client_->is_dead())
					break;
				sendmode = CHAT_SHOUT;
				break;
			case '%':
				message = message.substr(1, message.length() - 1);
				if (!client_->IsGM())
					break;
				sendmode = CHAT_GM;
				break;
			case '~':
				message = message.substr(1, message.length() - 1);
				if (client_->is_dead())
					break;
				sendmode = CHAT_NATIONSHOUT;
				break;
			case '/':
				//TODO: create functions for all the individual commands like classic hb? could even Lua-ize this

				//elegant solution
				std::vector<std::string> tokens;
				tokens = utility::tokenize(message, " ");

				if (tokens[0] == "/tp")
				{
					if (tokens.size() < 2)
					{
						gs->SendNotifyMsg(nullptr, client_, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid command parameters");
						return;
					}
					std::string mapname = tokens[1];
					int16_t x = -1, y = -1;

					if (tokens.size() > 3)
					{
						x = atoi(tokens[2].c_str());
						y = atoi(tokens[3].c_str());
					}

					map * pmap = gs->get_map(mapname);
					if (!pmap)
					{
						gs->SendNotifyMsg(nullptr, client_, NOTIFY_NOTICEMSG, 0, 0, 0, "Map does not exist");
						return;
					}
					gs->RequestTeleportHandler(client_, 2, mapname, x, y);
					return;
				}
				else if (tokens[0] == "/createguild")
				{
					//debug function
					guild * temp = new guild();
					temp->AddUser(client_);
					//temp->
					gs->guildlist.push_back(temp);
				}
				else if (tokens[0] == "/ginvite")
				{
					//if (client->guild != nullptr) && (client->guild->)
				}
				else if (tokens[0] == "/createitem")
				{
/*
					if (tokens.size() < 2)
					{
						gs->SendNotifyMsg(nullptr, client_, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid syntax");
						return;
					}
					item * pItem = new item;
					if (pItem->InitItemAttr(tokens[1], gs->m_pItemConfigList) == false)
					{
						delete pItem;
						gs->SendNotifyMsg(nullptr, client_, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid item name");
						return;
					}
					if (tokens.size() > 2)
					{
						pItem->m_cItemColor = (uint32_t)_atoi64(tokens[2].c_str());
					}

					int iEraseReq = 0;
					if (gs->_bAddClientItemList(client_, pItem, &iEraseReq) == true)
					{

						gs->SendItemNotifyMsg(client_, NOTIFY_ITEMOBTAINED, pItem, 0);

						if (iEraseReq == 1)
						{
							delete pItem;
							pItem = nullptr;
						}

						gs->log->info("GM Order({}): Create ItemName({})", client_->name, tokens[1]);
						return;
					}
					else
					{
						delete pItem;
						return;
					}*/
				}
				else if (tokens[0] == "/summon")
				{
					uint16_t pX, pY;
					pX = client_->x;
					pY = client_->y;
					char cWaypoint;
					if (tokens.size() < 2)
					{
						gs->SendNotifyMsg(nullptr, client_, NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid summon");
						return;
					}
					int16_t mobnum = 1;
					if (tokens.size() > 2)
					{
						mobnum = atoi(tokens[2].c_str());
						if (mobnum < 1) mobnum = 1;
						if (mobnum > 50) mobnum = 50;
					}
					std::shared_ptr<npc> master = client_->map_->CreateNpc(tokens[1], /*cSA*/0, MOVETYPE_RANDOM,
						&pX, &pY, (faction)-1, &cWaypoint, 0, 0, false, /*bSummoned*/false, /*isBerserked*/false, true);
					if (!master)
					{
						return;
					}
					for (int j = 0; j < (mobnum - 1); j++)
					{
						std::shared_ptr<npc> slave = client_->map_->CreateNpc(tokens[1], /*cSA*/0, MOVETYPE_RANDOM,
							&pX, &pY, (faction)-1, &cWaypoint, 0, 0, false, /*bSummoned*/false, /*isBerserked*/false, true);

						if (!slave)
							break;

						slave->Follow(master);
					}
					return;
				}

				break;
				if (message == "/to")
				{
					//TODO: tokenize to get client name and such
					client_->whisperTarget = gs->clients.back();
					gs->SendNotifyMsg(nullptr, client_, NOTIFY_WHISPERMODEON, 0, 0, 0, client_->whisperTarget.lock()->name);
					return;
				}
				break;
		}

		if (sendmode == CHAT_PARTY && !client_->GetParty())
		{
			gs->SendNotifyMsg(nullptr, client_, NOTIFY_NOTICEMSG, 0, 0, 0, "You are not in a party");
			return;
		}

		if (sendmode == 0 && !client_->whisperTarget.expired())
		{
			sendmode = CHAT_WHISPER;

			if (message[0] == '#') sendmode = CHAT_NORMAL;

			if (client_->m_iTimeLeft_ShutUp > 0) sendmode = CHAT_NORMAL;
		}

		if ((client_->magic_effect_status[MAGICTYPE_CONFUSE] == 1) && (utility::gen_rand(1, 2) != 2))
		{
			//mess up message
			string temp;
			temp.resize(message.size() + 2);
			transform(message.begin(), message.end(), temp.begin(), [&](char letter)
			{
				if (utility::gen_rand(1, 3) == 2)
					return char(letter + 5);
				return letter;
			});
			message = temp;
		}

		stream_write sw;
		sw.write_int32(MSGID_COMMAND_CHATMSG);
		sw.write_int16(int16_t(client_->handle));
		sw.write_int16(client_->x);
		sw.write_int16(client_->y);
		sw.write_string(client_->name, 10);
		sw.write_int8(sendmode);
		sw.write_string(message, message.length() + 1);

		if (sendmode == CHAT_GUILD)
		{
			if (!client_->guild_)
				return;
			//client->m_guild->Broadcast();//TODO: guild message
		}
		else if (sendmode != CHAT_WHISPER)
		{
			std::unique_lock<std::mutex> lock(client_->gs->cl_m);
			for (const std::shared_ptr<client> & target : client_->gs->clients)
			{
				if (target->socket_ && (target->disconnecttime == 0))
				{
					if (!target->m_bIsInitComplete)
						continue;

					bool bsend = true;
					if (gs->m_bIsCrusadeMode)
					{
						if ((!client_->is_neutral()) && (!target->is_neutral()) && (client_->side != target->side))
							bsend = false;
					}

					switch (sendmode)
					{
						//TODO: change how chat works? should make a chat mode for:
						// mapwide shout, mapwide town, global town - and make them "channels" that can be
						// /join'd so players can choose to participate in them (or let them be on by default
						// but able to be toggled)
						case CHAT_NORMAL:
							if (bsend && (target->map_ == client_->map_)
								&& (target->x > client_->x - 13) && (target->x < client_->x + 13)
								&& (target->y > client_->y - 10) && (target->y < client_->y + 10))
							{
								target->write(sw);
							}
							break;
						case CHAT_SHOUT://both town shout chat - local to map only
							if (bsend && (target->map_ == client_->map_))
							{
								target->write(sw);
							}
							break;
						case CHAT_GM://GM chat, sent global and always overrides
						{
							target->write(sw);
						}
						break;
						case CHAT_NATIONSHOUT://your town only shout chat - local to map only
							if (bsend && (target->side == client_->side) && (target->map_ == client_->map_))
							{
								target->write(sw);
							}
							break;
						case CHAT_PARTY:
							if (target->GetParty() && client_->GetParty() == target->GetParty())
							{
								target->write(sw);
							}
							break;
					}
				}
			}
		}
		else//CHAT_WHISPER
		{
			//have to make sure a client isn't deleted when dereferencing
            std::unique_lock<std::mutex> lock(client_->gs->cl_m);
			if (client_->whisperTarget.expired())
			{
				gs->SendNotifyMsg(nullptr, client_, NOTIFY_NOTICEMSG, 0, 0, 0, "Player is not online");
				return;
			}
			std::shared_ptr<client> whisperTarget = client_->whisperTarget.lock();
			whisperTarget->write(sw);

			client_->write(sw);
		}
	}
}

}
