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

EXPORT void Factory_ContinuePoint_Null_4817D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_PathTransition_Null_481720(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_ContinueZone_Null_481830(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Hoist_487230(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbehoistAOResID, loadMode);
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::LoadResource_446C90("DRPSPRK.BAN", ResourceManager::Resource_Animation, AOResourceID::kHoistRocksAOResID, loadMode);
                break;

            case LevelIds::eForest_3:
            case LevelIds::eDesert_8:
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
        auto pHoistTlv = static_cast<Path_Hoist*>(pTlv);
        if (pHoistTlv->field_18_hoist_type == Path_Hoist::Type::eOffScreen)
        {
            auto pHoistRocksEffect = ao_new<HoistRocksEffect>();
            if (pHoistRocksEffect)
            {
                pHoistRocksEffect->ctor_431820(pHoistTlv, tlvOffsetLevelIdPathId.all);
            }
            // OG issue, no reset on failure ??
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Edge_481730(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbehoistAOResID, loadMode, 0);

        if (gMap_507BA8.field_0_current_level == LevelIds::eForest_3 || gMap_507BA8.field_0_current_level == LevelIds::eDesert_8)
        {
            ResourceManager::LoadResource_446C90("ANEEDGE.BAN", ResourceManager::Resource_Animation, AOResourceID::kAneedgeAOResID, loadMode, 0);
        }
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_DeathDrop_Null_4817B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Door_481C80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kF2p3dorAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kHubdoorAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kRockdoorAOResID}};
        ResourceManager::LoadResource_446C90("ABEDOOR.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbedoorAOResID, loadMode);
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eBoardRoom_12:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::LoadResourcesFromList_446E80("RDOOR.BND", kResources.AsList(), loadMode, 0);
                break;

            case LevelIds::eLines_2:
                ResourceManager::LoadResourcesFromList_446E80("LDOOR.BND", kResources.AsList(), loadMode, 0);
                break;

            case LevelIds::eDesert_8:
            case LevelIds::eDesertTemple_9:
            case LevelIds::eDesertEscape_15:
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

        auto pDoor = ao_new<Door>();
        if (pDoor)
        {
            pDoor->ctor_40E010(static_cast<Path_Door*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ShadowZone_482080(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pShadowZone = ao_new<ShadowZone>();
        if (pShadowZone)
        {
            pShadowZone->ctor_435D30(static_cast<Path_ShadowZone*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
        }
    }
}



EXPORT void Factory_LiftPoint_4820F0(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    static CompileTimeResourceList<3> kResources = {
        {ResourceManager::Resource_Animation, AOResourceID::kAbeliftAOResID},
        {ResourceManager::Resource_Animation, AOResourceID::kLiftWheelsAOResID},
        {ResourceManager::Resource_Animation, AOResourceID::kLiftAOResID}};

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eBoardRoom_12:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::LoadResource_446C90("R1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("RLIFT.BND", kResources.AsList(), loadMode, 0);
                break;

            case LevelIds::eLines_2:
                ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("LLIFT.BND", kResources.AsList(), loadMode, 0);
                break;

            case LevelIds::eDesert_8:
                ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("D1LIFT.BND", kResources.AsList(), loadMode, 0);
                break;

            case LevelIds::eDesertTemple_9:
            case LevelIds::eDesertEscape_15:
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
        for (s32 idx = 0; idx < gBaseGameObject_list_9F2DF0->Size(); idx++)
        {
            BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(idx);
            if (!pObjIter)
            {
                break;
            }

            if (!pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pObjIter->field_4_typeId == Types::eLiftPoint_51)
            {
                auto pLiftObj = static_cast<LiftPoint*>(pObjIter);

                const s16 xpos_i = FP_GetExponent(pLiftObj->field_A8_xpos);
                if (pTlv->field_10_top_left.field_0_x <= xpos_i
                    && xpos_i <= pTlv->field_14_bottom_right.field_0_x
                    && pLiftObj->field_B2_lvl_number == gMap_507BA8.field_0_current_level
                    && pLiftObj->field_B0_path_number == gMap_507BA8.field_2_current_path)
                {
                    gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
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

        if (pTlv->field_1_unknown & 2 || (pTlv->field_1_unknown == 0 && static_cast<Path_LiftPoint*>(pTlv)->field_1A_bstart_point == Choice_short::eYes_1))
        {
            auto pLiftPoint = ao_new<LiftPoint>();
            if (pLiftPoint)
            {
                pLiftPoint->ctor_434710(static_cast<Path_LiftPoint*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            Path_TLV* pTlvIter = nullptr;
            s16 pointNumber = 1;
            while (pointNumber < 8)
            {
                pTlvIter = gMap_507BA8.Get_First_TLV_For_Offsetted_Camera_4463B0(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));
                while (pTlvIter)
                {
                    if (pTlvIter->field_4_type == TlvTypes::LiftPoint_8)
                    {
                        const auto tlv_x = pTlv->field_10_top_left.field_0_x;
                        const auto absX = pTlvIter->field_10_top_left.field_0_x - tlv_x >= 0 ? pTlvIter->field_10_top_left.field_0_x - tlv_x : tlv_x - pTlvIter->field_10_top_left.field_0_x;
                        if (absX < 5)
                        {
                            if (pTlvIter->field_1_unknown & 2 || (pTlvIter->field_1_unknown == 0 && static_cast<Path_LiftPoint*>(pTlvIter)->field_1A_bstart_point == Choice_short::eYes_1))
                            {
                                auto pLiftPoint = ao_new<LiftPoint>();
                                if (pLiftPoint)
                                {
                                    pLiftPoint->ctor_434710(static_cast<Path_LiftPoint*>(pTlvIter), pMap, tlvOffsetLevelIdPathId.all);
                                }
                                return;
                            }
                        }
                    }
                    pTlvIter = Path_TLV::Next_446460(pTlvIter);
                }
                pointNumber++;
            }

            auto pLiftPoint = ao_new<LiftPoint>();
            if (pLiftPoint)
            {
                pLiftPoint->ctor_434710(static_cast<Path_LiftPoint*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
            }
        }
    }
}


EXPORT void Factory_Null_482E70(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_483D90(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_WellExpress_483340(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        const FP xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
        const FP ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
        auto pWell = ao_new<Well>();
        if (pWell)
        {
            pWell->ctor_48AEE0(static_cast<Path_WellBase*>(pTlv), xpos, ypos, tlvOffsetLevelIdPathId.all);
            return;
        }
    }
}


EXPORT void Factory_Dove_4834C0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDovbasicAOResID);

        auto pDoveTlv = static_cast<Path_Dove*>(pTlv);

        const s16 width = pDoveTlv->field_14_bottom_right.field_0_x - pDoveTlv->field_10_top_left.field_0_x;
        const s16 height = pDoveTlv->field_14_bottom_right.field_2_y - pDoveTlv->field_10_top_left.field_2_y;

        for (s32 i = 0; i < pDoveTlv->field_18_dove_count; i++)
        {
            auto pDove = ao_new<Dove>();
            if (pDove)
            {
                const AnimRecord& doveRec = AO::AnimRec(AnimId::Dove_Idle);
                pDove->ctor_40EE50(
                    doveRec.mFrameTableOffset,
                    doveRec.mMaxW,
                    doveRec.mMaxH,
                    doveRec.mResourceId,
                    tlvOffsetLevelIdPathId.all,
                    pDoveTlv->field_1C_scale != Scale_short::eFull_0 ? FP_FromDouble(0.5) : FP_FromInteger(1));
            }

            s16 ypos = 0;
            if (pDoveTlv->field_1A_pixel_perfect == Choice_short::eYes_1)
            {
                pDove->field_A8_xpos = FP_FromInteger(pDoveTlv->field_10_top_left.field_0_x);
                ypos = pDoveTlv->field_10_top_left.field_2_y;
            }
            else
            {
                pDove->field_A8_xpos = FP_FromInteger(pDoveTlv->field_10_top_left.field_0_x + width * Math_NextRandom() / 256);
                ypos = pDoveTlv->field_10_top_left.field_2_y + height * Math_NextRandom() / 256;
            }

            pDove->field_AC_ypos = FP_FromInteger(ypos) + FP_FromInteger(10);
        }
    }
}


EXPORT void Factory_RockSack_483680(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources = {
            {ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kAberockAOResID}};

        ResourceManager::LoadResourcesFromList_446E80("RTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, AOResourceID::kP2c2bagAOResID, loadMode);
            ResourceManager::LoadResource_446C90("EPUIROCK.BAN", ResourceManager::Resource_Palt, AOResourceID::kAberockAOResID, loadMode);
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kP2c2bagAOResID);
        auto pRockSack = ao_new<RockSack>();
        if (pRockSack)
        {
            pRockSack->ctor_4573F0(static_cast<Path_RockSack*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ZBall_483890(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        if (gMap_507BA8.field_0_current_level == LevelIds::eForestTemple_4)
        {
            ResourceManager::LoadResource_446C90("F2ZBALL.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2zballAOResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEKNOKZ.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeknokzAOResID, loadMode);
        }
    }
    else
    {
        auto pZBall = ao_new<ZBall>();
        if (pZBall)
        {
            pZBall->ctor_478590(static_cast<Path_ZBall*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_FallingItem_483940(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::LoadResource_446C90("R1BARREL.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("R1MEAT.BAN", ResourceManager::Resource_Animation, AOResourceID::kMeatGibAOResID_6014, loadMode);
                break;

            case LevelIds::eLines_2:
            case LevelIds::eBoardRoom_12:
                ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, loadMode);
                break;

            case LevelIds::eStockYards_5:
                ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2rockAOResID, loadMode);
                ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, AOResourceID::kStickGibAOResID, loadMode);
                ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);
                ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, loadMode);
                break;

            case LevelIds::eDesert_8:
            case LevelIds::eDesertTemple_9:
            case LevelIds::eDesertEscape_15:
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
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMeatGibAOResID_6014);
                break;

            case LevelIds::eLines_2:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID);
                break;

            case LevelIds::eStockYards_5:
            case LevelIds::eDesert_8:
            case LevelIds::eDesertTemple_9:
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

        auto pFallingItem = ao_new<FallingItem>();
        if (pFallingItem)
        {
            pFallingItem->ctor_419F30(static_cast<Path_FallingItem*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Null_482E90(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_482E80(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_PullRingRope_483DA0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eBoardRoom_12:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::LoadResource_446C90("R1ROPES.BAN", ResourceManager::Resource_Animation, AOResourceID::kRopesAOResID, loadMode);
                ResourceManager::LoadResource_446C90("R1PULRNG.BAN", ResourceManager::Resource_Animation, AOResourceID::kPullringAOResID, loadMode);
                break;

            case LevelIds::eDesert_8:
            case LevelIds::eDesertTemple_9:
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

        auto pPullRingRope = ao_new<PullRingRope>();
        if (pPullRingRope)
        {
            pPullRingRope->ctor_4546B0(static_cast<Path_PullRingRope*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_BackgroundAnimation_4840A0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pBackgroundAnimation = ao_new<BackgroundAnimation>();
        if (pBackgroundAnimation)
        {
            pBackgroundAnimation->ctor_405A90(static_cast<Path_BackgroundAnimation*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Honey_4844A0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pHoney = ao_new<Honey>();
        if (pHoney)
        {
            const auto midPoint = (pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2;
            pHoney->ctor_431E30(
                FP_FromInteger(midPoint + pTlv->field_10_top_left.field_0_x),
                FP_FromInteger(pTlv->field_10_top_left.field_2_y + 24));

            pHoney->field_E4_tlvInfo = tlvOffsetLevelIdPathId.all;
        }
    }
}


EXPORT void Factory_Null_484640(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_TimedMine_484650(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pTimedMineTlv = static_cast<Path_TimedMine*>(pTlv);
    const auto disabledResource = pTimedMineTlv->field_20_disable_resources;
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, disabledResource & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, disabledResource & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, disabledResource & 4);

        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
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

        auto pTimedMine = ao_new<TimedMine>();
        if (pTimedMine)
        {
            pTimedMine->ctor_4083F0(pTimedMineTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Null_482EA0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
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

        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
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

EXPORT void Factory_Slig_482EC0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->field_50_disable_resources);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing
    }
    else
    {
        auto pSlig = ao_new<Slig>();
        if (pSlig)
        {
            pSlig->ctor_464D40(pSligTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Slog_485030(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pSlog = ao_new<Slog>();
        if (pSlog)
        {
            pSlog->ctor_472EE0(static_cast<Path_Slog*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Switch_485370(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eStockYards_5:
            case LevelIds::eStockYardsReturn_6:
            case LevelIds::eBoardRoom_12:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::LoadResource_446C90("R1SWITCH.BAN", ResourceManager::Resource_Animation, AOResourceID::kLeverAOResID, loadMode);
                break;

            case LevelIds::eLines_2:
                ResourceManager::LoadResource_446C90("L1SWITCH.BAN", ResourceManager::Resource_Animation, AOResourceID::kLeverAOResID, loadMode);
                break;

            case LevelIds::eDesert_8:
            case LevelIds::eDesertTemple_9:
            case LevelIds::eDesertEscape_15:
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
        auto pSwitch = ao_new<Lever>();
        if (pSwitch)
        {
            pSwitch->ctor_481110(static_cast<Path_Lever*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_BellHammer_4854B0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kHammerAOResID);
        auto pBellHammer = ao_new<BellHammer>();
        if (pBellHammer)
        {
            pBellHammer->ctor_405010(static_cast<Path_BellHammer*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_StartController_Null_4817E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_SecurityOrb_485550(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSecurityOrbTlv = static_cast<Path_SecurityOrb*>(pTlv);
    const auto disabledResources = pSecurityOrbTlv->field_1A_disable_resources;
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

        auto pSecurityEye = ao_new<SecurityOrb>();
        if (pSecurityEye)
        {
            pSecurityEye->ctor_436C80(pSecurityOrbTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Null_4817F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_487070(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_LiftMud_4857D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pMud = ao_new<Mudokon>();
        if (pMud)
        {
            pMud->ctor_43EED0(pTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Null_482EB0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_BeeSwarmHole_485E20(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEWASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbewaspAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("WASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eForest_3 || gMap_507BA8.field_0_current_level == LevelIds::eDesert_8)
        {
            ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, loadMode, 0);
        }
    }
    else
    {
        auto pBeeSwarmHole = ao_new<BeeSwarmHole>();
        if (pBeeSwarmHole)
        {
            pBeeSwarmHole->ctor_4782B0(static_cast<Path_BeeSwarmHole*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Pulley_Null_481800(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_HoneySack_485EF0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pHoneySack = ao_new<HoneySack>();
        if (pHoneySack)
        {
            pHoneySack->ctor_42BD10(static_cast<Path_HoneySack*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_AbeStart_486050(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2 && !gAttract_507698)
    {
        if (!pPauseMenu_5080E0)
        {
            pPauseMenu_5080E0 = ao_new<PauseMenu>();
            if (pPauseMenu_5080E0)
            {
                pPauseMenu_5080E0->ctor_44DEA0();
            }
        }

        if (!sActiveHero_507678)
        {
            sActiveHero_507678 = ao_new<Abe>();
            if (sActiveHero_507678)
            {
                sActiveHero_507678->ctor_420770(55888, 85, 57, 55);
                sActiveHero_507678->field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
                sActiveHero_507678->field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
            }
        }
    }
}


EXPORT void Factory_ElumStart_Null_481810(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_486140(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_ElumWall_487370(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ELMKNBK.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmnknbkAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ANEKNBK.BAN", ResourceManager::Resource_Animation, AOResourceID::kAneknbkAOResID, loadMode, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_SlingMud_485A30(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto kResources = {
            AOResourceID::kSlingmudAOResID_508,
            AOResourceID::kSparksAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        auto pSlingMud = ao_new<SlingMudokon>();
        if (pSlingMud)
        {
            pSlingMud->ctor_46F940(static_cast<Path_SlingMudokon*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_HoneyDrip_Null_481820(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}

EXPORT void Factory_BeeNest_486150(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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
        auto pBeeNest = ao_new<BeeNest>();
        if (pBeeNest)
        {
            pBeeNest->ctor_480E20(static_cast<Path_BeeNest*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Null_487080(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Well_4834A0(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    Factory_WellExpress_483340(pTlv, pMap, tlvOffsetLevelIdPathId, loadMode);
}


EXPORT void Factory_Mine_4848D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pMineTlv = static_cast<Path_Mine*>(pTlv);
    const auto disabledResources = pMineTlv->field_1E_disabled_resources;

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, disabledResources & 4);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
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

        auto pMine = ao_new<Mine>();
        if (pMine)
        {
            pMine->ctor_43A330(pMineTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Uxb_484B70(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pUxbTlv = static_cast<Path_UXB*>(pTlv);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, pUxbTlv->field_20_disabled_resources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, loadMode, pUxbTlv->field_20_disabled_resources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, pUxbTlv->field_20_disabled_resources & 4);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
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

        if (!(pUxbTlv->field_20_disabled_resources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        if (!(pUxbTlv->field_20_disabled_resources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID);
        }

        auto pUxb = ao_new<UXB>();
        if (pUxb)
        {
            pUxb->ctor_488C80(pUxbTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Paramite_4861F0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

    auto pParamite = ao_new<Paramite>();
    if (pParamite)
    {
        pParamite->ctor_44A7A0(static_cast<Path_Paramite*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


EXPORT void Factory_Bat_486630(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("BATBASIC.BAN", ResourceManager::Resource_Animation, AOResourceID::kBatBasicAOResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kBatBasicAOResID);

        auto pBat = ao_new<Bat>();
        if (pBat)
        {
            pBat->ctor_4046E0(static_cast<Path_Bat*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_RingMud_4858F0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pMud = ao_new<Mudokon>();
        if (pMud)
        {
            pMud->ctor_43EED0(pTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_MovieStone_Null_487430(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_BirdPortal_486710(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<Path_BirdPortal*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources1 = {
            {ResourceManager::Resource_Animation, AOResourceID::kPortalTerminatorAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kPortliteAOResID},
            {ResourceManager::Resource_Animation, AOResourceID::kPortlitAOResID}};
        ResourceManager::LoadResourcesFromList_446E80("PORTAL.BND", kResources1.AsList(), loadMode, 0);

        if (pBirdPortalTlv->field_24_portal_type == PortalType::eShrykull_2)
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

        if (pBirdPortalTlv->field_24_portal_type != PortalType::eShrykull_2)
        {
            auto pBirdPortal = ao_new<BirdPortal>();
            if (pBirdPortal)
            {
                pBirdPortal->ctor_4520A0(pBirdPortalTlv, tlvOffsetLevelIdPathId.all);
            }
            return;
        }

        auto kResources2 = {
            AOResourceID::kAbemorphAOResID,
            AOResourceID::kShrmorphAOResID,
            AOResourceID::kSplineAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources2);

        auto pBirdPortal = ao_new<BirdPortal>();
        if (pBirdPortal)
        {
            pBirdPortal->ctor_4520A0(pBirdPortalTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_BirdPortalExit_null_486700(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_BellSong_487450(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_TrapDoor_4868E0(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
            case LevelIds::eRuptureFarms_1:
            case LevelIds::eBoardRoom_12:
            case LevelIds::eRuptureFarmsReturn_13:
                ResourceManager::LoadResource_446C90("R1TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
            case LevelIds::eLines_2:
                ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
            case LevelIds::eStockYards_5:
                ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, loadMode, 0);
                break;
            case LevelIds::eDesert_8:
            case LevelIds::eDesertTemple_9:
            case LevelIds::eDesertEscape_15:
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

        auto pTrapDoor = ao_new<TrapDoor>();
        if (pTrapDoor)
        {
            pTrapDoor->ctor_488010(static_cast<Path_TrapDoor*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_RollingBall_486A60(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pRollingBall = ao_new<RollingBall>();
        if (pRollingBall)
        {
            pRollingBall->ctor_4578C0(static_cast<Path_RollingBall*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

static Path_TLV* FindMatchingSligTLV(Path_TLV* pTlvIter, Path_SligBound* pTlv)
{
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::Slig_24 && pTlv->field_18_slig_id == static_cast<Path_Slig*>(pTlvIter)->field_40_slig_bound_id && !pTlvIter->field_0_flags.Get(TLV_Flags::eBit2_Destroyed))
        {
            return pTlvIter;
        }
        pTlvIter = Path_TLV::Next_446460(pTlvIter);
    }
    return nullptr;
}

EXPORT void Factory_SligLeftBound_482520(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pBound = static_cast<Path_SligBound*>(pTlv);
    LoadWalkingSligResources(loadMode, pBound->field_1A_disabled_resources);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        pBound->field_0_flags.Clear(TLV_Flags::eBit1_Created);
        pBound->field_0_flags.Clear(TLV_Flags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            Path_TLV* pTlvIter = gMap_507BA8.Get_First_TLV_For_Offsetted_Camera_4463B0(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit1_Created);
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit2_Destroyed);
                auto pSlig = ao_new<Slig>();
                if (pSlig)
                {
                    // AO OG bug tlvOffsetLevelIdPathId not recalculated??
                    pSlig->ctor_464D40(static_cast<Path_Slig*>(pTlvIter), tlvOffsetLevelIdPathId.all);
                }
                return;
            }
        }
    }
}

EXPORT void Factory_InvisibleZone_Null_481840(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}

EXPORT void Factory_RollingBallStopper_486B90(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("F2STNSCK.BAN", ResourceManager::Resource_Animation, AOResourceID::kF2stnsckAOResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kF2stnsckAOResID);
        auto pRollingBallStopper = ao_new<RollingBallStopper>();
        pRollingBallStopper->ctor_43BCE0(static_cast<Path_RollingBallStopper*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

EXPORT void Factory_FootSwitch_486C60(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("TRIGGER.BAN", ResourceManager::Resource_Animation, AOResourceID::kTriggerAOResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kTriggerAOResID);

        auto pFootSwitch = ao_new<FootSwitch>();
        if (pFootSwitch)
        {
            pFootSwitch->ctor_4887F0(static_cast<Path_FootSwitch*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_SecurityClaw_486D50(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSecurityClawTlv = static_cast<Path_SecurityClaw*>(pTlv);
    const auto disabledResources = pSecurityClawTlv->field_1E_disabled_resources;

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

        auto pSecurityOrb = ao_new<SecurityClaw>();
        if (pSecurityOrb)
        {
            pSecurityOrb->ctor_418A70(pSecurityClawTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_MotionDector_486FD0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kDeathFlareAOResID);

        auto pMotionDetector = ao_new<MotionDetector>();
        if (pMotionDetector)
        {
            pMotionDetector->ctor_437A50(static_cast<Path_MotionDetector*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Null_4870A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_481C70(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_4870B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void CC Factory_SligSpawner_482A00(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->field_50_disable_resources);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        auto pSligSpawner = ao_new<SligSpawner>();
        if (pSligSpawner)
        {
            pSligSpawner->ctor_402850(pSligTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_ElectricWall_4874E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kElecwallAOResID);

        auto pElectricWall = ao_new<ElectricWall>();
        if (pElectricWall)
        {
            pElectricWall->ctor_40FCF0(static_cast<Path_ElectricWall*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_LiftMover_487580(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pLiftMover = ao_new<LiftMover>();
        if (pLiftMover)
        {
            pLiftMover->ctor_4054E0(static_cast<Path_LiftMover*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ChimeLock_4870D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto kResources = {
            AOResourceID::kChimeAOResID,
            AOResourceID::kF2ChimeBallAOResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        auto pChimeLock = ao_new<ChimeLock>();
        if (pChimeLock)
        {
            pChimeLock->ctor_40AB20(static_cast<Path_ChimeLock*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_ElumStart_Unknown_4873D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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
        Elum::Spawn_410E90(tlvOffsetLevelIdPathId);
        gElum_507680->field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
        gElum_507680->field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    }
}


EXPORT void Factory_MeatSack_483790(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        static CompileTimeResourceList<3> kResources(
            {{ResourceManager::Resource_Animation, AOResourceID::kAbepickAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kAbethrowAOResID},
             {ResourceManager::Resource_Animation, AOResourceID::kMeatAOResID}});

        ResourceManager::LoadResourcesFromList_446E80("MTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, AOResourceID::kP2c2bagAOResID, loadMode);
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMeatSackAOResID);

        auto pMeatSack = ao_new<MeatSack>();
        if (pMeatSack)
        {
            pMeatSack->ctor_4390F0(static_cast<Path_MeatSack*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Scrab_4863E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

    auto pScrab = ao_new<Scrab>();
    if (pScrab)
    {
        pScrab->ctor_45B5F0(static_cast<Path_Scrab*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


EXPORT void Factory_FlintLockFire_487640(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pFlintlockFire = ao_new<FlintLockFire>();
        if (pFlintlockFire)
        {
            pFlintlockFire->ctor_41AA90(static_cast<Path_FlintLockFire*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ScrabLeftBound_Null_481860(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_ScrabRightBound_Null_481870(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_SligRightBound_4829D0(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    Factory_SligLeftBound_482520(pTlv, pMap, tlvOffsetLevelIdPathId, loadMode);
}


EXPORT void Factory_SligPersist_Null_4829F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_4870C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_EnemyStopper_Null_481880(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_Null_4871C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_InvisibleSwitch_481C10(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pInvisibleSwitch = ao_new<InvisibleSwitch>();
        if (pInvisibleSwitch)
        {
            pInvisibleSwitch->ctor_4334E0(static_cast<Path_InvisibleSwitch*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_WorkerMud_485B20(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pMudTlv = static_cast<Path_Mudokon*>(pTlv);
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

        switch (pMudTlv->field_1A_job)
        {
            case Path_Mudokon::MudJobs::eStandScrub_0:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMudchslAOResID);
                break;

            case Path_Mudokon::MudJobs::eSitChant_2:
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kMudltusAOResID);
                break;
        }

        auto pMud = ao_new<Mudokon>();
        if (pMud)
        {
            pMud->ctor_43EED0(pTlv, tlvOffsetLevelIdPathId.all);
        }
    }
    else
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbebsic1AOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBSIC.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbebasicAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNFD.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeknfdAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNBK.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeknbkAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEEDGE.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeedgeAOResID, loadMode);
        ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kObjectShadowAOResID, loadMode);

        switch (pMudTlv->field_1A_job)
        {
            case Path_Mudokon::MudJobs::eStandScrub_0:
            {
                static CompileTimeResourceList<3> kResources = {
                    {ResourceManager::Resource_Animation, AOResourceID::kMudoduckAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudscrubAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudbtlnkAOResID}};
                ResourceManager::LoadResource_446C90("MUDCHSL.BAN", ResourceManager::Resource_Animation, AOResourceID::kMudchslAOResID, loadMode);
                ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
            }
            break;

            case Path_Mudokon::MudJobs::eSitScrub_1:
            {
                static CompileTimeResourceList<3> kResources = {
                    {ResourceManager::Resource_Animation, AOResourceID::kMudoduckAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudscrubAOResID},
                    {ResourceManager::Resource_Animation, AOResourceID::kMudbtlnkAOResID}};
                ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
            }
            break;

            case Path_Mudokon::MudJobs::eSitChant_2:
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


EXPORT void Factory_SligZCover_Null_481850(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_DoorFlame_481E80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pDoorFlame = ao_new<DoorFlame>();
        if (pDoorFlame)
        {
            pDoorFlame->ctor_432860(static_cast<Path_DoorFlame*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_OneShotSwitchIdSetter_4871D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pSwitchStates = ao_new<OneShotSwitchIdSetter>();
        if (pSwitchStates)
        {
            pSwitchStates->ctor_432E10(static_cast<Path_OneShotSwitchIdSetter*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_MovingBomb_484E00(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<Path_MovingBomb*>(pTlv);

    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("D1MBOMB.BAN", ResourceManager::Resource_Animation, AOResourceID::kMovebombAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, AOResourceID::kExplo2AOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, AOResourceID::kMetalGibAOResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, loadMode, pMovingBombTlv->field_22_disabled_resources & 1);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, loadMode, pMovingBombTlv->field_22_disabled_resources & 4);

        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
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

        if (!(pMovingBombTlv->field_22_disabled_resources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID);
        }

        auto pMovingBomb = ao_new<MovingBomb>();
        if (pMovingBomb)
        {
            pMovingBomb->ctor_43AFE0(pMovingBombTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_MovingBombStopper_Null_484DF0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_MeatSaw_483F70(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pMeatSaw = ao_new<MeatSaw>();
        if (pMeatSaw)
        {
            pMeatSaw->ctor_439570(static_cast<Path_MeatSaw*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_MudPathTrans_Null_4818B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_MenuController_481AC0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

            auto pMenu = ao_new<Menu>();
            if (pMenu)
            {
                pMenu->ctor_47A6F0(pTlv, tlvOffsetLevelIdPathId.all);
            }
        }
    }
}


EXPORT void Factory_481FB0(Path_TLV* /*pTlv*/, Map* pMap, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    LOG_ERROR("lvl = " << static_cast<s32>(pMap->field_0_current_level) << " path = " << pMap->field_2_current_path << " camera = " << pMap->field_4_current_camera);
    ALIVE_FATAL("Didn't expect Factory_481FB0 to ever be used, please post the log in #oddworld_dev on discord");
}


EXPORT void Factory_HintFly_4819E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pHintFly = ao_new<HintFly>();
        if (pHintFly)
        {
            pHintFly->ctor_42A820(static_cast<Path_HintFly*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ScrabNoFall_Null_481890(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_IdSplitter_4875E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pIdSplitter = ao_new<TimerTrigger>();
        if (pIdSplitter)
        {
            pIdSplitter->ctor_479B40(static_cast<Path_TimerTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_SecurityDoor_487790(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("R1SDOS.BAN", ResourceManager::Resource_Animation, AOResourceID::kR1sdosAOResID_6027, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kR1sdosAOResID_6027);

        auto pSecurityDoor = ao_new<SecurityDoor>();
        if (pSecurityDoor)
        {
            pSecurityDoor->ctor_461840(static_cast<Path_SecurityDoor*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Null_487440(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_GrenadeMachine_487860(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pBoomMachineTlv = static_cast<Path_BoomMachine*>(pTlv);
    const auto disabledResources = pBoomMachineTlv->field_1C_disabled_resources;
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

    auto pGrenadeMachine = ao_new<BoomMachine>();
    if (pGrenadeMachine)
    {
        pGrenadeMachine->ctor_41E420(pBoomMachineTlv, tlvOffsetLevelIdPathId.all);
    }
}


EXPORT void Factory_LCD_481950(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, AOResourceID::kLcdfontAOResID, loadMode);
    }
    else
    {
        auto pLCD = ao_new<LCDScreen>();
        if (pLCD)
        {
            pLCD->ctor_433F60(static_cast<Path_LCDScreen*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ElumPathTrans_Null_4818C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_HandStone_487480(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("ABESTONE.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbestoneAOResID, loadMode);
        ResourceManager::LoadResource_446C90("SPOTLITE.BAN", ResourceManager::Resource_Animation, AOResourceID::kSpotliteAOResID, loadMode);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_CreditsController_481A50(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        if (!gCreditsControllerExists_507684)
        {
            auto pCreditsController = ao_new<CreditsController>();
            if (pCreditsController)
            {
                pCreditsController->ctor_40CFC0(static_cast<Path_CreditsController*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
    }
}


EXPORT void Factory_Preloader_Null_4817A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_StatusBoard_487AF0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, AOResourceID::kLcdfontAOResID, loadMode);
    }
    else
    {
        auto pStatusBoard = ao_new<LCDStatusBoard>();
        if (pStatusBoard)
        {
            pStatusBoard->ctor_4418E0(static_cast<Path_LCDStatusBoard*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_SwitchStateBooleanLogic_487B80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pSwitchStateBooleanLogic = ao_new<SwitchStateBooleanLogic>();
        if (pSwitchStateBooleanLogic)
        {
            pSwitchStateBooleanLogic->ctor_436AB0(static_cast<Path_SwitchStateBooleanLogic*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_MusicTrigger_482020(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pMusicTrigger = ao_new<MusicTrigger>();
        if (pMusicTrigger)
        {
            pMusicTrigger->ctor_4439F0(static_cast<Path_MusicTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_LightEffect_484170(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pPathLightTlv = static_cast<Path_LightEffect*>(pTlv);
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        switch (pPathLightTlv->field_18_type)
        {
            case Path_LightEffect::Type::Star_0:
                ResourceManager::LoadResource_446C90("STAR.BAN", ResourceManager::Resource_Animation, AOResourceID::kBGStarAOResID, loadMode);
                break;

            case Path_LightEffect::Type::GoldGlow_1:
                ResourceManager::LoadResource_446C90("GOLDGLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kGoldGlowAOResID_6011, loadMode);
                break;

            case Path_LightEffect::Type::GreenGlow_2:
                ResourceManager::LoadResource_446C90("GRENGLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kGreenGlowAOResID_6010, loadMode);
                break;

            case Path_LightEffect::Type::FlintGlow_3:
                ResourceManager::LoadResource_446C90("FLNTGLOW.BAN", ResourceManager::Resource_Animation, AOResourceID::kFlintGlowAOResID_6028, loadMode);
                break;

            case Path_LightEffect::Type::Switchable_RedGreenDoorLights_4:
                ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, AOResourceID::kGreenDoorLightAOResID_6031, loadMode);
                ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, AOResourceID::kRedDoorLightAOResID_6032, loadMode);
                break;

            case Path_LightEffect::Type::Switchable_RedGreenHubLight_5:
                ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, AOResourceID::kGreenDoorLightAOResID_6031, loadMode);
                ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, AOResourceID::kRedDoorLightAOResID_6032, loadMode);
                break;

            default:
                return;
        }
    }
    else
    {
        switch (pPathLightTlv->field_18_type)
        {
            case Path_LightEffect::Type::Star_0:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kBGStarAOResID);
                auto pLight = ao_new<LightEffect>();
                if (pLight)
                {
                    pLight->ctor_4064C0(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
                break;
            }

            case Path_LightEffect::Type::GoldGlow_1:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kGoldGlowAOResID_6011);
                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
                break;
            }

            case Path_LightEffect::Type::GreenGlow_2:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kGreenGlowAOResID_6010);
                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
                break;
            }

            case Path_LightEffect::Type::FlintGlow_3:
            {
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, AOResourceID::kFlintGlowAOResID_6028);
                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
                break;
            }

            case Path_LightEffect::Type::Switchable_RedGreenDoorLights_4:
            case Path_LightEffect::Type::Switchable_RedGreenHubLight_5:
            {
                auto kResources = {
                    AOResourceID::kGreenDoorLightAOResID_6031,
                    AOResourceID::kRedDoorLightAOResID_6032,
                };
                ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
                break;
            }

            default:
                return;
        }
    }
}


EXPORT void Factory_SlogSpawner_4851D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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

        auto pSlogSpawner = ao_new<SlogSpawner>();
        if (pSlogSpawner)
        {
            pSlogSpawner->ctor_475DD0(static_cast<Path_SlogSpawner*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_GasCountDown_487BE0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::LoadResourceFromList_1 || loadMode == LoadMode::LoadResource_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, AOResourceID::kLcdfontAOResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEGAS.BAN", ResourceManager::Resource_Animation, AOResourceID::kAbegasAOResID, loadMode);
    }
    else
    {
        auto pDeathClock = ao_new<GasCountDown>();
        pDeathClock->ctor_40BF60(static_cast<Path_GasCountDown*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}



EXPORT void Factory_RingCancel_4818D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        // The field field_18_bShrykull_remove was removed from the Path_RingCancel TLV because it doesnt
        // actually exist in any path data. The actual value for this field was the 2 bytes after the TLV ended
        // which is always 0 apart from in the cases below.
        // However any level saved with the legacy level editor will have this field added which is handled by checking the size.
        bool bRemovesShrykull = false;

        struct Path_RingCancel_Corrected final : public Path_RingCancel
        {
            s16 field_18_bShrykull_remove;
        };

        if (pTlv->field_2_length == sizeof(Path_RingCancel_Corrected))
        {
            bRemovesShrykull = static_cast<Path_RingCancel_Corrected*>(pTlv)->field_18_bShrykull_remove;
        }
        else
        {
            switch (gMap_507BA8.field_0_current_level)
            {
                case LevelIds::eDesert_8: // d1.lvl
                    if (gMap_507BA8.field_2_current_path == 4)
                    {
                        // original TLV data is -1 part of collision line
                        bRemovesShrykull = true;
                    }
                    break;

                case LevelIds::eForestTemple_4: // f2.lvl
                    if (gMap_507BA8.field_2_current_path == 6)
                    {
                        // original TLV data is 4 part of the flags of the next object
                        bRemovesShrykull = true;
                    }
                    break;
            }
        }

        if (bRemovesShrykull)
        {
            if (sActiveHero_507678->field_168_ring_pulse_timer)
            {
                if (sActiveHero_507678->field_16C_bHaveShrykull)
                {
                    Abe::Free_Shrykull_Resources_42F4C0();
                    sActiveHero_507678->field_168_ring_pulse_timer = 0;
                }
            }
        }
        else
        {
            if (!sActiveHero_507678->field_16C_bHaveShrykull)
            {
                sActiveHero_507678->field_168_ring_pulse_timer = 0;
            }
        }
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_GasEmitter_484110(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pGasEmitter = ao_new<GasEmitter>();
        if (pGasEmitter)
        {
            pGasEmitter->ctor_41D760(static_cast<Path_GasEmitter*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ZzzSpawner_487C80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::LoadResourceFromList_1 && loadMode != LoadMode::LoadResource_2)
    {
        auto pZzzSpawner = ao_new<ZzzSpawner>();
        if (pZzzSpawner)
        {
            pZzzSpawner->ctor_472C80(static_cast<Path_ZzzSpawner*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_BackgroundGlukkon_487CE0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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
        auto pBackgroundGlukkon = ao_new<BackgroundGlukkon>();
        if (pBackgroundGlukkon)
        {
            pBackgroundGlukkon->ctor_41DBD0(static_cast<Path_BackgroundGlukkon*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_KillUnsavedMuds_487DA0(Path_TLV* /*pTlv*/, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
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
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 1);
        }
    }
}


EXPORT void Factory_SoftLanding_Null_4817C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}


EXPORT void Factory_ResetPath_Null_4818A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
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
