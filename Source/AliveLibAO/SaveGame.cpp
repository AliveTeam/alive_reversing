#include "stdafx_ao.h"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "Elum.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "Input.hpp"
#include "SaveGame.hpp"
#include "MusicController.hpp"
#include "ResourceManager.hpp"

namespace AO {

ALIVE_VAR_EXTERN(s32, sGasTimer_507700);
ALIVE_VAR_EXTERN(s16, sRescuedMudokons_5076C0);
ALIVE_VAR_EXTERN(s16, sKilledMudokons_5076BC);
ALIVE_VAR(1, 0x4CF2B0, Save_PSX_Header, sSaveHeader2_4CF2B0, {});
ALIVE_VAR(1, 0x4BC250, Save_PSX_Header, sSaveHeader1_4BC250, {});
ALIVE_VAR(1, 0x5076B4, u16, bUseAltSaveHeader_5076B4, 0);

ALIVE_VAR(1, 0x500C184, u32, dword_500C18, 0);
ALIVE_VAR(1, 0x500A18, SaveData, gSaveBuffer_500A18, {});

ALIVE_VAR(1, 0x505668, SaveData, gSaveBuffer_505668, {});

EXPORT void Kill_Objects_451720()
{
    ResourceManager::LoadingLoop_41EAD0(0);

    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < gBaseGameObjects->Size(); j++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(j);
            if (!pObj)
            {
                break;
            }

            // OG BaseGameObject name = Task ?
            //Debug_Print_Stub_48DD70("pTask = 0x%lx\n", pObjIter);

            // Kill during a reset and there are no references
            if (!pObj->mFlags.Get(BaseGameObject::eSurviveDeathReset_Bit9) && pObj->field_C_refCount == 0)
            {
                // So die
                j = gBaseGameObjects->RemoveAt(j);
                delete pObj;
            }
        }
    }
}

