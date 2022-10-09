#pragma once

#include "TlvObjectBaseAO.hpp"
#include "TlvObjectBaseMacros.hpp"
#include "../../relive_lib/data_conversion/PathTlvsAO.hpp"
#include "../../AliveLibCommon/AnimResources.hpp"

#define CTOR_AO(className, objectTypeName, tlvEnumType)\
    className() : TlvObjectBaseAO(sizeof(AO::className), tlvEnumType, objectTypeName, &mTlv)\
    {\
    }\
    \
    className(ReliveAPI::TypesCollectionBase& globalTypes, const AO::Path_TLV* pTlvSrc = nullptr) \
        : TlvObjectBaseAO(sizeof(AO::className), globalTypes, tlvEnumType, objectTypeName, &mTlv)\
    {\
        if (pTlvSrc)\
        {\
            mTlv = *static_cast<const AO::className*>(pTlvSrc);\
            ConvertXYPos(); \
        }\
        else\
        {\
            mPSelfTlv->mLength = static_cast<s16>(mSizeOfT);\
            mPSelfTlv->mTlvType32.mType = mType;\
        }\
        AddProperties(globalTypes);\
    }\
    AO::className mTlv = {};\
    void AddProperties(ReliveAPI::TypesCollectionBase& globalTypes)

#define EMPTY_CTOR_AO() (void) globalTypes

namespace AO {

struct Path_Preloader final : public Path_TLV
{
    s32 unload_cams_ASAP;
};

struct Path_Elum final : public Path_TLV
{
    // No fields
};

} // namespace AO

namespace AOTlvs {

struct Path_SwitchStateBooleanLogic final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_SwitchStateBooleanLogic::BooleanOperatorType>("Enum_BooleanOperatorType",
            {
                {AO::Path_SwitchStateBooleanLogic::BooleanOperatorType::eAllOn_0, "All On"},
                {AO::Path_SwitchStateBooleanLogic::BooleanOperatorType::e1OnAnd2Off_1, "1 On And 2 Off"},
                {AO::Path_SwitchStateBooleanLogic::BooleanOperatorType::e1Or2On_2, "1 Or 2 On"},
                {AO::Path_SwitchStateBooleanLogic::BooleanOperatorType::e1OnOr2Off_3, "1 On Or 2 Off"},
            });
    }

    CTOR_AO(Path_SwitchStateBooleanLogic, "SwitchStateBooleanLogic", AO::TlvTypes::SwitchStateBooleanLogic_104)
    {
        ADD("Input Switch ID 1", mTlv.field_18_input1);
        ADD("Input Switch ID 2", mTlv.field_1A_input2);
        ADD("Output Switch ID", mTlv.field_1C_output);
        ADD("Operator", mTlv.field_1E_operator);
    }
};

struct Path_Elum final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Elum, "Elum", AO::TlvTypes::Elum_70)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_MainMenuController final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MainMenuController, "MenuController", AO::TlvTypes::MainMenuController_90)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_KillUnsavedMuds final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_KillUnsavedMuds, "KillUnsavedMuds", AO::TlvTypes::KillUnsavedMuds_113)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_DoorFlame final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_DoorFlame::Scale>("Enum_DoorFlame_Scale",
            {
                {AO::Path_DoorFlame::Scale::eFull_0, "Full"},
                {AO::Path_DoorFlame::Scale::eHalf_1, "Half"},
                {AO::Path_DoorFlame::Scale::eHalf_2, "Half (ignore)"},
            });

        types.AddEnum<AO::Path_DoorFlame::Colour>("Enum_DoorFlame_Colour",
            {
                {AO::Path_DoorFlame::Colour::default_0, "Unknown"},
                {AO::Path_DoorFlame::Colour::red_1, "Red"},
                {AO::Path_DoorFlame::Colour::green_2, "Green"},
                {AO::Path_DoorFlame::Colour::blue_3, "Blue"}
            });
    }

    CTOR_AO(Path_DoorFlame, "DoorFlame", AO::TlvTypes::DoorFlame_84)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Colour", mTlv.mColour);
    }
};

struct Path_ResetPath final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ResetPath, "ResetPath", AO::TlvTypes::ResetPath_115)
    {
        ADD("clear_ids", mTlv.mClearIds);
        ADD("from", mTlv.mFrom);
        ADD("to", mTlv.mTo);
        ADD("exclude", mTlv.mExclude);
        ADD("clear_objects", mTlv.mClearObjects);
        ADD("path", mTlv.mPath);
    }
};

struct Path_CreditsController final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_CreditsController, "CreditsController", AO::TlvTypes::CreditsController_101)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_BackgroundGlukkon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BackgroundGlukkon, "BackgroundGlukkon", AO::TlvTypes::BackgroundGlukkon_112)
    {
        ADD("Scale Percent", mTlv.mScalePercent);
        ADD("pal_id", mTlv.mPalId);
        ADD("target_id", mTlv.mTargetId);
        ADD("voice_adjust", mTlv.mVoiceAdjust);

        ADD_RESOURCE(AnimId::Background_Glukkon_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Background_Glukkon_KillHim1, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Background_Glukkon_KillHim2, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Background_Glukkon_Laugh, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_GasEmitter final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_GasEmitter, "GasEmitter", AO::TlvTypes::GasEmitter_110)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_GasCountDown final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_GasCountDown, "GasCountDown", AO::TlvTypes::GasCountDown_108)
    {
        ADD("Start Switch ID", mTlv.mStartTimerSwitchId);
    }
};

struct Path_MovieStone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovieStone, "MovieStone", AO::TlvTypes::MovieStone_51)
    {
        ADD("FMV ID", mTlv.mData.mMovieId);
        ADD("Scale", mTlv.mData.mScale);
    }
};

struct Path_BellsongStone final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::BellsongTypes>("Enum_BellsongTypes",
        {
            {AO::BellsongTypes::eWhistle, "Whistle"},
            {AO::BellsongTypes::eChimes, "Chimes"},
        });
    }

    CTOR_AO(Path_BellsongStone, "BellSongStone", AO::TlvTypes::BellSongStone_54)
    {
        ADD("Scale", mTlv.mData.mScale);
        ADD("Type", mTlv.mData.mType);
        ADD("Code 1", mTlv.mData.mCode1);
        ADD("Code 2", mTlv.mData.mCode2);
        ADD("Switch ID", mTlv.mData.mSwitchId);
    }
};

struct Path_HandStone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HandStone, "HandStone", AO::TlvTypes::HandStone_100)
    {
        ADD("Scale", mTlv.mData.scale);

        ADD("Camera 1 Level", mTlv.mData.camera1.level);
        ADD("Camera 1 Path", mTlv.mData.camera1.path);
        ADD("Camera 1 Camera", mTlv.mData.camera1.camera);

        ADD("Camera 2 Level", mTlv.mData.camera2.level);
        ADD("Camera 2 Path", mTlv.mData.camera2.path);
        ADD("Camera 2 Camera", mTlv.mData.camera2.camera);

        ADD("Camera 3 Level", mTlv.mData.camera3.level);
        ADD("Camera 3 Path", mTlv.mData.camera3.path);
        ADD("Camera 3 Camera", mTlv.mData.camera3.camera);
    }
};

