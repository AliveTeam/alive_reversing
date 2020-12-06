#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slog.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "PsxDisplay.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "SwitchStates.hpp"
#include "PlatformBase.hpp"
#include "CameraSwapper.hpp"
#include "SnoozeParticle.hpp"
#include "Shadow.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "Particle.hpp"
#include "GameSpeak.hpp"

void Slog_ForceLink() {}

namespace AO {

using TSlogStateFunction = decltype(&Slog::State_0_Idle_4742E0);

const TSlogStateFunction sSlogMotionTable_4CFD30[] =
{
    &Slog::State_0_Idle_4742E0,
    &Slog::State_1_Walk_4743F0,
    &Slog::State_2_Run_4749A0,
    &Slog::State_3_TurnAround_474C70,
    &Slog::State_4_Fall_4750C0,
    &Slog::State_5_Unknown_474070,
    &Slog::State_6_MoveHeadUpwards_474220,
    &Slog::State_7_SlideTurn_474DB0,
    &Slog::State_8_StopRunning_474EC0,
    &Slog::State_9_StartWalking_474690,
    &Slog::State_10_EndWalking_4747D0,
    &Slog::State_11_Land_475AB0,
    &Slog::State_12_Unknown_475B50,
    &Slog::State_13_StartFastBarking_475B70,
    &Slog::State_14_EndFastBarking_475BB0,
    &Slog::State_15_Empty_475290,
    &Slog::State_16_Sleeping_4752E0,
    &Slog::State_17_FallAsleep_475510,
    &Slog::State_18_WakeUp_475460,
    &Slog::State_19_JumpForwards_475610,
    &Slog::State_20_JumpUpwards_475890,
    &Slog::State_21_Eating_475900,
    &Slog::State_22_Empty_475A90,
    &Slog::State_23_Scratch_475550,
    &Slog::State_24_Growl_475590,
};

using TSlogBrainFunction = decltype(&Slog::Brain_0_ListeningToSlig_472450);

const TSlogBrainFunction sSlog_fns_brain_4CFE00[] =
{
    &Slog::Brain_0_ListeningToSlig_472450,
    &Slog::Brain_1_Idle_4719C0,
    &Slog::Brain_2_ChasingAbe_470F50,
    &Slog::Brain_3_Dead_4721B0
};

const static AIFunctionData<TSlogBrainFunction> sSlogBrainTable[4] =
{
    { &Slog::Brain_0_ListeningToSlig_472450, 0x472450, "Brain_0_ListeningToSlig" },
    { &Slog::Brain_1_Idle_4719C0, 0x4719C0, "Brain_1_Idle" },
    { &Slog::Brain_2_ChasingAbe_470F50, 0x470F50, "Brain_2_ChasingAbe" },
    { &Slog::Brain_3_Dead_4721B0, 0x4721B0, "Brain_3_Dead" }
};

const int sSlogFrameOffsetTable_4CFD98[25] =
{
    94456,
    94336,
    94416,
    94852,
    94708,
    94456,
    94488,
    94524,
    94572,
    94632,
    94652,
    94684,
    94804,
    94816,
    94840,
    14744,
    14784,
    14832,
    14808,
    37952,
    38008,
    38112,
    12220,
    12412,
    12500
};

ALIVE_VAR(1, 0x9F11C8, short, gNumSlogs_9F11C8, 0);
ALIVE_VAR(1, 0x9F11C4, BYTE, sSlogRndSeed_9F11C4, 0);

static BYTE Slog_NextRandom()
{
    return sRandomBytes_4BBE30[sSlogRndSeed_9F11C4++];
}

Slog* Slog::ctor_472EE0(Path_Slog* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BCBC8);

    field_148 = -1;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    if (pTlv->field_18_scale == 0)
    {
        field_BC_sprite_scale = FP_FromInteger(1);
    }
    else
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
    }

    Init_473130();

    field_138_tlvInfo = tlvInfo;
    field_114_brain_idx = 1;
    field_176 = 1;

    field_158_bark_anger = pTlv->field_1E_bark_anger;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1A_direction == 0);

    field_15A = pTlv->field_1E_bark_anger + pTlv->field_20_sleeps + pTlv->field_22_chase_anger;
    field_15C = pTlv->field_1E_bark_anger + pTlv->field_20_sleeps + pTlv->field_22_chase_anger;
    field_10C_pTarget = 0;
    field_17E = pTlv->field_1C_wakeup_anger;
    field_170 = pTlv->field_24_jump_attack_delay;
    field_168_anger_switch_id = pTlv->field_28_anger_trigger_id;

    if (pTlv->field_1C_wakeup_anger)
    {
        field_FC_current_motion = eSlogStates::State_16_Sleeping_4752E0;
        field_13C_res_idx = 1;
        field_10_anim.Set_Animation_Data_402A40(sSlogFrameOffsetTable_4CFD98[16], field_184_resources[1]);
    }
    else
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
    return this;
}

Slog* Slog::ctor_473050(FP xpos, FP ypos, FP scale)
{
    ctor_401090();
    SetVTable(this, 0x4BCBC8);

    field_148 = -1;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_BC_sprite_scale = scale;

    Init_473130();

    field_116_brain_state = 0;

    field_10C_pTarget = sControlledCharacter_50767C;
    field_176 = 0;
    sControlledCharacter_50767C->field_C_refCount++;
    field_17E = 0;
    field_158_bark_anger = 0;

    field_170 = 0;
    field_168_anger_switch_id = 0;
    field_FC_current_motion = 0;
    field_138_tlvInfo = 0xFFFF;
    field_114_brain_idx = 2;
    field_15A = 10;
    field_15C = 20;

    return this;
}

BaseGameObject* Slog::VDestructor(signed int flags)
{
    return Vdtor_473CB0(flags);
}

Slog* Slog::Vdtor_473CB0(signed int flags)
{
    dtor_473370();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Slog::dtor_473370()
{
    SetVTable(this, 0x4BCBC8);

    if (field_10C_pTarget)
    {
        field_10C_pTarget->field_C_refCount--;
        field_10C_pTarget = nullptr;
    }

    if (field_14C_pSlig)
    {
        field_14C_pSlig->field_C_refCount--;
        field_14C_pSlig = nullptr;
    }

    if (field_16C)
    {
        field_16C->field_C_refCount--;
        field_16C = nullptr;
    }

    if (field_138_tlvInfo != 0xFFFF)
    {
        if (field_100_health <= FP_FromInteger(0))
        {
            gMap_507BA8.TLV_Reset_446870(field_138_tlvInfo, -1, 0, 1);
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(field_138_tlvInfo, -1, 0, 0);
        }
    }

    for (int i = 0; i < ALIVE_COUNTOF(field_184_resources); i++)
    {
        if (field_10_anim.field_20_ppBlock != field_184_resources[i])
        {
            if (field_184_resources[i])
            {
                ResourceManager::FreeResource_455550(field_184_resources[i]);
                field_184_resources[i] = nullptr;
            }
        }
    }

    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);

    if (!field_178)
    {
        gNumSlogs_9F11C8--;
    }

    return dtor_401000();
}

__int16 Slog::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_473610(pFrom);
}

