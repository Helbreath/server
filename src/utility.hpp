
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


}