struct Path_Door final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Door::DoorTypes>("Enum_DoorTypes",
        {
            {AO::Path_Door::DoorTypes::eBasicDoor_0, "Basic Door"},
            {AO::Path_Door::DoorTypes::eTrialDoor_1, "Trial Door"},
            {AO::Path_Door::DoorTypes::eHubDoor_2, "Hub Door"},
        });
    }

    CTOR_AO(Path_Door, "Door", AO::TlvTypes::Door_6)
    {
        ADD("Level", mTlv.mNextLevel);
        ADD("Path", mTlv.mNextPath);
        ADD("Camera", mTlv.mNextCamera);
        ADD("Scale", mTlv.mScale);
        ADD("Door Number", mTlv.mDoorId);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Target Door Number", mTlv.mTargetDoorId);
        ADD("Door Type", mTlv.mDoorType);
        ADD("Door Closed", mTlv.mDoorClosed);
        ADD("Hub 1 ID", mTlv.mHub1);
        ADD("Hub 2 ID", mTlv.mHub2);
        ADD("Hub 3 ID", mTlv.mHub3);
        ADD("Hub 4 ID", mTlv.mHub4);
        ADD("Hub 5 ID", mTlv.mHub5);
        ADD("Hub 6 ID", mTlv.mHub6);
        ADD("Hub 7 ID", mTlv.mHub7);
        ADD("Hub 8 ID", mTlv.mHub8);
        ADD("Wipe Effect", mTlv.mWipeEffect); // TODO: enum
        ADD("Movie Number", mTlv.mMovieId);
        ADD("X Offset", mTlv.mDoorOffsetX);
        ADD("Y Offset", mTlv.mDoorOffsetY);
        ADD("Wipe X Org (Unused?)", mTlv.field_42_wipe_x_org);
        ADD("Wipe Y Org (Unused?)", mTlv.field_44_wipe_y_org);
        ADD("Abe Direction On Exit", mTlv.mExitDirection);
    }
};

struct Path_ContinuePoint final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_ContinuePoint::spawnDirection>("Enum_ContinuePointSpawnDirection",
            {
                {AO::Path_ContinuePoint::spawnDirection::eRight_0, "Right"},
                {AO::Path_ContinuePoint::spawnDirection::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_ContinuePoint, "ContinuePoint", AO::TlvTypes::ContinuePoint_0)
    {
        ADD("Zone Number", mTlv.mZoneNumber);
        ADD("Clear From Id", mTlv.mClearFromId);
        ADD("Clear To Id", mTlv.mClearToId);
        ADD("Elum Restarts", mTlv.mElumRestarts);
        ADD("Abe Spawn Direction", mTlv.mAbeSpawnDir);
    }
};

struct Path_Hoist final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Hoist::Type>("Enum_HoistType",
        {
            {AO::Path_Hoist::Type::eNextEdge, "Next Edge"},
            {AO::Path_Hoist::Type::eNextFloor, "Next Floor"},
            {AO::Path_Hoist::Type::eOffScreen, "Off Screen"},
        });

        types.AddEnum<AO::Path_Hoist::GrabDirection>("Enum_HoistGrabDirection",
        {
            {AO::Path_Hoist::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
            {AO::Path_Hoist::GrabDirection::eFacingLeft, "Facing Left"},
            {AO::Path_Hoist::GrabDirection::eFacingRight, "Facing Right"},
        });
    }

    CTOR_AO(Path_Hoist, "Hoist", AO::TlvTypes::Hoist_3)
    {
        ADD("Hoist Type", mTlv.mHoistType);
        ADD("Grab Direction", mTlv.mGrabDirection);
        ADD("Switch ID (Unused?)", mTlv.field_1C_switch_id);
    }
};

struct Path_PathTransition final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_PathTransition, "PathTransition", AO::TlvTypes::PathTransition_1)
    {
        ADD("Level", mTlv.mNextLevel);
        ADD("Path", mTlv.mNextPath);
        ADD("Camera", mTlv.mNextCamera);
        ADD("Movie", mTlv.mMovieId);
        ADD("Wipe", mTlv.mWipeEffect);   // TODO: Enum
        ADD("Next Path Scale", mTlv.mNextPathScale);
    }
};

struct Path_Lever final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Lever::LeverSoundType>("Enum_LeverSoundType",
        {
            {AO::Path_Lever::LeverSoundType::eNone, "None"},
            {AO::Path_Lever::LeverSoundType::eWell_1, "Well"},
            {AO::Path_Lever::LeverSoundType::eSwitchBellHammer_2, "Switch Bell Hammer"},
            {AO::Path_Lever::LeverSoundType::eDoor_3, "Door"},
            {AO::Path_Lever::LeverSoundType::eElectricWall_4, "Electric Wall"},
            {AO::Path_Lever::LeverSoundType::eSecurityOrb_5, "Security Orb"},
        });

        types.AddEnum<AO::Path_Lever::LeverSoundDirection>("Enum_LeverSoundDirection",
        {
            {AO::Path_Lever::LeverSoundDirection::eLeftAndRight_0, "Left And Right"},
            {AO::Path_Lever::LeverSoundDirection::eLeft_1, "Left"},
            {AO::Path_Lever::LeverSoundDirection::eRight_2, "Right"},
        });
    }

    CTOR_AO(Path_Lever, "Lever", AO::TlvTypes::Lever_26)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("Scale", mTlv.mScale);
        ADD("On Sound", mTlv.mOnSound);
        ADD("Off Sound", mTlv.mOffSound);
        ADD("Sound Direction", mTlv.mSoundDirection);
    }
};

struct Path_LightEffect final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LightEffect::Type>("Enum_LightType",
        {
            {AO::Path_LightEffect::Type::Star_0, "Star"},
            {AO::Path_LightEffect::Type::GoldGlow_1, "Gold Glow"},
            {AO::Path_LightEffect::Type::GreenGlow_2, "Green Glow"},
            {AO::Path_LightEffect::Type::FlintGlow_3, "Flint Glow"},
            {AO::Path_LightEffect::Type::Switchable_RedGreenDoorLights_4, "RedGreen Door Light"},
            {AO::Path_LightEffect::Type::Switchable_RedGreenHubLight_5, "RedGreen Hub Light"},
        });
    }

    CTOR_AO(Path_LightEffect, "LightEffect", AO::TlvTypes::LightEffect_106)
    {
        ADD("Type", mTlv.mType);
        ADD("Size", mTlv.mSize);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Direction", mTlv.mDirection);
    }
};

