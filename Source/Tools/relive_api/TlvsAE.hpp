#pragma once

#include "TlvObjectBaseAE.hpp"
#include "TlvObjectBaseMacros.hpp"
#include "../../relive_lib/data_conversion/PathTlvsAE.hpp"
#include "../../relive_lib/AnimResources.hpp"

#define CTOR_AE(className, objectTypeName, tlvEnumType)\
    className() : TlvObjectBaseAE(sizeof(::className), tlvEnumType, objectTypeName, &mTlv)\
    {\
    }\
    \
    className(ReliveAPI::TypesCollectionBase& globalTypes, const Path_TLV* pTlvSrc = nullptr) \
        : TlvObjectBaseAE(sizeof(::className), globalTypes, tlvEnumType, objectTypeName, &mTlv)\
    {\
        if (pTlvSrc)\
        {\
            mTlv = *static_cast<const ::className*>(pTlvSrc);\
            ConvertXYPos(); \
        }\
        else\
        {\
            mPSelfTlv->mLength = static_cast<s16>(mSizeOfT);\
            mPSelfTlv->mTlvType32.mType = mType;\
        }\
        AddProperties(globalTypes);\
    }\
    ::className mTlv = {};\
    void AddProperties(ReliveAPI::TypesCollectionBase& globalTypes)

#define EMPTY_CTOR_AE() (void) globalTypes

struct Path_Null_63 final : public Path_TLV
{
    // TODO: check
    s16 padding1;
    s16 padding2;
};

namespace AETlvs {

struct Path_TimedMine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_TimedMine, "TimedMine", TlvTypes::TimedMine_14)
    {
        ADD("Switch ID (Unused?)", mTlv.mSwitchId);
        ADD("State (Unused?)", mTlv.mState);
        ADD("Scale", mTlv.mScale);
        ADD("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_ElectricWall final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_ElectricWall::ElectricWallStartState>("Enum_ElectricWallStartState",
        {
            {::Path_ElectricWall::ElectricWallStartState::eOff_0, "Off"},
            {::Path_ElectricWall::ElectricWallStartState::eOn_1, "On"},
        });
    }

    CTOR_AE(Path_ElectricWall, "ElectricWall", TlvTypes::ElectricWall_38)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Start State", mTlv.mStartState);
    }
};
struct Path_Mudokon final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Mudokon::Mud_TLV_Emotion>("Enum_Mud_TLV_Emotion",
        {
            {::Path_Mudokon::Mud_TLV_Emotion::eNormal_0, "Normal"},
            {::Path_Mudokon::Mud_TLV_Emotion::eAngry_1, "Angry"},
            {::Path_Mudokon::Mud_TLV_Emotion::eSad_2, "Sad"},
            {::Path_Mudokon::Mud_TLV_Emotion::eWired_3, "Wired"},
            {::Path_Mudokon::Mud_TLV_Emotion::eSick_4, "Sick"}
        });

        types.AddEnum<::Path_Mudokon::MudJobs>("Enum_Mud_State",
        {
            {::Path_Mudokon::MudJobs::eChisle_0, "Chisle"},
            {::Path_Mudokon::MudJobs::eSitScrub_1, "Scrub"},
            {::Path_Mudokon::MudJobs::eAngryWorker_2, "Angry Worker"},
            {::Path_Mudokon::MudJobs::eDamageRingGiver_3, "Damage Ring Giver"},
            {::Path_Mudokon::MudJobs::eHealthRingGiver_4, "Health Ring Giver"},
        });
    }

    CTOR_AE(Path_Mudokon, "Mudokon", TlvTypes::Mudokon_49)
    {
        ADD("Scale", mTlv.mScale);
        ADD("State", mTlv.mJob);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Voice Pitch", mTlv.mVoicePitch);
        ADD("Rescue Switch ID", mTlv.mRescueSwitchId);
        ADD_HIDDEN("Deaf (Unused?)", mTlv.mDeaf); // logic for this only exists in AO
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Persist & Reset Offscreen", mTlv.mPersistAndResetOffscreen);
        ADD("Emotion", mTlv.mEmotion);
        ADD("Blind", mTlv.mBlind);
        ADD("Angry Switch ID", mTlv.mAngrySwitchId);
        ADD("Work After Turning Wheel", mTlv.mWorkAfterTurningWheel);
        ADD("Gets Depressed", mTlv.mGetsDepressed);
        ADD("Ring Pulse Interval", mTlv.mRingPulseInterval);
        ADD("Give Ring Without Password", mTlv.mGiveRingWithoutPassword);
    }
};

struct Path_BirdPortal final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_BirdPortal::PortalType>("Enum_PortalType",
        {
            {::Path_BirdPortal::PortalType::eAbe_0, "Abe"},
            {::Path_BirdPortal::PortalType::eWorker_1, "Worker"},
            {::Path_BirdPortal::PortalType::eShrykull_2, "Shrykull"},
        });

        types.AddEnum<::PortalSide>("Enum_PortalSide",
        {
            {::PortalSide::eRight_0, "Right"},
            {::PortalSide::eLeft_1, "Left"},
        });
    }

    CTOR_AE(Path_BirdPortal, "BirdPortal", TlvTypes::BirdPortal_28)
    {
        ADD("Enter Side", mTlv.mEnterSide);
        ADD("BirdPortalExit Level", mTlv.mExitLevel);
        ADD("BirdPortalExit Path", mTlv.mExitPath);
        ADD("BirdPortalExit Camera", mTlv.mExitCamera);
        ADD("Scale", mTlv.mScale);
        ADD("Movie ID", mTlv.mMovieId);
        ADD("Portal Type", mTlv.mPortalType);
        ADD("Mudokon Amount For Shrykull", mTlv.mMudCountForShrykull);
        ADD("Create Portal Switch ID", mTlv.mCreatePortalSwitchId);
        ADD("Delete Portal Switch ID", mTlv.mDeletePortalSwitchId);
    }
};

struct Path_LCDStatusBoard final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_LCDStatusBoard, "LCDStatusBoard", TlvTypes::LCDStatusBoard_64)
    {
        ADD("Number Of Mudokons", mTlv.mNumberOfMuds);
        ADD("Zulag Number", mTlv.mZulagNumber);
        ADD("Hide Board", mTlv.mHideBoard);
    }
};

struct Path_Door final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Door::DoorStates>("Enum_DoorStates",
        {
            {::Path_Door::DoorStates::eOpen_0, "Open"},
            {::Path_Door::DoorStates::eClosed_1, "Closed"},
            {::Path_Door::DoorStates::eOpening_2, "Opening"},
            {::Path_Door::DoorStates::eClosing_3, "Closing"}
        });

        types.AddEnum<::Path_Door::DoorTypes>("Enum_DoorTypes",
        {
            {::Path_Door::DoorTypes::eBasicDoor_0, "Basic Door"},
            {::Path_Door::DoorTypes::eDoorPadding_1, "Padding (ignore)"},
            {::Path_Door::DoorTypes::eTasksDoorWithSecretMusic_2, "Tasks Door With Secret Music"},
            {::Path_Door::DoorTypes::eTasksDoor_3, "Tasks Door"}
        });
    }

    CTOR_AE(Path_Door, "Door", TlvTypes::Door_5)
    {
        ADD("Level", mTlv.mNextLevel);
        ADD("Path", mTlv.mNextPath);
        ADD("Camera", mTlv.mNextCamera);
        ADD("Scale", mTlv.mScale);
        ADD("Door Number", mTlv.mDoorId);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Target Door ID", mTlv.mTargetDoorNumber);
        ADD("Door Type", mTlv.mDoorType);
        ADD("Start State", mTlv.mStartState);
        ADD("Hub 1 ID", mTlv.mHub1);
        ADD("Hub 2 ID", mTlv.mHub2);
        ADD("Hub 3 ID", mTlv.mHub3);
        ADD("Hub 4 ID", mTlv.mHub4);
        ADD("Hub 5 ID", mTlv.mHub5);
        ADD("Hub 6 ID", mTlv.mHub6);
        ADD("Hub 7 ID", mTlv.mHub7);
        ADD("Hub 8 ID", mTlv.mHub8);
        ADD("Wipe Effect", mTlv.mWipeEffect);
        ADD("Movie Number", mTlv.mMovieId);
        ADD("X Offset", mTlv.mDoorOffsetX);
        ADD("Y Offset", mTlv.mDoorOffsetY);
        ADD("Wipe X Org (unused?)", mTlv.field_3A_wipe_x_org);
        ADD("Wipe Y Org (unused?)", mTlv.field_3C_wipe_y_org);
        ADD("Abe Direction On Exit", mTlv.mExitDirection);
        ADD("Close On Exit", mTlv.mCloseOnExit);
        ADD("Clear Throwables", mTlv.mClearThrowables);
    }
};

