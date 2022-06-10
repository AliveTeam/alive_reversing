#include "stdafx_ao.h"
#include "ScreenManager.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "VRam.hpp"
#include "stdlib.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"

#undef min
#undef max

namespace AO {

ALIVE_VAR(1, 0x4FF7C8, ScreenManager*, pScreenManager, nullptr);
ALIVE_ARY(1, 0x4FC8A8, SprtTPage, 300, sSpriteTPageBuffer, {});

void ScreenManager::MoveImage()
{
    PSX_RECT rect = {};
    rect.x = mUPos;
    rect.y = mVPos;
    rect.h = 240;
    rect.w = 640;
    PSX_MoveImage(&rect, 0, 0);
}

void ScreenManager::InvalidateRect(s32 x, s32 y, s32 width, s32 height, s32 idx)
{
    x = std::max(x, 0);
    y = std::max(y, 0);

    width = std::min(width, 639);
    height = std::min(height, 239);

    for (s32 tileX = x / 32; tileX <= width / 32; tileX++)
    {
        for (s32 tileY = y / 16; tileY <= height / 16; tileY++)
        {
            mDirtyBits[idx].SetTile(tileX, tileY, true);
        }
    }
}

void ScreenManager::InvalidateRect_Layer3(s32 x, s32 y, s32 width, s32 height)
{
    InvalidateRect(x, y, width, height, 3);
}

void ScreenManager::InvalidateRect_IdxPlus4(s32 x, s32 y, s32 width, s32 height, s32 idx)
{
    InvalidateRect(x, y, width, height, idx + 4);
}

void ScreenManager::InvalidateRectCurrentIdx(s32 x, s32 y, s32 width, s32 height)
{
    InvalidateRect(x, y, width, height, mIdx);
}

void ScreenManager::UnsetDirtyBits_FG1()
{
    memset(&mDirtyBits[4], 0, sizeof(this->mDirtyBits[4]));
    memset(&mDirtyBits[5], 0, sizeof(this->mDirtyBits[5]));
}






void ScreenManager::DecompressCameraToVRam(u16** ppBits)
{
    PSX_RECT rect = { 0, 0, 16, 240 };
    u8** pRes = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_VLC, 0, 0x7E00); // 4 KB
    if (pRes)
    {
        // Doesn't do anything since the images are not MDEC compressed in PC
        //PSX_MDEC_rest_498C30(0);

        u16* pIter = *ppBits;
        for (s16 xpos = 0; xpos < 640; xpos += 16)
        {
            const u16 slice_len = *pIter;
            pIter++; // Skip len

            // already in correct format - no need to convert
            //rgb_conv_44FFE0(pIter, tmpBuffer, sizeof(tmpBuffer));

            rect.x = mUPos + xpos;
            rect.y = mVPos;

            // TODO: Actually 16bit but must be uploaded as 8bit ??
            IRenderer::GetRenderer()->Upload(IRenderer::BitDepth::e8Bit, rect, reinterpret_cast<u8*>(pIter));

            // To next slice
            pIter += (slice_len / sizeof(s16));
        }

        ResourceManager::FreeResource_455550(pRes);

        mDirtyBits[0] = {};
        mDirtyBits[1] = {};
        mDirtyBits[2] = {};
        mDirtyBits[3] = {};
    }
}



ScreenManager::ScreenManager(u8** ppBits, FP_Point* pCameraOffset)
    : BaseGameObject(TRUE, 0)
{
    mCamPos = pCameraOffset;

    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    Init(ppBits);
}

