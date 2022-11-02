#include "stdafx.h"
#include "QuikSave.hpp"
#include "Function.hpp"
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
#include "../AliveLibCommon/FatalError.hpp"

u16 bUseAltSaveHeader_5C1BBC = 0;

u16 sQuickSave_saved_switchResetters_count_BB234C = 0;

static s32 RestoreObjectState(ReliveTypes type, const u8* pData)
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

void QuikSave_RestoreBlyData(const u8* pSaveData)
{
    const u16* pSaveData2 = reinterpret_cast<const u16*>(pSaveData);

    while (*reinterpret_cast<const u32*>(pSaveData2) != 0)
    {
        // Maps to AETypes
        pSaveData2 += RestoreObjectState(static_cast<ReliveTypes>(*pSaveData2), reinterpret_cast<const u8*>(pSaveData2)) / sizeof(u16);
    }

    // Skip the 2 zero entries, the saved flag words come after the object save state data
    const u8* pSrcFlags = reinterpret_cast<const u8*>(pSaveData2 + 2);

    for (auto& binaryPath : gMap.GetLoadedPaths())
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mAttribute == relive::QuiksaveAttribute::eClearTlvFlags_1 || pTlv->mAttribute == relive::QuiksaveAttribute::eKeepTlvFlags_2) // Type 0 ignored - actually it should never be written here anyway
                {
                    pTlv->mTlvFlags.Raw().all = *pSrcFlags;
                    pSrcFlags++;

                    pTlv->mTlvSpecificMeaning = *pSrcFlags;
                    pSrcFlags++;
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }

    pResourceManager_5C1BB0->LoadingLoop_465590(false);
}


Quicksave sActiveQuicksaveData = {};
SaveFileRec sSaveFileRecords_BB31D8[128] = {};
s32 sSavedGameToLoadIdx_BB43FC = 0;
s32 sTotalSaveFilesCount_BB43E0 = 0;

extern s32 sAccumulatedObjectCount_5C1BF4;

void Quicksave_LoadFromMemory_4C95A0(Quicksave* quicksaveData)
{
    sAccumulatedObjectCount_5C1BF4 = quicksaveData->field_200_accumulated_obj_count;
    DestroyObjects_4A1F20();
    EventsReset();
    bSkipGameObjectUpdates = 1;
    Quicksave_ReadWorldInfo(&quicksaveData->field_204_world_info);
    gSwitchStates = quicksaveData->field_45C_switch_states;
    gMap.mRestoreQuickSaveData = reinterpret_cast<u8*>(quicksaveData->field_55C_objects_state_data);
    gMap.SetActiveCam(
        MapWrapper::FromAE(quicksaveData->field_204_world_info.field_4_level),
        quicksaveData->field_204_world_info.field_6_path,
        quicksaveData->field_204_world_info.field_8_cam,
        CameraSwapEffects::eInstantChange_0,
        0,
        1);
    gMap.mForceLoad = 1;
}

void Quicksave_LoadActive()
{
    Game_ShowLoadingIcon_482D80();
    Quicksave_LoadFromMemory_4C95A0(&sActiveQuicksaveData);
}

static void WriteChars(u8*& pDst, u8 v1, u8 v2)
{
    *pDst = v1;
    pDst++;

    *pDst = v2;
    pDst++;
}

static void WriteFlags(u8*& pSaveBuffer, const relive::Path_TLV* pTlv, const BitField8<relive::TlvFlags>& flags)
{
    *pSaveBuffer = flags.Raw().all;
    pSaveBuffer++;

    *pSaveBuffer = pTlv->mTlvSpecificMeaning;
    pSaveBuffer++;
}

