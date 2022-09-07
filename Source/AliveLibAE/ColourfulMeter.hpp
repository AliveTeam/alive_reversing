#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Font.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Path.hpp"

constexpr s16 kMeterBarsXCount = 20;

class ColourfulMeter final : public BaseGameObject
{
public:
    ColourfulMeter(relive::Path_ColourfulMeter* pTlv, const Guid& tlvId);
    ~ColourfulMeter();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    Font_Context field_20_font_context = {};
    Alive::Font field_30_font = {};
    Guid mTlvInfo;
    s16 mTextX = 0;
    s16 mTextY = 0;
    s16 mPolysToRenderCount = 0;
    s16 mSwitchId = 0;
    s16 mTlvX = 0;
    s16 mTlvY = 0;
    s16 field_78_count = 0;
    s16 mNumberOfMeterBars = 0;
    s16 mMinesAlarmCountdown = 0;
    s16 mStartingSwitchState = 0;
    s16 field_80 = 0;
    s16 field_82_bar_count = 0;
    relive::reliveChoice mStartFilled = relive::reliveChoice::eNo;
    s16 field_86 = 0;
    Poly_G4 field_88_polyG4s[2][kMeterBarsXCount] = {};
};
ALIVE_ASSERT_SIZEOF(ColourfulMeter, 0x6c8);

ALIVE_VAR_EXTERN(u8, gTotalMeterBars_5C1BFA);
ALIVE_VAR_EXTERN(u16, gbDrawMeterCountDown_5C1BF8);
