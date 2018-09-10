#include "stdafx.h"
#include "Animation.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Compression.hpp"
#include "VRam.hpp"
#include "Game.hpp"

struct AnimationHeader
{
    // Meta data - the offset where this record was read from
    WORD field_0_fps = 0;            // Seems to be 0x1 or 0x2
    short field_2_num_frames = 0;      // Number of frames in the set

                              // If loop flag set then this is the frame to loop back to
    short field_4_loop_start_frame = 0;

    // These where reversed by editing data in memory on PSX version
    enum eFlags
    {
        eFlipXFlag = 0x4,
        eFlipYFlag = 0x8,
        eNeverUnload = 0x1,
        eLoopFlag = 0x2
    };
    WORD field_6_flags = 0;
    DWORD mFrameOffsets[1]; // Reading past 1 is UB.. will need to change this later (copy out the data or something)
};
//ALIVE_ASSERT_SIZEOF(AnimationHeader, 0x8);

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

// TODO: Can be combined with other DecompressFrame method
void AnimationEx::DecompressFrame_VramAlloc()
{
    const FrameInfoHeader* pFrameInfo = Get_FrameHeader_40B730(-1); // -1 = use current frame
    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

    if (pFrameInfo->field_6_count > 0)
    {
        Invoke_CallBacks_40B7A0();
    }

    if (field_4_flags.Get(AnimFlags::eBit23))
    {
        return;
    }
    
    field_84_vram_rect.w = 0;


    if (!Vram_alloc_4956C0(
        pFrameHeader->field_4_width,
        pFrameHeader->field_5_height,
        pFrameHeader->field_6_colour_depth,
        &field_84_vram_rect))
    {
        return;
    }

    // TODO: Allocated rect is copied some where unknown here

    short width_bpp_adjusted = 0;
    if (field_4_flags.Get(AnimFlags::eBit13))
    {
        // 8 bit, divided by half
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 3) / 2) & ~1;
    }
    else if (field_4_flags.Get(AnimFlags::eBit14))
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
        PSX_LoadImage_4F5FB0(&vram_rect, reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2)); // TODO: Refactor structure to get pixel data
        break;

    case 1:
        // This isn't in any of the animation data files on disk, therefore can't ever be used.
        ALIVE_FATAL("Compression type 1 never expected to be used");
        break;

    case 2:
        field_4_flags.Set(AnimFlags::eBit25);
        if (!EnsureDecompressionBuffer())
        {
            return;
        }

        // TODO: Refactor structure to get pixel data
        CompressionType2_Decompress_40AA50(
            reinterpret_cast<const BYTE*>(&pFrameHeader[1]),
            *field_24_dbuf,
            width_bpp_adjusted * pFrameHeader->field_5_height * 2);

        PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);

        break;

    case 3:
        if (!EnsureDecompressionBuffer())
        {
            return;
        }

        // TODO: Refactor structure to get pixel data
        CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
        PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
        break;

    case 4:
    case 5:
        if (!EnsureDecompressionBuffer())
        {
            return;
        }
        // TODO: Refactor structure to get pixel data
        CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf);
        PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
        break;

        // NOTE: No case 6 handled in this method.. strange

    case 7:
    case 8:
        if (!EnsureDecompressionBuffer())
        {
            return;
        }

        const int totalSize = width_bpp_adjusted * pFrameHeader->field_5_height * 2;

        // TODO: ABEINTRO.BAN in AE PC has this?
        ALIVE_FATAL("Decompression 7 and 8 not implemented");
        /*

        pData3 = *(DWORD *)&pFrameHeader->field_8_width2;

        for (i = Compression_type_7_8_4ABB90(
        &pFrameHeader[1],
        *field_24_dbuf,
        pData3,
        pFrameHeader->field_7_compression_type != 8 ? 8 : 6);
        i < totalSize;
        (*field_24_dbuf)[i - 1] = 0)
        {
        ++i;
        }
        PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
        */
        break;
    }

    if (field_24_dbuf)
    {
        ResourceManager::FreeResource_49C330(field_24_dbuf);
        field_24_dbuf = nullptr;
    }

    field_4_flags.Set(AnimFlags::eBit23);
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
    if (field_4_flags.Get(AnimFlags::eBit13))
    {
        // 8 bit, divided by half
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 3) / 2) & ~1;
    }
    else if (field_4_flags.Get(AnimFlags::eBit14))
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
        field_4_flags.Set(AnimFlags::eBit25);
        PSX_LoadImage_4F5FB0(&vram_rect, reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2)); // TODO: Refactor structure to get pixel data
        break;

    case 1:
        // This isn't in any of the animation data files on disk, therefore can't ever be used.
        ALIVE_FATAL("Compression type 1 never expected to be used");
        break;

    case 2:
        field_4_flags.Set(AnimFlags::eBit25);
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
        if (field_4_flags.Get(AnimFlags::eBit25))
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
        if (field_4_flags.Get(AnimFlags::eBit25))
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
        field_4_flags.Set(AnimFlags::eBit25);

        if (EnsureDecompressionBuffer())
        {
            const int totalSize = width_bpp_adjusted * pFrameHeader->field_5_height * 2;

            // TODO: ABEINTRO.BAN in AE PC has this?
            ALIVE_FATAL("Decompression 7 and 8 not implemented");
            /*

            pData3 = *(DWORD *)&pFrameHeader->field_8_width2;

            for (i = Compression_type_7_8_4ABB90(
                &pFrameHeader[1],
                *field_24_dbuf,
                pData3,
                pFrameHeader->field_7_compression_type != 8 ? 8 : 6);
                i < totalSize;
                (*field_24_dbuf)[i - 1] = 0)
            {
                ++i;
            }
            PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
            */
        }
        break;
    }
}

