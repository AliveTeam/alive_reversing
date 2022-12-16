#pragma once

#include "Types.hpp"

struct StringTable final
{
    u64 mStringCount; // u64 so that we take up the same space/padding on both 32/64bit
 
    union UniveralStringPtr
    {
        u64 rawData;
        char_type* string_ptr; // 4 bytes on 32bit, 8 bytes on 64bit, union'd so we access in 8 byte multiples even in 32bit
    };

    UniveralStringPtr mStrings[1]; // var length

    static u8* MakeTable(StringTable* pTable)
    {
        // Read the string count
        u8* pDataIter = reinterpret_cast<u8*>(pTable);
        const u64 stringCount = *reinterpret_cast<const u64*>(pDataIter);
        pDataIter += sizeof(u64);

        if (stringCount > 0)
        {
            // After stringCount lengths for each string we have the string data
            u8* ptrToLens = pDataIter;
            u8* ptrToStrings = pDataIter + (sizeof(u64) * stringCount);

            for (u64 i = 0; i < stringCount; i++)
            {
                // Pointer to lenth of string
                u64* stringLen = reinterpret_cast<u64*>(ptrToLens);

                // Keep pointer to current string
                u8* stringPtr = ptrToStrings;

                // Point to the next string
                ptrToStrings += *stringLen;

                // Overwrite the length with a pointer to the string
                *stringLen = 0; // Ensure all bits cleared for 32bit ptrs
                *stringLen = reinterpret_cast<u64>(stringPtr);

                ptrToLens += sizeof(u64);
            }
            return ptrToStrings;
        }

        return pDataIter;
    }
};

struct PerPathExtension final
{
    u32 mSize;
    char_type mBlyName[50];
    u32 mPathId;
    u32 mXSize;
    u32 mYSize;
    u32 mNumCollisionLines;
    u32 mObjectOffset;
    u32 mIndexTableOffset;
    u32 mCollisionOffset;
    s32 mGridWidth;
    s32 mGridHeight;

    s32 mAbeStartXPos;
    s32 mAbeStartYPos;

    s32 mNumMudsInPath;

    s32 mTotalMuds;
    s32 mBadEndingMuds;
    s32 mGoodEndingMuds;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(PerPathExtension, 116);

enum PerPathExtensionResTypes : u32
{
    Resource_Pxtd = 0x50787464
};
