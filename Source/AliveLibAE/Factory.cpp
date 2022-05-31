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
#include "LCDStatusBoard.hpp"
#include "BackgroundAnimation.hpp"
#include "HoistRocksEffect.hpp"
#include "Alarm.hpp"
#include "Door.hpp"
#include "WorkWheel.hpp"
#include "StatusLight.hpp"
#include "WheelSyncer.hpp"
#include "LevelLoader.hpp"
#include "InvisibleSwitch.hpp"
#include "Lever.hpp"
#include "MultiSwitchController.hpp"
#include "ShadowZone.hpp"
#include "Drill.hpp"
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
#include "BrewMachine.hpp"
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
#include "CrawlingSlig.hpp"
#include "Scrab.hpp"
#include "Paramite.hpp"
#include "Fleech.hpp"
#include "Slog.hpp"
#include "Slig.hpp"
#include "GasCountDown.hpp"
#include "GlukkonSwitch.hpp"
#include "CrawlingSligButton.hpp"
#include "SligGetPantsAndWings.hpp"
#include "BoomMachine.hpp"
#include "Meat.hpp"
#include "Bone.hpp"
#include "Rock.hpp"
#include "RockSack.hpp"
#include "SligSpawner.hpp"
#include "ScrabSpawner.hpp"
#include "SlogSpawner.hpp"
#include "ZzzSpawner.hpp"
#include "ParamiteWebLine.hpp"
#include "ExplosionSet.hpp"
#include "ColourfulMeter.hpp"
#include "SecurityDoor.hpp"
#include "LaughingGas.hpp"

template <size_t arraySize>
struct CompileTimeResourceList final
{
    s32 field_0_count = arraySize;
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

void Factory_MainMenuController_4D6DB0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (sMainMenuObjectCounter_BB4400 == 0)
    {
        if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
        {
            static CompileTimeResourceList<3> kResources({
                {ResourceManager::Resource_Animation, AEResourceID::kHighliteResID},
                {ResourceManager::Resource_Animation, AEResourceID::kOptionFlareResID},
                {ResourceManager::Resource_Palt, AEResourceID::kHighlitePalResID},
            });

            Map::LoadResourcesFromList_4DBE70("STARTANM.BND", kResources.AsList(), loadmode);
            Map::LoadResource_4DBE00("ABESPEK2.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbespek2ResID, loadmode);
        }
        else
        {
            ae_new<MainMenuController>(pTlv, tlvOffsetLevelIdPathId);
        }
    }
}

void Factory_ContinuePoint_4D6970(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_PathTransition_4D68A0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Hoist_4D9E90(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    Path_Hoist* pHoistTlv = static_cast<Path_Hoist*>(pTlv);
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
        Map::LoadResource_4DBE00("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadmode, 0);
    }
    else if (pHoistTlv->field_10_type == Path_Hoist::Type::eOffScreen)
    {
        // Its an off screen hoist so create the falling rocks effect
        ae_new<HoistRocksEffect>(pHoistTlv, tlvOffsetLevelIdPathId.all);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_Edge_4D68C0(Path_TLV* /*pTlv*/, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_DeathDrop_4D6930(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Door_4D6F00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABEDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbedoorResID, loadmode, FALSE);
        switch (gMap.mCurrentLevel)
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
                if (gMap.mOverlayId == 108)
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
        ae_new<Door>(static_cast<Path_Door*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Shadow_4D7200(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<ShadowZone>(static_cast<Path_ShadowZone*>(pTlv), pPath, tlvInfo.all);
    }
}


static void LoadLiftPointResources(const char_type* ropeBan, const char_type* liftBan, LoadMode loadMode)
{
    static CompileTimeResourceList<2> kResources({
        {ResourceManager::Resource_Animation, AEResourceID::kLiftWheelsResID},
        {ResourceManager::Resource_Animation, AEResourceID::kLiftResID},
    });

    gMap.LoadResource_4DBE00(ropeBan, ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadMode);
    gMap.LoadResourcesFromList_4DBE70(liftBan, kResources.AsList(), loadMode);
}

void Factory_LiftPoint_4D7250(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("ABELIFT.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, loadMode);
        switch (gMap.mCurrentLevel)
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
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (!(pObj->mFlags.Get(BaseGameObject::eDead)) && pObj->Type() == AETypes::eLiftPoint_78)
            {
                // Is there already an existing LiftPoint object for this TLV?
                LiftPoint* pLiftPoint = static_cast<LiftPoint*>(pObj);
                const s16 xpos = FP_GetExponent(pLiftPoint->field_B8_xpos);
                if (pTlv->field_8_top_left.field_0_x <= xpos && xpos <= pTlv->field_C_bottom_right.field_0_x && pLiftPoint->field_278_lift_point_id == pLiftTlv->field_10_lift_point_id && pLiftPoint->field_C2_lvl_number == gMap.mCurrentLevel && pLiftPoint->field_C0_path_number == gMap.mCurrentPath)
                {
                    // Yes so just reset its data
                    Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                    return;
                }
            }
        }

        // TODO: Meaning of the data in field_1_unknown for lift point
        if (pLiftTlv->field_1_tlv_state & 2 || (pLiftTlv->field_1_tlv_state == 0 && pLiftTlv->field_12_bStart_point == Choice_short::eYes_1))
        {
            ae_new<LiftPoint>(pLiftTlv, tlvOffsetLevelIdPathId.all);
            return;
        }
        else
        {
            // Find out where to create the lift point
            s16 pointNumber = 1;
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

                        const s32 tlvX = pTlv->field_8_top_left.field_0_x;
                        const s32 absX = pTlvIter->field_8_top_left.field_0_x - tlvX >= 0 ? pTlvIter->field_8_top_left.field_0_x - tlvX : tlvX - pTlvIter->field_8_top_left.field_0_x;

                        if (absX < 5 && pLiftPointIter->field_10_lift_point_id == pLiftTlv->field_10_lift_point_id && (pLiftPointIter->field_1_tlv_state & 2 || pLiftPointIter->field_1_tlv_state == 0) && pLiftPointIter->field_12_bStart_point == Choice_short::eYes_1)
                        {
                            ae_new<LiftPoint>(pLiftPointIter, tlvOffsetLevelIdPathId.all);
                            return;
                        }
                    }

                    pTlvIter = Path::Next_TLV_4DB6A0(pTlvIter);
                }
                pointNumber++;
            }

            // Default to original
            ae_new<LiftPoint>(pLiftTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

void Factory_ExpressWell_4D7D90(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("ABEWELL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbewellResID, loadmode);
        gMap.LoadResource_4DBE00("WELLLEAF.BAN", ResourceManager::Resource_Animation, AEResourceID::kWellLeafResID, loadmode);
    }
    else
    {
        Path_WellBase* pWellTlv = static_cast<Path_WellBase*>(pTlv);
        const FP xpos = FP_FromInteger(pWellTlv->field_8_top_left.field_0_x);
        const FP ypos = FP_FromInteger(pWellTlv->field_8_top_left.field_2_y + 5);
        ae_new<Well>(pWellTlv, xpos, ypos, tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Dove_4D7E90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pDoveTlv = static_cast<Path_Dove*>(pTlv);

        const s16 width = pDoveTlv->field_C_bottom_right.field_0_x - pDoveTlv->field_8_top_left.field_0_x;
        const s16 height = pDoveTlv->field_C_bottom_right.field_2_y - pDoveTlv->field_8_top_left.field_2_y;

        for (s32 i = 0; i < pDoveTlv->field_10_dove_count; i++)
        {
            const AnimRecord& doveRec = AnimRec(AnimId::Dove_Idle);
            auto pDove = ae_new<Dove>(doveRec.mFrameTableOffset,
                                      doveRec.mMaxW,
                                      doveRec.mMaxH,
                                      doveRec.mResourceId,
                                      tlvInfo.all,
                                      pDoveTlv->field_14_scale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1));

            s16 ypos = 0;
            if (pDoveTlv->field_12_pixel_perfect == Choice_short::eYes_1)
            {
                pDove->field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
                ypos = pTlv->field_8_top_left.field_2_y;
            }
            else
            {
                pDove->field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + width * Math_NextRandom() / 256);
                ypos = pTlv->field_8_top_left.field_2_y + height * Math_NextRandom() / 256;
            }
            pDove->field_BC_ypos = FP_FromInteger(ypos) + FP_FromInteger(10);
        }
    }
}

void Factory_RockSack_4D8040(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAberockResID}});
        gMap.LoadResourcesFromList_4DBE70("RTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        ae_new<RockSack>(static_cast<Path_RockSack*>(pTlv), tlvInfo.all);
    }
}

void Factory_FallingItem_4D81B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == LevelIds::eBonewerkz_8)
        {
            gMap.LoadResource_4DBE00("FALLBONZ.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            gMap.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadmode);
            gMap.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadmode);
            gMap.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadmode);
            gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        }
        else
        {
            gMap.LoadResource_4DBE00("FALLROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            gMap.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadmode);
            gMap.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadmode);
            gMap.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadmode);
        }
    }
    else
    {
        ae_new<FallingItem>(static_cast<Path_FallingItem*>(pTlv), tlvInfo.all);
    }
}

