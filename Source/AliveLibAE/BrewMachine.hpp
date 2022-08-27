#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "Font.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_BrewMachine final : public Path_TLV
{
    s16 mBrewCount;
    s16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BrewMachine, 0x14);

class BrewMachine final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BrewMachine(relive::Path_BrewMachine* pTlv, s32 tlvInfo);
    ~BrewMachine();
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    Font_Context mFontContext = {};
    Alive::Font mFont = {};
    s16 mTextX = 0;
    s16 mTextY = 0;
    s32 mTlvInfo = 0;

public:
    s16 mTotalBrewCount = 0;

private:
    s16 mRemainingBrewCount = 0;
    s16 mBrewMachineCamera = 0;
};
ALIVE_ASSERT_SIZEOF(BrewMachine, 0x1E8);
