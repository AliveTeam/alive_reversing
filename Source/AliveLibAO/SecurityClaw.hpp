#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/DynamicArray.hpp"

namespace relive
{
    struct Path_SecurityClaw;
}

namespace AO {

class Claw final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Claw();

    void VScreenChanged() override;
};

class MotionDetector;

enum class SecurityClawStates : s16
{
    eCamSwap_0,
    eIdle_1,
    eDoZapEffects_2,
    eAnimateClaw_DoFlashAndSound_3
};

class SecurityClaw final : public BaseAliveGameObject
{
public:
    SecurityClaw(relive::Path_SecurityClaw* pTlv, const Guid& tlvId);
    ~SecurityClaw();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    Guid mTlvInfo;
    SecurityClawStates mState = SecurityClawStates::eCamSwap_0;
    s32 mTimer = 0;
    s16 mAlarmSwitchId = 0;
    s16 mAlarmDuration = 0;
    FP mClawX = {};
    FP mClawY = {};
    u8 mAngle = 0;
    s32 mOrbSoundChannels = 0;
    bool mDetectorComeBack = false;
    Guid mClawId;
    PSX_Point mTlvTopLeft = {};
    PSX_Point mTlvBottomRight = {};
    DynamicArrayT<MotionDetector>* mMotionDetectorArray;
};

} // namespace AO
