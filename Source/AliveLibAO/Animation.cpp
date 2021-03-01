#include "stdafx_ao.h"
#include "Animation.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "Primitives.hpp"
#include "Sys_common.hpp"
#include "ResourceManager.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "Slig.hpp"
#include "Dove.hpp"
#include "Bullet.hpp"
#include "BulletShell.hpp"
#include "Dove.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Particle.hpp"
#include "Compression.hpp"
#include "Abe.hpp"
#include "Throwable.hpp"
#include "Collisions.hpp"
#include "Slog.hpp"
#include "Blood.hpp"
#include "Renderer/IRenderer.hpp"

// Fix pollution from windows.h
#undef min
#undef max

namespace AO {

EXPORT short* CC Animation_OnFrame_Slig_46F610(void* pObj, __int16* pData)
{
    auto pSlig = static_cast<Slig*>(pObj);
    if (pSlig->field_8_update_delay != 0)
    {
        return pData + 2;
    }

    BulletType bulletType = BulletType::ePossessedSlig_0;
    if (pSlig->field_10A_flags.Get(Flags_10A::e10A_Bit2_bPossesed))
    {
        pSlig->field_254_prevent_depossession |= 1u;
        bulletType = BulletType::ePossessedSlig_0;
    }
    else
    {
        bulletType = BulletType::eNormalBullet_1;
    }

    const FP xOff = pSlig->field_BC_sprite_scale * FP_FromInteger(pData[0]);
    const FP yOff = pSlig->field_BC_sprite_scale * FP_FromInteger(pData[1]);
    if (pSlig->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        auto pBullet = ao_new<Bullet>();
        if (pBullet)
        {
            pBullet->ctor_409380(
                pSlig,
                bulletType,
                pSlig->field_A8_xpos,
                yOff + pSlig->field_AC_ypos,
                FP_FromInteger(-640),
                0,
                pSlig->field_BC_sprite_scale,
                0);
        }

        New_ShootingFire_Particle_419720(
            pSlig->field_A8_xpos - xOff,
            pSlig->field_AC_ypos + yOff,
            1,
            pSlig->field_BC_sprite_scale);

        auto pBulletShell = ao_new<BulletShell>();
        if (pBulletShell)
        {
            pBulletShell->ctor_462790(
                pSlig->field_A8_xpos,
                pSlig->field_AC_ypos + yOff,
                0,
                pSlig->field_BC_sprite_scale);
        }
    }
    else
    {
        auto pBullet = ao_new<Bullet>();
        if (pBullet)
        {
            pBullet->ctor_409380(
                pSlig,
                bulletType,
                pSlig->field_A8_xpos,
                yOff + pSlig->field_AC_ypos,
                FP_FromInteger(640),
                0,
                pSlig->field_BC_sprite_scale,
                0);
        }

        New_ShootingFire_Particle_419720(
            pSlig->field_A8_xpos + xOff,
            pSlig->field_AC_ypos + yOff,
            0,
            pSlig->field_BC_sprite_scale);

        auto pBulletShell = ao_new<BulletShell>();
        if (pBulletShell)
        {
            pBulletShell->ctor_462790(
                pSlig->field_A8_xpos,
                pSlig->field_AC_ypos + yOff,
                1,
                pSlig->field_BC_sprite_scale);
        }
    }

    if (pSlig->field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        SFX_Play_43AD70(SoundEffect::SligShoot_6, 85);
    }
    else
    {
        SFX_Play_43AD70(SoundEffect::SligShoot_6, 0);
    }

    Event_Broadcast_417220(kEvent_2, pSlig);
    Event_Broadcast_417220(kEvent_14, pSlig);

    Dove::All_FlyAway_40F390();

    return pData + 2;
}

EXPORT short* CC Animation_OnFrame_ZBallSmacker_41FB00(void* pObj, short* pData);

EXPORT short *CC Slog_OnFrame_471FD0(void* pObj, __int16* pData)
{
    auto pSlog = static_cast<Slog*>(pObj);
    if (pSlog->field_10C_pTarget)
    {
        PSX_RECT targetRect = {};
        pSlog->field_10C_pTarget->VGetBoundingRect(&targetRect, 1);

        PSX_RECT slogRect = {};
        pSlog->VGetBoundingRect(&slogRect, 1);

        if (RectsOverlap(slogRect, targetRect))
        {
            if (pSlog->field_10C_pTarget->field_BC_sprite_scale == pSlog->field_BC_sprite_scale && !pSlog->field_110)
            {
                if (pSlog->field_10C_pTarget->VTakeDamage(pSlog))
                {
                    FP blood_xpos = {};
                    if (pSlog->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        blood_xpos = pSlog->field_A8_xpos - (pSlog->field_BC_sprite_scale * FP_FromInteger(pData[0]));
                    }
                    else
                    {
                        blood_xpos = pSlog->field_A8_xpos + (pSlog->field_BC_sprite_scale * FP_FromInteger(pData[0]));
                    }

                    const FP blood_ypos = (pSlog->field_BC_sprite_scale * FP_FromInteger(pData[1])) + pSlog->field_AC_ypos;

                    auto pBlood = ao_new<Blood>();
                    if (pBlood)
                    {
                        pBlood->ctor_4072B0(
                            blood_xpos,
                            blood_ypos - FP_FromInteger(8),
                            (pSlog->field_B4_velx * FP_FromInteger(2)),
                            FP_FromInteger(0),
                            pSlog->field_BC_sprite_scale,
                            50);
                    }

                    pSlog->field_110 = 1;

                    SFX_Play_43AD70(SoundEffect::SlogBite_39, 0);
                }
            }
        }
    }

    return pData + 2;
}

const FP_Point kAbeVelTable_4C6608[6] =
{
    { FP_FromInteger(3),    FP_FromInteger(-14) },
    { FP_FromInteger(10),   FP_FromInteger(-10) },
    { FP_FromInteger(15),   FP_FromInteger(-8) },
    { FP_FromInteger(10),   FP_FromInteger(3) },
    { FP_FromInteger(10),   FP_FromInteger(-4) },
    { FP_FromInteger(4),    FP_FromInteger(-3) }
};

EXPORT short* CC Abe_OnFrame_429E30(void* pObj, __int16* pData)
{
    auto pAbe = static_cast<Abe*>(pObj);

    FP xVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].field_0_x * pAbe->field_BC_sprite_scale;
    const FP yVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].field_4_y * pAbe->field_BC_sprite_scale;

    FP directed_x = {};
    if (sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        xVel = -xVel;
        directed_x = -(pAbe->field_BC_sprite_scale * FP_FromInteger(pData[0]));
    }
    else
    {
        directed_x = (pAbe->field_BC_sprite_scale * FP_FromInteger(pData[0]));
    }

    FP data_y = FP_FromInteger(pData[1]);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        pAbe->field_A8_xpos,
        pAbe->field_AC_ypos + data_y,
        pAbe->field_A8_xpos + directed_x,
        pAbe->field_AC_ypos + data_y,
        &pLine,
        &hitX,
        &hitY,
        pAbe->field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60))
    {
        directed_x = hitX - pAbe->field_A8_xpos;
        xVel = -xVel;
    }

    if (sActiveHero_507678->field_198_pThrowable)
    {
        sActiveHero_507678->field_198_pThrowable->field_A8_xpos = directed_x + sActiveHero_507678->field_A8_xpos;
        BaseThrowable* pThrowable = sActiveHero_507678->field_198_pThrowable;
        pThrowable->field_AC_ypos = (pAbe->field_BC_sprite_scale * data_y) + sActiveHero_507678->field_AC_ypos;
        pThrowable->VThrow(xVel, yVel);
        pThrowable->field_BC_sprite_scale = pAbe->field_BC_sprite_scale;
        sActiveHero_507678->field_198_pThrowable = nullptr;
    }
    return pData + 2;
}

TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[] = { Abe_OnFrame_429E30 };
TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[] = {Animation_OnFrame_Slig_46F610};
TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[] = { Slog_OnFrame_471FD0 };
TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[] = { Animation_OnFrame_ZBallSmacker_41FB00 };

void Animation::vDecode()
{
    VDecode_403550();
}

static IRenderer::BitDepth AnimFlagsToBitDepth(const BitField32<AnimFlags>& flags)
{
    if (flags.Get(AnimFlags::eBit14_Is16Bit))
    {
        return IRenderer::BitDepth::e16Bit;
    }
    else if (flags.Get(AnimFlags::eBit13_Is8Bit))
    {
        return IRenderer::BitDepth::e8Bit;
    }
    return IRenderer::BitDepth::e4Bit;
}

void Animation::UploadTexture(const FrameHeader* pFrameHeader, const PSX_RECT& vram_rect, short width_bpp_adjusted)
{
    IRenderer& renderer = *IRenderer::GetRenderer();
    switch (pFrameHeader->field_7_compression_type)
    {
    case CompressionType::eType_0_NoCompression:
        renderer.Upload(AnimFlagsToBitDepth(field_4_flags), vram_rect, reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2));
        break;

    case CompressionType::eType_1_NotUsed:
        if (EnsureDecompressionBuffer())
        {
            // TODO: Refactor structure to get pixel data/remove casts
            Decompress_Type_1_403150(
                (BYTE*)&pFrameHeader[1],
                *field_24_dbuf,
                *(DWORD*)&pFrameHeader->field_8_width2,
                2 * pFrameHeader->field_5_height * width_bpp_adjusted);
            renderer.Upload(AnimFlagsToBitDepth(field_4_flags), vram_rect, *field_24_dbuf);
        }
        break;

    case CompressionType::eType_2_ThreeToFourBytes:
        if (EnsureDecompressionBuffer())
        {
            Decompress_Type_2_403390(
                (BYTE*)&pFrameHeader[1],
                *field_24_dbuf,
                2 * pFrameHeader->field_5_height * width_bpp_adjusted);
            renderer.Upload(AnimFlagsToBitDepth(field_4_flags), vram_rect, *field_24_dbuf);
        }
        break;

    case CompressionType::eType_3_RLE_Blocks:
        if (EnsureDecompressionBuffer())
        {
            // TODO: Refactor structure to get pixel data/remove casts
            Decompress_Type_3_4031E0(
                (BYTE*)&pFrameHeader[1],
                *field_24_dbuf,
                *(DWORD*)&pFrameHeader->field_8_width2,
                2 * pFrameHeader->field_5_height * width_bpp_adjusted);
            renderer.Upload(AnimFlagsToBitDepth(field_4_flags), vram_rect, *field_24_dbuf);
        }
        break;

    case CompressionType::eType_4_RLE:
    case CompressionType::eType_5_RLE:
        if (EnsureDecompressionBuffer())
        {
            // TODO: Refactor structure to get pixel data/remove casts
            Decompress_Type_4_5_461770(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
            renderer.Upload(AnimFlagsToBitDepth(field_4_flags), vram_rect, *field_24_dbuf);
        }
        break;

    default:
        LOG_ERROR("Unknown compression type " << static_cast<int>(pFrameHeader->field_7_compression_type));
        ALIVE_FATAL("Unknown compression type");
        break;
    }
}

