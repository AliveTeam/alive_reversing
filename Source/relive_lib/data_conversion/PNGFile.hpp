#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include <vector>

struct AnimationPal;

class PNGFile final
{
public:
    std::vector<u8> Encode(const u32* pixelData, u32 width, u32 height);
    void Decode(const std::vector<u8>& pngData, std::vector<u8>& rawPixels, u32& width, u32& height);
    void Load(const char_type* pFileName, std::vector<u8>& pixelData, u32& width, u32& height);
   
    void Load(const char_type* pFileName, AnimationPal& pal256, std::vector<u8>& pixelData, u32& width, u32& height);
    void Save(const char_type* pFileName, const AnimationPal& pal256, const std::vector<u8>& pixelData, u32 width, u32 height);

    u32 mOffset = 0;
    u8* mPtr = nullptr;
    std::vector<u8> mOutBuffer;
    bool mWrite = false;
};
