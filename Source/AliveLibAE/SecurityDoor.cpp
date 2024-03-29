#include "stdafx.h"
#include "SecurityDoor.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "GameSpeak.hpp"
#include "Sound/Midi.hpp"
#include "SwitchStates.hpp"

SecurityDoor* SecurityDoor::ctor_4ABFC0(Path_SecurityDoor* pTlv, s32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    field_104_event_idx = -1;
    field_118_max_idx = 0;
    field_11A_event_idx = -1;

    SetVTable(this, 0x547028);

    const AnimRecord& rec = AnimRec(AnimId::Security_Door_Idle);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_F4_tlvInfo = tlvInfo;

    field_20_animation.field_C_render_layer = Layer::eLayer_BeforeWell_22;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_FA_switch_id = pTlv->field_12_switch_id;
    field_FC_code_converted = Code_Convert_4C9DF0(pTlv->field_14_code_1, pTlv->field_16_code_2);
    field_100_code_len = Code_Length_4C9DB0(field_FC_code_converted);
    field_11C_top_left = pTlv->field_8_top_left;
    field_120_bottom_right = pTlv->field_C_bottom_right;
    field_B8_xpos = FP_FromInteger(pTlv->field_18_xpos);
    field_BC_ypos = FP_FromInteger(pTlv->field_1A_ypos);

    PSX_Point point = {};
    gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&point);

    if (field_B8_xpos > FP_FromInteger(0))
    {
        field_B8_xpos -= FP_FromInteger(point.field_0_x);
    }
    else
    {
        field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    }

    if (field_BC_ypos > FP_FromInteger(0))
    {
        field_BC_ypos -= FP_FromInteger(point.field_2_y);
    }
    else
    {
        field_BC_ypos = FP_FromInteger((pTlv->field_8_top_left.field_2_y + pTlv->field_C_bottom_right.field_2_y) / 2);
    }

    if (pTlv->field_1_tlv_state)
    {
        field_F8_state = static_cast<SecurityDoorStates>(pTlv->field_1_tlv_state - 1);
    }
    else
    {
        field_F8_state = SecurityDoorStates::eInactive_0;
    }

    if (field_F8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_124_timer = sGnFrame_5C1B84 + 10;
    }

    return this;
}

void SecurityDoor::VScreenChanged()
{
    vScreenChanged_4AC970();
}

void SecurityDoor::VUpdate()
{
    vUpdate_4AC380();
}

BaseGameObject* SecurityDoor::VDestructor(s32 flags)
{
    return vdtor_4AC230(flags);
}

void SecurityDoor::dtor_4AC260()
{
    SetVTable(this, 0x547028);
    if (field_F8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_F8_state = SecurityDoorStates::eInactive_0;
    }
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, static_cast<s16>(field_F8_state) + 1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void SecurityDoor::vScreenChanged_4AC970()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

SecurityDoor* SecurityDoor::vdtor_4AC230(s32 flags)
{
    dtor_4AC260();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

s16 SecurityDoor::IsPlayerNear_4AC300()
{
    const s16 xpos = FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos);
    const s16 ypos = FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos);

    if (xpos < field_11C_top_left.field_0_x || xpos > field_120_bottom_right.field_0_x)
    {
        return 0;
    }

    if (ypos >= field_11C_top_left.field_2_y && ypos <= field_120_bottom_right.field_2_y)
    {
        return 1;
    }

    return 0;
}

