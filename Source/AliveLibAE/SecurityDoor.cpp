#include "stdafx.h"
#include "SecurityDoor.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Map.hpp"
#include "Path.hpp"

void SecurityDoor::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Speak));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Idle));
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

    PSX_Point point = {};
    gMap.Get_Abe_Spawn_Pos(&point);

    if (mXPos > FP_FromInteger(0))
    {
        mXPos -= FP_FromInteger(point.x);
    }
    else
    {
        mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    }

    if (mYPos > FP_FromInteger(0))
    {
        mYPos -= FP_FromInteger(point.y);
    }
    else
    {
        mYPos = FP_FromInteger((pTlv->mTopLeftY + pTlv->mBottomRightY) / 2);
    }

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

SecurityDoor::~SecurityDoor()
{
    if (mState != SecurityDoorStates::eSuccessChime_1)
    {
        mState = SecurityDoorStates::eInactive_0;
    }

    Path::TLV_Reset(mTlvId, static_cast<s16>(mState) + 1, 0, 0);
}

void SecurityDoor::VScreenChanged()
{
    SetDead(true);
}

s16 SecurityDoor::IsPlayerNear()
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
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }

            if (IsPlayerNear())
            {
                GetAnimation().SetRender(true);
                mState = SecurityDoorStates::eSayingHi_2;
            }
            else
            {
                GetAnimation().SetRender(false);
            }
            return;

        case SecurityDoorStates::eSuccessChime_1:
            if (static_cast<s32>(sGnFrame) == mTimer)
            {
                SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            }
            return;

        case SecurityDoorStates::eSayingHi_2:
        {
            Slig_GameSpeak_SFX(SligSpeak::eHi_0, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = SecurityDoorStates::eListeningForHi_3;
            mTimer = MakeTimer(150);
            return;
        }
        case SecurityDoorStates::eListeningForHi_3:
            if (mLastEventIdx != gEventSystem->mLastEventIndex)
            {
                mLastEventIdx = gEventSystem->mLastEventIndex;
                if (gEventSystem->mLastEvent != GameSpeakEvents::eNone && gEventSystem->mLastEvent != GameSpeakEvents::eSameAsLast)
                {
                    if (gEventSystem->mLastEvent == GameSpeakEvents::eSlig_Hi)
                    {
                        mState = SecurityDoorStates::eWaitingToSayPassword_4;
                        mTimer = MakeTimer(30);
                        return;
                    }
                    else
                    {
                        mState = SecurityDoorStates::eFailure_12;
                        mTimer = MakeTimer(15);
                    }
                }
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = SecurityDoorStates::eInactive_0;
            }
            return;

        case SecurityDoorStates::eWaitingToSayPassword_4:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = SecurityDoorStates::ePreparingToSayPassword_5;
            }
            return;

        case SecurityDoorStates::ePreparingToSayPassword_5:
            mMaxIdx2 = 0;
            mMaxIdx = GameSpeak::FillBuffer(mCodeConverted, mPasswordBuffer);
            mState = SecurityDoorStates::eSayingPassword_6;
            return;

        case SecurityDoorStates::eSayingPassword_6:
        {
            const GameSpeakEvents code = Code_LookUp(mCodeConverted, mMaxIdx2, mCodeLength);
            switch (code)
            {
                case GameSpeakEvents::eSlig_BS:
                    Slig_GameSpeak_SFX(SligSpeak::eBullshit_5, 127, -100, nullptr);
                    break;

                case GameSpeakEvents::eSlig_Laugh:
                    Slig_GameSpeak_SFX(SligSpeak::eLaugh_3, 127, -100, nullptr);
                    break;

                case GameSpeakEvents::eSlig_BS2:
                    Slig_GameSpeak_SFX(SligSpeak::eBullshit2_7, 127, -100, nullptr);
                    break;
            }

            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            if (++mMaxIdx2 >= mCodeLength)
            {
                mState = SecurityDoorStates::eListeningForPassword_9;
                mTimer = MakeTimer(60);
            }
            else
            {
                mState = SecurityDoorStates::ePausing_7;
                mTimer = MakeTimer(30);
            }
            return;
        }

        case SecurityDoorStates::ePausing_7:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = SecurityDoorStates::eSayingPassword_6;
            }
            return;

        case SecurityDoorStates::eListeningForPassword_9:
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                if (mLastEventIdx != gEventSystem->mLastEventIndex)
                {
                    mLastEventIdx = gEventSystem->mLastEventIndex;
                    if (gEventSystem->mLastEvent != GameSpeakEvents::eNone && gEventSystem->mLastEvent != GameSpeakEvents::eSameAsLast)
                    {
                        mBufferStartIdx = static_cast<s16>(gEventSystem->mLastEventIndex);
                        mState = SecurityDoorStates::eCheckingIfPasswordMatches_10;
                    }
                }
            }
            else
            {
                mState = SecurityDoorStates::eFailure_12;
                mTimer = MakeTimer(15);
            }
            return;

        case SecurityDoorStates::eCheckingIfPasswordMatches_10:
        {
            switch (gEventSystem->MatchBuffer(mPasswordBuffer, mMaxIdx, mBufferStartIdx))
            {
                case GameSpeakMatch::eNoMatch:
                    mState = SecurityDoorStates::eFailure_12;
                    mTimer = MakeTimer(15);
                    break;

                case GameSpeakMatch::eFullMatch:
                    mState = SecurityDoorStates::eSuccess_11;
                    mTimer = MakeTimer(15);
                    break;

                case GameSpeakMatch::ePartMatch:
                    if (mLastEventIdx != gEventSystem->mLastEventIndex)
                    {
                        mLastEventIdx = gEventSystem->mLastEventIndex;
                    }

                    if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
                    {
                        mState = SecurityDoorStates::eFailure_12;
                        mTimer = sGnFrame;
                    }
                    break;
            }
            return;
        }

        case SecurityDoorStates::eSuccess_11:
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }
            SwitchStates_Set(mSwitchId, 1);
            GetAnimation().SetRender(false);
            SFX_Play_Pitch(relive::SoundEffects::GlukkonSwitchBleh, 127, -700);
            mState = SecurityDoorStates::eSuccessChime_1;
            mTimer = MakeTimer(15);
            return;

        case SecurityDoorStates::eFailure_12:
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }
            SFX_Play_Pitch(relive::SoundEffects::SecurityDoorDeny, 60, -720);
            mState = SecurityDoorStates::eLaughAtFailure_13;
            mTimer = MakeTimer(15);
            return;

        case SecurityDoorStates::eLaughAtFailure_13:
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }
            SFX_Play_Pitch(relive::SoundEffects::SecurityDoorLaugh, 127, -1000);
            mState = SecurityDoorStates::eInactive_0;
            mTimer = MakeTimer(90);
            return;

        default:
            return;
    }
}
