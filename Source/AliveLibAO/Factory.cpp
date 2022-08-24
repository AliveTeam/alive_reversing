#include "stdafx_ao.h"
#include "Factory.hpp"
#include "Function.hpp"
#include "GasCountDown.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "RollingBallStopper.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Elum.hpp"
#include "PauseMenu.hpp"
#include "ZBall.hpp"
#include "SligSpawner.hpp"
#include "Slig.hpp"
#include "Bat.hpp"
#include "BellHammer.hpp"
#include "LiftMover.hpp"
#include "BackgroundAnimation.hpp"
#include "DoorLight.hpp"
#include "LightEffect.hpp"
#include "TimedMine.hpp"
#include "ChimeLock.hpp"
#include "CreditsController.hpp"
#include "Meat.hpp"
#include "ElectricWall.hpp"
#include "Dove.hpp"
#include "Math.hpp"
#include "MusicTrigger.hpp"
#include "SecurityOrb.hpp"
#include "SecurityClaw.hpp"
#include "BeeSwarmHole.hpp"
#include "BeeNest.hpp"
#include "Honey.hpp"
#include "HoneySack.hpp"
#include "TimerTrigger.hpp"
#include "PullRingRope.hpp"
#include "InvisibleSwitch.hpp"
#include "FallingItem.hpp"
#include "FootSwitch.hpp"
#include "HoistRocksEffect.hpp"
#include "RollingBall.hpp"
#include "Lever.hpp"
#include "SecurityDoor.hpp"
#include "BackgroundGlukkon.hpp"
#include "Rock.hpp"
#include "Well.hpp"
#include "ZzzSpawner.hpp"
#include "BirdPortal.hpp"
#include "SlogSpawner.hpp"
#include "DoorFlame.hpp"
#include "BoomMachine.hpp"
#include "GasEmitter.hpp"
#include "MotionDetector.hpp"
#include "LCDStatusBoard.hpp"
#include "FlintLockFire.hpp"
#include "MeatSaw.hpp"
#include "TrapDoor.hpp"
#include "Abe.hpp"
#include "Mudokon.hpp"
#include "HintFly.hpp"
#include "Door.hpp"
#include "Mine.hpp"
#include "SlingMudokon.hpp"
#include "MainMenu.hpp"
#include "DDCheat.hpp"
#include "Slog.hpp"
#include "LCDScreen.hpp"
#include "UXB.hpp"
#include "Scrab.hpp"
#include "Paramite.hpp"
#include "MovingBomb.hpp"
#include "ShadowZone.hpp"
#include "OneShotSwitchIdSetter.hpp"
#include "SwitchStateBooleanLogic.hpp"
#include "LiftPoint.hpp"
#include "Sys_common.hpp"
#include "RockSack.hpp"

namespace AO {

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

void Factory_ContinuePoint_Null_4817D0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_PathTransition_Null_481720(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_ContinueZone_Null_481830(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Hoist_487230(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbehoistAOResID, loadMode);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::LoadResource_446C90("DRPSPRK.BAN", ResourceManager::Resource_Animation, AOResourceID::kHoistRocksAOResID, loadMode);
                break;

           case EReliveLevelIds::eForest:
           case EReliveLevelIds::eDesert:
                ResourceManager::LoadResource_446C90("ANEEDGE.BAN", ResourceManager::Resource_Animation, AOResourceID::kAneedgeAOResID, loadMode);
                ResourceManager::LoadResource_446C90("DRPROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kHoistRocksAOResID, loadMode);
                break;

            default:
                ResourceManager::LoadResource_446C90("DRPROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kHoistRocksAOResID, loadMode);
                break;
        }
    }
    else
    {
        auto pHoistTlv = static_cast<relive::Path_Hoist*>(pTlv);
        if (pHoistTlv->mHoistType == relive::Path_Hoist::Type::eOffScreen)
        {
            relive_new HoistRocksEffect(pHoistTlv, tlvOffsetLevelIdPathId.all);
            // OG issue, no reset on failure ??
        }
        else
        {
            Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


void Factory_Edge_481730(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbehoistAOResID, loadMode, 0);

        if (gMap.mCurrentLevel == EReliveLevelIds::eForest || gMap.mCurrentLevel == EReliveLevelIds::eDesert)
        {
            ResourceManager::LoadResource_446C90("ANEEDGE.BAN", ResourceManager::Resource_Animation, AOResourceID::kAneedgeAOResID, loadMode, 0);
        }
    }
    else
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


void Factory_DeathDrop_Null_4817B0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Door_481C80(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kF2p3dorAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kHubdoorAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kRockdoorAOResID}};
        ResourceManager::LoadResource_446C90("ABEDOOR.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbedoorAOResID, loadMode);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::LoadResourcesFromList_446E80("RDOOR.BND", kResources.AsList(), loadMode, 0);
                break;

            case EReliveLevelIds::eLines:
                ResourceManager::LoadResourcesFromList_446E80("LDOOR.BND", kResources.AsList(), loadMode, 0);
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManager::LoadResourcesFromList_446E80("DDOOR.BND", kResources.AsList(), loadMode, 0);
                break;

            default:
                ResourceManager::LoadResourcesFromList_446E80("FDOOR.BND", kResources.AsList(), loadMode, 0);
                break;
        }
    }
    else
    {
        auto kResources = {
            AOResourceID::kAbedoorAOResID,
            AOResourceID::kF2p3dorAOResID,
            AOResourceID::kHubdoorAOResID,
            AOResourceID::kRockdoorAOResID};
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new Door(static_cast<relive::Path_Door*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_ShadowZone_482080(relive::Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ShadowZone(static_cast<relive::Path_ShadowZone*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
    }
}



void Factory_LiftPoint_4820F0(relive::Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    static CompileTimeResourceList<3> kResources = {
        {ResourceManager::Resource_Animation, AOResourceID::kAbeliftAOResID},
        {ResourceManager::Resource_Animation, AOResourceID::kLiftWheelsAOResID},
        {ResourceManager::Resource_Animation, AOResourceID::kLiftAOResID}};

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::LoadResource_446C90("R1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("RLIFT.BND", kResources.AsList(), loadMode, 0);
                break;

            case EReliveLevelIds::eLines:
                ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("LLIFT.BND", kResources.AsList(), loadMode, 0);
                break;

            case EReliveLevelIds::eDesert:
                ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("D1LIFT.BND", kResources.AsList(), loadMode, 0);
                break;

            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("D2LIFT.BND", kResources.AsList(), loadMode, 0);
                break;

            default:
                ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("FLIFT.BND", kResources.AsList(), loadMode, 0);
                break;
        }
    }
    else
    {
        for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(idx);
            if (!pObjIter)
            {
                break;
            }

            if (!pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && pObjIter->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
            {
                auto pLiftObj = static_cast<LiftPoint*>(pObjIter);

                const s16 xpos_i = FP_GetExponent(pLiftObj->mXPos);
                if (pTlv->mTopLeftX <= xpos_i
                    && xpos_i <= pTlv->mBottomRightX
                    && pLiftObj->mCurrentLevel == gMap.mCurrentLevel
                    && pLiftObj->mCurrentPath == gMap.mCurrentPath)
                {
                    Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                    return;
                }
            }
        }

        auto kResourcesToCheck = {
            AOResourceID::kLiftAOResID,
            AOResourceID::kAbeliftAOResID,
            AOResourceID::kRopesAOResID,
            AOResourceID::kRopesAOResID};
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        if (pTlv->mTlvSpecificMeaning & 2 || (pTlv->mTlvSpecificMeaning == 0 && static_cast<relive::Path_LiftPoint*>(pTlv)->mIsStartPoint == relive::reliveChoice::eYes))
        {
            relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
        }
        else
        {
            relive::Path_TLV* pTlvIter = nullptr;
            s16 pointNumber = 1;
            while (pointNumber < 8)
            {
                pTlvIter = gMap.Get_First_TLV_For_Offsetted_Camera(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));
                while (pTlvIter)
                {
                    if (pTlvIter->mTlvType == static_cast<s32>(TlvTypes::LiftPoint_8))
                    {
                        const auto tlv_x = pTlv->mTopLeftX;
                        const auto absX = pTlvIter->mTopLeftX - tlv_x >= 0 ? pTlvIter->mTopLeftX - tlv_x : tlv_x - pTlvIter->mTopLeftX;
                        if (absX < 5)
                        {
                            if (pTlvIter->mTlvSpecificMeaning & 2 || (pTlvIter->mTlvSpecificMeaning == 0 && static_cast<relive::Path_LiftPoint*>(pTlvIter)->mIsStartPoint == relive::reliveChoice::eYes))
                            {
                                relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlvIter), pMap, tlvOffsetLevelIdPathId.all);
                                return;
                            }
                        }
                    }
                    pTlvIter = Path_TLV::Next_446460(pTlvIter);
                }
                pointNumber++;
            }

            relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
        }
    }
}


