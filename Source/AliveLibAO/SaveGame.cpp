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
Save_PSX_Header sSaveHeader2_4CF2B0 = {};
Save_PSX_Header sSaveHeader1_4BC250 = {};
u16 bUseAltSaveHeader_5076B4 = 0;

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

    bUseAltSaveHeader_5076B4 = pData->field_2AC_bUseAltSaveHeader;

    sControlledCharacter = sActiveHero;

    sActiveHero->mContinueZoneNumber = pData->mContinuePoint_ZoneNumber;
    sActiveHero->mContinueClearFromId = pData->mContinuePoint_ClearFromId;
    sActiveHero->mContinueClearToId = pData->mContinuePoint_ClearToId;
    sActiveHero->mContinueTopLeft = pData->mContinuePoint_TopLeft;
    sActiveHero->mContinueBottomRight = pData->mContinuePoint_BottomRight;
    sActiveHero->mContinueLevel = MapWrapper::FromAO(pData->mContinuePoint_Level);
    sActiveHero->mContinuePath = pData->mContinuePoint_Path;
    sActiveHero->mContinueCamera = pData->mContinuePoint_Camera;
    sActiveHero->mContinueSpriteScale = pData->mContinuePoint_SpriteScale;
    sActiveHero->field_150_saved_ring_timer = pData->field_21C_saved_ring_timer;
    sActiveHero->field_154_bSavedHaveShrykull = pData->mActiveHero_SavedHaveShrykull;
    sActiveHero->field_168_ring_pulse_timer = pData->field_254_ring_pulse_timer;
    sActiveHero->field_16C_bHaveShrykull = pData->mActiveHero_HaveShrykull;

    sRescuedMudokons = pData->mRescuedMudokons;
    sKilledMudokons = pData->mKilledMudokons;

    gRestartRuptureFarmsSavedMuds = pData->field_2A4_restartRuptureFarmsSavedMudokons;
    gRestartRuptureFarmsKilledMuds = pData->mRestartRuptureFarmsKilledMuds;

    sActiveHero->mHealth = FP_FromInteger(1);
    sActiveHero->field_11C_regen_health_timer = sGnFrame;
    sActiveHero->SetSpriteScale(pData->mActiveHero_SpriteScale);
    sActiveHero->field_118_timer = pData->field_24C_field_118;
    sActiveHero->field_19C_throwable_count = static_cast<s8>(pData->field_250_throwable_count); // TODO: Type check when other save func done
    sActiveHero->mbGotShot = 0;

    sActiveHero->mShrivel = false;
    sActiveHero->mParamoniaDone = pData->mActiveHero_ParamoniaDone & 1;
    sActiveHero->mScrabaniaDone = pData->mActiveHero_ScrabaniaDone & 1;

    sActiveHero->GetAnimation().SetFlipX(pData->mActiveHero_FlipX & 1);

    sActiveHero->GetAnimation().SetRender(false);

    gMap.mSaveData = pData->field_2B0_pSaveBuffer;

    if (sActiveHero->field_168_ring_pulse_timer)
    {
        if (sActiveHero->field_16C_bHaveShrykull)
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

    sActiveHero->field_2AC_pSaveData = pData;
    sActiveHero->mCurrentMotion = eAbeMotions::Motion_62_LoadedSaveSpawn;
    sActiveHero->field_114_gnFrame = 0;

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, sActiveHero, 0, 0);

    gMap.SetActiveCam(
        MapWrapper::FromAO(pData->mCurrentLevel),
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
    Save_PSX_Header* pHeaderToUse = nullptr;
    if (bUseAltSaveHeader_5076B4)
    {
        pHeaderToUse = &sSaveHeader1_4BC250;
    }
    else
    {
        pHeaderToUse = &sSaveHeader2_4CF2B0;
    }

    pSaveData->mSavePsxHeader = *pHeaderToUse;

    auto lvName = rawLevelNames[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))];
    if (lvName != nullptr)
    {
        memcpy(
            reinterpret_cast<s8*>(&pSaveData->mSavePsxHeader.field_0_frame_1_name[4]),
            lvName,
            18);
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        s16 path_id = GetPathId(gMap.mCurrentPath);

        if (path_id != -1)
        {
            // - (minus sign)
            pSaveData->mSavePsxHeader.field_0_frame_1_name[44] = 0x81u;
            pSaveData->mSavePsxHeader.field_0_frame_1_name[45] = 0x7C;

            // 0x8250 = 1
            pSaveData->mSavePsxHeader.field_0_frame_1_name[46] = 0x82u;
            pSaveData->mSavePsxHeader.field_0_frame_1_name[47] = static_cast<s8>(path_id + 0x50);
        }
    }
    pSaveData->mCurrentLevel = MapWrapper::ToAO(gMap.mCurrentLevel);
    pSaveData->mContinuePoint_ClearFromId = sActiveHero->mContinueClearFromId;
    pSaveData->mContinuePoint_TopLeft = sActiveHero->mContinueTopLeft;
    pSaveData->mContinuePoint_BottomRight = sActiveHero->mContinueBottomRight;
    pSaveData->mContinuePoint_ZoneNumber = sActiveHero->mContinueZoneNumber;
    pSaveData->mContinuePoint_Level = MapWrapper::ToAO(sActiveHero->mContinueLevel);
    pSaveData->mContinuePoint_ClearToId = sActiveHero->mContinueClearToId;
    pSaveData->mContinuePoint_Camera = sActiveHero->mContinueCamera;
    pSaveData->field_21C_saved_ring_timer = sActiveHero->field_150_saved_ring_timer;
    pSaveData->mContinuePoint_Path = sActiveHero->mContinuePath;
    pSaveData->field_254_ring_pulse_timer = sActiveHero->field_168_ring_pulse_timer;
    pSaveData->mContinuePoint_SpriteScale = sActiveHero->mContinueSpriteScale;
    pSaveData->mRescuedMudokons = sRescuedMudokons;
    pSaveData->mActiveHero_SavedHaveShrykull = sActiveHero->field_154_bSavedHaveShrykull;
    pSaveData->field_2A4_restartRuptureFarmsSavedMudokons = gRestartRuptureFarmsSavedMuds;
    pSaveData->mActiveHero_HaveShrykull = sActiveHero->field_16C_bHaveShrykull;
    pSaveData->mCurrentPath = gMap.mCurrentPath;
    pSaveData->mKilledMudokons = sKilledMudokons;
    pSaveData->mCurrentCamera = gMap.mCurrentCamera;
    pSaveData->mRestartRuptureFarmsKilledMuds = gRestartRuptureFarmsKilledMuds;
    pSaveData->mActiveHero_CurrentFrame = static_cast<u16>(sActiveHero->GetAnimation().GetCurrentFrame());
    pSaveData->mActiveHero_CurrentMotion = sActiveHero->mCurrentMotion;
    pSaveData->mActiveHero_XPos = FP_GetExponent(sActiveHero->mXPos);
    pSaveData->mActiveHero_YPos = FP_GetExponent(sActiveHero->mYPos);
    if (sActiveHero->BaseAliveGameObjectCollisionLine)
    {
        pSaveData->field_23A_mode_mask = sActiveHero->BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveData->field_23A_mode_mask = 0;
    }
    pSaveData->mActiveHero_Health = sActiveHero->mHealth;
    pSaveData->mActiveHero_FlipX = sActiveHero->GetAnimation().GetFlipX();
    pSaveData->mActiveHero_SpriteScale = sActiveHero->GetSpriteScale();
    pSaveData->field_244_stone_state = static_cast<s32>(sActiveHero->field_110_state.raw);
    pSaveData->field_248_gnFrame = sActiveHero->field_114_gnFrame;
    pSaveData->field_24C_field_118 = sActiveHero->field_118_timer;
    pSaveData->field_250_throwable_count = sActiveHero->field_19C_throwable_count;
    pSaveData->mActiveHero_ScrabaniaDone = sActiveHero->mScrabaniaDone;
    pSaveData->mInfiniteGrenades = gInfiniteGrenades ? -1 : 0;
    pSaveData->mActiveHero_ParamoniaDone = sActiveHero->mParamoniaDone;
    pSaveData->mElumExists = gElum != nullptr;
    if (gElum != 0)
    {
        pSaveData->mControllingElum = sControlledCharacter == gElum;
        pSaveData->mElum_RespawnOnDead = gElum->mRespawnOnDead;
        pSaveData->mElum_ContinueRect = gElum->mContinueRect;
        pSaveData->mElum_PreviousContinueZoneNumber = gElum->mPreviousContinueZoneNumber;
        pSaveData->mElum_AbeZoneNumber = gElum->mAbeZoneNumber;
        pSaveData->mElum_ContinuePath = gElum->mContinuePath;
        pSaveData->mElum_ContinueLevel = MapWrapper::ToAO(gElum->mContinueLevel);
        pSaveData->mElum_ContinueSpriteScale = gElum->mContinueSpriteScale;
        pSaveData->mElum_CurrentLevel = MapWrapper::ToAO(gElum->mCurrentLevel);
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
        pSaveData->field_284_unused = 0;
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
    pSaveData->field_2AC_bUseAltSaveHeader = bUseAltSaveHeader_5076B4;
    pSaveData->mCurrentControllerIdx = Input().CurrentController() == InputObject::PadIndex::First ? 0 : 1;
    gMap.SaveBlyData(pSaveData->field_2B0_pSaveBuffer);

    pSaveData->mSaveHashValue = Hash(pSaveData);
}

s32 SaveGame::Hash(SaveData* sData)
{
    auto table = reinterpret_cast<s32*>(&sData->mContinuePoint_ZoneNumber);
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
    const auto readVar = fread(&sSaveToLoadBuffer, 1, sizeof(SaveData), file);
    fclose(file);
    if (readVar != sizeof(SaveData))
    {
        return 0;
    }

    auto hashVal = Hash(&sSaveToLoadBuffer);
    if (hashVal == sSaveToLoadBuffer.mSaveHashValue)
    {
        gSaveBuffer = sSaveToLoadBuffer;
        LoadFromMemory(&gSaveBuffer, 1);
        gSaveBuffer.mCurrentCamera = gSaveBuffer.mContinuePoint_Camera;
        Input().SetCurrentController(InputObject::PadIndex::First);
        gSaveBuffer.mCurrentLevel = gSaveBuffer.mContinuePoint_Level;
        gSaveBuffer.mCurrentPath = gSaveBuffer.mContinuePoint_Path;
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
