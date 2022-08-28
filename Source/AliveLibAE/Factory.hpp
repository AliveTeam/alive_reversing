#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Path;

struct TlvOffsetLevelIdPathId final
{
    u16 tlvOffset;
    u8 levelId;
    u8 pathId;
};

struct TlvOffsetCombinedLevelIdPathId final
{
    u16 tlvOffset;
    u16 levelIdPathId;
};

union TlvItemInfoUnion
{
    u32 all;
    TlvOffsetCombinedLevelIdPathId combined;
    TlvOffsetLevelIdPathId parts;
};
ALIVE_ASSERT_SIZEOF(TlvItemInfoUnion, 4);

enum class LoadMode : s16
{
    ConstructObject_0 = 0,
    LoadResourceFromList_1 = 1,
    LoadResource_2 = 2
};
using TPathFunctionFn = void(CC*)(relive::Path_TLV*, Path*, TlvItemInfoUnion, LoadMode);

struct PathFunctionTable
{
    TPathFunctionFn object_funcs[256];
};

extern const PathFunctionTable kObjectFactory;

// For debug purposes

void Factory_ContinuePoint(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PathTransition(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Hoist(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Edge(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DeathDrop(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Door(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Shadow(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LiftPoint(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ExpressWell(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Dove(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_RockSack(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FallingItem(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PullRingRope(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BackgroundAnimation(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TimedMine(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slig(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slog(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Switch(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D69E0(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SecurityOrb(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6A00(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Pulley(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_AbeStart(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WellExpress(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Mine(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_UXB(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Paramite(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MovieHandStone(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BirdPortal(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PortalExit(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TrapDoor(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_RollingRock(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligBoundLeft(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_InvisibleZone(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FootSwitch(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ChantSuppressor_WithArms(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MotionDetector(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligSpawner(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ElectricWall(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LiftMover(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MeatSack(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Scrab(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA130(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabLeftBound(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabRightBound(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligBoundRight(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligPersist(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_EnemyStopper(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_InvisibleSwitch(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Mudokon(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ZSligCover(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DoorFlame(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MovingBomb(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D8A30(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MainMenuController(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_4D7160(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode); // switch state related ??
void Factory_Null_4D6AE0(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TimerTrigger(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SecurityDoor(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BoomMachine(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LCDScreen(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_HandStone(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_CreditsController(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6910(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LCDStatusBoard(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WheelSyncer(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MusicTrigger(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Light(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlogSpawner(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_GasCountdown(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_4D6C50(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode); // Remove Shrykull ??
void Factory_GasEmitter(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ZzzSpawner(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Glukkon(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_KillUnsavedMudokons(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SoftLanding(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6B00(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Water(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA7C0(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WorkWheel(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA850(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LaughingGas(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FlyingSlig(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Fleech(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slurgs(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlamDoor(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LevelLoader(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DemoSpawnPoint(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Teleporter(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlurgSpawner(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Drill(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ColorfulMeter(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FlyingSligSpawner(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MineCar(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BoneBag(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ExplosionSet(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MultiSwitchController(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_StatusLight(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlapLock(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ParamiteWebLine(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Alarm(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BrewMachine(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabSpawner(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_CrawlingSlig(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligGetPants(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligGetWings(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Greeter(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_CrawlingSligButton(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_GlukkonSwitch(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DoorBlocker(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TorturedMudokon(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TrainDoor(relive::Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
