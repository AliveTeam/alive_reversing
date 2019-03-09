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
#include "Grinder.hpp"
#include "Teleporter.hpp"
#include "Well.hpp"
#include "Water.hpp"
#include "SlamDoor.hpp"
#include "MusicTrigger.hpp"
#include "PauseMenu.hpp"
#include "Abe.hpp"
#include "LiftPoint.hpp"
#include "PullRingRope.hpp"
#include "LiftMover.hpp"
#include "TrapDoor.hpp"
#include "TimerTrigger.hpp"
#include "CreditsController.hpp"
#include "FootSwitch.hpp"
#include "FartMachine.hpp"
#include "FallingItem.hpp"
#include "MineCar.hpp"
#include "ElectricWall.hpp"
#include "GasEmitter.hpp"

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


static void LoadLiftPointResources(const char* ropeBan, const char* liftBan, __int16 loadMode)
{
    static CompileTimeResourceList<2> kResources({
        { ResourceManager::Resource_Animation, ResourceID::kPulleyResID },
        { ResourceManager::Resource_Animation, ResourceID::kLiftResID },
    });

    gMap_5C3030.LoadResource_4DBE00(ropeBan, ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
    gMap_5C3030.LoadResourcesFromList_4DBE70(liftBan, kResources.AsList(), loadMode);
}

EXPORT void CC Factory_LiftPoint_4D7250(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("ABELIFT.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeliftResID, loadMode);
        switch (gMap_5C3030.sCurrentLevelId_5C3030)
        {
        case LevelIds::eNecrum_2:
            LoadLiftPointResources("NECROPE.BAN", "NELIFT.BND", loadMode);
            break;

        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudomoVault_Ender_11:
            LoadLiftPointResources("NECROPE.BAN", "PVLIFT.BND", loadMode);
            break;

        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
            LoadLiftPointResources("NECROPE.BAN", "SVLIFT.BND", loadMode);
            break;

        case LevelIds::eFeeCoDepot_5:
        case LevelIds::eFeeCoDepot_Ender_12:
            LoadLiftPointResources("ROPES.BAN", "FDLIFT.BND", loadMode);
            break;

        case LevelIds::eBarracks_6:
        case LevelIds::eBarracks_Ender_13:
            LoadLiftPointResources("ROPES.BAN", "BALIFT.BND", loadMode);
            break;

        case LevelIds::eBonewerkz_8:
        case LevelIds::eBonewerkz_Ender_14:
            LoadLiftPointResources("ROPES.BAN", "BWLIFT.BND", loadMode);
            break;

        case LevelIds::eBrewery_9:
        case LevelIds::eBrewery_Ender_10:
            LoadLiftPointResources("ROPES.BAN", "BRLIFT.BND", loadMode);
            break;

        default:
            LoadLiftPointResources("ROPES.BAN", "MILIFT.BND", loadMode);
            break;
        }
    }
    else
    {
        Path_LiftPoint* pLiftTlv = static_cast<Path_LiftPoint*>(pTlv);
        for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)) && pObj->field_4_typeId == BaseGameObject::Types::eLiftPoint_78)
            {
                // Is there already an existing LiftPoint object for this TLV?
                LiftPoint* pLiftPoint = static_cast<LiftPoint*>(pObj);
                const short xpos = FP_GetExponent(pLiftPoint->field_B8_xpos);
                if (pTlv->field_8_top_left.field_0_x <= xpos && 
                    xpos <= pTlv->field_C_bottom_right.field_0_x &&
                    pLiftPoint->field_278_lift_point_id == pLiftTlv->field_10_id && 
                    pLiftPoint->field_C2_lvl_number == gMap_5C3030.sCurrentLevelId_5C3030 && 
                    pLiftPoint->field_C0_path_number == gMap_5C3030.sCurrentPathId_5C3032)
                {
                    // Yes so just reset its data
                    Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                    return;
                }
            }
        }

        // TODO: Meaning of the data in field_1_unknown for lift point
        if (pLiftTlv->field_1_unknown & 2 || (pLiftTlv->field_1_unknown == 0 && pLiftTlv->field_12_bstart_point))
        {
            auto pLiftPoint = alive_new<LiftPoint>();
            if (pLiftPoint)
            {
                pLiftPoint->ctor_461030(pLiftTlv, tlvOffsetLevelIdPathId.all);
            }
            return;
        }
        else
        {
            // Find out where to create the lift point
            short pointNumber = 1;
            while (pointNumber < 8)
            {
                Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));

                while (pTlvIter)
                {
                    if (pTlvIter->field_4_type == TlvTypes::LiftPoint_7)
                    {
                        auto pLiftPointIter = static_cast<Path_LiftPoint*>(pTlvIter);

                        const int tlvX = pTlv->field_8_top_left.field_0_x;
                        const int absX = pTlvIter->field_8_top_left.field_0_x - tlvX >= 0 ? 
                            pTlvIter->field_8_top_left.field_0_x - tlvX : 
                            tlvX - pTlvIter->field_8_top_left.field_0_x;

                        if (absX < 5 && 
                            pLiftPointIter->field_10_id == pLiftTlv->field_10_id && 
                            (pLiftPointIter->field_1_unknown & 2 || pLiftPointIter->field_1_unknown == 0) &&
                            pLiftPointIter->field_12_bstart_point)
                        {

                            auto pLiftPoint = alive_new<LiftPoint>();
                            if (pLiftPoint)
                            {
                                pLiftPoint->ctor_461030(pLiftPointIter, tlvOffsetLevelIdPathId.all);
                            }
                            return;
                        }
                    }

                    pTlvIter = Path::Next_TLV_4DB6A0(pTlvIter);
                }
                pointNumber++;
            }

            // Default to original
            auto pLiftPoint = alive_new<LiftPoint>();
            if (pLiftPoint)
            {
                pLiftPoint->ctor_461030(pLiftTlv, tlvOffsetLevelIdPathId.all);
            }
        }
    }
}

