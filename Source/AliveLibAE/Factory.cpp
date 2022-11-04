#include "stdafx.h"
#include "Factory.hpp"
#include "Function.hpp"
#include "MainMenu.hpp"
#include "Map.hpp"
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
#include "../relive_lib/ShadowZone.hpp"
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
#include "Slurg.hpp"
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

static void Factory_MainMenuController(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (sMainMenuObjectCounter_BB4400 == 0)
    {
        if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
        {
            /*
            static CompileTimeResourceList<3> kResources({
                {ResourceManager::Resource_Animation, AEResourceID::kHighliteResID},
                {ResourceManager::Resource_Animation, AEResourceID::kOptionFlareResID},
                {ResourceManager::Resource_Palt, AEResourceID::kHighlitePalResID},
            });

            //Map::LoadResourcesFromList("STARTANM.BND", kResources.AsList(), loadmode);
            //Map::LoadResource("ABESPEK2.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbespek2ResID, loadmode);
            */
        }
        else
        {
            relive_new MainMenuController(pTlv, tlvId);
        }
    }
}

static void Factory_Hoist(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    relive::Path_Hoist* pHoistTlv = static_cast<relive::Path_Hoist*>(pTlv);
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        /*
        //Map::LoadResource("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
        //Map::LoadResource("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadmode, 0);
        */
    }
    else if (pHoistTlv->mHoistType == relive::Path_Hoist::Type::eOffScreen)
    {
        // Its an off screen hoist so create the falling rocks effect
        relive_new HoistRocksEffect(pHoistTlv, tlvId);
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_Edge(relive::Path_TLV* /*pTlv*/, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode, 0);
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_Door(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("ABEDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbedoorResID, loadmode, false);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                //Map::LoadResource("SVZDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, false);
                break;

            case EReliveLevelIds::eFeeCoDepot:
            case EReliveLevelIds::eFeeCoDepot_Ender:
                //Map::LoadResource("FDDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, false);
                break;

            case EReliveLevelIds::eBarracks:
            case EReliveLevelIds::eBarracks_Ender:
                if (gMap.mOverlayId == 108)
                {
                    //Map::LoadResource("SHDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, false);
                }
                else
                {
                    //Map::LoadResource("BADOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, false);
                }
                break;

            case EReliveLevelIds::eBonewerkz:
            case EReliveLevelIds::eBonewerkz_Ender:
                //Map::LoadResource("BWDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, false);
                break;

            case EReliveLevelIds::eBrewery:
            case EReliveLevelIds::eBrewery_Ender:
                //Map::LoadResource("BRDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, false);
                break;

            default:
                //Map::LoadResource("DOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2p3dorResID, loadmode, false);
                break;
        }
    }
    else
    {
        relive_new Door(static_cast<relive::Path_Door*>(pTlv), tlvId);
    }
}

static void Factory_ShadowZone(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new ShadowZone(static_cast<relive::Path_ShadowZone*>(pTlv), tlvId);
    }
}


static void LoadLiftPointResources(const char_type* /*ropeBan*/, const char_type* /*liftBan*/, LoadMode /*loadMode*/)
{
    static CompileTimeResourceList<2> kResources({
        {ResourceManager::Resource_Animation, AEResourceID::kLiftWheelsResID},
        {ResourceManager::Resource_Animation, AEResourceID::kLiftResID},
    });

    //gMap.LoadResource(ropeBan, ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadMode);
    //gMap.LoadResourcesFromList(liftBan, kResources.AsList(), loadMode);
}

static void Factory_LiftPoint(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("ABELIFT.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, loadMode);
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
        relive::Path_LiftPoint* pLiftTlv = static_cast<relive::Path_LiftPoint*>(pTlv);
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
                if (pTlv->mTopLeftX <= xpos && xpos <= pTlv->mBottomRightX && pLiftPoint->field_278_lift_point_id == pLiftTlv->mLiftPointId && pLiftPoint->mCurrentLevel == gMap.mCurrentLevel && pLiftPoint->mCurrentPath == gMap.mCurrentPath)
                {
                    // Yes so just reset its data
                    Path::TLV_Reset(tlvId, -1, 0, 0);
                    return;
                }
            }
        }

        // TODO: Meaning of the data in mTlvSpecificMeaning for lift point
        if (pLiftTlv->mTlvSpecificMeaning & 2 || (pLiftTlv->mTlvSpecificMeaning == 0 && pLiftTlv->mIsStartPoint == relive::reliveChoice::eYes))
        {
            relive_new LiftPoint(pLiftTlv, tlvId);
            return;
        }
        else
        {
            // Find out where to create the lift point
            s16 pointNumber = 1;
            while (pointNumber < 8)
            {
                relive::Path_TLV* pTlvIter = sPathInfo->Get_First_TLV_For_Offsetted_Camera(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));

                while (pTlvIter)
                {
                    if (pTlvIter->mTlvType == ReliveTypes::eLiftPoint)
                    {
                        auto pLiftPointIter = static_cast<relive::Path_LiftPoint*>(pTlvIter);

                        const s32 tlvX = pTlv->mTopLeftX;
                        const s32 absX = pTlvIter->mTopLeftX - tlvX >= 0 ? pTlvIter->mTopLeftX - tlvX : tlvX - pTlvIter->mTopLeftX;

                        if (absX < 5 && pLiftPointIter->mLiftPointId == pLiftTlv->mLiftPointId && (pLiftPointIter->mTlvSpecificMeaning & 2 || pLiftPointIter->mTlvSpecificMeaning == 0) && pLiftPointIter->mIsStartPoint == relive::reliveChoice::eYes)
                        {
                            relive_new LiftPoint(pLiftPointIter, tlvId);
                            return;
                        }
                    }

                    pTlvIter = Path::Next_TLV(pTlvIter);
                }
                pointNumber++;
            }

            // Default to original
            relive_new LiftPoint(pLiftTlv, tlvId);
        }
    }
}

