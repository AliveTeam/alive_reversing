#pragma once

template <class DataType, class EnumType>
class BitFieldT
{
public:
    void Toggle(EnumType value)
    {
        mData.all ^= value;
    }

    bool Get(EnumType value) const
    {
        return !!(mData.all & value);
    }

    void Clear()
    {
        mData.all = 0;
    }

    void Clear(EnumType value)
    {
        mData.all &= ~value;
    }

    void Set(EnumType value)
    {
        mData.all |= value;
    }

    void Set(EnumType value, bool set)
    {
        if (set)
        {
            Set(value);
        }
        else
        {
            Clear(value);
        }
    }

    void Set(EnumType value, s32 set)
    {
        Set(value, !!set);
    }

    DataType& Raw()
    {
        return mData;
    }

    const DataType& Raw() const
    {
        return mData;
    }

private:
    DataType mData = {};
};

union BitFieldUnion8
{
    u8 all;
};
template <class EnumType>
using BitField8 = BitFieldT<BitFieldUnion8, EnumType>;

struct BitFieldBytes16
{
    u8 b0;
    u8 b1;
};

union BitFieldUnion16
{
    u16 all;
    BitFieldBytes16 bytes;
};

template <class EnumType>
using BitField16 = BitFieldT<BitFieldUnion16, EnumType>;

struct BitFieldBytes32
{
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
};

struct BitFieldWords32
{
    u16 loword;
    u16 hiword;
};

union BitFieldUnion32
{
    u32 all;
    BitFieldBytes32 bytes;
    BitFieldWords32 words;
};

template <class EnumType>
using BitField32 = BitFieldT<BitFieldUnion32, EnumType>;
