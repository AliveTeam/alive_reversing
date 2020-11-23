#pragma once

class PtrStream
{
public:
    PtrStream(const BYTE** ppStream)
        : mppStream(ppStream)
    {

    }

    void Read(DWORD& v)
    {
        memcpy(&v, *mppStream, sizeof(v));
        *mppStream += sizeof(v);
    }

    BYTE ReadU8()
    {
        return ReadT<BYTE>();
    }

    WORD ReadU16()
    {
        return ReadT<WORD>();
    }

    DWORD ReadU32()
    {
        return ReadT<DWORD>();
    }

    const BYTE** mppStream;

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
