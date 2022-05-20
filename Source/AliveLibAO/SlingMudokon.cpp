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

using TSlingMudStateFunction = decltype(&SlingMudokon::Motion_0_Idle_46FCB0);

const TSlingMudStateFunction gSlingMudMotionTable_4CFCB0[] = {
    &SlingMudokon::Motion_0_Idle_46FCB0,
    &SlingMudokon::Motion_1_Angry_46FCF0,
    &SlingMudokon::Motion_2_Speak_46FD70,
    &SlingMudokon::Motion_3_ShootStart_46FD90,
    &SlingMudokon::Motion_4_ShootEnd_46FEA0,
    &SlingMudokon::Motion_5_AngryToIdle_46FD50};

const AnimId sSlingMudAnimIdTables_4CFCC8[6] = {
    AnimId::Mudokon_Sling_Idle, 
    AnimId::Mudokon_Sling_Angry,
    AnimId::Mudokon_Sling_Speak,
    AnimId::Mudokon_Sling_ShootStart,
    AnimId::Mudokon_Sling_ShootEnd,
    AnimId::Mudokon_Sling_AngryToIdle};

using TSlingMudBrain = decltype(&SlingMudokon::Brain_0_GiveCode_46FEC0);

const TSlingMudBrain gSlingMudBrainTable_4CFCE0[] = {
    &SlingMudokon::Brain_0_GiveCode_46FEC0,
    &SlingMudokon::Brain_1_Spawn_470230,
    &SlingMudokon::Brain_2_AskForPassword_4707B0};

SlingMudokon* SlingMudokon::ctor_46F940(Path_SlingMudokon* pTlv, s32 tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BCB40);

    field_4_typeId = Types::SlingMud_90;

    field_120_last_event_idx = -1;
    field_134_buffer_start = 0;
    field_136_buffer_idx = -1;
    field_150_res = 0;

    const AnimRecord& rec = AO::AnimRec(AnimId::Mudokon_Sling_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_FC_current_motion = eSlingMudMotions::Motion_0_Idle_46FCB0;

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
            FP_FromInteger(pTlv->field_10_top_left.field_0_x),
            FP_FromInteger(pTlv->field_10_top_left.field_2_y),
            FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
            FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
            &field_F4_pLine,
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
        field_138_brain_state = SlingMudBrainStates::Brain_2_AskForPassword_4707B0;
    }
    else
    {
        field_138_brain_state = SlingMudBrainStates::Brain_1_Spawn_470230;
    }

    field_118_code_converted = Code_Convert_476000(pTlv->field_1C_code_1, pTlv->field_1E_code_2);
    field_11C_code_length = Code_Length_475FD0(field_118_code_converted);

    field_154_previous_brain_state = 99;
    field_156_always_4 = 99;

    field_110_tlvInfo = tlvInfo;
    field_F8_pLiftPoint = nullptr;
    field_13A_brain_sub_state = 0;
    field_15A_bCodeMatches = 0;

    field_11E_flags.Clear(Flags_11E::eBit2_unused);
    field_11E_flags.Clear(Flags_11E::eBit4_bAbeGettingCloser);

    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);

    return this;
}

BaseGameObject* SlingMudokon::dtor_46FB30()
{
    SetVTable(this, 0x4BCB40);

    if (field_11E_flags.Get(Flags_11E::eBit1_bDontSetDestroyed))
    {
        gMap.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 1);
    }

    // TODO: Check it isn't 2 resources freed here
    if (field_10_anim.field_20_ppBlock != field_150_res)
    {
        if (field_150_res)
        {
            ResourceManager::FreeResource_455550(field_150_res);
        }
    }

    return dtor_401000();
}

BaseGameObject* SlingMudokon::VDestructor(s32 flags)
{
    return Vdtor_470F30(flags);
}

