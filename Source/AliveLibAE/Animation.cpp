#include "stdafx.h"
#include "Animation.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Compression.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"
#include "Abe.hpp"    // For anim call back table
#include "Slig.hpp"   // For slig frame call back
#include "Fleech.hpp" // For fleech frame call back
#include "stdlib.hpp"
#include "Particle.hpp"
#include "Events.hpp"
#include "Slurg.hpp"
#include "Slog.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "ObjectIds.hpp"
#include "Sys_common.hpp"
#include "Renderer/IRenderer.hpp"
#include <gmock/gmock.h>

// Frame call backs ??
s32 Animation_OnFrame_Common_Null_455F40(void*, s16*)
{
    return 1;
}

s32 Animation_OnFrame_Null_455F60(void*, s16*)
{
    return 1;
}

s32 Animation_OnFrame_Common_4561B0(void* pObjPtr, s16* pData)
{
    auto pObj = static_cast<BaseAliveGameObject*>(pObjPtr);
    const AnimRecord& dustRec = AnimRec(AnimId::Dust_Particle);
    u8** ppAnimData = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, dustRec.mResourceId, FALSE, FALSE);

    FP xOff = {};
    if (pObj->field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff = -(pObj->field_CC_sprite_scale * FP_FromInteger(pData[0]));
    }
    else
    {
        xOff = (pObj->field_CC_sprite_scale * FP_FromInteger(pData[0]));
    }

    FP xpos = xOff + pObj->mBaseAnimatedWithPhysicsGameObject_XPos;
    FP ypos = (pObj->field_CC_sprite_scale * FP_FromInteger(pData[1])) + pObj->mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(25);
    if (!pObj->BaseAliveGameObjectCollisionLine)
    {
        return 1;
    }

    const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
    if (count < 5)
    {
        Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
        pPoints->field_0_points[count].field_0_xPos = FP_GetExponent(xpos);
        pPoints->field_0_points[count].field_2_yPos = pObj->BaseAliveGameObjectCollisionLine->field_0_rect.y - 5;
        sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
    }

    if (pObj != sActiveHero)
    {
        return 1;
    }

    if (pObj->field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        ypos -= FP_FromInteger(14);
    }

    if (pObj->mCurrentMotion == eAbeMotions::Motion_71_Knockback_455090 && pObj->field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        ypos += FP_FromInteger(5);
    }

    if (pObj->mCurrentMotion == eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        ypos -= FP_FromInteger(5);
    }

    auto pPartical = ae_new<Particle>(xpos, ypos, dustRec.mFrameTableOffset, dustRec.mMaxW, dustRec.mMaxH, ppAnimData);
    if (pPartical)
    {
        pPartical->field_20_animation.mRenderMode = TPageAbr::eBlend_1;

        if (pObj->field_D6_scale == 1)
        {
            pPartical->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pPartical->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pPartical->field_D0_r = 45;
        pPartical->field_D2_g = 35;
        pPartical->field_D4_b = 5;

        switch (pObj->mCurrentMotion)
        {
            case eAbeMotions::Motion_1_WalkLoop_44FBA0:
                pPartical->field_CC_sprite_scale = FP_FromDouble(0.3) * pObj->field_CC_sprite_scale;
                break;

            case eAbeMotions::Motion_40_SneakLoop_450550:
                pPartical->field_CC_sprite_scale = FP_FromInteger(0);
                break;

            case eAbeMotions::Motion_71_Knockback_455090:
                pPartical->field_CC_sprite_scale = FP_FromInteger(1) * pObj->field_CC_sprite_scale;
                break;

            default:
                pPartical->field_CC_sprite_scale = FP_FromDouble(0.6) * pObj->field_CC_sprite_scale;
                break;
        }
    }
    return 1;
}