struct Path_ElectricWall final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_ElectricWall::ElectricWallStartState>("Enum_ElectricWallStartState",
        {
            {AO::Path_ElectricWall::ElectricWallStartState::eOff_0, "Off"},
            {AO::Path_ElectricWall::ElectricWallStartState::eOn_1, "On"},
        });
    }

    CTOR_AO(Path_ElectricWall, "ElectricWall", AO::TlvTypes::ElectricWall_67)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Start State", mTlv.mStartState);

        ADD_RESOURCE(AnimId::Electric_Wall, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_ContinueZone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ContinueZone, "ContinueZone", AO::TlvTypes::ContinueZone_2)
    {
        ADD("Zone Number", mTlv.field_10_zone_number);
    }
};

struct Path_StartController final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_StartController, "StartController", AO::TlvTypes::StartController_28)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_Edge final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Edge::GrabDirection>("Enum_EdgeGrabDirection",
        {
            {AO::Path_Edge::GrabDirection::eFacingLeft, "Facing Left"},
            {AO::Path_Edge::GrabDirection::eFacingRight, "Facing Right"},
            {AO::Path_Edge::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
        });
    }

    CTOR_AO(Path_Edge, "Edge", AO::TlvTypes::Edge_4)
    {
        ADD("Grab Direction", mTlv.mGrabDirection);
        ADD("Can Grab", mTlv.mCanGrab);
    }
};

struct Path_WellLocal final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_WellLocal, "WellLocal", AO::TlvTypes::WellLocal_11)
    {
        // Well base
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD_HIDDEN("Other Well ID", mTlv.mOtherWellId); // hidden because this is only used by the well express
        ADD("Animation ID", mTlv.mAnimId);
        ADD("Exit X", mTlv.mExitX);
        ADD("Exit Y", mTlv.mExitY);
        ADD("Disabled XPos", mTlv.mOffLevelOrDestX.dx);
        ADD("Disabled YPos", mTlv.mOffPathOrDestY);

        // Well local
        ADD("Enabled XPos", mTlv.mOnDestX);
        ADD("Enabled YPos", mTlv.mOnDestY);
        ADD("Emit Leaves", mTlv.mEmitLeaves);
        ADD("Leaf X", mTlv.mLeafX);
        ADD("Leaf Y", mTlv.mLeafY);
    }
};

struct Path_WellExpress final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_WellExpress, "WellExpress", AO::TlvTypes::WellExpress_45)
    {
        // Well base
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Other Well ID", mTlv.mOtherWellId);
        ADD("Animation ID", mTlv.mAnimId);
        ADD("Exit X", mTlv.mExitX);
        ADD("Exit Y", mTlv.mExitY);
        ADD("Off Level", mTlv.mOffLevelOrDestX.level);
        ADD("Off Path", mTlv.mOffPathOrDestY);

        // Well express
        ADD("Off Camera", mTlv.mOffDestCamera);
        ADD("Off Other Well ID", mTlv.mOffOtherWellId);
        ADD("On Level", mTlv.mOnDestLevel);
        ADD("On Path", mTlv.mOnDestPath);
        ADD("On Camera", mTlv.mOnDestCamera);
        ADD("On Other Well ID", mTlv.mOnOtherWellId);
        ADD("Emit Leaves", mTlv.mEmitLeaves);
        ADD("Leaf X", mTlv.mLeafX);
        ADD("Leaf Y", mTlv.mLeafY);
        ADD("Movie ID", mTlv.mMovieId);
    }
};

struct Path_InvisibleZone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_InvisibleZone, "InvisibleZone", AO::TlvTypes::InvisibleZone_58)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_EnemyStopper final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
        {
            {AO::Path_EnemyStopper::StopDirection::Left_0, "Left"},
            {AO::Path_EnemyStopper::StopDirection::Right_1, "Right"},
            {AO::Path_EnemyStopper::StopDirection::Both_2, "Both"},
        });
    }

    CTOR_AO(Path_EnemyStopper, "EnemyStopper", AO::TlvTypes::EnemyStopper_79)
    {
        ADD("Stop Direction", mTlv.mStopDirection);
        ADD("Switch ID", mTlv.mSwitchId);
    }
};

struct Path_Slig final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Slig, "Slig", AO::TlvTypes::Slig_24)
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
        ADD("unknown", mTlv.field_2C_unused);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("Chase Abe When Spotted", mTlv.mChaseAbeWhenSpotted);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Panic Timeout", mTlv.mPanicTimeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.mNumPanicSounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.mPanicSoundTimeout);
        ADD("Stop Chase Delay", mTlv.mStopChaseDelay);
        ADD("Time To Wait Before Chase", mTlv.mTimeToWaitBeforeChase);
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Alerted Listen Time", mTlv.mAlertedListenTime);
        ADD("Percent Say What", mTlv.mPercentSayWhat);
        ADD("Percent Beat Mudokon", mTlv.mPercentBeatMud);
        ADD_HIDDEN("Talk To Abe (Unused?)", mTlv.mTalkToAbe);
        ADD("Don't Shoot (Unused?)", mTlv.field_4A_dont_shoot);
        ADD("Z Shoot Delay", mTlv.mZShootDelay);
        ADD("Stay Awake", mTlv.mStayAwake);
        ADD("Disabled Resources", mTlv.mDisabledResources.Raw().all);
        ADD("Noise Wake Up Distance (Grids)", mTlv.mNoiseWakeUpDistance);
        ADD("Slig Spawner Switch ID", mTlv.mSligSpawnerSwitchId);
    }
};

struct Path_DeathDrop final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_DeathDrop, "DeathDrop", AO::TlvTypes::DeathDrop_5)
    {
        ADD_HIDDEN("animation", mTlv.animation);
        ADD_HIDDEN("sound", mTlv.sound);
        ADD_HIDDEN("id", mTlv.id);
        ADD_HIDDEN("action", mTlv.action);
        ADD_HIDDEN("set_value", mTlv.set_value);
    }
};

struct Path_SligBoundLeft final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligBoundLeft, "SligBoundLeft", AO::TlvTypes::eSligBoundLeft_57)
    {
        ADD("Slig ID", mTlv.mSligBoundId);
        ADD("Disabled Resources", mTlv.mDisabledResources.Raw().all);
    }
};

struct Path_SligBoundRight final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligBoundRight, "SligBoundRight", AO::TlvTypes::eSligBoundRight_76)
    {
        ADD("Slig ID", mTlv.mSligBoundId);
        ADD("Disabled Resources", mTlv.mDisabledResources.Raw().all);
    }
};

struct Path_SligPersist final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligPersist, "SligPersist", AO::TlvTypes::eSligPersist_77)
    {
        ADD("Slig ID", mTlv.mSligBoundId);
        ADD("Disabled Resources", mTlv.mDisabledResources.Raw().all);
    }
};