BaseGameObject* SlingMudokon::Vdtor_470F30(s32 flags)
{
    dtor_46FB30();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SlingMudokon::VScreenChanged()
{
    VScreenChanged_46FBE0();
}

void SlingMudokon::VScreenChanged_46FBE0()
{
    mFlags.Set(BaseGameObject::eDead);
}

void SlingMudokon::VUpdate()
{
    VUpdate_46FBF0();
}

void SlingMudokon::VUpdate_46FBF0()
{
    const auto old_motion = field_FC_current_motion;

    static auto oldBrain = field_138_brain_state;


    VCallBrain_46F880();

    if (oldBrain != field_138_brain_state)
    {
        LOG_INFO("Brain is " << field_138_brain_state);
        oldBrain = field_138_brain_state;
    }

    const auto old_x = field_A8_xpos;
    const auto old_y = field_AC_ypos;

    VCallMotion_46F8E0();

    if (old_x != field_A8_xpos || old_y != field_AC_ypos)
    {
        field_F0_pTlv = gMap.TLV_Get_At_446060(
            nullptr,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);

        VOn_TLV_Collision(field_F0_pTlv);
    }

    field_106_shot = 0;

    if (old_motion == field_FC_current_motion)
    {
        if (field_13C_redundant)
        {
            // TODO: dead code
            field_FC_current_motion = field_E4_previous_motion;
            VUpdateAnimData_46F8F0();
            field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
            field_13C_redundant = 0;
        }
    }
    else
    {
        VUpdateAnimData_46F8F0();
    }
}

void SlingMudokon::VCallBrain_46F880()
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
        mFlags.Set(BaseGameObject::eDead);
    }
}

void SlingMudokon::VCallMotion_46F8E0()
{
    (this->*gSlingMudMotionTable_4CFCB0[field_FC_current_motion])();
}

void SlingMudokon::VUpdateAnimData_46F8F0()
{
    u8** ppResBlock = nullptr;

    if (field_FC_current_motion >= 6)
    {
        ppResBlock = nullptr;
    }
    else
    {
        ppResBlock = field_150_res;
    }
    const AnimRecord& rec = AO::AnimRec(sSlingMudAnimIdTables_4CFCC8[field_FC_current_motion]);
    field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, ppResBlock);
}

void SlingMudokon::Motion_0_Idle_46FCB0()
{
    if (field_FE_next_motion == eSlingMudMotions::Motion_1_Angry_46FCF0)
    {
        field_FC_current_motion = eSlingMudMotions::Motion_1_Angry_46FCF0;
        field_FE_next_motion = -1;
    }
    else if (!field_10_anim.field_92_current_frame && field_FE_next_motion == eSlingMudMotions::Motion_2_Speak_46FD70)
    {
        field_FC_current_motion = eSlingMudMotions::Motion_2_Speak_46FD70;
        field_FE_next_motion = -1;
    }
}

void SlingMudokon::Motion_1_Angry_46FCF0()
{
    if (!field_10_anim.field_92_current_frame)
    {
        SFX_Play_43AD70(SoundEffect::SlingshotExtend_83, 0, 0);
    }

    if (field_FE_next_motion == eSlingMudMotions::Motion_3_ShootStart_46FD90)
    {
        field_FC_current_motion = eSlingMudMotions::Motion_3_ShootStart_46FD90;
        field_FE_next_motion = -1;
    }
    else if (field_FE_next_motion == eSlingMudMotions::Motion_5_AngryToIdle_46FD50)
    {
        field_FC_current_motion = eSlingMudMotions::Motion_5_AngryToIdle_46FD50;
        field_FE_next_motion = -1;
    }
}

void SlingMudokon::Motion_2_Speak_46FD70()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_10_anim.field_18_frame_table_offset == 22744)
        {
            field_FC_current_motion = eSlingMudMotions::Motion_0_Idle_46FCB0;
        }
    }
}

void SlingMudokon::Motion_3_ShootStart_46FD90()
{
    if (!field_10_anim.field_92_current_frame)
    {
        SFX_Play_43AD70(SoundEffect::SlingshotShoot_84, 0, 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_10_anim.field_18_frame_table_offset == 22700)
        {
            const FP frame_x = FP_FromInteger(field_10_anim.Get_FrameHeader_403A00(-1)->field_8_data.points[2].x);
            FP bulletXPos = {};
            FP xDistance = {};

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                bulletXPos = field_A8_xpos - frame_x;
                xDistance = -FP_FromInteger(640);
            }
            else
            {
                bulletXPos = frame_x + field_A8_xpos;
                xDistance = FP_FromInteger(640);
            }

            auto pBullet = ao_new<Bullet>();
            if (pBullet)
            {
                pBullet->ctor_409380(
                    this,
                    BulletType::eNormalBullet_1,
                    bulletXPos,
                    field_AC_ypos - FP_FromInteger(24),
                    xDistance,
                    0,
                    field_BC_sprite_scale,
                    0);
            }
            field_FC_current_motion = eSlingMudMotions::Motion_4_ShootEnd_46FEA0;
        }
    }
}

