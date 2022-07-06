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

void Factory_MainMenuController(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
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

            Map::LoadResourcesFromList("STARTANM.BND", kResources.AsList(), loadmode);
            Map::LoadResource("ABESPEK2.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbespek2ResID, loadmode);
        }
        else
        {
            relive_new MainMenuController(pTlv, tlvOffsetLevelIdPathId);
        }
    }
}

void Factory_ContinuePoint(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_PathTransition(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Hoist(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    Path_Hoist* pHoistTlv = static_cast<Path_Hoist*>(pTlv);
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
        Map::LoadResource("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadmode, 0);
    }
    else if (pHoistTlv->field_10_type == Path_Hoist::Type::eOffScreen)
    {
        // Its an off screen hoist so create the falling rocks effect
        relive_new HoistRocksEffect(pHoistTlv, tlvOffsetLevelIdPathId.all);
    }
    else
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_Edge(Path_TLV* /*pTlv*/, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
    }
    else
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_DeathDrop(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Door(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABEDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbedoorResID, loadmode, FALSE);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                Map::LoadResource("SVZDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
                break;

            case EReliveLevelIds::eFeeCoDepot:
            case EReliveLevelIds::eFeeCoDepot_Ender:
                Map::LoadResource("FDDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
                break;

            case EReliveLevelIds::eBarracks:
            case EReliveLevelIds::eBarracks_Ender:
                if (gMap.mOverlayId == 108)
                {
                    Map::LoadResource("SHDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
                }
                else
                {
                    Map::LoadResource("BADOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
                }
                break;

            case EReliveLevelIds::eBonewerkz:
            case EReliveLevelIds::eBonewerkz_Ender:
                Map::LoadResource("BWDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
                break;

            case EReliveLevelIds::eBrewery:
            case EReliveLevelIds::eBrewery_Ender:
                Map::LoadResource("BRDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
                break;

            default:
                Map::LoadResource("DOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, FALSE);
                break;
        }
    }
    else
    {
        relive_new Door(static_cast<Path_Door*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Shadow(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new ShadowZone(static_cast<Path_ShadowZone*>(pTlv), pPath, tlvInfo.all);
    }
}


static void LoadLiftPointResources(const char_type* ropeBan, const char_type* liftBan, LoadMode loadMode)
{
    static CompileTimeResourceList<2> kResources({
        {ResourceManager::Resource_Animation, AEResourceID::kLiftWheelsResID},
        {ResourceManager::Resource_Animation, AEResourceID::kLiftResID},
    });

    gMap.LoadResource(ropeBan, ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadMode);
    gMap.LoadResourcesFromList(liftBan, kResources.AsList(), loadMode);
}

void Factory_LiftPoint(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("ABELIFT.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, loadMode);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
                LoadLiftPointResources("NECROPE.BAN", "NELIFT.BND", loadMode);
                break;

            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudomoVault_Ender:
                LoadLiftPointResources("NECROPE.BAN", "PVLIFT.BND", loadMode);
                break;

            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
                LoadLiftPointResources("NECROPE.BAN", "SVLIFT.BND", loadMode);
                break;

            case EReliveLevelIds::eFeeCoDepot:
            case EReliveLevelIds::eFeeCoDepot_Ender:
                LoadLiftPointResources("ROPES.BAN", "FDLIFT.BND", loadMode);
                break;

            case EReliveLevelIds::eBarracks:
            case EReliveLevelIds::eBarracks_Ender:
                LoadLiftPointResources("ROPES.BAN", "BALIFT.BND", loadMode);
                break;

            case EReliveLevelIds::eBonewerkz:
            case EReliveLevelIds::eBonewerkz_Ender:
                LoadLiftPointResources("ROPES.BAN", "BWLIFT.BND", loadMode);
                break;

            case EReliveLevelIds::eBrewery:
            case EReliveLevelIds::eBrewery_Ender:
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

            if (!(pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead)) && pObj->Type() == ReliveTypes::eLiftPoint)
            {
                // Is there already an existing LiftPoint object for this TLV?
                LiftPoint* pLiftPoint = static_cast<LiftPoint*>(pObj);
                const s16 xpos = FP_GetExponent(pLiftPoint->mXPos);
                if (pTlv->mTopLeft.x <= xpos && xpos <= pTlv->mBottomRight.x && pLiftPoint->field_278_lift_point_id == pLiftTlv->field_10_lift_point_id && pLiftPoint->mCurrentLevel == gMap.mCurrentLevel && pLiftPoint->mCurrentPath == gMap.mCurrentPath)
                {
                    // Yes so just reset its data
                    Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                    return;
                }
            }
        }

        // TODO: Meaning of the data in field_1_unknown for lift point
        if (pLiftTlv->mTlvState & 2 || (pLiftTlv->mTlvState == 0 && pLiftTlv->field_12_bStart_point == Choice_short::eYes_1))
        {
            relive_new LiftPoint(pLiftTlv, tlvOffsetLevelIdPathId.all);
            return;
        }
        else
        {
            // Find out where to create the lift point
            s16 pointNumber = 1;
            while (pointNumber < 8)
            {
                Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));

                while (pTlvIter)
                {
                    if (pTlvIter->mTlvType32 == TlvTypes::LiftPoint_7)
                    {
                        auto pLiftPointIter = static_cast<Path_LiftPoint*>(pTlvIter);

                        const s32 tlvX = pTlv->mTopLeft.x;
                        const s32 absX = pTlvIter->mTopLeft.x - tlvX >= 0 ? pTlvIter->mTopLeft.x - tlvX : tlvX - pTlvIter->mTopLeft.x;

                        if (absX < 5 && pLiftPointIter->field_10_lift_point_id == pLiftTlv->field_10_lift_point_id && (pLiftPointIter->mTlvState & 2 || pLiftPointIter->mTlvState == 0) && pLiftPointIter->field_12_bStart_point == Choice_short::eYes_1)
                        {
                            relive_new LiftPoint(pLiftPointIter, tlvOffsetLevelIdPathId.all);
                            return;
                        }
                    }

                    pTlvIter = Path::Next_TLV(pTlvIter);
                }
                pointNumber++;
            }

            // Default to original
            relive_new LiftPoint(pLiftTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

void Factory_ExpressWell(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("ABEWELL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbewellResID, loadmode);
        gMap.LoadResource("WELLLEAF.BAN", ResourceManager::Resource_Animation, AEResourceID::kWellLeafResID, loadmode);
    }
    else
    {
        Path_WellBase* pWellTlv = static_cast<Path_WellBase*>(pTlv);
        const FP xpos = FP_FromInteger(pWellTlv->mTopLeft.x);
        const FP ypos = FP_FromInteger(pWellTlv->mTopLeft.y + 5);
        relive_new Well(pWellTlv, xpos, ypos, tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Dove(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pDoveTlv = static_cast<Path_Dove*>(pTlv);

        const s16 width = pDoveTlv->mBottomRight.x - pDoveTlv->mTopLeft.x;
        const s16 height = pDoveTlv->mBottomRight.y - pDoveTlv->mTopLeft.y;

        for (s32 i = 0; i < pDoveTlv->mDoveCount; i++)
        {
            auto pDove = relive_new Dove(
                AnimId::Dove_Idle,
                tlvInfo.all,
                pDoveTlv->mScale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1));

            s16 ypos = 0;
            if (pDoveTlv->mPixelPerfect == Choice_short::eYes_1)
            {
                pDove->mXPos = FP_FromInteger(pTlv->mTopLeft.x);
                ypos = pTlv->mTopLeft.y;
            }
            else
            {
                pDove->mXPos = FP_FromInteger(pTlv->mTopLeft.x + width * Math_NextRandom() / 256);
                ypos = pTlv->mTopLeft.y + height * Math_NextRandom() / 256;
            }
            pDove->mYPos = FP_FromInteger(ypos) + FP_FromInteger(10);
        }
    }
}

void Factory_RockSack(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAberockResID}});
        gMap.LoadResourcesFromList("RTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        relive_new RockSack(static_cast<Path_RockSack*>(pTlv), tlvInfo.all);
    }
}

void Factory_FallingItem(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
        {
            gMap.LoadResource("FALLBONZ.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            gMap.LoadResource("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadmode);
            gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadmode);
            gMap.LoadResource("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadmode);
            gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        }
        else
        {
            gMap.LoadResource("FALLROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            gMap.LoadResource("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadmode);
            gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadmode);
            gMap.LoadResource("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadmode);
        }
    }
    else
    {
        relive_new FallingItem(static_cast<Path_FallingItem*>(pTlv), tlvInfo.all);
    }
}

void Factory_PullRingRope(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                gMap.LoadResource("NECROPE.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
                break;
            default:
                gMap.LoadResource("ROPES.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
                break;
        }
        gMap.LoadResource("PULLRING.BAN", ResourceManager::Resource_Animation, AEResourceID::kPullringResID, loadmode);
    }
    else
    {
        relive_new PullRingRope(static_cast<Path_PullRingRope*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_TimedMine(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    auto mine_tlv = static_cast<Path_TimedMine*>(pTlv);

    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode, mine_tlv->mDisabledResources & 1);
        Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode, mine_tlv->mDisabledResources & 2);

        static CompileTimeResourceList<2> sTimedMineResourceList_563368({
            {ResourceManager::Resource_Animation, AEResourceID::kBombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBombflshResID},
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            {ResourceManager::Resource_Animation, AEResourceID::kAbebombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID},
        });

        Map::LoadResourcesFromList("BOMB.BND", sTimedMineResourceList_563368.AsList(), loadmode, 0);
        Map::LoadResourcesFromList("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        relive_new TimedMine(mine_tlv, tlvOffsetLevelIdPathId);
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
    gMap.LoadResource("SLGLEVER.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, loadMode, disabledResources & 1);
    gMap.LoadResource("SLGLIFT.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, loadMode, disabledResources & 2);
    gMap.LoadResource("SLGSLEEP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, loadMode, disabledResources & 0x40);
    gMap.LoadResource("SLGEDGE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, loadMode, disabledResources & 0x100);
    gMap.LoadResource("SLGSMASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, loadMode, disabledResources & 0x200);
    gMap.LoadResource("SLGBEAT.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, loadMode, disabledResources & 0x400);
    gMap.LoadResource("SLGKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, loadMode, disabledResources & 0x80);
    gMap.LoadResourcesFromList("SLIGZ.BND", kResources_5632A0.AsList(), loadMode, disabledResources & 4);
    gMap.LoadResourcesFromList("SLIG.BND", kResources_5632BC.AsList(), loadMode);
    gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
    gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
}

void Factory_Slig(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        relive_new Slig(pSligTlv, tlvInfo.all);
    }
}

static void LoadSlogResources(LoadMode loadMode)
{
    static CompileTimeResourceList<4> kResources(
        {{ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogrstnResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogattkResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogidleResID}});

    gMap.LoadResourcesFromList("SLOG.BND", kResources.AsList(), loadMode);
    gMap.LoadResource("DOGKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, loadMode);
}

void Factory_Slog(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        relive_new Slog(static_cast<Path_Slog*>(pTlv), tlvInfo.all);
    }
}

void Factory_Switch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("SWITCH1.BAN", ResourceManager::Resource_Animation, AEResourceID::kLeverResID, loadmode);
        Map::LoadResource("ABEPULL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, loadmode);
    }
    else
    {
        relive_new Lever(static_cast<Path_Lever*>(pTlv), tlvInfo.all);
    }
}

void Factory_Null_4D69E0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_SecurityOrb(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    Path_SecurityOrb* pSecurityOrbTlv = static_cast<Path_SecurityOrb*>(pTlv);
    const s16 disabledResources = pSecurityOrbTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("MAIMORB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMaimGameResID, loadMode);
        gMap.LoadResource("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode); 
        gMap.LoadResource("SPARKS.BAN", ResourceManager::Resource_Animation, AEResourceID::kSparksResID, loadMode);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, disabledResources & 1);
        gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, disabledResources & 0x10);
        gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        relive_new SecurityOrb(pSecurityOrbTlv, tlvInfo.all);
    }
}

void Factory_Null_4D6A00(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Pulley(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    // Nothing to do.
}

void Factory_AbeStart(Path_TLV* pTlv, Path*, TlvItemInfoUnion, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2 && !gAttract_5C1BA0)
    {
        if (!pPauseMenu_5C9300)
        {
            pPauseMenu_5C9300 = relive_new PauseMenu();
        }

        if (!sActiveHero)
        {
            const AnimRecord& rec = AnimRec(AnimId::Mudokon_Walk);
            sActiveHero = relive_new Abe(rec.mFrameTableOffset, 85, 57, 55);
            if (sActiveHero)
            {
                sActiveHero->mXPos = FP_FromInteger(pTlv->mTopLeft.x + 12);
                sActiveHero->mYPos = FP_FromInteger(pTlv->mTopLeft.y);
            }
        }
    }
}

void Factory_WellExpress(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    Factory_ExpressWell(pTlv, pPath, tlvOffsetLevelIdPathId, loadmode);
}

void Factory_Mine(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    auto mine_tlv = static_cast<Path_Mine*>(pTlv);

    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode, mine_tlv->field_16_disabled_resources & 1);
        Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode, mine_tlv->field_16_disabled_resources & 2);

        static CompileTimeResourceList<2> sMineResourceList_56337C({
            {ResourceManager::Resource_Animation, AEResourceID::kLandmineResID},
            {ResourceManager::Resource_Animation, AEResourceID::kMineflshResID},
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            {ResourceManager::Resource_Animation, AEResourceID::kAbebombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID},
        });

        Map::LoadResourcesFromList("MINE.BND", sMineResourceList_56337C.AsList(), loadmode, 0);
        Map::LoadResourcesFromList("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        relive_new Mine(mine_tlv, tlvOffsetLevelIdPathId);
    }
}

void Factory_UXB(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto uxb_tlv = static_cast<Path_UXB*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, uxb_tlv->field_18_disabled_resources & 1);
        Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, uxb_tlv->field_18_disabled_resources & 2);

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

        Map::LoadResourcesFromList("UXB.BND", sUXBResourceList_563390.AsList(), loadMode, 0);
        Map::LoadResourcesFromList("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadMode, 0);
    }
    else
    {
        relive_new UXB(uxb_tlv, tlvOffsetLevelIdPathId);
    }
}

void Factory_Paramite(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
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
        gMap.LoadResourcesFromList("PARAMITE.BND", kResources.AsList(), loadMode);
    }
    else
    {
         relive_new Paramite(static_cast<Path_Paramite*>(pTlv), tlvInfo.all);
    }
}

void Factory_MovieHandStone(Path_TLV*, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, FALSE);
    }
    else
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_BirdPortal(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<Path_BirdPortal*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources_5634E8({
            {ResourceManager::Resource_Animation, AEResourceID::kPortalTerminatorResID},
            {ResourceManager::Resource_Animation, AEResourceID::kPortliteResID},
            {ResourceManager::Resource_Animation, AEResourceID::kPortlitResID},
        });

        gMap.LoadResourcesFromList("PORTAL.BND", kResources_5634E8.AsList(), loadMode);
        if (pBirdPortalTlv->field_1C_portal_type == PortalType::eShrykull_2)
        {
            static CompileTimeResourceList<2> kResources_563504({
                {ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID},
                {ResourceManager::Resource_Animation, AEResourceID::kShrmorphResID},
            });

            gMap.LoadResourcesFromList("SHRYPORT.BND", kResources_563504.AsList(), loadMode);
            gMap.LoadResource("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode);
        }
    }
    else if (SwitchStates_Get(pBirdPortalTlv->field_20_create_portal_switch_id))
    {
        relive_new BirdPortal(pBirdPortalTlv, tlvInfo.all);
    }
    else
    {
        Path::TLV_Reset(tlvInfo.all, -1, 0, 0);
    }
}

void Factory_PortalExit(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_TrapDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                gMap.LoadResource("VLTSTRAP.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
                break;

            default:
                gMap.LoadResource("TRAPDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
                break;
        }
    }
    else
    {
        relive_new TrapDoor(static_cast<Path_TrapDoor*>(pTlv), tlvInfo.all);
    }
}

void Factory_RollingRock(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

static Path_TLV* FindMatchingSligTLV(Path_TLV* pTlvIter, Path_SligBound* pTlv)
{
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType32 == TlvTypes::Slig_15 && pTlv->field_10_slig_bound_id == static_cast<Path_Slig*>(pTlvIter)->field_38_slig_bound_id && !pTlvIter->mTlvFlags.Get(TlvFlags::eBit2_Destroyed))
        {
            return pTlvIter;
        }
        pTlvIter = Path::Next_TLV(pTlvIter);
    }
    return nullptr;
}

void Factory_SligBoundLeft(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pBound = static_cast<Path_SligBound*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(pBound->field_12_disabled_resources, loadMode);
    }
    else
    {
        pBound->mTlvFlags.Clear(TlvFlags::eBit1_Created);
        pBound->mTlvFlags.Clear(TlvFlags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->mTlvFlags.Set(TlvFlags::eBit1_Created);
                pTlvIter->mTlvFlags.Set(TlvFlags::eBit2_Destroyed);

                tlvInfo.parts.tlvOffset += static_cast<u16>(
                    reinterpret_cast<const u8*>(pTlvIter) - reinterpret_cast<const u8*>(pBound));
                relive_new Slig(static_cast<Path_Slig*>(pTlvIter), tlvInfo.all);

                return;
            }
        }
    }
}

void Factory_InvisibleZone(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_FootSwitch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                Map::LoadResource("VLTTRIGR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
                break;
            case EReliveLevelIds::eBonewerkz:
                Map::LoadResource("BWTRIGGR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
                break;
            default:
                Map::LoadResource("TRIGGER.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
                break;
        }
    }
    else
    {
        relive_new FootSwitch(static_cast<Path_FootSwitch*>(pTlv), tlvInfo.all);
    }
}

void Factory_ChantSuppressor_WithArms(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    // TODO: Most of the code for the chant suppressor with arms is still in the game
    // check if it can be re-added at some point.
    Factory_SecurityOrb(pTlv, pPath, tlvInfo, loadMode);
}

void Factory_MotionDetector(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new MotionDetector(static_cast<Path_MotionDetector*>(pTlv), tlvInfo.all, nullptr);
    }
}

void Factory_SligSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        relive_new SligSpawner(pSligTlv, tlvInfo.all);
    }
}

void Factory_ElectricWall(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
    }
    else
    {
        relive_new ElectricWall(static_cast<Path_ElectricWall*>(pTlv), tlvInfo.all);
    }
}

void Factory_LiftMover(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new LiftMover(static_cast<Path_LiftMover*>(pTlv), tlvInfo.all);
    }
}

