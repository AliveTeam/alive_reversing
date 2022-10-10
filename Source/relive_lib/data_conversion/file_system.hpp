#pragma once

#include "../../AliveLibCommon/FatalError.hpp"
#include "../../AliveLibAE/stdafx.h"

#if !_WIN32
#include <sys/stat.h>
#endif

#if _WIN32
constexpr inline char kDirSeparator = '\\';
#else
constexpr inline char kDirSeparator = '/';
#endif

// TODO: Lots of stuff missing, needs to do utf8 -> utf16 on windows
class FileSystem final
{
public:
    class Path final
    {
    public:
        Path& Append(const std::string& directory)
        {
            if (!directory.empty() && directory[directory.size() - 1] == kDirSeparator)
            {
                ALIVE_FATAL("Directory ends with slash");
            }
            if (!mPath.empty())
            {
                mPath += kDirSeparator;
            }
            mPath += directory;
            return *this;
        }

        Path Parent() const
        {
            const auto pos = mPath.find_last_of(kDirSeparator);
            if (pos != std::string::npos)
            {
                Path parent;
                parent.mPath = mPath.substr(0, pos);
                return parent;
            }
            return {};
        }

        const std::string& GetPath() const
        {
            return mPath;
        }

    private:
        std::string mPath;
    };

    void Save(const Path& path, const std::vector<u8>& data)
    {
        CreateDirectory(path.Parent());
        FILE* pFile = ::fopen(path.GetPath().c_str(), "wb");
        if (pFile)
        {
            ::fwrite(data.data(), 1, data.size(), pFile);
            ::fclose(pFile);
        }
    }

    std::string LoadToString(const Path& path)
    {
        return LoadToString(path.GetPath().c_str());
    }

    std::string LoadToString(const char* path)
    {
        FILE* pFile = ::fopen(path, "rb");
        if (pFile)
        {
            ::fseek(pFile, 0, SEEK_END);
            const auto fsize = ftell(pFile);
            ::fseek(pFile, 0, SEEK_SET);
            std::string r;
            r.resize(fsize);
            ::fread(r.data(), 1, fsize, pFile);
            ::fclose(pFile);
            return r;
        }
        return {};
    }

    std::vector<u8> LoadToVec(const Path& path)
    {
        return LoadToVec(path.GetPath().c_str());
    }

    std::vector<u8> LoadToVec(const char* path)
    {
        FILE* pFile = ::fopen(path, "rb");
        if (pFile)
        {
            ::fseek(pFile, 0, SEEK_END);
            const auto fsize = ftell(pFile);
            ::fseek(pFile, 0, SEEK_SET);
            std::vector<u8> r;
            r.resize(fsize);
            ::fread(r.data(), 1, fsize, pFile);
            ::fclose(pFile);
            return r;
        }
        return {};
    }

    void CreateDirectory(const Path& path)
    {
        std::string dirPart;

        std::string fullPath = path.GetPath();

        auto dirPos = fullPath.find_first_of(kDirSeparator);
        do
        {
            if (dirPos != std::string::npos)
            {
                dirPart = dirPart + fullPath.substr(0, dirPos);
                #ifdef _WIN32
                ::CreateDirectoryA(dirPart.c_str(), nullptr);
                #else
                mkdir(dirPart.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                #endif
                dirPart.append(1, kDirSeparator);
                fullPath = fullPath.substr(dirPos + 1);
                dirPos = fullPath.find_first_of(kDirSeparator);
            }
        }
        while (dirPos != std::string::npos);
        #ifdef _WIN32
        ::CreateDirectoryA(path.GetPath().c_str(), nullptr);
        #else
        mkdir(path.GetPath().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        #endif
    }

};