void SecurityDoor::vUpdate_4AC380()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_F8_state)
    {
        case SecurityDoorStates::eInactive_0:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
            {
                return;
            }

            if (IsPlayerNear_4AC300())
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_F8_state = SecurityDoorStates::eSayingHi_2;
            }
            else
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            }
            return;

        case SecurityDoorStates::eSuccessChime_1:
            if (static_cast<s32>(sGnFrame_5C1B84) == field_124_timer)
            {
                SND_SEQ_Play_4CAB10(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            }
            return;

        case SecurityDoorStates::eSayingHi_2:
        {
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHi_0, 127, -200, 0);
            const AnimRecord& animRec = AnimRec(AnimId::Security_Door_Speak);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            field_F8_state = SecurityDoorStates::eListeningForHi_3;
            field_124_timer = sGnFrame_5C1B84 + 150;
            return;
        }
        case SecurityDoorStates::eListeningForHi_3:
            if (field_104_event_idx != pEventSystem_5BC11C->field_28_last_event_index)
            {
                field_104_event_idx = pEventSystem_5BC11C->field_28_last_event_index;
                if (pEventSystem_5BC11C->field_20_last_event != GameSpeakEvents::eNone_m1 && pEventSystem_5BC11C->field_20_last_event != GameSpeakEvents::eSameAsLast_m2)
                {
                    if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_Hi_27)
                    {
                        field_F8_state = SecurityDoorStates::eWaitingToSayPassword_4;
                        field_124_timer = sGnFrame_5C1B84 + 30;
                        return;
                    }
                    else
                    {
                        field_F8_state = SecurityDoorStates::eFailure_12;
                        field_124_timer = sGnFrame_5C1B84 + 15;
                    }
                }
            }

            if (static_cast<s32>(sGnFrame_5C1B84) > field_124_timer)
            {
                field_F8_state = SecurityDoorStates::eInactive_0;
            }
            return;

        case SecurityDoorStates::eWaitingToSayPassword_4:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_124_timer)
            {
                field_F8_state = SecurityDoorStates::ePreparingToSayPassword_5;
            }
            return;

        case SecurityDoorStates::ePreparingToSayPassword_5:
            field_128_max_idx = 0;
            field_118_max_idx = static_cast<s16>(GameSpeak::FillBuffer_421970(field_FC_code_converted, field_108_stru));
            field_F8_state = SecurityDoorStates::eSayingPassword_6;
            return;

        case SecurityDoorStates::eSayingPassword_6:
        {
            const GameSpeakEvents code = Code_LookUp_4C9E40(field_FC_code_converted, field_128_max_idx, field_100_code_len);
            switch (code)
            {
                case GameSpeakEvents::Slig_BS_5:
                    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eBullshit_5, 127, -100, nullptr);
                    break;

                case GameSpeakEvents::Slig_Laugh_8:
                    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eLaugh_3, 127, -100, nullptr);
                    break;

                case GameSpeakEvents::Slig_BS2_7:
                    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eBullshit2_7, 127, -100, nullptr);
                    break;
            }

            const AnimRecord& animRec = AnimRec(AnimId::Security_Door_Speak);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            if (++field_128_max_idx >= field_100_code_len)
            {
                field_F8_state = SecurityDoorStates::eListeningForPassword_9;
                field_124_timer = sGnFrame_5C1B84 + 60;
            }
            else
            {
                field_F8_state = SecurityDoorStates::ePausing_7;
                field_124_timer = sGnFrame_5C1B84 + 30;
            }
            return;
        }

        case SecurityDoorStates::ePausing_7:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_124_timer)
            {
                field_F8_state = SecurityDoorStates::eSayingPassword_6;
            }
            return;

        case SecurityDoorStates::eListeningForPassword_9:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
            {
                if (field_104_event_idx != pEventSystem_5BC11C->field_28_last_event_index)
                {
                    field_104_event_idx = pEventSystem_5BC11C->field_28_last_event_index;
                    if (pEventSystem_5BC11C->field_20_last_event != GameSpeakEvents::eNone_m1 && pEventSystem_5BC11C->field_20_last_event != GameSpeakEvents::eSameAsLast_m2)
                    {
                        field_11A_event_idx = static_cast<s16>(pEventSystem_5BC11C->field_28_last_event_index);
                        field_F8_state = SecurityDoorStates::eCheckingIfPasswordMatches_10;
                    }
                }
            }
            else
            {
                field_F8_state = SecurityDoorStates::eFailure_12;
                field_124_timer = sGnFrame_5C1B84 + 15;
            }
            return;

        case SecurityDoorStates::eCheckingIfPasswordMatches_10:
        {
            switch (pEventSystem_5BC11C->MatchBuffer_4219E0(field_108_stru, field_118_max_idx, field_11A_event_idx))
            {
                case GameSpeakMatch::eNoMatch_0:
                    field_F8_state = SecurityDoorStates::eFailure_12;
                    field_124_timer = sGnFrame_5C1B84 + 15;
                    break;

                case GameSpeakMatch::eFullMatch_1:
                    field_F8_state = SecurityDoorStates::eSuccess_11;
                    field_124_timer = sGnFrame_5C1B84 + 15;
                    break;

                case GameSpeakMatch::ePartMatch_2:
                    if (field_104_event_idx != pEventSystem_5BC11C->field_28_last_event_index)
                    {
                        field_104_event_idx = pEventSystem_5BC11C->field_28_last_event_index;
                    }

                    if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
                    {
                        field_F8_state = SecurityDoorStates::eFailure_12;
                        field_124_timer = sGnFrame_5C1B84;
                    }
                    break;
            }
            return;
        }

        case SecurityDoorStates::eSuccess_11:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
            {
                return;
            }
            field_12A_unused = 1;
            SwitchStates_Set_465FF0(field_FA_switch_id, 1);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            SFX_Play_46FBA0(SoundEffect::GlukkonSwitchBleh_88, 127, -700);
            field_F8_state = SecurityDoorStates::eSuccessChime_1;
            field_124_timer = sGnFrame_5C1B84 + 15;
            return;

        case SecurityDoorStates::eFailure_12:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
            {
                return;
            }
            SFX_Play_46FBA0(SoundEffect::SecurityDoorDeny_38, 60, -720);
            field_F8_state = SecurityDoorStates::eLaughAtFailure_13;
            field_124_timer = sGnFrame_5C1B84 + 15;
            return;

        case SecurityDoorStates::eLaughAtFailure_13:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_124_timer)
            {
                return;
            }
            SFX_Play_46FBA0(SoundEffect::SecurityDoorLaugh_87, 127, -1000);
            field_F8_state = SecurityDoorStates::eInactive_0;
            field_124_timer = sGnFrame_5C1B84 + 90;
            return;

        default:
            return;
    }
}
