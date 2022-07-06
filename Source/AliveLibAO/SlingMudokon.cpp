#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlingMudokon.hpp"
#include "GameSpeak.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "Sfx.hpp"
#include "Bullet.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "CheatController.hpp"
#include "Particle.hpp"
#include "Flash.hpp"
#include "Dove.hpp"
#include "Math.hpp"
#include "Events.hpp"

void SlingMud_ForceLink()
{ }

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

SlingMudokon::SlingMudokon(Path_SlingMudokon* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::SlingMud;

    field_120_last_event_idx = -1;
    field_134_buffer_start = 0;
    field_136_buffer_idx = -1;
    field_150_res = 0;

    const AnimRecord rec = AO::AnimRec(AnimId::Mudokon_Sling_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Mudokon_Sling_Idle, ppRes);

    SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

    mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            FP_FromInteger(pTlv->mTopLeft.x),
            FP_FromInteger(pTlv->mTopLeft.y),
            FP_FromInteger(pTlv->mBottomRight.x),
            FP_FromInteger(pTlv->mBottomRight.y),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mSpriteScale != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
        == 1)
    {
        mXPos = hitX;
        mYPos = hitY;
    }

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    if (pTlv->field_1A_bDontWhistlePassword == Choice_short::eYes_1)
    {
        field_138_brain_state = SlingMudBrainStates::Brain_2_AskForPassword;
    }
    else
    {
        field_138_brain_state = SlingMudBrainStates::Brain_1_Spawn;
    }

    field_118_code_converted = Code_Convert(pTlv->field_1C_code_1, pTlv->field_1E_code_2);
    field_11C_code_length = Code_Length(field_118_code_converted);

    field_154_previous_brain_state = 99;
    field_156_always_4 = 99;

    field_110_tlvInfo = tlvInfo;
    mLiftPoint = nullptr;
    field_13A_brain_sub_state = 0;
    field_15A_bCodeMatches = 0;

    field_11E_flags.Clear(Flags_11E::eBit2_unused);
    field_11E_flags.Clear(Flags_11E::eBit4_bAbeGettingCloser);

    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
}

SlingMudokon::~SlingMudokon()
{
    if (field_11E_flags.Get(Flags_11E::eBit1_bDontSetDestroyed))
    {
        gMap.TLV_Reset(field_110_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_110_tlvInfo, -1, 0, 1);
    }

    // TODO: Check it isn't 2 resources freed here
    if (mAnim.field_20_ppBlock != field_150_res)
    {
        if (field_150_res)
        {
            ResourceManager::FreeResource_455550(field_150_res);
        }
    }
}

void SlingMudokon::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void SlingMudokon::VUpdate()
{
    const auto old_motion = mCurrentMotion;

    static auto oldBrain = field_138_brain_state;


    VCallBrain();

    if (oldBrain != field_138_brain_state)
    {
        LOG_INFO("Brain is " << field_138_brain_state);
        oldBrain = field_138_brain_state;
    }

    const auto old_x = mXPos;
    const auto old_y = mYPos;

    VCallMotion();

    if (old_x != mXPos || old_y != mYPos)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        VOnTlvCollision(BaseAliveGameObjectPathTLV);
    }

    field_106_shot = 0;

    if (old_motion == mCurrentMotion)
    {
        if (field_13C_redundant)
        {
            // TODO: dead code
            mCurrentMotion = mPreviousMotion;
            VUpdateAnimData();
            mAnim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
            field_13C_redundant = 0;
        }
    }
    else
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
        field_13A_brain_sub_state = (this->*gSlingMudBrainTable_4CFCE0[field_138_brain_state])();
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void SlingMudokon::VCallMotion()
{
    (this->*gSlingMudMotionTable_4CFCB0[mCurrentMotion])();
}

void SlingMudokon::VUpdateAnimData()
{
    u8** ppResBlock = nullptr;

    if (mCurrentMotion >= 6)
    {
        ppResBlock = nullptr;
    }
    else
    {
        ppResBlock = field_150_res;
    }
    
    mAnim.Set_Animation_Data(sSlingMudMotionAnimIds[mCurrentMotion], ppResBlock);
}

