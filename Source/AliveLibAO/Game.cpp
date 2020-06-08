#include "stdafx_ao.h"
#include "Game.hpp"
#include "logger.hpp"
#include "Function.hpp"
#include <set>
#include "../AliveLibAE/FixedPoint.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507670, int, gnFrameCount_507670, 0);
ALIVE_VAR(1, 0x504618, DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618, nullptr);

void Game_ForceLink()
{

}



struct Animation
{

    virtual void VDummy() {}

    __int16 field_4_flags;
    __int16 field_6;
    int field_8;
    __int16 field_C_layer;
    __int16 field_E;
    int field_10;
    int field_14;
    int field_18;
    int field_1C_fn_ptrs;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    __int16 field_84;
    __int16 field_86;
    __int16 field_88;
    __int16 field_8A;
    __int16 field_8C_vram_x_pal;
    __int16 field_8E_vram_y_pal;
    __int16 field_90_pal_depth;
    __int16 field_92;
    int field_94;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);

class BaseAnimatedObj : public AO::BaseGameObject
{
public:
    Animation field_10_anim;
    FP field_A8_xpos;
    FP field_AC_ypos;
    __int16 field_B0;
    __int16 field_B2;
    int field_B4;
    int field_B8;
    int field_BC_scale;
    __int16 field_C0_r;
    __int16 field_C2_g;
    __int16 field_C4_b;
    __int16 field_C6;
    __int16 field_C8;
    __int16 field_CA;
    char field_CC;
    char field_CD;
    char field_CE;
    char field_CF;
    int field_D0_res;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedObj, 0xD4);




ALIVE_VAR(1, 0xAC6420, int*, psxemu_videomem_AC6420, nullptr);

class Bmp
{
public:
    // 0x00ac6420
    // 8
    // 16
    // 0x00ac6420 fff7
    // 0x00ac6420 1
    // str
    EXPORT static int CC DrawString_48FB90(int* /*pBmp*/, int /*x*/, int /*y*/, int /*colour*/, int /*backColour*/, const char* /*lpString*/)
    {
        NOT_IMPLEMENTED();
        return 0;
    }
};

char sStatsSignFontPalette_4CD570[32] =
{
  '\0',
  '\0',
  '\x01',
  '€',
  '\x01',
  '„',
  ' ',
  '„',
  '!',
  '€',
  ' ',
  '„',
  '!',
  '„',
  'e',
  'Î',
  'e',
  'Œ',
  'Œ',
  '±',
  '\x13',
  '”',
  'd',
  'Î',
  'e',
  'Î',
  '×',
  '˜',
  '\x14',
  '¡',
  '\x18',
  'Ø'
};


// TODO: Wrong values
 BYTE pal_4D0090[32] = {
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x14, 0xE7, 0x1C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E };

ALIVE_VAR(1, 0x508BF4, BYTE, byte_508BF4, 0);

END_NS_AO

