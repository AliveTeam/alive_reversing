#include "stdafx_ao.h"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Elum.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "Input.hpp"
#include "SaveGame.hpp"
#include "MusicController.hpp"
#include "ResourceManager.hpp"

namespace AO {

extern s32 sGasTimer;
extern s16 sRescuedMudokons;
extern s16 sKilledMudokons;
Save_PSX_Header sSaveHeader2_4CF2B0 = {};
Save_PSX_Header sSaveHeader1_4BC250 = {};
u16 bUseAltSaveHeader_5076B4 = 0;

u32 dword_500C18 = 0;
SaveData gSaveBuffer_500A18 = {};

SaveData gSaveBuffer = {};

void Kill_Objects_451720()
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
            if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eSurviveDeathReset_Bit9) && pObj->mBaseGameObjectRefCount == 0)
            {
                // So die
                j = gBaseGameObjects->RemoveAt(j);
                delete pObj;
            }
        }
    }
}

void SaveGame::LoadFromMemory(SaveData* pData, s32 bKillObjects)
{
    TRACE_ENTRYEXIT;

    // Never actually used
    //const s32 hash = Hash(pData);

    if (bKillObjects)
    {
        Kill_Objects_451720();
    }

    bUseAltSaveHeader_5076B4 = pData->field_2AC_bUseAltSaveHeader;

    sControlledCharacter = sActiveHero;

    sActiveHero->mContinueZoneNumber = pData->field_204_zone_number;
    sActiveHero->mContinueClearFromId = pData->field_206_clear_from_id;
    sActiveHero->mContinueClearToId = pData->field_208_clear_to_id;
    sActiveHero->mContinueTopLeft = pData->field_20A_zone_top_left;
    sActiveHero->mContinueBottomRight = pData->field_20E_zone_bottom_right;
    sActiveHero->mContinueLevel = MapWrapper::FromAO(pData->field_212_saved_level);
    sActiveHero->mContinuePath = pData->field_214_saved_path;
    sActiveHero->mContinueCamera = pData->field_216_saved_camera;
    sActiveHero->mContinueSpriteScale = pData->field_218_saved_sprite_scale;
    sActiveHero->field_150_saved_ring_timer = pData->field_21C_saved_ring_timer;
    sActiveHero->field_154_bSavedHaveShrykull = pData->field_220_bSavedHaveShrykull;
    sActiveHero->field_168_ring_pulse_timer = pData->field_254_ring_pulse_timer;
    sActiveHero->field_16C_bHaveShrykull = pData->field_258_bHaveShrykull;

    sRescuedMudokons = pData->field_2A0_rescued_mudokons;
    sKilledMudokons = pData->field_2A2_killed_mudokons;

    gRestartRuptureFarmsSavedMuds = pData->field_2A4_restartRuptureFarmsSavedMudokons;
    gRestartRuptureFarmsKilledMuds = pData->field_2A6_restartRuptureFarmsKilledMudokons;

    sActiveHero->mHealth = FP_FromInteger(1);
    sActiveHero->field_11C_regen_health_timer = sGnFrame;
    sActiveHero->SetSpriteScale(pData->field_230_ah_sprite_scale);
    sActiveHero->field_118_timer = pData->field_24C_field_118;
    sActiveHero->field_19C_throwable_count = static_cast<s8>(pData->field_250_throwable_count); // TODO: Type check when other save func done
    sActiveHero->mbGotShot = 0;

    sActiveHero->field_2A8_flags.Clear(Flags_2A8::e2A8_Bit6_bShrivel);
    sActiveHero->field_2A8_flags.Set(Flags_2A8::e2A8_Bit12_bParamoniaDone, pData->field_252_paramonia_done & 1);
    sActiveHero->field_2A8_flags.Set(Flags_2A8::e2A8_eBit13_bScrabaniaDone, pData->field_253_scrabania_done & 1);

    sActiveHero->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pData->field_23C_ah_flipX & 1);

    sActiveHero->GetAnimation().mFlags.Clear(AnimFlags::eRender);

    gMap.field_E0_save_data = pData->field_2B0_pSaveBuffer;

    if (sActiveHero->field_168_ring_pulse_timer)
    {
        if (sActiveHero->field_16C_bHaveShrykull)
        {
            //if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbemorphAOResID, 0, 0))
            //{
            //    ResourceManager::LoadResourceFile_4551E0("SHRYPORT.BND", nullptr, nullptr, nullptr);
            //    ResourceManager::LoadingLoop_41EAD0(0);
            //}

            //if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kSplineAOResID, 0, 0))
            //{
            //    ResourceManager::LoadResourceFile_4551E0("SPLINE.BAN", nullptr, nullptr, nullptr);
            //    ResourceManager::LoadingLoop_41EAD0(0);
            //}

            //Abe::Get_Shrykull_Resources_42F480();
        }
    }

    if (pData->field_2A8_gasTimer)
    {
        sGasTimer = sGnFrame - pData->field_2A8_gasTimer;
    }
    else
    {
        sGasTimer = 0;
    }

    sActiveHero->field_2AC_pSaveData = pData;
    sActiveHero->mCurrentMotion = eAbeMotions::Motion_62_LoadedSaveSpawn;
    sActiveHero->field_114_gnFrame = 0;

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, sActiveHero, 0, 0);

    gMap.SetActiveCam(
        MapWrapper::FromAO(pData->field_234_current_level),
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

void SaveGame::SaveToMemory(SaveData* pSaveData)
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

    auto lvName = rawLevelNames[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))];
    if (lvName != nullptr)
    {
        memcpy(
            reinterpret_cast<s8*>(&pSaveData->field_0_header.field_0_frame_1_name[4]),
            lvName,
            18);
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
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
    pSaveData->field_234_current_level = MapWrapper::ToAO(gMap.mCurrentLevel);
    pSaveData->field_206_clear_from_id = sActiveHero->mContinueClearFromId;
    pSaveData->field_20A_zone_top_left = sActiveHero->mContinueTopLeft;
    pSaveData->field_20E_zone_bottom_right = sActiveHero->mContinueBottomRight;
    pSaveData->field_204_zone_number = sActiveHero->mContinueZoneNumber;
    pSaveData->field_212_saved_level = MapWrapper::ToAO(sActiveHero->mContinueLevel);
    pSaveData->field_208_clear_to_id = sActiveHero->mContinueClearToId;
    pSaveData->field_216_saved_camera = sActiveHero->mContinueCamera;
    pSaveData->field_21C_saved_ring_timer = sActiveHero->field_150_saved_ring_timer;
    pSaveData->field_214_saved_path = sActiveHero->mContinuePath;
    pSaveData->field_254_ring_pulse_timer = sActiveHero->field_168_ring_pulse_timer;
    pSaveData->field_218_saved_sprite_scale = sActiveHero->mContinueSpriteScale;
    pSaveData->field_2A0_rescued_mudokons = sRescuedMudokons;
    pSaveData->field_220_bSavedHaveShrykull = sActiveHero->field_154_bSavedHaveShrykull;
    pSaveData->field_2A4_restartRuptureFarmsSavedMudokons = gRestartRuptureFarmsSavedMuds;
    pSaveData->field_258_bHaveShrykull = sActiveHero->field_16C_bHaveShrykull;
    pSaveData->field_236_current_path = gMap.mCurrentPath;
    pSaveData->field_2A2_killed_mudokons = sKilledMudokons;
    pSaveData->field_238_current_camera = gMap.mCurrentCamera;
    pSaveData->field_2A6_restartRuptureFarmsKilledMudokons = gRestartRuptureFarmsKilledMuds;
    pSaveData->field_240_last_anim_frame = static_cast<u16>(sActiveHero->GetAnimation().GetCurrentFrame());
    pSaveData->field_23E_current_motion = sActiveHero->mCurrentMotion;
    pSaveData->field_224_xpos = FP_GetExponent(sActiveHero->mXPos);
    pSaveData->field_228_ypos = FP_GetExponent(sActiveHero->mYPos);
    if (sActiveHero->BaseAliveGameObjectCollisionLine)
    {
        pSaveData->field_23A_mode_mask = sActiveHero->BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveData->field_23A_mode_mask = 0;
    }
    pSaveData->field_22C_ah_health = sActiveHero->mHealth;
    pSaveData->field_23C_ah_flipX = sActiveHero->GetAnimation().mFlags.Get(AnimFlags::eFlipX);
    pSaveData->field_230_ah_sprite_scale = sActiveHero->GetSpriteScale();
    pSaveData->field_244_stone_state = static_cast<s32>(sActiveHero->field_110_state.raw);
    pSaveData->field_248_gnFrame = sActiveHero->field_114_gnFrame;
    pSaveData->field_24C_field_118 = sActiveHero->field_118_timer;
    pSaveData->field_250_throwable_count = sActiveHero->field_19C_throwable_count;
    pSaveData->field_253_scrabania_done = sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_eBit13_bScrabaniaDone);
    pSaveData->field_264_bInfiniteGrenades = gInfiniteGrenades ? -1 : 0;
    pSaveData->field_252_paramonia_done = sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_Bit12_bParamoniaDone);
    pSaveData->field_25A_bElumExists = gElum != nullptr;
    if (gElum != 0)
    {
        pSaveData->field_25C_bControllingElum = sControlledCharacter == gElum;
        pSaveData->field_25E_bElumRespawnOnDead = gElum->mRespawnOnDead;
        pSaveData->field_28C_elum_continue_rect = gElum->mContinueRect;
        pSaveData->field_294_continue_zone_number = gElum->mPreviousContinueZoneNumber;
        pSaveData->field_296_elum_zone_number = gElum->mAbeZoneNumber;
        pSaveData->field_298_elum_continue_path = gElum->mContinuePath;
        pSaveData->field_29A_continue_level = MapWrapper::ToAO(gElum->mContinueLevel);
        pSaveData->field_29C_elum_sprite_scale = gElum->mContinueSpriteScale;
        pSaveData->field_260_elum_lvl_number = MapWrapper::ToAO(gElum->mCurrentLevel);
        pSaveData->field_262_elum_path_number = gElum->mCurrentPath;
        pSaveData->field_268_elum_xpos = FP_GetExponent(gElum->mXPos);
        pSaveData->field_26C_elum_ypos = FP_GetExponent(gElum->mYPos);
        if (gElum->BaseAliveGameObjectCollisionLine)
        {
            pSaveData->field_270_elum_line_type = gElum->BaseAliveGameObjectCollisionLine->mLineType;
        }
        else
        {
            pSaveData->field_270_elum_line_type = -1;
        }
        pSaveData->field_274_elum_current_motion = gElum->mCurrentMotion;
        pSaveData->field_272_elum_flipX = gElum->GetAnimation().mFlags.Get(AnimFlags::eFlipX);
        pSaveData->field_278_brain_idx = gElum->mBrainIdx;
        pSaveData->field_276_bDontFollowAbe = gElum->mDontFollowAbe;
        pSaveData->field_27C_honey_xpos = gElum->field_12C_honey_xpos;
        pSaveData->field_27A_elum_brain_state = gElum->mBrainSubState;
        pSaveData->field_284_unused = 0;
        pSaveData->field_280_honey_ypos = gElum->field_146_honey_ypos;
        pSaveData->field_288_elum_StrugglingWithBees = gElum->field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1);
        pSaveData->field_289_elum_StungByBees = gElum->field_170_flags.Get(Elum::Flags_170::eStungByBees_Bit2);
        pSaveData->field_28A_elum_Falling = gElum->field_170_flags.Get(Elum::Flags_170::eFalling_Bit3);
        pSaveData->field_28B_elum_FoundHoney = gElum->field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4);
    }
    if (sGasTimer)
    {
        pSaveData->field_2A8_gasTimer = (sGnFrame - sGasTimer <= 1) ? 1 : sGnFrame - sGasTimer;
    }
    else
    {
        pSaveData->field_2A8_gasTimer = 0;
    }
    pSaveData->field_2AC_bUseAltSaveHeader = bUseAltSaveHeader_5076B4;
    pSaveData->field_2AE_controller_idx = Input().CurrentController() == InputObject::PadIndex::First ? 0 : 1;
    gMap.SaveBlyData(pSaveData->field_2B0_pSaveBuffer);

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

s16 SaveGame::LoadFromFile(const char_type* name)
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
        gSaveBuffer = gSaveBuffer_500A18;
        LoadFromMemory(&gSaveBuffer, 1);
        gSaveBuffer.field_238_current_camera = gSaveBuffer.field_216_saved_camera;
        Input().SetCurrentController(InputObject::PadIndex::First);
        gSaveBuffer.field_234_current_level = gSaveBuffer.field_212_saved_level;
        gSaveBuffer.field_236_current_path = gSaveBuffer.field_214_saved_path;
        return 1;
    }
    else
    {
        return 0;
    }
}

bool SaveGame::SaveToFile(const char_type* name)
{
    char_type buffer[40] = {};

    strcpy(buffer, name);
    strcat(buffer, ".sav");
    const auto file = fopen(buffer, "wb");
    if (!file)
    {
        return 0;
    }
    const auto written = fwrite(&gSaveBuffer, 1, sizeof(SaveData), file);
    fclose(file);

    return written == sizeof(SaveData) ? 1 : 0;
}

} // namespace AO
