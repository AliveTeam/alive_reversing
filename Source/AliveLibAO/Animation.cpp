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
#include "../AliveLibAE/Renderer/IRenderer.hpp"
#include "AnimResources.hpp"

// Fix pollution from windows.h
#undef min
#undef max

namespace AO {

s16* Animation_OnFrame_Slig(BaseGameObject* pObj, s16* pData)
{
    auto pSlig = static_cast<Slig*>(pObj);
    if (pSlig->mBaseGameObjectUpdateDelay != 0)
    {
        return pData + 2;
    }

    BulletType bulletType = BulletType::ePossessedSlig_0;
    if (pSlig->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit2_bPossesed))
    {
        pSlig->field_254_prevent_depossession |= 1u;
        bulletType = BulletType::ePossessedSlig_0;
    }
    else
    {
        bulletType = BulletType::eNormalBullet_1;
    }

    const FP xOff = pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]);
    const FP yOff = pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[1]);
    if (pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos,
            FP_FromInteger(-640),
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            0);

        New_ShootingFire_Particle_419720(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos - xOff,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            1,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        relive_new BulletShell(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos,
            FP_FromInteger(640),
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            0);

        New_ShootingFire_Particle_419720(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos + xOff,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        relive_new BulletShell(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            1,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        SFX_Play_Mono(SoundEffect::SligShoot_6, 85);
    }
    else
    {
        SFX_Play_Mono(SoundEffect::SligShoot_6, 0);
    }

    Event_Broadcast(kEventShooting, pSlig);
    Event_Broadcast(kEventLoudNoise, pSlig);

    Dove::All_FlyAway();

    return pData + 2;
}

s16* Animation_OnFrame_ZBallSmacker(BaseGameObject* pObj, s16* pData);

s16* Slog_OnFrame(BaseGameObject* pObj, s16* pData)
{
    auto pSlog = static_cast<Slog*>(pObj);
    if (pSlog->field_10C_pTarget)
    {
        const PSX_RECT targetRect = pSlog->field_10C_pTarget->VGetBoundingRect();
        const PSX_RECT slogRect = pSlog->VGetBoundingRect();

        if (RectsOverlap(slogRect, targetRect))
        {
            if (pSlog->field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale && !pSlog->field_110)
            {
                if (pSlog->field_10C_pTarget->VTakeDamage(pSlog))
                {
                    FP blood_xpos = {};
                    if (pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        blood_xpos = pSlog->mBaseAnimatedWithPhysicsGameObject_XPos - (pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
                    }
                    else
                    {
                        blood_xpos = pSlog->mBaseAnimatedWithPhysicsGameObject_XPos + (pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
                    }

                    const FP blood_ypos = (pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[1])) + pSlog->mBaseAnimatedWithPhysicsGameObject_YPos;

                    relive_new Blood(
                        blood_xpos,
                        blood_ypos - FP_FromInteger(8),
                        (pSlog->mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(2)),
                        FP_FromInteger(0),
                        pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                        50);

                    pSlog->field_110 = 1;

                    SFX_Play_Mono(SoundEffect::SlogBite_39, 0);
                }
            }
        }
    }

    return pData + 2;
}

const FP_Point kAbeVelTable_4C6608[6] = {
    {FP_FromInteger(3), FP_FromInteger(-14)},
    {FP_FromInteger(10), FP_FromInteger(-10)},
    {FP_FromInteger(15), FP_FromInteger(-8)},
    {FP_FromInteger(10), FP_FromInteger(3)},
    {FP_FromInteger(10), FP_FromInteger(-4)},
    {FP_FromInteger(4), FP_FromInteger(-3)}};

s16* Abe_OnFrame(BaseGameObject* pObj, s16* pData)
{
    auto pAbe = static_cast<Abe*>(pObj);

    FP xVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].field_0_x * pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    const FP yVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].field_4_y * pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    FP directed_x = {};
    if (sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xVel = -xVel;
        directed_x = -(pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
    }
    else
    {
        directed_x = (pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
    }

    FP data_y = FP_FromInteger(pData[1]);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            pAbe->mBaseAnimatedWithPhysicsGameObject_XPos,
            pAbe->mBaseAnimatedWithPhysicsGameObject_YPos + data_y,
            pAbe->mBaseAnimatedWithPhysicsGameObject_XPos + directed_x,
            pAbe->mBaseAnimatedWithPhysicsGameObject_YPos + data_y,
            &pLine,
            &hitX,
            &hitY,
            pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 6 : 0x60))
    {
        directed_x = hitX - pAbe->mBaseAnimatedWithPhysicsGameObject_XPos;
        xVel = -xVel;
    }

    if (sActiveHero_507678->field_198_pThrowable)
    {
        sActiveHero_507678->field_198_pThrowable->mBaseAnimatedWithPhysicsGameObject_XPos = directed_x + sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos;
        BaseThrowable* pThrowable = sActiveHero_507678->field_198_pThrowable;
        pThrowable->mBaseAnimatedWithPhysicsGameObject_YPos = (pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * data_y) + sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos;
        pThrowable->VThrow(xVel, yVel);
        pThrowable->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        sActiveHero_507678->field_198_pThrowable = nullptr;
    }
    return pData + 2;
}

TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[] = {Abe_OnFrame};
TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[] = {Animation_OnFrame_Slig};
TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[] = {Slog_OnFrame};
TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[] = {Animation_OnFrame_ZBallSmacker};

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

void Animation::UploadTexture(const FrameHeader* pFrameHeader, const PSX_RECT& vram_rect, s16 width_bpp_adjusted)
{
    IRenderer& renderer = *IRenderer::GetRenderer();
    switch (pFrameHeader->field_7_compression_type)
    {
        case CompressionType::eType_0_NoCompression:
            renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2));
            break;

        case CompressionType::eType_1_NotUsed:
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data/remove casts
                Decompress_Type_1(
                    (u8*) &pFrameHeader[1],
                    *field_24_dbuf,
                    *(u32*) &pFrameHeader->field_8_width2,
                    2 * pFrameHeader->field_5_height * width_bpp_adjusted);
                renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
            }
            break;

        case CompressionType::eType_2_ThreeToFourBytes:
            if (EnsureDecompressionBuffer())
            {
                Decompress_Type_2(
                    (u8*) &pFrameHeader[1],
                    *field_24_dbuf,
                    2 * pFrameHeader->field_5_height * width_bpp_adjusted);
                renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
            }
            break;

        case CompressionType::eType_3_RLE_Blocks:
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data/remove casts
                Decompress_Type_3(
                    (u8*) &pFrameHeader[1],
                    *field_24_dbuf,
                    *(u32*) &pFrameHeader->field_8_width2,
                    2 * pFrameHeader->field_5_height * width_bpp_adjusted);
                renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
            }
            break;

        case CompressionType::eType_4_RLE:
        case CompressionType::eType_5_RLE:
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data/remove casts
                Decompress_Type_4Or5(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
                renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
            }
            break;

        default:
            LOG_ERROR("Unknown compression type " << static_cast<s32>(pFrameHeader->field_7_compression_type));
            ALIVE_FATAL("Unknown compression type");
            break;
    }
}

bool Animation::EnsureDecompressionBuffer()
{
    if (!field_24_dbuf)
    {
        field_24_dbuf = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
    }
    return field_24_dbuf != nullptr;
}

