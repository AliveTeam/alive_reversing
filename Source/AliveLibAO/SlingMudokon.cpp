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
#include "Dove.hpp"
#include "Math.hpp"
#include "Grid.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Path.hpp"

namespace AO {

using TSlingMudStateFunction = decltype(&SlingMudokon::Motion_0_Idle);

const TSlingMudStateFunction gSlingMudMotionTable_4CFCB0[] = {
    &SlingMudokon::Motion_0_Idle,
    &SlingMudokon::Motion_1_Angry,
    &SlingMudokon::Motion_2_Speak,
    &SlingMudokon::Motion_3_ShootStart,
    &SlingMudokon::Motion_4_ShootEnd,
    &SlingMudokon::Motion_5_AngryToIdle};

const AnimId sSlingMudMotionAnimIds[6] = {
    AnimId::Mudokon_Sling_Idle, 
    AnimId::Mudokon_Sling_Angry,
    AnimId::Mudokon_Sling_Speak,
    AnimId::Mudokon_Sling_ShootStart,
    AnimId::Mudokon_Sling_ShootEnd,
    AnimId::Mudokon_Sling_AngryToIdle};

using TSlingMudBrain = decltype(&SlingMudokon::Brain_0_GiveCode);

const TSlingMudBrain gSlingMudBrainTable_4CFCE0[] = {
    &SlingMudokon::Brain_0_GiveCode,
    &SlingMudokon::Brain_1_Spawn,
    &SlingMudokon::Brain_2_AskForPassword};

void SlingMudokon::LoadAnimations()
{
    for (auto& animId : sSlingMudMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

SlingMudokon::SlingMudokon(relive::Path_SlingMudokon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::SlingMud);

    LoadAnimations();

    field_120_last_event_idx = -1;
    field_134_buffer_start = 0;
    field_136_buffer_idx = -1;
    
    Animation_Init(GetAnimRes(AnimId::Mudokon_Sling_Idle));

    SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

    GetAnimation().SetAnimate(false);
    GetAnimation().SetRender(false);
    GetAnimation().SetSemiTrans(true);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
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
        field_138_brain_state = SlingMudBrainStates::Brain_2_AskForPassword;
    }
    else
    {
        field_138_brain_state = SlingMudBrainStates::Brain_1_Spawn;
    }

    mCodeConverted = Code_Convert(pTlv->mCode1, pTlv->mCode2);
    mCodeLength = Code_Length(mCodeConverted);

    field_154_previous_brain_state = 99;
    field_156_always_4 = 99;

    mTlvId = tlvId;
    field_13A_brain_sub_state = 0;
    field_15A_bCodeMatches = 0;

    mAbeGettingCloser = false;
    mDontSetDestroyed = true;
}

SlingMudokon::~SlingMudokon()
{
    if (mDontSetDestroyed)
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
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

    static auto oldBrain = field_138_brain_state;


    VCallBrain();

    if (oldBrain != field_138_brain_state)
    {
        LOG_INFO("Brain is %d", field_138_brain_state);
        oldBrain = field_138_brain_state;
    }

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
        field_13A_brain_sub_state = InvokeMemberFunction(this, gSlingMudBrainTable_4CFCE0, field_138_brain_state);
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
    if (field_120_last_event_idx == gEventSystem->mLastEventIndex)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
        {
            return GameSpeakEvents::eNone_m1;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_120_last_event_idx = gEventSystem->mLastEventIndex;
        return gEventSystem->mLastEvent;
    }
}

enum Brain_0_GiveCode
{
    eBrain0_Init_0 = 0,
    eBrain0_GiveCode_1 = 1,
    eBrain0_PauseABit_2 = 2,
    eBrain0_WaitForCode_3 = 3,
    eBrain0_CheckCodeMatching_4 = 4,
    eBrain0_RespondToProvidedCode_5 = 5,
};

s16 SlingMudokon::Brain_0_GiveCode()
{
    switch (field_13A_brain_sub_state)
    {
        case Brain_0_GiveCode::eBrain0_Init_0:
            field_158_code_pos = 0;
            return Brain_0_GiveCode::eBrain0_GiveCode_1;

        case Brain_0_GiveCode::eBrain0_GiveCode_1:
            if (mCurrentMotion || GetAnimation().GetCurrentFrame() == 0)
            {
                mNextMotion = 2;
                switch (Code_LookUp(mCodeConverted, field_158_code_pos, mCodeLength))
                {
                    case GameSpeakEvents::eAbe_WhistleHigh_1:
                        Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
                        break;
                    case GameSpeakEvents::eAbe_WhistleLow_2:
                        Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
                        break;
                    case GameSpeakEvents::eAbe_Fart_3:
                        Mudokon_SFX(MudSounds::eFart_7, 0, 300, this);
                        break;
                    case GameSpeakEvents::eAbe_Laugh_4:
                        Mudokon_SFX(MudSounds::eLaugh1_8, 0, 300, this);
                        break;
                    default:
                        break;
                }

                field_158_code_pos++;
                if (field_158_code_pos >= mCodeLength)
                {
                    field_134_buffer_start = GameSpeak::sub_40FA60(mCodeConverted, field_124_code_buffer);
                    return Brain_0_GiveCode::eBrain0_WaitForCode_3;
                }
                else
                {
                    field_140_timer = sGnFrame + 30;
                    return Brain_0_GiveCode::eBrain0_PauseABit_2;
                }
            }
            return Brain_0_GiveCode::eBrain0_GiveCode_1;

        case Brain_0_GiveCode::eBrain0_PauseABit_2:
            if (static_cast<s32>(sGnFrame) <= field_140_timer)
            {
                if (VIsObj_GettingNear_On_X(sActiveHero))
                {
                    break;
                }

                return field_13A_brain_sub_state;
            }
            return Brain_0_GiveCode::eBrain0_GiveCode_1;

        case Brain_0_GiveCode::eBrain0_WaitForCode_3:
        {
            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                break;
            }

            GameSpeakEvents lastIdx = getLastIdx();
            if (lastIdx == GameSpeakEvents::eNone_m1)
            {
                return field_13A_brain_sub_state;
            }

            field_144_timer2 = sGnFrame + 40;
            field_136_buffer_idx = static_cast<s16>(gEventSystem->mLastEventIndex);
            return Brain_0_GiveCode::eBrain0_CheckCodeMatching_4;
        }

        case Brain_0_GiveCode::eBrain0_CheckCodeMatching_4:
            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                break;
            }
            else
            {
                GameSpeakEvents lastIdx = getLastIdx();
                if (lastIdx != GameSpeakEvents::eNone_m1)
                {
                    field_120_last_event_idx = sGnFrame + 40;
                    field_144_timer2 = sGnFrame + 40;
                }

                if (static_cast<s32>(sGnFrame) <= field_144_timer2)
                {
                    if (gEventSystem->MatchBuffer(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx) != GameSpeakMatch::eFullMatch_1 && gEventSystem->MatchBuffer(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx) > GameSpeakMatch::eFullMatch_1)
                    {
                        return field_13A_brain_sub_state;
                    }
                }

                const GameSpeakMatch MatchBuffer = gEventSystem->MatchBuffer(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx);
                field_13A_brain_sub_state = Brain_0_GiveCode::eBrain0_RespondToProvidedCode_5;
                if (MatchBuffer == GameSpeakMatch::eFullMatch_1 || gVoiceCheat)
                {
                    field_140_timer = sGnFrame + 30;
                    field_15A_bCodeMatches = 1;
                }
                else
                {
                    field_140_timer = sGnFrame + 10;
                    field_15A_bCodeMatches = 0;
                }
            }
            return field_13A_brain_sub_state;

