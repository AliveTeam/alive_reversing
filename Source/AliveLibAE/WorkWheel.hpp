#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

enum class WheelStates : s16
{
    eIdle_0 = 0,
    eTurning_1 = 1,
};


struct WorkWheel_SaveState final
{
    AETypes field_0_id;
    // pad
    s32 field_4_tlvInfo;
    s16 field_8_snd_counter;
    // pad
    WheelStates field_C_state;
    s16 field_E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(WorkWheel_SaveState, 0x10);

struct Path_WorkWheel final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_id;
    s16 field_14_activation_time;
    s16 field_16_off_time;
    Choice_short field_18_turn_off_when_stopped;
    s16 field_20_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WorkWheel, 0x1C);

class WorkWheel final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VStartTurning();
    virtual void VStopTurning(s16 bResetSwitch);

    EXPORT WorkWheel* ctor_4E35D0(Path_WorkWheel* pTlv, s32 tlvInfo);

private:
    EXPORT void dtor_4E3850();
    EXPORT WorkWheel* vdtor_4E3820(s32 flags);

public:
    EXPORT static s32 CC CreateFromSaveState_4E3B10(const u8* pState);

private:
    EXPORT s32 vGetSaveState_4E3C40(WorkWheel_SaveState* pState);
    EXPORT void vUpdate_4E38E0();
    EXPORT void vScreenChanged_4E3AD0();
    EXPORT void vStartTurning_4E3A20();
    EXPORT void vStopTurning_4E3A60(s16 bResetSwitch);

private:
    s32 field_F4_tlv_info;
    s16 field_F8_id;
    s16 field_FA_padding;
    WheelStates field_FC_state;
    s16 field_FE_activation_time;
    u16 field_100_on_counter;
    s16 field_102_off_time;
    Choice_short field_104_turn_off_when_stopped;
};
ALIVE_ASSERT_SIZEOF(WorkWheel, 0x108);