struct Path_Lever final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Lever::LeverSoundType>("Enum_LeverSoundType",
        {
            {::Path_Lever::LeverSoundType::eNone, "None"},
            {::Path_Lever::LeverSoundType::eWell_1, "Well"},
            {::Path_Lever::LeverSoundType::eSwitchBellHammer_2, "Unknown"},
            {::Path_Lever::LeverSoundType::eDoor_3, "Door"},
            {::Path_Lever::LeverSoundType::eElectricWall_4, "Electric Wall"},
            {::Path_Lever::LeverSoundType::eSecurityOrb_5, "Security Orb"},
            {::Path_Lever::LeverSoundType::eLift_6, "Lift"}
        });

        types.AddEnum<::Path_Lever::LeverSoundDirection>("Enum_LeverSoundDirection",
        {
            {::Path_Lever::LeverSoundDirection::eLeftAndRight_0, "Left And Right"},
            {::Path_Lever::LeverSoundDirection::eLeft_1, "Left"},
            {::Path_Lever::LeverSoundDirection::eRight_2, "Right"},
        });
    }

    CTOR_AE(Path_Lever, "Lever", TlvTypes::Lever_17)
    {
        ADD("Action", mTlv.mAction);
        ADD("Scale", mTlv.mScale);
        ADD("On Sound", mTlv.mOnSound);
        ADD("Off Sound", mTlv.mOffSound);
        ADD("Sound Direction", mTlv.mSoundDirection);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Path_Hoist final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Hoist::Type>("Enum_HoistType",
        {
            {::Path_Hoist::Type::eNextEdge, "Next Edge"},
            {::Path_Hoist::Type::eNextFloor, "Next Floor"},
            {::Path_Hoist::Type::eOffScreen, "Off Screen"},
        });

        types.AddEnum<::Path_Hoist::GrabDirection>("Enum_HoistGrabDirection",
        {
            {::Path_Hoist::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
            {::Path_Hoist::GrabDirection::eFacingLeft, "Facing Left"},
            {::Path_Hoist::GrabDirection::eFacingRight, "Facing Right"},
        });
    }

    CTOR_AE(Path_Hoist, "Hoist", TlvTypes::Hoist_2)
    {
        ADD("Hoist Type", mTlv.mHoistType);
        ADD("Grab Direction", mTlv.mGrabDirection);
        ADD("Switch ID (Unused?)", mTlv.field_14_switch_id);
        ADD("Scale", mTlv.field_16_scale);
    }
};

struct Path_BoomMachine final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_BoomMachine::NozzleSide>("Enum_BoomMachineNozzleSide",
            {
                {::Path_BoomMachine::NozzleSide::eRight_0, "Right"},
                {::Path_BoomMachine::NozzleSide::eLeft_1, "Left"},
            });
    }

    CTOR_AE(Path_BoomMachine, "BoomMachine", TlvTypes::BoomMachine_59)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Nozzle Side", mTlv.mNozzleSide);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Number Of Grenades", mTlv.mGrenadeAmount);

        ADD_RESOURCE(AnimId::Mudokon_GrenadeMachineUse, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_HandstoneBegin, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_HandstoneEnd, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Button_Off, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Button_On, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Nozzle_DropGrenade, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Nozzle_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Bone, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
    }
};

// TODO: figure out if some of these fields are unused and sync changes with Path_SligGetPants/Path_SligSpawner
struct Path_Slig final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Slig, "Slig", TlvTypes::Slig_15)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start State", mTlv.mStartState);
        ADD("Pause Time", mTlv.mPauseTime);
        ADD("Pause Left Min", mTlv.mPauseLeftMin);
        ADD("Pause Left Max", mTlv.mPauseLeftMax);
        ADD("Pause Right Min", mTlv.mPauseRightMin);
        ADD("Pause Right Max", mTlv.mPauseRightMax);
        ADD("Shoot Possessed Sligs", mTlv.mShootPossessedSligs);
        ADD("Shoot On Sight Delay", mTlv.mShootOnSightDelay);
        ADD("Bullet Shoot Count", mTlv.mNumTimesToShoot);
        ADD("unknown1", mTlv.field_24_unused);

        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("Chase Abe When Spotted", mTlv.mChaseAbeWhenSpotted);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Panic Timeout", mTlv.mPanicTimeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.field_30_num_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.field_32_panic_sound_timeout);
        ADD("Stop Chase Delay", mTlv.mStopChaseDelay);
        ADD("Time To Wait Before Chase", mTlv.mTimeToWaitBeforeChase);
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Alerted Listen Time", mTlv.mAlertedListenTime);

        ADD("Percent Say What", mTlv.mPercentSayWhat);
        ADD("Percent Beat Mudokon", mTlv.mPercentBeatMud);
        ADD_HIDDEN("Talk To Abe (Unused?)", mTlv.field_40_talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.field_42_dont_shoot);
        ADD("Z Shoot Delay", mTlv.mZShootDelay);
        ADD("Stay Awake", mTlv.mStayAwake);
        ADD("Disable Resources", mTlv.mDisabledResources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.mNoiseWakeUpDistance);
        ADD("Slig Spawner Switch ID", mTlv.mSligSpawnerSwitchId);
        ADD_HIDDEN("Unlimited Spawns", mTlv.mUnlimitedSpawns);
    }
};

struct Path_Fleech final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Fleech, "Fleech", TlvTypes::Fleech_83)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Asleep", mTlv.mAsleep);
        ADD_HIDDEN("Wake Up (Unused?)", mTlv.padding1);
      //  ADD_HIDDEN("Unused1", mTlv.padding1);

        ADD("Attack Anger Increaser", mTlv.mAttackAngerIncreaser);
        ADD_HIDDEN("Attack Delay (Unused?)", mTlv.padding3);
        ADD("Wake Up Switch ID", mTlv.mWakeUpSwitchId);
        ADD("Hanging", mTlv.mHanging);

        ADD("Lost Target Timeout", mTlv.mLostTargetTimeout);
        ADD("Goes To Sleep", mTlv.mGoesToSleep);
        ADD("Patrol Range (Grids)", mTlv.mPatrolRangeInGrids);
        ADD("Wake Up Switch Anger Value", mTlv.mWakeUpSwitchAngerValue);

        ADD("Can Wake Up Switch ID", mTlv.mCanWakeUpSwitchId);
        ADD("Persistant", mTlv.mPersistant);
        ADD_HIDDEN("Unused2(?)", mTlv.padding4);

        ADD_RESOURCE(AnimId::Fleech_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Climb, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Consume, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Crawl, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_DeathByFalling, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_ExtendTongueFromEnemy, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Fall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Knockback, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Land, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_PatrolCry, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_RaiseHead, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_RetractTongueFromEnemey, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_SettleOnGround, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_Sleeping, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_SleepingWithTongue, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_StopCrawling, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_StopMidCrawlCycle, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::Fleech_Unused, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Fleech_WakingUp, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_EnemyStopper final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
        {
            {::Path_EnemyStopper::StopDirection::Left_0, "Left"},
            {::Path_EnemyStopper::StopDirection::Right_1, "Right"},
            {::Path_EnemyStopper::StopDirection::Both_2, "Both"},
        });
    }

    CTOR_AE(Path_EnemyStopper, "EnemyStopper", TlvTypes::EnemyStopper_47)
    {
        ADD("Stop Direction", mTlv.mStopDirection);
        ADD("Switch ID", mTlv.mSwitchId);
    }
};

struct Path_Teleporter final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Teleporter, "Teleporter", TlvTypes::Teleporter_88)
    {
        ADD("Teleporter Switch ID", mTlv.mData.mTeleporterId);
        ADD("Other Teleporter Switch ID", mTlv.mData.mOtherTeleporterId);
        ADD("Camera", mTlv.mData.mDestCamera);
        ADD("Path", mTlv.mData.mDestPath);
        ADD("Level", mTlv.mData.mDestLevel);
        ADD("Switch ID", mTlv.mData.mSwitchId);
        ADD("Scale", mTlv.mData.mScale);
        ADD("Camera Swap Effect", mTlv.mData.mWipeEffect);
        ADD("Movie Number", mTlv.mData.mMovieId);
        ADD("Electric X", mTlv.mData.mElectricX);
        ADD("Electric Y", mTlv.mData.mElectricY);
    }
};

struct Path_UXB final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_UXB::StartState>("Enum_Path_UXB_StartState",
        {
            {::Path_UXB::StartState::eOn, "On"},
            {::Path_UXB::StartState::eOff, "Off"},
        });
    }

    CTOR_AE(Path_UXB, "UXB", TlvTypes::UXB_25)
    {
        ADD("Pattern Length (Max 4)", mTlv.mPatternLength);
        ADD("Pattern", mTlv.mPattern);
        ADD("Scale", mTlv.mScale);
        ADD("Start State", mTlv.mStartState);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_LCDScreen final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_LCDScreen, "LCDScreen", TlvTypes::LCDScreen_60)
    {
        ADD("Message 1 ID", mTlv.mMessageId1);
        ADD("Random Message Min ID", mTlv.mMessageRandMinId);
        ADD("Random Message Max ID", mTlv.mMessageRandMaxId);
        ADD("Message 2 ID", mTlv.mMessageId2);
        ADD("Toggle Message Switch ID", mTlv.mToggleMessageSwitchId);
    }
};

struct Path_Edge final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Edge::GrabDirection>("Enum_EdgeGrabDirection",
        {
            {::Path_Edge::GrabDirection::eFacingLeft, "Facing Left"},
            {::Path_Edge::GrabDirection::eFacingRight, "Facing Right"},
            {::Path_Edge::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
        });
    }

    CTOR_AE(Path_Edge, "Edge", TlvTypes::Edge_3)
    {
        ADD("Grab Direction", mTlv.mGrabDirection);
        ADD("Can Grab", mTlv.mCanGrab);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_StatusLight final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_StatusLight, "StatusLight", TlvTypes::StatusLight_97)
    {
        ADD("Switch ID", mTlv.mInputSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("ID 1", mTlv.mLinkedStatusLightSwitchId1);
        ADD("ID 2", mTlv.mLinkedStatusLightSwitchId2);
        ADD("ID 3", mTlv.mLinkedStatusLightSwitchId3);
        ADD("ID 4", mTlv.mLinkedStatusLightSwitchId4);
        ADD("ID 5", mTlv.mLinkedStatusLightSwitchId5);
        ADD("Ignore Grid Snapping", mTlv.mIgnoreGridSnapping);
    }
};

