
#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>
#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>
#endif
#else
#error "Cannot define getPeakRSS( ) or getCurrentRSS( ) for an unknown OS."
#endif

#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <spdlog/fmt/fmt.h>
#include <random>
#include <unordered_map>

static inline int64_t tilecount;

#define AEGIS_TRACE(logger, ...) if (logger->should_log(spdlog::level::level_enum::trace)) logger->trace("[ " __FILE__ ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)

//#define DEF_WEAPONSPEEDLIMIT
#define DEF_MAXCRUSADESTRUCTURES		300
#define DEF_MAXAGRICULTURE				200
#define DEF_BEGINNERTOWNPROTECT

namespace hbx
{

enum class server_status
{
    uninitialized = 0,
    preready,
    online,
    closing,
    closed
};

struct point
{
    int32_t x = 0;
    int32_t y = 0;
};

struct rect
{
    int32_t left = 0;
    int32_t top = 0;
    int32_t right = 0;
    int32_t bottom = 0;
    rect() = default;
    rect(const rect & a) = default;
    ~rect() = default;
};

}

namespace hbx::utility
{

inline std::vector<std::string> tokenize(const std::string & s, const std::string & delim)
{
    std::vector<std::string> toks;
    std::string target;
    std::string delims = fmt::format(" \n{}", delim);
    std::string::size_type n, nxt, offset = 0;
    while (true)
    {
        n = s.find_first_not_of(delims, offset);
        if (n != std::string::npos)//non-space found
        {
            if (s[n] == '"')
            {
                //this is a quoted string so capture it all
                nxt = s.find_first_of('"', n + 1);
                if (nxt != std::string::npos)
                {
                    //second quote found
                    toks.emplace_back(s.substr(n + 1, nxt - n - 1));
                    offset = nxt + 1;
                }
                else
                {
                    //only single quote given
                    //assume to end of content
                    toks.emplace_back(s.substr(n, s.size() - n));
                    break;
                }
            }
            else//first non-space is regular text
            {
                nxt = s.find_first_of(delims, n + 1);
                if (nxt != std::string::npos)//found another space
                {
                    toks.emplace_back(s.substr(n, nxt - n));
                    offset = nxt;
                }
                else//no space found, end of content
                {
                    toks.emplace_back(s.substr(n, s.size() - n));
                    break;
                }
            }
        }
        else//nothing else found
            break;
    }
    return toks;
}

struct tokenizer
{
    tokenizer(const std::string & str, const std::string & delim)
    {
        results = tokenize(str, delim);
    }
    std::string get()
    {
        if (results.size() > count)
            return results[count++];
        throw std::out_of_range("token out of range");
    }
    uint64_t count = 0;
    std::vector<std::string> results;
};

template<class Func>
std::string perf_run(const std::string & name, Func f)
{
    std::stringstream ss;
    ss << "Running [" << name << "]\n";
    auto n = std::chrono::steady_clock::now();

    f();

    auto n_end = std::chrono::steady_clock::now();

    ss << "Time: [" << std::chrono::duration_cast<std::chrono::microseconds>(n_end - n).count() << "us]\n";
    return ss.str();
}

/// Returns the duration of the time since epoch for the timer.
/**
 * @param t The time_point to convert to the provided duration template parameter
 * @returns std::chrono::duration of the time since epoch start until the provided time_point
 */
template<class Duration, class int_type, typename ratio = std::nano>
constexpr Duration to_t(const std::chrono::duration<int_type, ratio> & t)
{
    return std::chrono::duration_cast<Duration>(t);
}

/// Returns the duration of the time since epoch for the timer.
/**
 * @param t The time_point to convert to the provided duration template parameter
 * @returns std::chrono::duration of the time since epoch start until the provided time_point
 */
template<typename Duration, typename T = std::chrono::steady_clock::time_point>
constexpr Duration to_t(const T & t)
{
    return std::chrono::duration_cast<Duration>(t.time_since_epoch());
}

/// Returns the value of the time since epoch for the timer.
/// For steady_clock this is last reboot.
/// For system_clock this is the Unix epoch.
/**
 * @param t The duration to convert to milliseconds
 * @returns int64_t of the time since epoch start until the provided time_point
 */
template<class S, typename ratio = std::nano>
constexpr S to_ms(const std::chrono::duration<S, ratio> & t)
{
    return to_t<std::chrono::milliseconds>(t).count();
}

/// Returns the value of the time since epoch for the timer.
/// For steady_clock this is last reboot.
/// For system_clock this is the Unix epoch.
/**
 * @param t The time_point to convert to milliseconds
 * @returns int64_t of the time since epoch start until the provided time_point
 */
template<typename T = std::chrono::steady_clock::time_point>
constexpr typename T::rep to_ms(const T & t)
{
    return to_t<std::chrono::duration<typename T::rep, std::milli>>(t.time_since_epoch()).count();
}

/// Converts an ISO8601 date string to an std::chrono::system_clock::time_point
/**
 * @param _time_t String of the timestamp
 * @returns std::chrono::system_clock::time_point String timestamp converted to time_point
 */
inline std::chrono::system_clock::time_point from_iso8601(const std::string & _time_t)
{
    std::tm tm = {};
    std::istringstream ss(_time_t);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

inline std::string to_iso8601(std::chrono::system_clock::time_point tp)
{
    auto itt = std::chrono::system_clock::to_time_t(tp);
    std::ostringstream ss;
    ss << std::put_time(gmtime(&itt), "%FT%TZ");
    return ss.str();
}

inline std::string to_iso8601(int64_t tp)
{
    auto itt = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>(std::chrono::milliseconds(tp)));
    std::ostringstream ss;
    ss << std::put_time(gmtime(&itt), "%FT%TZ");
    return ss.str();
}

/// Converts an Day, DD, Mth YYYY HH:MM:SS GMT date string to an std::chrono::system_clock::time_point
/**
 * @param _time_t String of the timestamp
 * @returns std::chrono::system_clock::time_point String timestamp converted to time_point
 */
inline std::chrono::system_clock::time_point from_http_date(const std::string & _time_t)
{
    std::tm tm = {};
    std::istringstream ss(_time_t);
    ss >> std::get_time(&tm, "%a, %d %b %Y %T");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}
//Mon, 19 Nov 2018 06:51 : 17 GMT

inline std::string url_encode(const std::string & value)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::value_type c : value)
    {
        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char)c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

