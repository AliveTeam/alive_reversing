#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "ScopedSeq.hpp"
#include "../AliveLibAE/Path.hpp"
#include "SwitchStates.hpp"

namespace AO {

enum class LeverState : s16
{
    eWaiting_0 = 0,
    ePulled_1 = 1,
    eFinished_2 = 2
};

enum class LeverSoundType : s16
{
    eNone = 0,
    eWell_1 = 1,
    eSwitchBellHammer_2 = 2,
    eDoor_3 = 3,
    eElectricWall_4 = 4,
    eSecurityOrb_5 = 5
};

enum class LeverSoundDirection : s16
{
    eLeftAndRight_0 = 0,
    eLeft_1 = 1,
    eRight_2 = 2,
};

enum class SwitchOp : s16;

struct Path_Lever final : public Path_TLV
{
    s16 field_18_switch_id;
    SwitchOp field_1A_action;
    Scale_short field_1C_scale;
    LeverSoundType field_1E_on_sound;
    LeverSoundType field_20_off_sound;
    LeverSoundDirection field_22_sound_direction;
};
ALIVE_ASSERT_SIZEOF(Path_Lever, 0x24);


class Lever final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Lever(Path_Lever* pTlv, s32 tlvInfo);
    ~Lever();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    // New virtual
    virtual s32 VPull(s16 bLeftDirection);

    s16 field_E4_switch_id = 0;
    LeverState field_E8_state = LeverState::eWaiting_0;
    s32 field_EC_tlvInfo = 0;
    s16 field_F0_bPulledFromLeft = 0;
    SwitchOp field_F2_action = SwitchOp::eSetTrue_0;
    LeverSoundType field_F4_on_sound = LeverSoundType::eNone;
    LeverSoundType field_F6_off_sound = LeverSoundType::eNone;
    LeverSoundDirection field_F8_sound_direction = LeverSoundDirection::eLeftAndRight_0;
};
ALIVE_ASSERT_SIZEOF(Lever, 0xFC);

} // namespace AO
