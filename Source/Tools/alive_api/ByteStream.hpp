#pragma once

#include <vector>

class ByteStream
{
public:
    ByteStream() = default;

    explicit ByteStream(const std::vector<BYTE>& data) 
        : mData(data)
    {

    }

    // Read any fundamental type
    template<class T>
    void Read(T& type)
    {
        static_assert(std::is_fundamental<T>::value, "Can only read fundamental types");
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
        static_assert(std::is_fundamental<T>::value, "Can only read vectors of fundamental types");
        ReadBytes(reinterpret_cast<u8*>(type.data()), sizeof(T) * type.size());
    }

    // Read any std::array of fundamental type
    template<class T, std::size_t count>
    void Read(std::array<T, count>& type)
    {
        static_assert(std::is_fundamental<T>::value, "Can only read vectors of fundamental types");
        ReadBytes(reinterpret_cast<u8*>(type.data()), sizeof(T) * type.size());
    }

    // Read any fixed array of fundamental type
    template<typename T, std::size_t count>
    void Read(T(&value)[count])
    {
        static_assert(std::is_fundamental<T>::value, "Can only read fundamental types");
        ReadBytes(reinterpret_cast<u8*>(&value[0]), sizeof(T) * count);
    }


    // Write any fundamental type
    template<class T>
    void Write(const T& type)
    {
        static_assert(std::is_fundamental<T>::value, "Can only write fundamental types");
        WriteBytes(reinterpret_cast<const u8*>(&type), sizeof(type));
    }

    // Write vector of any fundamental type
    template<class T>
    void Write(std::vector<T>& type)
    {
        static_assert(std::is_fundamental<T>::value, "Can only write vectors of fundamental types");
        WriteBytes(reinterpret_cast<u8*>(type.data()), sizeof(T) * type.size());
    }

    // Write a string
    void Write(const std::string& type)
    {
        WriteBytes(reinterpret_cast<const u8*>(&type[0]), type.size());
    }

    [[nodiscard]] const std::vector<BYTE>& GetBuffer() const
    {
        return mData;
    }

    void ReserveSize(std::size_t len)
    {
        mData.reserve(len);
    }

private:
    void ReadBytes(BYTE* buffer, std::size_t len)
    {
        if (mReadPos + len > mData.size())
        {
            abort();
        }
        memcpy(buffer, mData.data(), len);
        mReadPos += len;
    }

    void WriteBytes(const BYTE* buffer, std::size_t len)
    {
        if (mWritePos + len > mData.size())
        {
            mWritePos.resize(mWritePos + len);
        }
        memcpy(&mData[mWritePos], mData.data(), len);
        mWritePos += len;
    }

    std::size_t mReadPos = 0;
    std::size_t mWritePos = 0;
    std::vector<BYTE> mData;
};
