#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "SlingMudokon.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Sfx.hpp"
#include "Bullet.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "CheatController.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "../relive_lib/GameObjects/Dove.hpp"
#include "Math.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"
#include "../relive_lib/FatalError.hpp"

namespace AO {

using TSlingMudStateFunction = decltype(&SlingMudokon::Motion_0_Idle);

const TSlingMudStateFunction gSlingMudMotionTable_4CFCB0[] = {
    &SlingMudokon::Motion_0_Idle,
    &SlingMudokon::Motion_1_Angry,
    &SlingMudokon::Motion_2_Speak,
    &SlingMudokon::Motion_3_ShootStart,
    &SlingMudokon::Motion_4_ShootEnd,
    &SlingMudokon::Motion_5_AngryToIdle};


void SlingMudokon::LoadAnimations()
{
    for (auto& animId : sSlingMudMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

SlingMudokon::SlingMudokon(relive::Path_SlingMudokon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(),
    mGiveCodeBrain(*this),
    mSpawnBrain(*this),
    mAskForPasswordBrain(*this)
{
    SetType(ReliveTypes::SlingMud);

    LoadAnimations();

    mLastEventIndex = -1;
    mBufferStart = 0;
    mBufferIdx = -1;
    
    Animation_Init(GetAnimRes(AnimId::Mudokon_Sling_Idle));

    SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

    GetAnimation().SetAnimate(false);
    GetAnimation().SetRender(false);
    GetAnimation().SetSemiTrans(true);

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            FP_FromInteger(pTlv->mTopLeftX),
            FP_FromInteger(pTlv->mTopLeftY),
            FP_FromInteger(pTlv->mBottomRightX),
            FP_FromInteger(pTlv->mBottomRightY),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
        == 1)
    {
        mXPos = hitX;
        mYPos = hitY;
    }

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

    if (pTlv->mDontWhistlePassword == relive::reliveChoice::eYes)
    {
        SetBrain(ISlingMudokonBrain::EBrainTypes::AskForPassword);
    }
    else
    {
        SetBrain(ISlingMudokonBrain::EBrainTypes::Spawn);
    }

    mCodeConverted = Code_Convert(pTlv->mCode1, pTlv->mCode2);
    mCodeLength = Code_Length(mCodeConverted);

    mTlvId = tlvId;

    mAbeGettingCloser = false;
    mDontSetDestroyed = true;
}

SlingMudokon::~SlingMudokon()
{
    if (mDontSetDestroyed)
    {
        Path::TLV_Reset(mTlvId);
    }
    else
    {
        Path::TLV_Delete(mTlvId);
    }

    /*
    // TODO: Check it isn't 2 resources freed here
    if (mAnim.field_20_ppBlock != field_150_res)
    {
        if (field_150_res)
        {
            ResourceManager::FreeResource_455550(field_150_res);
        }
    }*/
}

void SlingMudokon::VScreenChanged()
{
    SetDead(true);
}

void SlingMudokon::VUpdate()
{
    const auto old_motion = mCurrentMotion;

    VCallBrain();

    const auto old_x = mXPos;
    const auto old_y = mYPos;

    VCallMotion();

    if (old_x != mXPos || old_y != mYPos)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        VOnTlvCollision(BaseAliveGameObjectPathTLV);
    }

    mbGotShot = 0;

    if (old_motion != mCurrentMotion)
    {
        VUpdateAnimData();
    }
}

void SlingMudokon::VCallBrain()
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mCurrentBrain->VUpdate();
    }
    else
    {
        SetDead(true);
    }
}

void SlingMudokon::VCallMotion()
{
    InvokeMemberFunction(this, gSlingMudMotionTable_4CFCB0, mCurrentMotion);
}

void SlingMudokon::VUpdateAnimData()
{
    if (mCurrentMotion < 6)
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(sSlingMudMotionAnimIds[mCurrentMotion]));
    }
}

void SlingMudokon::Motion_0_Idle()
{
    if (GetNextMotion() == eSlingMudMotions::Motion_1_Angry)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_1_Angry);
        mNextMotion = -1;
    }
    else if (!GetAnimation().GetCurrentFrame() && GetNextMotion() == eSlingMudMotions::Motion_2_Speak)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_2_Speak);
        mNextMotion = -1;
    }
}

