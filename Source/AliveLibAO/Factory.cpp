#include "stdafx_ao.h"
#include "Factory.hpp"
#include "Function.hpp"
#include "GasCountDown.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
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
#include "../relive_lib/ShadowZone.hpp"
#include "LiftPoint.hpp"
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

static void Factory_Hoist(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                break;

           case EReliveLevelIds::eForest:
           case EReliveLevelIds::eDesert:
                break;

            default:
                break;
        }
    }
    else
    {
        auto pHoistTlv = static_cast<relive::Path_Hoist*>(pTlv);
        if (pHoistTlv->mHoistType == relive::Path_Hoist::Type::eOffScreen)
        {
            relive_new HoistRocksEffect(pHoistTlv, tlvId);
            // OG issue, no reset on failure ??
        }
        else
        {
            Path::TLV_Reset(tlvId, -1, 0, 0);
        }
    }
}

static void Factory_Edge(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {

        if (gMap.mCurrentLevel == EReliveLevelIds::eForest || gMap.mCurrentLevel == EReliveLevelIds::eDesert)
        {
        }
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_Door(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                break;

            case EReliveLevelIds::eLines:
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                break;

            default:
                break;
        }
    }
    else
    {
        relive_new Door(static_cast<relive::Path_Door*>(pTlv), tlvId);
    }
}

static void Factory_ShadowZone(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ShadowZone(static_cast<relive::Path_ShadowZone*>(pTlv), tlvId);
    }
}

static void Factory_LiftPoint(relive::Path_TLV* pTlv, Map* pMap, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                break;

            case EReliveLevelIds::eLines:
                break;

            case EReliveLevelIds::eDesert:
                break;

            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                break;

            default:
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

            if (!pObjIter->GetDead() && pObjIter->Type() == ReliveTypes::eLiftPoint)
            {
                auto pLiftObj = static_cast<LiftPoint*>(pObjIter);

                const s16 xpos_i = FP_GetExponent(pLiftObj->mXPos);
                if (pTlv->mTopLeftX <= xpos_i
                    && xpos_i <= pTlv->mBottomRightX
                    && pLiftObj->mCurrentLevel == gMap.mCurrentLevel
                    && pLiftObj->mCurrentPath == gMap.mCurrentPath)
                {
                    Path::TLV_Reset(tlvId, -1, 0, 0);
                    return;
                }
            }
        }

        if (pTlv->mTlvSpecificMeaning & 2 || (pTlv->mTlvSpecificMeaning == 0 && static_cast<relive::Path_LiftPoint*>(pTlv)->mIsStartPoint == relive::reliveChoice::eYes))
        {
            relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlv), pMap, tlvId);
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
                    if (pTlvIter->mTlvType == ReliveTypes::eLiftPoint)
                    {
                        const auto tlv_x = pTlv->mTopLeftX;
                        const auto absX = pTlvIter->mTopLeftX - tlv_x >= 0 ? pTlvIter->mTopLeftX - tlv_x : tlv_x - pTlvIter->mTopLeftX;
                        if (absX < 5)
                        {
                            if (pTlvIter->mTlvSpecificMeaning & 2 || (pTlvIter->mTlvSpecificMeaning == 0 && static_cast<relive::Path_LiftPoint*>(pTlvIter)->mIsStartPoint == relive::reliveChoice::eYes))
                            {
                                relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlvIter), pMap, tlvId);
                                return;
                            }
                        }
                    }
                    pTlvIter = Path_TLV::Next_446460(pTlvIter);
                }
                pointNumber++;
            }

            relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlv), pMap, tlvId);
        }
    }
}

static void Factory_Well(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        const FP xpos = FP_FromInteger(pTlv->mTopLeftX);
        const FP ypos = FP_FromInteger(pTlv->mTopLeftY);
        relive_new Well(static_cast<relive::Path_WellBase*>(pTlv), xpos, ypos, tlvId);
    }
}

