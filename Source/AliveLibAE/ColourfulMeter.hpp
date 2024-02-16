#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Font.hpp"
#include "../relive_lib/Primitives.hpp"

constexpr s16 kMeterBarsXCount = 20;

namespace relive
{
    struct Path_ColourfulMeter;
}

class ColourfulMeter final : public BaseGameObject
{
public:
    ColourfulMeter(relive::Path_ColourfulMeter* pTlv, const Guid& tlvId);
    ~ColourfulMeter();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    FontContext mFontContext = {};
    AliveFont mFont = {};
    Guid mTlvInfo;
    s16 mTlvX = 0;
    s16 mTlvY = 0;
    PalResource mPal;
    s16 mSwitchId = 0;
    s16 mNumberOfMeterBars = 0;
    s16 mTextX = 0;
    s16 mTextY = 0;
    s16 mPolysToRenderCount = 0;
    s16 mCount = 15;
    s16 mMinesAlarmCountdown = 0;
    s16 mStartingSwitchState = 0;
    s16 mBarCount = 0;
    bool mStartFilled = false;
    Poly_G4 mPolyG4s[kMeterBarsXCount] = {};
};

extern u8 gTotalMeterBars;
extern bool gbDrawMeterCountDown;