struct Path_ShadowZone final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_ShadowZone::ShadowZoneScale>("Enum_ShadowZoneScale",
        {
            {::Path_ShadowZone::ShadowZoneScale::eBoth_0, "Both"},
            {::Path_ShadowZone::ShadowZoneScale::eHalf_1, "Half"},
            {::Path_ShadowZone::ShadowZoneScale::eFull_2, "Full"},
        });
    }

    CTOR_AE(Path_ShadowZone, "ShadowZone", TlvTypes::ShadowZone_6)
    {
        ADD_HIDDEN("Center W (Unused?)", mTlv.field_10_center_w);
        ADD_HIDDEN("Center H (Unused?)", mTlv.field_12_center_h);
        ADD_LINKED("R", mTlv.mRed, "RGB_R_Half_FP");
        ADD_LINKED("G", mTlv.mGreen, "RGB_G_Half_FP");
        ADD_LINKED("B", mTlv.mBlue, "RGB_B_Half_FP");
        ADD("Switch ID (Unused?)", mTlv.field_1A_switch_id);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_WorkWheel final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_WorkWheel, "WorkWheel", TlvTypes::WorkWheel_79)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Activation Time", mTlv.mActivationTime);
        ADD("Off Time", mTlv.mTurnOffTime);
        ADD("Turn Off When Stopped", mTlv.mTurnOffWhenStopped);
    }
};

struct Path_MusicTrigger final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_MusicTrigger::MusicTriggerMusicType>("Enum_MusicTriggerMusicType",
        {
            {::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience_0, "Drum Ambience"},
            {::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort_1, "Death Drum Short"},
            {::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong_2, "Secret Area Long"},
            {::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase_3, "Soft Chase"},
            {::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase_4, "Intense Chase"},
            {::Path_MusicTrigger::MusicTriggerMusicType::eChime_5, "Chime"},
            {::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort_6, "Secret Area Short"},
        });

        types.AddEnum<::Path_MusicTrigger::TriggeredBy>("Enum_MusicTriggeredBy",
        {
            {::Path_MusicTrigger::TriggeredBy::eTimer_0, "Timer"},
            {::Path_MusicTrigger::TriggeredBy::eTouching_1, "Touching"},
        });
    }

    CTOR_AE(Path_MusicTrigger, "MusicTrigger", TlvTypes::MusicTrigger_66)
    {
        ADD("Music Type", mTlv.mMusicType);
        ADD("Triggered By", mTlv.mTriggeredBy);
        ADD("Music Delay", mTlv.mMusicDelay);
    }
};

struct Path_AbeStart final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_AbeStart, "AbeStart", TlvTypes::AbeStart_22)
    {
        ADD("X Position", mTlv.field_10_xpos);
        ADD("Y Position", mTlv.field_12_ypos);
    }
};

struct Path_SoftLanding final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SoftLanding, "SoftLanding", TlvTypes::SoftLanding_75)
    {
        ADD("Switch ID", mTlv.mSwitchId);
    }
};

struct Path_WellExpress final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_WellExpress, "WellExpress", TlvTypes::WellExpress_23)
    {
        // Path_WellBase
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Other Well ID", mTlv.mOtherWellId);
        ADD_HIDDEN("Animation ID (Unused?)", mTlv.mAnimId); // leftover from AO well FG hack

        // Path_WellExpress
        ADD("Exit X", mTlv.field_18_exit_x);
        ADD("Exit Y", mTlv.field_1A_exit_y);
        ADD("Disabled Well Level", mTlv.field_1C_disabled_well_level);
        ADD("Disabled Well Path", mTlv.field_1E_disabled_well_path);
        ADD("Disabled Well Camera", mTlv.mOffDestCamera);
        ADD("Disabled Well ID", mTlv.mOffOtherWellId);
        ADD("Enabled Well Level", mTlv.mOnDestLevel);
        ADD("Enabled Well Path", mTlv.mOnDestPath);
        ADD("Enabled Well Camera", mTlv.mOnDestCamera);
        ADD("Enabled Well ID", mTlv.mOnOtherWellId);
        ADD("Emit Leaves", mTlv.mEmitLeaves);
        ADD("Leaf X", mTlv.mLeafX);
        ADD("Leaf Y", mTlv.mLeafY);
        ADD("Movie ID", mTlv.mMovieId);
    }
};

struct Path_SlamDoor final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SlamDoor, "SlamDoor", TlvTypes::SlamDoor_85)
    {
        ADD("Start Shut", mTlv.mStartClosed);
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Flip On Y Axis", mTlv.mFlipY);
        ADD("Delete", mTlv.mDelete);
    }
};

struct Path_HandStone final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_HandStone, "HandStone", TlvTypes::HandStone_61)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Camera ID 1", mTlv.mCameraId1);
        ADD("Camera ID 2", mTlv.mCameraId2);
        ADD("Camera ID 3", mTlv.mCameraId3);
        ADD("Trigger Switch ID", mTlv.mTriggerSwitchId);
    }
};

struct Path_LaughingGas final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_LaughingGas, "LaughingGas", TlvTypes::LaughingGas_81)
    {
        ADD("Is Laughing Gas", mTlv.mLaughingGas);
        ADD("Laughing Gas Switch ID", mTlv.mLaughingGasSwitchId);
        ADD("Red %", mTlv.mRedPercent);
        ADD("Green %", mTlv.mGreenPercent);
        ADD("Blue %", mTlv.mBluePercent);
    }
};

struct Path_InvisibleSwitch final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_InvisibleSwitch::InvisibleSwitchScale>("Enum_InvisibleSwitchScale",
        {
            {::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf_0, "Half"},
            {::Path_InvisibleSwitch::InvisibleSwitchScale::eFull_1, "Full"},
            {::Path_InvisibleSwitch::InvisibleSwitchScale::eAny_2, "Any"},
        });
    }

    CTOR_AE(Path_InvisibleSwitch, "InvisibleSwitch", TlvTypes::InvisibleSwitch_48)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("Delay", mTlv.mActivationDelay);
        ADD("Set Off Alarm", mTlv.mSetOffAlarm);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_Water final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Water, "Water", TlvTypes::Water_77)
    {
        ADD("Max Drops", mTlv.mWaterData.mMaxDrops);
        ADD("Switch ID", mTlv.mWaterData.mSwitchId);
        ADD("Splash Time", mTlv.mWaterData.mSplashTime);
        ADD("Splash X Velocity", mTlv.mWaterData.mSplashVelX);
        ADD("Splash Y Velocity", mTlv.mWaterData.mSplashVelY);
        ADD("Water Duration", mTlv.mWaterData.mWaterDuration);
    }
};

struct Path_GasEmitter final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_GasEmitter::GasColour>("Enum_GasColour",
        {
            {::Path_GasEmitter::GasColour::Yellow_0, "Yellow"},
            {::Path_GasEmitter::GasColour::Red_1, "Red"},
            {::Path_GasEmitter::GasColour::Green_2, "Green"},
            {::Path_GasEmitter::GasColour::Blue_3, "Blue"},
            {::Path_GasEmitter::GasColour::White_4, "White"},
        });
    }

    CTOR_AE(Path_GasEmitter, "GasEmitter", TlvTypes::GasEmitter_71)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Colour", mTlv.mColour);
    }
};

struct Path_BackgroundAnimation final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::relive::TBlendModes>("Enum_relive::TBlendModes",
        {
            {::relive::TBlendModes::eBlend_0, "blend_0"},
            {::relive::TBlendModes::eBlend_1, "blend_1"},
            {::relive::TBlendModes::eBlend_2, "blend_2"},
            {::relive::TBlendModes::eBlend_3, "blend_3"},
        });

        types.AddEnum<::Layer>("Enum_Layer",
        {
            {::Layer::eLayer_0, "Layer_0"},
            {::Layer::eLayer_1, "Layer_1"},
            {::Layer::eLayer_NotUsed_2, "Layer_2"},
            {::Layer::eLayer_BeforeWell_Half_3, "Layer_3"},
            {::Layer::eLayer_Well_Half_4, "Layer_4"},
            {::Layer::eLayer_RopeWebDrillMeatSaw_Half_5, "Layer_5"},
            {::Layer::eLayer_BeforeShadow_Half_6, "Layer_6"},
            {::Layer::eLayer_Shadow_Half_7, "Layer_7"},
            {::Layer::eLayer_8, "Layer_8"},
            {::Layer::eLayer_ZapLinesMudsElum_Half_9, "Layer_9"},
            {::Layer::eLayer_BirdPortal_Half_10, "Layer_10"},
            {::Layer::eLayer_InBirdPortal_Half_11, "Layer_11"},
            {::Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12, "Layer_12"},
            {::Layer::eLayer_AbeMenu_Half_13, "Layer_13"},
            {::Layer::eLayer_SligGreeterFartsBat_Half_14, "Layer_14"},
            {::Layer::eLayer_SlogFleech_Half_15, "Layer_15"},
            {::Layer::eLayer_RollingBallBombMineCar_Half_16, "Layer_16"},
            {::Layer::eLayer_Foreground_Half_17, "Layer_17"},
            {::Layer::eLayer_FG1_Half_18, "Layer_18"},
            {::Layer::eLayer_19, "Layer_19"},
            {::Layer::eLayer_Above_FG1_Half_20, "Layer_20"},
            {::Layer::eLayer_NotUsed_21, "Layer_21"},
            {::Layer::eLayer_BeforeWell_22, "Layer_22"},
            {::Layer::eLayer_Well_23, "Layer_23"},
            {::Layer::eLayer_RopeWebDrillMeatSaw_24, "Layer_24"},
            {::Layer::eLayer_BeforeShadow_25, "Layer_25"},
            {::Layer::eLayer_Shadow_26, "Layer_26"},
            {::Layer::eLayer_27, "Layer_27"},
            {::Layer::eLayer_ZapLinesElumMuds_28, "Layer_28"},
            {::Layer::eLayer_BirdPortal_29, "Layer_29"},
            {::Layer::eLayer_InBirdPortal_30, "Layer_30"},
            {::Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31, "Layer_31"},
            {::Layer::eLayer_AbeMenu_32, "Layer_32"},
            {::Layer::eLayer_SligGreeterFartsBats_33, "Layer_33"},
            {::Layer::eLayer_SlogFleech_34, "Layer_34"},
            {::Layer::eLayer_RollingBallBombMineCar_35, "Layer_35"},
            {::Layer::eLayer_Foreground_36, "Layer_36"},
            {::Layer::eLayer_FG1_37, "Layer_37"},
            {::Layer::eLayer_MainMenuButtonBees_38, "Layer_38"},
            {::Layer::eLayer_Above_FG1_39, "Layer_39"},
            {::Layer::eLayer_FadeFlash_40, "Layer_40"},
            {::Layer::eLayer_Menu_41, "Layer_41"},
            {::Layer::eLayer_Text_42, "Layer_42"},
        });
    }

    CTOR_AE(Path_BackgroundAnimation, "BackgroundAnimation", TlvTypes::BackgroundAnimation_13)
    {
        ADD("Animation ID", mTlv.mAnimId);
        ADD("Is Semi Trans", mTlv.mIsSemiTrans);
        ADD("Semi Trans Mode", mTlv.mSemiTransMode);
        ADD_HIDDEN("Sound Effect (Unused)", mTlv.field_16_sound_effect); // was used for campfires in AO but AE doesn't have them
        ADD("ID", mTlv.field_18_id);
        ADD("Layer", mTlv.mLayer);
    }
};

