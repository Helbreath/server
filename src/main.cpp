// xtreme-server.cpp : Defines the entry point for the console application.
//

#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "Gate.h"
#include "LServer.h"
#include "GServer.h"
#include "Server.h"
#include "XLogger.h"


#ifdef WIN32
#define X_PAUSE system("pause")
#else
#define X_PAUSE

#endif
#ifndef WIN32
void sigfunc(int sig_no)
{
	GServer::GetInstance()->logger->critical(Poco::format("Signal received: %?d", sig_no));
    if (sig_no == SIGINT)
    {
		GServer::GetInstance()->logger->critical("Received SIGINT. Shutting down.");
		GServer::GetInstance()->serverstatus = SERVERSTATUS_SHUTDOWN;
		GServer::GetInstance()->handle_stop();
	}
	else if (sig_no == SIGHUP)
    {
		GServer::GetInstance()->logger->critical("Received SIGHUP. Reloading configs.");
    }
}
#endif

int main(int argc, char * argv[])
{
#ifndef WIN32
	struct sigaction sa={0};
	sa.sa_handler = &sigfunc;
	//sigaction(SIGINT, &sa, 0);
	sigaction(SIGHUP, &sa, 0);
	sigaction(SIGUSR1, &sa, 0);
	sigaction(SIGUSR2, &sa, 0);
#else
	// 	if (SetConsoleCtrlHandler( (PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE)
	// 	{
	// 		// unable to install handler...
	// 		// display message to the user
	// 		printf("Unable to install handler!\n");
	// 		return -1;
	// 	}//TODO: make a windows based control+c catch function?
#endif

	XLogger::CreateInstance();
	Gate::CreateInstance();

	Gate::GetSingleton()->logger = XLogger::GetSingleton();
	

	//Initialize server and load config files
	if (!Gate::GetSingleton()->Init("config.json")) { Gate::DestroyInstance(); X_PAUSE; return 0; }

	//Connect to SQL server
	if (!Gate::GetSingleton()->ConnectSQL()) { Gate::DestroyInstance(); X_PAUSE; return 0; }

	//Open sockets
	if (!Gate::GetSingleton()->InitSockets()) { Gate::DestroyInstance(); X_PAUSE; return 0; }


	// Run the server until stopped.
	Gate::GetSingleton()->run();

	//shouldn't really be here - should be where the server is stopped via remote call or console or something (likely remote call)
	Gate::GetSingleton()->handle_stop();

	Gate::DestroyInstance();
	LServer::DestroyInstance();
	XLogger::DestroyInstance();

	X_PAUSE;

	return 0;
}
