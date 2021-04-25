#pragma once

#include <cstddef>
#include <string>
#include <type_traits>
#include <vector>

class ByteStream
{
public:
    ByteStream() = default;

    explicit ByteStream(const std::vector<u8>& data)
        : mData(data)
    {
    }

    // Read any fundamental type
    template<class T>
    void Read(T& type)
    {
        static_assert(std::is_fundamental_v<T>, "Can only read fundamental types");
        ReadBytes(reinterpret_cast<u8*>(&type), sizeof(type));
    }

    // Read a string
    void Read(std::string& type)
    {
        ReadBytes(reinterpret_cast<u8*>(&type[0]), type.size());
    }

    // Read any vector of fundamental type
    template<class T>
    void Read(std::vector<T>& type)
    {
        static_assert(std::is_fundamental_v<T>, "Can only read vectors of fundamental types");
        ReadBytes(reinterpret_cast<u8*>(type.data()), sizeof(T) * type.size());
    }

    // Read any std::array of fundamental type
    template<class T, std::size_t count>
    void Read(std::array<T, count>& type)
    {
        static_assert(std::is_fundamental_v<T>, "Can only read vectors of fundamental types");
        ReadBytes(reinterpret_cast<u8*>(type.data()), sizeof(T) * type.size());
    }

    // Read any fixed array of fundamental type
    template<typename T, std::size_t count>
    void Read(T(&value)[count])
    {
        static_assert(std::is_fundamental_v<T>, "Can only read fundamental types");
        ReadBytes(reinterpret_cast<u8*>(&value[0]), sizeof(T) * count);
    }

    // Write any fixed array of fundamental type
    template<typename T, std::size_t count>
    void Write(const T(&value)[count])
    {
        static_assert(std::is_fundamental_v<T>, "Can only write fundamental types");
        WriteBytes(reinterpret_cast<const u8*>(&value[0]), sizeof(T) * count);
    }

    // Write any fundamental type
    template<class T>
    void Write(const T& type)
    {
        static_assert(std::is_fundamental_v<T>, "Can only write fundamental types");
        WriteBytes(reinterpret_cast<const u8*>(&type), sizeof(type));
    }

    // Write vector of any fundamental type
    template<class T>
    void Write(const std::vector<T>& type)
    {
        static_assert(std::is_fundamental_v<T>, "Can only write vectors of fundamental types");
        WriteBytes(reinterpret_cast<const u8*>(type.data()), sizeof(T) * type.size());
    }

    // Write a string
    void Write(const std::string& type)
    {
        WriteBytes(reinterpret_cast<const u8*>(&type[0]), type.size());
    }

    [[nodiscard]] const std::vector<u8>& GetBuffer() const
    {
        return mData;
    }

    void ReserveSize(std::size_t len)
    {
        mData.reserve(len);
    }

    [[nodiscard]] bool AtReadEnd() const
    {
        return mReadPos == mData.size();
    }

    [[nodiscard]] std::size_t WritePos() const
    {
        return mWritePos;
    }

    void SeekWrite(std::size_t pos)
    {
        mWritePos = pos;
        if (mWritePos > mData.size())
        {
            mData.resize(mWritePos);
        }
    }

private:
    void ReadBytes(u8* buffer, std::size_t len)
    {
        if (mReadPos + len > mData.size())
        {
            throw ReliveAPI::IOReadPastEOFException();
        }
        memcpy(buffer, &mData[mReadPos], len);
        mReadPos += len;
    }

    void WriteBytes(const u8* buffer, std::size_t len)
    {
        if (len > 0)
        {
            if (mWritePos + len > mData.size())
            {
                mData.resize(mWritePos + len);
            }
            memcpy(&mData[mWritePos], buffer, len);
            mWritePos += len;
        }
    }

    std::size_t mReadPos = 0;
    std::size_t mWritePos = 0;
    std::vector<u8> mData;
};
