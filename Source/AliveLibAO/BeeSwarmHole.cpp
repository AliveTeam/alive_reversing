#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeSwarmHole.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"

namespace AO {

BeeSwarmHole::BeeSwarmHole(Path_BeeSwarmHole* pTlv, s32 tlvInfo)
    : BaseGameObject(1)
{
    field_10_tlvInfo = tlvInfo;
    mBaseGameObjectTypeId = Types::eSligSpawner_91;

    field_1C_interval_timer = 0;

    field_14_rect.x = pTlv->field_10_top_left.field_0_x;
    field_14_rect.y = pTlv->field_10_top_left.field_2_y;

    field_14_rect.w = pTlv->field_14_bottom_right.field_0_x;
    field_14_rect.h = pTlv->field_14_bottom_right.field_2_y;

    field_20_interval = pTlv->field_1A_interval;
    field_22_fall_switch_id = pTlv->field_1C_fall_switch_id;
    field_24_movement_type = pTlv->field_1E_movement_type;

    field_26_bees_amount = pTlv->field_20_bees_amount;
    field_28_chase_time = pTlv->field_22_chase_time;
    field_2A_speed = pTlv->field_24_speed;
}

void BeeSwarmHole::VUpdate()
{
    if (gMap.Rect_Location_Relative_To_Active_Camera(&field_14_rect, 0) != CameraPos::eCamCurrent_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        gMap.TLV_Reset(field_10_tlvInfo, -1, 0, 0);
    }

    if (static_cast<s32>(sGnFrame) < field_1C_interval_timer)
    {
        return;
    }

    field_1C_interval_timer = static_cast<s32>(sGnFrame) + field_20_interval;

    bool bBallFillingHole = false;
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(idx);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectTypeId == Types::eRollingBall_72)
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObjIter);
            PSX_RECT bRect = {};
            pAliveObj->VGetBoundingRect(&bRect, 1);
            if (field_14_rect.x >= bRect.x && field_14_rect.x <= bRect.w)
            {
                if (field_14_rect.y >= bRect.y && field_14_rect.y <= bRect.h)
                {
                    bBallFillingHole = true;
                    break;
                }
            }
        }
    }

    if (!bBallFillingHole)
    {
        const FP speed = FP_FromRaw(field_2A_speed << 8);
        auto pSwarm = ao_new<BeeSwarm>(
            FP_FromInteger(field_14_rect.x),
            FP_FromInteger(field_14_rect.y),
            speed,
            field_26_bees_amount,
            field_28_chase_time);
        if (pSwarm)
        {
            switch (field_24_movement_type)
            {
            case Path_BeeSwarmHole::MovementType::eHover_0:
                // Idle around like a chav near a victim
                break;

            case Path_BeeSwarmHole::MovementType::eAttack_1:
                pSwarm->Chase(sActiveHero_507678);
                break;

            case Path_BeeSwarmHole::MovementType::eFollowPath_2:
            {
                PathLine* pLine = nullptr;
                FP targetX = {};
                FP targetY = {};
                if (sCollisions_DArray_504C6C->RayCast(
                    FP_FromInteger(field_14_rect.x),
                    FP_FromInteger(field_14_rect.y),
                    FP_FromInteger(field_14_rect.w),
                    FP_FromInteger(field_14_rect.h),
                    &pLine,
                    &targetX,
                    &targetY,
                    0x100)
                    == 1)
                {
                    pSwarm->FollowLine(pLine, targetX, targetY, speed + FP_FromInteger(1));
                }
            }
            break;

            default:
                break;
            }
        }
    }
}

} // namespace AO