void AnimationEx::vDecode2_40B200()
{
    if (DecodeCommon())
    {
        DecompressFrame_VramAlloc();
    }
}

void AnimationEx::vDecode_40AC90()
{
    if (field_4_flags.Get(AnimFlags::eBit22))
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
    // TODO: Add a union, clearly this data can be an array of DWORD's of field_6_count
    // which may contain more data used by the call back.
    __int16* pCallBackData = &pFrameHeaderCopy->mTopLeft.x + 2 * pFrameHeaderCopy->field_4_magic;
    for (int i = 0; i < pFrameHeaderCopy->field_6_count; i++)
    {
        auto pFnCallBack = field_1C_fn_ptr_array[*(DWORD *)pCallBackData];
        if (!pFnCallBack)
        {
            break;
        }
        pCallBackData += 2 * pFnCallBack(field_94_pGameObj, pCallBackData + 2) + 2;
    }
}

char Animation::Animation_v_40B820(signed int /*a2*/, int /*a3*/, int /*a4*/, __int16 /*a5*/, signed int /*op1*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

// Destructor ?
signed __int16 Animation::Animationv_40C630()
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animationv_40C630");
    return 0;
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
        if (field_4_flags.Get(AnimFlags::eBit22) && field_20_ppBlock != pAnimRes)
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
    NOT_IMPLEMENTED();
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

                //pAnim->field_4_flags |= AnimFlags::eBit12_LoopCompleted;
                //pAnim->field_4_flags &= ~AnimFlags::eBit12_LoopCompleted;

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
    if (reinterpret_cast<DWORD>(pFrame) & 3)
    {
        FrameInfoHeader* Unknown = &sBlankFrameInfoHeader_5440AC;
        return Unknown;
    }

    return pFrame;

}

void AnimationEx::Get_Frame_Rect_409E10(PSX_RECT * /*pRect*/)
{
    NOT_IMPLEMENTED();
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
    field_4_flags.Clear(AnimFlags::eBit5);
    field_4_flags.Clear(AnimFlags::eBit6);
    field_4_flags.Clear(AnimFlags::eBit7);
    field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_4_flags.Set(AnimFlags::eBit3);

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

    field_4_flags.Clear(AnimFlags::eBit14);
    field_4_flags.Clear(AnimFlags::eBit13);

    // Clear vram/pal inits to not allocated
    field_84_vram_rect.w = 0;
    field_90_pal_depth = 0;

    if (bFlag_17)
    {
        field_4_flags.Set(AnimFlags::eBit17);
    }

    field_4_flags.Clear(AnimFlags::eBit24);
    field_4_flags.Clear(AnimFlags::eBit23);
    field_4_flags.Clear(AnimFlags::eBit22);

    // TODO: Refactor
    if (*((DWORD *)*ppAnimData + 2) != 0)
    {
        field_4_flags.Set(AnimFlags::eBit22);
    }

    // NOTE: All branches related to bit 22 removed
    if (field_4_flags.Get(AnimFlags::eBit22))
    {
        ALIVE_FATAL("Unknown data");
    }

    field_4_flags.Clear(AnimFlags::eBit16);
    field_4_flags.Clear(AnimFlags::eBit15);
    field_4_flags.Set(AnimFlags::eBit16);

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

    FrameHeader* pFrameHeader_1 = reinterpret_cast<FrameHeader*>(&pAnimData[pFrameHeader->field_0_frame_header_offset]);
    
    BYTE* pClut = &pAnimData[pFrameHeader_1->field_0_clut_offset];
  
    __int16 result = Vram_alloc_4956C0(maxW, maxH, pFrameHeader_1->field_6_colour_depth, &field_84_vram_rect);
    if (!result)
    {
        return result;
    }
    
    WORD pal_depth = 0;

    int vram_width = 0;
    if (pFrameHeader_1->field_6_colour_depth == 4)
    {
        const int v37 = (unsigned int)(unsigned __int16)maxW >> 1;
        const int v38 = (unsigned __int16)maxW % 2;

        vram_width = v38 + v37;
        pal_depth = 16;
        goto LABEL_26;
    }

    char b16Bit = 0;
    if (pFrameHeader_1->field_6_colour_depth == 8)
    {
        vram_width = (unsigned __int16)maxW;
        field_4_flags.Set(AnimFlags::eBit13);
        if (*(DWORD *)pClut != 64)
        {
            pal_depth = 256;
            b16Bit = 1;
            goto LABEL_27;
        }
        pal_depth = 64;
    }
    else
    {
        if (pFrameHeader_1->field_6_colour_depth == 16)
        {
            vram_width = 2 * (unsigned __int16)maxW;
            field_4_flags.Set(AnimFlags::eBit14);
        }
        else
        {
            vram_width = pal_depth;
        }
        if (pal_depth != 16 && pal_depth != 64)
        {
            b16Bit = 1;
            goto LABEL_27;
        }
    }
LABEL_26:
    b16Bit = 0;
LABEL_27:
    __int16 b16BitFlag = b16Bit & 1;
    
    field_4_flags.Clear(AnimFlags::eBit25);
    if (b16BitFlag)
    {
        field_4_flags.Set(AnimFlags::eBit25);
    }

    if (field_4_flags.Get(AnimFlags::eBit17)==true && field_4_flags.Get(AnimFlags::eBit24) == false)
    {
        const __int16* vram_x = (__int16 *)&field_8C_pal_vram_x;
        if (!Pal_Allocate_483110((PSX_RECT *)&field_8C_pal_vram_x, pal_depth))
        {
            Animation_Pal_Free_40C4C0();
            return 0;
        }
        const __int16 vram_y = field_8C_pal_vram_x.field_2_y;

        PSX_RECT rect = {};
        rect.x = *vram_x;
        rect.y = vram_y;
        rect.w = pal_depth;
        rect.h = 1;
        PSX_LoadImage16_4F5E20(&rect, (BYTE *)(pClut + 4)); // Skips CLUT len
    }

    field_28_dbuf_size = maxH * (vram_width + 3);
    const DWORD dbuf_size = field_28_dbuf_size;
    field_24_dbuf = nullptr;
    field_28_dbuf_size = dbuf_size + 8;

    // NOTE: OG bug or odd compiler code gen? Why isn't it using the passed in list which appears to always be this anyway ??
    const __int16 bAdded = gObjList_animations_5C1A24->Push_Back(this);
    if (!bAdded)
    {
        return 0;
    }

    vDecode_40AC90();

    result = bAdded;
    field_E_frame_change_counter = 1;
    field_92_current_frame = -1;

    return result;
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
