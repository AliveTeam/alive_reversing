#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Font.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"

namespace relive
{
    struct Path_GasCountDown;
}

namespace AO {

class GasCountDown final : public ::BaseGameObject
{
public:
    GasCountDown(relive::Path_GasCountDown* pTlv, const Guid& tlvId);
    ~GasCountDown();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void DealDamage();

    PalResource mPal;
    FontContext field_10_FontContext;
    AliveFont field_20_font;
    Guid field_58_tlvInfo;
    s16 field_5C_xpos = 0;
    s16 field_5E_ypos = 0;
    u16 field_60_start_switch_id = 0;
    s16 field_62_time_left = 0;
};

extern s16 gGasOn;
extern s32 sGasTimer;

} // namespace AO
