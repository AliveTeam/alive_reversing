#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

struct SlapLockWhirlWind_State
{
    AETypes field_0_type;
    s16 field_2_switch_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLockWhirlWind_State, 0x4);

class SlapLockWhirlWind : public BaseGameObject
{
public:
    EXPORT SlapLockWhirlWind* ctor_43D7E0(s16 doorNumber, s16 switchId, FP xpos, FP ypos, FP scale);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(BYTE* pSaveBuffer) override;
    static EXPORT s32 CC CreateFromSaveState_43DC20(const BYTE* pBuffer);

private:
    EXPORT void dtor_43DA70();
    EXPORT SlapLockWhirlWind* vdtor_43DA40(s32 flags);
    EXPORT s32 vGetSaveState_43DC50(SlapLockWhirlWind_State* pSaveState);
    EXPORT void vUpdate_43DA90();

public:
    s16 SwitchId() const;

private:
    FP field_20_xpos;
    FP field_24_ypos;
    FP field_28_scale;
    FP field_2C_door_x;
    FP field_30_door_y;
    FP field_34_door_scale;
    s32 field_38_orb_whirlwind_id;
    s16 field_3C_state;
    s16 field_3E_padding;
    s32 field_40_timer;
    s16 field_44_switch_id;
    s16 field_46_padding;
};
ALIVE_ASSERT_SIZEOF(SlapLockWhirlWind, 0x48);