void SlingMudokon::Motion_0_Idle()
{
    if (GetNextMotion() == eSlingMudMotions::Motion_1_Angry)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_1_Angry);
        mNextMotion = -1;
    }
    else if (!mAnim.mCurrentFrame && GetNextMotion() == eSlingMudMotions::Motion_2_Speak)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_2_Speak);
        mNextMotion = -1;
    }
}

void SlingMudokon::Motion_1_Angry()
{
    if (!mAnim.mCurrentFrame)
    {
        SfxPlayMono(SoundEffect::SlingshotExtend_83, 0, 0);
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
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mAnim.mFrameTableOffset == 22744)
        {
            SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
        }
    }
}

void SlingMudokon::Motion_3_ShootStart()
{
    if (!mAnim.mCurrentFrame)
    {
        SfxPlayMono(SoundEffect::SlingshotShoot_84, 0, 0);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mAnim.mFrameTableOffset == 22700)
        {
            const FP frame_x = FP_FromInteger(mAnim.Get_FrameHeader(-1)->field_8_data.points[2].x);
            FP bulletXPos = {};
            FP xDistance = {};

            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
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
                0,
                mSpriteScale,
                0);
            SetCurrentMotion(eSlingMudMotions::Motion_4_ShootEnd);
        }
    }
}

void SlingMudokon::Motion_4_ShootEnd()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
    }
}

void SlingMudokon::Motion_5_AngryToIdle()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
    }
}

