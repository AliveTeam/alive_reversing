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

SecurityDoor::SecurityDoor(Path_SecurityDoor* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_104_event_idx = -1;
    field_118_max_idx = 0;
    field_11A_event_idx = -1;

    const AnimRecord& rec = AnimRec(AnimId::Security_Door_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    field_F4_tlvInfo = tlvInfo;

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_22;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_FA_switch_id = pTlv->field_12_switch_id;
    field_FC_code_converted = Code_Convert(pTlv->field_14_code_1, pTlv->field_16_code_2);
    field_100_code_len = Code_Length(field_FC_code_converted);
    field_11C_top_left = pTlv->mTopLeft;
    field_120_bottom_right = pTlv->mBottomRight;
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_18_xpos);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_1A_ypos);

    PSX_Point point = {};
    gMap.Get_Abe_Spawn_Pos(&point);

    if (mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos -= FP_FromInteger(point.x);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    }

    if (mBaseAnimatedWithPhysicsGameObject_YPos > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(point.y);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger((pTlv->mTopLeft.y + pTlv->mBottomRight.y) / 2);
    }

    if (pTlv->mTlvState)
    {
        field_F8_state = static_cast<SecurityDoorStates>(pTlv->mTlvState - 1);
    }
    else
    {
        field_F8_state = SecurityDoorStates::eInactive_0;
    }

    if (field_F8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_124_timer = sGnFrame + 10;
    }
}

SecurityDoor::~SecurityDoor()
{
    if (field_F8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_F8_state = SecurityDoorStates::eInactive_0;
    }

    Path::TLV_Reset(field_F4_tlvInfo, static_cast<s16>(field_F8_state) + 1, 0, 0);
}

void SecurityDoor::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 SecurityDoor::IsPlayerNear()
{
    const s16 xpos = FP_GetExponent(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos);
    const s16 ypos = FP_GetExponent(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos);

    if (xpos < field_11C_top_left.x || xpos > field_120_bottom_right.x)
    {
        return 0;
    }

    if (ypos >= field_11C_top_left.y && ypos <= field_120_bottom_right.y)
    {
        return 1;
    }

    return 0;
}

void SecurityDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_F8_state)
    {
        case SecurityDoorStates::eInactive_0:
            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }

            if (IsPlayerNear())
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                field_F8_state = SecurityDoorStates::eSayingHi_2;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            }
            return;

        case SecurityDoorStates::eSuccessChime_1:
            if (static_cast<s32>(sGnFrame) == field_124_timer)
            {
                SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            }
            return;

        case SecurityDoorStates::eSayingHi_2:
        {
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHi_0, 127, -200, 0);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Security_Door_Speak, nullptr);
            field_F8_state = SecurityDoorStates::eListeningForHi_3;
            field_124_timer = sGnFrame + 150;
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
                        field_124_timer = sGnFrame + 30;
                        return;
                    }
                    else
                    {
                        field_F8_state = SecurityDoorStates::eFailure_12;
                        field_124_timer = sGnFrame + 15;
                    }
                }
            }

            if (static_cast<s32>(sGnFrame) > field_124_timer)
            {
                field_F8_state = SecurityDoorStates::eInactive_0;
            }
            return;

        case SecurityDoorStates::eWaitingToSayPassword_4:
            if (static_cast<s32>(sGnFrame) > field_124_timer)
            {
                field_F8_state = SecurityDoorStates::ePreparingToSayPassword_5;
            }
            return;

        case SecurityDoorStates::ePreparingToSayPassword_5:
            field_128_max_idx = 0;
            field_118_max_idx = static_cast<s16>(GameSpeak::FillBuffer(field_FC_code_converted, field_108_stru));
            field_F8_state = SecurityDoorStates::eSayingPassword_6;
            return;

        case SecurityDoorStates::eSayingPassword_6:
        {
            const GameSpeakEvents code = Code_LookUp(field_FC_code_converted, field_128_max_idx, field_100_code_len);
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

            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Security_Door_Speak, nullptr);
            if (++field_128_max_idx >= field_100_code_len)
            {
                field_F8_state = SecurityDoorStates::eListeningForPassword_9;
                field_124_timer = sGnFrame + 60;
            }
            else
            {
                field_F8_state = SecurityDoorStates::ePausing_7;
                field_124_timer = sGnFrame + 30;
            }
            return;
        }

        case SecurityDoorStates::ePausing_7:
            if (static_cast<s32>(sGnFrame) > field_124_timer)
            {
                field_F8_state = SecurityDoorStates::eSayingPassword_6;
            }
            return;

        case SecurityDoorStates::eListeningForPassword_9:
            if (static_cast<s32>(sGnFrame) <= field_124_timer)
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
                field_124_timer = sGnFrame + 15;
            }
            return;

        case SecurityDoorStates::eCheckingIfPasswordMatches_10:
        {
            switch (pEventSystem_5BC11C->MatchBuffer(field_108_stru, field_118_max_idx, field_11A_event_idx))
            {
                case GameSpeakMatch::eNoMatch_0:
                    field_F8_state = SecurityDoorStates::eFailure_12;
                    field_124_timer = sGnFrame + 15;
                    break;

                case GameSpeakMatch::eFullMatch_1:
                    field_F8_state = SecurityDoorStates::eSuccess_11;
                    field_124_timer = sGnFrame + 15;
                    break;

                case GameSpeakMatch::ePartMatch_2:
                    if (field_104_event_idx != pEventSystem_5BC11C->field_28_last_event_index)
                    {
                        field_104_event_idx = pEventSystem_5BC11C->field_28_last_event_index;
                    }

                    if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
                    {
                        field_F8_state = SecurityDoorStates::eFailure_12;
                        field_124_timer = sGnFrame;
                    }
                    break;
            }
            return;
        }

        case SecurityDoorStates::eSuccess_11:
            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }
            field_12A_unused = 1;
            SwitchStates_Set(field_FA_switch_id, 1);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            SFX_Play_Pitch(SoundEffect::GlukkonSwitchBleh_88, 127, -700);
            field_F8_state = SecurityDoorStates::eSuccessChime_1;
            field_124_timer = sGnFrame + 15;
            return;

        case SecurityDoorStates::eFailure_12:
            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }
            SFX_Play_Pitch(SoundEffect::SecurityDoorDeny_38, 60, -720);
            field_F8_state = SecurityDoorStates::eLaughAtFailure_13;
            field_124_timer = sGnFrame + 15;
            return;

        case SecurityDoorStates::eLaughAtFailure_13:
            if (static_cast<s32>(sGnFrame) <= field_124_timer)
            {
                return;
            }
            SFX_Play_Pitch(SoundEffect::SecurityDoorLaugh_87, 127, -1000);
            field_F8_state = SecurityDoorStates::eInactive_0;
            field_124_timer = sGnFrame + 90;
            return;

        default:
            return;
    }
}
