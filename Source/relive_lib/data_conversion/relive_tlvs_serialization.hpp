#pragma once

#include <nlohmann/json_fwd.hpp>

class PathLine;
class Guid;
struct PSX_RECT;
struct PSX_Point;
struct FixedPoint;
using FP = FixedPoint;
struct RGB16;

namespace relive {
class Path_TLV;
class Path_ShadowZone;
class Path_SecurityOrb;
class Path_ContinuePoint;
struct Path_LiftPoint;
struct Path_Dove;
struct Path_RockSack;
struct Path_ZBall;
struct Path_FallingItem;
struct Path_PullRingRope;
struct Path_TimedMine;
struct Path_Hoist;
struct Path_TrapDoor;
struct Path_LCDScreen;
struct Path_Mine;
struct Path_InvisibleSwitch;
struct Path_ElectricWall;
struct Path_BoomMachine;
struct Path_UXB;
struct Path_MeatSaw;
struct Path_Lever;
struct Path_Edge;
struct Path_BirdPortal;
struct Path_BirdPortalExit;
struct Path_LightEffect;
struct Path_MusicTrigger;
struct Path_SoftLanding;
struct Path_LiftMover;
struct Path_HintFly;
struct Path_TimerTrigger;
struct Path_FlintLockFire;
struct Path_HoneySack;
struct Path_Bat;
struct Path_RollingBallStopper;
struct Path_RollingBall;
struct Path_MotionDetector;
struct Path_BellHammer;
struct Path_SligBoundLeft;
struct Path_SligBoundRight;
struct Path_SligPersist;
struct Path_BackgroundAnimation;
struct Path_MainMenuController;
struct Path_ElumWall;
struct Path_ElumStart;
struct Path_KillUnsavedMuds;
struct Path_InvisibleZone;
struct Path_StartController;
struct Path_ScrabNoFall;
struct Path_ScrabBoundLeft;
struct Path_ScrabBoundRight;
struct Path_HoneyDripTarget;
struct Path_ZSligCover;
struct Path_DeathDrop;
struct Path_ChimeLock;
struct Path_LCDStatusBoard;
struct Path_CreditsController;
struct Path_ResetPath;
struct Path_MeatSack;
struct Path_BeeNest;
struct Path_BellsongStone;
struct Path_MovieStone;
struct Path_HandStone;
struct Path_PathTransition;
struct Path_Pulley;
struct Path_Honey;
struct Path_BeeSwarmHole;
struct Path_Door;
struct Path_AbeStart;
struct Path_EnemyStopper;
struct Path_MovingBombStopper;
struct Path_DoorFlame;
struct Path_Mudokon;
struct Path_MovingBomb;
struct Path_ElumPathTrans;
struct Path_MudokonPathTrans;
struct Path_SecurityClaw;
struct Path_SlingMudokon;
struct Path_FootSwitch;
struct Path_Paramite;
struct Path_ZzzSpawner;
struct Path_BackgroundGlukkon;
struct Path_GasEmitter;
struct Path_GasCountDown;
struct Path_RingCancel;
struct Path_SecurityDoor;
struct Path_LiftMudokon;
struct Path_RingMudokon;
struct Path_WellLocal;
struct Path_WellExpress;
struct Path_Slog;
struct Path_SlogSpawner;
struct Path_Scrab;
struct Path_Slig_Data;
struct Path_Slig;
struct Path_SligSpawner;
struct Path_TrainDoor;
struct Path_TorturedMudokon;
struct Path_DoorBlocker;
struct Path_GlukkonSwitch;
struct Path_Greeter;
struct Path_BrewMachine;
struct Path_Alarm;
struct Path_ParamiteWebLine;
struct Path_SlapLock;
struct Path_StatusLight;
struct Path_MultiSwitchController;
struct Path_ExplosionSet;
struct Path_BoneBag;
struct Path_MineCar;
struct Path_ColourfulMeter;
struct Path_DemoSpawnPoint;
struct Path_LevelLoader;
struct Path_SlamDoor;
struct Path_Slurg;
struct Path_LaughingGas;
struct Path_WorkWheel;
struct Path_Water;
struct Path_WheelSyncer;
struct Path_Fleech;
struct Path_SlurgSpawner;
struct Path_Drill;
struct Path_Teleporter;
struct Path_Glukkon;
struct Path_CrawlingSligButton;
struct Path_FlyingSlig;
struct Path_FlyingSligSpawner;
struct Path_ScrabSpawner;
struct Path_CrawlingSlig;
struct Path_SligGetWings;
struct Path_SligGetPants;
} // namespace relive


