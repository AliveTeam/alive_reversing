#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

class LiftPoint;

enum class LiftMoverStates : short
{
    eInactive_0 = 0,
    eStartMovingDown_1 = 1,
    eMovingDown_2 = 2,
    eStartMovingUp_3 = 3,
    eMovingUp_4 = 4,
    eMovingDone_5 = 5,
};

struct LiftMover_State
{
    __int16 field_0_type;
    //__int16 field_2;
    int field_4_tlvInfo;
    LiftMoverStates field_8_state;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(LiftMover_State, 0xC);

struct Path_LiftMover : public Path_TLV
{
    __int16 field_10_switch_id;
    __int16 field_12_lift_id;
    __int16 field_14_direction_up;
    //__int16 pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftMover, 0x18);

class LiftMover : public BaseGameObject
{
public:
    EXPORT LiftMover* ctor_40CCD0(Path_LiftMover* pTlv, int tlvInfo);
    EXPORT static int CC CreateFromSaveState_40D180(const BYTE* pData);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

private:
    EXPORT void vUpdate_40CE20();
    EXPORT void dtor_40CDA0();
    EXPORT LiftMover* vdtor_40CD70(signed int flags);
    EXPORT int vGetSaveState_40D240(LiftMover_State* pState);
    EXPORT LiftPoint* GetLiftPoint_40D0F0();

private:
    __int16 field_20_enabled_by_switch_id;
    __int16 field_22_target_lift_point_id;
    int field_24_tlvInfo;
    int field_28_lift_id;
    FP field_2C_speed;
    LiftMoverStates field_30_state;
    __int16 field_32_bMoveInProgress;
};
ALIVE_ASSERT_SIZEOF(LiftMover, 0x34);
