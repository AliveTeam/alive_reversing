#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "GameSpeak.hpp"
#include "../AliveLibAE/Path.hpp"

void SlingMud_ForceLink();

namespace AO {

struct Path_SlingMudokon final : public Path_TLV
{
    Scale_short field_18_scale;
    Choice_short field_1A_bDontWhistlePassword;
    s16 field_1C_code_1;
    s16 field_1E_code_2;
};
ALIVE_ASSERT_SIZEOF(Path_SlingMudokon, 0x20);

#define SLING_MUD_MOTIONS_ENUM(ENTRY) \
    ENTRY(Motion_0_Idle_46FCB0)       \
    ENTRY(Motion_1_Angry_46FCF0)      \
    ENTRY(Motion_2_Speak_46FD70)      \
    ENTRY(Motion_3_ShootStart_46FD90) \
    ENTRY(Motion_4_ShootEnd_46FEA0)   \
    ENTRY(Motion_5_AngryToIdle_46FD50)

#define MAKE_ENUM(VAR) VAR,
enum eSlingMudMotions : s32
{
    SLING_MUD_MOTIONS_ENUM(MAKE_ENUM)
};

enum SlingMudBrainStates : u16
{
    Brain_0_GiveCode_46FEC0 = 0,
    Brain_1_Spawn_470230 = 1,
    Brain_2_AskForPassword_4707B0 = 2
};

class SlingMudokon final : public BaseAliveGameObject
{
public:
    SlingMudokon(Path_SlingMudokon* pTlv, s32 tlvInfo);
    ~SlingMudokon();

    virtual void VScreenChanged() override;

    void VScreenChanged_46FBE0();

    void VUpdate_46FBF0();

    virtual void VUpdate() override;

    virtual void VCallBrain_46F880();

    virtual void VCallMotion_46F8E0();

    void VUpdateAnimData_46F8F0();

    GameSpeakEvents getLastIdx();

    // Motions
    void Motion_0_Idle_46FCB0();
    void Motion_1_Angry_46FCF0();
    void Motion_2_Speak_46FD70();
    void Motion_3_ShootStart_46FD90();
    void Motion_4_ShootEnd_46FEA0();
    void Motion_5_AngryToIdle_46FD50();

    // Brains
    s16 Brain_0_GiveCode_46FEC0();
    s16 Brain_1_Spawn_470230();
    s16 Brain_2_AskForPassword_4707B0();

    s32 field_10C_padding;
    s32 field_110_tlvInfo;
    s32 field_114_paddding;
    s32 field_118_code_converted;
    s16 field_11C_code_length;

    enum Flags_11E
    {
        eBit1_bDontSetDestroyed = 0x1,
        eBit2_unused = 0x2,
        eBit3_padding = 0x4,
        eBit4_bAbeGettingCloser = 0x8
    };
    BitField16<Flags_11E> field_11E_flags;

    s32 field_120_last_event_idx;
    u8 field_124_code_buffer[16];
    s16 field_134_buffer_start;
    s16 field_136_buffer_idx;
    u16 field_138_brain_state;
    s16 field_13A_brain_sub_state;
    s16 field_13C_redundant;
    s16 field_13E_padding;
    s32 field_140_timer;
    s32 field_144_timer2;
    s32 field_148_padding;
    s32 field_14C_padding;
    u8** field_150_res;
    s16 field_154_previous_brain_state;
    s16 field_156_always_4;
    s16 field_158_code_pos;
    s16 field_15A_bCodeMatches;
};
ALIVE_ASSERT_SIZEOF(SlingMudokon, 0x15C);

} // namespace AO
