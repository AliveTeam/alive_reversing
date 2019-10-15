#pragma once

#include <exception>
#include <iostream>
#include "easylogging++.h"

#if _MSC_VER
#define FNAME __FUNCTION__
#else
#define FNAME __PRETTY_FUNCTION__
#endif


#undef ERROR

#define LOGGING 1

#ifdef LOGGING
#define TRACE_ENTRYEXIT Logging::AutoLog __funcTrace(FNAME)
#define LOG_TRACE(msg) LOG(INFO) << FNAME << " [T] " << msg
#define LOG_INFO(msg) LOG(INFO) << FNAME << " [I] " << msg
#define LOG_WARNING(msg) LOG(WARNING) << FNAME << " [W] " << msg
#define LOG_ERROR(msg) LOG(ERROR) << FNAME << " [E] " << msg
#define LOG_(msg) LOG(INFO) << msg;
#else
#define TRACE_ENTRYEXIT
#define LOG_TRACE(msg)
#define LOG_INFO(msg)
#define LOG_WARNING(msg)
#define LOG_ERROR(msg)
#define LOG_(msg)
#endif

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

namespace Logging
{
    class AutoLog
    {
    public:
        AutoLog(const AutoLog&) = delete;
        AutoLog& operator = (const AutoLog&) = delete;
        AutoLog(const char* funcName)
          : mFuncName(funcName)
        {
            LOG_("[ENTER] " << mFuncName);
        }

        ~AutoLog()
        {
            if (std::uncaught_exception())
            {
                LOG_("[EXIT_EXCEPTION] " << mFuncName);
            }
            else
            {
                LOG_("[EXIT]  " << mFuncName);
            }
        }

    private:
        const char* mFuncName;
    };
}
