#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_SecurityClaw final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_alarm_switch_id;
    s16 field_1C_alarm_duration;
    s16 field_1E_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_SecurityClaw, 0x20);

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
    SecurityClaw(Path_SecurityClaw* pTlv, s32 tlvInfo);
    ~SecurityClaw();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    s32 field_10C_tlvInfo;
    SecurityClawStates field_110_state;
    s16 field_112;
    s32 field_114_timer;
    s16 field_118_alarm_switch_id;
    s16 field_11A_alarm_duration;
    FP field_11C_clawX;
    FP field_120_clawY;
    u8 field_124;
    // 3 byte pad
    s32 field_128_sound_channels;
    s16 field_12C_pDetector;
    s16 field_12E;
    Claw* field_130_pClaw;
    PSX_Point field_134;
    PSX_Point field_138;
    DynamicArrayT<MotionDetector>* field_13C_pArray;
};
ALIVE_ASSERT_SIZEOF(SecurityClaw, 0x140);

} // namespace AO
