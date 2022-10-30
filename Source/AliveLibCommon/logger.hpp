#pragma once

#include <exception>
#include <iostream>
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
    printf("[%s] ", funcName);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

#ifdef LOGGING
    #define TRACE_ENTRYEXIT Logging::AutoLog __funcTrace(FNAME)
    #define LOG_TRACE(fmt, ...) log_impl(LogLevels::Trace, FNAME, fmt, __VA_ARGS__)
    #define LOG_INFO(fmt, ...) log_impl(LogLevels::Info, FNAME, fmt, __VA_ARGS__)
    #define LOG_WARNING(fmt, ...) log_impl(LogLevels::Warning, FNAME, fmt, __VA_ARGS__)
    #define LOG_ERROR(fmt, ...) log_impl(LogLevels::Error, FNAME, fmt, __VA_ARGS__)
    #define LOG(fmt, ...) log_impl(LogLevels::Trace, FNAME, fmt, __VA_ARGS__)
#else
    #define TRACE_ENTRYEXIT
    #define LOG_TRACE(fmt, ...)
    #define LOG_INFO(fmt, ...)
    #define LOG_WARNING(fmt, ...)
    #define LOG_ERROR(fmt, ...)
    #define LOG(fmt, ...)
#endif

[[noreturn]] inline void HOOK_FATAL(const char_type* errMsg)
{
    LOG_ERROR(errMsg);
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