inline std::string escape_quotes(const std::string & value)
{
    std::ostringstream escaped;

    for (std::string::value_type c : value)
    {
        if (c == '"')
            escaped << '\\';
        escaped << c;
    }
    return escaped.str();
}

inline char random_letter()
{
    return (rand() % 2) ? char(rand() % 26 + 'a') : char(rand() % 26 + 'A');
}

inline std::string random_string(const std::size_t length)
{
    std::string temp;
    temp.reserve(length);

    for (std::size_t i = 0; i < length; ++i)
        temp.push_back(random_letter());

    return temp;
}

inline std::string uptime_str(std::chrono::steady_clock::time_point _start) noexcept
{
    using seconds = std::chrono::duration<int, std::ratio<1, 1>>;
    using minutes = std::chrono::duration<int, std::ratio<60, 1>>;
    using hours = std::chrono::duration<int, std::ratio<3600, 1>>;
    using days = std::chrono::duration<int, std::ratio<24 * 3600, 1>>;

    std::stringstream ss;
    std::chrono::time_point<std::chrono::steady_clock> now_t = std::chrono::steady_clock::now();

    auto time_is = now_t - _start;
    auto d = std::chrono::duration_cast<days>(time_is).count();
    time_is -= days(d);
    auto h = std::chrono::duration_cast<hours>(time_is).count();
    time_is -= hours(h);
    auto m = std::chrono::duration_cast<minutes>(time_is).count();
    time_is -= minutes(m);
    auto s = std::chrono::duration_cast<seconds>(time_is).count();

    if (d)
        ss << d << "d ";
    if (h)
        ss << h << "h ";
    if (m)
        ss << m << "m ";
    ss << s << "s ";
    return ss.str();
}

inline std::string format_bytes(uint64_t size)
{
    if ((size > 1024ull * 5) && (size < 1024ull * 1024 * 5))// over 5KB and up to 5MB show KB
    {
        return fmt::format("{:.3f} KB", double(size) / 1024);
    }
    if ((size > 1024ull * 1024 * 5) && (size < 1024ull * 1024 * 1024 * 5))// over 5MB and up to 5GB show MB
    {
        return fmt::format("{:.3f} MB", (double(size) / 1024) / 1024);
    }
    if (size > 1024ull * 1024 * 1024 * 5)// over 5GB show GB
    {
        return fmt::format("{:.3f} GB", ((double(size) / 1024) / 1024) / 1024);
    }
    return fmt::format("{} B", size);
}