struct Path_SecurityOrb final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityOrb, "SecurityOrb", AO::TlvTypes::SecurityOrb_29)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Disabled Resources", mTlv.mDisabledResources);

        ADD_RESOURCE(AnimId::Abe_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Body_Gib, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_FallingItem final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_FallingItem, "FallingItem", AO::TlvTypes::FallingItem_15)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Fall Interval", mTlv.mFallInterval);
        ADD("Max Falling Items", mTlv.mMaxFallingItems);
        ADD("Reset Switch ID After Use", mTlv.mResetSwitchIdAfterUse);
    }
};

struct Path_Mine final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Mine, "Mine", AO::TlvTypes::Mine_46)
    {
        ADD_HIDDEN("Num Patterns (Unused?)", mTlv.field_18_num_patterns);
        ADD_HIDDEN("Pattern (Unused?)", mTlv.field_1A_pattern);
        ADD("Scale", mTlv.mScale);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Path_Dove final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Dove, "Dove", AO::TlvTypes::Dove_12)
    {
        ADD("Dove Count", mTlv.mDoveCount);
        ADD("Pixel Perfect", mTlv.mPixelPerfect);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_UXB final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_UXB::UXBStartState>("UXBStartState",
        {
            {AO::Path_UXB::UXBStartState::eOn, "On"},
            {AO::Path_UXB::UXBStartState::eOff, "Off"},
        });
    }

    CTOR_AO(Path_UXB, "UXB", AO::TlvTypes::UXB_47)
    {
        ADD("Pattern Length (Max 4)", mTlv.mPatternLength);
        ADD("Pattern", mTlv.mPattern);
        ADD("Scale", mTlv.mScale);
        ADD("Start State", mTlv.mStartState);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_HintFly final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HintFly, "HintFly", AO::TlvTypes::HintFly_92)
    {
        ADD("Message ID", mTlv.mMessageId);

        ADD_RESOURCE(AnimId::HintFly, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_Bat final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Bat, "Bat", AO::TlvTypes::Bat_49)
    {
        ADD("Ticks Before Moving", mTlv.mTimeBeforeMoving);
        ADD("Speed", mTlv.mSpeed);
        ADD("Scale", mTlv.mScale);
        ADD("Attack Duration", mTlv.mAttackDuration);

        //ADD_RESOURCE(AnimId::Bat, ReliveAPI::AddResourceTo::CameraBlock);
        //ADD_RESOURCE(AnimId::Bat_Flying, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_ShadowZone final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_ShadowZone::ShadowZoneScale>("Enum_ShadowZoneScale",
        {
            {AO::Path_ShadowZone::ShadowZoneScale::eBoth_0, "both"},
            {AO::Path_ShadowZone::ShadowZoneScale::eHalf_1, "half"},
            {AO::Path_ShadowZone::ShadowZoneScale::eFull_2, "full"},
        });
    }

    CTOR_AO(Path_ShadowZone, "ShadowZone", AO::TlvTypes::ShadowZone_7)
    {
        ADD("Centre W", mTlv.field_18_centre_w);
        ADD("Centre H", mTlv.field_1A_centre_h);
        ADD_LINKED("R", mTlv.mRed, "RGB_R_Half_FP");
        ADD_LINKED("G", mTlv.mGreen, "RGB_G_Half_FP");
        ADD_LINKED("B", mTlv.mBlue, "RGB_B_Half_FP");
        ADD("ID (Unused?)", mTlv.field_22_id);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_BellHammer final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BellHammer, "BellHammer", AO::TlvTypes::BellHammer_27)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("Scale", mTlv.mScale);
        ADD("Direction", mTlv.mDirection);

        ADD_RESOURCE(AnimId::BellHammer_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BellHammer_Smashing, ReliveAPI::AddResourceTo::CameraBlock);

        ADD_RESOURCE(AnimId::Elum_Land, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Idle, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_TimerTrigger final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_TimerTrigger, "TimerTrigger", AO::TlvTypes::TimerTrigger_94)
    {
        ADD("Source Switch ID", mTlv.mInputSwitchId);
        ADD("Trigger Interval", mTlv.mActivationDelay);
        ADD("ID 1", mTlv.mOutputSwitchId1);
        ADD("ID 2", mTlv.mOutputSwitchId2);
        ADD("ID 3", mTlv.mOutputSwitchId3);
        ADD("ID 4", mTlv.mOutputSwitchId4);
    }
};

struct Path_PullRingRope final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        
        types.AddEnum<AO::Path_PullRingRope::PullRingSwitchSound>("Enum_PullRingSwitchSound",
        {
            {AO::Path_PullRingRope::PullRingSwitchSound::eNone_0, "None"},
            {AO::Path_PullRingRope::PullRingSwitchSound::eWellExit_1, "WellExit"},
            {AO::Path_PullRingRope::PullRingSwitchSound::RingBellHammer_2, "RingBellHammer"},
            {AO::Path_PullRingRope::PullRingSwitchSound::eDoorEffect_3, "DoorEffect"},
        });

        types.AddEnum<AO::Path_PullRingRope::PullRingSoundDirection>("Enum_PullRingSoundDirection",
        {
            {AO::Path_PullRingRope::PullRingSoundDirection::eLeftAndRight_0, "Both"},
            {AO::Path_PullRingRope::PullRingSoundDirection::eLeft_1, "Left"},
            {AO::Path_PullRingRope::PullRingSoundDirection::eRight_2, "Right"},
        });
    }

    CTOR_AO(Path_PullRingRope, "PullRingRope", AO::TlvTypes::PullRingRope_18)
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

struct Path_MusicTrigger final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_MusicTrigger::TriggeredBy>("Enum_MusicTriggeredBy",
        {
            {AO::Path_MusicTrigger::TriggeredBy::eTimer_0, "Timer"},
            {AO::Path_MusicTrigger::TriggeredBy::eTouching_1, "Touching"},
            {AO::Path_MusicTrigger::TriggeredBy::eSwitchID_2, "Switch ID"},
            {AO::Path_MusicTrigger::TriggeredBy::eUnknown_3, "Unknown"},
        });

        types.AddEnum<AO::Path_MusicTrigger::MusicTriggerMusicType>("Enum_MusicTriggeredMusicType",
        {
            {AO::Path_MusicTrigger::MusicTriggerMusicType::eDrumAmbience_0, "Drum Ambience"},
            {AO::Path_MusicTrigger::MusicTriggerMusicType::eDeathDrumShort_1, "Death Drum Short"},
            {AO::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaLong_2, "Secret Area Long"},
            {AO::Path_MusicTrigger::MusicTriggerMusicType::eSoftChase_3, "Soft Chase"},
            {AO::Path_MusicTrigger::MusicTriggerMusicType::eIntenseChase_4, "Intense Chase"},
            {AO::Path_MusicTrigger::MusicTriggerMusicType::eChime_5, "Chime"},
            {AO::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort_6, "Secret Area Short"},
        });
    }

    CTOR_AO(Path_MusicTrigger, "MusicTrigger", AO::TlvTypes::MusicTrigger_105)
    {
        ADD("Music Type", mTlv.mMusicType);
        ADD("Triggered By", mTlv.mTriggeredBy);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Music Delay", mTlv.mMusicDelay);
    }
};

