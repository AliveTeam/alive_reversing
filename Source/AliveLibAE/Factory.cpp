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
#include "DemoPlayback.hpp"
#include "SecurityOrb.hpp"
#include "Greeter.hpp"
#include "MotionDetector.hpp"
#include "FlyingSlig.hpp"
#include "FlyingSligSpawner.hpp"
#include "Mudokon.hpp"
#include "BirdPortal.hpp"
#include "TorturedMudokon.hpp"
#include "Dove.hpp"
#include "DoorBlocker.hpp"
#include "SlapLock.hpp"
#include "Glukkon.hpp"
#include "MovingBomb.hpp"
#include "Throwable.hpp"
#include "Slurg.hpp"
#include "SlurgSpawner.hpp"
#include "DoorFlame.hpp"
#include "NakedSlig.hpp"
#include "Scrab.hpp"
#include "Paramite.hpp"
#include "Fleech.hpp"
#include "Slog.hpp"
#include "Slig.hpp"
#include "GasCountDown.hpp"
#include "GlukkonSwitch.hpp"
#include "NakedSligButton.hpp"
#include "SligGetPantsAndWings.hpp"
#include "BoomMachine.hpp"
#include "Meat.hpp"
#include "Bone.hpp"
#include "Rock.hpp"
#include "RockSack.hpp"
#include "SligSpawner.hpp"
#include "ScrabSpawner.hpp"
#include "SlogSpawner.hpp"
#include "SlogHut.hpp"
#include "ParamiteWebLine.hpp"
#include "ExplosionSet.hpp"
#include "ColourfulMeter.hpp"
#include "SecurityDoor.hpp"
#include "LaughingGas.hpp"

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
            auto pMainMenuController = ae_new<MainMenuController>();
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
        Map::LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
        Map::LoadResource_4DBE00("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kDrpRockResID, loadmode, 0);
    }
    else if (pHoistTlv->field_10_type == Path_Hoist::Type::eOffScreen)
    {
        // Its an off screen hoist so create the falling rocks effect
        auto pHoistFallingRocks = ae_new<HoistRocksEffect>();
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
        Map::LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
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
        Map::LoadResource_4DBE00("ABEDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbedoorResID, loadmode, FALSE);
        switch (gMap_5C3030.field_0_current_level)
        {
        case LevelIds::eNecrum_2:
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            Map::LoadResource_4DBE00("SVZDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        case LevelIds::eFeeCoDepot_5:
        case LevelIds::eFeeCoDepot_Ender_12:
            Map::LoadResource_4DBE00("FDDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        case LevelIds::eBarracks_6:
        case LevelIds::eBarracks_Ender_13:
            if (gMap_5C3030.field_22_overlayID == 108)
            {
                Map::LoadResource_4DBE00("SHDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
            }
            else
            {
                Map::LoadResource_4DBE00("BADOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
            }
            break;

        case LevelIds::eBonewerkz_8:
        case LevelIds::eBonewerkz_Ender_14:
            Map::LoadResource_4DBE00("BWDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        case LevelIds::eBrewery_9:
        case LevelIds::eBrewery_Ender_10:
            Map::LoadResource_4DBE00("BRDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
            break;

        default:
            Map::LoadResource_4DBE00("DOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
            break;
        }
    }
    else
    {
        auto pDoor = ae_new<Door>();
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
        auto pShadowZone = ae_new<ShadowZone>();
        if (pShadowZone)
        {
            pShadowZone->ctor_463900(static_cast<Path_ShadowZone*>(pTlv), pPath, tlvInfo.all);
        }
    }
}


static void LoadLiftPointResources(const char* ropeBan, const char* liftBan, __int16 loadMode)
{
    static CompileTimeResourceList<2> kResources({
        { ResourceManager::Resource_Animation, AEResourceID::kPulleyResID },
        { ResourceManager::Resource_Animation, AEResourceID::kLiftResID },
    });

    gMap_5C3030.LoadResource_4DBE00(ropeBan, ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadMode);
    gMap_5C3030.LoadResourcesFromList_4DBE70(liftBan, kResources.AsList(), loadMode);
}

EXPORT void CC Factory_LiftPoint_4D7250(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("ABELIFT.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, loadMode);
        switch (gMap_5C3030.field_0_current_level)
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

            if (!(pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)) && pObj->field_4_typeId == Types::eLiftPoint_78)
            {
                // Is there already an existing LiftPoint object for this TLV?
                LiftPoint* pLiftPoint = static_cast<LiftPoint*>(pObj);
                const short xpos = FP_GetExponent(pLiftPoint->field_B8_xpos);
                if (pTlv->field_8_top_left.field_0_x <= xpos && 
                    xpos <= pTlv->field_C_bottom_right.field_0_x &&
                    pLiftPoint->field_278_lift_point_id == pLiftTlv->field_10_id && 
                    pLiftPoint->field_C2_lvl_number == gMap_5C3030.field_0_current_level && 
                    pLiftPoint->field_C0_path_number == gMap_5C3030.field_2_current_path)
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
            auto pLiftPoint = ae_new<LiftPoint>();
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

                            auto pLiftPoint = ae_new<LiftPoint>();
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
            auto pLiftPoint = ae_new<LiftPoint>();
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
        gMap_5C3030.LoadResource_4DBE00("ABEWELL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbewellResID, loadmode);
        gMap_5C3030.LoadResource_4DBE00("WELLLEAF.BAN", ResourceManager::Resource_Animation, AEResourceID::kWellLeafResID, loadmode);
    }
    else
    {
        Path_Well_Base* pWellTlv = static_cast<Path_Well_Base*>(pTlv);
        const FP xpos = FP_FromInteger(pWellTlv->field_8_top_left.field_0_x);
        const FP ypos = FP_FromInteger(pWellTlv->field_8_top_left.field_2_y + 5);
        auto pExpressWell = ae_new<Well>();
        if (pExpressWell)
        {
            pExpressWell->ctor_4E2BE0(pWellTlv, xpos, ypos, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_Dove_4D7E90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pDoveTlv = static_cast<Path_Dove*>(pTlv);

        const short width = pDoveTlv->field_C_bottom_right.field_0_x - pDoveTlv->field_8_top_left.field_0_x;
        const short height = pDoveTlv->field_C_bottom_right.field_2_y - pDoveTlv->field_8_top_left.field_2_y;

        for (int i = 0; i < pDoveTlv->field_10_dove_count; i++)
        {
            auto pDove = ae_new<Dove>();
            if (pDove)
            {
                pDove->ctor_41F430(5580, 41, 20, 60, tlvInfo.all, pDoveTlv->field_14_scale != 0 ? FP_FromDouble(0.5) : FP_FromInteger(1));
            }

            short ypos = 0;
            if (pDoveTlv->field_12_pixel_perfect)
            {
                pDove->field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
                ypos = pTlv->field_8_top_left.field_2_y;
            }
            else
            {
                pDove->field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + width  * Math_NextRandom() / 256);
                ypos = pTlv->field_8_top_left.field_2_y + height * Math_NextRandom() / 256;
            }
            pDove->field_BC_ypos = FP_FromInteger(ypos) + FP_FromInteger(10);
        }
    }
}

EXPORT void CC Factory_RockSack_4D8040(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources(
        { 
            { ResourceManager::Resource_Animation, 12 },
            { ResourceManager::Resource_Animation, 14 },
            { ResourceManager::Resource_Animation, 350 } 
        });
        gMap_5C3030.LoadResourcesFromList_4DBE70("RTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        auto pRockSack = ae_new<RockSack>();
        if (pRockSack)
        {
            pRockSack->ctor_49F100(static_cast<Path_RockSack*>(pTlv), tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_FallingItem_4D81B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        if (gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8)
        {
            gMap_5C3030.LoadResource_4DBE00("FALLBONZ.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            gMap_5C3030.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickResID, loadmode);
            gMap_5C3030.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kRockShadowResID, loadmode);
            gMap_5C3030.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00, loadmode);
            gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        }
        else
        {
            gMap_5C3030.LoadResource_4DBE00("FALLROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            gMap_5C3030.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickResID, loadmode);
            gMap_5C3030.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kRockShadowResID, loadmode);
            gMap_5C3030.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00, loadmode);
        }
    }
    else
    {
        auto pFallingItem = ae_new<FallingItem>();
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
        gMap_5C3030.LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode);
        switch (gMap_5C3030.field_0_current_level)
        {
        case LevelIds::eNecrum_2:
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            gMap_5C3030.LoadResource_4DBE00("NECROPE.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
            break;
        default:
            gMap_5C3030.LoadResource_4DBE00("ROPES.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
            break;
        }
        gMap_5C3030.LoadResource_4DBE00("PULLRING.BAN", ResourceManager::Resource_Animation, AEResourceID::kPullringResID, loadmode);
    }
    else
    {
        auto pRope = ae_new<PullRingRope>();
        if (pRope)
        {
            pRope->ctor_49B2D0(static_cast<Path_PullRingRope*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_TimedMine_4D87C0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{ 
    auto mine_tlv = static_cast<Path_TimedMine*>(pTlv);

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
        auto pMine = ae_new<TimedMine>();
        if (pMine)
        {
            pMine->ctor_410600(mine_tlv, tlvOffsetLevelIdPathId);
        }
    }
}

static CompileTimeResourceList<3> kResources_5632A0(
{
    { ResourceManager::Resource_Animation, 417 },
    { ResourceManager::Resource_Animation, 344 },
    { ResourceManager::Resource_Animation, 28 }
});

static CompileTimeResourceList<4> kResources_5632BC(
{
    { ResourceManager::Resource_Animation, 412 },
    { ResourceManager::Resource_Animation, 414 },
    { ResourceManager::Resource_Animation, 319 },
    { ResourceManager::Resource_Animation, 360 }
});

static void LoadWalkingSligResources(__int16 disabledResources, __int16 loadMode)
{
    gMap_5C3030.LoadResource_4DBE00("SLGLEVER.BAN", ResourceManager::Resource_Animation, 419, loadMode, disabledResources & 1);
    gMap_5C3030.LoadResource_4DBE00("SLGLIFT.BAN", ResourceManager::Resource_Animation, 420, loadMode, disabledResources & 2);
    gMap_5C3030.LoadResource_4DBE00("SLGSLEEP.BAN", ResourceManager::Resource_Animation, 413, loadMode, disabledResources & 0x40);
    gMap_5C3030.LoadResource_4DBE00("SLGEDGE.BAN", ResourceManager::Resource_Animation, 415, loadMode, disabledResources & 0x100);
    gMap_5C3030.LoadResource_4DBE00("SLGSMASH.BAN", ResourceManager::Resource_Animation, 416, loadMode, disabledResources & 0x200);
    gMap_5C3030.LoadResource_4DBE00("SLGBEAT.BAN", ResourceManager::Resource_Animation, 426, loadMode, disabledResources & 0x400);
    gMap_5C3030.LoadResource_4DBE00("SLGKNFD.BAN", ResourceManager::Resource_Animation, 418, loadMode, disabledResources & 0x80);
    gMap_5C3030.LoadResourcesFromList_4DBE70("SLIGZ.BND", kResources_5632A0.AsList(), loadMode, disabledResources & 4);
    gMap_5C3030.LoadResourcesFromList_4DBE70("SLIG.BND", kResources_5632BC.AsList(), loadMode);
    gMap_5C3030.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, 423, loadMode);
    gMap_5C3030.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);
}

EXPORT void CC Factory_Slig_4D7BC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == 1 || loadMode == 2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        auto pSlig = ae_new<Slig>();
        if (pSlig)
        {
            pSlig->ctor_4B1370(pSligTlv, tlvInfo.all);
        }
    }
}

static void LoadSlogResources(__int16 loadMode)
{
    static CompileTimeResourceList<4> kResources(
    {
        { ResourceManager::Resource_Animation, 570 },
        { ResourceManager::Resource_Animation, 571 },
        { ResourceManager::Resource_Animation, 572 },
        { ResourceManager::Resource_Animation, 574 }
    });

    gMap_5C3030.LoadResourcesFromList_4DBE70("SLOG.BND", kResources.AsList(), loadMode);
    gMap_5C3030.LoadResource_4DBE00("DOGKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, loadMode);
}

EXPORT void CC Factory_Slog_4D8B20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{

    if (loadMode == 1 || loadMode == 2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        auto pSlog = ae_new<Slog>();
        if (pSlog)
        {
            pSlog->ctor_4C42E0(static_cast<Path_Slog*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Switch_4D8CF0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{ 
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("SWITCH1.BAN", ResourceManager::Resource_Animation, AEResourceID::kSwitchResID, loadmode);
        Map::LoadResource_4DBE00("ABEPULL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, loadmode);
    }
    else
    {
        auto pSwitch = ae_new<Switch>();
        if (pSwitch)
        {
            pSwitch->ctor_4D5860(static_cast<Path_Switch*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Null_4D69E0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_SecurityOrb_4D8D80(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    Path_SecurityOrb* pSecurityOrbTlv = static_cast<Path_SecurityOrb*>(pTlv);
    const __int16 disabledResources = pSecurityOrbTlv->field_12_disabled_resources;
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("MAIMORB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMaimGameResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode); // TODO: Res id constants
        gMap_5C3030.LoadResource_4DBE00("SPARKS.BAN", ResourceManager::Resource_Animation, AEResourceID::kSparksResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, disabledResources & 1);
        gMap_5C3030.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalResID, loadMode, disabledResources & 0x10);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        auto pSecurityOrb = ae_new<SecurityOrb>();
        if (pSecurityOrb)
        {
            pSecurityOrb->ctor_466350(pSecurityOrbTlv, tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Null_4D6A00(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Pulley_4D6A20(Path_TLV* , Path*, TlvItemInfoUnion, __int16) 
{
    // Nothing to do.
}

EXPORT void CC Factory_AbeStart_4D9030(Path_TLV* pTlv, Path*, TlvItemInfoUnion, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2 && !gAttract_5C1BA0)
    {
        if (!pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300 = ae_new<PauseMenu>();
            if (pPauseMenu_5C9300)
            {
                pPauseMenu_5C9300->ctor_48FB80();
            }
        }

        if (sActiveHero_5C1B68 == spAbe_554D5C)
        {
            sActiveHero_5C1B68 = ae_new<Abe>();
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
    auto mine_tlv = static_cast<Path_Mine*>(pTlv);

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
        auto pMine = ae_new<Mine>();
        if (pMine)
        {
            pMine->ctor_46B120(mine_tlv, tlvOffsetLevelIdPathId);
        }
    }
}

EXPORT void CC Factory_UXB_4D8960(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{ 
    auto uxb_tlv = static_cast<Path_UXB*>(pTlv);

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
        auto pUXB = ae_new<UXB>();
        if (pUXB)
        {
            pUXB->ctor_4DE9A0(uxb_tlv, tlvOffsetLevelIdPathId);
        }
    }
}

EXPORT void CC Factory_Paramite_4D9190(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<9> kResources(
        {
            { ResourceManager::Resource_Animation, 600 },
            { ResourceManager::Resource_Animation, 605 },
            { ResourceManager::Resource_Animation, 614 },
            { ResourceManager::Resource_Animation, 604 },
            { ResourceManager::Resource_Animation, 601 },
            { ResourceManager::Resource_Animation, 615 },
            { ResourceManager::Resource_Animation, 610 },
            { ResourceManager::Resource_Animation, 609 },
            { ResourceManager::Resource_Animation, 2034 }
        });
        gMap_5C3030.LoadResourcesFromList_4DBE70("PARAMITE.BND", kResources.AsList(), loadMode);
    }
    else
    {
        auto pParamite = ae_new<Paramite>();
        if (pParamite)
        {
            pParamite->ctor_4879B0(static_cast<Path_Paramite*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_MovieHandStone_4D9F50(Path_TLV* , Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, FALSE);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

EXPORT void CC Factory_BirdPortal_4D9AA0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pBirdPortalTlv = static_cast<Path_BirdPortal*>(pTlv);
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources_5634E8({
            { ResourceManager::Resource_Animation, kPortalTerminatorID },
            { ResourceManager::Resource_Animation, kPortliteResID },
            { ResourceManager::Resource_Animation, kPortlitResID },
        });

        gMap_5C3030.LoadResourcesFromList_4DBE70("PORTAL.BND", kResources_5634E8.AsList(), loadMode);
        if (pBirdPortalTlv->field_1C_portal_type == PortalType::eShrykull_2)
        {
            static CompileTimeResourceList<2> kResources_563504({
                { ResourceManager::Resource_Animation, kAbemorphResID },
                { ResourceManager::Resource_Animation, kShrmorphResID },
            });

            gMap_5C3030.LoadResourcesFromList_4DBE70("SHRYPORT.BND", kResources_563504.AsList(), loadMode);
            gMap_5C3030.LoadResource_4DBE00("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode);
        }
    }
    else if (SwitchStates_Get_466020(pBirdPortalTlv->field_20_create_id))
    {
        auto pBirdPortal = ae_new<BirdPortal>();
        if (pBirdPortal)
        {
            pBirdPortal->ctor_497E00(pBirdPortalTlv, tlvInfo.all);
        }
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvInfo.all, -1, 0, 0);
    }

}

EXPORT void CC Factory_PortalExit_4D9A80(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_TrapDoor_4D9B90(Path_TLV* pTlv, Path* , TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        switch (gMap_5C3030.field_0_current_level)
        {
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            gMap_5C3030.LoadResource_4DBE00("VLTSTRAP.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
            break;

        default:
            gMap_5C3030.LoadResource_4DBE00("TRAPDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
            break;
        }
    }
    else
    {
        auto pTrapDoor = ae_new<TrapDoor>();
        if (pTrapDoor)
        {
            // TODO: OG bug - actually passes Path* instead of Map* .. probably works because this parameter never gets used!
            pTrapDoor->ctor_4DD570(static_cast<Path_TrapDoor*>(pTlv), &gMap_5C3030, tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_RollingRock_4D9C70(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

static Path_TLV* FindMatchingSligTLV(Path_TLV* pTlvIter, Path_Slig_Bound* pTlv)
{
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::Slig_15 && 
            pTlv->field_10_slig_id == static_cast<Path_Slig*>(pTlvIter)->field_38_slig_id && 
            !pTlvIter->field_0_flags.Get(TLV_Flags::eBit2_Unknown))
        {
            return pTlvIter;
        }
        pTlvIter = Path::Next_TLV_4DB6A0(pTlvIter);
    }
    return nullptr;
}

EXPORT void CC Factory_SligBoundLeft_4D7740(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pBound = static_cast<Path_Slig_Bound*>(pTlv);
    if (loadMode == 1 || loadMode == 2)
    {
        LoadWalkingSligResources(pBound->field_12_disabled_resources, loadMode);
    }
    else
    {
        pBound->field_0_flags.Clear(TLV_Flags::eBit1_Created);
        pBound->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);

        for (short camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit1_Created);
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
                auto pSlig = ae_new<Slig>();
                if (pSlig)
                {
                    tlvInfo.parts.tlvOffset += 
                        static_cast<WORD>(
                            reinterpret_cast<const BYTE*>(pTlvIter) - 
                            reinterpret_cast<const BYTE*>(pBound));
                    pSlig->ctor_4B1370(static_cast<Path_Slig*>(pTlvIter), tlvInfo.all);
                }
                return;
            }
        }
    }
}

EXPORT void CC Factory_InvisibleZone_4D6A40(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_FootSwitch_4D9D00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        switch (gMap_5C3030.field_0_current_level)
        {
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            Map::LoadResource_4DBE00("VLTTRIGR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
            break;
        case LevelIds::eBonewerkz_8:
            Map::LoadResource_4DBE00("BWTRIGGR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
            break;
        default:
            Map::LoadResource_4DBE00("TRIGGER.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
            break;
        }
    }
    else
    {
        auto pFootSwitch = ae_new<FootSwitch>();
        if (pFootSwitch)
        {
            pFootSwitch->ctor_4DE090(static_cast<Path_FootSwitch*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_ChantSuppressor_WithArms_4D9E10(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    // TODO: Most of the code for the chant suppressor with arms is still in the game
    // check if it can be re-added at some point.
    Factory_SecurityOrb_4D8D80(pTlv, pPath, tlvInfo, loadMode);
}

EXPORT void CC Factory_MotionDetector_4D9E40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pMotionDetector = ae_new<MotionDetector>();
        if (pMotionDetector)
        {
            pMotionDetector->ctor_4683B0(static_cast<Path_MotionDetector*>(pTlv), tlvInfo.all, nullptr);
        }
    }
}

EXPORT void CC Factory_SligSpawner_4D79F0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == 1 || loadMode == 2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        auto pSlig = ae_new<SligSpawner>();
        if (pSlig)
        {
            pSlig->ctor_409740(pSligTlv, tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_ElectricWall_4DA020(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
    }
    else
    {
        auto pElectricWall = ae_new<ElectricWall>();
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
        auto pLiftMover = ae_new<LiftMover>();
        if (pLiftMover)
        {
            pLiftMover->ctor_40CCD0(static_cast<Path_LiftMover*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_MeatSack_4D8140(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{ 
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources(
        {
            { ResourceManager::Resource_Animation, 12 },
            { ResourceManager::Resource_Animation, 14 },
            { ResourceManager::Resource_Animation, 2023 } 
        });
        gMap_5C3030.LoadResourcesFromList_4DBE70("MTHROW.BND", kResources.AsList(), loadMode, 0);
    }
    else
    {
        auto pMeatSack = ae_new<MeatSack>();
        if (pMeatSack)
        {
            pMeatSack->ctor_46A410(static_cast<Path_MeatSack*>(pTlv), tlvInfo.all);
        }
    }
}

static CompileTimeResourceList<12> kScrabResources(
{
    { ResourceManager::Resource_Animation, 700 },
    { ResourceManager::Resource_Animation, 706 },
    { ResourceManager::Resource_Animation, 708 },
    { ResourceManager::Resource_Animation, 710 },
    { ResourceManager::Resource_Animation, 705 },
    { ResourceManager::Resource_Animation, 709 },
    { ResourceManager::Resource_Animation, 704 },
    { ResourceManager::Resource_Animation, 711 },
    { ResourceManager::Resource_Animation, 701 },
    { ResourceManager::Resource_Animation, 702 },
    { ResourceManager::Resource_Animation, 703 },
    { ResourceManager::Resource_Animation, 713 }
});

EXPORT void CC Factory_Scrab_4D9200(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        Map::LoadResourcesFromList_4DBE70("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        auto pScrab = ae_new<Scrab>();
        if (pScrab)
        {
            pScrab->ctor_4A3C40(static_cast<Path_Scrab*>(pTlv), tlvInfo.all, 0);
        }
    }
}

EXPORT void CC Factory_Null_4DA130(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ScrabLeftBound_4D6A80(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_ScrabRightBound_4D6AA0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_SligBoundRight_4D79A0(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    Factory_SligBoundLeft_4D7740(pTlv, pPath, tlvInfo, loadMode);
}

EXPORT void CC Factory_SligPersist_4D79D0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_EnemyStopper_4D6AC0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_InvisibleSwitch_4D6E40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pInvisibleSwitch = ae_new<InvisibleSwitch>();
        if (pInvisibleSwitch)
        {
            pInvisibleSwitch->ctor_45FA70(static_cast<Path_InvisibleSwitch*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Mudokon_4D8EC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pMudTlv = static_cast<Path_Mudokon*>(pTlv);
    if (loadMode == 1 || loadMode == 2)
    {
        // TODO: Ids
        gMap_5C3030.LoadResource_4DBE00("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEKNBK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEEDGE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kRockShadowResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("MUDIDLE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, loadMode);

        static CompileTimeResourceList<5> kPalResources(
        {
            { ResourceManager::Resource_Palt, AEResourceID::kMudangryResID },
            { ResourceManager::Resource_Palt, AEResourceID::kMudsadResID },
            { ResourceManager::Resource_Palt, AEResourceID::kMudwiredResID },
            { ResourceManager::Resource_Palt, AEResourceID::kMudblindResID },
            { ResourceManager::Resource_Palt, AEResourceID::kMudsickResID }
        });

        gMap_5C3030.LoadResourcesFromList_4DBE70("MUDPAL.BND", kPalResources.AsList(), loadMode, 0);
        if (pMudTlv->field_12_state == Mud_State::eChisle_0)
        {
            gMap_5C3030.LoadResource_4DBE00("MUDCHSL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, loadMode);
        }
        else
        {
            gMap_5C3030.LoadResource_4DBE00("MUDSCRUB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, loadMode);
        }

        static CompileTimeResourceList<2> kResources(
        {
            { ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID },
            { ResourceManager::Resource_Animation, AEResourceID::kMudbtlnkResID }  // TODO: Another block of 3 after this exists that is never used ??
        });

        gMap_5C3030.LoadResourcesFromList_4DBE70("MUDWORK.BND", kResources.AsList(), loadMode);
    }
    else
    {
        auto pMud = ae_new<Mudokon>();
        if (pMud)
        {
            pMud->ctor_474F30(pMudTlv, tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_ZSligCover_4D6A60(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_DoorFlame_4D70D0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("FIRE.BAN", ResourceManager::Resource_Animation, 304, loadMode);
        gMap_5C3030.LoadResource_4DBE00("GLOW1.BAN", ResourceManager::Resource_Animation, 361, loadMode);
    }
    else
    {
        auto pDoorFlame = ae_new<DoorFlame>();
        if (pDoorFlame)
        {
            pDoorFlame->ctor_45E460(static_cast<Path_DoorFlame*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_MovingBomb_4D8A50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pMovingBombTlv = static_cast<Path_MovingBomb*>(pTlv);
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("MOVEBOMB.BAN", ResourceManager::Resource_Animation, 3006, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, pMovingBombTlv->field_1A_disable_resources & 1);
    }
    else
    {
        auto pMovingBomb = ae_new<MovingBomb>();
        if (pMovingBomb)
        {
            pMovingBomb->ctor_46FD40(pMovingBombTlv, tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Null_4D8A30(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_4D7160(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D6AE0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_TimerTrigger_4DA0E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pTimerTrigger = ae_new<TimerTrigger>();
        if (pTimerTrigger)
        {
            pTimerTrigger->ctor_4CDC20(static_cast<Path_TimerTrigger*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SecurityDoor_4DA150(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("SECDOOR.BAN", ResourceManager::Resource_Animation, 6027, loadMode);
    }
    else
    {
        auto pSecurityDoor = ae_new<SecurityDoor>();
        if (pSecurityDoor)
        {
            pSecurityDoor->ctor_4ABFC0(static_cast<Path_SecurityDoor*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_GrenadeMachine_4DA1C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pTlvBooMachine = static_cast<Path_BoomMachine*>(pTlv);
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources(
        {
            { ResourceManager::Resource_Animation, 14 },
            { ResourceManager::Resource_Animation, 12 },
            { ResourceManager::Resource_Animation, 6005 } 
        });

        gMap_5C3030.LoadResourcesFromList_4DBE70("GTHROW.BND", kResources.AsList(), loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode);
        gMap_5C3030.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode, pTlvBooMachine->field_14_disabled_resources & 2);
    }
    else
    {
        auto pBoomMachine = ae_new<BoomMachine>();
        if (pBoomMachine)
        {
            pBoomMachine->ctor_445B30(pTlvBooMachine, tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_BackgroundAnimation_4D84F0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pAnim = ae_new<BackgroundAnimation>();
        if (pAnim)
        {
            pAnim->ctor_40D270(static_cast<BackgroundAnimation_TLV*>(pTlv), tlvOffsetLevelIdPathId);
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

    auto pLCD = ae_new<LCDScreen>();
    if (pLCD)
    {
        pLCD->ctor_460680(static_cast<Path_LCDScreen*>(pTlv), tlvOffsetLevelIdPathId);
    }
}

EXPORT void CC Factory_HandStone_4D9FA0(Path_TLV*, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, FALSE);
        Map::LoadResource_4DBE00("SPOTLITE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, loadmode, FALSE);
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
        auto pCreditsController = ae_new<CreditsController>();
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
    auto pStatsSign = ae_new<StatsSign>();
    if (pStatsSign)
    {
        pStatsSign->ctor_47B600(static_cast<Path_StatsSign*>(pTlv), tlvOffsetLevelIdPathId);
    }
}

EXPORT void CC Factory_WheelSyncer_4DA430(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pWheelSyncer = ae_new<WheelSyncer>();
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
        auto pMusicTrigger = ae_new<MusicTrigger>();
        if (pMusicTrigger)
        {
            pMusicTrigger->ctor_47FE40(static_cast<Path_MusicTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void CC Factory_Light_4D8590(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_SlogSpawner_4D8BB0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        auto pSlogSpawner = ae_new<SlogSpawner>();
        if (pSlogSpawner)
        {
            pSlogSpawner->ctor_4C7FF0(static_cast<Path_SlogSpawner*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_GasCountdown_4DA480(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::Resource_Font, 2, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEGAS.BAN", ResourceManager::Resource_Animation, 118, loadMode);
    }
    else
    {
        auto pCountDown = ae_new<GasCountDown>();
        if (pCountDown)
        {
            pCountDown->ctor_417010(static_cast<Path_GasCountDown*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_4D6C50(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_GasEmitter_4D8540(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{ 
    if (loadMode != 1 && loadMode != 2)
    {
        auto pGasEmitter = ae_new<GasEmitter>();
        if (pGasEmitter)
        {
            pGasEmitter->ctor_43CAA0(static_cast<Path_GasEmitter*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SlogHut_4DA500(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pSlogHut = ae_new<SlogHut>();
        if (pSlogHut)
        {
            pSlogHut->ctor_4C4070(static_cast<Path_SlogHut*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Glukkon_4DA550(Path_TLV* pTlv , Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    static CompileTimeResourceList<2> kGlukkon_563534({
        { ResourceManager::Resource_Animation, 800 },
        { ResourceManager::Resource_Animation, 801 }
    });

    static CompileTimeResourceList<2> kAslik_563548({
        { ResourceManager::Resource_Animation, 802 },
        { ResourceManager::Resource_Animation, 803 }
    });

    static CompileTimeResourceList<2> KDripik_56355C({
        { ResourceManager::Resource_Animation, 804 },
        { ResourceManager::Resource_Animation, 805 }
    });

    static CompileTimeResourceList<2> kPhleg_563570({
        { ResourceManager::Resource_Animation, 806 },
        { ResourceManager::Resource_Animation, 807 }
    });

    if (loadMode == 1 || loadMode == 2)
    {
        switch (gMap_5C3030.field_0_current_level)
        {
        case LevelIds::eFeeCoDepot_5:
        case LevelIds::eFeeCoDepot_Ender_12:
            gMap_5C3030.LoadResourcesFromList_4DBE70("ASLIK.BND", kAslik_563548.AsList(), loadMode);
            break;

        case LevelIds::eBarracks_6:
        case LevelIds::eBarracks_Ender_13:
            gMap_5C3030.LoadResourcesFromList_4DBE70("DRIPIK.BND", KDripik_56355C.AsList(), loadMode);
            break;

        case LevelIds::eBonewerkz_8:
        case LevelIds::eBonewerkz_Ender_14:
            gMap_5C3030.LoadResourcesFromList_4DBE70("PHLEG.BND", kPhleg_563570.AsList(), loadMode);
            break;

        default:
            gMap_5C3030.LoadResourcesFromList_4DBE70("GLUKKON.BND", kGlukkon_563534.AsList(), loadMode);
            break;
        }
    }
    else
    {
        auto pGlukkon = ae_new<Glukkon>();
        if (pGlukkon)
        {
            pGlukkon->ctor_43F030(static_cast<Path_Glukkon*>(pTlv), tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_KillUnsavedMudokons_4DA6E0(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_SoftLanding_4D6950(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }
EXPORT void CC Factory_Null_4D6B00(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Factory_Water_4DA730(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("WDROP.BAN", ResourceManager::Resource_Animation, AEResourceID::kWdropResID, loadmode);
        Map::LoadResource_4DBE00("SPLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplashResID, loadmode);
    }
    else
    {
        auto pWater = ae_new<Water>();
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
        Map::LoadResource_4DBE00("ABEWORK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, loadmode, FALSE);
        Map::LoadResource_4DBE00("WORKWHEL.BAN", ResourceManager::Resource_Animation, AEResourceID::kWorkwhelResID, loadmode, FALSE);
    }
    else
    {
        auto pWheel = ae_new<WorkWheel>();
        if (pWheel)
        {
            pWheel->ctor_4E35D0(static_cast<Path_Wheel*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Null_4DA850(Path_TLV* , Path*, TlvItemInfoUnion, __int16) { NOT_IMPLEMENTED(); }

EXPORT void CC Create_Gas_4DA8B0(__int16 layer, int neverUsed, Path_TLV* pTlv, int tlvInfo)
{
    if (!gGasInstanceCount_5BC214)
    {
        auto pGas = ae_new<LaughingGas>();
        if (pGas)
        {
            pGas->ctor_432400(layer, neverUsed, static_cast<Path_LaughingGas*>(pTlv), tlvInfo);
        }
    }
}

EXPORT void CC Factory_LaughingGas_4DA870(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        Create_Gas_4DA8B0(39, 2, pTlv, tlvInfo.all);
    }
}

static CompileTimeResourceList<1> kResources_5632E0({
    { ResourceManager::Resource_Animation, AEResourceID::kFlySligResID }
});

static void LoadFlyingSligResources(__int16 loadMode)
{
    gMap_5C3030.LoadResourcesFromList_4DBE70("FLYSLIG.BND", kResources_5632E0.AsList(), loadMode, 0);

    gMap_5C3030.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
    gMap_5C3030.LoadResource_4DBE00("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
    gMap_5C3030.LoadResource_4DBE00("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
    gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalResID, loadMode);
    gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
    gMap_5C3030.LoadResource_4DBE00("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
    gMap_5C3030.LoadResource_4DBE00("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
    gMap_5C3030.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
}


EXPORT void CC Factory_FlyingSlig_4D92E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        LoadFlyingSligResources(loadMode);
    }
    else
    {
        auto pFlyingSlig = ae_new<FlyingSlig>();
        if (pFlyingSlig)
        {
            pFlyingSlig->ctor_4342B0(static_cast<Path_FlyingSlig*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Fleech_4D8C30(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("FLEECH.BAN", ResourceManager::Resource_Animation, 900, loadMode);
        gMap_5C3030.LoadResource_4DBE00("FLEEBLOW.BAN", ResourceManager::Resource_Animation, 580, loadMode);
    }
    else if (
        gMap_5C3030.field_0_current_level != LevelIds::eMudancheeVault_Ender_7 || 
        gMap_5C3030.field_2_current_path != 9 || 
        gMap_5C3030.field_4_current_camera != 4)
    {
        auto pFleech = ae_new<Fleech>();
        if (pFleech)
        {
            pFleech->ctor_429DC0(static_cast<Path_Fleech*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Slurgs_4DA950(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("SLURG.BAN", ResourceManager::Resource_Animation, 306, loadMode);
    }
    else
    {
        auto pSlurg = ae_new<Slurg>();
        if (pSlurg)
        {
            pSlurg->ctor_4C84E0(static_cast<Path_Slurg*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SlamDoor_4DA9C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode == 1 || loadmode == 2)
    {
        switch (gMap_5C3030.field_0_current_level)
        {
        case LevelIds::eNecrum_2:
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            Map::LoadResource_4DBE00("SLAMVLTS.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlamResID, loadmode);
            break;
        default:
            Map::LoadResource_4DBE00("SLAM.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlamResID, loadmode);
            break;
        }
    }
    else
    {
        auto pSlamDoor = ae_new<SlamDoor>();
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
        auto pLevelLoader = ae_new<LevelLoader>();
        if (pLevelLoader)
        {
            pLevelLoader->ctor_4DD330(static_cast<Path_LevelLoader*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_DemoSpawnPoint_4D6990(Path_TLV* , Path*, TlvItemInfoUnion, __int16 loadMode)
{ 
    if (loadMode != 1 && loadMode != 2)
    {
        if (gAttract_5C1BA0)
        {
            if (!sDemoObj_dword_5D1E20)
            {
                auto pDemoPlayback = ae_new<DemoPlayback>();
                pDemoPlayback->ctor();
            }
        }
    }
}

EXPORT void CC Factory_Teleporter_4DAA90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pTeleporter = ae_new<Teleporter>();
        if (pTeleporter)
        {
            pTeleporter->ctor_4DC1E0(static_cast<Path_Teleporter*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SlurgSpawner_4DAB50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("SLURG.BAN", ResourceManager::Resource_Animation, 306, loadMode);
    }
    else
    {
        auto pSlurgSpawner = ae_new<SlurgSpawner>();
        if (pSlurgSpawner)
        {
            pSlurgSpawner->ctor_4C82E0(static_cast<Path_SlurgSpawner*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Grinder_4DABC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{ 
    if (loadmode == 1 || loadmode == 2)
    {
        Map::LoadResource_4DBE00("DRILL.BAN", ResourceManager::Resource_Animation, AEResourceID::kDrillResID, loadmode);
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode);
    }
    else
    {
        auto pGrinder = ae_new<Grinder>();
        if (pGrinder)
        {
            pGrinder->ctor_4200D0(static_cast<Path_Grinder*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_ColorfulMeter_4DAC60(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::Resource_Font, 2, loadMode);
    }
    else
    {
        auto pMeter = ae_new<ColourfulMeter>();
        if (pMeter)
        {
            pMeter->ctor_43CE40(static_cast<Path_ColourfulMeter*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_FlyingSligSpawner_4D9440(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        // TODO: Resource Ids
        static CompileTimeResourceList<1> kResources({
            { ResourceManager::Resource_Animation, AEResourceID::kFlySligResID }
        });

        gMap_5C3030.LoadResourcesFromList_4DBE70("FLYSLIG.BND", kResources.AsList(), loadMode);
        gMap_5C3030.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
    }
    else
    {
        auto pFlyingSligSpawner = ae_new<FlyingSligSpawner>();
        if (pFlyingSligSpawner)
        {
            pFlyingSligSpawner->ctor_433D50(static_cast<Path_FlyingSligSpawner*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_MineCar_4DACD0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("BAYROLL.BAN", ResourceManager::Resource_Animation, AEResourceID::kUnknownResID_6013, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("ABECAR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        auto pMineCar = ae_new<MineCar>();
        if (pMineCar)
        {
            pMineCar->ctor_46BC80(static_cast<Path_MineCar*>(pTlv), tlvInfo.all, 0, 0, 0);
        }
    }

}

EXPORT void CC Factory_BoneBag_4D80B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("BONEBAG.BAN", ResourceManager::Resource_Animation, 590, loadMode);

        // TODO: Resource Ids
        static CompileTimeResourceList<3> kResources({
            { ResourceManager::Resource_Animation, 12 },
            { ResourceManager::Resource_Animation, 14 },
            { ResourceManager::Resource_Animation, 591 }
        });

        gMap_5C3030.LoadResourcesFromList_4DBE70("BTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        auto pBoneBag = ae_new<BoneBag>();
        if (pBoneBag)
        {
            pBoneBag->ctor_4125C0(static_cast<Path_BoneBag*>(pTlv), tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_ExplosionSet_4DADC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        if (gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8)
        {
            gMap_5C3030.LoadResource_4DBE00("DRPROCK.BAN", ResourceManager::Resource_Animation, kDrpRockResID, loadMode);
            gMap_5C3030.LoadResource_4DBE00("FALLBONZ.BAN", ResourceManager::Resource_Animation, kF2rockResID, loadMode);
        }
        else
        {
            gMap_5C3030.LoadResource_4DBE00("DRPROCK.BAN", ResourceManager::Resource_Animation, kDrpRockResID, loadMode);
            gMap_5C3030.LoadResource_4DBE00("FALLROCK.BAN", ResourceManager::Resource_Animation, kF2rockResID, loadMode);
        }
        gMap_5C3030.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, kStickResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, kRockShadowResID, loadMode);
        gMap_5C3030.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, kDebrisID00, loadMode);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, kExplo2ResID, loadMode);
    }
    else
    {
        if (!pExplosionSet_5BBF68)
        {
            auto pExplosionSet = ae_new<ExplosionSet>();
            if (pExplosionSet)
            {
                pExplosionSet->ctor_414CA0();
            }
        }
        pExplosionSet_5BBF68->Init_4151D0(static_cast<Path_ExplosionSet*>(pTlv));
        Path::TLV_Reset_4DB8E0(tlvInfo.all, -1, 0, 0);
    }
}

EXPORT void CC Factory_MultiSwitchController_4D6C00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pMultiSwitchController = ae_new<MultiSwitchController>();
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
        Map::LoadResource_4DBE00("STATUSLT.BAN", ResourceManager::Resource_Animation, AEResourceID::kStatusLightResID, loadmode);
    }
    else
    {
        auto pStatusLight = ae_new<StatusLight>();
        if (pStatusLight)
        {
            pStatusLight->ctor_4D5250(static_cast<Path_StatusLight*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SlapLock_4DAF20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        Map::LoadResource_4DBE00("GHOSTTRP.BAN", ResourceManager::ResourceType::Resource_Animation, 1053, loadMode, 0);
    }
    else
    {
        auto pSlapLock = ae_new<SlapLock>();
        if (pSlapLock)
        {
            pSlapLock->ctor_43DC80(static_cast<Path_SlapLock*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_ParamiteWebLine_4D9120(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("WEB.BAN", ResourceManager::Resource_Animation, kWebResID, loadMode);
    }
    else
    {
        auto pWebLine = ae_new<ParamiteWebLine>();
        if (pWebLine)
        {
            pWebLine->ctor_4E1FC0(static_cast<Path_ParamiteWebLine*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_Alarm_4DAF90(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2)
    {
        auto pAlarm = ae_new<Alarm>();
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
        gMap_5C3030.LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode);
        gMap_5C3030.LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::Resource_Font, 2, loadmode);
        gMap_5C3030.LoadResource_4DBE00("BREWBTN.BAN", ResourceManager::Resource_Animation, AEResourceID::kUnknownResID_6016, loadmode);
        gMap_5C3030.LoadResource_4DBE00("EVILFART.BAN", ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, loadmode);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
    }
    else
    {
        auto pFartMachine = ae_new<FartMachine>();
        if (pFartMachine)
        {
            pFartMachine->ctor_413060(static_cast<Path_FartMachine*>(pTlv), tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_ScrabSpawner_4D9270(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        Map::LoadResourcesFromList_4DBE70("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        auto pScrabSpawner = ae_new<ScrabSpawner>();
        if (pScrabSpawner)
        {
            pScrabSpawner->ctor_4AB450(static_cast<Path_Scrab_Spawner*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_NakedSlig_4D95A0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {

        static CompileTimeResourceList<1> kResource(
        {
            { ResourceManager::Resource_Animation, 449 }
        });

        gMap_5C3030.LoadResourcesFromList_4DBE70("CRAWLSLG.BND", kResource.AsList(), loadMode);
        gMap_5C3030.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, 423, loadMode);
        gMap_5C3030.LoadResource_4DBE00("GRENADE.BAN", ResourceManager::Resource_Animation, 6005, loadMode);
        gMap_5C3030.LoadResource_4DBE00("SMEXP.BAN", ResourceManager::Resource_Animation, 372, loadMode);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode);
        gMap_5C3030.LoadResource_4DBE00("BIGFLASH.BAN", ResourceManager::Resource_Animation, 319, loadMode);
        gMap_5C3030.LoadResource_4DBE00("VAPOR.BAN", ResourceManager::Resource_Animation, 305, loadMode);
        gMap_5C3030.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode);
    }
    else
    {
        auto pNakedSlig = ae_new<NakedSlig>();
        if (pNakedSlig)
        {
            pNakedSlig->ctor_418C70(static_cast<Path_NakedSlig*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SligGetPants_4D9700(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == 1 || loadMode == 2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
        gMap_5C3030.LoadResource_4DBE00("LOCKER.BAN", ResourceManager::Resource_Animation, 448, loadMode);
    }
    else
    {
        auto pSligGetPantsAndWings = ae_new<SligGetPantsAndWings>();
        if (pSligGetPantsAndWings)
        {
            pSligGetPantsAndWings->ctor_465BF0(pTlv, tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_SligGetWings_4D9900(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        LoadFlyingSligResources(loadMode);
        gMap_5C3030.LoadResource_4DBE00("LOCKER.BAN", ResourceManager::Resource_Animation, 448, loadMode, 0);
    }
    else
    {
        auto pSligGetPantsAndWings = ae_new<SligGetPantsAndWings>();
        if (pSligGetPantsAndWings)
        {
            pSligGetPantsAndWings->ctor_465BF0(pTlv, tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_Greeter_4DAFE0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{ 
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("MFLARE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMflareResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("MOTION.BAN", ResourceManager::Resource_Animation, AEResourceID::kMotionResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("GREETER.BAN", ResourceManager::Resource_Animation, AEResourceID::kGreeterResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap_5C3030.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        auto pGreeter = ae_new<Greeter>();
        if (pGreeter)
        {
            pGreeter->ctor_4465B0(static_cast<Path_Greeter*>(pTlv), tlvInfo.all);
        }
    }

}

EXPORT void CC Factory_NakedSligButton_4DB100(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("CSLGBUTN.BAN", ResourceManager::Resource_Animation, 1057, loadMode);
    }
    else
    {
        auto pSligButton = ae_new<NakedSligButton>();
        if (pSligButton)
        {
            pSligButton->ctor_4148F0(static_cast<Path_NakedSligButton*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_GlukkonSwitch_4DA670(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("SECDOOR.BAN", ResourceManager::Resource_Animation, 6027, loadMode);
    }
    else
    {
        auto pGlukkonSwitch = ae_new<GlukkonSwitch>();
        if (pGlukkonSwitch)
        {
            pGlukkonSwitch->ctor_444E60(static_cast<Path_GlukkonSwitch*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_DoorBlocker_4DAAE0(Path_TLV* pTlv , Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("DOORLOCK.BAN", ResourceManager::Resource_Animation, 374, loadMode);
    }
    else
    {
        auto pDoorBlocker = ae_new<DoorBlocker>();
        if (pDoorBlocker)
        {
            pDoorBlocker->ctor_41F0A0(static_cast<Path_DoorBlocker*>(pTlv), tlvInfo.all);
        }
    }
}

EXPORT void CC Factory_TorturedMudokon_4D8430(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (!pTlv->field_1_unknown)
    {
        if (loadMode == 1 || loadMode == 2)
        {
            gMap_5C3030.LoadResource_4DBE00("MUDTORT.BAN", ResourceManager::Resource_Animation, AEResourceID::kTorturedMud, loadMode);
            gMap_5C3030.LoadResource_4DBE00("TEARS.BAN", ResourceManager::Resource_Animation, 375, loadMode); // TODO: Add ID
            gMap_5C3030.LoadResource_4DBE00("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
        }
        else
        {
            auto pTorturedMud = ae_new<TorturedMudokon>();
            if (pTorturedMud)
            {
                pTorturedMud->ctor_47BC60(static_cast<Path_TorturedMudokon*>(pTlv), tlvInfo.all);
            }
        }
    }
}

EXPORT void CC Factory_TrainDoor_4D6E90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        gMap_5C3030.LoadResource_4DBE00("TRAINDOR.BAN", ResourceManager::Resource_Animation, 2013, loadMode);
    }
    else
    {
        auto pTrainDoor = ae_new<TrainDoor>();
        if (pTrainDoor)
        {
            pTrainDoor->ctor_4DD090(static_cast<Path_TrainDoor*>(pTlv), tlvInfo.all);
        }
    }
}

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
        Factory_SecurityOrb_4D8D80,
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
        Factory_ChantSuppressor_WithArms_4D9E10,
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
        Factory_ParamiteWebLine_4D9120,
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
