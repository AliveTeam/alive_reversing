#pragma once

#include "TlvObjectBaseRelive.hpp"
#include "TlvObjectBaseMacros.hpp"
#include "../../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../../relive_lib/AnimResources.hpp"
#include <type_traits>

#define CTOR_RELIVE(editorClassName, pathClassName, tlvEnumType)\
    editorClassName() : TlvObjectBaseRelive(sizeof(relive::pathClassName), tlvEnumType, relive::pathClassName::kClassName, &mTlv)\
    {\
    }\
    \
    editorClassName(ReliveAPI::TypesCollectionBase& globalTypes, const relive::Path_TLV* pTlvSrc = nullptr) \
        : TlvObjectBaseRelive(sizeof(relive::pathClassName), globalTypes, tlvEnumType, relive::pathClassName::kClassName, &mTlv)\
    {\
        if (pTlvSrc)\
        {\
            mTlv = *static_cast<const ::relive::pathClassName*>(pTlvSrc);\
            ConvertXYPos(); \
        }\
        else\
        {\
            mPSelfTlv->mLength = static_cast<s16>(mSizeOfT);\
        }\
        AddProperties(globalTypes);\
    }\
    relive::pathClassName mTlv = {};\
    void AddProperties(ReliveAPI::TypesCollectionBase& globalTypes)

#define EMPTY_CTOR_RELIVE() (void) globalTypes

namespace relive
{
struct TypeDescription;
}

// TODO: Move back to the editor src
class MapObjectBase
{
public:
    MapObjectBase(relive::Path_TLV* pTlv, const relive::TypeDescription* pTypes, u32 count)
     : mBaseTlv(pTlv), mTypes(pTypes), mCount(count)
    {

    }

    relive::Path_TLV* mBaseTlv = nullptr;
    const relive::TypeDescription* mTypes = nullptr;
    u32 mCount = 0;

    void SetXPos(s32 xpos)
    {
        mBaseTlv->mTopLeftX = xpos;
    }

    void SetYPos(s32 ypos)
    {
        mBaseTlv->mTopLeftY = ypos;
    }

    void SetWidth(s32 width)
    {
        mBaseTlv->mBottomRightX = width;
    }

    void SetHeight(s32 height)
    {
        mBaseTlv->mBottomRightY = height;
    }

    s32 XPos() const
    {
        return mBaseTlv->mTopLeftX;
    }

    s32 YPos() const
    {
        return mBaseTlv->mTopLeftY;
    }

    s32 Width() const
    {
        return mBaseTlv->Width();
    }

    s32 Height() const
    {
        return mBaseTlv->Height();
    }

    // TODO qint64
    const void* PropertyPtr(u32 propertyIdx) const;
    void* PropertyPtr(u32 propertyIdx);
    long long ReadBasicType(u32 propertyIdx) const;

    void SetBasicType(u32 propertyIdx, long long value);

};

namespace relive
{

enum class FieldType
{
    Field_U16,
    Field_S16,
};

struct TypeDescription
{
    const char* mFieldName;
    FieldType mFieldType;
    u32 mFieldOffset;
};

template<typename T>
constexpr FieldType FieldPointerToFieldType();

template<>
constexpr FieldType FieldPointerToFieldType<u16*>()
{
    return FieldType::Field_U16;
}

template<>
constexpr FieldType FieldPointerToFieldType<s16*>()
{
    return FieldType::Field_S16;
}

template<>
constexpr FieldType FieldPointerToFieldType<relive::reliveScale*>()
{
    return FieldPointerToFieldType<std::add_pointer_t<std::underlying_type_t<relive::reliveScale>>>();
}

// 0x1 is an arbitary non nullptr address as subtracting a nullptr is apparently UB
#define DEFINE_FIELD(editor_name,typeName,memberName) { editor_name, FieldPointerToFieldType<decltype(std::addressof(((typeName*)0x1)->memberName))>(), static_cast<u32>(std::addressof(((typeName*)0x1)->memberName) - ((decltype(std::addressof(((typeName*)0x1)->memberName)))0x1)) }




struct Editor_TimedMine final : public MapObjectBase
{
    /*
    CTOR_RELIVE(Editor_TimedMine, Path_TimedMine, ReliveTypes::eTimedMine)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
    }
    */

    Editor_TimedMine();

    Path_TimedMine mTlv;

