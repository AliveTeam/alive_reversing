#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"

namespace relive
{
    struct Path_LiftMover;
}

namespace AO {

class LiftPoint;

class LiftMover final : public ::BaseGameObject
{
public:
    LiftMover(relive::Path_LiftMover* pTlv, const Guid& tlvId);
    ~LiftMover();
    
    virtual void VUpdate() override;

    LiftPoint* FindLiftPointWithId(s16 id);

    u16 field_10_lift_mover_switch_id = 0;
    s16 field_12_target_lift_point_id = 0;
    Guid field_14_tlvInfo;
    BaseGameObject* field_18_pLiftPoint = nullptr; // TODO: LiftPoint* or actually no because it can be a platform as well?
    FP field_1C_speed = {};
    s16 field_20_state = 0;
};

} // namespace AO
