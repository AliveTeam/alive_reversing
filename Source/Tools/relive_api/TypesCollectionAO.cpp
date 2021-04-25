#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "AOTlvs.hpp"
#include "../AliveLibAO/SwitchStates.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAE/Path.hpp"

void TypesCollection::AddAOTypes()
{
    #define REGISTER_TYPE_AO(TlvWrapperType) mTlvFactoryAO.DoRegisterType<TlvWrapperType>(*this)

    REGISTER_TYPE_AO(AOTlvs::Path_Hoist);
    REGISTER_TYPE_AO(AOTlvs::Path_ContinuePoint);
    REGISTER_TYPE_AO(AOTlvs::Path_Door);
    REGISTER_TYPE_AO(AOTlvs::Path_Change);
    REGISTER_TYPE_AO(AOTlvs::Path_Switch);
    REGISTER_TYPE_AO(AOTlvs::Path_LightEffect);
    REGISTER_TYPE_AO(AOTlvs::Path_ElectricWall);
    REGISTER_TYPE_AO(AOTlvs::Path_ContinueZone);
    REGISTER_TYPE_AO(AOTlvs::Path_StartController);
    REGISTER_TYPE_AO(AOTlvs::Path_Edge);
    REGISTER_TYPE_AO(AOTlvs::Path_WellLocal);
    REGISTER_TYPE_AO(AOTlvs::Path_WellExpress);
    REGISTER_TYPE_AO(AOTlvs::Path_InvisibleZone);
    REGISTER_TYPE_AO(AOTlvs::Path_EnemyStopper);
    REGISTER_TYPE_AO(AOTlvs::Path_Slig);
    REGISTER_TYPE_AO(AOTlvs::Path_DeathDrop);
    REGISTER_TYPE_AO(AOTlvs::Path_SligLeftBound);
    REGISTER_TYPE_AO(AOTlvs::Path_SligRightBound);
    REGISTER_TYPE_AO(AOTlvs::Path_SligPersist);
    REGISTER_TYPE_AO(AOTlvs::Path_SecurityOrb);
    REGISTER_TYPE_AO(AOTlvs::Path_FallingItem);
    REGISTER_TYPE_AO(AOTlvs::Path_Mine);
    REGISTER_TYPE_AO(AOTlvs::Path_Dove);
    REGISTER_TYPE_AO(AOTlvs::Path_UXB);
    REGISTER_TYPE_AO(AOTlvs::Path_HintFly);
    REGISTER_TYPE_AO(AOTlvs::Path_Bat);
    REGISTER_TYPE_AO(AOTlvs::Path_ShadowZone);
    REGISTER_TYPE_AO(AOTlvs::Path_BellHammer);
    REGISTER_TYPE_AO(AOTlvs::Path_IdSplitter);
    REGISTER_TYPE_AO(AOTlvs::Path_PullRingRope);
    REGISTER_TYPE_AO(AOTlvs::Path_MusicTrigger);
    REGISTER_TYPE_AO(AOTlvs::Path_ElumPathTrans);
    REGISTER_TYPE_AO(AOTlvs::Path_ElumStart);
    REGISTER_TYPE_AO(AOTlvs::Path_ElumWall);
    REGISTER_TYPE_AO(AOTlvs::Path_LiftPoint);
    REGISTER_TYPE_AO(AOTlvs::Path_MovingBomb);
    REGISTER_TYPE_AO(AOTlvs::Path_MovingBombStopper);
    REGISTER_TYPE_AO(AOTlvs::Path_RingMudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_RingCancel);
    REGISTER_TYPE_AO(AOTlvs::Path_MeatSaw);
    REGISTER_TYPE_AO(AOTlvs::Path_LCDScreen);
    REGISTER_TYPE_AO(AOTlvs::Path_InvisibleSwitch);
    REGISTER_TYPE_AO(AOTlvs::Path_TrapDoor);
    REGISTER_TYPE_AO(AOTlvs::Path_BirdPortal);
    REGISTER_TYPE_AO(AOTlvs::Path_BoomMachine);
    REGISTER_TYPE_AO(AOTlvs::Path_Mudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_BirdPortalExit);
    REGISTER_TYPE_AO(AOTlvs::Path_Slog);
    REGISTER_TYPE_AO(AOTlvs::Path_ChimeLock);
    REGISTER_TYPE_AO(AOTlvs::Path_FlintLockFire);
    REGISTER_TYPE_AO(AOTlvs::Path_LiftMover);
    REGISTER_TYPE_AO(AOTlvs::Path_Scrab);
    REGISTER_TYPE_AO(AOTlvs::Path_SlogSpawner);
    REGISTER_TYPE_AO(AOTlvs::Path_RockSack);
    REGISTER_TYPE_AO(AOTlvs::Path_SlogHut);
    REGISTER_TYPE_AO(AOTlvs::Path_SecurityClaw);
    REGISTER_TYPE_AO(AOTlvs::Path_SecurityDoor);
    REGISTER_TYPE_AO(AOTlvs::Path_TimedMine);
    REGISTER_TYPE_AO(AOTlvs::Path_SligSpawner);
    REGISTER_TYPE_AO(AOTlvs::Path_MotionDetector);
    REGISTER_TYPE_AO(AOTlvs::Path_BackgroundAnimation);
    REGISTER_TYPE_AO(AOTlvs::Path_LCDStatusBoard);
    REGISTER_TYPE_AO(AOTlvs::Path_Preloader);
    REGISTER_TYPE_AO(AOTlvs::Path_Pulley);
    REGISTER_TYPE_AO(AOTlvs::Path_SoftLanding);
    REGISTER_TYPE_AO(AOTlvs::Path_MudokonPathTrans);
    REGISTER_TYPE_AO(AOTlvs::Path_AbeStart);
    REGISTER_TYPE_AO(AOTlvs::Path_ZSligCover);
    REGISTER_TYPE_AO(AOTlvs::Path_ScrabLeftBound);
    REGISTER_TYPE_AO(AOTlvs::Path_ScrabRightBound);
    REGISTER_TYPE_AO(AOTlvs::Path_ScrabNoFall);
    REGISTER_TYPE_AO(AOTlvs::Path_LiftMudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_HoneySack);
    REGISTER_TYPE_AO(AOTlvs::Path_SlingMudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_BeeSwarmHole);
    REGISTER_TYPE_AO(AOTlvs::Path_MeatSack);
    REGISTER_TYPE_AO(AOTlvs::Path_RollingBall);
    REGISTER_TYPE_AO(AOTlvs::Path_RollingBallStopper);
    REGISTER_TYPE_AO(AOTlvs::Path_Bees);
    REGISTER_TYPE_AO(AOTlvs::Path_ZBall);
    REGISTER_TYPE_AO(AOTlvs::Path_FootSwitch);
    REGISTER_TYPE_AO(AOTlvs::Path_Paramite);
    REGISTER_TYPE_AO(AOTlvs::Path_Honey);
    REGISTER_TYPE_AO(AOTlvs::Path_HoneyDripTarget);
    REGISTER_TYPE_AO(AOTlvs::Path_HandStone);
    REGISTER_TYPE_AO(AOTlvs::Path_BellsongStone);
    REGISTER_TYPE_AO(AOTlvs::Path_MovieStone);

    #undef REGISTER_TYPE_AO

    AddEnum<AO::SwitchOp>("Enum_SwitchOp",
        {
             { AO::SwitchOp::eSetTrue_0, "SetTrue" },
             { AO::SwitchOp::eSetFalse_1, "SetFalse" },
             { AO::SwitchOp::eToggle_2, "Toggle" },
             { AO::SwitchOp::eIncrement_3, "Increment" },
             { AO::SwitchOp::eDecrement_4, "Decrement" },
        });

    AddEnum<AO::LevelIds>("Enum_LevelIds",
        {
            {AO::LevelIds::eNone, "None"},
            {AO::LevelIds::eMenu_0, "Menu"},
            {AO::LevelIds::eRuptureFarms_1, "RuptureFarms"},
            {AO::LevelIds::eLines_2, "Lines"},
            {AO::LevelIds::eForest_3, "Forest"},
            {AO::LevelIds::eForestTemple_4, "ForestTemple"},
            {AO::LevelIds::eStockYards_5, "StockYards"},
            {AO::LevelIds::eStockYardsReturn_6, "StockYardsReturn"},
            {AO::LevelIds::eRemoved_7, "Removed7"},
            {AO::LevelIds::eDesert_8, "Desert"},
            {AO::LevelIds::eDesertTemple_9, "DesertTemple"},
            {AO::LevelIds::eCredits_10, "Credits"},
            {AO::LevelIds::eRemoved_11, "Removed11"},
            {AO::LevelIds::eBoardRoom_12, "BoardRoom"},
            {AO::LevelIds::eRuptureFarmsReturn_13, "RuptureFarmsReturn"},
            {AO::LevelIds::eForestChase, "ForestChase"},
            {AO::LevelIds::eDesertEscape, "DesertEscape"},
        });

    AddEnum<Choice_short>("Enum_Choice_short",
        {
             { Choice_short::eNo_0, "No" },
             { Choice_short::eYes_1, "Yes" },
        });

    AddEnum<Scale_short>("Enum_Scale_short",
        {
             { Scale_short::eFull_0, "Full" },
             { Scale_short::eHalf_1, "Half" },
        });

    AddEnum<XDirection_short>("Enum_XDirection_short",
        {
             { XDirection_short::eLeft_0, "Left" },
             { XDirection_short::eRight_1, "Right" },
        });
}


