#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "AETlvs.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../AliveLibAE/SwitchStates.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/Map.hpp"

void TypesCollection::AddAETypes()
{
    #define REGISTER_TYPE_AE(TlvWrapperType) mTlvFactoryAE.DoRegisterType<TlvWrapperType>(*this)

    REGISTER_TYPE_AE(AETlvs::Path_Hoist);
    REGISTER_TYPE_AE(AETlvs::Path_Switch);
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
    REGISTER_TYPE_AE(AETlvs::Path_FartMachine);
    REGISTER_TYPE_AE(AETlvs::Path_Grinder);
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
    REGISTER_TYPE_AE(AETlvs::Path_NakedSlig);
    REGISTER_TYPE_AE(AETlvs::Path_SligGetPants);
    REGISTER_TYPE_AE(AETlvs::Path_SligGetWings);
    REGISTER_TYPE_AE(AETlvs::Path_NakedSligButton);
    REGISTER_TYPE_AE(AETlvs::Path_Glukkon);
    REGISTER_TYPE_AE(AETlvs::Path_GlukkonSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_GasCountDown);
    REGISTER_TYPE_AE(AETlvs::Path_FallingItem);
    REGISTER_TYPE_AE(AETlvs::Path_BoneBag);
    REGISTER_TYPE_AE(AETlvs::Path_SecurityClaw);
    REGISTER_TYPE_AE(AETlvs::Path_FootSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_SlogHut);
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

    AddEnum<CameraSwapEffects>("Enum_CameraSwapEffects",
        {
             { CameraSwapEffects::eEffect0_InstantChange, "Instant Change" },
             { CameraSwapEffects::eEffect1_LeftToRight, "Left To Right" },
             { CameraSwapEffects::eEffect2_RightToLeft, "Right To Left" },
             { CameraSwapEffects::eEffect3_TopToBottom, "Top To Bottom" },
             { CameraSwapEffects::eEffect4_BottomToTop, "Bottom To Top" },
             { CameraSwapEffects::eEffect5_1_FMV, "1 FMV" },
             { CameraSwapEffects::eEffect6_VerticalSplit, "Left To Vertical Split" },
             { CameraSwapEffects::eEffect7_HorizontalSplit, "Horizontal Split" },
             { CameraSwapEffects::eEffect8_BoxOut, "Box Out" },
             { CameraSwapEffects::eEffect9_2_FMV, "2 FMV" },
             { CameraSwapEffects::eEffect10_3_FMV, "3 FMV" },
             { CameraSwapEffects::eEffect11_Unknown, "Unknown" },
        });

    AddEnum<Choice_short>("Enum_Choice_short",
        {
             { Choice_short::eNo_0, "No" },
             { Choice_short::eYes_1, "Yes" },
        });

    AddEnum<XDirection_short>("Enum_XDirection_short",
        {
             { XDirection_short::eLeft_0, "Left" },
             { XDirection_short::eRight_1, "Right" },
        });

    AddEnum<Scale_short>("Enum_Scale_short",
        {
             { Scale_short::eFull_0, "Full" },
             { Scale_short::eHalf_1, "Half" },
        });

    AddEnum<Scale_int>("Enum_Scale_int",
        {
             { Scale_int::eFull_0, "Full" },
             { Scale_int::eHalf_1, "Half" },
        });

    AddEnum<SwitchOp>("Enum_SwitchOp",
        {
             { SwitchOp::eSetTrue_0, "SetTrue" },
             { SwitchOp::eSetFalse_1, "SetFalse" },
             { SwitchOp::eToggle_2, "Toggle" },
             { SwitchOp::eIncrement_3, "Increment" },
             { SwitchOp::eDecrement_4, "Decrement" },
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
