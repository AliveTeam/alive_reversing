#include "stdafx.h"
#include "QuikSave.hpp"
#include "../relive_lib/Function.hpp"
#include "ResourceManager.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "Glukkon.hpp"
#include "UXB.hpp"
#include "ThrowableArray.hpp"
#include "LCDStatusBoard.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Events.hpp"
#include "WorkWheel.hpp"
#include "Drill.hpp"
#include "Io.hpp"
#include "LiftPoint.hpp"
#include "LiftMover.hpp"
#include "TrapDoor.hpp"
#include "TimerTrigger.hpp"
#include "SlamDoor.hpp"
#include "FlyingSlig.hpp"
#include "SlapLock.hpp"
#include "SlapLockWhirlWind.hpp"
#include "AbilityRing.hpp"
#include "Slurg.hpp"
#include "GasCountDown.hpp"
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
#include "Grenade.hpp"
#include "Mudokon.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/BinaryPath.hpp"
#include "FlyingSligSpawner.hpp"
#include "Scrab.hpp"
#include "Game.hpp"

#include "nlohmann/json.hpp" // TODO: temp
#include "../relive_lib/data_conversion/AESaveSerialization.hpp"

u16 sQuickSave_saved_switchResetters_count_BB234C = 0;

static void ConvertObjectSaveStateDataToJson(nlohmann::json& j, ReliveTypes type, const SerializedObjectData& pData)
{
    switch (type)
    {
        case ::ReliveTypes::eSligSpawner:
            j["object_states"].push_back(*pData.ReadTmpPtr<::SligSpawnerSaveState>());
            break;
        case ::ReliveTypes::eLiftMover:
            j["object_states"].push_back(*pData.ReadTmpPtr<::LiftMoverSaveState>());
            break;

        case ::ReliveTypes::eBone:
            j["object_states"].push_back(*pData.ReadTmpPtr<::BoneSaveState>());
            break;

        case ::ReliveTypes::eMinesAlarm:
            j["object_states"].push_back(*pData.ReadTmpPtr<::MinesAlarmSaveState>());
            break;

        case ::ReliveTypes::eCrawlingSlig:
            j["object_states"].push_back(*pData.ReadTmpPtr<::CrawlingSligSaveState>());
            break;

        case ::ReliveTypes::eDrill:
            j["object_states"].push_back(*pData.ReadTmpPtr<::DrillSaveState>());
            break;

        case ::ReliveTypes::eEvilFart:
            j["object_states"].push_back(*pData.ReadTmpPtr<::EvilFartSaveState>());
            break;

        case ::ReliveTypes::eFleech:
            j["object_states"].push_back(*pData.ReadTmpPtr<::FleechSaveState>());
            break;

        case ::ReliveTypes::eFlyingSlig:
            j["object_states"].push_back(*pData.ReadTmpPtr<::FlyingSligSaveState>());
            break;

        case ::ReliveTypes::eFlyingSligSpawner:
            j["object_states"].push_back(*pData.ReadTmpPtr<::FlyingSligSpawnerSaveState>());
            break;

        case ::ReliveTypes::eGameEnderController:
            j["object_states"].push_back(*pData.ReadTmpPtr<::GameEnderControllerSaveState>());
            break;

        case ::ReliveTypes::eSlapLock_OrbWhirlWind:
            j["object_states"].push_back(*pData.ReadTmpPtr<::SlapLockWhirlWindSaveState>());
            break;

        case ::ReliveTypes::eSlapLock:
            j["object_states"].push_back(*pData.ReadTmpPtr<::SlapLockSaveState>());
            break;

        case ::ReliveTypes::eGreeter:
            j["object_states"].push_back(*pData.ReadTmpPtr<::GreeterSaveState>());
            break;

        case ::ReliveTypes::eGrenade:
            j["object_states"].push_back(*pData.ReadTmpPtr<::GrenadeSaveState>());
            break;

        case ::ReliveTypes::eGlukkon:
            j["object_states"].push_back(*pData.ReadTmpPtr<::GlukkonSaveState>());
            break;

        case ::ReliveTypes::eAbe:
            j["object_states"].push_back(*pData.ReadTmpPtr<::AbeSaveState>());
            break;

        case ::ReliveTypes::eLiftPoint:
            j["object_states"].push_back(*pData.ReadTmpPtr<::LiftPointSaveState>());
            break;

        case ::ReliveTypes::eMudokon:
        case ::ReliveTypes::eRingOrLiftMud:
            j["object_states"].push_back(*pData.ReadTmpPtr<::MudokonSaveState>());
            break;

        case ::ReliveTypes::eMeat:
            j["object_states"].push_back(*pData.ReadTmpPtr<::MeatSaveState>());
            break;

        case ::ReliveTypes::eMineCar:
            j["object_states"].push_back(*pData.ReadTmpPtr<::MineCarSaveState>());
            break;

        case ::ReliveTypes::eParamite:
            j["object_states"].push_back(*pData.ReadTmpPtr<::ParamiteSaveState>());
            break;

        case ::ReliveTypes::eBirdPortal:
            j["object_states"].push_back(*pData.ReadTmpPtr<::BirdPortalSaveState>());
            break;

        case ::ReliveTypes::eThrowableArray:
            j["object_states"].push_back(*pData.ReadTmpPtr<::ThrowableArraySaveState>());
            break;

        case ::ReliveTypes::eAbilityRing:
            j["object_states"].push_back(*pData.ReadTmpPtr<::AbilityRingSaveState>());
            break;

        case ::ReliveTypes::eRock:
            j["object_states"].push_back(*pData.ReadTmpPtr<::RockSaveState>());
            break;

        case ::ReliveTypes::eScrab:
            j["object_states"].push_back(*pData.ReadTmpPtr<::ScrabSaveState>());
            break;

        case ::ReliveTypes::eScrabSpawner:
            j["object_states"].push_back(*pData.ReadTmpPtr<::ScrabSpawnerSaveState>());
            break;

        case ::ReliveTypes::eSlamDoor:
            j["object_states"].push_back(*pData.ReadTmpPtr<::SlamDoorSaveState>());
            break;

        case ::ReliveTypes::eSlig:
            j["object_states"].push_back(*pData.ReadTmpPtr<::SligSaveState>());
            break;

        case ::ReliveTypes::eSlog:
            j["object_states"].push_back(*pData.ReadTmpPtr<::SlogSaveState>());
            break;

        case ::ReliveTypes::eSlurg:
            j["object_states"].push_back(*pData.ReadTmpPtr<::SlurgSaveState>());
            break;

        case ::ReliveTypes::eTimerTrigger:
            j["object_states"].push_back(*pData.ReadTmpPtr<::TimerTriggerSaveState>());
            break;

        case ::ReliveTypes::eTrapDoor:
            j["object_states"].push_back(*pData.ReadTmpPtr<::TrapDoorSaveState>());
            break;

        case ::ReliveTypes::eUXB:
            j["object_states"].push_back(*pData.ReadTmpPtr<::UXBSaveState>());
            break;

        case ::ReliveTypes::eWorkWheel:
            j["object_states"].push_back(*pData.ReadTmpPtr<::WorkWheelSaveState>());
            break;

        default:
            ALIVE_FATAL("No create json save state for type %d", static_cast<s32>(type));
    }
}

