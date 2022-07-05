//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "core.h"
#include "netmessages.h"
#include "Map.h"

//how to delete client?
//void Server::DeleteClient(Client * client,
//							bool save,//save player data
//							bool notify,//notify surrounding players
//							bool countlogout,//??
//							bool forceclose)//apply bleeding isle "ban" effect
//void Server::DeleteClient(int iClientH, bool bSave, bool bNotify, bool bCountLogout, bool bForceCloseConn)

// DeleteClient only closes the socket with all intents and purposes of leaving
// their character in game unless they are not fully logged in, in which case this
// would kill the client object as well unless param is passed to also delete the object
// a proper logout would get the deleteobj flag passed
// 
// originally Gate server code - moved to login server functionality
void core::DeleteClient(std::shared_ptr<Client> client, bool save /*= false*/, bool deleteobj /*= false*/)
{
    if (!client) return;

    // 	upgrade_lock<shared_mutex> upgradelist(mutclientlist);
    // 	mutclientlist.unlock_and_lock_upgrade();
        //upgrade_lock<shared_mutex> upgradelist(mutclientlist);

    std::shared_ptr<ix::ConnectionState> ixconnstate = client->connection_state.lock();
    connection_state_hb * connection_state = dynamic_cast<connection_state_hb*>(ixconnstate.get());
    if (connection_state)
    {
        std::lock_guard<std::recursive_mutex> l(websocket_list);

        for (auto & wspair : websocket_clients)
            if (wspair.second == ixconnstate)
            {
                wspair.first->close();
            }
    }
    // else // already closed?

    // remove player from client list - this function (and its caller) should hold the last shared_ptr reference
    // so once they finish it should be removed entirely
    client_list.erase(client);

    client->currentstatus = 0;
    client->disconnecttime = std::chrono::steady_clock::now();
    client->disconnected = true;

    if (deleteobj)
    {
//         if (!client->gserver)
//         {
//             //__debugbreak();
//             //client disconnects/times out before login or at char select screen
//             return;
//         }
        //delete the object
        //might already have ownership of gatelock
        // 

        //need to perform client removal
        //remove from map
//         if (client->IsDead())
//             client->map->ClearDeadOwner(client->x, client->y);
//         else
//             client->map->ClearOwner(client->x, client->y);

        //let npcs know it's an invalid target
        client->m_bActive = false;

//         client->gserver->SendEventToNearClient_TypeA(client, MSGID_MOTION_EVENT_REJECT, 0, 0, 0);

        // todo - shouldn't it always save...? in what instance is not saving truly useful?
        // this is how item duping and other methods of cheating are created
        if (save)
        {
            if (client->IsDead())
            {
                client->x = -1;
                client->y = -1;

                if (client->IsNeutral())
                {
                    client->mapName = sideMap[NEUTRAL];
                }
                else
                {
//                     if (client->gserver->m_bIsCrusadeMode)
//                     {
//                         if (client->deadPenaltyTime > 0)
//                         {
//                             client->lockedMapName = sideMap[client->side];
//                             client->deadPenaltyTime = 600;//10 minutes
//                             client->LockMap(sideMap[client->side], 300);
//                         }
//                         else
//                             client->deadPenaltyTime = 600;
// 
//                     }

                    if (client->side == ARESDEN)
                    {
                        if ((client->mapName == sideMap[ELVINE]) && !client->IsGM())
                        {
                            client->LockMap(sideMapJail[ELVINE], 180);
                        }
                        else if (client->level > 80)
                        {
                            client->mapName = "resurr1";
                        }
                        else
                        {
                            client->mapName = "aresden";
                        }
                    }
                    else// if (player->m_side == ELVINE)
                    {
                        if ((client->mapName == sideMap[ARESDEN]) && !client->IsGM())
                        {
                            client->lockedMapName = sideMapJail[ARESDEN];
                            client->lockedMapTime = 180;
                        }
                        else if (client->level > 80)
                        {
                            client->mapName = "resurr2";
                        }
                        else
                        {
                            client->mapName = "elvine";
                        }
                    }
                }
            }


            if (client->observerMode)
            {
                client->x = -1;
                client->y = -1;
                client->mapName = sideMap[client->side];
            }

            if (client->m_bIsInitComplete)
            {
                //TODO: savedata here
            }
        }

        //any non-save specific stuff goes here

        //Update party status and stuff (notify party of disconnect and set status)
        //player->gserver->partyMgr

        //Update friends of disconnect
        //player->gserver->friends
    }
}