__int16 Slog::VTakeDamage_473610(BaseGameObject* pFrom)
{
    if (field_100_health <= FP_FromInteger(0))
    {
        return 1;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_10:
    {
        auto pBullet = static_cast<Bullet*>(pFrom);

        if (pBullet->field_20 <= FP_FromInteger(0))
        {
            auto pBlood = ao_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_4072B0(
                    field_A8_xpos,
                    pBullet->field_1C_ypos,
                    -FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_BC_sprite_scale,
                    50);
            }
        }
        else
        {
            auto pBlood = ao_new<Blood>();
            if (pBlood)
            {
                pBlood->ctor_4072B0(
                    field_A8_xpos,
                    pBullet->field_1C_ypos,
                    FP_FromInteger(24),
                    FP_FromInteger(0),
                    field_BC_sprite_scale,
                    50);
            }
        }

        Sfx_475BD0(9);

        field_100_health = FP_FromInteger(0);
        field_114_brain_idx = 3;
        field_134 = 2;
        field_11C_timer = gnFrameCount_507670 + 90;
        field_FC_current_motion = eSlogStates::State_22_Empty_475A90;
        field_13C_res_idx = 3;
        field_10_anim.Set_Animation_Data_402A40(sSlogFrameOffsetTable_4CFD98[22], field_184_resources[3]);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
        gNumSlogs_9F11C8--;
        field_178 = 1;
        return 1;
    }

    case Types::eBaseBomb_30:
    case Types::eMeatSaw_56:
    case Types::eExplosion_74:
    {
        Sfx_475BD0(9);
        field_100_health = FP_FromInteger(0);
        auto pGibs = ao_new<Gibs>();
        if (pGibs)
        {
            pGibs->ctor_407B20(
                2,
                field_A8_xpos,
                field_AC_ypos,
                field_B4_velx,
                field_B8_vely,
                field_BC_sprite_scale);
        }

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);
        auto pBlood = ao_new<Blood>();
        if (pBlood)
        {
            pBlood->ctor_4072B0(
                FP_FromInteger((bRect.w + bRect.x) / 2),
                FP_FromInteger((bRect.h + bRect.y) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_BC_sprite_scale,
                50);
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return 1;
    }

    case Types::eAbilityRing_69:
    case Types::eSlig_88:
        if (field_17C_res)
        {
            return 1;
        }
        field_17C_res = 1;
        Sfx_475BD0(9);
        break;

    case Types::eZBall_92:
        return 1;

    case Types::eRockSpawner_32:
    case Types::eRollingBall_72:
        Slog::Sfx_475BD0(9);
        // Fall through

    case Types::eElectrocute_103:
        field_100_health = FP_FromInteger(0);
        field_114_brain_idx = 3;
        field_134 = 2;
        field_11C_timer = gnFrameCount_507670 + 90;
        field_FC_current_motion = eSlogStates::State_22_Empty_475A90;
        field_13C_res_idx = 3;
        field_10_anim.Set_Animation_Data_402A40(sSlogFrameOffsetTable_4CFD98[22], field_184_resources[3]);
        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
        break;

    default:
        Sfx_475BD0(9);
        break;
    }
    return 1;
}

void Slog::VOn_TLV_Collision(Path_TLV* pTlv)
{
    VOn_Tlv_Collision_473970(pTlv);
}

void Slog::VOn_Tlv_Collision_473970(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_5)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_100_health = FP_FromInteger(0);
        }
        pTlv = gMap_507BA8.TLV_Get_At_446060(pTlv, field_A8_xpos, field_AC_ypos, field_A8_xpos, field_AC_ypos);
    }
}

void Slog::VUpdate()
{
    VUpdate_4739C0();
}

void Slog::VUpdate_4739C0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const __int16 old_motion = field_FC_current_motion;
    const auto oldBrain = sSlog_fns_brain_4CFE00[field_114_brain_idx];
    field_116_brain_state = (this->*sSlog_fns_brain_4CFE00[field_114_brain_idx])();

    if (oldBrain != sSlog_fns_brain_4CFE00[field_114_brain_idx])
    {
        //LOG_INFO("brain changed from " << GetOriginalFn(oldBrain, sSlogBrainTable).fnName << " to " << GetOriginalFn(sSlog_fns_brain_4CFE00[field_114_brain_idx], sSlogBrainTable).fnName);
    }

    if (showDebugCreatureInfo_5076E0)
    {
        DDCheat::DebugOut_495990("Slog:  Motion=%d  BrainState=%d\n", field_FC_current_motion, field_116_brain_state);
    }

    const FP old_x = field_A8_xpos;
    const FP old_y = field_AC_ypos;
    (this->*sSlogMotionTable_4CFD30[field_FC_current_motion])();

    if (old_x != field_A8_xpos || old_y != field_AC_ypos)
    {
        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
            nullptr,
            field_A8_xpos,
            field_AC_ypos,
            field_A8_xpos,
            field_AC_ypos);

        VOn_TLV_Collision(field_F0_pTlv);
    }

    if (old_motion == field_FC_current_motion)
    {
        if (field_118) // TODO: Might always be zero, check me when slog is completed
        {
            field_FC_current_motion = field_E4_previous_motion;
            SetAnimFrame();
            field_118 = 0;
        }
        return;
    }

    SetAnimFrame();

    if (field_FC_current_motion == eSlogStates::State_5_Unknown_474070)
    {
        field_10_anim.SetFrame_402AC0(field_E6_last_anim_frame);
    }
}

void Slog::SetAnimFrame()
{
    BYTE** ppRes = ResBlockForMotion(field_FC_current_motion);
    field_10_anim.Set_Animation_Data_402A40(sSlogFrameOffsetTable_4CFD98[field_FC_current_motion], ppRes);
}

BYTE** Slog::ResBlockForMotion(__int16 motion)
{
    if (motion < eSlogStates::State_15_Empty_475290)
    {
        field_13C_res_idx = 0;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < eSlogStates::State_19_JumpForwards_475610)
    {
        field_13C_res_idx = 1;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < eSlogStates::State_22_Empty_475A90)
    {
        field_13C_res_idx = 2;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < eSlogStates::State_23_Scratch_475550)
    {
        field_13C_res_idx = 3;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < 25)
    {
        field_13C_res_idx = 4;
        return field_184_resources[field_13C_res_idx];
    }
    else
    {
        field_13C_res_idx = 0;
        return field_184_resources[field_13C_res_idx];
    }
}

void Slog::MoveOnLine_4740F0()
{
    const FP xpos = field_A8_xpos;
    if (field_F4_pLine)
    {
        field_F4_pLine = field_F4_pLine->MoveOnLine_40CA20(
            &field_A8_xpos,
            &field_AC_ypos,
            field_B4_velx);
        if (field_F4_pLine)
        {
            if (field_F8_pLiftPoint)
            {
                if (field_F4_pLine->field_8_type != 32 && field_F4_pLine->field_8_type != 36)
                {
                    field_F8_pLiftPoint->VRemove(this);
                    field_F8_pLiftPoint->field_C_refCount--;
                    field_F8_pLiftPoint = nullptr;
                }
            }
            else
            {
                if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
                {
                    PSX_RECT rect = {};
                    VGetBoundingRect(&rect, 1);
                    rect.y += 5;
                    rect.h += 5;
                    VOnCollisionWith(
                        { rect.x, rect.y },
                        { rect.w, rect.h },
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                }
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_E8_LastLineYPos = field_AC_ypos;
            field_12C = FP_FromInteger(0);
            field_FC_current_motion = eSlogStates::State_4_Fall_4750C0;
            field_A8_xpos = field_B4_velx + xpos;
        }
    }
    else
    {
        field_12C = FP_FromInteger(0);
        field_E8_LastLineYPos = field_AC_ypos;
        field_FC_current_motion = eSlogStates::State_4_Fall_4750C0;
    }
}

const TintEntry sSlogTints_4CFE10[3] =
{
    { 5, 48u, 48u, 48u },
    { 6, 48u, 48u, 48u },
    { -1, 127u, 127u, 127u }
};

void Slog::Init_473130()
{
    field_184_resources[0] = nullptr;
    field_184_resources[1] = nullptr;
    field_184_resources[2] = nullptr;
    field_184_resources[3] = nullptr;
    field_184_resources[4] = nullptr;

    field_184_resources[0] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDogbasicResID, 1, 0);
    field_184_resources[1] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDogrstnResID, 1, 0);
    field_184_resources[2] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDogattkResID, 1, 0);
    field_184_resources[3] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDogknfdResID, 1, 0);
    field_184_resources[4] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDogidleResID, 1, 0);
    
    Animation_Init_417FD0(
        94456,
        121,
        57,
        field_184_resources[0],
        1);

    field_6_flags.Set(Options::eCanExplode_Bit7);
    field_10_anim.field_1C_fn_ptr_array = kSlog_Anim_Frame_Fns_4CEBF4;
    field_11C_timer = 0;
    field_120 = 0;
    field_124 = -1;
    field_116_brain_state = 0;
    field_FE_next_state = -1;
    field_EC = 3;
    field_13C_res_idx = 0;
    field_4_typeId = Types::eSlog_89;
    field_F8_pLiftPoint = nullptr;
    field_118 = 0;
    field_134 = 2;
    field_17A = 1;
    field_17C_res = 0;
    field_14C_pSlig = 0;
    field_10C_pTarget = 0;

    field_10A_flags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_10A_flags.Set(Flags_10A::e10A_Bit6);

    field_178 = 0;
    field_16C = 0;

    SetTint_418750(sSlogTints_4CFE10, gMap_507BA8.field_0_current_level);

    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 34;
        field_C6_scale = 1;
    }
    else
    {
        field_10_anim.field_C_layer = 15;
        field_C6_scale = 0;
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_AC_ypos + FP_FromInteger(24),
        &field_F4_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale == FP_FromInteger(1) ? 0x01 : 0x10) == 1)
    {
        field_AC_ypos = hitY;
    }

    MapFollowMe_401D30(FALSE);

    field_D0_pShadow= ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    gNumSlogs_9F11C8++;
}