struct Path_ElumPathTrans final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ElumPathTrans, "ElumPathTrans", AO::TlvTypes::ElumPathTrans_99)
    {
        ADD("Level", mTlv.mNextLevel);
        ADD("Path", mTlv.mNextPath);
        ADD("Camera", mTlv.mNextCamera);
    }
};

struct Path_ElumStart final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ElumStart, "ElumStart", AO::TlvTypes::ElumStart_38)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ElumWall final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ElumWall, "ElumWall", AO::TlvTypes::ElumWall_40)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_LiftPoint final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LiftPoint::LiftPointStopType>("Enum_LiftPointStopType",
        {
            {AO::Path_LiftPoint::LiftPointStopType::eTopFloor_0, "Top Floor"},
            {AO::Path_LiftPoint::LiftPointStopType::eBottomFloor_1, "Bottom Floor"},
            {AO::Path_LiftPoint::LiftPointStopType::eMiddleFloor_2, "Middle Floor"},
            {AO::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor_3, "Middle Lock Floor"},
            {AO::Path_LiftPoint::LiftPointStopType::eStartPointOnly_4, "Start Point Only"},
        });
    }

    CTOR_AO(Path_LiftPoint, "LiftPoint", AO::TlvTypes::LiftPoint_8)
    {
        ADD("Lift Point ID", mTlv.mLiftPointId);
        ADD("Start Point", mTlv.mIsStartPoint);
        ADD("Lift Type (Unused?)", mTlv.field_1C_lift_type);
        ADD("Lift Point Stop Type", mTlv.mLiftPointStopType);
        ADD("Scale", mTlv.mScale);
        ADD("Ignore Lift Mover", mTlv.mIgnoreLiftMover);
    }
};

struct Path_MovingBomb final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovingBomb, "MovingBomb", AO::TlvTypes::MovingBomb_86)
    {
        ADD("Speed", mTlv.mSpeed);
        ADD("Switch ID", mTlv.mStartMovingSwitchId);
        ADD("Triggered By Alarm", mTlv.mTriggeredByAlarm);
        ADD("Scale", mTlv.mScale);
        ADD("Max Rise", mTlv.field_20_max_rise);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Start Speed", mTlv.mStartSpeed);
        ADD("Persist Offscreen", mTlv.mPersistOffscreen);
    }
};

struct Path_MovingBombStopper final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovingBombStopper, "MovingBombStopper", AO::TlvTypes::MovingBombStopper_87)
    {
        ADD("Min Delay", mTlv.mMinDelay);
        ADD("Max Delay", mTlv.mMaxDelay);
    }
};

struct Path_RingMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_RingMudokon::MustFaceMud>("Enum_RingMudokon_MustFaceMud",
        {
            {AO::Path_RingMudokon::MustFaceMud::eYes_0, "Yes"},
            {AO::Path_RingMudokon::MustFaceMud::eNo_1, "No"},
        });
    }

    CTOR_AO(Path_RingMudokon, "RingMudokon", AO::TlvTypes::RingMudokon_50)
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

struct Path_RingCancel final : public ReliveAPI::TlvObjectBaseAO // TODO: correct size is 24 not 28
{
    CTOR_AO(Path_RingCancel, "RingCancel", AO::TlvTypes::RingCancel_109)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_MeatSaw final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_MeatSaw::Type>("Enum_MeatSawType",
        {
            {AO::Path_MeatSaw::Type::eAutomaticPersistOffscreen_0, "Automatic Persist Offscreen"},
            {AO::Path_MeatSaw::Type::eAutomatic_1, "Automatic"},
            {AO::Path_MeatSaw::Type::eSwitchId_2, "Switch ID"},
        });

        types.AddEnum<AO::Path_MeatSaw::StartState>("Enum_MeatSawStartState",
        {
            {AO::Path_MeatSaw::StartState::eOff_0, "Off"},
            {AO::Path_MeatSaw::StartState::eOn_1, "On"},
        });
    }

    CTOR_AO(Path_MeatSaw, "MeatSaw", AO::TlvTypes::MeatSaw_88)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch Min Time Off", mTlv.mSwitchMinTimeOff);
        ADD("Switch Max Time Off", mTlv.mSwitchMaxTimeOff);
        ADD("Max Rise Time", mTlv.mMaxRiseTime);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Type", mTlv.mType);
        ADD("Speed", mTlv.mSpeed);
        ADD("Start State", mTlv.mStartState);
        ADD("Off Speed", mTlv.mOffSpeed);
        ADD("Automatic Min Time Off", mTlv.mAutomaticMinTimeOff);
        ADD("Automatic Max Time Off", mTlv.mAutomaticMaxTimeOff);
        ADD("Initial Position", mTlv.mInitialPosition);

        ADD_RESOURCE(AnimId::MeatSawMotor, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::MeatSaw_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::MeatSaw_Moving, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_LCDScreen final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_LCDScreen, "LCDScreen", AO::TlvTypes::LCDScreen_98)
    {
        ADD("Message 1 ID", mTlv.mMessageId1);
        ADD("Random Message Min ID", mTlv.mMessageRandMinId);
        ADD("Random Message Max ID", mTlv.mMessageRandMaxId);
    }
};

struct Path_InvisibleSwitch final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_InvisibleSwitch::InvisibleSwitchScale>("Enum_InvisibleSwitchScale",
        {
            {AO::Path_InvisibleSwitch::InvisibleSwitchScale::eHalf_0, "Half"},
            {AO::Path_InvisibleSwitch::InvisibleSwitchScale::eFull_1, "Full"},
            {AO::Path_InvisibleSwitch::InvisibleSwitchScale::eAny_2, "Any"},
        });
    }

    CTOR_AO(Path_InvisibleSwitch, "InvisibleSwitch", AO::TlvTypes::InvisibleSwitch_81)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Action", mTlv.mAction);
        ADD("Delay", mTlv.mActivationDelay);
        ADD("Set Off Alarm", mTlv.mSetOffAlarm);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_TrapDoor final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_TrapDoor::StartState>("Enum_TrapDoorStartState",
        {
            {AO::Path_TrapDoor::StartState::eOpen_0, "Open"},
            {AO::Path_TrapDoor::StartState::eClosed_1, "Closed"}
        });
    }

    CTOR_AO(Path_TrapDoor, "TrapDoor", AO::TlvTypes::TrapDoor_55)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Start State", mTlv.mStartState);
        ADD("Self Closing", mTlv.mSelfClosing);
        ADD("Scale", mTlv.mScale);
        ADD("Dest Level (Unused?)", mTlv.field_20_dest_level);
        ADD("Direction", mTlv.mDirection);
        ADD("X Offset", mTlv.mXOff);
    }
};

