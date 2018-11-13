#include "stdafx.h"
#include "Abe.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Midi.hpp"
#include "Sfx.hpp"
#include "DebugHelpers.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "ObjectIds.hpp"
#include "Input.hpp"
#include "Events.hpp"
#include "Quicksave.hpp"
#include "MainMenu.hpp"
#include "ThrowableArray.hpp"

const char * sAbeStateNames[] =
{
    "Idle_44EEB0",
    "WalkLoop_44FBA0",
    "StandingTurn_451830",
    "Fall_459B60",
    "WalkEndLeftFoot_44FFC0",
    "WalkEndRightFoot_00450080",
    "WalkBegin_44FEE0",
    "45B140",
    "45B160",
    "45B180",
    "Fart_45B1A0",
    "Speak_45B0A0",
    "nullsub_41",
    "HoistBegin_452B20",
    "HoistIdle_452440",
    "HoistLand_452BA0",
    "LandSoft_45A360",
    "CrouchIdle_456BC0",
    "CrouchToStand_454600",
    "StandToCrouch_453DC0",
    "454550",
    "j_j_454550",
    "RollBegin_4539A0",
    "RollLoop_453A90",
    "453D00",
    "RunSlideStop_451330",
    "RunTurn_451500",
    "HopBegin_4521C0",
    "HopMid_451C50",
    "HopLand_4523D0",
    "RunJumpBegin_4532E0",
    "RunJumpMid_452C10",
    "RunJumpLand_453460",
    "RunLoop_4508E0",
    "DunnoBegin_44ECF0",
    "DunnoEnd_44ED10",
    "nullsub_43",
    "CrouchTurn_454390",
    "RunToRoll_4539A0",
    "StandingToRun_450D40",
    "SneakLoop_450550",
    "450250",
    "4503D0",
    "450380",
    "450500",
    "SneakBegin_4507A0",
    "SneakEnd1_450870",
    "SneakEnd2_450870",
    "4500A0",
    "450200",
    "RunToWalk1_450E20",
    "RunToWalk2_450F50",
    "451710",
    "451800",
    "RunJumpLandRun_4538F0",
    "RunJumpLandWalk_453970",
    "4591F0",
    "Dead_4589A0",
    "DeadPre_4593E0",
    "nullsub_42",
    "4A3200",
    "TurnToRun_456530",
    "Punch_454750",
    "Sorry_454670",
    "454730",
    "LedgeAscend_4548E0",
    "LedgeDescend_454970",
    "LedgeHang_454E20",
    "454B80",
    "LedgeHangWobble_454EF0",
    "RingRopePull_455AF0",
    "Knockback_455090",
    "KnockbackGetUp_455340",
    "PushWall_4553A0",
    "455290",
    "45C7B0",
    "45CA40",
    "45D130",
    "WellBegin_45C810",
    "WellInside_45CA60",
    "WellShotOut_45D150",
    "j_j_WellBegin_45C810",
    "45CC80",
    "45CF70",
    "FallLandDie_45A420",
    "j_j_Fall_459B60",
    "HandstoneBegin_45BD00",
    "HandstoneEnd_45C4F0",
    "GrenadeMachineUse_45C510",
    "BrewMachineBegin_4584C0",
    "BrewMachineEnd_4585B0",
    "RingRopePullEnd_4557B0",
    "455800",
    "FallLedgeBegin_455970",
    "j_j_455970",
    "j_j_455970_0",
    "4559E0",
    "j_j_455970_1",
    "j_j_RollEdgeFall_455970",
    "LeverUse_455AC0",
    "455B60",
    "KnockForward_455420",
    "455310",
    "j_j_455340",
    "RockThrowStandingHold_455DF0",
    "RockThrowStandingThrow_456460",
    "RockThrowStandingEnd_455F20",
    "RockThrowCrouchingHold_454410",
    "RockThrowCrouchingThrow_454500",
    "455550",
    "455670",
    "GrabRock_4564A0",
    "Chant_45B1C0",
    "ChantEnd_45BBE0",
    "DoorEnter_459470",
    "DoorExit_459A40",
    "MineCarEnter_458780",
    "4587C0",
    "MineCarExit_458890",
    "45A990",
    "45AB00",
    "LiftGrabBegin_45A600",
    "LiftGrabEnd_45A670",
    "LiftGrabIdle_45A6A0",
    "LiftUseUp_45A780",
    "LiftUseDown_45A7B0",
    "TurnWheelBegin_456700",
    "TurnWheelLoop_456750",
    "TurnWheelEnd_4569A0",
    "PoisonGasDeath_4565C0",
};

SfxDefinition sAbeSFXList_555250[41] =
{
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 24u, 60u, 115u, 0, 0 },
    { 0u, 24u, 61u, 110u, 0, 0 },
    { 0u, 24u, 62u, 127u, 0, 0 },
    { 0u, 24u, 63u, 90u, 0, 0 },
    { 0u, 24u, 66u, 127u, 0, 0 },
    { 0u, 24u, 67u, 100u, 0, 0 },
    { 0u, 24u, 56u, 127u, 0, 0 },
    { 0u, 24u, 57u, 100u, 0, 0 },
    { 0u, 23u, 48u, 127u, 0, 0 },
    { 0u, 24u, 59u, 127u, 0, 0 },
    { 0u, 24u, 58u, 127u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 24u, 65u, 127u, 0, 0 },
    { 0u, 24u, 64u, 80u, 0, 0 },
    { 0u, 23u, 60u, 90u, 0, 0 },
    { 0u, 23u, 68u, 127u, 0, 0 },
    { 0u, 23u, 69u, 127u, 0, 0 },
    { 0u, 23u, 70u, 127u, 0, 0 },
    { 0u, 23u, 72u, 127u, 0, 0 },
    { 0u, 23u, 73u, 127u, 0, 0 },
    { 0u, 23u, 58u, 127u, 0, 0 },
    { 0u, 23u, 64u, 127u, 0, 0 },
    { 0u, 23u, 66u, 115u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 23u, 63u, 115u, 0, 0 },
    { 0u, 23u, 62u, 90u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 }
};