void Factory_MeatSack(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kMeatResID}});
        gMap.LoadResourcesFromList("MTHROW.BND", kResources.AsList(), loadMode, 0);
    }
    else
    {
        relive_new MeatSack(static_cast<Path_MeatSack*>(pTlv), tlvInfo.all);
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

void Factory_Scrab(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResourcesFromList("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        relive_new Scrab(static_cast<Path_Scrab*>(pTlv), tlvInfo.all, ScrabSpawnDirection::eNone_0);
    }
}

void Factory_Null_4DA130(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_ScrabLeftBound(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_ScrabRightBound(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_SligBoundRight(Path_TLV* pTlv, Path* pPath, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    Factory_SligBoundLeft(pTlv, pPath, tlvInfo, loadMode);
}

void Factory_SligPersist(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_EnemyStopper(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_InvisibleSwitch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new InvisibleSwitch(static_cast<Path_InvisibleSwitch*>(pTlv), tlvInfo.all);
    }
}

void Factory_Mudokon(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pMudTlv = static_cast<Path_Mudokon*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, loadMode);
        gMap.LoadResource("ABEKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, loadMode);
        gMap.LoadResource("ABEKNBK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, loadMode);
        gMap.LoadResource("ABEEDGE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, loadMode);
        gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
        gMap.LoadResource("MUDIDLE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, loadMode);

        static CompileTimeResourceList<5> kPalResources(
            {{ResourceManager::Resource_Palt, AEResourceID::kMudangryResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudsadResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudwiredResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudblindResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudsickResID}});

        gMap.LoadResourcesFromList("MUDPAL.BND", kPalResources.AsList(), loadMode, 0);
        if (pMudTlv->field_12_state == Mud_State::eChisle_0)
        {
            gMap.LoadResource("MUDCHSL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, loadMode);
        }
        else
        {
            gMap.LoadResource("MUDSCRUB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, loadMode);
        }

        static CompileTimeResourceList<2> kResources(
            {
                {ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID},
                {ResourceManager::Resource_Animation, AEResourceID::kMudbtlnkResID} // TODO: Another block of 3 after this exists that is never used ??
            });

        gMap.LoadResourcesFromList("MUDWORK.BND", kResources.AsList(), loadMode);
    }
    else
    {
        relive_new Mudokon(pMudTlv, tlvInfo.all);
    }
}

void Factory_ZSligCover(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_DoorFlame(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("FIRE.BAN", ResourceManager::Resource_Animation, AEResourceID::kHubFireResID, loadMode);
        gMap.LoadResource("GLOW1.BAN", ResourceManager::Resource_Animation, AEResourceID::kGlowResID, loadMode);
    }
    else
    {
        relive_new DoorFlame(static_cast<Path_DoorFlame*>(pTlv), tlvInfo.all);
    }
}

void Factory_MovingBomb(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<Path_MovingBomb*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("MOVEBOMB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMovebombResID, loadMode, 0);
        gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, pMovingBombTlv->field_1A_disabled_resources & 1);
    }
    else
    {
        relive_new MovingBomb(pMovingBombTlv, tlvInfo.all);
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

void Factory_TimerTrigger(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::ConstructObject_0)
    {
        relive_new TimerTrigger(static_cast<Path_TimerTrigger*>(pTlv), tlvInfo.all);
    }
}

void Factory_SecurityDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("SECDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kR1sdosResID_6027, loadMode);
    }
    else
    {
        relive_new SecurityDoor(static_cast<Path_SecurityDoor*>(pTlv), tlvInfo.all);
    }
}

void Factory_BoomMachine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pTlvBooMachine = static_cast<Path_BoomMachine*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID}});

        gMap.LoadResourcesFromList("GTHROW.BND", kResources.AsList(), loadMode, 0);
        gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, pTlvBooMachine->field_14_disabled_resources & 2);
    }
    else
    {
        relive_new BoomMachine(pTlvBooMachine, tlvInfo.all);
    }
}

void Factory_BackgroundAnimation(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new BackgroundAnimation(static_cast<Path_BackgroundAnimation*>(pTlv), tlvOffsetLevelIdPathId);
    }
}