struct Path_BirdPortal final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::PortalSide>("Enum_PortalSide",
        {
            {AO::PortalSide::eRight_0, "Right"},
            {AO::PortalSide::eLeft_1, "Left"},
        });

        types.AddEnum<AO::Path_BirdPortal::PortalType>("Enum_PortalType",
        {
            {AO::Path_BirdPortal::PortalType::eAbe_0, "Abe"},
            {AO::Path_BirdPortal::PortalType::eWorker_1, "Worker"},
            {AO::Path_BirdPortal::PortalType::eShrykull_2, "Shrykull"},
            {AO::Path_BirdPortal::PortalType::eMudTeleport_3, "Mudokon Teleport"},
        });
    }

    CTOR_AO(Path_BirdPortal, "BirdPortal", AO::TlvTypes::BirdPortal_52)
    {
        ADD("Enter Side", mTlv.mEnterSide);
        ADD("Level Destination", mTlv.mExitLevel);
        ADD("Path Destination", mTlv.mExitPath);
        ADD("Camera Destination", mTlv.mExitCamera);
        ADD("Scale", mTlv.mScale);
        ADD("Movie ID", mTlv.mMovieId);
        ADD("Portal Type", mTlv.mPortalType);
        ADD("Mudokon Amount For Shrykull", mTlv.mMudCountForShrykull);
    }
};

struct Path_BoomMachine final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_BoomMachine::NozzleSide>("Enum_BoomMachineNozzleSide",
            {
                {AO::Path_BoomMachine::NozzleSide::eRight_0, "Right"},
                {AO::Path_BoomMachine::NozzleSide::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_BoomMachine, "BoomMachine", AO::TlvTypes::BoomMachine_97)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Nozzle Side", mTlv.mNozzleSide);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Number Of Grenades", mTlv.mGrenadeAmount);

        ADD_RESOURCE(AnimId::Mudokon_GrenadeMachineUse, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_HandstoneBegin, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_HandstoneEnd, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::BoomMachine_Button_Off, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Button_On, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Nozzle_DropGrenade, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Nozzle_Idle, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_Mudokon final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Mudokon::MudJobs>("Enum_MudJobs",
        {
            {AO::Path_Mudokon::MudJobs::eStandScrub_0, "Stand Scrub"},
            {AO::Path_Mudokon::MudJobs::eSitScrub_1, "Sit Scrub"},
            {AO::Path_Mudokon::MudJobs::eSitChant_2, "Sit Chant"}
        });
    }

    CTOR_AO(Path_Mudokon, "Mudokon", AO::TlvTypes::Mudokon_82)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Job", mTlv.mJob);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Voice Pitch", mTlv.mVoicePitch);
        ADD("Rescue Switch ID", mTlv.mRescueSwitchId);
        ADD("Deaf", mTlv.mDeaf);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Persist Offscreen", mTlv.mPersistAndResetOffscreen);
    }
};

struct Path_BirdPortalExit final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BirdPortalExit, "BirdPortalExit", AO::TlvTypes::BirdPortalExit_53)
    {
        ADD("Exit Direction", mTlv.mExitSide);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_Slog final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Slog, "Slog", AO::TlvTypes::Slog_25)
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
        
        ADD_RESOURCE(AnimId::Slog_AngryBark, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Dying, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ChimeLock final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ChimeLock, "ChimeLock", AO::TlvTypes::ChimeLock_69)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Solve Switch ID", mTlv.mSolveSwitchId);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("Password Switch ID", mTlv.mPasswordSwitchId);

        ADD_RESOURCE(AnimId::Chime_Ball, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BigChime, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_FlintLockFire final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_FlintLockFire, "FlintLockFire", AO::TlvTypes::FlintLockFire_73)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);

        //ADD_RESOURCE(AnimId::Fire, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::FlintLock_Hammers_Activating, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::FlintLock_Hammers_Disabled, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::FlintLock_Gourd, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_LiftMover final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LiftMover::YDirection>("Enum_LiftMoverYDirection",
        {
            {AO::Path_LiftMover::YDirection::eDown_0, "Down"},
            {AO::Path_LiftMover::YDirection::eUp_1, "Up"},
        });
    }

    CTOR_AO(Path_LiftMover, "LiftMover", AO::TlvTypes::LiftMover_68)
    {
        ADD("Lift Mover Switch ID", mTlv.mLiftMoverSwitchId);
        ADD("Target Lift Point ID", mTlv.mTargetLiftPointId);
        ADD("Move Direction", mTlv.mMoveDirection);
    }
};