struct Path_LiftPoint final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_LiftPoint::LiftPointStopType>("Enum_LiftPointStopType",
        {
            {::Path_LiftPoint::LiftPointStopType::eTopFloor_0, "Top Floor"},
            {::Path_LiftPoint::LiftPointStopType::eBottomFloor_1, "Bottom Floor"},
            {::Path_LiftPoint::LiftPointStopType::eMiddleFloor_2, "Middle Floor"},
            {::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor_3, "Middle Lock Floor"},
            {::Path_LiftPoint::LiftPointStopType::eStartPointOnly_4, "Start Point Only"},
        });
    }

    CTOR_AE(Path_LiftPoint, "LiftPoint", TlvTypes::LiftPoint_7)
    {
        ADD("Lift Point ID", mTlv.mLiftPointId);
        ADD("Start Point", mTlv.mIsStartPoint);
        ADD("Lift Type (Unused?)", mTlv.field_14_lift_type);
        ADD("Lift Point Stop Type", mTlv.mLiftPointStopType);
        ADD("Scale", mTlv.mScale);
        ADD("Ignore Lift Mover", mTlv.mIgnoreLiftMover);
    }
};

struct Path_PullRingRope final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_PullRingRope::PullRingSwitchSound>("Enum_PullRingSwitchSound",
        {
            {::Path_PullRingRope::PullRingSwitchSound::eNone_0, "None"},
            {::Path_PullRingRope::PullRingSwitchSound::eWellExit_1, "Well Exit"},
            {::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer_2, "Ring Unknown Trigger"},
            {::Path_PullRingRope::PullRingSwitchSound::eDoorEffect_3, "Door Effect"},
        });

        types.AddEnum<::Path_PullRingRope::PullRingSoundDirection>("Enum_PullRingSoundDirection",
        {
            {::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight_0, "Left And Right"},
            {::Path_PullRingRope::PullRingSoundDirection::eLeft_1, "Left"},
            {::Path_PullRingRope::PullRingSoundDirection::eRight_2, "Right"},
        });
    }

    CTOR_AE(Path_PullRingRope, "PullRingRope", TlvTypes::PullRingRope_12)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("Rope Length", mTlv.mRopeLength);
        ADD("Scale", mTlv.mScale);
        ADD("On Sound", mTlv.mOnSound);
        ADD("Off Sound", mTlv.mOffSound);
        ADD("Sound Direction", mTlv.mSoundDirection);
    }
};

struct Path_MultiSwitchController final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MultiSwitchController, "MultiSwitchController", TlvTypes::MultiSwitchController_96)
    {
        ADD("Output Switch ID", mTlv.mOutputSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("On/Off Delay", mTlv.mOnOffDelay);
        ADD("Input Switch ID 1", mTlv.mInputSwitchId1);
        ADD("Input Switch ID 2", mTlv.mInputSwitchId2);
        ADD("Input Switch ID 3", mTlv.mInputSwitchId3);
        ADD("Input Switch ID 4", mTlv.mInputSwitchId4);
        ADD("Input Switch ID 5", mTlv.mInputSwitchId5);
        ADD("Input Switch ID 6", mTlv.mInputSwitchId6);
    }
};

struct Path_SecurityOrb final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SecurityOrb, "SecurityOrb", TlvTypes::SecurityOrb_19)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_InvisibleZone final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_InvisibleZone, "InvisibleZone", TlvTypes::InvisibleZone_33)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_ContinuePoint final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_ContinuePoint::Scale>("Enum_ContinuePointScale",
        {
            {::Path_ContinuePoint::Scale::eNone_0, "None"},
            {::Path_ContinuePoint::Scale::eHalf_1, "Half"},
            {::Path_ContinuePoint::Scale::eFull_2, "Full"},
        });
    }

    CTOR_AE(Path_ContinuePoint, "ContinuePoint", TlvTypes::ContinuePoint_0)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Save File ID", mTlv.mSaveFileId);
    }
};

struct Path_WheelSyncer final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_WheelSyncer::WheelSyncerOutputRequirement>("Enum_WheelSyncerOutputRequirement",
        {
            {::Path_WheelSyncer::WheelSyncerOutputRequirement::eAllOn_0, "All On"},
            {::Path_WheelSyncer::WheelSyncerOutputRequirement::e1OnAnd2Off_1, "1 On And 2 Off"},
            {::Path_WheelSyncer::WheelSyncerOutputRequirement::e1Or2On_2, "1 Or 2 On"},
            {::Path_WheelSyncer::WheelSyncerOutputRequirement::e1OnOr2Off_3, "1 On Or 2 Off"},
        });
    }

    CTOR_AE(Path_WheelSyncer, "WheelSyncer", TlvTypes::WheelSyncer_65)
    {
        ADD("Input Switch ID 1", mTlv.mInputSwitchId1);
        ADD("Input Switch ID 2", mTlv.mInputSwitchId2);
        ADD("Output Switch ID", mTlv.mOutputSwitchId);
        ADD("Output Requirement", mTlv.mOutputRequirement);
        ADD("Input Switch ID 3", mTlv.mInputSwitchId3);
        ADD("Input Switch ID 4", mTlv.mInputSwitchId4);
        ADD("Input Switch ID 5", mTlv.mInputSwitchId5);
        ADD("Input Switch ID 6", mTlv.mInputSwitchId6);
    }
};

struct Path_LevelLoader final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_LevelLoader, "LevelLoader", TlvTypes::LevelLoader_86)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Destination Level", mTlv.mDestLevel);
        ADD("Destination Path", mTlv.mDestPath);
        ADD("Destination Camera", mTlv.mDestCamera);
        ADD("Movie ID", mTlv.mMovieId);
    }
};

struct Path_Pulley final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Pulley, "Pulley", TlvTypes::Pulley_21)
    {
        ADD("Unused1", mTlv.field_10_unused1);
        ADD("Unused2", mTlv.field_12_unused2);
    }
};

struct Path_FlyingSlig final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_FlyingSlig, "FlyingSlig", TlvTypes::FlyingSlig_82)
    {
        ADD("Scale", mTlv.mFlyingSligData.mScale);
        ADD("Spawn Delay State", mTlv.mFlyingSligData.mSpawnDelayState);
        ADD("Spawn Move Delay", mTlv.mFlyingSligData.mSpawnMoveDelay);
        ADD("Patrol Pause Min", mTlv.mFlyingSligData.mPatrolPauseMin);
        ADD("Patrol Pause Max", mTlv.mFlyingSligData.mPatrolPauseMax);
        ADD("Start Direction", mTlv.mFlyingSligData.mFacing);
        ADD("Panic Delay", mTlv.mFlyingSligData.mPanicDelay);
        ADD("Give Up Chase Delay", mTlv.mFlyingSligData.mGiveUpChaseDelay);
        ADD("Pre-chase Delay", mTlv.mFlyingSligData.mPrechaseDelay);
        ADD("Slig Bound/Persist ID", mTlv.mFlyingSligData.mSligBoundId);
        ADD("Alerted Listen Time", mTlv.mFlyingSligData.mAlertedListenTime);
        ADD("Spawner Switch ID", mTlv.mFlyingSligData.mSpawnerSwitchId);
        ADD("Grenade Delay", mTlv.mFlyingSligData.mGrenadeDelay);
        ADD("Max Velocity", mTlv.mFlyingSligData.mMaxVelocity);
        ADD("Launch Grenade Switch ID", mTlv.mFlyingSligData.mLaunchGrenadeSwitchId);
        ADD("Persistant", mTlv.mFlyingSligData.mPersistant);
    }
};

struct Path_FlyingSligSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_FlyingSligSpawner, "FlyingSligSpawner", TlvTypes::FlyingSligSpawner_92)
    {
        ADD("Scale", mTlv.mFlyingSligSpawnerData.mScale);
        ADD("Spawn Delay State", mTlv.mFlyingSligSpawnerData.mSpawnDelayState);
        ADD("Spawn Move Delay", mTlv.mFlyingSligSpawnerData.mSpawnMoveDelay);
        ADD("Patrol Pause Min", mTlv.mFlyingSligSpawnerData.mPatrolPauseMin);
        ADD("Patrol Pause Max", mTlv.mFlyingSligSpawnerData.mPatrolPauseMax);
        ADD("Start Direction", mTlv.mFlyingSligSpawnerData.mFacing);
        ADD("Panic Delay", mTlv.mFlyingSligSpawnerData.mPanicDelay);
        ADD("Give Up Chase Delay", mTlv.mFlyingSligSpawnerData.mGiveUpChaseDelay);
        ADD("Pre-Chase Delay", mTlv.mFlyingSligSpawnerData.mPrechaseDelay);
        ADD("Slig Bound/Persist ID", mTlv.mFlyingSligSpawnerData.mSligBoundId);
        ADD("Listen Time", mTlv.mFlyingSligSpawnerData.mAlertedListenTime);
        ADD("Spawner Switch ID", mTlv.mFlyingSligSpawnerData.mSpawnerSwitchId);
        ADD("Grenade Delay", mTlv.mFlyingSligSpawnerData.mGrenadeDelay);
        ADD("Max Velocity", mTlv.mFlyingSligSpawnerData.mMaxVelocity);
        ADD("Launch Grenade Switch ID", mTlv.mFlyingSligSpawnerData.mLaunchGrenadeSwitchId);
        ADD("Persistant", mTlv.mFlyingSligSpawnerData.mPersistant);
    }
};

