#include "stdafx_ao.h"
#include "Function.hpp"
#include "Elum.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"
#include "LiftPoint.hpp"

START_NS_AO;

ALIVE_VAR(1, 0x507680, Elum*, gElum_507680, nullptr);

using TElumStateFunction = decltype(&Elum::State_0_Respawn_414C60);

const TElumStateFunction sElum_motion_table_4C5148[] =
{
    &Elum::State_0_Respawn_414C60,
    &Elum::State_1_Idle_412990,
    &Elum::State_2_Unknown_412C30,
    &Elum::State_3_WalkLoop_412C90,
    &Elum::State_4_Turn_4140F0,
    &Elum::State_5_WalkToIdle_4132D0,
    &Elum::State_6_MidWalkToIdle_4133F0,
    &Elum::State_7_Unknown_413200,
    &Elum::State_8_IdleToWalk_413270,
    &Elum::State_9_ToYell_415890,
    &Elum::State_10_Yell_4158E0,
    &Elum::State_11_Unknown_4159A0,
    &Elum::State_12_RunTurn_414520,
    &Elum::State_13_RunTurnToWalk_4147C0,
    &Elum::State_14_Speak_414860,
    &Elum::State_15_Speak_4148F0,
    &Elum::State_16_Speak_414980,
    &Elum::State_17_Unknown_413620,
    &Elum::State_18_Unknown_4136A0,
    &Elum::State_19_Dead_415F90,
    &Elum::State_20_Fall_415F70,
    &Elum::State_21_Land_414A20,
    &Elum::State_22_RunOffEdge_415810,
    &Elum::State_23_WalkOffEdge_415E90,
    &Elum::State_24_JumpToFall_415ED0,
    &Elum::State_25_LickingHoney_415B50,
    &Elum::State_26_LickingToStruggling_415AC0,
    &Elum::State_27_AbeMountingEnd_415CA0,
    &Elum::State_28_AbeUnmountingEnd_415D60,
    &Elum::State_29_BeesStruggling_412A90,
    &Elum::State_30_HopBegin_414E30,
    &Elum::State_31_HopMid_414C70,
    &Elum::State_32_HopLand_415140,
    &Elum::State_33_RunJumpBegin_415400,
    &Elum::State_34_RunJumpMid_415240,
    &Elum::State_35_RunJumpLand_415580,
    &Elum::State_36_RunLoop_413720,
    &Elum::State_37_RunSlideStop_4142E0,
    &Elum::State_38_RunTurnToRun_414810,
    &Elum::State_39_IdleToRun_413B00,
    &Elum::State_40_WalkToRun_4134B0,
    &Elum::State_41_MidWalkToRun_413560,
    &Elum::State_42_RunToWalk_413B60,
    &Elum::State_43_MidRunToWalk_413E20,
    &Elum::State_44_ScratchBegin_412730,
    &Elum::State_45_ScratchLoop_4127B0,
    &Elum::State_46_ScratchEnd_412800,
    &Elum::State_47_Unknown_415A30,
    &Elum::State_48_AbeMoutingBegin_415C40,
    &Elum::State_49_AbeUnmountingBegin_415D00,
    &Elum::State_50_Knockback_415DC0
};


BaseGameObject* Elum::VDestructor(signed int flags)
{
    return Vdtor_411710(flags);
}

Elum* Elum::Vdtor_411710(signed int flags)
{
    dtor_410BC0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

EXPORT BaseGameObject *Elum::dtor_410BC0()
{
    SetVTable(this, 0x4BA8F8);

    for (BYTE**& ppRes : field_174_resources.res)
    {
        if (ppRes && field_10_anim.field_20_ppBlock != ppRes)
        {
            ResourceManager::FreeResource_455550(ppRes);
        }
    }

    gElum_507680 = nullptr;
    Vsub_412700();

    ResourceManager::FreeResource_455550(
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 115, 0, 0)
    );

    if (field_104_pending_resource_count)
    {
        ResourceManager::CancelPendingResourcesFor_41EA60(this);
    }
    field_104_pending_resource_count = 0;

    const int anims[] = { 230, 222, 220, 221 };
    for (int anim : anims)
    {
        ResourceManager::FreeResource_455550(
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, anim, 1, 0)
        );
    }

    return dtor_base_416FE0();
}

void Elum::Vsub_412700()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }
}

BaseAliveGameObject* Elum::dtor_base_416FE0()
{
    SetVTable(this, 0x4BA970);
    return dtor_401000();
}


void Elum::State_0_Respawn_414C60()
{
    NOT_IMPLEMENTED();
}

void Elum::State_1_Idle_412990()
{
    NOT_IMPLEMENTED();
}

void Elum::State_2_Unknown_412C30()
{
    NOT_IMPLEMENTED();
}

void Elum::State_3_WalkLoop_412C90()
{
    NOT_IMPLEMENTED();
}

void Elum::State_4_Turn_4140F0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_5_WalkToIdle_4132D0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_6_MidWalkToIdle_4133F0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_7_Unknown_413200()
{
    NOT_IMPLEMENTED();
}

void Elum::State_8_IdleToWalk_413270()
{
    NOT_IMPLEMENTED();
}

void Elum::State_9_ToYell_415890()
{
    NOT_IMPLEMENTED();
}

void Elum::State_10_Yell_4158E0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_11_Unknown_4159A0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_12_RunTurn_414520()
{
    NOT_IMPLEMENTED();
}

