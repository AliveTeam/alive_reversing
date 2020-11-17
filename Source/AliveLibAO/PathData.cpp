#include "stdafx_ao.h"
#include "PathData.hpp"
#include "Function.hpp"
#include "Collisions.hpp"

namespace AO {

const PathBlyRec knullptrPathBlyRec = {};
const SoundBlockInfo knullptrSoundBlockInfo = {};
const FmvInfo knullptrFmvInfo = {};


EXPORT int sub_402560() { return 0; }

const PathData g_S1_Paths_4E4AF0[] =
{
    { sub_402560,  0,  0,  4096,  4800,  1024,  480,  1024,  480,  320,  524, kObjectFactory }
};

const CollisionInfo g_S1_Collisions_4E4AD0[] = 
{ 
    { Collisions::Factory_40CEC0, 0, 0, 4096, 4800, 320, 0, 1024, 480 }
};

PathBlyRec g_S1_Paths_4C8720[2] =
{
  { knullptrPathBlyRec },
  { "S1P1.BLY", g_S1_Paths_4E4AF0, &g_S1_Collisions_4E4AD0[0], 0, 0 }
};

FmvInfo g_R1_Fmvs_4C9A60[10] =
{
    { "", 0u, 1, 0, 0, 0, 0 },
    { "BEGIN.DDV",      3310u, 1, 1, 1, 127, 0 },
    { "GAMEBGN.DDV",    300u,  1, 1, 2, 127, 0 },
    { "R1P18P19.DDV",   60u,   1, 0, 2, 127, 0 },
    { "R1P19P18.DDV",   60u,   1, 0, 2, 127, 0 },
    { "DEMOOPEN.DDV",   500u,  1, 1, 1, 127, 0 },
    { "R1P18P1.DDV",    320u,  1, 0, 2, 127, 0 },
    { "R1P1P18.DDV",    320u,  1, 0, 2, 127, 0 },
    { "BARRELS.DDV",    349u,  1, 1, 1, 127, 0 },
    { "", 0u, 1, 0, 2, 127, 0 }
};

FmvInfo g_L1_Fmvs_4C9730[28] =
{
    {  "", 0u, 1, 0, 0, 0, 0 },
    { "$L1P6C6.DDV",  177u,  1, 0, 2, 127, 0 },
    { "E1P4C8.DDV",   45u,   1, 0, 2, 127, 0 },
    { "PARSCAR.DDV",  273u,  1, 1, 1, 127, 0 },
    { "SCRSCAR.DDV",  239u,  1, 1, 1, 127, 0 },
    { "$L1P1C3.DDV",  459u,  1, 0, 2, 127, 0 },
    { "$L1P1C4.DDV",  388u,  1, 0, 2, 127, 0 },
    { "$L1P1C9.DDV",  206u,  1, 0, 2, 127, 0 },
    { "L1P1P2.DDV",   60u,   1, 0, 2, 127, 0 },
    { "L1P2P1.DDV",   60u,   1, 0, 2, 127, 0 },
    { "L1P2P3.DDV",   90u,   1, 0, 2, 127, 0 },
    { "L1P3P2.DDV",   90u,   1, 0, 2, 127, 0 },
    { "L1P5P6.DDV",   60u,   1, 0, 2, 127, 0 },
    { "L1P6P5.DDV",   60u,   1, 0, 2, 127, 0 },
    { "MOON.DDV",     1258u, 1, 1, 1, 127, 0 },
    { "L1P1C14.DDV",  45u,   1, 0, 2, 127, 0 },
    { "ABEMORPH.DDV", 248u,  1, 1, 1, 127, 0 },
    { "L1P5C4.DDV",   45u,   1, 0, 2, 127, 0 },
    { "PARAMITE.DDV", 244u,  1, 1, 2, 127, 0 },
    { "SCRAB.DDV",    244u,  1, 1, 2, 127, 0 },
    { "LP5C3C4.DDV",  45u,   1, 0, 2, 127, 0 },
    { "LP5C4C3.DDV",  45u,   1, 0, 2, 127, 0 },
    { "LP1C3C15.DDV", 45u,   1, 0, 2, 127, 0 },
    { "LP1C15C3.DDV", 45u,   1, 0, 2, 127, 0 },
    { "LP1C3C16.DDV", 45u,   1, 0, 2, 127, 0 },
    { "LP1C16C3.DDV", 45u,   1, 0, 2, 127, 0 },
    { "L1P6E2.DDV",   45u,   1, 0, 2, 127, 0 },
    {  "", 0u, 1, 0, 2, 127, 0 }
};

FmvInfo g_F1_Fmvs_4C88E0[14] =
{
    { "", 0u, 1, 0, 0, 0, 0 },
    { "L1P6F1.DDV",   45u,  1, 0, 2, 127, 0 },
    { "F1P2P1.DDV",   45u,  1, 0, 2, 127, 0 },
    { "F1P2P5.DDV",   30u,  1, 0, 2, 127, 0 },
    { "F1P5P2.DDV",   30u,  1, 0, 2, 127, 0 },
    { "F1P5P6.DDV",   45u,  1, 0, 2, 127, 0 },
    { "F1P6P5.DDV",   45u,  1, 0, 2, 127, 0 },
    { "F1P8P9.DDV",   45u,  1, 0, 2, 127, 0 },
    { "F1P9P8.DDV",   45u,  1, 0, 2, 127, 0 },
    { "PARAMITE.DDV", 244u, 1, 1, 2, 127, 0 },
    { "F1P1C1.DDV",   45u,  1, 0, 2, 127, 0 },
    { "F1P1P2.DDV",   45u,  1, 0, 2, 127, 0 },
    { "F1P9F2.DDV",   139u, 1, 0, 2, 127, 0 },
    { "", 0u, 1, 0, 2, 127, 0 }
};

FmvInfo g_E1_Fmvs_4C8F10[6] =
{
  { "", 0u, 1, 0, 0, 0, 0 },
  { "E1P4C8.DDV",    45u,   1, 0, 2, 127, 0 },
  { "MOON.DDV",      1258u, 1, 1, 1, 127, 0 },
  { "L1P1C14.DDV",   45u,   1, 0, 2, 127, 0 },
  { "BARRELS.DDV",   349u,  1, 1, 1, 127, 0 },
  { "", 0u, 1, 0, 0, 0, 0 }
};

FmvInfo g_E2_Fmvs_4C8FE0[4] =
{
  { "", 0u, 1, 0, 0, 0, 0 },
  { "E2P2R2.DDV", 45u, 1, 0, 2, 127, 0 },
  { "L1P6E2.DDV", 45u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 }
};

FmvInfo g_D1_Fmvs_4C90E0[12] =
{
  {  "", 0u, 1, 0, 0, 0, 0 },
  { "L1P6D1.DDV", 45u,  1, 0, 2, 127, 0 },
  { "D1P1P2.DDV", 90u,  1, 0, 2, 127, 0 },
  { "D1P2P1.DDV", 90u,  1, 0, 2, 127, 0 },
  { "D1P3P4.DDV", 90u,  1, 0, 2, 127, 0 },
  { "D1P4P3.DDV", 90u,  1, 0, 2, 127, 0 },
  { "D1P6P7.DDV", 160u, 1, 0, 2, 127, 0 },
  { "D1P7P6.DDV", 160u, 1, 0, 2, 127, 0 },
  { "SCRAB.DDV",  244u, 1, 1, 2, 127, 0 },
  { "D1P1C1.DDV", 45u,  1, 0, 2, 127, 0 },
  { "D1P9D2.DDV", 135u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 }
};

FmvInfo g_D2_Fmvs_4C9270[53] =
{
  { "", 0u, 1, 0, 0, 0, 0 },
  { "D2P1P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "SCRSCAR.DDV",   239u, 1, 1, 1, 127, 0 },
  { "D2P2P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P2.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P3P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P3.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P4P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P4.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P5P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P5.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P6P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P6.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P7P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P7.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P8P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P8.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P9P10.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P9.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P10P10.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P10.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P11P10.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10D7.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D2P1P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P1B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P2P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P2B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P3P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P3B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P4P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P4B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P5P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P5B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P6P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P6B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P7P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P7B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P8P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P8B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P9P10B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P9B.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D2P10P10B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "D2P10P10B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "D2P11P10B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "D2P10P11B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "D2P10D7.DDV",   45u,  1, 0, 2, 127, 0 },
  { "D7C11C12.DDV",  45u,  1, 0, 2, 127, 0 },
  { "D7C12C11.DDV",  45u,  1, 0, 2, 127, 0 },
  { "SCRAB.DDV",     244u, 1, 1, 2, 127, 0 },
  { "D1P9D2.DDV",    135u, 1, 0, 2, 127, 0 },
  { "ABEMORPH.DDV",  248u, 1, 1, 1, 127, 0 },
  { "L1P5C4.DDV",    45u,  1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 }
};

FmvInfo g_C1_Fmvs_4C87D0[5] =
{
  { "", 0u, 1, 0, 0, 0, 0 },
  { "DRAG.DDV",    495u,  1, 1, 1, 127, 0 },
  { "GOODEND.DDV", 1218u, 1, 1, 1, 127, 0 },
  { "CREDITS.DDV", 1900u, 1, 1, 1, 127, 0 },
  { "", 0u, 1, 0, 0, 0, 0 }
};

FmvInfo g_R6_R2_Fmvs_4C9C70[21] =
{
    { "", 0u, 1, 0, 0, 0, 0 },
    { "HALTS.DDV",    280u,  1, 1, 1, 127, 0 },
    { "MOLLOCK.DDV",  225u,  1, 1, 1, 127, 0 },
    { "DRAG.DDV",     495u,  1, 1, 1, 127, 0 },
    { "BADEND.DDV",   410u,  1, 1, 1, 127, 0 },
    { "R1P18P19.DDV", 60u,   1, 0, 2, 127, 0 },
    { "R1P19P18.DDV", 60u,   1, 0, 2, 127, 0 },
    { "$R2P19C3.DDV", 340u,  1, 0, 2, 127, 0 },
    { "R1P11P6.DDV",  45u,   1, 0, 2, 127, 0 },
    { "R1P6P11.DDV",  45u,   1, 0, 2, 127, 0 },
    { "R1P18P1.DDV",  320u,  1, 1, 2, 127, 0 },
    { "R1P1P18.DDV",  320u,  1, 1, 2, 127, 0 },
    { "R1P13P14.DDV", 310u,  1, 1, 2, 127, 0 },
    { "R1P14P13.DDV", 310u,  1, 1, 2, 127, 0 },
    { "R1P13P1.DDV",  310u,  1, 1, 2, 127, 0 },
    { "R1P1P13.DDV",  310u,  1, 1, 2, 127, 0 },
    { "GOODEND.DDV",  1218u, 1, 1, 1, 127, 0 },
    { "E2P2R2.DDV",   45u,   1, 0, 2, 127, 0 },
    { "HALTS.DDV",    280u,  1, 1, 1, 127, 0 },
    { "TVCOMM.DDV",   450u,  1, 1, 1, 127, 0 },
    { "", 0u, 1, 0, 0, 0, 0 }
};

FmvInfo g_F2_Fmvs_4C8A70[53] =
{
  { "", 0u, 1, 0, 0, 0, 0 },
  { "F2P1P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "PARSCAR.DDV", 273u, 1, 1, 1, 127, 0 },
  { "F2P2P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P2.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P3P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P3.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P4P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P4.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P5P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P5.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P6P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P6.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P7P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P7.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P9P8.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P8F4.DDV",  45u,  1, 0, 2, 127, 0 },
  { "F2P1P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P1B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P2P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P2B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P3P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P3B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P4P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P4B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P5P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P5B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P6P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P6B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P7P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P7B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P9P8B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F2P8P9B.DDV", 45u,  1, 0, 2, 127, 0 },
  { "F1P9F2.DDV",  139u, 1, 0, 2, 127, 0 },
  { "F2P8F4.DDV",   45u, 1, 0, 2, 127, 0 },
  { "F4C11C13.DDV", 45u, 1, 0, 2, 127, 0 },
  { "F4C13C11.DDV", 45u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 },
  { "ABEMORPH.DDV", 248u, 1, 1, 1, 127, 0 },
  { "L1P5C4.DDV",    45u, 1, 0, 2, 127, 0 },
  { "", 0u, 1, 0, 2, 127, 0 }
};


PathData gR1P15_4D7670[] =
{
    { sub_402560,  0,  0,  12288,  1440,  1024,  480,  1024,  480,  1828,  11748, kObjectFactory },
    { sub_402560,  0,  0,  10240,  1440,  1024,  480,  1024,  480,  860,   6100,  kObjectFactory },
    { sub_402560,  0,  0,  9216,   1440,  1024,  480,  1024,  480,  856,   6468,  kObjectFactory },
    { sub_402560,  0,  0,  3072,    480,  1024,  480,  1024,  480,  104,   1032,  kObjectFactory },
    { sub_402560,  0,  0,  3072,   1440,  1024,  480,  1024,  480,  672,   3592,  kObjectFactory }
};

CollisionInfo gR1Collisions_4D7650[] =
{ 
    { Collisions::Factory_40CEC0, 0, 0, 12288, 1440, 288, 77, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 10240, 1440, 240, 31, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 9216,  1440, 216, 32, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072,   480, 24,   4, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072,  1440, 72 , 30, 1024, 480 }
};

PathBlyRec g_R1_Paths_4C9910[21] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "R1P15.BLY", &gR1P15_4D7670[0], &gR1Collisions_4D7650[0], 3, 0 },
  { "R1P16.BLY", &gR1P15_4D7670[1], &gR1Collisions_4D7650[1], 3, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "R1P18.BLY", &gR1P15_4D7670[2], &gR1Collisions_4D7650[2], 3, 0 },
  { "R1P19.BLY", &gR1P15_4D7670[3], &gR1Collisions_4D7650[3], 3, 0 },
  { "R1P20.BLY", &gR1P15_4D7670[4], &gR1Collisions_4D7650[4], 3, 0 }
};

PathData gL1Paths_4D8BB0[] =
{
    { sub_402560,  0,  0,  12288,  1440,  1024,  480,  1024,  480,  1248,  4792, kObjectFactory },
    { sub_402560,  0,  0,  8192,   1440,  1024,  480,  1024,  480,  952,   3084, kObjectFactory },
    { sub_402560,  0,  0,  3072,    960,  1024,  480,  1024,  480,  108,    580, kObjectFactory },
    { sub_402560,  0,  0,  3072,    960,  1024,  480,  1024,  480,  288,   1236, kObjectFactory },
    { sub_402560,  0,  0,  7168,    960,  1024,  480,  1024,  480,  352,   1948, kObjectFactory }
};

CollisionInfo gL1Collisions_4D8B90[] =
{ 
    { Collisions::Factory_40CEC0, 0, 0, 12288, 1440, 288, 48, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0,  8192, 1440, 192, 38, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0,  3072, 960,   48,  3, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0,  3072, 960,   48, 12, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0,  7168, 960,  112, 12, 1024, 480 }
};

PathBlyRec g_L1_Paths_4C96C0[7] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "L1P1.BLY", &gL1Paths_4D8BB0[0], &gL1Collisions_4D8B90[0], 11, 0 },
  { "L1P2.BLY", &gL1Paths_4D8BB0[1], &gL1Collisions_4D8B90[1], 11, 0 },
  { "L1P3.BLY", &gL1Paths_4D8BB0[2], &gL1Collisions_4D8B90[2], 13, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "L1P5.BLY", &gL1Paths_4D8BB0[3], &gL1Collisions_4D8B90[3], 13, 0 },
  { "L1P6.BLY", &gL1Paths_4D8BB0[4], &gL1Collisions_4D8B90[4], 13, 0 }
};

