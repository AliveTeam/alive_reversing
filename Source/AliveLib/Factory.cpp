#include "stdafx.h"
#include "Factory.hpp"
#include "Function.hpp"
#include "MainMenu.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "LCDScreen.hpp"
#include "UXB.hpp"
#include "Mine.hpp"
#include "TimedMine.hpp"
#include "StatsSign.hpp"
#include "BackgroundAnimation.hpp"
#include "HoistRocksEffect.hpp"
#include "Alarm.hpp"
#include "Door.hpp"
#include "WorkWheel.hpp"
#include "StatusLight.hpp"
#include "WheelSyncer.hpp"
#include "LevelLoader.hpp"
#include "InvisibleSwitch.hpp"
#include "Switch.hpp"
#include "MultiSwitchController.hpp"
#include "ShadowZone.hpp"

template<size_t arraySize>
struct CompileTimeResourceList
{
    int field_0_count = arraySize;
    ResourceManager::ResourcesToLoadList_Entry field_4_items[arraySize];

    CompileTimeResourceList(std::initializer_list<ResourceManager::ResourcesToLoadList_Entry> elements)
    {
        std::copy(std::begin(elements), std::end(elements), std::begin(field_4_items));
    }

    // HACK: Cast to memory layout compatible type as we can't pass template types into
    // ResourceManager (this can be fixed when everything is decompiled by using a
    // more sane compile time resource list type).
    ResourceManager::ResourcesToLoadList* AsList()
    {
        return reinterpret_cast<ResourceManager::ResourcesToLoadList*>(this);
    }
};

EXPORT void CC Factory_MainMenuController_4D6DB0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (sMainMenuObjectCounter_BB4400 == 0)
    {
        if (loadmode == 1 || loadmode == 2)
        {
            static CompileTimeResourceList<3> kResources({
                { ResourceManager::Resource_Animation, kHighliteResID },
                { ResourceManager::Resource_Animation, kOptflareResID },
                { ResourceManager::Resource_Palt, kHighlitePalResID },
            });

            Map::LoadResourcesFromList_4DBE70("STARTANM.BND", kResources.AsList(), loadmode);
            Map::LoadResource_4DBE00("ABESPEK2.BAN", ResourceManager::Resource_Animation, kAbespek2ResID, loadmode);
        }
        else
        {
            auto pMainMenuController = alive_new<MainMenuController>();
            if (pMainMenuController)
            {
                pMainMenuController->ctor_4CE9A0(pTlv, tlvOffsetLevelIdPathId);
            }
        }
    }
}

