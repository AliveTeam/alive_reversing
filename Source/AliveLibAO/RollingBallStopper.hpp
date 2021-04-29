#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

class PathLine;

struct Path_RollingBallStopper final : public Path_TLV
{
    s16 field_18_id_on;
    Scale_short field_1A_scale;
    s16 field_1C_id_off;
    XDirection_short field_1E_direction;
};
ALIVE_ASSERT_SIZEOF(Path_RollingBallStopper, 0x20);

class RollingBallStopper final : public BaseAliveGameObject
{
public:
    EXPORT RollingBallStopper* ctor_43BCE0(Path_RollingBallStopper* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* Vdtor_43C160(s32 flags);
    EXPORT BaseGameObject* dtor_43C0A0();
    EXPORT void VScreenChanged_43C130();
    EXPORT void VUpdate_43BF70();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 field_10C_tlvInfo;
    s16 field_110_unused;
    enum class States : s16
    {
        eWaitForTrigger_0,
        eMoveStopper_1,
        eMovingDone_2
    };
    States field_112_state;
    u16 field_114_switch_on_id;
    u16 field_116_switch_id_off;
    PathLine* field_118_pLine;
};
ALIVE_ASSERT_SIZEOF(RollingBallStopper, 0x11C);

} // namespace AO