PathData gF1_Paths_4E4270[] =
{
    { sub_402560,  0,  0,  4096,  1920,  1024,  480,  360,  280,  308,  1696, kObjectFactory },
    { sub_402560,  0,  0,  6144,  1440,  1024,  480,  360,  280,  624,  2800, kObjectFactory },
    { sub_402560,  0,  0,  9216,  2400,  1024,  480,  360,  280,  960,  4316, kObjectFactory },
    { sub_402560,  0,  0,  1024,  1920,  1024,  480,  360,  280,  432,  1472, kObjectFactory },
    { sub_402560,  0,  0,  1024,  960,   1024,  480,  360,  280,  316,  1056, kObjectFactory },
    { sub_402560,  0,  0,  4096,  1440,  1024,  480,  360,  280,  476,  1968, kObjectFactory },
    { sub_402560,  0,  0,  2048,  480,   1024,  480,  360,  280,  116,  1172, kObjectFactory }
};

CollisionInfo gF1_Collisions_4E4250[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 4096, 1920, 128,  9, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 6144, 1440, 144, 24, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 9216, 2400, 360, 30, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 1024, 1920, 32,  20, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 1024,  960, 16,  15, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 4096, 1440, 96,  19, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 2048,  480, 16,  5,  1024, 480 }
};

