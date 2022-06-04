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

const AnimId sSlingMudAnimIdTables_4CFCC8[6] = {
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

    const AnimRecord& rec = AO::AnimRec(AnimId::Mudokon_Sling_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast(
            FP_FromInteger(pTlv->field_10_top_left.field_0_x),
            FP_FromInteger(pTlv->field_10_top_left.field_2_y),
            FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
            FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10)
        == 1)
    {
        field_A8_xpos = hitX;
        field_AC_ypos = hitY;
    }

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
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
    if (field_10_anim.field_20_ppBlock != field_150_res)
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

    const auto old_x = field_A8_xpos;
    const auto old_y = field_AC_ypos;

    VCallMotion();

    if (old_x != field_A8_xpos || old_y != field_AC_ypos)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
            nullptr,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);

        VOn_TLV_Collision(BaseAliveGameObjectPathTLV);
    }

    field_106_shot = 0;

    if (old_motion == mCurrentMotion)
    {
        if (field_13C_redundant)
        {
            // TODO: dead code
            mCurrentMotion = mPreviousMotion;
            VUpdateAnimData();
            field_10_anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
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
    if (gMap.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
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
    const AnimRecord& rec = AO::AnimRec(sSlingMudAnimIdTables_4CFCC8[mCurrentMotion]);
    field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, ppResBlock);
}

void SlingMudokon::Motion_0_Idle()
{
    if (GetNextMotion() == eSlingMudMotions::Motion_1_Angry)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_1_Angry);
        mNextMotion = -1;
    }
    else if (!field_10_anim.field_92_current_frame && GetNextMotion() == eSlingMudMotions::Motion_2_Speak)
    {
        SetCurrentMotion(eSlingMudMotions::Motion_2_Speak);
        mNextMotion = -1;
    }
}

void SlingMudokon::Motion_1_Angry()
{
    if (!field_10_anim.field_92_current_frame)
    {
        SFX_Play_Mono(SoundEffect::SlingshotExtend_83, 0, 0);
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
    if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_10_anim.field_18_frame_table_offset == 22744)
        {
            SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
        }
    }
}

void SlingMudokon::Motion_3_ShootStart()
{
    if (!field_10_anim.field_92_current_frame)
    {
        SFX_Play_Mono(SoundEffect::SlingshotShoot_84, 0, 0);
    }

    if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_10_anim.field_18_frame_table_offset == 22700)
        {
            const FP frame_x = FP_FromInteger(field_10_anim.Get_FrameHeader(-1)->field_8_data.points[2].x);
            FP bulletXPos = {};
            FP xDistance = {};

            if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                bulletXPos = field_A8_xpos - frame_x;
                xDistance = -FP_FromInteger(640);
            }
            else
            {
                bulletXPos = frame_x + field_A8_xpos;
                xDistance = FP_FromInteger(640);
            }

            ao_new<Bullet>(
                this,
                BulletType::eNormalBullet_1,
                bulletXPos,
                field_AC_ypos - FP_FromInteger(24),
                xDistance,
                0,
                field_BC_sprite_scale,
                0);
            SetCurrentMotion(eSlingMudMotions::Motion_4_ShootEnd);
        }
    }
}

void SlingMudokon::Motion_4_ShootEnd()
{
    if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
    }
}

