#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "ScopedSeq.hpp"
#include "../AliveLibAE/Path.hpp"

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

    EXPORT void VScreenChanged_4816F0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4812D0();

    // New virtual
    virtual s32 VPull(s16 bLeftDirection);

    EXPORT s32 vPull_481640(s16 bLeftDirection);

    s32 field_D4_padding[4];
    s16 field_E4_switch_id;
    s16 field_E6_pad;
    LeverState field_E8_state;
    s16 field_EA_pad;
    s32 field_EC_tlvInfo;
    s16 field_F0_bPulledFromLeft;
    SwitchOp field_F2_action;
    LeverSoundType field_F4_on_sound;
    LeverSoundType field_F6_off_sound;
    LeverSoundDirection field_F8_sound_direction;
    s16 field_FA_pad;
};
ALIVE_ASSERT_SIZEOF(Lever, 0xFC);

} // namespace AO
