#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityDoor.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/Events.hpp"
#include "Midi.hpp"
#include "Slig.hpp"
#include "Abe.hpp"
#include "../relive_lib/Animation.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "CheatController.hpp"
#include "magic_enum/include/magic_enum.hpp"

namespace AO {

void SecurityDoor::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Speak));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Idle));
}

SecurityDoor::~SecurityDoor()
{
    if (field_E8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_E8_state = SecurityDoorStates::eInactive_0;
    }
    Path::TLV_Reset(field_E4_tlvInfo, field_E8_state + 1, 0, 0);
}

SecurityDoor::SecurityDoor(relive::Path_SecurityDoor* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    LoadAnimations();

    field_F4_event_idx = -1;
    field_108_max_idx = 0;
    field_10A_event_idx = -1;

    Animation_Init(GetAnimRes(AnimId::Security_Door_Idle));

    mAnim.mFlags.Clear(AnimFlags::eRender);

    field_E4_tlvInfo = tlvId;

    mYOffset = 0;
    mAnim.mRenderLayer = Layer::eLayer_BeforeWell_22;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    field_EA_switch_id = pTlv->mSwitchId;
    field_EC_code_converted = Code_Convert(pTlv->mCode1, pTlv->mCode2);
    field_F0_code_len = Code_Length(field_EC_code_converted);
    field_10C_top_left.x = pTlv->mTopLeftX;
    field_10C_top_left.y = pTlv->mTopLeftY;
    field_110_bottom_right.x = pTlv->mBottomRightX;
    field_110_bottom_right.y = pTlv->mBottomRightY;

    mXPos = FP_FromInteger(pTlv->mXPos);
    mYPos = FP_FromInteger(pTlv->mYPos);

    if (pTlv->mTlvSpecificMeaning)
    {
        field_E8_state = pTlv->mTlvSpecificMeaning - 1;
    }
    else
    {
        field_E8_state = SecurityDoorStates::eInactive_0;
    }

    if (field_E8_state != SecurityDoorStates::eSuccessChime_1)
    {
        field_114_timer = sGnFrame + 10;
    }
}

void SecurityDoor::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

bool SecurityDoor::IsPlayerNear()
{
    const s16 xpos = FP_GetExponent(sControlledCharacter->mXPos);
    const s16 ypos = FP_GetExponent(sControlledCharacter->mYPos);

    if (xpos < field_10C_top_left.x || xpos > field_110_bottom_right.x)
    {
        return 0;
    }

    if (ypos >= field_10C_top_left.y && ypos <= field_110_bottom_right.y)
    {
        return 1;
    }

    return 0;
}

void SecurityDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    switch (field_E8_state)
    {
        case SecurityDoorStates::eInactive_0:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                if (IsPlayerNear())
                {
                    mAnim.mFlags.Set(AnimFlags::eRender);
                    field_E8_state = SecurityDoorStates::eSayingHi_2;
                }
                else
                {
                    mAnim.mFlags.Clear(AnimFlags::eRender);
                }
            }
            break;

        case SecurityDoorStates::eSuccessChime_1:
            if (static_cast<s32>(sGnFrame) == field_114_timer)
            {
                SND_SEQ_Play_477760(SeqId::eSaveTriggerMusic_45, 1, 127, 127);
            }
            break;

        case SecurityDoorStates::eSayingHi_2:
            Slig::Slig_GameSpeak_SFX(SligSpeak::eHi_0, 127, -200, 0);
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            field_E8_state = SecurityDoorStates::eListeningForHi_3;
            field_114_timer = sGnFrame + 150;
            break;

        case SecurityDoorStates::eListeningForHi_3:
        {
            GameSpeakEvents last_event = {};
            if (field_F4_event_idx == gEventSystem->field_18_last_event_index)
            {
                if (gEventSystem->field_10_last_event == GameSpeakEvents::eNone_m1)
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
                last_event = gEventSystem->field_10_last_event;
                field_F4_event_idx = gEventSystem->field_18_last_event_index;
            }

            if (last_event == GameSpeakEvents::Slig_Hi_23)
            {
                field_E8_state = SecurityDoorStates::eWaitingToSayPassword_4;
                field_114_timer = sGnFrame + 30;
            }
            else if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                field_E8_state = SecurityDoorStates::eInactive_0;
            }
            break;
        }

        case SecurityDoorStates::eWaitingToSayPassword_4:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
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
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eBullshit_4, 127, -100, 0);
                    break;
                case GameSpeakEvents::Slig_LookOut_6:
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eLookOut_5, 127, -100, 0);
                    break;
                case GameSpeakEvents::eUnknown_7:
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eBullshit2_6, 127, -100, 0);
                    break;
                case GameSpeakEvents::eUnknown_8:
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eHereBoy_1, 127, -100, 0);
                    break;
                default:
                    LOG_WARNING("eSayingPassword_6 code was " << magic_enum::enum_name(code) << ". This is unhandled.");
                    break;
            }

            mAnim.Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));

            field_118_max_idx++;
            if (field_118_max_idx >= field_F0_code_len)
            {
                field_E8_state = SecurityDoorStates::eListeningForPassword_8;
                field_114_timer = sGnFrame + 60;
            }
            else
            {
                field_E8_state = SecurityDoorStates::ePausing_7;
                field_114_timer = sGnFrame + 30;
            }
            break;
        }

        case SecurityDoorStates::ePausing_7:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                field_E8_state = SecurityDoorStates::eSayingPassword_6;
            }
            break;

        case SecurityDoorStates::eListeningForPassword_8:
        {
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                SFX_Play_Pitch(relive::SoundEffects::Alarm, 60, -720);
                field_E8_state = SecurityDoorStates::eLaughAtFailure_10;
                field_114_timer = sGnFrame + 15;
            }

            GameSpeakEvents last_event = {};
            if (field_F4_event_idx == gEventSystem->field_18_last_event_index)
            {
                if (gEventSystem->field_10_last_event == GameSpeakEvents::eNone_m1)
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
                last_event = gEventSystem->field_10_last_event;
                field_F4_event_idx = gEventSystem->field_18_last_event_index;
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
            if (field_F4_event_idx == gEventSystem->field_18_last_event_index)
            {
                if (gEventSystem->field_10_last_event == GameSpeakEvents::eNone_m1)
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
                field_F4_event_idx = gEventSystem->field_18_last_event_index;
                last_event = gEventSystem->field_10_last_event;
            }

            if (last_event == GameSpeakEvents::eNone_m1)
            {
                const auto MatchBuffer = gEventSystem->MatchBuffer(field_F8_stru, field_108_max_idx, field_10A_event_idx);
                field_11A_unused = static_cast<s16>(MatchBuffer);
                if (MatchBuffer == GameSpeakMatch::eFullMatch_1 || gVoiceCheat)
                {
                    mAnim.mFlags.Clear(AnimFlags::eRender);
                    SwitchStates_Set(field_EA_switch_id, 1);
                    SFX_Play_Pitch(relive::SoundEffects::SligBleh, 127, -700);
                    field_E8_state = SecurityDoorStates::eSuccessChime_1;
                    field_114_timer = sGnFrame + 15;
                }
                else
                {
                    SFX_Play_Pitch(relive::SoundEffects::Alarm, 60, -720);
                    field_E8_state = SecurityDoorStates::eLaughAtFailure_10;
                    field_114_timer = sGnFrame + 15;
                }
            }
            break;
        }

        case SecurityDoorStates::eLaughAtFailure_10:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                SFX_Play_Pitch(relive::SoundEffects::SligLaugh, 127, -1000);
                field_E8_state = SecurityDoorStates::eInactive_0;
                field_114_timer = sGnFrame + 90;
            }
            break;

        default:
            LOG_WARNING("SecurityDoor field_E8_state was " << field_E8_state << ". This is unhandled.");
            return;
    }
}


} // namespace AO
