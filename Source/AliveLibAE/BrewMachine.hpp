#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Font.hpp"

namespace relive
{
    struct Path_BrewMachine;
}

class BrewMachine final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BrewMachine(relive::Path_BrewMachine* pTlv, const Guid& tlvId);
    ~BrewMachine();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

public:
    s16 mTotalBrewCount = 0;

private:
    FontContext mFontContext = {};
    AliveFont mFont = {};
    s16 mTextX = 0;
    s16 mTextY = 0;
    Guid mTlvInfo;
    s16 mRemainingBrewCount = 0;
    s16 mBrewMachineCamera = 0;
    PalResource mPal;
};
