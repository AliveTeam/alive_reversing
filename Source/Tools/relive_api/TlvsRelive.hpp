#pragma once

#include "TlvObjectBaseRelive.hpp"
#include "TlvObjectBaseMacros.hpp"
#include "../../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../../relive_lib/AnimResources.hpp"

#define CTOR_RELIVE(className, tlvEnumType)\
    className() : TlvObjectBaseRelive(sizeof(::className), tlvEnumType, className::kClassName, &mTlv)\
    {\
    }\
    \
    className(ReliveAPI::TypesCollectionBase& globalTypes, const relive::Path_TLV* pTlvSrc = nullptr) \
        : TlvObjectBaseRelive(sizeof(::className), globalTypes, tlvEnumType, className::kClassName, &mTlv)\
    {\
        if (pTlvSrc)\
        {\
            mTlv = *static_cast<const ::className*>(pTlvSrc);\
            ConvertXYPos(); \
        }\
        else\
        {\
            mPSelfTlv->mLength = static_cast<s16>(mSizeOfT);\
        }\
        AddProperties(globalTypes);\
    }\
    ::className mTlv = {};\
    void AddProperties(ReliveAPI::TypesCollectionBase& globalTypes)

#define EMPTY_CTOR_RELIVE() (void) globalTypes

namespace ReliveTlvs {

struct Path_TimedMine final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_TimedMine, ReliveTypes::eTimedMine)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
    }
};

struct Path_ElectricWall final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ElectricWall::ElectricWallStartState>("Enum_ElectricWallStartState",
        {
            {relive::Path_ElectricWall::ElectricWallStartState::eOff, "Off"},
            {relive::Path_ElectricWall::ElectricWallStartState::eOn, "On"},
        });
    }

    CTOR_RELIVE(relive::Path_ElectricWall, ReliveTypes::eElectricWall)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Start State", mTlv.mStartState);
    }
};

struct Path_Mudokon final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Mudokon::Mud_TLV_Emotion>("Enum_Mud_TLV_Emotion",
        {
            {relive::Path_Mudokon::Mud_TLV_Emotion::eNormal, "Normal"},
            {relive::Path_Mudokon::Mud_TLV_Emotion::eAngry, "Angry"},
            {relive::Path_Mudokon::Mud_TLV_Emotion::eSad, "Sad"},
            {relive::Path_Mudokon::Mud_TLV_Emotion::eWired, "Wired"},
            {relive::Path_Mudokon::Mud_TLV_Emotion::eSick, "Sick"}
        });

        types.AddEnum<relive::Path_Mudokon::MudJobs>("Enum_Mud_State",
        {
            {relive::Path_Mudokon::MudJobs::eChisle, "Chisle"},
            {relive::Path_Mudokon::MudJobs::eSitScrub, "Scrub"},
            {relive::Path_Mudokon::MudJobs::eAngryWorker, "Angry Worker"},
            {relive::Path_Mudokon::MudJobs::eDamageRingGiver, "Damage Ring Giver"},
            {relive::Path_Mudokon::MudJobs::eHealthRingGiver, "Health Ring Giver"},
        });
    }

    CTOR_RELIVE(relive::Path_Mudokon, ReliveTypes::eMudokon)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Job", mTlv.mJob);
        ADD("Facing", mTlv.mFacing);
        ADD("Voice Pitch", mTlv.mVoicePitch);
        ADD("Rescue Switch ID", mTlv.mRescueSwitchId);
        ADD("Deaf", mTlv.mDeaf);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Persist & Reset Offscreen", mTlv.mPersistAndResetOffscreen);
        ADD("Emotion", mTlv.mEmotion);
        ADD("Blind", mTlv.mBlind);
        ADD("Angry Switch ID", mTlv.mAngrySwitchId);
        ADD("Work After Turning Wheel", mTlv.mWorkAfterTurningWheel);
        ADD("Gets Depressed", mTlv.mGetsDepressed);
        ADD("Ring Pulse Interval", mTlv.mRingPulseInterval);
        ADD("Give RIng Without Password", mTlv.mGiveRingWithoutPassword);
    }
};

