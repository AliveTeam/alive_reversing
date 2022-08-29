#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_SecurityClaw final : public Path_TLV
{
    Scale_short mScale;
    s16 mAlarmSwitchId;
    s16 mAlarmDuration;
    s16 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SecurityClaw, 0x20);

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

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    Guid mTlvInfo;
    SecurityClawStates field_110_state = SecurityClawStates::eCamSwap_0;
    s32 field_114_timer = 0;
    s16 mAlarmSwitchId = 0;
    s16 mAlarmDuration = 0;
    FP mClawX = {};
    FP mClawY = {};
    u8 field_124 = 0;
    s32 field_128_sound_channels = 0;
    s16 field_12C_pDetector = 0;
    s16 field_12E = 0;
    Claw* mClaw = nullptr;
    PSX_Point field_134_top_left = {};
    PSX_Point field_138_bottom_right = {};
    DynamicArrayT<MotionDetector>* field_13C_pArray;
};
ALIVE_ASSERT_SIZEOF(SecurityClaw, 0x140);

} // namespace AO