void ConvertObjectsStatesToJson(nlohmann::json& j, const SerializedObjectData& pData)
{
    pData.ReadRewind();

    // Skip to after the per object data
    for (;;)
    {
        const u32 type = pData.ReadU32();
        if (type == 0)
        {
            break;
        }
        ConvertObjectSaveStateDataToJson(j, static_cast<ReliveTypes>(type), pData);
    }
}

static void RestoreObjectState(ReliveTypes type, SerializedObjectData& pData)
{
    switch (type)
    {
        case ::ReliveTypes::eSligSpawner:
            SligSpawner::CreateFromSaveState(pData);
            
        case ::ReliveTypes::eLiftMover:
            return LiftMover::CreateFromSaveState(pData);

        case ::ReliveTypes::eBone:
            return Bone::CreateFromSaveState(pData);

        case ::ReliveTypes::eMinesAlarm:
            return MinesAlarm::CreateFromSaveState(pData);

        case ::ReliveTypes::eCrawlingSlig:
            return CrawlingSlig::CreateFromSaveState(pData);

        case ::ReliveTypes::eDrill:
            return Drill::CreateFromSaveState(pData);

        case ::ReliveTypes::eEvilFart:
            return EvilFart::CreateFromSaveState(pData);

        case ::ReliveTypes::eFleech:
            return Fleech::CreateFromSaveState(pData);

        case ::ReliveTypes::eFlyingSlig:
            return FlyingSlig::CreateFromSaveState(pData);

        case ::ReliveTypes::eFlyingSligSpawner:
            return FlyingSligSpawner::CreateFromSaveState(pData);

        case ::ReliveTypes::eGameEnderController:
            return GameEnderController::CreateFromSaveState(pData);

        case ::ReliveTypes::eSlapLock_OrbWhirlWind:
            return SlapLockWhirlWind::CreateFromSaveState(pData);

        case ::ReliveTypes::eSlapLock:
            return SlapLock::CreateFromSaveState(pData);

        case ::ReliveTypes::eGreeter:
            return Greeter::CreateFromSaveState(pData);

        case ::ReliveTypes::eGrenade:
            return Grenade::CreateFromSaveState(pData);

        case ::ReliveTypes::eGlukkon:
            return Glukkon::CreateFromSaveState(pData);

        case ::ReliveTypes::eAbe:
            return Abe::CreateFromSaveState(pData);

        case ::ReliveTypes::eLiftPoint:
            return LiftPoint::CreateFromSaveState(pData);

        case ::ReliveTypes::eMudokon:
        case ::ReliveTypes::eRingOrLiftMud:
            return Mudokon::CreateFromSaveState(pData);

        case ::ReliveTypes::eMeat:
            return Meat::CreateFromSaveState(pData);

        case ::ReliveTypes::eMineCar:
            return MineCar::CreateFromSaveState(pData);

        case ::ReliveTypes::eParamite:
            return Paramite::CreateFromSaveState(pData);

        case ::ReliveTypes::eBirdPortal:
            return BirdPortal::CreateFromSaveState(pData);

        case ::ReliveTypes::eThrowableArray:
            return ThrowableArray::CreateFromSaveState(pData);

        case ::ReliveTypes::eAbilityRing:
            return AbilityRing::CreateFromSaveState(pData);

        case ::ReliveTypes::eRock:
            return Rock::CreateFromSaveState(pData);

        case ::ReliveTypes::eScrab:
            return Scrab::CreateFromSaveState(pData);

        case ::ReliveTypes::eScrabSpawner:
            return ScrabSpawner::CreateFromSaveState(pData);

        case ::ReliveTypes::eSlamDoor:
            return SlamDoor::CreateFromSaveState(pData);

        case ::ReliveTypes::eSlig:
            return Slig::CreateFromSaveState(pData);

        case ::ReliveTypes::eSlog:
            return Slog::CreateFromSaveState(pData);

        case ::ReliveTypes::eSlurg:
            return Slurg::CreateFromSaveState(pData);

        case ::ReliveTypes::eTimerTrigger:
            return TimerTrigger::CreateFromSaveState(pData);

        case ::ReliveTypes::eTrapDoor:
            return TrapDoor::CreateFromSaveState(pData);

        case ::ReliveTypes::eUXB:
            return UXB::CreateFromSaveState(pData);

        case ::ReliveTypes::eWorkWheel:
            return WorkWheel::CreateFromSaveState(pData);
            
        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}

void QuikSave_RestoreBlyData(Quicksave& pSaveData)
{
    pSaveData.mObjectsStateData.ReadRewind();

    // Skip to after the per object data
    for (;;)
    {
        const u32 type = pSaveData.mObjectsStateData.ReadU32();
        if (type == 0)
        {
            break;
        }
        RestoreObjectState(static_cast<ReliveTypes>(type), pSaveData.mObjectsStateData);
    }

    pSaveData.mObjectBlyData.ReadRewind();
    for (auto& binaryPath : gMap.GetLoadedPaths())
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mAttribute == relive::QuiksaveAttribute::eClearTlvFlags_1 || pTlv->mAttribute == relive::QuiksaveAttribute::eKeepTlvFlags_2) // Type 0 ignored - actually it should never be written here anyway
                {
                    pTlv->mTlvFlags.Raw().all = pSaveData.mObjectBlyData.ReadU8();
                    pTlv->mTlvSpecificMeaning = pSaveData.mObjectBlyData.ReadU8();
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }

    ResourceManagerWrapper::LoadingLoop(false);
}