void Animation::VDecode_403550()
{
    if (!field_20_ppBlock || !*field_20_ppBlock)
    {
        return;
    }

    AnimationHeader* pAnimationHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[field_18_frame_table_offset]);
    if (pAnimationHeader->field_2_num_frames == 1 && field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        return;
    }

    if (field_4_flags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        // Loop backwards
        const __int16 prevFrameNum = --field_92_current_frame;
        field_E_frame_change_counter = static_cast<short>(field_10_frame_delay);

        if (prevFrameNum < pAnimationHeader->field_4_loop_start_frame)
        {
            if (field_4_flags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop to last frame
                field_92_current_frame = pAnimationHeader->field_2_num_frames - 1;

            }
            else
            {
                // Stay on current frame
                field_E_frame_change_counter = 0;
                field_92_current_frame = prevFrameNum + 1;
            }
        }

        // Is first (last since running backwards) frame?
        if (field_92_current_frame == 0)
        {
            field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
        }
        else
        {
            field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
        }
    }
    else
    {
        // Loop forwards
        const __int16 nextFrameNum = ++field_92_current_frame;
        field_E_frame_change_counter = static_cast<short>(field_10_frame_delay);

        // Animation reached end point
        if (nextFrameNum >= pAnimationHeader->field_2_num_frames)
        {
            if (field_4_flags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop back to loop start frame
                field_92_current_frame = pAnimationHeader->field_4_loop_start_frame;
            }
            else
            {
                // Stay on current frame
                field_92_current_frame = nextFrameNum - 1;
                field_E_frame_change_counter = 0;
            }

            field_4_flags.Set(AnimFlags::eBit12_ForwardLoopCompleted);
        }

        // Is last frame ?
        if (field_92_current_frame == pAnimationHeader->field_2_num_frames - 1)
        {
            field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
        }
        else
        {
            field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
        }
    }

    if (field_4_flags.Get(AnimFlags::eBit11_bToggle_Bit10))
    {
        field_4_flags.Toggle(AnimFlags::eBit10_alternating_flag);
    }

    const FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader_403A00(-1); // -1 = use current frame
    if (pFrameInfoHeader->field_6_count > 0)
    {
        if (field_1C_fn_ptr_array)
        {
            FrameInfoHeader* pFrameHeaderCopy = this->Get_FrameHeader_403A00(-1);

            // This data can be an array of DWORD's + other data up to field_6_count
            // which appears AFTER the usual data.

            // TODO: Should be typed to short* ??
            const DWORD* pCallBackData = reinterpret_cast<const DWORD*>(&pFrameHeaderCopy->field_8_data.points[3]);
            for (int i = 0; i < pFrameHeaderCopy->field_6_count; i++)
            {
                const auto pFnCallBack = field_1C_fn_ptr_array[*pCallBackData];
                if (!pFnCallBack)
                {
                    break;
                }
                pCallBackData++; // Skip the array index
                // Pass the data pointer into the call back which will then read and skip any extra data
                pCallBackData += *pFnCallBack(field_94_pGameObj, (short*)pCallBackData);
            }
        }
    }

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

    // No VRAM allocation
    if (field_84_vram_rect.w <= 0)
    {
        return;
    }

    short width_bpp_adjusted = 0;
    if (field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
    {
        // 8 bit, divided by half
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 3) / 2) & ~1;
    }
    else if (field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
    {
        // 16 bit, only multiple of 2 rounding
        width_bpp_adjusted = (pFrameHeader->field_4_width + 1) & ~1;
    }
    else
    {
        // 4 bit divide by quarter
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 7) / 4) & ~1;
    }

    PSX_RECT vram_rect =
    {
        field_84_vram_rect.x,
        field_84_vram_rect.y,
        width_bpp_adjusted,
        pFrameHeader->field_5_height,
    };

    // Clamp width
    if (vram_rect.w > field_84_vram_rect.w)
    {
        vram_rect.w = field_84_vram_rect.w;
    }

    // Clamp height
    if (pFrameHeader->field_5_height > field_84_vram_rect.h)
    {
        vram_rect.h = field_84_vram_rect.h;
    }

    UploadTexture(pFrameHeader, vram_rect, width_bpp_adjusted);
}