EXPORT void CC Factory_ExpressWell_4D7D90(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("ABEWELL.BAN", ResourceManager::Resource_Animation, ResourceID::kAbewellResID, loadmode);
        gMap_5C3030.LoadResource_4DBE00("WELLLEAF.BAN", ResourceManager::Resource_Animation, ResourceID::kWellLeafResID, loadmode);
    }
    else
    {
        Path_Well_Base* pWellTlv = static_cast<Path_Well_Base*>(pTlv);
        const FP xpos = FP_FromInteger(pWellTlv->field_8_top_left.field_0_x);
        const FP ypos = FP_FromInteger(pWellTlv->field_8_top_left.field_2_y + 5);
        auto pExpressWell = alive_new<Well>();
        if (pExpressWell)
        {
            pExpressWell->ctor_4E2BE0(pWellTlv, xpos, ypos, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_Dove_4D7E90(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_RockSack_4D8040(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_FallingItem_4D81B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8)
        {
            gMap_5C3030.LoadResource_4DBE00("FALLBONZ.BAN", ResourceManager::Resource_Animation, 2007, loadmode);
            gMap_5C3030.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, 358, loadmode);
            gMap_5C3030.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadmode);
            gMap_5C3030.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, 1105, loadmode);
            gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadmode);
        }
        else
        {
            gMap_5C3030.LoadResource_4DBE00("FALLROCK.BAN", ResourceManager::Resource_Animation, 2007, loadmode);
            gMap_5C3030.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, 358, loadmode);
            gMap_5C3030.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadmode);
            gMap_5C3030.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, 1105, loadmode);
        }
    }
    else
    {
        auto pFallingItem = alive_new<FallingItem>();
        if (pFallingItem)
        {
            pFallingItem->ctor_4272C0(static_cast<Path_FallingItem*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_PullRingRope_4D8320(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, ResourceID::kAbehoistResID, loadmode);
        switch (gMap_5C3030.sCurrentLevelId_5C3030)
        {
        case LevelIds::eNecrum_2:
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            gMap_5C3030.LoadResource_4DBE00("NECROPE.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadmode);
            break;
        default:
            gMap_5C3030.LoadResource_4DBE00("ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadmode);
            break;
        }
        gMap_5C3030.LoadResource_4DBE00("PULLRING.BAN", ResourceManager::Resource_Animation, ResourceID::kPullringResID, loadmode);
    }
    else
    {
        auto pRope = alive_new<PullRingRope>();
        if (pRope)
        {
            pRope->ctor_49B2D0(static_cast<Path_PullRingRope*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

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

EXPORT void CC Factory_Pulley_4D6A20(Path_TLV* , Path*, TlvItemInfoUnion, __int16) 
{
    // Nothing to do.
}

EXPORT void CC Factory_AbeStart_4D9030(Path_TLV* pTlv, Path*, TlvItemInfoUnion, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2 && !word_5C1BA0)
    {
        if (!pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300 = alive_new<PauseMenu>();
            if (pPauseMenu_5C9300)
            {
                pPauseMenu_5C9300->ctor_48FB80();
            }
        }

        if (sActiveHero_5C1B68 == spAbe_554D5C)
        {
            sActiveHero_5C1B68 = alive_new<Abe>();
            if (sActiveHero_5C1B68)
            {
                sActiveHero_5C1B68->ctor_44AD10(58808, 85, 57, 55);
                sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
                sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
            }
        }
    }
}

EXPORT void CC Factory_WellExpress_4D7E60(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    Factory_ExpressWell_4D7D90(pTlv, pPath, tlvOffsetLevelIdPathId, loadmode);
}

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

EXPORT void CC Factory_TrapDoor_4D9B90(Path_TLV* pTlv, Path* , TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        switch (gMap_5C3030.sCurrentLevelId_5C3030)
        {
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            gMap_5C3030.LoadResource_4DBE00("VLTSTRAP.BAN", ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, loadmode);
            break;

        default:
            gMap_5C3030.LoadResource_4DBE00("TRAPDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, loadmode);
            break;
        }
    }
    else
    {
        auto pTrapDoor = alive_new<TrapDoor>();
        if (pTrapDoor)
        {
            // TODO: OG bug - actually passes Path* instead of Map* .. probably works because this parameter never gets used!
            pTrapDoor->ctor_4DD570(static_cast<Path_TrapDoor*>(pTlv), &gMap_5C3030, tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_RollingRock_4D9C70(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligBoundLeft_4D7740(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_InvisibleZone_4D6A40(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_FootSwitch_4D9D00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        switch (gMap_5C3030.sCurrentLevelId_5C3030)
        {
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            Map::LoadResource_4DBE00("VLTTRIGR.BAN", ResourceManager::Resource_Animation, ResourceID::kTriggerResID, loadmode);
            break;
        case LevelIds::eBonewerkz_8:
            Map::LoadResource_4DBE00("BWTRIGGR.BAN", ResourceManager::Resource_Animation, ResourceID::kTriggerResID, loadmode);
            break;
        default:
            Map::LoadResource_4DBE00("TRIGGER.BAN", ResourceManager::Resource_Animation, ResourceID::kTriggerResID, loadmode);
            break;
        }
    }
    else
    {
        auto pFootSwitch = alive_new<FootSwitch>();
        if (pFootSwitch)
        {
            pFootSwitch->ctor_4DE090(static_cast<Path_FootSwitch*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_ChantSuppressor_4D9E10(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_MotionDetector_4D9E40(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SligSpawner_4D79F0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_ElectricWall_4DA020(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("ELECWALL.BAN", ResourceManager::Resource_Animation, ResourceID::kElecwallResID, loadMode);
    }
    else
    {
        auto pElectricWall = alive_new<ElectricWall>();
        if (pElectricWall)
        {
            pElectricWall->ctor_421DA0(static_cast<Path_ElectricWall*>(pTlv), tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_LiftMover_4DA090(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pLiftMover = alive_new<LiftMover>();
        if (pLiftMover)
        {
            pLiftMover->ctor_40CCD0(static_cast<Path_LiftMover*>(pTlv), tlvInfo.all);
        }
    }
}

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

EXPORT void CC Factory_TimerTrigger_4DA0E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pTimerTrigger = alive_new<TimerTrigger>();
        if (pTimerTrigger)
        {
            pTimerTrigger->ctor_4CDC20(static_cast<Path_TimerTrigger*>(pTlv), tlvInfo.all);
        }
    }
}

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

EXPORT void CC Factory_CreditsController_4D6D60(Path_TLV* pTlv , Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{ 
    if (loadmode != 1 && loadmode != 2 && !sDoesCreditsControllerExist_5C1B90)
    {
        auto pCreditsController = alive_new<CreditsController>();
        if (pCreditsController)
        {
            pCreditsController->ctor_418A10(pTlv, tlvInfo.all);
        }
    }
}

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

EXPORT void CC Factory_MusicTrigger_4D71B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pMusicTrigger = alive_new<MusicTrigger>();
        if (pMusicTrigger)
        {
            pMusicTrigger->ctor_47FE40(static_cast<Path_MusicTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_Light_4D8590(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SlogSpawner_4D8BB0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_GasCountdown_4DA480(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_4D6C50(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_GasEmitter_4D8540(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{ 
    if (loadMode != 1 && loadMode != 2)
    {
        auto pGasEmitter = alive_new<GasEmitter>();
        if (pGasEmitter)
        {
            pGasEmitter->ctor_43CAA0(static_cast<Path_GasEmitter*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SlogHut_4DA500(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Glukkon_4DA550(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_KillUnsavedMudokons_4DA6E0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SoftLanding_4D6950(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D6B00(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Water_4DA730(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("WDROP.BAN", ResourceManager::Resource_Animation, ResourceID::kWdropResID, loadmode);
        Map::LoadResource_4DBE00("SPLASH.BAN", ResourceManager::Resource_Animation, ResourceID::kSplashResID, loadmode);
    }
    else
    {
        auto pWater = alive_new<Water>();
        if (pWater)
        {
            pWater->ctor_4E02C0(static_cast<Path_Water*>(pTlv), tlvInfo.all);
        }
    }
}

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

EXPORT void CC Factory_SlamDoor_4DA9C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        switch (gMap_5C3030.sCurrentLevelId_5C3030)
        {
        case LevelIds::eNecrum_2:
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            Map::LoadResource_4DBE00("SLAMVLTS.BAN", ResourceManager::Resource_Animation, ResourceID::kSlamResID, loadmode);
            break;
        default:
            Map::LoadResource_4DBE00("SLAM.BAN", ResourceManager::Resource_Animation, ResourceID::kSlamResID, loadmode);
            break;
        }
    }
    else
    {
        auto pSlamDoor = alive_new<SlamDoor>();
        if (pSlamDoor)
        {
            pSlamDoor->ctor_4AF700(static_cast<Path_SlamDoor*>(pTlv), tlvInfo);
        }
    }
}

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

EXPORT void CC Factory_Teleporter_4DAA90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pTeleporter = alive_new<Teleporter>();
        if (pTeleporter)
        {
            pTeleporter->ctor_4DC1E0(static_cast<Path_Teleporter*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SlurgSpawner_4DAB50(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Grinder_4DABC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{ 
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("DRILL.BAN", ResourceManager::Resource_Animation, 6004, loadmode); // TODO: ID
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadmode);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadmode);
    }
    else
    {
        auto pGrinder = alive_new<Grinder>();
        if (pGrinder)
        {
            pGrinder->ctor_4200D0(static_cast<Path_Grinder*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_ColorfulMeter_4DAC60(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_FlyingSligSpawner_4D9440(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_MineCar_4DACD0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        // TODO: Ids
        gMap_5C3030.LoadResource_4DBE00("BAYROLL.BAN", ResourceManager::Resource_Animation, 6013, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("ABECAR.BAN", ResourceManager::Resource_Animation, 113, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, 0);
    }
    else
    {
        auto pMineCar = alive_new<MineCar>();
        if (pMineCar)
        {
            pMineCar->ctor_46BC80(static_cast<Path_MineCar*>(pTlv), tlvInfo.all, 0, 0, 0);
        }
    }

}

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

EXPORT void CC Factory_FartMachine_4DA2B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, ResourceID::kAbestoneResID, loadmode);
        gMap_5C3030.LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::Resource_Font, 2, loadmode);
        gMap_5C3030.LoadResource_4DBE00("BREWBTN.BAN", ResourceManager::Resource_Animation, 6016, loadmode);
        gMap_5C3030.LoadResource_4DBE00("EVILFART.BAN", ResourceManager::Resource_Animation, 6017, loadmode);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, loadmode);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadmode);
    }
    else
    {
        auto pFartMachine = alive_new<FartMachine>();
        if (pFartMachine)
        {
            pFartMachine->ctor_413060(static_cast<Path_FartMachine*>(pTlv), tlvInfo.all);
        }
    }

}

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
