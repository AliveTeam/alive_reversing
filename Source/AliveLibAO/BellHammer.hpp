#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "SwitchStates.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_BellHammer final : public Path_TLV
{
    s16 field_18_switch_id;
    SwitchOp field_1A_action;
    Scale_short field_1C_scale;
    XDirection_short field_1E_direction;
};
ALIVE_ASSERT_SIZEOF(Path_BellHammer, 0x20);

enum class BellHammerStates : u16
{
    eWaitForActivation_0 = 0,
    eSmashingBell_1 = 1
};

class BellHammer final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BellHammer(Path_BellHammer* pTlv, s32 tlvInfo);
    ~BellHammer();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    static void OnResLoaded(BellHammer* pThis);

    BellHammerStates field_E4_state = BellHammerStates::eWaitForActivation_0;
    s16 field_E6_switch_id = 0;
    s32 field_E8_tlvInfo = 0;
    s32 field_EC_pending_resource_count = 0;
    s32 field_F0_bSpawnElum = 0;
};
ALIVE_ASSERT_SIZEOF(BellHammer, 0xF4);


} // namespace AO