void Animation::vRender(int xpos, int ypos, PrimHeader** ppOt, __int16 width, __int16 height)
{
    VRender_403AE0(xpos, ypos, ppOt, width, height);
}

void Animation::VRender_403AE0(int xpos, int ypos, PrimHeader** ppOt, __int16 width, __int16 height)
{
    if (field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        BYTE** ppBlock = field_20_ppBlock;
        if (ppBlock)
        {
            const BYTE* pBlock = *ppBlock;
            const FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader_403A00(-1);
            const FrameHeader* pFrameHeader = (const FrameHeader*)&pBlock[pFrameInfoHeader->field_0_frame_header_offset];

            FP frame_width_fixed = {};
            FP frame_height_fixed = {};
            if (width)
            {
                frame_width_fixed = FP_FromInteger(width);
                frame_height_fixed = FP_FromInteger(height);
            }
            else
            {
                frame_width_fixed = FP_FromInteger(PCToPsxX(pFrameHeader->field_4_width, 20));
                frame_height_fixed = FP_FromInteger(pFrameHeader->field_5_height);
            }

            FP xOffSet_fixed = {};
            FP yOffset_fixed = {};
            if (field_4_flags.Get(AnimFlags::eBit20_use_xy_offset))
            {
                xOffSet_fixed = FP_FromInteger(0);
                yOffset_fixed = FP_FromInteger(0);
            }
            else
            {
                xOffSet_fixed = FP_FromInteger(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x);
                yOffset_fixed = FP_FromInteger(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y);
            }

            TPageMode textureMode = {};
            if (field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
            {
                textureMode = TPageMode::e8Bit_1;
            }
            else if (field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
            {
                textureMode = TPageMode::e16Bit_2;
            }
            else
            {
                textureMode = TPageMode::e4Bit_0;
            }

            short tPageY = 0;
            if (field_4_flags.Get(AnimFlags::eBit10_alternating_flag) || field_84_vram_rect.y >= 256)
            {
                tPageY = 256;
            }
            else
            {
                tPageY = 0;
            }

            Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_504C78.field_A_buffer_index];
            PolyFT4_Init(pPoly);
            Poly_Set_SemiTrans_498A40(&pPoly->mBase.header, field_4_flags.Get(AnimFlags::eBit15_bSemiTrans));
            Poly_Set_Blending_498A00(&pPoly->mBase.header, field_4_flags.Get(AnimFlags::eBit16_bBlending));

            SetRGB0(pPoly, field_8_r, field_9_g, field_A_b);
            SetTPage(pPoly, static_cast<short>(PSX_getTPage_4965D0(textureMode, field_B_render_mode, field_84_vram_rect.x, tPageY)));
            SetClut(pPoly, static_cast<short>(PSX_getClut_496840(field_8C_pal_vram_xy.field_0_x, field_8C_pal_vram_xy.field_2_y)));

            BYTE u1 = field_84_vram_rect.x & 63;
            if (textureMode == TPageMode::e8Bit_1)
            {
                u1 *= 2;
            }
            else if (textureMode == TPageMode::e4Bit_0)
            {
                u1 *= 4;
            }
            else
            {
                // 16 bit
            }

            const BYTE v0 = static_cast<BYTE>(field_84_vram_rect.y);
            const BYTE u0 = pFrameHeader->field_4_width + u1 - 1;
            const BYTE v1 = pFrameHeader->field_5_height + v0 - 1;

            if (field_14_scale != FP_FromInteger(1))
            {
                // Apply scale to x/y pos
                frame_height_fixed = (frame_height_fixed * field_14_scale);
                frame_width_fixed = (frame_width_fixed * field_14_scale);

                // Apply scale to x/y offset
                xOffSet_fixed = (xOffSet_fixed * field_14_scale);
                yOffset_fixed = (yOffset_fixed * field_14_scale) - FP_FromInteger(1);
            }

            int polyXPos;
            if (field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                SetUV0(pPoly, u0, v0);
                SetUV1(pPoly, u1, v0);
                SetUV2(pPoly, u0, v1);
                SetUV3(pPoly, u1, v1);

                polyXPos = xpos - FP_GetExponent(xOffSet_fixed + frame_width_fixed + FP_FromDouble(0.499));
            }
            else
            {
                SetUV0(pPoly, u1, v0);
                SetUV1(pPoly, u0, v0);
                SetUV2(pPoly, u1, v1);
                SetUV3(pPoly, u0, v1);

                polyXPos = xpos + FP_GetExponent(xOffSet_fixed + FP_FromDouble(0.499));
            }

            const short polyYPos = static_cast<short>(ypos + FP_GetExponent((yOffset_fixed + FP_FromDouble(0.499))));
            const short xConverted = static_cast<short>(PsxToPCX(polyXPos));
            const short width_adjusted = FP_GetExponent(PsxToPCX(frame_width_fixed) - FP_FromDouble(0.501)) + xConverted;
            const short height_adjusted = FP_GetExponent(frame_height_fixed - FP_FromDouble(0.501)) + polyYPos;

            SetXY0(pPoly, xConverted, polyYPos);
            SetXY1(pPoly, width_adjusted, polyYPos);
            SetXY2(pPoly, xConverted, height_adjusted);
            SetXY3(pPoly, width_adjusted, height_adjusted);

            SetPrimExtraPointerHack(pPoly, nullptr);
            OrderingTable_Add_498A80(OtLayer(ppOt, field_C_layer), &pPoly->mBase.header);
        }
    }
}

void Animation::vCleanUp()
{
    VCleanUp_403F40();
}

void Animation::VCleanUp_403F40()
{
    if (field_4_flags.Get(AnimFlags::eBit17_bFreeResource))
    {
        ResourceManager::FreeResource_455550(field_20_ppBlock);
    }

    gObjList_animations_505564->Remove_Item(this);
    

    // inlined Animation_Pal_Free ?
    if (field_84_vram_rect.w > 0)
    {
        Vram_free_450CE0({ field_84_vram_rect.x, field_84_vram_rect.y }, { field_84_vram_rect.w, field_84_vram_rect.h });
    }

    if (field_90_pal_depth > 0)
    {
        Pal_Free_447870(field_8C_pal_vram_xy, field_90_pal_depth);
    }

    ResourceManager::FreeResource_455550(field_24_dbuf);

}

bool Animation::EnsureDecompressionBuffer()
{
    if (!field_24_dbuf)
    {
        field_24_dbuf = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
    }
    return field_24_dbuf != nullptr;
}

void CC AnimationBase::AnimateAll_4034F0(DynamicArrayT<AnimationBase>* pAnimList)
{
    for (int i = 0; i < pAnimList->Size(); i++)
    {
        auto pAnim = pAnimList->ItemAt(i);
        if (!pAnim)
        {
            break;
        }

        if (pAnim->field_4_flags.Get(AnimFlags::eBit2_Animate))
        {
            if (pAnim->field_E_frame_change_counter > 0)
            {
                pAnim->field_E_frame_change_counter--;
                if (pAnim->field_E_frame_change_counter == 0)
                {
                    pAnim->vDecode();
                }
            }
        }
    }
}

signed __int16 Animation::Set_Animation_Data_402A40(int frameTableOffset, BYTE** pAnimRes)
{
    if (pAnimRes)
    {
        field_20_ppBlock = pAnimRes;
    }

    if (!field_20_ppBlock)
    {
        return 0;
    }

    field_18_frame_table_offset = frameTableOffset;

    AnimationHeader* pAnimationHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[field_18_frame_table_offset]);
    field_10_frame_delay = pAnimationHeader->field_0_fps;

    field_4_flags.Clear(AnimFlags::eBit12_ForwardLoopCompleted);
    field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
    field_4_flags.Clear(AnimFlags::eBit8_Loop);

    if (pAnimationHeader->field_6_flags & AnimationHeader::eLoopFlag)
    {
        field_4_flags.Set(AnimFlags::eBit8_Loop);
    }

    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    vDecode();

    // Reset to start frame
    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    return 1;
}

