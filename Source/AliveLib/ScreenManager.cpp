#include "stdafx.h"
#include "ScreenManager.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "vlctable.hpp"
#include <gmock/gmock.h>
#include "vlctable.hpp"
#include "Primitives.hpp"
#include "VRam.hpp"

ALIVE_VAR(1, 0x5BB5F4, ScreenManager*, pScreenManager_5BB5F4, nullptr);
ALIVE_ARY(1, 0x5b86c8, SprtTPage, 300, sSpriteTPageBuffer_5B86C8, {});

Camera::Camera()
{

}

void Camera::ctor_480DD0()
{
    field_0.ctor_40CA60(3);
    field_30_flags &= ~1u;
    field_C_pCamRes = nullptr;
}

void Camera::dtor_480E00()
{
    ResourceManager::FreeResource_49C330(field_C_pCamRes);

    DynamicArrayIter iter = {};
    iter.field_0_pDynamicArray = &field_0;
    iter.field_4_idx = 0;
    while (iter.field_4_idx < field_0.Size())
    {
        BYTE** pItem = field_0.ItemAt(iter.field_4_idx);
        iter.field_4_idx = iter.field_4_idx + 1;
        if (!pItem)
        {
            break;
        }
        ResourceManager::FreeResource_49C330(pItem);
        iter.Remove_At_Iter_40CCA0();
    }

    field_0.dtor_40CAD0();
}

void CC Camera::On_Loaded_480ED0(Camera* pCamera)
{
    pCamera->field_30_flags |= 1;
    pCamera->field_C_pCamRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Bits, pCamera->field_10_camera_resource_id, 1, 0);
}

void ScreenManager::sub_40EE10()
{
    NOT_IMPLEMENTED();
}

void ScreenManager::MoveImage_40EB70()
{
    PSX_RECT rect = {};
    rect.x = field_2C_upos;
    rect.y = field_2E_vpos;
    rect.h = 240;
    rect.w = 640;
    PSX_MoveImage_4F5D50(&rect, 0, 0);
}

void ScreenManager::VDestructor(signed int flags)
{
    dtor_40E460(flags);
}

void ScreenManager::dtor_40E490()
{
    BaseGameObject_dtor_4DBEC0();
}