void SlingMudokon::Motion_4_ShootEnd_46FEA0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlingMudMotions::Motion_0_Idle_46FCB0;
    }
}

void SlingMudokon::Motion_5_AngryToIdle_46FD50()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlingMudMotions::Motion_0_Idle_46FCB0;
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

s16 SlingMudokon::Brain_0_GiveCode_46FEC0()
{
    switch (field_13A_brain_sub_state)
    {
        case Brain_0_GiveCode::eBrain0_Init_0:
            field_158_code_pos = 0;
            return Brain_0_GiveCode::eBrain0_GiveCode_1;

        case Brain_0_GiveCode::eBrain0_GiveCode_1:
            if (field_FC_current_motion || field_10_anim.field_92_current_frame == 0)
            {
                field_FE_next_motion = 2;
                switch (Code_LookUp_476050(field_118_code_converted, field_158_code_pos, field_11C_code_length))
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
                    field_140_timer = gnFrameCount_507670 + 30;
                    return Brain_0_GiveCode::eBrain0_PauseABit_2;
                }
            }
            return Brain_0_GiveCode::eBrain0_GiveCode_1;

        case Brain_0_GiveCode::eBrain0_PauseABit_2:
            if (static_cast<s32>(gnFrameCount_507670) <= field_140_timer)
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

            field_144_timer2 = gnFrameCount_507670 + 40;
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
                    field_120_last_event_idx = gnFrameCount_507670 + 40;
                    field_144_timer2 = gnFrameCount_507670 + 40;
                }

                if (static_cast<s32>(gnFrameCount_507670) <= field_144_timer2)
                {
                    if (pEventSystem_4FF954->MatchBuffer_40FAA0(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx) != GameSpeakMatch::eFullMatch_1 && pEventSystem_4FF954->MatchBuffer_40FAA0(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx) > GameSpeakMatch::eFullMatch_1)
                    {
                        return field_13A_brain_sub_state;
                    }
                }

                const GameSpeakMatch MatchBuffer = pEventSystem_4FF954->MatchBuffer_40FAA0(field_124_code_buffer, field_134_buffer_start, field_136_buffer_idx);
                field_13A_brain_sub_state = Brain_0_GiveCode::eBrain0_RespondToProvidedCode_5;
                if (MatchBuffer == GameSpeakMatch::eFullMatch_1 || sVoiceCheat_507708)
                {
                    field_140_timer = gnFrameCount_507670 + 30;
                    field_15A_bCodeMatches = 1;
                }
                else
                {
                    field_140_timer = gnFrameCount_507670 + 10;
                    field_15A_bCodeMatches = 0;
                }
            }
            return field_13A_brain_sub_state;

        case Brain_0_GiveCode::eBrain0_RespondToProvidedCode_5:
            if (field_10_anim.field_92_current_frame || static_cast<s32>(gnFrameCount_507670) <= field_140_timer)
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
            field_FE_next_motion = 2;
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

