#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityDoor.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "GameSpeak.hpp"
#include "Events.hpp"
#include "Midi.hpp"
#include "Slig.hpp"
#include "Abe.hpp"
#include "Animation.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "CheatController.hpp"
#include "magic_enum/include/magic_enum.hpp"

namespace AO {

void SecurityDoor::VScreenChanged()
{
    VScreenChanged_461F80();
}

void SecurityDoor::VUpdate()
{
    VUpdate_461AD0();
}

SecurityDoor::~SecurityDoor()
{
    if (field_E8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_E8_state = SecurityDoorStates::eInactive_0;
    }
    gMap.TLV_Reset(field_E4_tlvInfo, field_E8_state + 1, 0, 0);
}

SecurityDoor::SecurityDoor(Path_SecurityDoor* pTlv, s32 tlvInfo)
{
    field_F4_event_idx = -1;
    field_108_max_idx = 0;
    field_10A_event_idx = -1;

    const AnimRecord rec = AO::AnimRec(AnimId::Security_Door);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_E4_tlvInfo = tlvInfo;

    field_C8_yOffset = 0;
    field_10_anim.field_C_layer = Layer::eLayer_BeforeWell_22;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    field_EA_switch_id = pTlv->field_1A_switch_id;
    field_EC_code_converted = Code_Convert(pTlv->field_1C_code_1, pTlv->field_1E_code2);
    field_F0_code_len = Code_Length(field_EC_code_converted);
    field_10C_top_left = pTlv->field_10_top_left;
    field_110_bottom_right = pTlv->field_14_bottom_right;
    field_A8_xpos = FP_FromInteger(pTlv->field_20_xpos);
    field_AC_ypos = FP_FromInteger(pTlv->field_22_ypos);

    if (pTlv->field_1_unknown)
    {
        field_E8_state = pTlv->field_1_unknown - 1;
    }
    else
    {
        field_E8_state = SecurityDoorStates::eInactive_0;
    }

    if (field_E8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_114_timer = gnFrameCount_507670 + 10;
    }
}

void SecurityDoor::VScreenChanged_461F80()
{
    mFlags.Set(BaseGameObject::eDead);
}

Bool32 SecurityDoor::IsPlayerNear()
{
    const s16 xpos = FP_GetExponent(sControlledCharacter_50767C->field_A8_xpos);
    const s16 ypos = FP_GetExponent(sControlledCharacter_50767C->field_AC_ypos);

    if (xpos < field_10C_top_left.field_0_x || xpos > field_110_bottom_right.field_0_x)
    {
        return 0;
    }

    if (ypos >= field_10C_top_left.field_2_y && ypos <= field_110_bottom_right.field_2_y)
    {
        return 1;
    }

    return 0;
}

void SecurityDoor::VUpdate_461AD0()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mFlags.Set(Options::eDead);
    }

    switch (field_E8_state)
    {
        case SecurityDoorStates::eInactive_0:
            if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                if (IsPlayerNear())
                {
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                    field_E8_state = SecurityDoorStates::eSayingHi_2;
                }
                else
                {
                    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
                }
            }
            break;

        case SecurityDoorStates::eSuccessChime_1:
            if (static_cast<s32>(gnFrameCount_507670) == field_114_timer)
            {
                SND_SEQ_Play_477760(SeqId::eSaveTriggerMusic_45, 1, 127, 127);
            }
            break;

        case SecurityDoorStates::eSayingHi_2:
            Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eHi_0, 127, -200, 0);
            field_10_anim.Set_Animation_Data(992, 0);
            field_E8_state = SecurityDoorStates::eListeningForHi_3;
            field_114_timer = gnFrameCount_507670 + 150;
            break;

        case SecurityDoorStates::eListeningForHi_3:
        {
            GameSpeakEvents last_event = {};
            if (field_F4_event_idx == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_event = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_event = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                last_event = pEventSystem_4FF954->field_10_last_event;
                field_F4_event_idx = pEventSystem_4FF954->field_18_last_event_index;
            }

            if (last_event == GameSpeakEvents::Slig_Hi_23)
            {
                field_E8_state = SecurityDoorStates::eWaitingToSayPassword_4;
                field_114_timer = gnFrameCount_507670 + 30;
            }
            else if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                field_E8_state = SecurityDoorStates::eInactive_0;
            }
            break;
        }

        case SecurityDoorStates::eWaitingToSayPassword_4:
            if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                field_E8_state = SecurityDoorStates::ePreparingToSayPassword_5;
            }
            break;

        case SecurityDoorStates::ePreparingToSayPassword_5:
            field_118_max_idx = 0;
            field_108_max_idx = GameSpeak::sub_40FA60(field_EC_code_converted, field_F8_stru);
            field_E8_state = SecurityDoorStates::eSayingPassword_6;
            break;

        case SecurityDoorStates::eSayingPassword_6:
        {
            const GameSpeakEvents code = Code_LookUp(field_EC_code_converted, field_118_max_idx, field_F0_code_len);
            switch (code)
            {
                case GameSpeakEvents::eUnknown_5:
                    Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eBullshit_4, 127, -100, 0);
                    break;
                case GameSpeakEvents::Slig_LookOut_6:
                    Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eLookOut_5, 127, -100, 0);
                    break;
                case GameSpeakEvents::eUnknown_7:
                    Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eBullshit2_6, 127, -100, 0);
                    break;
                case GameSpeakEvents::eUnknown_8:
                    Slig::Slig_GameSpeak_SFX_46F560(SligSpeak::eHereBoy_1, 127, -100, 0);
                    break;
                default:
                    LOG_WARNING("eSayingPassword_6 code was " << magic_enum::enum_name(code) << ". This is unhandled.");
                    break;
            }

            field_10_anim.Set_Animation_Data(992, 0);

            field_118_max_idx++;
            if (field_118_max_idx >= field_F0_code_len)
            {
                field_E8_state = SecurityDoorStates::eListeningForPassword_8;
                field_114_timer = gnFrameCount_507670 + 60;
            }
            else
            {
                field_E8_state = SecurityDoorStates::ePausing_7;
                field_114_timer = gnFrameCount_507670 + 30;
            }
            break;
        }

        case SecurityDoorStates::ePausing_7:
            if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                field_E8_state = SecurityDoorStates::eSayingPassword_6;
            }
            break;

        case SecurityDoorStates::eListeningForPassword_8:
        {
            if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                SFX_Play_Pitch(SoundEffect::Alarm_45, 60, -720, 0);
                field_E8_state = SecurityDoorStates::eLaughAtFailure_10;
                field_114_timer = gnFrameCount_507670 + 15;
            }

            GameSpeakEvents last_event = {};
            if (field_F4_event_idx == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_event = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_event = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                last_event = pEventSystem_4FF954->field_10_last_event;
                field_F4_event_idx = pEventSystem_4FF954->field_18_last_event_index;
            }

            if (last_event != GameSpeakEvents::eNone_m1)
            {
                field_E8_state = SecurityDoorStates::eCheckingIfPasswordMatches_9;
            }
            break;
        }

        case SecurityDoorStates::eCheckingIfPasswordMatches_9:
        {
            GameSpeakEvents last_event = {};
            if (field_F4_event_idx == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    last_event = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    last_event = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_F4_event_idx = pEventSystem_4FF954->field_18_last_event_index;
                last_event = pEventSystem_4FF954->field_10_last_event;
            }

            if (last_event == GameSpeakEvents::eNone_m1)
            {
                const auto MatchBuffer = pEventSystem_4FF954->MatchBuffer(field_F8_stru, field_108_max_idx, field_10A_event_idx);
                field_11A_unused = static_cast<s16>(MatchBuffer);
                if (MatchBuffer == GameSpeakMatch::eFullMatch_1 || sVoiceCheat_507708)
                {
                    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
                    SwitchStates_Set(field_EA_switch_id, 1);
                    SFX_Play_Pitch(SoundEffect::SligBleh_112, 127, -700, 0);
                    field_E8_state = SecurityDoorStates::eSuccessChime_1;
                    field_114_timer = gnFrameCount_507670 + 15;
                }
                else
                {
                    SFX_Play_Pitch(SoundEffect::Alarm_45, 60, -720, 0);
                    field_E8_state = SecurityDoorStates::eLaughAtFailure_10;
                    field_114_timer = gnFrameCount_507670 + 15;
                }
            }
            break;
        }

        case SecurityDoorStates::eLaughAtFailure_10:
            if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                SFX_Play_Pitch(SoundEffect::SligLaugh_110, 127, -1000, 0);
                field_E8_state = SecurityDoorStates::eInactive_0;
                field_114_timer = gnFrameCount_507670 + 90;
            }
            break;

        default:
            LOG_WARNING("SecurityDoor field_E8_state was " << field_E8_state << ". This is unhandled.");
            return;
    }
}


} // namespace AO
