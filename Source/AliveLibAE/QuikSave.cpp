#include "stdafx.h"
#include "QuikSave.hpp"
#include "../relive_lib/Function.hpp"
#include "ResourceManagerWrapper.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "Glukkon.hpp"
#include "UXB.hpp"
#include "../relive_lib/GameObjects/ThrowableArray.hpp"
#include "LCDStatusBoard.hpp"
#include "LCDScreen.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Events.hpp"
#include "WorkWheel.hpp"
#include "Drill.hpp"
#include "../relive_lib/data_conversion/file_system.hpp"
#include "LiftPoint.hpp"
#include "LiftMover.hpp"
#include "../relive_lib/GameObjects/TrapDoor.hpp"
#include "../relive_lib/GameObjects/TimerTrigger.hpp"
#include "SlamDoor.hpp"
#include "FlyingSlig.hpp"
#include "SlapLock.hpp"
#include "SlapLockWhirlWind.hpp"
#include "../relive_lib/GameObjects/AbilityRing.hpp"
#include "../relive_lib/Engine.hpp"
#include "Slurg.hpp"
#include "../relive_lib/GameObjects/GasCountDown.hpp"
#include "Rock.hpp"
#include "Meat.hpp"
#include "Bone.hpp"
#include "MineCar.hpp"
#include "Slig.hpp"
#include "SligSpawner.hpp"
#include "ScrabSpawner.hpp"
#include "GameEnderController.hpp"
#include "Paramite.hpp"
#include "BirdPortal.hpp"
#include "ColourfulMeter.hpp"
#include "MinesAlarm.hpp"
#include "EvilFart.hpp"
#include "CrawlingSlig.hpp"
#include "Fleech.hpp"
#include "Greeter.hpp"
#include "Slog.hpp"
#include "../relive_lib/GameObjects/Grenade.hpp"
#include "Mudokon.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/BinaryPath.hpp"
#include "FlyingSligSpawner.hpp"
#include "Scrab.hpp"
#include "Engine.hpp"
#include "MainMenu.hpp" // only for global gSavedKilledMudsPerZulag

#include "nlohmann/json.hpp" // TODO: temp
#include "../relive_lib/data_conversion/AESaveSerialization.hpp"

Quicksave QuikSave::gActiveQuicksaveData;
SaveFileRec QuikSave::gSaveFileRecords[128];
s32 QuikSave::gSavedGameToLoadIdx;
s32 QuikSave::gTotalSaveFilesCount;