void Factory_PullRingRope_4D8320(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode);
        switch (gMap.mCurrentLevel)
        {
            case LevelIds::eNecrum_2:
            case LevelIds::eMudomoVault_3:
            case LevelIds::eMudancheeVault_4:
            case LevelIds::eMudancheeVault_Ender_7:
            case LevelIds::eMudomoVault_Ender_11:
                gMap.LoadResource_4DBE00("NECROPE.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
                break;
            default:
                gMap.LoadResource_4DBE00("ROPES.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
                break;
        }
        gMap.LoadResource_4DBE00("PULLRING.BAN", ResourceManager::Resource_Animation, AEResourceID::kPullringResID, loadmode);
    }
    else
    {
        ae_new<PullRingRope>(static_cast<Path_PullRingRope*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_TimedMine_4D87C0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    auto mine_tlv = static_cast<Path_TimedMine*>(pTlv);

    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode, mine_tlv->field_18_disabled_resources & 1);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode, mine_tlv->field_18_disabled_resources & 2);

        static CompileTimeResourceList<2> sTimedMineResourceList_563368({
            {ResourceManager::Resource_Animation, AEResourceID::kBombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBombflshResID},
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            {ResourceManager::Resource_Animation, AEResourceID::kAbebombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID},
        });

        Map::LoadResourcesFromList_4DBE70("BOMB.BND", sTimedMineResourceList_563368.AsList(), loadmode, 0);
        Map::LoadResourcesFromList_4DBE70("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        ae_new<TimedMine>(mine_tlv, tlvOffsetLevelIdPathId);
    }
}

static CompileTimeResourceList<3> kResources_5632A0(
    {{ResourceManager::Resource_Animation, AEResourceID::kSlgzshotResID},
     {ResourceManager::Resource_Animation, AEResourceID::kZflashResID},
     {ResourceManager::Resource_Animation, AEResourceID::kAbeknokzResID}});

