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
    AliveFont mFont1 = {};
    AliveFont field_58_font2 = {};
    AliveFont field_90_font3 = {};
    AliveFont field_C8_font4 = {};
    Guid field_100_objectId;
    s16 field_104_position_x = 0;
    s16 field_106_position_y = 0;
    s16 field_108_is_hidden = 0;
    s16 field_10A_muds_left_in_area = 0;
};

extern s16 sMudokonsInArea_5C1BC4;
extern s8 sZulagNumber_5C1A20;
