#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class WheelStates : s16
{
    eIdle_0 = 0,
    eTurning_1 = 1,
};

struct WorkWheel_SaveState final
{
    AETypes field_0_id;
    s16 padding_1;
    s32 field_4_tlvInfo;
    s16 field_8_snd_counter;
    s16 padding_2;
    WheelStates field_C_state;
    s16 padding_3;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(WorkWheel_SaveState, 0x10);

struct Path_WorkWheel final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mActivationTime;
    s16 mTurnOffTime;
    Choice_short mTurnOffWhenStopped;
    s16 field_20_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WorkWheel, 0x1C);

class WorkWheel final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VStartTurning();
    virtual void VStopTurning(s16 bResetSwitch);

    WorkWheel(Path_WorkWheel* pTlv, s32 tlvInfo);
    ~WorkWheel();

public:
    static s32 CreateFromSaveState(const u8* pState);

private:
    s32 mTlvInfo = 0;
    s16 mSwitchId = 0;
    WheelStates mState = WheelStates::eIdle_0;
    s16 mActivationTime = 0;
    u16 mTurningTime = 0;
    s16 mTurnOffTime = 0;
    Choice_short mTurnOffWhenStopped = Choice_short::eNo_0;
};
ALIVE_ASSERT_SIZEOF(WorkWheel, 0x108);
