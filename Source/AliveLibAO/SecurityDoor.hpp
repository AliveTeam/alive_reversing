#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_SecurityDoor : public Path_TLV
{
    s16 field_18_scale;
    s16 field_1A_id;
    s16 field_1C_code_1;
    s16 field_1E_code2;
    s16 field_20_xpos;
    s16 field_22_ypos;
};
ALIVE_ASSERT_SIZEOF(Path_SecurityDoor, 0x24);

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

class SecurityDoor : public BaseAnimatedWithPhysicsGameObject
{
public:
    BOOL IsPlayerNear();

    virtual void VUpdate() override;

    EXPORT void VUpdate_461AD0();

    EXPORT SecurityDoor* ctor_461840(Path_SecurityDoor* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_4619C0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT SecurityDoor* vdtor_461F90(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_461F80();

    int field_D4[4];
    int field_E4_tlvInfo;
    s16 field_E8_state;
    s16 field_EA_id;
    int field_EC_code_converted;
    s16 field_F0_code_len;
    s16 field_F2_padding;
    int field_F4_event_idx;
    BYTE field_F8_stru[16];
    s16 field_108_max_idx;
    s16 field_10A_event_idx;
    PSX_Point field_10C_top_left;
    PSX_Point field_110_bottom_right;
    int field_114_timer;
    s16 field_118_max_idx;
    s16 field_11A_unused;
};
ALIVE_ASSERT_SIZEOF(SecurityDoor, 0x11C);

}

