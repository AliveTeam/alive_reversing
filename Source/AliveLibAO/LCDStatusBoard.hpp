#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Font.hpp"

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

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;

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
