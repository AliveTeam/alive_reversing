#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include <map>

template<typename TextureType>
class TextureCache
{
public:
    virtual ~TextureCache()
    {
        Clear();
    }

    void Clear()
    {
        mTextureCache.clear();
    }

    TextureType Add(u32 uniqueId, u32 lifetime, TextureType texture)
    {
        CachedTexture newTex;

        newTex.mTexture = std::move(texture);
        newTex.mLifetime = lifetime;

        mTextureCache[uniqueId] = std::move(newTex);

        return mTextureCache[uniqueId].mTexture;
    }

    TextureType GetCachedTexture(u32 uniqueId, s32 bump)
    {
        auto it = mTextureCache.find(uniqueId);

        if (it == mTextureCache.end())
        {
            return TextureType();
        }

        if (bump > 0)
        {
            // Bump!
            it->second.mLifetime = bump;
        }

        return it->second.mTexture;
    }

    void DecreaseResourceLifetimes()
    {
        // Check texture lifetimes
        auto it = mTextureCache.begin();
        while (it != mTextureCache.end())
        {
            if (it->second.mLifetime-- <= 0)
            {
                it = mTextureCache.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

private:
    struct CachedTexture final
    {
        TextureType mTexture = {};
        s32 mLifetime = 0;


        CachedTexture()
        {
        }

        CachedTexture(CachedTexture&& src)
        : mTexture(std::move(src.mTexture)), mLifetime(src.mLifetime)
        {
        }


        CachedTexture& operator=(CachedTexture&& src)
        {
            mTexture = std::move(src.mTexture);
            mLifetime = src.mLifetime;
        }
    };
    std::map<u32, CachedTexture> mTextureCache;
};
