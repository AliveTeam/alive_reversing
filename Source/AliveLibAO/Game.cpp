#include "stdafx_ao.h"
#include "Game.hpp"
#include "logger.hpp"
#include "Function.hpp"
#include <set>
#include "../AliveLibAE/FixedPoint.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507670, unsigned int, gnFrameCount_507670, 0);
ALIVE_VAR(1, 0x504618, DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618, nullptr);

void Game_ForceLink()
{

}

EXPORT void SYS_EventsPump_44FF90()
{
    NOT_IMPLEMENTED();
}


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