void Animation::SetFrame_402AC0(__int16 newFrame)
{
    if (field_20_ppBlock)
    {
        if (newFrame == -1)
        {
            newFrame = 0;
        }

        AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset); // TODO: Make getting offset to animation header cleaner

        if (newFrame > pHead->field_2_num_frames)
        {
            newFrame = pHead->field_2_num_frames;
        }

        field_E_frame_change_counter = 1;
        field_92_current_frame = newFrame - 1;
    }
}

signed __int16 Animation::Init_402D20(int frameTableOffset, DynamicArray* /*animList*/, BaseGameObject* pGameObj, unsigned __int16 maxW, unsigned __int16 maxH, BYTE** ppAnimData, unsigned __int8 bAllocateVRam, signed int b_StartingAlternationState, char bEnable_flag10_alternating)
{
    field_4_flags.Raw().all = 0; // TODO extra - init to 0's first - this may be wrong if any bits are explicitly set before this is called

    field_18_frame_table_offset = frameTableOffset;
    field_20_ppBlock = ppAnimData;
    field_1C_fn_ptr_array = nullptr;
    field_24_dbuf = nullptr;

    if (!ppAnimData)
    {
        return 0;
    }

    field_94_pGameObj = pGameObj;

    AnimationHeader* pHeader = reinterpret_cast<AnimationHeader*>(&(*ppAnimData)[frameTableOffset]);

    field_10_frame_delay = pHeader->field_0_fps;
    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;
    field_B_render_mode = TPageAbr::eBlend_0;
    field_8_r = 0;
    field_9_g = 0;
    field_A_b = 0;
    field_14_scale = FP_FromInteger(1);
   
    FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader_403A00(0);
    BYTE* pAnimData = *ppAnimData;

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

    BYTE* pClut = &pAnimData[pFrameHeader->field_0_clut_offset];

    field_4_flags.Clear(AnimFlags::eBit1);
    field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    field_4_flags.Clear(AnimFlags::eBit6_FlipY);
    field_4_flags.Clear(AnimFlags::eBit7_SwapXY);
    field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_4_flags.Set(AnimFlags::eBit3_Render);

    field_4_flags.Set(AnimFlags::eBit8_Loop, pHeader->field_6_flags & AnimationHeader::eLoopFlag);

    field_4_flags.Set(AnimFlags::eBit10_alternating_flag, bEnable_flag10_alternating & 1);
    field_4_flags.Set(AnimFlags::eBit11_bToggle_Bit10, b_StartingAlternationState & 1);

    field_4_flags.Clear(AnimFlags::eBit14_Is16Bit);
    field_4_flags.Clear(AnimFlags::eBit13_Is8Bit);
    field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_4_flags.Set(AnimFlags::eBit16_bBlending);
    field_4_flags.Set(AnimFlags::eBit17_bFreeResource);

    if (bAllocateVRam)
    {
        field_84_vram_rect.w = 0;
    }
    
    field_90_pal_depth = 0;
    
    int vram_width = 0;
    short pal_depth = 0;
    if (pFrameHeader->field_6_colour_depth == 4)
    {
        vram_width = (maxW % 2) + (maxW / 2);
        pal_depth = 16;
    }
    else if (pFrameHeader->field_6_colour_depth == 8)
    {
        vram_width = maxW;

        if (*(WORD*)pClut == 64) // CLUT entry count/len
        {
            pal_depth = 64;
        }
        else
        {
            pal_depth = 256;
        }

        field_4_flags.Set(AnimFlags::eBit13_Is8Bit);
    }
    else if (pFrameHeader->field_6_colour_depth == 16)
    {
        vram_width = maxW * 2;
        field_4_flags.Set(AnimFlags::eBit14_Is16Bit);
    }
    else
    {
        return 0;
    }

    int bVramAllocOK = 1;
    if (bAllocateVRam)
    {
        bVramAllocOK = vram_alloc_450B20(maxW, maxH, pFrameHeader->field_6_colour_depth, &field_84_vram_rect);
    }

    int bPalAllocOK = 1;
    if (pal_depth > 0 && bVramAllocOK)
    {
        PSX_RECT palVRamRect = {};
        bPalAllocOK = Pal_Allocate_4476F0(&palVRamRect, pal_depth);

        field_8C_pal_vram_xy.field_0_x = palVRamRect.x;
        field_8C_pal_vram_xy.field_2_y = palVRamRect.y;

        palVRamRect.w = pal_depth;
        palVRamRect.h = 1;

        field_90_pal_depth = pal_depth;

        if (bVramAllocOK && bPalAllocOK)
        {
            PSX_LoadImage16_4962A0(&palVRamRect, pClut + 4); // +4 Skip len, load pal
        }
    }

    const bool bOk = bVramAllocOK && bPalAllocOK;
    if (!bOk)
    {
        return 0;
    }

    field_28_dbuf_size = maxH * (vram_width + 3);

    if (pFrameHeader->field_7_compression_type != CompressionType::eType_0_NoCompression)
    {
        const DWORD id = ResourceManager::Get_Header_455620(field_20_ppBlock)->field_C_id;
        field_24_dbuf = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_DecompressionBuffer, id, field_28_dbuf_size);
        if (!field_24_dbuf)
        {
            return 0;
        }
    }

    // NOTE: OG bug or odd compiler code gen? Why isn't it using the passed in list which appears to always be this anyway ??
    const auto result = gObjList_animations_505564->Push_Back(this);
    if (!result)
    {
        LOG_ERROR("gObjList_animations_505564->Push_Back(this) returned 0 but shouldn't");
        return 0;
    }
    
    // Get first frame decompressed/into VRAM
    vDecode();

    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    return result;
}

