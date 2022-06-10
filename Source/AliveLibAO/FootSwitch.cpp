#include "stdafx_ao.h"
#include "Function.hpp"
#include "FootSwitch.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"

namespace AO {

FootSwitch::FootSwitch(Path_FootSwitch* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eFootSwitch;

    const AnimRecord& rec = AO::AnimRec(AnimId::Foot_Switch_Temple);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;

    field_EA_switch_id = pTlv->field_18_switch_id;
    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
    }

    field_E8_state = States::eWaitForStepOnMe_0;
    field_EC_action = pTlv->field_1C_action;
    field_EE_trigger_by = pTlv->field_1E_trigger_by;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    SwitchStates_Set(field_EA_switch_id, 0);

    field_E4_tlvInfo = tlvInfo;
}

FootSwitch::~FootSwitch()
{
    if (field_F0_pStoodOnMe)
    {
        field_F0_pStoodOnMe->mBaseGameObjectRefCount--;
        field_F0_pStoodOnMe = nullptr;
    }
    gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

void FootSwitch::VUpdate()
{
    switch (field_E8_state)
    {
        case States::eWaitForStepOnMe_0:
            field_F0_pStoodOnMe = WhoIsStoodOnMe();
            if (field_F0_pStoodOnMe)
            {
                field_F0_pStoodOnMe->mBaseGameObjectRefCount++;
                SwitchStates_Do_Operation(field_EA_switch_id, field_EC_action);
                field_E8_state = States::eWaitForGetOffMe_1;
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(756, 0);
                SFX_Play_Mono(SoundEffect::FootSwitchPress_64, 0, 0);
            }
            break;

        case States::eWaitForGetOffMe_1:
        {
            const PSX_RECT bRect = VGetBoundingRect();

            if (field_F0_pStoodOnMe->mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(bRect.x) || field_F0_pStoodOnMe->mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(bRect.w) || field_F0_pStoodOnMe->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_E8_state = States::eWaitForStepOnMe_0;
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(744, 0);
                field_F0_pStoodOnMe->mBaseGameObjectRefCount--;
            }
            break;
        }

        default:
            break;
    }
}

void FootSwitch::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BaseAliveGameObject* FootSwitch::WhoIsStoodOnMe()
{
    const PSX_RECT bRectSwitch = VGetBoundingRect();
    // NOTE: AE  y -= 3 not done in AO

    if (field_EE_trigger_by == FootSwitchTriggerBy::eAnyone_1)
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                const PSX_RECT theirRect = pAliveObj->VGetBoundingRect();

                const s32 xpos = FP_GetExponent(pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos);

                if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= theirRect.w && bRectSwitch.w >= theirRect.x && bRectSwitch.h >= theirRect.y && bRectSwitch.y <= theirRect.h && pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                {
                    return pAliveObj;
                }
            }
        }
    }
    else if (field_EE_trigger_by == FootSwitchTriggerBy::eAbe_0)
    {
        const PSX_RECT bRect = sActiveHero_507678->VGetBoundingRect();
        const s32 xpos = FP_GetExponent(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos);

        if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= bRect.w && bRectSwitch.w >= bRect.x && bRectSwitch.h >= bRect.y && bRectSwitch.y <= bRect.h && sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
        {
            return sActiveHero_507678;
        }
    }
    return nullptr;
}

} // namespace AO
