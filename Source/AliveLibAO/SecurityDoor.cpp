#include "stdafx_ao.h"
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
#include "../relive_lib/SwitchStates.hpp"
#include "CheatController.hpp"
#include "Path.hpp"

namespace AO {

void SecurityDoor::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Speak));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Idle));
}

SecurityDoor::~SecurityDoor()
{
    if (mState != SecurityDoorStates::eSuccessChime_1)
    {
        mState = SecurityDoorStates::eInactive_0;
    }
    Path::TLV_Reset(mTlvId, mState + 1);
}

SecurityDoor::SecurityDoor(relive::Path_SecurityDoor* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    LoadAnimations();

    mLastEventIdx = -1;
    mMaxIdx = 0;
    mBufferStartIdx = -1;

    Animation_Init(GetAnimRes(AnimId::Security_Door_Idle));

    GetAnimation().SetRender(false);

    mTlvId = tlvId;

    mYOffset = 0;
    GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    mSwitchId = pTlv->mSwitchId;
    mCodeConverted = Code_Convert(pTlv->mCode1, pTlv->mCode2);
    mCodeLength = Code_Length(mCodeConverted);
    mTlvTopLeft.x = pTlv->mTopLeftX;
    mTlvTopLeft.y = pTlv->mTopLeftY;
    mTlvBottomRight.x = pTlv->mBottomRightX;
    mTlvBottomRight.y = pTlv->mBottomRightY;

    mXPos = FP_FromInteger(pTlv->mXPos);
    mYPos = FP_FromInteger(pTlv->mYPos);

    if (pTlv->mTlvSpecificMeaning)
    {
        mState = static_cast<SecurityDoorStates>(pTlv->mTlvSpecificMeaning - 1);
    }
    else
    {
        mState = SecurityDoorStates::eInactive_0;
    }

    if (mState != SecurityDoorStates::eSuccessChime_1)
    {
        mTimer = MakeTimer(10);
    }
}

void SecurityDoor::VScreenChanged()
{
    SetDead(true);
}

bool SecurityDoor::IsPlayerNear()
{
    const s16 xpos = FP_GetExponent(sControlledCharacter->mXPos);
    const s16 ypos = FP_GetExponent(sControlledCharacter->mYPos);

    if (xpos < mTlvTopLeft.x || xpos > mTlvBottomRight.x)
    {
        return 0;
    }

    if (ypos >= mTlvTopLeft.y && ypos <= mTlvBottomRight.y)
    {
        return 1;
    }

    return 0;
}

void SecurityDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case SecurityDoorStates::eInactive_0:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                if (IsPlayerNear())
                {
                    GetAnimation().SetRender(true);
                    mState = SecurityDoorStates::eSayingHi_2;
                }
                else
                {
                    GetAnimation().SetRender(false);
                }
            }
            break;

        case SecurityDoorStates::eSuccessChime_1:
            if (static_cast<s32>(sGnFrame) == mTimer)
            {
                SND_SEQ_Play(SeqId::eSaveTriggerMusic_45, 1, 127, 127);
            }
            break;

        case SecurityDoorStates::eSayingHi_2:
            Slig::Slig_GameSpeak_SFX(SligSpeak::eHi_0, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = SecurityDoorStates::eListeningForHi_3;
            mTimer = MakeTimer(150);
            break;

        case SecurityDoorStates::eListeningForHi_3:
        {
            GameSpeakEvents last_event = {};
            if (mLastEventIdx == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
                {
                    last_event = GameSpeakEvents::eNone;
                }
                else
                {
                    last_event = GameSpeakEvents::eSameAsLast;
                }
            }
            else
            {
                last_event = gEventSystem->mLastEvent;
                mLastEventIdx = gEventSystem->mLastEventIndex;
            }

            if (last_event == GameSpeakEvents::eSlig_Hi)
            {
                mState = SecurityDoorStates::eWaitingToSayPassword_4;
                mTimer = MakeTimer(30);
            }
            else if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = SecurityDoorStates::eInactive_0;
            }
            break;
        }

        case SecurityDoorStates::eWaitingToSayPassword_4:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = SecurityDoorStates::ePreparingToSayPassword_5;
            }
            break;

        case SecurityDoorStates::ePreparingToSayPassword_5:
            mMaxIdx2 = 0;
            mMaxIdx = GameSpeak::FillBuffer(mCodeConverted, mPasswordBuffer);
            mState = SecurityDoorStates::eSayingPassword_6;
            break;

        case SecurityDoorStates::eSayingPassword_6:
        {
            const GameSpeakEvents code = Code_LookUp(mCodeConverted, mMaxIdx2, mCodeLength);
            switch (code)
            {
                case GameSpeakEvents::eSlig_BS:
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eBullshit_4, 127, -100, 0);
                    break;
                case GameSpeakEvents::eSlig_LookOut:
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eLookOut_5, 127, -100, 0);
                    break;
                case GameSpeakEvents::eSlig_BS2:
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eBullshit2_6, 127, -100, 0);
                    break;
                case GameSpeakEvents::eSlig_Laugh:
                    Slig::Slig_GameSpeak_SFX(SligSpeak::eHereBoy_1, 127, -100, 0);
                    break;
                default:
                    LOG_WARNING("eSayingPassword_6 code was %d. This is unhandled.", static_cast<s32>(code));
                    break;
            }

            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));

            mMaxIdx2++;
            if (mMaxIdx2 >= mCodeLength)
            {
                mState = SecurityDoorStates::eListeningForPassword_8;
                mTimer = MakeTimer(60);
            }
            else
            {
                mState = SecurityDoorStates::ePausing_7;
                mTimer = MakeTimer(30);
            }
            break;
        }

        case SecurityDoorStates::ePausing_7:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = SecurityDoorStates::eSayingPassword_6;
            }
            break;

        case SecurityDoorStates::eListeningForPassword_8:
        {
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                SFX_Play_Pitch(relive::SoundEffects::Alarm, 60, -720);
                mState = SecurityDoorStates::eLaughAtFailure_10;
                mTimer = MakeTimer(15);
            }

            GameSpeakEvents last_event = {};
            if (mLastEventIdx == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
                {
                    last_event = GameSpeakEvents::eNone;
                }
                else
                {
                    last_event = GameSpeakEvents::eSameAsLast;
                }
            }
            else
            {
                last_event = gEventSystem->mLastEvent;
                mLastEventIdx = gEventSystem->mLastEventIndex;
            }

            if (last_event != GameSpeakEvents::eNone)
            {
                mState = SecurityDoorStates::eCheckingIfPasswordMatches_9;
            }
            break;
        }

        case SecurityDoorStates::eCheckingIfPasswordMatches_9:
        {
            GameSpeakEvents last_event = {};
            if (mLastEventIdx == gEventSystem->mLastEventIndex)
            {
                if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
                {
                    last_event = GameSpeakEvents::eNone;
                }
                else
                {
                    last_event = GameSpeakEvents::eSameAsLast;
                }
            }
            else
            {
                mLastEventIdx = gEventSystem->mLastEventIndex;
                last_event = gEventSystem->mLastEvent;
            }

            if (last_event == GameSpeakEvents::eNone)
            {
                const auto MatchBuffer = gEventSystem->MatchBuffer(mPasswordBuffer, mMaxIdx, mBufferStartIdx);
                if (MatchBuffer == GameSpeakMatch::eFullMatch || gVoiceCheat)
                {
                    GetAnimation().SetRender(false);
                    SwitchStates_Set(mSwitchId, 1);
                    SFX_Play_Pitch(relive::SoundEffects::SligBleh, 127, -700);
                    mState = SecurityDoorStates::eSuccessChime_1;
                    mTimer = MakeTimer(15);
                }
                else
                {
                    SFX_Play_Pitch(relive::SoundEffects::Alarm, 60, -720);
                    mState = SecurityDoorStates::eLaughAtFailure_10;
                    mTimer = MakeTimer(15);
                }
            }
            break;
        }

        case SecurityDoorStates::eLaughAtFailure_10:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                SFX_Play_Pitch(relive::SoundEffects::SligLaugh, 127, -1000);
                mState = SecurityDoorStates::eInactive_0;
                mTimer = MakeTimer(90);
            }
            break;

        default:
            LOG_WARNING("SecurityDoor mState was %d. This is unhandled.", mState);
            return;
    }
}


} // namespace AO
