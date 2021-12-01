#pragma once

#include <cstring>

class PtrStream final
{
public:
    PtrStream(const u8** ppStream)
        : mppStream(ppStream)
    {
    }

    void Read(u32& v)
    {
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
    }

    u8 ReadU8()
    {
        return ReadT<u8>();
    }

    u16 ReadU16()
    {
        return ReadT<u16>();
    }

    u32 ReadU32()
    {
        return ReadT<u32>();
    }

    const u8** mppStream;

private:
    template <class T>
    T ReadT()
    {
        T v = 0;
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
        return v;
    }
};
