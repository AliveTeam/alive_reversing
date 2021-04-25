#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "GameSpeak.hpp"

void SlingMud_ForceLink();

namespace AO {

struct Path_SlingMudokon : public Path_TLV
{
    s16 field_18_scale;
    s16 field_1A_silent;
    s16 field_1C_code_1;
    s16 field_1E_code_2;
};
ALIVE_ASSERT_SIZEOF(Path_SlingMudokon, 0x20);

#define SLING_MUD_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Idle_46FCB0) \
    ENTRY(State_1_Angry_46FCF0) \
    ENTRY(State_2_Speak_46FD70) \
    ENTRY(State_3_ShootStart_46FD90) \
    ENTRY(State_4_ShootEnd_46FEA0) \
    ENTRY(State_5_AngryToIdle_46FD50)

#define MAKE_ENUM(VAR) VAR,
enum eSlingMudStates : s32
{
    SLING_MUD_STATES_ENUM(MAKE_ENUM)
};

class SlingMudokon : public BaseAliveGameObject
{
public:
    EXPORT SlingMudokon* ctor_46F940(Path_SlingMudokon* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_46FB30();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_470F30(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_46FBE0();

    EXPORT void VUpdate_46FBF0();

    virtual void VUpdate() override;

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
    EXPORT s16 tsub_46FEC0();

    EXPORT s16 tsub_470230();

    EXPORT s16 tsub_4707B0();

    s32 field_10C_padding;
    s32 field_110_tlvInfo;
    s32 field_114_paddding;
    s32 field_118_code_converted;
    s16 field_11C_code_length;
    s16 field_11E_flags;
    s32 field_120_last_event_idx;
    u8 field_124_code_buffer[16];
    s16 field_134_buffer_start;
    s16 field_136;
    u16 field_138_brain_state;
    s16 field_13A_brain_state;
    s16 field_13C;
    s16 field_13E;
    s32 field_140_timer;
    s32 field_144_timer2;
    s32 field_148;
    s32 field_14C;
    u8** field_150_res;
    s16 field_154;
    s16 field_156;
    s16 field_158_code_pos;
    s16 field_15A;
};
ALIVE_ASSERT_SIZEOF(SlingMudokon, 0x15C);

}

