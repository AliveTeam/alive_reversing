#include "stdafx.h"
#include "StatusLight.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Collisions.hpp"
#include "SwitchStates.hpp"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "Grid.hpp"

StatusLight::StatusLight(Path_StatusLight* pTlv, u32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_F4_tlvInfo = tlvInfo;
    field_F8_switch_id = pTlv->field_10_switch_id;

    if (pTlv->field_12_scale != Scale_short::eFull_0)
    {
        if (pTlv->field_12_scale == Scale_short::eHalf_1)
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
            mBaseAnimatedWithPhysicsGameObject_Scale = 0;
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    }

    field_FA_id1 = pTlv->field_14_id1;
    field_FC_id2 = pTlv->field_16_id2;
    field_FE_id3 = pTlv->field_18_id3;
    field_100_id4 = pTlv->field_1A_id4;
    field_102_id5 = pTlv->field_1C_id5;
    field_104_bIgnore_grid_snapping = pTlv->field_1E_bIgnore_grid_snapping;

    const AnimRecord& rec = AnimRec(AnimId::Status_Light_Red);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mApplyShadows &= ~1u;
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);

    if (field_104_bIgnore_grid_snapping == Choice_short::eNo_0)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));
    }

    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger((pTlv->field_8_top_left.field_2_y + pTlv->field_C_bottom_right.field_2_y) / 2);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(56)),
        &pPathLine,
        &hitX,
        &hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == 1 ? kFgFloor : kBgFloor); // TODO: mouze check, 0xF1 : 0x10 seemed like it should be 0x1 : 0x10

    if (field_104_bIgnore_grid_snapping == Choice_short::eNo_0)
    {
        if (bCollision)
        {
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(56));
        }
    }

    field_108_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_10C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
}

void StatusLight::VUpdate()
{
    // TODO: Document how this works

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(sTweakX_5C1BD0) + field_108_xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(sTweakY_5C1BD4) + field_10C_ypos;

    if (SwitchStates_Get(field_F8_switch_id))
    {
        if ((!SwitchStates_Get(field_FA_id1) && field_FA_id1) || (!SwitchStates_Get(field_FC_id2) && field_FC_id2) || (!SwitchStates_Get(field_FE_id3) && field_FE_id3) || (!SwitchStates_Get(field_100_id4) && field_100_id4) || (!SwitchStates_Get(field_102_id5) && field_102_id5)
            || (sGnFrame % 8) >= 4)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        }
        const AnimRecord& animRec = AnimRec(AnimId::Status_Light_Green);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    }
    else if (SwitchStates_Get(field_FA_id1) || SwitchStates_Get(field_FC_id2) || SwitchStates_Get(field_FE_id3) || SwitchStates_Get(field_100_id4) || SwitchStates_Get(field_102_id5))
    {
        if ((sGnFrame % 8) >= 4)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        }
        const AnimRecord& animRec = AnimRec(AnimId::Status_Light_Red);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        const AnimRecord& animRec = AnimRec(AnimId::Status_Light_Red);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    }
}

StatusLight::~StatusLight()
{
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}

void StatusLight::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
