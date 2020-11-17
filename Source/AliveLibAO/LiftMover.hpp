#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

struct Path_LiftMover : public Path_TLV
{
    __int16 field_18_switch_id;
    __int16 field_1A_lift_id;
    __int16 field_1C_direction;
    __int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_LiftMover, 0x20);

class LiftPoint;

class LiftMover : public BaseGameObject
{
public:
    EXPORT LiftMover* ctor_4054E0(Path_LiftMover* pTlv, int tlvInfo);
    EXPORT BaseGameObject* dtor_405550();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    EXPORT LiftMover* Vdtor_405A50(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4055C0();

    LiftPoint* FindLiftPointWithId(short id);

    unsigned __int16 field_10_enabled_by_switch_id;
    __int16 field_12_target_lift_point_id;
    int field_14_tlvInfo;
    BaseGameObject* field_18_pLiftPoint; // TODO: LiftPoint* or actually no because it can be a platform as well?
    FP field_1C_speed;
    __int16 field_20_state;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(LiftMover, 0x24);

}
