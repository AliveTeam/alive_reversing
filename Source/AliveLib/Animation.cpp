#include "stdafx.h"
#include "Animation.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Compression.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"
#include <gmock/gmock.h>

void Animation::vDecode_40AC90()
{
    ALIVE_FATAL("Should never be called");
}

bool AnimationEx::EnsureDecompressionBuffer()
{
    if (!field_24_dbuf)
    {
        field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
    }
    return field_24_dbuf != nullptr;
}

void AnimationEx::DecompressFrame()
{
    if (field_4_flags.Get(AnimFlags::eBit11_bToggle_Bit10))
    {
        field_4_flags.Toggle(AnimFlags::eBit10_alternating_flag);
    }

    const FrameInfoHeader* pFrameInfo = Get_FrameHeader_40B730(-1); // -1 = use current frame
    if (pFrameInfo->field_6_count > 0)
    {
        Invoke_CallBacks_40B7A0();
    }

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

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
        pFrameHeader->field_5_height
    };

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

    switch (pFrameHeader->field_7_compression_type)
    {
    case 0:
        // No compression, load the data directly into frame buffer
        field_4_flags.Set(AnimFlags::eBit25_bDecompressDone);
        PSX_LoadImage_4F5FB0(&vram_rect, reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2)); // TODO: Refactor structure to get pixel data
        break;

    case 1:
        // This isn't in any of the animation data files on disk, therefore can't ever be used.
        ALIVE_FATAL("Compression type 1 never expected to be used");
        break;

    case 2:
        field_4_flags.Set(AnimFlags::eBit25_bDecompressDone);
        if (EnsureDecompressionBuffer())
        {
            // TODO: Refactor structure to get pixel data
            CompressionType2_Decompress_40AA50(
                reinterpret_cast<const BYTE*>(&pFrameHeader[1]),
                *field_24_dbuf,
                width_bpp_adjusted * pFrameHeader->field_5_height * 2);

            PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
        }
        break;

    case 3:
        if (field_4_flags.Get(AnimFlags::eBit25_bDecompressDone))
        {
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data
                CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
                PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
            }
        }
        break;

    case 4:
    case 5:
        if (EnsureDecompressionBuffer())
        {
            // TODO: Refactor structure to get pixel data
            CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
            PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
        }
        break;

    case 6:
        if (field_4_flags.Get(AnimFlags::eBit25_bDecompressDone))
        {
            if (EnsureDecompressionBuffer())
            {
                // TODO: Refactor structure to get pixel data
                CompressionType6Ae_Decompress_40A8A0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
                PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
            }
        }
        break;

    case 7:
    case 8:
        ALIVE_FATAL("Decompression 7 and 8 never expected to be used");
        break;
    }
}

inline short FP_AdjustedToInteger(FP fp, FP adjustment)
{
    return FP_GetExponent(fp + adjustment);
}