static void Factory_Dove(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
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

static void Factory_RockSack(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        relive_new RockSack(static_cast<relive::Path_RockSack*>(pTlv), tlvId);
    }
}

static void Factory_ZBall(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple)
        {
        }
    }
    else
    {
        relive_new ZBall(static_cast<relive::Path_ZBall*>(pTlv), tlvId);
    }
}

static void Factory_FallingItem(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                break;

            case EReliveLevelIds::eLines:
            case EReliveLevelIds::eBoardRoom:
                break;

            case EReliveLevelIds::eStockYards:
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                break;

            default:
                break;
        }
    }
    else
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                break;

            case EReliveLevelIds::eLines:
                break;

            case EReliveLevelIds::eStockYards:
            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
                break;

            default:
                break;
        }

        relive_new FallingItem(static_cast<relive::Path_FallingItem*>(pTlv), tlvId);
    }
}

static void Factory_PullRingRope(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
                break;

            default:
                break;
        }
    }
    else
    {
        relive_new PullRingRope(static_cast<relive::Path_PullRingRope*>(pTlv), tlvId);
    }
}

static void Factory_BackgroundAnimation(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new BackgroundAnimation(static_cast<relive::Path_BackgroundAnimation*>(pTlv), tlvId);
    }
}

static void Factory_Honey(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {

        const auto midPoint = pTlv->Width() / 2;
        auto pHoney = relive_new Honey(
            FP_FromInteger(midPoint + pTlv->mTopLeftX),
            FP_FromInteger(pTlv->mTopLeftY + 24));
        if (pHoney)
        {
            pHoney->mTlvInfo = tlvId;
        }
    }
}

static void Factory_TimedMine(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pTimedMineTlv = static_cast<relive::Path_TimedMine*>(pTlv);
    const auto disabledResource = pTimedMineTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }

    }
    else
    {
        if (!(disabledResource & 1))
        {
        }

        if (!(disabledResource & 2))
        {
        }

        relive_new TimedMine(pTimedMineTlv, tlvId);
    }
}

static void LoadWalkingSligResources(LoadMode loadMode, BitField16<SligFlags_DisabledRes> disabledResources)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        if (!(disabledResources.Get(eDisabledRes_Bit1_SligLever)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit2_SligLift)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit3_SligZ)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit7_SligSleep)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit9_SligEdge)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit10_SligSmash)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit11_SligBeat)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit8_SligKnfd)))
        {
        }

    }
}

static void Factory_Slig(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->mData.mDisabledResourcesAO);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing
    }
    else
    {
        relive_new Slig(pSligTlv, tlvId);
    }
}


static void Factory_Slog(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new Slog(static_cast<relive::Path_Slog*>(pTlv), tlvId);
    }
}

static void Factory_Lever(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
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
                break;

            case EReliveLevelIds::eLines:
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                break;

            default:
                break;
        }
    }
    else
    {
        relive_new Lever(static_cast<relive::Path_Lever*>(pTlv), tlvId);
    }
}

static void Factory_BellHammer(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new BellHammer(static_cast<relive::Path_BellHammer*>(pTlv), tlvId);
    }
}

static void Factory_SecurityOrb(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pSecurityOrbTlv = static_cast<relive::Path_SecurityOrb*>(pTlv);
    const auto disabledResources = pSecurityOrbTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        if (!(disabledResources & 1))
        {
        }

        if (!(disabledResources & 2))
        {
        }

        if (!(disabledResources & 0x10))
        {
        }

        if (!(disabledResources & 0x20))
        {
        }

        relive_new SecurityOrb(pSecurityOrbTlv, tlvId);
    }
}

static void Factory_LiftMud(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new Mudokon(pTlv, tlvId);
    }
}

static void Factory_BeeSwarmHole(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eForest || gMap.mCurrentLevel == EReliveLevelIds::eDesert)
        {
        }
    }
    else
    {
        relive_new BeeSwarmHole(static_cast<relive::Path_BeeSwarmHole*>(pTlv), tlvId);
    }
}