PathBlyRec g_F1_Paths_4C8840[10] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "F1P1.BLY", &gF1_Paths_4E4270[0], &gF1_Collisions_4E4250[0], 15, 0 },
  { "F1P2.BLY", &gF1_Paths_4E4270[1], &gF1_Collisions_4E4250[1], 15, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "F1P5.BLY", &gF1_Paths_4E4270[2], &gF1_Collisions_4E4250[2], 15, 0 },
  { "F1P6.BLY", &gF1_Paths_4E4270[3], &gF1_Collisions_4E4250[3], 15, 0 },
  { "F1P7.BLY", &gF1_Paths_4E4270[4], &gF1_Collisions_4E4250[4], 15, 0 },
  { "F1P8.BLY", &gF1_Paths_4E4270[5], &gF1_Collisions_4E4250[5], 15, 0 },
  { "F1P9.BLY", &gF1_Paths_4E4270[6], &gF1_Collisions_4E4250[6], 15, 0 }
};

PathData gF2_Paths_4E24B0[] =
{
    { sub_402560,  0,  0,  5120,  1920,  1024,  480,  360,  280,  1440,  3276, kObjectFactory },
    { sub_402560,  0,  0,  9216,  2400,  1024,  480,  360,  280,  1800,  5472, kObjectFactory },
    { sub_402560,  0,  0,  3072,   960,  1024,  480,  360,  280,  968,   2316, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1920,  1024,  480,  360,  280,  820,   2508, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1440,  1024,  480,  360,  280,  400,   1476, kObjectFactory },
    { sub_402560,  0,  0,  2048,  2880,  1024,  480,  360,  280,  676,   2928, kObjectFactory },
    { sub_402560,  0,  0,  5120,   480,  1024,  480,  360,  280,  720,   2364, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1440,  1024,  480,  1024, 480,  260,   1712, kObjectFactory }
};