Quicksave gActiveQuicksaveData = {};
SaveFileRec gSaveFileRecords[128] = {};
s32 gSavedGameToLoadIdx = 0;
s32 gTotalSaveFilesCount = 0;

void Quicksave_LoadFromMemory_4C95A0()
{
    DestroyObjects();
    EventsReset();
    gSkipGameObjectUpdates = true;
    Quicksave_ReadWorldInfo(&gActiveQuicksaveData.mWorldInfo);
    gSwitchStates = gActiveQuicksaveData.mSwitchStates;
    gMap.mRestoreMapObjectStates = true;
    gMap.SetActiveCam(
        MapWrapper::FromAE(gActiveQuicksaveData.mWorldInfo.mLevel),
        gActiveQuicksaveData.mWorldInfo.mPath,
        gActiveQuicksaveData.mWorldInfo.mCam,
        CameraSwapEffects::eInstantChange_0,
        0,
        1);
    gMap.mForceLoad = 1;
}

void Quicksave_LoadActive()
{
    Game_ShowLoadingIcon();
    Quicksave_LoadFromMemory_4C95A0();
}

static void WriteFlags(SerializedObjectData& pSaveBuffer, const relive::Path_TLV* pTlv, const BitField8<relive::TlvFlags>& flags)
{
    pSaveBuffer.WriteU8(flags.Raw().all);
    pSaveBuffer.WriteU8(pTlv->mTlvSpecificMeaning);
}