__int16 Slog::ToNextMotion_473CE0()
{
    switch (field_FE_next_state)
    {
    case eSlogStates::State_3_TurnAround_474C70:
        field_FC_current_motion = eSlogStates::State_3_TurnAround_474C70;
        field_FE_next_state = -1;
        return 1;

    case eSlogStates::State_1_Walk_4743F0:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(9));
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx * FP_FromInteger(9)))
        {
            field_FC_current_motion = eSlogStates::State_9_StartWalking_474690;
            field_FE_next_state = -1;
            return 1;
        }

        ToIdle();
        return 0;

    case eSlogStates::State_2_Run_4749A0:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(3));
        }
        else
        {
            field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(3));
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx * FP_FromInteger(4)))
        {
            field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
            field_FE_next_state = -1;
            return 1;
        }

        ToIdle();
        return 0;
    }
    return 0;
}

void Slog::ToIdle()
{
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

    MapFollowMe_401D30(FALSE);
    field_12C = FP_FromInteger(0);;
    field_130 = 0;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    field_FE_next_state = -1;

    field_128 = 60 * Slog_NextRandom() / 256 + gnFrameCount_507670 + 120;
}

void Slog::ToJump_473FB0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (FP_FromDouble(-10.18) * field_BC_sprite_scale);
    }
    else
    {
        field_B4_velx = (FP_FromDouble(10.18) * field_BC_sprite_scale);
    }

    field_B8_vely = (FP_FromInteger(-8) * field_BC_sprite_scale);
    field_E8_LastLineYPos = field_AC_ypos;

    field_FC_current_motion = eSlogStates::State_19_JumpForwards_475610;
    
    VOnTrapDoorOpen();

    field_F4_pLine = nullptr;

    Sfx_475BD0(8);

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType8, this, 0, 0);
    }
}

SfxDefinition sSlogSfx_4CFE40[20] =
{
    { 0, 12, 38, 30, 0, 0, 0 },
    { 0, 12, 39, 30, 0, 0, 0 },
    { 0, 12, 40, 100, -256, 0, 0 },
    { 0, 12, 41, 60, 0, 0, 0 },
    { 0, 12, 42, 50, 0, 0, 0 },
    { 0, 12, 40, 100, -256, 0, 0 },
    { 0, 12, 44, 90, 0, 0, 0 },
    { 0, 12, 60, 100, 0, 0, 0 },
    { 0, 18, 45, 100, -256, 0, 0 },
    { 0, 18, 46, 127, -127, 127, 0 },
    { 0, 18, 47, 70, 0, 0, 0 },
    { 0, 18, 48, 70, 0, 0, 0 },
    { 0, 18, 49, 30, 0, 0, 0 },
    { 0, 22, 61, 45, 0, 0, 0 },
    { 0, 22, 62, 45, 0, 0, 0 },
    { 0, 3, 59, 67, 0, 0, 0 },
    { 0, 22, 33, 45, 0, 0, 0 },
    { 0, 22, 34, 45, 0, 0, 0 },
    { 0, 22, 35, 45, 0, 0, 0 },
    { 0, 22, 36, 45, 0, 0, 0 }
};

void Slog::Sfx_475BD0(int soundId)
{
    int volumeLeft = 0;
    int volumeRight = 0;

    const SfxDefinition& sndDef = sSlogSfx_4CFE40[static_cast<int>(soundId)];
    const auto defaultSndIdxVol = sndDef.field_C_default_volume;
    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos
    );
    PSX_RECT worldRect;
    gMap_507BA8.Get_Camera_World_Rect_444C30(direction, &worldRect);
    volumeLeft = volumeRight;
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
        {
            break;
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            FP percentHowFar = (FP_FromInteger(worldRect.w) - field_A8_xpos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (field_A8_xpos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            break;
        }
        default:
            return;
    }
    SFX_SfxDefinition_Play_477330(&sndDef,
        static_cast<short>(volumeLeft),
        static_cast<short>(volumeRight),
        static_cast<short>(sndDef.field_E_pitch_min),
        static_cast<short>(sndDef.field_10_pitch_max)
    );
}

__int16 Slog::IsPlayerNear_471930()
{
    if (FP_Abs(sActiveHero_507678->field_A8_xpos - field_A8_xpos) >= (FP_FromInteger(100) * field_BC_sprite_scale) ||
        FP_Abs(sActiveHero_507678->field_AC_ypos - field_AC_ypos) >= (FP_FromInteger(25) * field_BC_sprite_scale) ||
        sActiveHero_507678->field_BC_sprite_scale != field_BC_sprite_scale)
    {
        return 0;
    }
    return 1;
}

BaseAliveGameObject* Slog::FindAbeMudOrSlig_4722F0()
{
    BaseAliveGameObject* pResult = nullptr;
    FP minDist = FP_FromInteger(gPsxDisplay_504C78.field_0_width);

    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        bRect.x -= gPsxDisplay_504C78.field_0_width;
    }
    else
    {
        bRect.w += gPsxDisplay_504C78.field_0_width;
    }

    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != field_14C_pSlig && pObj != this)
        {
            if (pObj->field_4_typeId == Types::eAbe_43 || pObj->field_4_typeId == Types::eMudokon_75 || pObj->field_4_typeId == Types::eSlig_88)
            {
                PSX_RECT objRect = {};
                pObj->VGetBoundingRect(&objRect, 1);

                if (objRect.x <= bRect.w
                    && objRect.w >= bRect.x
                    && objRect.h >= bRect.y
                    && objRect.y <= bRect.h)
                {
                    const FP xd = FP_Abs(pObj->field_A8_xpos - field_A8_xpos);
                    if (xd < minDist)
                    {
                        pResult = pObj;
                        minDist = xd;
                    }
                }
            }
        }
    }
    return pResult;
}

void Slog::VScreenChanged()
{
    VScreenChanged_473480();
}

void Slog::VScreenChanged_473480()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);

        if (field_10C_pTarget)
        {
            field_10C_pTarget->field_C_refCount--;
            field_10C_pTarget = nullptr;
        }

        if (field_14C_pSlig)
        {
            field_14C_pSlig->field_C_refCount--;
            field_14C_pSlig = nullptr;
        }

        if (field_16C)
        {
            field_16C->field_C_refCount--;
            field_16C = nullptr;
        }
    }
}

void Slog::VRender(int** pOrderingTable)
{
    VRender_4735D0(pOrderingTable);
}

void Slog::VRender_4735D0(int** ppOt)
{
    if (field_8_update_delay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Slog::VOnThrowableHit(BaseGameObject* pFrom)
{
    VOnThrowableHit_4735F0(pFrom);
}

void Slog::VOnThrowableHit_4735F0(BaseGameObject* /*pFrom*/)
{
    field_156 += field_15C;
}

__int16 Slog::HandleEnemyStopper_473BD0()
{
    FP xpos = {};
    if (field_B4_velx >= FP_FromInteger(0))
    {
        xpos = field_A8_xpos + (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2));
    }
    else
    {
        xpos = field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(2));
    }

    auto pStopper = static_cast<Path_EnemyStopper*>(gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(xpos),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::EnemyStopper_79));

    if (!pStopper)
    {
        return 0;
    }

    if (pStopper->field_18_direction != (field_B4_velx > FP_FromInteger(0) ? Path_EnemyStopper::StopDirection::Right_1 : Path_EnemyStopper::StopDirection::Left_0))
    {
        return 0;
    }

    if (!SwitchStates_Get(pStopper->field_1A_id))
    {
        return 1;
    }

    return 0;
}

void Slog::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_4740C0();
}