int sAbeFrameOffsetTable_554B18[130] =
{
    58888, 58808, 59064, 58888, 58788, 58748, 58768, 59028, 58992, 58956, 58920, 
    59112, 270240, 269976, 270024, 269928, 269928, 270092, 270060, 269876, 271080, 
    271120, 269688, 269708, 269764, 270860, 270328, 271004, 271056, 270964, 
    270468, 269804, 270488, 270748, 270180, 270616, 270180, 270428, 270708, 270252, 
    270508, 270688, 270596, 270840, 270728, 270288, 270160, 270308, 270120, 270268, 
    270668, 270820, 270408, 270140, 269784, 269856, 270092, 270092, 270092, 270092, 
    270092, 270944, 271152, 271192, 271248, 57440, 57580, 57540, 57540, 57712, 
    57680, 49740, 49800, 49912, 49856, 20572, 20552, 20552, 20404, 20552, 20552, 20404, 
    20552, 20552, 8104, 5724, 16096, 16040, 16220, 16096, 16040, 32012, 32012, 32040, 
    32068, 32112, 32168, 32152, 32132, 11396, 6520, 17240, 17180, 58888, 31504, 31392, 
    31632, 31576, 31432, 24580, 24628, 6004, 9992, 10040, 19040, 19088, 8540, 8540, 8588, 
    8732, 8772, 22548, 22572, 22464, 22596, 22496, 11856, 11816, 11888, 28824
};

TintEntry sTintTable_Abe_554D20[15] =
{
    { 1, 102u, 102u, 102u },
    { 2, 102u, 102u, 80u },
    { 3, 120u, 90u, 120u },
    { 4, 102u, 70u, 90u },
    { 5, 120u, 102u, 82u },
    { 6, 102u, 102u, 102u },
    { 7, 102u, 70u, 90u },
    { 8, 102u, 102u, 102u },
    { 9, 102u, 102u, 102u },
    { 10, 102u, 102u, 102u },
    { 11, 120u, 90u, 120u },
    { 12, 120u, 102u, 82u },
    { 13, 102u, 102u, 102u },
    { 14, 120u, 90u, 80u },
    { -1, 102u, 102u, 102u }
};

using TAbeStateFunction = decltype(&Abe::State_Idle_44EEB0);

TAbeStateFunction sAbeStateMachineTable_554910[130] =
{
    &Abe::State_Idle_44EEB0,
    &Abe::State_WalkLoop_44FBA0,
    &Abe::State_StandingTurn_451830,
    &Abe::State_Fall_459B60,
    &Abe::State_WalkEndLeftFoot_44FFC0,
    &Abe::State_WalkEndRightFoot_00450080,
    &Abe::State_WalkBegin_44FEE0,
    &Abe::State_45B140,
    &Abe::State_45B160,
    &Abe::State_45B180,
    &Abe::State_Fart_45B1A0,
    &Abe::State_Speak_45B0A0,
    &Abe::State_Null_4569C0,
    &Abe::State_HoistBegin_452B20,
    &Abe::State_HoistIdle_452440,
    &Abe::State_HoistLand_452BA0,
    &Abe::State_LandSoft_45A360,
    &Abe::State_CrouchIdle_456BC0,
    &Abe::State_CrouchToStand_454600,
    &Abe::State_StandToCrouch_453DC0,
    &Abe::State_454550,
    &Abe::jState_4545E0,
    &Abe::State_RollBegin_4539A0,
    &Abe::State_RollLoop_453A90,
    &Abe::State_453D00,
    &Abe::State_RunSlideStop_451330,
    &Abe::State_RunTurn_451500,
    &Abe::State_HopBegin_4521C0,
    &Abe::State_HopMid_451C50,
    &Abe::State_HopLand_4523D0,
    &Abe::State_RunJumpBegin_4532E0,
    &Abe::State_RunJumpMid_452C10,
    &Abe::State_RunJumpLand_453460,
    &Abe::State_RunLoop_4508E0,
    &Abe::State_DunnoBegin_44ECF0,
    &Abe::State_DunnoEnd_44ED10,
    &Abe::State_Null_45BC50,
    &Abe::State_CrouchTurn_454390,
    &Abe::jState_RollBegin_453A70,
    &Abe::State_StandingToRun_450D40,
    &Abe::State_SneakLoop_450550,
    &Abe::State_450250,
    &Abe::State_4503D0,
    &Abe::State_450380,
    &Abe::State_450500,
    &Abe::State_SneakBegin_4507A0,
    &Abe::State_SneakEnd_450870,
    &Abe::jState_SneakEnd_4508C0,
    &Abe::State_4500A0,
    &Abe::State_450200,
    &Abe::State_RunToWalk1_450E20,
    &Abe::State_RunToWalk2_450F50,
    &Abe::State_451710,
    &Abe::State_451800,
    &Abe::State_RunJumpLandRun_4538F0,
    &Abe::State_RunJumpLandWalk_453970,
    &Abe::State_4591F0,
    &Abe::State_Dead_4589A0,
    &Abe::State_DeadPre_4593E0,
    &Abe::State_Null_459450,
    &Abe::State_4A3200,
    &Abe::State_TurnToRun_456530,
    &Abe::State_Punch_454750,
    &Abe::State_Sorry_454670,
    &Abe::State_454730,
    &Abe::State_LedgeAscend_4548E0,
    &Abe::State_LedgeDescend_454970,
    &Abe::State_LedgeHang_454E20,
    &Abe::State_454B80,
    &Abe::State_LedgeHangWobble_454EF0,
    &Abe::State_RingRopePull_455AF0,
    &Abe::State_Knockback_455090,
    &Abe::State_KnockbackGetUp_455340,
    &Abe::State_PushWall_4553A0,
    &Abe::State_455290,
    &Abe::State_45C7B0,
    &Abe::State_45CA40,
    &Abe::State_45D130,
    &Abe::State_WellBegin_45C810,
    &Abe::State_WellInside_45CA60,
    &Abe::State_WellShotOut_45D150,
    &Abe::jState_WellBegin_45C7F0,
    &Abe::State_45CC80,
    &Abe::State_45CF70,
    &Abe::State_FallLandDie_45A420,
    &Abe::jState_Fall_455070,
    &Abe::State_HandstoneBegin_45BD00,
    &Abe::State_HandstoneEnd_45C4F0,
    &Abe::State_GrenadeMachineUse_45C510,
    &Abe::State_BrewMachineBegin_4584C0,
    &Abe::State_BrewMachineEnd_4585B0,
    &Abe::State_RingRopePullEnd_4557B0,
    &Abe::State_455800,
    &Abe::State_FallLedgeBegin_455970,
    &Abe::jState_FallLedgeBegin_4559A0,
    &Abe::jState_FallLedgeBegin_4559C0,
    &Abe::State_4559E0,
    &Abe::jState_FallLedgeBegin_455A80,
    &Abe::jState_FallLedgeBegin_455AA0,
    &Abe::State_LeverUse_455AC0,
    &Abe::State_455B60,
    &Abe::State_KnockForward_455420,
    &Abe::State_455310,
    &Abe::jState_KnockbackGetUp_455380,
    &Abe::State_RockThrowStandingHold_455DF0,
    &Abe::State_RockThrowStandingThrow_456460,
    &Abe::State_RockThrowStandingEnd_455F20,
    &Abe::State_RockThrowCrouchingHold_454410,
    &Abe::State_RockThrowCrouchingThrow_454500,
    &Abe::State_455550,
    &Abe::State_455670,
    &Abe::State_GrabRock_4564A0,
    &Abe::State_Chant_45B1C0,
    &Abe::State_ChantEnd_45BBE0,
    &Abe::State_DoorEnter_459470,
    &Abe::State_DoorExit_459A40,
    &Abe::State_MineCarEnter_458780,
    &Abe::State_4587C0,
    &Abe::State_MineCarExit_458890,
    &Abe::State_45A990,
    &Abe::State_45AB00,
    &Abe::State_LiftGrabBegin_45A600,
    &Abe::State_LiftGrabEnd_45A670,
    &Abe::State_LiftGrabIdle_45A6A0,
    &Abe::State_LiftUseUp_45A780,
    &Abe::State_LiftUseDown_45A7B0,
    &Abe::State_TurnWheelBegin_456700,
    &Abe::State_TurnWheelLoop_456750,
    &Abe::State_TurnWheelEnd_4569A0,
    &Abe::State_PoisonGasDeath_4565C0
};

