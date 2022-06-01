#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "SwitchStates.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class PullRingSwitchSound : s16
{
    eNone_0 = 0,
    eWellExit_1 = 1,
    RingBellHammer_2 = 2,
    eDoorEffect_3 = 3,
};

enum class PullRingSoundDirection : s16
{
    eBoth_0 = 0,
    eLeft_1 = 1,
    eRight_2 = 2
};

struct Path_PullRingRope final : public Path_TLV
{
    s16 field_18_switch_id;
    SwitchOp field_1A_action;
    u16 field_1C_rope_length;
    Scale_short field_1E_scale;
    PullRingSwitchSound field_20_on_sound;
    PullRingSwitchSound field_22_off_sound;
    PullRingSoundDirection field_24_sound_direction;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_PullRingRope, 0x28);

class Rope;

class PullRingRope final : public BaseAnimatedWithPhysicsGameObject
{
public:
    PullRingRope(Path_PullRingRope* pTlv, s32 tlvInfo);
    ~PullRingRope();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    Bool32 vIsNotBeingPulled();
    s16 Pull(BaseAliveGameObject* pFrom);

    s32 field_D4_padding[4];
    s32 field_E4_stay_in_state_ticks;
    s32 field_E8_tlv_info;
    enum class States : s16
    {
        eIdle_0 = 0,
        eBeingPulled_1 = 1,
        eTriggerEvent_2 = 2,
        eReturnToIdle_3 = 3
    };
    States field_EC_state;
    s16 field_EE_switch_id;
    SwitchOp field_F0_action;
    s16 field_F2_pad;
    BaseAliveGameObject* field_F4_pPuller;
    Rope* field_F8_pRope;
    PullRingSwitchSound field_FC_on_sound;
    PullRingSwitchSound field_FE_off_sound;
    PullRingSoundDirection field_100_sound_direction;
    s16 field_102_padding;
};
ALIVE_ASSERT_SIZEOF(PullRingRope, 0x104);


} // namespace AO
