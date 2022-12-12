#include "stdafx.h"
#include "GlukkonSwitch.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Sound/Midi.hpp"
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
        mState = pTlv->mTlvSpecificMeaning - 1;
    }
    else
    {
        mState = 0;
    }

    if (mState != 1)
    {
        mTimer = sGnFrame + 10;
    }
}

GlukkonSwitch::~GlukkonSwitch()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void GlukkonSwitch::VScreenChanged()
{
    SetDead(true);
}

bool GlukkonSwitch::PlayerNearMe()
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

    const s32 lastEventIdx = gEventSystem->field_28_last_event_index;
    GameSpeakEvents lastEventIdx2 = GameSpeakEvents::eNone_m1;
    if (mLastEventIdx == lastEventIdx)
    {
        if (gEventSystem->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            lastEventIdx2 = GameSpeakEvents::eNone_m1;
        }
        else
        {
            lastEventIdx2 = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        mLastEventIdx = lastEventIdx;
        lastEventIdx2 = gEventSystem->field_20_last_event;
    }

    switch (mState)
    {
        case 0:
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }

            if (PlayerNearMe())
            {
                GetAnimation().SetRender(true);
                mState = 2;
            }
            else
            {
                GetAnimation().SetRender(false);
            }
            return;

        case 1:
            if (static_cast<s32>(sGnFrame) == mTimer)
            {
                SND_SEQ_Play(SeqId::SaveTriggerMusic_31, 1, 127, 127);
            }
            else if (static_cast<s32>(sGnFrame) > mTimer && !PlayerNearMe())
            {
                mState = 0;
            }
            return;

        case 2:
        {
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Hey_0, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = 3;
            mTimer = sGnFrame + 150;
            return;
        }
        case 3:
            if (!PlayerNearMe())
            {
                mState = 0;
                mTimer = sGnFrame - 1;
                return;
            }

            if (lastEventIdx2 == GameSpeakEvents::eNone_m1 || lastEventIdx2 == GameSpeakEvents::eSameAsLast_m2)
            {
                if (static_cast<s32>(sGnFrame) > mTimer)
                {
                    mState = 0;
                }
            }
            else
            {
                if (lastEventIdx2 == GameSpeakEvents::Glukkon_Hey_36)
                {
                    mState = 4;
                    mTimer = sGnFrame + 30;
                }
                else
                {
                    if (lastEventIdx2 < GameSpeakEvents::Glukkon_Hey_36)
                    {
                        mState = 8;
                        mTimer = sGnFrame + 30;
                    }

                    if (static_cast<s32>(sGnFrame) > mTimer)
                    {
                        mState = 0;
                    }
                }
            }
            return;

        case 4:
        {
            if (static_cast<s32>(sGnFrame) <= mTimer)
            {
                return;
            }
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::What_11, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = 5;
            mTimer = sGnFrame + 60;
            return;
        }
        case 5:
            if (PlayerNearMe())
            {
                if (lastEventIdx2 == GameSpeakEvents::eNone_m1 || lastEventIdx2 == GameSpeakEvents::eSameAsLast_m2)
                {
                    if (static_cast<s32>(sGnFrame) > mTimer)
                    {
                        mState = 7;
                        mTimer = sGnFrame + 15;
                    }
                }
                else if (lastEventIdx2 == GameSpeakEvents::Glukkon_DoIt_37)
                {
                    mState = 6;
                    mTimer = sGnFrame + 30;
                }
                else if (lastEventIdx2 < GameSpeakEvents::Glukkon_Hey_36)
                {
                    mState = 8;
                    mTimer = sGnFrame + 30;
                }
                else
                {
                    mState = 7;
                    mTimer = sGnFrame + 15;
                }
            }
            else
            {
                mState = 0;
                mTimer = sGnFrame - 1;
            }
            return;

        case 6:
        {
            if (static_cast<s32>(sGnFrame) != mTimer)
            {
                return;
            }
            SFX_Play_Pitch(relive::SoundEffects::GlukkonSwitchBleh, 127, -700); // Bleh!
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Laugh_7, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            SwitchStates_Do_Operation(mOkSwitchId, relive::reliveSwitchOp::eToggle);
            mState = 1;
            mTimer = sGnFrame + 15;
            return;
        }
        case 7:
        {
            if (static_cast<s32>(sGnFrame) != mTimer)
            {
                return;
            }
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Heh_5, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            mState = 0;
            mTimer = sGnFrame + 90;
            return;
        }
        case 8:
        {
            if (static_cast<s32>(sGnFrame) != mTimer)
            {
                return;
            }
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Heh_5, 127, -200, 0);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Door_Speak));
            SwitchStates_Do_Operation(mFailSwitchId, relive::reliveSwitchOp::eSetTrue);
            mState = 0;
            mTimer = sGnFrame + 90;
            return;
        }
        default:
            return;
    }
}