struct Path_BirdPortal final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BirdPortal::PortalType>("Enum_PortalType",
        {
            {relive::Path_BirdPortal::PortalType::eAbe, "Abe"},
            {relive::Path_BirdPortal::PortalType::eWorker, "Worker"},
            {relive::Path_BirdPortal::PortalType::eShrykull, "Shrykull"},
            {relive::Path_BirdPortal::PortalType::eMudTeleport, "Mud Teleport"},
        });

        types.AddEnum<relive::Path_BirdPortal::PortalSide>("Enum_PortalSide",
        {
            {relive::Path_BirdPortal::PortalSide::eRight, "Right"},
            {relive::Path_BirdPortal::PortalSide::eLeft, "Left"},
        });
    }

    CTOR_RELIVE(relive::Path_BirdPortal, ReliveTypes::eBirdPortal)
    {
        ADD("Enter Side", mTlv.mEnterSide);
        ADD("BirdPortalExit Level", mTlv.mExitLevel);
        ADD("BirdPortalExit Path", mTlv.mExitPath);
        ADD("BirdPortalExit Camera", mTlv.mExitCamera);
        ADD("Scale", mTlv.mScale);
        ADD("Movie ID", mTlv.mMovieId);
        ADD("Portal Type", mTlv.mPortalType);
        ADD("Mudokon Amount For Shrykull", mTlv.mMudCountForShrykull);
        ADD("(AE) Create Portal Switch ID", mTlv.mCreatePortalSwitchId);
        ADD("(AE) Delete Portal Switch ID", mTlv.mDeletePortalSwitchId);
    }
};

struct Path_LCDStatusBoard final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_LCDStatusBoard, ReliveTypes::eLCDStatusBoard)
    {
        ADD("(AE) Number Of Mudokons", mTlv.mNumberOfMuds);
        ADD("(AE) Zulag Number", mTlv.mZulagNumber);
        ADD("(AE) Hide Board", mTlv.mHideBoard);
    }
};

struct Path_Door final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Door::DoorStates>("Enum_DoorStates",
        {{relive::Path_Door::DoorStates::eOpen, "Open"},
        {relive::Path_Door::DoorStates::eClosed, "Closed"},
        {relive::Path_Door::DoorStates::eOpening, "Opening"},
        {relive::Path_Door::DoorStates::eClosing, "Closing"}});

        types.AddEnum<relive::Path_Door::DoorTypes>("Enum_DoorTypes",
        {{relive::Path_Door::DoorTypes::eBasicDoor, "Basic Door"},
        {relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic, "Tasks Door With Secret Music"},
        {relive::Path_Door::DoorTypes::eTasksDoor, "Tasks Door"},
        {relive::Path_Door::DoorTypes::eTrialDoor, "(AO) Trial Door"},
        {relive::Path_Door::DoorTypes::eHubDoor, "(AO) Hub Door"}});
    }

    CTOR_RELIVE(relive::Path_Door, ReliveTypes::eDoor)
    {
        ADD("Next Level", mTlv.mNextLevel);
        ADD("Next Path", mTlv.mNextPath);
        ADD("Next Camera", mTlv.mNextCamera);
        ADD("Scale", mTlv.mScale);
        ADD("Door ID", mTlv.mDoorId);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Target Door ID", mTlv.mTargetDoorId);
        ADD("Door Type", mTlv.mDoorType);
        ADD("(AO) Door Closed", mTlv.mDoorClosed);
        ADD("(AE) Start State", mTlv.mStartState);
        ADD("Hub 1 ID", mTlv.mHub1);
        ADD("Hub 2 ID", mTlv.mHub2);
        ADD("Hub 3 ID", mTlv.mHub3);
        ADD("Hub 4 ID", mTlv.mHub4);
        ADD("Hub 5 ID", mTlv.mHub5);
        ADD("Hub 6 ID", mTlv.mHub6);
        ADD("Hub 7 ID", mTlv.mHub7);
        ADD("Hub 8 ID", mTlv.mHub8);
        ADD("Wipe Effect", mTlv.mWipeEffect);
        ADD("Movie ID", mTlv.mMovieId);
        ADD("X Offset", mTlv.mDoorOffsetX);
        ADD("Y Offset", mTlv.mDoorOffsetY);
        ADD("Abe Direction On Exit", mTlv.mExitDirection);
        ADD("(AE) Close On Exit", mTlv.mCloseOnExit);
        ADD("(AE) Clear Throwables", mTlv.mClearThrowables);
    }
};