GameSpeakEvents SlingMudokon::getLastIdx()
{
    if (field_120_last_event_idx == pEventSystem_4FF954->field_18_last_event_index)
    {
        if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
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
        field_120_last_event_idx = pEventSystem_4FF954->field_18_last_event_index;
        return pEventSystem_4FF954->field_10_last_event;
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
            if (mCurrentMotion || mAnim.mCurrentFrame == 0)
            {
                mNextMotion = 2;
                switch (Code_LookUp(field_118_code_converted, field_158_code_pos, field_11C_code_length))
                {
                    case GameSpeakEvents::eWhistleHigh_1:
                        Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, this);
                        break;
                    case GameSpeakEvents::eWhistleLow_2:
                        Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, this);
                        break;
                    case GameSpeakEvents::eFart_3:
                        Mudokon_SFX(MudSounds::eFart_7, 0, 300, this);
                        break;
                    case GameSpeakEvents::eLaugh_4:
                        Mudokon_SFX(MudSounds::eLaugh1_8, 0, 300, this);
                        break;
                    default:
                        break;
                }

                field_158_code_pos++;
                if (field_158_code_pos >= field_11C_code_length)
                {
                    field_134_buffer_start = GameSpeak::sub_40FA60(field_118_code_converted, field_124_code_buffer);
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
                if (VIsObj_GettingNear_On_X(sActiveHero_507678))
                {
                    break;
                }

                return field_13A_brain_sub_state;
            }
            return Brain_0_GiveCode::eBrain0_GiveCode_1;

        case Brain_0_GiveCode::eBrain0_WaitForCode_3:
        {
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                break;
            }

            GameSpeakEvents lastIdx = getLastIdx();
            if (lastIdx == GameSpeakEvents::eNone_m1)
            {
                return field_13A_brain_sub_state;
            }

            field_144_timer2 = sGnFrame + 40;
            field_136_buffer_idx = static_cast<s16>(pEventSystem_4FF954->field_18_last_event_index);
            return Brain_0_GiveCode::eBrain0_CheckCodeMatching_4;
        }

        case Brain_0_GiveCode::eBrain0_CheckCodeMatching_4:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
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
                    if (pEventSystem_4FF954->MatchBuffer(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx) != GameSpeakMatch::eFullMatch_1 && pEventSystem_4FF954->MatchBuffer(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx) > GameSpeakMatch::eFullMatch_1)
                    {
                        return field_13A_brain_sub_state;
                    }
                }

                const GameSpeakMatch MatchBuffer = pEventSystem_4FF954->MatchBuffer(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx);
                field_13A_brain_sub_state = Brain_0_GiveCode::eBrain0_RespondToProvidedCode_5;
                if (MatchBuffer == GameSpeakMatch::eFullMatch_1 || sVoiceCheat_507708)
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
            if (mAnim.mCurrentFrame || static_cast<s32>(sGnFrame) <= field_140_timer)
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
            return field_156_always_4;

        default:
            return field_13A_brain_sub_state;
    }

    field_11E_flags.Set(Flags_11E::eBit4_bAbeGettingCloser);
    field_138_brain_state = field_154_previous_brain_state;
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

            SFX_Play_Pitch(SoundEffect::MenuNavigation_61, 45, 400);
            New_DestroyOrCreateObject_Particle_419D00(
                mXPos,
                (mSpriteScale * FP_FromInteger(20)) + mYPos,
                mSpriteScale);
            field_140_timer = sGnFrame + 2;
            return Brain_1_Spawn::eBrain1_CreateFlash_2;

        case Brain_1_Spawn::eBrain1_CreateFlash_2:
            if (static_cast<s32>(sGnFrame) > field_140_timer)
            {
                mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                if (mXPos > sActiveHero_507678->mXPos)
                {
                    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
                }
                field_140_timer = sGnFrame + 40;
                return Brain_1_Spawn::eBrain1_GetAngry_3;
            }
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_GetAngry_3:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
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
                SFX_Play_Pitch(SoundEffect::PossessEffect_21, 0, -600);
                return Brain_1_Spawn::eBrain1_DisappearAsDoves_7;
            }
            else if (field_11E_flags.Get(Flags_11E::eBit4_bAbeGettingCloser))
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
            if (VIsObjNearby((ScaleToGridSize(mSpriteScale) * FP_FromInteger(4)), sActiveHero_507678))
            {
                field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                SetNextMotion(eSlingMudMotions::Motion_3_ShootStart);
                field_140_timer = sGnFrame + 15;
                return Brain_1_Spawn::eBrain1_Shoot_6;
            }

            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                field_140_timer = sGnFrame + 40;
            }
            else
            {
                if (field_140_timer <= static_cast<s32>(sGnFrame))
                {
                    field_11E_flags.Clear(Flags_11E::eBit4_bAbeGettingCloser);
                    field_140_timer = sGnFrame + 40;
                    SetNextMotion(eSlingMudMotions::Motion_5_AngryToIdle);
                    return Brain_1_Spawn::eBrain1_GetAngry_3;
                }
            }
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_Shoot_6:
            if (EventGet(kEventDeathReset))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            if (field_140_timer > static_cast<s32>(sGnFrame) || sActiveHero_507678->mHealth <= FP_FromInteger(0))
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
                        mSpriteScale);
                    if (pDove)
                    {
                        if (pDove->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            pDove->mXPos += FP_FromInteger(8);
                        }
                        else
                        {
                            pDove->mXPos -= FP_FromInteger(8);
                        }
                    }
                }

                SfxPlayMono(SoundEffect::FlyingDoves_19, 0);

                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

                if (field_15A_bCodeMatches)
                {
                    field_11E_flags.Clear(Flags_11E::eBit1_bDontSetDestroyed);
                }
                else
                {
                    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                }

                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                New_DestroyOrCreateObject_Particle_419D00(mXPos, (mSpriteScale * FP_FromInteger(20)) + mYPos, mSpriteScale);

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
            SFX_Play_Pitch(SoundEffect::PossessEffect_21, 0, -600);
            return 1;

        case 1:
            if (static_cast<s32>(sGnFrame) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            New_DestroyOrCreateObject_Particle_419D00(
                mXPos,
                (mSpriteScale * FP_FromInteger(20)) + mYPos,
                mSpriteScale);

            field_140_timer = sGnFrame + 2;
            return 2;

        case 2:
            if (static_cast<s32>(sGnFrame) > field_140_timer)
            {
                mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                field_140_timer = sGnFrame + 30;

                SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
                if (mXPos > sActiveHero_507678->mXPos)
                {
                    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
                }

                field_134_buffer_start = GameSpeak::sub_40FA60(field_118_code_converted, field_124_code_buffer);
                return 3;
            }
            return field_13A_brain_sub_state;

        case 3:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                field_140_timer = sGnFrame + 40;
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, this);
                return 7;
            }

            if (field_140_timer > static_cast<s32>(sGnFrame) || mAnim.mCurrentFrame)
            {
                return field_13A_brain_sub_state;
            }
            Mudokon_SFX(MudSounds::ePassword_9, 0, 300, this);
            SetNextMotion(eSlingMudMotions::Motion_2_Speak);
            return 4;

        case 4:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                field_140_timer = sGnFrame + 40;
                Mudokon_SFX(MudSounds::eAngry_5, 0, 300, this);
                return 7;
            }
            else
            {
                GameSpeakEvents speak = {};
                if (field_120_last_event_idx == pEventSystem_4FF954->field_18_last_event_index)
                {
                    if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
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
                    field_120_last_event_idx = pEventSystem_4FF954->field_18_last_event_index;
                    speak = pEventSystem_4FF954->field_10_last_event;
                }

                if (speak == GameSpeakEvents::eNone_m1)
                {
                    return field_13A_brain_sub_state;
                }

                field_136_buffer_idx = static_cast<s16>(pEventSystem_4FF954->field_18_last_event_index);
                field_144_timer2 = sGnFrame + 40;
                return 5;
            }
            break;

        case 5:
            if (!VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                GameSpeakEvents speak = {};
                if (field_120_last_event_idx == pEventSystem_4FF954->field_18_last_event_index)
                {
                    if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
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
                    field_120_last_event_idx = pEventSystem_4FF954->field_18_last_event_index;
                    speak = pEventSystem_4FF954->field_10_last_event;
                }

                if (speak != GameSpeakEvents::eNone_m1)
                {
                    field_144_timer2 = sGnFrame + 40;
                }

                if (static_cast<s32>(sGnFrame) <= field_144_timer2)
                {
                    if (pEventSystem_4FF954->MatchBuffer(
                            field_124_code_buffer,
                            field_134_buffer_start,
                            field_136_buffer_idx)
                        != GameSpeakMatch::eFullMatch_1)
                    {
                        if (pEventSystem_4FF954->MatchBuffer(
                                field_124_code_buffer,
                                field_134_buffer_start,
                                field_136_buffer_idx)
                            != GameSpeakMatch::eNoMatch_0)
                        {
                            return field_13A_brain_sub_state;
                        }
                    }
                }

                field_15A_bCodeMatches = pEventSystem_4FF954->MatchBuffer(
                                field_124_code_buffer,
                                field_134_buffer_start,
                                field_136_buffer_idx)
                             == GameSpeakMatch::eFullMatch_1
                         || sVoiceCheat_507708;

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
            if (mAnim.mCurrentFrame)
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
                SFX_Play_Pitch(SoundEffect::PossessEffect_21, 0, -600);
                return 9;
            }
            break;

        case 7:
            if (VIsObjNearby((ScaleToGridSize(mSpriteScale) * FP_FromInteger(4)), sActiveHero_507678))
            {
                field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
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
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            if (field_140_timer > static_cast<s32>(sGnFrame) || sActiveHero_507678->mHealth <= FP_FromInteger(0))
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
                        mSpriteScale);;
                    if (pDove)
                    {
                        if (pDove->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            pDove->mXPos += FP_FromInteger(8);
                        }
                        else
                        {
                            pDove->mXPos -= FP_FromInteger(8);
                        }
                    }
                }

                SfxPlayMono(SoundEffect::Dove_16, 0);

                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

                if (field_15A_bCodeMatches)
                {
                    field_11E_flags.Clear(Flags_11E::eBit1_bDontSetDestroyed);
                }
                else
                {
                    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                }

                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                New_DestroyOrCreateObject_Particle_419D00(mXPos, (mSpriteScale * FP_FromInteger(20)) + mYPos, mSpriteScale);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
            }
            return field_13A_brain_sub_state;

        default:
            return field_13A_brain_sub_state;
    }
}

} // namespace AO
