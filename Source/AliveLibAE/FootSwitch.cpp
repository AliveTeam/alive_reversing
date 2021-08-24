#include "stdafx.h"
#include "Function.hpp"
#include "FootSwitch.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Spark.hpp"
#include "ParticleBurst.hpp"
#include "PathData.hpp"
#include "ObjectIds.hpp"

const TintEntry sFootSwitchTints_5639F4[18] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};


const AnimId sFootSwitchData_547D60[15][2] = {
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Bonewerkz_Idle, AnimId::Foot_Switch_Bonewerkz_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Bonewerkz_Idle, AnimId::Foot_Switch_Bonewerkz_Pressed},
};


FootSwitch* FootSwitch::ctor_4DE090(Path_FootSwitch* pTlv, s32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x547E38);

    field_4_typeId = AETypes::eFootSwitch_56;
    field_100_obj_id = -1;

    const s32 idx = static_cast<s32>(gMap_5C3030.field_0_current_level);

    const AnimRecord& rec = AnimRec(sFootSwitchData_547D60[idx][0]);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_25;

    SetTint_425600(sFootSwitchTints_5639F4, gMap_5C3030.field_0_current_level);

    field_FA_id = pTlv->field_10_id;

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_Half_6;
    }

    field_FC_action = pTlv->field_14_action;
    field_FE_trigger_by = pTlv->field_16_trigger_by;
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_F8_state = States::eWaitForStepOnMe_0;
    field_DC_bApplyShadows |= 2u;
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_104_bCreateSparks = 0;
    field_F4_tlvInfo = tlvInfo;
    field_106_bFindStander = 1;
    return this;
}

BaseGameObject* FootSwitch::VDestructor(s32 flags)
{
    return vdtor_4DE240(flags);
}

FootSwitch* FootSwitch::vdtor_4DE240(s32 flags)
{
    dtor_4DE670();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void FootSwitch::dtor_4DE670()
{
    SetVTable(this, 0x547E38);
    field_100_obj_id = -1;
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void FootSwitch::VUpdate()
{
    vUpdate_4DE270();
}

void FootSwitch::vUpdate_4DE270()
{
    auto pLastStoodOnMe = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_100_obj_id));
    if (field_106_bFindStander)
    {
        field_106_bFindStander = FALSE;
        pLastStoodOnMe = WhoIsStoodOnMe_4DE700();
        if (pLastStoodOnMe)
        {
            field_100_obj_id = pLastStoodOnMe->field_8_object_id;
            const AnimRecord& animRec = AnimRec(sFootSwitchData_547D60[static_cast<s32>(gMap_5C3030.field_0_current_level)][1]);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            field_F8_state = States::eWaitForGetOffMe_1;
        }
    }

    switch (field_F8_state)
    {
        case States::eWaitForStepOnMe_0:
        {
            auto pStoodOnMeNow = WhoIsStoodOnMe_4DE700();
            if (pStoodOnMeNow)
            {
                field_100_obj_id = pStoodOnMeNow->field_8_object_id;

                SwitchStates_Do_Operation_465F00(field_FA_id, field_FC_action);
                field_F8_state = States::eWaitForGetOffMe_1;

                const AnimRecord& animRec = AnimRec(sFootSwitchData_547D60[static_cast<s32>(gMap_5C3030.field_0_current_level)][1]);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

                auto pParticleBurst = ae_new<ParticleBurst>();
                if (pParticleBurst)
                {
                    pParticleBurst->ctor_41CF50(
                        field_B8_xpos,
                        field_BC_ypos + FP_FromInteger(10),
                        3,
                        field_CC_sprite_scale,
                        BurstType::eBigRedSparks_3,
                        9);
                }

                if (gMap_5C3030.field_0_current_level == LevelIds::eMines_1 || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8 || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5 || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6 || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9)
                {
                    SFX_Play_46FBA0(SoundEffect::IndustrialTrigger_80, 30, 400);
                    SFX_Play_46FBA0(SoundEffect::IndustrialNoise1_76, 60, 800);
                }
                else
                {
                    SFX_Play_46FA90(SoundEffect::FootSwitchPress_55, 0);
                }
            }

            if (field_20_animation.field_92_current_frame == 0)
            {
                field_104_bCreateSparks = 1;
                return;
            }

            if (field_104_bCreateSparks)
            {
                auto pSpark = ae_new<Spark>();
                if (pSpark)
                {
                    pSpark->ctor_4CBBB0(
                        field_B8_xpos,
                        field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(6)),
                        field_CC_sprite_scale,
                        10,
                        100,
                        255,
                        0);
                }

                auto pParticleBurst = ae_new<ParticleBurst>();
                if (pParticleBurst)
                {
                    pParticleBurst->ctor_41CF50(
                        field_B8_xpos,
                        field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(10)),
                        1,
                        field_CC_sprite_scale,
                        BurstType::eBigRedSparks_3,
                        9);
                }

                field_104_bCreateSparks = 0;
            }

            if (field_20_animation.field_92_current_frame == 0)
            {
                field_104_bCreateSparks = 1;
            }
            break;
        }

        case States::eWaitForGetOffMe_1:
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            // Have they left the switch or died?
            if (!pLastStoodOnMe || // OG bug: If thing on the switch had died this would de-ref null and crash
                pLastStoodOnMe->field_B8_xpos < FP_FromInteger(bRect.x) || pLastStoodOnMe->field_B8_xpos > FP_FromInteger(bRect.w) || pLastStoodOnMe->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                field_F8_state = States::eWaitForStepOnMe_0;
                const AnimRecord& animRec = AnimRec(sFootSwitchData_547D60[static_cast<s32>(gMap_5C3030.field_0_current_level)][0]);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                field_100_obj_id = -1;
            }
            break;
        }

        default:
            break;
    }
}

void FootSwitch::VScreenChanged()
{
    vScreenChanged_4DE650();
}

void FootSwitch::vScreenChanged_4DE650()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

BaseAliveGameObject* FootSwitch::WhoIsStoodOnMe_4DE700()
{
    PSX_RECT bRectSwitch = {};
    vGetBoundingRect_424FD0(&bRectSwitch, 1);
    bRectSwitch.y -= 3;

    if (field_FE_trigger_by == FootSwitchTriggerBy::eAnyone_1)
    {
        for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                PSX_RECT theirRect = {};
                pAliveObj->vGetBoundingRect_424FD0(&theirRect, 1);

                const s32 xpos = FP_GetExponent(pAliveObj->field_B8_xpos);

                if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= theirRect.w && bRectSwitch.w >= theirRect.x && bRectSwitch.h >= theirRect.y && bRectSwitch.y <= theirRect.h && pAliveObj->field_D6_scale == field_D6_scale)
                {
                    return pAliveObj;
                }
            }
        }
    }
    else if (field_FE_trigger_by == FootSwitchTriggerBy::eOnlyAbe_0)
    {
        PSX_RECT bRect = {};
        sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bRect, 1);

        const s32 xpos = FP_GetExponent(sActiveHero_5C1B68->field_B8_xpos);

        if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= bRect.w && bRectSwitch.w >= bRect.x && bRectSwitch.h >= bRect.y && bRectSwitch.y <= bRect.h && sActiveHero_5C1B68->field_D6_scale == field_D6_scale)
        {
            return sActiveHero_5C1B68;
        }
    }

    return nullptr;
}