void Quicksave_SaveBlyData_4C9660(SerializedObjectData& pSaveBuffer)
{
    pSaveBuffer.WriteRewind();

    for (auto& binaryPath : gMap.GetLoadedPaths())
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mAttribute == relive::QuiksaveAttribute::eClearTlvFlags_1)
                {
                    BitField8<relive::TlvFlags> flags = pTlv->mTlvFlags;
                    if (flags.Get(relive::TlvFlags::eBit1_Created))
                    {
                        flags.Clear(relive::TlvFlags::eBit1_Created);
                        flags.Clear(relive::TlvFlags::eBit2_Destroyed);
                    }
                    WriteFlags(pSaveBuffer, pTlv, flags);
                }
                else if (pTlv->mAttribute == relive::QuiksaveAttribute::eKeepTlvFlags_2)
                {
                    WriteFlags(pSaveBuffer, pTlv, pTlv->mTlvFlags);
                }
                else
                {
                    // Type 0 ignored
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }
}

// TODO: See if this can be nuked in both games
struct SaveFlagsAndData final
{
    BitField8<relive::TlvFlags> flags;
    u8 data;
};
SaveFlagsAndData sSwitchReset_Saved_States_BB233C[8] = {};

void Quicksave_SaveSwitchResetterStates()
{
    sQuickSave_saved_switchResetters_count_BB234C = 0;

    for (auto& binaryPath : gMap.GetLoadedPaths())
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mTlvType == ReliveTypes::eResetPath)
                {
                    if (sQuickSave_saved_switchResetters_count_BB234C < 8)
                    {
                        sSwitchReset_Saved_States_BB233C[sQuickSave_saved_switchResetters_count_BB234C].flags = pTlv->mTlvFlags;
                        sSwitchReset_Saved_States_BB233C[sQuickSave_saved_switchResetters_count_BB234C].data = pTlv->mTlvSpecificMeaning;

                        sQuickSave_saved_switchResetters_count_BB234C++;
                    }
                    else
                    {
                        LOG_WARNING("Out of write space !!");
                    }
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }
}

