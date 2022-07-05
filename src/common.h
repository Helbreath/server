

#pragma once

#ifdef WIN32
#include <SDKDDKVer.h>
#include <tchar.h>
#include <direct.h>
#include <process.h>
#endif

#include <sys/timeb.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <memory.h>
#include <ctime>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <cstdint>
#include <cstdarg>
#include <algorithm>
#include <cstdio>

#include "maps.h"

#define DEFAULTBAGSIZE	20
#define DEFAULTEXTRABAGSIZE	5
#define DEFAULTBANKSIZE 20
#define MAXITEMTYPES			2000 // 1500->2000 xRisenx
#define MAXNPCTYPES			200
#define MAXNPCTIERS			10
#define MAXPARTYMEMBERS		40
#define MAXSIDES				4
#define MAXBANKITEMS			120 // 120->300 xRisenx
#define MAXITEMS		75			// 50->75 xRisenx
#define MAXGUILDSMAN	128
#define MAXMAGICTYPE	100
#define MAXSKILLTYPE	25
#define MAXTELEPORTLIST	20

#define DEF_LIMITHUNTERLEVEL 100

enum Side
{
	NEUTRAL,
	ARESDEN,
	ELVINE,
	HOSTILE
};

static std::string sideName[MAXSIDES] = { "Traveler", "Aresden", "Elvine" };
static std::string sideMap[MAXSIDES] = { "default", "aresden", "elvine" };
static std::string sideMapJail[MAXSIDES] = { "default", "arejail", "elvjail" };
static std::string sideMapFarm[MAXSIDES] = { "default", "arefarm", "elvfarm" };
static std::string sideMapRes[MAXSIDES] = { "default", "resurr1", "resurr2" };

#define ITEMS_PER_TILE	3
#define MAXGROUNDITEMS	5000

#define TILECLEANTIME		15*60
#define TILECLEANTIMEPLAYER	30*60

#define SPECABLTYTIMESEC	1200


#define SEND_MAIL_COST			300
#define SEND_MAIL_ITEM_COST	700
#define GUILDBOARD_POST_COST	500
#define MAX_MAIL_MSG_LENGTH	400+1
#define MAX_MAIL_ITEMS			10

#define MAXITEMSOCKETS		4

// ----------------------------------Guild-------------------------------------------------------------------------------------
#define MAXGUILDBANKITEMS		75*4
#define GUILDSUMMONSGOLDCOST	5000
#define GUILDSUMMONSTIME		5*60

enum GuildUpgrades{
	GU_WAREHOUSE,
	GU_SUMMONS,
	GU_CAPTAINCY,
	GU_RAIDMASTERS,
	GU_HUNTMASTERS,
	GU_MAX
};

const struct GuildUpgrade{
	std::string name;
	GuildUpgrades type;
	uint8_t maxLvl;
	uint32_t costGold[5];
	uint32_t costMaj[5];
	uint32_t costCont[5];
}
gldUps[GU_MAX] =
{
	{
		"Warehouse", GU_WAREHOUSE, 4,
		{0, 200000, 500000, 1000000, 5000000},	// gold
		{0, 1000, 2000, 10000, 50000},							// maj
		{0, 1000, 3000, 8000, 20000}					// cont
	},{
		"Summons", GU_SUMMONS, 4,
		{0, 200000, 500000, 1000000, 5000000},
		{0, 1000, 2000, 10000, 50000},
		{0, 1000, 3000, 8000, 20000}
	},{
		"Captaincy", GU_CAPTAINCY, 4,
		{0, 100000, 400000, 1000000, 5000000},
		{0, 500, 1000, 8000, 20000},
		{0, 2000, 5000, 10000, 50000}
	},{
		"Raidmasters", GU_RAIDMASTERS, 4,
		{0, 500000, 1000000, 3000000, 6000000},
		{0, 1500, 3000, 12000, 24000},
		{0, 4000, 8000, 24000, 48000}
	},{
		"Huntmasters", GU_HUNTMASTERS, 4,
		{0, 500000, 1000000, 3000000, 6000000},
		{0, 1500, 3000, 12000, 24000},
		{0, 4000, 8000, 24000, 48000}
	}
};

const uint32_t maxGWHItems[5] = { 0, 75, 75*2, 75*3, 75*4 };


enum GuildRank
{
	GUILDRANK_NONE = 0,
	GUILDRANK_MASTER,
	GUILDRANK_CAPTAIN,
	GUILDRANK_HUNTMASTER,
	GUILDRANK_RAIDMASTER,
	GUILDRANK_GUILDSMAN,
	GUILDRANK_RECRUIT,
	GUILDRANK_MAX
};

//TODO: Make this customizable ingame via UI?
const struct GuildRankPermissions
{
	bool canDisband;
	bool canInvite;
	bool canBan;
	bool canSummon;
	bool canWithdrawWH;
	bool crusadeCommander;
	bool canPromote;
	bool canPing;
}