void Quicksave_SaveBlyData_4C9660(u8* pSaveBuffer)
{
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

void MEMCARD_Write_SJISC_String_4A2770(u8* src, u8* dst, s32 srcLength)
{
    while (srcLength > 0)
    {
        const u8 srcChar = *src;
        if (*src >= 'A' && srcChar <= 'Z')
        {
            WriteChars(dst, 0x82, *src + 0x1F);
        }
        else if (srcChar >= 'a' && srcChar <= 'z')
        {
            WriteChars(dst, 0x82, *src + 0x20);
        }
        else if (srcChar < '0' || srcChar > '9')
        {
            if (srcChar == ':')
            {
                WriteChars(dst, 0x81, 0x46);
            }
            else
            {
                if (srcChar == '"')
                {
                    WriteChars(dst, 0x81, 0x67);
                }
                else
                {
                    WriteChars(dst, 0x81, 0x40);
                }
            }
        }
        else
        {
            WriteChars(dst, 0x82, *src + 0x1F);
        }
        src++;
        srcLength--;
    }
}

void MEMCARD_Generate_Timestamp_String_4A21F0(char_type* pStr)
{
    sprintf(pStr, "%03d:%02d",
            (s32)(sGnFrame - sGameStartedFrame_5C1B88) / 30 / 60 / 60 % 1000,
            (s32)(sGnFrame - sGameStartedFrame_5C1B88) / 30 / 60 % 60);
}

void MEMCARD_Write_Timestamp_SJISC_String_4A2290(u8* dst)
{
    char_type gameTimeStr[12] = {};
    MEMCARD_Generate_Timestamp_String_4A21F0(gameTimeStr);

    const size_t len = strlen(gameTimeStr);
    for (size_t idx = 0; idx < len; idx++)
    {
        // More SJISC conversion ?
        const s8 strChar = gameTimeStr[idx];
        if (strChar == ' ')
        {
            dst[2 * idx] = 0x81u;
            dst[2 * idx + 1] = 0x40;
        }
        else if (strChar == ':')
        {
            dst[2 * idx] = 0x81u;
            dst[2 * idx + 1] = 0x46;
        }
        else
        {
            dst[2 * idx] = 0x82u;
            dst[2 * idx + 1] = strChar + 0x1F;
        }
    }
}
Quicksave_PSX_Header sSaveHeader2_BB19F8 = {};
Quicksave_PSX_Header sSaveHeader1_BB17F8 = {};

void Quicksave_SaveToMemory_4C91A0(Quicksave* pSave)
{
    if (sActiveHero->mHealth > FP_FromInteger(0))
    {
        pSave->field_200_accumulated_obj_count = sAccumulatedObjectCount_5C1BF4;

        // Don't really know what the point of doing this is? Might as well just memset the pSave header?
        Quicksave_PSX_Header* pHeaderToUse = nullptr;
        if (bUseAltSaveHeader_5C1BBC == 0)
        {
            pHeaderToUse = &sSaveHeader1_BB17F8;
        }
        else
        {
            pHeaderToUse = &sSaveHeader2_BB19F8;
        }
        pSave->field_0_header = *pHeaderToUse;

        MEMCARD_Write_Timestamp_SJISC_String_4A2290(&pSave->field_0_header.field_0_frame_1_name[50]);

        char_type src[12] = {};
        sprintf(src, "%2sP%02dC%02d",
                Path_Get_Lvl_Name(gMap.mCurrentLevel),
                gMap.mCurrentPath,
                gMap.mCurrentCamera);
        MEMCARD_Write_SJISC_String_4A2770(reinterpret_cast<u8*>(src), &pSave->field_0_header.field_0_frame_1_name[32], 8);
        Quicksave_SaveWorldInfo(&pSave->field_204_world_info);
        pSave->field_45C_switch_states = gSwitchStates;

        u8* pDataIter = pSave->field_55C_objects_state_data;
        for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (!(pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead)))
            {
                pDataIter += pObj->VGetSaveState(pDataIter);
            }
        }

        // Write a u32 of 0
        pDataIter[0] = 0;
        pDataIter[1] = 0;
        pDataIter[2] = 0;
        pDataIter[3] = 0;

        Quicksave_SaveBlyData_4C9660(pDataIter + 4);
    }
}

void DoQuicksave()
{
    Game_ShowLoadingIcon_482D80();
    Path_Get_Bly_Record(gMap.mCurrentLevel, gMap.mCurrentPath);
    Quicksave_SaveToMemory_4C91A0(&sActiveQuicksaveData);
}

void Quicksave_ReadWorldInfo(const Quicksave_WorldInfo* pInfo)
{
    bUseAltSaveHeader_5C1BBC = pInfo->field_2E_use_alt_save_header;

    // Read all fields bar the last
    for (s32 i = 0; i < ALIVE_COUNTOF(pInfo->field_18_saved_killed_muds_per_zulag); i++)
    {
        sSavedKilledMudsPerZulag_5C1B50.mData[i] = pInfo->field_18_saved_killed_muds_per_zulag[i];
    }

    // Last is read from another field
    sSavedKilledMudsPerZulag_5C1B50.mData[ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData) - 1] = pInfo->field_17_last_saved_killed_muds_per_path;

    sActiveHero->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);
    sZulagNumber_5C1A20 = pInfo->field_2C_current_zulag_number;
    sKilledMudokons = pInfo->field_14_killed_muds;
    sRescuedMudokons = pInfo->field_12_saved_muds;
    sMudokonsInArea_5C1BC4 = pInfo->field_16_muds_in_area; // TODO: Check types
    gTotalMeterBars_5C1BFA = pInfo->field_2D_total_meter_bars;
    gbDrawMeterCountDown_5C1BF8 = pInfo->field_30_bDrawMeterCountDown;
    sGasTimer = pInfo->field_38_gas_timer;
    gAbeBulletProof_5C1BDA = pInfo->field_3C_bBulletProof;
    sVisitedBonewerks_5C1C02 = pInfo->field_32_visited_bonewerks;
    sVisitedBarracks_5C1C04 = pInfo->field_34_visited_barracks;
    sVisitedFeecoEnder_5C1C06 = pInfo->field_36_visited_feeco_ender;
    sGnFrame = pInfo->field_0_gnFrame;
}

