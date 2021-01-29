#pragma once

#include "FunctionFwd.hpp"
#include "PathData.hpp"
#include "Psx.hpp"

namespace AO {

struct Save_PSX_Header
{
    unsigned char field_0_frame_1_name[128];
    char field_80_frame_2_padding[128];
    char field_100_frame_3_padding[128];
    char field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF(Save_PSX_Header, 0x200);

struct SaveData
{
    Save_PSX_Header field_0_header;
    int field_200_hashValue;
    short field_204_zone_number;
    short field_206_clear_from_id;
    short field_208_clear_to_id;
    PSX_Point field_20A_zone_top_left;
    PSX_Point field_20E_zone_bottom_right;
    LevelIds field_212_saved_level;
    short field_214_saved_path;
    short field_216_saved_camera;
    FP field_218_saved_sprite_scale;
    int field_21C_saved_ring_timer;
    short field_220_bSavedHaveShrykull;
    short field_222_pad;
    int field_224_xpos;
    DWORD field_228_ypos;
    FP field_22C_ah_health;
    FP field_230_ah_sprite_scale;
    LevelIds field_234_current_level;
    unsigned short field_236_current_path;
    short field_238_current_camera;
    short field_23A_mode_mask;
    short field_23C_ah_flipX;
    short field_23E_current_motion;
    WORD field_240_last_anim_frame;
    short field_242_pad;
    int field_244_stone_state; //TODO @ 100%  fix type length
    DWORD field_248_gnFrame;
    int field_24C_field_118;
    short field_250_throwable_count;
    char field_252_paramonia_done;
    char field_253_scrabania_done;
    int field_254_ring_pulse_timer;
    short field_258_bHaveShrykull;
    WORD field_25A_bElumExists;
    short field_25C_bControllingElum;
    WORD field_25E_bElumRespawnOnDead;
    LevelIds field_260_elum_lvl_number;
    WORD field_262_elum_path_number;
    WORD field_264_bInfiniteGrenades;
    short field_266_pad;
    DWORD field_268_elum_xpos;
    DWORD field_26C_elum_ypos;
    short field_270_elum_line_type;
    short field_272_elum_flipX;
    short field_274_elum_current_motion;
    WORD field_276_bDontFollowAbe;
    WORD field_278_brain_idx;
    WORD field_27A_elum_brain_state;
    DWORD field_27C_honey_xpos;
    WORD field_280_honey_ypos;
    short field_282_pad;
    DWORD field_284_unused;
    char field_288_elum_StrugglingWithBees;
    char field_289_elum_StungByBees;
    char field_28A_elum_Falling;
    char field_28B_elum_FoundHoney;
    PSX_RECT field_28C_elum_continue_rect;
    WORD field_294_continue_zone_number;
    WORD field_296_elum_zone_number;
    WORD field_298_elum_continue_path;
    LevelIds field_29A_continue_level;
    FP field_29C_elum_sprite_scale;
    WORD field_2A0_rescued_mudokons;
    WORD field_2A2_killed_mudokons;
    WORD field_2A4_restartRuptureFarmsSavedMudokons;
    WORD field_2A6_restartRuptureFarmsKilledMudokons;
    int field_2A8_gasTimer;
    short field_2AC_bUseAltSaveHeader;
    short field_2AE_controller_idx;
    BYTE field_2B0_pSaveBuffer[7501];
};
ALIVE_ASSERT_SIZEOF(SaveData, 0x2000);

class SaveGame
{
public:
    EXPORT static void CC LoadFromMemory_459970(SaveData* pData, int bKillObjects);
    EXPORT static void CC SaveToMemory_459490(SaveData* pSaveData);
    EXPORT static short CC LoadFromFile_459D30(const char* name);
    EXPORT static BOOL CC SaveToFile_45A110(const char* name);

    static short GetPathId(short pathToFind, short* outFoundPathRow = nullptr);
    static int Hash(SaveData* table);
};

extern const char word_4BC670[6][8];

}

