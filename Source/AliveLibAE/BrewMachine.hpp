#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Font.hpp"
#include "../AliveLibCommon/Function.hpp"

class BrewMachine final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BrewMachine(relive::Path_BrewMachine* pTlv, const Guid& tlvId);
    ~BrewMachine();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    Font_Context mFontContext = {};
    Alive::Font mFont = {};
    s16 mTextX = 0;
    s16 mTextY = 0;
    Guid mTlvInfo;

public:
    s16 mTotalBrewCount = 0;

private:
    s16 mRemainingBrewCount = 0;
    s16 mBrewMachineCamera = 0;
    PalResource mPal;
};
ALIVE_ASSERT_SIZEOF(BrewMachine, 0x1E8);
