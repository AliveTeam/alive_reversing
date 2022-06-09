#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_SecurityDoor final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_switch_id;
    s16 field_14_code_1;
    s16 field_16_code_2;
    s16 field_18_xpos;
    s16 field_1A_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityDoor, 0x1C);

enum class SecurityDoorStates : s16
{
    eInactive_0 = 0,
    eSuccessChime_1 = 1,
    eSayingHi_2 = 2,
    eListeningForHi_3 = 3,
    eWaitingToSayPassword_4 = 4,
    ePreparingToSayPassword_5 = 5,
    eSayingPassword_6 = 6,
    ePausing_7 = 7,
    eListeningForPassword_9 = 9,
    eCheckingIfPasswordMatches_10 = 10,
    eSuccess_11 = 11,
    eFailure_12 = 12,
    eLaughAtFailure_13 = 13,
};

class SecurityDoor final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    SecurityDoor(Path_SecurityDoor* pTlv, s32 tlvInfo);
    ~SecurityDoor();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    s16 IsPlayerNear();

private:
    s32 field_F4_tlvInfo = 0;
    SecurityDoorStates field_F8_state = SecurityDoorStates::eInactive_0;
    s16 field_FA_switch_id = 0;
    s32 field_FC_code_converted = 0;
    s16 field_100_code_len = 0;
    s32 field_104_event_idx = 0;
    u8 field_108_stru[16] = {};
    s16 field_118_max_idx = 0;
    s16 field_11A_event_idx = 0;
    PSX_Point field_11C_top_left = {};
    PSX_Point field_120_bottom_right = {};
    s32 field_124_timer = 0;
    s16 field_128_max_idx = 0;
    s16 field_12A_unused = 0; // Gets set to 1 in vUpdate, but isn't used anywhere else. -- Nemin (6/7/2020)
};
ALIVE_ASSERT_SIZEOF(SecurityDoor, 0x12C);
