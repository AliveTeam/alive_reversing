#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

void SlingMud_ForceLink();

START_NS_AO

#define SLING_MUD_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_46FCB0) \
    ENTRY(State_1_Angry_46FCF0) \
    ENTRY(State_2_Speak_46FD70) \
    ENTRY(State_3_ShootStart_46FD90) \
    ENTRY(State_4_ShootEnd_46FEA0) \
    ENTRY(State_5_AngryToIdle_46FD50)

#define MAKE_ENUM(VAR) VAR,
enum eSlingMudStates : int
{
    SLING_MUD_STATES_ENUM(MAKE_ENUM)
};

class SlingMudokon : public BaseAliveGameObject
{
public:
    EXPORT void VUpdate_Real_46FBF0();

    EXPORT void VUpdate_46FBF0();

    // States
    EXPORT void State_0_Idle_46FCB0();

    EXPORT void State_1_Angry_46FCF0();

    EXPORT void State_2_Speak_46FD70();

    EXPORT void State_3_ShootStart_46FD90();

    EXPORT void State_4_ShootEnd_46FEA0();

    EXPORT void State_5_AngryToIdle_46FD50();

    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    __int16 field_11C;
    __int16 field_11E;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    __int16 field_134;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    BYTE** field_150;
    __int16 field_154;
    __int16 field_156;
    __int16 field_158;
    __int16 field_15A;
};
ALIVE_ASSERT_SIZEOF(SlingMudokon, 0x15C);

END_NS_AO