s32 Animation_OnFrame_Common_434130(void* pObjPtr, s16* pData)
{
    auto pObj = static_cast<BaseAliveGameObject*>(pObjPtr);

    if (pObj->Type() == ReliveTypes::eAbe || pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eMudokon2)
    {
        LOG_ERROR("never expected pObj type id to be abe or mudokon in Animation_OnFrame_Common_434130");
        ALIVE_FATAL("got wrong type id");
    }

    if (pObj->mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    // flying slig: kVaporResID
    u8** ppAnimRes = pObj->field_10_resources_array.ItemAt(7);
    FP xOff = {};
    if (pObj->field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff = -(pObj->field_CC_sprite_scale * FP_FromInteger(pData[0]));
    }
    else
    {
        xOff = (pObj->field_CC_sprite_scale * FP_FromInteger(pData[0]));
    }

    FP xpos = xOff + pObj->mBaseAnimatedWithPhysicsGameObject_XPos;
    FP ypos = (pObj->field_CC_sprite_scale * (FP_FromInteger(pData[1]) + FP_FromInteger(25))) + pObj->mBaseAnimatedWithPhysicsGameObject_YPos;

    if (Event_Get(kEventDeathReset))
    {
        pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const AnimRecord& vaporizeRec = AnimRec(AnimId::Vaporize_Particle);
    auto pParticle = ae_new<Particle>(xpos, ypos, vaporizeRec.mFrameTableOffset, vaporizeRec.mMaxW, vaporizeRec.mMaxH, ppAnimRes);
    if (pParticle)
    {
        pParticle->field_20_animation.mRenderMode = TPageAbr::eBlend_1;
        pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;
        pParticle->field_D0_r = 64;
        pParticle->field_D2_g = 64;
        pParticle->field_D4_b = 64;
        pParticle->field_CC_sprite_scale = FP_FromDouble(0.6) * pObj->field_CC_sprite_scale;
    }
    return 1;
}

s32 Animation_OnFrame_Slog_4C3030(void* pObjPtr, s16* pPoints)
{
    auto pSlog = static_cast<Slog*>(pObjPtr);
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(pSlog->field_118_target_id));
    if (!pTarget)
    {
        return 1;
    }

    if ((pTarget->Type() == ReliveTypes::eAbe && pTarget->mCurrentMotion == eAbeMotions::Motion_68_ToOffScreenHoist_454B80) || pSlog->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 1;
    }

    PSX_RECT bTargetRect = {};
    pTarget->VGetBoundingRect(&bTargetRect, 1);

    PSX_RECT bSlogRect = {};
    pSlog->VGetBoundingRect(&bSlogRect, 1);

    if (bSlogRect.x > bTargetRect.w || bSlogRect.w < bTargetRect.x || bSlogRect.h < bTargetRect.y || bSlogRect.y > bTargetRect.h || pTarget->field_CC_sprite_scale != FP_FromInteger(1) || pSlog->field_11C_biting_target)
    {
        return 1;
    }

    if (!pTarget->VTakeDamage(pSlog))
    {
        return 1;
    }

    FP bloodX = {};
    if (pSlog->field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        bloodX = pSlog->mBaseAnimatedWithPhysicsGameObject_XPos - (pSlog->field_CC_sprite_scale * FP_FromInteger(pPoints[0]));
    }
    else
    {
        bloodX = (pSlog->field_CC_sprite_scale * FP_FromInteger(pPoints[0])) + pSlog->mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    const FP bloodY = (pSlog->field_CC_sprite_scale * FP_FromInteger(pPoints[1])) + pSlog->mBaseAnimatedWithPhysicsGameObject_YPos;

    ae_new<Blood>(
        bloodX,
        bloodY - FP_FromInteger(8),
        pSlog->field_C4_velx * FP_FromInteger(2),
        FP_FromInteger(0),
        pSlog->field_CC_sprite_scale,
        50);

    pSlog->field_11C_biting_target = 1;
    SFX_Play_Mono(SoundEffect::SlogBite_34, 0);

    return 1;
}

// TODO: Array is possibly bigger, called by AnimationEx::Invoke_CallBacks
TFrameCallBackType kAbe_Anim_Frame_Fns_55EF98[5] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_Abe_455F80,
    Animation_OnFrame_Common_4561B0,
    Animation_OnFrame_Common_434130};

TFrameCallBackType kSlig_Anim_Frame_Fns_55EFAC[4] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_Slig_4C0600,
    Animation_OnFrame_Common_4561B0};

