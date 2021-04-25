#pragma once

#include "FunctionFwd.hpp"
#include "PathData.hpp"
#include "Psx.hpp"

namespace AO {

struct Save_PSX_Header
{
    u8 field_0_frame_1_name[128];
    s8 field_80_frame_2_padding[128];
    s8 field_100_frame_3_padding[128];
    s8 field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF(Save_PSX_Header, 0x200);

struct SaveData
{
    Save_PSX_Header field_0_header;
    s32 field_200_hashValue;
    s16 field_204_zone_number;
    s16 field_206_clear_from_id;
    s16 field_208_clear_to_id;
    PSX_Point field_20A_zone_top_left;
    PSX_Point field_20E_zone_bottom_right;
    LevelIds field_212_saved_level;
    s16 field_214_saved_path;
    s16 field_216_saved_camera;
    FP field_218_saved_sprite_scale;
    s32 field_21C_saved_ring_timer;
    s16 field_220_bSavedHaveShrykull;
    s16 field_222_pad;
    s32 field_224_xpos;
    u32 field_228_ypos;
    FP field_22C_ah_health;
    FP field_230_ah_sprite_scale;
    LevelIds field_234_current_level;
    u16 field_236_current_path;
    s16 field_238_current_camera;
    s16 field_23A_mode_mask;
    s16 field_23C_ah_flipX;
    s16 field_23E_current_motion;
    WORD field_240_last_anim_frame;
    s16 field_242_pad;
    s32 field_244_stone_state; //TODO @ 100%  fix type length
    u32 field_248_gnFrame;
    s32 field_24C_field_118;
    s16 field_250_throwable_count;
    s8 field_252_paramonia_done;
    s8 field_253_scrabania_done;
    s32 field_254_ring_pulse_timer;
    s16 field_258_bHaveShrykull;
    WORD field_25A_bElumExists;
    s16 field_25C_bControllingElum;
    WORD field_25E_bElumRespawnOnDead;
    LevelIds field_260_elum_lvl_number;
    WORD field_262_elum_path_number;
    WORD field_264_bInfiniteGrenades;
    s16 field_266_pad;
    u32 field_268_elum_xpos;
    u32 field_26C_elum_ypos;
    s16 field_270_elum_line_type;
    s16 field_272_elum_flipX;
    s16 field_274_elum_current_motion;
    WORD field_276_bDontFollowAbe;
    WORD field_278_brain_idx;
    WORD field_27A_elum_brain_state;
    u32 field_27C_honey_xpos;
    WORD field_280_honey_ypos;
    s16 field_282_pad;
    u32 field_284_unused;
    s8 field_288_elum_StrugglingWithBees;
    s8 field_289_elum_StungByBees;
    s8 field_28A_elum_Falling;
    s8 field_28B_elum_FoundHoney;
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
    s32 field_2A8_gasTimer;
    s16 field_2AC_bUseAltSaveHeader;
    s16 field_2AE_controller_idx;
    u8 field_2B0_pSaveBuffer[7501];
};
ALIVE_ASSERT_SIZEOF(SaveData, 0x2000);

class SaveGame
{
public:
    EXPORT static void CC LoadFromMemory_459970(SaveData* pData, s32 bKillObjects);
    EXPORT static void CC SaveToMemory_459490(SaveData* pSaveData);
    EXPORT static s16 CC LoadFromFile_459D30(const s8* name);
    EXPORT static BOOL CC SaveToFile_45A110(const s8* name);

    static s16 GetPathId(s16 pathToFind, s16* outFoundPathRow = nullptr);
    static s32 Hash(SaveData* table);
};

extern const s8 word_4BC670[6][8];

}

