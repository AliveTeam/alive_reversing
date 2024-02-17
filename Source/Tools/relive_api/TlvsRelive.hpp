#pragma once

#include "TlvObjectBaseRelive.hpp"
#include "TlvObjectBaseMacros.hpp"
#include "../../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../../relive_lib/AnimResources.hpp"
#include "../../relive_lib/data_conversion/relive_tlvs_serialization.hpp"

#include <type_traits>

class IReflector
{
public:
    virtual ~IReflector() { }

    virtual void Visit(const char* fieldName, relive::reliveScale& field) = 0;

    virtual void Visit(const char* fieldName, u16& field) = 0;
    virtual void Visit(const char* fieldName, s16& field) = 0;

    virtual void Visit(const char* fieldName, u32& field) = 0;
    virtual void Visit(const char* fieldName, s32& field) = 0;

};

// TODO: Move back to the editor src
class MapObjectBase
{
public:
    // Given some json make a derived MapObjectBase type
    using TEditorDeserializeFunc = std::function<std::unique_ptr<MapObjectBase>(const nlohmann::json&)>;

    static std::map<ReliveTypes, TEditorDeserializeFunc>& GetEditorFactoryRegistry()
    {
        static std::map<ReliveTypes, TEditorDeserializeFunc> registry;
        return registry;
    }

    virtual void DoNotInheritFromSMapObjectBaseDirectlyButFromMapObjectBaseInterfaceInstead() = 0;

    explicit MapObjectBase(relive::Path_TLV* pTlv)
     : mBaseTlv(pTlv)
    {

    }

    virtual ~MapObjectBase() { }

    relive::Path_TLV* mBaseTlv = nullptr;

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

    virtual void Visit(IReflector& r)
    {
        //r.Visit("Type", mBaseTlv->mTlvType);
        r.Visit("xpos", mBaseTlv->mTopLeftX);
        r.Visit("ypos", mBaseTlv->mTopLeftY);
        r.Visit("width", mBaseTlv->mBottomRightX);
        r.Visit("height", mBaseTlv->mBottomRightY);
    }

    virtual std::string GetIconPath() const
    {
        return "";
    }

    virtual std::unique_ptr<MapObjectBase> Clone() const = 0;
};

template<typename DerivedType, ReliveTypes ReliveEnumType>
struct MapObjectBaseInterface : public MapObjectBase
{
    using MapObjectBase::MapObjectBase;

    virtual ~MapObjectBaseInterface() { if(!registered_) std::cout << "bad" << std::endl; } // registerd_ needs to be accessed somewhere, otherwise it is opted away

    static inline bool register_type()
    {
        auto& registry = GetEditorFactoryRegistry();
        registry[ReliveEnumType] = DerivedType::EditorDeserializeFunc;
        return true;
    }

    static std::unique_ptr<MapObjectBase> EditorDeserializeFunc(const nlohmann::json& j)
    {
        auto tmpMapObject = std::make_unique<DerivedType>();
        from_json(j, tmpMapObject->mTlv);

        // Re-purpose to width/height for the editor
        tmpMapObject->mTlv.mBottomRightX = tmpMapObject->mTlv.Width();
        tmpMapObject->mTlv.mBottomRightY = tmpMapObject->mTlv.Height();

        return tmpMapObject;
    }

    std::unique_ptr<MapObjectBase> Clone() const final
    {
        return std::make_unique<DerivedType>(static_cast<const DerivedType&>(*this));
    }

    static const bool registered_;

protected:
    void DoNotInheritFromSMapObjectBaseDirectlyButFromMapObjectBaseInterfaceInstead() final {}
};
template<typename DerivedType, ReliveTypes ReliveEnumType>
const bool MapObjectBaseInterface<DerivedType, ReliveEnumType>::registered_ = MapObjectBaseInterface<DerivedType, ReliveEnumType>::register_type();

namespace relive
{


class Editor_TimedMine final : public MapObjectBaseInterface<Editor_TimedMine, Path_TimedMine::kReliveType>
{
public:
    Editor_TimedMine() : MapObjectBaseInterface<Editor_TimedMine, Path_TimedMine::kReliveType>(&mTlv) { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
    }

    std::string GetIconPath() const override
    {
        return "TimedMine";
    }

    Path_TimedMine mTlv;
};

class Editor_ElectricWall final : public MapObjectBaseInterface<Editor_ElectricWall, Path_ElectricWall::kReliveType>
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
    */
public:
    Editor_ElectricWall()
        : MapObjectBaseInterface<Editor_ElectricWall, Path_ElectricWall::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Start State", mTlv.mStartState);
    }

    std::string GetIconPath() const override
    {
        return "ElectricWall";
    }

    Path_ElectricWall mTlv;
};

struct Editor_Mudokon final : public MapObjectBaseInterface<Editor_Mudokon, Path_Mudokon::kReliveType>
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
    */

public:
    Editor_Mudokon()
        : MapObjectBaseInterface<Editor_Mudokon, Path_Mudokon::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Job", mTlv.mJob);
        //r.Visit("Facing", mTlv.mFacing);
        r.Visit("Voice Pitch", mTlv.mVoicePitch);
        r.Visit("Rescue Switch ID", mTlv.mRescueSwitchId);
        //r.Visit("Deaf", mTlv.mDeaf);
        r.Visit("Disabled Resources", mTlv.mDisabledResources);
        //r.Visit("Persist & Reset Offscreen", mTlv.mPersistAndResetOffscreen);
        //r.Visit("Emotion", mTlv.mEmotion);
        //r.Visit("Blind", mTlv.mBlind);
        r.Visit("Angry Switch ID", mTlv.mAngrySwitchId);
        //r.Visit("Work After Turning Wheel", mTlv.mWorkAfterTurningWheel);
        //r.Visit("Gets Depressed", mTlv.mGetsDepressed);
        r.Visit("Ring Pulse Interval", mTlv.mRingPulseInterval);
        //r.Visit("Give Ring Without Password", mTlv.mGiveRingWithoutPassword);
    }

    std::string GetIconPath() const override
    {
        std::string imagesPath = "Mudokon/";
        if (mTlv.mEmotion == Path_Mudokon::Mud_TLV_Emotion::eAngry)
        {
            imagesPath += "MudAngry";
        }
        else if (mTlv.mEmotion == Path_Mudokon::Mud_TLV_Emotion::eSad)
        {
            imagesPath += "MudSad";
        }
        else if (mTlv.mEmotion == Path_Mudokon::Mud_TLV_Emotion::eSick)
        {
            imagesPath += "MudSick";
        }
        else if (mTlv.mEmotion == Path_Mudokon::Mud_TLV_Emotion::eWired)
        {
            imagesPath += "MudWired";
        }
        else
        {
            imagesPath += "MudNormal";
        }

        if (mTlv.mBlind)
        {
            imagesPath += "B";
        }
        
        return imagesPath;
    }

    Path_Mudokon mTlv;
};

struct Editor_BirdPortal final : public MapObjectBaseInterface<Editor_BirdPortal, Path_BirdPortal::kReliveType>
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
    */

public:
    Editor_BirdPortal()
        : MapObjectBaseInterface<Editor_BirdPortal, Path_BirdPortal::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        //r.Visit("Enter Side", mTlv.mEnterSide);
        // r.Visit("BirdPortalExit Level", mTlv.mExitLevel);
         r.Visit("BirdPortalExit Path", mTlv.mExitPath);
        r.Visit("BirdPortalExit Camera", mTlv.mExitCamera);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Movie ID", mTlv.mMovieId);
        //r.Visit("Portal Type", mTlv.mPortalType);
        r.Visit("Mudokon Amount For Shrykull", mTlv.mMudCountForShrykull);
        r.Visit("(AE) Create Portal Switch ID", mTlv.mCreatePortalSwitchId);
        r.Visit("(AE) Delete Portal Switch ID", mTlv.mDeletePortalSwitchId);
    }

    std::string GetIconPath() const override
    {
        if (mTlv.mPortalType == Path_BirdPortal::PortalType::eAbe)
        {
            return "BirdPortalAbe";
        }
        else if (mTlv.mPortalType == Path_BirdPortal::PortalType::eShrykull)
        {
            return "BirdPortalShrykull";
        }
        else
        {
            return "BirdPortal";
        }
    }

    Path_BirdPortal mTlv;
};

struct Editor_LCDStatusBoard final : public MapObjectBaseInterface<Editor_LCDStatusBoard, Path_LCDStatusBoard::kReliveType>
{
public:
    Editor_LCDStatusBoard()
        : MapObjectBaseInterface<Editor_LCDStatusBoard, Path_LCDStatusBoard::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        r.Visit("(AE) Number of Mudokons", mTlv.mNumberOfMuds);
        r.Visit("(AE) Zulag Number", mTlv.mZulagNumber);
        //r.Visit("(AE) Hide Board", mTlv.mHideBoard);
    }

    std::string GetIconPath() const override
    {
        return "LCDStatusBoard";
    }

    Path_LCDStatusBoard mTlv;
};

struct Editor_Door final : public MapObjectBaseInterface<Editor_Door, Path_Door::kReliveType>
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
    */

public:
    Editor_Door()
        : MapObjectBaseInterface<Editor_Door, Path_Door::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        r.Visit("Next Path", mTlv.mNextPath);
        //r.Visit("Next Level", mTlv.mNextLevel);
        r.Visit("Next Camera", mTlv.mNextCamera);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Door ID", mTlv.mDoorId);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Target Door ID", mTlv.mTargetDoorId);
        //r.Visit("Door Type", mTlv.mDoorType);
        //r.Visit("(AO) Door Closed", mTlv.mDoorClosed);
        //r.Visit("(AE) Start State", mTlv.mStartState);
        r.Visit("Hub 1 ID", mTlv.mHub1);
        r.Visit("Hub 2 ID", mTlv.mHub2);
        r.Visit("Hub 3 ID", mTlv.mHub3);
        r.Visit("Hub 4 ID", mTlv.mHub4);
        r.Visit("Hub 5 ID", mTlv.mHub5);
        r.Visit("Hub 6 ID", mTlv.mHub6);
        r.Visit("Hub 7 ID", mTlv.mHub7);
        r.Visit("Hub 8 ID", mTlv.mHub8);
        //r.Visit("Wipe Effect", mTlv.mWipeEffect);
        r.Visit("Movie ID", mTlv.mMovieId);
        r.Visit("X Offset", mTlv.mDoorOffsetX);
        r.Visit("Y Offset", mTlv.mDoorOffsetY);
        //r.Visit("Abe Direction On Exit", mTlv.mExitDirection);
        //r.Visit("(AE) Close On Exit", mTlv.mCloseOnExit);
        //r.Visit("(AE) Clear Throwables", mTlv.mClearThrowables);
    }

    std::string GetIconPath() const override
    {
        return "Door";
    }

    Path_Door mTlv;
};

struct Editor_Lever final : public MapObjectBaseInterface<Editor_Lever, Path_Lever::kReliveType>
{
    /*
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
    */

public:
    Editor_Lever()
        : MapObjectBaseInterface<Editor_Lever, Path_Lever::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        //r.Visit("Action", mTlv.mAction);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("On Sound", mTlv.mOnSound);
        //r.Visit("Off Sound", mTlv.mOffSound);
        //r.Visit("Sound Direction", mTlv.mSoundDirection);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("(AE) Persist Offscreen", mTlv.mPersistOffscreen);
    }

    std::string GetIconPath() const override
    {
        return "Lever";
    }

    Path_Lever mTlv;
};

struct Editor_Hoist final : public MapObjectBaseInterface<Editor_Hoist, Path_Hoist::kReliveType>
{
    /*
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
    }*/

public:
    Editor_Hoist()
        : MapObjectBaseInterface<Editor_Hoist, Path_Hoist::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        //r.Visit("Hoist Type", mTlv.mHoistType);
        //r.Visit("Grab Direction", mTlv.mGrabDirection);
        r.Visit("(AE) Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        if (mTlv.mGrabDirection == Path_Hoist::GrabDirection::eFacingRight)
        {
            return "Hoist/Right";
        }
        else
        {
            return "Hoist/Right";
        }
    }

    Path_Hoist mTlv;
};

struct Editor_BoomMachine final : public MapObjectBaseInterface<Editor_BoomMachine, Path_BoomMachine::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BoomMachine::NozzleSide>("Enum_BoomMachineNozzleSide",
        {
            {relive::Path_BoomMachine::NozzleSide::eRight, "Right"},
            {relive::Path_BoomMachine::NozzleSide::eLeft, "Left"},
        });
    }
    */

public:
    Editor_BoomMachine()
        : MapObjectBaseInterface<Editor_BoomMachine, Path_BoomMachine::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Nozzle Side", mTlv.mNozzleSide);
        r.Visit("Number Of Grenades", mTlv.mGrenadeAmount);
    }

    std::string GetIconPath() const override
    {
        return "BoomMachine";
    }

    Path_BoomMachine mTlv;
};