void AnimationEx::vRender_40B820(int xpos, int ypos, int** pOt, __int16 width, signed int height)
{
    if ((field_84_vram_rect.x || field_84_vram_rect.y) && !(field_4_flags.Get(AnimFlags::eBit25_bDecompressDone)))
    {
        Vram_free_495A60({ field_84_vram_rect.x, field_84_vram_rect.y }, { field_84_vram_rect.w, field_84_vram_rect.h });
        field_84_vram_rect.x = 0;
        field_84_vram_rect.y = 0;
    }

    const short xpos_unknown =  static_cast<short>(PsxToPCX(xpos));
    const short width_unknown = static_cast<short>(PsxToPCX(width));

    if (field_4_flags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Unexpected flag 0x200000 / eBit22_DeadMode"); // Animation_vRender2_40BEE0()
    }

    if (!field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        return;
    }

    if (!field_20_ppBlock)
    {
        return;
    }

    FrameInfoHeader* pFrameInfoHeader = Get_FrameHeader_40B730(-1);
    FrameHeader* pFrameHeader = (FrameHeader *)&(*field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset];
  
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
    if (field_4_flags.Get(AnimFlags::eBit20))
    {
        xOffSet_fixed = FP_FromInteger(0);
        yOffset_fixed = FP_FromInteger(0);
    }
    else
    {
        xOffSet_fixed = FP_FromInteger(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x);
        yOffset_fixed = FP_FromInteger(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y);
    }

    char textureMode = 0;
    if (field_4_flags.Get(AnimFlags::eBit13_Is8Bit)) //  if (BYTE1(pFrameHeader) & 0x10)
    {
        // 8 bit
        textureMode = 1;
    }
    else if (field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
    {
        // 16 bit
        textureMode = 2;
    }
    else
    {
        // 4 bit
        textureMode = 0;
    }

    Poly_FT4* pPoly = &field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index];
    PolyFT4_Init_4F8870(pPoly);
    Poly_Set_SemiTrans_4F8A60(&pPoly->mBase.header, field_4_flags.Get(AnimFlags::eBit15_bSemiTrans));
    Poly_Set_Blending_4F8A20(&pPoly->mBase.header, field_4_flags.Get(AnimFlags::eBit16_bBlending));
    SetRGB0(pPoly, field_8_r, field_9_g, field_A_b);
    SetTPage(pPoly, static_cast<WORD>(PSX_getTPage_4F60E0(textureMode, field_B_render_mode, field_84_vram_rect.x, field_84_vram_rect.y)));
    SetClut(pPoly, static_cast<WORD>(PSX_getClut_4F6350(field_8C_pal_vram_x.field_0_x, field_8C_pal_vram_x.field_2_y)));

    BYTE u1 = field_84_vram_rect.x & 63;
    if (textureMode == 1)
    {
        // 8 bit
        u1 *= 2;
    }
    else if (textureMode == 0)
    {
        // 4 bit
        u1 *= 4;
    }
    else
    {
        // 16 bit
    }

    const BYTE v0 = static_cast<BYTE>(field_84_vram_rect.y);
    const BYTE u0 = pFrameHeader->field_4_width + u1 - 1;
    const BYTE v1 = pFrameHeader->field_5_height + v0 - 1;

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

    short polyXPos = 0;
    short polyYPos = 0;
    if (field_4_flags.Get(AnimFlags::eBit6_FlipY))
    {
        if (field_4_flags.Get(AnimFlags::eBit5_FlipX))
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
        polyYPos = static_cast<short>(ypos) - FP_AdjustedToInteger(yOffset_fixed, FP_FromDouble(0.499)) - FP_AdjustedToInteger(frame_height_fixed, FP_FromDouble(0.499));
    }
    else
    {
        FP yPosFixed;
        if (field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            SetUV0(pPoly, u0, v0);
            SetUV1(pPoly, u1, v0);
            SetUV2(pPoly, u0, v1);
            SetUV3(pPoly, u1, v1);

            polyXPos = xpos_unknown - FP_AdjustedToInteger(xOffSet_fixed, FP_FromDouble(0.499)) - FP_AdjustedToInteger(frame_width_fixed, FP_FromDouble(0.499));
            polyYPos = static_cast<short>(ypos) + FP_AdjustedToInteger(yOffset_fixed, FP_FromDouble(0.499));
        }
        else
        {
            SetUV0(pPoly, u1, v0);
            SetUV1(pPoly, u0, v0);
            SetUV2(pPoly, u1, v1);
            SetUV3(pPoly, u0, v1);

            polyXPos = xpos_unknown + FP_AdjustedToInteger(xOffSet_fixed, FP_FromDouble(0.499));

            yPosFixed = yOffset_fixed + FP_FromDouble(0.499);
            polyYPos = static_cast<short>(ypos) + FP_AdjustedToInteger(yOffset_fixed, FP_FromDouble(0.499));
        }
        
    }

    SetXY0(pPoly, polyXPos, polyYPos);
    SetXY1(pPoly, polyXPos + FP_GetExponent(frame_width_fixed - FP_FromDouble(0.501)), polyYPos);
    SetXY2(pPoly, polyXPos, polyYPos + FP_GetExponent(frame_height_fixed - FP_FromDouble(0.501)));
    SetXY3(pPoly, polyXPos + FP_GetExponent(frame_width_fixed - FP_FromDouble(0.501)), polyYPos + FP_GetExponent(frame_height_fixed - FP_FromDouble(0.501)));

    if (pFrameHeader->field_7_compression_type == 3 || pFrameHeader->field_7_compression_type == 6)
    {
        SetPrimExtraPointerHack(pPoly, &pFrameHeader->field_8_width2);
    }
    else
    {
        SetPrimExtraPointerHack(pPoly, nullptr);
    }

    OrderingTable_Add_4F8AA0(&pOt[field_C_render_layer], &pPoly->mBase.header);
}

