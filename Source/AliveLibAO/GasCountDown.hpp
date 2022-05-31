#pragma once

#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"

namespace AO {

struct Path_GasCountDown final : public Path_TLV
{
    s16 field_18_start_switch_id;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasCountDown, 0x1C);

class GasCountDown final : public BaseGameObject
{
public:
    GasCountDown(Path_GasCountDown* pTlv, s32 tlvInfo);
    ~GasCountDown();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void DealDamage();

    FontContext field_10_font_context;
    AliveFont field_20_font;
    s32 field_58_tlvInfo;
    s16 field_5C_xpos;
    s16 field_5E_ypos;
    u16 field_60_start_switch_id;
    s16 field_62_time_left;
};
ALIVE_ASSERT_SIZEOF(GasCountDown, 0x64);

ALIVE_VAR_EXTERN(s16, gGasOn_4FF888);
ALIVE_VAR_EXTERN(s32, sGasTimer_507700);

} // namespace AO
