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
    : BaseGameObject(1)
{
    mTypeId = Types::eBeeNest_96;

    field_1C_tlvInfo = tlvInfo;

    field_20_top_left = pTlv->field_10_top_left;
    field_18_level = gMap.mCurrentLevel;
    field_24_bottom_right = pTlv->field_14_bottom_right;

    field_2A_swarm_size = pTlv->field_1A_swarm_size;
    field_1A_path = gMap.mCurrentPath;

    field_28_switch_id = pTlv->field_18_id;

    field_10_bee_x = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_14_bee_y = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_2C_chase_ticks = pTlv->field_1C_chase_time;

    field_30_speed = FP_FromRaw(pTlv->field_1E_speed << 8);

    field_34_pBeeSwarm = nullptr;

    field_2E_state = BeeNestStates::eWaitForTrigger_0;

    if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
    {
        ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
    }

    // The "idle" swarm that hovers around the nest
    ao_new<BeeSwarm>(field_10_bee_x, field_14_bee_y, FP_FromInteger(0), pTlv->field_22_num_bees, 0);
}

BeeNest::~BeeNest()
{
    if (field_34_pBeeSwarm)
    {
        field_34_pBeeSwarm->field_C_refCount--;
    }
}

void BeeNest::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || !field_34_pBeeSwarm)
    {
        gMap.TLV_Reset(field_1C_tlvInfo, -1, 0, 0);
        if (field_34_pBeeSwarm)
        {
            field_34_pBeeSwarm->field_C_refCount--;
            field_34_pBeeSwarm = nullptr;
        }
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BeeNest::VUpdate()
{
    switch (field_2E_state)
    {
        case BeeNestStates::eWaitForTrigger_0:
            if (SwitchStates_Get(field_28_switch_id))
            {
                field_34_pBeeSwarm = ao_new<BeeSwarm>(
                    field_10_bee_x,
                    field_14_bee_y,
                    field_30_speed,
                    field_2A_swarm_size,
                    field_2C_chase_ticks);
                if (field_34_pBeeSwarm)
                {
                    field_34_pBeeSwarm->field_C_refCount++;
                    field_34_pBeeSwarm->Chase(sActiveHero);
                    field_2E_state = BeeNestStates::eResetIfDead_1;
                }
            }
            break;

        case BeeNestStates::eResetIfDead_1:
            if (field_34_pBeeSwarm->mGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_2E_state = BeeNestStates::eWaitForTrigger_0;
                field_34_pBeeSwarm->field_C_refCount--;
                field_34_pBeeSwarm = nullptr;
                SwitchStates_Set(field_28_switch_id, 0);
            }
            break;

        default:
            break;
    }
}

} // namespace AO
