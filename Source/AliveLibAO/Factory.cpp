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


EXPORT void Factory_Hoist_487230(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Edge_481730(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_DeathDrop_Null_4817B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Door_481C80(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ShadowZone_482080(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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


EXPORT void Factory_WellExpress_483340(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Dove_4834C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_RockSack_483680(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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


EXPORT void Factory_FallingItem_483940(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_482E90(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_482E80(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_PullRingRope_483DA0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BackgroundAnimation_4840A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Honey_4844A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_484640(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_TimedMine_484650(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_482EA0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Slig_482EC0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Slog_485030(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Switch_485370(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BellHammer_4854B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_StartController_Null_4817E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SecurityEye_485550(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_4817F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_487070(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_LiftMud_4857D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_482EB0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SwarmHole_485E20(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Pulley_Null_481800(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_HoneySack_485EF0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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
                pPauseMenu_5080E0->ctor_48FB80();
            }
        }

        if (!sActiveHero_5C1B68)
        {
            sActiveHero_5C1B68 = ao_new<Abe>();
            if (sActiveHero_5C1B68)
            {
                sActiveHero_5C1B68->ctor_420770(58808, 85, 57, 55);
                sActiveHero_5C1B68->field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
                sActiveHero_5C1B68->field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
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


EXPORT void Factory_ElumWall_487370(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SlingMud_485A30(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_HoneyDrip_Null_481820(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Bees_486150(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_487080(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Well_4834A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Mine_4848D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Uxb_484B70(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Paramite_4861F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Bat_486630(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_RingMud_4858F0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MovieStone_Null_487430(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BirdPortal_486710(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BirdPortalExit_486700(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_BellSong_487450(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_TrapDoor_4868E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_RollingBall_486A60(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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

EXPORT void Factory_FootSwitch_486C60(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void Factory_SecurityOrb_486D50(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MotionDector_486FD0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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

EXPORT void Factory_ElectricWall_4874E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_LiftMover_487580(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ChimeLock_4870D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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


EXPORT void Factory_MeatSack_483790(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Scrab_4863E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_FlintLockFire_487640(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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


EXPORT void Factory_InvisibleSwitch_481C10(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_WorkerMud_485B20(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SligZCover_Null_481850(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_DoorFlame_481E80(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SwitchStates_4871D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MovingBomb_484E00(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MovingBombStopper_Null_484DF0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MeatSaw_483F70(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MudPathTrans_Null_4818B0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_MenuController_481AC0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_481FB0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_HintFly_4819E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ScrabNoFall_Null_481890(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_IdSplitter_4875E0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SecurityDoor_487790(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Null_487440(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_GrenadeMachine_487860(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_LCD_481950(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_ElumPathTrans_Null_4818C0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_HandStone_487480(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_CreditsController_481A50(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Preloader_Null_4817A0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_StatusBoard_487AF0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_487B80(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Music_482020(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_LightEffect_484170(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SlogSpawner_4851D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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


EXPORT void Factory_RingCancel_4818D0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_GasEmitter_484110(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_SlogHut_487C80(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_Glukkon_487CE0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void Factory_KillUnsavedMuds_487DA0(Path_TLV* /*pTlv*/, Map* /*pMap*/, TlvItemInfoUnion /*tlvOffsetLevelIdPathId*/, __int16 /*loadMode*/)
{
    NOT_IMPLEMENTED();
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
    Factory_SecurityEye_485550,
    Factory_Null_4817F0,
    Factory_Null_487070,
    Factory_LiftMud_4857D0,
    Factory_Null_482EB0,
    Factory_SwarmHole_485E20,
    Factory_Pulley_Null_481800,
    Factory_HoneySack_485EF0,
    Factory_AbeStart_486050,
    Factory_ElumStart_Null_481810,
    Factory_Null_486140,
    Factory_ElumWall_487370,
    Factory_SlingMud_485A30,
    Factory_HoneyDrip_Null_481820,
    Factory_Bees_486150,
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
    Factory_SecurityOrb_486D50,
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
    Factory_SwitchStates_4871D0,
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
    Factory_487B80,
    Factory_Music_482020,
    Factory_LightEffect_484170,
    Factory_SlogSpawner_4851D0,
    Factory_GasCountDown_487BE0,
    Factory_RingCancel_4818D0,
    Factory_GasEmitter_484110,
    Factory_SlogHut_487C80,
    Factory_Glukkon_487CE0,
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