signed __int16 AnimationEx::vCleanUp_40C630()
{
    gObjList_animations_5C1A24->Remove_Item(this);
    Animation_Pal_Free_40C4C0();
    return ResourceManager::FreeResource_49C330(field_24_dbuf);
}

void AnimationEx::vDecode2_40B200()
{
    ALIVE_FATAL("Impossible - this kind of anim data don't exist");
}

void AnimationEx::vDecode_40AC90()
{
    if (field_4_flags.Get(AnimFlags::eBit22_DeadMode))
    {
        // Never hit ?
        vDecode2_40B200();
        return;
    }

    if (DecodeCommon())
    {
        DecompressFrame();
    }
}

bool AnimationEx::DecodeCommon()
{
    if (!field_20_ppBlock)
    {
        return false;
    }

    AnimationHeader* pAnimHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[field_18_frame_table_offset]);
    if (pAnimHeader->field_2_num_frames == 1 && field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        return false;
    }

    bool isLastFrame = false;
    if (field_4_flags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        // Loop backwards
        const __int16 prevFrameNum = --field_92_current_frame;
        field_E_frame_change_counter = field_10_frame_delay;

        if (prevFrameNum < pAnimHeader->field_4_loop_start_frame)
        {
            if (field_4_flags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop to last frame
                field_92_current_frame = pAnimHeader->field_2_num_frames - 1;
            }
            else
            {
                // Stay on current frame
                field_92_current_frame = prevFrameNum + 1;
                field_E_frame_change_counter = 0;
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
        const __int16 nextFrameNum = ++field_92_current_frame;
        field_E_frame_change_counter = field_10_frame_delay;

        // Animation reached end point
        if (nextFrameNum >= pAnimHeader->field_2_num_frames)
        {
            if (field_4_flags.Get(AnimFlags::eBit8_Loop))
            {
                // Loop back to loop start frame
                field_92_current_frame = pAnimHeader->field_4_loop_start_frame;
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
        if (field_92_current_frame == pAnimHeader->field_2_num_frames - 1)
        {
            isLastFrame = true;
        }
    }

    if (isLastFrame)
    {
        field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }
    else
    {
        field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    }

    return true;
}

void AnimationEx::Invoke_CallBacks_40B7A0()
{
    if (!field_20_ppBlock || !field_1C_fn_ptr_array)
    {
        return;
    }

    FrameInfoHeader* pFrameHeaderCopy = Get_FrameHeader_40B730(-1);
    // This data can be an array of DWORD's + other data up to field_6_count
    // which appears AFTER the usual data.
    DWORD* pCallBackData = reinterpret_cast<DWORD*>(&pFrameHeaderCopy->field_8_data.points[3]);
    for (int i = 0; i < pFrameHeaderCopy->field_6_count; i++)
    {
        auto pFnCallBack = field_1C_fn_ptr_array[*pCallBackData];
        if (!pFnCallBack)
        {
            break;
        }
        pCallBackData++; // Skip the array index
        // Pass the data pointer into the call back which will then read and skip any extra data
        pCallBackData += pFnCallBack(field_94_pGameObj, (short*)pCallBackData);
    }
}

void Animation::vRender_40B820(int /*xpos*/, int /*ypos*/, int** /*pOt*/, __int16 /*width*/, signed int /*height*/)
{
    ALIVE_FATAL("Should never be called");
//    return 0;
}

char Animation::Animation_v_40BEE0(__int16 /*a2*/, __int16 /*a3*/, int /*a4*/, __int16 /*a5*/, __int16 /*op1*/)
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animation_v_40BEE0");
    return 0;
}

signed __int16 AnimationEx::Set_Animation_Data_409C80(int frameTableOffset, BYTE** pAnimRes)
{
    if (pAnimRes)
    {
        // Animation block must match what was previously set
        if (field_4_flags.Get(AnimFlags::eBit22_DeadMode) && field_20_ppBlock != pAnimRes)
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

    vDecode_40AC90();

    // Reset to start frame
    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    return 1;
}

void AnimationEx::Animation_Pal_Free_40C4C0()
{
    if (field_4_flags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Impossible case - data of this type dosen't exist");
    }
    else
    {
        if (field_84_vram_rect.w > 0)
        {
            if (field_84_vram_rect.x || field_84_vram_rect.y || field_4_flags.Get(AnimFlags::eBit9))
            {
                Vram_free_495A60({ field_84_vram_rect.x, field_84_vram_rect.y }, { field_84_vram_rect.w, field_84_vram_rect.h });
            }
        }

        if (field_90_pal_depth > 0 && field_4_flags.Get(AnimFlags::eBit17))
        {
            Pal_free_483390(field_8C_pal_vram_x, field_90_pal_depth);
        }

    }
}

void CC Animation::AnimateAll_40AC20(DynamicArrayT<Animation>* pAnims)
{
    for (auto i = 0; i < pAnims->Size(); i++)
    {
        Animation* pAnim = pAnims->ItemAt(i);
        if (!pAnim)
        {
            break;
        }

        if (!pAnim->field_4_flags.Get(AnimFlags::eBit2_Animate))
        {
            continue;
        }

        if (pAnim->field_E_frame_change_counter > 0)
        {
            pAnim->field_E_frame_change_counter--;
            if (pAnim->field_E_frame_change_counter == 0)
            {
                pAnim->vDecode_40AC90();
            }
        }
    }
}

void AnimationEx::SetFrame_409D50(__int16 newFrame)
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

ALIVE_VAR(1, 0x5440AC, FrameInfoHeader, sBlankFrameInfoHeader_5440AC, {});

FrameInfoHeader* AnimationEx::Get_FrameHeader_40B730(__int16 frame)
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
        FrameInfoHeader* Unknown = &sBlankFrameInfoHeader_5440AC;
        return Unknown;
    }
#endif

    return pFrame;

}

void AnimationEx::Get_Frame_Rect_409E10(PSX_RECT* pRect)
{
    NOT_IMPLEMENTED();

    if (!field_4_flags.Get(AnimFlags::eBit16_bBlending))
    {
        Poly_FT4_Get_Rect_409DA0(pRect, &field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index]);
        return;
    }

    // The ASM here is crazy, but basically it gets a pointer to field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index] twice
    // so everything it is comparing is a pointer ot the same polygon.

    // TODO: Impl
}

