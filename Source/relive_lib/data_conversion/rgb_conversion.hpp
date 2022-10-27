#pragma once

class RGBConversion final
{
public:
    struct RGBA32 final
    {
        u8 r, g, b, a;
    };

    static u16 RGB888ToRGB565(u8 r8, u8 g8, u8 b8)
    {
        const u8 red = r8;
        const u8 green = g8;
        const u8 blue = b8;

        const u16 b = (blue >> 3) & 0x1f;
        const u16 g = ((green >> 2) & 0x3f) << 5;
        const u16 r = ((red >> 3) & 0x1f) << 11;

        return (r | g | b);
    }

    static u16 RGB888ToRGB555(u8 r8, u8 g8, u8 b8, u8 a8)
    {
        const u8 red = r8;
        const u8 green = g8;
        const u8 blue = b8;

        const u16 b = (blue >> 3) & 0x1f;
        const u16 g = ((green >> 3) & 0x1f) << 5;
        const u16 r = ((red >> 3) & 0x1f) << 10;

        u16 ret = (r | g | b);
        if (a8)
        {
            ret |= 1 << 15;
        }
        return ret;
    }

    static u16 RGB888ToRGB565(const u8* rgb888Pixel)
    {
        const u8 red = rgb888Pixel[0];
        const u8 green = rgb888Pixel[1];
        const u8 blue = rgb888Pixel[2];

        return RGB888ToRGB565(red, green, blue);
    }

    static RGBA32 RGBA555ToRGBA888Components(u16 pixel)
    {
        const u8 r5 = ((pixel >> 10) & 0x1F);
        const u8 g5 = ((pixel >> 5) & 0x1F);
        const u8 b5 = (pixel & 0x1F);
        bool bSemi = (pixel >> 15) & 0x1;

        const u32 r8 = ((r5 * 527) + 23) >> 6;
        const u32 g8 = ((g5 * 527) + 23) >> 6;
        const u32 b8 = ((b5 * 527) + 23) >> 6;
        u32 a8 = 0;
        if (bSemi)
        {
            a8 = 255;
        }
        /*
        if (!bSemi && r5 == 0 && g5 == 0 && b5 == 0)
        {
            a8 = 0;
        }*/

        RGBA32 ret;
        ret.r = static_cast<u8>(r8);
        ret.g = static_cast<u8>(g8);
        ret.b = static_cast<u8>(b8);
        ret.a = static_cast<u8>(a8);
        return ret;
    }

    static u32 RGB565ToRGB888(u16 pixel)
    {
        const u8 r5 = ((pixel >> 11) & 0x1F);
        const u8 g6 = ((pixel >> 5) & 0x3F);
        const u8 b5 = (pixel & 0x1F);

        const u32 r8 = ((r5 * 527) + 23) >> 6;
        const u32 g8 = ((g6 * 259) + 33) >> 6;
        const u32 b8 = ((b5 * 527) + 23) >> 6;

        const u32 rgb888 = (b8 << 16) | (g8 << 8) | r8;
        return rgb888;
    }

    // TODO: Only used by the software renderer which isn't yet working
    // and seems to be completely wrong
    static u32 RGBA555ToRGBA888(u16 pixel)
    {
        const u8 r5 = ((pixel >> 10) & 0x1F);
        const u8 g5 = ((pixel >> 5) & 0x1F);
        const u8 b5 = (pixel & 0x1F);
        bool bSemi = (pixel >> 15) & 0x1;

        const u32 r8 = ((r5 * 527) + 23) >> 6;
        const u32 g8 = ((g5 * 527) + 23) >> 6;
        const u32 b8 = ((b5 * 527) + 23) >> 6;
        u32 a8 = bSemi ? 127 : 255;

        if (!bSemi && r5 == 0 && g5 == 0 && b5 == 0)
        {
            a8 = 0;
        }

        const u32 rgb888 = (a8 << 24) | (b8 << 16) | (g8 << 8) | r8;
        return rgb888;
    }
};