void SlingMudokon::Motion_1_Angry()
{
    if (!GetAnimation().GetCurrentFrame())
    {
        SfxPlayMono(relive::SoundEffects::SlingshotExtend, 0);
    }

    if (GetNextMotion() == eSlingMudMotions::Motion_3_ShootStart)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_3_ShootStart);
        mNextMotion = -1;
    }
    else if (GetNextMotion() == eSlingMudMotions::Motion_5_AngryToIdle)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_5_AngryToIdle);
        mNextMotion = -1;
    }
}

void SlingMudokon::Motion_2_Speak()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (GetAnimation().mAnimRes.mId == AnimId::Mudokon_Sling_Speak)
        {
            SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
        }
    }
}

void SlingMudokon::Motion_3_ShootStart()
{
    if (!GetAnimation().GetCurrentFrame())
    {
        SfxPlayMono(relive::SoundEffects::SlingshotShoot, 0);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (GetAnimation().mAnimRes.mId == AnimId::Mudokon_Sling_ShootStart)
        {
            // TODO: Check field_8_data.points[2].x
            const FP frame_x = FP_FromInteger(GetAnimation().Get_FrameHeader(-1)->mBoundMax.x);
            FP bulletXPos = {};
            FP xDistance = {};

            if (GetAnimation().GetFlipX())
            {
                bulletXPos = mXPos - frame_x;
                xDistance = -FP_FromInteger(640);
            }
            else
            {
                bulletXPos = frame_x + mXPos;
                xDistance = FP_FromInteger(640);
            }

            relive_new Bullet(
                this,
                BulletType::eNormalBullet_1,
                bulletXPos,
                mYPos - FP_FromInteger(24),
                xDistance,
                GetSpriteScale(),
                0);
            SetCurrentMotion(eSlingMudMotions::Motion_4_ShootEnd);
        }
    }
}

void SlingMudokon::Motion_4_ShootEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
    }
}

void SlingMudokon::Motion_5_AngryToIdle()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
    }
}

GameSpeakEvents SlingMudokon::getLastIdx()
{
    if (mLastEventIndex == gEventSystem->mLastEventIndex)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
        {
            return GameSpeakEvents::eNone;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast;
        }
    }
    else
    {
        mLastEventIndex = gEventSystem->mLastEventIndex;
        return gEventSystem->mLastEvent;
    }
}

void SlingMudokon::SetBrain(ISlingMudokonBrain::EBrainTypes brain)
{
    switch (brain)
    {
        case ISlingMudokonBrain::EBrainTypes::GiveCode:
            mCurrentBrain = &mGiveCodeBrain;
            break;
        case ISlingMudokonBrain::EBrainTypes::Spawn:
            mCurrentBrain = &mSpawnBrain;
            break;
        case ISlingMudokonBrain::EBrainTypes::AskForPassword:
            mCurrentBrain = &mAskForPasswordBrain;
            break;

        default:
            ALIVE_FATAL("Invalid fleech brain type %d", static_cast<s32>(brain));
    }
}