void Slog::VOnTrapDoorOpen_4740C0()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove_451680(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }
}

void Slog::State_0_Idle_4742E0()
{
    if (!ToNextMotion_473CE0())
    {
        if (field_FE_next_state == eSlogStates::State_19_JumpForwards_475610)
        {
            ToJump_473FB0();
        }
        else if (field_FE_next_state == -1)
        {
            if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
            {
                if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
                {
                    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_17, 1, 0);
                }

                if (gMap_507BA8.GetDirection(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos) >= CameraPos::eCamCurrent_0
                    && gMap_507BA8.GetDirection(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_AC_ypos) >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::GetAbmientAndMusicInfo_443840(nullptr, nullptr, nullptr) == MusicController::MusicTypes::eType9)
                    {
                        MusicController::sub_443810(MusicController::MusicTypes::eType9, this, 0, 0);
                    }
                    else
                    {
                        MusicController::sub_443810(MusicController::MusicTypes::eType7, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

const int sSlogWalkVelXTable_4BCC28[18] =
{
    87355,
    162991,
    166294,
    129418,
    197630,
    203424,
    180786,
    205305,
    238954,
    153781,
    145622,
    142723,
    145083,
    131209,
    158174,
    143230,
    268753,
    285268
};

void Slog::State_1_Walk_4743F0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(-sSlogWalkVelXTable_4BCC28[field_10_anim.field_92_current_frame]));
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(sSlogWalkVelXTable_4BCC28[field_10_anim.field_92_current_frame]));
    }

    if (WallHit_401930(
        field_BC_sprite_scale * FP_FromInteger(20),
        field_B4_velx))
    {
        ToIdle();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_A8_xpos += field_B4_velx + ( ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
        }
        else
        {
            field_A8_xpos += field_B4_velx - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
        }
    }
    else
    {
        MoveOnLine_4740F0();

        if (field_FC_current_motion == eSlogStates::State_1_Walk_4743F0)
        {
            if (field_10_anim.field_92_current_frame == 2 || field_10_anim.field_92_current_frame == 11)
            {
                if (field_FE_next_state == eSlogStates::State_0_Idle_4742E0)
                {
                    field_FC_current_motion = eSlogStates::State_10_EndWalking_4747D0;
                    field_FE_next_state = -1;
                }

                if (field_FE_next_state == eSlogStates::State_3_TurnAround_474C70)
                {
                    ToIdle();
                    field_FC_current_motion = eSlogStates::State_3_TurnAround_474C70;
                }

            }
            else if (field_10_anim.field_92_current_frame == 5 || field_10_anim.field_92_current_frame == 14)
            {
                if (!field_126_movedOffScreen)
                {
                    field_126_movedOffScreen = 1;
                    MapFollowMe_401D30(FALSE);
                }

                if (field_FE_next_state == eSlogStates::State_2_Run_4749A0)
                {
                    field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
                    field_FE_next_state = -1;
                }
            }
            else
            {
                field_126_movedOffScreen = 0;
            }
        }
    }
}

const int sSlogRunVelXTable_4BCC70[10] =
{
    565307,
    221956,
    230637,
    334381,
    441752,
    512611,
    550112,
    428138,
    2097152000,
    0
};


void Slog::State_2_Run_4749A0()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType8, this, 0, 0);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(-sSlogRunVelXTable_4BCC70[field_10_anim.field_92_current_frame]));
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(sSlogRunVelXTable_4BCC70[field_10_anim.field_92_current_frame]));
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine_4740F0();

        if (field_FC_current_motion == eSlogStates::State_2_Run_4749A0)
        {
            if (Slog_NextRandom() < 35u)
            {
                Sfx_475BD0(5);
            }

            if (field_10_anim.field_92_current_frame == 7)
            {
                Sfx_475BD0(16);
            }

            if (field_10_anim.field_92_current_frame == 4 || field_10_anim.field_92_current_frame == 7)
            {
                if (!field_126_movedOffScreen)
                {
                    field_126_movedOffScreen = 1;
                    MapFollowMe_401D30(FALSE);
                }

                if (field_FE_next_state == eSlogStates::State_0_Idle_4742E0)
                {
                    field_FC_current_motion = eSlogStates::State_7_SlideTurn_474DB0;
                    field_FE_next_state = -1;
                }
                else if (field_FE_next_state == eSlogStates::State_19_JumpForwards_475610)
                {
                    ToJump_473FB0();
                    field_FE_next_state = -1;
                }
                else if (field_FE_next_state != -1)
                {
                    field_FC_current_motion = field_FE_next_state;
                    field_FE_next_state = -1;
                }
            }
            else
            {
                field_126_movedOffScreen = 0;
            }
        }
    }
}

void Slog::State_3_TurnAround_474C70()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle();
    }
}

void Slog::State_4_Fall_4750C0()
{
    if (field_B4_velx > FP_FromInteger(0))
    {
        field_B4_velx -= field_BC_sprite_scale * field_12C;

        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    if (field_B4_velx < FP_FromInteger(0))
    {
        field_B4_velx += field_BC_sprite_scale * field_12C;
        if (field_B4_velx > FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};

    if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->field_8_type)
        {
        case 0:
        case 4:
        case 32:
        case 36:
            field_F4_pLine = pLine;
            field_AC_ypos = hitY;
            field_A8_xpos = hitX;
            MapFollowMe_401D30(FALSE);

            if (field_F4_pLine->field_8_type == 32 || field_F4_pLine->field_8_type == 36)
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect,1);
                bRect.y += 5;
                bRect.h = FP_GetExponent(field_AC_ypos) + 5;
                VOnCollisionWith(
                    { bRect.x, bRect.y },
                    { bRect.w, bRect.h },
                    ObjListPlatforms_50766C,
                    1,
                    (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
            }
            field_FC_current_motion = eSlogStates::State_11_Land_475AB0;
            break;

        case 1:
        case 2:
        case 5:
        case 6:
            field_A8_xpos = hitX - field_B4_velx;
            field_AC_ypos = hitY;
            MapFollowMe_401D30(FALSE);
            field_B4_velx = FP_FromInteger(0);
            break;

        default:
            return;
        }
    }
}

void Slog::State_5_Unknown_474070()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        field_FC_current_motion = field_E4_previous_motion;
        if (field_F8_pLiftPoint)
        {
            field_A8_xpos = FP_FromInteger((field_F4_pLine->field_0_rect.x + field_F4_pLine->field_0_rect.w) / 2);
            field_AC_ypos = FP_FromInteger(field_F4_pLine->field_0_rect.y);
        }
    }
}

void Slog::State_6_MoveHeadUpwards_474220()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_475BD0(2);
        field_144 = 1;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange_450F20(0, 100) < 30)
        {
            Sfx_475BD0(4);
        }
        
        ToIdle();
    }
}

const int sSlogSlideTurnVelXTable_4BCC98[12] =
{
  227344,
  298123,
  566427,
  610336,
  425656,
  303562,
  256704,
  284098,
  239927,
  84017,
  2097152000,
  0
};

void Slog::State_7_SlideTurn_474DB0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_475BD0(13);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(-sSlogSlideTurnVelXTable_4BCC98[field_10_anim.field_92_current_frame]));
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(sSlogSlideTurnVelXTable_4BCC98[field_10_anim.field_92_current_frame]));
    }


    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx) || 
        field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine_4740F0();
    }
}

const int sSlogStopRunningVelX_4BCCC8[14] =
{
    1127088,
    780402,
    558956,
    480772,
    273168,
    348181,
    249772,
    98546,
    33064,
    // TODO: Convert all these to FP instead of hack casting to fit
    (int)4294951393,
    (int)4294928227,
    (int)4294885829,
    (int)4294646716,
    (int)2097152000
};


void Slog::State_8_StopRunning_474EC0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(-sSlogStopRunningVelX_4BCCC8[field_10_anim.field_92_current_frame]));
    }
    else
    {
        field_B4_velx = (field_BC_sprite_scale * FP_FromRaw(sSlogStopRunningVelX_4BCCC8[field_10_anim.field_92_current_frame]));
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine_4740F0();

        if (field_FC_current_motion == eSlogStates::State_8_StopRunning_474EC0)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_401D30(FALSE);

                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_B4_velx = (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(3));
                    field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
                    field_10_anim.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
                    field_B4_velx = -(ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(3));
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                }
            }
        }
    }
}

