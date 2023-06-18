#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/SaveStateBase.hpp"

enum class DrillStates : s16
{
    eRestartCycle_0 = 0,
    eGoingDown_1 = 1,
    eGoingUp_2 = 2,
};

struct DrillSaveState final : public SaveStateBase
{
    DrillSaveState()
        : SaveStateBase(ReliveTypes::eDrill, sizeof(*this))
    { }
    Guid mDrillTlvId;
    s32 mOffTimer;
    DrillStates mState;
    s16 mXYOff;
};

class Drill final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Drill(relive::Path_Drill* pTlv, const Guid& tlvId);
    ~Drill();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

    static void CreateFromSaveState(SerializedObjectData& pData);

private:
    void EmitSparks();
    s16 DamageTouchingObjects();

private:
    DrillStates mState = DrillStates::eRestartCycle_0;
    s16 mDrillDistance = 0;
    s16 mDrillSwitchId = 0;
    relive::Path_Drill::DrillDirection mDrillDirection = relive::Path_Drill::DrillDirection::eDown;
    s16 mOnMinPauseTime = 0;
    s16 mOnMaxPauseTime = 0;
    s16 mOffMinPauseTime = 0;
    s16 mOffMaxPauseTime = 0;
    Guid mTlvInfo;
    s32 mOffTimer = 0;
    s32 mAudioChannelsMask = 0;
    FP mAdjustedXPos = {};
    FP mAdjustedYPos = {};
    FP mInitialSpeed = {};
    FP mCurrentSpeed = {};
    FP mOffSpeed = {};
    FP mXYOff = {};
    bool mStartOff = false;
    bool mToggleStartState_StartOn = false;
    bool mUseId = false;
    bool mToggle = false;
    bool mSpeedChange = false;
    bool mStartPosIsBottom = false;
};
