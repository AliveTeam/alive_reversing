#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

namespace AO {

class PathLine;

struct Path_RollingBallStopper : public Path_TLV
{
    __int16 field_18_id_on;
    __int16 field_1A_scale_background;
    __int16 field_1C_id_off;
    __int16 field_1E_direction;
};
ALIVE_ASSERT_SIZEOF(Path_RollingBallStopper, 0x20);

class RollingBallStopper : public BaseAliveGameObject
{
public:
    EXPORT RollingBallStopper* ctor_43BCE0(Path_RollingBallStopper* pTlv, int tlvInfo);
    EXPORT BaseGameObject* Vdtor_43C160(signed int flags);
    EXPORT BaseGameObject* dtor_43C0A0();
    EXPORT void VScreenChanged_43C130();
    EXPORT void VUpdate_43BF70();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    int field_10C_tlvInfo;
    __int16 field_110_unused;
    enum class States : __int16
    {
        eWaitForTrigger_0,
        eMoveStopper_1,
        eMovingDone_2
    };
    States field_112_state;
    unsigned __int16 field_114_switch_on_id;
    unsigned __int16 field_116_switch_id_off;
    PathLine* field_118_pLine;
};
ALIVE_ASSERT_SIZEOF(RollingBallStopper, 0x11C);

}
