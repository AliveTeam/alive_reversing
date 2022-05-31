#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"

struct Path_GasCountDown final : public Path_TLV
{
    s16 field_10_start_timer_switch_id;
    u16 field_12_gas_countdown_timer;
    s16 field_14_stop_timer_switch_id;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasCountDown, 0x18);

class GasCountDown final : public BaseGameObject
{
public:
    GasCountDown(Path_GasCountDown* pTlv, s32 tlvInfo);
    ~GasCountDown();
    
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

private:
    void vScreenChanged_417700();

    void vRender_4175A0(PrimHeader** pOt);

    void vUpdate_4172E0();

    void DealDamage();

private:
    Font_Context field_20_font_context;
    Alive::Font field_30_font;
    s32 field_68_tlvInfo;
    s16 field_6C_xpos;
    s16 field_6E_ypos;
    s16 field_70_start_timer_switch_id;
    s16 field_72_stop_timer_switch_id;
    s16 field_74_time_left;
    u16 field_76_gas_countdown_timer;
};
ALIVE_ASSERT_SIZEOF(GasCountDown, 0x78);

ALIVE_VAR_EXTERN(s32, sGasTimer_5C1BE8);
ALIVE_VAR_EXTERN(s16, gGasOn_5C1C00);