TFrameCallBackType kSlog_Anim_Frame_Fns_55EFBC[2] = {
    Animation_OnFrame_Slog_4C3030,
    Animation_OnFrame_Null_455F60};

TFrameCallBackType kFlyingSlig_Anim_Frames_Fns_55EFC4[3] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_Common_434130};

TFrameCallBackType kFleech_Anim_Frame_Fns_55EFD0[3] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_Fleech_449A60};

// ================================================================
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
            // No compression, load the data directly into frame buffer
            mAnimFlags.Set(AnimFlags::eBit25_bDecompressDone);
            renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2)); // TODO: Refactor structure to get pixel data
            break;

        case CompressionType::eType_1_NotUsed:
            // This isn't in any of the animation data files on disk, therefore can't ever be used.
            ALIVE_FATAL("Compression type 1 never expected to be used.");
            break;

        case CompressionType::eType_2_ThreeToFourBytes:
            mAnimFlags.Set(AnimFlags::eBit25_bDecompressDone);
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data.
                CompressionType2_Decompress_40AA50(
                    reinterpret_cast<const u8*>(&pFrameHeader[1]),
                    *field_24_dbuf,
                    width_bpp_adjusted * pFrameHeader->field_5_height * 2);

                renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
            }
            break;

        case CompressionType::eType_3_RLE_Blocks:
            if (mAnimFlags.Get(AnimFlags::eBit25_bDecompressDone))
            {
                if (EnsureDecompressionBuffer())
                {
                    // TODO: Refactor structure to get pixel data.
                    CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *field_24_dbuf);

                    renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
                }
            }
            break;

        case CompressionType::eType_4_RLE:
        case CompressionType::eType_5_RLE:
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data.
                CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
                renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
            }
            break;

        case CompressionType::eType_6_RLE:
            if (mAnimFlags.Get(AnimFlags::eBit25_bDecompressDone))
            {
                if (EnsureDecompressionBuffer())
                {
                    // TODO: Refactor structure to get pixel data.
                    CompressionType6Ae_Decompress_40A8A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
                    renderer.Upload(AnimFlagsToBitDepth(mAnimFlags), vram_rect, *field_24_dbuf);
                }
            }
            break;

        case CompressionType::eType_7_NotUsed:
        case CompressionType::eType_8_NotUsed:
            ALIVE_FATAL("Decompression 7 and 8 never expected to be used");
            break;
    }
}

bool Animation::EnsureDecompressionBuffer()
{
    if (!field_24_dbuf)
    {
        field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
    }
    return field_24_dbuf != nullptr;
}

void Animation::DecompressFrame()
{
    if (mAnimFlags.Get(AnimFlags::eBit11_bToggle_Bit10))
    {
        mAnimFlags.Toggle(AnimFlags::eBit10_alternating_flag);
    }

    const FrameInfoHeader* pFrameInfo = Get_FrameHeader(-1); // -1 = use current frame
    if (pFrameInfo->field_6_count > 0)
    {
        Invoke_CallBacks();
    }

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

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
        pFrameHeader->field_5_height};

    // Clamp width
    if (width_bpp_adjusted > field_84_vram_rect.w)
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

inline s16 FP_AdjustedToInteger(FP fp, FP adjustment)
{
    return FP_GetExponent(fp + adjustment);
}

