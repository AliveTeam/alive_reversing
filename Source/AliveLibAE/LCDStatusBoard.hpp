#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Font.hpp"

class LCDStatusBoard final : public BaseGameObject
{
public:
    LCDStatusBoard(relive::Path_LCDStatusBoard* params, const Guid& tlvId);
    ~LCDStatusBoard();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

public:
    FontContext mFontContext;
    PalResource mPal;
    AliveFont mKilledMudsFont = {};
    AliveFont mRescuedMudsFont = {};
    AliveFont mMudsInLevelFont = {};
    AliveFont mMudsInAreaFont = {};
    Guid mTlvId;
    s16 mXPos = 0;
    s16 mYPos = 0;
    s16 mHideBoard = 0;
    s16 mMudsLeftInArea = 0;
};

extern s16 gMudokonsInArea;
extern s8 gZulagNumber;