static void Factory_Well(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("ABEWELL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbewellResID, loadmode);
        //gMap.LoadResource("WELLLEAF.BAN", ResourceManager::Resource_Animation, AEResourceID::kWellLeafResID, loadmode);
    }
    else
    {
        relive::Path_WellBase* pWellTlv = static_cast<relive::Path_WellBase*>(pTlv);
        const FP xpos = FP_FromInteger(pWellTlv->mTopLeftX);
        const FP ypos = FP_FromInteger(pWellTlv->mTopLeftY + 5);
        relive_new Well(pWellTlv, xpos, ypos, tlvId);
    }
}

static void Factory_Dove(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pDoveTlv = static_cast<relive::Path_Dove*>(pTlv);

        const s16 width = pDoveTlv->Width();
        const s16 height = pDoveTlv->Height();

        for (s32 i = 0; i < pDoveTlv->mDoveCount; i++)
        {
            auto pDove = relive_new Dove(
                AnimId::Dove_Idle,
                tlvId,
                pDoveTlv->mScale != relive::reliveScale::eFull ? FP_FromDouble(0.5) : FP_FromInteger(1));

            s16 ypos = 0;
            if (pDoveTlv->mPixelPerfect == relive::reliveChoice::eYes)
            {
                pDove->mXPos = FP_FromInteger(pTlv->mTopLeftX);
                ypos = pTlv->mTopLeftY;
            }
            else
            {
                pDove->mXPos = FP_FromInteger(pTlv->mTopLeftX + width * Math_NextRandom() / 256);
                ypos = pTlv->mTopLeftY + height * Math_NextRandom() / 256;
            }
            pDove->mYPos = FP_FromInteger(ypos) + FP_FromInteger(10);
        }
    }
}

static void Factory_RockSack(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAberockResID}});
        //gMap.LoadResourcesFromList("RTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        relive_new RockSack(static_cast<relive::Path_RockSack*>(pTlv), tlvId);
    }
}

static void Factory_FallingItem(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
        {
            //gMap.LoadResource("FALLBONZ.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            //gMap.LoadResource("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadmode);
            //gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadmode);
            //gMap.LoadResource("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadmode);
            //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        }
        else
        {
            //gMap.LoadResource("FALLROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadmode);
            //gMap.LoadResource("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadmode);
            //gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadmode);
            //gMap.LoadResource("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadmode);
        }
    }
    else
    {
        relive_new FallingItem(static_cast<relive::Path_FallingItem*>(pTlv), tlvId);
    }
}

static void Factory_PullRingRope(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("ABEHOIST.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbehoistResID, loadmode);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                //gMap.LoadResource("NECROPE.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
                break;
            default:
                //gMap.LoadResource("ROPES.BAN", ResourceManager::Resource_Animation, AEResourceID::kRopesResID, loadmode);
                break;
        }
        //gMap.LoadResource("PULLRING.BAN", ResourceManager::Resource_Animation, AEResourceID::kPullringResID, loadmode);
    }
    else
    {
        relive_new PullRingRope(static_cast<relive::Path_PullRingRope*>(pTlv), tlvId);
    }
}

static void Factory_TimedMine(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    auto mine_tlv = static_cast<relive::Path_TimedMine*>(pTlv);

    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode, mine_tlv->mDisabledResources & 1);
        //Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode, mine_tlv->mDisabledResources & 2);

        static CompileTimeResourceList<2> sTimedMineResourceList_563368({
            {ResourceManager::Resource_Animation, AEResourceID::kBombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBombflshResID},
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            {ResourceManager::Resource_Animation, AEResourceID::kAbebombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID},
        });

        //Map::LoadResourcesFromList("BOMB.BND", sTimedMineResourceList_563368.AsList(), loadmode, 0);
        //Map::LoadResourcesFromList("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        relive_new TimedMine(mine_tlv, tlvId);
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

static void LoadWalkingSligResources(s16 /*disabledResources*/, LoadMode /*loadMode*/)
{
    //gMap.LoadResource("SLGLEVER.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgleverResID, loadMode, disabledResources & 1);
    //gMap.LoadResource("SLGLIFT.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgliftResID, loadMode, disabledResources & 2);
    //gMap.LoadResource("SLGSLEEP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgsleepResID, loadMode, disabledResources & 0x40);
    //gMap.LoadResource("SLGEDGE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgedgeResID, loadMode, disabledResources & 0x100);
    //gMap.LoadResource("SLGSMASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgsmashResID, loadMode, disabledResources & 0x200);
    //gMap.LoadResource("SLGBEAT.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgbeatResID, loadMode, disabledResources & 0x400);
    //gMap.LoadResource("SLGKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlgknfdResID, loadMode, disabledResources & 0x80);
    //gMap.LoadResourcesFromList("SLIGZ.BND", kResources_5632A0.AsList(), loadMode, disabledResources & 4);
    //gMap.LoadResourcesFromList("SLIG.BND", kResources_5632BC.AsList(), loadMode);
    //gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
    //gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
}

static void Factory_Slig(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->mData.mDisabledResourcesAE;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        relive_new Slig(pSligTlv, tlvId);
    }
}

static void LoadSlogResources(LoadMode /*loadMode*/)
{
    static CompileTimeResourceList<4> kResources(
        {{ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogrstnResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogattkResID},
         {ResourceManager::Resource_Animation, AEResourceID::kDogidleResID}});

    //gMap.LoadResourcesFromList("SLOG.BND", kResources.AsList(), loadMode);
    //gMap.LoadResource("DOGKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, loadMode);
}

