#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "DynamicArray.hpp"

namespace AO {

struct Path_SecurityClaw final : public Path_TLV
{
    s16 field_18_scale;
    s16 field_1A_alarm_id;
    s16 field_1C_alarm_time;
    s16 field_1E_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_SecurityClaw, 0x20);

class Claw : public BaseAnimatedWithPhysicsGameObject
{
public:
    void ctor();

    BaseGameObject* VDestructor(s32 flags);

    BaseGameObject* Vdtor(s32 flags);
};

class MotionDetector;

enum class SecurityClawStates : s16
{
    eCamSwap_0,
    eIdle_1,
    eDoZapEffects_2,
    eAnimateClaw_DoFlashAndSound_3
};

class SecurityClaw : public BaseAliveGameObject
{
public:
    EXPORT SecurityClaw* ctor_418A70(Path_SecurityClaw* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_418CE0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT SecurityClaw* Vdtor_419700(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_4196F0();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_419520(BaseGameObject* pFrom);

    virtual void VUpdate() override;

    EXPORT void VUpdate_418DE0();

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    s32 field_10C_tlvInfo;
    SecurityClawStates field_110_state;
    s16 field_112;
    s32 field_114_timer;
    s16 field_118_alarm_id;
    s16 field_11A;
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