struct Path_DeathDrop final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DeathDrop, "DeathDrop", TlvTypes::DeathDrop_4)
    {
        ADD_HIDDEN("Unused1", mTlv.field_10_animation);
        ADD_HIDDEN("Unused2", mTlv.field_12_sound);
        ADD_HIDDEN("Unused3", mTlv.field_14_id);
        ADD_HIDDEN("Unused4", mTlv.field_16_action);
        ADD_HIDDEN("Unused5", mTlv.field_18_unknown1);
        ADD_HIDDEN("Unused6", mTlv.field_1A_unknown2);
    }
};

struct Path_SligSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligSpawner, "SligSpawner", TlvTypes::SligSpawner_37)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start State", mTlv.mStartState);
        ADD("Pause Time", mTlv.mPauseTime);
        ADD("Pause Left Min", mTlv.mPauseLeftMin);
        ADD("Pause Left Max", mTlv.mPauseLeftMax);
        ADD("Pause Right Min", mTlv.mPauseRightMin);
        ADD("Pause Right Max", mTlv.mPauseRightMax);
        ADD("Shoot Possessed Sligs", mTlv.mShootPossessedSligs);
        ADD("Shoot On Sight Delay", mTlv.mShootOnSightDelay);
        ADD("Bullet Shoot Count", mTlv.mNumTimesToShoot);
        ADD("unknown", mTlv.padding);
        ADD("code_1", mTlv.mCode1);
        ADD("code_2", mTlv.mCode2);
        ADD("Chase Abe When Spotted", mTlv.mChaseAbeWhenSpotted);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Panic Timeout", mTlv.mPanicTimeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.num_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.panic_sound_timeout);
        ADD("Stop Chase Delay", mTlv.mStopChaseDelay);
        ADD("Time To Wait Before Chase", mTlv.mTimeToWaitBeforeChase);
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Alerted Listen Time", mTlv.mAlertedListenTime);
        ADD("Percent Say What", mTlv.mPercentSayWhat);
        ADD("Percent Beat Mudokon", mTlv.mPercentBeatMud);
        ADD("Talk To Abe (Unused?)", mTlv.talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.dont_shoot);
        ADD("Z Shoot Delay", mTlv.mZShootDelay);
        ADD("Stay Awake", mTlv.mStayAwake);
        ADD("Disable Resources", mTlv.mDisabledResources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.mNoiseWakeUpDistance);
        ADD("ID", mTlv.mSligSpawnerSwitchId);
        ADD("Unlimited Spawns", mTlv.mUnlimitedSpawns);
    }
};

struct Path_SligBoundLeft final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligBoundLeft, "SligBoundLeft", TlvTypes::SligBoundLeft_32)
    {
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_SligBoundRight final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligBoundRight, "SligBoundRight", TlvTypes::SligBoundRight_45)
    {
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_SligPersist final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligPersist, "SligPersist", TlvTypes::SligPersist_46)
    {
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_ZSligCover final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ZSligCover, "ZSligCover", TlvTypes::ZSligCover_50)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_WellLocal final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_WellLocal, "WellLocal", TlvTypes::LocalWell_8)
    {
        // Path_WellBase
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD_HIDDEN("Other Well ID", mTlv.mOtherWellId); // hidden because this is only used by the well express
        ADD_HIDDEN("Animation ID (Unused?)", mTlv.mAnimId); // leftover from AO well FG hack

        // Path_WellLocal
        ADD("Disabled XPos", mTlv.field_18_off_dx);
        ADD("Disabled YPos", mTlv.field_1A_off_dy);
        ADD("Enabled XPos", mTlv.mOnDestX);
        ADD("Enabled YPos", mTlv.mOnDestY);
        ADD("Emit Leaves", mTlv.mEmitLeaves);
        ADD("Leaf XPos", mTlv.mLeafX);
        ADD("Leaf YPos", mTlv.mLeafY);
    }
};

struct Path_BrewMachine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_BrewMachine, "BrewMachine", TlvTypes::BrewMachine_101)
    {
        ADD("Brew Count", mTlv.mBrewCount);
    }
};

struct Path_Drill final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Drill_Data::DrillBehavior>("Enum_DrillBehavior",
        {
            {::Path_Drill_Data::DrillBehavior::eNotInteractable_0, "Not Interactable"},
            {::Path_Drill_Data::DrillBehavior::eToggle_1, "Toggle"},
            {::Path_Drill_Data::DrillBehavior::eUse_2, "Use"},
        });

        types.AddEnum<::Path_Drill_Data::DrillDirection>("Enum_DrillDirection",
        {
            {::Path_Drill_Data::DrillDirection::eDown_0, "Down"},
            {::Path_Drill_Data::DrillDirection::eRight_1, "Right"},
            {::Path_Drill_Data::DrillDirection::eLeft_2, "Left"},
        });
    }

    CTOR_AE(Path_Drill, "Drill", TlvTypes::Drill_90)
    {
        ADD("Scale", mTlv.mDrillData.mScale);
        ADD("Min Off Time", mTlv.mDrillData.mOnMinPauseTime);
        ADD("Max Off Time", mTlv.mDrillData.mOnMaxPauseTime);
        ADD("Switch ID", mTlv.mDrillData.mSwitchId);
        ADD("Behavior", mTlv.mDrillData.mDrillBehavior);
        ADD("Speed", mTlv.mDrillData.mOnSpeed);
        ADD("Start State On", mTlv.mDrillData.mStartStateOn);
        ADD("Off Speed", mTlv.mDrillData.mOffSpeed);
        ADD("Min Off Time Speed Change", mTlv.mDrillData.mOffMinPauseTime);
        ADD("Max Off Time Speed Change", mTlv.mDrillData.mOffMaxPauseTime);
        ADD("Start Position Bottom", mTlv.mDrillData.mStartPositionBottom);
        ADD("Start Direction", mTlv.mDrillData.mDirection);
    }
};

struct Path_Mine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Mine, "Mine", TlvTypes::Mine_24)
    {
        ADD_HIDDEN("Num Patterns (Unused?)", mTlv.field_10_num_patterns);
        ADD_HIDDEN("Pattern (Unused?)", mTlv.field_12_pattern);
        ADD("Scale", mTlv.mScale);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Path_Slog final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Slog, "Slog", TlvTypes::Slog_16)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Asleep", mTlv.mAsleep);
        ADD("Wake Up Anger", mTlv.mWakeUpAnger);
        ADD("Bark Anger", mTlv.mBarkAnger);
        ADD("Chase Anger", mTlv.mChaseAnger);
        ADD("Chase Delay", mTlv.mChaseDelay);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Anger Switch ID", mTlv.mAngerSwitchId);
        ADD("Bone Eating Time", mTlv.mBoneEatingTime);
    }
};

struct Path_ResetPath final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ResetPath, "ResetPath", TlvTypes::ResetPath_76)
    {
        ADD("Reset Switches", mTlv.mClearIds);
        ADD("Start Switch ID", mTlv.mFrom);
        ADD("End Switch ID", mTlv.mTo);
        ADD("Skip Switch ID", mTlv.mExclude);
        ADD("Free Path Resources", mTlv.mClearObjects);
        ADD("Path ID To Free Resources", mTlv.mPath);
        ADD("Enabled", mTlv.mEnabled);
    }
};

struct Path_TrapDoor final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_TrapDoor::StartState>("Enum_TrapDoorStartState",
        {
            {::Path_TrapDoor::StartState::eOpen_0, "Open"},
            {::Path_TrapDoor::StartState::eClosed_1, "Closed"}
        });
    }

    CTOR_AE(Path_TrapDoor, "TrapDoor", TlvTypes::TrapDoor_30)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Start State", mTlv.mStartState);
        ADD("Self Closing", mTlv.mSelfClosing);
        ADD("Scale", mTlv.mScale);
        ADD("Dest Level (Unused?)", mTlv.field_18_dest_level);
        ADD("Direction", mTlv.mDirection);
        ADD("X Offset", mTlv.mXOff);
        ADD("Stay Open Time", mTlv.mStayOpenTime);
    }
};

struct Path_PathTransition final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_PathTransition, "PathTransition", TlvTypes::PathTransition_1)
    {
        ADD("Level", mTlv.mNextLevel);
        ADD("Path", mTlv.mNextPath);
        ADD("Camera", mTlv.mNextCamera);
        ADD("Movie", mTlv.mMovieId);
        ADD("Wipe", mTlv.mWipeEffect);
        ADD("Scale", mTlv.mNextPathScale);
    }
};

struct Path_LiftMover final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_LiftMover::YDirection>("Enum_LiftMoverYDirection",
        {
            {::Path_LiftMover::YDirection::eDown_0, "Down"},
            {::Path_LiftMover::YDirection::eUp_1, "Up"},
        });
    }

    CTOR_AE(Path_LiftMover, "LiftMover", TlvTypes::LiftMover_39)
    {
        ADD("Lift Mover Switch ID", mTlv.mLiftMoverSwitchId);
        ADD("Target Lift Point ID", mTlv.mTargetLiftPointId);
        ADD("Move Direction", mTlv.mMoveDirection);
    }
};