static void Factory_Slog(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        relive_new Slog(static_cast<relive::Path_Slog*>(pTlv), tlvId);
    }
}

static void Factory_Lever(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("SWITCH1.BAN", ResourceManager::Resource_Animation, AEResourceID::kLeverResID, loadmode);
        //Map::LoadResource("ABEPULL.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbepullResID, loadmode);
    }
    else
    {
        relive_new Lever(static_cast<relive::Path_Lever*>(pTlv), tlvId);
    }
}

static void Factory_SecurityOrb(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    relive::Path_SecurityOrb* pSecurityOrbTlv = static_cast<relive::Path_SecurityOrb*>(pTlv);
    //const s16 disabledResources = pSecurityOrbTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("MAIMORB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMaimGameResID, loadMode);
        //gMap.LoadResource("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode); 
        //gMap.LoadResource("SPARKS.BAN", ResourceManager::Resource_Animation, AEResourceID::kSparksResID, loadMode);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, disabledResources & 1);
        //gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, disabledResources & 0x10);
        //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        relive_new SecurityOrb(pSecurityOrbTlv, tlvId);
    }
}

static void Factory_AbeStart(relive::Path_TLV* pTlv, Path*, const Guid& /*tlvId*/, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2 && !gAttract)
    {
        if (!gPauseMenu)
        {
            gPauseMenu = relive_new PauseMenu();
        }

        if (!sActiveHero)
        {
            sActiveHero = relive_new Abe();
            if (sActiveHero)
            {
                sActiveHero->mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
                sActiveHero->mYPos = FP_FromInteger(pTlv->mTopLeftY);
            }
        }
    }
}

static void Factory_Mine(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    auto mine_tlv = static_cast<relive::Path_Mine*>(pTlv);

    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode, mine_tlv->mDisabledResources & 1);
        //Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode, mine_tlv->mDisabledResources & 2);

        static CompileTimeResourceList<2> sMineResourceList_56337C({
            {ResourceManager::Resource_Animation, AEResourceID::kLandmineResID},
            {ResourceManager::Resource_Animation, AEResourceID::kMineflshResID},
        });

        static CompileTimeResourceList<3> sExplodeResourceList_56334C({
            {ResourceManager::Resource_Animation, AEResourceID::kAbebombResID},
            {ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID},
            {ResourceManager::Resource_Animation, AEResourceID::kBgexpldResID},
        });

        //Map::LoadResourcesFromList("MINE.BND", sMineResourceList_56337C.AsList(), loadmode, 0);
        //Map::LoadResourcesFromList("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadmode, 0);
    }
    else
    {
        relive_new Mine(mine_tlv, tlvId);
    }
}

static void Factory_UXB(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadMode)
{
    auto uxb_tlv = static_cast<relive::Path_UXB*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        /*
        //Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, uxb_tlv->mDisabledResources & 1);
        //Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, uxb_tlv->mDisabledResources & 2);

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

        //Map::LoadResourcesFromList("UXB.BND", sUXBResourceList_563390.AsList(), loadMode, 0);
        //Map::LoadResourcesFromList("EXPLODE.BND", sExplodeResourceList_56334C.AsList(), loadMode, 0);
        */
    }
    else
    {
        relive_new UXB(uxb_tlv, tlvId);
    }
}

static void Factory_Paramite(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
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
        //gMap.LoadResourcesFromList("PARAMITE.BND", kResources.AsList(), loadMode);
    }
    else
    {
         relive_new Paramite(static_cast<relive::Path_Paramite*>(pTlv), tlvId);
    }
}

static void Factory_MovieHandStone(relive::Path_TLV*, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, false);
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_BirdPortal(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<relive::Path_BirdPortal*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources_5634E8({
            {ResourceManager::Resource_Animation, AEResourceID::kPortalTerminatorResID},
            {ResourceManager::Resource_Animation, AEResourceID::kPortliteResID},
            {ResourceManager::Resource_Animation, AEResourceID::kPortlitResID},
        });

        //gMap.LoadResourcesFromList("PORTAL.BND", kResources_5634E8.AsList(), loadMode);
        if (pBirdPortalTlv->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
        {
            static CompileTimeResourceList<2> kResources_563504({
                {ResourceManager::Resource_Animation, AEResourceID::kAbemorphResID},
                {ResourceManager::Resource_Animation, AEResourceID::kShrmorphResID},
            });

            //gMap.LoadResourcesFromList("SHRYPORT.BND", kResources_563504.AsList(), loadMode);
            //gMap.LoadResource("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode);
        }
    }
    else if (SwitchStates_Get(pBirdPortalTlv->mCreatePortalSwitchId))
    {
        relive_new BirdPortal(pBirdPortalTlv, tlvId);
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_TrapDoor(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                //gMap.LoadResource("VLTSTRAP.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
                break;

            default:
                //gMap.LoadResource("TRAPDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, loadmode);
                break;
        }
    }
    else
    {
        relive_new TrapDoor(static_cast<relive::Path_TrapDoor*>(pTlv), tlvId);
    }
}

static relive::Path_TLV* FindMatchingSligTLV(relive::Path_TLV* pTlvIter, relive::Path_SligBound* pTlv)
{
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType == ReliveTypes::eSlig && pTlv->mSligBoundId == static_cast<relive::Path_Slig*>(pTlvIter)->mData.mSligBoundId && !pTlvIter->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed))
        {
            return pTlvIter;
        }
        pTlvIter = Path::Next_TLV(pTlvIter);
    }
    return nullptr;
}