CollisionInfo gF2_Collisions_4E2490[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1920, 160, 64, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 9216, 2400, 360, 72, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072, 960,   48, 46, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1920, 160, 33, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1440, 120, 14, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 2048, 2880,  96, 29, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120,  480,  40, 34, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1440, 120,  7, 1024, 480 }
};

PathBlyRec g_F2_Paths_4C89E0[9] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "F2P1.BLY", &gF2_Paths_4E24B0[0], &gF2_Collisions_4E2490[0], 18, 0 },
  { "F2P2.BLY", &gF2_Paths_4E24B0[1], &gF2_Collisions_4E2490[1], 20, 0 },
  { "F2P3.BLY", &gF2_Paths_4E24B0[2], &gF2_Collisions_4E2490[2], 18, 0 },
  { "F2P4.BLY", &gF2_Paths_4E24B0[3], &gF2_Collisions_4E2490[3], 20, 0 },
  { "F2P5.BLY", &gF2_Paths_4E24B0[4], &gF2_Collisions_4E2490[4], 20, 0 },
  { "F2P6.BLY", &gF2_Paths_4E24B0[5], &gF2_Collisions_4E2490[5], 20, 0 },
  { "F2P7.BLY", &gF2_Paths_4E24B0[6], &gF2_Collisions_4E2490[6], 18, 0 },
  { "F2P8.BLY", &gF2_Paths_4E24B0[7], &gF2_Collisions_4E2490[7], 20, 0 }
};

PathData gE1_Paths_4DFE70[] =
{
    { sub_402560,  0,  0,  6144,  1440,  1024,  480,   360,  280,  1424,  5212, kObjectFactory },
    { sub_402560,  0,  0,  6144,   960,  1024,  480,  1024,  480,  2696,  6760, kObjectFactory },
    { sub_402560,  0,  0,  5120,   960,  1024,  480,  1024,  480,  2800,  6576, kObjectFactory },
    { sub_402560,  0,  0,  7168,  1440,  1024,  480,  1024,  480,  2048,  5428, kObjectFactory },
    { sub_402560,  0,  0,  6144,  1440,  1024,  480,  1024,  480,  1704,  7092, kObjectFactory }
};

CollisionInfo gE1_Collisions_4DFE50[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 6144, 1440, 144, 64, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 6144,  960, 96, 130, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120,  960, 80, 136, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 7168, 1440, 168, 94, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 6144, 1440, 144, 78, 1024, 480 }
};

PathBlyRec g_E1_Paths_4C8EA0[7] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "E1P1.BLY", &gE1_Paths_4DFE70[0], &gE1_Collisions_4DFE50[0], 6, 0 },
  { "E1P2.BLY", &gE1_Paths_4DFE70[1], &gE1_Collisions_4DFE50[1], 6, 0 },
  { "E1P3.BLY", &gE1_Paths_4DFE70[2], &gE1_Collisions_4DFE50[2], 6, 0 },
  { "E1P4.BLY", &gE1_Paths_4DFE70[3], &gE1_Collisions_4DFE50[3], 9, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "E1P6.BLY", &gE1_Paths_4DFE70[4], &gE1_Collisions_4DFE50[4], 8, 0 }
};

PathData gE2_Paths_4DE930[] =
{
    { sub_402560,  0,  0,  3072,  960,  1024,  480,  1024,  480,  328,  2028, kObjectFactory },
    { sub_402560,  0,  0,  5120,  960,  1024,  480,  1024,  480,  760,  2420, kObjectFactory },
    { sub_402560,  0,  0,  5120,  960,  1024,  480,  1024,  480,  1220, 3096, kObjectFactory }
};