struct Editor_Slig final : public MapObjectBaseInterface<Editor_Slig, Path_Slig::kReliveType>
{
public:
    Editor_Slig()
        : MapObjectBaseInterface<Editor_Slig, Path_Slig::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mData.mScale);
        //r.Visit("Start State", mTlv.mData.mStartState);
        //r.Visit("(AE) Death Mode", mTlv.mData.mDeathMode);
        //r.Visit("(AE) Can Beat", mTlv.mData.mCanBeat);
        //r.Visit("(AE) Can Z Shoot", mTlv.mData.mCanZShoot);
        r.Visit("Pause Time", mTlv.mData.mPauseTime);
        r.Visit("Pause Left Min", mTlv.mData.mPauseLeftMin);
        r.Visit("Pause Left Max", mTlv.mData.mPauseLeftMax);
        r.Visit("Pause Right Min", mTlv.mData.mPauseRightMin);
        r.Visit("Pause Right Max", mTlv.mData.mPauseRightMax);
        //r.Visit("Shoot Possessed Sligs", mTlv.mData.mShootPossessedSligs);
        r.Visit("Shoot On Sight Delay", mTlv.mData.mShootOnSightDelay);
        r.Visit("Bullet Shoot Count", mTlv.mData.mNumTimesToShoot);

        r.Visit("Code 1", mTlv.mData.mCode1);
        r.Visit("Code 2", mTlv.mData.mCode2);
        //r.Visit("Chase Abe When Spotted", mTlv.mData.mChaseAbeWhenSpotted);
        //r.Visit("Facing", mTlv.mData.mFacing);
        r.Visit("Panic Timeout", mTlv.mData.mPanicTimeout);
        r.Visit("Stop Chase Delay", mTlv.mData.mStopChaseDelay);
        r.Visit("Time To Wait Before Chase", mTlv.mData.mTimeToWaitBeforeChase);
        r.Visit("Slig Bound/Persist ID", mTlv.mData.mSligBoundId);
        r.Visit("Alerted Listen Time", mTlv.mData.mAlertedListenTime);
        r.Visit("Percent Say What", mTlv.mData.mPercentSayWhat);
        r.Visit("Percent Beat Mudokon", mTlv.mData.mPercentBeatMud);
        r.Visit("Z Shoot Delay", mTlv.mData.mZShootDelay);
        //r.Visit("Stay Awake", mTlv.mData.mStayAwake);
        r.Visit("Noise Wake Up Distance (Grids)", mTlv.mData.mNoiseWakeUpDistance);
        r.Visit("Slig Spawner Switch ID", mTlv.mData.mSligSpawnerSwitchId);
        //r.Visit("(AE) Unlimited Spawns", mTlv.mData.mUnlimitedSpawns); // HIDDEN
        r.Visit("(AO) Disabled Resources", mTlv.mData.mDisabledResourcesAO.Raw().all);
        r.Visit("(AE) Disabled Resources", mTlv.mData.mDisabledResourcesAE);
    }

    std::string GetIconPath() const override
    {
        return "Slig";
    }

    Path_Slig mTlv;
};

struct Editor_Fleech final : public MapObjectBaseInterface<Editor_Fleech, Path_Fleech::kReliveType>
{
public:
    Editor_Fleech()
        : MapObjectBaseInterface<Editor_Fleech, Path_Fleech::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Facing", mTlv.mFacing);
        //r.Visit("Asleep", mTlv.mAsleep);
        r.Visit("Attack Anger Increaser", mTlv.mAttackAngerIncreaser);
        r.Visit("Wake Up Switch ID", mTlv.mWakeUpSwitchId);
        //r.Visit("Hanging", mTlv.mHanging);
        r.Visit("Lost Target Timeout", mTlv.mLostTargetTimeout);
        //r.Visit("Goes To Sleep", mTlv.mGoesToSleep);
        r.Visit("Patrol Range (Grids)", mTlv.mPatrolRangeInGrids);
        r.Visit("Wake Up Switch Anger Value", mTlv.mWakeUpSwitchAngerValue);
        r.Visit("Can Wake Up Switch ID", mTlv.mCanWakeUpSwitchId);
        //r.Visit("Persistant", mTlv.mPersistant);
    }

    std::string GetIconPath() const override
    {
        return "Fleech";
    }

    Path_Fleech mTlv;
};

struct Editor_EnemyStopper final : public MapObjectBaseInterface<Editor_EnemyStopper, Path_EnemyStopper::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
        {
            {relive::Path_EnemyStopper::StopDirection::Left, "Left"},
            {relive::Path_EnemyStopper::StopDirection::Right, "Right"},
            {relive::Path_EnemyStopper::StopDirection::Both, "Both"},
        });
    }
    */

public:
    Editor_EnemyStopper()
        : MapObjectBaseInterface<Editor_EnemyStopper, Path_EnemyStopper::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Stop Direction", mTlv.mStopDirection);
        r.Visit("Switch ID", mTlv.mSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "EnemyStopper";
    }

    Path_EnemyStopper mTlv;
};

struct Editor_Teleporter final : public MapObjectBaseInterface<Editor_Teleporter, Path_Teleporter::kReliveType>
{
public:
    Editor_Teleporter()
        : MapObjectBaseInterface<Editor_Teleporter, Path_Teleporter::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Teleporter Switch ID", mTlv.mTeleporterId);
        r.Visit("Other Teleporter Switch ID", mTlv.mOtherTeleporterId);
        r.Visit("Destination Camera", mTlv.mDestCamera);
        r.Visit("Destination Path", mTlv.mDestPath);
        //r.Visit("Destination Level", mTlv.mDestLevel);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Screen Change Effect", mTlv.mWipeEffect);
        r.Visit("Movie ID", mTlv.mMovieId);
        r.Visit("Electric X", mTlv.mElectricX);
        r.Visit("Electric Y", mTlv.mElectricY);
    }

    std::string GetIconPath() const override
    {
        return "Teleporter";
    }

    Path_Teleporter mTlv;
};

struct Editor_UXB final : public MapObjectBaseInterface<Editor_UXB, Path_UXB::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_UXB::StartState>("Enum_Path_UXB_StartState",
        {
            {relive::Path_UXB::StartState::eOn, "On"},
            {relive::Path_UXB::StartState::eOff, "Off"},
        });
    }
    */
public:
    Editor_UXB()
        : MapObjectBaseInterface<Editor_UXB, Path_UXB::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Pattern Length (Max 4)", mTlv.mPatternLength);
        r.Visit("Pattern", mTlv.mPattern);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Start State", mTlv.mStartState);
    }

    std::string GetIconPath() const override
    {
        if (mTlv.mStartState == Path_UXB::StartState::eOff)
        {
            return "UXBdisarmed";
        }
        else
        {
            return "UXB";
        }
    }

    Path_UXB mTlv;
};

struct Editor_LCDScreen final : public MapObjectBaseInterface<Editor_LCDScreen, Path_LCDScreen::kReliveType>
{
public:
    Editor_LCDScreen()
        : MapObjectBaseInterface<Editor_LCDScreen, Path_LCDScreen::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Message 1 ID", mTlv.mMessageId1);
        r.Visit("Random Message Min ID", mTlv.mMessageRandMinId);
        r.Visit("Random Message Max ID", mTlv.mMessageRandMaxId);

        r.Visit("(AE) Message 2 ID", mTlv.mMessageId2);
        r.Visit("(AE) Toggle Message Switch ID", mTlv.mToggleMessageSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "LCDScreen";
    }

    Path_LCDScreen mTlv;
};

struct Editor_Edge final : public MapObjectBaseInterface<Editor_Edge, Path_Edge::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Edge::GrabDirection>("Enum_EdgeGrabDirection",
        {
            {relive::Path_Edge::GrabDirection::eFacingLeft, "Facing Left"},
            {relive::Path_Edge::GrabDirection::eFacingRight, "Facing Right"},
            {relive::Path_Edge::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
        });
    }
    */
public:
    Editor_Edge()
        : MapObjectBaseInterface<Editor_Edge, Path_Edge::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Grab Direction", mTlv.mGrabDirection);
        //r.Visit("Can Grab", mTlv.mCanGrab);
        r.Visit("(AE) Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        if (mTlv.mGrabDirection == Path_Edge::GrabDirection::eFacingRight)
        {
            return "Edge/Right";
        }
        else
        {
            return "Edge/Right";
        }
    }

    Path_Edge mTlv;
};

struct Editor_StatusLight final : public MapObjectBaseInterface<Editor_StatusLight, Path_StatusLight::kReliveType>
{
public:
    Editor_StatusLight()
        : MapObjectBaseInterface<Editor_StatusLight, Path_StatusLight::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Input Switch ID", mTlv.mInputSwitchId);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Linked Status Light Switch ID 1", mTlv.mLinkedStatusLightSwitchId1);
        r.Visit("Linked Status Light Switch ID 2", mTlv.mLinkedStatusLightSwitchId2);
        r.Visit("Linked Status Light Switch ID 3", mTlv.mLinkedStatusLightSwitchId3);
        r.Visit("Linked Status Light Switch ID 4", mTlv.mLinkedStatusLightSwitchId4);
        r.Visit("Linked Status Light Switch ID 5", mTlv.mLinkedStatusLightSwitchId5);
        //r.Visit("Ignore Grid Snapping", mTlv.mIgnoreGridSnapping);
    }

    std::string GetIconPath() const override
    {
        return "StatusLight";
    }

    Path_StatusLight mTlv;
};

struct Editor_ShadowZone final : public MapObjectBaseInterface<Editor_ShadowZone, Path_ShadowZone::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ShadowZone::Scale>("Enum_ShadowZoneScale",
        {
            {relive::Path_ShadowZone::Scale::eBoth, "Both"},
            {relive::Path_ShadowZone::Scale::eHalf, "Half"},
            {relive::Path_ShadowZone::Scale::eFull, "Full"},
        });
    }
    */

public:
    Editor_ShadowZone()
        : MapObjectBaseInterface<Editor_ShadowZone, Path_ShadowZone::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("R", mTlv.mRGB.r);
        r.Visit("G", mTlv.mRGB.g);
        r.Visit("B", mTlv.mRGB.b);
        //r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "ShadowZone";
    }

    Path_ShadowZone mTlv;
};

struct Editor_WorkWheel final : public MapObjectBaseInterface<Editor_WorkWheel, Path_WorkWheel::kReliveType>
{
public:
    Editor_WorkWheel()
        : MapObjectBaseInterface<Editor_WorkWheel, Path_WorkWheel::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Activation Time", mTlv.mActivationTime);
        r.Visit("Turn Off Time", mTlv.mTurnOffTime);
        //r.Visit("Turn Off When Stopped", mTlv.mTurnOffWhenStopped);
    }

    std::string GetIconPath() const override
    {
        return "WorkWheel";
    }

    Path_WorkWheel mTlv;
};

struct Editor_MusicTrigger final : public MapObjectBaseInterface<Editor_MusicTrigger, Path_MusicTrigger::kReliveType>
{
    /*
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
    */
public:
    Editor_MusicTrigger()
        : MapObjectBaseInterface<Editor_MusicTrigger, Path_MusicTrigger::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Music Type", mTlv.mMusicType);
        //r.Visit("Triggered By", mTlv.mTriggeredBy);
        r.Visit("(AO) Switch ID", mTlv.mSwitchId);
        r.Visit("Music Delay", mTlv.mMusicDelay);
    }

    std::string GetIconPath() const override
    {
        return "MusicTrigger";
    }

    Path_MusicTrigger mTlv;
};

struct Editor_AbeStart final : public MapObjectBaseInterface<Editor_AbeStart, Path_AbeStart::kReliveType>
{
public:
    Editor_AbeStart()
        : MapObjectBaseInterface<Editor_AbeStart, Path_AbeStart::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }


    std::string GetIconPath() const override
    {
        return "AbeStart";
    }
    Path_AbeStart mTlv;
};

struct Editor_SoftLanding final : public MapObjectBaseInterface<Editor_SoftLanding, Path_SoftLanding::kReliveType>
{
public:
    Editor_SoftLanding()
        : MapObjectBaseInterface<Editor_SoftLanding, Path_SoftLanding::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("(AE) Switch ID", mTlv.mSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "SoftLanding";
    }

    Path_SoftLanding mTlv;
};

struct Editor_WellExpress final : public MapObjectBaseInterface<Editor_WellExpress, Path_WellExpress::kReliveType>
{
public:
    Editor_WellExpress()
        : MapObjectBaseInterface<Editor_WellExpress, Path_WellExpress::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        // Path_WellBase
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("(AO) Other Well ID", mTlv.mOtherWellId);
        r.Visit("(AO) Animation ID", mTlv.mAnimId);  // leftover from AO well FG hack