// todo - turn into its own request packet instead of part of login
void core::SendCharList(Client * client, StreamWrite & sw)
{
    try
    {
        pqxx::work txn{ *pq_login };
        pqxx::result r{ txn.exec_params("SELECT * FROM characters WHERE account_id=$1;", client->account_id) };
        txn.commit();

        sw.WriteByte((uint8_t)r.size());
        if (!r.empty())
        {
            for (pqxx::row row : r)
            {
                sw.WriteString(row["name"].as<std::string>(), 10);//char name
                sw.WriteByte(0x01); // 0x00 == empty npc + 39 bytes
                sw.WriteInt64(row["id"].as<int16_t>());
                sw.WriteShort(row["appr1"].as<int16_t>());//appr1
                sw.WriteShort(row["appr2"].as<int16_t>());//appr2
                sw.WriteShort(row["appr3"].as<int16_t>());//appr3
                sw.WriteShort(row["appr4"].as<int16_t>());//appr4
                sw.WriteShort(row["head_appr"].as<int16_t>());//headappr
                sw.WriteShort(row["body_appr"].as<int16_t>());//bodyappr
                sw.WriteShort(row["arm_appr"].as<int16_t>());//armappr
                sw.WriteShort(row["leg_appr"].as<int16_t>());//legappr
                sw.WriteShort(row["gender"].as<int16_t>());//gender
                sw.WriteShort(row["skin"].as<int16_t>());//skin
                sw.WriteShort(row["level"].as<int16_t>());//level
                sw.WriteInt64(row["experience"].as<int64_t>());//exp
                sw.WriteShort(row["strength"].as<int16_t>());//strength
                sw.WriteShort(row["vitality"].as<int16_t>());//vitality
                sw.WriteShort(row["dexterity"].as<int16_t>());//dexterity
                sw.WriteShort(row["intelligence"].as<int16_t>());//intelligence
                sw.WriteShort(row["magic"].as<int16_t>());//magic
                sw.WriteShort(row["charisma"].as<int16_t>());//agility/charisma
                sw.WriteInt(row["apprcolor"].as<int32_t>());//apprcolor
                sw.WriteShort(0);//unused data
                sw.WriteShort(0);//unused data
                sw.WriteShort(0);//unused data
                sw.WriteShort(0);//unused data
                sw.WriteShort(0);//unused data
                sw.WriteString(row["maploc"].as<std::string>(), 10);//char name
            }
        }
    }
    catch (std::exception & ex)
    {
        log->critical(fmt::format("Error querying login attempt for account [{}] - {}", client->account, ex.what()));
    }
}

bool core::RequestLogin(std::string account, std::string password)
{
//     StreamWrite sw;
// 
//     if (!CheckLogin(account, password))
//     {
//         sw.WriteInt(MSGID_RESPONSE_LOG);
//         sw.WriteShort(LOGRESMSGTYPE_PASSWORDMISMATCH);
//         client->SWrite(sw);
//     }
// 
//     std::string worldname = sr.ReadString(30);
// 
//     sw.WriteInt(MSGID_RESPONSE_LOG);
//     sw.WriteShort(MSGTYPE_CONFIRM);
//     sw.WriteShort(UPPER_VERSION);
//     sw.WriteShort(LOWER_VERSION);
//     sw.WriteByte(0x01);
//     sw.WriteShort(0);//dates \/
//     sw.WriteShort(0);
//     sw.WriteShort(0);
//     sw.WriteShort(0);
//     sw.WriteShort(0);
//     sw.WriteShort(0);//dates /\
// 
//     SendCharList(client, sw);
//     sw.WriteInt(500);
//     sw.WriteInt(500);
//     client->SWrite(sw);

    return true;
}

