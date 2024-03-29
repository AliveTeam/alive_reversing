#pragma once

#include "RoundUp.hpp"
#include <type_traits>
#include <memory>

namespace ReliveAPI {

class IFile
{
public:
    virtual ~IFile() { }

    virtual bool IsOpen() const = 0;

    virtual bool Seek(std::size_t absPos) = 0;

    virtual bool ReadInto(std::string& str) = 0;

    virtual bool Write(const u8* buffer, std::size_t len) = 0;

    bool Write(const std::string& s)
    {
        return Write(reinterpret_cast<const u8*>(s.data()), s.length());
    }

    template <typename T>
    bool Write(const T& type)
    {
        static_assert(std::is_pod<T>::value, "T must be a POD type.");
        return Write(reinterpret_cast<const u8*>(&type), sizeof(T));
    }

    virtual bool Read(u8* buffer, std::size_t len) = 0;

    bool Read(std::vector<u8>& buffer)
    {
        return Read(buffer.data(), buffer.size());
    }

    template <typename T>
    bool Read(std::vector<T>& buffer)
    {
        static_assert(std::is_pod<T>::value, "T must be a POD type.");
        return Read(reinterpret_cast<u8*>(buffer.data()), buffer.size() * sizeof(T));
    }

    template<typename T>
    bool Read(T& type)
    {
        static_assert(std::is_pod<T>::value, "T must be a POD type.");
        return Read(reinterpret_cast<u8*>(&type), sizeof(T));
    }

    virtual bool PadEOF(u32 multiple) = 0;
};

class IFileIO
{
public:
    virtual ~IFileIO()
    {
    }

    enum class Mode
    {
        WriteBinary,
        ReadBinary,
        Write,
        Read,
    };

    virtual std::unique_ptr<IFile> Open(const std::string& fileName, Mode mode) = 0;
};


class File : public IFile
{
public:
    File(const std::string& fileName, IFileIO::Mode mode)
    {
        switch (mode)
        {
            case IFileIO::Mode::Write:
                mFileHandle = ::fopen(fileName.c_str(), "w");
                break;

            case IFileIO::Mode::Read:
                mFileHandle = ::fopen(fileName.c_str(), "r");
                break;

            case IFileIO::Mode::ReadBinary:
                mFileHandle = ::fopen(fileName.c_str(), "rb");
                break;

            case IFileIO::Mode::WriteBinary:
                mFileHandle = ::fopen(fileName.c_str(), "wb");
                break;
        }
    }

    ~File() override
    {
        if (IsOpen())
        {
            ::fclose(mFileHandle);
        }
    }

    bool IsOpen() const override
    {
        return mFileHandle != nullptr;
    }

    bool Seek(std::size_t absPos) override
    {
        return ::fseek(mFileHandle, static_cast<long>(absPos), SEEK_SET) == 0;
    }

    bool Read(u8* buffer, std::size_t len) override
    {
        return ::fread(buffer, 1, len, mFileHandle) == len;
    }

    bool Write(const u8* buffer, std::size_t len) override
    {
        return ::fwrite(buffer, 1, len, mFileHandle) == len;
    }

    bool ReadInto(std::string& str) override
    {
        const auto oldPos = ::ftell(mFileHandle);
        if (::fseek(mFileHandle, 0, SEEK_END) != 0)
        {
            return false;
        }

        const auto fileSize = ftell(mFileHandle);
        if (fileSize == -1)
        {
            return false;
        }

        str.resize(fileSize);
        if (!Seek(oldPos))
        {
            return false;
        }

        return Read(reinterpret_cast<u8*>(str.data()), str.size());
    }

    bool PadEOF(u32 multiple) override
    {
        const auto pos = ::ftell(mFileHandle);
        if (pos == -1)
        {
            return false;
        }

        if (pos + 1 != RoundUp(static_cast<int>(pos), static_cast<int>(multiple)))
        {
            ::fseek(mFileHandle, RoundUp(pos) - 1, SEEK_SET);
            u8 emptyByte = 0;
            ::fwrite(&emptyByte, sizeof(u8), 1, mFileHandle);
        }

        return true;
    }

private:
    FILE* mFileHandle = nullptr;
};

class FileIO final : public IFileIO
{
public:
    std::unique_ptr<IFile> Open(const std::string& fileName, IFileIO::Mode mode) override
    {
        auto ret = std::make_unique<File>(fileName, mode);
        if (!ret->IsOpen())
        {
            return nullptr;
        }
        return ret;
    }
};

} // namespace ReliveAPI
