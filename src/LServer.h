// #pragma once
// 
// 
// #include "Server.h"
// #include "streams.h"
// 
// class LServer : public Server
// {
// public:
// 	static void CreateInstance() { _instance = new LServer(); }
// 	static LServer & GetSingleton() { return *_instance; }
// 	static void DestroyInstance() { delete _instance; }
// private:
// 	LServer();
// 	static LServer * _instance;
// public:
// 	~LServer();
// 
// 	// Initialize Server
// 	bool Init(string config);
// 
// 	void run();
// 	void handle_stop();
// 
// 	// Whether the timer thread is running or not
// 	bool timer_thread_running;
// 
// 	std::thread socketthread;
// 	std::thread runthread;
// 
// 	void TimerThread();
// 	void SocketThread();
// 
// 	void DeleteClient(std::shared_ptr<Client> client, bool deleteobj = false);
// 
// 	bool CheckLogin(std::shared_ptr<Client> client, string & account, string & pass);
// 	void SendCharList(std::shared_ptr<Client> client, StreamWrite & sw);
// 
// 	typedef Poco::Tuple<string, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint32_t, uint32_t, string> loginsqlcharacter;
// 	typedef std::vector<loginsqlcharacter> logincharacterlist;
// 
// 	void RequestLogin(const std::shared_ptr<Client> & client, StreamRead & sr);
// 	void CreateCharacter(const std::shared_ptr<Client> & client, StreamRead & sr);
// 	void DeleteCharacter(const std::shared_ptr<Client> & client, StreamRead & sr);
// 	void EnterGame(const std::shared_ptr<Client> & client, StreamRead & sr);
// };
// 
