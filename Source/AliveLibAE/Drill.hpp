#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class DrillStates : s16
{
    State_0_Restart_Cycle = 0,
    State_1_Going_Down = 1,
    State_2_GoingUp = 2,
};

struct Drill_State final
{
    s16 field_0;
    s16 field_2_padding;
    s32 field_4;
    Guid field_8_tlvInfo;
    s32 field_C_off_timer;
    DrillStates field_10_state;
    s16 field_12_xyoff;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(Drill_State, 0x14);

class Drill final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Drill(relive::Path_Drill* pTlv, const Guid& tlvId);
    ~Drill();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pData);

private:
    void EmitSparks();
    s16 DamageTouchingObjects();

private:
    DrillStates mDrillState = DrillStates::State_0_Restart_Cycle;
    s16 mDrillDistance = 0;
    s16 mDrillSwitchId = 0;
    relive::Path_Drill::DrillDirection mDrillDirection = relive::Path_Drill::DrillDirection::eDown;
    s16 mMinOffTime = 0;
    s16 mMaxOffTime = 0;
    s16 field_100_min_off_time_speed_change = 0;
    s16 field_102_max_off_time_speed_change = 0;
    Guid mTlvInfo;
    s32 mOffTimer = 0;
    s32 mAudioChannelsMask = 0;
    FP mAdjustedXPos = {};
    FP mAdjustedYPos = {};
    FP mInitialSpeed = {};
    FP mCurrentSpeed = {};
    FP mOffSpeed = {};
    FP field_124_xyoff = {};
    enum Flags
    {
        eBit1_StartOff = 0x1,
        eBit2_ToggleStartState_StartOn = 0x2,
        eBit3_UseId = 0x4,
        eBit4_Toggle = 0x8,
        eBit5_SpeedChanged = 0x10,
        eBit6_StartPosIsBottom = 0x20,
    };
    BitField16<Flags> field_128_flags = {};
};
ALIVE_ASSERT_SIZEOF(Drill, 0x12C);
