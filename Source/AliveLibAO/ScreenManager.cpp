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

ALIVE_VAR(1, 0x4FF7C8, ScreenManager*, pScreenManager_4FF7C8, nullptr);
ALIVE_ARY(1, 0x4FC8A8, SprtTPage, 300, sSpriteTPageBuffer_4FC8A8, {});

Camera* Camera::ctor_4446E0()
{
    field_0_array.ctor_4043E0(10);
    field_30_flags &= ~1u;
    field_C_ppBits = nullptr;
    return this;
}

void Camera::dtor_444700()
{
    ResourceManager::FreeResource_455550(field_C_ppBits);

    for (s32 i = 0; i < field_0_array.Size(); i++)
    {
        u8** ppRes = field_0_array.ItemAt(i);
        if (!ppRes)
        {
            break;
        }

        ResourceManager::FreeResource_455550(ppRes);
        i = field_0_array.RemoveAt(i);
    }

    field_0_array.dtor_404440();
}


void CC Camera::On_Loaded_4447A0(Camera* pThis)
{
    pThis->field_30_flags |= 1u;
    pThis->field_C_ppBits = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Bits, pThis->field_10_resId, 1, 0);
}

void ScreenManager::MoveImage_406C40()
{
    PSX_RECT rect = {};
    rect.x = field_20_upos;
    rect.y = field_22_vpos;
    rect.h = 240;
    rect.w = 640;
    PSX_MoveImage_4961A0(&rect, 0, 0);
}

void ScreenManager::DecompressCameraToVRam_407110(u16** ppBits)
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

            rect.x = field_20_upos + xpos;
            rect.y = field_22_vpos;

            // TODO: Actually 16bit but must be uploaded as 8bit ??
            IRenderer::GetRenderer()->Upload(IRenderer::BitDepth::e8Bit, rect, reinterpret_cast<u8*>(pIter));

            // To next slice
            pIter += (slice_len / sizeof(s16));
        }

        ResourceManager::FreeResource_455550(pRes);

        field_58_20x16_dirty_bits[0] = {};
        field_58_20x16_dirty_bits[1] = {};
        field_58_20x16_dirty_bits[2] = {};
        field_58_20x16_dirty_bits[3] = {};
    }
}

void ScreenManager::InvalidateRect_406CC0(s32 x, s32 y, s32 width, s32 height)
{
    InvalidateRect_406E40(x, y, width, height, field_2E_idx);
}

ScreenManager* ScreenManager::ctor_406830(u8** ppBits, FP_Point* pCameraOffset)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA230);

    field_10_pCamPos = pCameraOffset;

    mFlags.Set(Options::eSurviveDeathReset_Bit9);
    mFlags.Set(Options::eUpdateDuringCamSwap_Bit10);

    Init_4068A0(ppBits);
    return this;
}

void ScreenManager::Init_4068A0(u8** ppBits)
{
    field_36_flags |= 1;

    field_4_typeId = Types::eScreenManager_4;

    field_14_xpos = 184;
    field_16_ypos = 120;
    field_20_upos = 0;
    field_22_vpos = 272;
    field_24_cam_width = 640;
    field_26_cam_height = 240;

    Vram_alloc_explicit_4507F0(0, 272, 640, 512);
    DecompressCameraToVRam_407110(reinterpret_cast<u16**>(ppBits));

    field_18_screen_sprites = &sSpriteTPageBuffer_4FC8A8[0];

    s16 xpos = 0;
    s16 ypos = 0;
    for (s32 i = 0; i < 300; i++)
    {
        SprtTPage* pItem = &field_18_screen_sprites[i];
        Sprt_Init(&pItem->mSprt);
        SetRGB0(&pItem->mSprt, 128, 128, 128);
        SetXY0(&pItem->mSprt, xpos, ypos);

        pItem->mSprt.field_14_w = 32;
        pItem->mSprt.field_16_h = 16;

        s32 u0 = field_20_upos + 32 * (i % 20);
        s32 v0 = field_22_vpos + 16 * (i / 20);
        s32 tpage = ScreenManager::GetTPage(TPageMode::e16Bit_2, TPageAbr::eBlend_0, &u0, &v0);

        tpage |= 0x8000;

        Init_SetTPage_495FB0(&pItem->mTPage, 0, 0, tpage);

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
        memset(&field_58_20x16_dirty_bits[i], 0, sizeof(field_58_20x16_dirty_bits[0]));
    }

    field_2E_idx = 2;
    field_30_y_idx = 1;
    field_32_x_idx = 0;
}


