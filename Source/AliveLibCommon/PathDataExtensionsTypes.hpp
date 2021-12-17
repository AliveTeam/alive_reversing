#pragma once

#include "Types.hpp"

struct StringTable final
{
    u32 mStringCount;
    char_type* mStrings[1]; // var length

    static u8* MakeTable(StringTable* pTable)
    {
        // Read the string count
        u8* pDataIter = reinterpret_cast<u8*>(pTable);
        const u32 stringCount = *reinterpret_cast<u32*>(pDataIter);
        pDataIter += sizeof(u32);

        if (stringCount > 0)
        {
            // After stringCount lengths for each string we have the string data
            u8* ptrToLens = pDataIter;
            u8* ptrToStrings = pDataIter + (sizeof(u64) * stringCount);

            for (u32 i = 0; i < stringCount; i++)
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
};

enum PerPathExtensionResTypes : u32
{
    Resource_Pxtd = 0x50787464
};
