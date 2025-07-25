#include "AOSaveConverter.hpp"

static PSX_RECT From(const AO::AOData::PSX_RECT& data)
{
    PSX_RECT d;
    d.x = data.x;
    d.y = data.y;
    d.w = data.w;
    d.h = data.h;
    return d;
}

static PSX_Point From(const AO::AOData::PSX_Point& data)
{
    PSX_Point d;
    d.x = data.x;
    d.y = data.y;
    return d;
}

static eLineTypes From(const AO::AOData::eLineTypes type)
{
    switch (type)
    {
        case AO::AOData::eLineTypes::eNone_m1:
            return ::eLineTypes::eNone_m1;
        case AO::AOData::eLineTypes::eFloor_0:
            return ::eLineTypes::eFloor_0;
        case AO::AOData::eLineTypes::eWallLeft_1:
            return ::eLineTypes::eWallLeft_1;
        case AO::AOData::eLineTypes::eWallRight_2:
            return ::eLineTypes::eWallRight_2;
        case AO::AOData::eLineTypes::eCeiling_3:
            return ::eLineTypes::eCeiling_3;
        case AO::AOData::eLineTypes::eBackgroundFloor_4:
            return ::eLineTypes::eBackgroundFloor_4;
        case AO::AOData::eLineTypes::eBackgroundWallLeft_5:
            return ::eLineTypes::eBackgroundWallLeft_5;
        case AO::AOData::eLineTypes::eBackgroundWallRight_6:
            return ::eLineTypes::eBackgroundWallRight_6;
        case AO::AOData::eLineTypes::eBackgroundCeiling_7:
            return ::eLineTypes::eBackgroundCeiling_7;
        case AO::AOData::eLineTypes::eTrackLine_8:
            return ::eLineTypes::eTrackLine_8;
        case AO::AOData::eLineTypes::eArt_9:
            return ::eLineTypes::eArt_9;
        case AO::AOData::eLineTypes::eBulletWall_10:
            return ::eLineTypes::eBulletWall_10;
        case AO::AOData::eLineTypes::eMineCarFloor_11:
            return ::eLineTypes::eMineCarFloor_11;
        case AO::AOData::eLineTypes::eMineCarWall_12:
            return ::eLineTypes::eMineCarWall_12;
        case AO::AOData::eLineTypes::eMineCarCeiling_13:
            return ::eLineTypes::eMineCarCeiling_13;
        case AO::AOData::eLineTypes::eBackgroundMineCarFloor_14:
            return ::eLineTypes::eBackgroundMineCarFloor_14;
        case AO::AOData::eLineTypes::eBackgroundMineCarWall_15:
            return ::eLineTypes::eBackgroundMineCarWall_15;
        case AO::AOData::eLineTypes::eBackgroundMineCarCeiling_16:
            return ::eLineTypes::eBackgroundMineCarCeiling_16;
        case AO::AOData::eLineTypes::eFlyingObjectWall_17:
            return ::eLineTypes::eFlyingObjectWall_17;
        case AO::AOData::eLineTypes::eBackgroundFlyingObjectWall_18:
            return ::eLineTypes::eBackgroundFlyingObjectWall_18;
        case AO::AOData::eLineTypes::eDynamicCollision_32:
            return ::eLineTypes::eDynamicCollision_32;
        case AO::AOData::eLineTypes::eBackgroundDynamicCollision_36:
            return ::eLineTypes::eBackgroundDynamicCollision_36;
    }
    ALIVE_FATAL("Bad line type value");
}