BaseGameObject* ScreenManager::VDestructor(s32 flags)
{
    return vdtor_407290(flags);
}
void ScreenManager::UnsetDirtyBits_FG1_406EF0()
{
    memset(&field_58_20x16_dirty_bits[4], 0, sizeof(this->field_58_20x16_dirty_bits[4]));
    memset(&field_58_20x16_dirty_bits[5], 0, sizeof(this->field_58_20x16_dirty_bits[5]));
}

void ScreenManager::InvalidateRect_406E40(s32 x, s32 y, s32 width, s32 height, s32 idx)
{
    x = std::max(x, 0);
    y = std::max(y, 0);

    width = std::min(width, 639);
    height = std::min(height, 239);

    for (s32 tileX = x / 32; tileX <= width / 32; tileX++)
    {
        for (s32 tileY = y / 16; tileY <= height / 16; tileY++)
        {
            field_58_20x16_dirty_bits[idx].SetTile(tileX, tileY, true);
        }
    }
}

void ScreenManager::InvalidateRect_Layer3_406F20(s32 x, s32 y, s32 width, s32 height)
{
    InvalidateRect_406E40(x, y, width, height, 3);
}


void ScreenManager::InvalidateRect_406D80(s32 x, s32 y, s32 width, s32 height, s32 idx)
{
    InvalidateRect_406E40(x, y, width, height, idx + 4);
}

void ScreenManager::VScreenChanged()
{
    // Empty
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
    return PSX_getTPage_4965D0(tp, abr, clampedXPos, clampedYPos);
}

void ScreenManager::VRender(PrimHeader** ppOt)
{
    VRender_406A60(ppOt);
}

void ScreenManager::VRender_406A60(PrimHeader** ppOt)
{
    if (!(field_36_flags & 1)) // Render enabled flag ?
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
        SprtTPage* pSpriteTPage = &field_18_screen_sprites[i];

        const s32 spriteX = pSpriteTPage->mSprt.mBase.vert.x;
        const s32 spriteY = pSpriteTPage->mSprt.mBase.vert.y;

        Layer layer = Layer::eLayer_0;
        if (field_58_20x16_dirty_bits[4].GetTile(spriteX / 32, spriteY / 16))
        {
            if (!(field_58_20x16_dirty_bits[field_2E_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[field_30_y_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[field_32_x_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[3].GetTile(spriteX / 32, spriteY / 16)))
            {
                continue;
            }
            layer = Layer::eLayer_FG1_37;
        }
        else if (field_58_20x16_dirty_bits[5].GetTile(spriteX / 32, spriteY / 16))
        {
            if (!(field_58_20x16_dirty_bits[field_2E_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[field_30_y_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[field_32_x_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[3].GetTile(spriteX / 32, spriteY / 16)))
            {
                continue;
            }
            layer = Layer::eLayer_FG1_Half_18;
        }
        else
        {
            if (!(field_58_20x16_dirty_bits[field_32_x_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[field_30_y_idx].GetTile(spriteX / 32, spriteY / 16)) && !(field_58_20x16_dirty_bits[3].GetTile(spriteX / 32, spriteY / 16)))
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
        field_58_20x16_dirty_bits[field_32_x_idx].mData[i] |= field_58_20x16_dirty_bits[3].mData[i];
    }

    memset(&field_58_20x16_dirty_bits[3], 0, sizeof(field_58_20x16_dirty_bits[3]));
    return;
}

void ScreenManager::sub_406FF0()
{
    // NOTE: The algorithm calling Add_Dirty_Area_48D910 has not been implemented
    // as its not actually used.

    field_32_x_idx = field_30_y_idx;
    field_30_y_idx = field_2E_idx;
    field_2E_idx = (field_2E_idx + 1) % 3;
    memset(
        &field_58_20x16_dirty_bits[field_2E_idx],
        0,
        sizeof(field_58_20x16_dirty_bits[field_2E_idx]));
}

ScreenManager* ScreenManager::vdtor_407290(s32 flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

} // namespace AO