void GiveCodeBrain::VUpdate()
{
    switch (mBrainState)
    {
        case EState::Init:
            mSlingMudokon.mCodePos = 0;
            mBrainState = EState::GiveCode;
            return;

        case EState::GiveCode:
            if (mSlingMudokon.mCurrentMotion || mSlingMudokon.GetAnimation().GetCurrentFrame() == 0)
            {
                mSlingMudokon.mNextMotion = 2;
                switch (Code_LookUp(mSlingMudokon.mCodeConverted, mSlingMudokon.mCodePos, mSlingMudokon.mCodeLength))
                {
                    case GameSpeakEvents::eAbe_WhistleHigh:
                        Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, &mSlingMudokon);
                        break;
                    case GameSpeakEvents::eAbe_WhistleLow:
                        Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, &mSlingMudokon);
                        break;
                    case GameSpeakEvents::eAbe_Fart:
                        Mudokon_SFX(MudSounds::eFart_7, 0, 300, &mSlingMudokon);
                        break;
                    case GameSpeakEvents::eAbe_Laugh:
                        Mudokon_SFX(MudSounds::eLaugh1_8, 0, 300, &mSlingMudokon);
                        break;
                    default:
                        break;
                }

                mSlingMudokon.mCodePos++;
                if (mSlingMudokon.mCodePos >= mSlingMudokon.mCodeLength)
                {
                    mSlingMudokon.mBufferStart = GameSpeak::FillBuffer(mSlingMudokon.mCodeConverted, mSlingMudokon.mCodeBuffer);
                    mBrainState = EState::WaitForCode;
                    return;
                }
                else
                {
                    mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);
                    mBrainState = EState::PauseABit;
                    return;
                }
            }
            mBrainState = EState::GiveCode;
            return;

        case EState::PauseABit:
            if (static_cast<s32>(sGnFrame) <= mSlingMudokon.field_140_timer)
            {
                if (mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
                {
                    break;
                }

                return;
            }
            mBrainState = EState::GiveCode;
            return;

        case EState::WaitForCode:
        {
            if (mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
            {
                break;
            }

            GameSpeakEvents lastIdx = mSlingMudokon.getLastIdx();
            if (lastIdx == GameSpeakEvents::eNone)
            {
                return;
            }

            mSlingMudokon.field_144_timer2 = BaseGameObject::MakeTimer(40);
            mSlingMudokon.mBufferIdx = static_cast<s16>(gEventSystem->mLastEventIndex);
            mBrainState = EState::CheckCodeMatching;
            return;
        }

        case EState::CheckCodeMatching:
            if (mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
            {
                break;
            }
            else
            {
                GameSpeakEvents lastIdx = mSlingMudokon.getLastIdx();
                if (lastIdx != GameSpeakEvents::eNone)
                {
                    mSlingMudokon.mLastEventIndex = BaseGameObject::MakeTimer(40);
                    mSlingMudokon.field_144_timer2 = BaseGameObject::MakeTimer(40);
                }

                if (static_cast<s32>(sGnFrame) <= mSlingMudokon.field_144_timer2)
                {
                    const auto result = gEventSystem->MatchBuffer(mSlingMudokon.mCodeBuffer, mSlingMudokon.mBufferStart, mSlingMudokon.mBufferIdx);
                    if (result != GameSpeakMatch::eFullMatch &&
                        result > GameSpeakMatch::eFullMatch)
                    {
                        return;
                    }
                }

                const GameSpeakMatch MatchBuffer = gEventSystem->MatchBuffer(mSlingMudokon.mCodeBuffer, mSlingMudokon.mBufferStart, mSlingMudokon.mBufferIdx);
                if (MatchBuffer == GameSpeakMatch::eFullMatch || gVoiceCheat)
                {
                    mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);
                    mSlingMudokon.mCodeMatches = true;
                }
                else
                {
                    mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(10);
                    mSlingMudokon.mCodeMatches = false;
                }
                mBrainState = EState::RespondToProvidedCode;
            }
            return;

        case EState::RespondToProvidedCode:
            if (mSlingMudokon.GetAnimation().GetCurrentFrame() || static_cast<s32>(sGnFrame) <= mSlingMudokon.field_140_timer)
            {
                return;
            }

            if (mSlingMudokon.mCodeMatches)
            {
                Mudokon_SFX(MudSounds::eOkay_13, 0, 300, &mSlingMudokon);
            }
            else
            {
                Mudokon_SFX(MudSounds::eRefuse_14, 0, 300, &mSlingMudokon);
            }

            mSlingMudokon.SetBrain(ISlingMudokonBrain::EBrainTypes::Spawn);
            mSlingMudokon.mSpawnBrain.SetState(SpawnBrain::EState::ObserveAbe);

            mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_2_Speak);
            return;

        default:
            return;
    }

    mSlingMudokon.mAbeGettingCloser = true;
    mSlingMudokon.SetBrain(ISlingMudokonBrain::EBrainTypes::Spawn);
    mSlingMudokon.mSpawnBrain.SetState(SpawnBrain::EState::ObserveAbe);
}

