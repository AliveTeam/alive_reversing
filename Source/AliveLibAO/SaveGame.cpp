#include "stdafx_ao.h"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "Elum.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "Input.hpp"
#include "SaveGame.hpp"

START_NS_AO

void SaveGame::sub_459970(SaveData*, int)
{
    NOT_IMPLEMENTED();
}

void SaveGame::sub_45A2D0(BYTE *, BYTE *, int)
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR_EXTERN(int, sGasTimer_507700);
ALIVE_VAR_EXTERN(short, sRescuedMudokons_5076C0);
ALIVE_VAR_EXTERN(short, sKilledMudokons_5076BC);

ALIVE_ARY(1, 0x505668, SaveData, 1, gSaveBuffer_505668, {});

const char word_4BC670[6][8] =
{
    { 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x00, 0x00 },
    { 0x01, 0x02, 0x03, 0x0A, 0x00, 0x00, 0x00, 0x00 },
    { 0x05, 0x07, 0x09, 0x0C, 0x0D, 0x00, 0x00, 0x00 },
    { 0x04, 0x08, 0x0B, 0x0E, 0x00, 0x00, 0x00, 0x00 },
    { 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56 }
};

const unsigned char byte_4BC450[] =
{
    0x82, 0x71, 0x82, 0x95, 0x82, 0x90, 0x82, 0x94, 0x82, 0x95,
    0x82, 0x92, 0x82, 0x85, 0x82, 0x65, 0x82, 0x81, 0x82, 0x92,
    0x82, 0x8D, 0x82, 0x93, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC478[] =
{
    0x82, 0x71, 0x82, 0x95, 0x82, 0x90, 0x82, 0x94, 0x82, 0x95,
    0x82, 0x92, 0x82, 0x85, 0x82, 0x65, 0x82, 0x81, 0x82, 0x92,
    0x82, 0x8D, 0x82, 0x93, 0x81, 0x40, 0x82, 0x68, 0x82, 0x68,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC4A0[] =
{
    0x82, 0x73, 0x82, 0x88, 0x82, 0x85, 0x81, 0x40, 0x82, 0x61,
    0x82, 0x8F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x82, 0x92,
    0x82, 0x8F, 0x82, 0x8F, 0x82, 0x8D, 0x81, 0x40, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC4C8[] =
{
    0x82, 0x6F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x81, 0x82, 0x8D,
    0x82, 0x8F, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC4F0[] =
{
    0x82, 0x6F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x81, 0x82, 0x8D,
    0x82, 0x8F, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x73, 0x82, 0x85, 0x82, 0x8D, 0x82, 0x90,
    0x82, 0x8C, 0x82, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC518[] =
{
    0x82, 0x6F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x81, 0x82, 0x8D,
    0x82, 0x8F, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x6D, 0x82, 0x85, 0x82, 0x93, 0x82, 0x94,
    0x82, 0x93, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC540[] =
{
    0x82, 0x72, 0x82, 0x83, 0x82, 0x92, 0x82, 0x81, 0x82, 0x82,
    0x82, 0x81, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC568[] =
{
    0x82, 0x72, 0x82, 0x83, 0x82, 0x92, 0x82, 0x81, 0x82, 0x82,
    0x82, 0x81, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x73, 0x82, 0x85, 0x82, 0x8D, 0x82, 0x90,
    0x82, 0x8C, 0x82, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC590[] =
{
    0x82, 0x72, 0x82, 0x83, 0x82, 0x92, 0x82, 0x81, 0x82, 0x82,
    0x82, 0x81, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x6D, 0x82, 0x85, 0x82, 0x93, 0x82, 0x94,
    0x82, 0x93, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC5B8[] =
{
    0x82, 0x72, 0x82, 0x94, 0x82, 0x8F, 0x82, 0x83, 0x82, 0x8B,
    0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40,
    0x82, 0x64, 0x82, 0x93, 0x82, 0x83, 0x82, 0x81, 0x82, 0x90,
    0x82, 0x85, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char byte_4BC5E0[] =
{
    0x82, 0x72, 0x82, 0x94, 0x82, 0x8F, 0x82, 0x83, 0x82, 0x8B,
    0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40,
    0x82, 0x64, 0x82, 0x93, 0x82, 0x83, 0x82, 0x81, 0x82, 0x90,
    0x82, 0x85, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const BYTE byte_4BC608[] =
{
    0x82, 0x72, 0x82, 0x94, 0x82, 0x8F, 0x82, 0x83, 0x82, 0x8B,
    0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40,
    0x82, 0x71, 0x82, 0x85, 0x82, 0x94, 0x82, 0x95, 0x82, 0x92,
    0x82, 0x8E, 0x81, 0x40, 0x00, 0x00
};

using byteArray = decltype(byte_4BC450);
const unsigned char* const dword_4BC62C[] =
{
    nullptr,
    byte_4BC450,
    byte_4BC5B8,
    byte_4BC4C8,
    byte_4BC4F0,
    byte_4BC5E0,
    byte_4BC608,
    nullptr,
    byte_4BC540,
    byte_4BC568,
    nullptr,
    nullptr,
    byte_4BC4A0
};

ALIVE_VAR(1, 0x4CF2B0, Save_PSX_Header, sSaveHeader2_4CF2B0, {});
ALIVE_VAR(1, 0x4BC250, Save_PSX_Header, sSaveHeader1_4BC250, {});
ALIVE_VAR(1, 0x5076B4, WORD, bUseAltSaveHeader_5076B4, 0);

void CC SaveGame::Save_459490(SaveData* pSaveData)
{
    Save_PSX_Header* pHeaderToUse = nullptr;
    if (bUseAltSaveHeader_5076B4)
    {
        pHeaderToUse = &sSaveHeader1_4BC250;
    }
    else
    {
        pHeaderToUse = &sSaveHeader2_4CF2B0;
    }

    pSaveData->field_0_header = *pHeaderToUse;

    //TODO NUKE CASTS
    sub_45A2D0(
        (BYTE *)&pSaveData->field_0_header.field_0_frame_1_name[4],
        (BYTE *) &dword_4BC62C[static_cast<int>(gMap_507BA8.field_0_current_level)],
        34
    );

    char path = 0;
    if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
    {
        short path_id = -1;
        for (short i = 0; i < 4; i++)
        {
            for(int j = 0; j < 8; j++ )
            {
                path = word_4BC670[i][j];
                if (!path)
                {
                    break;
                }
                if (gMap_507BA8.field_2_current_path == path)
                {
                    path_id = i;
                    i = 4;
                    break;
                }
            }
        }
        if (path_id != -1)
        {
            pSaveData->field_0_header.field_0_frame_1_name[44] = -127;
            pSaveData->field_0_header.field_0_frame_1_name[45] = 124;
            pSaveData->field_0_header.field_0_frame_1_name[46] = -126;
            pSaveData->field_0_header.field_0_frame_1_name[47] = static_cast<char>(path_id + 80);
        }
    }
    pSaveData->field_234_current_level = gMap_507BA8.field_0_current_level;
    pSaveData->field_206_clear_from_id = sActiveHero_507678->field_148_clear_from_id;
    pSaveData->field_20A_zone_top_left = sActiveHero_507678->field_138_zone_top_left;
    pSaveData->field_20E_zone_bottom_right = sActiveHero_507678->field_13C_zone_bottom_right;
    pSaveData->field_204_zone_number = sActiveHero_507678->field_146_zone_number;
    pSaveData->field_212_saved_level = sActiveHero_507678->field_144_saved_level;
    pSaveData->field_208_clear_to_id = sActiveHero_507678->field_14A_clear_to_id;
    pSaveData->field_216_saved_camera = sActiveHero_507678->field_140_saved_camera;
    pSaveData->field_21C_saved_ring_timer = sActiveHero_507678->field_150_saved_ring_timer;
    pSaveData->field_214_saved_path = sActiveHero_507678->field_142_saved_path;
    pSaveData->field_254_ring_pulse_timer = sActiveHero_507678->field_168_ring_pulse_timer;
    pSaveData->field_218_saved_sprite_scale = sActiveHero_507678->field_14C_saved_sprite_scale;
    pSaveData->field_2A0_rescued_mudokons = sRescuedMudokons_5076C0;
    pSaveData->field_220_bSavedHaveShrykull = sActiveHero_507678->field_154_bSavedHaveShrykull;
    pSaveData->field_2A4_restartRuptureFarmsSavedMudokons = gRestartRuptureFarmsSavedMuds_5076C8;
    pSaveData->field_258_bHaveShrykull = sActiveHero_507678->field_16C_bHaveShrykull;
    pSaveData->field_236_current_path = gMap_507BA8.field_2_current_path;
    pSaveData->field_2A2_killed_mudokons = sKilledMudokons_5076BC;
    pSaveData->field_238_current_camera = gMap_507BA8.field_4_current_camera;
    pSaveData->field_2A6_restartRuptureFarmsKilledMudokons = gRestartRuptureFarmsKilledMuds_5076C4;
    pSaveData->field_240_last_anim_frame = sActiveHero_507678->field_10_anim.field_92_current_frame;
    pSaveData->field_23E = sActiveHero_507678->field_FC_current_motion;
    pSaveData->field_224_xpos = FP_GetExponent(sActiveHero_507678->field_A8_xpos);
    pSaveData->field_228_ypos = FP_GetExponent(sActiveHero_507678->field_AC_ypos);
    if (sActiveHero_507678->field_F4_pLine)
    {
        pSaveData->field_23A_mode_mask =  sActiveHero_507678->field_F4_pLine->field_8_type;
    }
    else
    {
        pSaveData->field_23A_mode_mask = 0;
    }
    pSaveData->field_22C_ah_health = sActiveHero_507678->field_100_health;
    pSaveData->field_23C_ah_flipX = sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pSaveData->field_230_ah_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;
    pSaveData->field_244_state = sActiveHero_507678->field_110_state;
    pSaveData->field_248_gnFrame = sActiveHero_507678->field_114_gnFrame;
    pSaveData->field_24C_field_118 = sActiveHero_507678->field_118;
    pSaveData->field_250_throwable_count = sActiveHero_507678->field_19C_throwable_count;
    pSaveData->field_253_scrabania_done = sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_eBit13_bScrabinaDone);
    pSaveData->field_264 = -(gInfiniteGrenades_5076EC != 0);
    pSaveData->field_252_paramonia_done = sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit12_bParamoniaDone);
    pSaveData->field_25A_something_elum_related = gElum_507680 != 0;
    if (gElum_507680 != 0)
    {
        pSaveData->field_25C = sControlledCharacter_50767C == gElum_507680;
        pSaveData->field_25E_elum_field_144 = gElum_507680->field_144;
        pSaveData->field_28C_elum_continue_rect = gElum_507680->field_138_continue_rect;
        pSaveData->field_294_continue_zone_number = gElum_507680->field_140_continue_zone_number;
        pSaveData->field_296_elum_zone_number = gElum_507680->field_142_zone_number;
        pSaveData->field_298_elum_continue_path = gElum_507680->field_148_continue_path;
        pSaveData->field_29A_continue_level = gElum_507680->field_14A_continue_level;
        pSaveData->field_29C_elum_sprite_scale = gElum_507680->field_150_continue_sprite_scale;
        pSaveData->field_260_elum_lvl_number = gElum_507680->field_B2_lvl_number;
        pSaveData->field_262_elum_path_number = gElum_507680->field_B0_path_number;
        pSaveData->field_268_elum_xpos = FP_GetExponent(gElum_507680->field_A8_xpos);
        pSaveData->field_26C_elum_ypos = FP_GetExponent(gElum_507680->field_AC_ypos);
        if (gElum_507680->field_F4_pLine)
        {
            pSaveData->field_270 = gElum_507680->field_F4_pLine->field_8_type;
        }
        else
        {
            pSaveData->field_270 = -1;
        }
        pSaveData->field_274 = gElum_507680->field_FC_current_motion;
        pSaveData->field_272_elum_flipX = gElum_507680->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
        pSaveData->field_278_brain_idx = gElum_507680->field_128_brain_idx;
        pSaveData->field_276_elum_122 = gElum_507680->field_122;
        pSaveData->field_27C_honey_xpos = gElum_507680->field_12C_honey_xpos;
        pSaveData->field_27A_elum_brain_state = gElum_507680->field_12A_brain_state;
        pSaveData->field_284_elum_field_130 = gElum_507680->field_130;
        pSaveData->field_280_honey_ypos = gElum_507680->field_146;
        pSaveData->field_288_elumflag1 = gElum_507680->field_170_flags.Get(Elum::Flags_170::eAttackedByBees_Bit1);
        pSaveData->field_289_elumflag2 = gElum_507680->field_170_flags.Get(Elum::Flags_170::eBit2);
        pSaveData->field_28A_elumflag3 = gElum_507680->field_170_flags.Get(Elum::Flags_170::eBit3);
        pSaveData->field_28B_elumflag4 = gElum_507680->field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4);
    }
    if (sGasTimer_507700)
    {
        pSaveData->field_2A8 = (gnFrameCount_507670 - sGasTimer_507700 <= 1) ? 1 : gnFrameCount_507670 - sGasTimer_507700;
    }
    else
    {
        pSaveData->field_2A8 = 0;
    }
    pSaveData->field_2AC = bUseAltSaveHeader_5076B4;
    pSaveData->field_2AE = sCurrentControllerIndex_5076B8;
    gMap_507BA8.SaveBlyData_446900(pSaveData->field_2B0_pSaveBuffer);
    int* pTable = reinterpret_cast<int*>(&pSaveData->field_204_zone_number);
    int counter = 0;
    for (int i = 1919; i > 0; i--)
    {
        counter += *pTable;
        pTable++;
    }
    pSaveData->field_200 = counter;
}

END_NS_AO