static CompileTimeResourceList<4> kResources_5632BC(
    {{ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID},
     {ResourceManager::Resource_Animation, AEResourceID::kSlgknbkResID},
     {ResourceManager::Resource_Animation, AEResourceID::kBigflashResID},
     {ResourceManager::Resource_Animation, AEResourceID::kShellResID}});

static void LoadWalkingSligResources(s16 disabledResources, LoadMode loadMode)
{
    gMap.LoadResource_4DBE00("SLGLEVER.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, loadMode, disabledResources & 1);
    gMap.LoadResource_4DBE00("SLGLIFT.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, loadMode, disabledResources & 2);
    gMap.LoadResource_4DBE00("SLGSLEEP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, loadMode, disabledResources & 0x40);
    gMap.LoadResource_4DBE00("SLGEDGE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, loadMode, disabledResources & 0x100);
    gMap.LoadResource_4DBE00("SLGSMASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, loadMode, disabledResources & 0x200);
    gMap.LoadResource_4DBE00("SLGBEAT.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, loadMode, disabledResources & 0x400);
    gMap.LoadResource_4DBE00("SLGKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, loadMode, disabledResources & 0x80);
    gMap.LoadResourcesFromList_4DBE70("SLIGZ.BND", kResources_5632A0.AsList(), loadMode, disabledResources & 4);
    gMap.LoadResourcesFromList_4DBE70("SLIG.BND", kResources_5632BC.AsList(), loadMode);
    gMap.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
    gMap.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
}

void Factory_Slig_4D7BC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        ae_new<Slig>(pSligTlv, tlvInfo.all);
    }
}

static void LoadSlogResources(LoadMode loadMode)
{
    static CompileTimeResourceList<4> kResources(
        {{ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogrstnResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogattkResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogidleResID}});

    gMap.LoadResourcesFromList_4DBE70("SLOG.BND", kResources.AsList(), loadMode);
    gMap.LoadResource_4DBE00("DOGKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, loadMode);
}

void Factory_Slog_4D8B20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        ae_new<Slog>(static_cast<Path_Slog*>(pTlv), tlvInfo.all);
    }
}

void Factory_Switch_4D8CF0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("SWITCH1.BAN", ResourceManager::Resource_Animation, AEResourceID::kLeverResID, loadmode);
        Map::LoadResource_4DBE00("ABEPULL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, loadmode);
    }
    else
    {
        ae_new<Lever>(static_cast<Path_Lever*>(pTlv), tlvInfo.all);
    }
}

void Factory_Null_4D69E0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_SecurityOrb_4D8D80(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    Path_SecurityOrb* pSecurityOrbTlv = static_cast<Path_SecurityOrb*>(pTlv);
    const s16 disabledResources = pSecurityOrbTlv->field_12_disabled_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("MAIMORB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMaimGameResID, loadMode);
        gMap.LoadResource_4DBE00("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode); 
        gMap.LoadResource_4DBE00("SPARKS.BAN", ResourceManager::Resource_Animation, AEResourceID::kSparksResID, loadMode);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, disabledResources & 1);
        gMap.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, disabledResources & 0x10);
        gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        ae_new<SecurityOrb>(pSecurityOrbTlv, tlvInfo.all);
    }
}

void Factory_Null_4D6A00(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Pulley_4D6A20(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    // Nothing to do.
}

void Factory_AbeStart_4D9030(Path_TLV* pTlv, Path*, TlvItemInfoUnion, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2 && !gAttract_5C1BA0)
    {
        if (!pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300 = ae_new<PauseMenu>();
        }

        if (sActiveHero_5C1B68 == spAbe_554D5C)
        {
            const AnimRecord& rec = AnimRec(AnimId::Mudokon_Walk);
            sActiveHero_5C1B68 = ae_new<Abe>(rec.mFrameTableOffset, 85, 57, 55);
            if (sActiveHero_5C1B68)
            {
                sActiveHero_5C1B68->field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
                sActiveHero_5C1B68->field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
            }
        }
    }
}

void Factory_WellExpress_4D7E60(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    Factory_ExpressWell_4D7D90(pTlv, pPath, tlvOffsetLevelIdPathId, loadmode);
}

void Factory_Mine_4D8890(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    auto mine_tlv = static_cast<Path_Mine*>(pTlv);

    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode, mine_tlv->field_16_disabled_resources & 1);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode, mine_tlv->field_16_disabled_resources & 2);

        static CompileTimeResourceList<2> sMineResourceList_56337C({
            {ResourceManager::Resource_Animation, AEResourceID::kLandmineResID},
            {ResourceManager::Resource_Animation, AEResourceID::kMineflshResID},
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            {ResourceManager::Resource_Animation, AEResourceID::kAbebombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID},
        });

        Map::LoadResourcesFromList_4DBE70("MINE.BND", sMineResourceList_56337C.AsList(), loadmode, 0);
        Map::LoadResourcesFromList_4DBE70("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        ae_new<Mine>(mine_tlv, tlvOffsetLevelIdPathId);
    }
}

void Factory_UXB_4D8960(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto uxb_tlv = static_cast<Path_UXB*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, uxb_tlv->field_18_disabled_resources & 1);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, uxb_tlv->field_18_disabled_resources & 2);

        static CompileTimeResourceList<3> sUXBResourceList_563390({
            {ResourceManager::Resource_Animation, AEResourceID::kUXBResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBombflshResID},
            {ResourceManager::Resource_Palt, AEResourceID::kGrenflshResID},
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            {ResourceManager::Resource_Animation, AEResourceID::kAbebombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID},
        });

        Map::LoadResourcesFromList_4DBE70("UXB.BND", sUXBResourceList_563390.AsList(), loadMode, 0);
        Map::LoadResourcesFromList_4DBE70("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadMode, 0);
    }
    else
    {
        ae_new<UXB>(uxb_tlv, tlvOffsetLevelIdPathId);
    }
}

