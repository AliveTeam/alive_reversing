#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "PathData.hpp"
#include "Font.hpp"
#include "../relive_lib/Primitives.hpp"

struct StringTable;

namespace relive
{
    struct Path_LCDScreen;
}

namespace AO {

void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

class LCDScreen final : public ::BaseGameObject
{
public:
    LCDScreen(relive::Path_LCDScreen* pTlv, const Guid& tlvId);
    ~LCDScreen();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    Prim_ScissorRect mPrimClippers[2] = {};
    FontContext mFontContext;
    AliveFont mFont;
    char_type* mActiveMessage = nullptr;
    const char_type* mMessageCutoffPtr = nullptr;
    char_type mMessageBuffer[512] = {};
    s32 mMessageId1 = 0;
    s32 mOffsetX = 0;
    s32 mCharacterWidth = 0;
    Guid mTlvId;
    s32 mShowRandomMessage = 0;
    s16 mMessageRandMinId = 0;
    s16 mMessageRandMaxId = 0;
    PSX_Point mTlvTopLeft = {};
    PSX_Point mTlvBottomRight = {};

    PalResource mPal1;
    PalResource mPal2;
};

} // namespace AO