void Elum::State_13_RunTurnToWalk_4147C0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_14_Speak_414860()
{
    NOT_IMPLEMENTED();
}

void Elum::State_15_Speak_4148F0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_16_Speak_414980()
{
    NOT_IMPLEMENTED();
}

void Elum::State_17_Unknown_413620()
{
    NOT_IMPLEMENTED();
}

void Elum::State_18_Unknown_4136A0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_19_Dead_415F90()
{
    NOT_IMPLEMENTED();
}

void Elum::State_20_Fall_415F70()
{
    NOT_IMPLEMENTED();
}

void Elum::State_21_Land_414A20()
{
    NOT_IMPLEMENTED();
}

void Elum::State_22_RunOffEdge_415810()
{
    NOT_IMPLEMENTED();
}

void Elum::State_23_WalkOffEdge_415E90()
{
    NOT_IMPLEMENTED();
}

void Elum::State_24_JumpToFall_415ED0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_25_LickingHoney_415B50()
{
    NOT_IMPLEMENTED();
}

void Elum::State_26_LickingToStruggling_415AC0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_27_AbeMountingEnd_415CA0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_28_AbeUnmountingEnd_415D60()
{
    NOT_IMPLEMENTED();
}

void Elum::State_29_BeesStruggling_412A90()
{
    NOT_IMPLEMENTED();
}

void Elum::State_30_HopBegin_414E30()
{
    NOT_IMPLEMENTED();
}

void Elum::State_31_HopMid_414C70()
{
    NOT_IMPLEMENTED();
}

void Elum::State_32_HopLand_415140()
{
    NOT_IMPLEMENTED();
}

void Elum::State_33_RunJumpBegin_415400()
{
    NOT_IMPLEMENTED();
}

void Elum::State_34_RunJumpMid_415240()
{
    NOT_IMPLEMENTED();
}

void Elum::State_35_RunJumpLand_415580()
{
    NOT_IMPLEMENTED();
}

void Elum::State_36_RunLoop_413720()
{
    NOT_IMPLEMENTED();
}

void Elum::State_37_RunSlideStop_4142E0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_38_RunTurnToRun_414810()
{
    NOT_IMPLEMENTED();
}

void Elum::State_39_IdleToRun_413B00()
{
    NOT_IMPLEMENTED();
}

void Elum::State_40_WalkToRun_4134B0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_41_MidWalkToRun_413560()
{
    NOT_IMPLEMENTED();
}

void Elum::State_42_RunToWalk_413B60()
{
    NOT_IMPLEMENTED();
}

void Elum::State_43_MidRunToWalk_413E20()
{
    NOT_IMPLEMENTED();
}

void Elum::State_44_ScratchBegin_412730()
{
    NOT_IMPLEMENTED();
}

void Elum::State_45_ScratchLoop_4127B0()
{
    NOT_IMPLEMENTED();
}

void Elum::State_46_ScratchEnd_412800()
{
    NOT_IMPLEMENTED();
}

void Elum::State_47_Unknown_415A30()
{
    NOT_IMPLEMENTED();
}

void Elum::State_48_AbeMoutingBegin_415C40()
{
    NOT_IMPLEMENTED();
}

void Elum::State_49_AbeUnmountingBegin_415D00()
{
    NOT_IMPLEMENTED();
}

void Elum::State_50_Knockback_415DC0()
{
    NOT_IMPLEMENTED();
}

void Elum::VUpdate()
{
    VUpdate_4102A0();
}

void Elum::VRender(int** pOrderingTable)
{
    VRender_410E40(pOrderingTable);
}

void Elum::VScreenChanged()
{
    vScreenChange_411340();
}

void Elum::VUpdate_4102A0()
{
    NOT_IMPLEMENTED();
}

void Elum::VRender_410E40(int** ot)
{
    if (field_B2_lvl_number == gMap_507BA8.field_0_current_level
        && field_B0_path_number == gMap_507BA8.field_2_current_path
        && !field_8_update_delay)
    {
        VRender_417DA0(ot);
    }
}

void Elum::vScreenChange_411340()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x4C52F0, short, word_4C52F0, 0x88);
ALIVE_VAR(1, 0x4C52F2, short, word_4C52F2, 0x38);
ALIVE_VAR(1, 0x4C52F4, DWORD, dword_4C52F4, 0x1C92C);
ALIVE_VAR(1, 0x4C52F8, DWORD, dword_4C52F8, 0xC8);
ALIVE_VAR(1, 0x4C52FA, short, word_4C52FA, 0);

void Elum::Spawn_410E90(TlvItemInfoUnion tlvInfo)
{
    anythingForTheTimeBeing struct1;
    anythingForTheTimeBeing struct2;

    auto pElum = ao_new<Elum>();
    struct1.field_0 = word_4C52F2;
    struct1.field_2 = tlvInfo.parts.tlvOffset; //todo check it (if it's ever used)

    struct2.field_0 = word_4C52F0;
    struct2.field_2 = word_4C52FA;

    pElum->ctor_410870(dword_4C52F4, struct2, struct1, dword_4C52F8, tlvInfo);
}

Elum* Elum::ctor_410870(int, anythingForTheTimeBeing, anythingForTheTimeBeing, int, TlvItemInfoUnion /*tlvInfo*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

END_NS_AO;