void Factory_Paramite_4D9190(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<9> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kArjbasicResID},
             {ResourceManager::Resource_Animation, AEResourceID::kArjpumpResID},
             {ResourceManager::Resource_Animation, AEResourceID::kArjponceResID},
             {ResourceManager::Resource_Animation, AEResourceID::kArjclimbResID},
             {ResourceManager::Resource_Animation, AEResourceID::kArjeatResID},
             {ResourceManager::Resource_Animation, AEResourceID::kArjfalrkResID},
             {ResourceManager::Resource_Animation, AEResourceID::kArjwaspResID},
             {ResourceManager::Resource_Animation, AEResourceID::kArjscrchResID},
             {ResourceManager::Resource_Animation, AEResourceID::kWebResID}});
        gMap.LoadResourcesFromList_4DBE70("PARAMITE.BND", kResources.AsList(), loadMode);
    }
    else
    {
         ae_new<Paramite>(static_cast<Path_Paramite*>(pTlv), tlvInfo.all);
    }
}

void Factory_MovieHandStone_4D9F50(Path_TLV*, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, FALSE);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_BirdPortal_4D9AA0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<Path_BirdPortal*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources_5634E8({
            {ResourceManager::Resource_Animation, AEResourceID::kPortalTerminatorResID},
            {ResourceManager::Resource_Animation, AEResourceID::kPortliteResID},
            {ResourceManager::Resource_Animation, AEResourceID::kPortlitResID},
        });

        gMap.LoadResourcesFromList_4DBE70("PORTAL.BND", kResources_5634E8.AsList(), loadMode);
        if (pBirdPortalTlv->field_1C_portal_type == PortalType::eShrykull_2)
        {
            static CompileTimeResourceList<2> kResources_563504({
                {ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID},
                {ResourceManager::Resource_Animation, AEResourceID::kShrmorphResID},
            });

            gMap.LoadResourcesFromList_4DBE70("SHRYPORT.BND", kResources_563504.AsList(), loadMode);
            gMap.LoadResource_4DBE00("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode);
        }
    }
    else if (SwitchStates_Get_466020(pBirdPortalTlv->field_20_create_portal_switch_id))
    {
        ae_new<BirdPortal>(pBirdPortalTlv, tlvInfo.all);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvInfo.all, -1, 0, 0);
    }
}

void Factory_PortalExit_4D9A80(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_TrapDoor_4D9B90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case LevelIds::eMudomoVault_3:
            case LevelIds::eMudancheeVault_4:
            case LevelIds::eMudancheeVault_Ender_7:
            case LevelIds::eMudomoVault_Ender_11:
                gMap.LoadResource_4DBE00("VLTSTRAP.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
                break;

            default:
                gMap.LoadResource_4DBE00("TRAPDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
                break;
        }
    }
    else
    {
        // TODO: OG bug - actually passes Path* instead of Map* .. probably works because this parameter never gets used!
        ae_new<TrapDoor>(static_cast<Path_TrapDoor*>(pTlv), &gMap, tlvInfo.all);
    }
}

void Factory_RollingRock_4D9C70(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

static Path_TLV* FindMatchingSligTLV(Path_TLV* pTlvIter, Path_SligBound* pTlv)
{
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::Slig_15 && pTlv->field_10_slig_bound_id == static_cast<Path_Slig*>(pTlvIter)->field_38_slig_bound_id && !pTlvIter->field_0_flags.Get(TLV_Flags::eBit2_Destroyed))
        {
            return pTlvIter;
        }
        pTlvIter = Path::Next_TLV_4DB6A0(pTlvIter);
    }
    return nullptr;
}

void Factory_SligBoundLeft_4D7740(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pBound = static_cast<Path_SligBound*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(pBound->field_12_disabled_resources, loadMode);
    }
    else
    {
        pBound->field_0_flags.Clear(TLV_Flags::eBit1_Created);
        pBound->field_0_flags.Clear(TLV_Flags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit1_Created);
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit2_Destroyed);

                tlvInfo.parts.tlvOffset += static_cast<u16>(
                    reinterpret_cast<const u8*>(pTlvIter) - reinterpret_cast<const u8*>(pBound));
                ae_new<Slig>(static_cast<Path_Slig*>(pTlvIter), tlvInfo.all);

                return;
            }
        }
    }
}

void Factory_InvisibleZone_4D6A40(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_FootSwitch_4D9D00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
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
        ae_new<FootSwitch>(static_cast<Path_FootSwitch*>(pTlv), tlvInfo.all);
    }
}

void Factory_ChantSuppressor_WithArms_4D9E10(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    // TODO: Most of the code for the chant suppressor with arms is still in the game
    // check if it can be re-added at some point.
    Factory_SecurityOrb_4D8D80(pTlv, pPath, tlvInfo, loadMode);
}

void Factory_MotionDetector_4D9E40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ae_new<MotionDetector>(static_cast<Path_MotionDetector*>(pTlv), tlvInfo.all, nullptr);
    }
}

void Factory_SligSpawner_4D79F0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        ae_new<SligSpawner>(pSligTlv, tlvInfo.all);
    }
}

void Factory_ElectricWall_4DA020(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
    }
    else
    {
        ae_new<ElectricWall>(static_cast<Path_ElectricWall*>(pTlv), tlvInfo.all);
    }
}

