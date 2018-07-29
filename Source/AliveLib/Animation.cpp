#include "stdafx.h"
#include "Animation.hpp"
#include "Function.hpp"
#include "Map.hpp"

void Animation::Animation__vdecode_40AC90()
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

signed __int16 Animation::Set_Animation_Data_409C80(int frameTableOffset, BYTE ** pAnimRes)
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

        if (pAnim->field_4_flags & 2)
        {
            if (pAnim->field_E_frame_change_counter > 0)
            {
                pAnim->field_E_frame_change_counter--;
                if (pAnim->field_E_frame_change_counter == 0)
                {
                    pAnim->Animation__vdecode_40AC90();
                }
            }
        }
    }
}

struct AnimationHeader
{
    // Meta data - the offset where this record was read from
    WORD mFps = 0;            // Seems to be 0x1 or 0x2
    WORD mNumFrames = 0;      // Number of frames in the set

    // If loop flag set then this is the frame to loop back to
    WORD mLoopStartFrame = 0;

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

void AnimationEx::SetFrame_409D50(__int16 newFrame)
{
    if (field_20_ppBlock)
    {
        if (newFrame == -1)
        {
            newFrame = 0;
        }

        AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset); // TODO: Make getting offset to animation header cleaner

        if (newFrame > pHead->mNumFrames)
        {
            newFrame = pHead->mNumFrames;
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

void AnimationEx::Get_Frame_Rect_409E10(PSX_RECT * pRect)
{
    NOT_IMPLEMENTED();
}

WORD AnimationEx::Get_Frame_Count_40AC70()
{
    AnimationHeader* pHead = reinterpret_cast<AnimationHeader*>(*field_20_ppBlock + field_18_frame_table_offset);  // TODO: Make getting offset to animation header cleaner
    return pHead->mNumFrames;
}

signed __int16 AnimationEx::Init_40A030(int frameTableOffset, DynamicArray * animList, void * pGameObj, int maxW, unsigned __int16 maxH, BYTE ** ppAnimData, unsigned __int8 unknown1, signed int pal_depth, char unknown3)
{
    NOT_IMPLEMENTED();
    return 0;
}

void AnimationEx::Load_Pal_40A530(BYTE ** pAnimData, int palOffset)
{
    NOT_IMPLEMENTED();
}

/*
void AnimationEx::Get_Bounding_Rect_Top_Left_40C480(signed __int16 frameNum, __int16* pBoundingX, __int16* pBoundingY)
{
     FrameInfoHeader* pFrameHeader = Get_FrameHeader_40B730(frameNum);
    *pBoundingX = pFrameHeader->mTopLeft.x;
    *pBoundingY = pFrameHeader->mTopLeft.y;
}
*/

void BackgroundAnimation::ctor_40D270(BackgroundAnimation_Params* pPathParams, int a3)
{
    //BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5440F0);

    field_4_typeId = Types::eBackgroundAnimation;
    field_F8_arg_a3 = a3;

    AnimHeader** pAnimHeader;
    /*
    AnimHeader** pAnimHeader = (AnimHeader **)BaseGameObject::Add_resource_4DC130(
        this,
        Resource_Animation,
        pPathParams->field_10_res_id);
        */

    field_F4_res = pAnimHeader;
    if (!pAnimHeader)
    {
        field_6_flags = field_6_flags & 0xFFF7 | 4; // TODO: Enum bits
    }

    field_B8_xpos = FP(pPathParams->field_8_xpos);
    field_BC_ypos = FP(pPathParams->field_A_ypos);

    field_FC_xpos = FP(pPathParams->field_8_xpos);
    field_100_ypos = FP(pPathParams->field_A_ypos);

    /*
    BaseAnimatedWithPhysicsGameObject_Animation_Init_424E10(
        (*pAnimHeader)->field_4_frame_table_offset,
        (*pAnimHeader)->field_0_max_w,
        (*pAnimHeader)->field_2_max_h,
        (AnimHeader *)pAnimHeader,
        1,
        1u);
    */

    //LOWORD(field_20_animation.field_4_flags) = field_20_animation.field_4_flags & 0xBFFF | ((pPathParams->field_12_is_semi_trans & 1) << 14);
    //BYTE1(field_20_animation.field_4_flags) |= 0x80u;

    field_20_animation.field_B_render_mode = static_cast<BYTE>(pPathParams->field_14_semi_trans_mode);

    const int v9 = pPathParams->field_1A_layer;
    if ((WORD)v9)
    {
        const int v10 = v9 - 1;
        if (!v10)
        {
            field_20_animation.field_C_render_layer = 20;
        }
        if (v10 == 1)
        {
            field_20_animation.field_C_render_layer = 39;
        }
    }
    else
    {
        field_20_animation.field_C_render_layer = 1;
    }
}

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject(signed __int16 resourceArraySize)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(resourceArraySize);
}

BaseAnimatedWithPhysicsGameObject * BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_ctor_424930(signed __int16 resourceArraySize)
{
    BaseGameObject_ctor_4DBFA0(1, resourceArraySize);
    
    SetVTable(&field_20_animation, 0x544290); // gVtbl_animation_2a_544290
    SetVTable(this, 0x544C9C); // gVtbl_BaseAnimatedWithPhysicsGameObject_544C9C

    field_C4_velx = 0;
    field_C8_vely = 0;

    field_B8_xpos = FP(0);
    field_BC_ypos = FP(0);

    field_D4_b = 127;
    field_D2_g = 127;
    field_D0_r = 127;

    field_6_flags &= ~(BaseGameObject::eInteractive | BaseGameObject::eCanExplode);
    field_6_flags |= BaseGameObject::eDrawable | BaseGameObject::eIsBaseAnimatedWithPhysicsObj;

    field_DC_bApplyShadows &= ~2;
    field_DC_bApplyShadows |= 1;

    field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
    field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;

    field_CC_sprite_scale = FP(1);

    field_D6_scale = 1;
    field_D8_yOffset = 0;
    field_DA_xOffset = 0;

    field_E0_176_ptr = nullptr;

    return this;
}

void BaseAnimatedWithPhysicsGameObject::dtor_408210(signed int flags)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::VDestructor(signed int flags)
{
    dtor_408210(flags);
}

signed __int16 BaseAnimatedWithPhysicsGameObject::Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, AnimHeader * a5, __int16 a6, unsigned __int8 a7)
{
    NOT_IMPLEMENTED();
    return 0;
}