// Frame call backs ??
EXPORT int CC sub_455F40(void*, signed __int16*)
{
    return 1;
}

EXPORT int CC sub_455F60(void*, signed __int16*)
{
    return 1;
}

EXPORT int CC sub_455F80(void*, signed __int16*)
{
    // Does ray casting checks?
    NOT_IMPLEMENTED();
    return 1;
}

EXPORT int CC sub_4561B0(void*, signed __int16*)
{
    // Slurg squish check/particles?
    NOT_IMPLEMENTED();
    return 1;
}

EXPORT int CC sub_434130(void*, signed __int16*)
{
    // ??
    NOT_IMPLEMENTED();
    return 1;
}

using TFrameCallBackType = decltype(&sub_434130);

// TODO: Array is possibly bigger, called by AnimationEx::Invoke_CallBacks_40B7A0
ALIVE_ARY(1, 0x55EF98, TFrameCallBackType, 5, off_55EF98,
{
    sub_455F40,
    sub_455F60,
    sub_455F80,
    sub_4561B0,
    sub_434130
});

enum AbeResources
{
    eAbeBSic = 0,
    eAbeBSic1 = 1,
};

EXPORT int CC GridXMidPos_4498F0(FP scale, int unknown)
{
    if (scale == FP_FromDouble(0.5))
    {
        // 12.5 = half grid size
        return (13 * unknown) - 8;
    }

    if (scale == FP_FromDouble(1.0))
    {
        // 25 = full grid size
        return (25 * unknown) - 13;
    }

    // Default to middle of the screen
    return (374 / 2);
}

Abe* Abe::ctor_44AD10(int frameTableOffset, int /*a3*/, int /*a4*/, int /*a5*/)
{
    const int kResourceArraySize = 28;

    ctor_408240(kResourceArraySize);

    SetVTable(this, 0x5457BC); // gVTbl_Abe_5457BC
    
    field_4_typeId = Types::eType_Abe;

    field_6_flags.Set(BaseGameObject::eBit08);
    field_C_objectId = -65536;

    Init_GameStates_43BF40();

    // Zero out the resource array
    for (int i = 0; i < kResourceArraySize; i++)
    {
        field_10_resources_array.SetAt(i, nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBSIC1.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, TRUE, FALSE);
    }
    field_10_resources_array.SetAt(AbeResources::eAbeBSic1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, FALSE, FALSE));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeedgeResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEEDGE.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeedgeResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknfdResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNFD.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknfdResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeommResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEOMM.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeommResID, TRUE, FALSE);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknbkResID, TRUE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEKNBK.BAN", nullptr);
        ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeknbkResID, TRUE, FALSE);
    }

    ResourceManager::LoadResourceFile_49C170("ABENOELM.BND", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbefallResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kAbesmashResID);

    ResourceManager::LoadResourceFile_49C170("OMMFLARE.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID);

    ResourceManager::LoadResourceFile_49C170("SQBSMK.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSquibSmokeResID);

    ResourceManager::LoadResourceFile_49C170("DUST.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDustResID);

    ResourceManager::LoadResourceFile_49C170("BLOODROP.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kBloodropResID);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SHADOW.BAN", nullptr);
    }
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kRockShadowResID);

    ResourceManager::LoadResourceFile_49C170("DEADFLR.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID);

    ResourceManager::LoadResourceFile_49C170("DOVBASIC.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDovbasicResID);

    ResourceManager::LoadResourceFile_49C170("SPOTLITE.BAN", nullptr);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSpotliteResID);

    field_128.field_10 = 1;

    Animation_Init_424E10(
        frameTableOffset,
        135, // Width
        80,  // Height
        field_10_resources_array.ItemAt(AbeResources::eAbeBSic1),
        1,
        1u);

    field_20_animation.field_1C_fn_ptr_array = off_55EF98;

    PSX_Point point = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&point);

    field_B8_xpos = FP_FromInteger(point.field_0_x + GridXMidPos_4498F0(field_CC_sprite_scale, 4));
    field_BC_ypos = FP_FromInteger(point.field_2_y + 120);

    field_F8 = field_BC_ypos;
    field_128.field_8 = 0;
    field_128.field_C = 0;

    field_C4_velx = FP(0);
    field_C8_vely = FP(0);
    field_100_pCollisionLine = nullptr;
    field_106_animation_num = 3;
    field_122 = -1;
    field_20_animation.field_C_render_layer = 32;
    field_198_has_evil_fart = 0;
    field_1A2_rock_or_bone_count = 0;
    field_158 = -1;
    field_154 = -1;
    field_150 = -1;
    field_14C = -1;
    field_148 = -1;
    field_1A8 = -1;
    field_164 = -1;
    field_160 = -1;
    field_15C = -1;
    field_1AE &= ~3u;

    __int16 v9 = field_114_flags;
    v9 = v9 & ~0x100;
    v9 = v9 | 0x20;
    field_114_flags = v9;

    field_1AC_flags = (unsigned __int8)(field_1AC_flags & 0xAB) | 0x40;
    
    // Changes Abe's "default" colour depending on the level we are in
    SetTint_425600(&sTintTable_Abe_554D20[0], gMap_5C3030.sCurrentLevelId_5C3030);
    
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = 0;

    field_118 = 0;
    field_F6 = 0;
    field_120_state = 0;
    field_168 = 0;
    field_16E = 0;
    field_170 = 0;
    field_174 = 0;
    field_176 = 0;
    field_178 = -1;
    field_1AC_flags &= ~3u;
    field_124_gnFrame = sGnFrame_5C1B84;
    field_FC_pPathTLV = nullptr;
    field_128.field_12_mood = 0;
    field_128.field_18 = -1;
    field_144 = 0;

    // Set Abe to be the current player controlled object
    sControlledCharacter_5C1B8C = this;

    // Create shadow
    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    return this;
}