__int16 Animation::Get_Frame_Count_403540()
{
    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset);  // TODO: Make getting offset to animation header cleaner
    return pHead->field_2_num_frames;
}

ALIVE_VAR(1, 0x4BA090, FrameInfoHeader, sBlankFrameInfoHeader_4BA090, {});

FrameInfoHeader* Animation::Get_FrameHeader_403A00(int frame)
{
    if (!field_20_ppBlock)
    {
        return nullptr;
    }

    if (frame < -1 || frame == -1)
    {
        frame = field_92_current_frame != -1 ? field_92_current_frame : 0;
    }

    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset);  // TODO: Make getting offset to animation header cleaner
    DWORD frameOffset = pHead->mFrameOffsets[frame];

    FrameInfoHeader* pFrame = reinterpret_cast<FrameInfoHeader*>(*field_20_ppBlock + frameOffset);

    // Never seen this get hit, perhaps some sort of PSX specific check as addresses have to be aligned there?
    // TODO: Remove it in the future when proven to be not required?
#if defined(_MSC_VER) && !defined(_WIN64)
    if (reinterpret_cast<DWORD>(pFrame) & 3)
    {
        FrameInfoHeader* Unknown = &sBlankFrameInfoHeader_4BA090;
        return Unknown;
    }
#endif

    return pFrame;
}