static void Factory_HoneySack(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new HoneySack(static_cast<relive::Path_HoneySack*>(pTlv), tlvId);
    }
}

static void Factory_AbeStart(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& /*tlvId*/, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2 && !gAttract)
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

static void Factory_ElumWall(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}

static void Factory_SlingMud(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new SlingMudokon(static_cast<relive::Path_SlingMudokon*>(pTlv), tlvId);
    }
}

static void Factory_BeeNest(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new BeeNest(static_cast<relive::Path_BeeNest*>(pTlv), tlvId);
    }
}

static void Factory_Mine(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pMineTlv = static_cast<relive::Path_Mine*>(pTlv);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        relive_new Mine(pMineTlv, tlvId);
    }
}

static void Factory_Uxb(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pUxbTlv = static_cast<relive::Path_UXB*>(pTlv);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        relive_new UXB(pUxbTlv, tlvId);
    }
}

static void Factory_Paramite(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        return;
    }
    relive_new Paramite(static_cast<relive::Path_Paramite*>(pTlv), tlvId);
}

static void Factory_Bat(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new Bat(static_cast<relive::Path_Bat*>(pTlv), tlvId);
    }
}

static void Factory_RingMud(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new Mudokon(pTlv, tlvId);
    }
}

static void Factory_BirdPortal(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<relive::Path_BirdPortal*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (pBirdPortalTlv->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
        {
        }
    }
    else
    {
        if (pBirdPortalTlv->mPortalType != relive::Path_BirdPortal::PortalType::eShrykull)
        {
            relive_new BirdPortal(pBirdPortalTlv, tlvId);
            return;
        }
        relive_new BirdPortal(pBirdPortalTlv, tlvId);
    }
}

static void Factory_BellSongStone(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}


static void Factory_TrapDoor(relive::Path_TLV* pTlv, Map* pMap, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                break;
            case EReliveLevelIds::eLines:
                break;
            case EReliveLevelIds::eStockYards:
                break;
            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                break;
            default:
                break;
        }
    }
    else
    {
        relive_new TrapDoor(static_cast<relive::Path_TrapDoor*>(pTlv), pMap, tlvId);
    }
}


static void Factory_RollingBall(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new RollingBall(static_cast<relive::Path_RollingBall*>(pTlv), tlvId);
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
        pTlvIter = Path_TLV::Next_446460(pTlvIter);
    }
    return nullptr;
}

static void Factory_SligBoundLeft(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pBound = static_cast<relive::Path_SligBound*>(pTlv);
    LoadWalkingSligResources(loadMode, pBound->mDisabledResourcesAO);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            relive::Path_TLV* pTlvIter = gMap.Get_First_TLV_For_Offsetted_Camera(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->mTlvFlags.Set(relive::TlvFlags::eBit1_Created);
                pTlvIter->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);
                // AO OG bug tlvId not recalculated??
                relive_new Slig(static_cast<relive::Path_Slig*>(pTlvIter), tlvId);
                return;
            }
        }
    }
}

static void Factory_RollingBallStopper(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new RollingBallStopper(static_cast<relive::Path_RollingBallStopper*>(pTlv), tlvId);
    }
}

static void Factory_FootSwitch(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new FootSwitch(static_cast<relive::Path_FootSwitch*>(pTlv), tlvId);
    }
}

static void Factory_SecurityClaw(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pSecurityClawTlv = static_cast<relive::Path_SecurityClaw*>(pTlv);
    const auto disabledResources = pSecurityClawTlv->mDisabledResources;

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        if (!(disabledResources & 1))
        {
        }

        if (!(disabledResources & 2))
        {
        }

        if (!(disabledResources & 0x10))
        {
        }

        if (!(disabledResources & 0x20))
        {
        }

        relive_new SecurityClaw(pSecurityClawTlv, tlvId);
    }
}


static void Factory_MotionDector(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new MotionDetector(static_cast<relive::Path_MotionDetector*>(pTlv), tlvId);
    }
}

