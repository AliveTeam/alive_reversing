#include "stdafx_ao.h"
#include "Function.hpp"
#include "Lever.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

namespace AO {

struct Lever_Data final
{
    AnimId field_0_idle_animId;
    s32 field_4_maxW;
    s32 field_8_maxH;
    AnimId field_C_pulling_left_animId;
    AnimId field_10_releasing_left_animId;
    AnimId field_14_pulling_right_animId;
    AnimId field_18_releasing_right_animId;
};
ALIVE_ASSERT_SIZEOF(Lever_Data, 0x1C);

const Lever_Data gLeverData_4BCF40[16] = {
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // menu
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // rupture farms
    {AnimId::Lines_Lever_Idle, 66, 41, AnimId::Lines_Lever_Pull_Left, AnimId::Lines_Lever_Pull_Release_Left, AnimId::Lines_Lever_Pull_Right, AnimId::Lines_Lever_Pull_Release_Right},                                    // lines
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest temple
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // stock yards
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // stock yards return
    {AnimId::Lines_Lever_Idle, 66, 41, AnimId::Lines_Lever_Pull_Left, AnimId::Lines_Lever_Pull_Release_Left, AnimId::Lines_Lever_Pull_Right, AnimId::Lines_Lever_Pull_Release_Right},                                    // removed
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // desert
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // desert temple
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // credits
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // removed
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // board room
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // rupture farms return
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest chase
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right}};                                                                 // desert escape

s32 Lever::VPull(s16 bLeftDirection)
{
    return vPull_481640(bLeftDirection);
}

void Lever::VUpdate_4812D0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_E8_state == LeverState::ePulled_1)
    {
        if (field_10_anim.field_92_current_frame == 3)
        {
            SFX_Play_43AD70(SoundEffect::LeverPull_75, 0, 0);
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            Event_Broadcast_417220(kEventNoise_0, this);
            Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
            const s32 lvl_idx = static_cast<s32>(gMap_507BA8.field_0_current_level);
            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1
                || gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12
                || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                SFX_Play_43AD70(SoundEffect::IndustrialTrigger_97, 60, 0);
            }
            field_E8_state = LeverState::eFinished_2;

            AnimId animId = AnimId::None;
            if (field_F0_bPulledFromLeft == 0)
            {
                animId = gLeverData_4BCF40[lvl_idx].field_18_releasing_right_animId;
            }
            else
            {
                animId = gLeverData_4BCF40[lvl_idx].field_10_releasing_left_animId;
            }

            const AnimRecord& rec = AO::AnimRec(animId);
            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);

            const auto oldSwitchState = SwitchStates_Get(field_E4_switch_id);
            SwitchStates_Do_Operation_436A10(field_E4_switch_id, field_F2_action);
            const auto newSwitchState = SwitchStates_Get(field_E4_switch_id);

            if (oldSwitchState != newSwitchState)
            {
                s32 leftVol = 0;
                s32 rightVol = 0;

                if (field_F8_sound_direction == LeverSoundDirection::eLeft_1)
                {
                    leftVol = 1;
                    rightVol = 0;
                }
                else if (field_F8_sound_direction == LeverSoundDirection::eRight_2)
                {
                    leftVol = 0;
                    rightVol = 1;
                }
                else
                {
                    leftVol = 1;
                    rightVol = 1;
                }

                if (SwitchStates_Get(field_E4_switch_id))
                {
                    switch (field_F4_on_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_43ADE0(SoundEffect::WellExit_24, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                            break;
                        case LeverSoundType::eSwitchBellHammer_2:
                            SFX_Play_43ADE0(SoundEffect::SwitchBellHammer_12, 100 * leftVol + 25, 100 * rightVol + 25, 0);
                            return;
                        case LeverSoundType::eDoor_3:
                            SFX_Play_43ADE0(SoundEffect::DoorEffect_66, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                            break;
                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_43ADE0(SoundEffect::Zap1_57, 35 * leftVol + 25, 35 * rightVol + 25, 0);
                            break;
                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_43ADE0(SoundEffect::SecurityOrb_56, 35 * leftVol + 25, 35 * rightVol + 25, 0);
                            break;
                        default:
                            return;
                    }
                }
                else
                {
                    switch (field_F6_off_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_43ADE0(SoundEffect::WellExit_24, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                            break;
                        case LeverSoundType::eSwitchBellHammer_2:
                            SFX_Play_43ADE0(SoundEffect::SwitchBellHammer_12, 110 * leftVol + 25, 110 * rightVol + 25, 0);
                            break;
                        case LeverSoundType::eDoor_3:
                            SFX_Play_43ADE0(SoundEffect::DoorEffect_66, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                            break;
                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_43ADE0(SoundEffect::Zap1_57, 80 * leftVol + 25, 80 * rightVol + 25, 0);
                            break;
                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_43ADE0(SoundEffect::SecurityOrb_56, 35 * leftVol + 75, 35 * rightVol + 75, 0);
                            break;
                        default:
                            return;
                    }
                }
            }
        }
    }
    else if (field_E8_state == LeverState::eFinished_2)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_E8_state = LeverState::eWaiting_0;
            const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[static_cast<s32>(gMap_507BA8.field_0_current_level)].field_0_idle_animId);
            field_10_anim.Set_Animation_Data_402A40(
                rec.mFrameTableOffset,
                nullptr);
        }
    }
}