        case Brain_0_GiveCode::eBrain0_RespondToProvidedCode_5:
            if (GetAnimation().GetCurrentFrame() || static_cast<s32>(sGnFrame) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            if (field_15A_bCodeMatches)
            {
                Mudokon_SFX(MudSounds::eOkay_13, 0, 300, this);
            }
            else
            {
                Mudokon_SFX(MudSounds::eRefuse_14, 0, 300, this);
            }

            field_138_brain_state = field_154_previous_brain_state;
            mNextMotion = 2;
            LOG_INFO("field_156_always_4 = %d", field_156_always_4);
            return field_156_always_4;

        default:
            return field_13A_brain_sub_state;
    }

    mAbeGettingCloser = true;
    field_138_brain_state = field_154_previous_brain_state;
    LOG_INFO("field_156_always_4 = %d", field_156_always_4);
    return field_156_always_4;
}

enum Brain_1_Spawn
{
    eBrain1_Init_0 = 0,
    eBrain1_CreateParticle_1 = 1,
    eBrain1_CreateFlash_2 = 2,
    eBrain1_GetAngry_3 = 3,
    eBrain1_ObserveAbe_4 = 4,
    eBrain1_PrepareToShoot_5 = 5,
    eBrain1_Shoot_6 = 6,
    eBrain1_DisappearAsDoves_7 = 7,
};