void Animation::DecompressFrame()
{
    if (mAnimFlags.Get(AnimFlags::eBit11_bToggle_Bit10))
    {
        mAnimFlags.Toggle(AnimFlags::eBit10_alternating_flag);
    }

    const FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader(-1); // -1 = use current frame
    if (pFrameInfoHeader->field_6_count > 0)
    {
        Invoke_CallBacks();
    }

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

    // No VRAM allocation
    if (field_84_vram_rect.w <= 0)
    {
        return;
    }

    s16 width_bpp_adjusted = 0;
    if (mAnimFlags.Get(AnimFlags::eBit13_Is8Bit))
    {
        // 8 bit, divided by half
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 3) / 2) & ~1;
    }
    else if (mAnimFlags.Get(AnimFlags::eBit14_Is16Bit))
    {
        // 16 bit, only multiple of 2 rounding
        width_bpp_adjusted = (pFrameHeader->field_4_width + 1) & ~1;
    }
    else
    {
        // 4 bit divide by quarter
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 7) / 4) & ~1;
    }

    PSX_RECT vram_rect = {
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


void Animation::VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s16 height)
{
    if (mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        u8** ppBlock = field_20_ppBlock;
        if (ppBlock)
        {
            const u8* pBlock = *ppBlock;
            const FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader(-1);
            const FrameHeader* pFrameHeader = (const FrameHeader*) &pBlock[pFrameInfoHeader->field_0_frame_header_offset];

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
            if (mAnimFlags.Get(AnimFlags::eBit20_use_xy_offset))
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
            if (mAnimFlags.Get(AnimFlags::eBit13_Is8Bit))
            {
                textureMode = TPageMode::e8Bit_1;
            }
            else if (mAnimFlags.Get(AnimFlags::eBit14_Is16Bit))
            {
                textureMode = TPageMode::e16Bit_2;
            }
            else
            {
                textureMode = TPageMode::e4Bit_0;
            }

            s16 tPageY = 0;
            if (mAnimFlags.Get(AnimFlags::eBit10_alternating_flag) || field_84_vram_rect.y >= 256)
            {
                tPageY = 256;
            }
            else
            {
                tPageY = 0;
            }

            Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_504C78.field_A_buffer_index];
            PolyFT4_Init(pPoly);
            Poly_Set_SemiTrans_498A40(&pPoly->mBase.header, mAnimFlags.Get(AnimFlags::eBit15_bSemiTrans));
            Poly_Set_Blending_498A00(&pPoly->mBase.header, mAnimFlags.Get(AnimFlags::eBit16_bBlending));

            SetRGB0(pPoly, mRed, mGreen, mBlue);
            SetTPage(pPoly, static_cast<s16>(PSX_getTPage(textureMode, mRenderMode, field_84_vram_rect.x, tPageY)));
            SetClut(pPoly, static_cast<s16>(PSX_getClut_496840(field_8C_pal_vram_xy.field_0_x, field_8C_pal_vram_xy.field_2_y)));

            u8 u1 = field_84_vram_rect.x & 63;
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

            const u8 v0 = static_cast<u8>(field_84_vram_rect.y);
            const u8 u0 = pFrameHeader->field_4_width + u1 - 1;
            const u8 v1 = pFrameHeader->field_5_height + v0 - 1;

            if (field_14_scale != FP_FromInteger(1))
            {
                // Apply scale to x/y pos
                frame_height_fixed *= field_14_scale;
                frame_width_fixed *= field_14_scale;

                // Apply scale to x/y offset
                xOffSet_fixed *= field_14_scale;
                yOffset_fixed = (yOffset_fixed * field_14_scale) - FP_FromInteger(1);
            }

            s32 polyXPos;
            if (mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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

            const s16 polyYPos = static_cast<s16>(ypos + FP_GetExponent((yOffset_fixed + FP_FromDouble(0.499))));
            const s16 xConverted = static_cast<s16>(PsxToPCX(polyXPos));
            const s16 width_adjusted = FP_GetExponent(PsxToPCX(frame_width_fixed) - FP_FromDouble(0.501)) + xConverted;
            const s16 height_adjusted = FP_GetExponent(frame_height_fixed - FP_FromDouble(0.501)) + polyYPos;

            SetXY0(pPoly, xConverted, polyYPos);
            SetXY1(pPoly, width_adjusted, polyYPos);
            SetXY2(pPoly, xConverted, height_adjusted);
            SetXY3(pPoly, width_adjusted, height_adjusted);

            SetPrimExtraPointerHack(pPoly, nullptr);
            OrderingTable_Add_498A80(OtLayer(ppOt, mRenderLayer), &pPoly->mBase.header);
        }
    }
}

void Animation::VCleanUp()
{
    gAnimations->Remove_Item(this);

    Animation_Pal_Free();

    ResourceManager::FreeResource_455550(field_24_dbuf);
}