static void ConvertObjectSaveStateDataToJson(nlohmann::json& j, ReliveTypes type, const SerializedObjectData& pData)
{
    switch (type)
    {
        case ::ReliveTypes::eSligSpawner:
            j.push_back(*pData.ReadTmpPtr<::SligSpawnerSaveState>());
            break;
        case ::ReliveTypes::eLiftMover:
            j.push_back(*pData.ReadTmpPtr<::LiftMoverSaveState>());
            break;

        case ::ReliveTypes::eBone:
            j.push_back(*pData.ReadTmpPtr<::BoneSaveState>());
            break;

        case ::ReliveTypes::eMinesAlarm:
            j.push_back(*pData.ReadTmpPtr<::MinesAlarmSaveState>());
            break;

        case ::ReliveTypes::eCrawlingSlig:
            j.push_back(*pData.ReadTmpPtr<::CrawlingSligSaveState>());
            break;

        case ::ReliveTypes::eDrill:
            j.push_back(*pData.ReadTmpPtr<::DrillSaveState>());
            break;

        case ::ReliveTypes::eEvilFart:
            j.push_back(*pData.ReadTmpPtr<::EvilFartSaveState>());
            break;

        case ::ReliveTypes::eFleech:
            j.push_back(*pData.ReadTmpPtr<::FleechSaveState>());
            break;

        case ::ReliveTypes::eFlyingSlig:
            j.push_back(*pData.ReadTmpPtr<::FlyingSligSaveState>());
            break;

        case ::ReliveTypes::eFlyingSligSpawner:
            j.push_back(*pData.ReadTmpPtr<::FlyingSligSpawnerSaveState>());
            break;

        case ::ReliveTypes::eGameEnderController:
            j.push_back(*pData.ReadTmpPtr<::GameEnderControllerSaveState>());
            break;

        case ::ReliveTypes::eSlapLock_OrbWhirlWind:
            j.push_back(*pData.ReadTmpPtr<::SlapLockWhirlWindSaveState>());
            break;

        case ::ReliveTypes::eSlapLock:
            j.push_back(*pData.ReadTmpPtr<::SlapLockSaveState>());
            break;

        case ::ReliveTypes::eGreeter:
            j.push_back(*pData.ReadTmpPtr<::GreeterSaveState>());
            break;

        case ::ReliveTypes::eGrenade:
            j.push_back(*pData.ReadTmpPtr<::GrenadeSaveState>());
            break;

        case ::ReliveTypes::eGlukkon:
            j.push_back(*pData.ReadTmpPtr<::GlukkonSaveState>());
            break;

        case ::ReliveTypes::eAbe:
            j.push_back(*pData.ReadTmpPtr<::AbeSaveState>());
            break;

        case ::ReliveTypes::eLiftPoint:
            j.push_back(*pData.ReadTmpPtr<::LiftPointSaveState>());
            break;

        case ::ReliveTypes::eMudokon:
        case ::ReliveTypes::eRingOrLiftMud:
            j.push_back(*pData.ReadTmpPtr<::MudokonSaveState>());
            break;

        case ::ReliveTypes::eMeat:
            j.push_back(*pData.ReadTmpPtr<::MeatSaveState>());
            break;

        case ::ReliveTypes::eMineCar:
            j.push_back(*pData.ReadTmpPtr<::MineCarSaveState>());
            break;

        case ::ReliveTypes::eParamite:
            j.push_back(*pData.ReadTmpPtr<::ParamiteSaveState>());
            break;

        case ::ReliveTypes::eBirdPortal:
            j.push_back(*pData.ReadTmpPtr<::BirdPortalSaveState>());
            break;

        case ::ReliveTypes::eThrowableArray:
            j.push_back(*pData.ReadTmpPtr<::ThrowableArraySaveState>());
            break;

        case ::ReliveTypes::eAbilityRing:
            j.push_back(*pData.ReadTmpPtr<::AbilityRingSaveState>());
            break;

        case ::ReliveTypes::eRock:
            j.push_back(*pData.ReadTmpPtr<::RockSaveState>());
            break;

        case ::ReliveTypes::eScrab:
            j.push_back(*pData.ReadTmpPtr<::ScrabSaveState>());
            break;

        case ::ReliveTypes::eScrabSpawner:
            j.push_back(*pData.ReadTmpPtr<::ScrabSpawnerSaveState>());
            break;

        case ::ReliveTypes::eSlamDoor:
            j.push_back(*pData.ReadTmpPtr<::SlamDoorSaveState>());
            break;

        case ::ReliveTypes::eSlig:
            j.push_back(*pData.ReadTmpPtr<::SligSaveState>());
            break;

        case ::ReliveTypes::eSlog:
            j.push_back(*pData.ReadTmpPtr<::SlogSaveState>());
            break;

        case ::ReliveTypes::eSlurg:
            j.push_back(*pData.ReadTmpPtr<::SlurgSaveState>());
            break;

        case ::ReliveTypes::eTimerTrigger:
            j.push_back(*pData.ReadTmpPtr<::TimerTriggerSaveState>());
            break;

        case ::ReliveTypes::eTrapDoor:
            j.push_back(*pData.ReadTmpPtr<::TrapDoorSaveState>());
            break;

        case ::ReliveTypes::eUXB:
            j.push_back(*pData.ReadTmpPtr<::UXBSaveState>());
            break;

        case ::ReliveTypes::eWorkWheel:
            j.push_back(*pData.ReadTmpPtr<::WorkWheelSaveState>());
            break;

        default:
            ALIVE_FATAL("No create json save state for type %d", static_cast<s32>(type));
    }
}


