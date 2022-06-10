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
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlingMudokon, 0x20);

enum class eSlingMudMotions : s32
{
    Motion_0_Idle,
    Motion_1_Angry,
    Motion_2_Speak,
    Motion_3_ShootStart,
    Motion_4_ShootEnd,
    Motion_5_AngryToIdle
};

enum SlingMudBrainStates : u16
{
    Brain_0_GiveCode = 0,
    Brain_1_Spawn = 1,
    Brain_2_AskForPassword = 2
};

class SlingMudokon final : public BaseAliveGameObject
{
public:
    SlingMudokon(Path_SlingMudokon* pTlv, s32 tlvInfo);
    ~SlingMudokon();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    virtual void VCallBrain();
    virtual void VCallMotion();

    void VUpdateAnimData();

    eSlingMudMotions GetNextMotion() const
    {
        return static_cast<eSlingMudMotions>(mNextMotion);
    }

    GameSpeakEvents getLastIdx();

    // Motions
    void Motion_0_Idle();
    void Motion_1_Angry();
    void Motion_2_Speak();
    void Motion_3_ShootStart();
    void Motion_4_ShootEnd();
    void Motion_5_AngryToIdle();

    // Brains
    s16 Brain_0_GiveCode();
    s16 Brain_1_Spawn();
    s16 Brain_2_AskForPassword();

    s32 field_110_tlvInfo;
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
    s32 field_140_timer;
    s32 field_144_timer2;
    u8** field_150_res;
    s16 field_154_previous_brain_state;
    s16 field_156_always_4;
    s16 field_158_code_pos;
    s16 field_15A_bCodeMatches;
};
ALIVE_ASSERT_SIZEOF(SlingMudokon, 0x15C);

} // namespace AO
