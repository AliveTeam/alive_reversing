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
#include "IdSplitter.hpp"
#include "PullRingRope.hpp"
#include "InvisibleSwitch.hpp"
#include "FallingItem.hpp"
#include "FootSwitch.hpp"
#include "HoistRocksEffect.hpp"
#include "RollingBall.hpp"
#include "Switch.hpp"
#include "SecurityDoor.hpp"
#include "BackgroundGlukkon.hpp"
#include "Rock.hpp"
#include "Well.hpp"
#include "SlogHut.hpp"
#include "BirdPortal.hpp"
#include "SlogSpawner.hpp"
#include "DoorFlame.hpp"
#include "BoomMachine.hpp"
#include "GasEmitter.hpp"
#include "MotionDetector.hpp"
#include "StatsSign.hpp"
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

namespace AO {

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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, ResourceID::kAbehoistResID, loadMode);
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("DRPSPRK.BAN", ResourceManager::Resource_Animation, ResourceID::kDrpRockResID, loadMode);
            break;

        case LevelIds::eForest_3:
        case LevelIds::eDesert_8:
            ResourceManager::LoadResource_446C90("ANEEDGE.BAN", ResourceManager::Resource_Animation, ResourceID::kAneedgeResID, loadMode);
            ResourceManager::LoadResource_446C90("DRPROCK.BAN", ResourceManager::Resource_Animation, ResourceID::kDrpRockResID, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("DRPROCK.BAN", ResourceManager::Resource_Animation, ResourceID::kDrpRockResID, loadMode);
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, ResourceID::kAbehoistResID, loadMode, 0);

        if (gMap_507BA8.field_0_current_level == LevelIds::eForest_3 || gMap_507BA8.field_0_current_level == LevelIds::eDesert_8)
        {
            ResourceManager::LoadResource_446C90("ANEEDGE.BAN", ResourceManager::Resource_Animation, ResourceID::kAneedgeResID, loadMode, 0);
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<3> kResources =
        {
            { ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID },
            { ResourceManager::Resource_Animation, ResourceID::kHubdoorResID },
            { ResourceManager::Resource_Animation, ResourceID::kRockdoorResID }
        };
        ResourceManager::LoadResource_446C90("ABEDOOR.BAN", ResourceManager::Resource_Animation, ResourceID::kAbedoorResID, loadMode);
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
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResourcesFromList_446E80("DDOOR.BND", kResources.AsList(), loadMode, 0);
            break;

        default:
            ResourceManager::LoadResourcesFromList_446E80("FDOOR.BND", kResources.AsList(), loadMode, 0);
            break;
        }
    }
    else
    {
        auto kResources = 
        { 
            ResourceID::kAbedoorResID,
            ResourceID::kF2p3dorResID,
            ResourceID::kHubdoorResID,
            ResourceID::kRockdoorResID 
        };
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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
    static CompileTimeResourceList<3> kResources =
    {
        { ResourceManager::Resource_Animation, ResourceID::kAbeliftResID },
        { ResourceManager::Resource_Animation, ResourceID::kPulleyResID },
        { ResourceManager::Resource_Animation, ResourceID::kLiftResID }
    };

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResourcesFromList_446E80("RLIFT.BND", kResources.AsList(), loadMode, 0);
            break;

        case LevelIds::eLines_2:
            ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResourcesFromList_446E80("LLIFT.BND", kResources.AsList(), loadMode, 0);
            break;

        case LevelIds::eDesert_8:
            ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResourcesFromList_446E80("D1LIFT.BND", kResources.AsList(), loadMode, 0);
            break;

        case LevelIds::eDesertTemple_9:
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResourcesFromList_446E80("D2LIFT.BND", kResources.AsList(), loadMode, 0);
            break;

        default:
            ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResourcesFromList_446E80("FLIFT.BND", kResources.AsList(), loadMode, 0);
            break;
        }
    }
    else
    {
        for (int idx = 0; idx < gBaseGameObject_list_9F2DF0->Size(); idx++)
        {
            BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(idx);
            if (!pObjIter)
            {
                break;
            }

            if (!pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pObjIter->field_4_typeId == Types::eLiftPoint_51)
            {
                auto pLiftObj = static_cast<LiftPoint*>(pObjIter);

                const short xpos_i = FP_GetExponent(pLiftObj->field_A8_xpos);
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

        auto kResourcesToCheck =
        {
            ResourceID::kLiftResID,
            ResourceID::kAbeliftResID,
            ResourceID::kRopesResID,
            ResourceID::kRopesResID
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        if (pTlv->field_1_unknown & 2 || (pTlv->field_1_unknown == 0 && static_cast<Path_LiftPoint*>(pTlv)->field_1A_bstart_point))
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
            short pointNumber = 1;
            while (pointNumber < 8)
            {
                pTlvIter = gMap_507BA8.Get_First_TLV_For_Offsetted_Camera_4463B0(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));
                while (pTlvIter)
                {
                    if (pTlvIter->field_4_type == TlvTypes::LiftPoint_8)
                    {
                        const auto tlv_x = pTlv->field_C_sound_pos.field_0_x;
                        const auto absX = pTlvIter->field_C_sound_pos.field_0_x - tlv_x >= 0 ? pTlvIter->field_C_sound_pos.field_0_x - tlv_x : tlv_x - pTlvIter->field_C_sound_pos.field_0_x;
                        if (absX < 5)
                        {
                            if (pTlvIter->field_1_unknown & 2 ||
                                (pTlvIter->field_1_unknown == 0 && static_cast<Path_LiftPoint*>(pTlvIter)->field_1A_bstart_point))
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEWELL.BAN", ResourceManager::Resource_Animation, ResourceID::kAbewellResID, loadMode);
        ResourceManager::LoadResource_446C90("WELLLEAF.BAN", ResourceManager::Resource_Animation, ResourceID::kWellLeafResID, loadMode);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kAbewellResID,
            ResourceID::kWellLeafResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        const FP xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
        const FP ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
        auto pWell = ao_new<Well>();
        if (pWell)
        {
            pWell->ctor_48AEE0(static_cast<Path_Well_Base*>(pTlv), xpos, ypos, tlvOffsetLevelIdPathId.all);
            return;
        }
    }
}


EXPORT void Factory_Dove_4834C0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kDovbasicResID });

        auto pDoveTlv = static_cast<Path_Dove*>(pTlv);

        const short width = pDoveTlv->field_14_bottom_right.field_0_x - pDoveTlv->field_10_top_left.field_0_x;
        const short height = pDoveTlv->field_14_bottom_right.field_2_y - pDoveTlv->field_10_top_left.field_2_y;

        for (int i = 0; i < pDoveTlv->field_18_dove_count; i++)
        {
            auto pDove = ao_new<Dove>();
            if (pDove)
            {
                pDove->ctor_40EE50(5052, 41, 20, 60, tlvOffsetLevelIdPathId.all, 
                    pDoveTlv->field_1C_scale != 0 ? FP_FromDouble(0.5) : FP_FromInteger(1));
            }

            short ypos = 0;
            if (pDoveTlv->field_1A_pixel_perfect)
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<3> kResources =
        {
            { ResourceManager::Resource_Animation, ResourceID::kAbepickResID },
            { ResourceManager::Resource_Animation, ResourceID::kAbethrowResID },
            { ResourceManager::Resource_Animation, ResourceID::kAberockResID }
        };

        ResourceManager::LoadResourcesFromList_446E80("RTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || 
            gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, ResourceID::kP2c2bagResID, loadMode);
            ResourceManager::LoadResource_446C90("EPUIROCK.BAN", ResourceManager::Resource_Palt, ResourceID::kAberockResID, loadMode);
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kP2c2bagResID });
        auto pRockSack = ao_new<RockSack>();
        if (pRockSack)
        {
            pRockSack->ctor_4573F0(static_cast<Path_RockSack*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ZBall_483890(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        if (gMap_507BA8.field_0_current_level == LevelIds::eForestTemple_4)
        {
            ResourceManager::LoadResource_446C90("F2ZBALL.BAN", ResourceManager::Resource_Animation, ResourceID::kF2zballResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEKNOKZ.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeknokzResID, loadMode);
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1BARREL.BAN", ResourceManager::Resource_Animation, ResourceID::kF2rockResID, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, ResourceID::kStickResID, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, loadMode);
            ResourceManager::LoadResource_446C90("R1MEAT.BAN", ResourceManager::Resource_Animation, ResourceID::kUnknownResID_6014, loadMode);
            break;

        case LevelIds::eLines_2:
        case LevelIds::eBoardRoom_12:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, ResourceID::kF2rockResID, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, ResourceID::kStickResID, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, ResourceID::kDebrisID00, loadMode);
            break;

        case LevelIds::eStockYards_5:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, ResourceID::kF2rockResID, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, ResourceID::kStickResID, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, ResourceID::kDebrisID00, loadMode);
            break;

        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, ResourceID::kF2rockResID, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, ResourceID::kStickResID, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, ResourceID::kDebrisID00, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, ResourceID::kF2rockResID, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, ResourceID::kStickResID, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, ResourceID::kDebrisID00, loadMode);
            break;
        }
    }
    else
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kUnknownResID_6014 });
            break;

        case LevelIds::eLines_2:
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kDebrisID00 });
            break;

        case LevelIds::eStockYards_5:
        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kDebrisID00 });
            break;

        default:
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kDebrisID00 });
        }

        auto kResourcesToCheck =
        {
            ResourceID::kF2rockResID,
            ResourceID::kStickResID,
            ResourceID::kRockShadowResID,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResource_446C90("R1PULRNG.BAN", ResourceManager::Resource_Animation, ResourceID::kPullringResID, loadMode);
            break;

        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
            ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResource_446C90("PULLRING.BAN", ResourceManager::Resource_Animation, ResourceID::kPullringResID, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, ResourceID::kRopesResID, loadMode);
            ResourceManager::LoadResource_446C90("PULLRING.BAN", ResourceManager::Resource_Animation, ResourceID::kPullringResID, loadMode);
            break;
        }
    }
    else
    {
        auto kResourcesToCheck =
        {
            ResourceID::kPullringResID,
            ResourceID::kRopesResID,
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ELMHONEY.BAN", ResourceManager::Resource_Animation, ResourceID::kNormaliconResID, loadMode);
        ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, ResourceID::kUnknownResID_204, loadMode);
        ResourceManager::LoadResource_446C90("HONEY.BAN", ResourceManager::Resource_Animation, ResourceID::kHoneyResID, loadMode);
    }
    else
    {
        auto kResourcesToCheck =
        {
            ResourceID::kNormaliconResID,
            ResourceID::kUnknownResID_204,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        auto pHoney = ao_new<Honey>();
        if (pHoney)
        {
            const auto midPoint = (pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2;
            pHoney->ctor_431E30(
                FP_FromInteger(midPoint + pTlv->field_C_sound_pos.field_0_x),
                FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 24));

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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode, disabledResource & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadMode, disabledResource & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, loadMode, disabledResource & 4);
        
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BOMB.BAN", ResourceManager::Resource_Palt, ResourceID::kBombResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, loadMode);
        }

        static CompileTimeResourceList<2> kBombResources = 
        {
            { ResourceManager::Resource_Animation, ResourceID::kBombResID },
            { ResourceManager::Resource_Animation, ResourceID::kBombflshResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("BOMB.BND", kBombResources.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kExplodeResources =
        {
            { ResourceManager::Resource_Animation, ResourceID::kAbebombResID },
            { ResourceManager::Resource_Animation, ResourceID::kDebrisID00 },
            { ResourceManager::Resource_Animation, ResourceID::kBgexpldResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kExplodeResources.AsList(), loadMode, 0);
    }
    else
    {
        auto kResourcesToCheck =
        {
            ResourceID::kAbebombResID,
            ResourceID::kBombResID,
            ResourceID::kBombflshResID,
            ResourceID::kBgexpldResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResourcesToCheck);

        if (!(disabledResource & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kAbeblowResID });
        }

        if (!(disabledResource & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlogBlowResID });
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

static CompileTimeResourceList<3> kResources_4BD1AC =
{
    { ResourceManager::Resource_Animation, ResourceID::kSlgzshotResID },
    { ResourceManager::Resource_Animation, ResourceID::kZflashResID },
    { ResourceManager::Resource_Animation, ResourceID::kAbeknokzResID }
};

static CompileTimeResourceList<4> kResources_4BD1CC =
{
    { ResourceManager::Resource_Animation, ResourceID::kSlgbasicResID },
    { ResourceManager::Resource_Animation, ResourceID::kSlgknbkResID },
    { ResourceManager::Resource_Animation, ResourceID::kBigflashResID },
    { ResourceManager::Resource_Animation, ResourceID::kShellResID }
};


static void LoadWalkingSligResources(LoadMode loadMode, BitField16<SligFlags_DisabledRes> disabledResources)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("SLGLEVER.BAN", ResourceManager::Resource_Animation, ResourceID::kSlgleverResID, loadMode, disabledResources.Get(eDisabledRes_Bit1));
        ResourceManager::LoadResource_446C90("SLGLIFT.BAN", ResourceManager::Resource_Animation, ResourceID::kSlgliftResID, loadMode, disabledResources.Get(eDisabledRes_Bit2));
        ResourceManager::LoadResource_446C90("SLGSLEEP.BAN", ResourceManager::Resource_Animation, ResourceID::kSlgsleepResID, loadMode, disabledResources.Get(eDisabledRes_Bit7));
        ResourceManager::LoadResource_446C90("SLGEDGE.BAN", ResourceManager::Resource_Animation, ResourceID::kSlgedgeResID, loadMode, disabledResources.Get(eDisabledRes_Bit9));
        ResourceManager::LoadResource_446C90("SLGSMASH.BAN", ResourceManager::Resource_Animation, ResourceID::kSlgsmashResID, loadMode, disabledResources.Get(eDisabledRes_Bit10));
        ResourceManager::LoadResource_446C90("SLGBEAT.BAN", ResourceManager::Resource_Animation, ResourceID::kSlgbeatResID, loadMode, disabledResources.Get(eDisabledRes_Bit11));
        ResourceManager::LoadResource_446C90("SLGKNFD.BAN", ResourceManager::Resource_Animation, ResourceID::kSlgknfdResID, loadMode, disabledResources.Get(eDisabledRes_Bit8));

        ResourceManager::LoadResourcesFromList_446E80(
            "SLIGZ.BND",
            kResources_4BD1AC.AsList(),
            loadMode,
            disabledResources.Get(eDisabledRes_Bit3));

        ResourceManager::LoadResourcesFromList_446E80(
            "SLIG.BND",
            kResources_4BD1CC.AsList(),
            loadMode,
            0);

        ResourceManager::LoadResource_446C90("SLGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSligBlowResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, loadMode, 0);

        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kSlgbasicResID, loadMode, 0);
        }
    }
    else
    {
        if (!(disabledResources.Get(eDisabledRes_Bit1)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgleverResID });
        }

        if (!(disabledResources.Get(eDisabledRes_Bit2)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgliftResID });
        }

        if (!(disabledResources.Get(eDisabledRes_Bit3)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgzshotResID });
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kZflashResID });
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kAbeknokzResID });
        }

        if (!(disabledResources.Get(eDisabledRes_Bit7)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgsleepResID });
        }

        if (!(disabledResources.Get(eDisabledRes_Bit9)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgedgeResID });
        }

        if (!(disabledResources.Get(eDisabledRes_Bit10)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgsmashResID });
        }

        if (!(disabledResources.Get(eDisabledRes_Bit11)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgbeatResID });
        }

        if (!(disabledResources.Get(eDisabledRes_Bit8)))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgknfdResID });
        }

        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kBigflashResID });
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kShellResID });
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kRockShadowResID });
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSligBlowResID });
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgbasicResID });
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlgknbkResID });
    }
}

