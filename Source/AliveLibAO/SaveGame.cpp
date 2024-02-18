#include "stdafx_ao.h"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Elum.hpp"
#include "Game.hpp"
#include "Grenade.hpp"
#include "Input.hpp"
#include "SaveGame.hpp"
#include "MusicController.hpp"
#include "ResourceManagerWrapper.hpp"
#include "../AliveLibAE/GasCountDown.hpp"
#include "nlohmann/json.hpp"
#include "../relive_lib/data_conversion/AOSaveSerialization.hpp"
#include "../relive_lib/data_conversion/data_conversion.hpp"
#include "Map.hpp"

namespace AO {

extern s16 gRescuedMudokons;
extern s16 gKilledMudokons;

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

    gAbe->mContinuePointZoneNumber = pData->mAbe_ContinuePointZoneNumber;
    gAbe->mContinuePointClearFromId = pData->mAbe_ContinuePointClearFromId;
    gAbe->mContinuePointClearToId = pData->mAbe_ContinuePointClearToId;
    gAbe->mContinuePointTopLeft = pData->mAbe_ContinuePointTopLeft;
    gAbe->mContinuePointBottomRight = pData->mAbe_ContinuePointBottomRight;
    gAbe->mContinuePointLevel = pData->mAbe_ContinuePointLevel;
    gAbe->mContinuePointPath = pData->mAbe_ContinuePointPath;
    gAbe->mContinuePointCamera = pData->mAbe_ContinuePoint_Camera;
    gAbe->mContinuePointSpriteScale = pData->mAbe_ContinuePointSpriteScale;
    gAbe->field_150_saved_ring_timer = pData->mAbe_SavedRingTimer;
    gAbe->field_154_bSavedHaveShrykull = pData->mAbe_SavedHaveShrykull;
    gAbe->field_168_ring_pulse_timer = pData->mAbe_RingPulseTimer;
    gAbe->field_16C_bHaveShrykull = pData->mAbe_HaveShrykull;

    gRescuedMudokons = pData->mRescuedMudokons;
    gKilledMudokons = pData->mKilledMudokons;

    gRestartRuptureFarmsSavedMuds = pData->field_2A4_restartRuptureFarmsSavedMudokons;
    gRestartRuptureFarmsKilledMuds = pData->mRestartRuptureFarmsKilledMuds;

    gAbe->mHealth = FP_FromInteger(1);
    gAbe->field_11C_regen_health_timer = sGnFrame;
    gAbe->SetSpriteScale(pData->mAbe_SpriteScale);
    gAbe->field_118_timer = pData->mAbe_Timer;
    gAbe->field_19C_throwable_count = static_cast<s8>(pData->mAbe_ThrowableCount); // TODO: Type check when other save func done
    gAbe->mbGotShot = 0;

    gAbe->mShrivel = false;
    gAbe->mParamoniaDone = pData->mAbe_ParamoniaDone & 1;
    gAbe->mScrabaniaDone = pData->mAbe_ScrabaniaDone & 1;

    gAbe->GetAnimation().SetFlipX(pData->mAbe_FlipX & 1);

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

