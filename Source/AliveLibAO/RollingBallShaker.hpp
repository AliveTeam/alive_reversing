#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Primitives_common.hpp"

void RollingBallShaker_ForceLink();

namespace AO {

class RollingBallShaker : public BaseGameObject
{
public:
    EXPORT RollingBallShaker* ctor_4361A0();
    EXPORT BaseGameObject* dtor_436200();
    EXPORT RollingBallShaker* Vdtor_436350(signed int flags);
    EXPORT void VUpdate_436260();
    EXPORT void VRender_436280(PrimHeader** ppOt);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Prim_ScreenOffset field_10_prim_screen_offset[2];
    __int16 field_30_shake_table_idx;
    __int16 field_32_bKillMe;
};
ALIVE_ASSERT_SIZEOF(RollingBallShaker, 0x34);

}
