#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "Font.hpp"

struct Path_LCDStatusBoard final : public Path_TLV
{
    s16 field_10_number_of_muds;
    s16 field_12_zulag_number;
    Choice_int field_14_hidden;
};
ALIVE_ASSERT_SIZEOF(Path_LCDStatusBoard, 0x18);

class LCDStatusBoard final : public BaseGameObject
{
public:
    LCDStatusBoard(Path_LCDStatusBoard* params, TlvItemInfoUnion a3);
    ~LCDStatusBoard();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

public:
    Alive::Font field_20_font1;
    Alive::Font field_58_font2;
    Alive::Font field_90_font3;
    Alive::Font field_C8_font4;
    s32 field_100_objectId;
    s16 field_104_position_x;
    s16 field_106_position_y;
    s16 field_108_is_hidden;
    s16 field_10A_muds_left_in_area;
};
ALIVE_ASSERT_SIZEOF(LCDStatusBoard, 0x10C);

ALIVE_VAR_EXTERN(s16, sMudokonsInArea_5C1BC4);
ALIVE_VAR_EXTERN(s8, sZulagNumber_5C1A20);