    if (pData->mDeathGasTimer)
    {
        gDeathGasTimer = sGnFrame - pData->mDeathGasTimer;
    }
    else
    {
        gDeathGasTimer = 0;
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
    pSaveData->mAbe_ContinuePointClearFromId = gAbe->mContinuePointClearFromId;
    pSaveData->mAbe_ContinuePointTopLeft = gAbe->mContinuePointTopLeft;
    pSaveData->mAbe_ContinuePointBottomRight = gAbe->mContinuePointBottomRight;
    pSaveData->mAbe_ContinuePointZoneNumber = gAbe->mContinuePointZoneNumber;
    pSaveData->mAbe_ContinuePointLevel = gAbe->mContinuePointLevel;
    pSaveData->mAbe_ContinuePointClearToId = gAbe->mContinuePointClearToId;
    pSaveData->mAbe_ContinuePoint_Camera = gAbe->mContinuePointCamera;
    pSaveData->mAbe_SavedRingTimer = gAbe->field_150_saved_ring_timer;
    pSaveData->mAbe_ContinuePointPath = gAbe->mContinuePointPath;
    pSaveData->mAbe_RingPulseTimer = gAbe->field_168_ring_pulse_timer;
    pSaveData->mAbe_ContinuePointSpriteScale = gAbe->mContinuePointSpriteScale;
    pSaveData->mRescuedMudokons = gRescuedMudokons;
    pSaveData->mAbe_SavedHaveShrykull = gAbe->field_154_bSavedHaveShrykull;
    pSaveData->field_2A4_restartRuptureFarmsSavedMudokons = gRestartRuptureFarmsSavedMuds;
    pSaveData->mAbe_HaveShrykull = gAbe->field_16C_bHaveShrykull;
    pSaveData->mCurrentPath = gMap.mCurrentPath;
    pSaveData->mKilledMudokons = gKilledMudokons;
    pSaveData->mCurrentCamera = gMap.mCurrentCamera;
    pSaveData->mRestartRuptureFarmsKilledMuds = gRestartRuptureFarmsKilledMuds;
    pSaveData->mAbe_CurrentFrame = static_cast<u16>(gAbe->GetAnimation().GetCurrentFrame());
    pSaveData->mAbe_CurrentMotion = gAbe->mCurrentMotion;
    pSaveData->mAbe_XPos = FP_GetExponent(gAbe->mXPos);
    pSaveData->mAbe_YPos = FP_GetExponent(gAbe->mYPos);

    if (gAbe->BaseAliveGameObjectCollisionLine)
    {
        pSaveData->mAbe_LineType = gAbe->BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveData->mAbe_LineType = eLineTypes::eFloor_0;
    }

    pSaveData->mAbe_Health = gAbe->mHealth;
    pSaveData->mAbe_FlipX = gAbe->GetAnimation().GetFlipX();
    pSaveData->mAbe_SpriteScale = gAbe->GetSpriteScale();
    pSaveData->mAbe_StoneState = static_cast<s32>(gAbe->field_110_state.raw);
    pSaveData->mAbe_GnFrame = gAbe->field_114_gnFrame;
    pSaveData->mAbe_Timer = gAbe->field_118_timer;
    pSaveData->mAbe_ThrowableCount = gAbe->field_19C_throwable_count;
    pSaveData->mAbe_ScrabaniaDone = gAbe->mScrabaniaDone;
    pSaveData->mInfiniteGrenades = gInfiniteGrenades ? -1 : 0;
    pSaveData->mAbe_ParamoniaDone = gAbe->mParamoniaDone;
    pSaveData->mElumExists = gElum != nullptr;

    if (gElum)
    {
        pSaveData->mControllingElum = sControlledCharacter == gElum;
        pSaveData->mElum_RespawnOnDead = gElum->mRespawnOnDead;
        pSaveData->mElum_ContinuePointRect = gElum->mContinuePointRect;
        pSaveData->mElum_PreviousContinueZonePointNumber = gElum->mPreviousContinuePointZoneNumber;
        pSaveData->mElum_AbeZoneNumber = gElum->mAbeZoneNumber;
        pSaveData->mElum_ContinuePointPath = gElum->mContinuePointPath;
        pSaveData->mElum_ContinuePointLevel = gElum->mContinuePointLevel;
        pSaveData->mElum_ContinuePointSpriteScale = gElum->mContinuePointSpriteScale;
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
            pSaveData->mElum_LineType = eLineTypes::eNone_m1;
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

    if (gDeathGasTimer)
    {
        pSaveData->mDeathGasTimer = (sGnFrame - gDeathGasTimer <= 1) ? 1 : sGnFrame - gDeathGasTimer;
    }
    else
    {
        pSaveData->mDeathGasTimer = 0;
    }
    pSaveData->mCurrentControllerIdx = Input().CurrentController() == InputObject::PadIndex::First ? 0 : 1;
    gMap.SaveBlyData(pSaveData->field_2B0_pSaveBuffer);
}


s16 SaveGame::LoadFromFile(const char_type* name)
{
    char_type buffer[40] = {};

    strcpy(buffer, name);
    strcat(buffer, ".json");

    FileSystem fs;
    std::string jsonStr = fs.LoadToString(buffer);

    if (jsonStr.empty())
    {
        ALIVE_FATAL("Save file is empty");
    }

    nlohmann::json j = nlohmann::json::parse(jsonStr);
    from_json(j, sSaveToLoadBuffer);
    gSaveBuffer = sSaveToLoadBuffer;

    LoadFromMemory(&gSaveBuffer, 1);
    gSaveBuffer.mCurrentCamera = gSaveBuffer.mAbe_ContinuePoint_Camera;
    Input().SetCurrentController(InputObject::PadIndex::First);
    gSaveBuffer.mCurrentLevel = gSaveBuffer.mAbe_ContinuePointLevel;
    gSaveBuffer.mCurrentPath = gSaveBuffer.mAbe_ContinuePointPath;
    return 1;
}

bool SaveGame::SaveToFile(const char_type* name)
{
    char_type buffer[40] = {};

    strcpy(buffer, name);
    strcat(buffer, ".json");

    nlohmann::json j;
    to_json(j, gSaveBuffer);

    FileSystem fs;
    return SaveJson(j, fs, buffer);
}

} // namespace AO
