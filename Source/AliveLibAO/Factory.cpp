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

START_NS_AO

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

EXPORT void Factory_ContinuePoint_Null_4817D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_PathTransition_Null_481720(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ContinueZone_Null_481830(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Hoist_487230(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, 42, loadMode);
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("DRPSPRK.BAN", ResourceManager::Resource_Animation, 357, loadMode);
            break;

        case LevelIds::eForest_3:
        case LevelIds::eDesert_8:
            ResourceManager::LoadResource_446C90("ANEEDGE.BAN", ResourceManager::Resource_Animation, 108, loadMode);
            ResourceManager::LoadResource_446C90("DRPROCK.BAN", ResourceManager::Resource_Animation, 357, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("DRPROCK.BAN", ResourceManager::Resource_Animation, 357, loadMode);
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


EXPORT void Factory_Edge_481730(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEHOIST.BAN", ResourceManager::Resource_Animation, 42, loadMode, 0);

        if (gMap_507BA8.field_0_current_level == LevelIds::eForest_3 || gMap_507BA8.field_0_current_level == LevelIds::eDesert_8)
        {
            ResourceManager::LoadResource_446C90("ANEEDGE.BAN", ResourceManager::Resource_Animation, 108, loadMode, 0);
        }
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_DeathDrop_Null_4817B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Door_481C80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources =
        {
            { ResourceManager::Resource_Animation, 2012 },
            { ResourceManager::Resource_Animation, 2019 },
            { ResourceManager::Resource_Animation, 2018 }
        };
        ResourceManager::LoadResource_446C90("ABEDOOR.BAN", ResourceManager::Resource_Animation, 45, loadMode);
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
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 45, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2012, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2019, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2018, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pDoor = ao_new<Door>();
        if (pDoor)
        {
            pDoor->ctor_40E010(static_cast<Path_Door*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ShadowZone_482080(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pShadowZone = ao_new<ShadowZone>();
        if (pShadowZone)
        {
            pShadowZone->ctor_435D30(static_cast<Path_ShadowZone*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_LiftPoint_4820F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_482E70(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_483D90(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_WellExpress_483340(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEWELL.BAN", ResourceManager::Resource_Animation, 47, loadMode);
        ResourceManager::LoadResource_446C90("WELLLEAF.BAN", ResourceManager::Resource_Animation, 341, loadMode);
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 47, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 341, 0, 0))
        {
            const FP xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
            const FP ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
            auto pWell = ao_new<Well>();
            if (pWell)
            {
                pWell->ctor_48AEE0(static_cast<Path_Well_Base*>(pTlv), xpos, ypos, tlvOffsetLevelIdPathId.all);
                return;
            }
        }
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_Dove_4834C0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 0, 0))
        {
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
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_RockSack_483680(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources =
        {
            { ResourceManager::Resource_Animation, 12 },
            { ResourceManager::Resource_Animation, 14 },
            { ResourceManager::Resource_Animation, 350 }
        };

        ResourceManager::LoadResourcesFromList_446E80("RTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || 
            gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, 1002, loadMode);
            ResourceManager::LoadResource_446C90("EPUIROCK.BAN", ResourceManager::Resource_Palt, 350, loadMode);
        }
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1002, 0, 0))
        {
            auto pRockSack = ao_new<RockSack>();
            if (pRockSack)
            {
                pRockSack->ctor_4573F0(static_cast<Path_RockSack*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_ZBall_483890(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        if (gMap_507BA8.field_0_current_level == LevelIds::eForestTemple_4)
        {
            ResourceManager::LoadResource_446C90("F2ZBALL.BAN", ResourceManager::Resource_Animation, 2001, loadMode);
            ResourceManager::LoadResource_446C90("ABEKNOKZ.BAN", ResourceManager::Resource_Animation, 28, loadMode);
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


EXPORT void Factory_FallingItem_483940(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1BARREL.BAN", ResourceManager::Resource_Animation, 2007, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, 358, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);
            ResourceManager::LoadResource_446C90("R1MEAT.BAN", ResourceManager::Resource_Animation, 6014, loadMode);
            break;

        case LevelIds::eLines_2:
        case LevelIds::eBoardRoom_12:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, 2007, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, 358, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, 1105, loadMode);
            break;

        case LevelIds::eStockYards_5:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, 2007, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, 358, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, 1105, loadMode);
            break;

        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, 2007, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, 358, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, 1105, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("F2ROCK.BAN", ResourceManager::Resource_Animation, 2007, loadMode);
            ResourceManager::LoadResource_446C90("STICK.BAN", ResourceManager::Resource_Animation, 358, loadMode);
            ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);
            ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, 1105, loadMode);
            break;
        }
    }
    else
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eRuptureFarmsReturn_13:
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6014, 0, 0))
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                return;
            }
            break;

        case LevelIds::eLines_2:
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 0, 0))
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                return;
            }
            break;

        case LevelIds::eStockYards_5:
        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 0, 0))
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                return;
            }
            break;

        default:
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 0, 0))
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                return;
            }
        }

        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2007, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 358, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2035, 0, 0))
        {
            auto pFallingItem = ao_new<FallingItem>();
            if (pFallingItem)
            {
                pFallingItem->ctor_419F30(static_cast<Path_FallingItem*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Null_482E90(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_482E80(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_PullRingRope_483DA0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1ROPES.BAN", ResourceManager::Resource_Animation, 1000, loadMode);
            ResourceManager::LoadResource_446C90("R1PULRNG.BAN", ResourceManager::Resource_Animation, 1014, loadMode);
            break;

        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
            ResourceManager::LoadResource_446C90("D1ROPES.BAN", ResourceManager::Resource_Animation, 1000, loadMode);
            ResourceManager::LoadResource_446C90("PULLRING.BAN", ResourceManager::Resource_Animation, 1014, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("ROPES.BAN", ResourceManager::Resource_Animation, 1000, loadMode);
            ResourceManager::LoadResource_446C90("PULLRING.BAN", ResourceManager::Resource_Animation, 1014, loadMode);
            break;
        }
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1014, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1000, 0, 0))
        {
            auto pPullRingRope = ao_new<PullRingRope>();
            if (pPullRingRope)
            {
                pPullRingRope->ctor_4546B0(static_cast<Path_PullRingRope*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}

EXPORT void Factory_BackgroundAnimation_4840A0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pBackgroundAnimation = ao_new<BackgroundAnimation>();
        if (pBackgroundAnimation)
        {
            pBackgroundAnimation->ctor_405A90(static_cast<Path_BackgroundAnimation*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Honey_4844A0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ELMHONEY.BAN", ResourceManager::Resource_Animation, 203, loadMode);
        ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, 204, loadMode);
        ResourceManager::LoadResource_446C90("HONEY.BAN", ResourceManager::Resource_Animation, 337, loadMode);
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 203, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 204, 0, 0))
        {
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
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Null_484640(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_TimedMine_484650(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pTimedMineTlv = static_cast<Path_TimedMine*>(pTlv);
    const auto disabledResource = pTimedMineTlv->field_20_disable_resources;
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, disabledResource & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode, disabledResource & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, 217, loadMode, disabledResource & 4);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BOMB.BAN", ResourceManager::Resource_Palt, 1005, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, 25, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, 576, loadMode);
        }

        static CompileTimeResourceList<2> kBombResources = 
        {
            { ResourceManager::Resource_Animation, 1005 },
            { ResourceManager::Resource_Animation, 1011 }
        };
        ResourceManager::LoadResourcesFromList_446E80("BOMB.BND", kBombResources.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kExplodeResources =
        {
            { ResourceManager::Resource_Animation, 13 },
            { ResourceManager::Resource_Animation, 1105 },
            { ResourceManager::Resource_Animation, 300 }
        };
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kExplodeResources.AsList(), loadMode, 0);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 13, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1005, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1011, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 300, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResource & 1) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (disabledResource & 2 || ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0))
        {
            auto pTimedMine = ao_new<TimedMine>();
            if (pTimedMine)
            {
                pTimedMine->ctor_4083F0(pTimedMineTlv, tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Null_482EA0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Slig_482EC0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Slog_485030(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<4> kResources = 
        {
            { ResourceManager::Resource_Animation, 570 },
            { ResourceManager::Resource_Animation, 571 },
            { ResourceManager::Resource_Animation, 572 },
            { ResourceManager::Resource_Animation, 574 }
        };
        ResourceManager::LoadResourcesFromList_446E80("SLOG.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGKNFD.BAN", ResourceManager::Resource_Animation, 573, loadMode, 0);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 570, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 571, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 572, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 573, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 574, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pSlog = ao_new<Slog>();
        if (pSlog)
        {
            pSlog->ctor_472EE0(static_cast<Path_Slog*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Switch_485370(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eStockYards_5:
        case LevelIds::eStockYardsReturn_6:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1SWITCH.BAN", ResourceManager::Resource_Animation, 1009, loadMode);
            break;

        case LevelIds::eLines_2:
            ResourceManager::LoadResource_446C90("L1SWITCH.BAN", ResourceManager::Resource_Animation, 1009, loadMode);
            break;

        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResource_446C90("SWITCH1.BAN", ResourceManager::Resource_Animation, 1009, loadMode);
            break;

        default:
            ResourceManager::LoadResource_446C90("SWITCH1.BAN", ResourceManager::Resource_Animation, 1009, loadMode);
            break;
        }
        ResourceManager::LoadResource_446C90("ABEPULL.BAN", ResourceManager::Resource_Animation, 11, loadMode);
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


EXPORT void Factory_BellHammer_4854B0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1019, 0, 0))
        {
            auto pBellHammer = ao_new<BellHammer>();
            if (pBellHammer)
            {
                pBellHammer->ctor_405010(static_cast<Path_BellHammer*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_StartController_Null_4817E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SecurityOrb_485550(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pSecurityOrbTlv = static_cast<Path_SecurityOrb*>(pTlv);
    const auto disabledResources = pSecurityOrbTlv->field_1A_disable_resources;
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("F2MAMORB.BAN", ResourceManager::Resource_Animation, 2006, loadMode, 0);
        ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, 355, loadMode, 0);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, 217, loadMode, disabledResources & 4);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode, disabledResources & 0x10);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode, disabledResources & 0x20);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2006, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 1) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 2) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 0x10) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 0x20) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pSecurityEye = ao_new<SecurityOrb>();
        if (pSecurityEye)
        {
            pSecurityEye->ctor_436C80(pSecurityOrbTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Null_4817F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_487070(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_LiftMud_4857D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, 55, loadMode);
        ResourceManager::LoadResource_446C90("ABELIFT.BAN", ResourceManager::Resource_Animation, 53, loadMode);
    }
    else 
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 0, 0) && 
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 53, 0, 0))
        {
            auto pMud = ao_new<Mudokon>();
            if (pMud)
            {
                pMud->ctor_43EED0(pTlv, tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Null_482EB0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BeeSwarmHole_485E20(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEWASP.BAN", ResourceManager::Resource_Animation, 16, loadMode, 0);
        ResourceManager::LoadResource_446C90("WASP.BAN", ResourceManager::Resource_Animation, 61, loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eForest_3 || gMap_507BA8.field_0_current_level == LevelIds::eDesert_8)
        {
            ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, 204, loadMode, 0);
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


EXPORT void Factory_Pulley_Null_481800(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_HoneySack_485EF0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("HONEY.BAN", ResourceManager::Resource_Animation, 337, loadMode);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1013, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 61, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 16, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 337, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pHoneySack = ao_new<HoneySack>();
        if (pHoneySack)
        {
            pHoneySack->ctor_42BD10(static_cast<Path_HoneySack*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_AbeStart_486050(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 loadmode)
{
    if (loadmode != 1 && loadmode != 2 && !gAttract_507698)
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


EXPORT void Factory_ElumStart_Null_481810(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_486140(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ElumWall_487370(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ELMKNBK.BAN", ResourceManager::Resource_Animation, 215, loadMode, 0);
        ResourceManager::LoadResource_446C90("ANEKNBK.BAN", ResourceManager::Resource_Animation, 106, loadMode, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_SlingMud_485A30(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 508, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 314, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
        else
        {
            auto pSlingMud = ao_new<SlingMudokon>();
            if (pSlingMud)
            {
                pSlingMud->ctor_46F940(static_cast<Path_SlingMud*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
    }
}


EXPORT void Factory_HoneyDrip_Null_481820(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    // Empty
}

EXPORT void Factory_BeeNest_486150(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<2> kResources =
        {
           { ResourceManager::Resource_Animation, 61 }, 
           { ResourceManager::Resource_Animation, 16 }
        };

        ResourceManager::LoadResourcesFromList_446E80("SWARMHOL.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("ELMWASP.BAN", ResourceManager::Resource_Animation, 204, loadMode);
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


EXPORT void Factory_Null_487080(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Well_4834A0(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    Factory_WellExpress_483340(pTlv, pMap, tlvOffsetLevelIdPathId, loadMode);
}


EXPORT void Factory_Mine_4848D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pMineTlv = static_cast<Path_Mine*>(pTlv);
    const auto disabledResources = pMineTlv->field_1E_disabled_resources;

    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, 217, loadMode, disabledResources & 4);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("LNDMNPAL.BAN", ResourceManager::Resource_Palt, 1036, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, 25, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, 576, loadMode);
        }

        static CompileTimeResourceList<2> kResourcesMine =
        {
            { ResourceManager::Resource_Animation, 1036 },
            { ResourceManager::Resource_Animation, 1040 }
        };
        ResourceManager::LoadResourcesFromList_446E80("MINE.BND", kResourcesMine.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kResourcesExplode =
        {
            { ResourceManager::Resource_Animation, 13 },
            { ResourceManager::Resource_Animation, 1105 },
            { ResourceManager::Resource_Animation, 300 }
        };
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kResourcesExplode.AsList(), loadMode, 0);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 13, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1036, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1040, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 300, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 1) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 2) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pMine = ao_new<Mine>();
        if (pMine)
        {
            pMine->ctor_43A330(pMineTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_Uxb_484B70(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pUxbTlv = static_cast<Path_UXB*>(pTlv);

    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, pUxbTlv->field_20_disabled_resources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode, pUxbTlv->field_20_disabled_resources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, 217, loadMode, pUxbTlv->field_20_disabled_resources & 4);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("TBMBPAL.BAN", ResourceManager::Resource_Palt, 1037, loadMode);
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, 25, loadMode);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, 576, loadMode);
        }

        static CompileTimeResourceList<3> kResourcesUxb =
        {
            { ResourceManager::Resource_Animation, 1037 },
            { ResourceManager::Resource_Animation, 1011 },
            { ResourceManager::Resource_Palt, 1006 }
        };
        ResourceManager::LoadResourcesFromList_446E80("UXB.BND", kResourcesUxb.AsList(), loadMode, 0);

        static CompileTimeResourceList<3> kResourcesExplode =
        {
            { ResourceManager::Resource_Animation, 13 },
            { ResourceManager::Resource_Animation, 1105 },
            { ResourceManager::Resource_Animation, 300 }
        };
        ResourceManager::LoadResourcesFromList_446E80("EXPLODE.BND", kResourcesExplode.AsList(), loadMode, 0);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 13, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1037, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1011, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 300, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(pUxbTlv->field_20_disabled_resources & 1) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(pUxbTlv->field_20_disabled_resources & 2) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;

        }

        auto pUxb = ao_new<UXB>();
        if (pUxb)
        {
            pUxb->ctor_488C80(pUxbTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_Paramite_4861F0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<9> kResources =
        {
            { ResourceManager::Resource_Animation, 600 },
            { ResourceManager::Resource_Animation, 605 },
            { ResourceManager::Resource_Animation, 614 },
            { ResourceManager::Resource_Animation, 604 },
            { ResourceManager::Resource_Animation, 609 },
            { ResourceManager::Resource_Animation, 601 },
            { ResourceManager::Resource_Animation, 615 },
            { ResourceManager::Resource_Animation, 610 },
            { ResourceManager::Resource_Animation, 2034 }
        };
        ResourceManager::LoadResourcesFromList_446E80("PARAMITE.BND", kResources.AsList(), loadMode, 0);
        return;
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 600, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 605, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 614, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 604, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 609, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 601, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 615, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 610, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2034, 0, 0))
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        return;
    }

    auto pParamite = ao_new<Paramite>();
    if (pParamite)
    {
        pParamite->ctor_44A7A0(static_cast<Path_Paramite*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


EXPORT void Factory_Bat_486630(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("BATBASIC.BAN", ResourceManager::Resource_Animation, 63, loadMode);
    }
    else if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 63, 0, 0))
    {
        auto pBat = ao_new<Bat>();
        if (pBat)
        {
            pBat->ctor_4046E0(static_cast<Path_Bat*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_RingMud_4858F0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, 55, loadMode);
        ResourceManager::LoadResource_446C90("ABEPULL.BAN", ResourceManager::Resource_Animation, 11, loadMode);
        ResourceManager::LoadResource_446C90("ABEOMM.BAN", ResourceManager::Resource_Animation, 48, loadMode);
    }
    else 
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 11, 0, 0))
        {
            auto pMud = ao_new<Mudokon>();
            if (pMud)
            {
                pMud->ctor_43EED0(pTlv, tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_MovieStone_Null_487430(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BirdPortal_486710(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pBirdPortalTlv = static_cast<Path_BirdPortal*>(pTlv);
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources1 =
        {
            { ResourceManager::Resource_Animation, 313 },
            { ResourceManager::Resource_Animation, 351 },
            { ResourceManager::Resource_Animation, 353 }
        };
        ResourceManager::LoadResourcesFromList_446E80("PORTAL.BND", kResources1.AsList(), loadMode, 0);

        if (pBirdPortalTlv->field_24_portal_type == PortalType::eShrykull_2)
        {
            static CompileTimeResourceList<2> kResources2 =
            {
                { ResourceManager::Resource_Animation, 117 },
                { ResourceManager::Resource_Animation, 121 }
            };
            ResourceManager::LoadResourcesFromList_446E80("SHRYPORT.BND", kResources2.AsList(), loadMode, 0);
            ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, 355, loadMode);
        }
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 313, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (pBirdPortalTlv->field_24_portal_type != PortalType::eShrykull_2)
        {
            auto pBirdPortal = ao_new<BirdPortal>();
            if (pBirdPortal)
            {
                pBirdPortal->ctor_4520A0(pBirdPortalTlv, tlvOffsetLevelIdPathId.all);
            }
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 117, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 121, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 0, 0))
        {
            auto pBirdPortal = ao_new<BirdPortal>();
            if (pBirdPortal)
            {
                pBirdPortal->ctor_4520A0(pBirdPortalTlv, tlvOffsetLevelIdPathId.all);
            }
            return;
        }

        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_BirdPortalExit_486700(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BellSong_487450(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_TrapDoor_4868E0(Path_TLV* pTlv, Map* pMap, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        switch (gMap_507BA8.field_0_current_level)
        {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
            ResourceManager::LoadResource_446C90("R1TRAP.BAN", ResourceManager::Resource_Animation, 1004, loadMode, 0);
            break;
        case LevelIds::eLines_2:
            ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, 1004, loadMode, 0);
            break;
        case LevelIds::eStockYards_5:
            ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, 1004, loadMode, 0);
            break;
        case LevelIds::eDesert_8:
        case LevelIds::eDesertTemple_9:
        case LevelIds::eDesertEscape:
            ResourceManager::LoadResource_446C90("D2TRAP.BAN", ResourceManager::Resource_Animation, 1004, loadMode, 0);
            break;
        default:
            ResourceManager::LoadResource_446C90("P6C1TRAP.BAN", ResourceManager::Resource_Animation, 1004, loadMode, 0);
            break;
        }
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1004, 0, 0))
        {
            auto pTrapDoor = ao_new<TrapDoor>();
            if (pTrapDoor)
            {
                pTrapDoor->ctor_488010(static_cast<Path_TrapDoor*>(pTlv), pMap, tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_RollingBall_486A60(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("F2STNBAL.BAN", ResourceManager::Resource_Animation, 2002, loadMode);
        ResourceManager::LoadResource_446C90("DEBRIS00.BAN", ResourceManager::Resource_Animation, 1105, loadMode);
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2002, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 0, 0))
        {
            auto pRollingBall = ao_new<RollingBall>();
            if (pRollingBall)
            {
                pRollingBall->ctor_4578C0(static_cast<Path_RollingBall*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_SligLeftBound_482520(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void Factory_InvisibleZone_Null_481840(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void Factory_RollingBallStopper_486B90(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("F2STNSCK.BAN", ResourceManager::Resource_Animation, 2003, loadMode);
    }
    else if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2003, FALSE, FALSE))
    {
        auto pRollingBallStopper = ao_new<RollingBallStopper>();
        pRollingBallStopper->ctor_43BCE0(static_cast<Path_RollingBallStopper*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}

EXPORT void Factory_FootSwitch_486C60(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("TRIGGER.BAN", ResourceManager::Resource_Animation, 2010, loadMode);
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2010, 0, 0))
        {
            auto pFootSwitch = ao_new<FootSwitch>();
            if (pFootSwitch)
            {
                pFootSwitch->ctor_4887F0(static_cast<Path_FootSwitch*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}

EXPORT void Factory_SecurityClaw_486D50(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pSecurityClawTlv = static_cast<Path_SecurityClaw*>(pTlv);
    const auto disabledResources = pSecurityClawTlv->field_1E_disabled_resources;

    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("F2EYEORB.BAN", ResourceManager::Resource_Animation, 2008, loadMode);
        ResourceManager::LoadResource_446C90("SPLINE.BAN", ResourceManager::Resource_Animation, 355, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, disabledResources & 1);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode, disabledResources & 2);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, 217, loadMode, disabledResources & 4);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode, disabledResources & 0x10);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode, disabledResources & 0x20);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2008, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 355, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 1) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 2) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 0x10) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(disabledResources & 0x20) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }

        auto pSecurityOrb = ao_new<SecurityClaw>();
        if (pSecurityOrb)
        {
            pSecurityOrb->ctor_418A70(pSecurityClawTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_MotionDector_486FD0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 349, 0, 0))
        {
            auto pMotionDetector = ao_new<MotionDetector>();
            if (pMotionDetector)
            {
                pMotionDetector->ctor_437A50(static_cast<Path_MotionDetector*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Null_4870A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_481C70(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_4870B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Factory_SligSpawner_482A00(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pSligTlv = static_cast<Path_Slig*>(pTlv);
    const auto kDisabledResources = pSligTlv->field_50_disable_resources;

    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kSligZResources =
        { 
            { ResourceManager::Resource_Animation, 417 },
            { ResourceManager::Resource_Animation, 344 },
            { ResourceManager::Resource_Animation, 28 }
        };

        static CompileTimeResourceList<4> kSligResources =
        {
            { ResourceManager::Resource_Animation, 412 },
            { ResourceManager::Resource_Animation, 414 },
            { ResourceManager::Resource_Animation, 319 },
            { ResourceManager::Resource_Animation, 360 }
        };

        ResourceManager::LoadResource_446C90("SLGLEVER.BAN", ResourceManager::Resource_Animation, 419, loadMode, kDisabledResources & 1);
        ResourceManager::LoadResource_446C90("SLGLIFT.BAN", ResourceManager::Resource_Animation, 420, loadMode, kDisabledResources & 2);
        ResourceManager::LoadResource_446C90("SLGSLEEP.BAN", ResourceManager::Resource_Animation, 413, loadMode, kDisabledResources & 0x40);
        ResourceManager::LoadResource_446C90("SLGEDGE.BAN", ResourceManager::Resource_Animation, 415, loadMode, kDisabledResources & 0x100);
        ResourceManager::LoadResource_446C90("SLGSMASH.BAN", ResourceManager::Resource_Animation, 416, loadMode, kDisabledResources & 0x200);
        ResourceManager::LoadResource_446C90("SLGBEAT.BAN", ResourceManager::Resource_Animation, 426, loadMode, kDisabledResources & 0x400);
        ResourceManager::LoadResource_446C90("SLGKNFD.BAN", ResourceManager::Resource_Animation, 418, loadMode, kDisabledResources & 0x80);
        ResourceManager::LoadResourcesFromList_446E80("SLIGZ.BND", kSligZResources.AsList(), loadMode, kDisabledResources & 4);
        ResourceManager::LoadResourcesFromList_446E80("SLIG.BND", kSligResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("SLGBLOW.BAN", ResourceManager::Resource_Animation, 423, loadMode);
        ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);

        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1PAL.BAN", ResourceManager::Resource_Palt, 412, loadMode);
        }
    }
    else
    {
        if (!(kDisabledResources & 1) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 419, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(kDisabledResources & 2) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 420, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(kDisabledResources & 4))
        {
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 417, 0, 0))
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                return;
            }

            if (!(kDisabledResources & 4))
            {
                if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 344, 0, 0))
                {
                    gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                    return;
                }

                if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 28, 0, 0))
                {
                    gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                    return;
                }
            }
        }

        if (!(kDisabledResources & 0x40) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 413, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }
        
        if (!(kDisabledResources & 0x100) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 415, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(kDisabledResources & 0x200) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 416, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(kDisabledResources & 0x400) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 426, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if ((kDisabledResources & 0x80u) == 0 && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 418, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 319, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 360, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2035, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 423, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 412, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 414, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pSligSpawner = ao_new<SligSpawner>();
        if (pSligSpawner)
        {
            pSligSpawner->ctor_402850(pSligTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_ElectricWall_4874E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6000, 0, 0))
        {
            auto pElectricWall = ao_new<ElectricWall>();
            if (pElectricWall)
            {
                pElectricWall->ctor_40FCF0(static_cast<Path_ElectricWall*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_LiftMover_487580(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pLiftMover = ao_new<LiftMover>();
        if (pLiftMover)
        {
            pLiftMover->ctor_4054E0(static_cast<Path_LiftMover*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ChimeLock_4870D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2005, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2024, 0, 0))
        {
            auto pChimeLock = ao_new<ChimeLock>();
            if (pChimeLock)
            {
                pChimeLock->ctor_40AB20(static_cast<Path_ChimeLock*>(pTlv), tlvOffsetLevelIdPathId.all);
            }

        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}

EXPORT void Factory_ElumStart_Unknown_4873D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources({
            { ResourceManager::Resource_Animation, 200 },
            { ResourceManager::Resource_Animation, 230 },
            { ResourceManager::Resource_Animation, 216 },
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


EXPORT void Factory_MeatSack_483790(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources(
        { 
            { ResourceManager::Resource_Animation, 12 },
            { ResourceManager::Resource_Animation, 14 },
            { ResourceManager::Resource_Animation, 2023 } 
        });

        ResourceManager::LoadResourcesFromList_446E80("MTHROW.BND", kResources.AsList(), loadMode, 0);
        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("E1BAG.BAN", ResourceManager::Resource_Palt, 1002, loadMode);
        }
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 4002, 0, 0))
        {
            auto pMeatSack = ao_new<MeatSack>();
            if (pMeatSack)
            {
                pMeatSack->ctor_4390F0(static_cast<Path_MeatSack*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Scrab_4863E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<12> kResources(
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
        ResourceManager::LoadResourcesFromList_446E80("SCRAB.BND", kResources.AsList(), loadMode, 0);
        return;
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 710, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 708, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 706, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 700, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 705, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 709, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 704, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 711, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 701, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 702, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 703, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 713, 0, 0))
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        return;
    }

    auto pScrab = ao_new<Scrab>();
    if (pScrab)
    {
        pScrab->ctor_45B5F0(static_cast<Path_Scrab*>(pTlv), tlvOffsetLevelIdPathId.all);
    }
}


EXPORT void Factory_FlintLockFire_487640(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("FIRE.BAN", ResourceManager::Resource_Animation, 304, loadMode);
    }
    else 
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2017, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2028, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 304, 0, 0))
        {
            auto pFlintlockFire = ao_new<FlintLockFire>();
            if (pFlintlockFire)
            {
                pFlintlockFire->ctor_41AA90(static_cast<Path_FlintLockFire*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_ScrabLeftBound_Null_481860(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ScrabRightBound_Null_481870(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SligRightBound_4829D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SligPersist_Null_4829F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_4870C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_EnemyStopper_Null_481880(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_4871C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_InvisibleSwitch_481C10(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pInvisibleSwitch = ao_new<InvisibleSwitch>();
        if (pInvisibleSwitch)
        {
            pInvisibleSwitch->ctor_4334E0(static_cast<Path_InvisibleSwitch*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_WorkerMud_485B20(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pMudTlv = static_cast<Path_Mud*>(pTlv);
    if (loadMode != 1 && loadMode != 2)
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 55, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 10, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 27, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 26, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 43, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 510, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 514, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 517, 0, 0)
            )
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        switch (pMudTlv->field_1A_job)
        {
        case 0:
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 511, 0, 0))
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                return;
            }
            break;

        case 2:
            if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 516, 0, 0))
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
                return;
            }
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
        ResourceManager::LoadResource_446C90("ABEBSIC1.BAN", ResourceManager::Resource_Animation, 55, loadMode);
        ResourceManager::LoadResource_446C90("ABEBSIC.BAN", ResourceManager::Resource_Animation, 10, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNFD.BAN", ResourceManager::Resource_Animation, 27, loadMode);
        ResourceManager::LoadResource_446C90("ABEKNBK.BAN", ResourceManager::Resource_Animation, 26, loadMode);
        ResourceManager::LoadResource_446C90("ABEEDGE.BAN", ResourceManager::Resource_Animation, 43, loadMode);
        ResourceManager::LoadResource_446C90("SHADOW.BAN", ResourceManager::Resource_Animation, 2035, loadMode);

        switch (pMudTlv->field_1A_job)
        {
        case 0:
        {
            static CompileTimeResourceList<3> kResources =
            {
                { ResourceManager::Resource_Animation, 514 },
                { ResourceManager::Resource_Animation, 510 },
                { ResourceManager::Resource_Animation, 517 }
            };
            ResourceManager::LoadResource_446C90("MUDCHSL.BAN", ResourceManager::Resource_Animation, 511, loadMode);
            ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
        }
            break;

        case 1:
        {
            static CompileTimeResourceList<3> kResources =
            {
                { ResourceManager::Resource_Animation, 514 },
                { ResourceManager::Resource_Animation, 510 },
                { ResourceManager::Resource_Animation, 517 }
            };
            ResourceManager::LoadResourcesFromList_446E80("MUDWORK.BND", kResources.AsList(), loadMode, 0);
        }
            break;

        case 2:
        {
            static CompileTimeResourceList<4> kResources =
            {
                { ResourceManager::Resource_Animation, 516 },
                { ResourceManager::Resource_Animation, 514 },
                { ResourceManager::Resource_Animation, 510 },
                { ResourceManager::Resource_Animation, 517 }
            };
            ResourceManager::LoadResourcesFromList_446E80("MUDLOTUS.BND", kResources.AsList(), loadMode, 0);
        }
            break;
        }
    }
}


EXPORT void Factory_SligZCover_Null_481850(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_DoorFlame_481E80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("FIRE.BAN", ResourceManager::Resource_Animation, 304, loadMode, 0);
        ResourceManager::LoadResource_446C90("GLOW1.BAN", ResourceManager::Resource_Animation, 361, loadMode, 0);
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 304, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 361, 0, 0))
        {
            auto pDoorFlame = ao_new<DoorFlame>();
            if (pDoorFlame)
            {
                pDoorFlame->ctor_432860(static_cast<Path_DoorFlame*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_OneShotSwitchIdSetter_4871D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pSwitchStates = ao_new<OneShotSwitchIdSetter>();
        if (pSwitchStates)
        {
            pSwitchStates->ctor_432E10(static_cast<Path_OneShotSwitchIdSetter*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_MovingBomb_484E00(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pMovingBombTlv = static_cast<Path_MovingBomb*>(pTlv);

    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("D1MBOMB.BAN", ResourceManager::Resource_Animation, 3006, loadMode, 0);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode, 0);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode, 0);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode, pMovingBombTlv->field_22_disabled_resources & 1);
        ResourceManager::LoadResource_446C90("ELMBLOW.BAN", ResourceManager::Resource_Animation, 217, loadMode, pMovingBombTlv->field_22_disabled_resources & 4);

        if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
        {
            ResourceManager::LoadResource_446C90("ABEE1PAL.BAN", ResourceManager::Resource_Palt, 25, loadMode, 0);
            ResourceManager::LoadResource_446C90("DOGE1PAL.BAN", ResourceManager::Resource_Palt, 576, loadMode, 0);
        }
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 3006, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 0, 0) ||
            !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!(pMovingBombTlv->field_22_disabled_resources & 1) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pMovingBomb = ao_new<MovingBomb>();
        if (pMovingBomb)
        {
            pMovingBomb->ctor_43AFE0(pMovingBombTlv, tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_MovingBombStopper_Null_484DF0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MeatSaw_483F70(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("R1METSAW.BAN", ResourceManager::Resource_Animation, 6003, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode);
    }
    else 
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6003, 0, 0) &&
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0))
        {
            auto pMeatSaw = ao_new<MeatSaw>();
            if (pMeatSaw)
            {
                pMeatSaw->ctor_439570(static_cast<Path_MeatSaw*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}

EXPORT void Factory_MudPathTrans_Null_4818B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MenuController_481AC0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (gMainMenuInstanceCount_9F2DE0 == 0)
    {
        if (loadMode == 1 || loadMode == 2)
        {
             static CompileTimeResourceList<4> kResources =
            {
                { ResourceManager::Resource_Animation, 132 },
                { ResourceManager::Resource_Animation, 8002 },
                { ResourceManager::Resource_Animation, 367 },
                { ResourceManager::Resource_Palt, 8003 }
            };
            ResourceManager::LoadResourcesFromList_446E80("STARTANM.BND", kResources.AsList(), loadMode, 0);
        }
        else if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 132, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
        else if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 8002, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
        else if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 367, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
        else
        {
            auto pMenu = ao_new<Menu>();
            if (pMenu)
            {
                pMenu->ctor_47A6F0(pTlv, tlvOffsetLevelIdPathId.all);
            }
        }
    }
}


EXPORT void Factory_481FB0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_HintFly_4819E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pHintFly = ao_new<HintFly>();
        if (pHintFly)
        {
            pHintFly->ctor_42A820(static_cast<Path_HintFly*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_ScrabNoFall_Null_481890(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_IdSplitter_4875E0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pIdSplitter = ao_new<IdSplitter>();
        if (pIdSplitter)
        {
            pIdSplitter->ctor_479B40(static_cast<Path_IdSplitter*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_SecurityDoor_487790(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("R1SDOS.BAN", ResourceManager::Resource_Animation, 6027, loadMode);
    }
    else
    {
        if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6027, 0, 0))
        {
            auto pSecurityDoor = ao_new<SecurityDoor>();
            if (pSecurityDoor)
            {
                pSecurityDoor->ctor_461840(static_cast<Path_SecurityDoor*>(pTlv), tlvOffsetLevelIdPathId.all);
            }
        }
        else
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        }
    }
}


EXPORT void Factory_Null_487440(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_GrenadeMachine_487860(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pBoomMachineTlv = static_cast<Path_BoomMachine*>(pTlv);
    const auto disabledResources = pBoomMachineTlv->field_1C_disabled_resources;
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<3> kResources =
        {
            { ResourceManager::Resource_Animation, 14 },
            { ResourceManager::Resource_Animation, 12 },
            { ResourceManager::Resource_Animation, 6005 }
        };
        ResourceManager::LoadResourcesFromList_446E80("GTHROW.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode);
        ResourceManager::LoadResource_446C90("ABEBLOW.BAN", ResourceManager::Resource_Animation, 25, loadMode);
        ResourceManager::LoadResource_446C90("METAL.BAN", ResourceManager::Resource_Animation, 365, loadMode);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode, disabledResources & 2);
        return;
    }

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6008, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6009, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 21, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 14, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 12, 0, 0) || 
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6005, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0) ||
        !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 0, 0))
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        return;
    }

    if (!(disabledResources & 2) && !ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 576, 0, 0))
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
        return;
    }

    auto pGrenadeMachine = ao_new<BoomMachine>();
    if (pGrenadeMachine)
    {
        pGrenadeMachine->ctor_41E420(pBoomMachineTlv, tlvOffsetLevelIdPathId.all);
    }
}


EXPORT void Factory_LCD_481950(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, 2, loadMode);
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


EXPORT void Factory_ElumPathTrans_Null_4818C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_HandStone_487480(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("ABESTONE.BAN", ResourceManager::Resource_Animation, 21, loadMode);
        ResourceManager::LoadResource_446C90("SPOTLITE.BAN", ResourceManager::Resource_Animation, 316, loadMode);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
    }
}


EXPORT void Factory_CreditsController_481A50(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
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


EXPORT void Factory_Preloader_Null_4817A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_StatusBoard_487AF0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT", ResourceManager::Resource_Font, 2, loadMode);
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


EXPORT void Factory_SwitchStateBooleanLogic_487B80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pSwitchStateBooleanLogic = ao_new<SwitchStateBooleanLogic>();
        if (pSwitchStateBooleanLogic)
        {
            pSwitchStateBooleanLogic->ctor_436AB0(static_cast<Path_SwitchStateBooleanLogic*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_MusicTrigger_482020(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pMusicTrigger = ao_new<MusicTrigger>();
        if (pMusicTrigger)
        {
            pMusicTrigger->ctor_4439F0(static_cast<Path_MusicTrigger*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}

EXPORT void Factory_LightEffect_484170(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    auto pPathLightTlv = static_cast<Path_LightEffect*>(pTlv);
    if (loadMode == 1 || loadMode == 2)
    {
        switch (pPathLightTlv->field_18_type)
        {
        case Path_LightEffect::Type::Star_0:
            ResourceManager::LoadResource_446C90("STAR.BAN", ResourceManager::Resource_Animation, 1039, loadMode);
            break;

        case Path_LightEffect::Type::RedGoldGlow_1:
            ResourceManager::LoadResource_446C90("GOLDGLOW.BAN", ResourceManager::Resource_Animation, 6011, loadMode);
            break;

        case Path_LightEffect::Type::GreenGlow_2:
            ResourceManager::LoadResource_446C90("GRENGLOW.BAN", ResourceManager::Resource_Animation, 6010, loadMode);
            break;

        case Path_LightEffect::Type::FlintGlow_3:
            ResourceManager::LoadResource_446C90("FLNTGLOW.BAN", ResourceManager::Resource_Animation, 6028, loadMode);
            break;

        case Path_LightEffect::Type::FlintDoor_4:
            ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, 6031, loadMode);
            ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, 6032, loadMode);
            break;

        case Path_LightEffect::Type::FlintHub_5:
            ResourceManager::LoadResource_446C90("HUBLIGHT.BAN", ResourceManager::Resource_Animation, 6031, loadMode);
            ResourceManager::LoadResource_446C90("HUBRED.BAN", ResourceManager::Resource_Animation, 6032, loadMode);
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
            if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1039, 0, 0))
            {
                auto pLight = ao_new<LightEffect>();
                if (pLight)
                {
                    pLight->ctor_4064C0(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            }
            break;

        case Path_LightEffect::Type::RedGoldGlow_1:
            if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6011, 0, 0))
            {
                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            }
            break;

        case Path_LightEffect::Type::GreenGlow_2:
            if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6010, 0, 0))
            {
                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            }
            break;

        case Path_LightEffect::Type::FlintGlow_3:
            if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6028, 0, 0))
            {
                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            }
            break;

        case Path_LightEffect::Type::FlintDoor_4:
        case Path_LightEffect::Type::FlintHub_5:
            if (ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6031, 0, 0) &&
                ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6032, 0, 0))
            {
                auto pLight = ao_new<DoorLight>();
                if (pLight)
                {
                    pLight->ctor_405D90(pPathLightTlv, tlvOffsetLevelIdPathId.all);
                }
            }
            else
            {
                gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            }
            break;

        default:
            return;
        }
    }
}


EXPORT void Factory_SlogSpawner_4851D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<4> kResources =
        {
            { ResourceManager::Resource_Animation, 570 },
            { ResourceManager::Resource_Animation, 571 },
            { ResourceManager::Resource_Animation, 572 },
            { ResourceManager::Resource_Animation, 574 }
        };

        ResourceManager::LoadResourcesFromList_446E80("SLOG.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGKNFD.BAN", ResourceManager::Resource_Animation, 573, loadMode);
    }
    else
    {
        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 570, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 572, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 573, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 574, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 571, 0, 0))
        {
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 0);
            return;
        }

        auto pSlogSpawner = ao_new<SlogSpawner>();
        if (pSlogSpawner)
        {
            pSlogSpawner->ctor_475DD0(static_cast<Path_SlogSpawner*>(pTlv), tlvOffsetLevelIdPathId.all);
        }

    }
}


