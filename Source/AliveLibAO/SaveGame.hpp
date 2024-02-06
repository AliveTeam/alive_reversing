#pragma once

#include "../relive_lib/Function.hpp"
#include "PathData.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/Collisions.hpp"

// Just for the motion enums
#include "Abe.hpp"
#include "Elum.hpp"

namespace AO {

// Note this is the "relive" copy/format of the save data
struct SaveData final
{
    s16 mAbe_ContinuePointZoneNumber;
    s16 mAbe_ContinuePointClearFromId;
    s16 mAbe_ContinuePointClearToId;
    PSX_Point mAbe_ContinuePointTopLeft;
    PSX_Point mAbe_ContinuePointBottomRight;
    EReliveLevelIds mAbe_ContinuePointLevel;
    s16 mAbe_ContinuePointPath;
    s16 mAbe_ContinuePoint_Camera;
    FP mAbe_ContinuePointSpriteScale;
    s32 mAbe_SavedRingTimer;
    s16 mAbe_SavedHaveShrykull;
    s32 mAbe_XPos;
    u32 mAbe_YPos;
    FP mAbe_Health;
    FP mAbe_SpriteScale;
    EReliveLevelIds mCurrentLevel;
    u16 mCurrentPath;
    s16 mCurrentCamera;
    eLineTypes mAbe_LineType;
    s16 mAbe_FlipX;
    eAbeMotions mAbe_CurrentMotion;
    u16 mAbe_CurrentFrame;
    s32 mAbe_StoneState; //TODO @ 100%  fix type length
    u32 mAbe_GnFrame;
    s32 mAbe_Timer;
    s16 mAbe_ThrowableCount;
    s8 mAbe_ParamoniaDone;
    s8 mAbe_ScrabaniaDone;
    s32 mAbe_RingPulseTimer;
    s16 mAbe_HaveShrykull;
    u16 mElumExists;
    s16 mControllingElum;
    u16 mElum_RespawnOnDead;
    EReliveLevelIds mElum_CurrentLevel;
    u16 mElum_CurrentPath;
    s16 mInfiniteGrenades;
    u32 mElum_XPos;
    u32 mElum_YPos;
    eLineTypes mElum_LineType;
    s16 mElum_FlipX;
    eElumMotions mElum_CurrentMotion;
    u16 mElum_DontFollowAbe;
    u16 mElum_BrainIdx;
    u16 mElum_BrainSubState;
    u32 mElum_HoneyXPos;
    u16 mElum_HoneyCamera;
    s8 mElum_StrugglingWithBees;
    s8 mElum_StungByBees;
    s8 mElum_Falling;
    s8 mElum_FoundHoney;
    PSX_RECT mElum_ContinuePointRect;
    u16 mElum_PreviousContinueZonePointNumber;
    u16 mElum_AbeZoneNumber;
    u16 mElum_ContinuePointPath;
    EReliveLevelIds mElum_ContinuePointLevel;
    FP mElum_ContinuePointSpriteScale;
    u16 mRescuedMudokons;
    u16 mKilledMudokons;
    u16 field_2A4_restartRuptureFarmsSavedMudokons;
    u16 mRestartRuptureFarmsKilledMuds;
    s32 mDeathGasTimer;
    s16 mCurrentControllerIdx;
    u8 field_2B0_pSaveBuffer[7501];
};

class SaveGame final
{
public:
    static void LoadFromMemory(SaveData* pData, s32 bKillObjects);
    static void SaveToMemory(SaveData* pSaveData);
    static s16 LoadFromFile(const char_type* name);
    static bool SaveToFile(const char_type* name);

    static s16 GetPathId(s16 pathToFind, s16* outFoundPathRow = nullptr);
};

extern const s8 word_4BC670[6][8];

} // namespace AO
