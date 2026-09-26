#include "stdafx.h"
#include "file_system.hpp"
#include "../FatalError.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>

#if !_WIN32
    #include <sys/stat.h>
    #include <dirent.h>
    #include <libgen.h>
    #include <string.h>
#endif

#if _WIN32
    #include <io.h>
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
    AutoFILE file = OpenFile(path, "wb");
    if (file.GetFile())
    {
        ::fwrite(data.data(), 1, data.size(), file.GetFile());
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
    AutoFILE file = OpenFile(path, "rb");
    if (file.GetFile())
    {
        FILE* pFile = file.GetFile();
        ::fseek(pFile, 0, SEEK_END);
        const auto fsize = ftell(pFile);
        ::fseek(pFile, 0, SEEK_SET);
        std::string r;
        r.resize(fsize);
        // Drop anything we couldn't actually read rather than returning zero padding
        r.resize(::fread(r.data(), 1, fsize, pFile));
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

    AutoFILE file = OpenFile(path, "rb");
    if (file.GetFile())
    {
        FILE* pFile = file.GetFile();
        ::fseek(pFile, 0, SEEK_END);
        const auto fsize = ftell(pFile);
        ::fseek(pFile, 0, SEEK_SET);
        buffer.resize(fsize);
        const size_t bytesRead = ::fread(buffer.data(), 1, fsize, pFile);
        buffer.resize(bytesRead);
        return bytesRead == static_cast<size_t>(fsize);
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
    return OpenFile(fileName, "r").GetFile() != nullptr;
}

// ===========================================================

AutoFILE FileSystem::OpenFile(const char_type* path, const char_type* mode, bool autoFlushFile)
{
    // Strip a leading "./" or ".\".
    if (strlen(path) >= 3 && path[0] == '.' && (path[1] == '/' || path[1] == '\\'))
    {
        path += 2;
    }

    FILE* file = ::fopen(path, mode);

#if !_WIN32
    if (!file)
    {
        // Game data is Windows-authored (mixed case); an exact-case fopen can fail on a
        // case-sensitive filesystem even though the file is there under different casing.
        // Fall back to a case-insensitive scan of the containing directory.
        char* dirNameBuf = ::strdup(path);
        char* baseNameBuf = ::strdup(path);
        const std::string dir = ::dirname(dirNameBuf);
        const std::string base = ::basename(baseNameBuf);
        ::free(dirNameBuf);
        ::free(baseNameBuf);

        std::string lowerBase = base;
        std::transform(lowerBase.begin(), lowerBase.end(), lowerBase.begin(), [](unsigned char c) { return static_cast<char>(::tolower(c)); });

        DIR* dirHandle = ::opendir(dir.c_str());
        if (dirHandle)
        {
            struct dirent* entry = nullptr;
            while ((entry = ::readdir(dirHandle)) != nullptr)
            {
                std::string lowerEntryName = entry->d_name;
                std::transform(lowerEntryName.begin(), lowerEntryName.end(), lowerEntryName.begin(), [](unsigned char c) { return static_cast<char>(::tolower(c)); });

                if (lowerEntryName == lowerBase)
                {
                    file = ::fopen((dir + "/" + entry->d_name).c_str(), mode);
                    if (file)
                    {
                        break;
                    }
                }
            }
            ::closedir(dirHandle);
        }
    }
#endif

    const bool isWriter = mode && ::strchr(mode, 'w') != nullptr;
    return AutoFILE(file, isWriter, autoFlushFile);
}

bool FileSystem::DirectoryExists(const char_type* pDirName)
{
#if _WIN32
    WIN32_FIND_DATA sFindData = {};
    HANDLE hFind = FindFirstFile(pDirName, &sFindData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    FindClose(hFind);
    return true;
#else
    DIR* dir = opendir(pDirName);
    if (dir)
    {
        closedir(dir);
        return true;
    }
    return false;
#endif
}

#if !_WIN32
namespace
{
    // Full match of text against a file name pattern where '*' matches any run of chars and '?' any single char
    bool WildCardMatcher(const std::string& text, const std::string& wildcardPattern, bool caseSensitive)
    {
        const auto charsEqual = [caseSensitive](char a, char b)
        {
            return caseSensitive ? a == b : std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
        };

        size_t t = 0;
        size_t p = 0;
        size_t starPos = std::string::npos; // position of the last '*' seen in the pattern
        size_t starMatchEnd = 0;            // text position that '*' currently extends to
        while (t < text.size())
        {
            if (p < wildcardPattern.size() && (wildcardPattern[p] == '?' || (wildcardPattern[p] != '*' && charsEqual(wildcardPattern[p], text[t]))))
            {
                t++;
                p++;
            }
            else if (p < wildcardPattern.size() && wildcardPattern[p] == '*')
            {
                starPos = p++;
                starMatchEnd = t;
            }
            else if (starPos != std::string::npos)
            {
                // Backtrack: let the last '*' swallow one more char
                p = starPos + 1;
                t = ++starMatchEnd;
            }
            else
            {
                return false;
            }
        }

        while (p < wildcardPattern.size() && wildcardPattern[p] == '*')
        {
            p++;
        }
        return p == wildcardPattern.size();
    }
} // namespace
#endif

namespace
{
#if _WIN32
    // Shared _findfirst/_findnext walk for both EnumerateDirectory (searchPattern is a wildcard
    // file pattern, e.g. "*.json", resolved relative to the CWD) and EnumerateSubDirectories
    // (searchPattern is "<dirPath>\*") - they differ only in that pattern and in what
    // shouldReport considers a match.
    void WalkFindFirst(const char_type* searchPattern, const std::function<void(const char_type*, u32)>& cb, const std::function<bool(const _finddata_t&)>& shouldReport)
    {
        _finddata_t findRec = {};
        intptr_t hFind = _findfirst(searchPattern, &findRec);
        if (hFind != -1)
        {
            for (;;)
            {
                if (shouldReport(findRec))
                {
                    cb(findRec.name, static_cast<u32>(findRec.time_write)); // TODO: Chopping off a lot of time stamp resolution here
                }

                if (_findnext(hFind, &findRec) == -1)
                {
                    break;
                }
            }
            _findclose(hFind);
        }
    }
#else
    // Shared opendir/readdir walk for both EnumerateDirectory (searchDir is always "." - fileName
    // is only ever a wildcard filter, not a real path, on this platform - see its own comment)
    // and EnumerateSubDirectories (searchDir is the real directory to list) - they differ only in
    // which directory that is and in what shouldReport considers a match.
    void WalkReadDir(const char_type* searchDir, const std::function<void(const char_type*, u32)>& cb, const std::function<bool(const std::string&, const struct stat&)>& shouldReport)
    {
        DIR* dir(opendir(searchDir));
        if (dir)
        {
            dirent* ent = nullptr;
            do
            {
                ent = readdir(dir);
                if (ent)
                {
                    const std::string itemName = ent->d_name;
                    if (itemName != "." && itemName != "..")
                    {
                        struct stat statbuf;
                        const std::string fullPath = std::string(searchDir) + "/" + itemName;
                        if (stat(fullPath.c_str(), &statbuf) == 0 && shouldReport(itemName, statbuf))
                        {
                            cb(itemName.c_str(), static_cast<u32>(statbuf.st_mtime));
                        }
                    }
                }
            }
            while (ent);
            closedir(dir);
        }
    }
#endif
}

void FileSystem::EnumerateDirectory(const char_type* fileName, FileSystem::TEnumCallBack cb)
{
#if _WIN32
    WalkFindFirst(fileName, cb, [](const _finddata_t& rec)
                  { return !(rec.attrib & FILE_ATTRIBUTE_DIRECTORY); });
#else
    // fileName is a wildcard filter (e.g. "*.json"), not a real path - always searches "."
    // (WildCardMatcher is POSIX-only, see its own #if !_WIN32 guard above, since _findfirst
    // already does wildcard matching natively on Windows).
    WalkReadDir(".", cb, [&fileName](const std::string& itemName, const struct stat& statbuf)
                { return !S_ISDIR(statbuf.st_mode) && WildCardMatcher(itemName, fileName, true); });
#endif
}

void FileSystem::EnumerateSubDirectories(const char_type* dirPath, const std::function<void(const char_type*, u32)>& cb)
{
#if _WIN32
    const std::string pattern = std::string(dirPath) + "\\*";
    WalkFindFirst(pattern.c_str(), cb, [](const _finddata_t& rec)
                  { return (rec.attrib & FILE_ATTRIBUTE_DIRECTORY) && std::strcmp(rec.name, ".") != 0 && std::strcmp(rec.name, "..") != 0; });
#else
    WalkReadDir(dirPath, cb, [](const std::string&, const struct stat& statbuf)
                { return static_cast<bool>(S_ISDIR(statbuf.st_mode)); });
#endif
}