EXPORT void Factory_GasCountDown_487BE0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        ResourceManager::LoadResource_446C90("LCDFONT.FNT",  ResourceManager::Resource_Font, 2, loadMode);
        ResourceManager::LoadResource_446C90("ABEGAS.BAN", ResourceManager::Resource_Animation, 118, loadMode);
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

EXPORT void Factory_RingCancel_4818D0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
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


EXPORT void Factory_GasEmitter_484110(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pGasEmitter = ao_new<GasEmitter>();
        if (pGasEmitter)
        {
            pGasEmitter->ctor_41D760(static_cast<Path_GasEmitter*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_SlogHut_487C80(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        auto pSlogHut = ao_new<SlogHut>();
        if (pSlogHut)
        {
            pSlogHut->ctor_472C80(static_cast<Path_SlogHut*>(pTlv), tlvOffsetLevelIdPathId.all);
        }
    }
}


EXPORT void Factory_BackgroundGlukkon_487CE0(Path_TLV* pTlv, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode == 1 || loadMode == 2)
    {
        static CompileTimeResourceList<5> kResources =
        {
          { ResourceManager::Resource_Animation, 800 },
          { ResourceManager::Resource_Palt, 825 },
          { ResourceManager::Resource_Palt, 826 },
          { ResourceManager::Resource_Palt, 827 },
          { ResourceManager::Resource_Palt, 828 }
        };


        ResourceManager::LoadResourcesFromList_446E80("GLUKKON.BND", kResources.AsList(), loadMode, 0);
        ResourceManager::LoadResource_446C90("DOGBLOW.BAN", ResourceManager::Resource_Animation, 576, loadMode);
        ResourceManager::LoadResource_446C90("EXPLO2.BAN", ResourceManager::Resource_Animation, 301, loadMode);
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


EXPORT void Factory_KillUnsavedMuds_487DA0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion tlvOffsetLevelIdPathId, __int16 loadMode)
{
    if (loadMode != 1 && loadMode != 2)
    {
        if (!gbKillUnsavedMudsDone_5076CC)
        {
            gbKillUnsavedMudsDone_5076CC = 1;
            sKilledMudokons_5076BC = 28 - sRescuedMudokons_5076C0;
            gMap_507BA8.TLV_Reset_446870(tlvOffsetLevelIdPathId.all, -1, 0, 1);
        }
    }
}


EXPORT void Factory_SoftLanding_Null_4817C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ResetPath_Null_4818A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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
    Factory_BirdPortalExit_486700,
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


END_NS_AO
