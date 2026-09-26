#pragma once

#include <exception>
#include <iostream>
#include <thread>
#include <stdarg.h>
#include "Types.hpp"

#if _MSC_VER
    #define FNAME __FUNCTION__
#else
    #define FNAME __PRETTY_FUNCTION__
#endif


#undef ERROR

#define LOGGING 1

enum class LogLevels
{
    Trace,
    Info,
    Warning,
    Error,
};

inline void log_impl(LogLevels logLevel, const char* funcName, const char* format, ...) RELIVE_PRINTF_FMT(3, 4);
inline void log_impl(LogLevels logLevel, const char* funcName, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    switch (logLevel)
    {
        case LogLevels::Trace:
            printf("[T] ");
            break;
        case LogLevels::Info:
            printf("[I] ");
            break;
        case LogLevels::Warning:
            printf("[W] ");
            break;
        case LogLevels::Error:
            printf("[!] ");
            break;
    }
    printf("[TID %zu] [%s] ", std::hash<std::thread::id>{}(std::this_thread::get_id()), funcName);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

#ifdef LOGGING
    #define TRACE_ENTRYEXIT Logging::AutoLog __funcTrace(FNAME)
    #define LOG_TRACE(...) log_impl(LogLevels::Trace, FNAME, __VA_ARGS__)
    #define LOG_INFO(...) log_impl(LogLevels::Info, FNAME, __VA_ARGS__)
    #define LOG_WARNING(...) log_impl(LogLevels::Warning, FNAME, __VA_ARGS__)
    #define LOG_ERROR(...) log_impl(LogLevels::Error, FNAME, __VA_ARGS__)
    #define LOG(...) log_impl(LogLevels::Trace, FNAME, __VA_ARGS__)
#else
    #define TRACE_ENTRYEXIT
    #define LOG_TRACE(...)
    #define LOG_INFO(...)
    #define LOG_WARNING(...)
    #define LOG_ERROR(...)
    #define LOG(...)
#endif

[[noreturn]] inline void HOOK_FATAL(const char_type* errMsg)
{
    LOG_ERROR("%s", errMsg);
    abort();
}


class outbuf : public std::streambuf
{
public:
    outbuf()
    {
        setp(0, 0);
    }

    virtual int_type overflow(int_type c = traits_type::eof()) override
    {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

inline void RedirectIoStream(bool replace)
{
    static std::streambuf* sb = nullptr;
    if (replace)
    {
        if (!sb)
        {
            static outbuf ob;
            sb = std::cout.rdbuf(&ob);
        }
    }
    else
    {
        // make sure to restore the original so we don't get a crash on close!
        if (sb)
        {
            std::cout.rdbuf(sb);
        }
        sb = nullptr;
    }
}

namespace Logging {
class AutoLog final
{
public:
    AutoLog(const AutoLog&) = delete;
    AutoLog& operator=(const AutoLog&) = delete;
    AutoLog(const char_type* funcName)
        : mFuncName(funcName)
    {
        log_impl(LogLevels::Trace, mFuncName, "[ENTER]");
    }

    ~AutoLog()
    {
        if (std::uncaught_exceptions())
        {
            log_impl(LogLevels::Trace, mFuncName, "[EXIT_EXCEPTION]");
        }
        else
        {
            log_impl(LogLevels::Trace, mFuncName, "[EXIT]");
        }
    }

private:
    const char_type* mFuncName;
};
} // namespace Logging