void Animation::LoadPal_403090(BYTE** pPalData, int palOffset)
{
    if (pPalData)
    {
        PSX_RECT rect = {};
        BYTE* pPalDataOffset = &(*pPalData)[palOffset];
        switch (field_90_pal_depth)
        {
        case 16:
            rect.x = field_8C_pal_vram_xy.field_0_x;
            rect.y = field_8C_pal_vram_xy.field_2_y;
            rect.w = 16;
            break;

        case 64:
            rect.x = field_8C_pal_vram_xy.field_0_x;
            rect.y = field_8C_pal_vram_xy.field_2_y;
            rect.w = 64;
            break;

        case 256:
            rect.x = field_8C_pal_vram_xy.field_0_x;
            rect.y = field_8C_pal_vram_xy.field_2_y;
            rect.w = 256;
            break;

        default:
            return;
        }
        rect.h = 1;
        PSX_LoadImage16_4962A0(&rect, pPalDataOffset + 4); // +4 skip len, load pal
    }
}


static void CC Poly_FT4_Get_Rect(PSX_RECT* pRect, const Poly_FT4* pPoly)
{
    if (PSX_Prim_Code_Without_Blending_Or_SemiTransparency(pPoly->mBase.header.rgb_code.code_or_pad) == PrimTypeCodes::ePolyFT4)
    {
        pRect->x = pPoly->mBase.vert.x;
        pRect->y = pPoly->mBase.vert.y;
        pRect->w = pPoly->mVerts[2].mVert.x;
        pRect->h = pPoly->mVerts[2].mVert.y;
    }
    else
    {
        pRect->h = 0;
        pRect->w = 0;
        pRect->y = 0;
        pRect->x = 0;
    }
}

EXPORT void Animation::Get_Frame_Rect_402B50(PSX_RECT* pRect)
{
    
    Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_504C78.field_A_buffer_index];
    if (!field_4_flags.Get(AnimFlags::eBit20_use_xy_offset))
    {
        Poly_FT4_Get_Rect(pRect, pPoly);
        return;
    }

    const auto min_x0_x1 = std::min(X0(pPoly), X1(pPoly));
    const auto min_x2_x3 = std::min(X2(pPoly), X3(pPoly));
    pRect->x = std::min(min_x0_x1, min_x2_x3);

    const auto max_x0_x1 = std::max(X0(pPoly), X1(pPoly));
    const auto max_x2_x3 = std::max(X2(pPoly), X3(pPoly));
    pRect->w = std::max(max_x0_x1, max_x2_x3);

    const auto min_y0_y1 = std::min(Y0(pPoly), Y1(pPoly));
    const auto min_y2_y3 = std::min(Y2(pPoly), Y3(pPoly));
    pRect->y = std::min(min_y0_y1, min_y2_y3);

    const auto max_y0_y1 = std::max(Y0(pPoly), Y1(pPoly));
    const auto max_y2_y3 = std::max(Y2(pPoly), Y3(pPoly));
    pRect->h = std::max(max_y0_y1, max_y2_y3);
}

