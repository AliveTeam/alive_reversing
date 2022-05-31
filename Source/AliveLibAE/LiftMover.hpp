#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

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

struct LiftMover_State final
{
    AETypes field_0_type_id; // never read?
    s16 field_2_padding;
    s32 field_4_tlvInfo;
    LiftMoverStates field_8_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(LiftMover_State, 0xC);

struct Path_LiftMover final : public Path_TLV
{
    s16 field_10_lift_mover_switch_id;
    s16 field_12_target_lift_point_id;
    enum class YDirection : s16
    {
        eDown_0 = 0,
        eUp_1 = 1,
    };
    YDirection field_14_move_direction;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftMover, 0x18);

class LiftMover final : public BaseGameObject
{
public:
    LiftMover(Path_LiftMover* pTlv, s32 tlvInfo);
    ~LiftMover();

    static s32 CreateFromSaveState_40D180(const u8* pData);

    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    void vUpdate_40CE20();
    s32 vGetSaveState_40D240(LiftMover_State* pState);
    LiftPoint* GetLiftPoint_40D0F0();

private:
    s16 field_20_lift_mover_switch_id;
    s16 field_22_target_lift_point_id;
    s32 field_24_tlvInfo;
    s32 field_28_lift_id;
    FP field_2C_speed;
    LiftMoverStates field_30_state;
    s16 field_32_bMoveInProgress;
};
ALIVE_ASSERT_SIZEOF(LiftMover, 0x34);