s16 SlingMudokon::Brain_1_Spawn_470230()
{
    switch (field_13A_brain_sub_state)
    {
        case Brain_1_Spawn::eBrain1_Init_0:
            field_140_timer = gnFrameCount_507670 + 10;
            return Brain_1_Spawn::eBrain1_CreateParticle_1;

        case Brain_1_Spawn::eBrain1_CreateParticle_1:
            if (static_cast<s32>(gnFrameCount_507670) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            SFX_Play_43AE60(SoundEffect::MenuNavigation_61, 45, 400);
            New_DestroyOrCreateObject_Particle_419D00(
                field_A8_xpos,
                (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos,
                field_BC_sprite_scale);
            field_140_timer = gnFrameCount_507670 + 2;
            return Brain_1_Spawn::eBrain1_CreateFlash_2;

        case Brain_1_Spawn::eBrain1_CreateFlash_2:
            if (static_cast<s32>(gnFrameCount_507670) > field_140_timer)
            {
                field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_FC_current_motion = eSlingMudMotions::Motion_0_Idle_46FCB0;

                auto pFlash = ao_new<Flash>();
                if (pFlash)
                {
                    pFlash->ctor_41A810(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
                }

                if (field_A8_xpos > sActiveHero_507678->field_A8_xpos)
                {
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                }
                field_140_timer = gnFrameCount_507670 + 40;
                return Brain_1_Spawn::eBrain1_GetAngry_3;
            }
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_GetAngry_3:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                field_FE_next_motion = eSlingMudMotions::Motion_1_Angry_46FCF0;
                field_140_timer = gnFrameCount_507670 + 40;
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
                return Brain_1_Spawn::eBrain1_PrepareToShoot_5;
            }

            if (field_140_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_13A_brain_sub_state;
            }
            field_154_previous_brain_state = field_138_brain_state;
            field_156_always_4 = 4;
            field_138_brain_state = SlingMudBrainStates::Brain_0_GiveCode_46FEC0;
            field_13A_brain_sub_state = Brain_1_Spawn::eBrain1_Init_0;
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_ObserveAbe_4:
            if (field_15A_bCodeMatches)
            {
                field_140_timer = gnFrameCount_507670 + 30;
                SFX_Play_43AE60(SoundEffect::PossessEffect_21, 0, -600);
                return Brain_1_Spawn::eBrain1_DisappearAsDoves_7;
            }
            else if (field_11E_flags.Get(Flags_11E::eBit4_bAbeGettingCloser))
            {
                field_FE_next_motion = eSlingMudMotions::Motion_1_Angry_46FCF0;
                field_140_timer = gnFrameCount_507670 + 40;
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
                return Brain_1_Spawn::eBrain1_PrepareToShoot_5;
            }
            else
            {
                field_140_timer = gnFrameCount_507670 + 40;
                return Brain_1_Spawn::eBrain1_GetAngry_3;
            }
            break;

        case Brain_1_Spawn::eBrain1_PrepareToShoot_5:
            if (VIsObjNearby((ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)), sActiveHero_507678))
            {
                field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                field_FE_next_motion = eSlingMudMotions::Motion_3_ShootStart_46FD90;
                field_140_timer = gnFrameCount_507670 + 15;
                return Brain_1_Spawn::eBrain1_Shoot_6;
            }

            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                field_140_timer = gnFrameCount_507670 + 40;
            }
            else
            {
                if (field_140_timer <= static_cast<s32>(gnFrameCount_507670))
                {
                    field_11E_flags.Clear(Flags_11E::eBit4_bAbeGettingCloser);
                    field_140_timer = gnFrameCount_507670 + 40;
                    field_FE_next_motion = eSlingMudMotions::Motion_5_AngryToIdle_46FD50;
                    return Brain_1_Spawn::eBrain1_GetAngry_3;
                }
            }
            return field_13A_brain_sub_state;

        case Brain_1_Spawn::eBrain1_Shoot_6:
            if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
            {
                mFlags.Set(BaseGameObject::eDead);
            }

            if (field_140_timer > static_cast<s32>(gnFrameCount_507670) || sActiveHero_507678->field_100_health <= FP_FromInteger(0))
            {
                return field_13A_brain_sub_state;
            }

            field_140_timer = gnFrameCount_507670 + 30;
            return Brain_1_Spawn::eBrain1_GetAngry_3;

        case Brain_1_Spawn::eBrain1_DisappearAsDoves_7:
            if (static_cast<s32>(gnFrameCount_507670) >= field_140_timer)
            {
                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = ao_new<Dove>();
                    if (pDove)
                    {
                        const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Flying);
                        pDove->ctor_40EFF0(
                            doveRec.mFrameTableOffset,
                            doveRec.mMaxW,
                            doveRec.mMaxH,
                            doveRec.mResourceId,
                            field_A8_xpos + FP_FromInteger(Math_NextRandom() % 16),
                            field_AC_ypos - FP_FromInteger(Math_NextRandom() % 16),
                            field_BC_sprite_scale);
                    }

                    if (pDove->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        pDove->field_A8_xpos += FP_FromInteger(8);
                    }
                    else
                    {
                        pDove->field_A8_xpos -= FP_FromInteger(8);
                    }
                }

                SFX_Play_43AD70(SoundEffect::FlyingDoves_19, 0);

                field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

                if (field_15A_bCodeMatches)
                {
                    field_11E_flags.Clear(Flags_11E::eBit1_bDontSetDestroyed);
                }
                else
                {
                    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                }

                mFlags.Set(BaseGameObject::eDead);
                New_DestroyOrCreateObject_Particle_419D00(field_A8_xpos, (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos, field_BC_sprite_scale);

                auto pFlash = ao_new<Flash>();
                if (pFlash)
                {
                    pFlash->ctor_41A810(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
                }
            }
            return field_13A_brain_sub_state;

        default:
            return field_13A_brain_sub_state;
    }
}