void SpawnBrain::VUpdate()
{
    switch (mBrainState)
    {
        case EState::Init:
            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(10);
            mBrainState = EState::CreateParticle;
            return;

        case EState::CreateParticle:
            if (static_cast<s32>(sGnFrame) <= mSlingMudokon.field_140_timer)
            {
                return;
            }

            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
            New_DestroyOrCreateObject_Particle(
                mSlingMudokon.mXPos,
                (mSlingMudokon.GetSpriteScale() * FP_FromInteger(20)) + mSlingMudokon.mYPos,
                mSlingMudokon.GetSpriteScale());
            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(2);
            mBrainState = EState::CreateFlash;
            return;

        case EState::CreateFlash:
            if (static_cast<s32>(sGnFrame) > mSlingMudokon.field_140_timer)
            {
                mSlingMudokon.GetAnimation().SetAnimate(true);
                mSlingMudokon.GetAnimation().SetRender(true);
                mSlingMudokon.SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                if (mSlingMudokon.mXPos > gAbe->mXPos)
                {
                    mSlingMudokon.GetAnimation().SetFlipX(true);
                }
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
                mBrainState = EState::GetAngry;
                return;
            }
            return;

        case EState::GetAngry:
            if (mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
            {
                mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, &mSlingMudokon);
                mBrainState = EState::PrepareToShoot;
                return;
            }

            if (mSlingMudokon.field_140_timer > static_cast<s32>(sGnFrame))
            {
                return;
            }
            
            mSlingMudokon.SetBrain(EBrainTypes::GiveCode);
            mSlingMudokon.mGiveCodeBrain.SetState(GiveCodeBrain::EState::Init);
            return;

        case EState::ObserveAbe:
            if (mSlingMudokon.mCodeMatches)
            {
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
                mBrainState = EState::DisappearAsDoves;
                return;
            }
            else if (mSlingMudokon.mAbeGettingCloser)
            {
                mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, &mSlingMudokon);
                mBrainState = EState::PrepareToShoot;
                return;
            }
            else
            {
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
                mBrainState = EState::GetAngry;
                return;
            }
            break;

        case EState::PrepareToShoot:
            if (mSlingMudokon.VIsObjNearby((ScaleToGridSize(mSlingMudokon.GetSpriteScale()) * FP_FromInteger(4)), gAbe))
            {
                mSlingMudokon.mDontSetDestroyed = true;
                mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_3_ShootStart);
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(15);
                mBrainState = EState::Shoot;
                return;
            }

            if (mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
            {
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
            }
            else
            {
                if (mSlingMudokon.field_140_timer <= static_cast<s32>(sGnFrame))
                {
                    mSlingMudokon.mAbeGettingCloser = false;
                    mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
                    mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_5_AngryToIdle);
                    mBrainState = EState::GetAngry;
                    return;
                }
            }
            return;

        case EState::Shoot:
            if (EventGet(kEventDeathReset))
            {
                mSlingMudokon.SetDead(true);
            }

            if (mSlingMudokon.field_140_timer > static_cast<s32>(sGnFrame) || gAbe->mHealth <= FP_FromInteger(0))
            {
                return;
            }

            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);
            mBrainState = EState::GetAngry;
            return;

        case EState::DisappearAsDoves:
            if (static_cast<s32>(sGnFrame) >= mSlingMudokon.field_140_timer)
            {
                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = relive_new Dove(
                        AnimId::Dove_Flying,
                        mSlingMudokon.mXPos + FP_FromInteger(Math_NextRandom() % 16),
                        mSlingMudokon.mYPos - FP_FromInteger(Math_NextRandom() % 16),
                        mSlingMudokon.GetSpriteScale());
                    if (pDove)
                    {
                        if (pDove->GetAnimation().GetFlipX())
                        {
                            pDove->mXPos += FP_FromInteger(8);
                        }
                        else
                        {
                            pDove->mXPos -= FP_FromInteger(8);
                        }
                    }
                }

                SfxPlayMono(relive::SoundEffects::FlyingDoves, 0);

                mSlingMudokon.GetAnimation().SetRender(false);

                mSlingMudokon.mDontSetDestroyed = !mSlingMudokon.mCodeMatches;

                mSlingMudokon.SetDead(true);
                New_DestroyOrCreateObject_Particle(mSlingMudokon.mXPos, (mSlingMudokon.GetSpriteScale() * FP_FromInteger(20)) + mSlingMudokon.mYPos, mSlingMudokon.GetSpriteScale());

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
            }
            return;

        default:
            return;
    }
}