EXPORT void Factory_Slig_482EC0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->field_50_disable_resources);

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<4> kResources = 
        {
            { ResourceManager::Resource_Animation, ResourceID::kDogbasicResID },
            { ResourceManager::Resource_Animation, ResourceID::kDogrstnResID },
            { ResourceManager::Resource_Animation, ResourceID::kDogattkResID },
            { ResourceManager::Resource_Animation, ResourceID::kDogidleResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("SLOG.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGKNFD.BAN", ResourceManager::Resource_Animation, ResourceID::kDogknfdResID, loadMode, 0);
    }
    else
    {
        auto kResourceToCheck =
        {
            ResourceID::kDogbasicResID,
            ResourceID::kDogrstnResID,
            ResourceID::kDogattkResID,
            ResourceID::kDogknfdResID,
            ResourceID::kDogidleResID,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eStockYards_5:
        case LevelIds::eStockYardsReturn_6:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1SWITCH.BAN", ResourceManager::Resource_Animation, ResourceID::kSwitchResID, loadMode);
            break;

        case LevelIds::eLines_2:
            ResourceManager::LoadResource_446C90("L1SWITCH.BAN", ResourceManager::Resource_Animation, ResourceID::kSwitchResID, loadMode);
            break;

        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResource_446C90("SWITCH1.BAN", ResourceManager::Resource_Animation, ResourceID::kSwitchResID, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("SWITCH1.BAN", ResourceManager::Resource_Animation, ResourceID::kSwitchResID, loadMode);
            break;
        }
        ResourceManager::LoadResource_446C90("ABEPULL.BAN", ResourceManager::Resource_Animation, ResourceID::kAbepullResID, loadMode);
    }
    else
    {
        auto pSwitch = ao_new<Switch>();
        if (pSwitch)
        {
            pSwitch->ctor_481110(static_cast<Path_Switch*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_BellHammer_4854B0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kHammerResID });
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("F2MAMORB.BAN", ResourceManager::Resource_Animation, ResourceID::kMaimGameResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, ResourceID::kSplineResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, loadMode, disabledResources & 4);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, ResourceID::kMetalResID, loadMode, disabledResources & 0x10);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kMaimGameResID});
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSplineResID });


        if (!(disabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kAbeblowResID });
        }

        if (!(disabledResources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlogBlowResID });
        }

        if (!(disabledResources & 0x10))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kMetalResID });
        }

        if (!(disabledResources & 0x20))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kExplo2ResID });
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, loadMode);
        ResourceManager::LoadResource_446C90("ABELIFT.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeliftResID, loadMode);
    }
    else 
    {
        auto kResources =
        {
            ResourceID::kAbebsic1ResID,
            ResourceID::kAbeliftResID,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEWASP.BAN", ResourceManager::Resource_Animation, ResourceID::kAbewaspResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("WASP.BAN", ResourceManager::Resource_Animation, ResourceID::kWaspResID, loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eForest_3 || gMap_507BA8.field_0_current_level == LevelIds::eDesert_8)
        {
            ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, ResourceID::kUnknownResID_204, loadMode, 0);
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("HONEY.BAN", ResourceManager::Resource_Animation, ResourceID::kHoneyResID, loadMode);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kP2c3HiveResID,
            ResourceID::kWaspResID,
            ResourceID::kAbewaspResID,
            ResourceID::kHoneyResID,
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2 && !gAttract_507698)
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
                sActiveHero_507678->field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);
                sActiveHero_507678->field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
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
    if (loadMode == LoadMode::Mode_1|| loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ELMKNBK.BAN", ResourceManager::Resource_Animation, ResourceID::kElmnknbkResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ANEKNBK.BAN", ResourceManager::Resource_Animation, ResourceID::kAneknbkResID, loadMode, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_SlingMud_485A30(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        auto kResources =
        {
            ResourceID::kSlingmudResID_508,
            ResourceID::kSparksResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        auto pSlingMud = ao_new<SlingMudokon>();
        if (pSlingMud)
        {
            pSlingMud->ctor_46F940(static_cast<Path_SlingMud*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_HoneyDrip_Null_481820(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, LoadMode /*loadMode*/)
{
    // Empty
}

EXPORT void Factory_BeeNest_486150(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<2> kResources =
        {
           { ResourceManager::Resource_Animation, ResourceID::kWaspResID },
           { ResourceManager::Resource_Animation, ResourceID::kAbewaspResID }
        };

        ResourceManager::LoadResourcesFromList_446E80("SWARMHOL.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, ResourceID::kUnknownResID_204, loadMode);
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

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, loadMode, disabledResources & 4);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("LNDMNPAL.BAN", ResourceManager::Resource_Palt, ResourceID::kLandmineResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, loadMode);
        }

        static CompileTimeResourceList<2> kResourcesMine =
        {
            { ResourceManager::Resource_Animation, ResourceID::kLandmineResID },
            { ResourceManager::Resource_Animation, ResourceID::kMineflshResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("MINE.BND", kResourcesMine.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kResourcesExplode =
        {
            { ResourceManager::Resource_Animation, ResourceID::kAbebombResID },
            { ResourceManager::Resource_Animation, ResourceID::kDebrisID00 },
            { ResourceManager::Resource_Animation, ResourceID::kBgexpldResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kResourcesExplode.AsList(), loadMode, 0);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kAbebombResID,
            ResourceID::kDebrisID00,
            ResourceID::kLandmineResID,
            ResourceID::kMineflshResID,
            ResourceID::kBgexpldResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(disabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID);
        }

        if (!(disabledResources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID);
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

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode, pUxbTlv->field_20_disabled_resources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadMode, pUxbTlv->field_20_disabled_resources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, loadMode, pUxbTlv->field_20_disabled_resources & 4);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("TBMBPAL.BAN", ResourceManager::Resource_Palt, ResourceID::kTbombResID, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, loadMode);
        }

        static CompileTimeResourceList<3> kResourcesUxb =
        {
            { ResourceManager::Resource_Animation, ResourceID::kTbombResID },
            { ResourceManager::Resource_Animation, ResourceID::kBombflshResID },
            { ResourceManager::Resource_Palt, ResourceID::kGrenflshResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("UXB.BND", kResourcesUxb.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kResourcesExplode =
        {
            { ResourceManager::Resource_Animation, ResourceID::kAbebombResID },
            { ResourceManager::Resource_Animation, ResourceID::kDebrisID00 },
            { ResourceManager::Resource_Animation, ResourceID::kBgexpldResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kResourcesExplode.AsList(), loadMode, 0);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kAbebombResID,
            ResourceID::kTbombResID,
            ResourceID::kBombflshResID,
            ResourceID::kBgexpldResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(pUxbTlv->field_20_disabled_resources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kAbeblowResID });
        }

        if (!(pUxbTlv->field_20_disabled_resources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlogBlowResID });
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<9> kResources =
        {
            { ResourceManager::Resource_Animation, ResourceID::kArjbasicResID },
            { ResourceManager::Resource_Animation, ResourceID::kArjpumpResID },
            { ResourceManager::Resource_Animation, ResourceID::kArjponceResID },
            { ResourceManager::Resource_Animation, ResourceID::kArjclimbResID },
            { ResourceManager::Resource_Animation, ResourceID::kArjscrchResID },
            { ResourceManager::Resource_Animation, ResourceID::kArjeatResID },
            { ResourceManager::Resource_Animation, ResourceID::kArjfalrkResID },
            { ResourceManager::Resource_Animation, ResourceID::kArjwaspResID },
            { ResourceManager::Resource_Animation, ResourceID::kWebResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("PARAMITE.BND", kResources.AsList(), loadMode, 0);
        return;
    }

    auto kResources =
    {
        ResourceID::kArjbasicResID,
        ResourceID::kArjpumpResID,
        ResourceID::kArjponceResID,
        ResourceID::kArjclimbResID,
        ResourceID::kArjscrchResID,
        ResourceID::kArjeatResID,
        ResourceID::kArjfalrkResID,
        ResourceID::kArjwaspResID,
        ResourceID::kWebResID,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("BATBASIC.BAN", ResourceManager::Resource_Animation, ResourceID::kBatBasicResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kBatBasicResID });

        auto pBat = ao_new<Bat>();
        if (pBat)
        {
            pBat->ctor_4046E0(static_cast<Path_Bat*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_RingMud_4858F0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEPULL.BAN", ResourceManager::Resource_Animation, ResourceID::kAbepullResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEOMM.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeommResID, loadMode);
    }
    else 
    {
        auto kResources =
        {
            ResourceID::kAbebsic1ResID,
            ResourceID::kAbepullResID,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<3> kResources1 =
        {
            { ResourceManager::Resource_Animation, ResourceID::kPortalTerminatorID },
            { ResourceManager::Resource_Animation, ResourceID::kPortliteResID },
            { ResourceManager::Resource_Animation, ResourceID::kPortlitResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("PORTAL.BND", kResources1.AsList(), loadMode, 0);

        if (pBirdPortalTlv->field_24_portal_type == PortalType::eShrykull_2)
        {
            static CompileTimeResourceList<2> kResources2 =
            {
                { ResourceManager::Resource_Animation, ResourceID::kAbemorphResID },
                { ResourceManager::Resource_Animation, ResourceID::kShrmorphResID }
            };
            ResourceManager::LoadResourcesFromList_446E80("SHRYPORT.BND", kResources2.AsList(), loadMode, 0);
            ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, ResourceID::kSplineResID, loadMode);
        }
    }
    else
    {
        auto kResources =
        {
            ResourceID::kPortalTerminatorID,
            ResourceID::kDovbasicResID,
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

        auto kResources2 =
        {
            ResourceID::kAbemorphResID,
            ResourceID::kShrmorphResID,
            ResourceID::kSplineResID,
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_TrapDoor_4868E0(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1TRAP.BAN", ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, loadMode, 0);
            break;
        case LevelIds::eLines_2:
            ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, loadMode, 0);
            break;
        case LevelIds::eStockYards_5:
            ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, loadMode, 0);
            break;
        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResource_446C90("D2TRAP.BAN", ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, loadMode, 0);
            break;
        default:
            ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, loadMode, 0);
            break;
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kP6c1trapResID });

        auto pTrapDoor = ao_new<TrapDoor>();
        if (pTrapDoor)
        {
            pTrapDoor->ctor_488010(static_cast<Path_TrapDoor*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_RollingBall_486A60(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("F2STNBAL.BAN", ResourceManager::Resource_Animation, ResourceID::kF2stnbalResID, loadMode);
        ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, ResourceID::kDebrisID00, loadMode);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kF2stnbalResID,
            ResourceID::kDebrisID00,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        auto pRollingBall = ao_new<RollingBall>();
        if (pRollingBall)
        {
            pRollingBall->ctor_4578C0(static_cast<Path_RollingBall*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

struct Path_SligBound : public Path_TLV
{
    __int16 field_18_slig_id;
    __int16 field_1A_disable_resources;
};
ALIVE_ASSERT_SIZEOF(Path_SligBound, 0x1C);

static Path_TLV* FindMatchingSligTLV(Path_TLV* pTlvIter, Path_Slig_Bound* pTlv)
{
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::Slig_24 &&
            pTlv->field_18_slig_id == static_cast<Path_Slig*>(pTlvIter)->field_40_slig_id &&
            !pTlvIter->field_0_flags.Get(TLV_Flags::eBit2_Unknown))
        {
            return pTlvIter;
        }
        pTlvIter = Path_TLV::Next_446460(pTlvIter);
    }
    return nullptr;
}

EXPORT void Factory_SligLeftBound_482520(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    auto pBound = static_cast<Path_Slig_Bound*>(pTlv);
    LoadWalkingSligResources(loadMode, pBound->field_1A_disabled_resources);

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        pBound->field_0_flags.Clear(TLV_Flags::eBit1_Created);
        pBound->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);

        for (short camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            Path_TLV* pTlvIter = gMap_507BA8.Get_First_TLV_For_Offsetted_Camera_4463B0(camX_idx, 0);
            pTlvIter = FindMatchingSligTLV(pTlvIter, pBound);
            if (pTlvIter)
            {
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit1_Created);
                pTlvIter->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("F2STNSCK.BAN", ResourceManager::Resource_Animation, ResourceID::kF2stnsckResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, ResourceID::kF2stnsckResID);
        auto pRollingBallStopper = ao_new<RollingBallStopper>();
        pRollingBallStopper->ctor_43BCE0(static_cast<Path_RollingBallStopper*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

EXPORT void Factory_FootSwitch_486C60(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("TRIGGER.BAN", ResourceManager::Resource_Animation, ResourceID::kTriggerResID, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, ResourceID::kTriggerResID);

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

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("F2EYEORB.BAN", ResourceManager::Resource_Animation, ResourceID::kF2eyeorbResID, loadMode);
        ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, ResourceID::kSplineResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, loadMode, disabledResources & 4);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, ResourceID::kMetalResID, loadMode, disabledResources & 0x10);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, loadMode, disabledResources & 0x20);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kF2eyeorbResID,
            ResourceID::kSplineResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(disabledResources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kAbeblowResID });
        }

        if (!(disabledResources & 2))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlogBlowResID });
        }

        if (!(disabledResources & 0x10))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kMetalResID });
        }

        if (!(disabledResources & 0x20))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kExplo2ResID });
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kDeathFlareResID });

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

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kElecwallResID });

        auto pElectricWall = ao_new<ElectricWall>();
        if (pElectricWall)
        {
            pElectricWall->ctor_40FCF0(static_cast<Path_ElectricWall*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_LiftMover_487580(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        auto kResources =
        {
            ResourceID::kChimeResID,
            ResourceID::kF2ChimeBallResID,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<3> kResources({
            { ResourceManager::Resource_Animation, ResourceID::kElmbasicResID_200 },
            { ResourceManager::Resource_Animation, ResourceID::kElmaloneResID_230 },
            { ResourceManager::Resource_Animation, ResourceID::kElmfallResID_216 },
            });
        ResourceManager::LoadResourcesFromList_446E80("ELMSTART.BND", kResources.AsList(), loadMode, 0);
    }
    else
    {
        Elum::Spawn_410E90(tlvOffsetLevelIdPathId);
        gElum_507680->field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
        gElum_507680->field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    }
}


EXPORT void Factory_MeatSack_483790(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<3> kResources(
        { 
            { ResourceManager::Resource_Animation, ResourceID::kAbepickResID },
            { ResourceManager::Resource_Animation, ResourceID::kAbethrowResID },
            { ResourceManager::Resource_Animation, ResourceID::kMeatResID }
        });

        ResourceManager::LoadResourcesFromList_446E80("MTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, ResourceID::kP2c2bagResID, loadMode);
        }
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kD2elumResID });

        auto pMeatSack = ao_new<MeatSack>();
        if (pMeatSack)
        {
            pMeatSack->ctor_4390F0(static_cast<Path_MeatSack*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Scrab_4863E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<12> kResources(
        {
            { ResourceManager::Resource_Animation, ResourceID::kArsbasicResID },
            { ResourceManager::Resource_Animation, ResourceID::kArsdanceResID },
            { ResourceManager::Resource_Animation, ResourceID::kArsdeadResID },
            { ResourceManager::Resource_Animation, ResourceID::kArsgrwlResID },
            { ResourceManager::Resource_Animation, ResourceID::kArshowlResID },
            { ResourceManager::Resource_Animation, ResourceID::kArsroarResID },
            { ResourceManager::Resource_Animation, ResourceID::kArswhirlResID },
            { ResourceManager::Resource_Animation, ResourceID::kArschewResID },
            { ResourceManager::Resource_Animation, ResourceID::kArseatResID },
            { ResourceManager::Resource_Animation, ResourceID::kArsprceResID },
            { ResourceManager::Resource_Animation, ResourceID::kArsskwrResID },
            { ResourceManager::Resource_Animation, ResourceID::kArscrshResID }
        });
        ResourceManager::LoadResourcesFromList_446E80("SCRAB.BND", kResources.AsList(), loadMode, 0);
        return;
    }

    auto kResources =
    {
        ResourceID::kArsgrwlResID,
        ResourceID::kArsdeadResID,
        ResourceID::kArsdanceResID,
        ResourceID::kArsbasicResID,
        ResourceID::kArshowlResID,
        ResourceID::kArsroarResID,
        ResourceID::kArswhirlResID,
        ResourceID::kArschewResID,
        ResourceID::kArseatResID,
        ResourceID::kArsprceResID,
        ResourceID::kArsskwrResID,
        ResourceID::kArscrshResID,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("FIRE.BAN", ResourceManager::Resource_Animation, ResourceID::kHubFireResID, loadMode);
    }
    else 
    {
        auto kResources =
        {
            ResourceID::kFlintLockResID,
            ResourceID::kGourdResID,
            ResourceID::kHubFireResID,
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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
    auto pMudTlv = static_cast<Path_Mud*>(pTlv);
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        auto kResources =
        {
            ResourceID::kAbebsic1ResID,
            ResourceID::kAbebasicResID,
            ResourceID::kAbeknfdResID,
            ResourceID::kAbeknbkResID,
            ResourceID::kAbeedgeResID,
            ResourceID::kMudscrubResID,
            ResourceID::kMudoduckResID,
            ResourceID::kMudbtlnkResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        switch (pMudTlv->field_1A_job)
        {
        case 0:
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kMudchslResID });
            break;

        case 2:
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kMudltusResID });
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
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, ResourceID::kAbebsic1ResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBSIC.BAN", ResourceManager::Resource_Animation, ResourceID::kAbebasicResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNFD.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeknfdResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNBK.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeknbkResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEEDGE.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeedgeResID, loadMode);
        ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRockShadowResID, loadMode);

        switch (pMudTlv->field_1A_job)
        {
        case 0:
        {
            static CompileTimeResourceList<3> kResources =
            {
                { ResourceManager::Resource_Animation, ResourceID::kMudoduckResID },
                { ResourceManager::Resource_Animation, ResourceID::kMudscrubResID },
                { ResourceManager::Resource_Animation, ResourceID::kMudbtlnkResID }
            };
            ResourceManager::LoadResource_446C90("MUDCHSL.BAN", ResourceManager::Resource_Animation, ResourceID::kMudchslResID, loadMode);
            ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
        }
            break;

        case 1:
        {
            static CompileTimeResourceList<3> kResources =
            {
                { ResourceManager::Resource_Animation, ResourceID::kMudoduckResID },
                { ResourceManager::Resource_Animation, ResourceID::kMudscrubResID },
                { ResourceManager::Resource_Animation, ResourceID::kMudbtlnkResID }
            };
            ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
        }
            break;

        case 2:
        {
            static CompileTimeResourceList<4> kResources =
            {
                { ResourceManager::Resource_Animation, ResourceID::kMudltusResID },
                { ResourceManager::Resource_Animation, ResourceID::kMudoduckResID },
                { ResourceManager::Resource_Animation, ResourceID::kMudscrubResID },
                { ResourceManager::Resource_Animation, ResourceID::kMudbtlnkResID }
            };
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("FIRE.BAN", ResourceManager::Resource_Animation, ResourceID::kHubFireResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("GLOW1.BAN", ResourceManager::Resource_Animation, ResourceID::kGlowResID, loadMode, 0);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kHubFireResID,
            ResourceID::kGlowResID,
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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

    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("D1MBOMB.BAN", ResourceManager::Resource_Animation, ResourceID::kMovebombResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, ResourceID::kMetalResID, loadMode, 0);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode, pMovingBombTlv->field_22_disabled_resources & 1);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, loadMode, pMovingBombTlv->field_22_disabled_resources & 4);

        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, loadMode, 0);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, loadMode, 0);
        }
    }
    else
    {
        auto kResources =
        {
            ResourceID::kMovebombResID,
            ResourceID::kExplo2ResID,
            ResourceID::kMetalResID,
        };
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

        if (!(pMovingBombTlv->field_22_disabled_resources & 1))
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kAbeblowResID });
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("R1METSAW.BAN", ResourceManager::Resource_Animation, ResourceID::kMeatsawResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode);
    }
    else 
    {
        auto kResources =
        {
            ResourceID::kMeatsawResID,
            ResourceID::kAbeblowResID,
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
        if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
        {
             static CompileTimeResourceList<4> kResources =
            {
                { ResourceManager::Resource_Animation, ResourceID::kAbespek2ResID },
                { ResourceManager::Resource_Animation, ResourceID::kHighliteResID },
                { ResourceManager::Resource_Animation, ResourceID::kOptflareResID },
                { ResourceManager::Resource_Palt, ResourceID::kHighlitePalResID }
            };
            ResourceManager::LoadResourcesFromList_446E80("STARTANM.BND", kResources.AsList(), loadMode, 0);
        }
        else
        {
            auto kResources =
            {
                ResourceID::kAbespek2ResID,
                ResourceID::kHighliteResID,
                ResourceID::kOptflareResID,
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
    LOG_ERROR("lvl = " << static_cast<int>(pMap->field_0_current_level) << " path = " << pMap->field_2_current_path << " camera = " << pMap->field_4_current_camera);
    ALIVE_FATAL("Didn't expect Factory_481FB0 to ever be used, please post the log in #oddworld_dev on discord");
}


EXPORT void Factory_HintFly_4819E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        auto pIdSplitter = ao_new<IdSplitter>();
        if (pIdSplitter)
        {
            pIdSplitter->ctor_479B40(static_cast<Path_IdSplitter*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_SecurityDoor_487790(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("R1SDOS.BAN", ResourceManager::Resource_Animation, ResourceID::kR1sdosResID_6027, loadMode);
    }
    else
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kR1sdosResID_6027 });

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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<3> kResources =
        {
            { ResourceManager::Resource_Animation, ResourceID::kAbethrowResID },
            { ResourceManager::Resource_Animation, ResourceID::kAbepickResID },
            { ResourceManager::Resource_Animation, ResourceID::kGrenadeResID }
        };
        ResourceManager::LoadResourcesFromList_446E80("GTHROW.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, loadMode);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, ResourceID::kMetalResID, loadMode);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadMode, disabledResources & 2);
        return;
    }

    auto kResources =
    {
        ResourceID::kR1buttonResID,
        ResourceID::kR1bpipeResID,
        ResourceID::kAbestoneResID,
        ResourceID::kAbethrowResID,
        ResourceID::kAbepickResID,
        ResourceID::kGrenadeResID,
        ResourceID::kExplo2ResID,
        ResourceID::kAbeblowResID,
        ResourceID::kMetalResID,
    };
    ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, kResources);

    if (!(disabledResources & 2))
    {
        ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kSlogBlowResID });
    }

    auto pGrenadeMachine = ao_new<BoomMachine>();
    if (pGrenadeMachine)
    {
        pGrenadeMachine->ctor_41E420(pBoomMachineTlv, tlvOffsetLevelIdPathId.all);
    }
}