template<typename rate = std::chrono::milliseconds>
static uint64_t current_time()
{
    return std::chrono::duration_cast<rate>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//////////////////////////////////////////////////////////////////////////
/*
* Author:  David Robert Nadeau
* Site:    http://NadeauSoftware.com/
* License: Creative Commons Attribution 3.0 Unported License
*          http://creativecommons.org/licenses/by/3.0/deed.en_US
*/
/**
* Returns the peak (maximum so far) resident set size (physical
* memory use) measured in bytes, or zero if the value cannot be
* determined on this OS.
*/
size_t getPeakRSS();
size_t getCurrentRSS();

inline size_t getPeakRSS()
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return (size_t)info.PeakWorkingSetSize;

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
#if defined(__APPLE__) && defined(__MACH__)
    return (size_t)rusage.ru_maxrss;
#else
    return (size_t)(rusage.ru_maxrss * 1024L);
#endif

#else
    /* Unknown OS ----------------------------------------------- */
    return (size_t)0L;			/* Unsupported. */
#endif
}

/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 */
inline size_t getCurrentRSS()
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return (size_t)info.WorkingSetSize;

#elif defined(__APPLE__) && defined(__MACH__)
    /* OSX ------------------------------------------------------ */
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount) != KERN_SUCCESS)
        return (size_t)0L;		/* Can't access? */
    return (size_t)info.resident_size;

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    /* Linux ---------------------------------------------------- */
    long rss = 0L;
    FILE * fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL)
        return (size_t)0L;		/* Can't open? */
    if (fscanf(fp, "%*s%ld", &rss) != 1)
    {
        fclose(fp);
        return (size_t)0L;		/* Can't read? */
    }
    fclose(fp);
    return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);

#else
    /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (size_t)0L;			/* Unsupported. */
#endif
}
//////////////////////////////////////////////////////////////////////////




inline void a_swap(unsigned char * a, unsigned char * b)
{
    char c, d;
    c = *a;
    d = *b;
    *b = c;
    *a = d;
}
inline void ByteSwap5(unsigned char * b, int n)
{
    int i = 0;
    int j = n - 1;
    while (i < j)
    {
        a_swap(&b[i], &b[j]);
        i++, j--;
    }
}

inline int htoi(char hex)
{
    switch (hex)
    {
        case 49:
            return 1;
        case 50:
            return 2;
        case 51:
            return 3;
        case 52:
            return 4;
        case 53:
            return 5;
        case 54:
            return 6;
        case 55:
            return 7;
        case 56:
            return 8;
        case 57:
            return 9;
        case 48:
            return 0;
        case 97://a
            return 10;
        case 98://b
            return 11;
        case 99://c
            return 12;
        case 100://d
            return 13;
        case 101://e
            return 14;
        case 102://f
            return 15;
    }
    return 0;
}
inline char itoh(int num)
{
    switch (num)
    {
        case 1:
            return 49;
        case 2:
            return 50;
        case 3:
            return 51;
        case 4:
            return 52;
        case 5:
            return 53;
        case 6:
            return 54;
        case 7:
            return 55;
        case 8:
            return 56;
        case 9:
            return 57;
        case 0:
            return 48;
        case 10://a
            return 97;
        case 11://b
            return 98;
        case 12://c
            return 99;
        case 13://d
            return 100;
        case 14://e
            return 101;
        case 15://f
            return 102;
    }
    return 0;
}

inline bool ci_equal(char ch1, char ch2)
{
    return toupper((unsigned char)ch1) == toupper((unsigned char)ch2);
}

inline size_t ci_find(const std::string & str1, const std::string & str2)
{
    std::string::const_iterator pos = std::search(str1.begin(), str1.end(), str2.
        begin(), str2.end(), ci_equal);
    if (pos == str1.end())
        return std::string::npos;
    else
        return pos - str1.begin();
}

inline std::string makesafe(std::string in)
{
    std::stringstream ss;
    for (char i : in)
    {
        if (((i >= '0') && (i <= '9')) || ((i >= 'A') && (i <= 'Z')) || ((i >= 'a') && (i <= 'z')) || (i == '_') || (i == '.') || (i == '/') || (i == ':') || (i == '@') || (i == '|'))
            ss << i;
        else
            ss << ' ';
    }
    return ss.str();
}

inline char * strtolower(char * x)
{
    for (std::size_t i = 0; i < strlen(x); ++i)
    {
        if (x[i] >= 65 && x[i] <= 90)
        {
            x[i] += 32;
        }
    }
    return x;
}