CollisionInfo gE2_Collisions_4DE910[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 3072, 960, 48, 14, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 960, 80, 34, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 960, 80, 57, 1024, 480 }
};

PathBlyRec g_E2_Paths_4C8F90[5] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "E2P2.BLY", &gE2_Paths_4DE930[0], &gE2_Collisions_4DE910[0], 36, 0 },
  { "E2P3.BLY", &gE2_Paths_4DE930[1], &gE2_Collisions_4DE910[1], 36, 0 },
  { "E2P4.BLY", &gE2_Paths_4DE930[2], &gE2_Collisions_4DE910[2], 36, 0 }
};

PathData gD1_Paths_4DDC70[] =
{
    { sub_402560,  0,  0,  5120,  1920,  1024,  480,  360,  280,  620,  2704, kObjectFactory },
    { sub_402560,  0,  0,  3072,  1920,  1024,  480,  360,  280,  896,  2564, kObjectFactory },
    { sub_402560,  0,  0,  4096,   480,  1024,  480,  360,  280,  752,  2200, kObjectFactory },
    { sub_402560,  0,  0,  8192,  1920,  1024,  480,  360,  280,  1036, 3000, kObjectFactory },
    { sub_402560,  0,  0,  7168,  1920,  1024,  480,  360,  280,  1804, 4628, kObjectFactory },
    { sub_402560,  0,  0,  9216,  2400,  1024,  480,  360,  280,  2500, 5328, kObjectFactory },
    { sub_402560,  0,  0,  6144,  480,   1024,  480,  360,  280,  588,  2448, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1440,  1024,  480,  360,  280,  940,  2696, kObjectFactory }
};

CollisionInfo gD1_Paths_4DDC50[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1920, 160,  23, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072, 1920, 96,   40, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 4096, 480,  32,   36, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 8192, 1920, 256,  39, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 7168, 1920, 224,  79, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 9216, 2400, 360, 107, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 6144, 480,  48,   27, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1440, 120,  41, 1024, 480 }
};

PathBlyRec g_D1_Paths_4C9040[10] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "D1P1.BLY", &gD1_Paths_4DDC70[0], &gD1_Paths_4DDC50[0], 24, 0 },
  { "D1P2.BLY", &gD1_Paths_4DDC70[1], &gD1_Paths_4DDC50[1], 24, 0 },
  { "D1P3.BLY", &gD1_Paths_4DDC70[2], &gD1_Paths_4DDC50[2], 24, 0 },
  { "D1P4.BLY", &gD1_Paths_4DDC70[3], &gD1_Paths_4DDC50[3], 24, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "D1P6.BLY", &gD1_Paths_4DDC70[4], &gD1_Paths_4DDC50[4], 24, 0 },
  { "D1P7.BLY", &gD1_Paths_4DDC70[5], &gD1_Paths_4DDC50[5], 24, 0 },
  { "D1P8.BLY", &gD1_Paths_4DDC70[6], &gD1_Paths_4DDC50[6], 24, 0 },
  { "D1P9.BLY", &gD1_Paths_4DDC70[7], &gD1_Paths_4DDC50[7], 26, 0 }
};

PathData gD2_Paths_4DBA70[] =
{
    { sub_402560,  0,  0,  9216,  480,   1024,  480,  360,  280, 2192,  5620, kObjectFactory },
    { sub_402560,  0,  0,  3072,  1440,  1024,  480,  360,  280,  932,  2388, kObjectFactory },
    { sub_402560,  0,  0,  4096,  1440,  1024,  480,  360,  280,  1236, 3308, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1440,  1024,  480,  360,  280,  620,  2648, kObjectFactory },
    { sub_402560,  0,  0,  7168,  1440,  1024,  480,  360,  280,  1548, 3296, kObjectFactory },
    { sub_402560,  0,  0,  8192,  1440,  1024,  480,  360,  280,  1512, 4224, kObjectFactory },
    { sub_402560,  0,  0,  5120,   960,  1024,  480,  360,  280,  480,  1792, kObjectFactory },
    { sub_402560,  0,  0,  8192,   960,  1024,  480,  360,  280,  968,  4000, kObjectFactory },
    { sub_402560,  0,  0,  2048,  1440,  1024,  480,  360,  280,  648,  2392, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1440,  1024,  480,  360,  280,  300,  2104, kObjectFactory }
};

CollisionInfo gD2_Collisions_4DBA50[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 9216,  480,  72, 106, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072, 1440,  72,  43, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 4096, 1440,  96,  57, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1440, 120,  25, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 7168, 1440, 168,  69, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 8192, 1440, 192,  66, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120,  960,  80,  20, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 8192,  960, 128,  42, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 2048, 1440,  48,  30, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120, 1440, 120  , 9, 1024, 480 }
};

PathBlyRec g_D2_Paths_4C91C0[11] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "D2P1.BLY",  &gD2_Paths_4DBA70[0], &gD2_Collisions_4DBA50[0], 32, 0 },
  { "D2P2.BLY",  &gD2_Paths_4DBA70[1], &gD2_Collisions_4DBA50[1], 30, 0 },
  { "D2P3.BLY",  &gD2_Paths_4DBA70[2], &gD2_Collisions_4DBA50[2], 27, 0 },
  { "D2P4.BLY",  &gD2_Paths_4DBA70[3], &gD2_Collisions_4DBA50[3], 30, 0 },
  { "D2P5.BLY",  &gD2_Paths_4DBA70[4], &gD2_Collisions_4DBA50[4], 30, 0 },
  { "D2P6.BLY",  &gD2_Paths_4DBA70[5], &gD2_Collisions_4DBA50[5], 30, 0 },
  { "D2P7.BLY",  &gD2_Paths_4DBA70[6], &gD2_Collisions_4DBA50[6], 30, 0 },
  { "D2P8.BLY",  &gD2_Paths_4DBA70[7], &gD2_Collisions_4DBA50[7], 30, 0 },
  { "D2P9.BLY",  &gD2_Paths_4DBA70[8], &gD2_Collisions_4DBA50[8], 30, 0 },
  { "D2P10.BLY", &gD2_Paths_4DBA70[9], &gD2_Collisions_4DBA50[9], 30, 0 }
};