struct Path_RockSack final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_RockSack, "RockSack", TlvTypes::RockSack_10)
    {
        ADD("Rock Fall Direction", mTlv.mRockFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Rock Amount", mTlv.mRockAmount);

        ADD_RESOURCE(AnimId::RockSack_HardHit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::RockSack_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::RockSack_SoftHit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Bone, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_TimerTrigger final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_TimerTrigger, "TimerTrigger", TlvTypes::TimerTrigger_57)
    {
        ADD("Input Switch ID", mTlv.mInputSwitchId);
        ADD("Trigger Interval", mTlv.mActivationDelay);
        ADD("Output Switch ID 1", mTlv.mOutputSwitchId1);
        ADD("Output Switch ID 2", mTlv.mOutputSwitchId2);
        ADD("Output Switch ID 3", mTlv.mOutputSwitchId3);
        ADD("Output Switch ID 4", mTlv.mOutputSwitchId4);
    }
};

struct Path_MotionDetector final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_MotionDetector::InitialMoveDirection>("Enum_InitialMoveDirection",
        {
            {::Path_MotionDetector::InitialMoveDirection::eRight_0, "Right"},
            {::Path_MotionDetector::InitialMoveDirection::eLeft_1, "Left"},
        });
    }

    CTOR_AE(Path_MotionDetector, "MotionDetector", TlvTypes::MotionDetector_36)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Device X", mTlv.mDeviceX);
        ADD("Device Y", mTlv.mDeviceY);
        ADD("Speed", mTlv.mSpeedx256);
        ADD("Initial Move Direction", mTlv.mInitialMoveDirection);
        ADD("Draw Flare", mTlv.mDrawFlare);
        ADD("Disable Switch ID", mTlv.mDisableSwitchId);
        ADD("Alarm Switch ID", mTlv.mAlarmSwitchId);
        ADD("Alarm Duration", mTlv.mAlarmDuration);

        ADD_RESOURCE(AnimId::MotionDetector_Flare, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::MotionDetector_Laser, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_MineCar final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MineCar, "MineCar", TlvTypes::MineCar_93)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Max Damage (Unused?)", mTlv.mMaxDamage);

        ADD_RESOURCE(AnimId::Mine_Car_Closed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Mine_Car_Open, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Mine_Car_Shake_A, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Mine_Car_Shake_B, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Mine_Car_Tread_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Mine_Car_Tread_Move_A, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Mine_Car_Tread_Move_B, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ExplosionSet final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ExplosionSet, "ExplosionSet", TlvTypes::ExplosionSet_95)
    {
        ADD("Start Enabled", mTlv.mStartEnabled);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Spawn Assets", mTlv.mSpawnAssets);
        ADD("Start Delay", mTlv.mStartDelay);
        ADD("Start Direction", mTlv.mStartDirection);
        ADD("Asset Interval", mTlv.mAssetInterval);
        ADD("Grid Spacing", mTlv.mGridSpacing);
        ADD("Increasing Grid Spacing", mTlv.mIncreasingGridSpacing);
        ADD("Scale", mTlv.mScale);

        ADD_RESOURCE(AnimId::AirExplosion, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::AirExplosion_Small, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ColourfulMeter final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ColourfulMeter, "ColourfulMeter", TlvTypes::ColourfulMeter_91)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Number Of Meter Bars", mTlv.mNumberOfMeterBars);
        ADD("Mines Alarm Countdown (Seconds)", mTlv.mMinesAlarmCountdown);
        ADD("Start Filled", mTlv.mStartFilled);
    }
};

struct Path_Alarm final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Alarm, "Alarm", TlvTypes::Alarm_100)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Duration", mTlv.mAlarmDuration);
    }
};

struct Path_DemoSpawnPoint final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DemoSpawnPoint, "DemoSpawnPoint", TlvTypes::DemoSpawnPoint_87)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_SlapLock final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SlapLock, "SlapLock", TlvTypes::SlapLock_98)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Target Tomb ID 1", mTlv.mTargetTombSwitchId1);
        ADD("Target Tomb ID 2", mTlv.mTargetTombSwitchId2);
        ADD("Persistant", mTlv.mPersistant);
        ADD("Has Ghost", mTlv.mHasGhost);
        ADD("Give Invisibility Power-up", mTlv.mGiveInvisibilityPowerup);
        ADD("Invisibility Duration", mTlv.mInvisibilityDuration);
        ADD("Toggle Switch ID", mTlv.mSlapOutputSwitchId);
    }
};

struct Path_Slurg final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Slurg, "Slurg", TlvTypes::Slurg_84)
    {
        ADD("Time Until Turning Around", mTlv.mSlurgData.mMovingTimer);
        ADD("Start Direction", mTlv.mSlurgData.mFacing);
        ADD("Scale", mTlv.mSlurgData.mScale);
        ADD("Switch ID (increment by 1 on death)", mTlv.mSlurgData.mSwitchId);

        ADD_RESOURCE(AnimId::Slurg_Burst, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slurg_Move, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slurg_Turn_Around, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_DoorBlocker final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DoorBlocker, "DoorBlocker", TlvTypes::DoorBlocker_109)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);

        ADD_RESOURCE(AnimId::Door_Lock_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Door_Lock_Open, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_Dove final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Dove, "Dove", TlvTypes::Dove_9)
    {
        ADD("Dove Count", mTlv.mDoveCount);
        ADD("Pixel Perfect", mTlv.mPixelPerfect);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_BirdPortalExit final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_BirdPortalExit, "BirdPortalExit", TlvTypes::BirdPortalExit_29)
    {
        ADD("Exit Direction", mTlv.mExitSide);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_DoorFlame final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DoorFlame, "DoorFlame", TlvTypes::DoorFlame_51)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Colour", mTlv.field_14_colour);
    }
};

struct Path_TrainDoor final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_TrainDoor, "TrainDoor", TlvTypes::TrainDoor_111)
    {
        ADD("Direction", mTlv.mDirection);

        ADD_RESOURCE(AnimId::Door_Train_Closed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Door_Train_Closing, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_Greeter final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Greeter, "Greeter", TlvTypes::Greeter_106)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Motion Detector Speed", mTlv.mMotionDetectorSpeed);
        ADD("Start Direction", mTlv.mFacing);
    }
};

struct Path_ScrabBoundLeft final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ScrabBoundLeft, "ScrabLeftBound", TlvTypes::ScrabBoundLeft_43)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_ScrabBoundRight final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ScrabBoundRight, "ScrabRightBound", TlvTypes::ScrabBoundRight_44)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_CreditsController final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_CreditsController, "CreditsController", TlvTypes::CreditsController_62)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_MovieHandstone final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MovieHandstone, "MovieHandstone", TlvTypes::MovieHandStone_27)
    {
        ADD("Scale", mTlv.scale);
        ADD("Movie Number", mTlv.movie_number);
        ADD("Trigger Switch ID", mTlv.trigger_switch_id);
        ADD("Padding", mTlv.padding);
    }
};