static void Factory_SligSpawner(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_SligSpawner*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->mData.mDisabledResourcesAO);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        relive_new SligSpawner(pTlv, &pSligTlv->mData, tlvId);
    }
}

static void Factory_ElectricWall(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ElectricWall(static_cast<relive::Path_ElectricWall*>(pTlv), tlvId);
    }
}


static void Factory_LiftMover(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new LiftMover(static_cast<relive::Path_LiftMover*>(pTlv), tlvId);
    }
}


static void Factory_ChimeLock(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ChimeLock(static_cast<relive::Path_ChimeLock*>(pTlv), tlvId);
    }
}

static void Factory_Elum(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        Elum::Spawn(tlvId);
        gElum->mXPos = FP_FromInteger(pTlv->mTopLeftX);
        gElum->mYPos = FP_FromInteger(pTlv->mTopLeftY);
    }
}


static void Factory_MeatSack(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new MeatSack(static_cast<relive::Path_MeatSack*>(pTlv), tlvId);
    }
}


static void Factory_Scrab(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        return;
    }
    relive_new Scrab(static_cast<relive::Path_Scrab*>(pTlv), tlvId);
}


static void Factory_FlintLockFire(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new FlintLockFire(static_cast<relive::Path_FlintLockFire*>(pTlv), tlvId);
    }
}

static void Factory_SligBoundRight(relive::Path_TLV* pTlv, Map* pMap, const Guid& tlvId, LoadMode loadMode)
{
    Factory_SligBoundLeft(pTlv, pMap, tlvId, loadMode);
}

static void Factory_InvisibleSwitch(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new InvisibleSwitch(static_cast<relive::Path_InvisibleSwitch*>(pTlv), tlvId);
    }
}

static void Factory_WorkerMudokon(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pMudTlv = static_cast<relive::Path_Mudokon*>(pTlv);
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        switch (pMudTlv->mJob)
        {
            case relive::Path_Mudokon::MudJobs::eStandScrub:
                break;

            case relive::Path_Mudokon::MudJobs::eSitChant:
                break;
        }

        relive_new Mudokon(pTlv, tlvId);
    }
    else
    {
        switch (pMudTlv->mJob)
        {
            case relive::Path_Mudokon::MudJobs::eStandScrub:
            {
            }
            break;

            case relive::Path_Mudokon::MudJobs::eSitScrub:
            {
            }
            break;

            case relive::Path_Mudokon::MudJobs::eSitChant:
            {
            }
            break;
        }
    }
}

static void Factory_DoorFlame(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new DoorFlame(static_cast<relive::Path_DoorFlame*>(pTlv), tlvId);
    }
}

static void Factory_MovingBomb(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<relive::Path_MovingBomb*>(pTlv);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        if (!(pMovingBombTlv->mDisabledResources & 1))
        {
        }

        relive_new MovingBomb(pMovingBombTlv, tlvId);
    }
}

static void Factory_MeatSaw(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new MeatSaw(static_cast<relive::Path_MeatSaw*>(pTlv), tlvId);
    }
}

static void Factory_MainMenuController(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (gMainMenuInstanceCount == 0)
    {
        if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
        {
        }
        else
        {
            relive_new Menu(pTlv, tlvId);
        }
    }
}

static void Factory_HintFly(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new HintFly(static_cast<relive::Path_HintFly*>(pTlv), tlvId);
    }
}

static void Factory_TimerTrigger(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new TimerTrigger(static_cast<relive::Path_TimerTrigger*>(pTlv), tlvId);
    }
}


static void Factory_SecurityDoor(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new SecurityDoor(static_cast<relive::Path_SecurityDoor*>(pTlv), tlvId);
    }
}

static void Factory_BoomMachine(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pBoomMachineTlv = static_cast<relive::Path_BoomMachine*>(pTlv);
    const auto disabledResources = pBoomMachineTlv->mDisabledResources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        return;
    }

    if (!(disabledResources & 2))
    {
    }

    relive_new BoomMachine(pBoomMachineTlv, tlvId);
}


