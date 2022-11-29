#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../AliveLibCommon/Psx_common.hpp"

namespace AO {

class BeeSwarmHole final : public ::BaseGameObject
{
public:
    BeeSwarmHole(relive::Path_BeeSwarmHole* pTlv, const Guid& tlvId);

    virtual void VUpdate() override;

    Guid mTlvId;
    PSX_RECT field_14_rect = {};
    s32 mStartIntervalTimer = 0;
    u16 mStartInterval = 0;
    relive::Path_BeeSwarmHole::MovementType mMovementType = relive::Path_BeeSwarmHole::MovementType::eHover;
    s16 field_26_bees_amount = 0;
    u16 field_28_chase_time = 0;
    u16 field_2A_speed = 0;
};


} // namespace AO
