#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum SecurityDoorStates : s16
{
    eInactive_0 = 0,
    eSuccessChime_1 = 1,
    eSayingHi_2 = 2,
    eListeningForHi_3 = 3,
    eWaitingToSayPassword_4 = 4,
    ePreparingToSayPassword_5 = 5,
    eSayingPassword_6 = 6,
    ePausing_7 = 7,
    eListeningForPassword_8 = 8,
    eCheckingIfPasswordMatches_9 = 9,
    eLaughAtFailure_10 = 10
};

class SecurityDoor final : public BaseAnimatedWithPhysicsGameObject
{
public:
    SecurityDoor(relive::Path_SecurityDoor* pTlv, const Guid& tlvId);
    ~SecurityDoor();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    bool IsPlayerNear();

    Guid field_E4_tlvInfo;
    s16 field_E8_state = 0;
    s16 field_EA_switch_id = 0;
    s32 field_EC_code_converted = 0;
    s16 field_F0_code_len = 0;
    s32 field_F4_event_idx = 0;
    u8 field_F8_stru[16] = {};
    s16 field_108_max_idx = 0;
    s16 field_10A_event_idx = 0;
    PSX_Point field_10C_top_left = {};
    PSX_Point field_110_bottom_right = {};
    s32 field_114_timer = 0;
    s16 field_118_max_idx = 0;
    s16 field_11A_unused = 0;
};
ALIVE_ASSERT_SIZEOF(SecurityDoor, 0x11C);

} // namespace AO