        // Path_WellExpress
        r.Visit("Exit X", mTlv.mExitX);
        r.Visit("Exit Y", mTlv.mExitY);
        //r.Visit("Disabled Well Level", mTlv.mOffDestLevel);
        r.Visit("Disabled Well Path", mTlv.mOffDestPath);
        r.Visit("Disabled Well Camera", mTlv.mOffDestCamera);
        r.Visit("Disabled Well ID", mTlv.mOffOtherWellId);
        //r.Visit("Enabled Well Level", mTlv.mOnDestLevel);
        r.Visit("Enabled Well Path", mTlv.mOnDestPath);
        r.Visit("Enabled Well Camera", mTlv.mOnDestCamera);
        r.Visit("Enabled Well ID", mTlv.mOnOtherWellId);
        //r.Visit("Emit Leaves", mTlv.mEmitLeaves);
        r.Visit("Leaf X", mTlv.mLeafX);
        r.Visit("Leaf Y", mTlv.mLeafY);
        r.Visit("Movie ID", mTlv.mMovieId);
    }

    std::string GetIconPath() const override
    {
        return "WellExpress";
    }

    Path_WellExpress mTlv;
};

struct Editor_SlamDoor final : public MapObjectBaseInterface<Editor_SlamDoor, Path_SlamDoor::kReliveType>
{
public:
    Editor_SlamDoor()
        : MapObjectBaseInterface<Editor_SlamDoor, Path_SlamDoor::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Start Shut", mTlv.mStartClosed);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Flip On Y Axis", mTlv.mFlipY);
        //r.Visit("Delete", mTlv.mDelete);
    }

    std::string GetIconPath() const override
    {
        return "SlamDoor";
    }

    Path_SlamDoor mTlv;
};

struct Editor_HandStone final : public MapObjectBaseInterface<Editor_HandStone, Path_HandStone::kReliveType>
{
public:
    Editor_HandStone()
        : MapObjectBaseInterface<Editor_HandStone, Path_HandStone::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Camera ID 1", mTlv.mCameraId1);
        r.Visit("Camera ID 2", mTlv.mCameraId2);
        r.Visit("Camera ID 3", mTlv.mCameraId3);
        r.Visit("(AE) Trigger Switch ID", mTlv.mTriggerSwitchId);

        //r.Visit("(AO) Level 1", mTlv.mLevel1);
        r.Visit("(AO) Path 1", mTlv.mPath1);
        //r.Visit("(AO) Level 2", mTlv.mLevel2);
        r.Visit("(AO) Path 2", mTlv.mPath2);
        //r.Visit("(AO) Level 3", mTlv.mLevel3);
        r.Visit("(AO) Path 3", mTlv.mPath3);
    }

    std::string GetIconPath() const override
    {
        return "HandStone";
    }

    Path_HandStone mTlv;
};

struct Editor_LaughingGas final : public MapObjectBaseInterface<Editor_LaughingGas, Path_LaughingGas::kReliveType>
{
public:
    Editor_LaughingGas()
        : MapObjectBaseInterface<Editor_LaughingGas, Path_LaughingGas::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Is Laughing Gas", mTlv.mLaughingGas);
        r.Visit("Laughing Gas Switch ID", mTlv.mLaughingGasSwitchId);
        r.Visit("Red %", mTlv.mRedPercent);
        r.Visit("Green %", mTlv.mGreenPercent);
        r.Visit("Blue %", mTlv.mBluePercent);
    }

    std::string GetIconPath() const override
    {
        return "LaughingGas";
    }

    Path_LaughingGas mTlv;
};

struct Editor_InvisibleSwitch final : public MapObjectBaseInterface<Editor_InvisibleSwitch, Path_InvisibleSwitch::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_InvisibleSwitch::InvisibleSwitchScale>("Enum_InvisibleSwitchScale",
        {
            {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf, "Half"},
            {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull, "Full"},
            {relive::Path_InvisibleSwitch::InvisibleSwitchScale::eAny, "Any"},
        });
    }*/

public:
    Editor_InvisibleSwitch()
        : MapObjectBaseInterface<Editor_InvisibleSwitch, Path_InvisibleSwitch::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Action", mTlv.mAction);
        r.Visit("Activation Delay", mTlv.mActivationDelay);
        //r.Visit("Set Off Alarm", mTlv.mSetOffAlarm);
        //r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "InvisibleSwitch";
    }

    Path_InvisibleSwitch mTlv;
};

struct Editor_Water final : public MapObjectBaseInterface<Editor_Water, Path_Water::kReliveType>
{
public:
    Editor_Water()
        : MapObjectBaseInterface<Editor_Water, Path_Water::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Max Drops", mTlv.mMaxDrops);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Splash Time", mTlv.mSplashTime);
        r.Visit("Splash X Velocity", mTlv.mSplashVelX);
        r.Visit("Water Duration", mTlv.mWaterDuration);
    }

    std::string GetIconPath() const override
    {
        return "Water";
    }

    Path_Water mTlv;
};

struct Editor_GasEmitter final : public MapObjectBaseInterface<Editor_GasEmitter, Path_GasEmitter::kReliveType>
{
    /*
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
    }*/

public:
    Editor_GasEmitter()
        : MapObjectBaseInterface<Editor_GasEmitter, Path_GasEmitter::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("(AE) Switch ID", mTlv.mSwitchId);
        //r.Visit("(AE) Colour", mTlv.mColour);
    }

    std::string GetIconPath() const override
    {
        return "GasEmitter";
    }

    Path_GasEmitter mTlv;
};

struct Editor_BackgroundAnimation final : public MapObjectBaseInterface<Editor_BackgroundAnimation, Path_BackgroundAnimation::kReliveType>
{
    /*
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
    */

public:
    Editor_BackgroundAnimation()
        : MapObjectBaseInterface<Editor_BackgroundAnimation, Path_BackgroundAnimation::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Animation ID", mTlv.mAnimId);
        //r.Visit("Is Semi Trans", mTlv.mIsSemiTrans);
        //r.Visit("Semi Trans Mode", mTlv.mSemiTransMode);
        //r.Visit("(AO) Sound Effect", mTlv.mSoundEffect);
        //r.Visit("(AE) Layer", mTlv.mLayer);
    }

    std::string GetIconPath() const override
    {
        return "BackgroundAnimation";
    }

    Path_BackgroundAnimation mTlv;
};

struct Editor_LiftPoint final : public MapObjectBaseInterface<Editor_LiftPoint, Path_LiftPoint::kReliveType>
{
    /*
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
    }*/

public:
    Editor_LiftPoint()
        : MapObjectBaseInterface<Editor_LiftPoint, Path_LiftPoint::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Lift Point ID", mTlv.mLiftPointId);
        //r.Visit("Start Point", mTlv.mIsStartPoint);
        //r.Visit("Lift Point Stop Type", mTlv.mLiftPointStopType);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Ignore Lift Mover", mTlv.mIgnoreLiftMover);
    }

    std::string GetIconPath() const override
    {
        return "LiftPoint";
    }

    Path_LiftPoint mTlv;
};

struct Editor_PullRingRope final : public MapObjectBaseInterface<Editor_PullRingRope, Path_PullRingRope::kReliveType>
{
    /*
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
    }*/

public:
    Editor_PullRingRope()
        : MapObjectBaseInterface<Editor_PullRingRope, Path_PullRingRope::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Action", mTlv.mAction);
        r.Visit("Rope Length", mTlv.mRopeLength);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("On Sound", mTlv.mOnSound);
        //r.Visit("Off Sound", mTlv.mOffSound);
        //r.Visit("Sound Direction", mTlv.mSoundDirection);
    }

    std::string GetIconPath() const override
    {
        return "PullRingRope";
    }

    Path_PullRingRope mTlv;
};

struct Editor_MultiSwitchController final : public MapObjectBaseInterface<Editor_MultiSwitchController, Path_MultiSwitchController::kReliveType>
{
public:
    Editor_MultiSwitchController()
        : MapObjectBaseInterface<Editor_MultiSwitchController, Path_MultiSwitchController::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Output Switch ID", mTlv.mOutputSwitchId);
        //r.Visit("Action", mTlv.mAction);
        r.Visit("On/Off Delay", mTlv.mOnOffDelay);
        r.Visit("Input Switch ID 1", mTlv.mInputSwitchId1);
        r.Visit("Input Switch ID 2", mTlv.mInputSwitchId2);
        r.Visit("Input Switch ID 3", mTlv.mInputSwitchId3);
        r.Visit("Input Switch ID 4", mTlv.mInputSwitchId4);
        r.Visit("Input Switch ID 5", mTlv.mInputSwitchId5);
        r.Visit("Input Switch ID 6", mTlv.mInputSwitchId6);
    }

    std::string GetIconPath() const override
    {
        return "MultiSwitchController";
    }

    Path_MultiSwitchController mTlv;
};

struct Editor_SecurityOrb final : public MapObjectBaseInterface<Editor_SecurityOrb, Path_SecurityOrb::kReliveType>
{
public:
    Editor_SecurityOrb()
        : MapObjectBaseInterface<Editor_SecurityOrb, Path_SecurityOrb::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "SecurityOrb";
    }

    Path_SecurityOrb mTlv;
};

struct Editor_InvisibleZone final : public MapObjectBaseInterface<Editor_InvisibleZone, Path_InvisibleZone::kReliveType>
{
public:
    Editor_InvisibleZone()
        : MapObjectBaseInterface<Editor_InvisibleZone, Path_InvisibleZone::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "InvisibleZone";
    }

    Path_InvisibleZone mTlv;
};

struct Editor_ContinuePoint final : public MapObjectBaseInterface<Editor_ContinuePoint, Path_ContinuePoint::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ContinuePoint::Scale>("Enum_ContinuePointScale",
        {
            {relive::Path_ContinuePoint::Scale::eNone, "None"},
            {relive::Path_ContinuePoint::Scale::eHalf, "Half"},
            {relive::Path_ContinuePoint::Scale::eFull, "Full"},
        });
    }*/

public:
    Editor_ContinuePoint()
        : MapObjectBaseInterface<Editor_ContinuePoint, Path_ContinuePoint::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("(AO) Zone Number", mTlv.mZoneNumber);
        //r.Visit("(AO) Clear From ID", mTlv.mClearFromId);
        //r.Visit("(AO) Clear To ID", mTlv.mClearToId);
        //r.Visit("(AO) Elum Restarts", mTlv.mElumRestarts);
        //r.Visit("(AO) Abe Spawn Dir", mTlv.mAbeSpawnDir);
        //r.Visit("(AE) Scale", mTlv.mScale);
        r.Visit("(AE) Save File ID", mTlv.mSaveFileId);
    }

    std::string GetIconPath() const override
    {
        return "ContinuePoint";
    }

    Path_ContinuePoint mTlv;
};

struct Editor_WheelSyncer final : public MapObjectBaseInterface<Editor_WheelSyncer, Path_WheelSyncer::kReliveType>
{
    /*
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
    */

public:
    Editor_WheelSyncer()
        : MapObjectBaseInterface<Editor_WheelSyncer, Path_WheelSyncer::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Input Switch ID 1", mTlv.mInputSwitchId1);
        r.Visit("Input Switch ID 2", mTlv.mInputSwitchId2);
        r.Visit("Output Switch ID", mTlv.mOutputSwitchId);
        //r.Visit("Output Requirement", mTlv.mOutputRequirement);
        r.Visit("Input Switch ID 3", mTlv.mInputSwitchId3);
        r.Visit("Input Switch ID 4", mTlv.mInputSwitchId4);
        r.Visit("Input Switch ID 5", mTlv.mInputSwitchId5);
        r.Visit("Input Switch ID 6", mTlv.mInputSwitchId6);
    }

    std::string GetIconPath() const override
    {
        return "WheelSyncer";
    }

    Path_WheelSyncer mTlv;
};

struct Editor_LevelLoader final : public MapObjectBaseInterface<Editor_LevelLoader, Path_LevelLoader::kReliveType>
{
public:
    Editor_LevelLoader()
        : MapObjectBaseInterface<Editor_LevelLoader, Path_LevelLoader::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Destination Level", mTlv.mDestLevel);
        r.Visit("Destination Path", mTlv.mDestPath);
        r.Visit("Destination Camera", mTlv.mDestCamera);
        r.Visit("Movie ID", mTlv.mMovieId);
    }

    std::string GetIconPath() const override
    {
        return "LevelLoader";
    }

    Path_LevelLoader mTlv;
};

struct Editor_Pulley final : public MapObjectBaseInterface<Editor_Pulley, Path_Pulley::kReliveType>
{
public:
    Editor_Pulley()
        : MapObjectBaseInterface<Editor_Pulley, Path_Pulley::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "Pulley";
    }

    Path_Pulley mTlv;
};

struct Editor_FlyingSlig final : public MapObjectBaseInterface<Editor_FlyingSlig, Path_FlyingSlig::kReliveType>
{
public:
    Editor_FlyingSlig()
        : MapObjectBaseInterface<Editor_FlyingSlig, Path_FlyingSlig::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Spawn Delay State", mTlv.mSpawnDelayState);
        r.Visit("Spawn Move Delay", mTlv.mSpawnMoveDelay);
        r.Visit("Patrol Pause Min", mTlv.mPatrolPauseMin);
        r.Visit("Patrol Pause Max", mTlv.mPatrolPauseMax);
        //r.Visit("Start Direction", mTlv.mFacing);
        r.Visit("Panic Delay", mTlv.mPanicDelay);
        r.Visit("Give Up Chase Delay", mTlv.mGiveUpChaseDelay);
        r.Visit("Pre-chase Delay", mTlv.mPrechaseDelay);
        r.Visit("Slig Bound/Persist ID", mTlv.mSligBoundId);
        r.Visit("Alerted Listen Time", mTlv.mAlertedListenTime);
        r.Visit("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        r.Visit("Grenade Delay", mTlv.mGrenadeDelay);
        r.Visit("Max Velocity", mTlv.mMaxVelocity);
        r.Visit("Launch Grenade Switch ID", mTlv.mLaunchGrenadeSwitchId);
        //r.Visit("Persistant", mTlv.mPersistant);
    }

