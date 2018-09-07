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
    WORD field_4_loop_start_frame = 0;

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

// WIP
void AnimationEx::Animation__vdecode_40AC90()
{
    NOT_IMPLEMENTED();

    __int16 flags; // cx
    BYTE **ppData; // edx
    AnimationHeader *pAnimHeader; // eax
    __int16 prevFrameNum; // dx
    __int16 newFrame; // dx
    __int16 tmpFlags1; // cx
    __int16 nextFrameNum; // di
    __int16 endFrame; // di
    FrameInfoHeader *pFrameInfo; // eax
    int v12; // edx
    int v13; // edi
    BYTE **pAnimData; // ecx
    FrameHeader *pFrameHeader; // edi
    unsigned __int16 vram_w; // cx
    int width_rounded; // ebx
    __int16 vram_x; // ax
    signed int vram_width; // ebx
    BYTE **pDBuf; // eax
    unsigned int pData; // ebp
    BYTE **pAllocatedDBuf; // eax
    BYTE **pDBuf2; // eax
    BYTE **pAllocatedDBuf2; // eax
    BYTE **pDBuf3; // eax
    int pData2; // edi
    BYTE **pDBuf4; // eax
    BYTE **pDBuf5; // eax
    BYTE **pDBuf6; // eax
    unsigned int totalSize; // ebp
    unsigned int i; // eax
    int pData3; // [esp+10h] [ebp-Ch]
    PSX_RECT rect; // [esp+14h] [ebp-8h]

    flags = field_4_flags.Raw().words.hiword;
    if (flags & AnimFlags::eBit6)
    {
        Animationv_40B200();
        return;
    }

    ppData = this->field_20_ppBlock;
    if (ppData)
    {
        pAnimHeader = (AnimationHeader *)&(*ppData)[this->field_18_frame_table_offset];
        if (pAnimHeader->field_2_num_frames != 1 || !(field_4_flags.Get(AnimFlags::eBit12)))
        {
            if (flags & AnimFlags::eBit3_LoopBackwards)
            {
                // Looping backwards?
                prevFrameNum = --this->field_92_current_frame;
                this->field_E_frame_change_counter = this->field_10_frame_delay;
                if (prevFrameNum < (signed int)pAnimHeader->field_4_loop_start_frame)
                {
                    if ((char)(this->field_4_flags.Raw().bytes.b0) >= 0)
                    {
                        newFrame = prevFrameNum + 1;
                        this->field_E_frame_change_counter = 0;
                    }
                    else
                    {
                        newFrame = pAnimHeader->field_2_num_frames - 1;
                    }
                    this->field_92_current_frame = newFrame;
                }
                if (!this->field_92_current_frame)
                {
                    tmpFlags1 = flags | AnimFlags::eBit2_Animate;
                LABEL_22:
                    // TODO
                    field_4_flags.Raw().words.hiword = tmpFlags1;
                    //field_4_flags &= 0xFFFF;
                    //field_4_flags |= tmpFlags1;
                    //HIWORD(this->field_4_flags) = tmpFlags1;

                    pFrameInfo = Get_FrameHeader_40B730(-1);
                    pAnimData = this->field_20_ppBlock;
                    v13 = (int)*pAnimData;
                    pFrameHeader = (FrameHeader *)(pFrameInfo->field_0_frame_header_offset + v13);

                    if (field_4_flags.Get(AnimFlags::eBit11))
                    {
                        field_4_flags.Toggle(AnimFlags::eBit10);
                    }

                    // TODO fix me
                    
                    if (pFrameInfo->field_6_count > 0)
                    {
                        //abort();
                        Invoke_CallBacks_40B7A0(); // v12
                    }
                    

                    vram_w = this->field_84_vram_rect.w;
                    if (vram_w)
                    {
                        if (field_4_flags.Get(AnimFlags::eBit13))
                        {
                            width_rounded = (pFrameHeader->field_4_width + 3) / 2;
                        }
                        else if (field_4_flags.Get(AnimFlags::eBit14))
                        {
                            width_rounded = pFrameHeader->field_4_width + 1;
                        }
                        else
                        {
                            width_rounded = (pFrameHeader->field_4_width + 7) / 4;
                        }
                        vram_x = this->field_84_vram_rect.x;
                        vram_width = width_rounded & 0xFFFFFFFE;
                        rect.y = this->field_84_vram_rect.y;
                        rect.x = vram_x;
                        rect.w = vram_width;
                        rect.h = pFrameHeader->field_5_height;
                        if (vram_width > vram_w)
                        {
                            rect.w = vram_w;
                        }
                        if ((unsigned __int16)pFrameHeader->field_5_height > this->field_84_vram_rect.h)
                        {
                            rect.h = this->field_84_vram_rect.h;
                        }
                        switch (pFrameHeader->field_7_compression_type)
                        {
                        case 0:
                            // TODO
                            //HIBYTE(this->field_4_flags) |= 1u;
                            PSX_LoadImage_4F5FB0(&rect, (BYTE *)&pFrameHeader->field_8_width2);
                            break;
                        case 1:
                            abort();
                            /*
                            // Dead case - no source data can ever hit this
                            pDBuf = this->field_24_dbuf;
                            // TODO
                            //HIBYTE(this->field_4_flags) |= 1u;
                            pData = *(DWORD *)&pFrameHeader->field_8_width2;
                            if (pDBuf
                                || (pAllocatedDBuf = ResourceManager::Alloc_New_Resource_49BED0('fuBD', 0, this->field_28_dbuf_size),
                                (this->field_24_dbuf = pAllocatedDBuf) != 0))
                            {
                                Animation::decompress_type_1_40A610(
                                    (int)&pFrameHeader[1],
                                    *this->field_24_dbuf,
                                    pData,
                                    2 * vram_width * pFrameHeader->field_5_height);
                                PSX_LoadImage_4F5FB0(&rect, *this->field_24_dbuf);
                            }*/
                            break;
                        case 2:
                            pDBuf2 = this->field_24_dbuf;
                            // TODO
                            //HIBYTE(this->field_4_flags) |= 1u;
                            if (pDBuf2
                                || (pAllocatedDBuf2 = ResourceManager::Alloc_New_Resource_49BED0(
                                    1718960708,
                                    0,
                                    this->field_28_dbuf_size),
                                    (this->field_24_dbuf = pAllocatedDBuf2) != 0))
                            {
                                // TODO
                                abort();
                                /*
                                Animation::decompress_type_2_40AA50(
                                    (unsigned __int8 *)&pFrameHeader[1],
                                    *this->field_24_dbuf,
                                    2 * vram_width * pFrameHeader->field_5_height);
                                PSX_LoadImage_4F5FB0(&rect, *this->field_24_dbuf);
                                */
                            }
                            break;
                        case 3:
                            if (this->field_4_flags.Raw().all & 0x1000000)
                            {
                                if (this->field_24_dbuf
                                    || (pDBuf3 = ResourceManager::Alloc_New_Resource_49BED0(1718960708, 0, this->field_28_dbuf_size),
                                    (this->field_24_dbuf = pDBuf3) != 0))
                                {
                                    CompressionType_3Ae_Decompress_40A6A0((const BYTE*)&pFrameHeader->field_8_width2, *this->field_24_dbuf);
                                    goto LABEL_55;
                                }
                            }
                            break;
                        case 4:
                        case 5:
                            pData2 = (int)&pFrameHeader->field_8_width2;
                            
                            if (!field_24_dbuf)
                            {
                                field_24_dbuf = ResourceManager::Alloc_New_Resource_49BED0(0x66754244, 0, this->field_28_dbuf_size);
                            }

                            if (field_24_dbuf)
                            {
                                CompressionType_4Or5_Decompress_4ABAB0((BYTE *)pData2, *field_24_dbuf);
                                goto LABEL_55;
                            }
                            break;
                        case 6:
                            if (this->field_4_flags.Raw().all & 0x1000000)
                            {
                                if (this->field_24_dbuf
                                    || (pDBuf5 = ResourceManager::Alloc_New_Resource_49BED0(1718960708, 0, this->field_28_dbuf_size),
                                    (this->field_24_dbuf = pDBuf5) != 0))
                                {
                                    // TODO:
                                    abort();
                                    //Animation::decompress_type_6_40A8A0(&pFrameHeader->field_8_width2, *this->field_24_dbuf);
                                LABEL_55:
                                    PSX_LoadImage_4F5FB0(&rect, *this->field_24_dbuf);
                                }
                            }
                            break;
                        case 7:
                        case 8:
                            // TODO
                            //HIBYTE(this->field_4_flags) |= 1u;
                            pData3 = *(DWORD *)&pFrameHeader->field_8_width2;
                            if (this->field_24_dbuf
                                || (pDBuf6 = ResourceManager::Alloc_New_Resource_49BED0(1718960708, 0, this->field_28_dbuf_size),
                                (this->field_24_dbuf = pDBuf6) != 0))
                            {
                                totalSize = 2 * vram_width * pFrameHeader->field_5_height;
                                // TODO
                                abort();
                                /*
                                for (i = Compression_type_7_8_4ABB90(
                                    &pFrameHeader[1],
                                    *this->field_24_dbuf,
                                    pData3,
                                    pFrameHeader->field_7_compression_type != 8 ? 8 : 6);
                                    i < totalSize;
                                    (*this->field_24_dbuf)[i - 1] = 0)
                                {
                                    ++i;
                                }
                                */
                                PSX_LoadImage_4F5FB0(&rect, *this->field_24_dbuf);
                            }
                            break;
                        default:
                            return;
                        }
                    }
                    return;
                }
            }
            else
            {
                // Loop forwards
                nextFrameNum = ++this->field_92_current_frame;
                this->field_E_frame_change_counter = this->field_10_frame_delay;

                // Animation reached end point
                if (nextFrameNum >= (signed int)pAnimHeader->field_2_num_frames)
                {
                    if (field_4_flags.Get(AnimFlags::eBit8_Loop))
                    {
                        // Loop back to loop start frame
                        endFrame = pAnimHeader->field_4_loop_start_frame;
                    }
                    else
                    {
                        // Do not loop
                        endFrame = nextFrameNum - 1;
                        this->field_E_frame_change_counter = 0;
                    }
                    
                    this->field_92_current_frame = endFrame;

                    // TODO
                    //HIBYTE(flags1) |= 8u;
                    //LOWORD(this->field_4_flags) = flags1;

                    // Set the "finished once" flag ?
                    field_4_flags.Set(AnimFlags::eBit12);
                }

                // Is last frame ?
                if (this->field_92_current_frame == pAnimHeader->field_2_num_frames - 1)
                {
                    tmpFlags1 = flags | AnimFlags::eBit2_Animate;
                    goto LABEL_22;
                }
            }

            tmpFlags1 = flags & ~AnimFlags::eBit2_Animate;
            goto LABEL_22;
        }
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

                //pAnim->field_4_flags |= AnimFlags::eBit12;
                //pAnim->field_4_flags &= ~AnimFlags::eBit12;

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
