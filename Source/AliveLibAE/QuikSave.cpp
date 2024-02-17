#include "stdafx.h"
#include "QuikSave.hpp"
#include "../relive_lib/Function.hpp"
#include "ResourceManagerWrapper.hpp"
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


static void RestoreObjectState(ReliveTypes type, SerializedObjectData& pData)
{
    switch (type)
    {
        case ::ReliveTypes::eSligSpawner:
            return SligSpawner::CreateFromSaveState(pData);
            
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

void ConvertObjectsStatesToJson(nlohmann::json& j, const SerializedObjectData& pData)
{
    pData.ReadRewind();
    while (pData.CanRead())
    {
        const SaveStateBase* pSaveStateBase = pData.PeekTmpPtr<SaveStateBase>();
        LOG_INFO("Converting type %d with size %d", pSaveStateBase->mType, pSaveStateBase->mSize);
        ConvertObjectSaveStateDataToJson(j, pSaveStateBase->mType, pData);
    }
}

void QuikSave::RestoreBlyData(Quicksave& pSaveData)
{
    pSaveData.mObjectsStateData.ReadRewind();
    while (pSaveData.mObjectsStateData.CanRead())
    {
        const SaveStateBase* pSaveStateBase = pSaveData.mObjectsStateData.PeekTmpPtr<SaveStateBase>();
        RestoreObjectState(pSaveStateBase->mType, pSaveData.mObjectsStateData);
    }

    pSaveData.mObjectBlyData.ReadRewind();

    const u32 flagsTotal = pSaveData.mObjectBlyData.ReadU32();
    u32 readFlagsCount = 0;
    for (auto& binaryPath : gMap.GetLoadedPaths())
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mAttribute == relive::QuiksaveAttribute::eClearTlvFlags_1 || pTlv->mAttribute == relive::QuiksaveAttribute::eKeepTlvFlags_2) // Type 0 ignored - actually it should never be written here anyway
                {
                    const bool isLastTlv = pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List);

                    pTlv->mTlvFlags.Raw().all = pSaveData.mObjectBlyData.ReadU8();

                    // OG bug: the bly data can overwrite the end tlv list flag so we restore it
                    if (pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List) != isLastTlv)
                    {
                        LOG_WARNING("Bly data load removed end list terminator flag, putting it back");
                        pTlv->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List);
                    }


                    pTlv->mTlvSpecificMeaning = pSaveData.mObjectBlyData.ReadU8();
                    readFlagsCount++;

                    // Note: We can't check for an exact match because some OG demo saves have flags
                    // that are not being read
                    if (readFlagsCount > flagsTotal)
                    {
                        ALIVE_FATAL("Save data contains %d sets of flags but read more than that", flagsTotal);
                    }
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }
    ResourceManagerWrapper::LoadingLoop(false);
}

void Quicksave_LoadFromMemory_4C95A0()
{
    DestroyObjects();
    EventsReset();
    gSkipGameObjectUpdates = true;
    QuikSave::RestoreWorldInfo(QuikSave::gActiveQuicksaveData.mWorldInfo);
    gSwitchStates = QuikSave::gActiveQuicksaveData.mSwitchStates;
    gMap.mRestoreMapObjectStates = true;
    gMap.SetActiveCam(
        QuikSave::gActiveQuicksaveData.mWorldInfo.mLevel,
        QuikSave::gActiveQuicksaveData.mWorldInfo.mPath,
        QuikSave::gActiveQuicksaveData.mWorldInfo.mCam,
        CameraSwapEffects::eInstantChange_0,
        0,
        1);
    gMap.mForceLoad = 1;
}

void QuikSave::LoadActive()
{
    ResourceManagerWrapper::ShowLoadingIcon();
    Quicksave_LoadFromMemory_4C95A0();
}

static void WriteFlags(SerializedObjectData& pSaveBuffer, const relive::Path_TLV* pTlv, const BitField8<relive::TlvFlags>& flags)
{
    pSaveBuffer.WriteU8(flags.Raw().all);
    pSaveBuffer.WriteU8(pTlv->mTlvSpecificMeaning);
}

static u32 Quicksave_SaveBlyData_CountOrSave(SerializedObjectData* pSaveBuffer)
{
    u32 flagsTotal = 0;

    for (auto& binaryPath : gMap.GetLoadedPaths())
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mAttribute == relive::QuiksaveAttribute::eClearTlvFlags_1)
                {
                    if (pSaveBuffer)
                    {
                        BitField8<relive::TlvFlags> flags = pTlv->mTlvFlags;
                        if (flags.Get(relive::TlvFlags::eBit1_Created))
                        {
                            flags.Clear(relive::TlvFlags::eBit1_Created);
                            flags.Clear(relive::TlvFlags::eBit2_Destroyed);
                        }

                        WriteFlags(*pSaveBuffer, pTlv, flags);
                    }
                    flagsTotal++;
                }
                else if (pTlv->mAttribute == relive::QuiksaveAttribute::eKeepTlvFlags_2)
                {
                    if (pSaveBuffer)
                    {
                        WriteFlags(*pSaveBuffer, pTlv, pTlv->mTlvFlags);
                    }
                    flagsTotal++;
                }
                else
                {
                    // Type 0 ignored
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }
    return flagsTotal;
}


void Quicksave_SaveBlyData_4C9660(SerializedObjectData& pSaveBuffer)
{
    pSaveBuffer.WriteRewind();

    const u32 flagsCount = Quicksave_SaveBlyData_CountOrSave(nullptr);
    pSaveBuffer.WriteU32(flagsCount);

    Quicksave_SaveBlyData_CountOrSave(&pSaveBuffer);
}

void QuikSave::SaveToMemory_4C91A0(Quicksave& pSave)
{
    if (gAbe->mHealth > FP_FromInteger(0))
    {
        QuikSave::SaveWorldInfo(&pSave.mWorldInfo);
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

void QuikSave::DoQuicksave()
{
    ResourceManagerWrapper::ShowLoadingIcon();
    QuikSave::SaveToMemory_4C91A0(gActiveQuicksaveData);
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

void QuikSave::SaveWorldInfo(Quicksave_WorldInfo* pInfo)
{
    const PSX_RECT rect = sControlledCharacter->VGetBoundingRect();

    pInfo->mGnFrame = sGnFrame;
    pInfo->mLevel = gMap.mCurrentLevel;
    pInfo->mPath = gMap.mCurrentPath;
    pInfo->mCam = gMap.mCurrentCamera;

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
    pInfo->mControlledCharScale = sControlledCharacter->GetSpriteScale() == FP_FromDouble(1.0);
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

void QuikSave::FindSaves()
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
