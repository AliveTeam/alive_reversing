#include "stdafx.h"
#include "VRam.hpp"
#include "Function.hpp"
#include <gmock/gmock.h>

ALIVE_ARY(1, 0x5cb888, PSX_RECT, 512, sVramAllocations_5CB888, {});
ALIVE_VAR(1, 0x5cc888, int, sVram_Count_dword_5CC888, 0);

EXPORT char CC Vram_calc_width_4955A0(int width, int depth)
{
    switch (depth)
    {
    case 0:
        return ((width + 7) >> 2) & 0xFE;
    case 1:
        return ((width + 3) >> 1) & 0xFE;
    case 2:
        return (width + 1) & 0xFE;
    }
    return 0;
}

EXPORT signed int __cdecl Vram_4958F0(PSX_RECT *pRect, char depth)
{
    // TODO: Needs to be cleaned up
    NOT_IMPLEMENTED();

    
    int v6; // eax

    pRect->x = 1024 - pRect->w;
    
    const char depthShift = 2 - depth;

    if (pRect->x >= 0)
    {
        while (true)
        {
            if ((pRect->w << depthShift) + ((pRect->x & 0x3F) << depthShift) > 256)
            {
                break;
            }
            
            if (sVram_Count_dword_5CC888 <= 0)
            {
                return 1;
            }

            int i = 0;
            PSX_RECT *currentVramAlloc = sVramAllocations_5CB888;
            while (!Vram_rects_overlap_4959E0(pRect, currentVramAlloc))
            {
                i++;
                currentVramAlloc++;
                if (i >= sVram_Count_dword_5CC888)
                {
                    return 1;
                }
            }

            v6 = currentVramAlloc->x - pRect->w + 1;

            if (v6 < pRect->x)
            {
                goto LABEL_12;
            }
        LABEL_13:
            if (--pRect->x < 0)
            {
                return 0;
            }
        }
        
        //v5 |= ((pRect->x + 63) & 0xC0) >> 24; //LOBYTE(v5) = (v4 + 63) & 0xC0; TODO: CHECK if this is okay
        v6 = (pRect->x + 63) - pRect->w + 1;
        if (v6 >= pRect->x)
        {
            goto LABEL_13;
        }
    LABEL_12:
        pRect->x = v6;
        goto LABEL_13;
    }
    return 0;
}

EXPORT signed int __cdecl Vram_4957B0(PSX_RECT *pRect, int depth)
{
    if (pRect->w > 1024 || pRect->h > 512)
    {
        return 0;
    }

    if (pRect->h * pRect->w >= 1024)
    {
        pRect->y = 512 - pRect->h;
        if (pRect->y >= 0)
        {
            do
            {
                // Old Code: if (static_cast<unsigned __int8>(pRect->y) + pRect->h <= 256)
                // Instead of casting to unsigned __int8 to wrap around the integer, we're
                // going to do it manually incase other platforms dont auto wrap integers on
                // cast.
                if ((pRect->y % 256) + pRect->h <= 256)
                {
                    if (Vram_4958F0(pRect, depth))
                    {
                        return 1;
                    }
                }
                else
                {
                    // v7 &= 0xFFFFFF00; // Todo: check this. was LOBYTE(v7) = 0; Doesn't seem needed
                    // to pass tests.
                    const int v8 = (pRect->y + 255) - pRect->h + 1;
                    if (v8 < pRect->y)
                    {
                        pRect->y = v8;
                    }
                }
                --pRect->y;
            } 
            while (pRect->y >= 0);
        }
        return 0;
    }

    pRect->y = 0;
    if (512 - pRect->h <= 0)
    {
        return 0;
    }

    // Search Loop
    while (true)
    {
        const short yPos = pRect->y;
        if (pRect->h + yPos <= 255 || yPos >= 256)
        {
            if (!Vram_4958F0(pRect, depth))
            {
                pRect->y++;
                if (pRect->y >= 512 - pRect->h)
                {
                    return 0;
                }

                continue;
            }
            else
            {
                return 1;
            }
        }
        pRect->y = 255;

        ++pRect->y;
        if (pRect->y >= 512 - pRect->h)
        {
            return 0;
        }
    }
    
    return 1;
}

signed __int16 CC Vram_alloc_4956C0(unsigned __int16 width, __int16 height, unsigned __int16 colourDepth, PSX_RECT* pRect)
{
    PSX_RECT rect;

    int depth = colourDepth / 8;

    rect.w = Vram_calc_width_4955A0(width, depth);
    rect.h = height;

    if (sVram_Count_dword_5CC888 >= 512 || !Vram_4957B0(&rect, depth))
    {
        return 0;
    }

    sVramAllocations_5CB888[sVram_Count_dword_5CC888++] = rect;
    *pRect = rect;

    return 1;
}

int __cdecl Vram_alloc_fixed_4955F0(__int16 /*a1*/, __int16 /*a2*/, __int16 /*a3*/, __int16 /*a4*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void CC Vram_free_495A60(int /*xy*/, int /*wh*/)
{
    NOT_IMPLEMENTED();
}

signed __int16 CC Pal_Allocate_483110(PSX_RECT* /*a1*/, unsigned int /*paletteColorCount*/)
{
    NOT_IMPLEMENTED();
}

void CC Pal_free_483390(PSX_Point /*xy*/, __int16 /*palDepth*/)
{
    NOT_IMPLEMENTED();
}

EXPORT BOOL CC Vram_rects_overlap_4959E0(const PSX_RECT* pRect1, const PSX_RECT* pRect2)
{
    const int x1 = pRect1->x;
    const int x2 = pRect2->x;
    if (x1 >= x2 + pRect2->w)
    {
        return 0;
    }

    const int y2 = pRect2->y;
    const int y1 = pRect1->y;
    if (y1 >= y2 + pRect2->h)
    {
        return 0;
    }

    if (x2 < x1 + pRect1->w)
    {
        return y2 < y1 + pRect1->h;
    }

    return 0;
}

using namespace ::testing;

namespace Test
{
    void Test_VRamAllocate()
    {
        PSX_RECT rect;
        Vram_alloc_4956C0(64, 128, 8, &rect);
        ASSERT_EQ(rect.x, 992);
        ASSERT_EQ(rect.y, 384);
        ASSERT_EQ(rect.w, 32);
        ASSERT_EQ(rect.h, 128);

        PSX_RECT rect2;
        Vram_alloc_4956C0(32, 45, 16, &rect2);
        ASSERT_EQ(rect2.x, 960);
        ASSERT_EQ(rect2.y, 467);
        ASSERT_EQ(rect2.w, 32);
        ASSERT_EQ(rect2.h, 45);

        PSX_RECT rect3;
        Vram_alloc_4956C0(32, 16, 8, &rect3);

        Vram_free_495A60(*reinterpret_cast<int*>(&rect.x), *reinterpret_cast<int*>(&rect.w));
        Vram_free_495A60(*reinterpret_cast<int*>(&rect2.x), *reinterpret_cast<int*>(&rect2.w));
        Vram_free_495A60(*reinterpret_cast<int*>(&rect3.x), *reinterpret_cast<int*>(&rect3.w));
    }

    void VRamTests()
    {
        Test_VRamAllocate();
    }
}