void Quicksave_RestoreSwitchResetterStates()
{
    s32 idx = 0;
    for (auto& binaryPath : gMap.GetLoadedPaths())
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mTlvType == ReliveTypes::eResetPath)
                {
                    if (idx < 8)
                    {
                        pTlv->mTlvFlags = sSwitchReset_Saved_States_BB233C[idx].flags;
                        pTlv->mTlvSpecificMeaning = sSwitchReset_Saved_States_BB233C[idx].data;

                        idx++;
                    }
                    else
                    {
                        LOG_WARNING("Out of read space !!");
                    }
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }

    sQuickSave_saved_switchResetters_count_BB234C = 0;
}

void Quicksave_SaveToMemory_4C91A0(Quicksave& pSave)
{
    if (sActiveHero->mHealth > FP_FromInteger(0))
    {
        char_type src[12] = {};
        sprintf(src, "%2sP%02dC%02d",
                Path_Get_Lvl_Name(gMap.mCurrentLevel),
                gMap.mCurrentPath,
                gMap.mCurrentCamera);

        Quicksave_SaveWorldInfo(&pSave.mWorldInfo);
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

        Quicksave_SaveBlyData_4C9660(pSave.mObjectBlyData);
    }
}

void DoQuicksave()
{
    Game_ShowLoadingIcon();
    Quicksave_SaveToMemory_4C91A0(gActiveQuicksaveData);
}

void Quicksave_ReadWorldInfo(const Quicksave_WorldInfo* pInfo)
{
    // Read all fields bar the last
    for (s32 i = 0; i < ALIVE_COUNTOF(pInfo->field_18_saved_killed_muds_per_zulag); i++)
    {
        sSavedKilledMudsPerZulag_5C1B50.mData[i] = pInfo->field_18_saved_killed_muds_per_zulag[i];
    }

    // Last is read from another field
    sSavedKilledMudsPerZulag_5C1B50.mData[ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData) - 1] = pInfo->field_17_last_saved_killed_muds_per_path;

    sActiveHero->SetRestoredFromQuickSave(true);
    gZulagNumber = pInfo->field_2C_current_zulag_number;
    gKilledMudokons = pInfo->mKilledMudokons;
    gRescuedMudokons = pInfo->mRescuedMudokons;
    gMudokonsInArea = pInfo->field_16_muds_in_area; // TODO: Check types
    gTotalMeterBars = pInfo->mTotalMeterBars;
    gbDrawMeterCountDown = pInfo->field_30_bDrawMeterCountDown;
    gGasTimer = pInfo->mGasTimer;
    gAbeInvincible = pInfo->mAbeInvincible;
    gVisitedBonewerkz = pInfo->mVisitedBonewerkz;
    gVisitedBarracks = pInfo->mVisitedBarracks;
    gVisitedFeecoEnder = pInfo->mVisitedFeecoEnder;
    sGnFrame = pInfo->mGnFrame;
}

void Quicksave_SaveWorldInfo(Quicksave_WorldInfo* pInfo)
{
    const PSX_RECT rect = sControlledCharacter->VGetBoundingRect();

    pInfo->mGnFrame = sGnFrame;
    pInfo->mLevel = MapWrapper::ToAE(gMap.mCurrentLevel);
    pInfo->mPath = gMap.mCurrentPath;
    pInfo->mCam = gMap.mCurrentCamera;

    for (s32 i = 0; i < ALIVE_COUNTOF(pInfo->field_18_saved_killed_muds_per_zulag); i++)
    {
        pInfo->field_18_saved_killed_muds_per_zulag[i] = sSavedKilledMudsPerZulag_5C1B50.mData[i];
    }

    pInfo->field_17_last_saved_killed_muds_per_path = sSavedKilledMudsPerZulag_5C1B50.mData[ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData) - 1];

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
    pInfo->mGasTimer = gGasTimer;
    pInfo->mControlledCharX = FP_GetExponent(sControlledCharacter->mXPos);
    pInfo->mControlledCharY = rect.h;
    pInfo->mControlledCharScale = sControlledCharacter->GetSpriteScale() == FP_FromDouble(1.0);
}

s32 Sort_comparitor_4D42C0(const void* pSaveRecLeft, const void* pSaveRecRight)
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

void Quicksave_FindSaves()
{
    gTotalSaveFilesCount = 0;

    IO_EnumerateDirectory("*.json", [](const char_type* fileName, u32 lastWriteTime)
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