void Factory_LiftMover_4DA090(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<LiftMover>(static_cast<Path_LiftMover*>(pTlv), tlvInfo.all);
    }
}

void Factory_MeatSack_4D8140(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kMeatResID}});
        gMap.LoadResourcesFromList_4DBE70("MTHROW.BND", kResources.AsList(), loadMode, 0);
    }
    else
    {
        ae_new<MeatSack>(static_cast<Path_MeatSack*>(pTlv), tlvInfo.all);
    }
}

static CompileTimeResourceList<12> kScrabResources(
    {{ResourceManager::Resource_Animation, AEResourceID::kArsbasicResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArsdanceResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArsdeadResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArsgrwlResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArshowlResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArsroarResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArswhirlResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArschewResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArseatResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArsprceResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArsskwrResID},
     {ResourceManager::Resource_Animation, AEResourceID::kArscrshResID}});

void Factory_Scrab_4D9200(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResourcesFromList_4DBE70("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        ae_new<Scrab>(static_cast<Path_Scrab*>(pTlv), tlvInfo.all, ScrabSpawnDirection::eNone_0);
    }
}

void Factory_Null_4DA130(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_ScrabLeftBound_4D6A80(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_ScrabRightBound_4D6AA0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_SligBoundRight_4D79A0(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    Factory_SligBoundLeft_4D7740(pTlv, pPath, tlvInfo, loadMode);
}

void Factory_SligPersist_4D79D0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_EnemyStopper_4D6AC0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_InvisibleSwitch_4D6E40(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ae_new<InvisibleSwitch>(static_cast<Path_InvisibleSwitch*>(pTlv), tlvInfo.all);
    }
}

void Factory_Mudokon_4D8EC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pMudTlv = static_cast<Path_Mudokon*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, loadMode);
        gMap.LoadResource_4DBE00("ABEKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, loadMode);
        gMap.LoadResource_4DBE00("ABEKNBK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, loadMode);
        gMap.LoadResource_4DBE00("ABEEDGE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, loadMode);
        gMap.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
        gMap.LoadResource_4DBE00("MUDIDLE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, loadMode);

        static CompileTimeResourceList<5> kPalResources(
            {{ResourceManager::Resource_Palt, AEResourceID::kMudangryResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudsadResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudwiredResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudblindResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudsickResID}});

        gMap.LoadResourcesFromList_4DBE70("MUDPAL.BND", kPalResources.AsList(), loadMode, 0);
        if (pMudTlv->field_12_state == Mud_State::eChisle_0)
        {
            gMap.LoadResource_4DBE00("MUDCHSL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, loadMode);
        }
        else
        {
            gMap.LoadResource_4DBE00("MUDSCRUB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, loadMode);
        }

        static CompileTimeResourceList<2> kResources(
            {
                {ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID},
                {ResourceManager::Resource_Animation, AEResourceID::kMudbtlnkResID} // TODO: Another block of 3 after this exists that is never used ??
            });

        gMap.LoadResourcesFromList_4DBE70("MUDWORK.BND", kResources.AsList(), loadMode);
    }
    else
    {
        ae_new<Mudokon>(pMudTlv, tlvInfo.all);
    }
}

void Factory_ZSligCover_4D6A60(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_DoorFlame_4D70D0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("FIRE.BAN", ResourceManager::Resource_Animation, AEResourceID::kHubFireResID, loadMode);
        gMap.LoadResource_4DBE00("GLOW1.BAN", ResourceManager::Resource_Animation, AEResourceID::kGlowResID, loadMode);
    }
    else
    {
        ae_new<DoorFlame>(static_cast<Path_DoorFlame*>(pTlv), tlvInfo.all);
    }
}

void Factory_MovingBomb_4D8A50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<Path_MovingBomb*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("MOVEBOMB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMovebombResID, loadMode, 0);
        gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, pMovingBombTlv->field_1A_disabled_resources & 1);
    }
    else
    {
        ae_new<MovingBomb>(pMovingBombTlv, tlvInfo.all);
    }
}

void Factory_Null_4D8A30(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_4D7160(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_Null_4D6AE0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_TimerTrigger_4DA0E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::ConstructObject_0)
    {
        ae_new<TimerTrigger>(static_cast<Path_TimerTrigger*>(pTlv), tlvInfo.all);
    }
}

void Factory_SecurityDoor_4DA150(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("SECDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kR1sdosResID_6027, loadMode);
    }
    else
    {
        ae_new<SecurityDoor>(static_cast<Path_SecurityDoor*>(pTlv), tlvInfo.all);
    }
}

void Factory_GrenadeMachine_4DA1C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pTlvBooMachine = static_cast<Path_BoomMachine*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID}});

        gMap.LoadResourcesFromList_4DBE70("GTHROW.BND", kResources.AsList(), loadMode, 0);
        gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        gMap.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, pTlvBooMachine->field_14_disabled_resources & 2);
    }
    else
    {
        ae_new<BoomMachine>(pTlvBooMachine, tlvInfo.all);
    }
}

void Factory_BackgroundAnimation_4D84F0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<BackgroundAnimation>(static_cast<Path_BackgroundAnimation*>(pTlv), tlvOffsetLevelIdPathId);
    }
}

void Factory_LCD_4D6CF0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, 2, loadmode); // TODO: Add to resource ID enum
        return;
    }

    ae_new<LCDScreen>(static_cast<Path_LCDScreen*>(pTlv), tlvOffsetLevelIdPathId);
}

