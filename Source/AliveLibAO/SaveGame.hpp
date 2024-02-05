#pragma once

#include "../relive_lib/Function.hpp"
#include "PathData.hpp"
#include "../relive_lib/Psx.hpp"

namespace AO {

// Note this is the "relive" copy/format of the save data
struct SaveData final
{
    s16 mContinuePoint_ZoneNumber;
    s16 mContinuePoint_ClearFromId;
    s16 mContinuePoint_ClearToId;
    PSX_Point mContinuePoint_TopLeft;
    PSX_Point mContinuePoint_BottomRight;
    EReliveLevelIds mContinuePoint_Level;
    s16 mContinuePoint_Path;
    s16 mContinuePoint_Camera;
    FP mContinuePoint_SpriteScale;
    s32 field_21C_saved_ring_timer;
    s16 mActiveHero_SavedHaveShrykull;
    s32 mActiveHero_XPos;
    u32 mActiveHero_YPos;
    FP mActiveHero_Health;
    FP mActiveHero_SpriteScale;
    EReliveLevelIds mCurrentLevel;
    u16 mCurrentPath;
    s16 mCurrentCamera;
    s16 field_23A_mode_mask;
    s16 mActiveHero_FlipX;
    eAbeMotions mActiveHero_CurrentMotion;
    u16 mActiveHero_CurrentFrame;
    s32 field_244_stone_state; //TODO @ 100%  fix type length
    u32 field_248_gnFrame;
    s32 field_24C_field_118;
    s16 field_250_throwable_count;
    s8 mActiveHero_ParamoniaDone;
    s8 mActiveHero_ScrabaniaDone;
    s32 field_254_ring_pulse_timer;
    s16 mActiveHero_HaveShrykull;
    u16 mElumExists;
    s16 mControllingElum;
    u16 mElum_RespawnOnDead;
    EReliveLevelIds mElum_CurrentLevel;
    u16 mElum_CurrentPath;
    s16 mInfiniteGrenades;
    u32 mElum_XPos;
    u32 mElum_YPos;
    s16 mElum_LineType;
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
    PSX_RECT mElum_ContinueRect;
    u16 mElum_PreviousContinueZoneNumber;
    u16 mElum_AbeZoneNumber;
    u16 mElum_ContinuePath;
    EReliveLevelIds mElum_ContinueLevel;
    FP mElum_ContinueSpriteScale;
    u16 mRescuedMudokons;
    u16 mKilledMudokons;
    u16 field_2A4_restartRuptureFarmsSavedMudokons;
    u16 mRestartRuptureFarmsKilledMuds;
    s32 field_2A8_gasTimer;
    s16 field_2AC_bUseAltSaveHeader;
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