    std::string GetIconPath() const override
    {
        return "FlyingSlig";
    }

    Path_FlyingSlig mTlv;
};

struct Editor_FlyingSligSpawner final : public MapObjectBaseInterface<Editor_FlyingSligSpawner, Path_FlyingSligSpawner::kReliveType>
{
public:
    Editor_FlyingSligSpawner()
        : MapObjectBaseInterface<Editor_FlyingSligSpawner, Path_FlyingSligSpawner::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Spawn Delay State", mTlv.mSpawnDelayState);
        r.Visit("Spawn Move Delay", mTlv.mSpawnMoveDelay);
        r.Visit("Patrol Pause Min", mTlv.mPatrolPauseMin);
        r.Visit("Patrol Pause Max", mTlv.mPatrolPauseMax);
        //r.Visit("Start Direction", mTlv.mFacing);
        r.Visit("Panic Delay", mTlv.mPanicDelay);
        r.Visit("Give Up Chase Delay", mTlv.mGiveUpChaseDelay);
        r.Visit("Pre-chase Delay", mTlv.mPrechaseDelay);
        r.Visit("Slig Bound/Persist ID", mTlv.mSligBoundId);
        r.Visit("Listen Time", mTlv.mAlertedListenTime);
        r.Visit("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        r.Visit("Grenade Delay", mTlv.mGrenadeDelay);
        r.Visit("Max Velocity", mTlv.mMaxVelocity);
        r.Visit("Launch Grenade Switch ID", mTlv.mLaunchGrenadeSwitchId);
        //r.Visit("Persistant", mTlv.mPersistant);
    }

    std::string GetIconPath() const override
    {
        return "FlyingSligSpawner";
    }

    Path_FlyingSligSpawner mTlv;
};

struct Editor_DeathDrop final : public MapObjectBaseInterface<Editor_DeathDrop, Path_DeathDrop::kReliveType>
{
public:
    Editor_DeathDrop()
        : MapObjectBaseInterface<Editor_DeathDrop, Path_DeathDrop::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "DeathDrop";
    }

    Path_DeathDrop mTlv;
};

struct Editor_SligSpawner final : public MapObjectBaseInterface<Editor_SligSpawner, Path_SligSpawner::kReliveType>
{
public:
    Editor_SligSpawner()
        : MapObjectBaseInterface<Editor_SligSpawner, Path_SligSpawner::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mData.mScale);
        //r.Visit("Start State", mTlv.mData.mStartState);
        //r.Visit("(AE) Death Mode", mTlv.mData.mDeathMode);
        //r.Visit("(AE) Can Beat", mTlv.mData.mCanBeat);
        //r.Visit("(AE) Can Z Shoot", mTlv.mData.mCanZShoot);
        r.Visit("Pause Time", mTlv.mData.mPauseTime);
        r.Visit("Pause Left Min", mTlv.mData.mPauseLeftMin);
        r.Visit("Pause Left Max", mTlv.mData.mPauseLeftMax);
        r.Visit("Pause Right Min", mTlv.mData.mPauseRightMin);
        r.Visit("Pause Right Max", mTlv.mData.mPauseRightMax);
        //r.Visit("Shoot Possessed Sligs", mTlv.mData.mShootPossessedSligs);
        r.Visit("Shoot On Sight Delay", mTlv.mData.mShootOnSightDelay);
        r.Visit("Bullet Shoot Count", mTlv.mData.mNumTimesToShoot);

        r.Visit("Code 1", mTlv.mData.mCode1);
        r.Visit("Code 2", mTlv.mData.mCode2);
        //r.Visit("Chase Abe When Spotted", mTlv.mData.mChaseAbeWhenSpotted);
        //r.Visit("Facing", mTlv.mData.mFacing);
        r.Visit("Panic Timeout", mTlv.mData.mPanicTimeout);
        r.Visit("Stop Chase Delay", mTlv.mData.mStopChaseDelay);
        r.Visit("Time To Wait Before Chase", mTlv.mData.mTimeToWaitBeforeChase);
        r.Visit("Slig Bound/Persist ID", mTlv.mData.mSligBoundId);
        r.Visit("Alerted Listen Time", mTlv.mData.mAlertedListenTime);
        r.Visit("Percent Say What", mTlv.mData.mPercentSayWhat);
        r.Visit("Percent Beat Mudokon", mTlv.mData.mPercentBeatMud);
        r.Visit("Z Shoot Delay", mTlv.mData.mZShootDelay);
        //r.Visit("Stay Awake", mTlv.mData.mStayAwake);
        r.Visit("Noise Wake Up Distance (Grids)", mTlv.mData.mNoiseWakeUpDistance);
        r.Visit("Slig Spawner Switch ID", mTlv.mData.mSligSpawnerSwitchId);
        //r.Visit("(AE) Unlimited Spawns", mTlv.mData.mUnlimitedSpawns); // HIDDEN
        r.Visit("(AO) Disabled Resources", mTlv.mData.mDisabledResourcesAO.Raw().all);
        r.Visit("(AE) Disabled Resources", mTlv.mData.mDisabledResourcesAE);
    }

    std::string GetIconPath() const override
    {
        return "SligSpawner";
    }

    Path_SligSpawner mTlv;
};

struct Editor_SligBoundLeft final : public MapObjectBaseInterface<Editor_SligBoundLeft, Path_SligBoundLeft::kReliveType>
{
public:
    Editor_SligBoundLeft()
        : MapObjectBaseInterface<Editor_SligBoundLeft, Path_SligBoundLeft::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "SligLeftBound";
    }

    Path_SligBoundLeft mTlv;
};

struct Editor_SligBoundRight final : public MapObjectBaseInterface<Editor_SligBoundRight, Path_SligBoundRight::kReliveType>
{
public:
    Editor_SligBoundRight()
        : MapObjectBaseInterface<Editor_SligBoundRight, Path_SligBoundRight::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "SligRightBound";
    }

    Path_SligBoundRight mTlv;
};

struct Editor_SligPersist final : public MapObjectBaseInterface<Editor_SligPersist, Path_SligPersist::kReliveType>
{
public:
    Editor_SligPersist()
        : MapObjectBaseInterface<Editor_SligPersist, Path_SligPersist::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "SligPersist";
    }

    Path_SligPersist mTlv;
};

struct Editor_ZSligCover final : public MapObjectBaseInterface<Editor_ZSligCover, Path_ZSligCover::kReliveType>
{
public:
    Editor_ZSligCover()
        : MapObjectBaseInterface<Editor_ZSligCover, Path_ZSligCover::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "ZSligCover";
    }

    Path_ZSligCover mTlv;
};

struct Editor_WellLocal final : public MapObjectBaseInterface<Editor_WellLocal, Path_WellLocal::kReliveType>
{
public:
    Editor_WellLocal()
        : MapObjectBaseInterface<Editor_WellLocal, Path_WellLocal::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        // Path_WellBase
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("(AO) Other Well ID", mTlv.mOtherWellId);     // hidden because this is only used by the well express
        r.Visit("(AO) Animation ID (Unused?)", mTlv.mAnimId); // leftover from AO well FG hack
        r.Visit("(AO) Exit X", mTlv.mExitX);
        r.Visit("(AO) Exit Y", mTlv.mExitY);

        // Path_WellLocal
        r.Visit("(AO) Disabled XPos", mTlv.mOffDestX);
        r.Visit("(AO) Disabled YPos", mTlv.mOffDestY);
        r.Visit("Enabled XPos", mTlv.mOnDestX);
        r.Visit("Enabled YPos", mTlv.mOnDestY);
        //r.Visit("Emit Leaves", mTlv.mEmitLeaves);
        r.Visit("Leaf XPos", mTlv.mLeafX);
        r.Visit("Leaf YPos", mTlv.mLeafY);
    }

    std::string GetIconPath() const override
    {
        return "WellLocal";
    }

    Path_WellLocal mTlv;
};

struct Editor_BrewMachine final : public MapObjectBaseInterface<Editor_BrewMachine, Path_BrewMachine::kReliveType>
{
public:
    Editor_BrewMachine()
        : MapObjectBaseInterface<Editor_BrewMachine, Path_BrewMachine::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Brew Count", mTlv.mBrewCount);
    }

    std::string GetIconPath() const override
    {
        return "BrewMachine";
    }

    Path_BrewMachine mTlv;
};

struct Editor_Drill final : public MapObjectBaseInterface<Editor_Drill, Path_Drill::kReliveType>
{
    /*
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
    */

public:
    Editor_Drill()
        : MapObjectBaseInterface<Editor_Drill, Path_Drill::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Min Off Time", mTlv.mOnMinPauseTime);
        r.Visit("Max Off Time", mTlv.mOnMaxPauseTime);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Behavior", mTlv.mDrillBehavior);
        r.Visit("Speed", mTlv.mOnSpeed);
        //r.Visit("Start State On", mTlv.mStartStateOn);
        r.Visit("Off Speed", mTlv.mOffSpeed);
        r.Visit("Min Off Time Speed Change", mTlv.mOffMinPauseTime);
        r.Visit("Max Off Time Speed Change", mTlv.mOffMaxPauseTime);
        //r.Visit("Start Position Bottom", mTlv.mStartPositionBottom);
        //r.Visit("Start Direction", mTlv.mDrillDirection);
    }

    std::string GetIconPath() const override
    {
        std::string imagesPath = "Drill/Drill_";
        if (mTlv.Width() > 25)
        {
            return imagesPath + std::to_string(std::min(mTlv.Width() / 25, 9)) + "_1";
        }
        else
        {
            return imagesPath + "1_" + std::to_string(std::min(mTlv.Height() / 20, 9));
        }
    }

    Path_Drill mTlv;
};

struct Editor_Mine final : public MapObjectBaseInterface<Editor_Mine, Path_Mine::kReliveType>
{
public:
    Editor_Mine()
        : MapObjectBaseInterface<Editor_Mine, Path_Mine::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Persist Offscreen", mTlv.mPersistOffscreen);
    }

    std::string GetIconPath() const override
    {
        return "Mine";
    }

    Path_Mine mTlv;
};

struct Editor_Slog final : public MapObjectBaseInterface<Editor_Slog, Path_Slog::kReliveType>
{
public:
    Editor_Slog()
        : MapObjectBaseInterface<Editor_Slog, Path_Slog::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Start Direction", mTlv.mFacing);
        //r.Visit("Asleep", mTlv.mAsleep);
        r.Visit("Wake Up Anger", mTlv.mWakeUpAnger);
        r.Visit("Bark Anger", mTlv.mBarkAnger);
        r.Visit("Chase Anger", mTlv.mChaseAnger);
        r.Visit("Chase Delay", mTlv.mChaseDelay);
        r.Visit("Anger Switch ID", mTlv.mAngerSwitchId);
        r.Visit("(AE) Bone Eating Time", mTlv.mBoneEatingTime);
    }

    std::string GetIconPath() const override
    {
        return "Slog";
    }

    Path_Slog mTlv;
};

struct Editor_ResetPath final : public MapObjectBaseInterface<Editor_ResetPath, Path_ResetPath::kReliveType>
{
public:
    Editor_ResetPath()
        : MapObjectBaseInterface<Editor_ResetPath, Path_ResetPath::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Clear Ids", mTlv.mClearIds);
        r.Visit("From", mTlv.mFrom);
        r.Visit("To", mTlv.mTo);
        r.Visit("Exclude", mTlv.mExclude);
        //r.Visit("Clear Objects", mTlv.mClearObjects);
        r.Visit("Path", mTlv.mPath);
        //r.Visit("(AE) Enabled", mTlv.mEnabled);
    }

    std::string GetIconPath() const override
    {
        return "ResetPath";
    }

    Path_ResetPath mTlv;
};

struct Editor_TrapDoor final : public MapObjectBaseInterface<Editor_TrapDoor, Path_TrapDoor::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_TrapDoor::StartState>("Enum_TrapDoorStartState",
        {{relive::Path_TrapDoor::StartState::eOpen, "Open"},
        {relive::Path_TrapDoor::StartState::eClosed, "Closed"}});
    }
    */

public:
    Editor_TrapDoor()
        : MapObjectBaseInterface<Editor_TrapDoor, Path_TrapDoor::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Start State", mTlv.mStartState);
        //r.Visit("Self Closing", mTlv.mSelfClosing);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Direction", mTlv.mDirection);
        r.Visit("X Offset", mTlv.mXOff);
        r.Visit("(AE) Stay Open Time", mTlv.mStayOpenTime);
    }

    std::string GetIconPath() const override
    {
        return "TrapDoor";
    }

    Path_TrapDoor mTlv;
};