gldRankPerm[GUILDRANK_MAX] = {
	{true, true, true, true, true, true, true, true},		// GUILDRANK_MASTER
	{false, true, true, false, true, true, true, true},	// GUILDRANK_CAPTAIN
	{false, false, false, false, true, false, false, true},	// GUILDRANK_HUNTMASTER
	{false, false, false, false, true, false, false, true},	// GUILDRANK_RAIDMASTER
	{false, false, false, false, true, false, false, false},	// GUILDRANK_GUILDSMAN
	{false, false, false, false, false, false, false, false}	// GUILDRANK_RECRUIT
};

#define GUILDSTARTRANK		GUILDRANK_RECRUIT

//-----------------------------------------------------------------------------------------------------------------------

enum ChatType
{
	CHAT_NORMAL,
	CHAT_GUILD,
	CHAT_SHOUT,
	CHAT_NATIONSHOUT,
	CHAT_PARTY,
	CHAT_GM = 10,
	CHAT_WHISPER = 20,
	CHAT_MAX
};

//-----------------------------------------------------------------------------------------------------------------------

enum StatusFlags
{
	STATUS_INVISIBILITY =		1 << 4,	// 0x00000010
	STATUS_BERSERK =				1 << 5,	// 0x00000020
	STATUS_FROZEN =				1 << 6,	// 0x00000040
	STATUS_POISON =				1 << 7,	// 0x00000080

	STATUS_ANGELSTR =				1 << 12,	// 0x00001000
	STATUS_ANGELDEX =				1 << 13,	// 0x00002000
	STATUS_ANGELINT =				1 << 14,	// 0x00004000
	STATUS_ANGELMAG =				1 << 15,	// 0x00008000

	STATUS_RELICHOLDER = 		1 << 17,	// 0x00020000 - previously STATUS_HEROFLAG
	STATUS_AFK =					1 << 18,	// 0x00040000
	STATUS_GREENSLATE =			1 << 16,	// 0x00010000
	STATUS_REDSLATE =				1 << 22,	// 0x00400000
	STATUS_BLUESLATE =			1 << 23,	// 0x00800000
	STATUS_ILLUSIONMOVEMENT =	1 << 21,	// 0x00200000
	STATUS_ILLUSION =				1 << 24,	// 0x01000000
	STATUS_DEFENSESHIELD =		1 << 25,	// 0x02000000
	STATUS_PFM =					1 << 26,	// 0x04000000
	STATUS_PFA =					1 << 27,	// 0x08000000
	STATUS_PK =						1 << 20,	// 0x00100000
	STATUS_SIDE =					0xF0000000,
	STATUS_ALL =					0xFFFFFFFF,
	STATUS_ENEMYFLAGS =			STATUS_ALL - ( STATUS_PFA |
	STATUS_PFM |
	STATUS_DEFENSESHIELD |
	STATUS_ILLUSION |
	STATUS_POISON )
};

//-----------------------------------------------------------------------------------------------------------------------

enum GMFlags
{
	GMFLAG_INVINCIBLE =		1,
	GMFLAG_NOAGGRO =				1 << 1,
	GMFLAG_ETHEREAL =				1 << 2
};

enum Weather
{
	WEATHER_SUNNY,
	WEATHER_LIGHTRAIN,
	WEATHER_MEDIUMRAIN,
	WEATHER_HEAVYRAIN,
	WEATHER_LIGHTSNOW,
	WEATHER_MEDIUMSNOW,
	WEATHER_HEAVYSNOW
};

enum Element
{
	ELEMENT_NONE =			0,
	ELEMENT_EARTH =		1,
	ELEMENT_AIR =			2,
	ELEMENT_FIRE =			3,
	ELEMENT_WATER =		4,
	ELEMENT_MAX
};
//----------------------------------Events-------------------------------------------------------------------------------------

#define RELICVICTORYTIME		(10 _m)

enum EventType{
	ET_NONE,
	ET_CAPTURE,
	ET_DESTROY_SHIELD,
	ET_CRUSADE,
	ET_APOCALYPSE,
	ET_MONSTEREVENT, // Monster Kill Event xRisenx
	ET_MAX
};
//static const char * eventName[] = { "", "Capture the Relic", "Destroy the Shield", "Crusade", "Apocalypse", "MonsterEvent"};

struct Casualties{
	uint32_t deaths;
	uint32_t kills;
};

//----------------------------------------------------------------------------------------------------------------------------------------

enum HeroReq{
	HR_CAPE,
	HR_HELM,
	HR_CAP,
	HR_PLATE,
	HR_ROBE,
	HR_HAUBERK,
	HR_LEGGINGS,
	HR_MAX
};

static const struct HeroItemCost{
	uint32_t EK;
	uint32_t contribution;
}

heroPrice[HR_MAX] =
{
	{300,0},		// HR_CAPE
	{150,20},	// HR_HELM
	{100,20},	// HR_CAP
	{300,30},	// HR_PLATE
	{200,20},	// HR_ROBE
	{100,10},	// HR_HAUBERK
	{150,15}		// HR_LEGGINGS
};

