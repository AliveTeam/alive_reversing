#pragma once

#include "../../AliveLibAO/SaveGame.hpp"

namespace AO {

// Any enum/struct in the AOData namespace is related to OG data and can't ever be changed
// otherwise interpreting the OG data will break.
namespace AOData {

struct PSX_RECT final
{
    s16 x, y, w, h;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(PSX_RECT, 8);

struct PSX_Point final
{
    s16 x, y;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(PSX_Point, 4);

// TODO: check if the enum is the same as AE
enum eLineTypes : s16
{
    eNone_m1 = -1,
    eFloor_0 = 0,
    eWallLeft_1 = 1,
    eWallRight_2 = 2,
    eCeiling_3 = 3,
    eBackgroundFloor_4 = 4,
    eBackgroundWallLeft_5 = 5,
    eBackgroundWallRight_6 = 6,
    eBackgroundCeiling_7 = 7,
    eTrackLine_8 = 8,
    eArt_9 = 9,
    eBulletWall_10 = 10,
    eMineCarFloor_11 = 11,
    eMineCarWall_12 = 12,
    eMineCarCeiling_13 = 13,
    eBackgroundMineCarFloor_14 = 14,
    eBackgroundMineCarWall_15 = 15,
    eBackgroundMineCarCeiling_16 = 16,
    eFlyingObjectWall_17 = 17,
    eBackgroundFlyingObjectWall_18 = 18,

    eDynamicCollision_32 = 32,
    eBackgroundDynamicCollision_36 = 36,
};

struct Save_PSX_Header final
{
    u8 field_0_frame_1_name[128];
    s8 field_80_frame_2_padding[128];
    s8 field_100_frame_3_padding[128];
    s8 field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Save_PSX_Header, 0x200);

struct SaveData final
{
    Save_PSX_Header mSavePsxHeader;
    s32 mSaveHashValue;
    s16 mContinuePoint_ZoneNumber;
    s16 mContinuePoint_ClearFromId;
    s16 mContinuePoint_ClearToId;
    PSX_Point mContinuePoint_TopLeft;
    PSX_Point mContinuePoint_BottomRight;
    LevelIds mContinuePoint_Level;
    s16 mContinuePoint_Path;
    s16 mContinuePoint_Camera;
    FP mContinuePoint_SpriteScale;
    s32 field_21C_saved_ring_timer;
    s16 mActiveHero_SavedHaveShrykull;
    s16 field_222_pad;
    s32 mActiveHero_XPos;
    u32 mActiveHero_YPos;
    FP mActiveHero_Health;
    FP mActiveHero_SpriteScale;
    LevelIds mCurrentLevel;
    u16 mCurrentPath;
    s16 mCurrentCamera;
    s16 field_23A_mode_mask;
    s16 mActiveHero_FlipX;
    s16 mActiveHero_CurrentMotion;
    u16 mActiveHero_CurrentFrame;
    s16 field_242_pad;
    s32 field_244_stone_state; // TODO @ 100%  fix type length
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
    LevelIds mElum_CurrentLevel;
    u16 mElum_CurrentPath;
    s16 mInfiniteGrenades;
    s16 field_266_pad;
    u32 mElum_XPos;
    u32 mElum_YPos;
    s16 mElum_LineType;
    s16 mElum_FlipX;
    s16 mElum_CurrentMotion;
    u16 mElum_DontFollowAbe;
    u16 mElum_BrainIdx;
    u16 mElum_BrainSubState;
    u32 mElum_HoneyXPos;
    u16 mElum_HoneyCamera;
    s16 field_282_pad;
    u32 field_284_unused;
    s8 mElum_StrugglingWithBees;
    s8 mElum_StungByBees;
    s8 mElum_Falling;
    s8 mElum_FoundHoney;
    PSX_RECT mElum_ContinueRect;
    u16 mElum_PreviousContinueZoneNumber;
    u16 mElum_AbeZoneNumber;
    u16 mElum_ContinuePath;
    LevelIds mElum_ContinueLevel;
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
ALIVE_ASSERT_SIZEOF_ALWAYS(SaveData, 0x2000);

} // namespace AOData

} // namespace AO


class AOSaveConverter final
{
public:
    AO::SaveData Convert(const AO::AOData::SaveData& data);
};