static void Factory_SligBoundLeft(relive::Path_TLV* pTlv, Path*, const Guid& /*tlvId*/, LoadMode loadMode)
{
    auto pBound = static_cast<relive::Path_SligBound*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(pBound->mDisabledResourcesAE, loadMode);
    }
    else
    {
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            relive::Path_TLV* pTlvIter = sPathInfo->Get_First_TLV_For_Offsetted_Camera(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->mTlvFlags.Set(relive::TlvFlags::eBit1_Created);
                pTlvIter->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);

                relive_new Slig(static_cast<relive::Path_Slig*>(pTlvIter), pTlvIter->mId); // id of the slig to spawn at the bound, not the bound itself

                return;
            }
        }
    }
}

static void Factory_FootSwitch(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                //Map::LoadResource("VLTTRIGR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
                break;
            case EReliveLevelIds::eBonewerkz:
                //Map::LoadResource("BWTRIGGR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
                break;
            default:
                //Map::LoadResource("TRIGGER.BAN", ResourceManager::Resource_Animation, AEResourceID::kTriggerResID, loadmode);
                break;
        }
    }
    else
    {
        relive_new FootSwitch(static_cast<relive::Path_FootSwitch*>(pTlv), tlvId);
    }
}

static void Factory_ChantSuppressor_WithArms(relive::Path_TLV* pTlv, Path* pPath, const Guid& tlvId, LoadMode loadMode)
{
    // TODO: Most of the code for the chant suppressor with arms is still in the game
    // check if it can be re-added at some point.
    Factory_SecurityOrb(pTlv, pPath, tlvId, loadMode);
}

static void Factory_MotionDetector(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new MotionDetector(static_cast<relive::Path_MotionDetector*>(pTlv), tlvId, nullptr);
    }
}

static void Factory_SligSpawner(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->mData.mDisabledResourcesAE;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
    }
    else
    {
        relive_new SligSpawner(pSligTlv, tlvId);
    }
}

static void Factory_ElectricWall(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
    }
    else
    {
        relive_new ElectricWall(static_cast<relive::Path_ElectricWall*>(pTlv), tlvId);
    }
}

static void Factory_LiftMover(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new LiftMover(static_cast<relive::Path_LiftMover*>(pTlv), tlvId);
    }
}

static void Factory_MeatSack(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kMeatResID}});
        //gMap.LoadResourcesFromList("MTHROW.BND", kResources.AsList(), loadMode, 0);
    }
    else
    {
        relive_new MeatSack(static_cast<relive::Path_MeatSack*>(pTlv), tlvId);
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

static void Factory_Scrab(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //Map::LoadResourcesFromList("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        relive_new Scrab(static_cast<relive::Path_Scrab*>(pTlv), tlvId, relive::Path_ScrabSpawner::SpawnDirection::eNone);
    }
}

static void Factory_SligBoundRight(relive::Path_TLV* pTlv, Path* pPath, const Guid& tlvId, LoadMode loadMode)
{
    Factory_SligBoundLeft(pTlv, pPath, tlvId, loadMode);
}

static void Factory_InvisibleSwitch(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new InvisibleSwitch(static_cast<relive::Path_InvisibleSwitch*>(pTlv), tlvId);
    }
}

static void Factory_Mudokon(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    auto pMudTlv = static_cast<relive::Path_Mudokon*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbebsic1ResID, loadMode);
        //gMap.LoadResource("ABEKNFD.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknfdResID, loadMode);
        //gMap.LoadResource("ABEKNBK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeknbkResID, loadMode);
        //gMap.LoadResource("ABEEDGE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeedgeResID, loadMode);
        //gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
        //gMap.LoadResource("MUDIDLE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudidleResID, loadMode);

        static CompileTimeResourceList<5> kPalResources(
            {{ResourceManager::Resource_Palt, AEResourceID::kMudangryResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudsadResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudwiredResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudblindResID},
             {ResourceManager::Resource_Palt, AEResourceID::kMudsickResID}});

        //gMap.LoadResourcesFromList("MUDPAL.BND", kPalResources.AsList(), loadMode, 0);
        if (pMudTlv->mJob == relive::Path_Mudokon::MudJobs::eChisle)
        {
            //gMap.LoadResource("MUDCHSL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudchslResID, loadMode);
        }
        else
        {
            //gMap.LoadResource("MUDSCRUB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMudscrubResID, loadMode);
        }

        static CompileTimeResourceList<2> kResources(
            {
                {ResourceManager::Resource_Animation, AEResourceID::kMudoduckResID},
                {ResourceManager::Resource_Animation, AEResourceID::kMudbtlnkResID} // TODO: Another block of 3 after this exists that is never used ??
            });

        //gMap.LoadResourcesFromList("MUDWORK.BND", kResources.AsList(), loadMode);
    }
    else
    {
        relive_new Mudokon(pMudTlv, tlvId);
    }
}

static void Factory_DoorFlame(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("FIRE.BAN", ResourceManager::Resource_Animation, AEResourceID::kHubFireResID, loadMode);
        //gMap.LoadResource("GLOW1.BAN", ResourceManager::Resource_Animation, AEResourceID::kGlowResID, loadMode);
    }
    else
    {
        relive_new DoorFlame(static_cast<relive::Path_DoorFlame*>(pTlv), tlvId);
    }
}

static void Factory_MovingBomb(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<relive::Path_MovingBomb*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("MOVEBOMB.BAN", ResourceManager::Resource_Animation, AEResourceID::kMovebombResID, loadMode, 0);
        //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, pMovingBombTlv->mDisabledResources & 1);
    }
    else
    {
        relive_new MovingBomb(pMovingBombTlv, tlvId);
    }
}

static void Factory_TimerTrigger(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::ConstructObject_0)
    {
        relive_new TimerTrigger(static_cast<relive::Path_TimerTrigger*>(pTlv), tlvId);
    }
}

