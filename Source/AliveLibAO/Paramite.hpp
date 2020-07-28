#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

void Paramite_ForceLink();

START_NS_AO

#define PARAMITE_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_44B900) \
    ENTRY(State_1_WalkBegin_44BCA0) \
    ENTRY(State_2_Walking_44B9E0) \
    ENTRY(State_3_Running_44C070) \
    ENTRY(State_4_Unknown_44B6C0) \
    ENTRY(State_5_Turn_44C8E0) \
    ENTRY(State_6_Hop_44CB20) \
    ENTRY(State_7_Unknown_44BF10) \
    ENTRY(State_8_WalkRunTransition_44C790) \
    ENTRY(State_9_WalkEnd_44BDE0) \
    ENTRY(State_10_RunBegin_44C4C0) \
    ENTRY(State_11_RunEnd_44C620) \
    ENTRY(State_12_Falling_44C960) \
    ENTRY(State_13_GameSpeakBegin_44D050) \
    ENTRY(State_14_PreHiss_44D170) \
    ENTRY(State_15_Hiss_44D300) \
    ENTRY(State_16_PostHiss_44D440) \
    ENTRY(State_17_GameSpeakEnd_44D4F0) \
    ENTRY(State_18_RunningAttack_44D5D0) \
    ENTRY(State_19_Empty_44D990) \
    ENTRY(State_20_SurpriseWeb_44D9A0) \
    ENTRY(State_21_WebLeave_44DB00) \
    ENTRY(State_22_Unknown_44D8F0) \
    ENTRY(State_23_Eating_44B970) \
    ENTRY(State_24_Struggle_44DB70) \
    ENTRY(State_25_Death_44DB90)

#define MAKE_ENUM(VAR) VAR,
enum eParamiteStates : int
{
    PARAMITE_STATES_ENUM(MAKE_ENUM)
};

class ParamiteWeb;

struct Path_Paramite : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_entrance_type;
    __int16 field_1C_attack_delay;
    __int16 field_1E_meat_eating_time;
    __int16 field_20_attack_duration;
    __int16 field_22_disabled_resources;
    __int16 field_24_id;
    __int16 field_26_hiss_before_attack;
    __int16 field_28_unload_when_far_away;
    __int16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Paramite, 0x2C);

class Paramite : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_44A490();

    EXPORT void VUpdate_44A490();

    EXPORT void ToIdle_44B580();

    EXPORT __int16 sub_44B320();

    // States
    EXPORT void State_0_Idle_44B900();
    EXPORT void State_1_WalkBegin_44BCA0();
    EXPORT void State_2_Walking_44B9E0();
    EXPORT void State_3_Running_44C070();
    EXPORT void State_4_Unknown_44B6C0();
    EXPORT void State_5_Turn_44C8E0();
    EXPORT void State_6_Hop_44CB20();
    EXPORT void State_7_Unknown_44BF10();
    EXPORT void State_8_WalkRunTransition_44C790();
    EXPORT void State_9_WalkEnd_44BDE0();
    EXPORT void State_10_RunBegin_44C4C0();
    EXPORT void State_11_RunEnd_44C620();
    EXPORT void State_12_Falling_44C960();
    EXPORT void State_13_GameSpeakBegin_44D050();
    EXPORT void State_14_PreHiss_44D170();
    EXPORT void State_15_Hiss_44D300();
    EXPORT void State_16_PostHiss_44D440();
    EXPORT void State_17_GameSpeakEnd_44D4F0();
    EXPORT void State_18_RunningAttack_44D5D0();
    EXPORT void State_19_Empty_44D990();
    EXPORT void State_20_SurpriseWeb_44D9A0();
    EXPORT void State_21_WebLeave_44DB00();
    EXPORT void State_22_Unknown_44D8F0();
    EXPORT void State_23_Eating_44B970();
    EXPORT void State_24_Struggle_44DB70();
    EXPORT void State_25_Death_44DB90();

    int field_10C_fn;
    __int16 field_110_state;
    __int16 field_112;
    int field_114;
    int field_118;
    __int16 field_11C;
    __int16 field_11E;
    int field_120;
    int field_124;
    __int16 field_128;
    __int16 field_12A;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    int field_144;
    int field_148;
    ParamiteWeb* field_14C_pWeb;
    int field_150_resources[16];
};
ALIVE_ASSERT_SIZEOF(Paramite, 0x190);

END_NS_AO