void ScreenManager::Init(u8** ppBits)
{
    mFlags |= 1;

    SetType(ReliveTypes::eScreenManager);

    mCamXOff = 184;
    mCamYOff = 120;

    mUPos = 0;
    mVPos = 272;
    mCamWidth = 640;
    mCamHeight = 240;

    Vram_alloc_explicit(0, 272, 640, 512);
    DecompressCameraToVRam(reinterpret_cast<u16**>(ppBits));

    mScreenSprites = &sSpriteTPageBuffer[0];

    s16 xpos = 0;
    s16 ypos = 0;
    for (s32 i = 0; i < 300; i++)
    {
        SprtTPage* pItem = &mScreenSprites[i];
        Sprt_Init(&pItem->mSprt);
        SetRGB0(&pItem->mSprt, 128, 128, 128);
        SetXY0(&pItem->mSprt, xpos, ypos);

        pItem->mSprt.field_14_w = 32;
        pItem->mSprt.field_16_h = 16;

        s32 u0 = mUPos + 32 * (i % 20);
        s32 v0 = mVPos + 16 * (i / 20);
        s32 tpage = ScreenManager::GetTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_0, &u0, &v0);

        tpage |= 0x8000;

        Init_SetTPage(&pItem->mTPage, 0, 0, tpage);

        SetUV0(&pItem->mSprt, static_cast<u8>(u0), static_cast<u8>(v0));

        xpos += 32;
        if (xpos == 640)
        {
            xpos = 0;
            ypos += 16;
        }
    }

    for (s32 i = 0; i < 6; i++)
    {
        memset(&mDirtyBits[i], 0, sizeof(mDirtyBits[0]));
    }

    mIdx = 2;
    mYIdx = 1;
    mXIdx = 0;
}

void ScreenManager::VUpdate()
{
    // Empty
}

s32 ScreenManager::GetTPage(TPageMode tp, TPageAbr abr, s32* xpos, s32* ypos)
{
    const s16 clampedYPos = *ypos & 0xFF00;
    const s16 clampedXPos = *xpos & 0xFFC0;
    *xpos -= clampedXPos;
    *ypos -= clampedYPos;
    return PSX_getTPage(tp, abr, clampedXPos, clampedYPos);
}

void ScreenManager::sub_406FF0()
{
    // NOTE: The algorithm calling Add_Dirty_Area_48D910 has not been implemented
    // as its not actually used.

    mXIdx = mYIdx;
    mYIdx = mIdx;
    mIdx = (mIdx + 1) % 3;
    memset(
        &mDirtyBits[mIdx],
        0,
        sizeof(mDirtyBits[mIdx]));
}


void ScreenManager::VRender(PrimHeader** ppOt)
{
    if (!(mFlags & 1)) // Render enabled flag ?
    {
        return;
    }

#if RENDERER_OPENGL
    // TODO: A custom sprite prim with magic numbers
    // to trigger proper order rendering of our cam.
    static Prim_Sprt MagicBackgroundPrim;
    Sprt_Init(&MagicBackgroundPrim);
    SetRGB0(&MagicBackgroundPrim, 255, 254, 253);
    OrderingTable_Add_498A80(OtLayer(ppOt, Layer::eLayer_1), &MagicBackgroundPrim.mBase.header);
#endif

    PSX_DrawSync_496750(0);

    for (s32 i = 0; i < 300; i++)
    {
        SprtTPage* pSpriteTPage = &mScreenSprites[i];
        const s32 spriteX = pSpriteTPage->mSprt.mBase.vert.x;
        const s32 spriteY = pSpriteTPage->mSprt.mBase.vert.y;

        Layer layer = Layer::eLayer_0;
        if (mDirtyBits[4].GetTile(spriteX / 32, spriteY / 16))
        {
            if (!(mDirtyBits[mIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[mYIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[mXIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[3].GetTile(spriteX / 32, spriteY / 16)))
            {
                continue;
            }
            layer = Layer::eLayer_FG1_37;
        }
        else if (mDirtyBits[5].GetTile(spriteX / 32, spriteY / 16))
        {
            if (!(mDirtyBits[mIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[mYIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[mXIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[3].GetTile(spriteX / 32, spriteY / 16)))
            {
                continue;
            }
            layer = Layer::eLayer_FG1_Half_18;
        }
        else
        {
            if (!(mDirtyBits[mXIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[mYIdx].GetTile(spriteX / 32, spriteY / 16)) && !(mDirtyBits[3].GetTile(spriteX / 32, spriteY / 16)))
            {
                continue;
            }
            layer = Layer::eLayer_1;
        }

        OrderingTable_Add_498A80(OtLayer(ppOt, layer), &pSpriteTPage->mSprt.mBase.header);
        OrderingTable_Add_498A80(OtLayer(ppOt, layer), &pSpriteTPage->mTPage.mBase);
    }

    sub_406FF0();

    for (s32 i = 0; i < 20; i++)
    {
        mDirtyBits[mXIdx].mData[i] |= mDirtyBits[3].mData[i];
    }

    memset(&mDirtyBits[3], 0, sizeof(mDirtyBits[3]));
    return;
}

void ScreenManager::VScreenChanged()
{
    // Empty
}

} // namespace AO