void ScreenManager::dtor_40E460(signed int flags)
{
    dtor_40E490();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void ScreenManager::InvalidateRect_40EC90(int x, int y, signed int width, signed int height, int idx)
{
    x = max(x, 0);
    y = max(y, 0);

    width = min(width, 639);
    height = min(height, 239);

    for (int tileX = x / 32; tileX <= width / 32; tileX++)
    {
        for (int tileY = y / 16; tileY <= height / 16; tileY++)
        {
            field_64_20x16_dirty_bits[idx].SetTile(tileX, tileY, true);
        }
    }
}

void ScreenManager::InvalidateRect_Layer3_40EDB0(int x, int y, signed int width, signed int height)
{
    LOG_INFO("x" << x << " y " << y << " w " << width << " h " << height);
    InvalidateRect_40EC90(x, y, width, height, 3);
}

void ScreenManager::InvalidateRect_40EC50(int x, int y, signed int width, signed int height, int idx)
{
    InvalidateRect_40EC90(x, y, width, height, idx + 4);
}

__int16 ScreenManager::IsDirty_40EBC0(int idx, int x, int y)
{
    return field_64_20x16_dirty_bits[idx].GetTile(x / 32, y / 16);
}

void ScreenManager::UnsetDirtyBits_40EDE0(int idx)
{
    memset(&field_64_20x16_dirty_bits[idx], 0, sizeof(field_64_20x16_dirty_bits[idx]));
}

void ScreenManager::UnsetDirtyBits_FG1_40ED70()
{
    UnsetDirtyBits_40EDE0(7);
    UnsetDirtyBits_40EDE0(5);
    UnsetDirtyBits_40EDE0(6);
    UnsetDirtyBits_40EDE0(4);
}

void ScreenManager::InvalidateRect_40EC10(int x, int y, signed int width, signed int height)
{
    InvalidateRect_40EC90(x, y, width, height, field_3A_idx);
}

namespace Oddlib
{
    // NOTE: More reversing is required to fully understand these algorithms, but its something like JPEG
    // and since its enough to actually decode the data this is where the work on reversing stopped :)
    static const unsigned short int g_red_table[] =
    {
        0x00000, 0x00800, 0x01000, 0x01800, 0x02000, 0x02800, // 0
        0x03000, 0x03800, 0x04000, 0x04800, 0x05000, 0x05800, // 6
        0x06000, 0x06800, 0x07000, 0x07800, 0x08000, 0x08800, // 12
        0x09000, 0x09800, 0x0A000, 0x0A800, 0x0B000, 0x0B800, // 18
        0x0C000, 0x0C800, 0x0D000, 0x0D800, 0x0E000, 0x0E800, // 24
        0x0F000, 0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, // 30
        0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, // 36
        0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, // 42
        0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, // 48
        0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, 0x0F800, // 54
        0x0F800, 0x0F800, 0x0F800, 0x0F800                    // 60-64
    };

    static const unsigned short int g_blue_table[] =
    {
        0, 1, 2, 3, 4, 5,                   // 0
        6, 7, 8, 9, 0x0A, 0x0B,             // 6
        0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, // 12
        0x12, 0x13, 0x14, 0x15, 0x16, 0x17, // 18
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, // 24
        0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 30
        0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 36
        0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 42
        0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 48
        0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, // 54
        0x1F, 0x1F, 0x1F, 0x1F              // 60
    };

    static const unsigned short int g_green_table[] =
    {
        0x000, 0x040, 0x080, 0x0C0, 0x100, 0x140, // 0
        0x180, 0x1C0, 0x200, 0x240, 0x280, 0x2C0, // 6
        0x300, 0x340, 0x380, 0x3C0, 0x400, 0x440, // 12
        0x480, 0x4C0, 0x500, 0x540, 0x580, 0x5C0, // 18
        0x600, 0x640, 0x680, 0x6C0, 0x700, 0x740, // 24
        0x780, 0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, // 30
        0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, // 36
        0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, // 42
        0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, // 48
        0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x7C0, // 54
        0x7C0, 0x7C0, 0x7C0, 0x7C0, 0x000         // 60
    };

    // Encapsulates the logic of vlc_decoder() each call can read 3 words or 6 bytes max
    struct BitsLogic
    {
        BitsLogic()
            : param1(0), param2(0), param3(0), param4(0)
        {

        }

        BitsLogic(int& aPrev, ScreenManager* aStrat)
            : param1(0), param2(0), param3(0), param4(0)
        {
            // Grab 3x next bits
            bits[0] = aStrat->next_bits();
            bits[1] = aStrat->next_bits();
            bits[2] = aStrat->next_bits();

            // Round 1
            int calc1 = bits[2] - (bits[0] >> 1);
            int calc2 = calc1 + bits[0];
            int calc3 = aPrev - (bits[1] >> 1);

            // Round 2
            param1 = calc3 - (calc1 >> 1);
            param2 = param1 + calc1; // E.g calc1 = param2-param1, so calc3 = param1 + (calc1 >> 1) ?
            param3 = calc3 - (calc2 >> 1) + bits[1]; // knowing calc3 and 2 should yield bits[1]?

                                                     // Final magic thats only used outside of the loop for pixel 4
            param4 = param3 + calc2; //calc2 = param4 - param3 ?

            aPrev = param3 + calc2; // how we calc bottom right
        }

        // Read from the cam file data
        int bits[3]; // Used outside

        int param1;
        int param2;
        int param3;

        // Only used out of the loop
        int param4;
    };

    const auto red_mask = 0xF800;
    const auto green_mask = 0x7E0;
    const auto blue_mask = 0x1F;

}

int ScreenManager::next_bits()
{
    int ret = 0;
    if (g_left7_array <= 0)
    {
        ret = g_right25_array; // Always the previous g_right25_array! Or zero on first/ when its RLE data

        unsigned short int o = *g_pointer_to_vlc_buffer;

        g_left7_array = o >> 7;

        // Get 25 bits
        g_right25_array = (o << 25) >> 25;

        // To next word
        ++g_pointer_to_vlc_buffer;
    }
    else
    {
        // This is RLE? I.e if g_left7_array is 7 then we repeat 7 pixels?
        --g_left7_array;
    }
    return ret;
}


void ScreenManager::vlc_decode(WORD* aCamSeg, WORD* aDst)
{
    unsigned int vlcPtrIndex = 0;
    unsigned int camSrcPtrIndex = 0;
    unsigned int vlcTabIndex = 0;

    // Or two source words together to make a DWORD
    unsigned int dstVlcWord = aCamSeg[camSrcPtrIndex + 1] | (aCamSeg[camSrcPtrIndex] << 16);
    camSrcPtrIndex += 2; // Skip the two words we just OR'ed

    signed int totalBitsToShiftBy = 0;

    for (;;)
    {
        // Get 11 bits
        const unsigned int shiftedData = (dstVlcWord >> 21);

        // 0b11111111111 = 2047 * 4 =8192 = 8kb max index of 11 bits
        vlcTabIndex = 4 * shiftedData;

        // Grab vlc tab short using 11bit index * 4
        const unsigned int bitsToShiftBy = Oddlib::g_VlcTab[vlcTabIndex];

        // Shift var
        totalBitsToShiftBy += bitsToShiftBy;

        // Shift the other way by the vlc word
        dstVlcWord = dstVlcWord << bitsToShiftBy;

        // If we've shifted more than sizeof(short)
        if (totalBitsToShiftBy > 0xF)
        {
            // Limit to short, and set 1st short to the next
            // source word
            totalBitsToShiftBy = totalBitsToShiftBy & 0xF;
            dstVlcWord |= aCamSeg[camSrcPtrIndex++] << totalBitsToShiftBy;
        }

        int counter = 4;
        while (--counter)
        {
            unsigned short vlcWord = Oddlib::g_VlcTab[++vlcTabIndex];
            if (vlcWord == 0)
            {
                counter = 0; // continue
                break;
            }
            else if (vlcWord == 0xFFFF) // 1. Get an index that results in >0xf before here and ends up in here.
            {
                counter = 1; // don't continue
                break;
            }
            else
            {
                // Copy next table word
                aDst[vlcPtrIndex++] = vlcWord;
            }
        }
        if (counter == 0)
        {
            ++vlcTabIndex;
            continue;
        }

        // 2. Then get a none "1" value here and we can just take the 13bits to get
        // whatever value we want? Would have to assume we always want 0-0x1fff
        // values!

        // This can happen a lot (i.e this exit path is not taken once)
        // 0x1fff = 0b1111111111111 = max of >> 19 (13bits)
        aDst[vlcPtrIndex++] = dstVlcWord >> 19; // Thus the last word is always 1?

        if (dstVlcWord >> 19 == 1) // If bit 1 of the shifted 13bits is enabled only (could be &1 == 1)
        {
            break; // or return
        }

        totalBitsToShiftBy += 0xD;
        dstVlcWord = dstVlcWord << 0xD; // 0xD = 13 jib the bits above?

        if (totalBitsToShiftBy > 0xF) // Limit to 16 and pull another shifted byte in
        {
            totalBitsToShiftBy = totalBitsToShiftBy & 0xF; // Could just assign here?
            dstVlcWord |= aCamSeg[camSrcPtrIndex++] << totalBitsToShiftBy; // Move the word over if 0xF!
        }
    }
}


// This function takes a 16x240 strip of bits and processes as 16x16 sized macro blocks, thus there are 240/16=15 macro blocks
void ScreenManager::process_segment(WORD* aVlcBufferPtr, int xPos)
{
    g_pointer_to_vlc_buffer = aVlcBufferPtr;       // This is decoding one 16x240 seg

    g_left7_array = 0;
    next_bits();

    // 240/16 = 15 macro blocks for this strip
    for (int blockNo = 0; blockNo < 16; blockNo++)
    {
        // Each 16x16 block is decoded using a quad tree breaking it up in to 64 2x2 blocks
        int notUsed = 0;

        Oddlib::BitsLogic logic(notUsed, this);
        vlc_decoder(logic.bits[0], logic.bits[1], logic.bits[2], 16, xPos, blockNo * 16); // 16 is the width/block size
    }
}

void ScreenManager::vlc_decoder(int aR, int aG, int aB, signed int aWidth, int aVramX, int aVramY)
{
    while (aWidth != 2) // Quad tree through 16, 8, 4, 2 sizes
    {
        // These extra 3 are for each quad tree block? (of 16,8,4?) plus an extra 3 on the very first call?
        Oddlib::BitsLogic logic1(aR, this); // 1st param is a reference so call ordering matters
        Oddlib::BitsLogic logic2(aG, this);
        Oddlib::BitsLogic logic3(aB, this);

        aWidth = aWidth / 2;

        vlc_decoder(logic1.param1, logic2.param1, logic3.param1, aWidth, aVramX, aVramY);          // first block (top left)
        vlc_decoder(logic1.param2, logic2.param2, logic3.param2, aWidth, aVramX + aWidth, aVramY); // (top right)
        vlc_decoder(logic1.param3, logic2.param3, logic3.param3, aWidth, aVramX, aVramY + aWidth); // (bottom left)

        // last block (bottom right), never done until w=2 though?
        aVramY = aVramY + aWidth;
        aVramX = aVramX + aWidth;
    }

    Oddlib::BitsLogic r(aR, this);
    Oddlib::BitsLogic g(aG, this);
    Oddlib::BitsLogic b(aB, this);
    write_4_pixel_block(r, g, b, aVramX, aVramY);
}

static void SetPixel16(WORD* pLocked, DWORD pitch, int x, int y, WORD colour)
{
    y += (512/2)+16; // Write to lower half of vram
    pLocked[x + (y * pitch)] = colour;
}

void ScreenManager::write_4_pixel_block(const Oddlib::BitsLogic& aR, const Oddlib::BitsLogic& aG, const Oddlib::BitsLogic& aB, int aVramX, int aVramY)
{
    using namespace Oddlib;

    WORD* pData = reinterpret_cast<WORD*>(sPsxVram_C1D160.field_4_pLockedPixels);
    DWORD pitch = sPsxVram_C1D160.field_10_locked_pitch / 2;
    
    // Will go out of bounds due to macro blocks being 16x16, hence bounds check
    if (aVramY < 240)
    {
        SetPixel16(pData, pitch, aVramX, aVramY, g_red_table[aR.param1] | g_green_table[aG.param1] | g_blue_table[aB.param1]);
        SetPixel16(pData, pitch, aVramX + 1, aVramY, g_red_table[aR.param2] | g_green_table[aG.param2] | g_blue_table[aB.param2]);
    }

    if (aVramY + 1 < 240)
    {
        SetPixel16(pData, pitch, aVramX, aVramY + 1, g_red_table[aR.param3] | g_green_table[aG.param3] | g_blue_table[aB.param3]);
        SetPixel16(pData, pitch, aVramX + 1, aVramY + 1, g_red_table[aR.param4] | g_green_table[aG.param4] | g_blue_table[aB.param4]);
    }
}

void ScreenManager::DecompressToVRam_40EF60(WORD** ppBits)
{
    BYTE** ppVlc = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_VLC, 0, 0x7E00); // 4 KB
    if (ppVlc)
    {
        if (BMP_Lock_4F1FF0(&sPsxVram_C1D160))
        {
            const int kStripSize = 16;
            const int kNumStrips = 640 / kStripSize;

            WORD* pIter = *ppBits;
            for (int i = 0; i < kNumStrips; i++)
            {
                WORD stripSize = *pIter;
                pIter++;

                if (stripSize > 0)
                {
                    vlc_decode(pIter, reinterpret_cast<WORD*>(*ppVlc));
                    process_segment(reinterpret_cast<WORD*>(*ppVlc), i * kStripSize);
                }

                pIter += (stripSize / sizeof(WORD));
            }
            BMP_unlock_4F2100(&sPsxVram_C1D160);
        }
        ResourceManager::FreeResource_49C330(ppVlc);
        UnsetDirtyBits_40EDE0(0);
        UnsetDirtyBits_40EDE0(1);
        UnsetDirtyBits_40EDE0(2);
        UnsetDirtyBits_40EDE0(3);
    }
}