static void Factory_SecurityDoor(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("SECDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kR1sdosResID_6027, loadMode);
    }
    else
    {
        relive_new SecurityDoor(static_cast<relive::Path_SecurityDoor*>(pTlv), tlvId);
    }
}

static void Factory_BoomMachine(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    auto pTlvBooMachine = static_cast<relive::Path_BoomMachine*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
             {ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
             {ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID}});

        //gMap.LoadResourcesFromList("GTHROW.BND", kResources.AsList(), loadMode, 0);
        //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        //gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode, pTlvBooMachine->mDisabledResources & 2);
    }
    else
    {
        relive_new BoomMachine(pTlvBooMachine, tlvId);
    }
}

static void Factory_BackgroundAnimation(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new BackgroundAnimation(static_cast<relive::Path_BackgroundAnimation*>(pTlv), tlvId);
    }
}

static void Factory_LCDScreen(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, 2, loadmode); // TODO: Add to resource ID enum
        return;
    }

    relive_new LCDScreen(static_cast<relive::Path_LCDScreen*>(pTlv), tlvId);
}

static void Factory_HandStone(relive::Path_TLV*, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode, false);
        //Map::LoadResource("SPOTLITE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSpotliteResID, loadmode, false);
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_CreditsController(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2 && !gCreditsControllerExists)
    {
        relive_new CreditsController(pTlv, tlvId);
    }
}

static void Factory_LCDStatusBoard(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
        return;
    }
    
    relive_new LCDStatusBoard(static_cast<relive::Path_LCDStatusBoard*>(pTlv), tlvId);
}

static void Factory_WheelSyncer(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new WheelSyncer(static_cast<relive::Path_WheelSyncer*>(pTlv), tlvId);
    }
}

static void Factory_MusicTrigger(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new MusicTrigger(static_cast<relive::Path_MusicTrigger*>(pTlv), tlvId);
    }
}

static void Factory_SlogSpawner(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadSlogResources(loadMode);
    }
    else
    {
        relive_new SlogSpawner(static_cast<relive::Path_SlogSpawner*>(pTlv), tlvId);
    }
}

static void Factory_GasCountdown(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
        //gMap.LoadResource("ABEGAS.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbegasResID, loadMode);
    }
    else
    {
        relive_new GasCountDown(static_cast<relive::Path_GasCountDown*>(pTlv), tlvId);
    }
}

static void Factory_GasEmitter(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new GasEmitter(static_cast<relive::Path_GasEmitter*>(pTlv), tlvId);
    }
}

static void Factory_ZzzSpawner(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ZzzSpawner(static_cast<relive::Path_ZzzSpawner*>(pTlv), tlvId);
    }
}

static void Factory_Glukkon(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
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
                //gMap.LoadResourcesFromList("ASLIK.BND", kAslik_563548.AsList(), loadMode);
                break;

            case EReliveLevelIds::eBarracks:
            case EReliveLevelIds::eBarracks_Ender:
                //gMap.LoadResourcesFromList("DRIPIK.BND", KDripik_56355C.AsList(), loadMode);
                break;

            case EReliveLevelIds::eBonewerkz:
            case EReliveLevelIds::eBonewerkz_Ender:
                //gMap.LoadResourcesFromList("PHLEG.BND", kPhleg_563570.AsList(), loadMode);
                break;

            default:
                //gMap.LoadResourcesFromList("GLUKKON.BND", kGlukkon_563534.AsList(), loadMode);
                break;
        }
    }
    else
    {
        relive_new Glukkon(static_cast<relive::Path_Glukkon*>(pTlv), tlvId);
    }
}

static void Factory_Water(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("WDROP.BAN", ResourceManager::Resource_Animation, AEResourceID::kWaterDropResID, loadmode);
        //Map::LoadResource("SPLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplashResID, loadmode);
    }
    else
    {
        relive_new Water(static_cast<relive::Path_Water*>(pTlv), tlvId);
    }
}

static void Factory_WorkWheel(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("ABEWORK.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, loadmode, false);
        //Map::LoadResource("WORKWHEL.BAN", ResourceManager::Resource_Animation, AEResourceID::kWorkwhelResID, loadmode, false);
    }
    else
    {
        relive_new WorkWheel(static_cast<relive::Path_WorkWheel*>(pTlv), tlvId);
    }
}

static void Create_Gas_4DA8B0(Layer layer, s32 neverUsed, relive::Path_TLV* pTlv, const Guid& tlvId)
{
    if (!gGasInstanceCount_5BC214)
    {
        relive_new LaughingGas(layer, neverUsed, static_cast<relive::Path_LaughingGas*>(pTlv), tlvId);
    }
}

static void Factory_LaughingGas(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        Create_Gas_4DA8B0(Layer::eLayer_Above_FG1_39, 2, pTlv, tlvId);
    }
}

static CompileTimeResourceList<1> kResources_5632E0({{ResourceManager::Resource_Animation, AEResourceID::kFlySligResID}});

static void LoadFlyingSligResources(LoadMode /*loadMode*/)
{
    //gMap.LoadResourcesFromList("FLYSLIG.BND", kResources_5632E0.AsList(), loadMode, 0);

    //gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
    //gMap.LoadResource("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
    //gMap.LoadResource("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
    //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
    //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
    //gMap.LoadResource("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
    //gMap.LoadResource("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
    //gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
}


static void Factory_FlyingSlig(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources(loadMode);
    }
    else
    {
        relive_new FlyingSlig(static_cast<relive::Path_FlyingSlig*>(pTlv), tlvId);
    }
}

