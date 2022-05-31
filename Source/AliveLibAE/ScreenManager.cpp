#include "stdafx.h"
#include "ScreenManager.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include <gmock/gmock.h>
#include "Primitives.hpp"
#include "VRam.hpp"
#include "Psx.hpp"
#include "Renderer/IRenderer.hpp"
#include "../AliveLibCommon/CamDecompressor.hpp"

ALIVE_VAR(1, 0x5BB5F4, ScreenManager*, pScreenManager_5BB5F4, nullptr);
ALIVE_ARY(1, 0x5b86c8, SprtTPage, 300, sSpriteTPageBuffer_5B86C8, {});

void ScreenManager::sub_40EE10()
{
    for (s32 i = 0; i < 20; i++)
    {
        field_64_20x16_dirty_bits[field_3C_y_idx].mData[i] |= field_64_20x16_dirty_bits[field_3E_x_idx].mData[i];
    }
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

void ScreenManager::InvalidateRect_40EC90(s32 x, s32 y, s32 width, s32 height, s32 idx)
{
    x = std::max(x, 0);
    y = std::max(y, 0);

    width = std::min(width, 639);
    height = std::min(height, 239);

    for (s32 tileX = x / 32; tileX <= width / 32; tileX++)
    {
        for (s32 tileY = y / 16; tileY <= height / 16; tileY++)
        {
            field_64_20x16_dirty_bits[idx].SetTile(tileX, tileY, true);
        }
    }
}

void ScreenManager::InvalidateRect_Layer3_40EDB0(s32 x, s32 y, s32 width, s32 height)
{
    InvalidateRect_40EC90(x, y, width, height, 3);
}

void ScreenManager::InvalidateRect_40EC50(s32 x, s32 y, s32 width, s32 height, s32 idx)
{
    InvalidateRect_40EC90(x, y, width, height, idx + 4);
}

s16 ScreenManager::IsDirty_40EBC0(s32 idx, s32 x, s32 y)
{
    return field_64_20x16_dirty_bits[idx].GetTile(x / 32, y / 16);
}

void ScreenManager::UnsetDirtyBits_40EDE0(s32 idx)
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

void ScreenManager::InvalidateRect_40EC10(s32 x, s32 y, s32 width, s32 height)
{
    InvalidateRect_40EC90(x, y, width, height, field_3A_idx);
}

const s32 kStripSize = 16;
const s32 kNumStrips = 640 / kStripSize;

static bool IsHackedAOCamera(u16** ppBits)
{
    // If they are its a "hacked" camera from paulsapps level editor. This editor used an
    // injected dll to replace camera images. So this code here replicates that so "old" mods
    // can still work.

    // Check if all the segments are the same specific size
    u16* pIter = *ppBits;
    s32 countOf7680SizedSegments = 0;
    for (s32 i = 0; i < kNumStrips; i++)
    {
        const u16 stripSize = *pIter;
        pIter++;
        if (stripSize == 7680)
        {
            countOf7680SizedSegments++;
        }
        pIter += (stripSize / sizeof(u16));
    }

    return countOf7680SizedSegments == kNumStrips;
}

void ScreenManager::DecompressCameraToVRam_40EF60(u16** ppBits)
{
    if (IsHackedAOCamera(ppBits))
    {
        LOG_INFO("Applying AO camera");

        u16* pIter = *ppBits;
        for (s32 i = 0; i < kNumStrips; i++)
        {
            const u16 stripSize = *pIter;
            pIter++;

            const PSX_RECT rect = {static_cast<s16>(i * kStripSize), 256 + 16, kStripSize, 240};
            IRenderer::GetRenderer()->Upload(IRenderer::BitDepth::e8Bit, rect, reinterpret_cast<const u8*>(pIter));
            pIter += (stripSize / sizeof(u16));
        }
    }
    else
    {
        // AE camera

        u8** ppVlc = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_VLC, 0, 0x7E00); // 4 KB
        if (ppVlc)
        {
            PSX_RECT rect = { 0, 0, 16, 240 };
            CamDecompressor decompressor;
 
            u16* pIter = *ppBits;
            for (s16 xpos = 0; xpos < 640; xpos += 16)
            {
                const u16 stripSize = *pIter;
                pIter++;

                if (stripSize > 0)
                {
                    decompressor.vlc_decode(pIter, reinterpret_cast<u16*>(*ppVlc));
                    decompressor.process_segment(reinterpret_cast<u16*>(*ppVlc), 0);

                    rect.x = field_2C_upos + xpos;
                    rect.y = field_2E_vpos;

                    IRenderer::GetRenderer()->Upload(IRenderer::BitDepth::e8Bit, rect, reinterpret_cast<const u8*>(decompressor.mDecompressedStrip));
                }

                pIter += (stripSize / sizeof(u16));
            }

            ResourceManager::FreeResource_49C330(ppVlc);
        }
    }

    UnsetDirtyBits_40EDE0(0);
    UnsetDirtyBits_40EDE0(1);
    UnsetDirtyBits_40EDE0(2);
    UnsetDirtyBits_40EDE0(3);
}