struct Path_Scrab final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Scrab::ScrabPatrolType>("Enum_ScrabPatrolType",
            {
                {AO::Path_Scrab::ScrabPatrolType::eWalk_0, "Walk"},
                {AO::Path_Scrab::ScrabPatrolType::eRunOrWalk192_1, "Run Or Walk RNG 192"},
                {AO::Path_Scrab::ScrabPatrolType::eRunOrWalk128_2, "Run OR Walk RNG 128"},
                {AO::Path_Scrab::ScrabPatrolType::eRunOrWalk64_3, "Run Or Walk RNG 64"},
                {AO::Path_Scrab::ScrabPatrolType::eRun_4, "Run"},
            });
    }

    CTOR_AO(Path_Scrab, "Scrab", AO::TlvTypes::Scrab_72)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Attack Delay", mTlv.mAttackDelay);
        ADD("Patrol Type", mTlv.mPatrolType);
        ADD("Left Min Delay", mTlv.mPauseLeftMin);
        ADD("Left Max Delay", mTlv.mPauseLeftMax);
        ADD("Right Min Delay", mTlv.mPauseRightMin);
        ADD("Right Max Delay", mTlv.mPauseRightMax);
        ADD("Spotting Abe Delay", mTlv.mSpottingAbeDelay);
        ADD("Disable Resources", mTlv.mDisabledResources);
        ADD("Roar Randomly", mTlv.mRoarRandomly);

        ADD_RESOURCE(AnimId::Scrab_AttackLunge, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SlogSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_SlogSpawner::StartDirection>("Enum_SlogSpawnerStartDirection",
            {
                {AO::Path_SlogSpawner::StartDirection::eRight_0, "Right"},
                {AO::Path_SlogSpawner::StartDirection::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_SlogSpawner, "SlogSpawner", AO::TlvTypes::SlogSpawner_107)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Max Slogs", mTlv.mMaxSlogs);
        ADD("Max Slogs At A Time", mTlv.mMaxSlogsAtATime);
        ADD("Start Direction", mTlv.mStartDirection);
        ADD("Slog Spawn Interval", mTlv.mSlogSpawnInterval);
        ADD("Spawner Switch ID", mTlv.mSpawnerSwitchId);

        ADD_RESOURCE(AnimId::Slog_AngryBark, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Dying, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_RockSack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RockSack, "RockSack", AO::TlvTypes::RockSack_13)
    {
        ADD("Rock Fall Direction", mTlv.mRockFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Rock Amount", mTlv.mRockAmount);

        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::RockSack_HardHit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::RockSack_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::RockSack_SoftHit, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_ZzzSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ZzzSpawner, "ZzzSpawner", AO::TlvTypes::ZzzSpawner_111)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Zzz Delay", mTlv.mZzzInterval);
    }
};

struct Path_SecurityClaw final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityClaw, "SecurityClaw", AO::TlvTypes::SecurityClaw_61)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Alarm Switch ID", mTlv.mAlarmSwitchId);
        ADD("Alarm Duration", mTlv.mAlarmDuration);
        ADD("Disabled Resources", mTlv.mDisabledResources);

        ADD_RESOURCE(AnimId::Abe_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Body_Gib, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SecurityDoor final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityDoor, "SecurityDoor", AO::TlvTypes::SecurityDoor_95)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("X Position", mTlv.mXPos);
        ADD("Y Position", mTlv.mYPos);

        ADD_RESOURCE(AnimId::Security_Door_Idle, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_TimedMine final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_TimedMine, "TimedMine", AO::TlvTypes::TimedMine_22)
    {
        ADD("Switch ID(Unused?)", mTlv.mSwitchId);
        ADD("State", mTlv.mState);
        ADD("Scale", mTlv.mScale);
        ADD("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_SligSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligSpawner, "SligSpawner", AO::TlvTypes::SligSpawner_66)
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
        ADD("unknown", mTlv.field_2C_unused);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
        ADD("Chase Abe When Spotted", mTlv.mChaseAbeWhenSpotted);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Panic Timeout", mTlv.mPanicTimeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.field_38_num_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.field_3A_panic_sound_timeout);
        ADD("Stop Chase Delay", mTlv.mStopChaseDelay);
        ADD("Time To Wait Before Chase", mTlv.mTimeToWaitBeforeChase);
        ADD("Slig Bound/Persist ID", mTlv.mSligBoundId);
        ADD("Alerted Listen Time", mTlv.mAlertedListenTime);
        ADD("Percent Say What", mTlv.mPercentSayWhat);
        ADD("Percent Beat Mudokon", mTlv.mPercentBeatMud);
        ADD_HIDDEN("Talk To Abe (Unused?)", mTlv.field_48_talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.field_4A_dont_shoot);
        ADD("Z Shoot Delay", mTlv.mZShootDelay);
        ADD("Stay Awake", mTlv.mStayAwake);
        ADD("Disabled Resources", mTlv.mDisabledResources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.mNoiseWakeUpDistance);
        ADD("Slig Spawner Switch ID", mTlv.mSligSpawnerSwitchId);
    }
};

struct Path_MotionDetector final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_MotionDetector::InitialMoveDirection>("Enum_MotionDetectorInitialMoveDirection",
        {
            {AO::Path_MotionDetector::InitialMoveDirection::eRight_0, "right"},
            {AO::Path_MotionDetector::InitialMoveDirection::eLeft_1, "left"},
        });
    }

    CTOR_AO(Path_MotionDetector, "MotionDetector", AO::TlvTypes::MotionDetector_62)
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

struct Path_BackgroundAnimation final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<TPageAbr>("Enum_TPageAbr",
        {
            {TPageAbr::eBlend_1, "blend_1"},
            {TPageAbr::eBlend_2, "blend_2"},
            {TPageAbr::eBlend_3, "blend_3"},
            {TPageAbr::eBlend_0, "blend_0"},
        });

        types.AddEnum<AO::Path_BackgroundAnimation::BgAnimSounds>("Enum_BgAnimSounds",
        {
            {AO::Path_BackgroundAnimation::BgAnimSounds::eNone_m1, "None -1"},
            {AO::Path_BackgroundAnimation::BgAnimSounds::eNone_0, "None 0"},
            {AO::Path_BackgroundAnimation::BgAnimSounds::eFire_1, "Fire"},
        });
    }

    CTOR_AO(Path_BackgroundAnimation, "BackgroundAnimation", AO::TlvTypes::BackgroundAnimation_19)
    {
        ADD("Animation ID", mTlv.mAnimId);
        ADD("Is Semi Trans", mTlv.mIsSemiTrans);
        ADD("Semi Trans Mode", mTlv.mSemiTransMode);
        ADD("Sound Effect", mTlv.mSoundEffect);
    }
};

struct Path_LCDStatusBoard final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_LCDStatusBoard, "LCDStatusBoard", AO::TlvTypes::LCDStatusBoard_103)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_Preloader final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Preloader, "Preloader", AO::TlvTypes::Preloader_102)
    {
        ADD("Unload Cams ASAP", mTlv.unload_cams_ASAP);
    }
};

struct Path_Pulley final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Pulley, "Pulley", AO::TlvTypes::Pulley_35)
    {
        ADD("Scale", mTlv.scale);
    }
};

struct Path_SoftLanding final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SoftLanding, "SoftLanding", AO::TlvTypes::SoftLanding_114)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_MudokonPathTrans final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MudokonPathTrans, "MudokonPathTrans", AO::TlvTypes::MudokonPathTrans_89)
    {
        ADD("Level", mTlv.mNextLevel);
        ADD("Path", mTlv.mNextPath);
        ADD("Camera", mTlv.mNextCamera);
    }
};

struct Path_AbeStart final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_AbeStart, "AbeStart", AO::TlvTypes::AbeStart_37)
    {
        ADD("Scale", mTlv.scale);
    }
};

struct Path_ZSligCover final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ZSligCover, "ZSligCover", AO::TlvTypes::ZSligCover_83)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ScrabBoundLeft final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ScrabBoundLeft, "ScrabLeftBound", AO::TlvTypes::ScrabLeftBound_74)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ScrabBoundRight final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ScrabBoundRight, "ScrabRightBound", AO::TlvTypes::ScrabRightBound_75)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ScrabNoFall final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ScrabNoFall, "ScrabNoFall", AO::TlvTypes::ScrabNoFall_93)
    {
        EMPTY_CTOR_AO();
    }
};

// TODO: lift mudokon might not be the best name because this mud can also be a password giver
struct Path_LiftMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LiftMudokon::Direction>("Enum_LiftMudokonDirection",
            {
                {AO::Path_LiftMudokon::Direction::eRight_0, "Right"},
                {AO::Path_LiftMudokon::Direction::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_LiftMudokon, "LiftMudokon", AO::TlvTypes::LiftMudokon_32)
    {
        ADD("How Far To Walk", mTlv.mHowFarToWalk);
        ADD("Lift Switch ID", mTlv.mLiftSwitchId);
        ADD("Start Direction", mTlv.mFacing);
        ADD("Give Password", mTlv.mGivePassword);
        ADD("Scale", mTlv.mScale);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);
    }
};

