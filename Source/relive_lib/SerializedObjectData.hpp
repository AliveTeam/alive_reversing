#pragma once

#include "Types.hpp"
#include "FatalError.hpp"

#include <vector>
#include <cstring>
#include <type_traits>

// A growable byte buffer with sequential read/write cursors. Used to
// serialize per-object save state - engine agnostic, both AO and AE quicksave
// formats use it the same way.
class SerializedObjectData final
{
public:
    SerializedObjectData()
    {
        // OG allowed for 6820 bytes of data, we allow for as
        // big as whatever we can allocate
        mBuffer.reserve(1024 * 8);
    }

    template<typename T>
    void Write(const T& item)
    {
        static_assert(std::is_trivially_copyable_v<T>, "item must be trivially_copyable");
        const auto posToWrite = mBuffer.size();

        // Allocate space for item
        mBuffer.resize(mBuffer.size() + sizeof(T));

        // Bitwise copy it into the buffer (safe cos its POD)
        ::memcpy(mBuffer.data() + posToWrite, &item, sizeof(T));
    }

    template<typename T>
    const T* ReadTmpPtr() const
    {
        const T* tmp = PeekPtr<T>();
        mBufferReadPos += sizeof(T);
        return tmp;
    }

    template <typename T>
    const T* PeekTmpPtr() const
    {
        return PeekPtr<T>();
    }

    void WriteU8(u8 v)
    {
        WriteBasicType(v);
    }

    void WriteU32(u32 v)
    {
        WriteBasicType(v);
    }

    [[nodiscard]] u32 PeekU32() const
    {
        return *PeekPtr<u32>();
    }

    [[nodiscard]] u32 ReadU32() const
    {
        return ReadBasicType<u32>();
    }

    [[nodiscard]] u8 ReadU8() const
    {
        return ReadBasicType<u8>();
    }

    void ReadRewind() const
    {
        mBufferReadPos = 0;
    }

    // Advances the read cursor without interpreting the skipped bytes -
    // used to step over a record whose concrete type isn't handled by a
    // reader, via its SaveStateBase::mSize.
    void SkipRead(u32 numBytes) const
    {
        ReadCheck(numBytes);
        mBufferReadPos += numBytes;
    }

    bool CanRead() const
    {
        return mBufferReadPos < mBuffer.size();
    }

    void WriteRewind()
    {
        mBufferReadPos = 0;
        mBuffer.clear();
    }

private:
    void ReadCheck(u32 readSize) const
    {
        if (mBufferReadPos + readSize > mBuffer.size())
        {
            ALIVE_FATAL("Attempted to read %u bytes from offset %u but total length is %zu", readSize, mBufferReadPos, mBuffer.size());
        }
    }

    template<typename T>
    void WriteBasicType(T value)
    {
        const auto writePos = mBuffer.size();
        mBuffer.resize(mBuffer.size() + sizeof(T));
        *reinterpret_cast<T*>(mBuffer.data() + writePos) = value;
    }

    template <typename T>
    [[nodiscard]] const T* PeekPtr() const
    {
        ReadCheck(sizeof(T));
        return reinterpret_cast<const T*>(mBuffer.data() + mBufferReadPos);
    }

    template <typename T>
    [[nodiscard]] T ReadBasicType() const
    {
        const T* v = PeekPtr<T>();
        mBufferReadPos += sizeof(T);
        return *v;
    }

    mutable u32 mBufferReadPos = 0;
    std::vector<u8> mBuffer;
};