void core::CreateCharacter(Client * client, StreamRead & sr)
{
//     StreamWrite sw;
// 
//     try
//     {
//         uint16_t strength, vitality, dexterity, intelligence, magic, agility;
//         uint8_t gender, skin, hairstyle, haircolor, underwear;
// 
//         string playername, worldname;
// 
//         playername = sr.ReadString(10);
//         worldname = sr.ReadString(30);
// 
//         worldname = "Xtreme";
// 
//         bool test = false;
//         for (const GServer * gs : Gate::GetSingleton().gameserver)
//         {
//             if (gs->servername == worldname)
//             {
//                 test = true;
//                 break;
//             }
//         }
// 
//         if (!test)
//         {
//             //server doesn't exist
//             sw.WriteInt(MSGID_RESPONSE_LOG);
//             sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERFAILED);
//             client->SWrite(sw);
//             return;
//         }
// 
//         uint8_t charcount = 0;
// 
//         {
//             Statement select(ses);
//             select << "SELECT COUNT(*) FROM char_database WHERE account_name=? AND servername=?", use(client->account), use(worldname), into(charcount), now;
// 
//             if (charcount > 3)
//             {
//                 sw.WriteInt(MSGID_RESPONSE_LOG);
//                 sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERFAILED);
//                 client->SWrite(sw);
//                 return;
//             }
//         }
// 
//         {
//             Statement select(ses);
//             select << "SELECT COUNT(*) FROM char_database WHERE char_name=? AND servername=?", use(playername), use(worldname), into(charcount), now;
// 
//             if (charcount > 0)
//             {
//                 sw.WriteInt(MSGID_RESPONSE_LOG);
//                 sw.WriteShort(LOGRESMSGTYPE_ALREADYEXISTINGCHARACTER);
//                 client->SWrite(sw);
//                 return;
//             }
//         }
// 
//         gender = sr.ReadByte();
//         skin = sr.ReadByte();
//         hairstyle = sr.ReadByte();
//         haircolor = sr.ReadByte();
//         underwear = sr.ReadByte();
// 
//         strength = sr.ReadByte();
//         vitality = sr.ReadByte();
//         dexterity = sr.ReadByte();
//         intelligence = sr.ReadByte();
//         magic = sr.ReadByte();
//         agility = sr.ReadByte();
// 
//         if (((strength + vitality + dexterity + intelligence + magic + agility) != 70)
//             || strength < 10 || strength>14 || vitality < 10 || vitality>14 || dexterity < 10 || dexterity>14
//             || intelligence < 10 || intelligence>14 || magic < 10 || magic>14 || agility < 10 || agility>14)
//         {
//             sw.WriteInt(MSGID_RESPONSE_LOG);
//             sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERFAILED);
//             client->SWrite(sw);
//             return;
//         }
//         uint32_t appr1;
//         appr1 = (hairstyle << 8) | (haircolor << 4) | underwear;
//         uint16_t HP, MP, SP;
//         HP = (vitality * 8) + (strength * 2) + (intelligence * 2) + 8;
//         MP = (magic * 3) + (intelligence * 2) + 2;
//         SP = strength + 17;
// 
//         {
//             Statement select(ses);
//             select << "INSERT INTO char_database (account_name, servername, char_name, strength, vitality, dexterity, intelligence, magic, agility, appr1, gender, skin, hairstyle, haircolor, underwear, hp, mp, sp) "
//                 "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",
//                 use(client->account), use(worldname), use(playername), use(strength), use(vitality), use(dexterity), use(intelligence), use(magic), use(agility), use(appr1), use(gender), use(skin), use(hairstyle), use(haircolor), use(underwear), use(HP), use(MP), use(SP), now;
//         }
// 
//         uint64_t insertid = 0;
//         {
//             Statement select(ses);
//             select << "SELECT LAST_INSERT_ID();", into(insertid), now;
//         }
// 
//         for (uint8_t s = 0; s < 24; ++s)//24 skills total
//         {
//             if (/*s == 4 ||*/ s == 5 || s == 6 || s == 7 || s == 8 || s == 9 || s == 10 || s == 11 || s == 14 || s == 19 || s == 21)// All attack skills starts at 20%
//             {
//                 Statement select(ses);
//                 select << "INSERT INTO skill (charid, skillid, skillmastery, skillssn) VALUES (?,?,20,0);", use(insertid), use(s), now;
//             }
//             else if (s == 3 || s == 23) // Magic Res / Poison Res starts at 2%
//             {
//                 Statement select(ses);
//                 select << "INSERT INTO skill (charid, skillid, skillmastery, skillssn) VALUES (?,?,2,0);", use(insertid), use(s), now;
//             }
//             else// All crafting skills starts at 0% Magic skills starts at 0%
//             {
//                 Statement select(ses);
//                 select << "INSERT INTO skill (charid, skillid, skillmastery, skillssn) VALUES (?,?,0,0);", use(insertid), use(s), now;
//             }
//         }
// 
//         sw.WriteInt(MSGID_RESPONSE_LOG);
//         sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERCREATED);
//         sw.WriteString(playername.c_str(), 10);
//         SendCharList(client, sw);
//         client->SWrite(sw);
//     }
//     SQLCATCH(DeleteClient(client, true));
}