static void Factory_Fleech(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("FLEECH.BAN", ResourceManager::Resource_Animation, AEResourceID::kFleechResID, loadMode);
        //gMap.LoadResource("FLEEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kFleeBlowResID_580, loadMode);
    }
    else if (
        gMap.mCurrentLevel != EReliveLevelIds::eMudancheeVault_Ender || gMap.mCurrentPath != 9 || gMap.mCurrentCamera != 4)
    {
        relive_new Fleech(static_cast<relive::Path_Fleech*>(pTlv), tlvId);
    }
}

static void Factory_Slurg(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("SLURG.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, loadMode);
    }
    else
    {
        relive_new Slurg(static_cast<relive::Path_Slurg*>(pTlv), tlvId);
    }
}

static void Factory_SlamDoor(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
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
                //Map::LoadResource("SLAMVLTS.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlamResID, loadmode);
                break;
            default:
                //Map::LoadResource("SLAM.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlamResID, loadmode);
                break;
        }
    }
    else
    {
        relive_new SlamDoor(static_cast<relive::Path_SlamDoor*>(pTlv), tlvId);
    }
}

static void Factory_LevelLoader(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new LevelLoader(static_cast<relive::Path_LevelLoader*>(pTlv), tlvId);
    }
}

static void Factory_DemoSpawnPoint(relive::Path_TLV*, Path*, const Guid& /*tlvId*/, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        if (gAttract)
        {
            if (!sDemoObj_dword_5D1E20)
            {
                relive_new DemoPlayback();
            }
        }
    }
}

static void Factory_Teleporter(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new Teleporter(static_cast<relive::Path_Teleporter*>(pTlv), tlvId);
    }
}

static void Factory_SlurgSpawner(relive::Path_TLV* /*pTlv*/, Path*, const Guid& /*tlvId*/, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("SLURG.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, loadMode);
    }
    else
    {
       /*
       // TODO/HACK disabled because it crashes the game when a quick load is done
       // after you save when a slurg is spawned as it has no TLV.
       // Its only used to spawn 1 slurg in 1 screen of the game. Fix in abi_break
       // branch by converting to a normal slurg.
        relive_new SlurgSpawner(static_cast<relive::Path_SlurgSpawner*>(pTlv), tlvId);
        */
    }
}

static void Factory_Drill(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("DRILL.BAN", ResourceManager::Resource_Animation, AEResourceID::kDrillResID, loadmode);
        //Map::LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
        //Map::LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadmode);
    }
    else
    {
        relive_new Drill(static_cast<relive::Path_Drill*>(pTlv), tlvId);
    }
}

static void Factory_ColourfulMeter(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
    }
    else
    {
        relive_new ColourfulMeter(static_cast<relive::Path_ColourfulMeter*>(pTlv), tlvId);
    }
}

static void Factory_FlyingSligSpawner(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // TODO: Resource Ids
        static CompileTimeResourceList<1> kResources({{ResourceManager::Resource_Animation, AEResourceID::kFlySligResID}});

        //gMap.LoadResourcesFromList("FLYSLIG.BND", kResources.AsList(), loadMode);
        //gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
        //gMap.LoadResource("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
        //gMap.LoadResource("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
        //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        //gMap.LoadResource("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
        //gMap.LoadResource("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
        //gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
    }
    else
    {
        relive_new FlyingSligSpawner(static_cast<relive::Path_FlyingSligSpawner*>(pTlv), tlvId);
    }
}

static void Factory_MineCar(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("BAYROLL.BAN", ResourceManager::Resource_Animation, AEResourceID::kBayrollResID_6013, loadMode, 0);
        //gMap.LoadResource("ABECAR.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId, loadMode, 0);
        //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        relive_new MineCar(static_cast<relive::Path_MineCar*>(pTlv), tlvId, 0, 0, 0);
    }
}

static void Factory_BoneBag(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("BONEBAG.BAN", ResourceManager::Resource_Animation, AEResourceID::kBoneBagResID_590, loadMode);

        static CompileTimeResourceList<3> kResources({{ResourceManager::Resource_Animation, AEResourceID::kAbepickResID},
                                                      {ResourceManager::Resource_Animation, AEResourceID::kAbethrowResID},
                                                      {ResourceManager::Resource_Animation, AEResourceID::kBoneResID}});

        //gMap.LoadResourcesFromList("BTHROW.BND", kResources.AsList(), loadMode);
    }
    else
    {
        relive_new BoneBag(static_cast<relive::Path_BoneBag*>(pTlv), tlvId);
    }
}

static void Factory_ExplosionSet(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
        {
            //gMap.LoadResource("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadMode);
            //gMap.LoadResource("FALLBONZ.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadMode);
        }
        else
        {
            //gMap.LoadResource("DRPROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kHoistRocks, loadMode);
            //gMap.LoadResource("FALLROCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kF2rockResID, loadMode);
        }
        //gMap.LoadResource("STICK.BAN", ResourceManager::Resource_Animation, AEResourceID::kStickGibResID, loadMode);
        //gMap.LoadResource("SHADOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kObjectShadowResID, loadMode);
        //gMap.LoadResource("DEBRIS00.BAN", ResourceManager::Resource_Animation, AEResourceID::kDebrisID00ResID, loadMode);
        //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode);
    }
    else
    {
        if (!gExplosionSet)
        {
            // pExplosionSet_5BBF68 is assigned in the ctor (OWI programming)
            relive_new ExplosionSet();
        }
        gExplosionSet->Init(static_cast<relive::Path_ExplosionSet*>(pTlv));
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_MultiSwitchController(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode != LoadMode::LoadResourceFromList_1 && loadmode != LoadMode::LoadResource_2)
    {
        relive_new MultiSwitchController(static_cast<relive::Path_MultiSwitchController*>(pTlv), tlvId);
    }
}