void ScreenManager::ctor_40E3E0(BYTE** ppBits, FP_Point* pCameraOffset)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    field_20_pCamPos = pCameraOffset;

    field_6_flags.Set(BaseGameObject::eBit08);
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);

    SetVTable(this, 0x5441E4);

    Init_40E4B0(ppBits);
}

void ScreenManager::Init_40E4B0(BYTE** ppBits)
{
    field_40_flags |= 0x10000;

    field_4_typeId = BaseGameObject::eScreenManager;

    field_2C_upos = 0;
    field_2E_vpos = 272;
    field_30_cam_width = 640;
    field_32_cam_height = 240;

    Vram_alloc_fixed_4955F0(0, 272, 640 - 1, 512 - 1);
    DecompressToVRam_40EF60(reinterpret_cast<WORD**>(ppBits));

    field_24_screen_sprites = &sSpriteTPageBuffer_5B86C8[0];

    short xpos = 0;
    short ypos = 0;
    for (int i = 0; i < 300; i++)
    {
        SprtTPage* pItem = &field_24_screen_sprites[i];
        Sprt_Init_4F8910(&pItem->mSprt);
        SetRGB0(&pItem->mSprt, 128, 128, 128);
        SetXY0(&pItem->mSprt, xpos, ypos);

        pItem->mSprt.field_14_w = 32;
        pItem->mSprt.field_16_h = 16;

        int u0 = field_2C_upos + 32 * (i % 20);
        int v0 = field_2E_vpos + 16 * (i / 20);
        int tpage = ScreenManager::GetTPage_40F040(2, 0, &u0, &v0);

        tpage |= 0x8000;

        Init_SetTPage_4F5B60(&pItem->mTPage, 0, 0, tpage);

        SetUV0(&pItem->mSprt, static_cast<BYTE>(u0), static_cast<BYTE>(v0));

        xpos += 32;
        if (xpos == 640)
        {
            xpos = 0;
            ypos += 16;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        UnsetDirtyBits_40EDE0(i);
    }

    field_3A_idx = 2;
    field_3C_y_idx = 1;
    field_3E_x_idx = 0;
}

int CC ScreenManager::GetTPage_40F040(char tp, char abr, int* xpos, int* ypos)
{
    const short clampedYPos = *ypos & 0xFF00;
    const short clampedXPos = *xpos & 0xFFC0;
    *xpos -= clampedXPos;
    *ypos -= clampedYPos;
    return PSX_getTPage_4F60E0(tp, abr, clampedXPos, clampedYPos);
}

void ScreenManager::VRender(int** pOrderingTable)
{
    VRender_40E6E0(pOrderingTable);
}

ALIVE_VAR(1, 0x5BB5DC, SprtTPage*, pCurrent_SprtTPage_5BB5DC, nullptr);
ALIVE_VAR(1, 0x5bb5f0, int, sCurrentYPos_5BB5F0, 0);
ALIVE_VAR(1, 0x5bb5d8, int, sIdx_5BB5D8, 0);

void ScreenManager::Render_Helper_40E9F0(int /*xpos*/, int /*ypos*/, int /*idx*/, int /*sprite_idx*/, int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

void ScreenManager::sub_40EE50()
{
    NOT_IMPLEMENTED();
}

void ScreenManager::VRender_40E6E0(int **ot)
{
    if (!(field_40_flags & 0x10000)) // Render enabled flag ?
    {
        return;
    }

    PSX_DrawSync_4F6280(0);
    pCurrent_SprtTPage_5BB5DC = nullptr;
    sCurrentYPos_5BB5F0 = -1;

    for (int i = 0; i < 300; i++)
    {
        SprtTPage* pSpriteTPage = &field_24_screen_sprites[i];
        const int spriteX = pSpriteTPage->mSprt.mBase.vert.x;
        const int spriteY = pSpriteTPage->mSprt.mBase.vert.y;

        if (IsDirty_40EBC0(7, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, 37, i, ot);
        }
        else if (IsDirty_40EBC0(6, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, 23, i, ot);
        }
        else if (IsDirty_40EBC0(5, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, 18, i, ot);
        }
        else if (IsDirty_40EBC0(4, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, 4, i, ot);
        }
        else if (IsDirty_40EBC0(field_3C_y_idx, spriteX, spriteY) || IsDirty_40EBC0(3, spriteX, spriteY))
        {
            if (spriteY != sCurrentYPos_5BB5F0 || sIdx_5BB5D8 != 1)
            {
                AddCurrentSPRT_TPage(ot);
                pSpriteTPage->mSprt.field_14_w = 32;
                pCurrent_SprtTPage_5BB5DC = pSpriteTPage;
                sIdx_5BB5D8 = 1;
                sCurrentYPos_5BB5F0 = spriteY;
            }
            else
            {
                pCurrent_SprtTPage_5BB5DC->mSprt.field_14_w += 32;
            }
        }
        else
        {
            if (pCurrent_SprtTPage_5BB5DC)
            {
                AddCurrentSPRT_TPage(ot);
                pCurrent_SprtTPage_5BB5DC = nullptr;
                sCurrentYPos_5BB5F0 = -1;
            }
        }
    }

    AddCurrentSPRT_TPage(ot);

    sub_40EE50();

    for (int i = 0; i < 20; i++)
    {
        field_64_20x16_dirty_bits[field_3C_y_idx].mData[i] |= field_64_20x16_dirty_bits[3].mData[i];
    }

    UnsetDirtyBits_40EDE0(3);
}

void ScreenManager::VScreenChanged()
{
    // NullSub@0x40F090
}

void ScreenManager::AddCurrentSPRT_TPage(int **ot)
{
    if (pCurrent_SprtTPage_5BB5DC)
    {
        OrderingTable_Add_4F8AA0(&ot[sIdx_5BB5D8], &pCurrent_SprtTPage_5BB5DC->mSprt.mBase.header);
        OrderingTable_Add_4F8AA0(&ot[sIdx_5BB5D8], &pCurrent_SprtTPage_5BB5DC->mTPage.mBase);
    }
}

namespace Test
{
    static void DirtyBitTests()
    {
        gBaseGameObject_list_BB47C4 = alive_new<DynamicArrayT<BaseGameObject>>();
        gBaseGameObject_list_BB47C4->ctor_40CA60(50);

        ScreenManager sm;
        sm.ctor_40E3E0(nullptr, nullptr);

        sm.UnsetDirtyBits_40EDE0(0);

        ASSERT_EQ(0, sm.IsDirty_40EBC0(0, 1, 1));

        sm.InvalidateRect_40EC10(0, 0, 32, 16);

        // ?? should be 1 ??
        ASSERT_EQ(0, sm.IsDirty_40EBC0(0, 0, 0));

        gBaseGameObject_list_BB47C4->dtor_40CAD0();
        gBaseGameObject_list_BB47C4 = nullptr;

        // Test dirty bit helpers

        sm.field_64_20x16_dirty_bits->SetTile(2, 8, true);
        sm.field_64_20x16_dirty_bits->SetTile(1, 4, false);

        ASSERT_EQ(sm.field_64_20x16_dirty_bits->GetTile(2, 8), true);
        ASSERT_EQ(sm.field_64_20x16_dirty_bits->GetTile(1, 4), false);
    }

    void ScreenManagerTests()
    {
        DirtyBitTests();
    }
}