void Animation::VDecode()
{
    if (DecodeCommon())
    {
        DecompressFrame();
    }
}

bool Animation::DecodeCommon()
{
    if (!field_20_ppBlock || !*field_20_ppBlock)
    {
        return false;
    }

    AnimationHeader* pAnimationHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[field_18_frame_table_offset]);
    if (pAnimationHeader->field_2_num_frames == 1 && mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        return false;
    }

    bool isLastFrame = false;
    if (mAnimFlags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        // Loop backwards
        const s16 prevFrameNum = --field_92_current_frame;
        mFrameChangeCounter = static_cast<s16>(field_10_frame_delay);

        if (prevFrameNum < pAnimationHeader->field_4_loop_start_frame)
        {
            if (mAnimFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop to last frame
                field_92_current_frame = pAnimationHeader->field_2_num_frames - 1;
            }
            else
            {
                // Stay on current frame
                mFrameChangeCounter = 0;
                field_92_current_frame = prevFrameNum + 1;
            }

            // For some reason eBit12_ForwardLoopCompleted isn't set when going backwards
        }

        // Is first (last since running backwards) frame?
        if (field_92_current_frame == 0)
        {
            isLastFrame = true;
        }
    }
    else
    {
        // Loop forwards
        const s16 nextFrameNum = ++field_92_current_frame;
        mFrameChangeCounter = static_cast<s16>(field_10_frame_delay);

        // Animation reached end point
        if (nextFrameNum >= pAnimationHeader->field_2_num_frames)
        {
            if (mAnimFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop back to loop start frame
                field_92_current_frame = pAnimationHeader->field_4_loop_start_frame;
            }
            else
            {
                // Stay on current frame
                field_92_current_frame = nextFrameNum - 1;
                mFrameChangeCounter = 0;
            }

            mAnimFlags.Set(AnimFlags::eBit12_ForwardLoopCompleted);
        }

        // Is last frame ?
        if (field_92_current_frame == pAnimationHeader->field_2_num_frames - 1)
        {
            isLastFrame = true;
        }
    }

    if (isLastFrame)
    {
        mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }
    else
    {
        mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    }

    return true;
}

void Animation::Invoke_CallBacks()
{
    if (field_1C_fn_ptr_array)
    {
        FrameInfoHeader* pFrameHeaderCopy = Get_FrameHeader(-1);

        // This data can be an array of u32's + other data up to field_6_count
        // which appears AFTER the usual data.

        // TODO: Should be typed to s16* ??
        const u32* pCallBackData = reinterpret_cast<const u32*>(&pFrameHeaderCopy->field_8_data.points[3]);
        for (s32 i = 0; i < pFrameHeaderCopy->field_6_count; i++)
        {
            const auto pFnCallBack = field_1C_fn_ptr_array[*pCallBackData];
            if (!pFnCallBack)
            {
                break;
            }
            pCallBackData++; // Skip the array index
            // Pass the data pointer into the call back which will then read and skip any extra data
            pCallBackData += *pFnCallBack(field_94_pGameObj, (s16*) pCallBackData);
        }
    }
}

s16 Animation::Set_Animation_Data(s32 frameTableOffset, u8** pAnimRes)
{
    FrameTableOffsetExists(frameTableOffset, false);
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

    mAnimFlags.Clear(AnimFlags::eBit12_ForwardLoopCompleted);
    mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards);
    mAnimFlags.Clear(AnimFlags::eBit8_Loop);

    if (pAnimationHeader->field_6_flags & AnimationHeader::eLoopFlag)
    {
        mAnimFlags.Set(AnimFlags::eBit8_Loop);
    }

    mFrameChangeCounter = 1;
    field_92_current_frame = -1;

    VDecode();

    // Reset to start frame
    mFrameChangeCounter = 1;
    field_92_current_frame = -1;

    return 1;
}


