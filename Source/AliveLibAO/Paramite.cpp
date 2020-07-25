#include "stdafx_ao.h"
#include "Function.hpp"
#include "Paramite.hpp"
#include "Math.hpp"
#include "Sfx.hpp"

void Paramite_ForceLink() {}

START_NS_AO

using TParamiteStateFunction = decltype(&Paramite::State_0_Idle_44B900);

const TParamiteStateFunction sParamiteMotionTable_4CDCB0[] =
{
    &Paramite::State_0_Idle_44B900,
    &Paramite::State_1_WalkBegin_44BCA0,
    &Paramite::State_2_Walking_44B9E0,
    &Paramite::State_3_Running_44C070,
    &Paramite::State_4_Unknown_44B6C0,
    &Paramite::State_5_Turn_44C8E0,
    &Paramite::State_6_Hop_44CB20,
    &Paramite::State_7_Unknown_44BF10,
    &Paramite::State_8_WalkRunTransition_44C790,
    &Paramite::State_9_WalkEnd_44BDE0,
    &Paramite::State_10_RunBegin_44C4C0,
    &Paramite::State_11_RunEnd_44C620,
    &Paramite::State_12_Falling_44C960,
    &Paramite::State_13_GameSpeakBegin_44D050,
    &Paramite::State_14_PreHiss_44D170,
    &Paramite::State_15_Hiss_44D300,
    &Paramite::State_16_PostHiss_44D440,
    &Paramite::State_17_GameSpeakEnd_44D4F0,
    &Paramite::State_18_RunningAttack_44D5D0,
    &Paramite::State_19_Empty_44D990,
    &Paramite::State_20_SurpriseWeb_44D9A0,
    &Paramite::State_21_WebLeave_44DB00,
    &Paramite::State_22_Unknown_44D8F0,
    &Paramite::State_23_Eating_44B970,
    &Paramite::State_24_Struggle_44DB70,
    &Paramite::State_25_Death_44DB90,
};

void Paramite::VUpdate_Real_44A490()
{
    NOT_IMPLEMENTED();
}

void Paramite::VUpdate_44A490()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_44A490();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}


void Paramite::ToIdle_44B580()
{
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit7_SwapXY);

    field_124 = 0;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    
    field_FC_current_motion = eParamiteStates::State_0_Idle_44B900;

    MapFollowMe_401D30(1);
}

__int16 Paramite::sub_44B320()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Paramite::State_0_Idle_44B900()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_1_WalkBegin_44BCA0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_2_Walking_44B9E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_3_Running_44C070()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_4_Unknown_44B6C0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_5_Turn_44C8E0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_6_Hop_44CB20()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_7_Unknown_44BF10()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_8_WalkRunTransition_44C790()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_9_WalkEnd_44BDE0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_10_RunBegin_44C4C0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_11_RunEnd_44C620()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_12_Falling_44C960()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_13_GameSpeakBegin_44D050()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_14_PreHiss_44D170()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_15_Hiss_44D300()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_16_PostHiss_44D440()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_17_GameSpeakEnd_44D4F0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_18_RunningAttack_44D5D0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_19_Empty_44D990()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_20_SurpriseWeb_44D9A0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_21_WebLeave_44DB00()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_22_Unknown_44D8F0()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_23_Eating_44B970()
{
    if (field_10_anim.field_92_current_frame == 5)
    {
        SFX_Play_43AD70(static_cast<unsigned char>(Math_RandomRange_450F20(79, 80)), 0, 0);
    }

    if (field_FE_next_state != -1 && !sub_44B320())
    {
        ToIdle_44B580();
    }
}

void Paramite::State_24_Struggle_44DB70()
{
    NOT_IMPLEMENTED();
}

void Paramite::State_25_Death_44DB90()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