AO::SaveData AOSaveConverter::Convert(const AO::AOData::SaveData& data)
{
    AO::SaveData d;

    d.mAbe_ContinuePointZoneNumber = data.mContinuePoint_ZoneNumber;
    d.mAbe_ContinuePointClearFromId = data.mContinuePoint_ClearFromId;
    d.mAbe_ContinuePointClearToId = data.mContinuePoint_ClearToId;
    d.mAbe_ContinuePointTopLeft = From(data.mContinuePoint_TopLeft);
    d.mAbe_ContinuePointBottomRight = From(data.mContinuePoint_BottomRight);
    d.mAbe_ContinuePointLevel = MapWrapper::FromAOSaveData(data.mContinuePoint_Level);
    d.mAbe_ContinuePointPath = data.mContinuePoint_Path;
    d.mAbe_ContinuePoint_Camera = data.mContinuePoint_Camera;
    d.mAbe_ContinuePointSpriteScale = data.mContinuePoint_SpriteScale;
    d.mAbe_SavedRingTimer = data.field_21C_saved_ring_timer;
    d.mAbe_SavedHaveShrykull = data.mActiveHero_SavedHaveShrykull;
    d.mAbe_XPos = data.mActiveHero_XPos;
    d.mAbe_YPos = data.mActiveHero_YPos;
    d.mAbe_Health = data.mActiveHero_Health;
    d.mAbe_SpriteScale = data.mActiveHero_SpriteScale;
    d.mCurrentLevel = MapWrapper::FromAOSaveData(data.mCurrentLevel);
    d.mCurrentPath = data.mCurrentPath;
    d.mCurrentCamera = data.mCurrentCamera;
    d.mAbe_LineType = From(static_cast<AO::AOData::eLineTypes>(data.field_23A_mode_mask)); // TODO: check if this is correct
    d.mAbe_FlipX = data.mActiveHero_FlipX;
    d.mAbe_CurrentMotion = static_cast<AO::eAbeMotions>(data.mActiveHero_CurrentMotion); // TODO: enum conversion
    d.mAbe_CurrentFrame = data.mActiveHero_CurrentFrame;
    d.mAbe_StoneState = data.field_244_stone_state; // TODO: likely needs an enum/conversion
    d.mAbe_GnFrame = data.field_248_gnFrame;
    d.mAbe_Timer = data.field_24C_field_118;
    d.mAbe_ThrowableCount = data.field_250_throwable_count;
    d.mAbe_ParamoniaDone = data.mActiveHero_ParamoniaDone;
    d.mAbe_ScrabaniaDone = data.mActiveHero_ScrabaniaDone;
    d.mAbe_RingPulseTimer = data.field_254_ring_pulse_timer;
    d.mAbe_HaveShrykull = data.mActiveHero_HaveShrykull;
    d.mElumExists = data.mElumExists;
    d.mControllingElum = data.mControllingElum;
    d.mElum_RespawnOnDead = data.mElum_RespawnOnDead;
    d.mElum_CurrentLevel = MapWrapper::FromAOSaveData(data.mElum_ContinueLevel);
    d.mElum_CurrentPath = data.mElum_CurrentPath;
    d.mInfiniteGrenades = data.mInfiniteGrenades;
    d.mElum_XPos = data.mElum_XPos;
    d.mElum_YPos = data.mElum_YPos;
    d.mElum_LineType = From(static_cast<AO::AOData::eLineTypes>(data.mElum_LineType));
    d.mElum_FlipX = data.mElum_FlipX;
    d.mElum_CurrentMotion = static_cast<AO::eElumMotions>(data.mElum_CurrentMotion); // TODO: enum conversion
    d.mElum_DontFollowAbe = data.mElum_DontFollowAbe;
    d.mElum_BrainIdx = data.mElum_BrainIdx;
    d.mElum_BrainSubState = data.mElum_BrainSubState;
    d.mElum_HoneyXPos = data.mElum_HoneyXPos;
    d.mElum_HoneyCamera = data.mElum_HoneyCamera;
    d.mElum_StrugglingWithBees = data.mElum_StrugglingWithBees;
    d.mElum_StungByBees = data.mElum_StungByBees;
    d.mElum_Falling = data.mElum_Falling;
    d.mElum_FoundHoney = data.mElum_FoundHoney;
    d.mElum_ContinuePointRect = From(data.mElum_ContinueRect);
    d.mElum_PreviousContinueZonePointNumber = data.mElum_PreviousContinueZoneNumber;
    d.mElum_AbeZoneNumber = data.mElum_AbeZoneNumber;
    d.mElum_ContinuePointPath = data.mElum_ContinuePath;
    d.mElum_ContinuePointLevel = MapWrapper::FromAOSaveData(data.mElum_ContinueLevel);
    d.mElum_ContinuePointSpriteScale = data.mElum_ContinueSpriteScale;
    d.mRescuedMudokons = data.mRescuedMudokons;
    d.mKilledMudokons = data.mKilledMudokons;
    d.field_2A4_restartRuptureFarmsSavedMudokons = data.field_2A4_restartRuptureFarmsSavedMudokons;
    d.mRestartRuptureFarmsKilledMuds = data.mRestartRuptureFarmsKilledMuds;
    d.mDeathGasTimer = data.field_2A8_gasTimer;
    d.mCurrentControllerIdx = data.mCurrentControllerIdx;

    // TODO: switch states and bly data
    memcpy(d.field_2B0_pSaveBuffer, data.field_2B0_pSaveBuffer, sizeof(data.field_2B0_pSaveBuffer));

    return d;
}