s16 SlingMudokon::Brain_2_AskForPassword_4707B0()
{
    LOG_WARNING("never expected Brain_2_AskForPassword_4707B0() to be used in the original levels");
    
    // NOTE: most of this stuff seems to be inlined ( see Brain_0_GiveCode_46FEC0() )
    switch (field_13A_brain_sub_state)
    {
        case 0:
            field_140_timer = gnFrameCount_507670 + 10;
            SFX_Play_43AE60(SoundEffect::PossessEffect_21, 0, -600);
            return 1;

        case 1:
            if (static_cast<s32>(gnFrameCount_507670) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            New_DestroyOrCreateObject_Particle_419D00(
                field_A8_xpos,
                (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos,
                field_BC_sprite_scale);

            field_140_timer = gnFrameCount_507670 + 2;
            return 2;

        case 2:
            if (static_cast<s32>(gnFrameCount_507670) > field_140_timer)
            {
                field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

                auto pFlash = ao_new<Flash>();
                if (pFlash)
                {
                    pFlash->ctor_41A810(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
                }

                field_140_timer = gnFrameCount_507670 + 30;

                field_FC_current_motion = eSlingMudMotions::Motion_0_Idle_46FCB0;
                if (field_A8_xpos > sActiveHero_507678->field_A8_xpos)
                {
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                }

                field_134_buffer_start = GameSpeak::sub_40FA60(field_118_code_converted, field_124_code_buffer);
                return 3;
            }
            return field_13A_brain_sub_state;

        case 3:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                field_FE_next_motion = eSlingMudMotions::Motion_1_Angry_46FCF0;
                field_140_timer = gnFrameCount_507670 + 40;
                Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
                return 7;
            }

            if (field_140_timer > static_cast<s32>(gnFrameCount_507670) || field_10_anim.field_92_current_frame)
            {
                return field_13A_brain_sub_state;
            }
            Mudokon_SFX_42A4D0(MudSounds::ePassword_9, 0, 300, this);
            field_FE_next_motion = eSlingMudMotions::Motion_2_Speak_46FD70;
            return 4;

        case 4:
            if (VIsObj_GettingNear_On_X(sActiveHero_507678))
            {
                field_FE_next_motion = eSlingMudMotions::Motion_1_Angry_46FCF0;
                field_140_timer = gnFrameCount_507670 + 40;
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
                field_144_timer2 = gnFrameCount_507670 + 40;
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
                    field_144_timer2 = gnFrameCount_507670 + 40;
                }

                if (static_cast<s32>(gnFrameCount_507670) <= field_144_timer2)
                {
                    if (pEventSystem_4FF954->MatchBuffer_40FAA0(
                            field_124_code_buffer,
                            field_134_buffer_start,
                            field_136_buffer_idx)
                        != GameSpeakMatch::eFullMatch_1)
                    {
                        if (pEventSystem_4FF954->MatchBuffer_40FAA0(
                                field_124_code_buffer,
                                field_134_buffer_start,
                                field_136_buffer_idx)
                            != GameSpeakMatch::eNoMatch_0)
                        {
                            return field_13A_brain_sub_state;
                        }
                    }
                }

                field_15A_bCodeMatches = pEventSystem_4FF954->MatchBuffer_40FAA0(
                                field_124_code_buffer,
                                field_134_buffer_start,
                                field_136_buffer_idx)
                             == GameSpeakMatch::eFullMatch_1
                         || sVoiceCheat_507708;

                field_13A_brain_sub_state = 6;

                if (field_15A_bCodeMatches)
                {
                    field_140_timer = gnFrameCount_507670 + 30;
                }
                else
                {
                    field_140_timer = gnFrameCount_507670 + 10;
                }
                return field_13A_brain_sub_state;
            }

            field_FE_next_motion = eSlingMudMotions::Motion_1_Angry_46FCF0;
            field_140_timer = gnFrameCount_507670 + 40;
            Mudokon_SFX_42A4D0(MudSounds::eAngry_5, 0, 300, this);
            return 7;

        case 6:
            if (field_10_anim.field_92_current_frame)
            {
                return field_13A_brain_sub_state;
            }

            if (static_cast<s32>(gnFrameCount_507670) <= field_140_timer)
            {
                return field_13A_brain_sub_state;
            }

            field_FE_next_motion = 2;

            if (field_15A_bCodeMatches == 0)
            {
                field_140_timer = gnFrameCount_507670 + 60;
                Mudokon_SFX_42A4D0(MudSounds::eRefuse_14, 0, 300, this);
                return 3;
            }
            else
            {
                field_140_timer = gnFrameCount_507670 + 30;
                Mudokon_SFX_42A4D0(MudSounds::eOkay_13, 0, 300, this);
                SFX_Play_43AE60(SoundEffect::PossessEffect_21, 0, -600);
                return 9;
            }
            break;

        case 7:
            if (VIsObjNearby((ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)), sActiveHero_507678))
            {
                field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                field_FE_next_motion = eSlingMudMotions::Motion_3_ShootStart_46FD90;
                field_140_timer = gnFrameCount_507670 + 15;
                return 8;
            }

            if (field_140_timer > static_cast<s32>(gnFrameCount_507670))
            {
                return field_13A_brain_sub_state;
            }

            field_FE_next_motion = eSlingMudMotions::Motion_5_AngryToIdle_46FD50;
            field_140_timer = gnFrameCount_507670 + 30;
            return 3;

        case 8:
            if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
            {
                mFlags.Set(BaseGameObject::eDead);
            }

            if (field_140_timer > static_cast<s32>(gnFrameCount_507670) || sActiveHero_507678->field_100_health <= FP_FromInteger(0))
            {
                return field_13A_brain_sub_state;
            }

            field_140_timer = gnFrameCount_507670 + 30;
            return 3;

        case 9:
            if (static_cast<s32>(gnFrameCount_507670) >= field_140_timer)
            {
                for (s32 i = 0; i < 8; i++)
                {
                    auto pDove = ao_new<Dove>();
                    if (pDove)
                    {
                        const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Flying);
                        pDove->ctor_40EFF0(
                            doveRec.mFrameTableOffset,
                            doveRec.mMaxW,
                            doveRec.mMaxH,
                            doveRec.mResourceId,
                            field_A8_xpos + FP_FromInteger(Math_NextRandom() % 16),
                            field_AC_ypos - FP_FromInteger(Math_NextRandom() % 16),
                            field_BC_sprite_scale);
                    }

                    if (pDove->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        pDove->field_A8_xpos += FP_FromInteger(8);
                    }
                    else
                    {
                        pDove->field_A8_xpos -= FP_FromInteger(8);
                    }
                }

                SFX_Play_43AD70(SoundEffect::Dove_16, 0);

                field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

                if (field_15A_bCodeMatches)
                {
                    field_11E_flags.Clear(Flags_11E::eBit1_bDontSetDestroyed);
                }
                else
                {
                    field_11E_flags.Set(Flags_11E::eBit1_bDontSetDestroyed);
                }

                mFlags.Set(BaseGameObject::eDead);
                New_DestroyOrCreateObject_Particle_419D00(field_A8_xpos, (field_BC_sprite_scale * FP_FromInteger(20)) + field_AC_ypos, field_BC_sprite_scale);

                auto pFlash = ao_new<Flash>();
                if (pFlash)
                {
                    pFlash->ctor_41A810(Layer::eLayer_Above_FG1_39, 255u, 0, 255u);
                }
            }
            return field_13A_brain_sub_state;

        default:
            return field_13A_brain_sub_state;
    }
}

} // namespace AO