PathData gC1_Paths_4E46B0[] =
{
    { sub_402560,  0,  0,  10240,  4800,  1024,  480,  1024,  480,  800,  824, kObjectFactory }
};

CollisionInfo gC1_Collisions_4E4690[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 10240, 4800, 800, 0, 1024, 480 }
};

PathBlyRec g_C1_Paths_4C87B0[2] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "C1P1.BLY", &gC1_Paths_4E46B0[0], &gC1_Collisions_4E4690[0], 0, 0 }
};

PathData gR6_Paths_4D14B0[] =
{
    { sub_402560,  0,  0,  3072,  2880,  1024,  480,  1024,  480,  964,  6052, kObjectFactory }
};

CollisionInfo gR6_Collsions_4D1490[] =
{
    { &Collisions::Factory_40CEC0, 0, 0, 3072, 2880, 144, 41, 1024, 480 }
};

PathBlyRec g_R6_Paths_4C9DE0[7] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "R6P6.BLY", &gR6_Paths_4D14B0[0], &gR6_Collsions_4D1490[0], 49, 0 }
};

PathData gR2_Paths_4D6130[] =
{
    { sub_402560,  0,  0,  6144,  960,  1024,  480,  1024,  480,   496,  2800, kObjectFactory },
    { sub_402560,  0,  0,  7168,  960,  1024,  480,  1024,  480,  1252,  4524, kObjectFactory },
    { sub_402560,  0,  0,  4096,  960,  1024,  480,  1024,  480,  344,   2480, kObjectFactory },
    { sub_402560,  0,  0,  7168,  960,  1024,  480,  1024,  480,  512,   5292, kObjectFactory },
    { sub_402560,  0,  0,  3072,  960,  1024,  480,  1024,  480,  568,   2580, kObjectFactory },
    { sub_402560,  0,  0,  6144,  1440, 1024,  480,  1024,  480,  1424,  6556, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1440, 1024,  480,  1024,  480,  440,   3004, kObjectFactory },
    { sub_402560,  0,  0,  6144,  960,  1024,  480,  1024,  480,  356,   3000, kObjectFactory },
    { sub_402560,  0,  0,  5120,  1440, 1024,  480,  1024,  480,  480,   2940, kObjectFactory },
    { sub_402560,  0,  0,  5120,  480,  1024,  480,  1024,  480,  360,   3080, kObjectFactory },
    { sub_402560,  0,  0,  3072,  480,  1024,  480,  1024,  480,  284,   2484, kObjectFactory },
    { sub_402560,  0,  0,  3072,  1920, 1024,  480,  1024,  480,  616,   3512, kObjectFactory },
    { sub_402560,  0,  0,  3072,  960,  1024,  480,  1024,  480,  428,   2936, kObjectFactory },
    { sub_402560,  0,  0,  9216,  1440, 1024,  480,  1024,  480, 1756,  10644, kObjectFactory },
    { sub_402560,  0,  0,  10240, 1920, 1024,  480,  1024,  480, 1040,   7764, kObjectFactory },
    { sub_402560,  0,  0,  10240, 1920, 1024,  480,  1024,  480, 1040,   6440, kObjectFactory },
    { sub_402560,  0,  0,  3072,  1440, 1024,  480,  1024,  480,  152,   1200, kObjectFactory },
    { sub_402560,  0,  0,  3072,  1440, 1024,  480,  1024,  480,  312,   1436, kObjectFactory }
};

CollisionInfo gR2_Collisions_4D6110[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 6144,   960,   96, 20, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 7168,   960,  112, 57, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 4096,   960,   64, 14, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 7168,   960,  112, 20, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072,   960,   48, 26, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 6144,  1440,  144, 64, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120,  1440,  120, 16, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 6144,   960,   96, 13, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120,  1440,  120, 18, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 5120,   480,   40, 16, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072,   480,   24, 13, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072,  1920,   96, 26, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 3072,   960,   48, 19, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 9216,  1440,  216, 77, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 10240, 1920,  320, 36, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0, 10240, 1920,  320, 36, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0,  3072, 1440,   72,  4, 1024, 480 },
    { Collisions::Factory_40CEC0, 0, 0,  3072, 1440,   72, 12, 1024, 480 }
};

