#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeSwarmHole.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

BeeSwarmHole::BeeSwarmHole(relive::Path_BeeSwarmHole* pTlv, const TLVUniqueId& tlvId)
    : BaseGameObject(TRUE, 0)
{
    field_10_tlvInfo = tlvId;
    mBaseGameObjectTypeId = ReliveTypes::eSligSpawner;

    mStartIntervalTimer = 0;

    field_14_rect.x = pTlv->mTopLeftX;
    field_14_rect.y = pTlv->mTopLeftY;

    field_14_rect.w = pTlv->mBottomRightX;
    field_14_rect.h = pTlv->mBottomRightY;

    mStartInterval = pTlv->mStartInterval;
    mMovementType = pTlv->mMovementType;

    field_26_bees_amount = pTlv->mBeesAmount;
    field_28_chase_time = pTlv->mChaseTime;
    field_2A_speed = pTlv->mSpeed;
}

void BeeSwarmHole::VUpdate()
{
    if (gMap.Rect_Location_Relative_To_Active_Camera(&field_14_rect, 0) != CameraPos::eCamCurrent_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        Path::TLV_Reset(field_10_tlvInfo, -1, 0, 0);
    }

    if (static_cast<s32>(sGnFrame) < mStartIntervalTimer)
    {
        return;
    }

    mStartIntervalTimer = static_cast<s32>(sGnFrame) + mStartInterval;

    bool bBallFillingHole = false;
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(idx);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eRollingBall)
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObjIter);
            const PSX_RECT bRect = pAliveObj->VGetBoundingRect();
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
        auto pSwarm = relive_new BeeSwarm(
            FP_FromInteger(field_14_rect.x),
            FP_FromInteger(field_14_rect.y),
            speed,
            field_26_bees_amount,
            field_28_chase_time);
        if (pSwarm)
        {
            switch (mMovementType)
            {
            case relive::Path_BeeSwarmHole::MovementType::eHover:
                // Idle around like a chav near a victim
                break;

            case relive::Path_BeeSwarmHole::MovementType::eAttack:
                pSwarm->Chase(sActiveHero);
                break;

            case relive::Path_BeeSwarmHole::MovementType::eFollowPath:
            {
                PathLine* pLine = nullptr;
                FP targetX = {};
                FP targetY = {};
                if (sCollisions->Raycast(
                    FP_FromInteger(field_14_rect.x),
                    FP_FromInteger(field_14_rect.y),
                    FP_FromInteger(field_14_rect.w),
                    FP_FromInteger(field_14_rect.h),
                    &pLine,
                    &targetX,
                    &targetY,
                    CollisionMask(eTrackLine_8))
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
