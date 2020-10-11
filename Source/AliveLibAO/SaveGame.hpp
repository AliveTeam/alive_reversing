#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

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
    char field_222;
    char field_223;
    int field_224_xpos;
    DWORD field_228_ypos;
    FP field_22C_ah_health;
    FP field_230_ah_sprite_scale;
    LevelIds field_234_current_level;
    unsigned short field_236_current_path;
    short field_238_current_camera;
    short field_23A_mode_mask;
    short field_23C_ah_flipX;
    short field_23E;
    WORD field_240_last_anim_frame;
    char field_242;
    char field_243;
    int field_244_state;
    DWORD field_248_gnFrame;
    int field_24C_field_118;
    short field_250_throwable_count;
    char field_252_paramonia_done;
    char field_253_scrabania_done;
    int field_254_ring_pulse_timer;
    short field_258_bHaveShrykull;
    WORD field_25A_something_elum_related;
    short field_25C;
    WORD field_25E_elum_field_144;
    LevelIds field_260_elum_lvl_number;
    WORD field_262_elum_path_number;
    WORD field_264;
    char field_266;
    char field_267;
    DWORD field_268_elum_xpos;
    DWORD field_26C_elum_ypos;
    short field_270;
    short field_272_elum_flipX;
    short field_274;
    WORD field_276_elum_122;
    WORD field_278_brain_idx;
    WORD field_27A_elum_brain_state;
    DWORD field_27C_honey_xpos;
    WORD field_280_honey_ypos;
    char field_282;
    char field_283;
    DWORD field_284_elum_field_130;
    char field_288_elumflag1;
    char field_289_elumflag2;
    char field_28A_elumflag3;
    char field_28B_elumflag4;
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
    int field_2A8;
    short field_2AC;
    short field_2AE;
    BYTE field_2B0_pSaveBuffer[7501];
};
ALIVE_ASSERT_SIZEOF(SaveData, 0x2000);

class SaveGame
{
public:
    EXPORT static void CC sub_459970(SaveData*, int);
    EXPORT static void CC sub_45A2D0(unsigned char*, const unsigned char* const*, int);
    EXPORT static void CC Save_459490(SaveData* pSaveData);
    EXPORT static short CC Read_459D30(const char* name);
    EXPORT static BOOL CC WriteSave_45A110(const char* name);

    static int Hash(SaveData* table);
};

END_NS_AO