static void Factory_LCDScreen(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new LCDScreen(static_cast<relive::Path_LCDScreen*>(pTlv), tlvId);
    }
}

static void Factory_HandStone(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}


static void Factory_CreditsController(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        if (!gCreditsControllerExists)
        {
            relive_new CreditsController(static_cast<relive::Path_CreditsController*>(pTlv), tlvId);
        }
    }
}

static void Factory_LCDStatusBoard(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new LCDStatusBoard(static_cast<relive::Path_LCDStatusBoard*>(pTlv), tlvId);
    }
}


static void Factory_SwitchStateBooleanLogic(relive::Path_TLV* /*pTlv*/, Map* /*pMap*/, const Guid& /*tlvId*/, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        LOG_WARNING("Factory_SwitchStateBooleanLogic_487B80 tlv converter not implemented");
        //relive_new SwitchStateBooleanLogic(static_cast<relive::Path_SwitchStateBooleanLogic*>(pTlv), tlvId);
    }
}

static void Factory_MusicTrigger(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new MusicTrigger(static_cast<relive::Path_MusicTrigger*>(pTlv), tlvId);
    }
}

static void Factory_LightEffect(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    auto pPathLightTlv = static_cast<relive::Path_LightEffect*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (pPathLightTlv->mType)
        {
            case relive::Path_LightEffect::Type::Star:
                break;

            case relive::Path_LightEffect::Type::GoldGlow:
                break;

            case relive::Path_LightEffect::Type::GreenGlow:
                break;

            case relive::Path_LightEffect::Type::FlintGlow:
                break;

            case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
                break;

            case relive::Path_LightEffect::Type::Switchable_RedGreenHubLight:
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
                relive_new LightEffect(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::GoldGlow:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::GreenGlow:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::FlintGlow:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
            case relive::Path_LightEffect::Type::Switchable_RedGreenHubLight:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            default:
                return;
        }
    }
}


static void Factory_SlogSpawner(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new SlogSpawner(static_cast<relive::Path_SlogSpawner*>(pTlv), tlvId);
    }
}


static void Factory_GasCountDown(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new GasCountDown(static_cast<relive::Path_GasCountDown*>(pTlv), tlvId);
    }
}



static void Factory_RingCancel(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pRingCancel = static_cast<relive::Path_RingCancel*>(pTlv);
        if (pRingCancel->mRemovesShrykull)
        {
            if (sActiveHero->field_168_ring_pulse_timer)
            {
                if (sActiveHero->field_16C_bHaveShrykull)
                {
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
        Path::TLV_Reset(tlvId, -1, 0, 0);
    }
}


static void Factory_GasEmitter(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new GasEmitter(static_cast<relive::Path_GasEmitter*>(pTlv), tlvId);
    }
}


static void Factory_ZzzSpawner(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        relive_new ZzzSpawner(static_cast<relive::Path_ZzzSpawner*>(pTlv), tlvId);
    }
}


static void Factory_BackgroundGlukkon(relive::Path_TLV* pTlv, Map* /*pMap*/, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new BackgroundGlukkon(static_cast<relive::Path_BackgroundGlukkon*>(pTlv), tlvId);
    }
}


static void Factory_KillUnsavedMuds(relive::Path_TLV* /*pTlv*/, Map* pMap, const Guid& tlvId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        // OG bug fix - added an extra check that checks if the map has changed
        // which prevents that the killed mudokon count becomes inaccurate or even negative.
        if (!gbKillUnsavedMudsDone &&
            pMap->field_DA_bMapChanged)
        {
            gbKillUnsavedMudsDone = true;
            sKilledMudokons = 28 - sRescuedMudokons;
            Path::TLV_Reset(tlvId, -1, 0, 1);
        }
    }
}