void Slog::State_9_StartWalking_474690()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlogStates::State_1_Walk_4743F0;
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
    {
        ToIdle();
        
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_A8_xpos += field_B4_velx + (ScaleToGridSize_41FA30(field_BC_sprite_scale) /  FP_FromInteger(2));
        }
        else
        {
            field_A8_xpos += field_B4_velx - (ScaleToGridSize_41FA30(field_BC_sprite_scale) /  FP_FromInteger(2));
        }
    }
    else
    {
        MoveOnLine_4740F0();
    }
}

void Slog::State_10_EndWalking_4747D0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(0);

        if (!ToNextMotion_473CE0())
        {
            ToIdle();
        }
    }

    if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
    {
        ToIdle();

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_A8_xpos += field_B4_velx + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2)); 
        }
        else
        {
            field_A8_xpos += field_B4_velx - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
        }
    }
    else
    {
        MoveOnLine_4740F0();
    }
}

void Slog::State_11_Land_475AB0()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_475BD0(15);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::State_12_Unknown_475B50()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eSlogStates::State_13_StartFastBarking_475B70;
    }
}

void Slog::State_13_StartFastBarking_475B70()
{
    if (field_10_anim.field_92_current_frame == 0)
    {
        Sfx_475BD0(2);
        field_144 = 1;
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = eSlogStates::State_14_EndFastBarking_475BB0;
        }
    }
}

void Slog::State_14_EndFastBarking_475BB0()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
}

void Slog::State_15_Empty_475290()
{
    if (field_10_anim.field_92_current_frame == 0 || field_10_anim.field_92_current_frame == 6)
    {
        Sfx_475BD0(5);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

void Slog::State_16_Sleeping_4752E0()
{
    if (field_FE_next_state != -1 && field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = field_FE_next_state;
        field_FE_next_state = -1;
        return;
    }

    bool bSpawnParticle = false;
    if (gnFrameCount_507670 % 60)
    {
        if (!((gnFrameCount_507670 - 20) % 60))
        {
            Sfx_475BD0(11);
            if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos,
                0))
            {
                bSpawnParticle = true;
            }
        }
    }
    else
    {
        Sfx_475BD0(10);
        if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            bSpawnParticle = true;
        }
    }

    auto pSnoozeParticle = ao_new<SnoozeParticle>();
    if (pSnoozeParticle && bSpawnParticle)
    {
        pSnoozeParticle->ctor_464320(
            field_A8_xpos
            + ((field_10_anim.field_4_flags.Get(AnimFlags::eBit10_alternating_flag)) != 0 ? FP_FromInteger(-18) : FP_FromInteger(18)),
            field_AC_ypos - FP_FromInteger(13),
            field_10_anim.field_C_layer,
            field_10_anim.field_14_scale);
    }
}

void Slog::State_17_FallAsleep_475510()
{
    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }

    MusicController::sub_443810(MusicController::MusicTypes::eType0, this, 0, 0);
}

void Slog::State_18_WakeUp_475460()
{
    for (int i=0; i<gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eSnoozParticle_87)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1D4_state = SnoozeParticle::SnoozeParticleState::BlowingUp_2;
        }
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
            Sfx_475BD0(4);
        }
    }

    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::sub_443810(MusicController::MusicTypes::eType7, this, 0, 0);
    }
}

void Slog::State_19_JumpForwards_475610()
{
    field_B8_vely += (field_BC_sprite_scale * FP_FromDouble(1.8));

    if (field_B8_vely > (field_BC_sprite_scale * FP_FromInteger(20)))
    {
        field_B8_vely = (field_BC_sprite_scale * FP_FromInteger(20));
    }

    const FP oldXPos = field_A8_xpos;
    const FP ypos1 = field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(20));
    
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        oldXPos,
        ypos1,
        field_A8_xpos,
        field_AC_ypos,
        &pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60) == 1)
    {
        switch (pLine->field_8_type)
        {
        case 1:
        case 5:
            if (field_B4_velx < FP_FromInteger(0))
            {
                field_B4_velx = (-field_B4_velx /  FP_FromInteger(2));
                field_A8_xpos = oldXPos;
            }
            break;

        case 2:
        case 6:
            if (field_B4_velx > FP_FromInteger(0))
            {
                field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                field_A8_xpos = oldXPos;
            }
            break;
        default:
            break;
        }
    }

    if (sCollisions_DArray_504C6C->RayCast_40C410(
        oldXPos,
        ypos1,
        field_A8_xpos,
        field_AC_ypos,
        &pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0x10))
    {
        switch (pLine->field_8_type)
        {
        case 0:
        case 4:
        case 32:
        case 36:
            if (field_B8_vely > FP_FromInteger(0))
            {
                field_F4_pLine = pLine;
                field_FE_next_state = -1;
                field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
                field_AC_ypos = hitY;
                field_B8_vely = FP_FromInteger(0);
            }
            break;
        default:
            break;
        }
    }

    if (field_AC_ypos - field_E8_LastLineYPos > FP_FromInteger(2))
    {
        field_12C = FP_FromInteger(0);
        field_E8_LastLineYPos = field_AC_ypos;
        field_FC_current_motion = eSlogStates::State_4_Fall_4750C0;
    }
}

void Slog::State_20_JumpUpwards_475890()
{
    if (gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos) >= CameraPos::eCamCurrent_0)
        MusicController::sub_443810(MusicController::MusicTypes::eType9, this, 0, 0);

    if (field_10_anim.field_92_current_frame == 5)
    {
        Sfx_475BD0(6);
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
        field_FE_next_state = -1;
    }
}

void Slog::State_21_Eating_475900()
{
    SND_Seq_Stop_477A60(SeqId::Unknown_17);

    if (field_10_anim.field_92_current_frame != 0
        || (field_10_anim.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards),
            field_FE_next_state == -1)
        || field_FE_next_state == eSlogStates::State_21_Eating_475900)
    {
        if (field_10_anim.field_92_current_frame == 3 && !field_10_anim.field_4_flags.Get(AnimFlags::eBit19_LoopBackwards))
        {
            SFX_Play_43AD70(static_cast<char>(Math_RandomRange_450F20(79, 80)), 100, 0);
            auto pBlood = ao_new<Blood>();
            if (pBlood)
            {
                const FP bloodYPos = field_AC_ypos - (FP_FromInteger(4) * field_BC_sprite_scale);
                const FP bloodXPos = ((field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -FP_FromInteger(25) : FP_FromInteger(25) * field_BC_sprite_scale);
                pBlood->ctor_4072B0(
                    bloodXPos + field_A8_xpos,
                    bloodYPos,
                    FP_FromInteger(0),
                    FP_FromInteger(0),
                    field_BC_sprite_scale,
                    12);
            }
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (Math_RandomRange_450F20(0, 100) < 85)
            {
                if (static_cast<int>(gnFrameCount_507670) > field_164_timer && Math_RandomRange_450F20(0, 100) < 60)
                {
                    field_164_timer = gnFrameCount_507670 + 16;
                    Sfx_475BD0(3);
                }
                field_10_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
            }
        }

        if (field_10_anim.field_92_current_frame == 0)
        {
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
        }
    }
    else
    {
        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
    }
}

void Slog::State_22_Empty_475A90()
{
    if (!field_F4_pLine)
    {
        State_4_Fall_4750C0();
        field_FC_current_motion = eSlogStates::State_22_Empty_475A90;
    }
}

void Slog::State_23_Scratch_475550()
{
    if (field_10_anim.field_92_current_frame == 4)
    {
        SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_16, 1, 1);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

void Slog::State_24_Growl_475590()
{
    if (field_10_anim.field_92_current_frame == 3)
    {
        if (field_144)
        {
            Sfx_475BD0(3);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
            field_144 = 0;
            field_164_timer = gnFrameCount_507670 + 12;
        }
    }

    if (static_cast<int>(gnFrameCount_507670) > field_164_timer)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
    }

    if (field_FE_next_state != -1)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_FC_current_motion = field_FE_next_state;
            field_FE_next_state = -1;
        }
    }
}