static void Factory_StatusLight(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("STATUSLT.BAN", ResourceManager::Resource_Animation, AEResourceID::kStatusLightResID, loadmode);
    }
    else
    {
        relive_new StatusLight(static_cast<relive::Path_StatusLight*>(pTlv), tlvId);
    }
}

static void Factory_SlapLock(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //Map::LoadResource("GHOSTTRP.BAN", ResourceManager::ResourceType::Resource_Animation, AEResourceID::kGhostTrpResID_1053, loadMode, 0);
    }
    else
    {
        relive_new SlapLock(static_cast<relive::Path_SlapLock*>(pTlv), tlvId);
    }
}

static void Factory_ParamiteWebLine(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("WEB.BAN", ResourceManager::Resource_Animation, AEResourceID::kWebResID, loadMode);
    }
    else
    {
        relive_new ParamiteWebLine(static_cast<relive::Path_ParamiteWebLine*>(pTlv), tlvId);
    }
}

static void Factory_Alarm(relive::Path_TLV* pTlv, Path* /*pPath*/, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::ConstructObject_0)
    {
        relive_new Alarm(static_cast<relive::Path_Alarm*>(pTlv), tlvId);
    }
}

static void Factory_BrewMachine(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadmode)
{
    if (loadmode == LoadMode::LoadResourceFromList_1 || loadmode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("ABESTONE.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbestoneResID, loadmode);
        //gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
        //gMap.LoadResource("BREWBTN.BAN", ResourceManager::Resource_Animation, AEResourceID::kBrewButtonResID_6016, loadmode);
        //gMap.LoadResource("EVILFART.BAN", ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID, loadmode);
        //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadmode);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadmode);
    }
    else
    {
        relive_new BrewMachine(static_cast<relive::Path_BrewMachine*>(pTlv), tlvId);
    }
}

static void Factory_ScrabSpawner(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //Map::LoadResourcesFromList("SCRAB.BND", kScrabResources.AsList(), loadMode);
    }
    else
    {
        relive_new ScrabSpawner(static_cast<relive::Path_ScrabSpawner*>(pTlv), tlvId);
    }
}

static void Factory_CrawlingSlig(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<1> kResource(
            {{ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligResID_449}});

        //gMap.LoadResourcesFromList("CRAWLSLG.BND", kResource.AsList(), loadMode);
        //gMap.LoadResource("SLGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, loadMode);
        //gMap.LoadResource("GRENADE.BAN", ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, loadMode);
        //gMap.LoadResource("SMEXP.BAN", ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, loadMode);
        //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode);
        //gMap.LoadResource("BIGFLASH.BAN", ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, loadMode);
        //gMap.LoadResource("VAPOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kVaporResID, loadMode);
        //gMap.LoadResource("DOGBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, loadMode);
    }
    else
    {
        relive_new CrawlingSlig(static_cast<relive::Path_CrawlingSlig*>(pTlv), tlvId);
    }
}

static void Factory_SligGetPants(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    const auto disabledResources = pSligTlv->mData.mDisabledResourcesAE;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources(disabledResources, loadMode);
        //gMap.LoadResource("LOCKER.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligLockerResID_448, loadMode);
    }
    else
    {
        relive_new SligGetPantsAndWings(pTlv, tlvId);
    }
}

static void Factory_SligGetWings(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources(loadMode);
        //gMap.LoadResource("LOCKER.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligLockerResID_448, loadMode, 0);
    }
    else
    {
        relive_new SligGetPantsAndWings(pTlv, tlvId);
    }
}

static void Factory_Greeter(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("MFLARE.BAN", ResourceManager::Resource_Animation, AEResourceID::kMflareResID, loadMode, 0);
        //gMap.LoadResource("MOTION.BAN", ResourceManager::Resource_Animation, AEResourceID::kMotionResID, loadMode, 0);
        //gMap.LoadResource("GREETER.BAN", ResourceManager::Resource_Animation, AEResourceID::kGreeterResID, loadMode, 0);
        //gMap.LoadResource("SPLINE.BAN", ResourceManager::Resource_Animation, AEResourceID::kSplineResID, loadMode, 0);
        //gMap.LoadResource("METAL.BAN", ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, loadMode, 0);
        //gMap.LoadResource("EXPLO2.BAN", ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, loadMode, 0);
        //gMap.LoadResource("ABEBLOW.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, loadMode, 0);
    }
    else
    {
        relive_new Greeter(static_cast<relive::Path_Greeter*>(pTlv), tlvId);
    }
}

static void Factory_CrawlingSligButton(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("CSLGBUTN.BAN", ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligButtonResID_1057, loadMode);
    }
    else
    {
        relive_new CrawlingSligButton(static_cast<relive::Path_CrawlingSligButton*>(pTlv), tlvId);
    }
}

static void Factory_GlukkonSwitch(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("SECDOOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kR1sdosResID_6027, loadMode);
    }
    else
    {
        relive_new GlukkonSwitch(static_cast<relive::Path_GlukkonSwitch*>(pTlv), tlvId);
    }
}

static void Factory_DoorBlocker(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("DOORLOCK.BAN", ResourceManager::Resource_Animation, AEResourceID::kDoorLockResID_374, loadMode);
    }
    else
    {
        relive_new DoorBlocker(static_cast<relive::Path_DoorBlocker*>(pTlv), tlvId);
    }
}