PathBlyRec g_R2_Paths_4C9B20[21] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { "R2P1.BLY",  &gR2_Paths_4D6130[0],  &gR2_Collisions_4D6110[0],  42, 0 },
  { "R2P2.BLY",  &gR2_Paths_4D6130[1],  &gR2_Collisions_4D6110[1],  43, 0 },
  { "R2P3.BLY",  &gR2_Paths_4D6130[2],  &gR2_Collisions_4D6110[2],  43, 0 },
  { "R2P4.BLY",  &gR2_Paths_4D6130[3],  &gR2_Collisions_4D6110[3],  43, 0 },
  { "R2P5.BLY",  &gR2_Paths_4D6130[4],  &gR2_Collisions_4D6110[4],  43, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },                   
  { "R2P7.BLY",  &gR2_Paths_4D6130[5],  &gR2_Collisions_4D6110[5],  43, 0 },
  { "R2P8.BLY",  &gR2_Paths_4D6130[6],  &gR2_Collisions_4D6110[6],  45, 0 },
  { "R2P9.BLY",  &gR2_Paths_4D6130[7],  &gR2_Collisions_4D6110[7],  41, 0 },
  { "R2P10.BLY", &gR2_Paths_4D6130[8],  &gR2_Collisions_4D6110[8],  41, 0 },
  { "R2P11.BLY", &gR2_Paths_4D6130[9],  &gR2_Collisions_4D6110[9],  46, 0 },
  { "R2P12.BLY", &gR2_Paths_4D6130[10], &gR2_Collisions_4D6110[10], 43, 0 },
  { "R2P13.BLY", &gR2_Paths_4D6130[11], &gR2_Collisions_4D6110[11], 42, 0 },
  { "R2P14.BLY", &gR2_Paths_4D6130[12], &gR2_Collisions_4D6110[12], 42, 0 },
  { "R2P15.BLY", &gR2_Paths_4D6130[13], &gR2_Collisions_4D6110[13], 39, 0 },
  { "R2P16.BLY", &gR2_Paths_4D6130[14], &gR2_Collisions_4D6110[14], 47, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "R2P18.BLY", &gR2_Paths_4D6130[15], &gR2_Collisions_4D6110[15], 44, 0 },
  { "R2P19.BLY", &gR2_Paths_4D6130[16], &gR2_Collisions_4D6110[16], 43, 0 },
  { "R2P20.BLY", &gR2_Paths_4D6130[17], &gR2_Collisions_4D6110[17], 43, 0 }
};

PathData gF2_Paths_4E02B0[] =
{
    { sub_402560,  0,  0,  5120,  2880,  1024,  480,  1024,  480,  2300,  7092, kObjectFactory }
};

CollisionInfo gF2_Collisions_4E0290[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 5120, 2880, 240, 103, 1024, 480 }
};

PathBlyRec g_F2_Paths_4C8DE0[10] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "F4P9.BLY", &gF2_Paths_4E02B0[0], &gF2_Collisions_4E0290[0], 22, 0 }
};

PathData gD2_Paths_4D8FF0[] =
{
    { sub_402560,  0,  0,  8192,  2400,  1024,  480,  1024,  480,  2120,  7216, kObjectFactory }
};

CollisionInfo gD2_Collisions_4D8FD0[] =
{
    { Collisions::Factory_40CEC0, 0, 0, 8192, 2400, 320, 90, 1024, 480 }
};

PathBlyRec g_D2_Paths_4C95E0[12] =
{
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "D7P11.BLY", &gD2_Paths_4D8FF0[0], &gD2_Collisions_4D8FD0[0], 34, 0 }
};

FmvInfo g_S1_Fmvs_4C8740[5] =
{
    knullptrFmvInfo,
    { "LOGOINT.DDV", 579u, 0, 1, 1, 127, 0 },
    { "XAOSOPEN.DDV", 45u, 1, 1, 2, 127, 0 },
    { "GTDDLOGO.DDV", 210u, 0, 1, 1, 127, 0 },
    { "PROPHECY.DDV", 890u, 1, 1, 2, 127, 0 }
};

SoundBlockInfo g_S1_SoundBlock_4C8790[2] =
{ 
    { "OPTSNDFX.VH", "OPTSNDFX.VB", -1, nullptr }, 
    knullptrSoundBlockInfo
};

