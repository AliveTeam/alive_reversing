#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Font.hpp"
#include "Map.hpp"

namespace relive
{
    struct Path_LCDStatusBoard;
}

namespace AO {

class LCDStatusBoard final : public ::BaseGameObject
{
public:
    LCDStatusBoard(relive::Path_LCDStatusBoard* pTlv, const Guid& tlvId);
    ~LCDStatusBoard();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    PalResource mPal;
    FontContext mFontContext;
    AliveFont mKilledMudsFont;
    AliveFont mRescuedMudsFont;
    AliveFont mEmployeesFont;
    Guid mTlvId;
    s16 mXPos = 0;
    s16 mYPos = 0;
};

} // namespace AO
