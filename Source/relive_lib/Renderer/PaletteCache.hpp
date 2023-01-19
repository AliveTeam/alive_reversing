#pragma once

#include "../../relive_lib/Types.hpp"
#include "../../relive_lib/data_conversion/AnimationConverter.hpp"
#include "../../relive_lib/FatalError.hpp"
#include <map>

class PaletteCache final
{
public:
    explicit PaletteCache(u32 maxCacheSize = 256)
        : mMaxCacheSize(maxCacheSize)
    {

    }

    struct AddResult final
    {
        u32 mIndex = 0;
        bool mAllocated = false;
    };

    AddResult Add(AnimationPal& pCache)
    {
        AddResult ret;

        // Check we don't already have this palette
        const u32 paletteHash = HashPalette(&pCache);
        auto searchResult = mPaletteCache.find(paletteHash);

        if (searchResult != std::end(mPaletteCache))
        {
            searchResult->second.mInUse = true;
            ret.mIndex = searchResult->second.mIndex; // Palette index
            return ret;
        }

        // Get an index for the new palette
        u32 nextIndex = static_cast<u32>(mPaletteCache.size());

        if (nextIndex >= mMaxCacheSize)
        {
            // Look for a unused slot
            u32 unusedPaletteHash = 0;

            auto iter = mPaletteCache.begin();
            while (iter != mPaletteCache.end())
            {
                if (!iter->second.mInUse)
                {
                    // Found an unused one
                    unusedPaletteHash = iter->first;
                    break;
                }
                iter++;
            }

            if (iter == std::end(mPaletteCache))
            {
                ALIVE_FATAL("Ran out of palettes!");
            }

            // Acquire the index we're taking over
            nextIndex = iter->second.mIndex;

            // Bin the old one
            mPaletteCache.erase(unusedPaletteHash);
        }

        mPaletteCache[paletteHash] = PalCacheEntry{nextIndex, true};

        ret.mAllocated = true;
        ret.mIndex = nextIndex;

        return ret;
    }

    void ResetUseFlags()
    {
        for (auto iter = mPaletteCache.begin(); iter != mPaletteCache.end(); iter++)
        {
            // Default all palettes to unused for next draw
            iter->second.mInUse = false;
        }
    }

    void Clear()
    {
        mPaletteCache.clear();
    }

private:
    u32 HashPalette(const AnimationPal* pPal)
    {
        // This is the 'djb2' algorithm
        u32 hash = 5381;

        for (int i = 0; i < 256; i++)
        {
            hash = ((hash << 5) + hash) + pPal->mPal[i].r;
            hash = ((hash << 5) + hash) + pPal->mPal[i].g;
            hash = ((hash << 5) + hash) + pPal->mPal[i].b;
            hash = ((hash << 5) + hash) + pPal->mPal[i].a;
        }

        return hash;
    }

    struct PalCacheEntry final
    {
        u32 mIndex = 0;
        bool mInUse = false;
    };
    std::map<u32, PalCacheEntry> mPaletteCache;
    u32 mMaxCacheSize = 0;
};
