#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Primitives_common.hpp"

void RollingBallShaker_ForceLink();

namespace AO {

class RollingBallShaker final : public BaseGameObject
{
public:
    RollingBallShaker();
    ~RollingBallShaker();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Prim_ScreenOffset field_10_prim_screen_offset[2];
    s16 field_30_shake_table_idx;
    s16 field_32_bKillMe;
};
ALIVE_ASSERT_SIZEOF(RollingBallShaker, 0x34);

} // namespace AO