struct Path_MovingBomb final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MovingBomb, "MovingBomb", TlvTypes::MovingBomb_52)
    {
        ADD("Speed", mTlv.mSpeed);
        ADD("Start Moving Switch ID", mTlv.mStartMovingSwitchId);
        ADD("Triggered By Alarm", mTlv.mTriggeredByAlarm);
        ADD("Scale", mTlv.mScale);
        ADD("Max Rise", mTlv.field_18_max_rise);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Start Speed", mTlv.mStartSpeed);
        ADD("Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Path_MovingBombStopper final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MovingBombStopper, "MovingBombStopper", TlvTypes::MovingBombStopper_53)
    {
        ADD("Min Delay", mTlv.mMinStopTime);
        ADD("Max Delay", mTlv.mMaxStopTime);
    }
};

struct Path_SecurityDoor final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SecurityDoor, "SecurityDoor", TlvTypes::SecurityDoor_58)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("X Position", mTlv.mXPos);
        ADD("Y Position", mTlv.mYPos);

        ADD_RESOURCE(AnimId::Security_Door_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Security_Door_Speak, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_CrawlingSlig final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_CrawlingSlig::StartState>("Enum_CrawlingSligState",
        {
            {::Path_CrawlingSlig::StartState::eSleeping_0, "Sleeping"},
            {::Path_CrawlingSlig::StartState::eSleeping_1, "Sleeping2 (Duplicate)"},
            {::Path_CrawlingSlig::StartState::eAwake_2, "Awake"},
        });

        types.AddEnum<::Path_CrawlingSlig::CrawlDirection>("Enum_CrawlDirection",
        {
            {::Path_CrawlingSlig::CrawlDirection::eLeft_0, "Left"},
            {::Path_CrawlingSlig::CrawlDirection::eRight_1, "Right"},
            {::Path_CrawlingSlig::CrawlDirection::eRandom_2, "Random"},
        });
    }

    CTOR_AE(Path_CrawlingSlig, "CrawlingSlig", TlvTypes::CrawlingSlig_103)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start Direction", mTlv.mFacing);
        ADD("State", mTlv.mStartState);
        ADD("Crawl Direction", mTlv.mCrawlDirection);
        ADD("Panic Switch ID", mTlv.mPanicSwitchId);
        ADD("Respawn On Death", mTlv.mRespawnOnDeath);

        ADD_RESOURCE(AnimId::CrawlingSlig_Crawling, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_Empty, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_EndCrawling, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_EndPushingWall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_Falling, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_IdleToPushingWall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_Landing, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_PushingWall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_Shaking, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_ShakingToIdle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_Snoozing, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_Speaking, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_StartFalling, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_ToShakingToIdle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_TurnAround, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_UsingButton, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSlig_WakingUp, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SligGetPants final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligGetPants, "SligGetPants", TlvTypes::SligGetPants_104)
    {
        ADD("Scale", mTlv.Scale);
        ADD("Start State", mTlv.start_state);
        ADD("Pause Time", mTlv.pause_time);
        ADD("Pause Left Min", mTlv.pause_left_min);
        ADD("Pause Left Max", mTlv.pause_left_max);
        ADD("Pause Right Min", mTlv.pause_right_min);
        ADD("Pause Right Max", mTlv.pause_right_max);
        ADD("Shoot Possessed Sligs", mTlv.shoot_possessed_sligs);
        ADD("Shoot On Sight Delay", mTlv.shoot_on_sight_delay);
        ADD("Bullet Shoot Count", mTlv.num_times_to_shoot);
        ADD("unknown1", mTlv.padding);
        ADD("Code 1", mTlv.code1);
        ADD("Code 2", mTlv.code2);
        ADD("Chase Abe When Spotted", mTlv.chase_abe_when_spotted);
        ADD("Start Direction", mTlv.start_direction);
        ADD("Panic Timeout", mTlv.panic_timeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.amount_of_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.panic_sounds_timeout);
        ADD("Stop Chase Delay", mTlv.stop_chase_delay);
        ADD("Time To Wait Before Chase", mTlv.time_to_wait_before_chase);
        ADD("Slig Bound/Persist ID", mTlv.slig_bound_id);
        ADD("Alerted Listen Time", mTlv.alerted_listen_time);
        ADD("Percent Say What", mTlv.percent_say_what);
        ADD("Percent Beat Mudokon", mTlv.percent_beat_mud);
        ADD("Talk To Abe (Unused?)", mTlv.talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.dont_shoot);
        ADD("Z Shoot Delay", mTlv.z_shoot_delay);
        ADD("Stay Awake", mTlv.stay_awake);
        ADD("Disable Resources", mTlv.disable_resources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.noise_wake_up_distance);
        ADD("Slig Spawner Switch ID", mTlv.mSligSpawnerSwitchId);
        ADD("Unlimited Spawns", mTlv.unlimited_spawns);

        ADD_RESOURCE(AnimId::CrawlingSligLocker_Closed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSligLocker_Open, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SligGetWings final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligGetWings, "SligGetWings", TlvTypes::SligGetWings_105)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Spawn Delay State", mTlv.mSpawnDelayState);
        ADD("Spawn Move Delay", mTlv.mSpawnMoveDelay);
        ADD("Patrol Pause Min", mTlv.mPatrolPauseMin);
        ADD("Patrol Pause Max", mTlv.mPatrolPauseMax);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Panic Delay", mTlv.mPanicDelay);
        ADD("Give Up Chase Delay", mTlv.mGiveUpChaseDelay);
        ADD("Pre-chase Delay", mTlv.mPrechaseDelay);
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Alerted Listen Time", mTlv.mAlertedListenTime);
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        ADD("Grenade Delay", mTlv.mGrenadeDelay);
        ADD("Max Velocity", mTlv.mMaxVelocity);
        ADD("Launch Grenade Switch ID", mTlv.mLaunchGrenadeSwitchId);
        ADD("Persistant", mTlv.mPersistant);

        ADD_RESOURCE(AnimId::CrawlingSligLocker_Closed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSligLocker_Open, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_CrawlingSligButton final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        // this probably shouldn't be here since the sound/sound direction implementation is extremely scuffed
        // but maybe someone wants to use it anyway
        types.AddEnum<::Path_CrawlingSligButton::CrawlingSligButtonSounds>("Enum_CrawlingSligButtonSounds",
        {
            {::Path_CrawlingSligButton::CrawlingSligButtonSounds::None_0, "None"},
            {::Path_CrawlingSligButton::CrawlingSligButtonSounds::SackHit_1, "Sack Hit"},
            {::Path_CrawlingSligButton::CrawlingSligButtonSounds::FallingItemPresence2_2, "Falling Item Presence 2"},
            {::Path_CrawlingSligButton::CrawlingSligButtonSounds::SecurityOrb_3, "Security Orb"},
            {::Path_CrawlingSligButton::CrawlingSligButtonSounds::SackHit_4, "Sack Hit (duplicate)"},
            {::Path_CrawlingSligButton::CrawlingSligButtonSounds::Bullet1_5, "Bullet"},
            {::Path_CrawlingSligButton::CrawlingSligButtonSounds::AbeGenericMovement_6, "Abe Generic Movement"},
        });
    }
    CTOR_AE(Path_CrawlingSligButton, "CrawlingSligButton", TlvTypes::CrawlingSligButton_107)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("On Sound", mTlv.mOnSound);
        ADD("Off Sound", mTlv.mOffSound);
        ADD_HIDDEN("Sound Direction", mTlv.mSoundDirection);

        ADD_RESOURCE(AnimId::CrawlingSligButton, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::CrawlingSligButtonUse, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_Glukkon final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Glukkon::GlukkonTypes>("Enum_GlukkonTypes",
        {
            {::Path_Glukkon::GlukkonTypes::eNormal_0, "Normal"},
            {::Path_Glukkon::GlukkonTypes::eStoryAslik_1, "Story Aslik"},
            {::Path_Glukkon::GlukkonTypes::eStoryDripik_2, "Story Dripik"},
            {::Path_Glukkon::GlukkonTypes::eStoryPhleg_3, "Story Phleg"},
            {::Path_Glukkon::GlukkonTypes::eNormal_4, "Normal 4"},
            {::Path_Glukkon::GlukkonTypes::eNormal_5, "Normal 5"},
        });

        types.AddEnum<::Path_Glukkon::Facing>("Enum_GlukkonStartDirection",
        {
            {::Path_Glukkon::Facing::eRight_0, "Right"},
            {::Path_Glukkon::Facing::eLeft_1, "Left"},
        });

        types.AddEnum<::Path_Glukkon::SpawnType>("Enum_GlukkonSpawnType",
        {
            {::Path_Glukkon::SpawnType::eRegularSpawn_0, "Regular Spawn"},
            {::Path_Glukkon::SpawnType::eFacingLeft_1, "Facing Left"},
            {::Path_Glukkon::SpawnType::eFacingRight_2, "Facing Right"},
            {::Path_Glukkon::SpawnType::eFullSpawnEffects_3, "Full Spawn Effects"},
        });

        types.AddEnum<::Path_Glukkon::Behavior>("Enum_GlukkonBehavior",
        {
            {::Path_Glukkon::Behavior::eIgnoreWalls_0, "Ignore Walls"},
            {::Path_Glukkon::Behavior::eCheckForWalls_1, "Check For Walls"},
        });
    }

    CTOR_AE(Path_Glukkon, "Glukkon", TlvTypes::Glukkon_73)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Behavior", mTlv.mBehavior);
        ADD("Scream Help Delay", mTlv.mScreamHelpDelay);
        ADD("Help Switch ID", mTlv.mHelpSwitchId);
        ADD("To Calm Delay", mTlv.mToCalmDelay);
        ADD("Spawn Switch ID", mTlv.mSpawnSwitchId);
        ADD("Spawn Type", mTlv.mSpawnType);
        ADD("Spawn Delay", mTlv.mSpawnDelay);
        ADD("Glukkon Type", mTlv.mGlukkonType);
        ADD("Death Switch ID", mTlv.mDeathSwitchId);
        ADD("Play Movie Switch ID", mTlv.mPlayMovieSwitchId);
        ADD("Movie To Play (fmv ID)", mTlv.mMovieId);

        ADD_RESOURCE(AnimId::Glukkon_Normal_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Glukkon_Aslik_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Glukkon_Phleg_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Glukkon_Dripik_Idle, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_GlukkonSwitch final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_GlukkonSwitch::Scale>("Enum_GlukkonSwitchScale",
        {
            {::Path_GlukkonSwitch::Scale::eHalf_0, "Half"},
            {::Path_GlukkonSwitch::Scale::eFull_1, "Full"},
        });
    };

    CTOR_AE(Path_GlukkonSwitch, "GlukkonSwitch", TlvTypes::GlukkonSwitch_108)
    {
        ADD("Scale", mTlv.mScale);
        ADD("OK Switch ID", mTlv.mOkSwitchId);
        ADD("Fail Switch ID", mTlv.mFailSwitchId);
        ADD("X Position", mTlv.mXPos);
        ADD("Y Position", mTlv.mYPos);
    }
};

struct Path_GasCountDown final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_GasCountDown, "GasCountDown", TlvTypes::GasCountdown_69)
    {
        ADD("Start Timer Switch ID", mTlv.mStartTimerSwitchId);
        ADD("Gas Countdown Time", mTlv.mGasCountdownTimer);
        ADD("Stop Timer Switch ID", mTlv.mStopTimerSwitchId);
    }
};

struct Path_FallingItem final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_FallingItem, "FallingItem", TlvTypes::FallingItem_11)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Fall Interval", mTlv.mFallInterval);
        ADD("Max Falling Items", mTlv.mMaxFallingItems);
        ADD("Reset Switch ID After Use", mTlv.mResetSwitchIdAfterUse);
    }
};

struct Path_BoneBag final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_BoneBag, "BoneBag", TlvTypes::BoneBag_94)
    {
        ADD("Bone Fall Direction", mTlv.mBoneFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Bone Amount", mTlv.mBoneAmount);

        ADD_RESOURCE(AnimId::BoneBag_HardHit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoneBag_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoneBag_SoftHit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Bone, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
    }
};

// TODO: unknown field
struct Path_SecurityClaw final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SecurityClaw, "SecurityClaw", TlvTypes::SecurityClaw_35)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Unknown", mTlv.field_14_unknown);
    }
};

struct Path_FootSwitch final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_FootSwitch::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
        {
            {::Path_FootSwitch::FootSwitchTriggerBy::eAbe_0, "Abe"},
            {::Path_FootSwitch::FootSwitchTriggerBy::eAnyone_1, "Anyone"},
        });
    }

    CTOR_AE(Path_FootSwitch, "FootSwitch", TlvTypes::FootSwitch_34)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Action", mTlv.mAction);
        ADD("Triggered By", mTlv.mTriggeredBy);
    }
};

