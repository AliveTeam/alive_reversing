#include "stdafx.h"
#include "DoorBlocker.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

DoorBlocker::DoorBlocker(Path_DoorBlocker* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    field_11A_switch_id = pTlv->field_12_switch_id;

    const AnimRecord& rec = AnimRec(AnimId::Door_Lock_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_118_bDone &= ~1u;

    SetType(ReliveTypes::eDoorLock);
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);
    field_11C_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_26;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    if (SwitchStates_Get(field_11A_switch_id))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
}

DoorBlocker::~DoorBlocker()
{
    Path::TLV_Reset(field_11C_tlvInfo, -1, 0, 0);
}

void DoorBlocker::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (field_118_bDone & 1)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        else if (SwitchStates_Get(field_11A_switch_id))
        {
            SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, 900);
            SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, -100);
            const AnimRecord& animRec = AnimRec(AnimId::Door_Lock_Open);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, 0);
            field_118_bDone |= 1u;
        }
    }
}