void core::DeleteCharacter(Client * client, StreamRead & sr)
{
//     Session ses(sqlpool->get());
//     StreamWrite sw;
//     try
//     {
//         string playername = sr.ReadString(10);
//         string worldname = sr.ReadString(30);
// 
//         worldname = "Xtreme";
// 
//         uint32_t charid = 0;
//         {
//             Statement select(ses);
//             select << "SELECT charid FROM char_database WHERE account_name=? AND char_name=?", use(client->account), use(playername), into(charid), now;
//         }
//         if (charid == 0)
//         {
//             sw.WriteInt(MSGID_RESPONSE_LOG);
//             sw.WriteShort(LOGRESMSGTYPE_NOTEXISTINGCHARACTER);
//             client->SWrite(sw);
//             return;
//         }
// 
//         if (Gate::GetSingleton().is_account_in_use(client))
//         {
//             sw.WriteInt(MSGID_RESPONSE_LOG);
//             sw.WriteShort(ENTERGAMERESTYPE_PLAYING);
//             client->SWrite(sw);
//         }
//         else
//         {
//             {
//                 Statement select(ses);
//                 select << "DELETE FROM char_database WHERE char_name=? LIMIT 1;", use(playername), now;
//             }
//             {
//                 Statement select(ses);
//                 select << "DELETE FROM skill WHERE CharID=?;", use(charid), now;
//             }
//             {
//                 Statement select(ses);
//                 select << "DELETE FROM item WHERE CharID=?;", use(charid), now;
//             }
//             sw.WriteInt(MSGID_RESPONSE_LOG);
//             sw.WriteShort(LOGRESMSGTYPE_CHARACTERDELETED);
//             sw.WriteByte(0x01);
//             SendCharList(client, sw);
//             client->SWrite(sw);
//         }
//     }
//     SQLCATCH(DeleteClient(client, true));
}

