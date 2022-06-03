#pragma once

#include "../AliveLibCommon/Function.hpp"

struct Path_TLV;
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
using TPathFunctionFn = void(CC*)(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode);

struct PathFunctionTable
{
    TPathFunctionFn object_funcs[256];
};

extern const PathFunctionTable kObjectFactory;

// For debug purposes

void Factory_ContinuePoint(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PathTransition(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Hoist(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Edge(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DeathDrop(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Door(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Shadow(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LiftPoint(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ExpressWell(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Dove(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_RockSack(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FallingItem(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PullRingRope(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BackgroundAnimation(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TimedMine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slig(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slog(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Switch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D69E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SecurityOrb(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6A00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Pulley(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_AbeStart(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WellExpress(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Mine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_UXB(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Paramite(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MovieHandStone(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BirdPortal(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PortalExit(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TrapDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_RollingRock(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligBoundLeft(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_InvisibleZone(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FootSwitch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ChantSuppressor_WithArms(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MotionDetector(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ElectricWall(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LiftMover(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MeatSack(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Scrab(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA130(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabLeftBound(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabRightBound(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligBoundRight(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligPersist(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_EnemyStopper(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_InvisibleSwitch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Mudokon(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ZSligCover(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DoorFlame(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MovingBomb(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D8A30(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MainMenuController(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_4D7160(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode); // switch state related ??
void Factory_Null_4D6AE0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TimerTrigger(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SecurityDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BoomMachine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LCDScreen(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_HandStone(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_CreditsController(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6910(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LCDStatusBoard(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WheelSyncer(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MusicTrigger(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Light(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlogSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_GasCountdown(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_4D6C50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode); // Remove Shrykull ??
void Factory_GasEmitter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ZzzSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Glukkon(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_KillUnsavedMudokons(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SoftLanding(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6B00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Water(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA7C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WorkWheel(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA850(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LaughingGas(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FlyingSlig(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Fleech(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slurgs(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlamDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LevelLoader(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DemoSpawnPoint(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Teleporter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlurgSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Drill(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ColorfulMeter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FlyingSligSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MineCar(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BoneBag(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ExplosionSet(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MultiSwitchController(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_StatusLight(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlapLock(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ParamiteWebLine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Alarm(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BrewMachine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_CrawlingSlig(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligGetPants(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligGetWings(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Greeter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_CrawlingSligButton(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_GlukkonSwitch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DoorBlocker(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TorturedMudokon(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TrainDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