inline double randn_notrig(double mu = 0.0, double sigma = 1.0)
{
    static bool deviateAvailable = false;	//	flag
    static double storedDeviate;			//	deviate from previous calculation
    double polar, rsquared = 0, var1, var2;

    //	If no deviate has been stored, the polar Box-Muller transformation is
    //	performed, producing two independent normally-distributed random
    //	deviates.  One is stored for the next round, and one is returned.
    if (!deviateAvailable) {

        //	choose pairs of uniformly distributed deviates, discarding those
        //	that don't fall within the unit circle
        do {
            var1 = 2.0 * (double(rand()) / double(RAND_MAX)) - 1.0;
            var2 = 2.0 * (double(rand()) / double(RAND_MAX)) - 1.0;
            rsquared = var1 * var1 + var2 * var2;
        } while (rsquared >= 1.0 || rsquared == 0.0);

        //	calculate polar transformation for each deviate
        polar = sqrt(-2.0 * log(rsquared) / rsquared);

        //	store first deviate and set flag
        storedDeviate = var1 * polar;
        deviateAvailable = true;

        //	return second deviate
        return var2 * polar * sigma + mu;
    }

    //	If a deviate is available from a previous call to this function, it is
    //	returned, and the flag is set to false.
    deviateAvailable = false;
    return storedDeviate * sigma + mu;
}

inline int64_t gen_rand(int64_t _min = 0, int64_t _max = INT64_MAX)
{
    static std::uniform_int_distribution<int64_t> distribution(_min, _max);
    static std::mt19937_64 engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    return distribution(engine);
}

inline int32_t ndice(int _throw, int range)
{
    int32_t i, ret;

    if (range <= 0 || _throw <= 0) return 0;

    ret = 0;
    for (i = 1; i <= _throw; i++) {

        ret += (rand() % range) + 1;
    }

    return ret;
}

inline int32_t dice(int _throw, int range)
{
    if (_throw > 10)
        return (int32_t)(double(_throw * range / 2) + (_throw * randn_notrig() / 3) + double(_throw / 2));
    return ndice(_throw, range);
    //for decimal mean values
    //return (_throw * range / 2) + (_throw * randn_notrig()/3) + ((range%2) / 2.0) + (_throw / 2);
}

#define GetNibble(var, index)		(  ( var & ( 0xF << (4*index) ) ) >> (4*index)  )
#define GetBit(var, index)			( (var & (1 << index)) >> index )
#define ToBit(index)					(1 << index)

inline void SetNibble(uint32_t & var, uint8_t index, uint8_t val)
{
    index *= 4;
    var &= ~(0xF << index);
    var |= val << index;
}

inline void SetBit(uint32_t & var, uint8_t index, bool val)
{
    var &= 0xffffffff - (0x1 << index);
    var |= val << index;
}



inline char cGetNextMoveDir(short sX, short sY, short dX, short dY)
{
    short absX, absY;
    char  cRet = 0;

    absX = sX - dX;
    absY = sY - dY;

    if ((absX == 0) && (absY == 0)) cRet = 0;

    else if (absX == 0) {
        if (absY > 0) cRet = 1;
        else if (absY < 0) cRet = 5;
    }
    else if (absY == 0) {
        if (absX > 0) cRet = 7;
        else if (absX < 0) cRet = 3;
    }
    else if ((absX > 0) && (absY > 0)) cRet = 8;
    else if ((absX < 0) && (absY > 0)) cRet = 2;
    else if ((absX > 0) && (absY < 0)) cRet = 6;
    else if ((absX < 0) && (absY < 0)) cRet = 4;

    return cRet;
}


inline void GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError)
{
    int dx, dy, x_inc, y_inc, error, index;
    int iResultX, iResultY, iDstCnt;

    if ((x0 == x1) && (y0 == y1)) {
        *pX = x0;
        *pY = y0;
        return;
    }

    error = *pError;

    iResultX = x0;
    iResultY = y0;
    iDstCnt = 0;

    dx = x1 - x0;
    dy = y1 - y0;

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy = -dy;
    }

    if (dx > dy)
    {
        for (index = 0; index <= dx; index++)
        {
            error += dy;
            if (error > dx)
            {
                error -= dx;
                iResultY += y_inc;
            }
            iResultX += x_inc;
            goto CALC_OK;
        }
    }
    else
    {
        for (index = 0; index <= dy; index++)
        {
            error += dx;
            if (error > 0)
            {
                error -= dy;
                iResultX += x_inc;
            }
            iResultY += y_inc;
            goto CALC_OK;
        }
    }