const __int16 sSlogResponseMotion_4CFCF0[3][10] =
{
    { 
        eSlogStates::State_3_TurnAround_474C70,
        eSlogStates::State_2_Run_4749A0,
        eSlogStates::State_8_StopRunning_474EC0,
        -1,
        eSlogStates::State_8_StopRunning_474EC0,
        -1,
        eSlogStates::State_7_SlideTurn_474DB0,
        -2,
        0,
        0
    },
    {
        eSlogStates::State_13_StartFastBarking_475B70,
        eSlogStates::State_2_Run_4749A0,
        eSlogStates::State_7_SlideTurn_474DB0,
        -2,
        0,
        0,
        0,
        0,
        0,
        0
    },
    {
        eSlogStates::State_13_StartFastBarking_475B70,
        -2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    }
};

__int16 Slog::Brain_0_ListeningToSlig_472450()
{
    if (!field_14C_pSlig)
    {
        return 99;
    }

    if (field_14C_pSlig->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_14C_pSlig->field_C_refCount--;
        field_14C_pSlig = nullptr;

        if (field_10C_pTarget)
        {
            field_10C_pTarget->field_C_refCount--;
            field_10C_pTarget = nullptr;
        }

        field_156 = 0;
        field_114_brain_idx = 1;
        return 0;
    }

    FP gridScale = ScaleToGridSize_41FA30(field_BC_sprite_scale);
    FP scaled1Directed = (gridScale * FP_FromInteger(1));
    if (field_14C_pSlig->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        scaled1Directed = -scaled1Directed;
    }

    WORD result = 0;
    const FP xSkip = CamX_VoidSkipper_418590(scaled1Directed + field_14C_pSlig->field_A8_xpos, scaled1Directed, 0, &result);

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;
    switch (field_116_brain_state)
    {
    case 0:
        field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
        field_150_waiting_counter = 0;
        field_11C_timer = gnFrameCount_507670 + 15;
        field_134 = 0;
        return 1;

    case 1:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            return field_116_brain_state;
        }

        if (!Facing(xSkip))
        {
            field_FE_next_state = eSlogStates::State_3_TurnAround_474C70;
            return field_116_brain_state;
        }

        if (static_cast<int>(gnFrameCount_507670) <= field_11C_timer)
        {
            return field_116_brain_state;
        }

        field_FE_next_state = eSlogStates::State_6_MoveHeadUpwards_474220;
        return 2;

    case 2:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            return field_116_brain_state;
        }

        if (field_148 == pEventSystem_4FF954->field_18_last_event_index)
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
            field_148 = pEventSystem_4FF954->field_18_last_event_index;
            speak = pEventSystem_4FF954->field_10_last_event;
        }
        // Down to game speak handler
        break;

    case 3:
        if (field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            return 2;
        }

        if (field_FC_current_motion != eSlogStates::State_1_Walk_4743F0)
        {
            return field_116_brain_state;
        }

        if (!Facing(xSkip))
        {
            field_FE_next_state = eSlogStates::State_3_TurnAround_474C70;
            return 2;
        }

        if (FP_Abs(xSkip - field_A8_xpos) > (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)))
        {
            field_FE_next_state = eSlogStates::State_2_Run_4749A0;
            return 4;
        }

        if (FP_Abs(xSkip - field_A8_xpos) > (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(1)))
        {
            return field_116_brain_state;
        }

        field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
        return 2;

    case 4:
        if (field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            return 2;
        }

        if (field_FC_current_motion != eSlogStates::State_2_Run_4749A0)
        {
            return field_116_brain_state;
        }

        if (!Facing(xSkip))
        {
            field_FE_next_state = eSlogStates::State_8_StopRunning_474EC0;
            return field_116_brain_state;
        }

        if (FP_Abs(xSkip - field_A8_xpos) <= (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(3)))
        {
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
            return 2;
        }

        return field_116_brain_state;

    case 5:
        if (static_cast<int>(gnFrameCount_507670) <= field_11C_timer)
        {
            return field_116_brain_state;
        }
        field_150_waiting_counter--;
        field_FE_next_state = eSlogStates::State_6_MoveHeadUpwards_474220;
        return 2;

    case 6:
        if (static_cast<int>(gnFrameCount_507670) <= field_11C_timer)
        {
            return field_116_brain_state;
        }

        if (field_180)
        {
            return 2;
        }

        field_11C_timer = gnFrameCount_507670 + 10;
        field_FE_next_state = sSlogResponseMotion_4CFCF0[field_152_response_index][field_154_response_part++];

        if (field_FE_next_state == -2)
        {
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
            return 2;
        }
        return field_116_brain_state;

    default:
        return field_116_brain_state;
    }

    switch (speak)
    {
    case GameSpeakEvents::Slig_LookOut_6:
        DelayedResponse(2);
        return 6;

    case GameSpeakEvents::Slig_Hi_23:
        field_150_waiting_counter++;
        if (static_cast<int>(gnFrameCount_507670) % 2)
        {
            field_150_waiting_counter++;
        }
        // Fall through
    case GameSpeakEvents::Slig_HereBoy_24:
        field_150_waiting_counter++;
        field_11C_timer = gnFrameCount_507670 - (Math_NextRandom() % 8) + 15;
        break;

    case GameSpeakEvents::eUnknown_25:
    {
        field_10C_pTarget = FindAbeMudOrSlig_4722F0();
        if (field_10C_pTarget)
        {
            field_10C_pTarget->field_C_refCount++;

            field_14C_pSlig->field_C_refCount--;
            field_14C_pSlig = nullptr;

            field_114_brain_idx = 2;
            return 0;
        }

        FP gridSize = {};
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }
        else
        {
            gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), gridSize))
        {
            DelayedResponse(1);
            return 6;
        }
        break;
    }

    case GameSpeakEvents::eUnknown_29:
        DelayedResponse(0);
        return 6;

    default:
        break;
    }


    if (!(Math_NextRandom() % 128))
    {
        field_150_waiting_counter++;
    }

    if (field_150_waiting_counter)
    {
        field_11C_timer += Math_NextRandom() % 16;
        return 5;
    }

    if (FP_Abs(xSkip - field_A8_xpos) > (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(4)))
    {
        if (!Facing(xSkip))
        {
            field_FE_next_state = eSlogStates::State_3_TurnAround_474C70;
            return field_116_brain_state;
        }

        FP gridSize2 = {};
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize2 = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }
        else
        {
            gridSize2 = ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), gridSize2))
        {
            field_FE_next_state = eSlogStates::State_2_Run_4749A0;
            return 4;
        }
    }

    if (FP_Abs(xSkip - field_A8_xpos) > (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(1)))
    {
        if (!Facing(xSkip))
        {
            field_FE_next_state = eSlogStates::State_3_TurnAround_474C70;
            return field_116_brain_state;
        }

        FP gridSize3 = {};
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize3 = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }
        else
        {
            gridSize3 = ScaleToGridSize_41FA30(field_BC_sprite_scale);
        }

        if (!WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), gridSize3))
        {
            field_FE_next_state = eSlogStates::State_1_Walk_4743F0;
            return 3;
        }
    }

    if (field_14C_pSlig->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return eSlogStates::State_3_TurnAround_474C70;
    }

     return field_116_brain_state;
}

