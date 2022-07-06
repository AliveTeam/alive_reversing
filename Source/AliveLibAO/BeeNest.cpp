#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeNest.hpp"
#include "ResourceManager.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Abe.hpp"

namespace AO {

BeeNest::BeeNest(Path_BeeNest* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eBeeNest;

    field_1C_tlvInfo = tlvInfo;

    field_20_top_left = pTlv->mTopLeft;
    field_18_level = gMap.mCurrentLevel;
    field_24_bottom_right = pTlv->mBottomRight;

    field_2A_swarm_size = pTlv->field_1A_swarm_size;
    field_1A_path = gMap.mCurrentPath;

    field_28_switch_id = pTlv->field_18_id;

    field_10_bee_x = FP_FromInteger(pTlv->mTopLeft.x);
    field_14_bee_y = FP_FromInteger(pTlv->mTopLeft.y);

    field_2C_chase_ticks = pTlv->field_1C_chase_time;

    field_30_speed = FP_FromRaw(pTlv->field_1E_speed << 8);

    field_34_pBeeSwarm = nullptr;

    field_2E_state = BeeNestStates::eWaitForTrigger_0;

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
    }

    // The "idle" swarm that hovers around the nest
    relive_new BeeSwarm(field_10_bee_x, field_14_bee_y, FP_FromInteger(0), pTlv->field_22_num_bees, 0);
}

BeeNest::~BeeNest()
{
    if (field_34_pBeeSwarm)
    {
        field_34_pBeeSwarm->mBaseGameObjectRefCount--;
    }
}

void BeeNest::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || !field_34_pBeeSwarm)
    {
        gMap.TLV_Reset(field_1C_tlvInfo, -1, 0, 0);
        if (field_34_pBeeSwarm)
        {
            field_34_pBeeSwarm->mBaseGameObjectRefCount--;
            field_34_pBeeSwarm = nullptr;
        }
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BeeNest::VUpdate()
{
    switch (field_2E_state)
    {
        case BeeNestStates::eWaitForTrigger_0:
            if (SwitchStates_Get(field_28_switch_id))
            {
                field_34_pBeeSwarm = relive_new BeeSwarm(
                    field_10_bee_x,
                    field_14_bee_y,
                    field_30_speed,
                    field_2A_swarm_size,
                    field_2C_chase_ticks);
                if (field_34_pBeeSwarm)
                {
                    field_34_pBeeSwarm->mBaseGameObjectRefCount++;
                    field_34_pBeeSwarm->Chase(sActiveHero_507678);
                    field_2E_state = BeeNestStates::eResetIfDead_1;
                }
            }
            break;

        case BeeNestStates::eResetIfDead_1:
            if (field_34_pBeeSwarm->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_2E_state = BeeNestStates::eWaitForTrigger_0;
                field_34_pBeeSwarm->mBaseGameObjectRefCount--;
                field_34_pBeeSwarm = nullptr;
                SwitchStates_Set(field_28_switch_id, 0);
            }
            break;

        default:
            break;
    }
}

} // namespace AO