struct Editor_PathTransition final : public MapObjectBaseInterface<Editor_PathTransition, Path_PathTransition::kReliveType>
{
public:
    Editor_PathTransition()
        : MapObjectBaseInterface<Editor_PathTransition, Path_PathTransition::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Next Level", mTlv.mNextLevel);
        r.Visit("Next Path", mTlv.mNextPath);
        r.Visit("Next Camera", mTlv.mNextCamera);
        r.Visit("Movie ID", mTlv.mMovieId);
        r.Visit("Wipe Effect", mTlv.mWipeEffect);
        r.Visit("Next Path Scale", mTlv.mNextPathScale);
    }

    std::string GetIconPath() const override
    {
        return "PathTransition";
    }

    Path_PathTransition mTlv;
};

struct Editor_LiftMover final : public MapObjectBaseInterface<Editor_LiftMover, Path_LiftMover::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_LiftMover::YDirection>("Enum_LiftMoverYDirection",
        {
            {relive::Path_LiftMover::YDirection::eDown, "Down"},
            {relive::Path_LiftMover::YDirection::eUp, "Up"},
        });
    }*/

public:
    Editor_LiftMover()
        : MapObjectBaseInterface<Editor_LiftMover, Path_LiftMover::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Lift Mover Switch ID", mTlv.mLiftMoverSwitchId);
        r.Visit("Target Lift Point ID", mTlv.mTargetLiftPointId);
        //r.Visit("Move Direction", mTlv.mMoveDirection);
    }

    std::string GetIconPath() const override
    {
        return "LiftMover";
    }

    Path_LiftMover mTlv;
};

struct Editor_RockSack final : public MapObjectBaseInterface<Editor_RockSack, Path_RockSack::kReliveType>
{
public:
    Editor_RockSack()
        : MapObjectBaseInterface<Editor_RockSack, Path_RockSack::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Rock Fall Direction", mTlv.mRockFallDirection);
        r.Visit("X Velocity", mTlv.mVelX);
        r.Visit("Y Velocity", mTlv.mVelY);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Rock Amount", mTlv.mRockAmount);
    }

    std::string GetIconPath() const override
    {
        return "RockSack";
    }

    Path_RockSack mTlv;
};

struct Editor_TimerTrigger final : public MapObjectBaseInterface<Editor_TimerTrigger, Path_TimerTrigger::kReliveType>
{
public:
    Editor_TimerTrigger()
        : MapObjectBaseInterface<Editor_TimerTrigger, Path_TimerTrigger::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Input Switch ID", mTlv.mInputSwitchId);
        r.Visit("Activation Delay", mTlv.mActivationDelay);
        r.Visit("Output Switch ID 1", mTlv.mOutputSwitchId1);
        r.Visit("Output Switch ID 2", mTlv.mOutputSwitchId2);
        r.Visit("Output Switch ID 3", mTlv.mOutputSwitchId3);
        r.Visit("Output Switch ID 4", mTlv.mOutputSwitchId4);
    }

    std::string GetIconPath() const override
    {
        return "TimerTrigger";
    }

    Path_TimerTrigger mTlv;
};

struct Editor_MotionDetector final : public MapObjectBaseInterface<Editor_MotionDetector, Path_MotionDetector::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_MotionDetector::InitialMoveDirection>("Enum_InitialMoveDirection",
        {
            {relive::Path_MotionDetector::InitialMoveDirection::eRight, "Right"},
            {relive::Path_MotionDetector::InitialMoveDirection::eLeft, "Left"},
        });
    }*/

public:
    Editor_MotionDetector()
        : MapObjectBaseInterface<Editor_MotionDetector, Path_MotionDetector::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Device X", mTlv.mDeviceX);
        r.Visit("Device Y", mTlv.mDeviceY);
        r.Visit("Speed", mTlv.mSpeedx256); // Note the potential scaling factor of 256 based on field name
        //r.Visit("Initial Move Direction", mTlv.mInitialMoveDirection);
        //r.Visit("Draw Flare", mTlv.mDrawFlare);
        r.Visit("Disable Switch ID", mTlv.mDisableSwitchId);
        r.Visit("Alarm Switch ID", mTlv.mAlarmSwitchId);
        r.Visit("Alarm Duration", mTlv.mAlarmDuration);
    }

    std::string GetIconPath() const override
    {
        return "MotionDetector/" + std::to_string(std::max(std::min((mTlv.Width() / 26), 10), 0));
    }

    Path_MotionDetector mTlv;
};

struct Editor_MineCar final : public MapObjectBaseInterface<Editor_MineCar, Path_MineCar::kReliveType>
{
public:
    Editor_MineCar()
        : MapObjectBaseInterface<Editor_MineCar, Path_MineCar::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "MineCar";
    }

    Path_MineCar mTlv;
};

struct Editor_ExplosionSet final : public MapObjectBaseInterface<Editor_ExplosionSet, Path_ExplosionSet::kReliveType>
{
public:
    Editor_ExplosionSet()
        : MapObjectBaseInterface<Editor_ExplosionSet, Path_ExplosionSet::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Start Enabled", mTlv.mStartEnabled);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Spawn Assets", mTlv.mSpawnAssets);
        r.Visit("Start Delay", mTlv.mStartDelay);
        //r.Visit("Start Direction", mTlv.mStartDirection);
        r.Visit("Asset Interval", mTlv.mAssetInterval);
        r.Visit("Grid Spacing", mTlv.mGridSpacing);
        r.Visit("Increasing Grid Spacing", mTlv.mIncreasingGridSpacing);
        r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "ExplosionSet";
    }

    Path_ExplosionSet mTlv;
};

struct Editor_ColourfulMeter final : public MapObjectBaseInterface<Editor_ColourfulMeter, Path_ColourfulMeter::kReliveType>
{
public:
    Editor_ColourfulMeter()
        : MapObjectBaseInterface<Editor_ColourfulMeter, Path_ColourfulMeter::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Number Of Meter Bars", mTlv.mNumberOfMeterBars);
        r.Visit("Mines Alarm Countdown (Seconds)", mTlv.mMinesAlarmCountdown);
        //r.Visit("Start Filled", mTlv.mStartFilled);
    }

    std::string GetIconPath() const override
    {
        return "ColourfulMeter";
    }

    Path_ColourfulMeter mTlv;
};

struct Editor_Alarm final : public MapObjectBaseInterface<Editor_Alarm, Path_Alarm::kReliveType>
{
public:
    Editor_Alarm()
        : MapObjectBaseInterface<Editor_Alarm, Path_Alarm::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Duration", mTlv.mAlarmDuration);
    }

    std::string GetIconPath() const override
    {
        return "Alarm";
    }

    Path_Alarm mTlv;
};

struct Editor_DemoSpawnPoint final : public MapObjectBaseInterface<Editor_DemoSpawnPoint, Path_DemoSpawnPoint::kReliveType>
{
public:
    Editor_DemoSpawnPoint()
        : MapObjectBaseInterface<Editor_DemoSpawnPoint, Path_DemoSpawnPoint::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "DemoSpawnPoint";
    }

    Path_DemoSpawnPoint mTlv;
};

struct Editor_SlapLock final : public MapObjectBaseInterface<Editor_SlapLock, Path_SlapLock::kReliveType>
{
public:
    Editor_SlapLock()
        : MapObjectBaseInterface<Editor_SlapLock, Path_SlapLock::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Target Tomb ID 1", mTlv.mTargetTombSwitchId1);
        r.Visit("Target Tomb ID 2", mTlv.mTargetTombSwitchId2);
        //r.Visit("Has Ghost", mTlv.mHasGhost);
        //r.Visit("Give Invisibility Power-up", mTlv.mGiveInvisibilityPowerup);
        r.Visit("Invisibility Duration", mTlv.mInvisibilityDuration);
        r.Visit("Toggle Switch ID", mTlv.mSlapOutputSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "SlapLock";
    }

    Path_SlapLock mTlv;
};

struct Editor_Slurg final : public MapObjectBaseInterface<Editor_Slurg, Path_Slurg::kReliveType>
{
public:
    Editor_Slurg()
        : MapObjectBaseInterface<Editor_Slurg, Path_Slurg::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Time Until Turning Around", mTlv.mMovingTimer);
        //r.Visit("Start Direction", mTlv.mFacing);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID (increment by 1 on death)", mTlv.mSlurgSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "Slurg";
    }

    Path_Slurg mTlv;
};

struct Editor_DoorBlocker final : public MapObjectBaseInterface<Editor_DoorBlocker, Path_DoorBlocker::kReliveType>
{
public:
    Editor_DoorBlocker()
        : MapObjectBaseInterface<Editor_DoorBlocker, Path_DoorBlocker::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "DoorBlocker";
    }

    Path_DoorBlocker mTlv;
};

struct Editor_Dove final : public MapObjectBaseInterface<Editor_Dove, Path_Dove::kReliveType>
{
public:
    Editor_Dove()
        : MapObjectBaseInterface<Editor_Dove, Path_Dove::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Dove Count", mTlv.mDoveCount);
        //r.Visit("Pixel Perfect", mTlv.mPixelPerfect);
        r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "Dove";
    }

    Path_Dove mTlv;
};

struct Editor_BirdPortalExit final : public MapObjectBaseInterface<Editor_BirdPortalExit, Path_BirdPortalExit::kReliveType>
{
public:
    Editor_BirdPortalExit()
        : MapObjectBaseInterface<Editor_BirdPortalExit, Path_BirdPortalExit::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Exit Direction", mTlv.mExitSide);
        r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "BirdPortalExit";
    }

    Path_BirdPortalExit mTlv;
};

struct Editor_DoorFlame final : public MapObjectBaseInterface<Editor_DoorFlame, Path_DoorFlame::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_DoorFlame::Colour>("Enum_DoorFlameColour",
        {
            {relive::Path_DoorFlame::Colour::eDefault, "Default"},
            {relive::Path_DoorFlame::Colour::eRed, "Red"},
            {relive::Path_DoorFlame::Colour::eGreen, "Green"},
            {relive::Path_DoorFlame::Colour::eBlue, "Blue"},
        });
    }*/

public:
    Editor_DoorFlame()
        : MapObjectBaseInterface<Editor_DoorFlame, Path_DoorFlame::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Colour", mTlv.mColour);
    }

    std::string GetIconPath() const override
    {
        return "DoorFlame";
    }

    Path_DoorFlame mTlv;
};

struct Editor_TrainDoor final : public MapObjectBaseInterface<Editor_TrainDoor, Path_TrainDoor::kReliveType>
{
public:
    Editor_TrainDoor()
        : MapObjectBaseInterface<Editor_TrainDoor, Path_TrainDoor::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Direction", mTlv.mDirection);
    }

    std::string GetIconPath() const override
    {
        return "TrainDoor";
    }

    Path_TrainDoor mTlv;
};

struct Editor_Greeter final : public MapObjectBaseInterface<Editor_Greeter, Path_Greeter::kReliveType>
{
public:
    Editor_Greeter()
        : MapObjectBaseInterface<Editor_Greeter, Path_Greeter::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Motion Detector Speed", mTlv.mMotionDetectorSpeed);
        //r.Visit("Facing", mTlv.mFacing);
    }

    std::string GetIconPath() const override
    {
        return "Greeter";
    }

    Path_Greeter mTlv;
};

struct Editor_ScrabBoundLeft final : public MapObjectBaseInterface<Editor_ScrabBoundLeft, Path_ScrabBoundLeft::kReliveType>
{
public:
    Editor_ScrabBoundLeft()
        : MapObjectBaseInterface<Editor_ScrabBoundLeft, Path_ScrabBoundLeft::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "ScrabLeftBound";
    }

    Path_ScrabBoundLeft mTlv;
};

struct Editor_ScrabBoundRight final : public MapObjectBaseInterface<Editor_ScrabBoundRight, Path_ScrabBoundRight::kReliveType>
{
public:
    Editor_ScrabBoundRight()
        : MapObjectBaseInterface<Editor_ScrabBoundRight, Path_ScrabBoundRight::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "ScrabRightBound";
    }

    Path_ScrabBoundRight mTlv;
};

struct Editor_CreditsController final : public MapObjectBaseInterface<Editor_CreditsController, Path_CreditsController::kReliveType>
{
public:
    Editor_CreditsController()
        : MapObjectBaseInterface<Editor_CreditsController, Path_CreditsController::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    Path_CreditsController mTlv;
};

struct Editor_MovieStone final : public MapObjectBaseInterface<Editor_MovieStone, Path_MovieStone::kReliveType>
{
public:
    Editor_MovieStone()
        : MapObjectBaseInterface<Editor_MovieStone, Path_MovieStone::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Movie ID", mTlv.mMovieId);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("(AE) Trigger Switch ID", mTlv.mTriggerSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "MovieStone";
    }

    Path_MovieStone mTlv;
};