void AskForPasswordBrain::VUpdate()
{
    LOG_WARNING("never expected Brain_2_AskForPassword_4707B0() to be used in the original levels");
    
    // NOTE: most of this stuff seems to be inlined ( see Brain_0_GiveCode() )
    switch (mBrainState)
    {
        case EState::Unknown_0:
            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(10);
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
            mBrainState = EState::Unknown_1;
            return;

        case EState::Unknown_1:
            if (static_cast<s32>(sGnFrame) <= mSlingMudokon.field_140_timer)
            {
                return;
            }

            New_DestroyOrCreateObject_Particle(
                mSlingMudokon.mXPos,
                (mSlingMudokon.GetSpriteScale() * FP_FromInteger(20)) + mSlingMudokon.mYPos,
                mSlingMudokon.GetSpriteScale());

            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(2);
            mBrainState = EState::Unknown_2;
            return;

        case EState::Unknown_2:
            if (static_cast<s32>(sGnFrame) > mSlingMudokon.field_140_timer)
            {
                mSlingMudokon.GetAnimation().SetAnimate(true);
                mSlingMudokon.GetAnimation().SetRender(true);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);

                mSlingMudokon.SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
                if (mSlingMudokon.mXPos > gAbe->mXPos)
                {
                    mSlingMudokon.GetAnimation().SetFlipX(true);
                }

                mSlingMudokon.mBufferStart = GameSpeak::FillBuffer(mSlingMudokon.mCodeConverted, mSlingMudokon.mCodeBuffer);
                mBrainState = EState::Unknown_3;
                return;
            }
            return;

        case EState::Unknown_3:
            if (mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
            {
                mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, &mSlingMudokon);
                mBrainState = EState::Unknown_7;
                return;
            }

            if (mSlingMudokon.field_140_timer > static_cast<s32>(sGnFrame) || mSlingMudokon.GetAnimation().GetCurrentFrame())
            {
                return;
            }
            Mudokon_SFX(MudSounds::ePassword_9, 0, 300, &mSlingMudokon);
            mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_2_Speak);
            mBrainState = EState::Unknown_4;
            return;

        case EState::Unknown_4:
            if (mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
            {
                mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, &mSlingMudokon);
                mBrainState = EState::Unknown_7;
                return;
            }
            else
            {
                GameSpeakEvents speak = {};
                if (mSlingMudokon.mLastEventIndex == gEventSystem->mLastEventIndex)
                {
                    if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
                    {
                        speak = GameSpeakEvents::eNone;
                    }
                    else
                    {
                        speak = GameSpeakEvents::eSameAsLast;
                    }
                }
                else
                {
                    mSlingMudokon.mLastEventIndex = gEventSystem->mLastEventIndex;
                    speak = gEventSystem->mLastEvent;
                }

                if (speak == GameSpeakEvents::eNone)
                {
                    return;
                }

                mSlingMudokon.mBufferIdx = static_cast<s16>(gEventSystem->mLastEventIndex);
                mSlingMudokon.field_144_timer2 = BaseGameObject::MakeTimer(40);
                mBrainState = EState::Unknown_5;
                return;
            }
            break;

        case EState::Unknown_5:
            if (!mSlingMudokon.VIsObj_GettingNear_On_X(gAbe))
            {
                GameSpeakEvents speak = {};
                if (mSlingMudokon.mLastEventIndex == gEventSystem->mLastEventIndex)
                {
                    if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
                    {
                        speak = GameSpeakEvents::eNone;
                    }
                    else
                    {
                        speak = GameSpeakEvents::eSameAsLast;
                    }
                }
                else
                {
                    mSlingMudokon.mLastEventIndex = gEventSystem->mLastEventIndex;
                    speak = gEventSystem->mLastEvent;
                }

                if (speak != GameSpeakEvents::eNone)
                {
                    mSlingMudokon.field_144_timer2 = BaseGameObject::MakeTimer(40);
                }

                if (static_cast<s32>(sGnFrame) <= mSlingMudokon.field_144_timer2)
                {
                    if (gEventSystem->MatchBuffer(
                            mSlingMudokon.mCodeBuffer,
                            mSlingMudokon.mBufferStart,
                            mSlingMudokon.mBufferIdx)
                        != GameSpeakMatch::eFullMatch)
                    {
                        if (gEventSystem->MatchBuffer(
                                mSlingMudokon.mCodeBuffer,
                                mSlingMudokon.mBufferStart,
                                mSlingMudokon.mBufferIdx)
                            != GameSpeakMatch::eNoMatch)
                        {
                            return;
                        }
                    }
                }

                mSlingMudokon.mCodeMatches = gEventSystem->MatchBuffer(
                                mSlingMudokon.mCodeBuffer,
                                mSlingMudokon.mBufferStart,
                                mSlingMudokon.mBufferIdx)
                             == GameSpeakMatch::eFullMatch
                         || gVoiceCheat;

                mSlingMudokon.field_140_timer = mSlingMudokon.mCodeMatches ? BaseGameObject::MakeTimer(30) : BaseGameObject::MakeTimer(10);
                mBrainState = EState::Unknown_6;
                return;
            }

            mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_1_Angry);
            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(40);
            Mudokon_SFX(MudSounds::eAngry_5, 0, 300, &mSlingMudokon);
            mBrainState = EState::Unknown_7;
            return;

        case EState::Unknown_6:
            if (mSlingMudokon.GetAnimation().GetCurrentFrame())
            {
                return;
            }

            if (static_cast<s32>(sGnFrame) <= mSlingMudokon.field_140_timer)
            {
                return;
            }

            mSlingMudokon.mNextMotion = 2;

            if (!mSlingMudokon.mCodeMatches)
            {
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(60);
                Mudokon_SFX(MudSounds::eRefuse_14, 0, 300, &mSlingMudokon);
                mBrainState = EState::Unknown_3;
                return;
            }
            else
            {
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);
                Mudokon_SFX(MudSounds::eOkay_13, 0, 300, &mSlingMudokon);
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
                mBrainState = EState::Unknown_9;
                return;
            }
            break;

        case EState::Unknown_7:
            if (mSlingMudokon.VIsObjNearby((ScaleToGridSize(mSlingMudokon.GetSpriteScale()) * FP_FromInteger(4)), gAbe))
            {
                mSlingMudokon.mDontSetDestroyed = true;
                mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_3_ShootStart);
                mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(15);
                mBrainState = EState::Unknown_8;
                return;
            }

            if (mSlingMudokon.field_140_timer > static_cast<s32>(sGnFrame))
            {
                return;
            }

            mSlingMudokon.SetNextMotion(eSlingMudMotions::Motion_5_AngryToIdle);
            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);
            mBrainState = EState::Unknown_3;
            return;

        case EState::Unknown_8:
            if (EventGet(kEventDeathReset))
            {
                mSlingMudokon.SetDead(true);
            }

            if (mSlingMudokon.field_140_timer > static_cast<s32>(sGnFrame) || gAbe->mHealth <= FP_FromInteger(0))
            {
                return;
            }

            mSlingMudokon.field_140_timer = BaseGameObject::MakeTimer(30);
            mBrainState = EState::Unknown_3;
            return;

        case EState::Unknown_9:
            if (static_cast<s32>(sGnFrame) >= mSlingMudokon.field_140_timer)
            {
                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = relive_new Dove(
                        AnimId::Dove_Flying,
                        mSlingMudokon.mXPos + FP_FromInteger(Math_NextRandom() % 16),
                        mSlingMudokon.mYPos - FP_FromInteger(Math_NextRandom() % 16),
                        mSlingMudokon.GetSpriteScale());;
                    if (pDove)
                    {
                        if (pDove->GetAnimation().GetFlipX())
                        {
                            pDove->mXPos += FP_FromInteger(8);
                        }
                        else
                        {
                            pDove->mXPos -= FP_FromInteger(8);
                        }
                    }
                }

                SfxPlayMono(relive::SoundEffects::Dove, 0);

                mSlingMudokon.GetAnimation().SetRender(false);

                mSlingMudokon.mDontSetDestroyed = !mSlingMudokon.mCodeMatches;

                mSlingMudokon.SetDead(true);
                New_DestroyOrCreateObject_Particle(mSlingMudokon.mXPos, (mSlingMudokon.GetSpriteScale() * FP_FromInteger(20)) + mSlingMudokon.mYPos, mSlingMudokon.GetSpriteScale());

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
            }
            return;

        default:
            return;
    }
}

} // namespace AO
