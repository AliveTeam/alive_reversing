#include "TypesCollectionRelive.hpp"

#include "TlvsRelive.hpp"
#include "TlvObjectBase.hpp"

#include "../../AliveLibAE/PathData.hpp"
#include "../../relive_lib/SwitchStates.hpp"
#include "../../AliveLibAE/Path.hpp"
#include "../../AliveLibAE/Map.hpp"

#include "../../relive_lib/stdafx.h"

namespace ReliveAPI {
TypesCollectionRelive::TypesCollectionRelive()
{
    AddReliveTypes();
}

void TypesCollectionRelive::AddTlvsToJsonArray(nlohmann::json& array)
{
    mTlvFactoryRelive.AddTlvsToJsonArray(*this, array);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvRelive(ReliveTypes tlvType, relive::Path_TLV* pTlv, s32 instanceCount)
{
    return mTlvFactoryRelive.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvRelive(const std::string& tlvTypeName, relive::Path_TLV* pTlv)
{
    return mTlvFactoryRelive.MakeTlvByName(*this, tlvTypeName, pTlv);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvFromString(const std::string& tlvTypeName)
{
    return mTlvFactoryRelive.MakeTlvByName(*this, tlvTypeName, nullptr);
}

void TypesCollectionRelive::AddReliveTypes()
{
#define REGISTER_TYPE_RELIVE(TlvWrapperType) mTlvFactoryRelive.DoRegisterType<TlvWrapperType>(*this)
    //REGISTER_TYPE_RELIVE(relive::Editor_TimedMine);
    /* REGISTER_TYPE_RELIVE(relive::Path_ElectricWall);
    REGISTER_TYPE_RELIVE(relive::Path_Mudokon);
    REGISTER_TYPE_RELIVE(relive::Path_BirdPortal);
    REGISTER_TYPE_RELIVE(relive::Path_LCDStatusBoard);
    REGISTER_TYPE_RELIVE(relive::Path_Door);
    REGISTER_TYPE_RELIVE(relive::Path_Lever);
    REGISTER_TYPE_RELIVE(relive::Path_Hoist);
    REGISTER_TYPE_RELIVE(relive::Path_BoomMachine);
    REGISTER_TYPE_RELIVE(relive::Path_Slig);
    REGISTER_TYPE_RELIVE(relive::Path_Fleech);
    REGISTER_TYPE_RELIVE(relive::Path_EnemyStopper);
    REGISTER_TYPE_RELIVE(relive::Path_Teleporter);
    REGISTER_TYPE_RELIVE(relive::Path_UXB);
    REGISTER_TYPE_RELIVE(relive::Path_LCDScreen);
    REGISTER_TYPE_RELIVE(relive::Path_Edge);
    REGISTER_TYPE_RELIVE(relive::Path_StatusLight);
    REGISTER_TYPE_RELIVE(relive::Path_ShadowZone);
    REGISTER_TYPE_RELIVE(relive::Path_WorkWheel);
    REGISTER_TYPE_RELIVE(relive::Path_MusicTrigger);
    REGISTER_TYPE_RELIVE(relive::Path_AbeStart);
    REGISTER_TYPE_RELIVE(relive::Path_SoftLanding);
    REGISTER_TYPE_RELIVE(relive::Path_WellExpress);
    REGISTER_TYPE_RELIVE(relive::Path_SlamDoor);
    REGISTER_TYPE_RELIVE(relive::Path_HandStone);
    REGISTER_TYPE_RELIVE(relive::Path_LaughingGas);
    REGISTER_TYPE_RELIVE(relive::Path_InvisibleSwitch);
    REGISTER_TYPE_RELIVE(relive::Path_Water);
    REGISTER_TYPE_RELIVE(relive::Path_GasEmitter);
    REGISTER_TYPE_RELIVE(relive::Path_BackgroundAnimation);
    REGISTER_TYPE_RELIVE(relive::Path_LiftPoint);
    REGISTER_TYPE_RELIVE(relive::Path_PullRingRope);
    REGISTER_TYPE_RELIVE(relive::Path_MultiSwitchController);
    REGISTER_TYPE_RELIVE(relive::Path_SecurityOrb);
    REGISTER_TYPE_RELIVE(relive::Path_InvisibleZone);
    REGISTER_TYPE_RELIVE(relive::Path_ContinuePoint);
    REGISTER_TYPE_RELIVE(relive::Path_WheelSyncer);
    REGISTER_TYPE_RELIVE(relive::Path_LevelLoader);
    REGISTER_TYPE_RELIVE(relive::Path_Pulley);
    REGISTER_TYPE_RELIVE(relive::Path_FlyingSlig);
    REGISTER_TYPE_RELIVE(relive::Path_FlyingSligSpawner);
    REGISTER_TYPE_RELIVE(relive::Path_DeathDrop);
    REGISTER_TYPE_RELIVE(relive::Path_SligSpawner);
    REGISTER_TYPE_RELIVE(relive::Path_SligBoundLeft);
    REGISTER_TYPE_RELIVE(relive::Path_SligBoundRight);
    REGISTER_TYPE_RELIVE(relive::Path_SligPersist);
    REGISTER_TYPE_RELIVE(relive::Path_ZSligCover);
    REGISTER_TYPE_RELIVE(relive::Path_WellLocal);
    REGISTER_TYPE_RELIVE(relive::Path_BrewMachine);
    REGISTER_TYPE_RELIVE(relive::Path_Drill);
    REGISTER_TYPE_RELIVE(relive::Path_Mine);
    REGISTER_TYPE_RELIVE(relive::Path_Slog);
    REGISTER_TYPE_RELIVE(relive::Path_ResetPath);
    REGISTER_TYPE_RELIVE(relive::Path_TrapDoor);
    REGISTER_TYPE_RELIVE(relive::Path_PathTransition);
    REGISTER_TYPE_RELIVE(relive::Path_LiftMover);
    REGISTER_TYPE_RELIVE(relive::Path_RockSack);
    REGISTER_TYPE_RELIVE(relive::Path_TimerTrigger);
    REGISTER_TYPE_RELIVE(relive::Path_MotionDetector);
    REGISTER_TYPE_RELIVE(relive::Path_MineCar);
    REGISTER_TYPE_RELIVE(relive::Path_ExplosionSet);
    REGISTER_TYPE_RELIVE(relive::Path_ColourfulMeter);
    REGISTER_TYPE_RELIVE(relive::Path_Alarm);
    REGISTER_TYPE_RELIVE(relive::Path_DemoSpawnPoint);
    REGISTER_TYPE_RELIVE(relive::Path_SlapLock);
    REGISTER_TYPE_RELIVE(relive::Path_Slurg);
    REGISTER_TYPE_RELIVE(relive::Path_DoorBlocker);
    REGISTER_TYPE_RELIVE(relive::Path_Dove);
    REGISTER_TYPE_RELIVE(relive::Path_BirdPortalExit);
    REGISTER_TYPE_RELIVE(relive::Path_DoorFlame);
    REGISTER_TYPE_RELIVE(relive::Path_TrainDoor);
    REGISTER_TYPE_RELIVE(relive::Path_Greeter);
    REGISTER_TYPE_RELIVE(relive::Path_ScrabBoundLeft);
    REGISTER_TYPE_RELIVE(relive::Path_ScrabBoundRight);
    REGISTER_TYPE_RELIVE(relive::Path_CreditsController);
    REGISTER_TYPE_RELIVE(relive::Path_MovieStone);
    REGISTER_TYPE_RELIVE(relive::Path_MovingBomb);
    REGISTER_TYPE_RELIVE(relive::Path_MovingBombStopper);
    REGISTER_TYPE_RELIVE(relive::Path_SecurityDoor);
    REGISTER_TYPE_RELIVE(relive::Path_CrawlingSlig);
    REGISTER_TYPE_RELIVE(relive::Path_SligGetPants);
    REGISTER_TYPE_RELIVE(relive::Path_SligGetWings);
    REGISTER_TYPE_RELIVE(relive::Path_CrawlingSligButton);
    REGISTER_TYPE_RELIVE(relive::Path_Glukkon);
    REGISTER_TYPE_RELIVE(relive::Path_GlukkonSwitch);
    REGISTER_TYPE_RELIVE(relive::Path_GasCountDown);
    REGISTER_TYPE_RELIVE(relive::Path_FallingItem);
    REGISTER_TYPE_RELIVE(relive::Path_BoneBag);
    REGISTER_TYPE_RELIVE(relive::Path_SecurityClaw);
    REGISTER_TYPE_RELIVE(relive::Path_FootSwitch);
    REGISTER_TYPE_RELIVE(relive::Path_ZzzSpawner);
    REGISTER_TYPE_RELIVE(relive::Path_SlogSpawner);
    REGISTER_TYPE_RELIVE(relive::Path_MainMenuController);
    REGISTER_TYPE_RELIVE(relive::Path_Scrab);
    REGISTER_TYPE_RELIVE(relive::Path_ScrabSpawner);
    REGISTER_TYPE_RELIVE(relive::Path_SlurgSpawner);
    REGISTER_TYPE_RELIVE(relive::Path_Paramite);
    REGISTER_TYPE_RELIVE(relive::Path_ParamiteWebLine);
    REGISTER_TYPE_RELIVE(relive::Path_MeatSack);
    REGISTER_TYPE_RELIVE(relive::Path_TorturedMudokon);
    REGISTER_TYPE_RELIVE(relive::Path_KillUnsavedMuds);
    REGISTER_TYPE_RELIVE(relive::Path_BackgroundGlukkon);
    REGISTER_TYPE_RELIVE(relive::Path_BellsongStone);
    REGISTER_TYPE_RELIVE(relive::Path_LightEffect);
    REGISTER_TYPE_RELIVE(relive::Path_StartController);
    REGISTER_TYPE_RELIVE(relive::Path_HintFly);
    REGISTER_TYPE_RELIVE(relive::Path_Bat);
    REGISTER_TYPE_RELIVE(relive::Path_BellHammer);
    REGISTER_TYPE_RELIVE(relive::Path_ElumPathTrans);
    REGISTER_TYPE_RELIVE(relive::Path_ElumStart);
    REGISTER_TYPE_RELIVE(relive::Path_ElumWall);
    REGISTER_TYPE_RELIVE(relive::Path_RingMudokon);
    REGISTER_TYPE_RELIVE(relive::Path_RingCancel);
    REGISTER_TYPE_RELIVE(relive::Path_MeatSaw);
    REGISTER_TYPE_RELIVE(relive::Path_ChimeLock);
    REGISTER_TYPE_RELIVE(relive::Path_FlintLockFire);
    REGISTER_TYPE_RELIVE(relive::Path_MudokonPathTrans);
    REGISTER_TYPE_RELIVE(relive::Path_ScrabNoFall);
    REGISTER_TYPE_RELIVE(relive::Path_LiftMudokon);
    REGISTER_TYPE_RELIVE(relive::Path_HoneySack);
    REGISTER_TYPE_RELIVE(relive::Path_SlingMudokon);
    REGISTER_TYPE_RELIVE(relive::Path_BeeSwarmHole);
    REGISTER_TYPE_RELIVE(relive::Path_RollingBall);
    REGISTER_TYPE_RELIVE(relive::Path_RollingBallStopper);
    REGISTER_TYPE_RELIVE(relive::Path_BeeNest);
    REGISTER_TYPE_RELIVE(relive::Path_ZBall);
    REGISTER_TYPE_RELIVE(relive::Path_Honey);
    REGISTER_TYPE_RELIVE(relive::Path_HoneyDripTarget);
    REGISTER_TYPE_RELIVE(relive::Path_Elum);*/

#undef REGISTER_TYPE_RELIVE

}
} // namespace ReliveAPI