struct Editor_MovingBomb final : public MapObjectBaseInterface<Editor_MovingBomb, Path_MovingBomb::kReliveType>
{
public:
    Editor_MovingBomb()
        : MapObjectBaseInterface<Editor_MovingBomb, Path_MovingBomb::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Speed", mTlv.mSpeed);
        r.Visit("Start Moving Switch ID", mTlv.mStartMovingSwitchId);
        //r.Visit("Triggered By Alarm", mTlv.mTriggeredByAlarm);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Start Speed", mTlv.mStartSpeed);
        //r.Visit("Persist Offscreen", mTlv.mPersistOffscreen);
    }

    std::string GetIconPath() const override
    {
        return "MovingBomb";
    }

    Path_MovingBomb mTlv;
};

struct Editor_MovingBombStopper final : public MapObjectBaseInterface<Editor_MovingBombStopper, Path_MovingBombStopper::kReliveType>
{
public:
    Editor_MovingBombStopper()
        : MapObjectBaseInterface<Editor_MovingBombStopper, Path_MovingBombStopper::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Min Stop Time", mTlv.mMinStopTime);
        r.Visit("Max Stop Time", mTlv.mMaxStopTime);
    }

    std::string GetIconPath() const override
    {
        return "MovingBombStopper";
    }

    Path_MovingBombStopper mTlv;
};

struct Editor_SecurityDoor final : public MapObjectBaseInterface<Editor_SecurityDoor, Path_SecurityDoor::kReliveType>
{
public:
    Editor_SecurityDoor()
        : MapObjectBaseInterface<Editor_SecurityDoor, Path_SecurityDoor::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Code 1", mTlv.mCode1);
        r.Visit("Code 2", mTlv.mCode2);
        r.Visit("X Position", mTlv.mXPos);
        r.Visit("Y Position", mTlv.mYPos);
    }

    std::string GetIconPath() const override
    {
        return "SecurityDoor";
    }

    Path_SecurityDoor mTlv;
};

struct Editor_CrawlingSlig final : public MapObjectBaseInterface<Editor_CrawlingSlig, Path_CrawlingSlig::kReliveType>
{
    /*
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
    }*/

public:
    Editor_CrawlingSlig()
        : MapObjectBaseInterface<Editor_CrawlingSlig, Path_CrawlingSlig::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Start State", mTlv.mStartState);
        //r.Visit("Crawl Direction", mTlv.mCrawlDirection);
        r.Visit("Panic Switch ID", mTlv.mPanicSwitchId);
        //r.Visit("Respawn On Death", mTlv.mRespawnOnDeath);
    }

    std::string GetIconPath() const override
    {
        return "CrawlingSlig";
    }

    Path_CrawlingSlig mTlv;
};

struct Editor_SligGetPants final : public MapObjectBaseInterface<Editor_SligGetPants, Path_SligGetPants::kReliveType>
{
public:
    Editor_SligGetPants()
        : MapObjectBaseInterface<Editor_SligGetPants, Path_SligGetPants::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mData.mScale);
        //r.Visit("Start State", mTlv.mData.mStartState);
        //r.Visit("(AE) Death Mode", mTlv.mData.mDeathMode);
        //r.Visit("(AE) Can Beat", mTlv.mData.mCanBeat);
        //r.Visit("(AE) Can Z Shoot", mTlv.mData.mCanZShoot);
        r.Visit("Pause Time", mTlv.mData.mPauseTime);
        r.Visit("Pause Left Min", mTlv.mData.mPauseLeftMin);
        r.Visit("Pause Left Max", mTlv.mData.mPauseLeftMax);
        r.Visit("Pause Right Min", mTlv.mData.mPauseRightMin);
        r.Visit("Pause Right Max", mTlv.mData.mPauseRightMax);
        //r.Visit("Shoot Possessed Sligs", mTlv.mData.mShootPossessedSligs);
        r.Visit("Shoot On Sight Delay", mTlv.mData.mShootOnSightDelay);
        r.Visit("Bullet Shoot Count", mTlv.mData.mNumTimesToShoot);

        r.Visit("Code 1", mTlv.mData.mCode1);
        r.Visit("Code 2", mTlv.mData.mCode2);
        //r.Visit("Chase Abe When Spotted", mTlv.mData.mChaseAbeWhenSpotted);
        //r.Visit("Facing", mTlv.mData.mFacing);
        r.Visit("Panic Timeout", mTlv.mData.mPanicTimeout);
        r.Visit("Stop Chase Delay", mTlv.mData.mStopChaseDelay);
        r.Visit("Time To Wait Before Chase", mTlv.mData.mTimeToWaitBeforeChase);
        r.Visit("Slig Bound/Persist ID", mTlv.mData.mSligBoundId);
        r.Visit("Alerted Listen Time", mTlv.mData.mAlertedListenTime);
        r.Visit("Percent Say What", mTlv.mData.mPercentSayWhat);
        r.Visit("Percent Beat Mudokon", mTlv.mData.mPercentBeatMud);
        r.Visit("Z Shoot Delay", mTlv.mData.mZShootDelay);
        //r.Visit("Stay Awake", mTlv.mData.mStayAwake);
        r.Visit("Noise Wake Up Distance (Grids)", mTlv.mData.mNoiseWakeUpDistance);
        r.Visit("Slig Spawner Switch ID", mTlv.mData.mSligSpawnerSwitchId);
        //r.Visit("(AE) Unlimited Spawns", mTlv.mData.mUnlimitedSpawns); // HIDDEN
        r.Visit("(AO) Disabled Resources", mTlv.mData.mDisabledResourcesAO.Raw().all);
        r.Visit("(AE) Disabled Resources", mTlv.mData.mDisabledResourcesAE);
    }

    std::string GetIconPath() const override
    {
        return "SligGetPants";
    }

    Path_SligGetPants mTlv;
};

struct Editor_SligGetWings final : public MapObjectBaseInterface<Editor_SligGetWings, Path_SligGetWings::kReliveType>
{
public:
    Editor_SligGetWings()
        : MapObjectBaseInterface<Editor_SligGetWings, Path_SligGetWings::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Spawn Delay State", mTlv.mSpawnDelayState);
        r.Visit("Spawn Move Delay", mTlv.mSpawnMoveDelay);
        r.Visit("Patrol Pause Min", mTlv.mPatrolPauseMin);
        r.Visit("Patrol Pause Max", mTlv.mPatrolPauseMax);
        //r.Visit("Start Direction", mTlv.mFacing);
        r.Visit("Panic Delay", mTlv.mPanicDelay);
        r.Visit("Give Up Chase Delay", mTlv.mGiveUpChaseDelay);
        r.Visit("Pre-chase Delay", mTlv.mPrechaseDelay);
        r.Visit("Slig Bound/Persist ID", mTlv.mSligBoundId);
        r.Visit("Alerted Listen Time", mTlv.mAlertedListenTime);
        r.Visit("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        r.Visit("Grenade Delay", mTlv.mGrenadeDelay);
        r.Visit("Max Velocity", mTlv.mMaxVelocity);
        r.Visit("Launch Grenade Switch ID", mTlv.mLaunchGrenadeSwitchId);
        //r.Visit("Persistant", mTlv.mPersistant);
    }

    std::string GetIconPath() const override
    {
        return "SligGetWings";
    }

    Path_SligGetWings mTlv;
};

struct Editor_CrawlingSligButton final : public MapObjectBaseInterface<Editor_CrawlingSligButton, Path_CrawlingSligButton::kReliveType>
{
    /*
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
    */

public:
    Editor_CrawlingSligButton()
        : MapObjectBaseInterface<Editor_CrawlingSligButton, Path_CrawlingSligButton::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Action", mTlv.mAction);
        //r.Visit("On Sound", mTlv.mOnSound);
        //r.Visit("Off Sound", mTlv.mOffSound);
        //r.Visit("Sound Direction", mTlv.mSoundDirection); // HIDDEN
    }

    std::string GetIconPath() const override
    {
        return "CrawlingSligButton";
    }

    Path_CrawlingSligButton mTlv;
};

struct Editor_Glukkon final : public MapObjectBaseInterface<Editor_Glukkon, Path_Glukkon::kReliveType>
{
    /*
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
    */

public:
    Editor_Glukkon()
        : MapObjectBaseInterface<Editor_Glukkon, Path_Glukkon::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Starting Direction", mTlv.mFacing);
        //r.Visit("Behavior", mTlv.mBehavior);
        r.Visit("Scream Help Delay", mTlv.mScreamHelpDelay);
        r.Visit("Help Switch ID", mTlv.mHelpSwitchId);
        r.Visit("To Calm Delay", mTlv.mToCalmDelay);
        r.Visit("Spawner Switch ID", mTlv.mSpawnSwitchId);
        //r.Visit("Spawn Type", mTlv.mSpawnType);
        r.Visit("Spawn Delay", mTlv.mSpawnDelay);
        //r.Visit("Glukkon Type", mTlv.mGlukkonType);
        r.Visit("Death Switch ID", mTlv.mDeathSwitchId);
        r.Visit("Play Movie Switch ID", mTlv.mPlayMovieSwitchId);
        r.Visit("Movie To Play (FMV ID)", mTlv.mMovieId);
    }

    std::string GetIconPath() const override
    {
        return "Glukkon";
    }

    Path_Glukkon mTlv;
};

struct Editor_GlukkonSwitch final : public MapObjectBaseInterface<Editor_GlukkonSwitch, Path_GlukkonSwitch::kReliveType>
{
public:
    Editor_GlukkonSwitch()
        : MapObjectBaseInterface<Editor_GlukkonSwitch, Path_GlukkonSwitch::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("OK Switch ID", mTlv.mOkSwitchId);
        r.Visit("Fail Switch ID", mTlv.mFailSwitchId);
        r.Visit("X Position", mTlv.mXPos);
        r.Visit("Y Position", mTlv.mYPos);
    }

    std::string GetIconPath() const override
    {
        return "GlukkonSwitch";
    }

    Path_GlukkonSwitch mTlv;
};

struct Editor_GasCountDown final : public MapObjectBaseInterface<Editor_GasCountDown, Path_GasCountDown::kReliveType>
{
public:
    Editor_GasCountDown()
        : MapObjectBaseInterface<Editor_GasCountDown, Path_GasCountDown::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Start Timer Switch ID", mTlv.mStartTimerSwitchId);
        r.Visit("(AE) Gas Countdown Time", mTlv.mGasCountdownTimer);
        r.Visit("(AE) Stop Timer Switch ID", mTlv.mStopTimerSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "GasCountDown";
    }

    Path_GasCountDown mTlv;
};

struct Editor_FallingItem final : public MapObjectBaseInterface<Editor_FallingItem, Path_FallingItem::kReliveType>
{
public:
    Editor_FallingItem()
        : MapObjectBaseInterface<Editor_FallingItem, Path_FallingItem::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Fall Interval", mTlv.mFallInterval);
        r.Visit("Maximum Falling Items", mTlv.mMaxFallingItems);
        //r.Visit("Reset Switch ID After Use", mTlv.mResetSwitchIdAfterUse);
    }

    std::string GetIconPath() const override
    {
        return "FallingItem";
    }

    Path_FallingItem mTlv;
};

struct Editor_BoneBag final : public MapObjectBaseInterface<Editor_BoneBag, Path_BoneBag::kReliveType>
{
public:
    Editor_BoneBag()
        : MapObjectBaseInterface<Editor_BoneBag, Path_BoneBag::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Bone Fall Direction", mTlv.mBoneFallDirection);
        r.Visit("X Velocity", mTlv.mVelX);
        r.Visit("Y Velocity", mTlv.mVelY);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Bone Amount", mTlv.mBoneAmount);
    }

    std::string GetIconPath() const override
    {
        return "BoneBag";
    }

    Path_BoneBag mTlv;
};

struct Editor_SecurityClaw final : public MapObjectBaseInterface<Editor_SecurityClaw, Path_SecurityClaw::kReliveType>
{
public:
    Editor_SecurityClaw()
        : MapObjectBaseInterface<Editor_SecurityClaw, Path_SecurityClaw::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Alarm Switch ID", mTlv.mAlarmSwitchId);
        r.Visit("Alarm Duration", mTlv.mAlarmDuration);
    }

    std::string GetIconPath() const override
    {
        return "SecurityClaw";
    }

    Path_SecurityClaw mTlv;
};

struct Editor_FootSwitch final : public MapObjectBaseInterface<Editor_FootSwitch, Path_FootSwitch::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_FootSwitch::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
        {
            {relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe, "Abe"},
            {relive::Path_FootSwitch::FootSwitchTriggerBy::eAnyone, "Anyone"},
        });
    }*/

public:
    Editor_FootSwitch()
        : MapObjectBaseInterface<Editor_FootSwitch, Path_FootSwitch::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Action", mTlv.mAction);
        //r.Visit("Triggered By", mTlv.mTriggeredBy);
    }

    std::string GetIconPath() const override
    {
        return "FootSwitch";
    }

    Path_FootSwitch mTlv;
};

