// xtreme-server.cpp : Defines the entry point for the console application.
//

#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "LServer.h"
#include "GServer.h"
#include "Server.h"
#include "Gate.h"

Gate * gateserver = 0;

#ifndef WIN32
void sigfunc(int sig_no)
{
	gateserver->consoleLogger->critical(Poco::format("Signal received: %?d", sig_no));
    if (sig_no == SIGINT)
    {
        gateserver->consoleLogger->critical("Received SIGINT. Shutting down.");
        gateserver->serverstatus = SERVERSTATUS_SHUTDOWN;
        gateserver->handle_stop();
	}
	else if (sig_no == SIGHUP)
    {
        gateserver->consoleLogger->critical("Received SIGHUP. Reloading configs.");
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

	gateserver = new Gate();


	//Initialize server and load config files
	if (!gateserver->Init()) { delete gateserver;return 0; }

	//Connect to SQL server
	if (!gateserver->ConnectSQL()) { delete gateserver;return 0; }

	//Open sockets
	if (!gateserver->InitSockets()) {delete gateserver;return 0; }


	// Run the server until stopped.
	gateserver->run();

	//shouldn't really be here - should be where the server is stopped via remote call or console or something (likely remote call)
	gateserver->handle_stop();

	delete gateserver;

	return 0;
}