static void RestoreObjectState(ReliveTypes type, SerializedObjectData& pData, ResourceManagerWrapper& resMan, BaseMap& map)
{
    switch (type)
    {
        case ::ReliveTypes::eSligSpawner:
            return SligSpawner::CreateFromSaveState(pData, resMan, map);
            
        case ::ReliveTypes::eLiftMover:
            return LiftMover::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eBone:
            return Bone::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eMinesAlarm:
            return MinesAlarm::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eCrawlingSlig:
            return CrawlingSlig::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eDrill:
            return Drill::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eEvilFart:
            return EvilFart::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eFleech:
            return Fleech::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eFlyingSlig:
            return FlyingSlig::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eFlyingSligSpawner:
            return FlyingSligSpawner::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eGameEnderController:
            return GameEnderController::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eSlapLock_OrbWhirlWind:
            return SlapLockWhirlWind::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eSlapLock:
            return SlapLock::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eGreeter:
            return Greeter::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eGrenade:
            return Grenade::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eGlukkon:
            return Glukkon::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eAbe:
            return Abe::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eLiftPoint:
            return LiftPoint::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eMudokon:
        case ::ReliveTypes::eRingOrLiftMud:
            return Mudokon::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eMeat:
            return Meat::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eMineCar:
            return MineCar::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eParamite:
            return Paramite::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eBirdPortal:
            return BirdPortal::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eThrowableArray:
            return ThrowableArray::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eAbilityRing:
            return AbilityRing::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eRock:
            return Rock::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eScrab:
            return Scrab::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eScrabSpawner:
            return ScrabSpawner::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eSlamDoor:
            return SlamDoor::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eSlig:
            return Slig::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eSlog:
            return Slog::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eSlurg:
            return Slurg::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eTimerTrigger:
            return TimerTrigger::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eTrapDoor:
            return TrapDoor::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eUXB:
            return UXB::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eWorkWheel:
            return WorkWheel::CreateFromSaveState(pData, resMan, map);

        case ::ReliveTypes::eLCDScreen:
            return LCDScreen::CreateFromSaveState(pData, resMan, map);

        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}

void ConvertObjectsStatesToJson(nlohmann::json& j, const SerializedObjectData& pData)
{
    pData.ReadRewind();
    while (pData.CanRead())
    {
        const SaveStateBase* pSaveStateBase = pData.PeekTmpPtr<SaveStateBase>();
        LOG_INFO("Converting type %d with size %u", static_cast<s32>(pSaveStateBase->mType), pSaveStateBase->mSize);
        ConvertObjectSaveStateDataToJson(j, pSaveStateBase->mType, pData);
    }
}

void QuikSave::RestoreBlyData(PendingObjectRestoreData& pSaveData, ResourceManagerWrapper& resMan, BaseMap& map)
{
    pSaveData.mObjectsStateData.ReadRewind();
    while (pSaveData.mObjectsStateData.CanRead())
    {
        const SaveStateBase* pSaveStateBase = pSaveData.mObjectsStateData.PeekTmpPtr<SaveStateBase>();
        RestoreObjectState(pSaveStateBase->mType, pSaveData.mObjectsStateData, resMan, map);
    }

    map.RestoreQuicksaveBlyData(pSaveData.mObjectBlyData);
    resMan.LoadingLoop(false);
}

void Quicksave_LoadFromMemory_4C95A0(BaseMap& map)
{
    DestroyObjects(map.GetResourceManager());
    EventsReset();
    gSkipGameObjectUpdates = true;
    QuikSave::RestoreWorldInfo(QuikSave::gActiveQuicksaveData.mWorldInfo);
    gSwitchStates = QuikSave::gActiveQuicksaveData.mSwitchStates;
    map.mPendingSaveRestore = &QuikSave::gActiveQuicksaveData;
    map.SetActiveCam(
        QuikSave::gActiveQuicksaveData.mWorldInfo.mLevel,
        QuikSave::gActiveQuicksaveData.mWorldInfo.mPath,
        QuikSave::gActiveQuicksaveData.mWorldInfo.mCam,
        CameraSwapEffects::eInstantChange_0,{},
        1);
    map.mForceLoad = 1;
}

void QuikSave::LoadActive(BaseMap& map)
{
    map.GetResourceManager().ShowLoadingIcon(map);
    Quicksave_LoadFromMemory_4C95A0(map);
}

void QuikSave::SaveToMemory_4C91A0(Quicksave& pSave, BaseMap& map)
{
    if (gAbe->mHealth > FP_FromInteger(0))
    {
        QuikSave::SaveWorldInfo(&pSave.mWorldInfo, map);
        pSave.mSwitchStates = gSwitchStates;

        pSave.mObjectsStateData.WriteRewind();
        for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (!pObj->GetDead())
            {
                pObj->VGetSaveState(pSave.mObjectsStateData);
            }
        }

        map.SaveQuicksaveBlyData(pSave.mObjectBlyData);
    }
}

void QuikSave::DoQuicksave(BaseMap& map)
{
    map.GetResourceManager().ShowLoadingIcon(map);
    QuikSave::SaveToMemory_4C91A0(gActiveQuicksaveData, map);
}

void QuikSave::RestoreWorldInfo(const Quicksave_WorldInfo& rInfo)
{
    // Read all fields bar the last
    for (s32 i = 0; i < ALIVE_COUNTOF(rInfo.field_18_saved_killed_muds_per_zulag); i++)
    {
        gSavedKilledMudsPerZulag.mData[i] = rInfo.field_18_saved_killed_muds_per_zulag[i];
    }

    // Last is read from another field
    gSavedKilledMudsPerZulag.mData[ALIVE_COUNTOF(gSavedKilledMudsPerZulag.mData) - 1] = rInfo.field_17_last_saved_killed_muds_per_path;

    gAbe->SetRestoredFromQuickSave(true);
    gZulagNumber = rInfo.field_2C_current_zulag_number;
    gKilledMudokons = rInfo.mKilledMudokons;
    gRescuedMudokons = rInfo.mRescuedMudokons;
    gMudokonsInArea = rInfo.field_16_muds_in_area; // TODO: Check types
    gTotalMeterBars = rInfo.mTotalMeterBars;
    gbDrawMeterCountDown = rInfo.field_30_bDrawMeterCountDown;
    gDeathGasTimer = rInfo.mGasTimer;
    gAbeInvincible = rInfo.mAbeInvincible;
    gVisitedBonewerkz = rInfo.mVisitedBonewerkz;
    gVisitedBarracks = rInfo.mVisitedBarracks;
    gVisitedFeecoEnder = rInfo.mVisitedFeecoEnder;
    sGnFrame = rInfo.mGnFrame;
}

