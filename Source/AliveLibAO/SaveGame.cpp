#include "stdafx_ao.h"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Elum.hpp"
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

static SaveData sSaveToLoadBuffer = {};

SaveData gSaveBuffer = {};

void Kill_Objects()
{
    ResourceManagerWrapper::LoadingLoop(0);

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
            if (!pObj->GetSurviveDeathReset() && pObj->mBaseGameObjectRefCount == 0)
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
        Kill_Objects();
    }

    sControlledCharacter = gAbe;

    gAbe->mContinueZoneNumber = pData->mContinuePoint_ZoneNumber;
    gAbe->mContinueClearFromId = pData->mContinuePoint_ClearFromId;
    gAbe->mContinueClearToId = pData->mContinuePoint_ClearToId;
    gAbe->mContinueTopLeft = pData->mContinuePoint_TopLeft;
    gAbe->mContinueBottomRight = pData->mContinuePoint_BottomRight;
    gAbe->mContinueLevel = pData->mContinuePoint_Level;
    gAbe->mContinuePath = pData->mContinuePoint_Path;
    gAbe->mContinueCamera = pData->mContinuePoint_Camera;
    gAbe->mContinueSpriteScale = pData->mContinuePoint_SpriteScale;
    gAbe->field_150_saved_ring_timer = pData->field_21C_saved_ring_timer;
    gAbe->field_154_bSavedHaveShrykull = pData->mActiveHero_SavedHaveShrykull;
    gAbe->field_168_ring_pulse_timer = pData->field_254_ring_pulse_timer;
    gAbe->field_16C_bHaveShrykull = pData->mActiveHero_HaveShrykull;

    sRescuedMudokons = pData->mRescuedMudokons;
    sKilledMudokons = pData->mKilledMudokons;

    gRestartRuptureFarmsSavedMuds = pData->field_2A4_restartRuptureFarmsSavedMudokons;
    gRestartRuptureFarmsKilledMuds = pData->mRestartRuptureFarmsKilledMuds;

    gAbe->mHealth = FP_FromInteger(1);
    gAbe->field_11C_regen_health_timer = sGnFrame;
    gAbe->SetSpriteScale(pData->mActiveHero_SpriteScale);
    gAbe->field_118_timer = pData->field_24C_field_118;
    gAbe->field_19C_throwable_count = static_cast<s8>(pData->field_250_throwable_count); // TODO: Type check when other save func done
    gAbe->mbGotShot = 0;

    gAbe->mShrivel = false;
    gAbe->mParamoniaDone = pData->mActiveHero_ParamoniaDone & 1;
    gAbe->mScrabaniaDone = pData->mActiveHero_ScrabaniaDone & 1;

    gAbe->GetAnimation().SetFlipX(pData->mActiveHero_FlipX & 1);

    gAbe->GetAnimation().SetRender(false);

    gMap.mSaveData = pData->field_2B0_pSaveBuffer;

    if (gAbe->field_168_ring_pulse_timer)
    {
        if (gAbe->field_16C_bHaveShrykull)
        {
            //if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbemorphAOResID, 0, 0))
            //{
            //    ResourceManager::LoadResourceFile_4551E0("SHRYPORT.BND", nullptr, nullptr, nullptr);
            //    ResourceManager::LoadingLoop(0);
            //}

            //if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kSplineAOResID, 0, 0))
            //{
            //    ResourceManager::LoadResourceFile_4551E0("SPLINE.BAN", nullptr, nullptr, nullptr);
            //    ResourceManager::LoadingLoop(0);
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

    gAbe->field_2AC_pSaveData = pData;
    gAbe->mCurrentMotion = eAbeMotions::Motion_62_LoadedSaveSpawn;
    gAbe->field_114_gnFrame = 0;

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, gAbe, 0, 0);

    gMap.SetActiveCam(
        pData->mCurrentLevel,
        pData->mCurrentPath,
        pData->mCurrentCamera,
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
    pSaveData->mCurrentLevel = gMap.mCurrentLevel;
    pSaveData->mContinuePoint_ClearFromId = gAbe->mContinueClearFromId;
    pSaveData->mContinuePoint_TopLeft = gAbe->mContinueTopLeft;
    pSaveData->mContinuePoint_BottomRight = gAbe->mContinueBottomRight;
    pSaveData->mContinuePoint_ZoneNumber = gAbe->mContinueZoneNumber;
    pSaveData->mContinuePoint_Level = gAbe->mContinueLevel;
    pSaveData->mContinuePoint_ClearToId = gAbe->mContinueClearToId;
    pSaveData->mContinuePoint_Camera = gAbe->mContinueCamera;
    pSaveData->field_21C_saved_ring_timer = gAbe->field_150_saved_ring_timer;
    pSaveData->mContinuePoint_Path = gAbe->mContinuePath;
    pSaveData->field_254_ring_pulse_timer = gAbe->field_168_ring_pulse_timer;
    pSaveData->mContinuePoint_SpriteScale = gAbe->mContinueSpriteScale;
    pSaveData->mRescuedMudokons = sRescuedMudokons;
    pSaveData->mActiveHero_SavedHaveShrykull = gAbe->field_154_bSavedHaveShrykull;
    pSaveData->field_2A4_restartRuptureFarmsSavedMudokons = gRestartRuptureFarmsSavedMuds;
    pSaveData->mActiveHero_HaveShrykull = gAbe->field_16C_bHaveShrykull;
    pSaveData->mCurrentPath = gMap.mCurrentPath;
    pSaveData->mKilledMudokons = sKilledMudokons;
    pSaveData->mCurrentCamera = gMap.mCurrentCamera;
    pSaveData->mRestartRuptureFarmsKilledMuds = gRestartRuptureFarmsKilledMuds;
    pSaveData->mActiveHero_CurrentFrame = static_cast<u16>(gAbe->GetAnimation().GetCurrentFrame());
    pSaveData->mActiveHero_CurrentMotion = gAbe->mCurrentMotion;
    pSaveData->mActiveHero_XPos = FP_GetExponent(gAbe->mXPos);
    pSaveData->mActiveHero_YPos = FP_GetExponent(gAbe->mYPos);
    if (gAbe->BaseAliveGameObjectCollisionLine)
    {
        pSaveData->field_23A_mode_mask = gAbe->BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveData->field_23A_mode_mask = 0;
    }
    pSaveData->mActiveHero_Health = gAbe->mHealth;
    pSaveData->mActiveHero_FlipX = gAbe->GetAnimation().GetFlipX();
    pSaveData->mActiveHero_SpriteScale = gAbe->GetSpriteScale();
    pSaveData->field_244_stone_state = static_cast<s32>(gAbe->field_110_state.raw);
    pSaveData->field_248_gnFrame = gAbe->field_114_gnFrame;
    pSaveData->field_24C_field_118 = gAbe->field_118_timer;
    pSaveData->field_250_throwable_count = gAbe->field_19C_throwable_count;
    pSaveData->mActiveHero_ScrabaniaDone = gAbe->mScrabaniaDone;
    pSaveData->mInfiniteGrenades = gInfiniteGrenades ? -1 : 0;
    pSaveData->mActiveHero_ParamoniaDone = gAbe->mParamoniaDone;
    pSaveData->mElumExists = gElum != nullptr;
    if (gElum != 0)
    {
        pSaveData->mControllingElum = sControlledCharacter == gElum;
        pSaveData->mElum_RespawnOnDead = gElum->mRespawnOnDead;
        pSaveData->mElum_ContinueRect = gElum->mContinueRect;
        pSaveData->mElum_PreviousContinueZoneNumber = gElum->mPreviousContinueZoneNumber;
        pSaveData->mElum_AbeZoneNumber = gElum->mAbeZoneNumber;
        pSaveData->mElum_ContinuePath = gElum->mContinuePath;
        pSaveData->mElum_ContinueLevel = gElum->mContinueLevel;
        pSaveData->mElum_ContinueSpriteScale = gElum->mContinueSpriteScale;
        pSaveData->mElum_CurrentLevel = gElum->mCurrentLevel;
        pSaveData->mElum_CurrentPath = gElum->mCurrentPath;
        pSaveData->mElum_XPos = FP_GetExponent(gElum->mXPos);
        pSaveData->mElum_YPos = FP_GetExponent(gElum->mYPos);
        if (gElum->BaseAliveGameObjectCollisionLine)
        {
            pSaveData->mElum_LineType = gElum->BaseAliveGameObjectCollisionLine->mLineType;
        }
        else
        {
            pSaveData->mElum_LineType = -1;
        }
        pSaveData->mElum_CurrentMotion = gElum->mCurrentMotion;
        pSaveData->mElum_FlipX = gElum->GetAnimation().GetFlipX();
        pSaveData->mElum_BrainIdx = gElum->mBrainIdx;
        pSaveData->mElum_DontFollowAbe = gElum->mDontFollowAbe;
        pSaveData->mElum_HoneyXPos = gElum->mHoneyXPos;
        pSaveData->mElum_BrainSubState = gElum->mBrainSubState;
        pSaveData->mElum_HoneyCamera = gElum->mHoneyCamera;
        pSaveData->mElum_StrugglingWithBees = gElum->mStrugglingWithBees;
        pSaveData->mElum_StungByBees = gElum->mStungByBees;
        pSaveData->mElum_Falling = gElum->mFalling;
        pSaveData->mElum_FoundHoney = gElum->mFoundHoney;
    }
    if (sGasTimer)
    {
        pSaveData->field_2A8_gasTimer = (sGnFrame - sGasTimer <= 1) ? 1 : sGnFrame - sGasTimer;
    }
    else
    {
        pSaveData->field_2A8_gasTimer = 0;
    }
    pSaveData->mCurrentControllerIdx = Input().CurrentController() == InputObject::PadIndex::First ? 0 : 1;
    gMap.SaveBlyData(pSaveData->field_2B0_pSaveBuffer);
}


s16 SaveGame::LoadFromFile(const char_type* name)
{
    char_type buffer[40] = {};

    strcpy(buffer, name);
    strcat(buffer, ".json");

    const auto file = fopen(buffer, "rb");
    if (!file)
    {
        return 0;
    }
    const auto readVar = fread(&sSaveToLoadBuffer, 1, sizeof(SaveData), file);
    fclose(file);
    if (readVar != sizeof(SaveData))
    {
        return 0;
    }

    gSaveBuffer = sSaveToLoadBuffer;
    LoadFromMemory(&gSaveBuffer, 1);
    gSaveBuffer.mCurrentCamera = gSaveBuffer.mContinuePoint_Camera;
    Input().SetCurrentController(InputObject::PadIndex::First);
    gSaveBuffer.mCurrentLevel = gSaveBuffer.mContinuePoint_Level;
    gSaveBuffer.mCurrentPath = gSaveBuffer.mContinuePoint_Path;
    return 1;
}

bool SaveGame::SaveToFile(const char_type* name)
{
    char_type buffer[40] = {};

    strcpy(buffer, name);
    strcat(buffer, ".json");
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