void Quicksave_SaveWorldInfo(Quicksave_WorldInfo* pInfo)
{
    const PSX_RECT rect = sControlledCharacter->VGetBoundingRect();

    pInfo->field_0_gnFrame = sGnFrame;
    pInfo->field_4_level = MapWrapper::ToAE(gMap.mCurrentLevel);
    pInfo->field_6_path = gMap.mCurrentPath;
    pInfo->field_8_cam = gMap.mCurrentCamera;
    pInfo->field_2E_use_alt_save_header = bUseAltSaveHeader_5C1BBC;

    for (s32 i = 0; i < ALIVE_COUNTOF(pInfo->field_18_saved_killed_muds_per_zulag); i++)
    {
        pInfo->field_18_saved_killed_muds_per_zulag[i] = sSavedKilledMudsPerZulag_5C1B50.mData[i];
    }

    pInfo->field_17_last_saved_killed_muds_per_path = sSavedKilledMudsPerZulag_5C1B50.mData[ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData) - 1];

    pInfo->field_2C_current_zulag_number = sZulagNumber_5C1A20;
    pInfo->field_12_saved_muds = sRescuedMudokons;
    pInfo->field_14_killed_muds = sKilledMudokons;
    pInfo->field_16_muds_in_area = static_cast<s8>(sMudokonsInArea_5C1BC4); // TODO: Check types
    pInfo->field_2D_total_meter_bars = gTotalMeterBars_5C1BFA;
    pInfo->field_30_bDrawMeterCountDown = gbDrawMeterCountDown_5C1BF8;
    pInfo->field_3C_bBulletProof = gAbeBulletProof_5C1BDA;
    pInfo->field_32_visited_bonewerks = sVisitedBonewerks_5C1C02;
    pInfo->field_34_visited_barracks = sVisitedBarracks_5C1C04;
    pInfo->field_36_visited_feeco_ender = sVisitedFeecoEnder_5C1C06;
    pInfo->field_38_gas_timer = sGasTimer;
    pInfo->field_C_controlled_x = FP_GetExponent(sControlledCharacter->mXPos);
    pInfo->field_E_controlled_y = rect.h;
    pInfo->field_10_controlled_scale = sControlledCharacter->GetSpriteScale() == FP_FromDouble(1.0);
}

s32 Sort_comparitor_4D42C0(const void* pSaveRecLeft, const void* pSaveRecRight)
{
    const s32 leftTime = reinterpret_cast<const SaveFileRec*>(pSaveRecLeft)->field_20_lastWriteTimeStamp;
    const s32 rightTime = reinterpret_cast<const SaveFileRec*>(pSaveRecRight)->field_20_lastWriteTimeStamp;

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
    sTotalSaveFilesCount_BB43E0 = 0;

    IO_EnumerateDirectory("*.sav", [](const char_type* fileName, u32 lastWriteTime)
                          {
                              if (sTotalSaveFilesCount_BB43E0 < 128)
                              {
                                  size_t fileNameLen = strlen(fileName) - 4;
                                  if (fileNameLen > 0)
                                  {
                                      // Limit length to prevent buffer overflow
                                      if (fileNameLen > 20)
                                      {
                                          fileNameLen = 20;
                                      }

                                      SaveFileRec* pRec = &sSaveFileRecords_BB31D8[sTotalSaveFilesCount_BB43E0];
                                      memcpy(pRec->field_0_fileName, fileName, fileNameLen);
                                      pRec->field_0_fileName[fileNameLen] = 0;

                                      pRec->field_20_lastWriteTimeStamp = lastWriteTime;
                                      sTotalSaveFilesCount_BB43E0++;
                                  }
                              }
                          });

    // Sort all we've found by time stamp, users probably want to load their last save first
    qsort(sSaveFileRecords_BB31D8, sTotalSaveFilesCount_BB43E0, sizeof(SaveFileRec), Sort_comparitor_4D42C0);

    // Underflow
    if (sSavedGameToLoadIdx_BB43FC < 0)
    {
        sSavedGameToLoadIdx_BB43FC = 0;
    }

    // Overflow
    if (sSavedGameToLoadIdx_BB43FC >= sTotalSaveFilesCount_BB43E0)
    {
        sSavedGameToLoadIdx_BB43FC = sTotalSaveFilesCount_BB43E0 - 1;
    }
}