s16 SlingMudokon::Brain_1_Spawn()
{
    switch (field_13A_brain_sub_state)
    {
        case Brain_1_Spawn::eBrain1_Init_0:
            field_140_timer = sGnFrame + 10;
            return Brain_1_Spawn::eBrain1_CreateParticle_1;

        case Brain_1_Spawn::eBrain1_CreateParticle_1:
            if (static_cast<s32>(sGnFrame) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            SFX_Play_Pitch(relive::SoundEffects::MenuNavigation, 45, 400);
            New_DestroyOrCreateObject_Particle(
                mXPos,
                (GetSpriteScale() * FP_FromInteger(20)) + mYPos,
                GetSpriteScale());
            field_140_timer = sGnFrame + 2;
            return Brain_1_Spawn::eBrain1_CreateFlash_2;

        case Brain_1_Spawn::eBrain1_CreateFlash_2:
            if (static_cast<s32>(sGnFrame) > field_140_timer)
            {
                GetAnimation().SetAnimate(true);
                GetAnimation().SetRender(true);
                SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                if (mXPos > sActiveHero->mXPos)
                {
                    GetAnimation().SetFlipX(true);
                }
                field_140_timer = sGnFrame + 40;
                return Brain_1_Spawn::eBrain1_GetAngry_3;
            }
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_GetAngry_3:
            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                field_140_timer = sGnFrame + 40;
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, this);
                return Brain_1_Spawn::eBrain1_PrepareToShoot_5;
            }

            if (field_140_timer > static_cast<s32>(sGnFrame))
            {
                return field_13A_brain_sub_state;
            }
            field_154_previous_brain_state = field_138_brain_state;
            field_156_always_4 = 4;
            field_138_brain_state = SlingMudBrainStates::Brain_0_GiveCode;
            field_13A_brain_sub_state = Brain_1_Spawn::eBrain1_Init_0;
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_ObserveAbe_4:
            if (field_15A_bCodeMatches)
            {
                field_140_timer = sGnFrame + 30;
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
                return Brain_1_Spawn::eBrain1_DisappearAsDoves_7;
            }
            else if (mAbeGettingCloser)
            {
                SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                field_140_timer = sGnFrame + 40;
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, this);
                return Brain_1_Spawn::eBrain1_PrepareToShoot_5;
            }
            else
            {
                field_140_timer = sGnFrame + 40;
                return Brain_1_Spawn::eBrain1_GetAngry_3;
            }
            break;

        case Brain_1_Spawn::eBrain1_PrepareToShoot_5:
            if (VIsObjNearby((ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)), sActiveHero))
            {
                mDontSetDestroyed = true;
                SetNextMotion(eSlingMudMotions::Motion_3_ShootStart);
                field_140_timer = sGnFrame + 15;
                return Brain_1_Spawn::eBrain1_Shoot_6;
            }

            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                field_140_timer = sGnFrame + 40;
            }
            else
            {
                if (field_140_timer <= static_cast<s32>(sGnFrame))
                {
                    mAbeGettingCloser = false;
                    field_140_timer = sGnFrame + 40;
                    SetNextMotion(eSlingMudMotions::Motion_5_AngryToIdle);
                    return Brain_1_Spawn::eBrain1_GetAngry_3;
                }
            }
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_Shoot_6:
            if (EventGet(kEventDeathReset))
            {
                SetDead(true);
            }

            if (field_140_timer > static_cast<s32>(sGnFrame) || sActiveHero->mHealth <= FP_FromInteger(0))
            {
                return field_13A_brain_sub_state;
            }

            field_140_timer = sGnFrame + 30;
            return Brain_1_Spawn::eBrain1_GetAngry_3;

        case Brain_1_Spawn::eBrain1_DisappearAsDoves_7:
            if (static_cast<s32>(sGnFrame) >= field_140_timer)
            {
                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = relive_new Dove(
                        AnimId::Dove_Flying,
                        mXPos + FP_FromInteger(Math_NextRandom() % 16),
                        mYPos - FP_FromInteger(Math_NextRandom() % 16),
                        GetSpriteScale());
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

                GetAnimation().SetRender(false);

                mDontSetDestroyed = field_15A_bCodeMatches ? false : true;

                SetDead(true);
                New_DestroyOrCreateObject_Particle(mXPos, (GetSpriteScale() * FP_FromInteger(20)) + mYPos, GetSpriteScale());

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
            }
            return field_13A_brain_sub_state;

        default:
            return field_13A_brain_sub_state;
    }
}

