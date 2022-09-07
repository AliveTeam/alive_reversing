#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Psx.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class BeeSwarmHole final : public ::BaseGameObject
{
public:
    BeeSwarmHole(relive::Path_BeeSwarmHole* pTlv, const Guid& tlvId);

    virtual void VUpdate() override;

    Guid field_10_tlvInfo;
    PSX_RECT field_14_rect = {};
    s32 mStartIntervalTimer = 0;
    u16 mStartInterval = 0;
    relive::Path_BeeSwarmHole::MovementType mMovementType = relive::Path_BeeSwarmHole::MovementType::eHover;
    s16 field_26_bees_amount = 0;
    u16 field_28_chase_time = 0;
    u16 field_2A_speed = 0;
};
ALIVE_ASSERT_SIZEOF(BeeSwarmHole, 0x2C);


} // namespace AO
