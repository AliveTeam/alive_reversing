#pragma once

#include "../relive_lib/Function.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Abe.hpp"

struct Quicksave_PSX_Header final
{
    u8 field_0_frame_1_name[128];
    s8 field_80_frame_2_padding[128];
    s8 field_100_frame_3_padding[128];
    s8 field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF(Quicksave_PSX_Header, 0x200);

enum class LevelIds : s16;

struct Quicksave_WorldInfo final
{
    s32 mGnFrame;
    LevelIds mLevel;
    s16 mPath;
    s16 mCam;
    s16 mSaveFileId;
    s16 mControlledCharX;
    s16 mControlledCharY;
    s16 mControlledCharScale;
    s16 mRescuedMudokons;
    s16 mKilledMudokons;
    s8 field_16_muds_in_area;
    s8 field_17_last_saved_killed_muds_per_path;
    s8 field_18_saved_killed_muds_per_zulag[20];
    s8 field_2C_current_zulag_number;
    s8 mTotalMeterBars;
    s16 field_2E_use_alt_save_header;
    s16 field_30_bDrawMeterCountDown;
    s16 mVisitedBonewerkz;
    s16 mVisitedBarracks;
    s16 mVisitedFeecoEnder;
    s32 mGasTimer;
    s16 mAbeInvincible;
    s16 field_3E_padding;
};

struct Quicksave final
{
    Quicksave_PSX_Header mPsxHeader;
    s32 mAccumulatedObjCount;
    Quicksave_WorldInfo mWorldInfo;
    Quicksave_WorldInfo mRestartPathWorldInfo;
    AbeSaveState mRestartPathAbeState;
    SwitchStates mRestartPathSwitchStates;
    SwitchStates mSwitchStates;
    u8 mObjectsStateData[6820];
};
ALIVE_ASSERT_SIZEOF(Quicksave, 0x2000);

struct SaveFileRec final
{
    char_type mFileName[32];
    u32 mLastWriteTimeStamp;
};
ALIVE_ASSERT_SIZEOF(SaveFileRec, 0x24);

extern SaveFileRec gSaveFileRecords[128];
extern Quicksave gActiveQuicksaveData;
extern s32 gSavedGameToLoadIdx;
extern s32 gTotalSaveFilesCount;
extern u16 sQuickSave_saved_switchResetters_count_BB234C;

void Quicksave_LoadActive();
void DoQuicksave();
void Quicksave_ReadWorldInfo(const Quicksave_WorldInfo* pInfo);
void Quicksave_SaveWorldInfo(Quicksave_WorldInfo* pInfo);
void Quicksave_FindSaves();
void QuikSave_RestoreBlyData(const u8* pSaveData);
void Quicksave_SaveSwitchResetterStates();
void Quicksave_RestoreSwitchResetterStates();