EXPORT void CC Factory_ContinuePoint_4D6970(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_PathTransition_4D68A0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Hoist_4D9E90(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    Path_Hoist* pHoistTlv = static_cast<Path_Hoist*>(pTlv);
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, ResourceID::kAbehoistResID, loadmode, 0);
        Map::LoadResource_4DBE00("DRPROCK.BAN", ResourceManager::Resource_Animation, ResourceID::kDrpRockResID, loadmode, 0);
    }
    else if (pHoistTlv->field_10_type == Path_Hoist::Type::eOffScreen)
    {
        // Its an off screen hoist so create the falling rocks effect
        auto pHoistFallingRocks = alive_new<HoistRocksEffect>();
        if (pHoistFallingRocks)
        {
            pHoistFallingRocks->ctor_45D270(pHoistTlv, tlvOffsetLevelIdPathId.all);
        }
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

EXPORT void CC Factory_Edge_4D68C0(Path_TLV* /*pTlv*/, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{ 
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, ResourceID::kAbehoistResID, loadmode, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

EXPORT void CC Factory_DeathDrop_4D6930(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Door_4D6F00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABEDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kAbedoorResID, loadmode, FALSE);
        switch (gMap_5C3030.sCurrentLevelId_5C3030)
        {
        case LevelIds::eNecrum_2:
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            Map::LoadResource_4DBE00("SVZDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        case LevelIds::eFeeCoDepot_5:
        case LevelIds::eFeeCoDepot_Ender_12:
            Map::LoadResource_4DBE00("FDDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        case LevelIds::eBarracks_6:
        case LevelIds::eBarracks_Ender_13:
            if (gMap_5C3030.field_22 == 108)
            {
                Map::LoadResource_4DBE00("SHDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID, loadmode, FALSE);
            }
            else
            {
                Map::LoadResource_4DBE00("BADOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID, loadmode, FALSE);
            }
            break;

        case LevelIds::eBonewerkz_8:
        case LevelIds::eBonewerkz_Ender_14:
            Map::LoadResource_4DBE00("BWDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        case LevelIds::eBrewery_9:
        case LevelIds::eBrewery_Ender_10:
            Map::LoadResource_4DBE00("BRDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        default:
            Map::LoadResource_4DBE00("DOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID, loadmode, FALSE);
            break;
        }
    }
    else
    {
        auto pDoor = alive_new<Door>();
        if (pDoor)
        {
            pDoor->ctor_41E250(static_cast<Path_Door*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_Shadow_4D7200(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pShadowZone = alive_new<ShadowZone>();
        if (pShadowZone)
        {
            pShadowZone->ctor_463900(static_cast<Path_ShadowZone*>(pTlv), pPath, tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_LiftPoint_4D7250(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ExpressWell_4D7D90(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Dove_4D7E90(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_RockSack_4D8040(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_FallingItem_4D81B0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_PullRingRope_4D8320(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_TimedMine_4D87C0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{ 
    Path_TimedMine * mine_tlv = reinterpret_cast<Path_TimedMine *>(pTlv);

    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, kAbeblowResID, loadmode, mine_tlv->field_18_disabled_resources & 1);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, kSlogBlowResID, loadmode, mine_tlv->field_18_disabled_resources & 2);

        static CompileTimeResourceList<2> sTimedMineResourceList_563368({
            { ResourceManager::Resource_Animation, kBombResID },
            { ResourceManager::Resource_Animation, kBombflshResID },
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            { ResourceManager::Resource_Animation, kAbebombResID },
            { ResourceManager::Resource_Animation, kDebrisID00 },
            { ResourceManager::Resource_Animation, kBgexpldResID },
        });

        Map::LoadResourcesFromList_4DBE70("BOMB.BND", sTimedMineResourceList_563368.AsList(), loadmode, 0);
        Map::LoadResourcesFromList_4DBE70("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        auto pMine = alive_new<TimedMine>();
        if (pMine)
        {
            pMine->ctor_410600(mine_tlv, tlvOffsetLevelIdPathId);
        }
    }
}

EXPORT void CC Factory_Slig_4D7BC0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Slog_4D8B20(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Switch_4D8CF0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{ 
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("SWITCH1.BAN", ResourceManager::Resource_Animation, ResourceID::kSwitchResID, loadmode);
        Map::LoadResource_4DBE00("ABEPULL.BAN", ResourceManager::Resource_Animation, ResourceID::kAbepullResID, loadmode);
    }
    else
    {
        auto pSwitch = alive_new<Switch>();
        if (pSwitch)
        {
            pSwitch->ctor_4D5860(static_cast<Path_Switch*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Null_4D69E0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ChantSuppressor_4D8D80(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D6A00(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Pulley_4D6A20(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_AbeStart_4D9030(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_WellExpress_4D7E60(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Mine_4D8890(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    Path_Mine * mine_tlv = reinterpret_cast<Path_Mine *>(pTlv);

    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, kAbeblowResID, loadmode, mine_tlv->field_16_disabled_resources & 1);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, kSlogBlowResID, loadmode, mine_tlv->field_16_disabled_resources & 2);

        static CompileTimeResourceList<2> sMineResourceList_56337C({
            { ResourceManager::Resource_Animation, kLandmineResID },
            { ResourceManager::Resource_Animation, kMineflshResID },
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            { ResourceManager::Resource_Animation, kAbebombResID },
            { ResourceManager::Resource_Animation, kDebrisID00 },
            { ResourceManager::Resource_Animation, kBgexpldResID },
        });

        Map::LoadResourcesFromList_4DBE70("MINE.BND", sMineResourceList_56337C.AsList(), loadmode, 0);
        Map::LoadResourcesFromList_4DBE70("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        auto pMine = alive_new<Mine>();
        if (pMine)
        {
            pMine->ctor_46B120(mine_tlv, tlvOffsetLevelIdPathId);
        }
    }
}

EXPORT void CC Factory_UXB_4D8960(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{ 
    // Use this to force spawn Timed Mines for testing purposes.
    /*Path_TimedMine dummyData;
    memcpy(&dummyData, pTlv, sizeof(Path_TLV));
    dummyData.field_10_id = 1;
    dummyData.field_12_state = 0;
    dummyData.field_14_scale = 0;
    dummyData.field_18_disabled_resources = 0;
    dummyData.field_16_timeout = 100;
    Factory_TimedMine_4D87C0(reinterpret_cast<Path_TLV*>(&dummyData), nullptr, tlvOffsetLevelIdPathId, loadmode);
    return; */

    Path_UXB * uxb_tlv = reinterpret_cast<Path_UXB *>(pTlv);

    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, kAbeblowResID, loadmode, uxb_tlv->field_18_disabled_resources & 1);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, kSlogBlowResID, loadmode, uxb_tlv->field_18_disabled_resources & 2);
        
        static CompileTimeResourceList<3> sUXBResourceList_563390({
            { ResourceManager::Resource_Animation, kTbombResID },
            { ResourceManager::Resource_Animation, kBombflshResID },
            { ResourceManager::Resource_Palt, kGrenflshResID },
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            { ResourceManager::Resource_Animation, kAbebombResID },
            { ResourceManager::Resource_Animation, kDebrisID00 },
            { ResourceManager::Resource_Animation, kBgexpldResID },
        });

        Map::LoadResourcesFromList_4DBE70("UXB.BND", sUXBResourceList_563390.AsList(), loadmode, 0);
        Map::LoadResourcesFromList_4DBE70("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        auto pUXB = alive_new<UXB>();
        if (pUXB)
        {
            pUXB->ctor_4DE9A0(uxb_tlv, tlvOffsetLevelIdPathId);
        }
    }
}

EXPORT void CC Factory_Paramite_4D9190(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_MovieHandStone_4D9F50(Path_TLV* , Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, ResourceID::kAbestoneResID, loadmode, FALSE);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

EXPORT void CC Factory_BirdPortal_4D9AA0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_PortalExit_4D9A80(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_TrapDoor_4D9B90(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_RollingRock_4D9C70(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligBoundLeft_4D7740(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_InvisibleZone_4D6A40(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_FootSwitch_4D9D00(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ChantSuppressor_4D9E10(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_MotionDetector_4D9E40(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligSpawner_4D79F0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ElectricWall_4DA020(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_LiftMover_4DA090(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_MeatSack_4D8140(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Scrab_4D9200(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4DA130(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ScrabLeftBound_4D6A80(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ScrabRightBound_4D6AA0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligBoundRight_4D79A0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligPersist_4D79D0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_EnemyStopper_4D6AC0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_InvisibleSwitch_4D6E40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pInvisibleSwitch = alive_new<InvisibleSwitch>();
        if (pInvisibleSwitch)
        {
            pInvisibleSwitch->ctor_45FA70(static_cast<Path_InvisibleSwitch*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Mudokon_4D8EC0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ZSligCover_4D6A60(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_DoorFlame_4D70D0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_MovingBomb_4D8A50(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D8A30(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_4D7160(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D6AE0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_TimerTrigger_4DA0E0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SecurityDoor_4DA150(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_GrenadeMachine_4DA1C0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_BackgroundAnimation_4D84F0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pAnim = alive_new<BackgroundAnimation>();
        if (pAnim)
        {
            pAnim->ctor_40D270(reinterpret_cast<BackgroundAnimation_TLV*>(pTlv), tlvOffsetLevelIdPathId);
        }
    }
}

EXPORT void CC Factory_LCD_4D6CF0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, 2, loadmode); // TODO: Add to resource ID enum
        return;
    }

    auto pLCD = alive_new<LCDScreen>();
    if (pLCD)
    {
        pLCD->ctor_460680(reinterpret_cast<Path_LCDScreen*>(pTlv), tlvOffsetLevelIdPathId);
    }
}

EXPORT void CC Factory_HandStone_4D9FA0(Path_TLV*, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, ResourceID::kAbestoneResID, loadmode, FALSE);
        Map::LoadResource_4DBE00("SPOTLITE.BAN", ResourceManager::Resource_Animation, ResourceID::kSpotliteResID, loadmode, FALSE);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

EXPORT void CC Factory_CreditsController_4D6D60(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D6910(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_StatusBoard_4DA3C0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, 2, loadmode); // TODO: Add to resource ID enum
        return;
    }
    auto pStatsSign = alive_new<StatsSign>();
    if (pStatsSign)
    {
        pStatsSign->ctor_47B600(reinterpret_cast<Path_StatsSign*>(pTlv), tlvOffsetLevelIdPathId);
    }
}

EXPORT void CC Factory_WheelSyncer_4DA430(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pWheelSyncer = alive_new<WheelSyncer>();
        if (pWheelSyncer)
        {
            pWheelSyncer->ctor_466090(static_cast<Path_WheelSyncer*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_MusicTrigger_4D71B0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Light_4D8590(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SlogSpawner_4D8BB0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_GasCountdown_4DA480(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_4D6C50(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_GasEmitter_4D8540(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SlogHut_4DA500(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Glukkon_4DA550(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_KillUnsavedMudokons_4DA6E0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SoftLanding_4D6950(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D6B00(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Water_4DA730(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4DA7C0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_WorkWheel_4D6B20(Path_TLV* pTlv , Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABEWORK.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeworkResID, loadmode, FALSE);
        Map::LoadResource_4DBE00("WORKWHEL.BAN", ResourceManager::Resource_Animation, ResourceID::kWorkwhelResID, loadmode, FALSE);
    }
    else
    {
        auto pWheel = alive_new<WorkWheel>();
        if (pWheel)
        {
            pWheel->ctor_4E35D0(static_cast<Path_Wheel*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Null_4DA850(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_LaughingGas_4DA870(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_FlyingSlig_4D92E0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Fleech_4D8C30(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Slurgs_4DA950(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SlamDoor_4DA9C0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_LevelLoader_4D6BB0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pLevelLoader = alive_new<LevelLoader>();
        if (pLevelLoader)
        {
            pLevelLoader->ctor_4DD330(static_cast<Path_LevelLoader*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_DemoSpawnPoint_4D6990(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Teleporter_4DAA90(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SlurgSpawner_4DAB50(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Grinder_4DABC0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ColorfulMeter_4DAC60(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_FlyingSligSpawner_4D9440(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_MineCar_4DACD0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_BoneBag_4D80B0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ExplosionSet_4DADC0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_MultiSwitchController_4D6C00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pMultiSwitchController = alive_new<MultiSwitchController>();
        if (pMultiSwitchController)
        {
            pMultiSwitchController->ctor_4D60E0(static_cast<Path_MultiSwitchController*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_StatusLight_4DA7E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("STATUSLT.BAN", ResourceManager::Resource_Animation, 373, loadmode); // TODO: Id
    }
    else
    {
        auto pStatusLight = alive_new<StatusLight>();
        if (pStatusLight)
        {
            pStatusLight->ctor_4D5250(static_cast<Path_StatusLight*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SlapLock_4DAF20(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ParamiteWeb_4D9120(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Alarm_4DAF90(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pAlarm = alive_new<Alarm>();
        if (pAlarm)
        {
            pAlarm->ctor_409300(static_cast<Path_Alarm*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_FartMachine_4DA2B0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ScrabSpawner_4D9270(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_NakedSlig_4D95A0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligGetPants_4D9700(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligGetWings_4D9900(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Greeter_4DAFE0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_NakedSligButton_4DB100(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_GlukkonSwitch_4DA670(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_DoorBlocker_4DAAE0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_TorturedMudokon_4D8430(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_TrainDoor_4D6E90(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

const PathFunctionTable kObjectFactory =
{
    {
        Factory_ContinuePoint_4D6970,
        Factory_PathTransition_4D68A0,
        Factory_Hoist_4D9E90,
        Factory_Edge_4D68C0,
        Factory_DeathDrop_4D6930,
        Factory_Door_4D6F00,
        Factory_Shadow_4D7200,
        Factory_LiftPoint_4D7250,
        Factory_ExpressWell_4D7D90,
        Factory_Dove_4D7E90,
        Factory_RockSack_4D8040,
        Factory_FallingItem_4D81B0,
        Factory_PullRingRope_4D8320,
        Factory_BackgroundAnimation_4D84F0,
        Factory_TimedMine_4D87C0,
        Factory_Slig_4D7BC0,
        Factory_Slog_4D8B20,
        Factory_Switch_4D8CF0,
        Factory_Null_4D69E0,
        Factory_ChantSuppressor_4D8D80,
        Factory_Null_4D6A00,
        Factory_Pulley_4D6A20,
        Factory_AbeStart_4D9030,
        Factory_WellExpress_4D7E60,
        Factory_Mine_4D8890,
        Factory_UXB_4D8960,
        Factory_Paramite_4D9190,
        Factory_MovieHandStone_4D9F50,
        Factory_BirdPortal_4D9AA0,
        Factory_PortalExit_4D9A80,
        Factory_TrapDoor_4D9B90,
        Factory_RollingRock_4D9C70,
        Factory_SligBoundLeft_4D7740,
        Factory_InvisibleZone_4D6A40,
        Factory_FootSwitch_4D9D00,
        Factory_ChantSuppressor_4D9E10,
        Factory_MotionDetector_4D9E40,
        Factory_SligSpawner_4D79F0,
        Factory_ElectricWall_4DA020,
        Factory_LiftMover_4DA090,
        Factory_MeatSack_4D8140,
        Factory_Scrab_4D9200,
        Factory_Null_4DA130,
        Factory_ScrabLeftBound_4D6A80,
        Factory_ScrabRightBound_4D6AA0,
        Factory_SligBoundRight_4D79A0,
        Factory_SligPersist_4D79D0,
        Factory_EnemyStopper_4D6AC0,
        Factory_InvisibleSwitch_4D6E40,
        Factory_Mudokon_4D8EC0,
        Factory_ZSligCover_4D6A60,
        Factory_DoorFlame_4D70D0,
        Factory_MovingBomb_4D8A50,
        Factory_Null_4D8A30,
        Factory_MainMenuController_4D6DB0,
        Factory_4D7160, // switch state related ??
        Factory_Null_4D6AE0,
        Factory_TimerTrigger_4DA0E0,
        Factory_SecurityDoor_4DA150,
        Factory_GrenadeMachine_4DA1C0,
        Factory_LCD_4D6CF0,
        Factory_HandStone_4D9FA0,
        Factory_CreditsController_4D6D60,
        Factory_Null_4D6910,
        Factory_StatusBoard_4DA3C0,
        Factory_WheelSyncer_4DA430,
        Factory_MusicTrigger_4D71B0,
        Factory_Light_4D8590,
        Factory_SlogSpawner_4D8BB0,
        Factory_GasCountdown_4DA480,
        Factory_4D6C50, // Remove Shrykull ??
        Factory_GasEmitter_4D8540,
        Factory_SlogHut_4DA500,
        Factory_Glukkon_4DA550,
        Factory_KillUnsavedMudokons_4DA6E0,
        Factory_SoftLanding_4D6950,
        Factory_Null_4D6B00,
        Factory_Water_4DA730,
        Factory_Null_4DA7C0,
        Factory_WorkWheel_4D6B20,
        Factory_Null_4DA850,
        Factory_LaughingGas_4DA870,
        Factory_FlyingSlig_4D92E0,
        Factory_Fleech_4D8C30,
        Factory_Slurgs_4DA950,
        Factory_SlamDoor_4DA9C0,
        Factory_LevelLoader_4D6BB0,
        Factory_DemoSpawnPoint_4D6990,
        Factory_Teleporter_4DAA90,
        Factory_SlurgSpawner_4DAB50,
        Factory_Grinder_4DABC0,
        Factory_ColorfulMeter_4DAC60,
        Factory_FlyingSligSpawner_4D9440,
        Factory_MineCar_4DACD0,
        Factory_BoneBag_4D80B0,
        Factory_ExplosionSet_4DADC0,
        Factory_MultiSwitchController_4D6C00,
        Factory_StatusLight_4DA7E0,
        Factory_SlapLock_4DAF20,
        Factory_ParamiteWeb_4D9120,
        Factory_Alarm_4DAF90,
        Factory_FartMachine_4DA2B0,
        Factory_ScrabSpawner_4D9270,
        Factory_NakedSlig_4D95A0,
        Factory_SligGetPants_4D9700,
        Factory_SligGetWings_4D9900,
        Factory_Greeter_4DAFE0,
        Factory_NakedSligButton_4DB100,
        Factory_GlukkonSwitch_4DA670,
        Factory_DoorBlocker_4DAAE0,
        Factory_TorturedMudokon_4D8430,
        Factory_TrainDoor_4D6E90,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr
    }
};