void core::EnterGame(std::shared_ptr<Client> client, StreamRead & sr)
{
    StreamWrite sw;
    try
    {
        if (game_status_ != game_server_status::running)
        {
            sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
            sw.WriteShort(ENTERGAMERESTYPE_REJECT);
            sw.WriteString("Game server is offline");
            sw.WriteInt(0);
            sw.WriteInt(0);
            sw.WriteInt(0);
            sw.WriteByte(5);
            client->SWrite(sw);
            return;
        }

        std::string playername = sr.ReadString(10);
        uint16_t entertype = sr.ReadShort();
        std::string worldname = sr.ReadString(30);
        // string commandline = sr.ReadString(120);

        //client is requesting to disconnect currently connected player
//         if (entertype == ENTERGAMEMSGTYPE_NOENTER_FORCEDISCONN)
//         {
//             //send disconnect to all online servers
//             for (auto gs : Gate::GetSingleton().gameserver)
//             {
//                 sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
//                 sw.WriteShort(ENTERGAMERESTYPE_FORCEDISCONN);
//                 client->SWrite(sw);
// 
//                 //check if client is even in game
//                 std::shared_ptr<Client> clientfound;
// 
//                 {
//                     boost::shared_lock_guard<boost::shared_mutex> lock(Gate::GetSingleton().mutclientlist);
//                     bool accountfound = false;
//                     for (std::shared_ptr<Client> & clnt : gs->clientlist)
//                     {
//                         if (clnt->account == client->account)
//                         {
//                             //account found
//                             accountfound = true;
//                             clientfound = clnt;
//                             break;
//                         }
//                     }
//                 }
// 
//                 //client found? send force disconnect if so
// 
//                 if (clientfound != nullptr)
//                 {
//                     clientfound->Notify(nullptr, NOTIFY_FORCEDISCONN);
//                 }
//             }
//             return;
//         }

        worldname = "Xtreme";

        uint8_t charcount = 0;

        uint32_t charid = 0;
        std::string mapname;

        try
        {
            pqxx::work txn{ *pq_login };
            pqxx::row r{ txn.exec_params1("SELECT * FROM characters WHERE account_id=$1 AND name=$2 LIMIT 1", client->account_id, pq_login->esc(playername)) };
            txn.commit();

            charid = r["id"].as<uint32_t>();
        }
        catch (pqxx::unexpected_rows & ex)
        {
            // character doesn't exist
            sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
            sw.WriteShort(ENTERGAMERESTYPE_REJECT);
            sw.WriteString("Character does not exist");
            sw.WriteInt(0);
            sw.WriteInt(0);
            sw.WriteInt(0);
            sw.WriteByte(4);
            client->SWrite(sw);
            return;
        }

        //server is up

        //check if account or char already logged in

        //there is a far better way to accomplish this. use this for now, but make it better later? or keep?
        //can keep track of logged in status via db, will add that eventually anyway
        //login can keep its own list of matching data to not have to lock the gameserver list - better a slow login internal query than slow game thread


        std::shared_ptr<Client> clientfound;

        bool accountfound = false;
        {
            std::lock_guard<std::shared_mutex> lock(client_list_mtx);
            for (const std::shared_ptr<Client> & clnt : client_list)
            {
                // this will have a hit for itself, so check for map being set which only a character that has made it in game will have
                // that way it continues iterating to find any other instance of the account connected
                // this also lets the account have multiple connections to the character select screen as a side-effect
                // todo - allow this?
                if (clnt->account_id == client->account_id && clnt->map)
                {
                    //BUG: potential bug point if charID == 0 - only occurs on unsuccessful login
                    // ^ is this still true?
                    //account found
                    accountfound = true;
                    if (clnt->charid == charid)
                    {
                        //exact char found
                        clientfound = clnt;
                        break;
                    }
                }
            }
        }

        // set the map for the client
        for (auto & map : maps)
            if (map->name == mapname)
                client->map = map.get();

        if (clientfound && clientfound->disconnected == true)
        {
            //client has recently disconnected
            //at this point, can do some fancy swapping of socket and place this current active socket
            // into the old client object letting them retake control of the disconnected session
            // - to do that though, it'd need to resend initial data - need a special case in GServer
            // - to not do the usual map data stuff when init data is sent or risk duplicating client
            // - objects in mapdata - also needs a case in GServer to not reject another initdata
            // - from an already connected client (used to be the cause of item duping on Int'l)
            // - and will need code in place to handle this specific case
            auto connection_state = clientfound->get_connection_state();
            auto websocket = clientfound->get_websocket();
            if (websocket->getReadyState() == ix::ReadyState::Open)
            {
                //false alarm of some sort (or other issue) should probably breakpoint this or have some sort
                //of check if client even still exists. socket should never be open if disconnected was set
                //__debugbreak();
            }
            else
            {
                clientfound->disconnecttime = std::chrono::steady_clock::time_point();
                clientfound->connection_state = client->connection_state;
                auto connstate = clientfound->get_connection_state();
                if (connstate)
                {
                    connstate->client = clientfound;
                }
                else
                {
                    // error .. connection state should exist
                    log->critical(fmt::format("Connection state invalid when resuming client connection - account: [{}]", clientfound->account));
                }
                client->connection_state.reset();

                //since nothing is technically associated with this client, just remove it from the list to force it to delete

                client_list.erase(client);

                if (clientfound->map != nullptr)
                {
                    clientfound->currentstatus = 2;
                    clientfound->name = playername;
                    //pgs->clientlist.push_back(client);
                    //clientlist.remove(client);

                    sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
                    sw.WriteShort(ENTERGAMERESTYPE_CONFIRM);
                    sw.WriteString(string("127.0.0.1"), 16);
                    sw.WriteShort(2848);
                    sw.WriteString(worldname, 20);
                    clientfound->SWrite(sw);
                }
                else
                {
                    sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
                    sw.WriteShort(ENTERGAMERESTYPE_REJECT);
                    sw.WriteString("Map does not exist");
                    sw.WriteInt(0);
                    sw.WriteInt(0);
                    sw.WriteInt(0);
                    sw.WriteByte(5);
                    clientfound->SWrite(sw);
                }
            }
        }
        else if (accountfound)
        {
            //account already logged in and not resuming object
            sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
            sw.WriteShort(ENTERGAMERESTYPE_PLAYING);
            client->SWrite(sw);
        }
        else
        {
            if (client->map != nullptr)
            {
                client->currentstatus = 2;
                client->name = playername;

                sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
                sw.WriteShort(ENTERGAMERESTYPE_CONFIRM);
                sw.WriteString(string("127.0.0.1"), 16);
                sw.WriteShort(2848);
                sw.WriteString(worldname, 20);
                client->SWrite(sw);
            }
            else
            {
                sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
                sw.WriteShort(ENTERGAMERESTYPE_REJECT);
                sw.WriteString("Map does not exist");
                sw.WriteInt(0);
                sw.WriteInt(0);
                sw.WriteInt(0);
                sw.WriteByte(3);
                client->SWrite(sw);
            }
        }
    }
    catch (std::exception & ex)
    {
        log->critical(fmt::format("EnterGame() Exception: %s", string(ex.what())));
        sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
        sw.WriteShort(ENTERGAMERESTYPE_REJECT);
        sw.WriteString("Unknown");
        sw.WriteInt(0);
        sw.WriteInt(0);
        sw.WriteInt(0);
        sw.WriteByte(3);
        client->SWrite(sw);
    }
}
