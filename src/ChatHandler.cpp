
#include "ChatHandler.h"
#include "GServer.h"
#include "LServer.h"
#include "Client.h"
#include "Npc.h"
#include "Map.h"
#include "Guild.h"
#include "netmessages.h"

ChatHandler::ChatHandler(GServer * server)
{
	this->server = server;
}

ChatHandler::~ChatHandler()
{

}

void ChatHandler::ParseChat(const std::shared_ptr<Client> & client, std::string message)
{
// 	if (client->m_iAdminUserLevel > 0)
// 	{
// 		//TODO: log gm chat
// 	}
// 
// 	char sendmode = 0;
// 
// 	if (message.length() > 0)
// 	{
// 		switch (message[0])
// 		{
// 		case '$':
// 			message[0] = ' ';
// 			sendmode = CHAT_PARTY;
// 			break;
// 		case '@':
// 		case '^':
// 			message[0] = ' ';
// 			if (!client->guild)
// 				break;
// 			sendmode = CHAT_GUILD;
// 			break;
// 		case '!':
// 			message[0] = ' ';
// 			if (/*client->m_iLevel <= 10 ||*/ client->IsDead())
// 				break;
// 			sendmode = CHAT_SHOUT;
// 			break;
// 		case '%':
// 			message[0] = ' ';
// 			if (!client->IsGM())
// 				break;
// 			sendmode = CHAT_GM;
// 			break;
// 		case '~':
// 			message[0] = ' ';
// 			if (client->IsDead())
// 				break;
// 			sendmode = CHAT_NATIONSHOUT;
// 			break;
// 		case '/':
// 			//TODO: create functions for all the individual commands like classic hb? could even Lua-ize this
// 
// 			//elegant solution
// 			std::vector<string> tokens;
// 			boost::split(tokens, message, boost::is_any_of(" "));
// 
// 
// 			if (tokens[0] == "/tp")
// 			{
// 				if (tokens.size() < 2)
// 				{
// 					server->SendNotifyMsg(0, client.get(), NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid command parameters");
// 					return;
// 				}
// 				string mapname = tokens[1];
// 				int16_t x = -1, y = -1;
// 
// 				if (tokens.size() > 3)
// 				{
// 					x = atoi(tokens[2].c_str());
// 					y = atoi(tokens[3].c_str());
// 				}
// 
// 				Map * pmap = server->GetMap(mapname);
// 				if (!pmap)
// 				{
// 					server->SendNotifyMsg(0, client.get(), NOTIFY_NOTICEMSG, 0, 0, 0, "Map does not exist");
// 					return;
// 				}
// 				server->RequestTeleportHandler(client.get(), 2, mapname, x, y);
// 				return;
// 			}
// 			else if (tokens[0] == "/createguild")
// 			{
// 				//debug function
// 				Guild * temp = new Guild();
// 				temp->AddUser(client);
// 				//temp->
// 				server->guildlist.push_back(temp);
// 			}
// 			else if (tokens[0] == "/ginvite")
// 			{
// 				//if (client->guild != nullptr) && (client->guild->)
// 			}
// 			else if (tokens[0] == "/createitem")
// 			{
// 				if (tokens.size() < 2)
// 				{
// 					server->SendNotifyMsg(0, client.get(), NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid syntax");
// 					return;
// 				}
// 				Item * pItem = new Item;
// 				if (pItem->InitItemAttr(tokens[1], server->m_pItemConfigList) == false)
// 				{
// 					delete pItem;
// 					server->SendNotifyMsg(0, client.get(), NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid item name");
// 					return;
// 				}
// 				if (tokens.size() > 2)
// 				{
// 					pItem->color = (uint32_t)_atoi64(tokens[2].c_str());
// 				}
// 
// 				int iEraseReq;
// 				if (server->_bAddClientItemList(client, pItem, &iEraseReq) == true)
// 				{
// 
// 					server->SendItemNotifyMsg(client, NOTIFY_ITEMOBTAINED, pItem, 0);
// 
// 					if (iEraseReq == 1)
// 					{
// 						delete pItem;
// 						pItem = nullptr;
// 					}
// 
// 					server->log->info(fmt::format("GM Order(%s): Create ItemName(%s)", client->name, tokens[1]));
// 					return;
// 				}
// 				else
// 				{
// 					delete pItem;
// 					return;
// 				}
// 			}
// 			else if (tokens[0] == "/summon")
// 			{
// 				uint16_t pX, pY;
// 				pX = client->x;
// 				pY = client->y;
// 				char cWaypoint;
// 				if (tokens.size() < 2)
// 				{
// 					server->SendNotifyMsg(0, client.get(), NOTIFY_NOTICEMSG, 0, 0, 0, "Invalid summon");
// 					return;
// 				}
// 				int16_t mobnum = 1;
// 				if (tokens.size() > 2)
// 				{
// 					mobnum = atoi(tokens[2].c_str());
// 					if (mobnum < 1) mobnum = 1;
// 					if (mobnum > 50) mobnum = 50;
// 				}
// 				std::shared_ptr<Npc> master = client->map->CreateNpc(tokens[1], /*cSA*/0, MOVETYPE_RANDOM,
// 					&pX, &pY, (Side)-1, &cWaypoint, 0, 0, false, /*bSummoned*/false, /*isBerserked*/false, true);
// 				if (!master)
// 				{
// 					return;
// 				}
// 				for (int j = 0; j < (mobnum - 1); j++)
// 				{
// 					std::shared_ptr<Npc> slave = client->map->CreateNpc(tokens[1], /*cSA*/0, MOVETYPE_RANDOM,
// 						&pX, &pY, (Side)-1, &cWaypoint, 0, 0, false, /*bSummoned*/false, /*isBerserked*/false, true);
// 
// 					if (!slave)
// 						break;
// 
// 					slave->Follow(master);
// 				}
// 				return;
// 			}
// 
// 			break;
// 			if (message == "/to")
// 			{
// 				//TODO: tokenize to get client name and such
// 				client->whisperTarget = server->clientlist.back();
// 				server->SendNotifyMsg(0, client.get(), NOTIFY_WHISPERMODEON, 0, 0, 0, client->whisperTarget.lock()->name);
// 				return;
// 			}
// 			break;
// 		}
// 
// 		if (sendmode == CHAT_PARTY && !client->GetParty())
// 		{
// 			server->SendNotifyMsg(0, client.get(), NOTIFY_NOTICEMSG, 0, 0, 0, "You are not in a party");
// 			return;
// 		}
// 
// 		if (sendmode == 0 && !client->whisperTarget.expired())
// 		{
// 			sendmode = CHAT_WHISPER;
// 
// 			if (message[0] == '#') sendmode = CHAT_NORMAL;
// 
// 			if (client->m_iTimeLeft_ShutUp > 0) sendmode = CHAT_NORMAL;
// 		}
// 
// 		if ((client->magicEffectStatus[MAGICTYPE_CONFUSE] == 1) && (dice(1, 2) != 2))
// 		{
// 			//mess up message
// 			string temp;
// 			temp.resize(message.size() + 2);
// 			transform(message.begin(), message.end(), temp.begin(), [&](char letter)
// 			{
// 				if (dice(1, 3) == 2)
// 					return char(letter + 5);
// 				return letter;
// 			});
// 			message = temp;
// 		}
// 
// 		StreamWrite sw;
// 		sw.WriteInt(MSGID_COMMAND_CHATMSG);
// 		sw.WriteShort(uint16_t(client->handle));
// 		sw.WriteShort(client->x);
// 		sw.WriteShort(client->y);
// 		sw.WriteString(client->name, 10);
// 		sw.WriteByte(sendmode);
// 		sw.WriteString(message, message.length() + 1);
// 
// 		if (sendmode == CHAT_GUILD)
// 		{
// 			if (!client->guild)
// 				return;
// 			//client->m_guild->Broadcast();//TODO: guild message
// 		}
// 		else if (sendmode != CHAT_WHISPER)
// 		{
// 			shared_lock_guard<std::shared_mutex> lock(Gate::GetSingleton().mutclientlist);
// 			for (std::shared_ptr<Client> target : client->gserver->clientlist)
// 			{
// 				if (target->socket && (target->disconnecttime == 0))
// 				{
// 					if (!target->m_bIsInitComplete)
// 						continue;
// 
// 					bool bsend = true;
// 					if (server->m_bIsCrusadeMode)
// 					{
// 						if ((!client->IsNeutral()) && (!target->IsNeutral()) && (client->side != target->side))
// 							bsend = false;
// 					}
// 
// 					switch (sendmode)
// 					{
// 						//TODO: change how chat works? should make a chat mode for:
// 						// mapwide shout, mapwide town, global town - and make them "channels" that can be
// 						// /join'd so players can choose to participate in them (or let them be on by default
// 						// but able to be toggled)
// 					case CHAT_NORMAL:
// 						if (bsend && (target->map == client->map)
// 							&& (target->x > client->x - 13) && (target->x < client->x + 13)
// 							&& (target->y > client->y - 10) && (target->y < client->y + 10))
// 						{
// 							target->SWrite(sw);
// 						}
// 						break;
// 					case CHAT_SHOUT://both town shout chat - local to map only
// 						if (bsend && (target->map == client->map))
// 						{
// 							target->SWrite(sw);
// 						}
// 						break;
// 					case CHAT_GM://GM chat, sent global and always overrides
// 					{
// 						target->SWrite(sw);
// 					}
// 						break;
// 					case CHAT_NATIONSHOUT://your town only shout chat - local to map only
// 						if (bsend && (target->side == client->side) && (target->map == client->map))
// 						{
// 							target->SWrite(sw);
// 						}
// 						break;
// 					case CHAT_PARTY:
// 						if (target->GetParty() && client->GetParty() == target->GetParty())
// 						{
// 							target->SWrite(sw);
// 						}
// 						break;
// 					}
// 				}
// 			}
// 		}
// 		else//CHAT_WHISPER
// 		{
// 			//have to make sure a client isn't deleted when dereferencing
// 			shared_lock_guard<std::shared_mutex> lock(Gate::GetSingleton().mutclientlist);
// 			if (client->whisperTarget.expired())
// 			{
// 				server->SendNotifyMsg(0, client.get(), NOTIFY_NOTICEMSG, 0, 0, 0, "Player is not online");
// 				return;
// 			}
// 			std::shared_ptr<Client> whisperTarget = client->whisperTarget.lock();
// 			whisperTarget->SWrite(sw);
// 
// 			client->SWrite(sw);
// 		}
// 	}
}

