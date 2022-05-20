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

FootSwitch* FootSwitch::ctor_4887F0(Path_FootSwitch* pTlv, s32 tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BD648);
    field_4_typeId = Types::eFootSwitch36;

    const AnimRecord& rec = AO::AnimRec(AnimId::Foot_Switch_Temple);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_25;

    field_EA_switch_id = pTlv->field_18_switch_id;
    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }

    field_E8_state = States::eWaitForStepOnMe_0;
    field_EC_action = pTlv->field_1C_action;
    field_EE_trigger_by = pTlv->field_1E_trigger_by;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    SwitchStates_Set(field_EA_switch_id, 0);

    field_E4_tlvInfo = tlvInfo;
    return this;
}

BaseGameObject* FootSwitch::VDestructor(s32 flags)
{
    return vdtor_488C10(flags);
}

FootSwitch* FootSwitch::vdtor_488C10(s32 flags)
{
    dtor_4889E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* FootSwitch::dtor_4889E0()
{
    SetVTable(this, 0x4BD648);
    if (field_F0_pStoodOnMe)
    {
        field_F0_pStoodOnMe->field_C_refCount--;
        field_F0_pStoodOnMe = nullptr;
    }
    gMap.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

void FootSwitch::VUpdate()
{
    VUpdate_4888E0();
}

void FootSwitch::VUpdate_4888E0()
{
    switch (field_E8_state)
    {
        case States::eWaitForStepOnMe_0:
            field_F0_pStoodOnMe = WhoIsStoodOnMe_488A60();
            if (field_F0_pStoodOnMe)
            {
                field_F0_pStoodOnMe->field_C_refCount++;
                SwitchStates_Do_Operation_436A10(field_EA_switch_id, field_EC_action);
                field_E8_state = States::eWaitForGetOffMe_1;
                field_10_anim.Set_Animation_Data_402A40(756, 0);
                SFX_Play_43AD70(SoundEffect::FootSwitchPress_64, 0, 0);
            }
            break;

        case States::eWaitForGetOffMe_1:
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            if (field_F0_pStoodOnMe->field_A8_xpos < FP_FromInteger(bRect.x) || field_F0_pStoodOnMe->field_A8_xpos > FP_FromInteger(bRect.w) || field_F0_pStoodOnMe->mFlags.Get(BaseGameObject::eDead))
            {
                field_E8_state = States::eWaitForStepOnMe_0;
                field_10_anim.Set_Animation_Data_402A40(744, 0);
                field_F0_pStoodOnMe->field_C_refCount--;
            }
            break;
        }

        default:
            break;
    }
}

void FootSwitch::VScreenChanged()
{
    VScreenChanged_4889D0();
}

void FootSwitch::VScreenChanged_4889D0()
{
    mFlags.Set(BaseGameObject::eDead);
}

BaseAliveGameObject* FootSwitch::WhoIsStoodOnMe_488A60()
{
    PSX_RECT bRectSwitch = {};
    VGetBoundingRect(&bRectSwitch, 1);
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

            if (pObj->mFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                PSX_RECT theirRect = {};
                pAliveObj->VGetBoundingRect(&theirRect, 1);

                const s32 xpos = FP_GetExponent(pAliveObj->field_A8_xpos);

                if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= theirRect.w && bRectSwitch.w >= theirRect.x && bRectSwitch.h >= theirRect.y && bRectSwitch.y <= theirRect.h && pAliveObj->field_BC_sprite_scale == field_BC_sprite_scale)
                {
                    return pAliveObj;
                }
            }
        }
    }
    else if (field_EE_trigger_by == FootSwitchTriggerBy::eAbe_0)
    {
        PSX_RECT bRect = {};
        sActiveHero_507678->VGetBoundingRect(&bRect, 1);

        const s32 xpos = FP_GetExponent(sActiveHero_507678->field_A8_xpos);

        if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= bRect.w && bRectSwitch.w >= bRect.x && bRectSwitch.h >= bRect.y && bRectSwitch.y <= bRect.h && sActiveHero_507678->field_BC_sprite_scale == field_BC_sprite_scale)
        {
            return sActiveHero_507678;
        }
    }
    return nullptr;
}

} // namespace AO
