#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Font.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/Primitives_common.hpp"

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
    PalResource mPal;
    FontContext mFontContext = {};
    AliveFont mFont = {};
    Guid mTlvInfo;
    s16 mTextX = 0;
    s16 mTextY = 0;
    s16 mPolysToRenderCount = 0;
    s16 mSwitchId = 0;
    s16 mTlvX = 0;
    s16 mTlvY = 0;
    s16 mCount = 0;
    s16 mNumberOfMeterBars = 0;
    s16 mMinesAlarmCountdown = 0;
    s16 mStartingSwitchState = 0;
    s16 mBarCount = 0;
    relive::reliveChoice mStartFilled = relive::reliveChoice::eNo;
    Poly_G4 mPolyG4s[2][kMeterBarsXCount] = {};
};

extern u8 gTotalMeterBars;
extern bool gbDrawMeterCountDown;