void Animation::Animation_Pal_Free()
{
    if (mAnimFlags.Get(AnimFlags::eBit17_bOwnPal))
    {
        ResourceManager::FreeResource_455550(field_20_ppBlock);
    }

    if (field_84_vram_rect.w > 0)
    {
        Vram_free_450CE0({field_84_vram_rect.x, field_84_vram_rect.y}, {field_84_vram_rect.w, field_84_vram_rect.h});
    }

    if (field_90_pal_depth > 0)
    {
        IRenderer::GetRenderer()->PalFree(IRenderer::PalRecord{field_8C_pal_vram_xy.field_0_x, field_8C_pal_vram_xy.field_2_y, field_90_pal_depth});
    }
}

void Animation::SetFrame(s16 newFrame)
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

        mFrameChangeCounter = 1;
        field_92_current_frame = newFrame - 1;
    }
}

ALIVE_VAR(1, 0x4BA090, FrameInfoHeader, sBlankFrameInfoHeader_4BA090, {});

FrameInfoHeader* Animation::Get_FrameHeader(s32 frame)
{
    if (!field_20_ppBlock)
    {
        return nullptr;
    }

    if (frame < -1 || frame == -1)
    {
        frame = field_92_current_frame != -1 ? field_92_current_frame : 0;
    }

    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset); // TODO: Make getting offset to animation header cleaner
    u32 frameOffset = pHead->mFrameOffsets[frame];

    FrameInfoHeader* pFrame = reinterpret_cast<FrameInfoHeader*>(*field_20_ppBlock + frameOffset);

    // Never seen this get hit, perhaps some sort of PSX specific check as addresses have to be aligned there?
    // TODO: Remove it in the future when proven to be not required?
#if defined(_MSC_VER) && !defined(_WIN64)
    if (reinterpret_cast<u32>(pFrame) & 3)
    {
        FrameInfoHeader* Unknown = &sBlankFrameInfoHeader_4BA090;
        return Unknown;
    }
#endif

    return pFrame;
}

void Animation::Get_Frame_Rect(PSX_RECT* pRect)
{
    Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_504C78.field_A_buffer_index];
    if (!mAnimFlags.Get(AnimFlags::eBit20_use_xy_offset))
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

s16 Animation::Get_Frame_Count()
{
    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset); // TODO: Make getting offset to animation header cleaner
    return pHead->field_2_num_frames;
}