static void Factory_TorturedMudokon(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (!pTlv->mTlvSpecificMeaning)
    {
        if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
        {
            //gMap.LoadResource("MUDTORT.BAN", ResourceManager::Resource_Animation, AEResourceID::kTorturedMud, loadMode);
            //gMap.LoadResource("TEARS.BAN", ResourceManager::Resource_Animation, AEResourceID::kTorturedMudTearsResID, loadMode);
            //gMap.LoadResource("ELECWALL.BAN", ResourceManager::Resource_Animation, AEResourceID::kElecwallResID, loadMode);
        }
        else
        {
            relive_new TorturedMudokon(static_cast<relive::Path_TorturedMudokon*>(pTlv), tlvId);
        }
    }
}

static void Factory_TrainDoor(relive::Path_TLV* pTlv, Path*, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("TRAINDOR.BAN", ResourceManager::Resource_Animation, AEResourceID::kTrainDoorResID_2013, loadMode);
    }
    else
    {
        relive_new TrainDoor(static_cast<relive::Path_TrainDoor*>(pTlv), tlvId);
    }
}

void ConstructTLVObject(relive::Path_TLV* pTlv, Path* pPath, const Guid& tlvId, LoadMode loadMode)
{
    switch (pTlv->mTlvType)
    {
        case ReliveTypes::eBackgroundAnimation:
            Factory_BackgroundAnimation(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eLiftMover:
            Factory_LiftMover(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eTimedMine:
            Factory_TimedMine(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eGasCountDown:
            Factory_GasCountdown(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eDoor:
            Factory_Door(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eElectricWall:
            Factory_ElectricWall(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSecurityClaw:
            Factory_ChantSuppressor_WithArms(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eBoomMachine:
            Factory_BoomMachine(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMudokon:
            Factory_Mudokon(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eLiftPoint:
            Factory_LiftPoint(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSecurityOrb:
            Factory_SecurityOrb(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMeatSack:
            Factory_MeatSack(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMine:
            Factory_Mine(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMotionDetector:
            Factory_MotionDetector(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eParamite:
            Factory_Paramite(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eBirdPortal:
            Factory_BirdPortal(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::ePullRingRope:
            Factory_PullRingRope(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eRockSack:
            Factory_RockSack(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eScrab:
            Factory_Scrab(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSlig:
            Factory_Slig(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSlog:
            Factory_Slog(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSligSpawner:
            Factory_SligSpawner(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eLever:
            Factory_Lever(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eTrapDoor:
            Factory_TrapDoor(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eUXB:
            Factory_UXB(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMusicTrigger:
            Factory_MusicTrigger(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eWellLocal:
        case ReliveTypes::eWellExpress:
            Factory_Well(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eZzzSpawner:
            Factory_ZzzSpawner(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eHoist:
            Factory_Hoist(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eEdge:
            Factory_Edge(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eShadowZone:
            Factory_ShadowZone(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eDove:
            Factory_Dove(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eFallingItem:
            Factory_FallingItem(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eAbeStart:
            Factory_AbeStart(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eFootSwitch:
            Factory_FootSwitch(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSligBoundRight:
            Factory_SligBoundRight(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSligBoundLeft:
            Factory_SligBoundLeft(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eGasEmitter:
            Factory_GasEmitter(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSlogSpawner:
            Factory_SlogSpawner(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eLCDStatusBoard:
            Factory_LCDStatusBoard(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eCreditsController:
            Factory_CreditsController(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eHandStone:
            Factory_HandStone(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSecurityDoor:
            Factory_SecurityDoor(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMainMenuController:
            Factory_MainMenuController(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMovingBomb:
            Factory_MovingBomb(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eDoorFlame:
            Factory_DoorFlame(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eInvisibleSwitch:
            Factory_InvisibleSwitch(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eTimerTrigger:
            Factory_TimerTrigger(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eLCDScreen:
            Factory_LCDScreen(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMovieHandStone:
            Factory_MovieHandStone(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eWheelSyncer:
            Factory_WheelSyncer(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eGlukkon:
            Factory_Glukkon(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eWater:
            Factory_Water(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eWorkWheel:
            Factory_WorkWheel(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eLaughingGas:
            Factory_LaughingGas(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eFlyingSlig:
            Factory_FlyingSlig(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eFleech:
            Factory_Fleech(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSlurg:
            Factory_Slurg(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSlamDoor:
            Factory_SlamDoor(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eLevelLoader:
            Factory_LevelLoader(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eDemoSpawnPoint:
            Factory_DemoSpawnPoint(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eTeleporter:
            Factory_Teleporter(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSlurgSpawner:
            Factory_SlurgSpawner(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eDrill:
            Factory_Drill(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eColourfulMeter:
            Factory_ColourfulMeter(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eFlyingSligSpawner:
            Factory_FlyingSligSpawner(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMineCar:
            Factory_MineCar(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eBoneBag:
            Factory_BoneBag(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eExplosionSet:
            Factory_ExplosionSet(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eMultiSwitchController:
            Factory_MultiSwitchController(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eStatusLight:
            Factory_StatusLight(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSlapLock:
            Factory_SlapLock(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eParamiteWebLine:
            Factory_ParamiteWebLine(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eAlarm:
            Factory_Alarm(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eBrewMachine:
            Factory_BrewMachine(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eScrabSpawner:
            Factory_ScrabSpawner(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eCrawlingSlig:
            Factory_CrawlingSlig(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSligGetPants:
            Factory_SligGetPants(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eSligGetWings:
            Factory_SligGetWings(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eGreeter:
            Factory_Greeter(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eCrawlingSligButton:
            Factory_CrawlingSligButton(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eGlukkonSwitch:
            Factory_GlukkonSwitch(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eDoorBlocker:
            Factory_DoorBlocker(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eTorturedMud:
            Factory_TorturedMudokon(pTlv, pPath, tlvId, loadMode);
            break;
        case ReliveTypes::eTrainDoor:
            Factory_TrainDoor(pTlv, pPath, tlvId, loadMode);
            break;
    }
}
