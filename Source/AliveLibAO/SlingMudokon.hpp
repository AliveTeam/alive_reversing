#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

void SlingMud_ForceLink();

START_NS_AO

struct Path_SlingMud : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_silent;
    __int16 field_1C_code_1;
    __int16 field_1E_code_2;
};
ALIVE_ASSERT_SIZEOF(Path_SlingMud, 0x20);

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
    EXPORT SlingMudokon* ctor_46F940(Path_SlingMud* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_46FB30();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_470F30(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_46FBE0();

    EXPORT void VUpdate_46FBF0();

    EXPORT virtual void VCallBrain_46F880();

    EXPORT virtual void VCallMotion_46F8E0();

    EXPORT void VUpdateAnimData_46F8F0();

    // States
    EXPORT void State_0_Idle_46FCB0();

    EXPORT void State_1_Angry_46FCF0();

    EXPORT void State_2_Speak_46FD70();

    EXPORT void State_3_ShootStart_46FD90();

    EXPORT void State_4_ShootEnd_46FEA0();

    EXPORT void State_5_AngryToIdle_46FD50();

    // Brains
    EXPORT __int16 tsub_46FEC0();

    EXPORT __int16 tsub_470230();

    EXPORT __int16 tsub_4707B0();

    int field_10C;
    int field_110_tlvInfo;
    int field_114;
    int field_118_code_converted;
    __int16 field_11C_code_length;
    __int16 field_11E_flags;
    int field_120;
    int field_124_code_buffer;
    int field_128;
    int field_12C;
    int field_130;
    __int16 field_134_buffer_start;
    __int16 field_136;
    unsigned __int16 field_138_brain_state;
    __int16 field_13A_brain_state;
    __int16 field_13C;
    __int16 field_13E;
    int field_140_timer;
    int field_144_timer2;
    int field_148;
    int field_14C;
    BYTE** field_150_res;
    __int16 field_154;
    __int16 field_156;
    __int16 field_158_code_pos;
    __int16 field_15A;
};
ALIVE_ASSERT_SIZEOF(SlingMudokon, 0x15C);

END_NS_AO