struct Path_Lever final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Lever::LeverSoundType>("Enum_LeverSoundType",
        {
            {relive::Path_Lever::LeverSoundType::eNone, "None"},
            {relive::Path_Lever::LeverSoundType::eWell, "Well"},
            {relive::Path_Lever::LeverSoundType::eSwitchBellHammer, "Switch Bell Hammer"},
            {relive::Path_Lever::LeverSoundType::eDoor, "Door"},
            {relive::Path_Lever::LeverSoundType::eElectricWall, "Electric Wall"},
            {relive::Path_Lever::LeverSoundType::eSecurityOrb, "Security Orb"},
            {relive::Path_Lever::LeverSoundType::eLift, "(AE) Lift"}
        });

        types.AddEnum<relive::Path_Lever::LeverSoundDirection>("Enum_LeverSoundDirection",
        {
            {relive::Path_Lever::LeverSoundDirection::eLeftAndRight, "Left And Right"},
            {relive::Path_Lever::LeverSoundDirection::eLeft, "Left"},
            {relive::Path_Lever::LeverSoundDirection::eRight, "Right"},
        });
    }

    CTOR_RELIVE(relive::Path_Lever, ReliveTypes::eLever)
    {
        ADD("Action", mTlv.mAction);
        ADD("Scale", mTlv.mScale);
        ADD("On Sound", mTlv.mOnSound);
        ADD("Off Sound", mTlv.mOffSound);
        ADD("Sound Direction", mTlv.mSoundDirection);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("(AE) Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Path_Hoist final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Hoist::Type>("Enum_HoistType",
        {
            {relive::Path_Hoist::Type::eNextFloor, "Next Floor"},
            {relive::Path_Hoist::Type::eOffScreen, "Off Screen"},
        });

        types.AddEnum<relive::Path_Hoist::GrabDirection>("Enum_HoistGrabDirection",
        {
            {relive::Path_Hoist::GrabDirection::eFacingLeft, "Facing Left"},
            {relive::Path_Hoist::GrabDirection::eFacingRight, "Facing Right"},
            {relive::Path_Hoist::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
        });
    }

    CTOR_RELIVE(relive::Path_Hoist, ReliveTypes::eHoist)
    {
        ADD("Hoist Type", mTlv.mHoistType);
        ADD("Grab Direction", mTlv.mGrabDirection);
        ADD("(AE) Scale", mTlv.mScale);
    }
};

struct Path_BoomMachine final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BoomMachine::NozzleSide>("Enum_BoomMachineNozzleSide",
        {
            {relive::Path_BoomMachine::NozzleSide::eRight, "Right"},
            {relive::Path_BoomMachine::NozzleSide::eLeft, "Left"},
        });
    }

    CTOR_RELIVE(relive::Path_BoomMachine, ReliveTypes::eBoomMachine)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Nozzle Side", mTlv.mNozzleSide);
        ADD("Number Of Grenades", mTlv.mGrenadeAmount);
    }
};

