#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint_common.hpp"

namespace relive
{
    struct Path_LiftMover;
}

class LiftPoint;

enum class LiftMoverStates : s16
{
    eInactive_0 = 0,
    eStartMovingDown_1 = 1,
    eMovingDown_2 = 2,
    eStartMovingUp_3 = 3,
    eMovingUp_4 = 4,
    eMovingDone_5 = 5,
};

struct LiftMoverSaveState final
{
    AETypes field_0_type_id; // never read?
    s16 field_2_padding;
    Guid field_4_tlvInfo;
    LiftMoverStates field_8_state;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(LiftMoverSaveState, 0xC);

class LiftMover final : public BaseGameObject
{
public:
    LiftMover(relive::Path_LiftMover* pTlv, const Guid& tlvId);
    ~LiftMover();

    static s32 CreateFromSaveState(const u8* pData);
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    LiftPoint* GetLiftPoint();

private:
    s16 field_20_lift_mover_switch_id = 0;
    s16 field_22_target_lift_point_id = 0;
    Guid field_24_tlvInfo;
    Guid field_28_lift_id;
    FP field_2C_speed = {};
    LiftMoverStates field_30_state = LiftMoverStates::eInactive_0;
    s16 field_32_bMoveInProgress = 0;
};