static const uint32_t HeroItemID[HR_MAX][MAXSIDES-1][2] =
{
	{	// HR_CAPE
		{400, 400},		// ARESDEN
		{401, 401},		// ELVINE
	},
	{	// HR_HELM
		{403, 404},		// ARESDEN
		{405, 406},		// ELVINE
	},
	{	// HR_CAP
		{407, 408},		// ARESDEN
		{409, 410},		// ELVINE
	},
	{	// HR_PLATE
		{411, 412},		// ARESDEN
		{413, 414},		// ELVINE
	},
	{	// HR_ROBE
		{415, 416},		// ARESDEN
		{417, 418},		// ELVINE
	},
	{	// HR_HAUBERK
		{419, 420},		// ARESDEN
		{421, 422},		// ELVINE
	},
	{	// HR_LEGGINGS
		{423, 424},		// ARESDEN
		{425, 426},		// ELVINE
	}
};

//----------------------------------------------------------------------------------------------------------------------------------------

enum Directions{
	CENTER,
	NORTH,
	NORTHEAST,
	EAST,
	SOUTHEAST,
	SOUTH,
	SOUTHWEST,
	WEST,
	NORTHWEST
};

//----------------------------------------------------------------------------------------------------------------------------------------

#define atoul(str)			strtoul(str, 0, 10)
#define atoull(str)			_strtoui64(str, 0, 10)
#define strcpyn(dst, src)	strncpy(dst, src, sizeof(dst))

//#define logb(n, b)	(log(n)/log(b))

struct Point{
	int32_t x,y;

	bool operator == (Point p)
	{
		return (p.x == x && p.y == y) ? true : false;
	}

	bool operator != (Point p)
	{
		return !(p == *this);
	}

	Point operator ++ (int)
	{
		Point copy = *this;
		x++;
		y++;
		return copy;
	}

	Point operator -- (int)
	{
		Point copy = *this;
		x--;
		y--;
		return copy;
	}

	Point & operator += (const Point & p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}

	Point & operator -= (const Point & p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}

	Point operator + (const Point & p) const
	{
		Point copy = *this;
		return (copy += p);
	}

	Point operator - (const Point & p) const
	{
		Point copy = *this;
		return (copy -= p);
	}
};


#define TOTALACTION			10

#define OBJECTSTOP			0
#define OBJECTMOVE			1
#define OBJECTRUN			2
#define OBJECTATTACK		3
#define OBJECTMAGIC			4
#define OBJECTGETITEM		5
#define OBJECTDAMAGE		6
#define OBJECTDAMAGEMOVE	7
#define OBJECTATTACKMOVE	8
#define OBJECTDYING			10			   
#define OBJECTNULLACTION	100

#define OBJECTMOVE_CONFIRM				1001
#define OBJECTMOVE_REJECT				1010
#define OBJECTMOTION_CONFIRM			1020
#define OBJECTMOTION_ATTACK_CONFIRM		1030
#define OBJECTMOTION_REJECT				1040

#define HITRATIOFACTOR		80.0f
#define MAXSKILLPOINTS		3000

typedef struct hbxrect
{
	hbxrect(int32_t a, int32_t b, int32_t c, int32_t d) { left = a; top = b; right = c; bottom = d; }
	hbxrect(){ left = top = right = bottom = 0; };
	hbxrect( const hbxrect & r ) { left = r.left; top = r.top; right = r.right; bottom = r.bottom; }
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
} rect;

typedef struct hbxpoint
{
	hbxpoint(int32_t a, int32_t b) { x = a; y = b; }
	hbxpoint(){ x = y = 0; };
	int32_t  x;
	int32_t  y;
} point;


#define DYNAMICOBJECT_FIRE			1
#define DYNAMICOBJECT_FISH			2
#define DYNAMICOBJECT_FISHOBJECT	3
#define DYNAMICOBJECT_MINERAL1		4
#define DYNAMICOBJECT_MINERAL2		5
#define DYNAMICOBJECT_ARESDENFLAG	6
#define DYNAMICOBJECT_ELVINEFLAG	7
#define DYNAMICOBJECT_ICESTORM		8
#define DYNAMICOBJECT_SPIKE			9
#define DYNAMICOBJECT_PCLOUD_BEGIN	10
#define DYNAMICOBJECT_PCLOUD_LOOP	11
#define DYNAMICOBJECT_PCLOUD_END	12
#define DYNAMICOBJECT_FIRE2			13
#define DYNAMICOBJECT_FIRE3			14 //for FireBow




//#include <cmath>
//#include <math.h>

//TODO: need a new way to detect speed hacking and not "you can speed hack 10 steps then slow down"
#ifdef _DEBUG
#define SPEEDCHECKTURNS 1
#else
#define DOSPEEDCHECK
#define SPEEDCHECKTURNS 10
#endif

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
extern double randn_notrig(double mu = 0.0, double sigma = 1.0);
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