struct Path_ZzzSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ZzzSpawner, "ZzzSpawner", TlvTypes::ZzzSpawner_72)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Zzz Interval", mTlv.mZzzInterval);
    }
};

struct Path_SlogSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_SlogSpawner::StartDirection>("Enum_SlogSpawnerStartDirection",
            {
                {::Path_SlogSpawner::StartDirection::eRight_0, "Right"},
                {::Path_SlogSpawner::StartDirection::eLeft_1, "Left"},
            });
    }

    CTOR_AE(Path_SlogSpawner, "SlogSpawner", TlvTypes::SlogSpawner_68)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Max Slogs", mTlv.mMaxSlogs);
        ADD("Max Slogs At A Time", mTlv.mMaxSlogsAtATime);
        ADD("Start Direction", mTlv.mStartDirection);
        ADD("Slog Spawn Interval", mTlv.mSlogSpawnInterval);
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        ADD("Listen To Sligs", mTlv.mListenToSligs);
        ADD("Chase Delay", mTlv.mChaseDelay);
    }
};

struct Path_MainMenuController final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MainMenuController, "MainMenuController", TlvTypes::MainMenuController_54)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_Null_63 final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Null_63, "Null_63", TlvTypes::Null_63)
    {
        ADD("padding1", mTlv.padding1);
        ADD("padding2", mTlv.padding2);
    }
};

struct Path_Scrab final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Scrab, "Scrab", TlvTypes::Scrab_41)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Attack Delay", mTlv.mAttackDelay);
        ADD("Patrol Type Run Or Walk Chance (6 Max)", mTlv.mPatrolTypeRunOrWalkChance);
        ADD("Left Min Delay", mTlv.mPauseLeftMin);
        ADD("Left Max Delay", mTlv.mPauseLeftMax);
        ADD("Right Min Delay", mTlv.mPauseRightMin);
        ADD("Right Max Delay", mTlv.mPauseRightMax);
        ADD("Pause After Chase Delay", mTlv.mPauseAfterChaseTime);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Roar Randomly", mTlv.mRoarRandomly);
        ADD("Persistant", mTlv.mPersistant);
        ADD("Possessed Max Whirl Attack Duration", mTlv.mPossessedMaxWhirlAttackDuration);
        ADD_HIDDEN("Unused", mTlv.field_28_unused);
        ADD("Kill Enemy", mTlv.mKillEnemy);

        ADD_RESOURCE(AnimId::Scrab_AttackLunge, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_AttackSpin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_DeathBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_DeathEnd, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::Scrab_Empty, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Feed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_FeedToGulp, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_GetEaten, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_GulpToStand, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_HopBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_HowlBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_HowlEnd, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Jump, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_JumpAndRunToFall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Knockback, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Landing, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_LegKick, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Run, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_RunToStand, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_RunToWalk, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_ScrabBattleAnim, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Shriek, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Stamp, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_StandToFeed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_StandToRun, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_StandToWalk, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Turn, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Walk, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_WalkToFall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_WalkToRun, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_WalkToStand, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ScrabSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_ScrabSpawner::ScrabSpawnDirection>("Enum_ScrabSpawnDirection",
        {
            {::Path_ScrabSpawner::ScrabSpawnDirection::eNone_0, "None"},
            {::Path_ScrabSpawner::ScrabSpawnDirection::eLeft_1, "Left"},
            {::Path_ScrabSpawner::ScrabSpawnDirection::eRight_2, "Right"},
        });
    }

    CTOR_AE(Path_ScrabSpawner, "ScrabSpawner", TlvTypes::ScrabSpawner_102)
    {
        // Scrab properties
        ADD("Scrab Scale", mTlv.mScale);
        ADD("Scrab Attack Delay", mTlv.mAttackDelay);
        ADD("Scrab Patrol Type Run Or Walk Chance (6 Max)", mTlv.mPatrolTypeRunOrWalkChance);
        ADD("Scrab Left Min Delay", mTlv.mPauseLeftMin);
        ADD("Scrab Left Max Delay", mTlv.mPauseLeftMax);
        ADD("Scrab Right Min Delay", mTlv.mPauseRightMin);
        ADD("Scrab Right Max Delay", mTlv.mPauseRightMax);
        ADD("Scrab Pause After Chase Delay", mTlv.mPauseAfterChaseTime);
        ADD("Scrab Disabled Resources", mTlv.mDisabledResources);
        ADD("Scrab Roar Randamly", mTlv.mRoarRandomly);
        ADD("Scrab Persistant", mTlv.mPersistant);
        ADD("Scrab Whirl Attack Duration", mTlv.mPossessedMaxWhirlAttackDuration);
        ADD("Scrab Unused", mTlv.field_28_unused);
        ADD("Scrab Kill Enemy", mTlv.mKillEnemy);

        // Spawner properties
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        ADD("Scrab Spawn Direction", mTlv.mFacing);

        ADD_RESOURCE(AnimId::Scrab_AttackLunge, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_AttackSpin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_DeathBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_DeathEnd, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::Scrab_Empty, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Feed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_FeedToGulp, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_GetEaten, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_GulpToStand, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_HopBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_HowlBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_HowlEnd, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Jump, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_JumpAndRunToFall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Knockback, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Landing, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_LegKick, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Run, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_RunToStand, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_RunToWalk, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_ScrabBattleAnim, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Shriek, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Stamp, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_StandToFeed, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_StandToRun, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_StandToWalk, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Turn, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_Walk, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_WalkToFall, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_WalkToRun, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Scrab_WalkToStand, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SlurgSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SlurgSpawner, "SlurgSpawner", TlvTypes::SlurgSpawner_89)
    {
        ADD("Time Until Turning Around", mTlv.mSlurgData.mMovingTimer);
        ADD("Start Direction", mTlv.mSlurgData.mFacing);
        ADD("Scale", mTlv.mSlurgData.mScale);
        ADD("Switch ID (increment by 1 on death)", mTlv.mSlurgData.mSwitchId);

        ADD("Spawn Interval", mTlv.mSpawnerData.mSpawnInterval);
        ADD("Max Slurgs", mTlv.mSpawnerData.mMaxSlurgs);
        ADD("Spawner Switch ID", mTlv.mSpawnerData.mSwitchId);

        ADD_RESOURCE(AnimId::Slurg_Burst, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slurg_Move, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slurg_Turn_Around, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_Paramite final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Paramite::EntranceType>("Enum_ParamiteEntranceType",
        {
            {::Path_Paramite::EntranceType::ePatrol_0, "Normal Patrol"},
            {::Path_Paramite::EntranceType::eSurpriseWeb_1, "Normal Surprise Web"},
            {::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb_2, "Slightly Higher Spawn Surprise Web"},
            {::Path_Paramite::EntranceType::eUnused_ScaleToLeftGridSize_3, "Unused Scale To Left Grid Size"},
            {::Path_Paramite::EntranceType::eUnused_ScaleToRightGridSize_4, "Unused Scale To Right Grid Size"},
        });
    }

    CTOR_AE(Path_Paramite, "Paramite", TlvTypes::Paramite_26)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Entrance Type", mTlv.mEntranceType);
        ADD("Alone - Chase Delay", mTlv.mAloneChaseDelay);
        ADD("Surprise Web Delay", mTlv.mSurpriseWebDelayTimer);
        ADD("Meat Eating Time", mTlv.mMeatEatingTime);
        ADD("Group - Chase Delay", mTlv.mGroupChaseDelay);
        ADD("Disabled Resources", mTlv.field_1C_disabled_resources);
        ADD("Surprise Web Switch ID", mTlv.mSurpriseWebSwitchId);
        ADD("Hiss Before Attacking", mTlv.mHissBeforeAttack);
        ADD("Delete When Out Of Sight", mTlv.mDeleteWhenOutOfSight);
        ADD("Attack Fleeches", mTlv.mAttackFleeches);

        ADD_RESOURCE(AnimId::ParamiteWeb, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_AllOYaGameSpeakBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Attack, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_CloseAttack, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Death, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Eating, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Falling, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_GameSpeakBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_GameSpeakEnd, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Hiss, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Hop, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_JumpUpBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_JumpUpLand, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_JumpUpMidair, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Knockback, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Landing, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_PostHiss, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_PreHiss, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_RopePull, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_RunBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_RunEnd, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Running, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_RunningAttack, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Struggle, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::Paramite_Struggle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_SurpriseWeb, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Turn, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::Paramite_Unused, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::Paramite_Unused2, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WalkBegin, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WalkEnd, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_Walking, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WalkRunTransition, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WebGoingDown, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WebGoingUp, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WebGrab, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WebIdle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WebLeaveDown, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Paramite_WebLeaveUp, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ParamiteWebLine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ParamiteWebLine, "ParamiteWebLine", TlvTypes::ParamiteWebLine_99)
    {
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_MeatSack final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MeatSack, "MeatSack", TlvTypes::MeatSack_40)
    {
        ADD("Meat Fall Direction", mTlv.mMeatFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Amount Of Meat", mTlv.mMeatAmount);

        ADD_RESOURCE(AnimId::MeatSack_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::MeatSack_Hit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Bone, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_TorturedMudokon final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_TorturedMudokon, "TorturedMudokon", TlvTypes::TorturedMudokon_110)
    {
        ADD("Kill Switch ID", mTlv.mKillSwitchId);
        ADD("Release Switch ID", mTlv.mReleaseSwitchId);
        
        ADD_RESOURCE(AnimId::Tortured_Mudokon, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Tortured_Mudokon_Tears, ReliveAPI::AddResourceTo::File);
    }
};

} // namespace AETlvs

#undef EMPTY_CTOR_AE
#undef CTOR_AE