void ConstructTLVObject(relive::Path_TLV* pTlv, Map* pMap, const Guid& tlvInfo, LoadMode loadMode)
{
    switch (pTlv->mTlvType)
    {
        case ReliveTypes::eKillUnsavedMuds:
            Factory_KillUnsavedMuds(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBackgroundAnimation:
            Factory_BackgroundAnimation(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBat:
            Factory_Bat(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLiftMover:
            Factory_LiftMover(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTimedMine:
            Factory_TimedMine(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eChimeLock:
            Factory_ChimeLock(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eGasCountDown:
            Factory_GasCountDown(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDoor:
            Factory_Door(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eElectricWall:
            Factory_ElectricWall(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eElum:
            Factory_Elum(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBellHammer:
            Factory_BellHammer(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSecurityClaw:
            Factory_SecurityClaw(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFlintLockFire:
            Factory_FlintLockFire(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBoomMachine:
            Factory_BoomMachine(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBackgroundGlukkon:
            Factory_BackgroundGlukkon(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHoneySack:
            Factory_HoneySack(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHoney:
            Factory_Honey(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMudokon:
            Factory_WorkerMudokon(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLiftPoint:
            Factory_LiftPoint(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSecurityOrb:
            Factory_SecurityOrb(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMeatSack:
            Factory_MeatSack(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMeatSaw:
            Factory_MeatSaw(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMine:
            Factory_Mine(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMotionDetector:
            Factory_MotionDector(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRollingBallStopper:
            Factory_RollingBallStopper(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eParamite:
            Factory_Paramite(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBirdPortal:
            Factory_BirdPortal(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::ePullRingRope:
            Factory_PullRingRope(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRockSack:
            Factory_RockSack(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRollingBall:
            Factory_RollingBall(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eScrab:
            Factory_Scrab(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlig:
            Factory_Slig(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlog:
            Factory_Slog(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::SlingMud:
            Factory_SlingMud(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligSpawner:
            Factory_SligSpawner(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eZBall:
            Factory_ZBall(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLever:
            Factory_Lever(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTrapDoor:
            Factory_TrapDoor(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eUXB:
            Factory_Uxb(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMusicTrigger:
            Factory_MusicTrigger(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eWellLocal:
        case ReliveTypes::eWellExpress:
            Factory_Well(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eZzzSpawner:
            Factory_ZzzSpawner(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHoist:
            Factory_Hoist(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eEdge:
            Factory_Edge(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eShadowZone:
            Factory_ShadowZone(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDove:
            Factory_Dove(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFallingItem:
            Factory_FallingItem(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLiftMudokon:
            Factory_LiftMud(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBeeSwarmHole:
            Factory_BeeSwarmHole(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eAbeStart:
            Factory_AbeStart(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eElumWall:
            Factory_ElumWall(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBeeNest:
            Factory_BeeNest(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRingMudokon:
            Factory_RingMud(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFootSwitch:
            Factory_FootSwitch(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligBoundRight:
            Factory_SligBoundRight(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligBoundLeft:
            Factory_SligBoundLeft(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eGasEmitter:
            Factory_GasEmitter(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRingCancel:
            Factory_RingCancel(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlogSpawner:
            Factory_SlogSpawner(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLightEffect:
            Factory_LightEffect(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSwitchStateBooleanLogic:
            Factory_SwitchStateBooleanLogic(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLCDStatusBoard:
            Factory_LCDStatusBoard(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eCreditsController:
            Factory_CreditsController(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHandStone:
            Factory_HandStone(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSecurityDoor:
            Factory_SecurityDoor(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHintFly:
            Factory_HintFly(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMainMenuController:
            Factory_MainMenuController(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMovingBomb:
            Factory_MovingBomb(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDoorFlame:
            Factory_DoorFlame(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eInvisibleSwitch:
            Factory_InvisibleSwitch(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTimerTrigger:
            Factory_TimerTrigger(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLCDScreen:
            Factory_LCDScreen(pTlv, pMap, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBellSongStone:
            Factory_BellSongStone(pTlv, pMap, tlvInfo, loadMode);
            break;
    }
}


} // namespace AO
