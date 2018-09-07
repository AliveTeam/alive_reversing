#pragma once

template<class DataType, class EnumType>
class BitFieldT
{
public:
    void Toggle(EnumType value)
    {
        mData.all ^= value;
    }

    bool Get(EnumType value)
    {
        return !!(mData.all & value);
    }

    void Clear(EnumType value)
    {
        mData.all &= ~value;
    }

    void Set(EnumType value)
    {
        mData.all |= value;
    }

    DataType& Raw()
    {
        return mData;
    }

private:
    DataType mData = {};
};

struct BitFieldBytes16
{
    BYTE b0;
    BYTE b1;
};

union BitFieldUnion16
{
    WORD all;
    BitFieldBytes16 bytes;
};

template<class EnumType>
using BitField16 = BitFieldT<BitFieldUnion16, EnumType>;

struct BitFieldBytes32
{
    BYTE b0;
    BYTE b1;
    BYTE b2;
    BYTE b3;
};

struct BitFieldWords32
{
    WORD loword;
    WORD hiword;
};

union BitFieldUnion32
{
    DWORD all;
    BitFieldBytes32 bytes;
    BitFieldWords32 words;
};

template<class EnumType>
using BitField32 = BitFieldT<BitFieldUnion32, EnumType>;