CALC_OK:;

    *pX = iResultX;
    *pY = iResultY;
    *pError = error;
}


inline void GetPoint2(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount)
{
    int dx, dy, x_inc, y_inc, error, index;
    int iResultX, iResultY, iCnt = 0;


    if ((x0 == x1) && (y0 == y1)) {
        *pX = x0;
        *pY = y0;
        return;
    }

    error = *pError;

    iResultX = x0;
    iResultY = y0;

    dx = x1 - x0;
    dy = y1 - y0;

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy = -dy;
    }

    if (dx > dy)
    {
        for (index = 0; index <= dx; index++)
        {
            error += dy;
            if (error > dx)
            {
                error -= dx;
                iResultY += y_inc;
            }
            iResultX += x_inc;
            iCnt++;
            if (iCnt >= iCount)
                goto CALC_OK;
        }
    }
    else
    {
        for (index = 0; index <= dy; index++)
        {
            error += dx;
            if (error > dy)
            {
                error -= dy;
                iResultX += x_inc;
            }
            iResultY += y_inc;
            iCnt++;
            if (iCnt >= iCount)
                goto CALC_OK;
        }
    }

CALC_OK:;

    *pX = iResultX;
    *pY = iResultY;
    *pError = error;
}


inline void GetDirPoint(char cDir, int * pX, int * pY)
{
    switch (cDir) {
        case 1:	(*pY)--; break;
        case 2:	(*pY)--; (*pX)++;	break;
        case 3:	(*pX)++; break;
        case 4:	(*pX)++; (*pY)++;	break;
        case 5:	(*pY)++; break;
        case 6:	(*pX)--; (*pY)++;	break;
        case 7:	(*pX)--; break;
        case 8:	(*pX)--; (*pY)--;	break;
    }

}


inline bool bEncode(char cKey, char * pStr)
{
    std::size_t i, iLen;

    // !!
    return true;

    iLen = strlen(pStr);
    for (i = 0; i <= iLen - 1; i++) {
        pStr[i] = pStr[i] ^ (cKey);
    }

    return true;
}


inline bool bDecode(char cKey, char * pStr)
{
    std::size_t i, iLen;

    // !!
    return true;

    iLen = strlen(pStr);
    for (i = 0; i <= iLen - 1; i++) {
        pStr[i] = pStr[i] ^ (cKey);
    }

    return true;
}


inline bool bCheckValidName(char * pStr)
{
    std::size_t i, iLen;

    iLen = strlen(pStr);
    for (i = 0; i < iLen; i++) {

        if ((pStr[i] == ',') || (pStr[i] == '=') || (pStr[i] == ' ') ||
            (pStr[i] == '\n') || (pStr[i] == '\t') || /*(pStr[i] == '.') ||*/
            (pStr[i] == '\\') || (pStr[i] == '/') || (pStr[i] == ':') ||
            (pStr[i] == '*') || (pStr[i] == '?') || (pStr[i] == '<') ||
            (pStr[i] == '>') || (pStr[i] == '|') || (pStr[i] == '"')) return false;

        /*		if ((i <= iLen-2) && ((unsigned char)pStr[i] >= 128)) {
                    if (((unsigned char)pStr[i] == 164) && ((unsigned char)pStr[i+1] >= 161) &&
                        ((unsigned char)pStr[i+1] <= 211)) {

                    }
                    else
                    if (((unsigned char)pStr[i] >= 176) && ((unsigned char)pStr[i] <= 200) &&
                        ((unsigned char)pStr[i+1] >= 161) && ((unsigned char)pStr[i+1] <= 254)) {

                    }
                    else return false;
                    i++;
                }*/

    }

    return true;
}