s16 Animation::Init(s32 frameTableOffset, DynamicArray* /*animList*/, BaseGameObject* pGameObj, u16 maxW, u16 maxH, u8** ppAnimData, u8 bAllocateVRam, s32 b_StartingAlternationState, s8 bEnable_flag10_alternating)
{
    FrameTableOffsetExists(frameTableOffset, false, maxW, maxH);
    mAnimFlags.Raw().all = 0; // TODO extra - init to 0's first - this may be wrong if any bits are explicitly set before this is called

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

    mAnimFlags.Clear(AnimFlags::eBit1);
    mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    mAnimFlags.Clear(AnimFlags::eBit6_FlipY);
    mAnimFlags.Clear(AnimFlags::eBit7_SwapXY);
    mAnimFlags.Set(AnimFlags::eBit2_Animate);
    mAnimFlags.Set(AnimFlags::eBit3_Render);

    mAnimFlags.Set(AnimFlags::eBit8_Loop, pHeader->field_6_flags & AnimationHeader::eLoopFlag);

    mAnimFlags.Set(AnimFlags::eBit10_alternating_flag, bEnable_flag10_alternating);

    mAnimFlags.Set(AnimFlags::eBit11_bToggle_Bit10, b_StartingAlternationState);

    mAnimFlags.Clear(AnimFlags::eBit14_Is16Bit);
    mAnimFlags.Clear(AnimFlags::eBit13_Is8Bit);

    // Clear vram/pal inits to not allocated
    field_84_vram_rect.w = 0;
    field_90_pal_depth = 0;
    mAnimFlags.Set(AnimFlags::eBit17_bOwnPal);

    
    mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    mAnimFlags.Set(AnimFlags::eBit16_bBlending);

    field_10_frame_delay = pHeader->field_0_fps;
    mFrameChangeCounter = 1;
    field_92_current_frame = -1;
    mRenderMode = TPageAbr::eBlend_0;
    mBlue = 0;
    mGreen = 0;
    mRed = 0;
    field_14_scale = FP_FromInteger(1);

    FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader(0);
    u8* pAnimData = *field_20_ppBlock;

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

    u8* pClut = &pAnimData[pFrameHeader->field_0_clut_offset];

    if (bAllocateVRam)
    {
        field_84_vram_rect.w = 0;
    }

    //s8 b256Pal = 0;
    s32 vram_width = 0;
    s16 pal_depth = 0;

    switch (pFrameHeader->field_6_colour_depth)
    {
        case 4:
        {
            vram_width = (maxW % 2) + (maxW / 2);
            pal_depth = 16;
            //b256Pal = 0; // is 16 pal
        }
        break;

        case 8:
        {
            vram_width = maxW;
            if (*(u16*) pClut == 64) // CLUT entry count/len
            {
                pal_depth = 64;
            }
            else
            {
				pal_depth = 256;
                //b256Pal = 1; // is 256 pal
            }
            mAnimFlags.Set(AnimFlags::eBit13_Is8Bit);

        }
        break;

        case 16:
        {
            vram_width = maxW * 2;
            mAnimFlags.Set(AnimFlags::eBit14_Is16Bit);
        }
        break;

        default:
            return 0;
    }

    s32 bVramAllocOK = 1;
    if (bAllocateVRam)
    {
        bVramAllocOK = vram_alloc_450B20(maxW, maxH, pFrameHeader->field_6_colour_depth, &field_84_vram_rect);
    }

    bool bPalAllocOK = true;
    if (pal_depth > 0 && bVramAllocOK)
    {
        IRenderer::PalRecord palRec{0, 0, pal_depth};
        if (!IRenderer::GetRenderer()->PalAlloc(palRec))
        {
            bPalAllocOK = false;
        }

        field_8C_pal_vram_xy.field_0_x = palRec.x;
        field_8C_pal_vram_xy.field_2_y = palRec.y;
        field_90_pal_depth = palRec.depth;

        if (bVramAllocOK && bPalAllocOK)
        {
            IRenderer::GetRenderer()->PalSetData(palRec, pClut + 4); // +4 Skip len, load pal
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
        const u32 id = ResourceManager::Get_Header_455620(field_20_ppBlock)->field_C_id;
        field_24_dbuf = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_DecompressionBuffer, id, field_28_dbuf_size);
        if (!field_24_dbuf)
        {
            return 0;
        }
    }

    // NOTE: OG bug or odd compiler code gen? Why isn't it using the passed in list which appears to always be this anyway ??
    if (!gAnimations->Push_Back(this))
    {
        LOG_ERROR("gAnimations->Push_Back(this) returned 0 but shouldn't");
        return 0;
    }

    // Get first frame decompressed/into VRAM
    VDecode();

    mFrameChangeCounter = 1;
    field_92_current_frame = -1;

    return 1;
}

void Animation::LoadPal(u8** pAnimData, s32 palOffset)
{
    if (pAnimData)
    {
        // +4 = skip CLUT len
        const u8* pPalDataOffset = &(*pAnimData)[palOffset];
        if (field_90_pal_depth != 16 && field_90_pal_depth != 64 && field_90_pal_depth != 256)
        {
            LOG_ERROR("Bad pal depth " << field_90_pal_depth);
            ALIVE_FATAL("Bad pal depth");
        }
        IRenderer::GetRenderer()->PalSetData(IRenderer::PalRecord{field_8C_pal_vram_xy.field_0_x, field_8C_pal_vram_xy.field_2_y, field_90_pal_depth}, pPalDataOffset + 4); // +4 skip len, load pal
    }
}

void Animation::Get_Frame_Offset(s16* pBoundingX, s16* pBoundingY)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader(-1);
    *pBoundingX = pFrameHeader->field_8_data.offsetAndRect.mOffset.x;
    *pBoundingY = pFrameHeader->field_8_data.offsetAndRect.mOffset.y;
}


void Animation::Get_Frame_Width_Height(s16* pWidth, s16* pHeight)
{
    FrameInfoHeader* pFrameHeader = Get_FrameHeader(-1);
    if (mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
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


} // namespace AO