__int16 Slog::Brain_1_Idle_4719C0()
{
    if (field_10C_pTarget)
    {
        if (field_10C_pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_10C_pTarget->field_C_refCount--;
            field_10C_pTarget = nullptr;
        }
    }

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    if (field_148 == pEventSystem_4FF954->field_18_last_event_index)
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
        speak = pEventSystem_4FF954->field_10_last_event;
        field_148 = pEventSystem_4FF954->field_18_last_event_index;
    }

    if (speak == GameSpeakEvents::Slig_HereBoy_24)
    {
        // Listen to slig
        field_114_brain_idx = 0;
        field_14C_pSlig = sControlledCharacter_50767C;
        field_14C_pSlig->field_C_refCount++;

        if (field_10C_pTarget)
        {
            field_10C_pTarget = nullptr;
            field_10C_pTarget->field_C_refCount--;
        }
        return 0;
    }

    if (SwitchStates_Get(field_168_anger_switch_id))
    {
        // Slog triggered, not gonna listen to you anymore, you gotta die
        field_114_brain_idx = 2;
        return 0;
    }

    switch (field_116_brain_state)
    {
    case 0:
        if (field_FC_current_motion != eSlogStates::State_16_Sleeping_4752E0 && field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
            return field_116_brain_state;
        }
        else if (field_17E)
        {
            field_156 = 0;
            return 1;
        }
        field_156 = field_158_bark_anger;
        return 4;

    case 1:
        if (Event_Is_Event_In_Range_417270(
            kEventSuspiciousNoise_10,
            field_A8_xpos,
            field_AC_ypos,
            field_BC_sprite_scale))
        {
            field_156++;
        }

        if (Event_Is_Event_In_Range_417270(
            kEventSpeaking_1,
            field_A8_xpos,
            field_AC_ypos,
            field_BC_sprite_scale))
        {
            field_156 += (Slog_NextRandom() % 8) + 15;
        }

        if (!(gnFrameCount_507670 % 16))
        {
            if (field_156)
            {
                field_156--;
            }
        }

        if (field_156 <= field_158_bark_anger)
        {
            return field_116_brain_state;
        }
        field_FE_next_state = eSlogStates::State_18_WakeUp_475460;
        return 2;

    case 2:
        if (field_FC_current_motion != eSlogStates::State_18_WakeUp_475460)
        {
            return field_116_brain_state;
        }
        field_FE_next_state = eSlogStates::State_6_MoveHeadUpwards_474220;
        field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
        field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
        return 4;

    case 3:
        if (field_FC_current_motion != eSlogStates::State_17_FallAsleep_475510)
        {
            return field_116_brain_state;
        }
        field_FE_next_state = eSlogStates::State_16_Sleeping_4752E0;
        return 1;

    case 4:
        if (Event_Is_Event_In_Range_417270(
            kEventSuspiciousNoise_10,
            field_A8_xpos,
            field_AC_ypos,
            field_BC_sprite_scale))
        {
            field_156++;
        }

        if (Event_Is_Event_In_Range_417270(
            kEventSpeaking_1,
            field_A8_xpos,
            field_AC_ypos,
            field_BC_sprite_scale))
        {
            field_156 += (Slog_NextRandom() % 8) + 15;
        }

        if (!(gnFrameCount_507670 % 32))
        {
            if (field_156)
            {
                if (field_17E)
                {
                    field_156--;
                }
            }
        }

        if (IsPlayerNear_471930())
        {
            field_156 += 2;
        }

        if (!(Slog_NextRandom() % 64) && field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            field_FC_current_motion = eSlogStates::State_6_MoveHeadUpwards_474220;
            return field_116_brain_state;
        }

        if (static_cast<int>(gnFrameCount_507670) > field_164_timer && field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            field_FC_current_motion = eSlogStates::State_24_Growl_475590;
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;

            field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
        }

        if (static_cast<int>(gnFrameCount_507670) > field_160 && field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            field_FC_current_motion = eSlogStates::State_23_Scratch_475550;
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;

            field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
        }

        if (field_156 > field_15A)
        {
            field_FE_next_state = eSlogStates::State_15_Empty_475290;
            field_156 += Slog_NextRandom() % 8;
            return 5;
        }

        if (field_156 >= field_158_bark_anger)
        {
            return field_116_brain_state;
        }
        field_FE_next_state = eSlogStates::State_17_FallAsleep_475510;
        return 3;

    case 5:
        if (Event_Is_Event_In_Range_417270(
            kEventSuspiciousNoise_10,
            field_A8_xpos,
            field_AC_ypos,
            field_BC_sprite_scale))
        {
            field_156++;
        }

        if (Event_Is_Event_In_Range_417270(
            kEventSpeaking_1,
            field_A8_xpos,
            field_AC_ypos,
            field_BC_sprite_scale))
        {
            field_156 += (Slog_NextRandom() % 8) + 15;
        }

        if (!(gnFrameCount_507670 % 2))
        {
            if (field_156)
            {
                field_156--;
            }
        }

        if (IsPlayerNear_471930())
        {
            field_156 += 2;
        }

        if (field_156 >= field_15A)
        {
            if (field_156 > field_15C)
            {
                field_114_brain_idx = 2;
                return 0;
            }
            return field_116_brain_state;
        }

        field_FC_current_motion = eSlogStates::State_0_Idle_4742E0;
        field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
        field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
        return 4;

    default:
        return field_116_brain_state;
    }
}