void Abe::dtor_44B380()
{
    SetVTable(this, 0x5457BC); // gVTbl_Abe_5457BC

    BaseGameObject* pField_148 = sObjectIds_5C1B70.Find_449CF0(field_148);
    BaseGameObject* pField_14C = sObjectIds_5C1B70.Find_449CF0(field_14C);
    BaseGameObject* pField_150 = sObjectIds_5C1B70.Find_449CF0(field_150);
    BaseGameObject* pField_154 = sObjectIds_5C1B70.Find_449CF0(field_154);
    BaseGameObject* pField_158 = sObjectIds_5C1B70.Find_449CF0(field_158);
    BaseGameObject* pField_15C = sObjectIds_5C1B70.Find_449CF0(field_15C);
    BaseGameObject* pField_160 = sObjectIds_5C1B70.Find_449CF0(field_160);
    BaseGameObject* pField_178 = sObjectIds_5C1B70.Find_449CF0(field_178);

    SND_SEQ_Stop_4CAE60(0xAu);

    if (pField_148)
    {
        pField_148->field_6_flags.Set(BaseGameObject::eDead);
        field_148 = -1;
    }

    if (pField_160)
    {
        field_160 = -1;
    }

    if (pField_15C)
    {
        pField_15C->field_6_flags.Set(BaseGameObject::eDead);
        field_15C = -1;
    }

    if (pField_14C)
    {
        pField_14C->field_6_flags.Set(BaseGameObject::eDead);
        field_14C = -1;
    }

    if (pField_150)
    {
        pField_150->field_6_flags.Set(BaseGameObject::eDead);
        field_150 = -1;
    }

    if (pField_154)
    {
        field_154 = -1;
    }

    if (pField_158)
    {
        pField_158->field_6_flags.Set(BaseGameObject::eDead);
        field_158 = -1;
    }

    if (pField_178)
    {
        pField_178->field_6_flags.Set(BaseGameObject::eDead);
        field_178 = -1;
    }

    field_164 = -1;

    sActiveHero_5C1B68 = spAbe_554D5C;

    dtor_4080B0();
}

signed int CC Abe::CreateFromSaveState_44D4F0(const BYTE* /*a1*/)
{
    NOT_IMPLEMENTED();
    return 216;
}

void Abe::VDestructor(signed int flags)
{
    vdtor_44B350(flags);
}

void Abe::VUpdate()
{
    Update_449DC0();
}

void Abe::VRender(int** pOrderingTable)
{
    vRender_44B580(pOrderingTable);
}

void Abe::VScreenChanged()
{
    vScreenChanged_44D240();
}

int Abe::GetSaveState_4DC110(BYTE* pSaveBuffer)
{
    return vGetSaveState_457110(pSaveBuffer);
}

__int16 Abe::Vsub_408730(int arg0)
{
    return vsub_Kill_44BB50(reinterpret_cast<BaseAliveGameObject*>(arg0));
}

__int16 Abe::Vnull_4087F0(int a2a)
{
    return vsub_44B5D0(a2a);
}

int Abe::Vsub_408FD0(__int16 a2)
{
    return vsub_44E970(a2);
}

int Abe::Vnull_4081F0()
{
    return reinterpret_cast<int>(vsub_45A570());
}

void Abe::vdtor_44B350(signed int flags)
{
    dtor_44B380();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

const FP sAbe_xVel_table_545770[8] = 
{
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP(0), 
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP(0),
    FP_FromInteger(4)
};

const FP sAbe_yVel_table_545790[8] =
{
    FP(0), 
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4)
};

const InputCommands sInputKey_Run_5550E8 = eRun;

ALIVE_VAR(1, 0x5c1bda, short, word_5C1BDA, 0);