struct Editor_ZzzSpawner final : public MapObjectBaseInterface<Editor_ZzzSpawner, Path_ZzzSpawner::kReliveType>
{
public:
    Editor_ZzzSpawner()
        : MapObjectBaseInterface<Editor_ZzzSpawner, Path_ZzzSpawner::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Zzz Interval", mTlv.mZzzInterval);
    }

    std::string GetIconPath() const override
    {
        return "SlogHut";
    }
    Path_ZzzSpawner mTlv;
};

struct Editor_SlogSpawner final : public MapObjectBaseInterface<Editor_SlogSpawner, Path_SlogSpawner::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_SlogSpawner::StartDirection>("Enum_SlogSpawnerStartDirection",
        {
            {relive::Path_SlogSpawner::StartDirection::eRight, "Right"},
            {relive::Path_SlogSpawner::StartDirection::eLeft, "Left"},
        });
    }*/

public:
    Editor_SlogSpawner()
        : MapObjectBaseInterface<Editor_SlogSpawner, Path_SlogSpawner::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Max Slogs", mTlv.mMaxSlogs);
        r.Visit("Max Slogs At A Time", mTlv.mMaxSlogsAtATime);
        //r.Visit("Start Direction", mTlv.mStartDirection);
        r.Visit("Slog Spawn Interval", mTlv.mSlogSpawnInterval);
        r.Visit("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        //r.Visit("(AE) Listen To Sligs", mTlv.mListenToSligs);
        r.Visit("(AE) Chase Delay", mTlv.mChaseDelay);
    }

    std::string GetIconPath() const override
    {
        return "SlogSpawner";
    }

    Path_SlogSpawner mTlv;
};

struct Editor_MainMenuController final : public MapObjectBaseInterface<Editor_MainMenuController, Path_MainMenuController::kReliveType>
{
public:
    Editor_MainMenuController()
        : MapObjectBaseInterface<Editor_MainMenuController, Path_MainMenuController::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    Path_MainMenuController mTlv;
};

struct Editor_Scrab final : public MapObjectBaseInterface<Editor_Scrab, Path_Scrab::kReliveType>
{
public:
    Editor_Scrab()
        : MapObjectBaseInterface<Editor_Scrab, Path_Scrab::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Attack Delay", mTlv.mAttackDelay);
        r.Visit("(AE) Patrol Type Run Or Walk Chance (6 Max)", mTlv.mPatrolTypeRunOrWalkChance);
        //r.Visit("(AO) Patrol Type", mTlv.mPatrolType);
        r.Visit("Left Min Delay", mTlv.mPauseLeftMin);
        r.Visit("Left Max Delay", mTlv.mPauseLeftMax);
        r.Visit("Right Min Delay", mTlv.mPauseRightMin);
        r.Visit("Right Max Delay", mTlv.mPauseRightMax);
        r.Visit("Pause After Chase Delay", mTlv.mPauseAfterChaseTime);
        r.Visit("Spotting Abe Delay", mTlv.mSpottingAbeDelay);
        //r.Visit("Roar Randomly", mTlv.mRoarRandomly);
        //r.Visit("(AE) Persistant", mTlv.mPersistant);
        r.Visit("(AE) Possessed Max Whirl Attack Duration", mTlv.mPossessedMaxWhirlAttackDuration);
        //r.Visit("(AE) Kill Enemy", mTlv.mKillEnemy);
    }

    std::string GetIconPath() const override
    {
        return "Scrab";
    }

    Path_Scrab mTlv;
};

struct Editor_ScrabSpawner final : public MapObjectBaseInterface<Editor_ScrabSpawner, Path_ScrabSpawner::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_ScrabSpawner::SpawnDirection>("Enum_ScrabSpawnDirection",
        {
            {relive::Path_ScrabSpawner::SpawnDirection::eNone, "None"},
            {relive::Path_ScrabSpawner::SpawnDirection::eLeft, "Left"},
            {relive::Path_ScrabSpawner::SpawnDirection::eRight, "Right"},
        });
    }*/

public:
    Editor_ScrabSpawner()
        : MapObjectBaseInterface<Editor_ScrabSpawner, Path_ScrabSpawner::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        // Scrab properties
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Attack Delay", mTlv.mAttackDelay);
        r.Visit("(AE) Patrol Type Run Or Walk Chance (6 Max)", mTlv.mPatrolTypeRunOrWalkChance);
        //r.Visit("(AO) Patrol Type", mTlv.mPatrolType);
        r.Visit("Left Min Delay", mTlv.mPauseLeftMin);
        r.Visit("Left Max Delay", mTlv.mPauseLeftMax);
        r.Visit("Right Min Delay", mTlv.mPauseRightMin);
        r.Visit("Right Max Delay", mTlv.mPauseRightMax);
        r.Visit("Pause After Chase Delay", mTlv.mPauseAfterChaseTime);
        r.Visit("Spotting Abe Delay", mTlv.mSpottingAbeDelay);
        //r.Visit("Roar Randomly", mTlv.mRoarRandomly);
        //r.Visit("(AE) Persistant", mTlv.mPersistant);
        r.Visit("(AE) Possessed Max Whirl Attack Duration", mTlv.mPossessedMaxWhirlAttackDuration);
        //r.Visit("(AE) Kill Enemy", mTlv.mKillEnemy);

        // Spawner properties
        r.Visit("Spawner Switch ID", mTlv.mSpawnerSwitchId);
        //r.Visit("Facing", mTlv.mFacing);
    }

    std::string GetIconPath() const override
    {
        return "ScrabSpawner";
    }

    Path_ScrabSpawner mTlv;
};

struct Editor_SlurgSpawner final : public MapObjectBaseInterface<Editor_SlurgSpawner, Path_SlurgSpawner::kReliveType>
{
public:
    Editor_SlurgSpawner()
        : MapObjectBaseInterface<Editor_SlurgSpawner, Path_SlurgSpawner::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        // Slurg properties
        r.Visit("Time Until Turning Around", mTlv.mMovingTimer);
        //r.Visit("Facing", mTlv.mFacing);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID (increment by 1 on death)", mTlv.mSlurgSwitchId);

        // Spawner properties
        r.Visit("Spawn Interval", mTlv.mSpawnInterval);
        r.Visit("Max Slurgs", mTlv.mMaxSlurgs);
        r.Visit("Spawner Switch ID", mTlv.mSpawnerSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "SlurgSpawner";
    }

    Path_SlurgSpawner mTlv;
};

struct Editor_Paramite final : public MapObjectBaseInterface<Editor_Paramite, Path_Paramite::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_Paramite::EntranceType>("Enum_ParamiteEntranceType",
        {
            {relive::Path_Paramite::EntranceType::ePatrol, "Patrol"},
            {relive::Path_Paramite::EntranceType::eSurpriseWeb, "Surprise Web"},
            {relive::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb, "Slightly Higher Spawn Surprise Web"},
        });
    }*/

public:
    Editor_Paramite()
        : MapObjectBaseInterface<Editor_Paramite, Path_Paramite::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Entrance Type", mTlv.mEntranceType);
        r.Visit("Alone - Chase Delay", mTlv.mAloneChaseDelay);
        r.Visit("Surprise Web Delay", mTlv.mSurpriseWebDelayTimer);
        r.Visit("Meat Eating Time", mTlv.mMeatEatingTime);
        r.Visit("Group - Chase Delay", mTlv.mGroupChaseDelay);
        r.Visit("Surprise Web Switch ID", mTlv.mSurpriseWebSwitchId);
        //r.Visit("Hiss Before Attacking", mTlv.mHissBeforeAttack);
        //r.Visit("Delete When Out Of Sight", mTlv.mDeleteWhenOutOfSight);
        //r.Visit("(AE) Attack Fleeches", mTlv.mAttackFleeches);
    }

    std::string GetIconPath() const override
    {
        return "Paramite";
    }

    Path_Paramite mTlv;
};

struct Editor_ParamiteWebLine final : public MapObjectBaseInterface<Editor_ParamiteWebLine, Path_ParamiteWebLine::kReliveType>
{
public:
    Editor_ParamiteWebLine()
        : MapObjectBaseInterface<Editor_ParamiteWebLine, Path_ParamiteWebLine::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "ParamiteWebLine";
    }

    Path_ParamiteWebLine mTlv;
};

struct Editor_MeatSack final : public MapObjectBaseInterface<Editor_MeatSack, Path_MeatSack::kReliveType>
{
public:
    Editor_MeatSack()
        : MapObjectBaseInterface<Editor_MeatSack, Path_MeatSack::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Meat Fall Direction", mTlv.mMeatFallDirection);
        r.Visit("X Velocity", mTlv.mVelX);
        r.Visit("Y Velocity", mTlv.mVelY);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Amount Of Meat", mTlv.mMeatAmount);
    }

    std::string GetIconPath() const override
    {
        return "MeatSack";
    }

    Path_MeatSack mTlv;
};

struct Editor_TorturedMudokon final : public MapObjectBaseInterface<Editor_TorturedMudokon, Path_TorturedMudokon::kReliveType>
{
public:
    Editor_TorturedMudokon()
        : MapObjectBaseInterface<Editor_TorturedMudokon, Path_TorturedMudokon::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Kill Switch ID", mTlv.mKillSwitchId);
        r.Visit("Release Switch ID", mTlv.mReleaseSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "TorturedMudokon";
    }

    Path_TorturedMudokon mTlv;
};

struct Editor_KillUnsavedMuds final : public MapObjectBaseInterface<Editor_KillUnsavedMuds, Path_KillUnsavedMuds::kReliveType>
{
public:
    Editor_KillUnsavedMuds()
        : MapObjectBaseInterface<Editor_KillUnsavedMuds, Path_KillUnsavedMuds::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "KillUnsavedMuds";
    }

    Path_KillUnsavedMuds mTlv;
};

struct Editor_BackgroundGlukkon final : public MapObjectBaseInterface<Editor_BackgroundGlukkon, Path_BackgroundGlukkon::kReliveType>
{
public:
    Editor_BackgroundGlukkon()
        : MapObjectBaseInterface<Editor_BackgroundGlukkon, Path_BackgroundGlukkon::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale Percent", mTlv.mScalePercent);
        r.Visit("Pal ID", mTlv.mPalId);
    }

    std::string GetIconPath() const override
    {
        return "BackgroundGlukkon";
    }

    Path_BackgroundGlukkon mTlv;
};

struct Editor_BellsongStone final : public MapObjectBaseInterface<Editor_BellsongStone, Path_BellsongStone::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BellsongStone::BellsongTypes>("Enum_BellsongTypes",
        {
            {relive::Path_BellsongStone::BellsongTypes::eWhistle, "Whistle"},
            {relive::Path_BellsongStone::BellsongTypes::eChimes, "Chimes"},
        });
    }
    */
public:
    Editor_BellsongStone()
        : MapObjectBaseInterface<Editor_BellsongStone, Path_BellsongStone::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Type", mTlv.mType);
        r.Visit("Code 1", mTlv.mCode1);
        r.Visit("Code 2", mTlv.mCode2);
        r.Visit("Switch ID", mTlv.mSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "BellSongStone";
    }

    Path_BellsongStone mTlv;
};

struct Editor_LightEffect final : public MapObjectBaseInterface<Editor_LightEffect, Path_LightEffect::kReliveType>
{
    /*
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
    */
public:
    Editor_LightEffect()
        : MapObjectBaseInterface<Editor_LightEffect, Path_LightEffect::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Type", mTlv.mType);
        r.Visit("Size", mTlv.mSize);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Direction", mTlv.mDirection);
    }

    std::string GetIconPath() const override
    {
        return "LightEffect";
    }

    Path_LightEffect mTlv;
};

struct Editor_StartController final : public MapObjectBaseInterface<Editor_StartController, Path_StartController::kReliveType>
{
public:
    Editor_StartController()
        : MapObjectBaseInterface<Editor_StartController, Path_StartController::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "StartController";
    }

    Path_StartController mTlv;
};

struct Editor_HintFly final : public MapObjectBaseInterface<Editor_HintFly, Path_HintFly::kReliveType>
{
public:
    Editor_HintFly()
        : MapObjectBaseInterface<Editor_HintFly, Path_HintFly::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Message ID", mTlv.mMessageId);
    }

    std::string GetIconPath() const override
    {
        return "HintFly";
    }

    Path_HintFly mTlv;
};

struct Editor_Bat final : public MapObjectBaseInterface<Editor_Bat, Path_Bat::kReliveType>
{
public:
    Editor_Bat()
        : MapObjectBaseInterface<Editor_Bat, Path_Bat::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Time Before Moving", mTlv.mTimeBeforeMoving);
        r.Visit("Speed", mTlv.mSpeed);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Attack Duration", mTlv.mAttackDuration);
    }

    std::string GetIconPath() const override
    {
        return "Bat";
    }

    Path_Bat mTlv;
};

struct Editor_BellHammer final : public MapObjectBaseInterface<Editor_BellHammer, Path_BellHammer::kReliveType>
{
public:
    Editor_BellHammer()
        : MapObjectBaseInterface<Editor_BellHammer, Path_BellHammer::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Action", mTlv.mAction);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Direction", mTlv.mDirection);
    }

    std::string GetIconPath() const override
    {
        return "BellHammer";
    }

    Path_BellHammer mTlv;
};