void Factory_LCDScreen(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, 2, loadmode); // TODO: Add to resource ID enum
        return;
    }

    relive_new LCDScreen(static_cast<Path_LCDScreen*>(pTlv), tlvOffsetLevelIdPathId);
}

void Factory_HandStone(Path_TLV*, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, FALSE);
        Map::LoadResource("SPOTLITE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, loadmode, FALSE);
    }
    else
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

void Factory_CreditsController(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2 && !sDoesCreditsControllerExist_5C1B90)
    {
        relive_new CreditsController(pTlv, tlvInfo.all);
    }
}

void Factory_Null_4D6910(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_LCDStatusBoard(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
        return;
    }
    
    relive_new LCDStatusBoard(static_cast<Path_LCDStatusBoard*>(pTlv), tlvOffsetLevelIdPathId);
}

void Factory_WheelSyncer(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new WheelSyncer(static_cast<Path_WheelSyncer*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_MusicTrigger(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new MusicTrigger(static_cast<Path_MusicTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Light(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_SlogSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        relive_new SlogSpawner(static_cast<Path_SlogSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_GasCountdown(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
        gMap.LoadResource("ABEGAS.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbegasResID, loadMode);
    }
    else
    {
        relive_new GasCountDown(static_cast<Path_GasCountDown*>(pTlv), tlvInfo.all);
    }
}

void Factory_4D6C50(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_GasEmitter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new GasEmitter(static_cast<Path_GasEmitter*>(pTlv), tlvInfo.all);
    }
}

void Factory_ZzzSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ZzzSpawner(static_cast<Path_ZzzSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_Glukkon(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
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
            case EReliveLevelIds::eFeeCoDepot:
            case EReliveLevelIds::eFeeCoDepot_Ender:
                gMap.LoadResourcesFromList("ASLIK.BND", kAslik_563548.AsList(), loadMode);
                break;

            case EReliveLevelIds::eBarracks:
            case EReliveLevelIds::eBarracks_Ender:
                gMap.LoadResourcesFromList("DRIPIK.BND", KDripik_56355C.AsList(), loadMode);
                break;

            case EReliveLevelIds::eBonewerkz:
            case EReliveLevelIds::eBonewerkz_Ender:
                gMap.LoadResourcesFromList("PHLEG.BND", kPhleg_563570.AsList(), loadMode);
                break;

            default:
                gMap.LoadResourcesFromList("GLUKKON.BND", kGlukkon_563534.AsList(), loadMode);
                break;
        }
    }
    else
    {
        relive_new Glukkon(static_cast<Path_Glukkon*>(pTlv), tlvInfo.all);
    }
}

void Factory_KillUnsavedMudokons(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_SoftLanding(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}
void Factory_Null_4D6B00(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_Water(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("WDROP.BAN", ResourceManager::Resource_Animation, AEResourceID::kWaterDropResID, loadmode);
        Map::LoadResource("SPLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplashResID, loadmode);
    }
    else
    {
        relive_new Water(static_cast<Path_Water*>(pTlv), tlvInfo.all);
    }
}

void Factory_Null_4DA7C0(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Factory_WorkWheel(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("ABEWORK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, loadmode, FALSE);
        Map::LoadResource("WORKWHEL.BAN", ResourceManager::Resource_Animation, AEResourceID::kWorkwhelResID, loadmode, FALSE);
    }
    else
    {
        relive_new WorkWheel(static_cast<Path_WorkWheel*>(pTlv), tlvInfo.all);
    }
}

void Factory_Null_4DA850(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode)
{
    
}

void Create_Gas_4DA8B0(Layer layer, s32 neverUsed, Path_TLV* pTlv, s32 tlvInfo)
{
    if (!gGasInstanceCount_5BC214)
    {
        relive_new LaughingGas(layer, neverUsed, static_cast<Path_LaughingGas*>(pTlv), tlvInfo);
    }
}

void Factory_LaughingGas(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        Create_Gas_4DA8B0(Layer::eLayer_Above_FG1_39, 2, pTlv, tlvInfo.all);
    }
}

static CompileTimeResourceList<1> kResources_5632E0({{ResourceManager::Resource_Animation, AEResourceID::kFlySligResID}});

static void LoadFlyingSligResources(LoadMode loadMode)
{
    gMap.LoadResourcesFromList("FLYSLIG.BND", kResources_5632E0.AsList(), loadMode, 0);

    gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
    gMap.LoadResource("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
    gMap.LoadResource("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
    gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
    gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
    gMap.LoadResource("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
    gMap.LoadResource("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
    gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
}


void Factory_FlyingSlig(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources(loadMode);
    }
    else
    {
        relive_new FlyingSlig(static_cast<Path_FlyingSlig*>(pTlv), tlvInfo.all);
    }
}

void Factory_Fleech(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("FLEECH.BAN", ResourceManager::Resource_Animation, AEResourceID::kFleechResID, loadMode);
        gMap.LoadResource("FLEEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kFleeBlowResID_580, loadMode);
    }
    else if (
        gMap.mCurrentLevel != EReliveLevelIds::eMudancheeVault_Ender || gMap.mCurrentPath != 9 || gMap.mCurrentCamera != 4)
    {
        relive_new Fleech(static_cast<Path_Fleech*>(pTlv), tlvInfo.all);
    }
}

void Factory_Slurgs(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("SLURG.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, loadMode);
    }
    else
    {
        relive_new Slurg(static_cast<Path_Slurg*>(pTlv), tlvInfo.all);
    }
}

void Factory_SlamDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                Map::LoadResource("SLAMVLTS.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlamResID, loadmode);
                break;
            default:
                Map::LoadResource("SLAM.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlamResID, loadmode);
                break;
        }
    }
    else
    {
        relive_new SlamDoor(static_cast<Path_SlamDoor*>(pTlv), tlvInfo);
    }
}

void Factory_LevelLoader(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new LevelLoader(static_cast<Path_LevelLoader*>(pTlv), tlvInfo.all);
    }
}

void Factory_DemoSpawnPoint(Path_TLV*, Path*, TlvItemInfoUnion, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        if (gAttract_5C1BA0)
        {
            if (!sDemoObj_dword_5D1E20)
            {
                relive_new DemoPlayback();
            }
        }
    }
}

void Factory_Teleporter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new Teleporter(static_cast<Path_Teleporter*>(pTlv), tlvInfo.all);
    }
}

void Factory_SlurgSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("SLURG.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, loadMode);
    }
    else
    {
        relive_new SlurgSpawner(static_cast<Path_SlurgSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_Drill(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("DRILL.BAN", ResourceManager::Resource_Animation, AEResourceID::kDrillResID, loadmode);
        Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
        Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode);
    }
    else
    {
        relive_new Drill(static_cast<Path_Drill*>(pTlv), tlvInfo.all);
    }
}

void Factory_ColorfulMeter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
    }
    else
    {
        relive_new ColourfulMeter(static_cast<Path_ColourfulMeter*>(pTlv), tlvInfo.all);
    }
}

void Factory_FlyingSligSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // TODO: Resource Ids
        static CompileTimeResourceList<1> kResources({{ResourceManager::Resource_Animation, AEResourceID::kFlySligResID}});

        gMap.LoadResourcesFromList("FLYSLIG.BND", kResources.AsList(), loadMode);
        gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
        gMap.LoadResource("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
        gMap.LoadResource("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
        gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        gMap.LoadResource("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
        gMap.LoadResource("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
        gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
    }
    else
    {
        relive_new FlyingSligSpawner(static_cast<Path_FlyingSligSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_MineCar(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("BAYROLL.BAN", ResourceManager::Resource_Animation, AEResourceID::kBayrollResID_6013, loadMode, 0);
        gMap.LoadResource("ABECAR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId, loadMode, 0);
        gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        relive_new MineCar(static_cast<Path_MineCar*>(pTlv), tlvInfo.all, 0, 0, 0);
    }
}

void Factory_BoneBag(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("BONEBAG.BAN", ResourceManager::Resource_Animation, AEResourceID::kBoneBagResID_590, loadMode);

        static CompileTimeResourceList<3> kResources({{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
                                                      {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
                                                      {ResourceManager::Resource_Animation, AEResourceID::kBoneResID}});

        gMap.LoadResourcesFromList("BTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        relive_new BoneBag(static_cast<Path_BoneBag*>(pTlv), tlvInfo.all);
    }
}

void Factory_ExplosionSet(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
        {
            gMap.LoadResource("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadMode);
            gMap.LoadResource("FALLBONZ.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadMode);
        }
        else
        {
            gMap.LoadResource("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadMode);
            gMap.LoadResource("FALLROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadMode);
        }
        gMap.LoadResource("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadMode);
        gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
        gMap.LoadResource("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadMode);
        gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode);
    }
    else
    {
        if (!pExplosionSet_5BBF68)
        {
            // pExplosionSet_5BBF68 is assigned in the ctor (OWI programming)
            relive_new ExplosionSet();
        }
        pExplosionSet_5BBF68->Init(static_cast<Path_ExplosionSet*>(pTlv));
        Path::TLV_Reset(tlvInfo.all, -1, 0, 0);
    }
}

void Factory_MultiSwitchController(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new MultiSwitchController(static_cast<Path_MultiSwitchController*>(pTlv), tlvInfo.all);
    }
}

void Factory_StatusLight(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("STATUSLT.BAN", ResourceManager::Resource_Animation, AEResourceID::kStatusLightResID, loadmode);
    }
    else
    {
        relive_new StatusLight(static_cast<Path_StatusLight*>(pTlv), tlvInfo.all);
    }
}

void Factory_SlapLock(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResource("GHOSTTRP.BAN", ResourceManager::ResourceType::Resource_Animation, AEResourceID::kGhostTrpResID_1053, loadMode, 0);
    }
    else
    {
        relive_new SlapLock(static_cast<Path_SlapLock*>(pTlv), tlvInfo.all);
    }
}

void Factory_ParamiteWebLine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("WEB.BAN", ResourceManager::Resource_Animation, AEResourceID::kWebResID, loadMode);
    }
    else
    {
        relive_new ParamiteWebLine(static_cast<Path_ParamiteWebLine*>(pTlv), tlvInfo.all);
    }
}

void Factory_Alarm(Path_TLV* pTlv, Path* /*pPath*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadmode)
{
    if (loadmode == LoadMode::ConstructObject_0)
    {
        relive_new Alarm(static_cast<Path_Alarm*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_BrewMachine(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode);
        gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
        gMap.LoadResource("BREWBTN.BAN", ResourceManager::Resource_Animation, AEResourceID::kBrewButtonResID_6016, loadmode);
        gMap.LoadResource("EVILFART.BAN", ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, loadmode);
        gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
    }
    else
    {
        relive_new BrewMachine(static_cast<Path_BrewMachine*>(pTlv), tlvInfo.all);
    }
}

void Factory_ScrabSpawner(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        Map::LoadResourcesFromList("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        relive_new ScrabSpawner(static_cast<Path_ScrabSpawner*>(pTlv), tlvInfo.all);
    }
}

void Factory_CrawlingSlig(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<1> kResource(
            {{ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligResID_449}});

        gMap.LoadResourcesFromList("CRAWLSLG.BND", kResource.AsList(), loadMode);
        gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
        gMap.LoadResource("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
        gMap.LoadResource("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
        gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        gMap.LoadResource("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
        gMap.LoadResource("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
        gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
    }
    else
    {
        relive_new CrawlingSlig(static_cast<Path_CrawlingSlig*>(pTlv), tlvInfo.all);
    }
}

void Factory_SligGetPants(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->field_48_disable_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
        gMap.LoadResource("LOCKER.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligLockerResID_448, loadMode);
    }
    else
    {
        relive_new SligGetPantsAndWings(pTlv, tlvInfo.all);
    }
}

void Factory_SligGetWings(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources(loadMode);
        gMap.LoadResource("LOCKER.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligLockerResID_448, loadMode, 0);
    }
    else
    {
        relive_new SligGetPantsAndWings(pTlv, tlvInfo.all);
    }
}

void Factory_Greeter(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("MFLARE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMflareResID, loadMode, 0);
        gMap.LoadResource("MOTION.BAN", ResourceManager::Resource_Animation, AEResourceID::kMotionResID, loadMode, 0);
        gMap.LoadResource("GREETER.BAN", ResourceManager::Resource_Animation, AEResourceID::kGreeterResID, loadMode, 0);
        gMap.LoadResource("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode, 0);
        gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        relive_new Greeter(static_cast<Path_Greeter*>(pTlv), tlvInfo.all);
    }
}

void Factory_CrawlingSligButton(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("CSLGBUTN.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligButtonResID_1057, loadMode);
    }
    else
    {
        relive_new CrawlingSligButton(static_cast<Path_CrawlingSligButton*>(pTlv), tlvInfo.all);
    }
}

void Factory_GlukkonSwitch(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("SECDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kR1sdosResID_6027, loadMode);
    }
    else
    {
        relive_new GlukkonSwitch(static_cast<Path_GlukkonSwitch*>(pTlv), tlvInfo.all);
    }
}

void Factory_DoorBlocker(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("DOORLOCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kDoorLockResID_374, loadMode);
    }
    else
    {
        relive_new DoorBlocker(static_cast<Path_DoorBlocker*>(pTlv), tlvInfo.all);
    }
}

void Factory_TorturedMudokon(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (!pTlv->mTlvState)
    {
        if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
        {
            gMap.LoadResource("MUDTORT.BAN", ResourceManager::Resource_Animation, AEResourceID::kTorturedMud, loadMode);
            gMap.LoadResource("TEARS.BAN", ResourceManager::Resource_Animation, AEResourceID::kTorturedMudTearsResID, loadMode);
            gMap.LoadResource("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
        }
        else
        {
            relive_new TorturedMudokon(static_cast<Path_TorturedMudokon*>(pTlv), tlvInfo.all);
        }
    }
}

void Factory_TrainDoor(Path_TLV* pTlv, Path*, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        gMap.LoadResource("TRAINDOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTrainDoorResID_2013, loadMode);
    }
    else
    {
        relive_new TrainDoor(static_cast<Path_TrainDoor*>(pTlv), tlvInfo.all);
    }
}

const PathFunctionTable kObjectFactory = {
    {Factory_ContinuePoint,
     Factory_PathTransition,
     Factory_Hoist,
     Factory_Edge,
     Factory_DeathDrop,
     Factory_Door,
     Factory_Shadow,
     Factory_LiftPoint,
     Factory_ExpressWell,
     Factory_Dove,
     Factory_RockSack,
     Factory_FallingItem,
     Factory_PullRingRope,
     Factory_BackgroundAnimation,
     Factory_TimedMine,
     Factory_Slig,
     Factory_Slog,
     Factory_Switch,
     Factory_Null_4D69E0,
     Factory_SecurityOrb,
     Factory_Null_4D6A00,
     Factory_Pulley,
     Factory_AbeStart,
     Factory_WellExpress,
     Factory_Mine,
     Factory_UXB,
     Factory_Paramite,
     Factory_MovieHandStone,
     Factory_BirdPortal,
     Factory_PortalExit,
     Factory_TrapDoor,
     Factory_RollingRock,
     Factory_SligBoundLeft,
     Factory_InvisibleZone,
     Factory_FootSwitch,
     Factory_ChantSuppressor_WithArms,
     Factory_MotionDetector,
     Factory_SligSpawner,
     Factory_ElectricWall,
     Factory_LiftMover,
     Factory_MeatSack,
     Factory_Scrab,
     Factory_Null_4DA130,
     Factory_ScrabLeftBound,
     Factory_ScrabRightBound,
     Factory_SligBoundRight,
     Factory_SligPersist,
     Factory_EnemyStopper,
     Factory_InvisibleSwitch,
     Factory_Mudokon,
     Factory_ZSligCover,
     Factory_DoorFlame,
     Factory_MovingBomb,
     Factory_Null_4D8A30,
     Factory_MainMenuController,
     Factory_4D7160, // switch state related ??
     Factory_Null_4D6AE0,
     Factory_TimerTrigger,
     Factory_SecurityDoor,
     Factory_BoomMachine,
     Factory_LCDScreen,
     Factory_HandStone,
     Factory_CreditsController,
     Factory_Null_4D6910,
     Factory_LCDStatusBoard,
     Factory_WheelSyncer,
     Factory_MusicTrigger,
     Factory_Light,
     Factory_SlogSpawner,
     Factory_GasCountdown,
     Factory_4D6C50, // Remove Shrykull ??
     Factory_GasEmitter,
     Factory_ZzzSpawner,
     Factory_Glukkon,
     Factory_KillUnsavedMudokons,
     Factory_SoftLanding,
     Factory_Null_4D6B00,
     Factory_Water,
     Factory_Null_4DA7C0,
     Factory_WorkWheel,
     Factory_Null_4DA850,
     Factory_LaughingGas,
     Factory_FlyingSlig,
     Factory_Fleech,
     Factory_Slurgs,
     Factory_SlamDoor,
     Factory_LevelLoader,
     Factory_DemoSpawnPoint,
     Factory_Teleporter,
     Factory_SlurgSpawner,
     Factory_Drill,
     Factory_ColorfulMeter,
     Factory_FlyingSligSpawner,
     Factory_MineCar,
     Factory_BoneBag,
     Factory_ExplosionSet,
     Factory_MultiSwitchController,
     Factory_StatusLight,
     Factory_SlapLock,
     Factory_ParamiteWebLine,
     Factory_Alarm,
     Factory_BrewMachine,
     Factory_ScrabSpawner,
     Factory_CrawlingSlig,
     Factory_SligGetPants,
     Factory_SligGetWings,
     Factory_Greeter,
     Factory_CrawlingSligButton,
     Factory_GlukkonSwitch,
     Factory_DoorBlocker,
     Factory_TorturedMudokon,
     Factory_TrainDoor,
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