void Factory_HandStone_4D9FA0(Path_TLV*, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, FALSE);
        Map::LoadResource_4DBE00("SPOTLITE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, loadmode, FALSE);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_CreditsController_4D6D60(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2 && !sDoesCreditsControllerExist_5C1B90)
    {
        ae_new<CreditsController>(pTlv, tlvInfo.all);
    }
}

void Factory_Null_4D6910(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_StatusBoard_4DA3C0(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
        return;
    }
    
    ae_new<LCDStatusBoard>(static_cast<Path_LCDStatusBoard*>(pTlv), tlvOffsetLevelIdPathId);
}

void Factory_WheelSyncer_4DA430(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<WheelSyncer>(static_cast<Path_WheelSyncer*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_MusicTrigger_4D71B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<MusicTrigger>(static_cast<Path_MusicTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Light_4D8590(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_SlogSpawner_4D8BB0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        ae_new<SlogSpawner>(static_cast<Path_SlogSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_GasCountdown_4DA480(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
        gMap.LoadResource_4DBE00("ABEGAS.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbegasResID, loadMode);
    }
    else
    {
        ae_new<GasCountDown>(static_cast<Path_GasCountDown*>(pTlv), tlvInfo.all);
    }
}

void Factory_4D6C50(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_GasEmitter_4D8540(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ae_new<GasEmitter>(static_cast<Path_GasEmitter*>(pTlv), tlvInfo.all);
    }
}

void Factory_ZzzSpawner_4DA500(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ae_new<ZzzSpawner>(static_cast<Path_ZzzSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_Glukkon_4DA550(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    static CompileTimeResourceList<2> kGlukkon_563534({{ResourceManager::Resource_Animation, AEResourceID::kGlkbasicResID},
                                                       {ResourceManager::Resource_Animation, AEResourceID::kGlukkonResID_801}});

    static CompileTimeResourceList<2> kAslik_563548({{ResourceManager::Resource_Animation, AEResourceID::kGlukAslikResID},
                                                     {ResourceManager::Resource_Animation, AEResourceID::kAslikResID_803}});

    static CompileTimeResourceList<2> KDripik_56355C({{ResourceManager::Resource_Animation, AEResourceID::kGlukDripikResID},
                                                      {ResourceManager::Resource_Animation, AEResourceID::kDripikResID_805}});

    static CompileTimeResourceList<2> kPhleg_563570({{ResourceManager::Resource_Animation, AEResourceID::kGlukPhlegResID},
                                                     {ResourceManager::Resource_Animation, AEResourceID::kPhlegResID_807}});

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case LevelIds::eFeeCoDepot_5:
            case LevelIds::eFeeCoDepot_Ender_12:
                gMap.LoadResourcesFromList_4DBE70("ASLIK.BND", kAslik_563548.AsList(), loadMode);
                break;

            case LevelIds::eBarracks_6:
            case LevelIds::eBarracks_Ender_13:
                gMap.LoadResourcesFromList_4DBE70("DRIPIK.BND", KDripik_56355C.AsList(), loadMode);
                break;

            case LevelIds::eBonewerkz_8:
            case LevelIds::eBonewerkz_Ender_14:
                gMap.LoadResourcesFromList_4DBE70("PHLEG.BND", kPhleg_563570.AsList(), loadMode);
                break;

            default:
                gMap.LoadResourcesFromList_4DBE70("GLUKKON.BND", kGlukkon_563534.AsList(), loadMode);
                break;
        }
    }
    else
    {
        ae_new<Glukkon>(static_cast<Path_Glukkon*>(pTlv), tlvInfo.all);
    }
}

void Factory_KillUnsavedMudokons_4DA6E0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_SoftLanding_4D6950(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_Null_4D6B00(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Water_4DA730(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("WDROP.BAN", ResourceManager::Resource_Animation, AEResourceID::kWaterDropResID, loadmode);
        Map::LoadResource_4DBE00("SPLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplashResID, loadmode);
    }
    else
    {
        ae_new<Water>(static_cast<Path_Water*>(pTlv), tlvInfo.all);
    }
}

void Factory_Null_4DA7C0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_WorkWheel_4D6B20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("ABEWORK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, loadmode, FALSE);
        Map::LoadResource_4DBE00("WORKWHEL.BAN", ResourceManager::Resource_Animation, AEResourceID::kWorkwhelResID, loadmode, FALSE);
    }
    else
    {
        ae_new<WorkWheel>(static_cast<Path_WorkWheel*>(pTlv), tlvInfo.all);
    }
}

void Factory_Null_4DA850(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Create_Gas_4DA8B0(Layer layer, s32 neverUsed, Path_TLV* pTlv, s32 tlvInfo)
{
    if (!gGasInstanceCount_5BC214)
    {
        ae_new<LaughingGas>(layer, neverUsed, static_cast<Path_LaughingGas*>(pTlv), tlvInfo);
    }
}

void Factory_LaughingGas_4DA870(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        Create_Gas_4DA8B0(Layer::eLayer_Above_FG1_39, 2, pTlv, tlvInfo.all);
    }
}

static CompileTimeResourceList<1> kResources_5632E0({{ResourceManager::Resource_Animation, AEResourceID::kFlySligResID}});

static void LoadFlyingSligResources(LoadMode loadMode)
{
    gMap.LoadResourcesFromList_4DBE70("FLYSLIG.BND", kResources_5632E0.AsList(), loadMode, 0);

    gMap.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
    gMap.LoadResource_4DBE00("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
    gMap.LoadResource_4DBE00("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
    gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
    gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
    gMap.LoadResource_4DBE00("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
    gMap.LoadResource_4DBE00("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
    gMap.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
}


void Factory_FlyingSlig_4D92E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources(loadMode);
    }
    else
    {
        ae_new<FlyingSlig>(static_cast<Path_FlyingSlig*>(pTlv), tlvInfo.all);
    }
}

void Factory_Fleech_4D8C30(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("FLEECH.BAN", ResourceManager::Resource_Animation, AEResourceID::kFleechResID, loadMode);
        gMap.LoadResource_4DBE00("FLEEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kFleeBlowResID_580, loadMode);
    }
    else if (
        gMap.mCurrentLevel != LevelIds::eMudancheeVault_Ender_7 || gMap.mCurrentPath != 9 || gMap.field_4_current_camera != 4)
    {
        ae_new<Fleech>(static_cast<Path_Fleech*>(pTlv), tlvInfo.all);
    }
}

void Factory_Slurgs_4DA950(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("SLURG.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, loadMode);
    }
    else
    {
        ae_new<Slurg>(static_cast<Path_Slurg*>(pTlv), tlvInfo.all);
    }
}

void Factory_SlamDoor_4DA9C0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
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
        ae_new<SlamDoor>(static_cast<Path_SlamDoor*>(pTlv), tlvInfo);
    }
}

void Factory_LevelLoader_4D6BB0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<LevelLoader>(static_cast<Path_LevelLoader*>(pTlv), tlvInfo.all);
    }
}

void Factory_DemoSpawnPoint_4D6990(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        if (gAttract_5C1BA0)
        {
            if (!sDemoObj_dword_5D1E20)
            {
                ae_new<DemoPlayback>();
            }
        }
    }
}

void Factory_Teleporter_4DAA90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<Teleporter>(static_cast<Path_Teleporter*>(pTlv), tlvInfo.all);
    }
}

void Factory_SlurgSpawner_4DAB50(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("SLURG.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, loadMode);
    }
    else
    {
        ae_new<SlurgSpawner>(static_cast<Path_SlurgSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_Drill_4DABC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("DRILL.BAN", ResourceManager::Resource_Animation, AEResourceID::kDrillResID, loadmode);
        Map::LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
        Map::LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode);
    }
    else
    {
        ae_new<Drill>(static_cast<Path_Drill*>(pTlv), tlvInfo.all);
    }
}

void Factory_ColorfulMeter_4DAC60(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
    }
    else
    {
        ae_new<ColourfulMeter>(static_cast<Path_ColourfulMeter*>(pTlv), tlvInfo.all);
    }
}