struct Path_Slig final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_Slig, ReliveTypes::eSlig)
    {
        ADD("Scale", mTlv.mData.mScale);
        ADD("Start State", mTlv.mData.mStartState);
        ADD("(AE) Death Mode", mTlv.mData.mDeathMode);
        ADD("(AE) Can Beat", mTlv.mData.mCanBeat);
        ADD("(AE) Can Z Shoot", mTlv.mData.mCanZShoot);
        ADD("Pause Time", mTlv.mData.mPauseTime);
        ADD("Pause Left Min", mTlv.mData.mPauseLeftMin);
        ADD("Pause Left Max", mTlv.mData.mPauseLeftMax);
        ADD("Pause Right Min", mTlv.mData.mPauseRightMin);
        ADD("Pause Right Max", mTlv.mData.mPauseRightMax);
        ADD("Shoot Possessed Sligs", mTlv.mData.mShootPossessedSligs);
        ADD("Shoot On Sight Delay", mTlv.mData.mShootOnSightDelay);
        ADD("Bullet Shoot Count", mTlv.mData.mNumTimesToShoot);

        ADD("Code 1", mTlv.mData.mCode1);
        ADD("Code 2", mTlv.mData.mCode2);
        ADD("Chase Abe When Spotted", mTlv.mData.mChaseAbeWhenSpotted);
        ADD("Facing", mTlv.mData.mFacing);
        ADD("Panic Timeout", mTlv.mData.mPanicTimeout);
        ADD("Stop Chase Delay", mTlv.mData.mStopChaseDelay);
        ADD("Time To Wait Before Chase", mTlv.mData.mTimeToWaitBeforeChase);
        ADD("Slig Bound/Persist ID", mTlv.mData.mSligBoundId);
        ADD("Alerted Listen Time", mTlv.mData.mAlertedListenTime);
        ADD("Percent Say What", mTlv.mData.mPercentSayWhat);
        ADD("Percent Beat Mudokon", mTlv.mData.mPercentBeatMud);
        ADD("Z Shoot Delay", mTlv.mData.mZShootDelay);
        ADD("Stay Awake", mTlv.mData.mStayAwake);
        ADD("Noise Wake Up Distance (Grids)", mTlv.mData.mNoiseWakeUpDistance);
        ADD("Slig Spawner Switch ID", mTlv.mData.mSligSpawnerSwitchId);
        ADD_HIDDEN("(AE) Unlimited Spawns", mTlv.mData.mUnlimitedSpawns);
        ADD("(AO) Disabled Resources", mTlv.mData.mDisabledResourcesAO.Raw().all);
        ADD("(AE) Disabled Resources", mTlv.mData.mDisabledResourcesAE);
    }
};

struct Path_Fleech final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_Fleech, ReliveTypes::eFleech)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Facing", mTlv.mFacing);
        ADD("Asleep", mTlv.mAsleep);
        ADD("Attack Anger Increaser", mTlv.mAttackAngerIncreaser);
        ADD("Wake Up Switch ID", mTlv.mWakeUpSwitchId);
        ADD("Hanging", mTlv.mHanging);
        ADD("Lost Target Timeout", mTlv.mLostTargetTimeout);
        ADD("Goes To Sleep", mTlv.mGoesToSleep);
        ADD("Patrol Range (Grids)", mTlv.mPatrolRangeInGrids);
        ADD("Wake Up Switch Anger Value", mTlv.mWakeUpSwitchAngerValue);
        ADD("Can Wake Up Switch ID", mTlv.mCanWakeUpSwitchId);
        ADD("Persistant", mTlv.mPersistant);
    }
};

struct Path_EnemyStopper final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
        {
            {relive::Path_EnemyStopper::StopDirection::Left, "Left"},
            {relive::Path_EnemyStopper::StopDirection::Right, "Right"},
            {relive::Path_EnemyStopper::StopDirection::Both, "Both"},
        });
    }

    CTOR_RELIVE(relive::Path_EnemyStopper, ReliveTypes::eEnemyStopper)
    {
        ADD("Stop Direction", mTlv.mStopDirection);
        ADD("Switch ID", mTlv.mSwitchId);
    }
};

struct Path_Teleporter final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_Teleporter, ReliveTypes::eTeleporter)
    {
        ADD("Teleporter Switch ID", mTlv.mTeleporterId);
        ADD("Other Teleporter Switch ID", mTlv.mOtherTeleporterId);
        ADD("Destination Camera", mTlv.mDestCamera);
        ADD("Destination Path", mTlv.mDestPath);
        ADD("Destination Level", mTlv.mDestLevel);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Screen Change Effect", mTlv.mWipeEffect);
        ADD("Movie ID", mTlv.mMovieId);
        ADD("Electric X", mTlv.mElectricX);
        ADD("Electric Y", mTlv.mElectricY);
    }
};

struct Path_UXB final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_UXB::StartState>("Enum_Path_UXB_StartState",
        {
            {relive::Path_UXB::StartState::eOn, "On"},
            {relive::Path_UXB::StartState::eOff, "Off"},
        });
    }

    CTOR_RELIVE(relive::Path_UXB, ReliveTypes::eUXB)
    {
        ADD("Pattern Length (Max 4)", mTlv.mPatternLength);
        ADD("Pattern", mTlv.mPattern);
        ADD("Scale", mTlv.mScale);
        ADD("Start State", mTlv.mStartState);
    }
};

