#include "stdafx.h"
#include "GlukkonSwitch.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "Glukkon.hpp"
#include "Sfx.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "GameSpeak.hpp"
#include "Map.hpp"
#include "Path.hpp"

void GlukkonSwitch::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Speak));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Door_Idle));
}

GlukkonSwitch::GlukkonSwitch(relive::Path_GlukkonSwitch* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mLastEventIdx = -1;

    SetType(ReliveTypes::eHelpPhone);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::Security_Door_Idle));

    GetAnimation().SetRender(false);
    mTlvId = tlvId;
    GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);

    if (pTlv->mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }
    else if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }

    mOkSwitchId = pTlv->mOkSwitchId;
    mFailSwitchId = pTlv->mFailSwitchId;
    mTlvTopLeft.x = pTlv->mTopLeftX;
    mTlvTopLeft.y = pTlv->mTopLeftY;
    mTlvBottomRight.x = pTlv->mBottomRightX;
    mTlvBottomRight.y = pTlv->mBottomRightY;

    mXPos = FP_FromInteger(pTlv->mXPos);
    mYPos = FP_FromInteger(pTlv->mYPos);

    PSX_Point pos = {};
    gMap.Get_Abe_Spawn_Pos(&pos);
    if (mXPos > FP_FromInteger(0))
    {
        mXPos -= FP_FromInteger(pos.x);
    }
    else
    {
        mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    }

    if (mYPos > FP_FromInteger(0))
    {
        mYPos -= FP_FromInteger(pos.y);
    }
    else
    {
        mYPos = FP_FromInteger((pTlv->mTopLeftY + pTlv->mBottomRightY) / 2);
    }

    if (pTlv->mTlvSpecificMeaning)
    {
        mState = static_cast<State>(pTlv->mTlvSpecificMeaning - 1);
    }
    else
    {
        mState = State::eWaitUntilGlukkonNearby;
    }

    if (mState != State::eSuccessFinish)
    {
        mTimer = MakeTimer(10);
    }
}

GlukkonSwitch::~GlukkonSwitch()
{
    Path::TLV_Reset(mTlvId);
}

void GlukkonSwitch::VScreenChanged()
{
    SetDead(true);
}

bool GlukkonSwitch::PlayerNearMe() const
{
    const s16 playerXPos = FP_GetExponent(sControlledCharacter->mXPos);
    const s16 playerYPos = FP_GetExponent(sControlledCharacter->mYPos);

    if ((playerXPos >= mTlvTopLeft.x && playerXPos <= mTlvBottomRight.x) && (playerYPos >= mTlvTopLeft.y && playerYPos <= mTlvBottomRight.y))
    {
        return true;
    }

    return false;
}

void GlukkonSwitch::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    const s32 lastEventIdx = gEventSystem->mLastEventIndex;
    GameSpeakEvents lastEventIdx2 = GameSpeakEvents::eNone;
    if (mLastEventIdx == lastEventIdx)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
        {
            lastEventIdx2 = GameSpeakEvents::eNone;
        }
        else
        {
            lastEventIdx2 = GameSpeakEvents::eSameAsLast;
        }
    }
    else
    {
        mLastEventIdx = lastEventIdx;
        lastEventIdx2 = gEventSystem->mLastEvent;
    }

    switch (mState)
    {
        case State::eWaitUntilGlukkonNearby:
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }

            if (PlayerNearMe())
            {
                GetAnimation().SetRender(true);
                mState = State::eSayHey;
            }
            else
            {
                GetAnimation().SetRender(false);
            }
            return;

        case State::eSuccessFinish:
            if (static_cast<s32>(sGnFrame) == mTimer)
            {
                SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            }
            else if (static_cast<s32>(sGnFrame) > mTimer && !PlayerNearMe())
            {
                mState = State::eWaitUntilGlukkonNearby;
            }
            return;

        case State::eSayHey:
        {
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Hey_0, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = State::eWaitForHeyInput;
            mTimer = MakeTimer(150);
            return;
        }
        case State::eWaitForHeyInput:
            if (!PlayerNearMe())
            {
                mState = State::eWaitUntilGlukkonNearby;
                mTimer = sGnFrame - 1;
                return;
            }

            if (lastEventIdx2 == GameSpeakEvents::eNone || lastEventIdx2 == GameSpeakEvents::eSameAsLast)
            {
                if (static_cast<s32>(sGnFrame) > mTimer)
                {
                    mState = State::eWaitUntilGlukkonNearby;
                }
            }
            else
            {
                if (lastEventIdx2 == GameSpeakEvents::eGlukkon_Hey)
                {
                    mState = State::eSayWhat;
                    mTimer = MakeTimer(30);
                }
                else
                {
                    if (lastEventIdx2 < GameSpeakEvents::eGlukkon_Hey)
                    {
                        mState = State::eFail;
                        mTimer = MakeTimer(30);
                    }

                    if (static_cast<s32>(sGnFrame) > mTimer)
                    {
                        mState = State::eWaitUntilGlukkonNearby;
                    }
                }
            }
            return;

        case State::eSayWhat:
        {
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::What_11, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = State::eWaitForDoItInput;
            mTimer = MakeTimer(60);
            return;
        }
        case State::eWaitForDoItInput:
            if (PlayerNearMe())
            {
                if (lastEventIdx2 == GameSpeakEvents::eNone || lastEventIdx2 == GameSpeakEvents::eSameAsLast)
                {
                    if (static_cast<s32>(sGnFrame) > mTimer)
                    {
                        mState = State::eListeningTimedOut;
                        mTimer = MakeTimer(15);
                    }
                }
                else if (lastEventIdx2 == GameSpeakEvents::eGlukkon_DoIt)
                {
                    mState = State::eSuccess;
                    mTimer = MakeTimer(30);
                }
                else if (lastEventIdx2 < GameSpeakEvents::eGlukkon_Hey)
                {
                    mState = State::eFail;
                    mTimer = MakeTimer(30);
                }
                else
                {
                    mState = State::eListeningTimedOut;
                    mTimer = MakeTimer(15);
                }
            }
            else
            {
                mState = State::eWaitUntilGlukkonNearby;
                mTimer = sGnFrame - 1;
            }
            return;

        case State::eSuccess:
        {
            if (static_cast<s32>(sGnFrame) != mTimer)
            {
                return;
            }
            SFX_Play_Pitch(relive::SoundEffects::GlukkonSwitchBleh, 127, -700); // Bleh!
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Laugh_7, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            SwitchStates_Do_Operation(mOkSwitchId, relive::reliveSwitchOp::eToggle);
            mState = State::eSuccessFinish;
            mTimer = MakeTimer(15);
            return;
        }
        case State::eListeningTimedOut:
        {
            if (static_cast<s32>(sGnFrame) != mTimer)
            {
                return;
            }
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Heh_5, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = State::eWaitUntilGlukkonNearby;
            mTimer = MakeTimer(90);
            return;
        }
        case State::eFail:
        {
            if (static_cast<s32>(sGnFrame) != mTimer)
            {
                return;
            }
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Heh_5, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            SwitchStates_Do_Operation(mFailSwitchId, relive::reliveSwitchOp::eSetTrue);
            mState = State::eWaitUntilGlukkonNearby;
            mTimer = MakeTimer(90);
            return;
        }
        default:
            return;
    }
}
