// #include "LServer.h"
// #include "GServer.h"
// #include "Client.h"
// #include "Map.h"
// #include "netmessages.h"
// 
// LServer * LServer::_instance = nullptr;
// 
// LServer::LServer()
// {
// 	//asio socket thread count
// 	currentplayersonline = 0;
// 	currentconnections = 0;
// 
// 	sqlpool = 0;
// 
// 	MySQL::Connector::registerConnector();
// 	SQLite::Connector::registerConnector();
// 
// 
// 	L = luaL_newstate();
// 	luaL_openlibs(L);
// 	timer_thread_running = false;
// }
// 
// LServer::~LServer()
// {
// 	MySQL::Connector::unregisterConnector();
// 	SQLite::Connector::unregisterConnector();
// }
// 
// bool LServer::Init(string config)
// {
// 	try
// 	{
// 		configfile = config;
// 		log->info("Loading Config.");
// 		if (luaL_dofile(L, configfile.c_str()) != 0)
// 		{
// 			log->critical(fmt::format("%s", (string)lua_tostring(L,-1)));
// 			return false;
// 		}
// 		lua_getglobal(L, "config");
// 
// 		char * temp;
// 
// 		//sql host
// 		{
// 			lua_getfield(L, -1, "sqlhost");
// 			temp = (char*)lua_tostring(L, -1);
// 			if (temp == 0) { log->critical("Invalid sqlhost setting."); return false; }
// 			sqlhost = temp;
// 			log->info(fmt::format("sqlhost: %s", sqlhost));
// 			lua_pop(L, 1);
// 		}
// 
// 		//sql user
// 		{
// 			lua_getfield(L, -1, "sqluser");
// 			temp = (char*)lua_tostring(L, -1);
// 			if (temp == 0) { log->critical("Invalid sqluser setting."); return false; }
// 			sqluser = temp;
// 			log->info(fmt::format("sqluser: %s", sqluser));
// 			lua_pop(L, 1);
// 		}
// 
// 		//sql pass
// 		{
// 			lua_getfield(L, -1, "sqlpass");
// 			temp = (char*)lua_tostring(L, -1);
// 			if (temp == 0) { log->critical("Invalid sqlpass setting."); return false; }
// 			log->info("sqlpass set");
// 			sqlpass = temp;
// 			lua_pop(L, 1);
// 		}
// 
// 		lua_pop(L, 1);
// 	}
// 	catch (std::exception& e)
// 	{
// 		log->critical(fmt::format("Init() Exception: %s", (string)e.what()));
// 		system("pause");
// 		return false;
// 	}
// 	catch(...)
// 	{
// 		log->critical("Unspecified Init() Exception.");
// 		system("pause");
// 		return false;
// 	}
// 	ConnectSQL();
// 	return true;
// }
// 
// void LServer::run()
// {
// 	try
// 	{
// 		serverstatus = SERVERSTATUS_ONLINE;
// 		log->info("Creating LServer threads.");
// 
// 		timerthread = thread(std::bind(std::mem_fun(&LServer::TimerThread), this));
// 		socketthread = thread(std::bind(std::mem_fun(&LServer::SocketThread), this));
// 
// 		timerthread.join();
// 		socketthread.join();
// 
// 		lua_close(L);
// 
// 
// 	}
// 	catch (std::exception& e)
// 	{
// 		log->critical(fmt::format("LServer::run() Exception: %s", e.what() ));
// 	}
// }
// 
// void LServer::handle_stop()
// {
// 	log->info("handle_stop()");
// 	this->serverstatus = SERVERSTATUS_SHUTDOWN;
// }
// 
// void LServer::TimerThread()
// {
// #ifndef WIN32
// 	struct timespec req={0};
// 	req.tv_sec = 0;
// 	req.tv_nsec = 1000000L;//1ms
// #else
// 	_tzset();
// #endif
// 
// 	timer_thread_running = true;
// 
// 	uint64_t t1htimer;
// 	uint64_t t30mintimer;
// 	uint64_t t6mintimer;
// 	uint64_t t5mintimer;
// 	uint64_t t3mintimer;
// 	uint64_t t1mintimer;
// 	uint64_t t30sectimer;
// 	uint64_t t5sectimer;
// 	uint64_t t1sectimer;
// 	uint64_t t100msectimer;
// 	uint64_t ltime;
// 
// 	t1htimer = t30mintimer = t6mintimer = t5mintimer = t3mintimer = t1mintimer = t30sectimer = t5sectimer = t1sectimer = t100msectimer = unixtime();
// 
// 	while (serverstatus == SERVERSTATUS_ONLINE)
// 	{
// 		try
// 		{
// 			ltime = ltime = unixtime();
// 
// 			if (t100msectimer < ltime)
// 			{
// 				t100msectimer += 100;
// 			}
// 			if (t1sectimer < ltime)
// 			{
// 				t1sectimer += 1000;
// 			}
// 			if (t5sectimer < ltime)
// 			{
// 				//check connections for recent data (ghost sockets)
// 				for (std::shared_ptr<Client> & client : clientlist)
// 				{
// 					if (client->disconnecttime == 0 && client->lastpackettime + 30000 < ltime)
// 					{
// 						//socket idle for 30 seconds (should never happen unless disconnected)
// 						log->info(fmt::format("Client Timeout! <%s>", client->socket->address));
// 						Gate::GetSingleton().stop(client->socket);
// 						//when to delete client object...... ?
// 						//force a ~10 second delay on object deletion to prevent logout hacking/plug pulling/etc? or destroy instantly?
// 					}
// 				}
// 				t5sectimer += 5000;
// 			}
// 			if (t30sectimer < ltime)
// 			{
// 				t30sectimer += 30000;
// 			}
// 			if (t1mintimer < ltime)
// 			{
// 				t1mintimer += 60000;
// 			}
// 			if (t3mintimer < ltime)
// 			{
// 				t3mintimer += 180000;
// 			}
// 			if (t6mintimer < ltime)
// 			{
// 				t6mintimer += 360000;
// 			}
// 			if (t1htimer < ltime)
// 			{
// 
// 				t1htimer += 3600000;
// 			}
// 	#ifdef WIN32
// 			Sleep(1);
// 	#else
// 			nanosleep(&req,0);
// 	#endif
// 		}
// 		catch (Poco::Data::MySQL::MySQLException * e)
// 		{
// 			log->critical(fmt::format("TimerThread() SQL Exception: %s", e->displayText() ));
// 		}
// 		catch (...)
// 		{
// 			log->critical("Unspecified LServer::TimerThread() Exception.");
// 			timer_thread_running = false;
// 			//TODO: invoke server shutdown/restart?
// 			//timer thread should never exit - needs some sort of something here
// 			return;
// 		}
// 	}
// 	timer_thread_running = false;
// 	log->info("TimerThread() complete.");
// 
// 	return;
// }
// 
// void LServer::SocketThread()
// {
// #ifndef WIN32
// 	struct timespec req={0};
// 	req.tv_sec = 0;
// 	req.tv_nsec = 1000000L;//1ms
// #else
// 	_tzset();
// #endif
// 
// 	while (serverstatus == SERVERSTATUS_ONLINE)
// 	{
// 		if (socketpipe.size() > 0)
// 		try
// 		{
// 			std::shared_ptr<MsgQueueEntry> msg = GetMsgQueue(socketpipe);
// 
// 			StreamRead sr = StreamRead(msg->data, msg->size);
// 			StreamWrite sw = StreamWrite();
// 
// 			std::shared_ptr<Client> client = msg->client;
// 
// 			uint32_t msgid = sr.ReadInt();
// 
// 			if (msgid == MSGID_COMMAND_CHECKCONNECTION)
// 			{
// 				continue;
// 			}
// 
// 			std::string macaddress = sr.ReadString(12);
// 			client->account = sr.ReadString(10);
// 			client->password = sr.ReadString(10);
// 
// 			if (!CheckLogin(client, client->account, client->password))
// 			{
// 				sw.WriteInt(MSGID_RESPONSE_LOG);
// 				sw.WriteShort(LOGRESMSGTYPE_PASSWORDMISMATCH);
// 				client->SWrite(sw);
// 			}
// 			else switch (msgid)
// 			{
// 			case MSGID_REQUEST_LOGIN:
// 				RequestLogin(client, sr);
// 				break;
// 			case MSGID_REQUEST_CREATENEWCHARACTER:
// 				CreateCharacter(client, sr);
// 				break;
// 			case MSGID_REQUEST_CREATENEWACCOUNT:
// 			{
// 				// cannot be called
// 				sw.WriteInt(MSGID_RESPONSE_LOG);
// 				sw.WriteShort(LOGRESMSGTYPE_ALREADYEXISTINGACCOUNT);
// 				client->SWrite(sw);
// 			}
// 				break;
// 			case MSGID_REQUEST_DELETECHARACTER:
// 				DeleteCharacter(client, sr);
// 				break;
// 			case MSGID_REQUEST_CHANGEPASSWORD:
// 				// cannot be called
// 			{
// 				sw.WriteInt(MSGID_RESPONSE_LOG);
// 				sw.WriteShort(LOGRESMSGTYPE_PASSWORDCHANGEFAIL);
// 				client->SWrite(sw);
// 			}
// 				break;
// 			case MSGID_REQUEST_ENTERGAME:
// 				EnterGame(client, sr);
// 				break;
// 			default:
// 				log->error(fmt::format("Unknown packet received from client - %X", msgid));
// 				//DeleteClient(msg->client, true);
// 				break;
// 			}
// 		}
// 		catch(int32_t reason)
// 		{
// 			if (reason == -192)
// 			{
// 				log->error("(data == 0)");
// 			}
// 			else if (reason == -193)
// 			{
// 				log->error("LServer: (position+a > size)");
// 			}
// 		}
// 		SQLCATCH(0);
// 
// #ifdef WIN32
// 		Sleep(1);
// #else
// 		nanosleep(&req,0);
// #endif
// 	}
// 	log->info("ActionThread() complete.");
// #ifndef WIN32
// 	mysql_thread_end();
// #endif
// }
// 
// //how to delete client?
// //void Server::DeleteClient(Client * client,
// //							bool save,//save player data
// //							bool notify,//notify surrounding players
// //							bool countlogout,//??
// //							bool forceclose)//apply bleeding isle "ban" effect
// //void Server::DeleteClient(int iClientH, bool bSave, bool bNotify, bool bCountLogout, bool bForceCloseConn)
// 
// // DeleteClient only closes the socket with all intents and purposes of leaving
// // their character in game unless they are not fully logged in, in which case this
// // would kill the client object as well unless param is passed to also delete the object
// // a proper logout would get the deleteobj flag passed
// void LServer::DeleteClient(std::shared_ptr<Client> client, bool deleteobj)
// {
// 	if (!client) return;
// 
// 	Gate::GetSingleton().DeleteClient(client, false, deleteobj);
// }
// 
// bool LServer::CheckLogin(std::shared_ptr<Client> client, string & account, string & pass)
// {
// 	try
// 	{
// 		Session ses(sqlpool->get());
// 		Statement select(ses);
// 		select << "SELECT * FROM account_database WHERE name=? LIMIT 1", use(account), now;
// 		RecordSet rs(select);
// 
// 		if (rs.rowCount() > 0)
// 		{
// 			if (rs.value("password").convert<string>() == pass)
// 				return true;
// 			else
// 				return false;
// 		}
// 		else
// 		{
// 			//create a nonexisting account?
// #define LOGINACCOUNTCREATION
// #ifdef LOGINACCOUNTCREATION
// 			{
// 				ses << "INSERT INTO account_database SET name=?,password=?", use(account), use(pass), now;
// 				return true;
// 			}
// #else
// 			return false;
// #endif
// 		}
// 	}
// 	SQLCATCH(void(0))
// 
// 	return false;
// }
// 
// void LServer::SendCharList(std::shared_ptr<Client> client, StreamWrite & sw)
// {
// 	try
// 	{
// 		Session ses(sqlpool->get());
// 		Statement select(ses);
// 		logincharacterlist chars;
// 		select << "SELECT char_name,appr1,appr2,appr3,appr4,headapprvalue,bodyapprvalue,armapprvalue,legapprvalue,gender,skin,level,exp,apprcolor,maploc FROM char_database WHERE account_name=? LIMIT 4;", use(client->account), into(chars), now;
// 		sw.WriteByte((uint8_t)chars.size());
// 		if (chars.size() > 0)//check for valid characters?
// 		{
// 			for (loginsqlcharacter & character : chars)
// 			{
// 				sw.WriteString(character.get<0>(), 10);//char name
// 				sw.WriteByte(0x01);
// 				sw.WriteShort(character.get<1>());//appr1
// 				sw.WriteShort(character.get<2>());//appr2
// 				sw.WriteShort(character.get<3>());//appr3
// 				sw.WriteShort(character.get<4>());//appr4
// 				sw.WriteShort(character.get<5>());//headappr
// 				sw.WriteShort(character.get<6>());//bodyappr
// 				sw.WriteShort(character.get<7>());//armappr
// 				sw.WriteShort(character.get<8>());//legappr
// 				sw.WriteShort(character.get<9>());//gender
// 				sw.WriteShort(character.get<10>());//skin
// 				sw.WriteShort(character.get<11>());//level
// 				sw.WriteInt64(character.get<12>());//exp
// 				sw.WriteShort(0);//strength
// 				sw.WriteShort(0);//vitality
// 				sw.WriteShort(0);//dexterity
// 				sw.WriteShort(0);//intelligence
// 				sw.WriteShort(0);//magic
// 				sw.WriteShort(0);//agility/charisma
// 				sw.WriteInt(character.get<13>());//apprcolor
// 				sw.WriteShort(0);//unused data
// 				sw.WriteShort(0);//unused data
// 				sw.WriteShort(0);//unused data
// 				sw.WriteShort(0);//unused data
// 				sw.WriteShort(0);//unused data
// 				sw.WriteString(character.get<14>(), 10);//char name
// 			}
// 		}
// // 		client->mutsocket.lock();
// // 		if (client->socket)
// // 			client->socket->write(sw.data, sw.position);
// // 		client->mutsocket.unlock();
// 		//need a way to "organize"? outgoing packets
// 	}
// 	SQLCATCH(DeleteClient(client, true))
// }
// 
// void LServer::RequestLogin(const std::shared_ptr<Client> & client, StreamRead & sr)
// {
// 	Session ses(sqlpool->get());
// 	StreamWrite sw;
// 
// 	string worldname = sr.ReadString(30);
// 
// 	sw.WriteInt(MSGID_RESPONSE_LOG);
// 	sw.WriteShort(MSGTYPE_CONFIRM);
// 	sw.WriteShort(UPPER_VERSION);
// 	sw.WriteShort(LOWER_VERSION);
// 	sw.WriteByte(0x01);
// 	sw.WriteShort(0);//dates \/
// 	sw.WriteShort(0);
// 	sw.WriteShort(0);
// 	sw.WriteShort(0);
// 	sw.WriteShort(0);
// 	sw.WriteShort(0);//dates /\
// 
// 	SendCharList(client, sw);
// 	sw.WriteInt(500);
// 	sw.WriteInt(500);
// 	client->SWrite(sw);
// }
// 
// void LServer::CreateCharacter(const std::shared_ptr<Client> & client, StreamRead & sr)
// {
// 	Session ses(sqlpool->get());
// 	StreamWrite sw;
// 
// 	try
// 	{
// 		uint16_t strength, vitality, dexterity, intelligence, magic, agility;
// 		uint8_t gender, skin, hairstyle, haircolor, underwear;
// 
// 		string playername, worldname;
// 
// 		playername = sr.ReadString(10);
// 		worldname = sr.ReadString(30);
// 
// 		worldname = "Xtreme";
// 
// 		bool test = false;
// 		for (const GServer * gs : Gate::GetSingleton().gameserver)
// 		{
// 			if (gs->servername == worldname)
// 			{
// 				test = true;
// 				break;
// 			}
// 		}
// 
// 		if (!test)
// 		{
// 			//server doesn't exist
// 			sw.WriteInt(MSGID_RESPONSE_LOG);
// 			sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERFAILED);
// 			client->SWrite(sw);
// 			return;
// 		}
// 
// 		uint8_t charcount = 0;
// 
// 		{
// 			Statement select(ses);
// 			select << "SELECT COUNT(*) FROM char_database WHERE account_name=? AND servername=?", use(client->account), use(worldname), into(charcount), now;
// 
// 			if (charcount > 3)
// 			{
// 				sw.WriteInt(MSGID_RESPONSE_LOG);
// 				sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERFAILED);
// 				client->SWrite(sw);
// 				return;
// 			}
// 		}
// 
// 		{
// 			Statement select(ses);
// 			select << "SELECT COUNT(*) FROM char_database WHERE char_name=? AND servername=?", use(playername), use(worldname), into(charcount), now;
// 
// 			if (charcount > 0)
// 			{
// 				sw.WriteInt(MSGID_RESPONSE_LOG);
// 				sw.WriteShort(LOGRESMSGTYPE_ALREADYEXISTINGCHARACTER);
// 				client->SWrite(sw);
// 				return;
// 			}
// 		}
// 
// 		gender = sr.ReadByte();
// 		skin = sr.ReadByte();
// 		hairstyle = sr.ReadByte();
// 		haircolor = sr.ReadByte();
// 		underwear = sr.ReadByte();
// 
// 		strength = sr.ReadByte();
// 		vitality = sr.ReadByte();
// 		dexterity = sr.ReadByte();
// 		intelligence = sr.ReadByte();
// 		magic = sr.ReadByte();
// 		agility = sr.ReadByte();
// 
// 		if (((strength + vitality + dexterity + intelligence + magic + agility) != 70)
// 			|| strength < 10 || strength>14 || vitality < 10 || vitality>14 || dexterity < 10 || dexterity>14
// 			|| intelligence < 10 || intelligence>14 || magic < 10 || magic>14 || agility < 10 || agility>14)
// 		{
// 			sw.WriteInt(MSGID_RESPONSE_LOG);
// 			sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERFAILED);
// 			client->SWrite(sw);
// 			return;
// 		}
// 		uint32_t appr1;
// 		appr1 = (hairstyle << 8) | (haircolor << 4) | underwear;
// 		uint16_t HP, MP, SP;
// 		HP = (vitality * 8) + (strength * 2) + (intelligence * 2) + 8;
// 		MP = (magic * 3) + (intelligence * 2) + 2;
// 		SP = strength + 17;
// 
// 		{
// 			Statement select(ses);
// 			select << "INSERT INTO char_database (account_name, servername, char_name, strength, vitality, dexterity, intelligence, magic, agility, appr1, gender, skin, hairstyle, haircolor, underwear, hp, mp, sp) "
// 				"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",
// 				use(client->account), use(worldname), use(playername), use(strength), use(vitality), use(dexterity), use(intelligence), use(magic), use(agility), use(appr1), use(gender), use(skin), use(hairstyle), use(haircolor), use(underwear), use(HP), use(MP), use(SP), now;
// 		}
// 
// 		uint64_t insertid = 0;
// 		{
// 			Statement select(ses);
// 			select << "SELECT LAST_INSERT_ID();", into(insertid), now;
// 		}
// 
// 		for (uint8_t s = 0; s < 24; ++s)//24 skills total
// 		{
// 			if (/*s == 4 ||*/ s == 5 || s == 6 || s == 7 || s == 8 || s == 9 || s == 10 || s == 11 || s == 14 || s == 19 || s == 21)// All attack skills starts at 20%
// 			{
// 				Statement select(ses);
// 				select << "INSERT INTO skill (charid, skillid, skillmastery, skillssn) VALUES (?,?,20,0);", use(insertid), use(s), now;
// 			}
// 			else if (s == 3 || s == 23) // Magic Res / Poison Res starts at 2%
// 			{
// 				Statement select(ses);
// 				select << "INSERT INTO skill (charid, skillid, skillmastery, skillssn) VALUES (?,?,2,0);", use(insertid), use(s), now;
// 			}
// 			else// All crafting skills starts at 0% Magic skills starts at 0%
// 			{
// 				Statement select(ses);
// 				select << "INSERT INTO skill (charid, skillid, skillmastery, skillssn) VALUES (?,?,0,0);", use(insertid), use(s), now;
// 			}
// 		}
// 
// 		sw.WriteInt(MSGID_RESPONSE_LOG);
// 		sw.WriteShort(LOGRESMSGTYPE_NEWCHARACTERCREATED);
// 		sw.WriteString(playername.c_str(), 10);
// 		SendCharList(client, sw);
// 		client->SWrite(sw);
// 	}
// 	SQLCATCH(DeleteClient(client, true));
// }
// 
// void LServer::DeleteCharacter(const std::shared_ptr<Client> & client, StreamRead & sr)
// {
// 	Session ses(sqlpool->get());
// 	StreamWrite sw;
// 	try
// 	{
// 		string playername = sr.ReadString(10);
// 		string worldname = sr.ReadString(30);
// 
// 		worldname = "Xtreme";
// 
// 		uint32_t charid = 0;
// 		{
// 			Statement select(ses);
// 			select << "SELECT charid FROM char_database WHERE account_name=? AND char_name=?", use(client->account), use(playername), into(charid), now;
// 		}
// 		if (charid == 0)
// 		{
// 			sw.WriteInt(MSGID_RESPONSE_LOG);
// 			sw.WriteShort(LOGRESMSGTYPE_NOTEXISTINGCHARACTER);
// 			client->SWrite(sw);
// 			return;
// 		}
// 
// 		if (Gate::GetSingleton().is_account_in_use(client))
// 		{
// 			sw.WriteInt(MSGID_RESPONSE_LOG);
// 			sw.WriteShort(ENTERGAMERESTYPE_PLAYING);
// 			client->SWrite(sw);
// 		}
// 		else
// 		{
// 			{
// 				Statement select(ses);
// 				select << "DELETE FROM char_database WHERE char_name=? LIMIT 1;", use(playername), now;
// 			}
// 					{
// 						Statement select(ses);
// 						select << "DELETE FROM skill WHERE CharID=?;", use(charid), now;
// 					}
// 					{
// 						Statement select(ses);
// 						select << "DELETE FROM item WHERE CharID=?;", use(charid), now;
// 					}
// 			sw.WriteInt(MSGID_RESPONSE_LOG);
// 			sw.WriteShort(LOGRESMSGTYPE_CHARACTERDELETED);
// 			sw.WriteByte(0x01);
// 			SendCharList(client, sw);
// 			client->SWrite(sw);
// 		}
// 	}
// 	SQLCATCH(DeleteClient(client, true));
// }
// 
// void LServer::EnterGame(const std::shared_ptr<Client> & client, StreamRead & sr)
// {
// 	Session ses(sqlpool->get());
// 	StreamWrite sw;
// 	try
// 	{
// 		string playername = sr.ReadString(10);
// 		uint16_t entertype = sr.ReadShort();
// 		string worldname = sr.ReadString(30);
// 		string commandline = sr.ReadString(120);
// 
// 		//client is requesting to disconnect currently connected player
// 		if (entertype == ENTERGAMEMSGTYPE_NOENTER_FORCEDISCONN)
// 		{
// 			//send disconnect to all online servers
// 			for (auto gs : Gate::GetSingleton().gameserver)
// 			{
// 				sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 				sw.WriteShort(ENTERGAMERESTYPE_FORCEDISCONN);
// 				client->SWrite(sw);
// 
// 				//check if client is even in game
// 				std::shared_ptr<Client> clientfound;
// 
// 				{
// 					boost::shared_lock_guard<boost::shared_mutex> lock(Gate::GetSingleton().mutclientlist);
// 					bool accountfound = false;
// 					for (std::shared_ptr<Client> & clnt : gs->clientlist)
// 					{
// 						if (clnt->account == client->account)
// 						{
// 							//account found
// 							accountfound = true;
// 							clientfound = clnt;
// 							break;
// 						}
// 					}
// 				}
// 
// 				//client found? send force disconnect if so
// 
// 				if (clientfound != nullptr)
// 				{
// 					clientfound->Notify(nullptr, NOTIFY_FORCEDISCONN);
// 				}
// 			}
// 			return;
// 		}
// 
// 		worldname = "Xtreme";
// 
// 		uint8_t charcount = 0;
// 
// 		Statement select(ses);
// 		uint32_t charid = 0;
// 		string mapname;
// 		select << "SELECT charid,maploc,COUNT(*) FROM char_database WHERE account_name=? AND char_name=? AND servername=?", use(client->account), use(playername), use(worldname), into(charid), into(mapname), into(charcount), now;
// 
// 		if (charcount == 0)
// 		{
// 			sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 			sw.WriteShort(ENTERGAMERESTYPE_REJECT);
// 			sw.WriteByte(4);
// 			client->SWrite(sw);
// 		}
// 		else
// 		{
// 			auto getGS = [&client, &worldname](string mapname){
// 				//check if world server up
// 				for (auto gs : Gate::GetSingleton().gameserver)
// 				{
// 					if (gs->worldname == worldname)
// 					{
// 						for (auto map : gs->maplist)
// 						{
// 							if (map->name == mapname)
// 							{
// 								client->map = map;
// 								return gs;
// 							}
// 						}
// 					}
// 				}
// 				return (GServer*)0;
// 			};
// 			GServer * pgs = getGS(mapname);
// 
// 
// 
// 			if (pgs == nullptr)
// 			{
// 				//server not up?
// 				sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 				sw.WriteShort(ENTERGAMERESTYPE_REJECT);
// 				sw.WriteByte(5);
// 				client->SWrite(sw);
// 			}
// 			else
// 			{
// 				//server is up
// 
// 				//check if account or char already logged in
// 
// 				//there is a far better way to accomplish this. use this for now, but make it better later? or keep?
// 				//can keep track of logged in status via db, will add that eventually anyway
// 				//login can keep its own list of matching data to not have to lock the gameserver list - better a slow login internal query than slow game thread
// 
// 
// 				std::shared_ptr<Client> clientfound;
// 
// 				bool accountfound = false;
// 				{
// 					boost::shared_lock_guard<boost::shared_mutex> lock(Gate::GetSingleton().mutclientlist);
// 					for (std::shared_ptr<Client> & clnt : pgs->clientlist)
// 					{
// 						if (clnt->account == client->account)
// 						{
// 							//BUG: potential bug point if charID == 0 - only occurs on unsuccessful login
// 							//account found
// 							accountfound = true;
// 							if (clnt->charid == charid)
// 							{
// 								//exact char found
// 								clientfound = clnt;
// 								break;
// 							}
// 						}
// 					}
// 				}
// 
// 				if (clientfound && clientfound->disconnecttime > 0)
// 				{
// 					//client has recently disconnected
// 					//at this point, can do some fancy swapping of socket and place this current active socket
// 					// into the old client object letting them retake control of the disconnected session
// 					// - to do that though, it'd need to resend initial data - need a special case in GServer
// 					// - to not do the usual map data stuff when init data is sent or risk duplicating client
// 					// - objects in mapdata - also needs a case in GServer to not reject another initdata
// 					// - from an already connected client (used to be the cause of item duping on Int'l)
// 					// - and will need code in place to handle this specific case
// 					if (clientfound->socket)
// 					{
// 						//false alarm of some sort (or other issue) should probably breakpoint this or have some sort
// 						//of check if client even still exists. socket should never be set if disconnecttime was set
// 						__debugbreak();
// 					}
// 					else
// 					{
// 						clientfound->disconnecttime = 0;
// 						clientfound->socket = client->socket;
// 						clientfound->socket->client_ = clientfound;
// 						client->socket.reset();
// 
// 						//since nothing is technically associated with this client, just remove it from the list to force it to delete
// 
// 						pgs->clientlist.remove(client);
// 
// 						if (clientfound->map != nullptr)
// 						{
// 							clientfound->currentstatus = 2;
// 							clientfound->name = playername;
// 							//pgs->clientlist.push_back(client);
// 							//clientlist.remove(client);
// 
// 							sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 							sw.WriteShort(ENTERGAMERESTYPE_CONFIRM);
// 							sw.WriteString(string("127.0.0.1"), 16);
// 							sw.WriteShort(2848);
// 							sw.WriteString(worldname, 20);
// 							clientfound->SWrite(sw);
// 							clientfound->gserver = pgs;
// 						}
// 						else
// 						{
// 							sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 							sw.WriteShort(ENTERGAMERESTYPE_REJECT);
// 							sw.WriteByte(5);
// 							clientfound->SWrite(sw);
// 						}
// 					}
// 				}
// 				else if (accountfound)
// 				{
// 					//account already logged in and not resuming object
// 					sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 					sw.WriteShort(ENTERGAMERESTYPE_PLAYING);
// 					client->SWrite(sw);
// 				}
// 				else
// 				{
// 					if (client->map != nullptr)
// 					{
// 						client->currentstatus = 2;
// 						client->name = playername;
// 						pgs->clientlist.push_back(client);
// 						clientlist.remove(client);
// 						client->gserver = pgs;
// 
// 						sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 						sw.WriteShort(ENTERGAMERESTYPE_CONFIRM);
// 						sw.WriteString(string("127.0.0.1"), 16);
// 						sw.WriteShort(2848);
// 						sw.WriteString(worldname, 20);
// 						client->SWrite(sw);
// 					}
// 					else
// 					{
// 						sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 						sw.WriteShort(ENTERGAMERESTYPE_REJECT);
// 						sw.WriteByte(3);
// 						client->SWrite(sw);
// 					}
// 				}
// 
// 
// 			}
// 		}
// 	}
// 	SQLCATCH(DeleteClient(client, true))
// 		catch (exception & e)
// 	{
// 		//this should never trigger - future info sake, this is thrown when a lock is attempted to be obtained when already owning
// 		log->critical(fmt::format("SocketThread() Exception: %s", string(e.what())));
// 		sw.WriteInt(MSGID_RESPONSE_ENTERGAME);
// 		sw.WriteShort(ENTERGAMERESTYPE_REJECT);
// 		sw.WriteByte(3);
// 		client->SWrite(sw);
// 	}
// }
