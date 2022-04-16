#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

struct Path_LiftMover final : public Path_TLV
{
    s16 field_18_lift_mover_switch_id;
    s16 field_1A_target_lift_point_id;
    enum class YDirection : s16
    {
        eDown_0 = 0,
        eUp_1 = 1
    };
    YDirection field_1C_direction;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_LiftMover, 0x20);

class LiftPoint;

class LiftMover final : public BaseGameObject
{
public:
    EXPORT LiftMover* ctor_4054E0(Path_LiftMover* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* dtor_405550();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    EXPORT LiftMover* Vdtor_405A50(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4055C0();

    LiftPoint* FindLiftPointWithId(s16 id);

    u16 field_10_lift_mover_switch_id;
    s16 field_12_target_lift_point_id;
    s32 field_14_tlvInfo;
    BaseGameObject* field_18_pLiftPoint; // TODO: LiftPoint* or actually no because it can be a platform as well?
    FP field_1C_speed;
    s16 field_20_state;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(LiftMover, 0x24);

} // namespace AO