void to_json(nlohmann::json& j, const PathLine& p);
void from_json(const nlohmann::json& j, PathLine& p);

// RGB16
void to_json(nlohmann::json& j, const RGB16& p);
void from_json(const nlohmann::json& j, RGB16& p);

namespace relive {

template<class T>
static const relive::Path_TLV& ToBase(const T& derivedType)
{
    return derivedType;
}

template <class T>
static relive::Path_TLV& ToBase(T& derivedType)
{
    return derivedType;
}

// Path_TLV
void to_json(nlohmann::json& j, const Path_TLV& p);
void from_json(const nlohmann::json& j, Path_TLV& p);

void to_json(nlohmann::json& j, const Path_ShadowZone& p);
void from_json(const nlohmann::json& j, Path_ShadowZone& p);

// Path_SecurityOrb
void to_json(nlohmann::json& j, const Path_SecurityOrb& p);
void from_json(const nlohmann::json& j, Path_SecurityOrb& p);

void to_json(nlohmann::json& j, const Path_ContinuePoint& p);
void from_json(const nlohmann::json& j, Path_ContinuePoint& p);

void to_json(nlohmann::json& j, const Path_LiftPoint& p);
void from_json(const nlohmann::json& j, Path_LiftPoint& p);

// Path_Dove
void to_json(nlohmann::json& j, const Path_Dove& p);
void from_json(const nlohmann::json& j, Path_Dove& p);

// Path_RockSack
void to_json(nlohmann::json& j, const Path_RockSack& p);
void from_json(const nlohmann::json& j, Path_RockSack& p);

void to_json(nlohmann::json& j, const Path_ZBall& p);
void from_json(const nlohmann::json& j, Path_ZBall& p);

// Path_FallingItem
void to_json(nlohmann::json& j, const Path_FallingItem& p);
void from_json(const nlohmann::json& j, Path_FallingItem& p);

void to_json(nlohmann::json& j, const Path_PullRingRope& p);
void from_json(const nlohmann::json& j, Path_PullRingRope& p);

// Path_TimedMine
void to_json(nlohmann::json& j, const Path_TimedMine& p);
void from_json(const nlohmann::json& j, Path_TimedMine& p);

void to_json(nlohmann::json& j, const Path_Hoist& p);
void from_json(const nlohmann::json& j, Path_Hoist& p);

void to_json(nlohmann::json& j, const Path_TrapDoor& p);
void from_json(const nlohmann::json& j, Path_TrapDoor& p);

// Path_LCDScreen
void to_json(nlohmann::json& j, const Path_LCDScreen& p);
void from_json(const nlohmann::json& j, Path_LCDScreen& p);

// Path_Mine
void to_json(nlohmann::json& j, const Path_Mine& p);
void from_json(const nlohmann::json& j, Path_Mine& p);

void to_json(nlohmann::json& j, const Path_InvisibleSwitch& p);
void from_json(const nlohmann::json& j, Path_InvisibleSwitch& p);

void to_json(nlohmann::json& j, const Path_ElectricWall& p);
void from_json(const nlohmann::json& j, Path_ElectricWall& p);

void to_json(nlohmann::json& j, const Path_BoomMachine& p);
void from_json(const nlohmann::json& j, Path_BoomMachine& p);

void to_json(nlohmann::json& j, const Path_UXB& p);
void from_json(const nlohmann::json& j, Path_UXB& p);

void to_json(nlohmann::json& j, const Path_MeatSaw& p);
void from_json(const nlohmann::json& j, Path_MeatSaw& p);

void to_json(nlohmann::json& j, const Path_Lever& p);
void from_json(const nlohmann::json& j, Path_Lever& p);

void to_json(nlohmann::json& j, const Path_Edge& p);
void from_json(const nlohmann::json& j, Path_Edge& p);

void to_json(nlohmann::json& j, const Path_BirdPortal& p);
void from_json(const nlohmann::json& j, Path_BirdPortal& p);