void QuikSave::SaveWorldInfo(Quicksave_WorldInfo* pInfo, BaseMap& map)
{
    const PSX_RECT rect = sControlledCharacter->VGetBoundingRect();

    pInfo->mGnFrame = sGnFrame;
    pInfo->mLevel = map.mCurrentLevel;
    pInfo->mPath = map.mCurrentPath;
    pInfo->mCam = map.mCurrentCamera;

    for (s32 i = 0; i < ALIVE_COUNTOF(pInfo->field_18_saved_killed_muds_per_zulag); i++)
    {
        pInfo->field_18_saved_killed_muds_per_zulag[i] = gSavedKilledMudsPerZulag.mData[i];
    }

    pInfo->field_17_last_saved_killed_muds_per_path = gSavedKilledMudsPerZulag.mData[ALIVE_COUNTOF(gSavedKilledMudsPerZulag.mData) - 1];

    pInfo->field_2C_current_zulag_number = gZulagNumber;
    pInfo->mRescuedMudokons = gRescuedMudokons;
    pInfo->mKilledMudokons = gKilledMudokons;
    pInfo->field_16_muds_in_area = static_cast<s8>(gMudokonsInArea); // TODO: Check types
    pInfo->mTotalMeterBars = gTotalMeterBars;
    pInfo->field_30_bDrawMeterCountDown = gbDrawMeterCountDown;
    pInfo->mAbeInvincible = gAbeInvincible;
    pInfo->mVisitedBonewerkz = gVisitedBonewerkz;
    pInfo->mVisitedBarracks = gVisitedBarracks;
    pInfo->mVisitedFeecoEnder = gVisitedFeecoEnder;
    pInfo->mGasTimer = gDeathGasTimer;
    pInfo->mControlledCharX = FP_GetExponent(sControlledCharacter->mXPos);
    pInfo->mControlledCharY = rect.h;
    pInfo->mControlledCharAtFullScale = sControlledCharacter->GetSpriteScale() == FP_FromDouble(1.0);
}

static s32 Sort_comparitor_4D42C0(const void* pSaveRecLeft, const void* pSaveRecRight)
{
    const s32 leftTime = reinterpret_cast<const SaveFileRec*>(pSaveRecLeft)->mLastWriteTimeStamp;
    const s32 rightTime = reinterpret_cast<const SaveFileRec*>(pSaveRecRight)->mLastWriteTimeStamp;

    if (leftTime <= rightTime)
    {
        return leftTime < rightTime;
    }
    else
    {
        return -1;
    }
}

void QuikSave::FindSaves(FileSystem& fs)
{
    gTotalSaveFilesCount = 0;

    fs.EnumerateDirectory("*.json", [](const char_type* fileName, u32 lastWriteTime)
                          {
                              if (gTotalSaveFilesCount < 128)
                              {
                                  size_t fileNameLen = strlen(fileName) - 5; // remove .json
                                  if (fileNameLen > 0)
                                  {
                                      // Limit length to prevent buffer overflow
                                      if (fileNameLen > 20)
                                      {
                                          fileNameLen = 20;
                                      }

                                      SaveFileRec* pRec = &gSaveFileRecords[gTotalSaveFilesCount];
                                      memcpy(pRec->mFileName, fileName, fileNameLen);
                                      pRec->mFileName[fileNameLen] = 0;

                                      pRec->mLastWriteTimeStamp = lastWriteTime;
                                      gTotalSaveFilesCount++;
                                  }
                              }
                          });

    // Sort all we've found by time stamp, users probably want to load their last save first
    qsort(gSaveFileRecords, gTotalSaveFilesCount, sizeof(SaveFileRec), Sort_comparitor_4D42C0);

    // Underflow
    if (gSavedGameToLoadIdx < 0)
    {
        gSavedGameToLoadIdx = 0;
    }

    // Overflow
    if (gSavedGameToLoadIdx >= gTotalSaveFilesCount)
    {
        gSavedGameToLoadIdx = gTotalSaveFilesCount - 1;
    }
}