struct Path_LCDScreen final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_LCDScreen, ReliveTypes::eLCDScreen)
    {
        ADD("Message 1 ID", mTlv.mMessageId1);
        ADD("Random Message Min ID", mTlv.mMessageRandMinId);
        ADD("Random Message Max ID", mTlv.mMessageRandMaxId);
        ADD("(AE) Message 2 ID", mTlv.mMessageId2);
        ADD("(AE) Toggle Message Switch ID", mTlv.mToggleMessageSwitchId);
    }
};

struct Path_Edge final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Edge::GrabDirection>("Enum_EdgeGrabDirection",
        {
            {relive::Path_Edge::GrabDirection::eFacingLeft, "Facing Left"},
            {relive::Path_Edge::GrabDirection::eFacingRight, "Facing Right"},
            {relive::Path_Edge::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
        });
    }

    CTOR_RELIVE(relive::Path_Edge, ReliveTypes::eEdge)
    {
        ADD("Grab Direction", mTlv.mGrabDirection);
        ADD("Can Grab", mTlv.mCanGrab);
        ADD("(AE) Scale", mTlv.mScale);
    }
};

struct Path_StatusLight final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_StatusLight, ReliveTypes::eStatusLight)
    {
        ADD("Input Switch ID", mTlv.mInputSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Linked Status Light Switch ID 1", mTlv.mLinkedStatusLightSwitchId1);
        ADD("Linked Status Light Switch ID 2", mTlv.mLinkedStatusLightSwitchId2);
        ADD("Linked Status Light Switch ID 3", mTlv.mLinkedStatusLightSwitchId3);
        ADD("Linked Status Light Switch ID 4", mTlv.mLinkedStatusLightSwitchId4);
        ADD("Linked Status Light Switch ID 5", mTlv.mLinkedStatusLightSwitchId5);
        ADD("Ignore Grid Snapping", mTlv.mIgnoreGridSnapping);
    }
};

struct Path_ShadowZone final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ShadowZone::Scale>("Enum_ShadowZoneScale",
        {
            {relive::Path_ShadowZone::Scale::eBoth, "Both"},
            {relive::Path_ShadowZone::Scale::eHalf, "Half"},
            {relive::Path_ShadowZone::Scale::eFull, "Full"},
        });
    }

    CTOR_RELIVE(relive::Path_ShadowZone, ReliveTypes::eShadowZone)
    {
        ADD("R", mTlv.mRGB.r);
        ADD("G", mTlv.mRGB.g);
        ADD("B", mTlv.mRGB.b);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_WorkWheel final : public ReliveAPI::TlvObjectBaseRelive
{
    CTOR_RELIVE(relive::Path_WorkWheel, ReliveTypes::eWorkWheel)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Activation Time", mTlv.mActivationTime);
        ADD("Turn Off Time", mTlv.mTurnOffTime);
        ADD("Turn Off When Stopped", mTlv.mTurnOffWhenStopped);
    }
};

struct Path_MusicTrigger final : public ReliveAPI::TlvObjectBaseRelive
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_MusicTrigger::MusicTriggerMusicType>("Enum_MusicTriggerMusicType",
        {
            {relive::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience, "Drum Ambience"},
            {relive::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort, "Death Drum Short"},
            {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong, "Secret Area Long"},
            {relive::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase, "Soft Chase"},
            {relive::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase, "Intense Chase"},
            {relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, "Chime"},
            {relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, "Secret Area Short"},
        });

        types.AddEnum<relive::Path_MusicTrigger::TriggeredBy>("Enum_MusicTriggeredBy",
        {
            {relive::Path_MusicTrigger::TriggeredBy::eTimer, "Timer"},
            {relive::Path_MusicTrigger::TriggeredBy::eTouching, "Touching"},
            {relive::Path_MusicTrigger::TriggeredBy::eSwitchId, "(AO) Switch ID"},
            {relive::Path_MusicTrigger::TriggeredBy::eUnknown, "(AO) Unknown"},
        });
    }

    CTOR_RELIVE(relive::Path_MusicTrigger, ReliveTypes::eMusicTrigger)
    {
        ADD("Music Type", mTlv.mMusicType);
        ADD("Triggered By", mTlv.mTriggeredBy);
        ADD("(AO) Switch ID", mTlv.mSwitchId);
        ADD("Music Delay", mTlv.mMusicDelay);
    }
};

} // namespace ReliveTlvs

#undef EMPTY_CTOR_RELIVE
#undef CTOR_RELIVE