struct Editor_ElumPathTrans final : public MapObjectBaseInterface<Editor_ElumPathTrans, Path_ElumPathTrans::kReliveType>
{
public:
    Editor_ElumPathTrans()
        : MapObjectBaseInterface<Editor_ElumPathTrans, Path_ElumPathTrans::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Next Level", mTlv.mNextLevel);
        r.Visit("Next Path", mTlv.mNextPath);
        r.Visit("Next Camera", mTlv.mNextCamera);
    }

    std::string GetIconPath() const override
    {
        return "ElumPathTrans";
    }

    Path_ElumPathTrans mTlv;
};

struct Editor_ElumStart final : public MapObjectBaseInterface<Editor_ElumStart, Path_ElumStart::kReliveType>
{
public:
    Editor_ElumStart()
        : MapObjectBaseInterface<Editor_ElumStart, Path_ElumStart::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "ElumStart";
    }

    Path_ElumStart mTlv;
};

struct Editor_ElumWall final : public MapObjectBaseInterface<Editor_ElumWall, Path_ElumWall::kReliveType>
{
public:
    Editor_ElumWall()
        : MapObjectBaseInterface<Editor_ElumWall, Path_ElumWall::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "ElumWall";
    }

    Path_ElumWall mTlv;
};

struct Editor_RingMudokon final : public MapObjectBaseInterface<Editor_RingMudokon, Path_RingMudokon::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_RingMudokon::MustFaceMud>("Enum_RingMudokon_MustFaceMud",
        {
            {relive::Path_RingMudokon::MustFaceMud::eYes, "Yes"},
            {relive::Path_RingMudokon::MustFaceMud::eNo, "No"},
        });
    }*/

public:
    Editor_RingMudokon()
        : MapObjectBaseInterface<Editor_RingMudokon, Path_RingMudokon::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Facing", mTlv.mFacing);
        //r.Visit("Abe Must Face Mudokon", mTlv.mAbeMustFaceMud);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Give Password", mTlv.mGivePassword);
        r.Visit("Code 1", mTlv.mCode1);
        r.Visit("Code 2", mTlv.mCode2);
        //r.Visit("Action", mTlv.mAction);
        r.Visit("Ring And Angry Mudokon Timeout", mTlv.mRingTimeout);
        //r.Visit("Give Ring Without Password", mTlv.mGiveRingWithoutPassword);
    }

    std::string GetIconPath() const override
    {
        return "RingMudokon";
    }

    Path_RingMudokon mTlv;
};

struct Editor_RingCancel final : public MapObjectBaseInterface<Editor_RingCancel, Path_RingCancel::kReliveType>
{
public:
    Editor_RingCancel()
        : MapObjectBaseInterface<Editor_RingCancel, Path_RingCancel::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "RingCancel";
    }

    Path_RingCancel mTlv;
};

struct Editor_MeatSaw final : public MapObjectBaseInterface<Editor_MeatSaw, Path_MeatSaw::kReliveType>
{
    /*
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
    */
public:
    Editor_MeatSaw()
        : MapObjectBaseInterface<Editor_MeatSaw, Path_MeatSaw::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch Min Time Off", mTlv.mSwitchMinTimeOff);
        r.Visit("Switch Max Time Off", mTlv.mSwitchMaxTimeOff);
        r.Visit("Y Travel Distance", mTlv.mYTravelDistance);
        r.Visit("Switch ID", mTlv.mSwitchId);
        //r.Visit("Type", mTlv.mType);
        r.Visit("Switch Speed", mTlv.mSwitchSpeed);
        //r.Visit("Start State", mTlv.mStartState);
        r.Visit("Automatic Speed", mTlv.mAutomaticSpeed);
        r.Visit("Automatic Min Time Off", mTlv.mAutomaticMinTimeOff);
        r.Visit("Automatic Max Time Off", mTlv.mAutomaticMaxTimeOff);
        r.Visit("Start At Bottom", mTlv.mStartAtBottom);
    }

    std::string GetIconPath() const override
    {
        return "MeatSaw";
    }

    Path_MeatSaw mTlv;
};

struct Editor_ChimeLock final : public MapObjectBaseInterface<Editor_ChimeLock, Path_ChimeLock::kReliveType>
{
public:
    Editor_ChimeLock()
        : MapObjectBaseInterface<Editor_ChimeLock, Path_ChimeLock::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Solve Switch ID", mTlv.mSolveSwitchId);
        r.Visit("Code 1", mTlv.mCode1);
        r.Visit("Code 2", mTlv.mCode2);
        r.Visit("Password Switch ID", mTlv.mPasswordSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "ChimeLock";
    }

    Path_ChimeLock mTlv;
};

struct Editor_FlintLockFire final : public MapObjectBaseInterface<Editor_FlintLockFire, Path_FlintLockFire::kReliveType>
{
public:
    Editor_FlintLockFire()
        : MapObjectBaseInterface<Editor_FlintLockFire, Path_FlintLockFire::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        r.Visit("Switch ID", mTlv.mSwitchId);
    }

    std::string GetIconPath() const override
    {
        return "FlintLockFire";
    }

    Path_FlintLockFire mTlv;
};

struct Editor_MudokonPathTrans final : public MapObjectBaseInterface<Editor_MudokonPathTrans, Path_MudokonPathTrans::kReliveType>
{
public:
    Editor_MudokonPathTrans()
        : MapObjectBaseInterface<Editor_MudokonPathTrans, Path_MudokonPathTrans::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Next Level", mTlv.mNextLevel);
        r.Visit("Next Path", mTlv.mNextPath);
        r.Visit("Next Camera", mTlv.mNextCamera);
    }

    Path_MudokonPathTrans mTlv;
};

struct Editor_ScrabNoFall final : public MapObjectBaseInterface<Editor_ScrabNoFall, Path_ScrabNoFall::kReliveType>
{
public:
    Editor_ScrabNoFall()
        : MapObjectBaseInterface<Editor_ScrabNoFall, Path_ScrabNoFall::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    Path_ScrabNoFall mTlv;
};

struct Editor_LiftMudokon final : public MapObjectBaseInterface<Editor_LiftMudokon, Path_LiftMudokon::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_LiftMudokon::Direction>("Enum_LiftMudokonDirection",
        {
            {relive::Path_LiftMudokon::Direction::eRight, "Right"},
            {relive::Path_LiftMudokon::Direction::eLeft, "Left"},
        });
    }
    */
public:
    Editor_LiftMudokon()
        : MapObjectBaseInterface<Editor_LiftMudokon, Path_LiftMudokon::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("How Far To Walk", mTlv.mHowFarToWalk);
        r.Visit("Lift Switch ID", mTlv.mLiftSwitchId);
        //r.Visit("Facing", mTlv.mFacing);
        //r.Visit("Give Password", mTlv.mGivePassword);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Code 1", mTlv.mCode1);
        r.Visit("Code 2", mTlv.mCode2);
    }

    std::string GetIconPath() const override
    {
        return "LiftMudokon";
    }

    Path_LiftMudokon mTlv;
};

struct Editor_HoneySack final : public MapObjectBaseInterface<Editor_HoneySack, Path_HoneySack::kReliveType>
{
public:
    Editor_HoneySack()
        : MapObjectBaseInterface<Editor_HoneySack, Path_HoneySack::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Chase Time", mTlv.mChaseTime);
        r.Visit("Scale", mTlv.mScale);
    }

    std::string GetIconPath() const override
    {
        return "HoneySack";
    }

    Path_HoneySack mTlv;
};

struct Editor_SlingMudokon final : public MapObjectBaseInterface<Editor_SlingMudokon, Path_SlingMudokon::kReliveType>
{
public:
    Editor_SlingMudokon()
        : MapObjectBaseInterface<Editor_SlingMudokon, Path_SlingMudokon::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Don't Whistle Password", mTlv.mDontWhistlePassword);
        r.Visit("Code 1", mTlv.mCode1);
        r.Visit("Code 2", mTlv.mCode2);
    }

    std::string GetIconPath() const override
    {
        return "SlingMudokon";
    }

    Path_SlingMudokon mTlv;
};

struct Editor_BeeSwarmHole final : public MapObjectBaseInterface<Editor_BeeSwarmHole, Path_BeeSwarmHole::kReliveType>
{
    /*
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<relive::Path_BeeSwarmHole::MovementType>("Enum_BeeSwarmHoleMovementType",
        {
            {relive::Path_BeeSwarmHole::MovementType::eHover, "Hover"},
            {relive::Path_BeeSwarmHole::MovementType::eAttack, "Attack"},
            {relive::Path_BeeSwarmHole::MovementType::eFollowPath, "Follow Path"},
        });
    }
    */
public:
    Editor_BeeSwarmHole()
        : MapObjectBaseInterface<Editor_BeeSwarmHole, Path_BeeSwarmHole::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Start Interval", mTlv.mStartInterval);
        //r.Visit("Movement Type", mTlv.mMovementType);
        r.Visit("Bees Amount", mTlv.mBeesAmount);
        r.Visit("Chase Time", mTlv.mChaseTime);
        r.Visit("Speed", mTlv.mSpeed);
    }

    std::string GetIconPath() const override
    {
        return "BeeSwarmHole";
    }

    Path_BeeSwarmHole mTlv;
};

struct Editor_RollingBall final : public MapObjectBaseInterface<Editor_RollingBall, Path_RollingBall::kReliveType>
{
public:
    Editor_RollingBall()
        : MapObjectBaseInterface<Editor_RollingBall, Path_RollingBall::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Roll Direction", mTlv.mRollDirection);
        r.Visit("Release Switch ID", mTlv.mReleaseSwitchId);
        r.Visit("Max Speed", mTlv.mMaxSpeed);
        r.Visit("Acceleration", mTlv.mAcceleration);
    }

    std::string GetIconPath() const override
    {
        return "RollingBall";
    }

    Path_RollingBall mTlv;
};

struct Editor_RollingBallStopper final : public MapObjectBaseInterface<Editor_RollingBallStopper, Path_RollingBallStopper::kReliveType>
{
public:
    Editor_RollingBallStopper()
        : MapObjectBaseInterface<Editor_RollingBallStopper, Path_RollingBallStopper::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Stopper Switch ID", mTlv.mStopperSwitchId);
        r.Visit("Scale", mTlv.mScale);
        r.Visit("Ball Switch ID", mTlv.mBallSwitchId);
        //r.Visit("Stop Direction", mTlv.mStopDirection);
    }

    std::string GetIconPath() const override
    {
        return "RollingBallStopper";
    }

    Path_RollingBallStopper mTlv;
};

struct Editor_BeeNest final : public MapObjectBaseInterface<Editor_BeeNest, Path_BeeNest::kReliveType>
{
public:
    Editor_BeeNest()
        : MapObjectBaseInterface<Editor_BeeNest, Path_BeeNest::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        r.Visit("Switch ID", mTlv.mSwitchId);
        r.Visit("Swarm Size", mTlv.mSwarmSize);
        r.Visit("Chase Time", mTlv.mChaseTime);
        r.Visit("Speed", mTlv.mSpeed);
        r.Visit("Amount Of Bees", mTlv.mBeesAmount);
    }

    std::string GetIconPath() const override
    {
        return "Bees";
    }

    Path_BeeNest mTlv;
};

struct Editor_ZBall final : public MapObjectBaseInterface<Editor_ZBall, Path_ZBall::kReliveType>
{
    /*
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
    */
public:
    Editor_ZBall()
        : MapObjectBaseInterface<Editor_ZBall, Path_ZBall::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);

        //r.Visit("Start Position", mTlv.mStartPos);
        r.Visit("Scale", mTlv.mScale);
        //r.Visit("Speed", mTlv.mSpeed);
    }

    std::string GetIconPath() const override
    {
        return "ZBall";
    }

    Path_ZBall mTlv;
};

struct Editor_Honey final : public MapObjectBaseInterface<Editor_Honey, Path_Honey::kReliveType>
{
public:
    Editor_Honey()
        : MapObjectBaseInterface<Editor_Honey, Path_Honey::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "Honey";
    }

    Path_Honey mTlv;
};

struct Editor_HoneyDripTarget final : public MapObjectBaseInterface<Editor_HoneyDripTarget, Path_HoneyDripTarget::kReliveType>
{
public:
    Editor_HoneyDripTarget()
        : MapObjectBaseInterface<Editor_HoneyDripTarget, Path_HoneyDripTarget::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    std::string GetIconPath() const override
    {
        return "HoneyDripTarget";
    }

    Path_HoneyDripTarget mTlv;
};

struct Editor_Elum final : public MapObjectBaseInterface<Editor_Elum, Path_Elum::kReliveType>
{
public:
    Editor_Elum()
        : MapObjectBaseInterface<Editor_Elum, Path_Elum::kReliveType>(&mTlv)
    { }

    void Visit(IReflector& r) override
    {
        MapObjectBase::Visit(r);
    }

    Path_Elum mTlv;
};
} // namespace relive