void Factory_Null_482E70(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_483D90(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_WellExpress_483340(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEWELL.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbewellAOResID, loadMode);
        ResourceManager::LoadResource_446C90("WELLLEAF.BAN", ResourceManager::Resource_Animation, AOResourceID::kWellLeafAOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kAbewellAOResID,
            AOResourceID::kWellLeafAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        const FP xpos = FP_FromInteger(pTlv->mTopLeftX);
        const FP ypos = FP_FromInteger(pTlv->mTopLeftY);
        relive_new Well(static_cast<relive::Path_WellBase*>(pTlv), xpos, ypos, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Dove_4834C0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDovbasicAOResID);

        auto pDoveTlv = static_cast<relive::Path_Dove*>(pTlv);

        const s16 width = pDoveTlv->mWidth;
        const s16 height = pDoveTlv->mHeight;

        for (s32 i = 0; i < pDoveTlv->mDoveCount; i++)
        {
            auto pDove = relive_new Dove(
                AnimId::Dove_Idle,
                tlvOffsetLevelIdPathId.all,
                pDoveTlv->mScale != relive::reliveScale::eFull ? FP_FromDouble(0.5) : FP_FromInteger(1));

            if (pDove)
            {
                s16 ypos = 0;
                if (pDoveTlv->mPixelPerfect == relive::reliveChoice::eYes)
                {
                    pDove->mXPos = FP_FromInteger(pDoveTlv->mTopLeftX);
                    ypos = pDoveTlv->mTopLeftY;
                }
                else
                {
                    pDove->mXPos = FP_FromInteger(pDoveTlv->mTopLeftX + width * Math_NextRandom() / 256);
                    ypos = pDoveTlv->mTopLeftY + height * Math_NextRandom() / 256;
                }

                pDove->mYPos = FP_FromInteger(ypos) + FP_FromInteger(10);
            }
        }
    }
}


void Factory_RockSack_483680(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kAberockAOResID}};

        ResourceManager::LoadResourcesFromList_446E80("RTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, AOResourceID::kP2c2bagAOResID, loadMode);
            ResourceManager::LoadResource_446C90("EPUIROCK.BAN", ResourceManager::Resource_Palt, AOResourceID::kAberockAOResID, loadMode);
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kP2c2bagAOResID);
        relive_new RockSack(static_cast<relive::Path_RockSack*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_ZBall_483890(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple)
        {
            ResourceManager::LoadResource_446C90("F2ZBALL.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2zballAOResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEKNOKZ.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeknokzAOResID, loadMode);
        }
    }
    else
    {
        relive_new ZBall(static_cast<relive::Path_ZBall*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_FallingItem_483940(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::LoadResource_446C90("R1BARREL.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("R1MEAT.BAN", ResourceManager::Resource_Animation, AOResourceID::kMeatGibAOResID_6014, loadMode);
                break;

            case EReliveLevelIds::eLines:
            case EReliveLevelIds::eBoardRoom:
                ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, loadMode);
                break;

            case EReliveLevelIds::eStockYards:
                ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, loadMode);
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, loadMode);
                break;

            default:
                ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, loadMode);
                break;
        }
    }
    else
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMeatGibAOResID_6014);
                break;

            case EReliveLevelIds::eLines:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID);
                break;

            case EReliveLevelIds::eStockYards:
            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID);
                break;

            default:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID);
        }

        auto kResourcesToCheck = {
            AOResourceID::kF2rockAOResID,
            AOResourceID::kStickGibAOResID,
            AOResourceID::kObjectShadowAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        relive_new FallingItem(static_cast<relive::Path_FallingItem*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Null_482E90(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_482E80(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_PullRingRope_483DA0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::LoadResource_446C90("R1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResource_446C90("R1PULRNG.BAN", ResourceManager::Resource_Animation, AOResourceID::kPullringAOResID, loadMode);
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
                ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResource_446C90("PULLRING.BAN", ResourceManager::Resource_Animation, AOResourceID::kPullringAOResID, loadMode);
                break;

            default:
                ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResource_446C90("PULLRING.BAN", ResourceManager::Resource_Animation, AOResourceID::kPullringAOResID, loadMode);
                break;
        }
    }
    else
    {
        auto kResourcesToCheck = {
            AOResourceID::kPullringAOResID,
            AOResourceID::kRopesAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        relive_new PullRingRope(static_cast<relive::Path_PullRingRope*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_BackgroundAnimation_4840A0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new BackgroundAnimation(static_cast<relive::Path_BackgroundAnimation*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Honey_4844A0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ELMHONEY.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmHoneyAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, loadMode);
        ResourceManager::LoadResource_446C90("HONEY.BAN", ResourceManager::Resource_Animation, AOResourceID::kHoneyAOResID, loadMode);
    }
    else
    {
        auto kResourcesToCheck = {
            AOResourceID::kElmHoneyAOResID,
            AOResourceID::kElmWaspAOResID_204,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        const auto midPoint = pTlv->mWidth / 2;
        auto pHoney = relive_new Honey(
            FP_FromInteger(midPoint + pTlv->mTopLeftX),
            FP_FromInteger(pTlv->mTopLeftY + 24));
        if (pHoney)
        {
            pHoney->mTlvInfo = tlvOffsetLevelIdPathId.all;
        }
    }
}


void Factory_Null_484640(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_TimedMine_484650(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pTimedMineTlv = static_cast<relive::Path_TimedMine*>(pTlv);
    const auto disabledResource = pTimedMineTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, disabledResource & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, disabledResource & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, disabledResource & 4);

        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
            ResourceManager::LoadResource_446C90("E1BOMB.BAN", ResourceManager::Resource_Palt, AOResourceID::kBombAOResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, loadMode);
        }

        static CompileTimeResourceList<2> kBombResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kBombAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kBombflshAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("BOMB.BND", kBombResources.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kExplodeResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kExplodeResources.AsList(), loadMode, 0);
    }
    else
    {
        auto kResourcesToCheck = {
            AOResourceID::kAbebombAOResID,
            AOResourceID::kBombAOResID,
            AOResourceID::kBombflshAOResID,
            AOResourceID::kBgexpldAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        if (!(disabledResource & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        if (!(disabledResource & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID);
        }

        relive_new TimedMine(pTimedMineTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Null_482EA0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}

static CompileTimeResourceList<3> kSligZResources_4BD1AC = {
    {ResourceManager::Resource_Animation, AOResourceID::kSlgzshotAOResID},
    {ResourceManager::Resource_Animation, AOResourceID::kZflashAOResID},
    {ResourceManager::Resource_Animation, AOResourceID::kAbeknokzAOResID}};

static CompileTimeResourceList<4> kSligResources_4BD1CC = {
    {ResourceManager::Resource_Animation, AOResourceID::kSlgbasicAOResID},
    {ResourceManager::Resource_Animation, AOResourceID::kSlgknbkAOResID},
    {ResourceManager::Resource_Animation, AOResourceID::kBigflashAOResID},
    {ResourceManager::Resource_Animation, AOResourceID::kShellAOResID}};


static void LoadWalkingSligResources(LoadMode loadMode, BitField16<SligFlags_DisabledRes> disabledResources)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("SLGLEVER.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlgleverAOResID, loadMode, disabledResources.Get(eDisabledRes_Bit1_SligLever));
        ResourceManager::LoadResource_446C90("SLGLIFT.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlgliftAOResID, loadMode, disabledResources.Get(eDisabledRes_Bit2_SligLift));
        ResourceManager::LoadResource_446C90("SLGSLEEP.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlgsleepAOResID, loadMode, disabledResources.Get(eDisabledRes_Bit7_SligSleep));
        ResourceManager::LoadResource_446C90("SLGEDGE.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlgedgeAOResID, loadMode, disabledResources.Get(eDisabledRes_Bit9_SligEdge));
        ResourceManager::LoadResource_446C90("SLGSMASH.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlgsmashAOResID, loadMode, disabledResources.Get(eDisabledRes_Bit10_SligSmash));
        ResourceManager::LoadResource_446C90("SLGBEAT.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlgbeatAOResID, loadMode, disabledResources.Get(eDisabledRes_Bit11_SligBeat));
        ResourceManager::LoadResource_446C90("SLGKNFD.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlgknfdAOResID, loadMode, disabledResources.Get(eDisabledRes_Bit8_SligKnfd));

        ResourceManager::LoadResourcesFromList_446E80(
            "SLIGZ.BND",
            kSligZResources_4BD1AC.AsList(),
            loadMode,
            disabledResources.Get(eDisabledRes_Bit3_SligZ));

        ResourceManager::LoadResourcesFromList_446E80(
            "SLIG.BND",
            kSligResources_4BD1CC.AsList(),
            loadMode,
            0);

        ResourceManager::LoadResource_446C90("SLGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSligBlowAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode, 0);

        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
            ResourceManager::LoadResource_446C90("E1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kSlgbasicAOResID, loadMode, 0);
        }
    }
    else
    {
        if (!(disabledResources.Get(eDisabledRes_Bit1_SligLever)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgleverAOResID);
        }

        if (!(disabledResources.Get(eDisabledRes_Bit2_SligLift)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgliftAOResID);
        }

        if (!(disabledResources.Get(eDisabledRes_Bit3_SligZ)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgzshotAOResID);
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kZflashAOResID);
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeknokzAOResID);
        }

        if (!(disabledResources.Get(eDisabledRes_Bit7_SligSleep)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgsleepAOResID);
        }

        if (!(disabledResources.Get(eDisabledRes_Bit9_SligEdge)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgedgeAOResID);
        }

        if (!(disabledResources.Get(eDisabledRes_Bit10_SligSmash)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgsmashAOResID);
        }

        if (!(disabledResources.Get(eDisabledRes_Bit11_SligBeat)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgbeatAOResID);
        }

        if (!(disabledResources.Get(eDisabledRes_Bit8_SligKnfd)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgknfdAOResID);
        }

        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kBigflashAOResID);
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kShellAOResID);
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID);
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSligBlowAOResID);
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgbasicAOResID);
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlgknbkAOResID);
    }
}

void Factory_Slig_482EC0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->mDisabledResources);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing
    }
    else
    {
        relive_new Slig(pSligTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Slog_485030(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<4> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kDogbasicAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDogrstnAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDogattkAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDogidleAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("SLOG.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGKNFD.BAN", ResourceManager::Resource_Animation, AOResourceID::kDogknfdAOResID, loadMode, 0);
    }
    else
    {
        auto kResourceToCheck = {
            AOResourceID::kDogbasicAOResID,
            AOResourceID::kDogrstnAOResID,
            AOResourceID::kDogattkAOResID,
            AOResourceID::kDogknfdAOResID,
            AOResourceID::kDogidleAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourceToCheck);

        relive_new Slog(static_cast<relive::Path_Slog*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Switch_485370(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eStockYards:
            case EReliveLevelIds::eStockYardsReturn:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::LoadResource_446C90("R1SWITCH.BAN", ResourceManager::Resource_Animation, AOResourceID::kLeverAOResID, loadMode);
                break;

            case EReliveLevelIds::eLines:
                ResourceManager::LoadResource_446C90("L1SWITCH.BAN", ResourceManager::Resource_Animation, AOResourceID::kLeverAOResID, loadMode);
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManager::LoadResource_446C90("SWITCH1.BAN", ResourceManager::Resource_Animation, AOResourceID::kLeverAOResID, loadMode);
                break;

            default:
                ResourceManager::LoadResource_446C90("SWITCH1.BAN", ResourceManager::Resource_Animation, AOResourceID::kLeverAOResID, loadMode);
                break;
        }
        ResourceManager::LoadResource_446C90("ABEPULL.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbepullAOResID, loadMode);
    }
    else
    {
        relive_new Lever(static_cast<relive::Path_Lever*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_BellHammer_4854B0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kHammerAOResID);
        relive_new BellHammer(static_cast<relive::Path_BellHammer*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_StartController_Null_4817E0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_SecurityOrb_485550(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSecurityOrbTlv = static_cast<relive::Path_SecurityOrb*>(pTlv);
    const auto disabledResources = pSecurityOrbTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("F2MAMORB.BAN", ResourceManager::Resource_Animation, AOResourceID::kMaimGameAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, AOResourceID::kSplineAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, disabledResources & 4);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, loadMode, disabledResources & 0x10);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMaimGameAOResID);
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSplineAOResID);


        if (!(disabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        if (!(disabledResources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID);
        }

        if (!(disabledResources & 0x10))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID);
        }

        if (!(disabledResources & 0x20))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID);
        }

        relive_new SecurityOrb(pSecurityOrbTlv, tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Null_4817F0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_487070(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_LiftMud_4857D0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbebsic1AOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABELIFT.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeliftAOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kAbebsic1AOResID,
            AOResourceID::kAbeliftAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new Mudokon(pTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Null_482EB0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_BeeSwarmHole_485E20(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEWASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbewaspAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("WASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, loadMode, 0);
        if (gMap.mCurrentLevel == EReliveLevelIds::eForest || gMap.mCurrentLevel == EReliveLevelIds::eDesert)
        {
            ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, loadMode, 0);
        }
    }
    else
    {
        relive_new BeeSwarmHole(static_cast<relive::Path_BeeSwarmHole*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Pulley_Null_481800(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_HoneySack_485EF0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("HONEY.BAN", ResourceManager::Resource_Animation, AOResourceID::kHoneyAOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kP2c3HiveAOResID,
            AOResourceID::kWaspAOResID,
            AOResourceID::kAbewaspAOResID,
            AOResourceID::kHoneyAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new HoneySack(static_cast<relive::Path_HoneySack*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_AbeStart_486050(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2 && !gAttract_507698)
    {
        if (!pPauseMenu_5080E0)
        {
            pPauseMenu_5080E0 = relive_new PauseMenu();
        }

        if (!sActiveHero)
        {
            sActiveHero = relive_new Abe(55888, 85, 57, 55);
            if (sActiveHero)
            {
                sActiveHero->mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
                sActiveHero->mYPos = FP_FromInteger(pTlv->mTopLeftY);
            }
        }
    }
}


void Factory_ElumStart_Null_481810(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_486140(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_ElumWall_487370(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ELMKNBK.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmnknbkAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ANEKNBK.BAN", ResourceManager::Resource_Animation, AOResourceID::kAneknbkAOResID, loadMode, 0);
    }
    else
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


void Factory_SlingMud_485A30(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto kResources = {
            AOResourceID::kSlingmudAOResID_508,
            AOResourceID::kSparksAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new SlingMudokon(static_cast<relive::Path_SlingMudokon*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_HoneyDrip_Null_481820(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}

void Factory_BeeNest_486150(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<2> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kAbewaspAOResID}};

        ResourceManager::LoadResourcesFromList_446E80("SWARMHOL.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, loadMode);
    }
    else
    {
        relive_new BeeNest(static_cast<Path_BeeNest*>(pTlv), tlvOffsetLevelIdPathId.all); // missing converter or different name??
    }
}


void Factory_Null_487080(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Well_4834A0(relive::Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    Factory_WellExpress_483340(pTlv, pMap, tlvOffsetLevelIdPathId, loadMode);
}


void Factory_Mine_4848D0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pMineTlv = static_cast<relive::Path_Mine*>(pTlv);
    const auto disabledResources = pMineTlv->mDisabledResources;

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, disabledResources & 4);
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
            ResourceManager::LoadResource_446C90("LNDMNPAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kLandmineAOResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, loadMode);
        }

        static CompileTimeResourceList<2> kResourcesMine = {
            {ResourceManager::Resource_Animation, AOResourceID::kLandmineAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kMineflshAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("MINE.BND", kResourcesMine.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kResourcesExplode = {
            {ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kResourcesExplode.AsList(), loadMode, 0);
    }
    else
    {
        auto kResources = {
            AOResourceID::kAbebombAOResID,
            AOResourceID::kDebrisID00AOResID,
            AOResourceID::kLandmineAOResID,
            AOResourceID::kMineflshAOResID,
            AOResourceID::kBgexpldAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(disabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        if (!(disabledResources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID);
        }

        relive_new Mine(pMineTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Uxb_484B70(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pUxbTlv = static_cast<relive::Path_UXB*>(pTlv);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, pUxbTlv->mDisabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, pUxbTlv->mDisabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, pUxbTlv->mDisabledResources & 4);
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
            ResourceManager::LoadResource_446C90("TBMBPAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kUXBAOResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, loadMode);
        }

        static CompileTimeResourceList<3> kResourcesUxb = {
            {ResourceManager::Resource_Animation, AOResourceID::kUXBAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kBombflshAOResID},
            {ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("UXB.BND", kResourcesUxb.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kResourcesExplode = {
            {ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kResourcesExplode.AsList(), loadMode, 0);
    }
    else
    {
        auto kResources = {
            AOResourceID::kAbebombAOResID,
            AOResourceID::kUXBAOResID,
            AOResourceID::kBombflshAOResID,
            AOResourceID::kBgexpldAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(pUxbTlv->mDisabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        if (!(pUxbTlv->mDisabledResources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID);
        }

        relive_new UXB(pUxbTlv, tlvOffsetLevelIdPathId.all);
    }
}

void Factory_Paramite_4861F0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<9> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kArjbasicAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kArjpumpAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kArjponceAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kArjclimbAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kArjscrchAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kArjeatAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kArjfalrkAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kArjwaspAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kWebAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("PARAMITE.BND", kResources.AsList(), loadMode, 0);
        return;
    }

    auto kResources = {
        AOResourceID::kArjbasicAOResID,
        AOResourceID::kArjpumpAOResID,
        AOResourceID::kArjponceAOResID,
        AOResourceID::kArjclimbAOResID,
        AOResourceID::kArjscrchAOResID,
        AOResourceID::kArjeatAOResID,
        AOResourceID::kArjfalrkAOResID,
        AOResourceID::kArjwaspAOResID,
        AOResourceID::kWebAOResID,
    };
    ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

    relive_new Paramite(static_cast<relive::Path_Paramite*>(pTlv), tlvOffsetLevelIdPathId.all);
}


void Factory_Bat_486630(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("BATBASIC.BAN", ResourceManager::Resource_Animation, AOResourceID::kBatBasicAOResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kBatBasicAOResID);

        relive_new Bat(static_cast<relive::Path_Bat*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_RingMud_4858F0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbebsic1AOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEPULL.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbepullAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEOMM.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeommAOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kAbebsic1AOResID,
            AOResourceID::kAbepullAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new Mudokon(pTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_MovieStone_Null_487430(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_BirdPortal_486710(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<relive::Path_BirdPortal*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources1 = {
            {ResourceManager::Resource_Animation, AOResourceID::kPortalTerminatorAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kPortliteAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kPortlitAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("PORTAL.BND", kResources1.AsList(), loadMode, 0);

        if (pBirdPortalTlv->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
        {
            static CompileTimeResourceList<2> kResources2 = {
                {ResourceManager::Resource_Animation, AOResourceID::kAbemorphAOResID},
                {ResourceManager::Resource_Animation, AOResourceID::kShrmorphAOResID}};
            ResourceManager::LoadResourcesFromList_446E80("SHRYPORT.BND", kResources2.AsList(), loadMode, 0);
            ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, AOResourceID::kSplineAOResID, loadMode);
        }
    }
    else
    {
        auto kResources = {
            AOResourceID::kPortalTerminatorAOResID,
            AOResourceID::kDovbasicAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (pBirdPortalTlv->mPortalType != relive::Path_BirdPortal::PortalType::eShrykull)
        {
            relive_new BirdPortal(pBirdPortalTlv, tlvOffsetLevelIdPathId.all);
            return;
        }

        auto kResources2 = {
            AOResourceID::kAbemorphAOResID,
            AOResourceID::kShrmorphAOResID,
            AOResourceID::kSplineAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources2);

        relive_new BirdPortal(pBirdPortalTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_BirdPortalExit_null_486700(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_BellSong_487450(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


void Factory_TrapDoor_4868E0(relive::Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManager::LoadResource_446C90("R1TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
            case EReliveLevelIds::eLines:
                ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
            case EReliveLevelIds::eStockYards:
                ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManager::LoadResource_446C90("D2TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
            default:
                ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID);

        relive_new TrapDoor(static_cast<relive::Path_TrapDoor*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_RollingBall_486A60(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("F2STNBAL.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2stnbalAOResID, loadMode);
        ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kF2stnbalAOResID,
            AOResourceID::kDebrisID00AOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new RollingBall(static_cast<relive::Path_RollingBall*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

static relive::Path_TLV* FindMatchingSligTLV(relive::Path_TLV* pTlvIter, relive::Path_SligBound* pTlv)
{
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType == static_cast<s32>(TlvTypes::Slig_24) && pTlv->mSligBoundId == static_cast<relive::Path_Slig*>(pTlvIter)->mSligBoundId && !pTlvIter->mTlvFlags.Get(::TlvFlags::eBit2_Destroyed))
        {
            return pTlvIter;
        }
        pTlvIter = Path_TLV::Next_446460(pTlvIter);
    }
    return nullptr;
}

void Factory_SligLeftBound_482520(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pBound = static_cast<relive::Path_SligBound*>(pTlv);
    LoadWalkingSligResources(loadMode, pBound->mDisabledResources);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        pBound->mTlvFlags.Clear(::TlvFlags::eBit1_Created);
        pBound->mTlvFlags.Clear(::TlvFlags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            relive::Path_TLV* pTlvIter = gMap.Get_First_TLV_For_Offsetted_Camera(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->mTlvFlags.Set(::TlvFlags::eBit1_Created);
                pTlvIter->mTlvFlags.Set(::TlvFlags::eBit2_Destroyed);
                // AO OG bug tlvOffsetLevelIdPathId not recalculated??
                relive_new Slig(static_cast<relive::Path_Slig*>(pTlvIter), tlvOffsetLevelIdPathId.all);
                return;
            }
        }
    }
}

void Factory_InvisibleZone_Null_481840(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}

void Factory_RollingBallStopper_486B90(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("F2STNSCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2stnsckAOResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kF2stnsckAOResID);
        relive_new RollingBallStopper(static_cast<relive::Path_RollingBallStopper*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_FootSwitch_486C60(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("TRIGGER.BAN", ResourceManager::Resource_Animation, AOResourceID::kTriggerAOResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kTriggerAOResID);

        relive_new FootSwitch(static_cast<relive::Path_FootSwitch*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_SecurityClaw_486D50(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSecurityClawTlv = static_cast<relive::Path_SecurityClaw*>(pTlv);
    const auto disabledResources = pSecurityClawTlv->mDisabledResources;

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("F2EYEORB.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2eyeorbAOResID, loadMode);
        ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, AOResourceID::kSplineAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, disabledResources & 4);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, loadMode, disabledResources & 0x10);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        auto kResources = {
            AOResourceID::kF2eyeorbAOResID,
            AOResourceID::kSplineAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(disabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        if (!(disabledResources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID);
        }

        if (!(disabledResources & 0x10))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID);
        }

        if (!(disabledResources & 0x20))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID);
        }

        relive_new SecurityClaw(pSecurityClawTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_MotionDector_486FD0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDeathFlareAOResID);

        relive_new MotionDetector(static_cast<relive::Path_MotionDetector*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Null_4870A0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_481C70(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_4870B0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_SligSpawner_482A00(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->mDisabledResources);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        relive_new SligSpawner(pSligTlv, tlvOffsetLevelIdPathId.all);
    }
}

void Factory_ElectricWall_4874E0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kElecwallAOResID);

        relive_new ElectricWall(static_cast<relive::Path_ElectricWall*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_LiftMover_487580(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new LiftMover(static_cast<relive::Path_LiftMover*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_ChimeLock_4870D0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto kResources = {
            AOResourceID::kChimeAOResID,
            AOResourceID::kF2ChimeBallAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new ChimeLock(static_cast<relive::Path_ChimeLock*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_ElumStart_Unknown_4873D0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources({
            {ResourceManager::Resource_Animation, AOResourceID::kElmbasicAOResID_200},
            {ResourceManager::Resource_Animation, AOResourceID::kElmaloneAOResID_230},
            {ResourceManager::Resource_Animation, AOResourceID::kElmfallAOResID_216},
        });
        ResourceManager::LoadResourcesFromList_446E80("ELMSTART.BND", kResources.AsList(), loadMode, 0);
    }
    else
    {
        Elum::Spawn(tlvOffsetLevelIdPathId);
        gElum->mXPos = FP_FromInteger(pTlv->mTopLeftX);
        gElum->mYPos = FP_FromInteger(pTlv->mTopLeftY);
    }
}


void Factory_MeatSack_483790(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kMeatAOResID}});

        ResourceManager::LoadResourcesFromList_446E80("MTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, AOResourceID::kP2c2bagAOResID, loadMode);
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMeatSackAOResID);

        relive_new MeatSack(static_cast<relive::Path_MeatSack*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Scrab_4863E0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<12> kResources(
            {{ResourceManager::Resource_Animation, AOResourceID::kArsbasicAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArsdanceAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArsdeadAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArsgrwlAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArshowlAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArsroarAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArswhirlAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArschewAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArseatAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArsprceAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArsskwrAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kArscrshAOResID}});
        ResourceManager::LoadResourcesFromList_446E80("SCRAB.BND", kResources.AsList(), loadMode, 0);
        return;
    }

    auto kResources = {
        AOResourceID::kArsgrwlAOResID,
        AOResourceID::kArsdeadAOResID,
        AOResourceID::kArsdanceAOResID,
        AOResourceID::kArsbasicAOResID,
        AOResourceID::kArshowlAOResID,
        AOResourceID::kArsroarAOResID,
        AOResourceID::kArswhirlAOResID,
        AOResourceID::kArschewAOResID,
        AOResourceID::kArseatAOResID,
        AOResourceID::kArsprceAOResID,
        AOResourceID::kArsskwrAOResID,
        AOResourceID::kArscrshAOResID,
    };
    ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

    relive_new Scrab(static_cast<relive::Path_Scrab*>(pTlv), tlvOffsetLevelIdPathId.all);
}


void Factory_FlintLockFire_487640(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("FIRE.BAN", ResourceManager::Resource_Animation, AOResourceID::kHubFireAOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kFlintLockAOResID,
            AOResourceID::kGourdAOResID,
            AOResourceID::kHubFireAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new FlintLockFire(static_cast<relive::Path_FlintLockFire*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_ScrabLeftBound_Null_481860(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_ScrabRightBound_Null_481870(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_SligRightBound_4829D0(relive::Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    Factory_SligLeftBound_482520(pTlv, pMap, tlvOffsetLevelIdPathId, loadMode);
}


void Factory_SligPersist_Null_4829F0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_4870C0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_EnemyStopper_Null_481880(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_Null_4871C0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_InvisibleSwitch_481C10(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new InvisibleSwitch(static_cast<relive::Path_InvisibleSwitch*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_WorkerMud_485B20(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pMudTlv = static_cast<relive::Path_Mudokon*>(pTlv);
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto kResources = {
            AOResourceID::kAbebsic1AOResID,
            AOResourceID::kAbebasicAOResID,
            AOResourceID::kAbeknfdAOResID,
            AOResourceID::kAbeknbkAOResID,
            AOResourceID::kAbeedgeAOResID,
            AOResourceID::kMudscrubAOResID,
            AOResourceID::kMudoduckAOResID,
            AOResourceID::kMudbtlnkAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        switch (pMudTlv->mJob)
        {
            case relive::Path_Mudokon::MudJobs::eStandScrub:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMudchslAOResID);
                break;

            case relive::Path_Mudokon::MudJobs::eSitChant:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMudltusAOResID);
                break;
        }

        relive_new Mudokon(pTlv, tlvOffsetLevelIdPathId.all);
    }
    else
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbebsic1AOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBSIC.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbebasicAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNFD.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeknfdAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNBK.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeknbkAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEEDGE.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeedgeAOResID, loadMode);
        ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);

        switch (pMudTlv->mJob)
        {
            case relive::Path_Mudokon::MudJobs::eStandScrub:
            {
                static CompileTimeResourceList<3> kResources = {
                    {ResourceManager::Resource_Animation, AOResourceID::kMudoduckAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudscrubAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudbtlnkAOResID}};
                ResourceManager::LoadResource_446C90("MUDCHSL.BAN", ResourceManager::Resource_Animation, AOResourceID::kMudchslAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
            }
            break;

            case relive::Path_Mudokon::MudJobs::eSitScrub:
            {
                static CompileTimeResourceList<3> kResources = {
                    {ResourceManager::Resource_Animation, AOResourceID::kMudoduckAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudscrubAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudbtlnkAOResID}};
                ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
            }
            break;

            case relive::Path_Mudokon::MudJobs::eSitChant:
            {
                static CompileTimeResourceList<4> kResources = {
                    {ResourceManager::Resource_Animation, AOResourceID::kMudltusAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudoduckAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudscrubAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudbtlnkAOResID}};
                ResourceManager::LoadResourcesFromList_446E80("MUDLOTUS.BND", kResources.AsList(), loadMode, 0);
            }
            break;
        }
    }
}


void Factory_SligZCover_Null_481850(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_DoorFlame_481E80(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("FIRE.BAN", ResourceManager::Resource_Animation, AOResourceID::kHubFireAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("GLOW1.BAN", ResourceManager::Resource_Animation, AOResourceID::kGlowAOResID, loadMode, 0);
    }
    else
    {
        auto kResources = {
            AOResourceID::kHubFireAOResID,
            AOResourceID::kGlowAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new DoorFlame(static_cast<relive::Path_DoorFlame*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_OneShotSwitchIdSetter_4871D0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        // dead tlv
        //relive_new OneShotSwitchIdSetter(static_cast<Path_OneShotSwitchIdSetter*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_MovingBomb_484E00(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<relive::Path_MovingBomb*>(pTlv);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("D1MBOMB.BAN", ResourceManager::Resource_Animation, AOResourceID::kMovebombAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, pMovingBombTlv->mDisabledResources & 1);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, pMovingBombTlv->mDisabledResources & 4);

        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, loadMode, 0);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, loadMode, 0);
        }
    }
    else
    {
        auto kResources = {
            AOResourceID::kMovebombAOResID,
            AOResourceID::kExplo2AOResID,
            AOResourceID::kMetalGibAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(pMovingBombTlv->mDisabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        relive_new MovingBomb(pMovingBombTlv, tlvOffsetLevelIdPathId.all);
    }
}


void Factory_MovingBombStopper_Null_484DF0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_MeatSaw_483F70(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("R1METSAW.BAN", ResourceManager::Resource_Animation, AOResourceID::kMeatSawAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kMeatSawAOResID,
            AOResourceID::kAbeblowAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new MeatSaw(static_cast<relive::Path_MeatSaw*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_MudPathTrans_Null_4818B0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_MenuController_481AC0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (gMainMenuInstanceCount_9F2DE0 == 0)
    {
        if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
        {
            static CompileTimeResourceList<4> kResources = {
                {ResourceManager::Resource_Animation, AOResourceID::kAbespek2AOResID},
                {ResourceManager::Resource_Animation, AOResourceID::kHighliteAOResID},
                {ResourceManager::Resource_Animation, AOResourceID::kOptionFlareAOResID},
                {ResourceManager::Resource_Palt, AOResourceID::kHighlitePalAOResID}};
            ResourceManager::LoadResourcesFromList_446E80("STARTANM.BND", kResources.AsList(), loadMode, 0);
        }
        else
        {
            auto kResources = {
                AOResourceID::kAbespek2AOResID,
                AOResourceID::kHighliteAOResID,
                AOResourceID::kOptionFlareAOResID,
            };
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

            relive_new Menu(pTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


void Factory_481FB0(relive::Path_TLV* /*pTlv*/, Map* pMap, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    LOG_ERROR("lvl = " << static_cast<s32>(pMap->mCurrentLevel) << " path = " << pMap->mCurrentPath << " camera = " << pMap->mCurrentCamera);
    ALIVE_FATAL("Didn't expect Factory_481FB0 to ever be used, please post the log in #oddworld_dev on discord");
}


void Factory_HintFly_4819E0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new HintFly(static_cast<relive::Path_HintFly*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_ScrabNoFall_Null_481890(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_IdSplitter_4875E0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new TimerTrigger(static_cast<relive::Path_TimerTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_SecurityDoor_487790(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("R1SDOS.BAN", ResourceManager::Resource_Animation, AOResourceID::kR1sdosAOResID_6027, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kR1sdosAOResID_6027);

        relive_new SecurityDoor(static_cast<relive::Path_SecurityDoor*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_Null_487440(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_GrenadeMachine_487860(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pBoomMachineTlv = static_cast<relive::Path_BoomMachine*>(pTlv);
    const auto disabledResources = pBoomMachineTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kGrenadeAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("GTHROW.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, loadMode);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, disabledResources & 2);
        return;
    }

    auto kResources = {
        AOResourceID::kR1buttonAOResID,
        AOResourceID::kR1bpipeAOResID,
        AOResourceID::kAbestoneAOResID,
        AOResourceID::kAbethrowAOResID,
        AOResourceID::kAbepickAOResID,
        AOResourceID::kGrenadeAOResID,
        AOResourceID::kExplo2AOResID,
        AOResourceID::kAbeblowAOResID,
        AOResourceID::kMetalGibAOResID,
    };
    ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

    if (!(disabledResources & 2))
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID);
    }

    relive_new BoomMachine(pBoomMachineTlv, tlvOffsetLevelIdPathId.all);
}


void Factory_LCD_481950(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, AOResourceID::kLcdfontAOResID, loadMode);
    }
    else
    {
        relive_new LCDScreen(static_cast<relive::Path_LCDScreen*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_ElumPathTrans_Null_4818C0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_HandStone_487480(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABESTONE.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbestoneAOResID, loadMode);
        ResourceManager::LoadResource_446C90("SPOTLITE.BAN", ResourceManager::Resource_Animation, AOResourceID::kSpotliteAOResID, loadMode);
    }
    else
    {
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


void Factory_CreditsController_481A50(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        if (!gCreditsControllerExists)
        {
            relive_new CreditsController(static_cast<relive::Path_CreditsController*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


void Factory_Preloader_Null_4817A0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_StatusBoard_487AF0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, AOResourceID::kLcdfontAOResID, loadMode);
    }
    else
    {
        relive_new LCDStatusBoard(static_cast<relive::Path_LCDStatusBoard*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_SwitchStateBooleanLogic_487B80(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        // does the converter have a different name?
        relive_new SwitchStateBooleanLogic(static_cast<relive::Path_SwitchStateBooleanLogic*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_MusicTrigger_482020(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new MusicTrigger(static_cast<relive::Path_MusicTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

void Factory_LightEffect_484170(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pPathLightTlv = static_cast<relive::Path_LightEffect*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (pPathLightTlv->mType)
        {
            case relive::Path_LightEffect::Type::Star:
                ResourceManager::LoadResource_446C90("STAR.BAN", ResourceManager::Resource_Animation, AOResourceID::kBGStarAOResID, loadMode);
                break;

            case relive::Path_LightEffect::Type::GoldGlow:
                ResourceManager::LoadResource_446C90("GOLDGLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kGoldGlowAOResID_6011, loadMode);
                break;

            case relive::Path_LightEffect::Type::GreenGlow:
                ResourceManager::LoadResource_446C90("GRENGLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kGreenGlowAOResID_6010, loadMode);
                break;

            case relive::Path_LightEffect::Type::FlintGlow:
                ResourceManager::LoadResource_446C90("FLNTGLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kFlintGlowAOResID_6028, loadMode);
                break;

            case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
                ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, AOResourceID::kGreenDoorLightAOResID_6031, loadMode);
                ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, AOResourceID::kRedDoorLightAOResID_6032, loadMode);
                break;

            case relive::Path_LightEffect::Type::Switchable_RedGreenHubLight:
                ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, AOResourceID::kGreenDoorLightAOResID_6031, loadMode);
                ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, AOResourceID::kRedDoorLightAOResID_6032, loadMode);
                break;

            default:
                return;
        }
    }
    else
    {
        switch (pPathLightTlv->mType)
        {
            case relive::Path_LightEffect::Type::Star:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kBGStarAOResID);
                relive_new LightEffect(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                break;
            }

            case relive::Path_LightEffect::Type::GoldGlow:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kGoldGlowAOResID_6011);
                relive_new DoorLight(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                break;
            }

            case relive::Path_LightEffect::Type::GreenGlow:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kGreenGlowAOResID_6010);
                relive_new DoorLight(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                break;
            }

            case relive::Path_LightEffect::Type::FlintGlow:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kFlintGlowAOResID_6028);
                relive_new DoorLight(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                break;
            }

            case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
            case relive::Path_LightEffect::Type::Switchable_RedGreenHubLight:
            {
                auto kResources = {
                    AOResourceID::kGreenDoorLightAOResID_6031,
                    AOResourceID::kRedDoorLightAOResID_6032,
                };
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

                relive_new DoorLight(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                break;
            }

            default:
                return;
        }
    }
}


void Factory_SlogSpawner_4851D0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<4> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kDogbasicAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDogrstnAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDogattkAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kDogidleAOResID}};

        ResourceManager::LoadResourcesFromList_446E80("SLOG.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGKNFD.BAN", ResourceManager::Resource_Animation, AOResourceID::kDogknfdAOResID, loadMode);
    }
    else
    {
        auto kResources = {
            AOResourceID::kDogbasicAOResID,
            AOResourceID::kDogattkAOResID,
            AOResourceID::kDogknfdAOResID,
            AOResourceID::kDogidleAOResID,
            AOResourceID::kDogrstnAOResID,

        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        relive_new SlogSpawner(static_cast<relive::Path_SlogSpawner*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_GasCountDown_487BE0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, AOResourceID::kLcdfontAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEGAS.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbegasAOResID, loadMode);
    }
    else
    {
        relive_new GasCountDown(static_cast<relive::Path_GasCountDown*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}



void Factory_RingCancel_4818D0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        // The field field_18_bShrykull_remove was removed from the Path_RingCancel TLV because it doesnt
        // actually exist in any path data. The actual value for this field was the 2 bytes after the TLV ended
        // which is always 0 apart from in the cases below.
        // However any level saved with the legacy level editor will have this field added which is handled by checking the size.
        bool bRemovesShrykull = false;

        struct Path_RingCancel_Corrected final : public relive::Path_RingCancel
        {
            s16 field_18_bShrykull_remove;
        };

        if (pTlv->mLength == sizeof(Path_RingCancel_Corrected))
        {
            bRemovesShrykull = static_cast<Path_RingCancel_Corrected*>(pTlv)->field_18_bShrykull_remove;
        }
        else
        {
            switch (gMap.mCurrentLevel)
            {
                case EReliveLevelIds::eDesert: // d1.lvl
                    if (gMap.mCurrentPath == 4)
                    {
                        // original TLV data is -1 part of collision line
                        bRemovesShrykull = true;
                    }
                    break;

                case EReliveLevelIds::eForestTemple: // f2.lvl
                    if (gMap.mCurrentPath == 6)
                    {
                        // original TLV data is 4 part of the flags of the next object
                        bRemovesShrykull = true;
                    }
                    break;
            }
        }

        if (bRemovesShrykull)
        {
            if (sActiveHero->field_168_ring_pulse_timer)
            {
                if (sActiveHero->field_16C_bHaveShrykull)
                {
                    Abe::Free_Shrykull_Resources_42F4C0();
                    sActiveHero->field_168_ring_pulse_timer = 0;
                }
            }
        }
        else
        {
            if (!sActiveHero->field_16C_bHaveShrykull)
            {
                sActiveHero->field_168_ring_pulse_timer = 0;
            }
        }
        Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


void Factory_GasEmitter_484110(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new GasEmitter(static_cast<relive::Path_GasEmitter*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_ZzzSpawner_487C80(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ZzzSpawner(static_cast<relive::Path_ZzzSpawner*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_BackgroundGlukkon_487CE0(relive::Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<5> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kGlkbasicAOResID},
            {ResourceManager::Resource_Palt, AOResourceID::kGlukredAOResID},
            {ResourceManager::Resource_Palt, AOResourceID::kGlukgrenAOResID},
            {ResourceManager::Resource_Palt, AOResourceID::kGlukblueAOResID},
            {ResourceManager::Resource_Palt, AOResourceID::kGlukaquaAOResID}};


        ResourceManager::LoadResourcesFromList_446E80("GLUKKON.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, loadMode);
    }
    else
    {
        relive_new BackgroundGlukkon(static_cast<relive::Path_BackgroundGlukkon*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


void Factory_KillUnsavedMuds_487DA0(relive::Path_TLV* /*pTlv*/, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        // OG bug fix - added an extra check that checks if the map has changed
        // which prevents that the killed mudokon count becomes inaccurate or even negative.
        if (!gbKillUnsavedMudsDone_5076CC &&
            pMap->field_DA_bMapChanged)
        {
            gbKillUnsavedMudsDone_5076CC = 1;
            sKilledMudokons_5076BC = 28 - sRescuedMudokons_5076C0;
            Path::TLV_Reset(tlvOffsetLevelIdPathId.all, -1, 0, 1);
        }
    }
}


void Factory_SoftLanding_Null_4817C0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void Factory_ResetPath_Null_4818A0(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


void ConstructTLVObject(relive::Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvInfo, LoadMode loadMode)
{
    switch (pTlv->mTlvType)
    {
        case TlvTypes::KillUnsavedMuds_113:
            Factory_KillUnsavedMuds_487DA0(pTlv, pMap, tlvInfo, loadMode);
            break;

        // TODO: All the others
    }
}

const PathFunctionTable kObjectFactory = {
    {Factory_ContinuePoint_Null_4817D0,
     Factory_PathTransition_Null_481720,
     Factory_ContinueZone_Null_481830,
     Factory_Hoist_487230,
     Factory_Edge_481730,
     Factory_DeathDrop_Null_4817B0,
     Factory_Door_481C80,
     Factory_ShadowZone_482080,
     Factory_LiftPoint_4820F0,
     Factory_Null_482E70,
     Factory_Null_483D90,
     Factory_WellExpress_483340,
     Factory_Dove_4834C0,
     Factory_RockSack_483680,
     Factory_ZBall_483890,
     Factory_FallingItem_483940,
     Factory_Null_482E90,
     Factory_Null_482E80,
     Factory_PullRingRope_483DA0,
     Factory_BackgroundAnimation_4840A0,
     Factory_Honey_4844A0,
     Factory_Null_484640,
     Factory_TimedMine_484650,
     Factory_Null_482EA0,
     Factory_Slig_482EC0,
     Factory_Slog_485030,
     Factory_Switch_485370,
     Factory_BellHammer_4854B0,
     Factory_StartController_Null_4817E0,
     Factory_SecurityOrb_485550,
     Factory_Null_4817F0,
     Factory_Null_487070,
     Factory_LiftMud_4857D0,
     Factory_Null_482EB0,
     Factory_BeeSwarmHole_485E20,
     Factory_Pulley_Null_481800,
     Factory_HoneySack_485EF0,
     Factory_AbeStart_486050,
     Factory_ElumStart_Null_481810,
     Factory_Null_486140,
     Factory_ElumWall_487370,
     Factory_SlingMud_485A30,
     Factory_HoneyDrip_Null_481820,
     Factory_BeeNest_486150,
     Factory_Null_487080,
     Factory_Well_4834A0,
     Factory_Mine_4848D0,
     Factory_Uxb_484B70,
     Factory_Paramite_4861F0,
     Factory_Bat_486630,
     Factory_RingMud_4858F0,
     Factory_MovieStone_Null_487430,
     Factory_BirdPortal_486710,
     Factory_BirdPortalExit_null_486700,
     Factory_BellSong_487450,
     Factory_TrapDoor_4868E0,
     Factory_RollingBall_486A60,
     Factory_SligLeftBound_482520,
     Factory_InvisibleZone_Null_481840,
     Factory_RollingBallStopper_486B90,
     Factory_FootSwitch_486C60,
     Factory_SecurityClaw_486D50,
     Factory_MotionDector_486FD0,
     Factory_Null_4870A0,
     Factory_Null_481C70,
     Factory_Null_4870B0,
     Factory_SligSpawner_482A00,
     Factory_ElectricWall_4874E0,
     Factory_LiftMover_487580,
     Factory_ChimeLock_4870D0,
     Factory_ElumStart_Unknown_4873D0,
     Factory_MeatSack_483790,
     Factory_Scrab_4863E0,
     Factory_FlintLockFire_487640,
     Factory_ScrabLeftBound_Null_481860,
     Factory_ScrabRightBound_Null_481870,
     Factory_SligRightBound_4829D0,
     Factory_SligPersist_Null_4829F0,
     Factory_Null_4870C0,
     Factory_EnemyStopper_Null_481880,
     Factory_Null_4871C0,
     Factory_InvisibleSwitch_481C10,
     Factory_WorkerMud_485B20,
     Factory_SligZCover_Null_481850,
     Factory_DoorFlame_481E80,
     Factory_OneShotSwitchIdSetter_4871D0,
     Factory_MovingBomb_484E00,
     Factory_MovingBombStopper_Null_484DF0,
     Factory_MeatSaw_483F70,
     Factory_MudPathTrans_Null_4818B0,
     Factory_MenuController_481AC0,
     Factory_481FB0,
     Factory_HintFly_4819E0,
     Factory_ScrabNoFall_Null_481890,
     Factory_IdSplitter_4875E0,
     Factory_SecurityDoor_487790,
     Factory_Null_487440,
     Factory_GrenadeMachine_487860,
     Factory_LCD_481950,
     Factory_ElumPathTrans_Null_4818C0,
     Factory_HandStone_487480,
     Factory_CreditsController_481A50,
     Factory_Preloader_Null_4817A0,
     Factory_StatusBoard_487AF0,
     Factory_SwitchStateBooleanLogic_487B80,
     Factory_MusicTrigger_482020,
     Factory_LightEffect_484170,
     Factory_SlogSpawner_4851D0,
     Factory_GasCountDown_487BE0,
     Factory_RingCancel_4818D0,
     Factory_GasEmitter_484110,
     Factory_ZzzSpawner_487C80,
     Factory_BackgroundGlukkon_487CE0,
     Factory_KillUnsavedMuds_487DA0,
     Factory_SoftLanding_Null_4817C0,
     Factory_ResetPath_Null_4818A0,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr,
     nullptr}};


} // namespace AO