void Lever::VUpdate()
{
    VUpdate_4812D0();
}

void Lever::VScreenChanged_4816F0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Lever::VScreenChanged()
{
    VScreenChanged_4816F0();
}

Lever* Lever::Vdtor_481700(s32 flags)
{
    dtor_481260();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Lever::VDestructor(s32 flags)
{
    return Vdtor_481700(flags);
}

BaseGameObject* Lever::dtor_481260()
{
    SetVTable(this, 0x4BD100);
    gMap_507BA8.TLV_Reset_446870(field_EC_tlvInfo, -1, 0, 0);
    return dtor_417D10(); // Note: intermediate base skipped
}

Lever* Lever::ctor_481110(Path_Lever* pTlv, s32 tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BD100);

    field_4_typeId = Types::eLever_97;
    const s32 lvl_idx = static_cast<s32>(gMap_507BA8.field_0_current_level);
    const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[lvl_idx].field_0_idle_animId);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(
        rec.mFrameTableOffset,
        rec.mMaxW,
        rec.mMaxH,
        ppRes,
        1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_A8_xpos = FP_FromInteger((pTlv->field_14_bottom_right.field_0_x
                                    + pTlv->field_10_top_left.field_0_x)
                                   / 2);

    field_E4_switch_id = pTlv->field_18_switch_id;
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_F2_action = pTlv->field_1A_action;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_Half_6;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_25;
        field_C6_scale = 1;
    }

    field_F4_on_sound = pTlv->field_1E_on_sound;
    field_F6_off_sound = pTlv->field_20_off_sound;
    field_EC_tlvInfo = tlvInfo;
    field_F8_sound_direction = pTlv->field_22_sound_direction;

    field_E8_state = LeverState::eWaiting_0;

    return this;
}

s32 Lever::vPull_481640(s16 bLeftDirection)
{
    if (field_E8_state == LeverState::eWaiting_0)
    {
        const s32 lvl_idx = static_cast<s32>(gMap_507BA8.field_0_current_level);
        field_E8_state = LeverState::ePulled_1;
        if (bLeftDirection)
        {
            const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[lvl_idx].field_C_pulling_left_animId);
            field_10_anim.Set_Animation_Data_402A40(
                rec.mFrameTableOffset,
                nullptr);
            field_F0_bPulledFromLeft = 1;
        }
        else
        {
            const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[lvl_idx].field_14_pulling_right_animId);
            field_10_anim.Set_Animation_Data_402A40(
                rec.mFrameTableOffset,
                nullptr);
            field_F0_bPulledFromLeft = 0;
        }
    }
    return SwitchStates_Get(field_E4_switch_id);
}

} // namespace AO