SoundBlockInfo g_R1_SoundBlock_4C9B00[2] =
{ 
    { "RFSNDFX.VH", "RFSNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_L1_SoundBlock_4C98F0[2] = 
{ 
    { "MLSNDFX.VH", "MLSNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_F1_SoundBlock_4C89C0[] =
{
    { "F1SNDFX.VH", "F1SNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_F2_SoundBlock_4C8DC0[] =
{
    { "F2SNDFX.VH", "F2SNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_E1_SoundBlock_4C8F70[] =
{
    { "E1SNDFX.VH", "E1SNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_E2_SoundBlock_4C9020[] =
{
    { "E2SNDFX.VH", "E2SNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_D1_SoundBlock_4C91A0[] =
{ 
    { "D1SNDFX.VH", "D1SNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_D2_SoundBlock_4C95C0[] =
{
    { "D2SNDFX.VH", "D2SNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_C1_SoundBlock_4C8820[] =
{
    { "OPTSNDFX.VH", "OPTSNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_R6_SoundBlock_4C9E50[] =
{
    { "RFENDER.VH", "RFENDER.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_R2_SoundBlock_4C9DC0[] =
{
    { "RFSNDFX.VH", "RFSNDFX.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_F2_SoundBlock_4C8E80[] =
{
    { "F2ENDER.VH", "F2ENDER.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

SoundBlockInfo g_D2_SoundBlock_4C96A0[] =
{
    { "D2ENDER.VH", "D2ENDER.VB", -1, nullptr },
    knullptrSoundBlockInfo
};

PathRootContainer gMapData_4CAB58 =
{
    {
        { g_S1_Paths_4C8720, g_S1_Fmvs_4C8740,    g_S1_SoundBlock_4C8790, "S1SEQ.BSQ", 16, 9, "S1",  1, 0,  1, "S1.LVL;1",    0, "S1.OVL;1",   2,  "S1.MOV;1",   "S1.IDX", "S1PATH.BND" },
        { g_R1_Paths_4C9910, g_R1_Fmvs_4C9A60,    g_R1_SoundBlock_4C9B00, "R1SEQ.BSQ", 16, 8, "R1", 20, 0,  4, "R1.LVL;1",    3, "R1.OVL;1",   5,  "R1.MOV;1",   "R1.IDX", "R1PATH.BND" },
        { g_L1_Paths_4C96C0, g_L1_Fmvs_4C9730,    g_L1_SoundBlock_4C98F0, "L1SEQ.BSQ", 23, 7, "L1", 6,  0, 12, "L1.LVL;1",   11, "L1.OVL;1",   14, "L1.MOV;1",   "L1.IDX", "L1PATH.BND" },
        { g_F1_Paths_4C8840, g_F1_Fmvs_4C88E0,    g_F1_SoundBlock_4C89C0, "F1SEQ.BSQ", 16, 5, "F1", 9,  0, 16, "F1.LVL;1",   15, "F1.OVL;1",   17, "F1.MOV;1",   "F1.IDX", "F1PATH.BND" },
        { g_F2_Paths_4C89E0, g_F2_Fmvs_4C8A70,    g_F2_SoundBlock_4C8DC0, "F2SEQ.BSQ", 23, 6, "F2", 8,  0, 19, "F2.LVL;1",   18, "F2.OVL;1",   21, "F2.MOV;1",   "F2.IDX", "F2PATH.BND" },
        { g_E1_Paths_4C8EA0, g_E1_Fmvs_4C8F10,    g_E1_SoundBlock_4C8F70, "E1SEQ.BSQ", 16, 2, "E1", 6,  0,  7, "E1.LVL;1",    6, "E1.OVL;1",   10, "E1.MOV;1",   "E1.IDX", "E1PATH.BND" },
        { g_E2_Paths_4C8F90, g_E2_Fmvs_4C8FE0,    g_E2_SoundBlock_4C9020, "E2SEQ.BSQ", 23, 3, "E2", 4,  0, 37, "E2.LVL;1",   36, "E2.OVL;1",   38, "E2.MOV;1",   "E2.IDX", "E2PATH.BND" },
        {  nullptr,    nullptr,    nullptr,    nullptr,       16,    5,    nullptr,    0,    0,    0,    nullptr,    0,    nullptr,    0,    nullptr,    nullptr,    nullptr  },
        { g_D1_Paths_4C9040, g_D1_Fmvs_4C90E0,    g_D1_SoundBlock_4C91A0, "D1SEQ.BSQ", 16, 0, "D1", 9,  0, 25, "D1.LVL;1",   24, "D1.OVL;1",   29, "D1.MOV;1",   "D1.IDX", "D1PATH.BND" },
        { g_D2_Paths_4C91C0, g_D2_Fmvs_4C9270,    g_D2_SoundBlock_4C95C0, "D2SEQ.BSQ", 23, 1, "D2", 10, 0, 31, "D2.LVL;1",   30, "D2.OVL;1",   33, "D2.MOV;1",   "D2.IDX", "D2PATH.BND" },
        { g_C1_Paths_4C87B0, g_C1_Fmvs_4C87D0,    g_C1_SoundBlock_4C8820, "C1SEQ.BSQ", 16, 9, "C1", 1,  0, 52, "\\C1.LVL;1",  0, "\\S1.OVL;1", 53, "\\C1.MOV;1", "C1.IDX", "C1PATH.BND" },
        { nullptr,    nullptr,    nullptr,    nullptr,    16,    5,    nullptr,    0,    0,    0,    nullptr,    0,    nullptr,    0,    nullptr,    nullptr,    nullptr },
        { g_R6_Paths_4C9DE0, g_R6_R2_Fmvs_4C9C70, g_R6_SoundBlock_4C9E50, "R6SEQ.BSQ", 16, 8, "R6", 6,  0, 50, "\\R6.LVL;1", 49, "\\R6.OVL;1", 48, "\\R2.MOV;1", "R2.IDX", "R6PATH.BND" },
        { g_R2_Paths_4C9B20, g_R6_R2_Fmvs_4C9C70, g_R2_SoundBlock_4C9DC0, "R2SEQ.BSQ", 16, 8, "R2", 20, 0, 40, "R2.LVL;1",   39, "R2.OVL;1",   48, "R2.MOV;1",   "R2.IDX", "R2PATH.BND" },
        { g_F2_Paths_4C8DE0, g_F2_Fmvs_4C8A70,    g_F2_SoundBlock_4C8E80, "F2SEQ.BSQ", 23, 6, "F4",  9, 0, 23, "\\F4.LVL;1", 22, "\\F4.OVL;1", 21, "\\F2.MOV;1", "F2.IDX", "F4PATH.BND" },
        { g_D2_Paths_4C95E0, g_D2_Fmvs_4C9270,    g_D2_SoundBlock_4C96A0, "D2SEQ.BSQ", 23, 1, "D7", 11, 0, 35, "\\D7.LVL;1", 34, "\\D7.OVL;1", 33, "\\D2.MOV;1", "D2.IDX", "D7PATH.BND" }
    }
};

const PathBlyRec* CC Path_Get_Bly_Record_434650(LevelIds level, unsigned __int16 path)
{
    return &gMapData_4CAB58.paths[static_cast<int>(level)].field_0_pBlyArrayPtr[path];
}

FmvInfo* CC Path_Get_FMV_Record_434680(LevelIds levelId, unsigned __int16 fmvId)
{
    return &gMapData_4CAB58.paths[static_cast<int>(levelId)].field_4_pFmvArray[fmvId];
}

int CC Path_Format_CameraName_4346B0(char* pNameBuffer, LevelIds level, __int16 path, __int16 camera)
{
    return sprintf(pNameBuffer, "%sP%02dC%02d.CAM", gMapData_4CAB58.paths[static_cast<int>(level)].field_14_lvl_name, path, camera);
}

}
