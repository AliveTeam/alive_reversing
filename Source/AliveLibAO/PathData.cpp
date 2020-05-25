#include "stdafx_ao.h"
#include "PathData.hpp"
#include "Function.hpp"

struct Path_TLV;
class Path;

struct SoundBlockInfo
{
    const char *field_0_vab_header_name;
    const char *field_4_vab_body_name;
    int field_8_vab_id;
    BYTE *field_C_pVabHeader;
};

struct FmvInfo
{
    const char *field_0_pName;
    unsigned __int16 field_4;
    __int16 field_6;
    __int16 field_8_stop_music;
    __int16 field_A;
    __int16 field_C_volume;
    __int16 field_E;
};

struct TlvOffsetLevelIdPathId
{
    WORD tlvOffset;
    BYTE levelId;
    BYTE pathId;
};

struct TlvOffsetCombinedLevelIdPathId
{
    WORD tlvOffset;
    WORD levelIdPathId;
};

union TlvItemInfoUnion
{
    DWORD all;
    TlvOffsetCombinedLevelIdPathId combined;
    TlvOffsetLevelIdPathId parts;
};
ALIVE_ASSERT_SIZEOF(TlvItemInfoUnion, 4);

using TPathFunctionFn = void(CC*)(Path_TLV*, Path*, TlvItemInfoUnion, __int16);

struct PathFunctionTable
{
    TPathFunctionFn object_funcs[256];
};

struct PathData
{
    void *field_0; // fn
    __int16 field_4;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E;
    __int16 field_10;
    __int16 field_12;
    int field_14_offset;
    int field_18;
    PathFunctionTable field_1C_object_funcs;
};

using TCollisionsFactory = void*; // TODO

struct CollisionInfo
{
    TCollisionsFactory field_0_fn_ptr;
    __int16 field_4_left;
    __int16 field_6_right;
    __int16 field_8_top;
    __int16 field_A_bottom;
    unsigned int field_C_collision_offset;
    unsigned int field_10_num_collision_items;
    unsigned int field_14_grid_width;
    unsigned int field_18_grid_height;
};

struct PathBlyRec
{
    const char *field_0_blyName;
    const PathData *field_4_pPathData;
    const CollisionInfo *field_8_pCollisionData;
    WORD field_C_overlay_id;
    WORD field_E;
};

struct PathRoot
{
    const PathBlyRec* field_0_pBlyArrayPtr;
    FmvInfo* field_4_pFmvArray;
    SoundBlockInfo* field_8_pMusicInfo;
    const char* field_C_bsq_file_name;
    __int16 field_10_reverb;
    __int16 field_12_bg_music_id;
    const char* field_14_lvl_name;
    __int16 field_18_num_paths;
    __int16 field_1A_unused; // message to display to change cd ??
    int field_1C;
    const char* field_20_lvl_name_cd;
    int field_24;
    const char* field_28_ovl_name_cd;
    int field_2C;
    const char* field_30_mov_name_cd;
    const char* field_34_idx_name;
    const char* field_38_bnd_name;
};

struct PathRootContainer
{
    PathRoot paths[16];
};

const PathBlyRec knullptrPathBlyRec = {};
const SoundBlockInfo knullptrSoundBlockInfo = {};
const FmvInfo knullptrFmvInfo = {};

const PathFunctionTable kObjectFactory = {};

class Collisions
{
public:

    static void CC Factory_40CEC0(Path_TLV*, Path*, TlvItemInfoUnion, __int16) {}
};

void sub_402560() {}

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

CollisionInfo stru_4D7650[] =
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
  { "R1P15.BLY", &gR1P15_4D7670[0], &stru_4D7650[0], 3, 0 },
  { "R1P16.BLY", &gR1P15_4D7670[1], &stru_4D7650[1], 3, 0 },
  { nullptr, nullptr, nullptr, 0, 0 },
  { "R1P18.BLY", &gR1P15_4D7670[2], &stru_4D7650[2], 3, 0 },
  { "R1P19.BLY", &gR1P15_4D7670[3], &stru_4D7650[3], 3, 0 },
  { "R1P20.BLY", &gR1P15_4D7670[4], &stru_4D7650[4], 3, 0 }
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
        /*
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
        */
    }
};