void Animation::VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height)
{
    if ((field_84_vram_rect.x || field_84_vram_rect.y) && !(mAnimFlags.Get(AnimFlags::eBit25_bDecompressDone)))
    {
        Vram_free_495A60({field_84_vram_rect.x, field_84_vram_rect.y}, {field_84_vram_rect.w, field_84_vram_rect.h});
        field_84_vram_rect.x = 0;
        field_84_vram_rect.y = 0;
    }

    const s16 xpos_unknown = static_cast<s16>(PsxToPCX(xpos));
    const s16 width_unknown = static_cast<s16>(PsxToPCX(width));

    if (mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Unexpected flag 0x200000 / eBit22_DeadMode"); // Animation_vRender2_40BEE0()
    }

    if (!mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        return;
    }

    if (!field_20_ppBlock)
    {
        return;
    }

    FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader(-1);
    FrameHeader* pFrameHeader = (FrameHeader*) &(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset];

    FP frame_width_fixed;
    FP frame_height_fixed;
    if (width_unknown)
    {
        frame_width_fixed = FP_FromInteger(width_unknown);
        frame_height_fixed = FP_FromInteger(height);
    }
    else
    {
        frame_width_fixed = FP_FromInteger(pFrameHeader->field_4_width);
        frame_height_fixed = FP_FromInteger(pFrameHeader->field_5_height);
    }

    FP xOffSet_fixed;
    FP yOffset_fixed;
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

    Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index];
    PolyFT4_Init(pPoly);
    Poly_Set_SemiTrans_4F8A60(&pPoly->mBase.header, mAnimFlags.Get(AnimFlags::eBit15_bSemiTrans));
    Poly_Set_Blending_4F8A20(&pPoly->mBase.header, mAnimFlags.Get(AnimFlags::eBit16_bBlending));
    SetRGB0(pPoly, mRed, mGreen, mBlue);
    SetTPage(pPoly, static_cast<u16>(PSX_getTPage_4F60E0(textureMode, mRenderMode, field_84_vram_rect.x, field_84_vram_rect.y)));
    SetClut(pPoly, static_cast<u16>(PSX_getClut_4F6350(field_8C_pal_vram_xy.field_0_x, field_8C_pal_vram_xy.field_2_y)));

    u8 u1 = field_84_vram_rect.x & 63;
    if (textureMode == TPageMode::e8Bit_1)
    {
        // 8 bit
        u1 *= 2;
    }
    else if (textureMode == TPageMode::e4Bit_0)
    {
        // 4 bit
        u1 *= 4;
    }
    else
    {
        // 16 bit
    }

    const u8 v0 = static_cast<u8>(field_84_vram_rect.y);
    const u8 u0 = pFrameHeader->field_4_width + u1 - 1;
    const u8 v1 = pFrameHeader->field_5_height + v0 - 1;

    if (field_14_scale != FP_FromDouble(1.0))
    {
        // Apply scale to x/y pos
        frame_height_fixed *= field_14_scale;
        frame_width_fixed *= field_14_scale;

        if (field_14_scale == FP_FromDouble(0.5))
        {
            // Add 1 if half scale
            frame_height_fixed += FP_FromDouble(1.0);
            frame_width_fixed += FP_FromDouble(1.0);
        }

        // Apply scale to x/y offset
        xOffSet_fixed *= field_14_scale;
        yOffset_fixed = (yOffset_fixed * field_14_scale) - FP_FromDouble(1.0);
    }

    s16 polyXPos = 0;
    s16 polyYPos = 0;
    if (mAnimFlags.Get(AnimFlags::eBit6_FlipY))
    {
        if (mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            SetUV0(pPoly, u0, v1);
            SetUV1(pPoly, u1, v1);
            SetUV2(pPoly, u0, v0);
            SetUV3(pPoly, u1, v0);
            polyXPos = xpos_unknown - FP_AdjustedToInteger(xOffSet_fixed, FP_FromDouble(0.499)) - FP_AdjustedToInteger(frame_width_fixed, FP_FromDouble(0.499));
        }
        else
        {
            SetUV0(pPoly, u1, v1);
            SetUV1(pPoly, u0, v1);
            SetUV2(pPoly, u1, v0);
            SetUV3(pPoly, u0, v0);
            polyXPos = xpos_unknown + FP_AdjustedToInteger(xOffSet_fixed, FP_FromDouble(0.499));
        }
        // TODO: Might be wrong because this was doing something with the sign bit abs() ??
        polyYPos = static_cast<s16>(ypos) - FP_AdjustedToInteger(yOffset_fixed, FP_FromDouble(0.499)) - FP_AdjustedToInteger(frame_height_fixed, FP_FromDouble(0.499));
    }
    else
    {
        FP yPosFixed;
        if (mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            SetUV0(pPoly, u0, v0);
            SetUV1(pPoly, u1, v0);
            SetUV2(pPoly, u0, v1);
            SetUV3(pPoly, u1, v1);

            polyXPos = xpos_unknown - FP_AdjustedToInteger(xOffSet_fixed, FP_FromDouble(0.499)) - FP_AdjustedToInteger(frame_width_fixed, FP_FromDouble(0.499));
            polyYPos = static_cast<s16>(ypos) + FP_AdjustedToInteger(yOffset_fixed, FP_FromDouble(0.499));
        }
        else
        {
            SetUV0(pPoly, u1, v0);
            SetUV1(pPoly, u0, v0);
            SetUV2(pPoly, u1, v1);
            SetUV3(pPoly, u0, v1);

            polyXPos = xpos_unknown + FP_AdjustedToInteger(xOffSet_fixed, FP_FromDouble(0.499));

            yPosFixed = yOffset_fixed + FP_FromDouble(0.499);
            polyYPos = static_cast<s16>(ypos) + FP_AdjustedToInteger(yOffset_fixed, FP_FromDouble(0.499));
        }
    }

    SetXY0(pPoly, polyXPos, polyYPos);
    SetXY1(pPoly, polyXPos + FP_GetExponent(frame_width_fixed - FP_FromDouble(0.501)), polyYPos);
    SetXY2(pPoly, polyXPos, polyYPos + FP_GetExponent(frame_height_fixed - FP_FromDouble(0.501)));
    SetXY3(pPoly, polyXPos + FP_GetExponent(frame_width_fixed - FP_FromDouble(0.501)), polyYPos + FP_GetExponent(frame_height_fixed - FP_FromDouble(0.501)));

    if (pFrameHeader->field_7_compression_type == CompressionType::eType_3_RLE_Blocks || pFrameHeader->field_7_compression_type == CompressionType::eType_6_RLE)
    {
        SetPrimExtraPointerHack(pPoly, &pFrameHeader->field_8_width2);
    }
    else
    {
        SetPrimExtraPointerHack(pPoly, nullptr);
    }

    OrderingTable_Add_4F8AA0(OtLayer(ppOt, mRenderLayer), &pPoly->mBase.header);
}