void SaveGame::LoadFromMemory_459970(SaveData* pData, s32 bKillObjects)
{
    TRACE_ENTRYEXIT;

    // Never actually used
    //const s32 hash = Hash(pData);

    if (bKillObjects)
    {
        Kill_Objects_451720();
    }

    bUseAltSaveHeader_5076B4 = pData->field_2AC_bUseAltSaveHeader;

    sControlledCharacter_50767C = sActiveHero_507678;

    sActiveHero_507678->field_146_zone_number = pData->field_204_zone_number;
    sActiveHero_507678->field_148_clear_from_id = pData->field_206_clear_from_id;
    sActiveHero_507678->field_14A_clear_to_id = pData->field_208_clear_to_id;
    sActiveHero_507678->field_138_zone_top_left = pData->field_20A_zone_top_left;
    sActiveHero_507678->field_13C_zone_bottom_right = pData->field_20E_zone_bottom_right;
    sActiveHero_507678->field_144_saved_level = pData->field_212_saved_level;
    sActiveHero_507678->field_142_saved_path = pData->field_214_saved_path;
    sActiveHero_507678->field_140_saved_camera = pData->field_216_saved_camera;
    sActiveHero_507678->field_14C_saved_sprite_scale = pData->field_218_saved_sprite_scale;
    sActiveHero_507678->field_150_saved_ring_timer = pData->field_21C_saved_ring_timer;
    sActiveHero_507678->field_154_bSavedHaveShrykull = pData->field_220_bSavedHaveShrykull;
    sActiveHero_507678->field_168_ring_pulse_timer = pData->field_254_ring_pulse_timer;
    sActiveHero_507678->field_16C_bHaveShrykull = pData->field_258_bHaveShrykull;

    sRescuedMudokons_5076C0 = pData->field_2A0_rescued_mudokons;
    sKilledMudokons_5076BC = pData->field_2A2_killed_mudokons;

    gRestartRuptureFarmsSavedMuds_5076C8 = pData->field_2A4_restartRuptureFarmsSavedMudokons;
    gRestartRuptureFarmsKilledMuds_5076C4 = pData->field_2A6_restartRuptureFarmsKilledMudokons;

    sActiveHero_507678->field_100_health = FP_FromInteger(1);
    sActiveHero_507678->field_11C_regen_health_timer = gnFrameCount_507670;
    sActiveHero_507678->field_BC_sprite_scale = pData->field_230_ah_sprite_scale;
    sActiveHero_507678->field_118_timer = pData->field_24C_field_118;
    sActiveHero_507678->field_19C_throwable_count = static_cast<s8>(pData->field_250_throwable_count); // TODO: Type check when other save func done
    sActiveHero_507678->field_106_shot = 0;

    sActiveHero_507678->field_2A8_flags.Clear(Flags_2A8::e2A8_Bit6_bShrivel);
    sActiveHero_507678->field_2A8_flags.Set(Flags_2A8::e2A8_Bit12_bParamoniaDone, pData->field_252_paramonia_done & 1);
    sActiveHero_507678->field_2A8_flags.Set(Flags_2A8::e2A8_eBit13_bScrabaniaDone, pData->field_253_scrabania_done & 1);

    sActiveHero_507678->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pData->field_23C_ah_flipX & 1);

    sActiveHero_507678->field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

    gMap.field_E0_save_data = pData->field_2B0_pSaveBuffer;

    if (sActiveHero_507678->field_168_ring_pulse_timer)
    {
        if (sActiveHero_507678->field_16C_bHaveShrykull)
        {
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbemorphAOResID, 0, 0))
            {
                ResourceManager::LoadResourceFile_4551E0("SHRYPORT.BND", nullptr, nullptr, nullptr);
                ResourceManager::LoadingLoop_41EAD0(0);
            }

            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kSplineAOResID, 0, 0))
            {
                ResourceManager::LoadResourceFile_4551E0("SPLINE.BAN", nullptr, nullptr, nullptr);
                ResourceManager::LoadingLoop_41EAD0(0);
            }

            Abe::Get_Shrykull_Resources_42F480();
        }
    }

    if (pData->field_2A8_gasTimer)
    {
        sGasTimer_507700 = gnFrameCount_507670 - pData->field_2A8_gasTimer;
    }
    else
    {
        sGasTimer_507700 = 0;
    }

    sActiveHero_507678->field_2AC_pSaveData = pData;
    sActiveHero_507678->field_FC_current_motion = eAbeMotions::Motion_62_LoadedSaveSpawn_45ADD0;
    sActiveHero_507678->field_114_gnFrame = 0;

    MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, sActiveHero_507678, 0, 0);

    gMap.SetActiveCam_444660(
        pData->field_234_current_level,
        pData->field_236_current_path,
        pData->field_238_current_camera,
        CameraSwapEffects::eInstantChange_0,
        0,
        1);
}

const s8 word_4BC670[6][8] = {
    {0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x00, 0x00},
    {0x01, 0x02, 0x03, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x05, 0x07, 0x09, 0x0C, 0x0D, 0x00, 0x00, 0x00},
    {0x04, 0x08, 0x0B, 0x0E, 0x00, 0x00, 0x00, 0x00},
    {0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56}};

// The text in the PSX save is in Shift JS encoding:
// https://github.com/python/pythontestdotnet/blob/master/www/unicode/SHIFTJIS.TXT

const u8 byte_4BC450[] = {
    0x82, 0x71, // R
    0x82, 0x95, // u
    0x82, 0x90, // p
    0x82, 0x94, // t
    0x82, 0x95, // u
    0x82, 0x92, // r
    0x82, 0x85, // e
    0x82, 0x65, // f
    0x82, 0x81, // a
    0x82, 0x92, // r
    0x82, 0x8D, // m
    0x82, 0x93, // s
    0x81, 0x40, // space
    0x81, 0x40, // space
    0x81, 0x40, // space
    0x81, 0x40, // space
    0x81, 0x40, // space
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00};

