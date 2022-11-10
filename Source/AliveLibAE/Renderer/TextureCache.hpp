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

    virtual void DeleteTexture(TextureType texture) = 0;

    void Clear()
    {
        for (auto& it : mTextureCache)
        {
            DeleteTexture(it.second.mTextureId);
        }
        mTextureCache.clear();
    }

    TextureType Add(u32 uniqueId, u32 lifetime, TextureType texId)
    {
        CachedTexture newTex;

        newTex.mTextureId = texId;
        newTex.mLifetime = lifetime;

        mTextureCache[uniqueId] = newTex;

        return texId;
    }

    TextureType GetCachedTextureId(u32 uniqueId, s32 bump)
    {
        auto it = mTextureCache.find(uniqueId);

        if (it == mTextureCache.end())
        {
            return 0;
        }

        if (bump > 0)
        {
            // Bump!
            CachedTexture bumpTex;

            bumpTex.mTextureId = it->second.mTextureId;
            bumpTex.mLifetime = bump;

            mTextureCache[uniqueId] = bumpTex;
        }

        return it->second.mTextureId;
    }

    void DecreaseResourceLifetimes()
    {
        // Check texture lifetimes
        auto it = mTextureCache.begin();
        while (it != mTextureCache.end())
        {
            if (it->second.mLifetime-- <= 0)
            {
                DeleteTexture(it->second.mTextureId);
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
        TextureType mTextureId = {};
        s32 mLifetime = 0;
    };
    std::map<u32, CachedTexture> mTextureCache;
};
