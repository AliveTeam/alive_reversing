#pragma once

#include "../../AliveLibAE/stdafx.h"

// TODO: Lots of stuff missing, needs to do utf8 -> utf16 on windows
class FileSystem final
{
public:


    class Path final
    {
    public:
        Path& Append(const std::string& directory);

        Path Parent() const;

        const std::string& GetPath() const;

    private:
        std::string mPath;
    };

    bool Save(const Path& path, const std::vector<u8>& data);
    bool Save(const char_type* path, const std::vector<u8>& data);

    std::string LoadToString(const Path& path);

    std::string LoadToString(const char* path);

    std::vector<u8> LoadToVec(const Path& path);
    std::vector<u8> LoadToVec(const char* path);

    bool LoadToVec(const Path& path, std::vector<u8>& buffer);
    bool LoadToVec(const char* path, std::vector<u8>& buffer);

    void CreateDirectory(const Path& path);

    bool FileExists(const char_type* fileName);
};


class [[nodiscard]] AutoFILE final
{
public:
    enum class SeekMode
    {
        Current,
    };

    AutoFILE(const AutoFILE&) = delete;
    AutoFILE& operator=(const AutoFILE&) const = delete;
    AutoFILE() = default;

    bool Open(const char* pFileName, const char* pMode, bool autoFlushFile);

    ~AutoFILE();

    FILE* GetFile();

    bool Write(const u8* pBytes, u32 numBytes);

    template <typename TypeToWrite>
    bool Write(const TypeToWrite& value)
    {
        static_assert(std::is_pod<TypeToWrite>::value, "TypeToWrite must be pod");
        const bool ret = ::fwrite(&value, sizeof(TypeToWrite), 1, mFile) == 1;
        Flush();
        return ret;
    }

    template <typename TypeToWrite>
    bool Write(const std::vector<TypeToWrite>& value)
    {
        static_assert(std::is_pod<TypeToWrite>::value, "TypeToWrite must be pod");
        const bool ret = ::fwrite(value.data(), sizeof(TypeToWrite), value.size(), mFile) == value.size();
        Flush();
        return ret;
    }

    template <typename TypeToRead>
    bool Read(TypeToRead& value)
    {
        static_assert(std::is_pod<TypeToRead>::value, "TypeToRead must be pod");
        return ::fread(&value, sizeof(TypeToRead), 1, mFile) == 1;
    }

    template <typename TypeToRead>
    bool Read(std::vector<TypeToRead>& value)
    {
        static_assert(std::is_pod<TypeToRead>::value, "TypeToRead must be pod");
        return ::fread(value.data(), sizeof(TypeToRead), value.size(), mFile) == value.size();
    }

    u32 PeekU32();

    u32 ReadU32() const;

    u32 Pos();

    void Seek(u32 pos, SeekMode mode);

    long FileSize();

    void Close();

private:
    void Flush();

    FILE* mFile = nullptr;
    bool mIsWriter = false;
    bool mAutoFlushFile = false;
};
