#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Font.hpp"

namespace relive
{
    struct Path_GasCountDown;
}

class GasCountDown final : public BaseGameObject
{
public:
    GasCountDown(relive::Path_GasCountDown* pTlv, const Guid& tlvId);
    ~GasCountDown();
    
    virtual void VScreenChanged() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;

private:
    void DealDamage();

private:
    PalResource mPal;
    FontContext mFontContext = {};
    AliveFont mFont = {};
    Guid mTlvId;
    s16 mGasXPos = 0;
    s16 mGasYPos = 0;
    s16 mStartTimerSwitchId = 0;
    s16 mStopTimerSwitchId = 0;
    s16 mGasTimeLeftSecs = 0;
    u16 mGasCountdownTimer = 0;
};

extern bool gDeathGasOn;
extern s32 gDeathGasTimer;