struct Path_HoneySack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HoneySack, "HoneySack", AO::TlvTypes::HoneySack_36)
    {
        ADD("Chase Time", mTlv.mChaseTime);
        ADD("Scale", mTlv.mScale);

        ADD_RESOURCE(AnimId::HoneySack_Falling, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::HoneySack_FallingToSmashed, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::HoneySack_Hanging, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::HoneySack_OnGround, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Elum_BeesStruggling, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SlingMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SlingMudokon, "SlingMudokon", AO::TlvTypes::SlingMudokon_41)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Don't Whistle Password", mTlv.mDontWhistlePassword);
        ADD("Code 1", mTlv.mCode1);
        ADD("Code 2", mTlv.mCode2);

        ADD_RESOURCE(AnimId::Mudokon_Sling_Angry, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_AngryToIdle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_ShootEnd, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_ShootStart, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_Speak, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_BeeSwarmHole final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_BeeSwarmHole::MovementType>("Enum_BeeSwarmHoleMovementType",
        {
            {AO::Path_BeeSwarmHole::MovementType::eHover_0, "Hover"},
            {AO::Path_BeeSwarmHole::MovementType::eAttack_1, "Attack"},
            {AO::Path_BeeSwarmHole::MovementType::eFollowPath_2, "Follow Path"},
        });
    }

    CTOR_AO(Path_BeeSwarmHole, "BeeSwarmHole", AO::TlvTypes::BeeSwarmHole_34)
    {
        ADD("what_to_spawn", mTlv.field_18_what_to_spawn);
        ADD("Start Interval", mTlv.mStartInterval);
        ADD("Fall Switch ID", mTlv.field_1C_fall_switch_id);
        ADD("Movement Type", mTlv.mMovementType);
        ADD("Bees Amount", mTlv.mBeesAmount);
        ADD("Chase Time", mTlv.mChaseTime);
        ADD("Speed", mTlv.mSpeed);
        ADD("Scale (Unused?)", mTlv.field_26_scale);
    }
};

struct Path_MeatSack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MeatSack, "MeatSack", AO::TlvTypes::MeatSack_71)
    {
        ADD("Meat Fall Direction", mTlv.mMeatFallDirection);
        ADD("X Velocity", mTlv.mVelX);
        ADD("Y Velocity", mTlv.mVelY);
        ADD("Scale", mTlv.mScale);
        ADD("Amount Of Meat", mTlv.mMeatAmount);

        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::MeatSack_Hit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::MeatSack_Idle, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_RollingBall final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RollingBall, "RollingBall", AO::TlvTypes::RollingBall_56)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Roll Direction", mTlv.mRollDirection);
        ADD("Release Switch ID", mTlv.mReleaseSwitchId);
        ADD("Speed", mTlv.mMaxSpeed);
        ADD("Acceleration", mTlv.mAcceleration);
    }
};

struct Path_RollingBallStopper final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RollingBallStopper, "RollingBallStopper", AO::TlvTypes::RollingBallStopper_59)
    {
        ADD("Stopper Switch ID", mTlv.mStopperSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Ball Switch ID", mTlv.mBallSwitchId);
        ADD("Direction", mTlv.mStopDirection);
    }
};

struct Path_BeeNest final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BeeNest, "BeeNest", AO::TlvTypes::BeeNest_43)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Swarm Size", mTlv.mSwarmSize);
        ADD("Chase Time", mTlv.mChaseTime);
        ADD("Speed", mTlv.mSpeed);
        ADD("Disable Resources", mTlv.mDisabledResources);
        ADD("Amount Of Bees", mTlv.mBeesAmount);

        ADD_RESOURCE(AnimId::Elum_BeesStruggling, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ZBall final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_ZBall::StartPos>("Enum_ZBallStartPos",
        {
            {AO::Path_ZBall::StartPos::eCenter_0, "Center"},
            {AO::Path_ZBall::StartPos::eOut_1, "Out"},
            {AO::Path_ZBall::StartPos::eIn_2, "In"},
        });

        types.AddEnum<AO::Path_ZBall::Speed>("Enum_ZBallSpeed",
        {
            {AO::Path_ZBall::Speed::eNormal_0, "Normal"},
            {AO::Path_ZBall::Speed::eFast_1, "Fast"},
            {AO::Path_ZBall::Speed::eSlow_2, "Slow"},
        });
    }

    CTOR_AO(Path_ZBall, "ZBall", AO::TlvTypes::ZBall_14)
    {
        ADD("Start Position", mTlv.mStartPos);
        ADD("Scale", mTlv.mScale);
        ADD("Speed", mTlv.mSpeed);

        ADD_RESOURCE(AnimId::Swinging_Ball_Fast, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Swinging_Ball_Normal, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Swinging_Ball_Slow, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_ZShot, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_ZShotRolling, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_FootSwitch final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_FootSwitch::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
        {
            {AO::Path_FootSwitch::FootSwitchTriggerBy::eAbe_0, "Abe"},
            {AO::Path_FootSwitch::FootSwitchTriggerBy::eAnyone_1, "Anyone"},
        });
    }

    CTOR_AO(Path_FootSwitch, "FootSwitch", AO::TlvTypes::FootSwitch_60)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("Scale", mTlv.mScale);
        ADD("Action", mTlv.mAction);
        ADD("Triggered By", mTlv.mTriggeredBy);
    }
};

struct Path_Paramite final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Paramite, "Paramite", AO::TlvTypes::Paramite_48)
    {
        ADD("Scale", mTlv.mScale);
        ADD("Enter From Web", mTlv.mEnterFromWeb);
        ADD("Alone - Chase Delay", mTlv.mAloneChaseDelay);
        ADD("Surprise Web Delay", mTlv.mSurpriseWebDelayTimer);
        ADD("Meat Eating Time", mTlv.mMeatEatingTime);
        ADD("Group - Chase Delay", mTlv.mGroupChaseDelay);
        ADD("Disabled Resources", mTlv.field_24_disabled_resources);
        ADD("Surprise Web Switch ID", mTlv.mSurpriseWebSwitchId);
        ADD("Hiss Before Attack", mTlv.mHissBeforeAttack);
        ADD("Delete When Far Away", mTlv.mDeleteWhenOutOfSight);

        ADD_RESOURCE(AnimId::Paramite_Death, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_Honey final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Honey, "Honey", AO::TlvTypes::Honey_20)
    {
        ADD("Switch ID", mTlv.mSwitchId);
        ADD("State", mTlv.mState);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_HoneyDripTarget final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HoneyDripTarget, "HoneyDripTarget", AO::TlvTypes::HoneyDripTarget_42)
    {
        EMPTY_CTOR_AO();
    }
   
};
} // namespace AOTlvs

#undef EMPTY_CTOR_AO
#undef CTOR_AO
