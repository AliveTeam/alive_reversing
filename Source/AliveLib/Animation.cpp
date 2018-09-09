#include "stdafx.h"
#include "Animation.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Compression.hpp"

struct AnimationHeader
{
    // Meta data - the offset where this record was read from
    WORD mFps = 0;            // Seems to be 0x1 or 0x2
    short field_2_num_frames = 0;      // Number of frames in the set

                              // If loop flag set then this is the frame to loop back to
    short field_4_loop_start_frame = 0;

    enum eFlags
    {
        eFlipXFlag = 0x4,
        eFlipYFlag = 0x8,
        eNeverUnload = 0x1,
        eLoopFlag = 0x2
    };
    WORD mFlags = 0;
    DWORD mFrameOffsets[1]; // Reading past 1 is UB.. will need to change this later (copy out the data or something)
};
//ALIVE_ASSERT_SIZEOF(AnimationHeader, 0x8);

void Animation::Animation__vdecode_40AC90()
{

}

void AnimationEx::DecompressFrame()
{
    if (field_4_flags.Get(AnimFlags::eBit11))
    {
        field_4_flags.Toggle(AnimFlags::eBit10);
    }

    const FrameInfoHeader* pFrameInfo = Get_FrameHeader_40B730(-1); // -1 = use current frame
    if (pFrameInfo->field_6_count > 0)
    {
        Invoke_CallBacks_40B7A0();
    }

    if (field_84_vram_rect.w <= 0)
    {
        return;
    }

    const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

    short width_bpp_adjusted = 0;
    if (field_4_flags.Get(AnimFlags::eBit13))
    {
        // 8 bit, divided by half
        width_bpp_adjusted = ((pFrameHeader->field_4_width + 3) / 2) & ~ 1;
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

        if (!field_24_dbuf)
        {
            field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
        }

        if (field_24_dbuf)
        {
            CompressionType2_Decompress_40AA50(
                reinterpret_cast<const BYTE*>(&pFrameHeader[1]), // TODO: Refactor structure to get pixel data
                *field_24_dbuf,
                width_bpp_adjusted * pFrameHeader->field_5_height * 2);

            PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
        }
        break;

    case 3:
        if (field_4_flags.Get(AnimFlags::eBit25))
        {
            if (!field_24_dbuf)
            {
                field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
            }

            if (field_24_dbuf)
            {
                CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf); // TODO: Refactor structure to get pixel data
                PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
            }
        }
        break;

    case 4:
    case 5:
        if (!field_24_dbuf)
        {
            field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
        }

        if (field_24_dbuf)
        {
            CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf); // TODO: Refactor structure to get pixel data
            PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
        }
        break;

    case 6:
        if (field_4_flags.Get(AnimFlags::eBit25))
        {
            if (!field_24_dbuf)
            {
                field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
            }

            if (field_24_dbuf)
            {
                CompressionType6Ae_Decompress_40A8A0(reinterpret_cast<const BYTE*>(&pFrameHeader->field_8_width2), *field_24_dbuf); // TODO: Refactor structure to get pixel data
                PSX_LoadImage_4F5FB0(&vram_rect, *field_24_dbuf);
            }
        }
        break;

    case 7:
    case 8:
        field_4_flags.Set(AnimFlags::eBit25);

        if (!field_24_dbuf)
        {
            field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_DecompressionBuffer, 0, field_28_dbuf_size);
        }

        if (field_24_dbuf)
        {
            const int totalSize = width_bpp_adjusted * pFrameHeader->field_5_height * 2;
            
            // TODO: ABEINTRO.BAN in AE PC has this?
            ALIVE_FATAL("Not implemented");
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

void AnimationEx::Animation__vdecode_40AC90()
{
    if (field_4_flags.Get(AnimFlags::eBit22))
    {
        Animationv_40B200();
        return;
    }

    if (!field_20_ppBlock)
    {
        return;
    }

    AnimationHeader* pAnimHeader = reinterpret_cast<AnimationHeader*>(&(*field_20_ppBlock)[field_18_frame_table_offset]);
    if (pAnimHeader->field_2_num_frames != 1 || !(field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted)))
    {
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
                field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
                DecompressFrame();
                return;
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
                field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
                DecompressFrame();
                return;
            }
        }

        field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
        DecompressFrame();
        return;
    }
}

void AnimationEx::Invoke_CallBacks_40B7A0()
{
    NOT_IMPLEMENTED();
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

__int16 Animation::Animationv_40B200()
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animationv_40B200");
    return 0;
}

char Animation::Animation_v_40BEE0(__int16 /*a2*/, __int16 /*a3*/, int /*a4*/, __int16 /*a5*/, __int16 /*op1*/)
{
    NOT_IMPLEMENTED();
    LOG_INFO("Animation_v_40BEE0");
    return 0;
}

signed __int16 Animation::Set_Animation_Data_409C80(int /*frameTableOffset*/, BYTE** /*pAnimRes*/)
{
    NOT_IMPLEMENTED();
    return 0;
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
                pAnim->Animation__vdecode_40AC90();

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

signed __int16 AnimationEx::Init_40A030(int /*frameTableOffset*/, DynamicArray* /*animList*/, void* /*pGameObj*/, int /*maxW*/, unsigned __int16 /*maxH*/, BYTE** /*ppAnimData*/, unsigned __int8 /*unknown1*/, signed int /*pal_depth*/, char /*unknown3*/)
{
    NOT_IMPLEMENTED();
    return 0;
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