s16 SlingMudokon::Brain_2_AskForPassword()
{
    LOG_WARNING("never expected Brain_2_AskForPassword_4707B0() to be used in the original levels");
    
    // NOTE: most of this stuff seems to be inlined ( see Brain_0_GiveCode() )
    switch (field_13A_brain_sub_state)
    {
        case 0:
            field_140_timer = sGnFrame + 10;
            SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
            return 1;

        case 1:
            if (static_cast<s32>(sGnFrame) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            New_DestroyOrCreateObject_Particle(
                mXPos,
                (GetSpriteScale() * FP_FromInteger(20)) + mYPos,
                GetSpriteScale());

            field_140_timer = sGnFrame + 2;
            return 2;

        case 2:
            if (static_cast<s32>(sGnFrame) > field_140_timer)
            {
                GetAnimation().SetAnimate(true);
                GetAnimation().SetRender(true);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                field_140_timer = sGnFrame + 30;

                SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
                if (mXPos > sActiveHero->mXPos)
                {
                    GetAnimation().SetFlipX(true);
                }

                field_134_buffer_start = GameSpeak::sub_40FA60(mCodeConverted, field_124_code_buffer);
                return 3;
            }
            return field_13A_brain_sub_state;

        case 3:
            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                field_140_timer = sGnFrame + 40;
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, this);
                return 7;
            }

            if (field_140_timer > static_cast<s32>(sGnFrame) || GetAnimation().GetCurrentFrame())
            {
                return field_13A_brain_sub_state;
            }
            Mudokon_SFX(MudSounds::ePassword_9, 0, 300, this);
            SetNextMotion(eSlingMudMotions::Motion_2_Speak);
            return 4;

        case 4:
            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                field_140_timer = sGnFrame + 40;
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, this);
                return 7;
            }
            else
            {
                GameSpeakEvents speak = {};
                if (field_120_last_event_idx == gEventSystem->mLastEventIndex)
                {
                    if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
                    {
                        speak = GameSpeakEvents::eNone_m1;
                    }
                    else
                    {
                        speak = GameSpeakEvents::eSameAsLast_m2;
                    }
                }
                else
                {
                    field_120_last_event_idx = gEventSystem->mLastEventIndex;
                    speak = gEventSystem->mLastEvent;
                }

                if (speak == GameSpeakEvents::eNone_m1)
                {
                    return field_13A_brain_sub_state;
                }

                field_136_buffer_idx = static_cast<s16>(gEventSystem->mLastEventIndex);
                field_144_timer2 = sGnFrame + 40;
                return 5;
            }
            break;

        case 5:
            if (!VIsObj_GettingNear_On_X(sActiveHero))
            {
                GameSpeakEvents speak = {};
                if (field_120_last_event_idx == gEventSystem->mLastEventIndex)
                {
                    if (gEventSystem->mLastEvent == GameSpeakEvents::eNone_m1)
                    {
                        speak = GameSpeakEvents::eNone_m1;
                    }
                    else
                    {
                        speak = GameSpeakEvents::eSameAsLast_m2;
                    }
                }
                else
                {
                    field_120_last_event_idx = gEventSystem->mLastEventIndex;
                    speak = gEventSystem->mLastEvent;
                }

                if (speak != GameSpeakEvents::eNone_m1)
                {
                    field_144_timer2 = sGnFrame + 40;
                }

                if (static_cast<s32>(sGnFrame) <= field_144_timer2)
                {
                    if (gEventSystem->MatchBuffer(
                            field_124_code_buffer,
                            field_134_buffer_start,
                            field_136_buffer_idx)
                        != GameSpeakMatch::eFullMatch_1)
                    {
                        if (gEventSystem->MatchBuffer(
                                field_124_code_buffer,
                                field_134_buffer_start,
                                field_136_buffer_idx)
                            != GameSpeakMatch::eNoMatch_0)
                        {
                            return field_13A_brain_sub_state;
                        }
                    }
                }

                field_15A_bCodeMatches = gEventSystem->MatchBuffer(
                                field_124_code_buffer,
                                field_134_buffer_start,
                                field_136_buffer_idx)
                             == GameSpeakMatch::eFullMatch_1
                         || gVoiceCheat;

                field_13A_brain_sub_state = 6;

                if (field_15A_bCodeMatches)
                {
                    field_140_timer = sGnFrame + 30;
                }
                else
                {
                    field_140_timer = sGnFrame + 10;
                }
                return field_13A_brain_sub_state;
            }

            SetNextMotion(eSlingMudMotions::Motion_1_Angry);
            field_140_timer = sGnFrame + 40;
            Mudokon_SFX(MudSounds::eAngry_5, 0, 300, this);
            return 7;

        case 6:
            if (GetAnimation().GetCurrentFrame())
            {
                return field_13A_brain_sub_state;
            }

            if (static_cast<s32>(sGnFrame) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            mNextMotion = 2;

            if (field_15A_bCodeMatches == 0)
            {
                field_140_timer = sGnFrame + 60;
                Mudokon_SFX(MudSounds::eRefuse_14, 0, 300, this);
                return 3;
            }
            else
            {
                field_140_timer = sGnFrame + 30;
                Mudokon_SFX(MudSounds::eOkay_13, 0, 300, this);
                SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 0, -600);
                return 9;
            }
            break;

        case 7:
            if (VIsObjNearby((ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4)), sActiveHero))
            {
                mDontSetDestroyed = true;
                SetNextMotion(eSlingMudMotions::Motion_3_ShootStart);
                field_140_timer = sGnFrame + 15;
                return 8;
            }

            if (field_140_timer > static_cast<s32>(sGnFrame))
            {
                return field_13A_brain_sub_state;
            }

            SetNextMotion(eSlingMudMotions::Motion_5_AngryToIdle);
            field_140_timer = sGnFrame + 30;
            return 3;

        case 8:
            if (EventGet(kEventDeathReset))
            {
                SetDead(true);
            }

            if (field_140_timer > static_cast<s32>(sGnFrame) || sActiveHero->mHealth <= FP_FromInteger(0))
            {
                return field_13A_brain_sub_state;
            }

            field_140_timer = sGnFrame + 30;
            return 3;

        case 9:
            if (static_cast<s32>(sGnFrame) >= field_140_timer)
            {
                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = relive_new Dove(
                        AnimId::Dove_Flying,
                        mXPos + FP_FromInteger(Math_NextRandom() % 16),
                        mYPos - FP_FromInteger(Math_NextRandom() % 16),
                        GetSpriteScale());;
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

                GetAnimation().SetRender(false);

                mDontSetDestroyed = field_15A_bCodeMatches ? false : true;

                SetDead(true);
                New_DestroyOrCreateObject_Particle(mXPos, (GetSpriteScale() * FP_FromInteger(20)) + mYPos, GetSpriteScale());

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
            }
            return field_13A_brain_sub_state;

        default:
            return field_13A_brain_sub_state;
    }
}

} // namespace AO