EXPORT void Factory_LCD_481950(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, ResourceID::kLcdfontResID, loadMode);
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("ABESTONE.BAN", ResourceManager::Resource_Animation, ResourceID::kAbestoneResID, loadMode);
        ResourceManager::LoadResource_446C90("SPOTLITE.BAN", ResourceManager::Resource_Animation, ResourceID::kSpotliteResID, loadMode);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_CreditsController_481A50(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        if (!gCreditsControllerExists_507684)
        {
            auto pCreditsController = ao_new<CreditsController>();
            if (pCreditsController)
            {
                pCreditsController->ctor_40CFC0(static_cast<Path_TLV*>(pTlv), tlvOffsetLevelIdPathId.all);
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, ResourceID::kLcdfontResID, loadMode);
    }
    else
    {
        auto pStatusBoard = ao_new<StatsSign>();
        if (pStatusBoard)
        {
            pStatusBoard->ctor_4418E0(static_cast<Path_StatusBoard*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_SwitchStateBooleanLogic_487B80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        switch (pPathLightTlv->field_18_type)
        {
        case Path_LightEffect::Type::Star_0:
            ResourceManager::LoadResource_446C90("STAR.BAN", ResourceManager::Resource_Animation, ResourceID::kBGStarResID, loadMode);
            break;

        case Path_LightEffect::Type::RedGlow_1:
            ResourceManager::LoadResource_446C90("GOLDGLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kRedGlowResID_6011, loadMode);
            break;

        case Path_LightEffect::Type::GreenGlow_2:
            ResourceManager::LoadResource_446C90("GRENGLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kGreenGlowResID_6010, loadMode);
            break;

        case Path_LightEffect::Type::FlintGlow_3:
            ResourceManager::LoadResource_446C90("FLNTGLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kFlintGlowResID_6028, loadMode);
            break;

        case Path_LightEffect::Type::Switchable_RedGreenDoorLights_4:
            ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, ResourceID::kGreenDoorLightResID_6031, loadMode);
            ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, ResourceID::kRedDoorLightResID_6032, loadMode);
            break;

        case Path_LightEffect::Type::Switchable_RedGreenHubLight_5:
            ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, ResourceID::kGreenDoorLightResID_6031, loadMode);
            ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, ResourceID::kRedDoorLightResID_6032, loadMode);
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
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kBGStarResID });
            auto pLight = ao_new<LightEffect>();
            if (pLight)
            {
                pLight->ctor_4064C0(pPathLightTlv, tlvOffsetLevelIdPathId.all);
            }
            break;
        }

        case Path_LightEffect::Type::RedGlow_1:
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kRedGlowResID_6011 });
            auto pLight = ao_new<DoorLight>();
            if (pLight)
            {
                pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
            }
            break;
        }

        case Path_LightEffect::Type::GreenGlow_2:
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kGreenGlowResID_6010 });
            auto pLight = ao_new<DoorLight>();
            if (pLight)
            {
                pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
            }
            break;
        }

        case Path_LightEffect::Type::FlintGlow_3:
        {
            ResourceManager::CheckResourceIsLoaded(ResourceManager::Resource_Animation, { ResourceID::kFlintGlowResID_6028 });
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
            auto kResources =
            {
                ResourceID::kGreenDoorLightResID_6031,
                ResourceID::kRedDoorLightResID_6032,
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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<4> kResources =
        {
            { ResourceManager::Resource_Animation, ResourceID::kDogbasicResID },
            { ResourceManager::Resource_Animation, ResourceID::kDogrstnResID },
            { ResourceManager::Resource_Animation, ResourceID::kDogattkResID },
            { ResourceManager::Resource_Animation, ResourceID::kDogidleResID }
        };

        ResourceManager::LoadResourcesFromList_446E80("SLOG.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGKNFD.BAN", ResourceManager::Resource_Animation, ResourceID::kDogknfdResID, loadMode);
    }
    else
    {
        auto kResources =
        {
            ResourceID::kDogbasicResID,
            ResourceID::kDogattkResID,
            ResourceID::kDogknfdResID,
            ResourceID::kDogidleResID,
            ResourceID::kDogrstnResID,

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
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT",  ResourceManager::Resource_Font, ResourceID::kLcdfontResID, loadMode);
        ResourceManager::LoadResource_446C90("ABEGAS.BAN", ResourceManager::Resource_Animation, ResourceID::kAbegasResID, loadMode);
    }
    else
    {
        auto pDeathClock = ao_new<GasCountDown>();
        pDeathClock->ctor_40BF60(static_cast<Path_GasCountDown*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}

struct Path_RingCancel : public Path_TLV
{
    __int16 field_18_bShrykull_remove;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_RingCancel, 0x1C);

EXPORT void Factory_RingCancel_4818D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        if (static_cast<Path_RingCancel*>(pTlv)->field_18_bShrykull_remove)
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
        else if (!sActiveHero_507678->field_16C_bHaveShrykull)
        {
            sActiveHero_507678->field_168_ring_pulse_timer = 0;
        }
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_GasEmitter_484110(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        auto pGasEmitter = ao_new<GasEmitter>();
        if (pGasEmitter)
        {
            pGasEmitter->ctor_41D760(static_cast<Path_GasEmitter*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_SlogHut_487C80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        auto pSlogHut = ao_new<SlogHut>();
        if (pSlogHut)
        {
            pSlogHut->ctor_472C80(static_cast<Path_SlogHut*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_BackgroundGlukkon_487CE0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode == LoadMode::Mode_1 || loadMode == LoadMode::Mode_2)
    {
        static CompileTimeResourceList<5> kResources =
        {
          { ResourceManager::Resource_Animation, ResourceID::kGlkbasicResID },
          { ResourceManager::Resource_Palt, ResourceID::kGlukredResID },
          { ResourceManager::Resource_Palt, ResourceID::kGlukgrenResID },
          { ResourceManager::Resource_Palt, ResourceID::kGlukblueResID },
          { ResourceManager::Resource_Palt, ResourceID::kGlukaquaResID }
        };


        ResourceManager::LoadResourcesFromList_446E80("GLUKKON.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, ResourceID::kSlogBlowResID, loadMode);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, loadMode);
    }
    else
    {
        auto pBackgroundGlukkon = ao_new<BackgroundGlukkon>();
        if (pBackgroundGlukkon)
        {
            pBackgroundGlukkon->ctor_41DBD0(static_cast<Path_Glukkon*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_KillUnsavedMuds_487DA0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, LoadMode loadMode)
{
    if (loadMode != LoadMode::Mode_1 && loadMode != LoadMode::Mode_2)
    {
        if (!gbKillUnsavedMudsDone_5076CC)
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

const PathFunctionTable kObjectFactory =
{
    {
    Factory_ContinuePoint_Null_4817D0,
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
    Factory_SlogHut_487C80,
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
    nullptr
    }
};


}
