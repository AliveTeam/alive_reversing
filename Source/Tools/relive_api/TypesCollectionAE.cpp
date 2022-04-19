#include "TypesCollectionAE.hpp"

#include "TlvsAE.hpp"
#include "TlvObjectBase.hpp"

#include "../../AliveLibAE/PathData.hpp"
#include "../../AliveLibAE/SwitchStates.hpp"
#include "../../AliveLibAE/Path.hpp"
#include "../../AliveLibAE/Map.hpp"

#include "../../AliveLibCommon/stdafx_common.h"

namespace ReliveAPI {
TypesCollectionAE::TypesCollectionAE()
{
    AddAETypes();
}

void TypesCollectionAE::AddTlvsToJsonArray(jsonxx::Array& array)
{
    mTlvFactoryAE.AddTlvsToJsonArray(*this, array);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionAE::MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv, s32 instanceCount)
{
    return mTlvFactoryAE.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionAE::MakeTlvAE(const std::string& tlvTypeName, Path_TLV* pTlv)
{
    return mTlvFactoryAE.MakeTlvByName(*this, tlvTypeName, pTlv);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionAE::MakeTlvFromString(const std::string& tlvTypeName)
{
    return mTlvFactoryAE.MakeTlvByName(*this, tlvTypeName, nullptr);
}

void TypesCollectionAE::AddAETypes()
{
#define REGISTER_TYPE_AE(TlvWrapperType) mTlvFactoryAE.DoRegisterType<TlvWrapperType>(*this)

    REGISTER_TYPE_AE(AETlvs::Path_Hoist);
    REGISTER_TYPE_AE(AETlvs::Path_Lever);
    REGISTER_TYPE_AE(AETlvs::Path_Door);
    REGISTER_TYPE_AE(AETlvs::Path_LCDStatusBoard);
    REGISTER_TYPE_AE(AETlvs::Path_BirdPortal);
    REGISTER_TYPE_AE(AETlvs::Path_Mudokon);
    REGISTER_TYPE_AE(AETlvs::Path_ElectricWall);
    REGISTER_TYPE_AE(AETlvs::Path_BoomMachine);
    REGISTER_TYPE_AE(AETlvs::Path_Slig);
    REGISTER_TYPE_AE(AETlvs::Path_Fleech);
    REGISTER_TYPE_AE(AETlvs::Path_EnemyStopper);
    REGISTER_TYPE_AE(AETlvs::Path_Teleporter);
    REGISTER_TYPE_AE(AETlvs::Path_UXB);
    REGISTER_TYPE_AE(AETlvs::Path_LCDScreen);
    REGISTER_TYPE_AE(AETlvs::Path_Edge);
    REGISTER_TYPE_AE(AETlvs::Path_StatusLight);
    REGISTER_TYPE_AE(AETlvs::Path_ShadowZone);
    REGISTER_TYPE_AE(AETlvs::Path_WorkWheel);
    REGISTER_TYPE_AE(AETlvs::Path_MusicTrigger);
    REGISTER_TYPE_AE(AETlvs::Path_AbeStart);
    REGISTER_TYPE_AE(AETlvs::Path_SoftLanding);
    REGISTER_TYPE_AE(AETlvs::Path_WellExpress);
    REGISTER_TYPE_AE(AETlvs::Path_SlamDoor);
    REGISTER_TYPE_AE(AETlvs::Path_HandStone);
    REGISTER_TYPE_AE(AETlvs::Path_LaughingGas);
    REGISTER_TYPE_AE(AETlvs::Path_InvisibleSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_Water);
    REGISTER_TYPE_AE(AETlvs::Path_GasEmitter);
    REGISTER_TYPE_AE(AETlvs::Path_BackgroundAnimation);
    REGISTER_TYPE_AE(AETlvs::Path_LiftPoint);
    REGISTER_TYPE_AE(AETlvs::Path_PullRingRope);
    REGISTER_TYPE_AE(AETlvs::Path_MultiSwitchController);
    REGISTER_TYPE_AE(AETlvs::Path_SecurityOrb);
    REGISTER_TYPE_AE(AETlvs::Path_InvisibleZone);
    REGISTER_TYPE_AE(AETlvs::Path_ContinuePoint);
    REGISTER_TYPE_AE(AETlvs::Path_WheelSyncer);
    REGISTER_TYPE_AE(AETlvs::Path_LevelLoader);
    REGISTER_TYPE_AE(AETlvs::Path_Pulley);
    REGISTER_TYPE_AE(AETlvs::Path_FlyingSlig);
    REGISTER_TYPE_AE(AETlvs::Path_FlyingSligSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_DeathDrop);
    REGISTER_TYPE_AE(AETlvs::Path_SligSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_SligLeftBound);
    REGISTER_TYPE_AE(AETlvs::Path_SligRightBound);
    REGISTER_TYPE_AE(AETlvs::Path_SligPersist);
    REGISTER_TYPE_AE(AETlvs::Path_ZSligCover);
    REGISTER_TYPE_AE(AETlvs::Path_WellLocal);
    REGISTER_TYPE_AE(AETlvs::Path_BrewMachine);
    REGISTER_TYPE_AE(AETlvs::Path_Drill);
    REGISTER_TYPE_AE(AETlvs::Path_Mine);
    REGISTER_TYPE_AE(AETlvs::Path_Slog);
    REGISTER_TYPE_AE(AETlvs::Path_ResetSwitchRange);
    REGISTER_TYPE_AE(AETlvs::Path_TrapDoor);
    REGISTER_TYPE_AE(AETlvs::Path_PathTransition);
    REGISTER_TYPE_AE(AETlvs::Path_LiftMover);
    REGISTER_TYPE_AE(AETlvs::Path_RockSack);
    REGISTER_TYPE_AE(AETlvs::Path_TimerTrigger);
    REGISTER_TYPE_AE(AETlvs::Path_MotionDetector);
    REGISTER_TYPE_AE(AETlvs::Path_MineCar);
    REGISTER_TYPE_AE(AETlvs::Path_ExplosionSet);
    REGISTER_TYPE_AE(AETlvs::Path_ColourfulMeter);
    REGISTER_TYPE_AE(AETlvs::Path_Alarm);
    REGISTER_TYPE_AE(AETlvs::Path_DemoSpawnPoint);
    REGISTER_TYPE_AE(AETlvs::Path_SlapLock);
    REGISTER_TYPE_AE(AETlvs::Path_Slurg);
    REGISTER_TYPE_AE(AETlvs::Path_DoorBlocker);
    REGISTER_TYPE_AE(AETlvs::Path_Dove);
    REGISTER_TYPE_AE(AETlvs::Path_BirdPortalExit);
    REGISTER_TYPE_AE(AETlvs::Path_DoorFlame);
    REGISTER_TYPE_AE(AETlvs::Path_TrainDoor);
    REGISTER_TYPE_AE(AETlvs::Path_Greeter);
    REGISTER_TYPE_AE(AETlvs::Path_ScrabLeftBound);
    REGISTER_TYPE_AE(AETlvs::Path_ScrabRightBound);
    REGISTER_TYPE_AE(AETlvs::Path_CreditsController);
    REGISTER_TYPE_AE(AETlvs::Path_MovieHandstone);
    REGISTER_TYPE_AE(AETlvs::Path_MovingBomb);
    REGISTER_TYPE_AE(AETlvs::Path_SecurityDoor);
    REGISTER_TYPE_AE(AETlvs::Path_CrawlingSlig);
    REGISTER_TYPE_AE(AETlvs::Path_SligGetPants);
    REGISTER_TYPE_AE(AETlvs::Path_SligGetWings);
    REGISTER_TYPE_AE(AETlvs::Path_CrawlingSligButton);
    REGISTER_TYPE_AE(AETlvs::Path_Glukkon);
    REGISTER_TYPE_AE(AETlvs::Path_GlukkonSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_GasCountDown);
    REGISTER_TYPE_AE(AETlvs::Path_FallingItem);
    REGISTER_TYPE_AE(AETlvs::Path_BoneBag);
    REGISTER_TYPE_AE(AETlvs::Path_SecurityClaw);
    REGISTER_TYPE_AE(AETlvs::Path_FootSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_ZzzSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_SlogSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_MainMenuController);
    REGISTER_TYPE_AE(AETlvs::Path_Null_63);
    REGISTER_TYPE_AE(AETlvs::Path_Scrab);
    REGISTER_TYPE_AE(AETlvs::Path_ScrabSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_SlurgSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_Paramite);
    REGISTER_TYPE_AE(AETlvs::Path_ParamiteWebLine);
    REGISTER_TYPE_AE(AETlvs::Path_MeatSack);
    REGISTER_TYPE_AE(AETlvs::Path_TorturedMudokon);

#undef REGISTER_TYPE_AE

    AddEnum<::Path_FlyingSlig_Data::SpawnDelayStates>("Enum_FlyingSligSpawnDelayStates",
    {
        {::Path_FlyingSlig_Data::SpawnDelayStates::eMoveImmediately_0, "Move Immediately"},
        {::Path_FlyingSlig_Data::SpawnDelayStates::eUseCustomSpawnMoveDelay_1, "Use Custom Spawn Move Delay"},
    });

    AddEnum<::Path_Slig::StartState>("Enum_SligStartState",
    {
        {::Path_Slig::StartState::Listening_0, "Listening"},
        {::Path_Slig::StartState::Patrol_1, "Patrol"},
        {::Path_Slig::StartState::Sleeping_2, "Sleeping"},
        {::Path_Slig::StartState::Chase_3, "Chase"},
        {::Path_Slig::StartState::ChaseAndDisappear_4, "Chase And Disappear"},
        {::Path_Slig::StartState::Unused_5, "Unused (crashes the game)"},
        {::Path_Slig::StartState::ListeningToGlukkon_6, "Listening To Glukkon"},
    });

    AddEnum<::Path_Slig::ShootPossessedSligs>("Enum_ShootPossessedSligs",
    {
        {::Path_Slig::ShootPossessedSligs::eYes_0, "Yes 0"},
        {::Path_Slig::ShootPossessedSligs::eYes_1, "Yes 1"},
        {::Path_Slig::ShootPossessedSligs::eNo_3, "No"},
    });

    AddEnum<eLineTypes>("Enum_LineTypes",
    {
        {eLineTypes::eFloor_0, "Floor"},
        {eLineTypes::eWallLeft_1, "Wall Left"},
        {eLineTypes::eWallRight_2, "Wall Right"},
        {eLineTypes::eCeiling_3, "Ceiling"},
        {eLineTypes::eBackgroundFloor_4, "Background Floor"},
        {eLineTypes::eBackgroundWallLeft_5, "Background Wall Left"},
        {eLineTypes::eBackgroundWallRight_6, "Background Wall Right"},
        {eLineTypes::eBackgroundCeiling_7, "Background Ceiling"},
        {eLineTypes::eTrackLine_8, "Track Line"},
        {eLineTypes::eArt_9, "Art"},
        {eLineTypes::eBulletWall_10, "Bullet Wall"},
        {eLineTypes::eMineCarFloor_11, "Mine Car Floor"},
        {eLineTypes::eMineCarWall_12, "Mine Car Wall"},
        {eLineTypes::eMineCarCeiling_13, "Mine Car Ceiling"},
        {eLineTypes::eFlyingSligCeiling_17, "Flying Slig Ceiling"},
        {eLineTypes::eUnknown_32, "Unknown 32"},
        {eLineTypes::eUnknown_36, "Unknown 36"},
    });

    AddEnum<ScreenChangeEffects>("Enum_ScreenChangeEffects",
    {
        {ScreenChangeEffects::ePlay1FMV_0, "Play 1 FMV"},
        {ScreenChangeEffects::eRightToLeft_1, "Right To Left"},
        {ScreenChangeEffects::eLeftToRight_2, "Left To Right"},
        {ScreenChangeEffects::eBottomToTop_3, "Bottom To Top"},
        {ScreenChangeEffects::eTopToBottom_4, "Top To Bottom"},
        {ScreenChangeEffects::eBoxOut_5, "Box Out"},
        {ScreenChangeEffects::eVerticalSplit_6, "Left To Vertical Split"},
        {ScreenChangeEffects::eHorizontalSplit_7, "Horizontal Split"},
        {ScreenChangeEffects::eUnknown_8, "Unknown"},
        {ScreenChangeEffects::eInstantChange_9, "Instant Change"}
    });

    AddEnum<CameraSwapEffects>("Enum_CameraSwapEffects",
    {
        {CameraSwapEffects::eInstantChange_0, "Instant Change"},
        {CameraSwapEffects::eLeftToRight_1, "Left To Right"},
        {CameraSwapEffects::eRightToLeft_2, "Right To Left"},
        {CameraSwapEffects::eTopToBottom_3, "Top To Bottom"},
        {CameraSwapEffects::eBottomToTop_4, "Bottom To Top"},
        {CameraSwapEffects::ePlay1FMV_5, "Play 1 FMV"},
        {CameraSwapEffects::eVerticalSplit_6, "Left To Vertical Split"},
        {CameraSwapEffects::eHorizontalSplit_7, "Horizontal Split"},
        {CameraSwapEffects::eBoxOut_8, "Box Out"},
        {CameraSwapEffects::ePlay2FMVs_9, "Play 2 FMV's"},
        {CameraSwapEffects::ePlay3FMVs_10, "Play 3 FMV's"},
        {CameraSwapEffects::eUnknown_11, "Unknown"},
    });

    AddEnum<Choice_int>("Enum_Choice_int",
    {
        {Choice_int::eNo_0, "No"},
        {Choice_int::eYes_1, "Yes"},
    });

    AddEnum<Choice_short>("Enum_Choice_short",
    {
        {Choice_short::eNo_0, "No"},
        {Choice_short::eYes_1, "Yes"},
    });

    AddEnum<XDirection_int>("Enum_XDirection_int",
    {
        {XDirection_int::eLeft_0, "Left"},
        {XDirection_int::eRight_1, "Right"},
    });

    AddEnum<XDirection_short>("Enum_XDirection_short",
    {
        {XDirection_short::eLeft_0, "Left"},
        {XDirection_short::eRight_1, "Right"},
    });

    AddEnum<Scale_short>("Enum_Scale_short",
    {
        {Scale_short::eFull_0, "Full"},
        {Scale_short::eHalf_1, "Half"},
    });

    AddEnum<Scale_int>("Enum_Scale_int",
    {
        {Scale_int::eFull_0, "Full"},
        {Scale_int::eHalf_1, "Half"},
    });

    AddEnum<SwitchOp>("Enum_SwitchOp",
    {
        {SwitchOp::eSetTrue_0, "SetTrue"},
        {SwitchOp::eSetFalse_1, "SetFalse"},
        {SwitchOp::eToggle_2, "Toggle"},
        {SwitchOp::eIncrement_3, "Increment"},
        {SwitchOp::eDecrement_4, "Decrement"},
    });

    AddEnum<LevelIds>("Enum_LevelIds",
    {
        {LevelIds::eNone, "None"},
        {LevelIds::eMenu_0, "Menu"},
        {LevelIds::eMines_1, "Mines"},
        {LevelIds::eNecrum_2, "Necrum"},
        {LevelIds::eMudomoVault_3, "MudomoVault"},
        {LevelIds::eMudancheeVault_4, "MudancheeVault"},
        {LevelIds::eFeeCoDepot_5, "FeeCoDepot"},
        {LevelIds::eBarracks_6, "Barracks"},
        {LevelIds::eMudancheeVault_Ender_7, "MudancheeVault_Ender"},
        {LevelIds::eBonewerkz_8, "Bonewerkz"},
        {LevelIds::eBrewery_9, "Brewery"},
        {LevelIds::eBrewery_Ender_10, "Brewery_Ender"},
        {LevelIds::eMudomoVault_Ender_11, "MudomoVault_Ender"},
        {LevelIds::eFeeCoDepot_Ender_12, "FeeCoDepot_Ender"},
        {LevelIds::eBarracks_Ender_13, "Barracks_Ender"},
        {LevelIds::eBonewerkz_Ender_14, "Bonewerkz_Ender"},
        {LevelIds::eNotUsed_15, "NotUsed"},
        {LevelIds::eCredits_16, "Credits"},
    });
}
} // namespace ReliveAPI