const u8 byte_4BC478[] = {
    0x82, 0x71, 0x82, 0x95, 0x82, 0x90, 0x82, 0x94, 0x82, 0x95,
    0x82, 0x92, 0x82, 0x85, 0x82, 0x65, 0x82, 0x81, 0x82, 0x92,
    0x82, 0x8D, 0x82, 0x93, 0x81, 0x40, 0x82, 0x68, 0x82, 0x68,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC4A0[] = {
    0x82, 0x73, 0x82, 0x88, 0x82, 0x85, 0x81, 0x40, 0x82, 0x61,
    0x82, 0x8F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x82, 0x92,
    0x82, 0x8F, 0x82, 0x8F, 0x82, 0x8D, 0x81, 0x40, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC4C8[] = {
    0x82, 0x6F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x81, 0x82, 0x8D,
    0x82, 0x8F, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC4F0[] = {
    0x82, 0x6F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x81, 0x82, 0x8D,
    0x82, 0x8F, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x73, 0x82, 0x85, 0x82, 0x8D, 0x82, 0x90,
    0x82, 0x8C, 0x82, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC518[] = {
    0x82, 0x6F, 0x82, 0x81, 0x82, 0x92, 0x82, 0x81, 0x82, 0x8D,
    0x82, 0x8F, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x6D, 0x82, 0x85, 0x82, 0x93, 0x82, 0x94,
    0x82, 0x93, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC540[] = {
    0x82, 0x72, 0x82, 0x83, 0x82, 0x92, 0x82, 0x81, 0x82, 0x82,
    0x82, 0x81, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40,
    0x81, 0x40, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC568[] = {
    0x82, 0x72, 0x82, 0x83, 0x82, 0x92, 0x82, 0x81, 0x82, 0x82,
    0x82, 0x81, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x73, 0x82, 0x85, 0x82, 0x8D, 0x82, 0x90,
    0x82, 0x8C, 0x82, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC590[] = {
    0x82, 0x72, 0x82, 0x83, 0x82, 0x92, 0x82, 0x81, 0x82, 0x82,
    0x82, 0x81, 0x82, 0x8E, 0x82, 0x89, 0x82, 0x81, 0x82, 0x8E,
    0x81, 0x40, 0x82, 0x6D, 0x82, 0x85, 0x82, 0x93, 0x82, 0x94,
    0x82, 0x93, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC5B8[] = {
    0x82, 0x72, 0x82, 0x94, 0x82, 0x8F, 0x82, 0x83, 0x82, 0x8B,
    0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40,
    0x82, 0x64, 0x82, 0x93, 0x82, 0x83, 0x82, 0x81, 0x82, 0x90,
    0x82, 0x85, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC5E0[] = {
    0x82, 0x72, 0x82, 0x94, 0x82, 0x8F, 0x82, 0x83, 0x82, 0x8B,
    0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40,
    0x82, 0x64, 0x82, 0x93, 0x82, 0x83, 0x82, 0x81, 0x82, 0x90,
    0x82, 0x85, 0x81, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const u8 byte_4BC608[] = {
    0x82, 0x72, 0x82, 0x94, 0x82, 0x8F, 0x82, 0x83, 0x82, 0x8B,
    0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40,
    0x82, 0x71, 0x82, 0x85, 0x82, 0x94, 0x82, 0x95, 0x82, 0x92,
    0x82, 0x8E, 0x81, 0x40, 0x00, 0x00};

using byteArray = decltype(byte_4BC450);
const u8* encodedLevelNames_4BC62C[] = {
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
    byte_4BC4A0};

const char_type* rawLevelNames[] = {
    nullptr,
    "RuptureFarms     ",
    "Stockyard Escape ",
    "Paramonia        ",
    "Paramonian Temple",
    "Stockyard Escape ",
    "Stockyard Return ",
    nullptr,
    "Scrabania        ",
    "Scrabanian Temple",
    nullptr,
    nullptr,
    "The Boardroom    ",
    "Rupture Farms II ",
    "Paramonian Chase ",
    "Scrabania Chase  ",
};

s16 SaveGame::GetPathId(s16 pathToFind, s16* outFoundPathRow)
{
    s16 path_id = -1;
    for (s16 i = 0; i < 4; i++)
    {
        for (s32 j = 0; j < 8; j++)
        {
            const auto path = word_4BC670[i][j];
            if (!path)
            {
                break;
            }
            if (pathToFind == path)
            {
                if (outFoundPathRow)
                {
                    *outFoundPathRow = i;
                }
                return path_id;
            }
        }
    }
    return path_id;
}

void CC SaveGame::SaveToMemory_459490(SaveData* pSaveData)
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

    auto lvName = rawLevelNames[static_cast<s32>(gMap.mCurrentLevel)];
    if (lvName != nullptr)
    {
        memcpy(
            reinterpret_cast<s8*>(&pSaveData->field_0_header.field_0_frame_1_name[4]),
            lvName,
            18);
    }

    if (gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
    {
        s16 path_id = GetPathId(gMap.mCurrentPath);

        if (path_id != -1)
        {
            // - (minus sign)
            pSaveData->field_0_header.field_0_frame_1_name[44] = 0x81u;
            pSaveData->field_0_header.field_0_frame_1_name[45] = 0x7C;

            // 0x8250 = 1
            pSaveData->field_0_header.field_0_frame_1_name[46] = 0x82u;
            pSaveData->field_0_header.field_0_frame_1_name[47] = static_cast<s8>(path_id + 0x50);
        }
    }
    pSaveData->field_234_current_level = gMap.mCurrentLevel;
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
    pSaveData->field_236_current_path = gMap.mCurrentPath;
    pSaveData->field_2A2_killed_mudokons = sKilledMudokons_5076BC;
    pSaveData->field_238_current_camera = gMap.field_4_current_camera;
    pSaveData->field_2A6_restartRuptureFarmsKilledMudokons = gRestartRuptureFarmsKilledMuds_5076C4;
    pSaveData->field_240_last_anim_frame = sActiveHero_507678->field_10_anim.field_92_current_frame;
    pSaveData->field_23E_current_motion = sActiveHero_507678->field_FC_current_motion;
    pSaveData->field_224_xpos = FP_GetExponent(sActiveHero_507678->field_A8_xpos);
    pSaveData->field_228_ypos = FP_GetExponent(sActiveHero_507678->field_AC_ypos);
    if (sActiveHero_507678->field_F4_pLine)
    {
        pSaveData->field_23A_mode_mask = sActiveHero_507678->field_F4_pLine->field_8_type;
    }
    else
    {
        pSaveData->field_23A_mode_mask = 0;
    }
    pSaveData->field_22C_ah_health = sActiveHero_507678->field_100_health;
    pSaveData->field_23C_ah_flipX = sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pSaveData->field_230_ah_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;
    pSaveData->field_244_stone_state = static_cast<s32>(sActiveHero_507678->field_110_state.raw);
    pSaveData->field_248_gnFrame = sActiveHero_507678->field_114_gnFrame;
    pSaveData->field_24C_field_118 = sActiveHero_507678->field_118_timer;
    pSaveData->field_250_throwable_count = sActiveHero_507678->field_19C_throwable_count;
    pSaveData->field_253_scrabania_done = sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_eBit13_bScrabaniaDone);
    pSaveData->field_264_bInfiniteGrenades = gInfiniteGrenades_5076EC ? -1 : 0;
    pSaveData->field_252_paramonia_done = sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit12_bParamoniaDone);
    pSaveData->field_25A_bElumExists = gElum_507680 != nullptr;
    if (gElum_507680 != 0)
    {
        pSaveData->field_25C_bControllingElum = sControlledCharacter_50767C == gElum_507680;
        pSaveData->field_25E_bElumRespawnOnDead = gElum_507680->field_144_bRespawnOnDead;
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
            pSaveData->field_270_elum_line_type = gElum_507680->field_F4_pLine->field_8_type;
        }
        else
        {
            pSaveData->field_270_elum_line_type = -1;
        }
        pSaveData->field_274_elum_current_motion = gElum_507680->field_FC_current_motion;
        pSaveData->field_272_elum_flipX = gElum_507680->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX);
        pSaveData->field_278_brain_idx = gElum_507680->field_128_brain_idx;
        pSaveData->field_276_bDontFollowAbe = gElum_507680->field_122_bDontFollowAbe;
        pSaveData->field_27C_honey_xpos = gElum_507680->field_12C_honey_xpos;
        pSaveData->field_27A_elum_brain_state = gElum_507680->field_12A_brain_sub_state;
        pSaveData->field_284_unused = gElum_507680->field_130_unused;
        pSaveData->field_280_honey_ypos = gElum_507680->field_146_honey_ypos;
        pSaveData->field_288_elum_StrugglingWithBees = gElum_507680->field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1);
        pSaveData->field_289_elum_StungByBees = gElum_507680->field_170_flags.Get(Elum::Flags_170::eStungByBees_Bit2);
        pSaveData->field_28A_elum_Falling = gElum_507680->field_170_flags.Get(Elum::Flags_170::eFalling_Bit3);
        pSaveData->field_28B_elum_FoundHoney = gElum_507680->field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4);
    }
    if (sGasTimer_507700)
    {
        pSaveData->field_2A8_gasTimer = (gnFrameCount_507670 - sGasTimer_507700 <= 1) ? 1 : gnFrameCount_507670 - sGasTimer_507700;
    }
    else
    {
        pSaveData->field_2A8_gasTimer = 0;
    }
    pSaveData->field_2AC_bUseAltSaveHeader = bUseAltSaveHeader_5076B4;
    pSaveData->field_2AE_controller_idx = Input().CurrentController() == InputObject::PadIndex::First ? 0 : 1;
    gMap.SaveBlyData_446900(pSaveData->field_2B0_pSaveBuffer);

    pSaveData->field_200_hashValue = Hash(pSaveData);
}

s32 SaveGame::Hash(SaveData* sData)
{
    auto table = reinterpret_cast<s32*>(&sData->field_204_zone_number);
    s32 counter = 0;
    for (s32 hashIter = 1919; hashIter > 0; hashIter--)
    {
        counter += *table;
        table++;
    }
    return counter;
}

s16 CC SaveGame::LoadFromFile_459D30(const char_type* name)
{
    char_type buffer[40] = {};

    strcpy(buffer, name);
    strcat(buffer, ".sav");

    const auto file = fopen(buffer, "rb");
    if (!file)
    {
        return 0;
    }
    const auto readVar = fread(&gSaveBuffer_500A18, 1, sizeof(SaveData), file);
    fclose(file);
    if (readVar != sizeof(SaveData))
    {
        return 0;
    }

    auto hashVal = Hash(&gSaveBuffer_500A18);
    if (hashVal == gSaveBuffer_500A18.field_200_hashValue)
    {
        gSaveBuffer_505668 = gSaveBuffer_500A18;
        LoadFromMemory_459970(&gSaveBuffer_505668, 1);
        gSaveBuffer_505668.field_238_current_camera = gSaveBuffer_505668.field_216_saved_camera;
        Input().SetCurrentController(InputObject::PadIndex::First);
        gSaveBuffer_505668.field_234_current_level = gSaveBuffer_505668.field_212_saved_level;
        gSaveBuffer_505668.field_236_current_path = gSaveBuffer_505668.field_214_saved_path;
        return 1;
    }
    else
    {
        return 0;
    }
}

Bool32 CC SaveGame::SaveToFile_45A110(const char_type* name)
{
    char_type buffer[40] = {};

    strcpy(buffer, name);
    strcat(buffer, ".sav");
    const auto file = fopen(buffer, "wb");
    if (!file)
    {
        return 0;
    }
    const auto written = fwrite(&gSaveBuffer_505668, 1, sizeof(SaveData), file);
    fclose(file);

    return written == sizeof(SaveData) ? 1 : 0;
}

} // namespace AO