void Animation::VCleanUp()
{
    gObjList_animations_5C1A24->Remove_Item(this);
    Animation_Pal_Free();
    ResourceManager::FreeResource_49C330(field_24_dbuf);
}

void Animation::vDecode2()
{
    ALIVE_FATAL("Impossible - this kind of anim data don't exist");
}

void Animation::VDecode()
{
    if (mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
    {
        // Never hit ?
        vDecode2();
        return;
    }

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

    AnimationHeader* pAnimHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[field_18_frame_table_offset]);
    if (pAnimHeader->field_2_num_frames == 1 && mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        return false;
    }

    bool isLastFrame = false;
    if (mAnimFlags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        // Loop backwards
        const s16 prevFrameNum = --field_92_current_frame;
        mFrameChangeCounter = field_10_frame_delay;

        if (prevFrameNum < pAnimHeader->field_4_loop_start_frame)
        {
            if (mAnimFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop to last frame
                field_92_current_frame = pAnimHeader->field_2_num_frames - 1;
            }
            else
            {
                // Stay on current frame
                field_92_current_frame = prevFrameNum + 1;
                mFrameChangeCounter = 0;
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
        mFrameChangeCounter = field_10_frame_delay;

        // Animation reached end point
        if (nextFrameNum >= pAnimHeader->field_2_num_frames)
        {
            if (mAnimFlags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop back to loop start frame
                field_92_current_frame = pAnimHeader->field_4_loop_start_frame;
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
        if (field_92_current_frame == pAnimHeader->field_2_num_frames - 1)
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
    if (!field_20_ppBlock || !field_1C_fn_ptr_array)
    {
        return;
    }

    FrameInfoHeader* pFrameHeaderCopy = Get_FrameHeader(-1);
    // This data can be an array of u32's + other data up to field_6_count
    // which appears AFTER the usual data.
    u32* pCallBackData = reinterpret_cast<u32*>(&pFrameHeaderCopy->field_8_data.points[3]);
    for (s32 i = 0; i < pFrameHeaderCopy->field_6_count; i++)
    {
        auto pFnCallBack = field_1C_fn_ptr_array[*pCallBackData];
        if (!pFnCallBack)
        {
            break;
        }
        pCallBackData++; // Skip the array index
        // Pass the data pointer into the call back which will then read and skip any extra data
        pCallBackData += pFnCallBack(field_94_pGameObj, (s16*) pCallBackData);
    }
}

s16 Animation::Set_Animation_Data(s32 frameTableOffset, u8** pAnimRes)
{
    FrameTableOffsetExists(frameTableOffset, true);
    if (pAnimRes)
    {
        // Animation block must match what was previously set
        if (mAnimFlags.Get(AnimFlags::eBit22_DeadMode) && field_20_ppBlock != pAnimRes)
        {
            return 0;
        }
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
    if (mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Impossible case - data of this type dosen't exist");
    }
    else
    {
        if (field_84_vram_rect.w > 0)
        {
            if (field_84_vram_rect.x || field_84_vram_rect.y || mAnimFlags.Get(AnimFlags::eBit9))
            {
                Vram_free_495A60({field_84_vram_rect.x, field_84_vram_rect.y}, {field_84_vram_rect.w, field_84_vram_rect.h});
            }
        }

        if (field_90_pal_depth > 0 && mAnimFlags.Get(AnimFlags::eBit17_bOwnPal))
        {
            IRenderer::GetRenderer()->PalFree(IRenderer::PalRecord{field_8C_pal_vram_xy.field_0_x, field_8C_pal_vram_xy.field_2_y, field_90_pal_depth});
        }
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

ALIVE_VAR(1, 0x5440AC, FrameInfoHeader, sBlankFrameInfoHeader_5440AC, {});

FrameInfoHeader* Animation::Get_FrameHeader(s16 frame)
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
        FrameInfoHeader* Unknown = &sBlankFrameInfoHeader_5440AC;
        return Unknown;
    }
#endif

    return pFrame;
}

void Animation::Get_Frame_Rect(PSX_RECT* pRect)
{
    Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index];
    if (!mAnimFlags.Get(AnimFlags::eBit20_use_xy_offset))
    {
        Poly_FT4_Get_Rect_409DA0(pRect, pPoly);
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

u16 Animation::Get_Frame_Count()
{
    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset); // TODO: Make getting offset to animation header cleaner
    return pHead->field_2_num_frames;
}

s16 Animation::Init(s32 frameTableOffset, DynamicArray* /*animList*/, BaseGameObject* pGameObj, u16 maxW, u16 maxH, u8** ppAnimData, u8 bOwnsPalData, s32 b_StartingAlternationState, s8 bEnable_flag10_alternating)
{
    FrameTableOffsetExists(frameTableOffset, true, maxW, maxH);
    mAnimFlags.Raw().all = 0; // TODO extra - init to 0's first - this may be wrong if any bits are explicitly set before this is called
    mAnimFlags.Set(AnimFlags::eBit21);

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

    if (bOwnsPalData)
    {
        mAnimFlags.Set(AnimFlags::eBit17_bOwnPal);
    }

    mAnimFlags.Clear(AnimFlags::eBit24);
    mAnimFlags.Clear(AnimFlags::eBit23);
    mAnimFlags.Clear(AnimFlags::eBit22_DeadMode);

    // TODO: Refactor
    if (*((u32*) *ppAnimData + 2) != 0)
    {
        // Never in any source data ?
        mAnimFlags.Set(AnimFlags::eBit22_DeadMode);
        ALIVE_FATAL("This can't happen");
    }

    // NOTE: All branches related to bit 22 removed
    if (mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Unknown data");
    }

    mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
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

    const FrameHeader* pFrameHeader_1 = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

    u8* pClut = &pAnimData[pFrameHeader_1->field_0_clut_offset];

    if (!Vram_alloc_4956C0(maxW, maxH, pFrameHeader_1->field_6_colour_depth, &field_84_vram_rect))
    {
        return 0;
    }

    s16 pal_depth = 0;
    s8 b256Pal = 0;
    s32 vram_width = 0;
    if (pFrameHeader_1->field_6_colour_depth == 4)
    {
        const s32 halfW = maxW / 2;
        const s32 wMod2 = maxW % 2;

        vram_width = wMod2 + halfW;
        pal_depth = 16;
        b256Pal = 0; // is 16 pal
    }
    else if (pFrameHeader_1->field_6_colour_depth == 8)
    {
        vram_width = maxW;
        mAnimFlags.Set(AnimFlags::eBit13_Is8Bit);
        if (*(u32*) pClut != 64) // CLUT entry count
        {
            pal_depth = 256;
            b256Pal = 1; // is 256 pal
        }
        else
        {
            pal_depth = 64;
        }
    }
    else
    {
        if (pFrameHeader_1->field_6_colour_depth == 16)
        {
            vram_width = maxW * 2;
            mAnimFlags.Set(AnimFlags::eBit14_Is16Bit);
        }
        else
        {
            vram_width = pal_depth;
        }

        if (pal_depth != 16 && pal_depth != 64)
        {
            b256Pal = 1; // not 16 or 64 so must be 256 ??
        }
    }

    mAnimFlags.Set(AnimFlags::eBit25_bDecompressDone, b256Pal);

    if (mAnimFlags.Get(AnimFlags::eBit17_bOwnPal) && !mAnimFlags.Get(AnimFlags::eBit24))
    {
        IRenderer::PalRecord palRec{0, 0, pal_depth};
        if (!IRenderer::GetRenderer()->PalAlloc(palRec))
        {
            Animation_Pal_Free();
            return 0;
        }

        field_8C_pal_vram_xy.field_0_x = palRec.x;
        field_8C_pal_vram_xy.field_2_y = palRec.y;
        field_90_pal_depth = pal_depth;

        IRenderer::GetRenderer()->PalSetData(palRec, pClut + 4); // Skips CLUT len
    }

    field_28_dbuf_size = maxH * (vram_width + 3);
    field_28_dbuf_size += 8; // Add 8 for some reason
    field_24_dbuf = nullptr;

    // NOTE: OG bug or odd compiler code gen? Why isn't it using the passed in list which appears to always be this anyway ??
    if (!gObjList_animations_5C1A24->Push_Back(this))
    {
        return 0;
    }

    // Get first frame decompressed/into VRAM
    VDecode();

    mFrameChangeCounter = 1;
    field_92_current_frame = -1;

    return 1;
}

void Animation::Load_Pal(u8** pAnimData, s32 palOffset)
{
    if (pAnimData)
    {
        // +4 = skip CLUT len
        const u8* pPal = &(*pAnimData)[palOffset];
        if (field_90_pal_depth != 16 && field_90_pal_depth != 64 && field_90_pal_depth != 256)
        {
            LOG_ERROR("Bad pal depth " << field_90_pal_depth);
            ALIVE_FATAL("Bad pal depth");
        }
        IRenderer::GetRenderer()->PalSetData(IRenderer::PalRecord{field_8C_pal_vram_xy.field_0_x, field_8C_pal_vram_xy.field_2_y, field_90_pal_depth}, pPal + 4);
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