/*

inline void Temp()
{
    FILE * pSrcFile, * pDestFile, * pSrcFileA, * pSrcFileB;
    int i;
    char cTemp[100000];

    pSrcFile = fopen("middleland.amd", "rb");
    pDestFile = fopen("middleland.amd.result", "wb");

    pSrcFileA = fopen("middleland1.amd", "rb");
    pSrcFileB = fopen("middleland2.amd", "rb");

    fread(cTemp, 1, 256, pSrcFile);
    fread(cTemp, 1, 256, pSrcFileA);
    fread(cTemp, 1, 256, pSrcFileB);
    for (i = 1; i <= 444; i++)
        fread(cTemp, 1, 5240, pSrcFileB);

    memset(cTemp, 0, sizeof(cTemp));
    strcpy(cTemp, "MAPSIZEX = 824 MAPSIZEY = 824 TILESIZE = 10");


    fwrite(cTemp, 1, 256, pDestFile);


    for (i = 1; i <= 80; i++) {
        memset(cTemp, 0, sizeof(cTemp));
        fread((cTemp + 1500), 1, 5240, pSrcFileA);
        fwrite(cTemp, 1, 824 * 10, pDestFile);
    }

    memset(cTemp, 0, sizeof(cTemp));
    for (i = 1; i <= 68; i++) fwrite(cTemp, 1, 824 * 10, pDestFile);

    //148
    / *
    ZeroMemory(cTemp, sizeof(cTemp));
    for (i = 1; i <= 150; i++) fwrite(cTemp, 1, 824*10, pDestFile);
    * /


    for (i = 1; i <= 524; i++) {
        memset(cTemp, 0, sizeof(cTemp));
        fread((cTemp + 1500), 1, 5240, pSrcFile);
        fwrite(cTemp, 1, 824 * 10, pDestFile);
    }


    memset(cTemp, 0, sizeof(cTemp));
    for (i = 1; i <= 68; i++) fwrite(cTemp, 1, 824 * 10, pDestFile);

    for (i = 1; i <= 80; i++) {
        memset(cTemp, 0, sizeof(cTemp));
        fread((cTemp + 1500), 1, 5240, pSrcFileB);
        fwrite(cTemp, 1, 824 * 10, pDestFile);
    }

    memset(cTemp, 0, sizeof(cTemp));
    for (i = 1; i <= 2; i++) fwrite(cTemp, 1, 824 * 10, pDestFile);

    / *
    ZeroMemory(cTemp, sizeof(cTemp));
    for (i = 1; i <= 150; i++) fwrite(cTemp, 1, 824*10, pDestFile);
    * /

    fclose(pSrcFile);
    fclose(pDestFile);
    fclose(pSrcFileA);
    fclose(pSrcFileB);
}*/

static inline int SKILLCROPSTABLE[9][9] =
// 20  30  40  50  60  70  80  90 100
{
    42, 34, 27, 21, 16, 12,  9,  7,  6,  //20
    43, 40, 33, 27, 22, 18, 15, 13, 10,	//30
    44, 41, 38, 32, 27, 23, 20, 18, 13,  //40
    45, 42, 39, 36, 31, 27, 24, 22, 15,  //50
    46, 43, 40, 37, 34, 30, 27, 25, 16,  //60
    47, 44, 41, 38, 35, 32, 29, 27, 20,  //70
    48, 45, 42, 39, 36, 33, 30, 28, 23,  //80
    49, 46, 43, 40, 37, 34, 31, 28, 25,  //90
    50, 47, 44, 41, 38, 35, 32, 29, 26  //100
};

static inline int CROPSDROPTABLE[9][9] =
// 20  30  40  50  60  70  80  90 100
{
    40,  0,  0,  0,  0,  0,  0,  0,  0,  //20
    41, 38,  0,  0,  0,  0,  0,  0,  0,	//30
    43, 40, 36,  0,  0,  0,  0,  0,  0,  //40
    46, 42, 38, 35,  0,  0,  0,  0,  0,  //50
    50, 45, 41, 37, 33,  0,  0,  0,  0,  //60
    55, 49, 44, 40, 35, 31,  0,  0,  0,  //70
    61, 54, 48, 43, 38, 33, 30,  0,  0,  //80
    68, 60, 53, 47, 42, 37, 32, 28,  0,  //90
    76, 67, 59, 52, 46, 41, 35, 29, 24  //100
};

inline int probabilityTable(int x, int y, int iTable)
{
    int iProb;
    int iX, iY;

    iX = (x - 20) / 10;
    iY = (y - 20) / 10;

    if (iX < 0 || iX > 8) return 0;
    if (iY < 0 || iY > 8) return 0;

    iProb = dice(1, 100);

    if (iTable == 1) {
        if (SKILLCROPSTABLE[iX][iY] >= iProb)
            return SKILLCROPSTABLE[iX][iY];
        else
            return 0;
    }
    else if (iTable == 2) {
        if (CROPSDROPTABLE[iX][iY] >= iProb)
            return CROPSDROPTABLE[iX][iY];
        else
            return 0;
    }
    else return 0;
}

static inline std::unordered_map<uint8_t, const char *> g_socketMap;

}