EXPORT void Animation::Get_Frame_Width_Height_403E80(short* pWidth, short* pHeight)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader_403A00(-1);
    if (field_4_flags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Mode should never be used");
    }
    else
    {
        auto pHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameHeader->field_0_frame_header_offset]);
        *pWidth = pHeader->field_4_width;
        *pHeight = pHeader->field_5_height;
    }
}

EXPORT void Animation::Get_Frame_Offset_403EE0(short* pBoundingX, short* pBoundingY)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader_403A00(-1);
    *pBoundingX = pFrameHeader->field_8_data.offsetAndRect.mOffset.x;
    *pBoundingY = pFrameHeader->field_8_data.offsetAndRect.mOffset.y;
}

void AnimationUnknown::vCleanUp()
{
    VCleanUp2_404280();
}

void AnimationUnknown::VRender2(int xpos, int ypos, PrimHeader** ppOt)
{
    VRender2_403FD0(xpos, ypos, ppOt);
}

void AnimationUnknown::vRender(int /*xpos*/, int /*ypos*/, PrimHeader** /*pOt*/, __int16 /*width*/, __int16 /*height*/)
{
    // Empty @ 402A20
}

void AnimationUnknown::vDecode()
{
    // Empty @ 402A10
}

void AnimationUnknown::VCleanUp2_404280()
{
    field_68_anim_ptr = nullptr;
}

void AnimationUnknown::VRender2_403FD0(int xpos, int ypos, PrimHeader** ppOt)
{
    Poly_FT4* pPoly = &field_10_polys[gPsxDisplay_504C78.field_A_buffer_index];
    if (field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        // Copy from animation to local
        *pPoly = field_68_anim_ptr->field_2C_ot_data[gPsxDisplay_504C78.field_A_buffer_index];

        FrameInfoHeader* pFrameInfoHeader = field_68_anim_ptr->Get_FrameHeader_403A00(-1);

        FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_68_anim_ptr->field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

        int frameOffX = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x;
        int frameOffY = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y;
        int frameH = pFrameHeader->field_5_height;
        int frameW = pFrameHeader->field_4_width;

        if (field_6C_scale != FP_FromInteger(1))
        {
            frameH = FP_GetExponent(FP_FromInteger(frameH) * field_6C_scale);
            frameW = FP_GetExponent(FP_FromInteger(frameW) * field_6C_scale);
            frameOffX = FP_GetExponent(FP_FromInteger(frameOffX) * field_6C_scale);
            frameOffY = FP_GetExponent(FP_FromInteger(frameOffY) * field_6C_scale);
        }

        int polyX = 0;
        int polyY = 0;
        if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit7_SwapXY))
        {
            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit6_FlipY))
            {
                if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    polyX = xpos - frameOffY - frameH;
                }
                else
                {
                    polyX = frameOffY + xpos;
                }
                polyY = frameOffX + ypos;
            }
            else
            {
                if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    polyX = xpos - frameOffY - frameH;
                }
                else
                {
                    polyX = frameOffY + xpos;
                }
                polyY = ypos - frameOffX - frameW;
            }
        }
        else if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit6_FlipY))
        {
            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                polyX = xpos - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xpos;
            }
            polyY = ypos - frameOffY - frameH;
        }
        else
        {
            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                polyX = xpos - frameOffX - frameW;
            }
            else
            {
                polyX = frameOffX + xpos;
            }
            polyY = frameOffY + ypos;
        }

        if (!field_4_flags.Get(AnimFlags::eBit16_bBlending))
        {
            SetRGB0(pPoly, field_8_r, field_9_g, field_A_b);
        }

        const int w = frameW + polyX - 1;
        const int h = frameH + polyY - 1;
        SetXY0(pPoly, static_cast<short>(polyX), static_cast<short>(polyY));
        SetXY1(pPoly, static_cast<short>(w), static_cast<short>(polyY));
        SetXY2(pPoly, static_cast<short>(polyX), static_cast<short>(h));
        SetXY3(pPoly, static_cast<short>(w), static_cast<short>(h));

        SetPrimExtraPointerHack(pPoly, nullptr);
        OrderingTable_Add_498A80(OtLayer(ppOt, field_C_layer), &pPoly->mBase.header);
    }
}

void AnimationUnknown::GetRenderedSize_404220(PSX_RECT* pRect)
{
    Poly_FT4_Get_Rect(pRect, &field_10_polys[gPsxDisplay_504C78.field_A_buffer_index]);
}

}