__int16 Slog::Brain_2_ChasingAbe_470F50()
{
    const int lastIdx = pEventSystem_4FF954->field_18_last_event_index;

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    if (field_148 == lastIdx)
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
        field_148 = lastIdx;
        speak = pEventSystem_4FF954->field_10_last_event;
    }

    if (field_176 && speak == GameSpeakEvents::Slig_HereBoy_24)
    {
        // Listen to slig
        field_114_brain_idx = 0;
        field_14C_pSlig = sControlledCharacter_50767C;
        field_14C_pSlig->field_C_refCount++;
      
        if (field_10C_pTarget)
        {
            field_10C_pTarget->field_C_refCount--;
            field_10C_pTarget = nullptr;
        }
        return 0;
    }

    if (field_10C_pTarget && field_10C_pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        // Idle
        field_10C_pTarget->field_C_refCount--;
        field_10C_pTarget = nullptr;
        field_156 = 0;
        field_114_brain_idx = 1;
        field_FE_next_state = 0;
        return 0;
    }

    switch (field_116_brain_state)
    {
    case 0:
        if (!field_10C_pTarget)
        {
            field_10C_pTarget = sActiveHero_507678;
            field_10C_pTarget->field_C_refCount++;
        }
        field_110 = 0;
        field_134 = 0;
        field_172 = 0;
        field_16C = 0;
        field_FE_next_state = eSlogStates::State_2_Run_4749A0;
        Sfx_475BD0(8);
        return 1;

    case 1:
    {
        if ((field_B4_velx != FP_FromInteger(0) && HandleEnemyStopper_473BD0()) ||
            WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), field_B4_velx))
        {
            field_FE_next_state = eSlogStates::State_7_SlideTurn_474DB0;
            field_174 = field_B4_velx < FP_FromInteger(0);
            field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
            field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
            return 13;
        }

        if (!VIsFacingMe(field_10C_pTarget) && field_FC_current_motion == eSlogStates::State_2_Run_4749A0)
        {
            field_FE_next_state = eSlogStates::State_8_StopRunning_474EC0;
        }

        if (VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(3), field_10C_pTarget))
        {
            if (VOnSameYLevel(field_10C_pTarget))
            {
                if (field_10C_pTarget->field_BC_sprite_scale == field_BC_sprite_scale)
                {
                    if (VIsFacingMe(field_10C_pTarget))
                    {
                        field_FE_next_state = eSlogStates::State_19_JumpForwards_475610;
                    }
                }
            }
        }

        if (field_FC_current_motion == eSlogStates::State_8_StopRunning_474EC0)
        {
            field_FE_next_state = eSlogStates::State_2_Run_4749A0;
        }

        if (field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            if (VIsFacingMe(field_10C_pTarget))
            {
                FP scaleDirected = {};
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    scaleDirected = -ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }
                else
                {
                    scaleDirected = ScaleToGridSize_41FA30(field_BC_sprite_scale);
                }

                if (WallHit_401930(field_BC_sprite_scale * FP_FromInteger(20), FP_FromInteger(2) * scaleDirected))
                {
                    field_174 = field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
                    field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
                    field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
                    return 13;
                }
                field_FE_next_state = eSlogStates::State_2_Run_4749A0;
            }
            else
            {
                field_FC_current_motion = eSlogStates::State_3_TurnAround_474C70;
            }
        }

        if (field_FC_current_motion == eSlogStates::State_4_Fall_4750C0)
        {
            return 8;
        }

        if (field_FC_current_motion == eSlogStates::State_19_JumpForwards_475610)
        {
            return 3;
        }

        const FP k10Scaled = field_BC_sprite_scale * FP_FromInteger(10);
        if (field_AC_ypos <= field_10C_pTarget->field_AC_ypos + k10Scaled)
        {
            if (field_10C_pTarget->field_BC_sprite_scale == field_BC_sprite_scale)
            {
                if (field_AC_ypos >= (field_10C_pTarget->field_AC_ypos - (k10Scaled * FP_FromInteger(3))))
                {
                    return field_116_brain_state;
                }
            }
        }
        else
        {
            if (field_10C_pTarget->field_BC_sprite_scale == field_BC_sprite_scale)
            {
                return 9;
            }
        }

        field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
        field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
        return 2;
    }

    case 2:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
            return field_116_brain_state;
        }

        if ((Slog_NextRandom() % 64) == 0)
        {
            field_FC_current_motion = eSlogStates::State_6_MoveHeadUpwards_474220;
            return field_116_brain_state;
        }

        if (static_cast<int>(gnFrameCount_507670) > field_164_timer)
        {
            field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;

            field_FC_current_motion = eSlogStates::State_24_Growl_475590;
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
        }

        if (static_cast<int>(gnFrameCount_507670) > field_160)
        {
            field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;

            field_FC_current_motion = eSlogStates::State_23_Scratch_475550;
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;

        }

        if (field_10C_pTarget->field_BC_sprite_scale != field_BC_sprite_scale)
        {
            return field_116_brain_state;
        }
        return 1;

    case 3:
        if (field_FC_current_motion == eSlogStates::State_2_Run_4749A0)
        {
            if (!field_110)
            {
                return 1;
            }

            if (VIsObjNearby(field_BC_sprite_scale * FP_FromInteger(20), field_10C_pTarget) || VIsFacingMe(field_10C_pTarget))
            {
                field_FC_current_motion = eSlogStates::State_21_Eating_475900;
                field_FE_next_state = -1;
            }
            else
            {
                field_FC_current_motion = eSlogStates::State_3_TurnAround_474C70;
                field_FE_next_state = eSlogStates::State_21_Eating_475900;
            }
            field_11C_timer = gnFrameCount_507670 + 90;
            return 6;
        }

        if (field_FC_current_motion != eSlogStates::State_4_Fall_4750C0)
        {
            return field_116_brain_state;
        }

        return 8;

    case 6:
        if (static_cast<int>(gnFrameCount_507670) <= field_11C_timer)
        {
            if (field_FC_current_motion)
            {
                return field_116_brain_state;
            }
            field_FC_current_motion = eSlogStates::State_21_Eating_475900;
            return field_116_brain_state;
        }

        field_FE_next_state = eSlogStates::State_0_Idle_4742E0;
        return 7;

    case 7:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            return field_116_brain_state;
        }
        field_156 = 0;
        field_114_brain_idx = 1;
        return 0;

    case 8:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            return field_116_brain_state;
        }
        field_FC_current_motion = eSlogStates::State_2_Run_4749A0;
        return 1;

    case 9:
        if (field_B4_velx > FP_FromInteger(0) && HandleEnemyStopper_473BD0())
        {
            field_FE_next_state = eSlogStates::State_7_SlideTurn_474DB0;
            field_174 = field_B4_velx < FP_FromInteger(0);
            field_160 =  (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
            field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
            return 13;
        }

        if (!VIsFacingMe(field_10C_pTarget) && field_FC_current_motion == eSlogStates::State_2_Run_4749A0)
        {
            field_FE_next_state = eSlogStates::State_8_StopRunning_474EC0;
        }

        if (VIsObjNearby(ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(3), field_10C_pTarget))
        {
            if (field_10C_pTarget->field_BC_sprite_scale == field_BC_sprite_scale && field_FC_current_motion == eSlogStates::State_2_Run_4749A0)
            {
                if (VIsFacingMe(field_10C_pTarget))
                {
                    field_FE_next_state = eSlogStates::State_7_SlideTurn_474DB0;
                }
            }
        }

        if (field_FC_current_motion == eSlogStates::State_8_StopRunning_474EC0)
        {
            field_FE_next_state = eSlogStates::State_2_Run_4749A0;
        }

        if (field_AC_ypos < ((field_BC_sprite_scale * FP_FromInteger(10)) + field_10C_pTarget->field_AC_ypos))
        {
            field_11C_timer = gnFrameCount_507670 + field_170;
            return 11;
        }

        if (field_FC_current_motion == eSlogStates::State_4_Fall_4750C0)
        {
            return 8;
        }

        if (field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            if (VIsFacingMe(field_10C_pTarget))
            {
                if (field_172 >= 100)
                {
                    if (Math_RandomRange_450F20(0, 100) < 20)
                    {
                        field_FC_current_motion = eSlogStates::State_24_Growl_475590;
                    }
                    field_FE_next_state = eSlogStates::State_6_MoveHeadUpwards_474220;
                    return 12;
                }
                else
                {
                    field_FE_next_state = eSlogStates::State_20_JumpUpwards_475890;
                    return 10;
                }
            }
            else
            {
                field_FE_next_state = eSlogStates::State_3_TurnAround_474C70;
            }
            return field_116_brain_state;
        }
        return field_116_brain_state;

    case 10:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            return field_116_brain_state;
        }

        field_172 += Slog_NextRandom() % 64;
        return 9;

    case 11:
        if (field_FC_current_motion == eSlogStates::State_0_Idle_4742E0)
        {
            if (!VIsFacingMe(field_10C_pTarget))
            {
                field_FE_next_state = eSlogStates::State_3_TurnAround_474C70;
                return field_116_brain_state;
            }

            field_FE_next_state = eSlogStates::State_6_MoveHeadUpwards_474220;
        }

        if (static_cast<int>(gnFrameCount_507670) <= field_11C_timer)
        {
            return field_116_brain_state;
        }
        return 1;

    case 12:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            return field_116_brain_state;
        }
        field_172 -= 20;
        return 9;

    case 13:
        if (field_FC_current_motion != eSlogStates::State_0_Idle_4742E0)
        {
            return field_116_brain_state;
        }

        if (field_174)
        {
            if (field_10C_pTarget->field_A8_xpos > field_A8_xpos)
            {
                return 1;
            }
        }
        else
        {
            if (field_10C_pTarget->field_A8_xpos < field_A8_xpos)
            {
                return 1;
            }
        }

        if (static_cast<int>(gnFrameCount_507670) > field_164_timer)
        {
            field_FC_current_motion = eSlogStates::State_24_Growl_475590;
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;

            field_164_timer = (Math_NextRandom() % 32) + gnFrameCount_507670 + 60;
        }

        if (static_cast<int>(gnFrameCount_507670) > field_160)
        {
            field_FC_current_motion = eSlogStates::State_23_Scratch_475550;
            field_FE_next_state = eSlogStates::State_0_Idle_4742E0;

            field_160 = (Math_NextRandom() % 32) + gnFrameCount_507670 + 120;
        }

        return field_116_brain_state;

    default:
        break;
    }

    return field_116_brain_state;
}

__int16 Slog::Brain_3_Dead_4721B0()
{
    if (field_14C_pSlig)
    {
        field_14C_pSlig->field_C_refCount--;
        field_14C_pSlig = nullptr;
    }

    if (field_10C_pTarget)
    {
        field_10C_pTarget->field_C_refCount--;
        field_10C_pTarget = nullptr;
    }

    if (field_11C_timer < static_cast<int>(gnFrameCount_507670) + 80)
    {
        field_C0_r -= 2;
        field_C2_g -= 2;
        field_C4_b -= 2;
        field_BC_sprite_scale -= FP_FromDouble(0.023);
    }

    if (static_cast<int>(gnFrameCount_507670) < field_11C_timer - 24 && !(gnFrameCount_507670 % 5))
    {
        New_Smoke_Particles_419A80(
            (FP_FromInteger(Math_RandomRange_450F20(-24, 24)) * field_BC_sprite_scale) + field_A8_xpos,
            field_AC_ypos - FP_FromInteger(6),
            field_BC_sprite_scale / FP_FromInteger(2),
            2, 0);
        SFX_Play_43AE60(SoundEffect::Vaporize_96, 25, FP_GetExponent(FP_FromInteger(2200) * field_BC_sprite_scale), 0);
    }

    if (field_BC_sprite_scale < FP_FromInteger(0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return 100;
}

__int16 Slog::Facing(FP xpos)
{
    if (field_A8_xpos < xpos && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    if (xpos < field_A8_xpos && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    return FALSE;
}

void Slog::DelayedResponse(__int16 responseIdx)
{
    field_152_response_index = responseIdx;
    field_154_response_part = 0;
    field_180 = 0;
    field_11C_timer = gnFrameCount_507670 + 10;
}

}