WORD AnimationEx::Get_Frame_Count_40AC70()
{
    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset);  // TODO: Make getting offset to animation header cleaner
    return pHead->field_2_num_frames;
}

signed __int16 AnimationEx::Init_40A030(int frameTableOffset, DynamicArray* /*animList*/, void *pGameObj, unsigned __int16 maxW, unsigned __int16 maxH, BYTE **ppAnimData, unsigned __int8 bFlag_17, signed int b_StartingAlternationState, char bEnable_flag10_alternating)
{
    field_4_flags.Raw().all = 0; // TODO extra - init to 0's first - this may be wrong if any bits are explicitly set before this is called
    field_4_flags.Set(AnimFlags::eBit21);
    
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

    field_4_flags.Clear(AnimFlags::eBit1);
    field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    field_4_flags.Clear(AnimFlags::eBit6_FlipY);
    field_4_flags.Clear(AnimFlags::eBit7);
    field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_4_flags.Set(AnimFlags::eBit3_Render);

    if (pHeader->field_6_flags & AnimationHeader::eLoopFlag)
    {
        field_4_flags.Set(AnimFlags::eBit8_Loop);
    }

    if (bEnable_flag10_alternating)
    {
        field_4_flags.Set(AnimFlags::eBit10_alternating_flag);
    }

    if (b_StartingAlternationState)
    {
        field_4_flags.Set(AnimFlags::eBit11_bToggle_Bit10);
    }

    field_4_flags.Clear(AnimFlags::eBit14_Is16Bit);
    field_4_flags.Clear(AnimFlags::eBit13_Is8Bit);

    // Clear vram/pal inits to not allocated
    field_84_vram_rect.w = 0;
    field_90_pal_depth = 0;

    if (bFlag_17)
    {
        field_4_flags.Set(AnimFlags::eBit17);
    }

    field_4_flags.Clear(AnimFlags::eBit24);
    field_4_flags.Clear(AnimFlags::eBit23);
    field_4_flags.Clear(AnimFlags::eBit22_DeadMode);

    // TODO: Refactor
    if (*((DWORD *)*ppAnimData + 2) != 0)
    {
        // Never in any source data ?
        field_4_flags.Set(AnimFlags::eBit22_DeadMode);
        ALIVE_FATAL("This can't happen");
    }

    // NOTE: All branches related to bit 22 removed
    if (field_4_flags.Get(AnimFlags::eBit22_DeadMode))
    {
        ALIVE_FATAL("Unknown data");
    }

    field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_4_flags.Set(AnimFlags::eBit16_bBlending);

    field_10_frame_delay = pHeader->field_0_fps;
    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;
    field_B_render_mode = 0;
    field_A_b = 0;
    field_9_g = 0;
    field_8_r = 0;
    field_14_scale.fpValue = 0x10000;

    FrameInfoHeader* pFrameHeader = Get_FrameHeader_40B730(0);
    BYTE* pAnimData = *field_20_ppBlock;

    const FrameHeader* pFrameHeader_1 = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameHeader->field_0_frame_header_offset]);
    
    BYTE* pClut = &pAnimData[pFrameHeader_1->field_0_clut_offset];
  
    if (!Vram_alloc_4956C0(maxW, maxH, pFrameHeader_1->field_6_colour_depth, &field_84_vram_rect))
    {
        return 0;
    }
    
    WORD pal_depth = 0;
    char b256Pal = 0;
    int vram_width = 0;
    if (pFrameHeader_1->field_6_colour_depth == 4)
    {
        const int v37 = maxW / 2;
        const int v38 = maxW % 2;

        vram_width = v38 + v37;
        pal_depth = 16;
        b256Pal = 0; // is 16 pal
    }
    else if (pFrameHeader_1->field_6_colour_depth == 8)
    {
        vram_width = maxW;
        field_4_flags.Set(AnimFlags::eBit13_Is8Bit);
        if (*(DWORD *)pClut != 64) // CLUT entry count
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
            field_4_flags.Set(AnimFlags::eBit14_Is16Bit);
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

    field_4_flags.Clear(AnimFlags::eBit25_bDecompressDone);
    if (b256Pal)
    {
        field_4_flags.Set(AnimFlags::eBit25_bDecompressDone);
    }

    if (field_4_flags.Get(AnimFlags::eBit17)==true && field_4_flags.Get(AnimFlags::eBit24) == false)
    {
        PSX_RECT rect = {}; // TODO: Not sure if its really a rect passed here, seems to populate x,y,colour depth?
        if (!Pal_Allocate_483110(&rect, pal_depth))
        {
            Animation_Pal_Free_40C4C0();
            return 0;
        }

        field_8C_pal_vram_x.field_0_x = rect.x;
        field_8C_pal_vram_x.field_2_y = rect.y;
        field_90_pal_depth = pal_depth;

        rect.w = pal_depth;
        rect.h = 1;

        PSX_LoadImage16_4F5E20(&rect, (BYTE *)(pClut + 4)); // Skips CLUT len
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
    vDecode_40AC90();

    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    return 1;
}

void AnimationEx::Load_Pal_40A530(BYTE ** pAnimData, int palOffset)
{
    if (!pAnimData)
    {
        return;
    }

    PSX_RECT rect = {};
    rect.x = field_8C_pal_vram_x.field_0_x;
    rect.y = field_8C_pal_vram_x.field_2_y;
    rect.w = field_90_pal_depth; // 16, 64, 256
    rect.h = 1;

    BYTE* pPal = &(*pAnimData)[palOffset];
    PSX_LoadImage16_4F5E20(&rect, pPal + 4); // First 4 pal bytes are the length, TODO: Add structure for pallete to avoid this
}

/*
void AnimationEx::Get_Bounding_Rect_Top_Left_40C480(signed __int16 frameNum, __int16* pBoundingX, __int16* pBoundingY)
{
     FrameInfoHeader* pFrameHeader = Get_FrameHeader_40B730(frameNum);
    *pBoundingX = pFrameHeader->mTopLeft.x;
    *pBoundingY = pFrameHeader->mTopLeft.y;
}
*/

namespace Test
{
    struct TestAnimData
    {
        AnimationHeader mHeader;
        FrameInfoHeader mFrameInfoHeader;
        FrameHeader mFrameHeader;
    };

    static void RenderTest()
    {
        AnimationEx anim;
        anim.field_84_vram_rect.x = 0;
        anim.field_84_vram_rect.y = 0;
        anim.field_4_flags.Raw().all = 0;
        anim.field_4_flags.Set(AnimFlags::eBit3_Render);
        anim.field_92_current_frame = 0;
        anim.field_C_render_layer = 0;
        anim.field_14_scale.fpValue = 0x10000;
        anim.field_B_render_mode = 0;

        TestAnimData testData = {};
        testData.mHeader.mFrameOffsets[0] = sizeof(AnimationHeader);
        testData.mHeader.field_0_fps = 2;

        testData.mFrameInfoHeader.field_0_frame_header_offset = sizeof(AnimationHeader) + sizeof(FrameInfoHeader);
        testData.mFrameInfoHeader.field_6_count = 1;
        testData.mFrameInfoHeader.field_8_data.offsetAndRect.mOffset.x = 3;
        testData.mFrameInfoHeader.field_8_data.offsetAndRect.mOffset.y = 7;

        testData.mFrameHeader.field_4_width = 20;
        testData.mFrameHeader.field_5_height = 30;
        testData.mFrameHeader.field_6_colour_depth = 8;

        TestAnimData* pTestData = &testData;

        anim.field_20_ppBlock = (BYTE **)&pTestData;
        anim.field_18_frame_table_offset = 0;

        gPsxDisplay_5C1130.field_C_buffer_index = 0;
      
        int ot[256] = {};
        Poly_FT4* pPoly = &anim.field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index];

        {
            *pPoly = {};

            anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            anim.vRender_40B820(20, 30, (int**)&ot, 0, 0);

            ASSERT_EQ(X0(pPoly), 37);
            ASSERT_EQ(Y0(pPoly), 37);

            ASSERT_EQ(X1(pPoly), 56);
            ASSERT_EQ(Y1(pPoly), 37);

            ASSERT_EQ(X2(pPoly), 37);
            ASSERT_EQ(Y2(pPoly), 66);

            ASSERT_EQ(X3(pPoly), 56);
            ASSERT_EQ(Y3(pPoly), 66);

            ASSERT_EQ(U0(pPoly), 0);
            ASSERT_EQ(V0(pPoly), 0);

            ASSERT_EQ(U1(pPoly), 19);
            ASSERT_EQ(V1(pPoly), 0);

            ASSERT_EQ(U2(pPoly), 0);
            ASSERT_EQ(V2(pPoly), 29);

            ASSERT_EQ(U3(pPoly), 19);
            ASSERT_EQ(V3(pPoly), 29);


        }

        {
            *pPoly = {};
            anim.field_4_flags.Set(eBit5_FlipX);
            anim.vRender_40B820(20, 30, (int**)&ot, 0, 0);

            ASSERT_EQ(X0(pPoly), 11);
            ASSERT_EQ(Y0(pPoly), 37);

            ASSERT_EQ(X1(pPoly), 30);
            ASSERT_EQ(Y1(pPoly), 37);

            ASSERT_EQ(X2(pPoly), 11);
            ASSERT_EQ(Y2(pPoly), 66);

            ASSERT_EQ(X3(pPoly), 30);
            ASSERT_EQ(Y3(pPoly), 66);


            ASSERT_EQ(U0(pPoly), 19);
            ASSERT_EQ(V0(pPoly), 0);

            ASSERT_EQ(U1(pPoly), 0);
            ASSERT_EQ(V1(pPoly), 0);

            ASSERT_EQ(U2(pPoly), 19);
            ASSERT_EQ(V2(pPoly), 29);

            ASSERT_EQ(U3(pPoly), 0);
            ASSERT_EQ(V3(pPoly), 29);
        }

        {
            *pPoly = {};
            anim.field_4_flags.Set(eBit5_FlipX);
            anim.field_4_flags.Set(eBit13_Is8Bit);

            anim.vRender_40B820(20, 30, (int**)&ot, 0, 0);

            ASSERT_EQ(X0(pPoly), 11);
            ASSERT_EQ(Y0(pPoly), 37);

            ASSERT_EQ(X1(pPoly), 30);
            ASSERT_EQ(Y1(pPoly), 37);

            ASSERT_EQ(X2(pPoly), 11);
            ASSERT_EQ(Y2(pPoly), 66);

            ASSERT_EQ(X3(pPoly), 30);
            ASSERT_EQ(Y3(pPoly), 66);


            ASSERT_EQ(U0(pPoly), 19);
            ASSERT_EQ(V0(pPoly), 0);

            ASSERT_EQ(U1(pPoly), 0);
            ASSERT_EQ(V1(pPoly), 0);

            ASSERT_EQ(U2(pPoly), 19);
            ASSERT_EQ(V2(pPoly), 29);

            ASSERT_EQ(U3(pPoly), 0);
            ASSERT_EQ(V3(pPoly), 29);
        }
    }

    void AnimationTests()
    {
        RenderTest();
    }
}
