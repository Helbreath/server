
#pragma once

#ifdef WIN32
#include <SDKDDKVer.h>
#include <tchar.h>
#include <direct.h>
#include <process.h>
#endif

#include <thread>

#include <stdint.h>
#include <stdarg.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <queue>

#include <stdio.h>

#include <memory>
#include <memory.h>

//#include <cmath>
//#include <math.h>

//TODO: need a new way to detect speed hacking and not "you can speed hack 10 steps then slow down"
#define SPEEDCHECKTURNS 10

#define MAXCRUSADESTRUCTURES		300
#define MAXAGRICULTURE				200

#define PFMABSORBVAL					0.50

//TODO: time you can spend in enemy maps - needs some changing to the system of raiding
#define RAIDTIME				600

//TODO: should not be hard coded
#define GOLDDROPMULTIPLIER			2
#define EXPMULTIPLIER				355

//TODO: what are these and why do they exist hardcoded?
#define BISLEGOLDCOST				100
#define ARENAGOLDCOST				500


#define HITRATIOFACTOR		80.0f
#define MAXSKILLPOINTS		3000


extern char itoh(int num);
extern int htoi(char hex);
extern void a_swap(unsigned char * a, unsigned char * b);
extern void ByteSwap5(unsigned char * b, int n);
extern uint64_t unixtime();

extern char * strtolower(char * x);
extern std::string makesafe(std::string in);
extern size_t ci_find(const std::string& str1, const std::string& str2);
extern bool ci_equal(char ch1, char ch2);
extern double randn_notrig(double mu=0.0, double sigma=1.0);
extern int32_t ndice(int _throw, int range);
extern int32_t dice(int _throw, int range);
extern void SetNibble(uint32_t &var, uint8_t index, uint8_t val);
extern void SetBit(uint32_t &var, uint8_t index, bool val);

#define SERVERSTATUS_STOPPED 1
#define SERVERSTATUS_STARTING 2
#define SERVERSTATUS_ONLINE 3
#define SERVERSTATUS_SHUTDOWN 4


#define ByteSwap(x) ByteSwap5((unsigned char *) &x, sizeof(x))

#ifdef WIN32

#define _CRT_SECURE_NO_WARNINGS

#ifndef VA_COPY
# ifdef HAVE_VA_COPY
#  define VA_COPY(dest, src) va_copy(dest, src)
# else
#  ifdef HAVE___VA_COPY
#   define VA_COPY(dest, src) __va_copy(dest, src)
#  else
#   define VA_COPY(dest, src) (dest) = (src)
#  endif
# endif
#endif

#define INIT_SZ 1024

extern int vasprintf(char **str, const char *fmt, va_list ap);
extern int asprintf(char **str, const char *fmt, ...);

#else
extern void __debugbreak();
#endif

#ifndef WIN32
#define strtok_s strtok_r
#define _atoi64 atoll
#define sprintf_s snprintf
#define strcpy_s(a,b,c) strcpy(a,c)
#endif

#ifdef WIN32
#define DBL "%Lf"
#define DBL2 "Lf"
#define XI64 "%I64d"
#else
#define DBL "%f"
#define DBL2 "f"
#define XI64 "%lld"
#endif

#ifdef WIN32
#define SLEEP(a) Sleep(a)
#else
#define SLEEP(a) { struct timespec req={0}; req.tv_sec = 0; req.tv_nsec = 1000000 * a; nanosleep(&req,0); }
#endif


#define SQLCATCH(a)	catch (Poco::Data::MySQL::ConnectionException& e)\
{\
	consoleLogger->error(Poco::format("ConnectionException: %s", e.displayText() ));\
	a; \
}\
catch (Poco::Data::MySQL::StatementException& e)\
{\
	consoleLogger->error(Poco::format("StatementException: %s", e.displayText() ));\
	a; \
}\
catch (Poco::Data::MySQL::MySQLException& e)\
{\
	consoleLogger->error(Poco::format("MySQLException: %s", e.displayText() ));\
	a; \
}\
catch (Poco::InvalidArgumentException& e)\
{\
	consoleLogger->error(Poco::format("InvalidArgumentException: %s", e.displayText() ));\
	a; \
}\
catch (Poco::NotFoundException&e)\
{\
	consoleLogger->error(Poco::format("NotFoundException: %s", e.displayText() ));\
	a; \
}\
catch (Poco::Exception& e)\
{\
	consoleLogger->fatal(Poco::format("Uncaught Exception: %s", e.displayText()));\
	a; \
}