ScreenManager::ScreenManager(u8** ppBits, FP_Point* pCameraOffset)
    : BaseGameObject(TRUE, 0)
{
    field_20_pCamPos = pCameraOffset;

    mFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    Init_40E4B0(ppBits);
}

void ScreenManager::Init_40E4B0(u8** ppBits)
{
    field_40_flags |= 0x10000;

    SetType(AETypes::eScreenManager_6);

    field_2C_upos = 0;
    field_2E_vpos = 272;
    field_30_cam_width = 640;
    field_32_cam_height = 240;

    Vram_alloc_explicit_4955F0(0, 272, 640 - 1, 512 - 1);
    DecompressCameraToVRam_40EF60(reinterpret_cast<u16**>(ppBits));

    field_24_screen_sprites = &sSpriteTPageBuffer_5B86C8[0];

    s16 xpos = 0;
    s16 ypos = 0;
    for (s32 i = 0; i < 300; i++)
    {
        SprtTPage* pItem = &field_24_screen_sprites[i];
        Sprt_Init_4F8910(&pItem->mSprt);
        SetRGB0(&pItem->mSprt, 128, 128, 128);
        SetXY0(&pItem->mSprt, xpos, ypos);

        pItem->mSprt.field_14_w = 32;
        pItem->mSprt.field_16_h = 16;

        s32 u0 = field_2C_upos + 32 * (i % 20);
        s32 v0 = field_2E_vpos + 16 * (i / 20);
        s32 tpage = ScreenManager::GetTPage_40F040(TPageMode::e16Bit_2, TPageAbr::eBlend_0, &u0, &v0);

        tpage |= 0x8000;

        Init_SetTPage_4F5B60(&pItem->mTPage, 0, 0, tpage);

        SetUV0(&pItem->mSprt, static_cast<u8>(u0), static_cast<u8>(v0));

        xpos += 32;
        if (xpos == 640)
        {
            xpos = 0;
            ypos += 16;
        }
    }

    for (s32 i = 0; i < 8; i++)
    {
        UnsetDirtyBits_40EDE0(i);
    }

    field_3A_idx = 2;
    field_3C_y_idx = 1;
    field_3E_x_idx = 0;
}

s32 ScreenManager::GetTPage_40F040(TPageMode tp, TPageAbr abr, s32* xpos, s32* ypos)
{
    const s16 clampedYPos = *ypos & 0xFF00;
    const s16 clampedXPos = *xpos & 0xFFC0;
    *xpos -= clampedXPos;
    *ypos -= clampedYPos;
    return PSX_getTPage_4F60E0(tp, abr, clampedXPos, clampedYPos);
}

void ScreenManager::VRender(PrimHeader** ppOt)
{
    VRender_40E6E0(ppOt);
}

ALIVE_VAR(1, 0x5BB5DC, SprtTPage*, pCurrent_SprtTPage_5BB5DC, nullptr);
ALIVE_VAR(1, 0x5bb5f0, s32, sCurrentYPos_5BB5F0, 0);
ALIVE_VAR(1, 0x5bb5d8, Layer, sIdx_5BB5D8, Layer::eLayer_0);

void ScreenManager::Render_Helper_40E9F0(s32 xpos, s32 ypos, Layer idx, s32 sprite_idx, PrimHeader** ppOt)
{
    if (IsDirty_40EBC0(field_3A_idx, xpos, ypos) || IsDirty_40EBC0(field_3C_y_idx, xpos, ypos) || IsDirty_40EBC0(3, xpos, ypos))
    {
        SprtTPage* pSprite = &field_24_screen_sprites[sprite_idx];
        if (Y0(&pSprite->mSprt) != sCurrentYPos_5BB5F0 || sIdx_5BB5D8 != idx)
        {
            if (pCurrent_SprtTPage_5BB5DC)
            {
                OrderingTable_Add_4F8AA0(OtLayer(ppOt, sIdx_5BB5D8), &pCurrent_SprtTPage_5BB5DC->mSprt.mBase.header);
                OrderingTable_Add_4F8AA0(OtLayer(ppOt, sIdx_5BB5D8), &pCurrent_SprtTPage_5BB5DC->mTPage.mBase);
            }
            pSprite->mSprt.field_14_w = 32;
            pCurrent_SprtTPage_5BB5DC = pSprite;
            sIdx_5BB5D8 = idx;
            sCurrentYPos_5BB5F0 = Y0(&pSprite->mSprt);
        }
        else
        {
            pCurrent_SprtTPage_5BB5DC->mSprt.field_14_w += 32;
        }
    }
    else if (pCurrent_SprtTPage_5BB5DC)
    {
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, sIdx_5BB5D8), &pCurrent_SprtTPage_5BB5DC->mSprt.mBase.header);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, sIdx_5BB5D8), &pCurrent_SprtTPage_5BB5DC->mTPage.mBase);
        pCurrent_SprtTPage_5BB5DC = nullptr;
        sCurrentYPos_5BB5F0 = -1;
    }
}