void Factory_FlyingSligSpawner_4D9440(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // TODO: Resource Ids
        static CompileTimeResourceList<1> kResources({{ResourceManager::Resource_Animation, AEResourceID::kFlySligResID}});

        gMap.LoadResourcesFromList_4DBE70("FLYSLIG.BND", kResources.AsList(), loadMode);
        gMap.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
        gMap.LoadResource_4DBE00("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
        gMap.LoadResource_4DBE00("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
        gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        gMap.LoadResource_4DBE00("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
        gMap.LoadResource_4DBE00("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
        gMap.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
    }
    else
    {
        ae_new<FlyingSligSpawner>(static_cast<Path_FlyingSligSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_MineCar_4DACD0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("BAYROLL.BAN", ResourceManager::Resource_Animation, AEResourceID::kBayrollResID_6013, loadMode, 0);
        gMap.LoadResource_4DBE00("ABECAR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId, loadMode, 0);
        gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        ae_new<MineCar>(static_cast<Path_MineCar*>(pTlv), tlvInfo.all, 0, 0, 0);
    }
}

void Factory_BoneBag_4D80B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("BONEBAG.BAN", ResourceManager::Resource_Animation, AEResourceID::kBoneBagResID_590, loadMode);

        static CompileTimeResourceList<3> kResources({{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
                                                      {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
                                                      {ResourceManager::Resource_Animation, AEResourceID::kBoneResID}});

        gMap.LoadResourcesFromList_4DBE70("BTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        ae_new<BoneBag>(static_cast<Path_BoneBag*>(pTlv), tlvInfo.all);
    }
}

void Factory_ExplosionSet_4DADC0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == LevelIds::eBonewerkz_8)
        {
            gMap.LoadResource_4DBE00("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadMode);
            gMap.LoadResource_4DBE00("FALLBONZ.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadMode);
        }
        else
        {
            gMap.LoadResource_4DBE00("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadMode);
            gMap.LoadResource_4DBE00("FALLROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadMode);
        }
        gMap.LoadResource_4DBE00("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadMode);
        gMap.LoadResource_4DBE00("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
        gMap.LoadResource_4DBE00("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadMode);
        gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode);
    }
    else
    {
        if (!pExplosionSet_5BBF68)
        {
            // pExplosionSet_5BBF68 is assigned in the ctor (OWI programming)
            ae_new<ExplosionSet>();
        }
        pExplosionSet_5BBF68->Init_4151D0(static_cast<Path_ExplosionSet*>(pTlv));
        Path::TLV_Reset_4DB8E0(tlvInfo.all, -1, 0, 0);
    }
}

void Factory_MultiSwitchController_4D6C00(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        ae_new<MultiSwitchController>(static_cast<Path_MultiSwitchController*>(pTlv), tlvInfo.all);
    }
}

void Factory_StatusLight_4DA7E0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("STATUSLT.BAN", ResourceManager::Resource_Animation, AEResourceID::kStatusLightResID, loadmode);
    }
    else
    {
        ae_new<StatusLight>(static_cast<Path_StatusLight*>(pTlv), tlvInfo.all);
    }
}

void Factory_SlapLock_4DAF20(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResource_4DBE00("GHOSTTRP.BAN", ResourceManager::ResourceType::Resource_Animation, AEResourceID::kGhostTrpResID_1053, loadMode, 0);
    }
    else
    {
        ae_new<SlapLock>(static_cast<Path_SlapLock*>(pTlv), tlvInfo.all);
    }
}

void Factory_ParamiteWebLine_4D9120(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("WEB.BAN", ResourceManager::Resource_Animation, AEResourceID::kWebResID, loadMode);
    }
    else
    {
        ae_new<ParamiteWebLine>(static_cast<Path_ParamiteWebLine*>(pTlv), tlvInfo.all);
    }
}

void Factory_Alarm_4DAF90(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::ConstructObject_0)
    {
        ae_new<Alarm>(static_cast<Path_Alarm*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_BrewMachine_4DA2B0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode);
        gMap.LoadResource_4DBE00("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
        gMap.LoadResource_4DBE00("BREWBTN.BAN", ResourceManager::Resource_Animation, AEResourceID::kBrewButtonResID_6016, loadmode);
        gMap.LoadResource_4DBE00("EVILFART.BAN", ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, loadmode);
        gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
    }
    else
    {
        ae_new<BrewMachine>(static_cast<Path_BrewMachine*>(pTlv), tlvInfo.all);
    }
}

void Factory_ScrabSpawner_4D9270(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResourcesFromList_4DBE70("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        ae_new<ScrabSpawner>(static_cast<Path_ScrabSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_NakedSlig_4D95A0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<1> kResource(
            {{ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligResID_449}});

        gMap.LoadResourcesFromList_4DBE70("CRAWLSLG.BND", kResource.AsList(), loadMode);
        gMap.LoadResource_4DBE00("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
        gMap.LoadResource_4DBE00("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
        gMap.LoadResource_4DBE00("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
        gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        gMap.LoadResource_4DBE00("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
        gMap.LoadResource_4DBE00("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
        gMap.LoadResource_4DBE00("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
    }
    else
    {
        ae_new<CrawlingSlig>(static_cast<Path_CrawlingSlig*>(pTlv), tlvInfo.all);
    }
}

void Factory_SligGetPants_4D9700(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
        gMap.LoadResource_4DBE00("LOCKER.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligLockerResID_448, loadMode);
    }
    else
    {
        ae_new<SligGetPantsAndWings>(pTlv, tlvInfo.all);
    }
}

void Factory_SligGetWings_4D9900(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources(loadMode);
        gMap.LoadResource_4DBE00("LOCKER.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligLockerResID_448, loadMode, 0);
    }
    else
    {
        ae_new<SligGetPantsAndWings>(pTlv, tlvInfo.all);
    }
}

void Factory_Greeter_4DAFE0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("MFLARE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMflareResID, loadMode, 0);
        gMap.LoadResource_4DBE00("MOTION.BAN", ResourceManager::Resource_Animation, AEResourceID::kMotionResID, loadMode, 0);
        gMap.LoadResource_4DBE00("GREETER.BAN", ResourceManager::Resource_Animation, AEResourceID::kGreeterResID, loadMode, 0);
        gMap.LoadResource_4DBE00("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode, 0);
        gMap.LoadResource_4DBE00("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        gMap.LoadResource_4DBE00("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap.LoadResource_4DBE00("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        ae_new<Greeter>(static_cast<Path_Greeter*>(pTlv), tlvInfo.all);
    }
}

void Factory_NakedSligButton_4DB100(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("CSLGBUTN.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligButtonResID_1057, loadMode);
    }
    else
    {
        ae_new<CrawlingSligButton>(static_cast<Path_CrawlingSligButton*>(pTlv), tlvInfo.all);
    }
}

void Factory_GlukkonSwitch_4DA670(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("SECDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kR1sdosResID_6027, loadMode);
    }
    else
    {
        ae_new<GlukkonSwitch>(static_cast<Path_GlukkonSwitch*>(pTlv), tlvInfo.all);
    }
}

void Factory_DoorBlocker_4DAAE0(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("DOORLOCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kDoorLockResID_374, loadMode);
    }
    else
    {
        ae_new<DoorBlocker>(static_cast<Path_DoorBlocker*>(pTlv), tlvInfo.all);
    }
}

void Factory_TorturedMudokon_4D8430(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (!pTlv->field_1_tlv_state)
    {
        if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
        {
            gMap.LoadResource_4DBE00("MUDTORT.BAN", ResourceManager::Resource_Animation, AEResourceID::kTorturedMud, loadMode);
            gMap.LoadResource_4DBE00("TEARS.BAN", ResourceManager::Resource_Animation, AEResourceID::kTorturedMudTearsResID, loadMode);
            gMap.LoadResource_4DBE00("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
        }
        else
        {
            ae_new<TorturedMudokon>(static_cast<Path_TorturedMudokon*>(pTlv), tlvInfo.all);
        }
    }
}

void Factory_TrainDoor_4D6E90(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource_4DBE00("TRAINDOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTrainDoorResID_2013, loadMode);
    }
    else
    {
        ae_new<TrainDoor>(static_cast<Path_TrainDoor*>(pTlv), tlvInfo.all);
    }
}

const PathFunctionTable kObjectFactory = {
    {Factory_ContinuePoint_4D6970,
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
     Factory_ZzzSpawner_4DA500,
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
     Factory_Drill_4DABC0,
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
     Factory_BrewMachine_4DA2B0,
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
     nullptr, nullptr, nullptr, nullptr}};
