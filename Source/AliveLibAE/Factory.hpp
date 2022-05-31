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

void Factory_ContinuePoint_4D6970(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PathTransition_4D68A0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Hoist_4D9E90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Edge_4D68C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DeathDrop_4D6930(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Door_4D6F00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Shadow_4D7200(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LiftPoint_4D7250(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ExpressWell_4D7D90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Dove_4D7E90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_RockSack_4D8040(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FallingItem_4D81B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PullRingRope_4D8320(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BackgroundAnimation_4D84F0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TimedMine_4D87C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slig_4D7BC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slog_4D8B20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Switch_4D8CF0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D69E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SecurityOrb_4D8D80(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6A00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Pulley_4D6A20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_AbeStart_4D9030(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WellExpress_4D7E60(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Mine_4D8890(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_UXB_4D8960(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Paramite_4D9190(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MovieHandStone_4D9F50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BirdPortal_4D9AA0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_PortalExit_4D9A80(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TrapDoor_4D9B90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_RollingRock_4D9C70(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligBoundLeft_4D7740(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_InvisibleZone_4D6A40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FootSwitch_4D9D00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ChantSuppressor_WithArms_4D9E10(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MotionDetector_4D9E40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligSpawner_4D79F0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ElectricWall_4DA020(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LiftMover_4DA090(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MeatSack_4D8140(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Scrab_4D9200(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA130(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabLeftBound_4D6A80(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabRightBound_4D6AA0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligBoundRight_4D79A0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligPersist_4D79D0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_EnemyStopper_4D6AC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_InvisibleSwitch_4D6E40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Mudokon_4D8EC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ZSligCover_4D6A60(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DoorFlame_4D70D0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MovingBomb_4D8A50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D8A30(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MainMenuController_4D6DB0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_4D7160(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode); // switch state related ??
void Factory_Null_4D6AE0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TimerTrigger_4DA0E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SecurityDoor_4DA150(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_GrenadeMachine_4DA1C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LCD_4D6CF0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_HandStone_4D9FA0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_CreditsController_4D6D60(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6910(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_StatusBoard_4DA3C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WheelSyncer_4DA430(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MusicTrigger_4D71B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Light_4D8590(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlogSpawner_4D8BB0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_GasCountdown_4DA480(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_4D6C50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode); // Remove Shrykull ??
void Factory_GasEmitter_4D8540(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ZzzSpawner_4DA500(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Glukkon_4DA550(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_KillUnsavedMudokons_4DA6E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SoftLanding_4D6950(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4D6B00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Water_4DA730(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA7C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_WorkWheel_4D6B20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Null_4DA850(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LaughingGas_4DA870(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FlyingSlig_4D92E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Fleech_4D8C30(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Slurgs_4DA950(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlamDoor_4DA9C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_LevelLoader_4D6BB0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DemoSpawnPoint_4D6990(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Teleporter_4DAA90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlurgSpawner_4DAB50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Drill_4DABC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ColorfulMeter_4DAC60(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_FlyingSligSpawner_4D9440(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MineCar_4DACD0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BoneBag_4D80B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ExplosionSet_4DADC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_MultiSwitchController_4D6C00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_StatusLight_4DA7E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SlapLock_4DAF20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ParamiteWebLine_4D9120(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Alarm_4DAF90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_BrewMachine_4DA2B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_ScrabSpawner_4D9270(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_NakedSlig_4D95A0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligGetPants_4D9700(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_SligGetWings_4D9900(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_Greeter_4DAFE0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_NakedSligButton_4DB100(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_GlukkonSwitch_4DA670(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_DoorBlocker_4DAAE0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TorturedMudokon_4D8430(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
void Factory_TrainDoor_4D6E90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode);