void ScreenManager::sub_40EE50()
{
    // NOTE: The algorithm calling Add_Dirty_Area_4ED970 has not been implemented
    // as its not actually used.

    field_3E_x_idx = field_3C_y_idx;
    field_3C_y_idx = field_3A_idx;
    field_3A_idx = (field_3A_idx + 1) % 3;
    UnsetDirtyBits_40EDE0(field_3A_idx);
}

void ScreenManager::VRender_40E6E0(PrimHeader** ppOt)
{
    if (!(field_40_flags & 0x10000)) // Render enabled flag ?
    {
        return;
    }

#if RENDERER_OPENGL
    // TODO: A custom sprite prim with magic numbers
    // to trigger proper order rendering of our cam.
    static Prim_Sprt MagicBackgroundPrim;
    Sprt_Init_4F8910(&MagicBackgroundPrim);
    SetRGB0(&MagicBackgroundPrim, 255, 254, 253);
    OrderingTable_Add_4F8AA0(OtLayer(ppOt, Layer::eLayer_1), &MagicBackgroundPrim.mBase.header);
#endif

    PSX_DrawSync_4F6280(0);
    pCurrent_SprtTPage_5BB5DC = nullptr;
    sCurrentYPos_5BB5F0 = -1;

    for (s32 i = 0; i < 300; i++)
    {
        SprtTPage* pSpriteTPage = &field_24_screen_sprites[i];
        const s32 spriteX = pSpriteTPage->mSprt.mBase.vert.x;
        const s32 spriteY = pSpriteTPage->mSprt.mBase.vert.y;

        if (IsDirty_40EBC0(7, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, Layer::eLayer_FG1_37, i, ppOt);
        }
        else if (IsDirty_40EBC0(6, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, Layer::eLayer_Well_23, i, ppOt);
        }
        else if (IsDirty_40EBC0(5, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, Layer::eLayer_FG1_Half_18, i, ppOt);
        }
        else if (IsDirty_40EBC0(4, spriteX, spriteY))
        {
            Render_Helper_40E9F0(spriteX, spriteY, Layer::eLayer_Well_Half_4, i, ppOt);
        }
        else if (IsDirty_40EBC0(field_3C_y_idx, spriteX, spriteY) || IsDirty_40EBC0(3, spriteX, spriteY))
        {
            if (spriteY != sCurrentYPos_5BB5F0 || sIdx_5BB5D8 != Layer::eLayer_1)
            {
                AddCurrentSPRT_TPage(ppOt);
                pSpriteTPage->mSprt.field_14_w = 32;
                pCurrent_SprtTPage_5BB5DC = pSpriteTPage;
                sIdx_5BB5D8 = Layer::eLayer_1;
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
                AddCurrentSPRT_TPage(ppOt);
                pCurrent_SprtTPage_5BB5DC = nullptr;
                sCurrentYPos_5BB5F0 = -1;
            }
        }
    }

    AddCurrentSPRT_TPage(ppOt);

    sub_40EE50();

    for (s32 i = 0; i < 20; i++)
    {
        field_64_20x16_dirty_bits[field_3C_y_idx].mData[i] |= field_64_20x16_dirty_bits[3].mData[i];
    }

    UnsetDirtyBits_40EDE0(3);
}

void ScreenManager::VScreenChanged()
{
    // NullSub@0x40F090
}

void ScreenManager::AddCurrentSPRT_TPage(PrimHeader** ppOt)
{
    if (pCurrent_SprtTPage_5BB5DC)
    {
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, sIdx_5BB5D8), &pCurrent_SprtTPage_5BB5DC->mSprt.mBase.header);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, sIdx_5BB5D8), &pCurrent_SprtTPage_5BB5DC->mTPage.mBase);
    }
}

namespace AETest::TestsScreenManager {
void DirtyBitTests()
{
    gBaseGameObjects = ae_new<DynamicArrayT<BaseGameObject>>();
    gBaseGameObjects->ctor_40CA60(50);

    ScreenManager sm(nullptr, nullptr);

    sm.UnsetDirtyBits_40EDE0(0);

    ASSERT_EQ(0, sm.IsDirty_40EBC0(0, 1, 1));

    sm.InvalidateRect_40EC10(0, 0, 32, 16);

    // ?? should be 1 ??
    ASSERT_EQ(0, sm.IsDirty_40EBC0(0, 0, 0));

    gBaseGameObjects->dtor_40CAD0();
    gBaseGameObjects = nullptr;

    // Test dirty bit helpers

    sm.field_64_20x16_dirty_bits->SetTile(2, 8, true);
    sm.field_64_20x16_dirty_bits->SetTile(1, 4, false);

    ASSERT_EQ(sm.field_64_20x16_dirty_bits->GetTile(2, 8), true);
    ASSERT_EQ(sm.field_64_20x16_dirty_bits->GetTile(1, 4), false);
}

void ScreenManagerTests()
{
    //DirtyBitTests();
}
} // namespace AETest::TestsScreenManager
