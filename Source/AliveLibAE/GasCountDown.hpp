#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Font.hpp"

class GasCountDown final : public BaseGameObject
{
public:
    GasCountDown(relive::Path_GasCountDown* pTlv, const Guid& tlvId);
    ~GasCountDown();
    
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

private:
    void DealDamage();

private:
    PalResource mPal;
    Font_Context field_20_font_context = {};
    Alive::Font field_30_font = {};
    Guid field_68_tlvInfo;
    s16 field_6C_xpos = 0;
    s16 field_6E_ypos = 0;
    s16 field_70_start_timer_switch_id = 0;
    s16 field_72_stop_timer_switch_id = 0;
    s16 field_74_time_left = 0;
    u16 field_76_gas_countdown_timer = 0;
};
ALIVE_ASSERT_SIZEOF(GasCountDown, 0x78);

ALIVE_VAR_EXTERN(s32, sGasTimer_5C1BE8);
ALIVE_VAR_EXTERN(s16, gGasOn_5C1C00);
