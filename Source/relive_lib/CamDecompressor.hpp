#pragma once

#include "Types.hpp"

namespace Oddlib {
struct BitsLogic;
}
class CamDecompressor final
{
public:
    void process_segment(u16* aVlcBufferPtr, s32 xPos);
    void vlc_decode(const u16* aCamSeg, u16* aDst);
    void vlc_decoder(s32 aR, s32 aG, s32 aB, s32 aWidth, s32 aVramX, s32 aVramY);
    void write_4_pixel_block(const Oddlib::BitsLogic& aR, const Oddlib::BitsLogic& aG, const Oddlib::BitsLogic& aB, s32 aVramX, s32 aVramY);
    s32 next_bits();
    u16 mDecompressedStrip[16 * 240] = {};

private:
    s32 m_left7_array = 0;
    s32 m_right25_array = 0;
    u16* m_pointer_to_vlc_buffer = nullptr;
};