void SlingMudokon::Motion_5_AngryToIdle()
{
    if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
            if (mCurrentMotion || field_10_anim.field_92_current_frame == 0)
            {
                mNextMotion = 2;
                switch (Code_LookUp(field_118_code_converted, field_158_code_pos, field_11C_code_length))
                {
                    case GameSpeakEvents::eWhistleHigh_1:
                        Mudokon_SFX_42A4D0(MudSounds::eWhistleHigh_1, 0, 0, this);
                        break;
                    case GameSpeakEvents::eWhistleLow_2:
                        Mudokon_SFX_42A4D0(MudSounds::eWhistleLow_2, 0, 0, this);
                        break;
                    case GameSpeakEvents::eFart_3:
                        Mudokon_SFX_42A4D0(MudSounds::eFart_7, 0, 300, this);
                        break;
                    case GameSpeakEvents::eLaugh_4:
                        Mudokon_SFX_42A4D0(MudSounds::eLaugh1_8, 0, 300, this);
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
            if (field_10_anim.field_92_current_frame || static_cast<s32>(sGnFrame) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            if (field_15A_bCodeMatches)
            {
                Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 300, this);
            }
            else
            {
                Mudokon_SFX_42A4D0(MudSounds::eRefuse_14, 0, 300, this);
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
                field_A8_xpos,
                (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos,
                field_BC_sprite_scale);
            field_140_timer = sGnFrame + 2;
            return Brain_1_Spawn::eBrain1_CreateFlash_2;

        case Brain_1_Spawn::eBrain1_CreateFlash_2:
            if (static_cast<s32>(sGnFrame) > field_140_timer)
            {
                field_10_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
                field_10_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
                SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);

                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                if (field_A8_xpos > sActiveHero_507678->field_A8_xpos)
                {
                    field_10_anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
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
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
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
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
                return Brain_1_Spawn::eBrain1_PrepareToShoot_5;
            }
            else
            {
                field_140_timer = sGnFrame + 40;
                return Brain_1_Spawn::eBrain1_GetAngry_3;
            }
            break;

        case Brain_1_Spawn::eBrain1_PrepareToShoot_5:
            if (VIsObjNearby((ScaleToGridSize(field_BC_sprite_scale) * FP_FromInteger(4)), sActiveHero_507678))
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
            if (Event_Get(kEventDeathReset_4))
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
                    const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Flying);
                    auto pDove = ao_new<Dove>(
                        doveRec.mFrameTableOffset,
                        doveRec.mMaxW,
                        doveRec.mMaxH,
                        doveRec.mResourceId,
                        field_A8_xpos + FP_FromInteger(Math_NextRandom() % 16),
                        field_AC_ypos - FP_FromInteger(Math_NextRandom() % 16),
                        field_BC_sprite_scale);
                    if (pDove)
                    {
                        if (pDove->field_10_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            pDove->field_A8_xpos += FP_FromInteger(8);
                        }
                        else
                        {
                            pDove->field_A8_xpos -= FP_FromInteger(8);
                        }
                    }
                }

                SFX_Play_Mono(SoundEffect::FlyingDoves_19, 0);

                field_10_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

                if (field_15A_bCodeMatches)
                {
                    field_11E_flags.Clear(Flags_11E::eBit1_bDontSetDestroyed);
                }
                else
                {
                    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                }

                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                New_DestroyOrCreateObject_Particle_419D00(field_A8_xpos, (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos, field_BC_sprite_scale);

                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
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
                field_A8_xpos,
                (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos,
                field_BC_sprite_scale);

            field_140_timer = sGnFrame + 2;
            return 2;

        case 2:
            if (static_cast<s32>(sGnFrame) > field_140_timer)
            {
                field_10_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
                field_10_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);

                field_140_timer = sGnFrame + 30;

                SetCurrentMotion(eSlingMudMotions::Motion_0_Idle);
                if (field_A8_xpos > sActiveHero_507678->field_A8_xpos)
                {
                    field_10_anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
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
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
                return 7;
            }

            if (field_140_timer > static_cast<s32>(sGnFrame) || field_10_anim.field_92_current_frame)
            {
                return field_13A_brain_sub_state;
            }
            Mudokon_SFX_42A4D0(MudSounds::ePassword_9, 0, 300, this);
            SetNextMotion(eSlingMudMotions::Motion_2_Speak);
            return 4;

        case 4:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                SetNextMotion(eSlingMudMotions::Motion_1_Angry);
                field_140_timer = sGnFrame + 40;
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
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
            Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
            return 7;

        case 6:
            if (field_10_anim.field_92_current_frame)
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
                Mudokon_SFX_42A4D0(MudSounds::eRefuse_14, 0, 300, this);
                return 3;
            }
            else
            {
                field_140_timer = sGnFrame + 30;
                Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 300, this);
                SFX_Play_Pitch(SoundEffect::PossessEffect_21, 0, -600);
                return 9;
            }
            break;

        case 7:
            if (VIsObjNearby((ScaleToGridSize(field_BC_sprite_scale) * FP_FromInteger(4)), sActiveHero_507678))
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
            if (Event_Get(kEventDeathReset_4))
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
                    const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Flying);
                    auto pDove = ao_new<Dove>(
                        doveRec.mFrameTableOffset,
                        doveRec.mMaxW,
                        doveRec.mMaxH,
                        doveRec.mResourceId,
                        field_A8_xpos + FP_FromInteger(Math_NextRandom() % 16),
                        field_AC_ypos - FP_FromInteger(Math_NextRandom() % 16),
                        field_BC_sprite_scale);;
                    if (pDove)
                    {
                        if (pDove->field_10_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            pDove->field_A8_xpos += FP_FromInteger(8);
                        }
                        else
                        {
                            pDove->field_A8_xpos -= FP_FromInteger(8);
                        }
                    }
                }

                SFX_Play_Mono(SoundEffect::Dove_16, 0);

                field_10_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

                if (field_15A_bCodeMatches)
                {
                    field_11E_flags.Clear(Flags_11E::eBit1_bDontSetDestroyed);
                }
                else
                {
                    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                }

                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                New_DestroyOrCreateObject_Particle_419D00(field_A8_xpos, (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos, field_BC_sprite_scale);

                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
            }
            return field_13A_brain_sub_state;

        default:
            return field_13A_brain_sub_state;
    }
}

} // namespace AO
