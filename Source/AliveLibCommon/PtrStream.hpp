#pragma once

class PtrStream
{
public:
    PtrStream(const u8** ppStream)
        : mppStream(ppStream)
    {

    }

    void Read(DWORD& v)
    {
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
    }

    u8 ReadU8()
    {
        return ReadT<u8>();
    }

    WORD ReadU16()
    {
        return ReadT<WORD>();
    }

    DWORD ReadU32()
    {
        return ReadT<DWORD>();
    }

    const u8** mppStream;

private:
    template<class T>
    T ReadT()
    {
        T v = 0;
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
        return v;
    }
};
