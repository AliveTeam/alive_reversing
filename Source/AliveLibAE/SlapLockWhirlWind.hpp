#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"

struct SlapLockWhirlWind_State final
{
    AETypes field_0_type;
    s16 field_2_switch_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLockWhirlWind_State, 0x4);

class SlapLockWhirlWind final : public BaseGameObject
{
public:
    SlapLockWhirlWind(s16 doorNumber, s16 switchId, FP xpos, FP ypos, FP scale);
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    static s32 CreateFromSaveState(const u8* pBuffer);

    s16 SwitchId() const;

private:
    FP field_20_xpos = {};
    FP field_24_ypos = {};
    FP field_28_scale = {};
    FP field_2C_door_x = {};
    FP field_30_door_y = {};
    FP field_34_door_scale = {};
    s32 field_38_orb_whirlwind_id = 0;
    s16 field_3C_state = 0;
    s32 field_40_timer = 0;
    s16 field_44_switch_id = 0;
};
ALIVE_ASSERT_SIZEOF(SlapLockWhirlWind, 0x48);
