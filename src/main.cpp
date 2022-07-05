//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "core.h"
#if !defined(WIN32)
#include <signal.h>
#endif

int main(int argc, char * argv[])
{
    srand((uint32_t)time(nullptr));

#if defined(WIN32)
	ix::initNetSystem();
#else
	signal(SIGPIPE, SIG_IGN);
#endif

	core core_{};

	core_.run();

#if defined(WIN32)
    ix::uninitNetSystem();
#endif

	return 0;
}