void Abe::Update_449DC0()
{
    NOT_IMPLEMENTED();

    if (word_5C1BDA) // Some flag to reset HP?
    {
        field_114_flags &= ~0x40u;
        field_10C_health = FP_FromDouble(1.0);
    }

    if (field_114_flags & 0x100)
    {
        field_114_flags &= ~0x100;
        if (field_104 != -1)
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104);

            field_104 = -1;
        }

        field_110 = BaseGameObject::Find_Flags_4DC170(field_110);
        field_148 = BaseGameObject::Find_Flags_4DC170(field_148);
        field_14C = BaseGameObject::Find_Flags_4DC170(field_14C);
        field_1A8 = BaseGameObject::Find_Flags_4DC170(field_1A8);
        field_150 = BaseGameObject::Find_Flags_4DC170(field_150);
        field_154 = BaseGameObject::Find_Flags_4DC170(field_154);
        field_158 = BaseGameObject::Find_Flags_4DC170(field_158);
        field_15C = BaseGameObject::Find_Flags_4DC170(field_15C);
        field_160 = BaseGameObject::Find_Flags_4DC170(field_160);
        field_164 = BaseGameObject::Find_Flags_4DC170(field_164);

        if (!(field_114_flags & 0x80))
        {
            if (!field_170)
            {
                field_170 = sGnFrame_5C1B84 + 2;
            }
            /*
            pClass_545A60Mem = (BaseGameObject *)malloc_4954D0(0x4Cu);
            pClass_545A60Mem_Copy = pClass_545A60Mem;
            unknown = 0;
            if (pClass_545A60Mem)
            {
                pClass_545A60 = Class_545A60::ctor_45F280(pClass_545A60Mem, (int)this);
            }
            else
            {
                pClass_545A60 = 0;
            }
            l_field_8_flags_ex = pClass_545A60->field_8_flags_ex;
            unknown = -1;
            this->field_178 = l_field_8_flags_ex;
            Class_545A60::sub_45FA00((int)pClass_545A60);
            */
        }
    }

    if (field_1AC_flags & 0x800)
    {
        return;
    }

    if (word_5C1BDA)
    {
        field_10C_health = FP_FromDouble(1.0);
    }

    if (!Input_IsChanting_45F260())
    {
        field_1AC_flags &= ~0x20u;
        field_174 = 1;
    }

    const int totalAliveSavedMuds = sRescuedMudokons_5C1BC2 - sKilledMudokons_5C1BC0;
    if (totalAliveSavedMuds == 0)
    {
        // "Normal" voice
        field_128.field_12_mood = 5;
    }
    else if (totalAliveSavedMuds >= 0)
    {
        // "Happy" voice
        field_128.field_12_mood = 0;
    }
    else if (totalAliveSavedMuds <= 0)
    {
        // "Sad" voice
        field_128.field_12_mood = 3;
    }

    if (!sDDCheat_FlyingEnabled_5C2C08 || sControlledCharacter_5C1B8C != this)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    
        short state_idx = field_106_animation_num;

        // Execute the current state
        (this->*(sAbeStateMachineTable_554910)[state_idx])();

        if (field_114_flags & 0x100 || field_1AC_flags & 0x10)
        {
            return;
        }

        if (field_100_pCollisionLine)
        {
            // Snap to a whole number so we are "on" the line
            field_BC_ypos.RemoveFractional();
        }

        /*
        if (xpos != field_B8_xpos || ypos != field_BC_ypos)
        {
            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                0,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);
            ((void(__thiscall *)(Abe *, Path_TLV *))field_0_VTbl->VAbe.field_0.field_50)(this, field_FC_pPathTLV);
        }
        */

        if (field_114_flags & 1)
        {
            state_idx = field_122;
            Knockback_44E700(1, 1);
            if (state_idx != -1)
            {
                field_106_animation_num = state_idx;
            }

            field_108 = 0;
            field_1AC_flags &= ~2u;
            field_122 = 0;
            field_114_flags = (field_114_flags & ~1) | 2;
        }

        BaseGameObject* pScreenShake = Event_Get_422C00(kEventScreenShake);
        if (pScreenShake && field_10C_health > FP(0))
        {
            if (sub_449D30())
            {
                Knockback_44E700(1, 0);
            }
        }

        if (field_128.field_18 < 0 || static_cast<int>(sGnFrame_5C1B84) < field_144)
        {
            //LABEL_75:
            if (state_idx != field_106_animation_num || field_114_flags & 2)
            {
                field_114_flags &= ~2u;
                if (field_106_animation_num != 12 && !(field_1AC_flags & 0x10))
                {
                    field_20_animation.Set_Animation_Data_409C80(
                        sAbeFrameOffsetTable_554B18[field_106_animation_num],
                        StateToAnimResource_44AAB0(field_106_animation_num));

                    field_128.field_14 = sGnFrame_5C1B84;

                    if (state_idx == 12 || state_idx == 60)
                    {
                        field_20_animation.SetFrame_409D50(field_F6);
                    }
                }
            }

            if (field_1AC_flags & 2)
            {
                field_106_animation_num = field_F4;
                field_20_animation.Set_Animation_Data_409C80(
                    sAbeFrameOffsetTable_554B18[field_106_animation_num],
                    StateToAnimResource_44AAB0(field_106_animation_num));

                field_128.field_14 = sGnFrame_5C1B84;
                field_20_animation.SetFrame_409D50(field_F6);
                field_1AC_flags &= ~2u;
            }

            if (field_128.field_4 <= static_cast<int>(sGnFrame_5C1B84)  && field_10C_health > FP(0))
            {
                field_10C_health = FP_FromDouble(1.0);
            }

            if (field_168)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
                {
                    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                    {
                        if (static_cast<int>(sGnFrame_5C1B84) <= field_168)
                        {
                            if (!(sGnFrame_5C1B84 % 32))
                            {
                                int v27 = 0;
                                if (field_16C)
                                {
                                    v27 = 4;
                                }
                                else if (field_16E)
                                {
                                    v27 = 7;
                                }
                                else if (field_1AC_flags & 0x4000)
                                {
                                    v27 = 14;
                                }
                                /*
                                v28 = ((int(__thiscall *)(Abe *, char *, signed int))this->field_0_mBase.field_0_mBase.field_0_mBase.field_0_VTbl->VBaseAliveGameObject.field_0_mBase.field_0_mBase.field_1C_update_delay)(
                                    this,
                                    &v38,
                                    1);
                                pClass_545A60Mem_Copy = *(BaseGameObject **)v28;
                                v37 = *(_DWORD *)(v28 + 4);
                                AbilityRing::ctor_482F80(
                                    ((signed __int16)pClass_545A60Mem_Copy + (signed __int16)v37) / 2 << 16,
                                    (SHIWORD(pClass_545A60Mem_Copy) + SHIWORD(v37)) / 2 << 16,
                                    v27,
                                    this->field_0_mBase.field_0_mBase.field_CC_sprite_scale);
                                */
                                SFX_Play_46FBA0(0x11u, 25, 2650, 0x10000);
                            }
                        }
                        else
                        {
                            if (field_168 > 0 && field_16C > 0)
                            {
                                sub_45AA90();
                            }
                            field_168 = 0;
                        }
                    }
                }
            }


            BaseGameObject* pObj_field_178 = sObjectIds_5C1B70.Find_449CF0(field_178);
            if (pObj_field_178 && field_170 > 0)
            {
                if (static_cast<int>(sGnFrame_5C1B84) > field_170)
                {
                    field_170 = 0;
                    //sub_45FA30(pObj_field_178);
                }
            }

            if (field_1AC_flags < 0)
            {
                if (field_1AE & 1)
                {
                    if (gMap_5C3030.sCurrentLevelId_5C3030 == 2)
                    {
                        field_168 = sGnFrame_5C1B84 + 200000;
                        field_16C = 0;
                        field_16E = 0;
                        field_1AE &= ~1;
                        field_1AC_flags &= ~0xC000u;
                        field_1AC_flags |= 0x4000u;
                    }
                }
            }

            if (Event_Get_422C00(kEventMudokonDied))
            {
                field_128.field_18 = 14;
                field_144 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
                /*
                pMusicTrigger =  (BaseGameObject *)malloc_4954D0(0x34u);
                pClass_545A60Mem_Copy = pMusicTrigger;
                unknown = 1;
                if (pMusicTrigger)
                {
                    MusicTrigger::ctor_47FF10(pMusicTrigger, 1, 0, 90, 0);
                }
                unknown = -1;
                */
            }

            if (Event_Get_422C00(kEventMudokonComfort))
            {
                field_128.field_18 = 8;
                field_144 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
            }

            //if (Event_Get_422C00(kEventMudokonComfort | kEventSpeaking))
            {
                /*
                this->field_128.field_18 = 14;
                this->field_144 = sGnFrame_5C1B84 + Math_RandomRange_496AB0(22, 30);
                */
            }

            if (!(field_1AE & 2))
            {
                return;
            }

          
            field_1AE &= ~2;
            sActiveQuicksaveData_BAF7F8.field_204_world_info.field_A_unknown_1 = static_cast<short>(field_1B0_save_num);
            Quicksave_SaveWorldInfo_4C9310(&sActiveQuicksaveData_BAF7F8.field_244_restart_path_world_info);
            vGetSaveState_457110(reinterpret_cast<BYTE*>(&sActiveQuicksaveData_BAF7F8.field_284_restart_path_abe_state));
            memcpy(
                sActiveQuicksaveData_BAF7F8.field_35C_restart_path_switch_states.mData,
                sSwitchStates_5C1A28.mData,
                sizeof(sActiveQuicksaveData_BAF7F8.field_35C_restart_path_switch_states.mData));
            Quicksave_4C90D0();
            return;
        }

        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0)
            || (field_106_animation_num == 112)
            || field_106_animation_num == 7
            || field_106_animation_num == 8
            || field_106_animation_num == 9
            || field_106_animation_num == 10)
        {
        //LABEL_74:
            field_128.field_18 = -1;
            //goto LABEL_75;
        }

        if (field_106_animation_num == 0 || field_106_animation_num == 12)
        {
            field_114_flags |= 2u;
            switch (field_128.field_18)
            {
            case 14:
                field_106_animation_num = 34;
                break;
            case 5:
                field_106_animation_num = 10;
                break;
            case 28:
                field_106_animation_num = 10;
                break;
            default:
                field_106_animation_num = 9;
                break;
            }
        }

        if (field_128.field_18 == 5)
        {
            Event_Broadcast_422BC0(kEventMudokonLaugh, sActiveHero_5C1B68);
        }

        if (field_128.field_18 == 28)
        {
            Abe_SFX_457EC0(static_cast<unsigned char>(field_128.field_18), 80, 0, this);
        }
        else
        {
            Abe_SFX_457EC0(static_cast<unsigned char>(field_128.field_18), 0, 0, this);
        }

        /*
        goto LABEL_74;
        */
    }

    /*
    // vcall 23
    ((void(__thiscall *)(Abe *))this->field_0_mBase.field_0_mBase.field_0_mBase.field_0_VTbl->VAbe.field_0.field_5C)(this);
    */
    Vnull_4081F0();

    field_F8 = field_BC_ypos;
    field_1AC_flags &= ~0x10u;
    field_106_animation_num = 85;
    field_100_pCollisionLine = nullptr;

    if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (eRight | eLeft | eDown | eUp))
    {
        field_C4_velx = sAbe_xVel_table_545770[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
        field_C8_vely = sAbe_yVel_table_545790[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
        
        if (sInputKey_Run_5550E8 & sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
        {
            field_C4_velx += sAbe_xVel_table_545770[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            field_C4_velx += sAbe_xVel_table_545770[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
            field_C8_vely += sAbe_yVel_table_545790[(unsigned __int8)sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
        }

        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;

        // Keep within map max min bounds
        if (field_B8_xpos < FP(0))
        {
            field_B8_xpos = FP(0);
        }

        if (field_BC_ypos < FP(0))
        {
            field_BC_ypos = FP(0);
        }

        // Keep within map max bounds
        PSX_Point mapSize = {};
        gMap_5C3030.Get_map_size_480640(&mapSize);

        FP mapWidth = FP_FromInteger(mapSize.field_0_x);
        if (field_B8_xpos >= mapWidth)
        {
            field_B8_xpos = mapWidth - FP_FromDouble(1.0);
        }

        FP mapHeight = FP_FromInteger(mapSize.field_2_y);
        if (field_BC_ypos >= mapHeight)
        {
            field_BC_ypos = mapHeight - FP_FromDouble(1.0);
            sub_408C40();
            return;
        }
    }
    else
    {
        field_C4_velx = FP(0);
        field_C8_vely = FP(0);
    }
    
    sub_408C40();
}

int Abe::vsub_44E970(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseGameObject* Abe::vsub_45A570()
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void Abe::Knockback_44E700(__int16 /*a2*/, __int16 /*a3*/)
{
    NOT_IMPLEMENTED();
}

void Abe::vRender_44B580(int** pOrderingTable)
{
    if (!(field_1AC_flags & 0x10))
    {
        field_20_animation.field_14_scale = field_CC_sprite_scale;
    }

    if (field_106_animation_num != 79 && field_106_animation_num != 82 && field_106_animation_num != 76)
    {
        Render_424B90(pOrderingTable);
    }
}

void Abe::vScreenChanged_44D240()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        field_C2_lvl_number = gMap_5C3030.field_A_5C303A_levelId;
        field_C0_path_number = gMap_5C3030.field_C_5C303C_pathId;
    }

    // Level has changed?
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        if (gMap_5C3030.field_A_5C303A_levelId == 1 && !word_5C1BA0)
        {
            field_128.field_18 = 3;
            field_144 = sGnFrame_5C1B84 + 35;
        }

        // Set the correct tint for this map
        SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.field_A_5C303A_levelId);

        if (gMap_5C3030.sCurrentLevelId_5C3030)
        {
            if (field_1A2_rock_or_bone_count > 0)
            {
                if (gpThrowableArray_5D1E2C)
                {
                    gpThrowableArray_5D1E2C->Remove_49AA00(field_1A2_rock_or_bone_count);
                }
            }
            
            field_1A2_rock_or_bone_count = 0;
            
            if (field_168 > 0 && field_16C)
            {
                sub_45AA90();
            }

            field_168 = 0;
        }

        if (gMap_5C3030.field_A_5C303A_levelId == 2)
        {
            if (gMap_5C3030.sCurrentLevelId_5C3030 == 7)
            {
                field_1AC_flags |= 0x8000u;
            }

            if (gMap_5C3030.sCurrentLevelId_5C3030 == 11)
            {
                field_1AE |= 1u;
            }
        }

        if (gMap_5C3030.field_A_5C303A_levelId == 16 || gMap_5C3030.field_A_5C303A_levelId == 0)
        {
            // Remove Abe for menu/credits levels?
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_168 = 0;
        if (gMap_5C3030.sCurrentLevelId_5C3030)
        {
            field_198_has_evil_fart = 0;
        }
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId && !(field_114_flags & 0x100))
    {
        for (BYTE& val : sSavedKilledMudsPerPath_5C1B50.mData)
        {
            val = 0;
        }
        byte_5C1B64 = 0;
    }
}

int Abe::vGetSaveState_457110(BYTE* /*pSaveBuffer*/)
{
    NOT_IMPLEMENTED();
    return 216;
}

bool Abe::vsub_Kill_44BB50(BaseGameObject * /*otherObj*/)
{
    NOT_IMPLEMENTED();
    return false;
}

__int16 Abe::vsub_44B5D0(int /*a2a*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int Abe::sub_44B7B0()
{
#ifdef STUPID_FUN
    // THIS IS A HACK TO MAKE ABE POSSESS ANYTHING :D
    for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
    {
        BaseAliveGameObject* pBaseGameObject = reinterpret_cast<BaseAliveGameObject*>(gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx));
        if (!pBaseGameObject)
        {
            break;
        }

        if (pBaseGameObject->field_4_typeId == 83)
        {
            pBaseGameObject->field_6_flags |= 4;
        }

        PSX_Point currentScreenCoords;
        gMap_5C3030.GetCurrentCamCoords_480680(&currentScreenCoords);
        if (pBaseGameObject != sActiveHero_5C1B68 && pBaseGameObject->field_6_flags & BaseGameObject::eIsBaseAliveGameObject &&
            pBaseGameObject->field_B8_xpos.GetExponent() > currentScreenCoords.field_0_x && pBaseGameObject->field_B8_xpos.GetExponent() < currentScreenCoords.field_0_x + 350
            && pBaseGameObject->field_BC_ypos.GetExponent() > currentScreenCoords.field_2_y && pBaseGameObject->field_BC_ypos.GetExponent() < currentScreenCoords.field_2_y + 240)
        {
            return (int)pBaseGameObject;
        }
    }
    return 0;
#else
    NOT_IMPLEMENTED();
#endif
}

void Abe::Load_Basic_Resources_44D460()
{
    NOT_IMPLEMENTED();
}

void Abe::Free_Resources_44D420()
{
    NOT_IMPLEMENTED();
}

EXPORT BOOL Abe::sub_449D30()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Abe::sub_45AA90()
{
    ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(25));
    field_10_resources_array.SetAt(25, nullptr);

    ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(26));
    field_10_resources_array.SetAt(26, nullptr);

    ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(27));
    field_10_resources_array.SetAt(27, nullptr);
}


BYTE ** Abe::StateToAnimResource_44AAB0(signed int /*state*/)
{
    NOT_IMPLEMENTED();
}


void Abe::State_Idle_44EEB0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_WalkLoop_44FBA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_StandingTurn_451830()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Fall_459B60()
{
    NOT_IMPLEMENTED();
}

void Abe::State_WalkEndLeftFoot_44FFC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_WalkEndRightFoot_00450080()
{
    NOT_IMPLEMENTED();
}

void Abe::State_WalkBegin_44FEE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45B140()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45B160()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45B180()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Fart_45B1A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Speak_45B0A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Null_4569C0()
{

}

void Abe::State_HoistBegin_452B20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_HoistIdle_452440()
{
    NOT_IMPLEMENTED();
}

void Abe::State_HoistLand_452BA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LandSoft_45A360()
{
    NOT_IMPLEMENTED();
}

void Abe::State_CrouchIdle_456BC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_CrouchToStand_454600()
{
    NOT_IMPLEMENTED();
}

void Abe::State_StandToCrouch_453DC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_454550()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_4545E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RollBegin_4539A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RollLoop_453A90()
{
    NOT_IMPLEMENTED();
}

void Abe::State_453D00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunSlideStop_451330()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunTurn_451500()
{
    NOT_IMPLEMENTED();
}

void Abe::State_HopBegin_4521C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_HopMid_451C50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_HopLand_4523D0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunJumpBegin_4532E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunJumpMid_452C10()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunJumpLand_453460()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunLoop_4508E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_DunnoBegin_44ECF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_DunnoEnd_44ED10()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Null_45BC50()
{

}

void Abe::State_CrouchTurn_454390()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_RollBegin_453A70()
{
    NOT_IMPLEMENTED();
}

void Abe::State_StandingToRun_450D40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_SneakLoop_450550()
{
    NOT_IMPLEMENTED();
}

void Abe::State_450250()
{
    NOT_IMPLEMENTED();
}

void Abe::State_4503D0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_450380()
{
    NOT_IMPLEMENTED();
}

void Abe::State_450500()
{
    NOT_IMPLEMENTED();
}

void Abe::State_SneakBegin_4507A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_SneakEnd_450870()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_SneakEnd_4508C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_4500A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_450200()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunToWalk1_450E20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunToWalk2_450F50()
{
    NOT_IMPLEMENTED();
}

void Abe::State_451710()
{
    NOT_IMPLEMENTED();
}

void Abe::State_451800()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunJumpLandRun_4538F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RunJumpLandWalk_453970()
{
    NOT_IMPLEMENTED();
}

void Abe::State_4591F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Dead_4589A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_DeadPre_4593E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Null_459450()
{

}

void Abe::State_4A3200()
{
    NOT_IMPLEMENTED();
}

void Abe::State_TurnToRun_456530()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Punch_454750()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Sorry_454670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_454730()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LedgeAscend_4548E0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LedgeDescend_454970()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LedgeHang_454E20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_454B80()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LedgeHangWobble_454EF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RingRopePull_455AF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Knockback_455090()
{
    NOT_IMPLEMENTED();
}

void Abe::State_KnockbackGetUp_455340()
{
    NOT_IMPLEMENTED();
}

void Abe::State_PushWall_4553A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_455290()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45C7B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45CA40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45D130()
{
    NOT_IMPLEMENTED();
}

void Abe::State_WellBegin_45C810()
{
    NOT_IMPLEMENTED();
}

void Abe::State_WellInside_45CA60()
{
    NOT_IMPLEMENTED();
}

void Abe::State_WellShotOut_45D150()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_WellBegin_45C7F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45CC80()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45CF70()
{
    NOT_IMPLEMENTED();
}

void Abe::State_FallLandDie_45A420()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_Fall_455070()
{
    NOT_IMPLEMENTED();
}

void Abe::State_HandstoneBegin_45BD00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_HandstoneEnd_45C4F0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_GrenadeMachineUse_45C510()
{
    NOT_IMPLEMENTED();
}

void Abe::State_BrewMachineBegin_4584C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_BrewMachineEnd_4585B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RingRopePullEnd_4557B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_455800()
{
    NOT_IMPLEMENTED();
}

void Abe::State_FallLedgeBegin_455970()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_FallLedgeBegin_4559A0()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_FallLedgeBegin_4559C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_4559E0()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_FallLedgeBegin_455A80()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_FallLedgeBegin_455AA0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LeverUse_455AC0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_455B60()
{
    NOT_IMPLEMENTED();
}

void Abe::State_KnockForward_455420()
{
    NOT_IMPLEMENTED();
}

void Abe::State_455310()
{
    NOT_IMPLEMENTED();
}

void Abe::jState_KnockbackGetUp_455380()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RockThrowStandingHold_455DF0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RockThrowStandingThrow_456460()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RockThrowStandingEnd_455F20()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RockThrowCrouchingHold_454410()
{
    NOT_IMPLEMENTED();
}

void Abe::State_RockThrowCrouchingThrow_454500()
{
    NOT_IMPLEMENTED();
}

void Abe::State_455550()
{
    NOT_IMPLEMENTED();
}

void Abe::State_455670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_GrabRock_4564A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_Chant_45B1C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_ChantEnd_45BBE0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_DoorEnter_459470()
{
    NOT_IMPLEMENTED();
}

void Abe::State_DoorExit_459A40()
{
    NOT_IMPLEMENTED();
}

void Abe::State_MineCarEnter_458780()
{
    NOT_IMPLEMENTED();
}

void Abe::State_4587C0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_MineCarExit_458890()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45A990()
{
    NOT_IMPLEMENTED();
}

void Abe::State_45AB00()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LiftGrabBegin_45A600()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LiftGrabEnd_45A670()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LiftGrabIdle_45A6A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LiftUseUp_45A780()
{
    NOT_IMPLEMENTED();
}

void Abe::State_LiftUseDown_45A7B0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_TurnWheelBegin_456700()
{
    NOT_IMPLEMENTED();
}

void Abe::State_TurnWheelLoop_456750()
{
    NOT_IMPLEMENTED();
}

void Abe::State_TurnWheelEnd_4569A0()
{
    NOT_IMPLEMENTED();
}

void Abe::State_PoisonGasDeath_4565C0()
{
    NOT_IMPLEMENTED();
}

// TODO: Clean up
EXPORT void CC Abe_SFX_457EC0(unsigned __int8 idx, __int16 volume, int pitch, Abe* pHero)
{
    //DEV_CONSOLE_PRINTF("Abe SFX: %i", idx);

    switch (idx)
    {
    case 14u:
        if (pHero && pHero->field_CC_sprite_scale.GetDouble() == 0.5)
        {
            SND_SEQ_Play_4CAB10(0x10u, 1, 90, 90);
        }
        else
        {
            SND_SEQ_Play_4CAB10(0x10u, 1, 127, 127);
        }
        break;
    case 26u:
        if (pHero && pHero->field_CC_sprite_scale.GetDouble() == 0.5)
        {
            SND_SEQ_Play_4CAB10(0x12u, 1, 80, 80);
        }
        else
        {
            SND_SEQ_Play_4CAB10(0x12u, 1, 115, 115);
        }
        break;
    case 8u:
        if (pHero == sActiveHero_5C1B68 && gMap_5C3030.sCurrentLevelId_5C3030 == 10)
        {
            idx = 10;
        }
        // Fall through
    default:
        if (!volume)
        {
            volume = sAbeSFXList_555250[idx].field_3_default_volume;
        }

        // OG bug - isn't null checked in other cases before de-ref?
        if (!pHero)
        {
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume, pitch, pitch);
            return;
        }

        if (pHero->field_CC_sprite_scale.GetDouble() == 0.5)
        {
            volume = 2 * volume / 3;
        }

        if (pHero == sActiveHero_5C1B68)
        {
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume, pitch, pitch);
            return;
        }

        switch (gMap_5C3030.sub_4811A0(
            pHero->field_C2_lvl_number,
            pHero->field_C0_path_number,
            pHero->field_B8_xpos,
            pHero->field_BC_ypos))
        {
        case 0:
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume, pitch, pitch);
            break;
        case 1:
        case 2:
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], 2 * volume / 3, pitch, pitch);
            break;
        case 3:
            // TODO: Deoptimise math
            SFX_SfxDefinition_Play_4CA700(
                &sAbeSFXList_555250[idx],
                ((unsigned int)((unsigned __int64)(2863311532i64 * (signed __int16)volume) >> 32) >> 31)
                + ((unsigned __int64)(2863311532i64 * (signed __int16)volume) >> 32),
                2 * (signed __int16)volume / 9,
                pitch,
                pitch);
            break;
        case 4:
            // TODO: Deoptimise math
            SFX_SfxDefinition_Play_4CA700(
                &sAbeSFXList_555250[idx],
                ((unsigned int)((unsigned __int64)(1908874354i64 * (signed __int16)volume) >> 32) >> 31)
                + ((signed int)((unsigned __int64)(1908874354i64 * (signed __int16)volume) >> 32) >> 1),
                2 * (signed __int16)volume / 3,
                pitch,
                pitch);
            break;
        default:
            break;
        }
    }
}