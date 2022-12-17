#include "stdafx.h"
#include "file_system.hpp"
#include "../FatalError.hpp"

#if !_WIN32
    #include <sys/stat.h>
#endif

#if _WIN32
#include <windows.h>
#undef CreateDirectory
constexpr inline char kDirSeparator = '\\';
#else
constexpr inline char kDirSeparator = '/';
#endif

FileSystem::Path& FileSystem::Path::Append(const std::string& directory)
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

FileSystem::Path FileSystem::Path::Parent() const
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

const std::string& FileSystem::Path::GetPath() const
{
    return mPath;
}

// ===========================================================

bool FileSystem::Save(const FileSystem::Path& path, const std::vector<u8>& data)
{
    this->CreateDirectory(path.Parent());
    return Save(path.GetPath().c_str(), data);
}

bool FileSystem::Save(const char_type* path, const std::vector<u8>& data)
{
    FILE* pFile = ::fopen(path, "wb");
    if (pFile)
    {
        ::fwrite(data.data(), 1, data.size(), pFile);
        ::fclose(pFile);
        return true;
    }
    return false;
}

std::string FileSystem::LoadToString(const FileSystem::Path& path)
{
    return LoadToString(path.GetPath().c_str());
}

std::string FileSystem::LoadToString(const char* path)
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

std::vector<u8> FileSystem::LoadToVec(const FileSystem::Path& path)
{
    std::vector<u8> buffer;
    LoadToVec(path.GetPath().c_str(), buffer);
    return buffer;
}

std::vector<u8> FileSystem::LoadToVec(const char* path)
{
    std::vector<u8> buffer;
    LoadToVec(path, buffer);
    return buffer;
}

bool FileSystem::LoadToVec(const Path& path, std::vector<u8>& buffer)
{
    return LoadToVec(path.GetPath().c_str(), buffer);
}

bool FileSystem::LoadToVec(const char* path, std::vector<u8>& buffer)
{
    buffer.clear();

    FILE* pFile = ::fopen(path, "rb");
    if (pFile)
    {
        ::fseek(pFile, 0, SEEK_END);
        const auto fsize = ftell(pFile);
        ::fseek(pFile, 0, SEEK_SET);
        buffer.resize(fsize);
        ::fread(buffer.data(), 1, fsize, pFile);
        ::fclose(pFile);
        return true;
    }
    return false;
}

void FileSystem::CreateDirectory(const FileSystem::Path& path)
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

bool FileSystem::FileExists(const char_type* fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f)
    {
        fclose(f);
        return true;
    }
    return false;
}

// ===========================================================

u32 AutoFILE::PeekU32()
{
    const auto oldPos = ::ftell(mFile);

    const u32 data = ReadU32();

    if (::fseek(mFile, oldPos, SEEK_SET) != 0)
    {
        ALIVE_FATAL("Seek back failed");
    }

    return data;
}

u32 AutoFILE::ReadU32() const
{
    u32 value = 0;
    if (::fread(&value, sizeof(u32), 1, mFile) != 1)
    {
        ALIVE_FATAL("Read U32 failed");
    }
    return value;
}

bool AutoFILE::Open(const char* pFileName, const char* pMode, bool autoFlushFile)
{
    Close();
    mFile = ::fopen(pFileName, pMode);
    if (strchr(pMode, 'w'))
    {
        mIsWriter = true;
    }
    mAutoFlushFile = autoFlushFile;
    return mFile != nullptr;
}

AutoFILE::~AutoFILE()
{
    Close();
}

FILE* AutoFILE::GetFile()
{
    return mFile;
}

bool AutoFILE::Write(const u8* pBytes, u32 numBytes)
{
    const bool ret = ::fwrite(pBytes, 1, numBytes, mFile) == 1;
    Flush();
    return ret;
}

long AutoFILE::FileSize()
{
    const long oldPos = ftell(mFile);
    fseek(mFile, 0, SEEK_END);
    const long fileSize = ftell(mFile);
    fseek(mFile, oldPos, SEEK_SET);
    return fileSize;
}

void AutoFILE::Close()
{
    if (mFile)
    {
        if (mIsWriter)
        {
            ::fflush(mFile);
        }
        ::fclose(mFile);
    }
}

void AutoFILE::Flush()
{
    if (mAutoFlushFile)
    {
        if (::fflush(mFile) != 0)
        {
            ALIVE_FATAL("fflush failed");
        }
    }
}