 // Path_BirdPortalExit
void to_json(nlohmann::json& j, const Path_BirdPortalExit& p);

void from_json(const nlohmann::json& j, Path_BirdPortalExit& p);



void to_json(nlohmann::json& j, const Path_LightEffect& p);

void from_json(const nlohmann::json& j, Path_LightEffect& p);



void to_json(nlohmann::json& j, const Path_MusicTrigger& p);

void from_json(const nlohmann::json& j, Path_MusicTrigger& p);

// Path_SoftLanding
void to_json(nlohmann::json& j, const Path_SoftLanding& p);

void from_json(const nlohmann::json& j, Path_SoftLanding& p);


void to_json(nlohmann::json& j, const Path_LiftMover& p);

void from_json(const nlohmann::json& j, Path_LiftMover& p);

// Path_Hintfly
void to_json(nlohmann::json& j, const Path_HintFly& p);

void from_json(const nlohmann::json& j, Path_HintFly& p);

// Path_TimerTrigger
void to_json(nlohmann::json& j, const Path_TimerTrigger& p);

void from_json(const nlohmann::json& j, Path_TimerTrigger& p);

// Path_FlintLockFire
void to_json(nlohmann::json& j, const Path_FlintLockFire& p);

void from_json(const nlohmann::json& j, Path_FlintLockFire& p);

// Path_HoneySack
void to_json(nlohmann::json& j, const Path_HoneySack& p);

void from_json(const nlohmann::json& j, Path_HoneySack& p);

// Path_Bat
void to_json(nlohmann::json& j, const Path_Bat& p);

void from_json(const nlohmann::json& j, Path_Bat& p);

// Path_RollingBallStopper
void to_json(nlohmann::json& j, const Path_RollingBallStopper& p);

void from_json(const nlohmann::json& j, Path_RollingBallStopper& p);

// Path_RollingBall
void to_json(nlohmann::json& j, const Path_RollingBall& p);

void from_json(const nlohmann::json& j, Path_RollingBall& p);



void to_json(nlohmann::json& j, const Path_MotionDetector& p);

void from_json(const nlohmann::json& j, Path_MotionDetector& p);

// Path_BellHammer
void to_json(nlohmann::json& j, const Path_BellHammer& p);

void from_json(const nlohmann::json& j, Path_BellHammer& p);

// Path_SligBoundLeft
void to_json(nlohmann::json& j, const Path_SligBoundLeft& p);

void from_json(const nlohmann::json& j, Path_SligBoundLeft& p);

// Path_SligBoundRight
void to_json(nlohmann::json& j, const Path_SligBoundRight& p);

void from_json(const nlohmann::json& j, Path_SligBoundRight& p);

// Path_SligPersist
void to_json(nlohmann::json& j, const Path_SligPersist& p);

void from_json(const nlohmann::json& j, Path_SligPersist& p);



void to_json(nlohmann::json& j, const Path_BackgroundAnimation& p);

void from_json(const nlohmann::json& j, Path_BackgroundAnimation& p);

// Path_MainMenuController
void to_json(nlohmann::json& j, const Path_MainMenuController& p);

void from_json(const nlohmann::json& j, Path_MainMenuController& p);

// Path_ElumWall
void to_json(nlohmann::json& j, const Path_ElumWall& p);

void from_json(const nlohmann::json& j, Path_ElumWall& p);

// Path_ElumStart
void to_json(nlohmann::json& j, const Path_ElumStart& p);

void from_json(const nlohmann::json& j, Path_ElumStart& p);

// Path_KillUnsavedMuds
void to_json(nlohmann::json& j, const Path_KillUnsavedMuds& p);

void from_json(const nlohmann::json& j, Path_KillUnsavedMuds& p);

// Path_InvisibleZone
void to_json(nlohmann::json& j, const Path_InvisibleZone& p);

void from_json(const nlohmann::json& j, Path_InvisibleZone& p);

// Path_StartController
void to_json(nlohmann::json& j, const Path_StartController& p);

void from_json(const nlohmann::json& j, Path_StartController& p);

// Path_ScrabNoFall
void to_json(nlohmann::json& j, const Path_ScrabNoFall& p);

void from_json(const nlohmann::json& j, Path_ScrabNoFall& p);

// Path_ScrabBoundLeft
void to_json(nlohmann::json& j, const Path_ScrabBoundLeft& p);

void from_json(const nlohmann::json& j, Path_ScrabBoundLeft& p);

// Path_ScrabBoundRight
void to_json(nlohmann::json& j, const Path_ScrabBoundRight& p);

void from_json(const nlohmann::json& j, Path_ScrabBoundRight& p);

// Path_HoneyDripTarget
void to_json(nlohmann::json& j, const Path_HoneyDripTarget& p);

void from_json(const nlohmann::json& j, Path_HoneyDripTarget& p);

// Path_ZSligCover
void to_json(nlohmann::json& j, const Path_ZSligCover& p);

void from_json(const nlohmann::json& j, Path_ZSligCover& p);

// Path_DeathDrop
void to_json(nlohmann::json& j, const Path_DeathDrop& p);

void from_json(const nlohmann::json& j, Path_DeathDrop& p);

// Path_ChimeLock
void to_json(nlohmann::json& j, const Path_ChimeLock& p);

void from_json(const nlohmann::json& j, Path_ChimeLock& p);

// Path_LCDStatusBoard
void to_json(nlohmann::json& j, const Path_LCDStatusBoard& p);

void from_json(const nlohmann::json& j, Path_LCDStatusBoard& p);

// Path_CreditsController
void to_json(nlohmann::json& j, const Path_CreditsController& p);

void from_json(const nlohmann::json& j, Path_CreditsController& p);

// Path_ResetPath
void to_json(nlohmann::json& j, const Path_ResetPath& p);

void from_json(const nlohmann::json& j, Path_ResetPath& p);

// Path_MeatSack
void to_json(nlohmann::json& j, const Path_MeatSack& p);

void from_json(const nlohmann::json& j, Path_MeatSack& p);

// Path_BeeNest
void to_json(nlohmann::json& j, const Path_BeeNest& p);

void from_json(const nlohmann::json& j, Path_BeeNest& p);



void to_json(nlohmann::json& j, const Path_BellsongStone& p);

void from_json(const nlohmann::json& j, Path_BellsongStone& p);

// Path_MovieStone
void to_json(nlohmann::json& j, const Path_MovieStone& p);

void from_json(const nlohmann::json& j, Path_MovieStone& p);

// Path_HandStone
void to_json(nlohmann::json& j, const Path_HandStone& p);

void from_json(const nlohmann::json& j, Path_HandStone& p);

// Path_PathTransition
void to_json(nlohmann::json& j, const Path_PathTransition& p);

void from_json(const nlohmann::json& j, Path_PathTransition& p);

// Path_Pulley
void to_json(nlohmann::json& j, const Path_Pulley& p);

void from_json(const nlohmann::json& j, Path_Pulley& p);

// Path_Honey
void to_json(nlohmann::json& j, const Path_Honey& p);

void from_json(const nlohmann::json& j, Path_Honey& p);



void to_json(nlohmann::json& j, const Path_BeeSwarmHole& p);

void from_json(const nlohmann::json& j, Path_BeeSwarmHole& p);



void to_json(nlohmann::json& j, const Path_Door& p);

void from_json(const nlohmann::json& j, Path_Door& p);

// Path_AbeStart
void to_json(nlohmann::json& j, const Path_AbeStart& p);

void from_json(const nlohmann::json& j, Path_AbeStart& p);



void to_json(nlohmann::json& j, const Path_EnemyStopper& p);

void from_json(const nlohmann::json& j, Path_EnemyStopper& p);

// Path_MovingBombStopper
void to_json(nlohmann::json& j, const Path_MovingBombStopper& p);

void from_json(const nlohmann::json& j, Path_MovingBombStopper& p);



void to_json(nlohmann::json& j, const Path_DoorFlame& p);

void from_json(const nlohmann::json& j, Path_DoorFlame& p);



void to_json(nlohmann::json& j, const Path_Mudokon& p);

void from_json(const nlohmann::json& j, Path_Mudokon& p);

// Path_MovingBomb
void to_json(nlohmann::json& j, const Path_MovingBomb& p);

void from_json(const nlohmann::json& j, Path_MovingBomb& p);

// Path_ElumPathTrans
void to_json(nlohmann::json& j, const Path_ElumPathTrans& p);

void from_json(const nlohmann::json& j, Path_ElumPathTrans& p);

// Path_MudokonPathTrans
void to_json(nlohmann::json& j, const Path_MudokonPathTrans& p);

void from_json(const nlohmann::json& j, Path_MudokonPathTrans& p);

// Path_SecurityClaw
void to_json(nlohmann::json& j, const Path_SecurityClaw& p);

void from_json(const nlohmann::json& j, Path_SecurityClaw& p);

// Path_SlingMudokon
void to_json(nlohmann::json& j, const Path_SlingMudokon& p);

void from_json(const nlohmann::json& j, Path_SlingMudokon& p);



void to_json(nlohmann::json& j, const Path_FootSwitch& p);

void from_json(const nlohmann::json& j, Path_FootSwitch& p);



void to_json(nlohmann::json& j, const Path_Paramite& p);

void from_json(const nlohmann::json& j, Path_Paramite& p);

// Path_ZzzSpawner
void to_json(nlohmann::json& j, const Path_ZzzSpawner& p);

void from_json(const nlohmann::json& j, Path_ZzzSpawner& p);

// Path_BackgroundGlukkon
void to_json(nlohmann::json& j, const Path_BackgroundGlukkon& p);

void from_json(const nlohmann::json& j, Path_BackgroundGlukkon& p);



void to_json(nlohmann::json& j, const Path_GasEmitter& p);

void from_json(const nlohmann::json& j, Path_GasEmitter& p);

// Path_GasCountDown
void to_json(nlohmann::json& j, const Path_GasCountDown& p);

void from_json(const nlohmann::json& j, Path_GasCountDown& p);

// Path_RingCancel
void to_json(nlohmann::json& j, const Path_RingCancel& p);

void from_json(const nlohmann::json& j, Path_RingCancel& p);

// Path_SecurityDoor
void to_json(nlohmann::json& j, const Path_SecurityDoor& p);
void from_json(const nlohmann::json& j, Path_SecurityDoor& p);

void to_json(nlohmann::json& j, const Path_LiftMudokon& p);
void from_json(const nlohmann::json& j, Path_LiftMudokon& p);

void to_json(nlohmann::json& j, const Path_RingMudokon& p);
void from_json(const nlohmann::json& j, Path_RingMudokon& p);

// Path_WellLocal
void to_json(nlohmann::json& j, const Path_WellLocal& p);
void from_json(const nlohmann::json& j, Path_WellLocal& p);

// Path_WellExpress
void to_json(nlohmann::json& j, const Path_WellExpress& p);
void from_json(const nlohmann::json& j, Path_WellExpress& p);

// Path_Slog
void to_json(nlohmann::json& j, const Path_Slog& p);

void from_json(const nlohmann::json& j, Path_Slog& p);

void to_json(nlohmann::json& j, const Path_SlogSpawner& p);
void from_json(const nlohmann::json& j, Path_SlogSpawner& p);

void to_json(nlohmann::json& j, const Path_Scrab& p);
void from_json(const nlohmann::json& j, Path_Scrab& p);

void to_json(nlohmann::json& j, const Path_Slig_Data& p);
void from_json(const nlohmann::json& j, Path_Slig_Data& p);

void to_json(nlohmann::json& j, const Path_Slig& p);
void from_json(const nlohmann::json& j, Path_Slig& p);

// Path_SligSpawner
void to_json(nlohmann::json& j, const Path_SligSpawner& p);
void from_json(const nlohmann::json& j, Path_SligSpawner& p);

// Path_TrainDoor
void to_json(nlohmann::json& j, const Path_TrainDoor& p);
void from_json(const nlohmann::json& j, Path_TrainDoor& p);

// Path_TorturedMudokon
void to_json(nlohmann::json& j, const Path_TorturedMudokon& p);
void from_json(const nlohmann::json& j, Path_TorturedMudokon& p);

// Path_DoorBlocker
void to_json(nlohmann::json& j, const Path_DoorBlocker& p);
void from_json(const nlohmann::json& j, Path_DoorBlocker& p);

// Path_GlukkonSwitch
void to_json(nlohmann::json& j, const Path_GlukkonSwitch& p);
void from_json(const nlohmann::json& j, Path_GlukkonSwitch& p);

// Path_Greeter
void to_json(nlohmann::json& j, const Path_Greeter& p);
void from_json(const nlohmann::json& j, Path_Greeter& p);

// Path_BrewMachine
void to_json(nlohmann::json& j, const Path_BrewMachine& p);
void from_json(const nlohmann::json& j, Path_BrewMachine& p);

// Path_Alarm
void to_json(nlohmann::json& j, const Path_Alarm& p);
void from_json(const nlohmann::json& j, Path_Alarm& p);

// Path_ParamiteWebLine
void to_json(nlohmann::json& j, const Path_ParamiteWebLine& p);
void from_json(const nlohmann::json& j, Path_ParamiteWebLine& p);

// Path_SlapLock
void to_json(nlohmann::json& j, const Path_SlapLock& p);

void from_json(const nlohmann::json& j, Path_SlapLock& p);

// Path_StatusLight
void to_json(nlohmann::json& j, const Path_StatusLight& p);
void from_json(const nlohmann::json& j, Path_StatusLight& p);

// Path_MultiSwitchController
void to_json(nlohmann::json& j, const Path_MultiSwitchController& p);
void from_json(const nlohmann::json& j, Path_MultiSwitchController& p);

// Path_ExplosionSet
void to_json(nlohmann::json& j, const Path_ExplosionSet& p);
void from_json(const nlohmann::json& j, Path_ExplosionSet& p);

// Path_BoneBag
void to_json(nlohmann::json& j, const Path_BoneBag& p);
void from_json(const nlohmann::json& j, Path_BoneBag& p);

// Path_MineCar
void to_json(nlohmann::json& j, const Path_MineCar& p);
void from_json(const nlohmann::json& j, Path_MineCar& p);

// Path_ColourfulMeter
void to_json(nlohmann::json& j, const Path_ColourfulMeter& p);
void from_json(const nlohmann::json& j, Path_ColourfulMeter& p);

// Path_DemoSpawnPoint
void to_json(nlohmann::json& j, const Path_DemoSpawnPoint& p);
void from_json(const nlohmann::json& j, Path_DemoSpawnPoint& p);

// Path_LevelLoader
void to_json(nlohmann::json& j, const Path_LevelLoader& p);
void from_json(const nlohmann::json& j, Path_LevelLoader& p);

// Path_SlamDoor
void to_json(nlohmann::json& j, const Path_SlamDoor& p);
void from_json(const nlohmann::json& j, Path_SlamDoor& p);

// Path_Slurg
void to_json(nlohmann::json& j, const Path_Slurg& p);
void from_json(const nlohmann::json& j, Path_Slurg& p);

// Path_LaughingGas
void to_json(nlohmann::json& j, const Path_LaughingGas& p);
void from_json(const nlohmann::json& j, Path_LaughingGas& p);

// Path_WorkWheel
void to_json(nlohmann::json& j, const Path_WorkWheel& p);
void from_json(const nlohmann::json& j, Path_WorkWheel& p);

// Path_Water
void to_json(nlohmann::json& j, const Path_Water& p);
void from_json(const nlohmann::json& j, Path_Water& p);

void to_json(nlohmann::json& j, const Path_WheelSyncer& p);
void from_json(const nlohmann::json& j, Path_WheelSyncer& p);

// Path_Fleech
void to_json(nlohmann::json& j, const Path_Fleech& p);

void from_json(const nlohmann::json& j, Path_Fleech& p);

// Path_SlurgSpawner
void to_json(nlohmann::json& j, const Path_SlurgSpawner& p);
void from_json(const nlohmann::json& j, Path_SlurgSpawner& p);

void to_json(nlohmann::json& j, const Path_Drill& p);
void from_json(const nlohmann::json& j, Path_Drill& p);

// Path_Teleporter
void to_json(nlohmann::json& j, const Path_Teleporter& p);

void from_json(const nlohmann::json& j, Path_Teleporter& p);

void to_json(nlohmann::json& j, const Path_Glukkon& p);
void from_json(const nlohmann::json& j, Path_Glukkon& p);

void to_json(nlohmann::json& j, const Path_CrawlingSligButton& p);
void from_json(const nlohmann::json& j, Path_CrawlingSligButton& p);

void to_json(nlohmann::json& j, const Path_FlyingSlig& p);
void from_json(const nlohmann::json& j, Path_FlyingSlig& p);

// Path_FlyingSligSpawner
void to_json(nlohmann::json& j, const Path_FlyingSligSpawner& p);
void from_json(const nlohmann::json& j, Path_FlyingSligSpawner& p);

void to_json(nlohmann::json& j, const Path_ScrabSpawner& p);
void from_json(const nlohmann::json& j, Path_ScrabSpawner& p);

void to_json(nlohmann::json& j, const Path_CrawlingSlig& p);
void from_json(const nlohmann::json& j, Path_CrawlingSlig& p);

// Path_SligGetWings
void to_json(nlohmann::json& j, const Path_SligGetWings& p);
void from_json(const nlohmann::json& j, Path_SligGetWings& p);

// Path_SligGetPants
void to_json(nlohmann::json& j, const Path_SligGetPants& p);
void from_json(const nlohmann::json& j, Path_SligGetPants& p);

} // namespace relive