    static const TypeDescription mSaveData[];
};

struct Editor_ElectricWall final : public MapObjectBase
{
    /*
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
    */
};

struct Editor_Mudokon final : public MapObjectBase
{
    /*
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
    */
};

struct Editor_BirdPortal final : public MapObjectBase
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BirdPortal::PortalType>("Enum_PortalType",
        {
            {relive::Path_BirdPortal::PortalType::eAbe, "Abe"},
            {relive::Path_BirdPortal::PortalType::eWorker, "Worker"},
            {relive::Path_BirdPortal::PortalType::eShrykull, "Shrykull"},
            {relive::Path_BirdPortal::PortalType::eMudTeleport, "Mud Teleport"},
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
    */
};

struct Editor_LCDStatusBoard final : public MapObjectBase
{
    /*
    CTOR_RELIVE(relive::Path_LCDStatusBoard, ReliveTypes::eLCDStatusBoard)
    {
        ADD("(AE) Number Of Mudokons", mTlv.mNumberOfMuds);
        ADD("(AE) Zulag Number", mTlv.mZulagNumber);
        ADD("(AE) Hide Board", mTlv.mHideBoard);
    }
    */
};

struct Editor_Door final : public MapObjectBase
{
    /*
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
    */
};

/*
struct Editor_Lever final : public MapObjectBase
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

struct Editor_Hoist final : public MapObjectBase
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

struct Editor_BoomMachine final : public MapObjectBase
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

struct Editor_Slig final : public MapObjectBase
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

struct Editor_Fleech final : public MapObjectBase
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

struct Editor_EnemyStopper final : public MapObjectBase
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

struct Editor_Teleporter final : public MapObjectBase
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

struct Editor_UXB final : public MapObjectBase
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

struct Editor_LCDScreen final : public MapObjectBase
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

struct Editor_Edge final : public MapObjectBase
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

struct Editor_StatusLight final : public MapObjectBase
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

struct Editor_ShadowZone final : public MapObjectBase
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

struct Editor_WorkWheel final : public MapObjectBase
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

struct Editor_MusicTrigger final : public MapObjectBase
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

struct Editor_AbeStart final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_AbeStart, ReliveTypes::eAbeStart)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_SoftLanding final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SoftLanding, ReliveTypes::eSoftLanding)
    {
        ADD("(AE) Switch ID", mTlv.mSwitchId);
    }
};

struct Editor_WellExpress final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_WellExpress, ReliveTypes::eWellExpress)
    {
        // Path_WellBase
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("(AO) Other Well ID", mTlv.mOtherWellId);
        ADD("(AO) Animation ID", mTlv.mAnimId); // leftover from AO well FG hack

        // Path_WellExpress
        ADD("Exit X", mTlv.mExitX);
        ADD("Exit Y", mTlv.mExitY);
        ADD("Disabled Well Level", mTlv.mOffDestLevel);
        ADD("Disabled Well Path", mTlv.mOffDestPath);
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

struct Editor_SlamDoor final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SlamDoor, ReliveTypes::eSlamDoor)
    {
        ADD("Start Shut", mTlv.mStartClosed);
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Flip On Y Axis", mTlv.mFlipY);
        ADD("Delete", mTlv.mDelete);
    }
};

struct Editor_HandStone final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_HandStone, ReliveTypes::eHandStone)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Camera ID 1", mTlv.mCameraId1);
        ADD("Camera ID 2", mTlv.mCameraId2);
        ADD("Camera ID 3", mTlv.mCameraId3);
        ADD("(AE) Trigger Switch ID", mTlv.mTriggerSwitchId);

        ADD("(AO) Level 1", mTlv.mLevel1);
        ADD("(AO) Path 1", mTlv.mPath1);
        ADD("(AO) Level 2", mTlv.mLevel2);
        ADD("(AO) Path 2", mTlv.mPath2);
        ADD("(AO) Level 3", mTlv.mLevel3);
        ADD("(AO) Path 3", mTlv.mPath3);
    }
};

struct Editor_LaughingGas final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_LaughingGas, ReliveTypes::eLaughingGas)
    {
        ADD("Is Laughing Gas", mTlv.mLaughingGas);
        ADD("Laughing Gas Switch ID", mTlv.mLaughingGasSwitchId);
        ADD("Red %", mTlv.mRedPercent);
        ADD("Green %", mTlv.mGreenPercent);
        ADD("Blue %", mTlv.mBluePercent);
    }
};

struct Editor_InvisibleSwitch final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_InvisibleSwitch::InvisibleSwitchScale>("Enum_InvisibleSwitchScale",
        {
            {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, "Half"},
            {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull, "Full"},
            {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny, "Any"},
        });
    }

    CTOR_RELIVE(relive::Path_InvisibleSwitch, ReliveTypes::eInvisibleSwitch)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("Delay", mTlv.mActivationDelay);
        ADD("Set Off Alarm", mTlv.mSetOffAlarm);
        ADD("Scale", mTlv.mScale);
    }
};

struct Editor_Water final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Water, ReliveTypes::eWater)
    {
        ADD("Max Drops", mTlv.mMaxDrops);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Splash Time", mTlv.mSplashTime);
        ADD("Splash X Velocity", mTlv.mSplashVelX);
        ADD("Water Duration", mTlv.mWaterDuration);
    }
};

struct Editor_GasEmitter final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_GasEmitter::GasColour>("Enum_GasColour",
        {
            {relive::Path_GasEmitter::GasColour::eYellow, "Yellow"},
            {relive::Path_GasEmitter::GasColour::eRed, "Red"},
            {relive::Path_GasEmitter::GasColour::eGreen, "Green"},
            {relive::Path_GasEmitter::GasColour::eBlue, "Blue"},
            {relive::Path_GasEmitter::GasColour::eWhite, "White"},
        });
    }

    CTOR_RELIVE(relive::Path_GasEmitter, ReliveTypes::eGasEmitter)
    {
        ADD("(AE) Switch ID", mTlv.mSwitchId);
        ADD("(AE) Colour", mTlv.mColour);
    }
};

struct Editor_BackgroundAnimation final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::relive::Path_BackgroundAnimation::BgAnimSounds>("Enum_BackgroundAnimation_BgAnimSounds",
        {
            {relive::Path_BackgroundAnimation::BgAnimSounds::eNone, "None"},
            {relive::Path_BackgroundAnimation::BgAnimSounds::eFire, "Fire"},
        });

        types.AddEnum<relive::Path_BackgroundAnimation::Layer>("Enum_Layer",
        {
            {relive::Path_BackgroundAnimation::Layer::eLayer0, "Layer 0"},
            {relive::Path_BackgroundAnimation::Layer::eLayer2, "Layer 2"},
        });

        types.AddEnum<relive::TBlendModes>("Enum_relive::TBlendModes",
        {
            {relive::TBlendModes::eBlend_0, "Blend 0"},
            {relive::TBlendModes::eBlend_1, "Blend 1"},
            {relive::TBlendModes::eBlend_2, "Blend 2"},
            {relive::TBlendModes::eBlend_3, "Blend 3"},
        });
    }

    CTOR_RELIVE(relive::Path_BackgroundAnimation, ReliveTypes::eBackgroundAnimation)
    {
        ADD("Animation ID", mTlv.mAnimId);
        ADD("Is Semi Trans", mTlv.mIsSemiTrans);
        ADD("Semi Trans Mode", mTlv.mSemiTransMode);
        ADD("(AO) Sound Effect", mTlv.mSoundEffect); // was used for campfires in AO but AE doesn't have them
        ADD("(AE) Layer", mTlv.mLayer);
    }
};

struct Editor_LiftPoint final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_LiftPoint::LiftPointStopType>("Enum_LiftPointStopType",
        {
            {relive::Path_LiftPoint::LiftPointStopType::eTopFloor, "Top Floor"},
            {relive::Path_LiftPoint::LiftPointStopType::eBottomFloor, "Bottom Floor"},
            {relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor, "Middle Floor"},
            {relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor, "Middle Lock Floor"},
            {relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly, "Start Point Only"},
        });
    }

    CTOR_RELIVE(relive::Path_LiftPoint, ReliveTypes::eLiftPoint)
    {
        ADD("Lift Point ID", mTlv.mLiftPointId);
        ADD("Start Point", mTlv.mIsStartPoint);
        ADD("Lift Point Stop Type", mTlv.mLiftPointStopType);
        ADD("Scale", mTlv.mScale);
        ADD("Ignore Lift Mover", mTlv.mIgnoreLiftMover);
    }
};

struct Editor_PullRingRope final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_PullRingRope::PullRingSwitchSound>("Enum_PullRingSwitchSound",
        {
            {relive::Path_PullRingRope::PullRingSwitchSound::eNone, "None"},
            {relive::Path_PullRingRope::PullRingSwitchSound::eWellExit, "Well Exit"},
            {relive::Path_PullRingRope::PullRingSwitchSound::eRingBellHammer, "Ring Unknown Trigger"},
            {relive::Path_PullRingRope::PullRingSwitchSound::eDoorEffect, "Door Effect"},
        });

        types.AddEnum<relive::Path_PullRingRope::PullRingSoundDirection>("Enum_PullRingSoundDirection",
        {
            {relive::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight, "Left And Right"},
            {relive::Path_PullRingRope::PullRingSoundDirection::eLeft, "Left"},
            {relive::Path_PullRingRope::PullRingSoundDirection::eRight, "Right"},
        });
    }

    CTOR_RELIVE(relive::Path_PullRingRope, ReliveTypes::ePullRingRope)
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

struct Editor_MultiSwitchController final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MultiSwitchController, ReliveTypes::eMultiSwitchController)
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

struct Editor_SecurityOrb final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SecurityOrb, ReliveTypes::eSecurityOrb)
    {
        ADD("Scale", mTlv.mScale);
    }
};

struct Editor_InvisibleZone final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_InvisibleZone, ReliveTypes::eInvisibleZone)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_ContinuePoint final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ContinuePoint::Scale>("Enum_ContinuePointScale",
        {
            {relive::Path_ContinuePoint::Scale::eNone, "None"},
            {relive::Path_ContinuePoint::Scale::eHalf, "Half"},
            {relive::Path_ContinuePoint::Scale::eFull, "Full"},
        });
    }

    CTOR_RELIVE(relive::Path_ContinuePoint, ReliveTypes::eContinuePoint)
    {
        ADD("(AO) Zone Number", mTlv.mZoneNumber);
        ADD("(AO) Clear From ID", mTlv.mClearFromId);
        ADD("(AO) Clear To ID", mTlv.mClearToId);
        ADD("(AO) Elum Restarts", mTlv.mElumRestarts);
        ADD("(AO) Abe Spawn Dir", mTlv.mAbeSpawnDir);
        ADD("(AE) Scale", mTlv.mScale);
        ADD("(AE) Save File ID", mTlv.mSaveFileId);
    }
};

struct Editor_WheelSyncer final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_WheelSyncer::OutputRequirement>("Enum_WheelSyncerOutputRequirement",
        {
            {relive::Path_WheelSyncer::OutputRequirement::eAllOn, "All On"},
            {relive::Path_WheelSyncer::OutputRequirement::e1OnAnd2Off, "1 On And 2 Off"},
            {relive::Path_WheelSyncer::OutputRequirement::e1Or2On, "1 Or 2 On"},
            {relive::Path_WheelSyncer::OutputRequirement::e1OnOr2Off, "1 On Or 2 Off"},
        });
    }

    CTOR_RELIVE(relive::Path_WheelSyncer, ReliveTypes::eWheelSyncer)
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

struct Editor_LevelLoader final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_LevelLoader, ReliveTypes::eLevelLoader)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Destination Level", mTlv.mDestLevel);
        ADD("Destination Path", mTlv.mDestPath);
        ADD("Destination Camera", mTlv.mDestCamera);
        ADD("Movie ID", mTlv.mMovieId);
    }
};

struct Editor_Pulley final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Pulley, ReliveTypes::ePulley)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_FlyingSlig final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_FlyingSlig, ReliveTypes::eFlyingSlig)
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
    }
};

struct Editor_FlyingSligSpawner final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_FlyingSligSpawner, ReliveTypes::eFlyingSligSpawner)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Spawn Delay State", mTlv.mSpawnDelayState);
        ADD("Spawn Move Delay", mTlv.mSpawnMoveDelay);
        ADD("Patrol Pause Min", mTlv.mPatrolPauseMin);
        ADD("Patrol Pause Max", mTlv.mPatrolPauseMax);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Panic Delay", mTlv.mPanicDelay);
        ADD("Give Up Chase Delay", mTlv.mGiveUpChaseDelay);
        ADD("Pre-Chase Delay", mTlv.mPrechaseDelay);
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Listen Time", mTlv.mAlertedListenTime);
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        ADD("Grenade Delay", mTlv.mGrenadeDelay);
        ADD("Max Velocity", mTlv.mMaxVelocity);
        ADD("Launch Grenade Switch ID", mTlv.mLaunchGrenadeSwitchId);
        ADD("Persistant", mTlv.mPersistant);
    }
};

struct Editor_DeathDrop final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_DeathDrop, ReliveTypes::eDeathDrop)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_SligSpawner final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SligSpawner, ReliveTypes::eSligSpawner)
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

struct Editor_SligBoundLeft final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SligBoundLeft, ReliveTypes::eSligBoundLeft)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_SligBoundRight final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SligBoundRight, ReliveTypes::eSligBoundRight)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_SligPersist final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SligPersist, ReliveTypes::eSligPersist)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_ZSligCover final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ZSligCover, ReliveTypes::eZSligCover)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_WellLocal final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_WellLocal, ReliveTypes::eWellLocal)
    {
        // Path_WellBase
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD_HIDDEN("(AO) Other Well ID", mTlv.mOtherWellId);     // hidden because this is only used by the well express
        ADD_HIDDEN("(AO) Animation ID (Unused?)", mTlv.mAnimId); // leftover from AO well FG hack
        ADD("(AO) Exit X", mTlv.mExitX);
        ADD("(AO) Exit Y", mTlv.mExitY);

        // Path_WellLocal
        ADD("(AO) Disabled XPos", mTlv.mOffDestX);
        ADD("(AO) Disabled YPos", mTlv.mOffDestY);
        ADD("Enabled XPos", mTlv.mOnDestX);
        ADD("Enabled YPos", mTlv.mOnDestY);
        ADD("Emit Leaves", mTlv.mEmitLeaves);
        ADD("Leaf XPos", mTlv.mLeafX);
        ADD("Leaf YPos", mTlv.mLeafY);
    }
};

struct Editor_BrewMachine final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_BrewMachine, ReliveTypes::eBrewMachine)
    {
        ADD("Brew Count", mTlv.mBrewCount);
    }
};

struct Editor_Drill final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Drill::DrillBehavior>("Enum_DrillBehavior",
        {
            {relive::Path_Drill::DrillBehavior::eNotInteractable, "Not Interactable"},
            {relive::Path_Drill::DrillBehavior::eToggle, "Toggle"},
            {relive::Path_Drill::DrillBehavior::eUse, "Use"},
        });

        types.AddEnum<relive::Path_Drill::DrillDirection>("Enum_DrillDirection",
        {
            {relive::Path_Drill::DrillDirection::eDown, "Down"},
            {relive::Path_Drill::DrillDirection::eRight, "Right"},
            {relive::Path_Drill::DrillDirection::eLeft, "Left"},
        });
    }

    CTOR_RELIVE(relive::Path_Drill, ReliveTypes::eDrill)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Min Off Time", mTlv.mOnMinPauseTime);
        ADD("Max Off Time", mTlv.mOnMaxPauseTime);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Behavior", mTlv.mDrillBehavior);
        ADD("Speed", mTlv.mOnSpeed);
        ADD("Start State On", mTlv.mStartStateOn);
        ADD("Off Speed", mTlv.mOffSpeed);
        ADD("Min Off Time Speed Change", mTlv.mOffMinPauseTime);
        ADD("Max Off Time Speed Change", mTlv.mOffMaxPauseTime);
        ADD("Start Position Bottom", mTlv.mStartPositionBottom);
        ADD("Start Direction", mTlv.mDrillDirection);
    }
};

struct Editor_Mine final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Mine, ReliveTypes::eMine)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Editor_Slog final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Slog, ReliveTypes::eSlog)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Asleep", mTlv.mAsleep);
        ADD("Wake Up Anger", mTlv.mWakeUpAnger);
        ADD("Bark Anger", mTlv.mBarkAnger);
        ADD("Chase Anger", mTlv.mChaseAnger);
        ADD("Chase Delay", mTlv.mChaseDelay);
        ADD("Anger Switch ID", mTlv.mAngerSwitchId);
        ADD("(AE) Bone Eating Time", mTlv.mBoneEatingTime);
    }
};

struct Editor_ResetPath final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ResetPath, ReliveTypes::eResetPath)
    {
        ADD("Clear Ids", mTlv.mClearIds);
        ADD("From", mTlv.mFrom);
        ADD("To", mTlv.mTo);
        ADD("Exclude", mTlv.mExclude);
        ADD("Clear Objects", mTlv.mClearObjects);
        ADD("Path", mTlv.mPath);
        ADD("(AE) Enabled", mTlv.mEnabled);
    }
};

struct Editor_TrapDoor final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_TrapDoor::StartState>("Enum_TrapDoorStartState",
        {{relive::Path_TrapDoor::StartState::eOpen, "Open"},
        {relive::Path_TrapDoor::StartState::eClosed, "Closed"}});
    }

    CTOR_RELIVE(relive::Path_TrapDoor, ReliveTypes::eTrapDoor)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Start State", mTlv.mStartState);
        ADD("Self Closing", mTlv.mSelfClosing);
        ADD("Scale", mTlv.mScale);
        ADD("Direction", mTlv.mDirection);
        ADD("X Offset", mTlv.mXOff);
        ADD("(AE) Stay Open Time", mTlv.mStayOpenTime);
    }
};

struct Editor_PathTransition final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_PathTransition, ReliveTypes::ePathTransition)
    {
        ADD("Next Level", mTlv.mNextLevel);
        ADD("Next Path", mTlv.mNextPath);
        ADD("Next Camera", mTlv.mNextCamera);
        ADD("Movie ID", mTlv.mMovieId);
        ADD("Wipe Effect", mTlv.mWipeEffect);
        ADD("Next Path Scale", mTlv.mNextPathScale);
    }
};

struct Editor_LiftMover final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_LiftMover::YDirection>("Enum_LiftMoverYDirection",
        {
            {relive::Path_LiftMover::YDirection::eDown, "Down"},
            {relive::Path_LiftMover::YDirection::eUp, "Up"},
        });
    }

    CTOR_RELIVE(relive::Path_LiftMover, ReliveTypes::eLiftMover)
    {
        ADD("Lift Mover Switch ID", mTlv.mLiftMoverSwitchId);
        ADD("Target Lift Point ID", mTlv.mTargetLiftPointId);
        ADD("Move Direction", mTlv.mMoveDirection);
    }
};

struct Editor_RockSack final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_RockSack, ReliveTypes::eRockSack)
    {
        ADD("Rock Fall Direction", mTlv.mRockFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Rock Amount", mTlv.mRockAmount);
    }
};

struct Editor_TimerTrigger final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_TimerTrigger, ReliveTypes::eTimerTrigger)
    {
        ADD("Input Switch ID", mTlv.mInputSwitchId);
        ADD("Activation Delay", mTlv.mActivationDelay);
        ADD("Output Switch ID 1", mTlv.mOutputSwitchId1);
        ADD("Output Switch ID 2", mTlv.mOutputSwitchId2);
        ADD("Output Switch ID 3", mTlv.mOutputSwitchId3);
        ADD("Output Switch ID 4", mTlv.mOutputSwitchId4);
    }
};

struct Editor_MotionDetector final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_MotionDetector::InitialMoveDirection>("Enum_InitialMoveDirection",
        {
            {relive::Path_MotionDetector::InitialMoveDirection::eRight, "Right"},
            {relive::Path_MotionDetector::InitialMoveDirection::eLeft, "Left"},
        });
    }

    CTOR_RELIVE(relive::Path_MotionDetector, ReliveTypes::eMotionDetector)
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
    }
};

struct Editor_MineCar final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MineCar, ReliveTypes::eMineCar)
    {
        ADD("Scale", mTlv.mScale);
    }
};

struct Editor_ExplosionSet final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ExplosionSet, ReliveTypes::eExplosionSet)
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
    }
};

struct Editor_ColourfulMeter final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ColourfulMeter, ReliveTypes::eColourfulMeter)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Number Of Meter Bars", mTlv.mNumberOfMeterBars);
        ADD("Mines Alarm Countdown (Seconds)", mTlv.mMinesAlarmCountdown);
        ADD("Start Filled", mTlv.mStartFilled);
    }
};

struct Editor_Alarm final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Alarm, ReliveTypes::eAlarm)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Duration", mTlv.mAlarmDuration);
    }
};

struct Editor_DemoSpawnPoint final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_DemoSpawnPoint, ReliveTypes::eDemoSpawnPoint)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_SlapLock final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SlapLock, ReliveTypes::eSlapLock)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Target Tomb ID 1", mTlv.mTargetTombSwitchId1);
        ADD("Target Tomb ID 2", mTlv.mTargetTombSwitchId2);
        ADD("Has Ghost", mTlv.mHasGhost);
        ADD("Give Invisibility Power-up", mTlv.mGiveInvisibilityPowerup);
        ADD("Invisibility Duration", mTlv.mInvisibilityDuration);
        ADD("Toggle Switch ID", mTlv.mSlapOutputSwitchId);
    }
};

struct Editor_Slurg final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Slurg, ReliveTypes::eSlurg)
    {
        ADD("Time Until Turning Around", mTlv.mMovingTimer);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID (increment by 1 on death)", mTlv.mSlurgSwitchId);
    }
};

struct Editor_DoorBlocker final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_DoorBlocker, ReliveTypes::eDoorBlocker)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
    }
};

struct Editor_Dove final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Dove, ReliveTypes::eDove)
    {
        ADD("Dove Count", mTlv.mDoveCount);
        ADD("Pixel Perfect", mTlv.mPixelPerfect);
        ADD("Scale", mTlv.mScale);
    }
};

struct Editor_BirdPortalExit final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_BirdPortalExit, ReliveTypes::eBirdPortalExit)
    {
        ADD("Exit Direction", mTlv.mExitSide);
        ADD("Scale", mTlv.mScale);
    }
};

struct Editor_DoorFlame final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_DoorFlame::Colour>("Enum_DoorFlameColour",
        {
            {relive::Path_DoorFlame::Colour::eDefault, "Default"},
            {relive::Path_DoorFlame::Colour::eRed, "Red"},
            {relive::Path_DoorFlame::Colour::eGreen, "Green"},
            {relive::Path_DoorFlame::Colour::eBlue, "Blue"},
        });
    }

    CTOR_RELIVE(relive::Path_DoorFlame, ReliveTypes::eDoorFlame)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Colour", mTlv.mColour);
    }
};

struct Editor_TrainDoor final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_TrainDoor, ReliveTypes::eTrainDoor)
    {
        ADD("Direction", mTlv.mDirection);
    }
};

struct Editor_Greeter final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Greeter, ReliveTypes::eGreeter)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Motion Detector Speed", mTlv.mMotionDetectorSpeed);
        ADD("Facing", mTlv.mFacing);
    }
};

struct Editor_ScrabBoundLeft final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ScrabBoundLeft, ReliveTypes::eScrabLeftBound)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_ScrabBoundRight final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ScrabBoundRight, ReliveTypes::eScrabRightBound)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_CreditsController final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_CreditsController, ReliveTypes::eCreditsController)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_MovieStone final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MovieStone, ReliveTypes::eMovieHandStone)
    {
        ADD("Movie ID", mTlv.mMovieId);
        ADD("Scale", mTlv.mScale);
        ADD("(AE) Trigger Switch ID", mTlv.mTriggerSwitchId);
    }
};

struct Editor_MovingBomb final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MovingBomb, ReliveTypes::eMovingBomb)
    {
        ADD("Speed", mTlv.mSpeed);
        ADD("Start Moving Switch ID", mTlv.mStartMovingSwitchId);
        ADD("Triggered By Alarm", mTlv.mTriggeredByAlarm);
        ADD("Scale", mTlv.mScale);
        ADD("Start Speed", mTlv.mStartSpeed);
        ADD("Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Editor_MovingBombStopper final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MovingBombStopper, ReliveTypes::eMovingBombStopper)
    {
        ADD("Min Delay", mTlv.mMinStopTime);
        ADD("Max Delay", mTlv.mMaxStopTime);
    }
};

struct Editor_SecurityDoor final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SecurityDoor, ReliveTypes::eSecurityDoor)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("X Position", mTlv.mXPos);
        ADD("Y Position", mTlv.mYPos);
    }
};

struct Editor_CrawlingSlig final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_CrawlingSlig::StartState>("Enum_CrawlingSligState",
        {
            {relive::Path_CrawlingSlig::StartState::eSleeping, "Sleeping"},
            {relive::Path_CrawlingSlig::StartState::eAwake, "Awake"},
        });

        types.AddEnum<relive::Path_CrawlingSlig::CrawlDirection>("Enum_CrawlDirection",
        {
            {relive::Path_CrawlingSlig::CrawlDirection::eLeft, "Left"},
            {relive::Path_CrawlingSlig::CrawlDirection::eRight, "Right"},
            {relive::Path_CrawlingSlig::CrawlDirection::eRandom, "Random"},
        });
    }

    CTOR_RELIVE(relive::Path_CrawlingSlig, ReliveTypes::eCrawlingSlig)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Start State", mTlv.mStartState);
        ADD("Crawl Direction", mTlv.mCrawlDirection);
        ADD("Panic Switch ID", mTlv.mPanicSwitchId);
        ADD("Respawn On Death", mTlv.mRespawnOnDeath);
    }
};

struct Editor_SligGetPants final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SligGetPants, ReliveTypes::eSligGetPants)
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

struct Editor_SligGetWings final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SligGetWings, ReliveTypes::eSligGetWings)
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
    }
};

struct Editor_CrawlingSligButton final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        // this probably shouldn't be here since the sound/sound direction implementation is extremely scuffed
        // but maybe someone wants to use it anyway
        types.AddEnum<relive::Path_CrawlingSligButton::ButtonSounds>("Enum_CrawlingSligButtonSounds",
        {
            {relive::Path_CrawlingSligButton::ButtonSounds::None, "None"},
            {relive::Path_CrawlingSligButton::ButtonSounds::SackHit1, "Sack Hit 1"},
            {relive::Path_CrawlingSligButton::ButtonSounds::SackHit2, "Sack Hit 2"},
            {relive::Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, "Falling Item Presence 2"},
            {relive::Path_CrawlingSligButton::ButtonSounds::SecurityOrb, "Security Orb"},
            {relive::Path_CrawlingSligButton::ButtonSounds::Bullet1, "Bullet 1"},
            {relive::Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, "Abe Generic Movement"},
        });
    }
    CTOR_RELIVE(relive::Path_CrawlingSligButton, ReliveTypes::eCrawlingSligButton)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("On Sound", mTlv.mOnSound);
        ADD("Off Sound", mTlv.mOffSound);
        ADD_HIDDEN("Sound Direction", mTlv.mSoundDirection);
    }
};

struct Editor_Glukkon final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Glukkon::GlukkonTypes>("Enum_GlukkonTypes",
        {
            {relive::Path_Glukkon::GlukkonTypes::eNormal, "Normal"},
            {relive::Path_Glukkon::GlukkonTypes::eStoryAslik, "Story Aslik"},
            {relive::Path_Glukkon::GlukkonTypes::eStoryDripik, "Story Dripik"},
            {relive::Path_Glukkon::GlukkonTypes::eStoryPhleg, "Story Phleg"},
        });

        types.AddEnum<relive::Path_Glukkon::Facing>("Enum_GlukkonFacing",
        {
            {relive::Path_Glukkon::Facing::eRight, "Right"},
            {relive::Path_Glukkon::Facing::eLeft, "Left"},
        });

        types.AddEnum<relive::Path_Glukkon::SpawnType>("Enum_GlukkonSpawnType",
        {
            {relive::Path_Glukkon::SpawnType::eRegularSpawn, "Regular Spawn"},
            {relive::Path_Glukkon::SpawnType::eFacingLeft, "Facing Left"},
            {relive::Path_Glukkon::SpawnType::eFacingRight, "Facing Right"},
            {relive::Path_Glukkon::SpawnType::eFullSpawnEffects, "Full Spawn Effects"},
        });

        types.AddEnum<relive::Path_Glukkon::Behavior>("Enum_GlukkonBehavior",
        {
            {relive::Path_Glukkon::Behavior::eIgnoreWalls, "Ignore Walls"},
            {relive::Path_Glukkon::Behavior::eCheckForWalls, "Check For Walls"},
        });
    }

    CTOR_RELIVE(relive::Path_Glukkon, ReliveTypes::eGlukkon)
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
    }
};

struct Editor_GlukkonSwitch final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_GlukkonSwitch, ReliveTypes::eGlukkonSwitch)
    {
        ADD("Scale", mTlv.mScale);
        ADD("OK Switch ID", mTlv.mOkSwitchId);
        ADD("Fail Switch ID", mTlv.mFailSwitchId);
        ADD("X Position", mTlv.mXPos);
        ADD("Y Position", mTlv.mYPos);
    }
};

struct Editor_GasCountDown final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_GasCountDown, ReliveTypes::eGasCountDown)
    {
        ADD("Start Timer Switch ID", mTlv.mStartTimerSwitchId);
        ADD("(AE) Gas Countdown Time", mTlv.mGasCountdownTimer);
        ADD("(AE) Stop Timer Switch ID", mTlv.mStopTimerSwitchId);
    }
};

struct Editor_FallingItem final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_FallingItem, ReliveTypes::eFallingItem)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Fall Interval", mTlv.mFallInterval);
        ADD("Max Falling Items", mTlv.mMaxFallingItems);
        ADD("Reset Switch ID After Use", mTlv.mResetSwitchIdAfterUse);
    }
};

struct Editor_BoneBag final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_BoneBag, ReliveTypes::eBoneBag)
    {
        ADD("Bone Fall Direction", mTlv.mBoneFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Bone Amount", mTlv.mBoneAmount);
    }
};

struct Editor_SecurityClaw final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SecurityClaw, ReliveTypes::eSecurityClaw)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Alarm Switch ID", mTlv.mAlarmSwitchId);
        ADD("Alarm Duration", mTlv.mAlarmDuration);
    }
};

struct Editor_FootSwitch final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_FootSwitch::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
        {
            {relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe, "Abe"},
            {relive::Path_FootSwitch::FootSwitchTriggerBy::eAnyone, "Anyone"},
        });
    }

    CTOR_RELIVE(relive::Path_FootSwitch, ReliveTypes::eFootSwitch)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Action", mTlv.mAction);
        ADD("Triggered By", mTlv.mTriggeredBy);
    }
};

struct Editor_ZzzSpawner final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ZzzSpawner, ReliveTypes::eZzzSpawner)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Zzz Interval", mTlv.mZzzInterval);
    }
};

struct Editor_SlogSpawner final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_SlogSpawner::StartDirection>("Enum_SlogSpawnerStartDirection",
        {
            {relive::Path_SlogSpawner::StartDirection::eRight, "Right"},
            {relive::Path_SlogSpawner::StartDirection::eLeft, "Left"},
        });
    }

    CTOR_RELIVE(relive::Path_SlogSpawner, ReliveTypes::eSlogSpawner)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Max Slogs", mTlv.mMaxSlogs);
        ADD("Max Slogs At A Time", mTlv.mMaxSlogsAtATime);
        ADD("Start Direction", mTlv.mStartDirection);
        ADD("Slog Spawn Interval", mTlv.mSlogSpawnInterval);
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        ADD("(AE) Listen To Sligs", mTlv.mListenToSligs);
        ADD("(AE) Chase Delay", mTlv.mChaseDelay);
    }
};

struct Editor_MainMenuController final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MainMenuController, ReliveTypes::eMainMenuController)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_Scrab final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Scrab, ReliveTypes::eScrab)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Attack Delay", mTlv.mAttackDelay);
        ADD("(AE) Patrol Type Run Or Walk Chance (6 Max)", mTlv.mPatrolTypeRunOrWalkChance);
        ADD("(AO) Patrol Type", mTlv.mPatrolType);
        ADD("Left Min Delay", mTlv.mPauseLeftMin);
        ADD("Left Max Delay", mTlv.mPauseLeftMax);
        ADD("Right Min Delay", mTlv.mPauseRightMin);
        ADD("Right Max Delay", mTlv.mPauseRightMax);
        ADD("Pause After Chase Delay", mTlv.mPauseAfterChaseTime);
        ADD("Spotting Abe Delay", mTlv.mSpottingAbeDelay);
        ADD("Roar Randomly", mTlv.mRoarRandomly);
        ADD("(AE) Persistant", mTlv.mPersistant);
        ADD("(AE) Possessed Max Whirl Attack Duration", mTlv.mPossessedMaxWhirlAttackDuration);
        ADD("(AE) Kill Enemy", mTlv.mKillEnemy);
    }
};

struct Editor_ScrabSpawner final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ScrabSpawner::SpawnDirection>("Enum_ScrabSpawnDirection",
        {
            {relive::Path_ScrabSpawner::SpawnDirection::eNone, "None"},
            {relive::Path_ScrabSpawner::SpawnDirection::eLeft, "Left"},
            {relive::Path_ScrabSpawner::SpawnDirection::eRight, "Right"},
        });
    }

    CTOR_RELIVE(relive::Path_ScrabSpawner, ReliveTypes::eScrabSpawner)
    {
        // Scrab properties
        ADD("Scale", mTlv.mScale);
        ADD("Attack Delay", mTlv.mAttackDelay);
        ADD("(AE) Patrol Type Run Or Walk Chance (6 Max)", mTlv.mPatrolTypeRunOrWalkChance);
        ADD("(AO) Patrol Type", mTlv.mPatrolType);
        ADD("Left Min Delay", mTlv.mPauseLeftMin);
        ADD("Left Max Delay", mTlv.mPauseLeftMax);
        ADD("Right Min Delay", mTlv.mPauseRightMin);
        ADD("Right Max Delay", mTlv.mPauseRightMax);
        ADD("Pause After Chase Delay", mTlv.mPauseAfterChaseTime);
        ADD("Spotting Abe Delay", mTlv.mSpottingAbeDelay);
        ADD("Roar Randomly", mTlv.mRoarRandomly);
        ADD("(AE) Persistant", mTlv.mPersistant);
        ADD("(AE) Possessed Max Whirl Attack Duration", mTlv.mPossessedMaxWhirlAttackDuration);
        ADD("(AE) Kill Enemy", mTlv.mKillEnemy);

        // Spawner properties
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        ADD("Facing", mTlv.mFacing);
    }
};

struct Editor_SlurgSpawner final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SlurgSpawner, ReliveTypes::eSlurgSpawner)
    {
        // Slurg properties
        ADD("Time Until Turning Around", mTlv.mMovingTimer);
        ADD("Facing", mTlv.mFacing);
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID (increment by 1 on death)", mTlv.mSlurgSwitchId);

        // Spawner properties
        ADD("Spawn Interval", mTlv.mSpawnInterval);
        ADD("Max Slurgs", mTlv.mMaxSlurgs);
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);
    }
};

struct Editor_Paramite final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Paramite::EntranceType>("Enum_ParamiteEntranceType",
        {
            {relive::Path_Paramite::EntranceType::ePatrol, "Patrol"},
            {relive::Path_Paramite::EntranceType::eSurpriseWeb, "Surprise Web"},
            {relive::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, "Slightly Higher Spawn Surprise Web"},
        });
    }

    CTOR_RELIVE(relive::Path_Paramite, ReliveTypes::eParamite)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Entrance Type", mTlv.mEntranceType);
        ADD("Alone - Chase Delay", mTlv.mAloneChaseDelay);
        ADD("Surprise Web Delay", mTlv.mSurpriseWebDelayTimer);
        ADD("Meat Eating Time", mTlv.mMeatEatingTime);
        ADD("Group - Chase Delay", mTlv.mGroupChaseDelay);
        ADD("Surprise Web Switch ID", mTlv.mSurpriseWebSwitchId);
        ADD("Hiss Before Attacking", mTlv.mHissBeforeAttack);
        ADD("Delete When Out Of Sight", mTlv.mDeleteWhenOutOfSight);
        ADD("(AE) Attack Fleeches", mTlv.mAttackFleeches);
    }
};

struct Editor_ParamiteWebLine final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ParamiteWebLine, ReliveTypes::eParamiteWebLine)
    {
        ADD("Scale", mTlv.mScale);
    }
};

struct Editor_MeatSack final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MeatSack, ReliveTypes::eMeatSack)
    {
        ADD("Meat Fall Direction", mTlv.mMeatFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Amount Of Meat", mTlv.mMeatAmount);
    }
};

struct Editor_TorturedMudokon final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_TorturedMudokon, ReliveTypes::eTorturedMud)
    {
        ADD("Kill Switch ID", mTlv.mKillSwitchId);
        ADD("Release Switch ID", mTlv.mReleaseSwitchId);
    }
};

struct Editor_KillUnsavedMuds final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_KillUnsavedMuds, ReliveTypes::eKillUnsavedMuds)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_BackgroundGlukkon final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_BackgroundGlukkon, ReliveTypes::eBackgroundGlukkon)
    {
        ADD("Scale Percent", mTlv.mScalePercent);
        ADD("Pal ID", mTlv.mPalId);
    }
};

struct Editor_BellsongStone final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BellsongStone::BellsongTypes>("Enum_BellsongTypes",
        {
            {relive::Path_BellsongStone::BellsongTypes::eWhistle, "Whistle"},
            {relive::Path_BellsongStone::BellsongTypes::eChimes, "Chimes"},
        });
    }

    CTOR_RELIVE(relive::Path_BellsongStone, ReliveTypes::eBellSongStone)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Type", mTlv.mType);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("Switch ID", mTlv.mSwitchId);
    }
};

struct Editor_LightEffect final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_LightEffect::Type>("Enum_LightType",
        {
            {relive::Path_LightEffect::Type::Star, "Star"},
            {relive::Path_LightEffect::Type::GoldGlow, "Gold Glow"},
            {relive::Path_LightEffect::Type::GreenGlow, "Green Glow"},
            {relive::Path_LightEffect::Type::FlintGlow, "Flint Glow"},
            {relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights, "RedGreen Door Light"},
            {relive::Path_LightEffect::Type::Switchable_RedGreenHubLight, "RedGreen Hub Light"},
        });
    }

    CTOR_RELIVE(relive::Path_LightEffect, ReliveTypes::eLightEffect)
    {
        ADD("Type", mTlv.mType);
        ADD("Size", mTlv.mSize);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Direction", mTlv.mDirection);
    }
};

struct Editor_StartController final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_StartController, ReliveTypes::eStartController)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_HintFly final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_HintFly, ReliveTypes::eHintFly)
    {
        ADD("Message ID", mTlv.mMessageId);
    }
};

struct Editor_Bat final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Bat, ReliveTypes::eBat)
    {
        ADD("Time Before Moving", mTlv.mTimeBeforeMoving);
        ADD("Speed", mTlv.mSpeed);
        ADD("Scale", mTlv.mScale);
        ADD("Attack Duration", mTlv.mAttackDuration);
    }
};

struct Editor_BellHammer final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_BellHammer, ReliveTypes::eBellHammer)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("Scale", mTlv.mScale);
        ADD("Direction", mTlv.mDirection);
    }
};

struct Editor_ElumPathTrans final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ElumPathTrans, ReliveTypes::eElumPathTrans)
    {
        ADD("Level", mTlv.mNextLevel);
        ADD("Path", mTlv.mNextPath);
        ADD("Camera", mTlv.mNextCamera);
    }
};

struct Editor_ElumStart final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ElumStart, ReliveTypes::eElumStart)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_ElumWall final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ElumWall, ReliveTypes::eElumWall)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_RingMudokon final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_RingMudokon::MustFaceMud>("Enum_RingMudokon_MustFaceMud",
        {
            {relive::Path_RingMudokon::MustFaceMud::eYes, "Yes"},
            {relive::Path_RingMudokon::MustFaceMud::eNo, "No"},
        });
    }

    CTOR_RELIVE(relive::Path_RingMudokon, ReliveTypes::eRingMudokon)
    {
        ADD("Start Direction", mTlv.mFacing);
        ADD("Abe Must Face Mudokon", mTlv.mAbeMustFaceMud);
        ADD("Scale", mTlv.mScale);
        ADD("Give Password", mTlv.mGivePassword);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("Action", mTlv.mAction);
        ADD("Ring And Angry Mudokon Timeout", mTlv.mRingTimeout);
        ADD("Give Ring Without Password", mTlv.mGiveRingWithoutPassword);
    }
};

struct Editor_RingCancel final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_RingCancel, ReliveTypes::eRingCancel)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_MeatSaw final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_MeatSaw::Type>("Enum_MeatSawType",
        {
            {relive::Path_MeatSaw::Type::eAutomaticPersistOffscreen, "Automatic Persist Offscreen"},
            {relive::Path_MeatSaw::Type::eAutomatic, "Automatic"},
            {relive::Path_MeatSaw::Type::eSwitchId, "Switch ID"},
        });

        types.AddEnum<relive::Path_MeatSaw::StartState>("Enum_MeatSawStartState",
        {
            {relive::Path_MeatSaw::StartState::eOff, "Off"},
            {relive::Path_MeatSaw::StartState::eOn, "On"},
        });
    }

    CTOR_RELIVE(relive::Path_MeatSaw, ReliveTypes::eMeatSaw)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch Min Time Off", mTlv.mSwitchMinTimeOff);
        ADD("Switch Max Time Off", mTlv.mSwitchMaxTimeOff);
        ADD("Y Travel Distance", mTlv.mYTravelDistance);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Type", mTlv.mType);
        ADD("Switch Speed", mTlv.mSwitchSpeed);
        ADD("Start State", mTlv.mStartState);
        ADD("Automatic Speed", mTlv.mAutomaticSpeed);
        ADD("Automatic Min Time Off", mTlv.mAutomaticMinTimeOff);
        ADD("Automatic Max Time Off", mTlv.mAutomaticMaxTimeOff);
        ADD("Start At Bottom", mTlv.mStartAtBottom);
    }
};

struct Editor_ChimeLock final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ChimeLock, ReliveTypes::eChimeLock)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Solve Switch ID", mTlv.mSolveSwitchId);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("Password Switch ID", mTlv.mPasswordSwitchId);
    }
};

struct Editor_FlintLockFire final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_FlintLockFire, ReliveTypes::eFlintLockFire)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
    }
};

struct Editor_MudokonPathTrans final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_MudokonPathTrans, ReliveTypes::eMudokonPathTrans)
    {
        ADD("Next Level", mTlv.mNextLevel);
        ADD("Next Path", mTlv.mNextPath);
        ADD("Next Camera", mTlv.mNextCamera);
    }
};

struct Editor_ScrabNoFall final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_ScrabNoFall, ReliveTypes::eScrabNoFall)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_LiftMudokon final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_LiftMudokon::Direction>("Enum_LiftMudokonDirection",
        {
            {relive::Path_LiftMudokon::Direction::eRight, "Right"},
            {relive::Path_LiftMudokon::Direction::eLeft, "Left"},
        });
    }

    CTOR_RELIVE(relive::Path_LiftMudokon, ReliveTypes::eLiftMudokon)
    {
        ADD("How Far To Walk", mTlv.mHowFarToWalk);
        ADD("Lift Switch ID", mTlv.mLiftSwitchId);
        ADD("Facing", mTlv.mFacing);
        ADD("Give Password", mTlv.mGivePassword);
        ADD("Scale", mTlv.mScale);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
    }
};

struct Editor_HoneySack final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_HoneySack, ReliveTypes::eHoneySack)
    {
        ADD("Chase Time", mTlv.mChaseTime);
        ADD("Scale", mTlv.mScale);
    }
};

struct Editor_SlingMudokon final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_SlingMudokon, ReliveTypes::SlingMud)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Don't Whistle Password", mTlv.mDontWhistlePassword);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
    }
};

struct Editor_BeeSwarmHole final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BeeSwarmHole::MovementType>("Enum_BeeSwarmHoleMovementType",
        {
            {relive::Path_BeeSwarmHole::MovementType::eHover, "Hover"},
            {relive::Path_BeeSwarmHole::MovementType::eAttack, "Attack"},
            {relive::Path_BeeSwarmHole::MovementType::eFollowPath, "Follow Path"},
        });
    }

    CTOR_RELIVE(relive::Path_BeeSwarmHole, ReliveTypes::eBeeSwarmHole)
    {
        ADD("Start Interval", mTlv.mStartInterval);
        ADD("Movement Type", mTlv.mMovementType);
        ADD("Bees Amount", mTlv.mBeesAmount);
        ADD("Chase Time", mTlv.mChaseTime);
        ADD("Speed", mTlv.mSpeed);
    }
};

struct Editor_RollingBall final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_RollingBall, ReliveTypes::eRollingBall)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Roll Direction", mTlv.mRollDirection);
        ADD("Release Switch ID", mTlv.mReleaseSwitchId);
        ADD("Max Speed", mTlv.mMaxSpeed);
        ADD("Acceleration", mTlv.mAcceleration);
    }
};

struct Editor_RollingBallStopper final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_RollingBallStopper, ReliveTypes::eRollingBallStopper)
    {
        ADD("Stopper Switch ID", mTlv.mStopperSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Ball Switch ID", mTlv.mBallSwitchId);
        ADD("Stop Direction", mTlv.mStopDirection);
    }
};

struct Editor_BeeNest final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_BeeNest, ReliveTypes::eBeeNest)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Swarm Size", mTlv.mSwarmSize);
        ADD("Chase Time", mTlv.mChaseTime);
        ADD("Speed", mTlv.mSpeed);
        ADD("Amount Of Bees", mTlv.mBeesAmount);
    }
};

struct Editor_ZBall final : public MapObjectBase
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ZBall::StartPos>("Enum_ZBallStartPos",
        {
            {relive::Path_ZBall::StartPos::eCenter, "Center"},
            {relive::Path_ZBall::StartPos::eOut, "Out"},
            {relive::Path_ZBall::StartPos::eIn, "In"},
        });

        types.AddEnum<relive::Path_ZBall::Speed>("Enum_ZBallSpeed",
        {
            {relive::Path_ZBall::Speed::eNormal, "Normal"},
            {relive::Path_ZBall::Speed::eFast, "Fast"},
            {relive::Path_ZBall::Speed::eSlow, "Slow"},
        });
    }

    CTOR_RELIVE(relive::Path_ZBall, ReliveTypes::eZBall)
    {
        ADD("Start Position", mTlv.mStartPos);
        ADD("Scale", mTlv.mScale);
        ADD("Speed", mTlv.mSpeed);
    }
};

struct Editor_Honey final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Honey, ReliveTypes::eHoney)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_HoneyDripTarget final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_HoneyDripTarget, ReliveTypes::eHoneyDripTarget)
    {
        EMPTY_CTOR_RELIVE();
    }
};

struct Editor_Elum final : public MapObjectBase
{
    CTOR_RELIVE(relive::Path_Elum, ReliveTypes::eElum)
    {
        EMPTY_CTOR_RELIVE();
    }
};*/
} // namespace relive

#undef EMPTY_CTOR_RELIVE
#undef CTOR_RELIVE

