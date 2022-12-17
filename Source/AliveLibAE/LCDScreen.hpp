#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "Font.hpp"
#include "PathData.hpp"
#include "../relive_lib/Primitives.hpp"

struct StringTable;

namespace relive
{
    struct Path_LCDScreen;
}

void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId);

class LCDScreen final : public BaseGameObject
{
public:
    LCDScreen(relive::Path_LCDScreen* pTlv, const Guid& tlvId);
    ~LCDScreen();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

private:
    Prim_PrimClipper mPrimClippers[2][2] = {};
    FontContext mFontContext;
    AliveFont mFont = {};
    const char_type* mActiveMessage = nullptr;
    const char_type* mMessageCutoffPtr = nullptr;
    char_type mMessageBuffer[512] = {};
    bool mPlayLetterSound = false;
    s16 mMessageId1 = 0;
    s16 mOffsetX = 0;
    u16 mCharacterWidth = 0;
    u16 mMessageId2 = 0;
    u16 mToggleMessageSwitchId = 0;
    bool mShowRandomMessage = false;
    u16 mMessageRandMinId = 0;
    u16 mMessageRandMaxId = 0;
    Guid mTlvId;
    PSX_Point mTlvTopLeft = {};
    PSX_Point mTlvBottomRight = {};

    PalResource mPal1;
    PalResource mPal2;
};
