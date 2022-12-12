#include "stdafx.h"
#include "LiftMover.hpp"
#include "Function.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "Path.hpp"

LiftMover::LiftMover(relive::Path_LiftMover* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvId = tlvId;
    mTargetLiftId = Guid{};
    SetType(ReliveTypes::eLiftMover);

    mLiftMoverSwitchId = pTlv->mLiftMoverSwitchId;
    mTargetLiftPointId = pTlv->mTargetLiftPointId;

    if (pTlv->mMoveDirection == relive::Path_LiftMover::YDirection::eUp)
    {
        mLiftSpeed = FP_FromInteger(-4);
    }
    else
    {
        mLiftSpeed = FP_FromInteger(4);
    }

    mState = LiftMoverStates::eInactive_0;
    mMoving = false;
}

s32 LiftMover::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const LiftMoverSaveState*>(pData);

    relive::Path_LiftMover* pTlv = static_cast<relive::Path_LiftMover*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvId));
    auto pLiftMover = relive_new LiftMover(pTlv, pState->mTlvId);
    if (pLiftMover)
    {
        if (pState->mState != LiftMoverStates::eInactive_0)
        {
            pLiftMover->mMoving = true;
        }
        pLiftMover->mState = pState->mState;
    }

    return sizeof(LiftMoverSaveState);
}

void LiftMover::VUpdate()
{
    LiftPoint* pLift = static_cast<LiftPoint*>(sObjectIds.Find(mTargetLiftId, ReliveTypes::eLiftPoint));
    if (mMoving)
    {
        pLift = GetLiftPoint();
        if (!pLift)
        {
            return;
        }
        mMoving = false;
    }

    if (pLift && pLift->GetDead())
    {
        // Set extra dead for good measure
        SetDead(true);
        mTargetLiftId = Guid{};
    }
    else
    {
        switch (mState)
        {
            case LiftMoverStates::eInactive_0:
                if (SwitchStates_Get(mLiftMoverSwitchId))
                {
                    if (!pLift)
                    {
                        pLift = GetLiftPoint();
                    }

                    if (pLift)
                    {
                        mTargetLiftId = pLift->mBaseGameObjectId;
                        mState = LiftMoverStates::eStartMovingDown_1;
                    }
                }
                break;

            case LiftMoverStates::eStartMovingDown_1:
                if (!pLift)
                {
                    return;
                }

                if (!pLift->vOnAnyFloor())
                {
                    mState = LiftMoverStates::eMovingDown_2;
                    pLift->vKeepOnMiddleFloor();
                }
                else
                {
                    pLift->Move(FP_FromInteger(0), mLiftSpeed, 0);
                    if ((mLiftSpeed > FP_FromInteger(0) && pLift->vOnBottomFloor()) || (mLiftSpeed < FP_FromInteger(0) && pLift->vOnTopFloor()))
                    {
                        mState = LiftMoverStates::eMovingDown_2;
                    }
                }
                break;

            case LiftMoverStates::eMovingDown_2:
                if (!pLift)
                {
                    return;
                }

                if (!pLift->vOnAFloorLiftMoverCanUse())
                {
                    pLift->Move(FP_FromInteger(0), mLiftSpeed, 0);
                }
                else
                {
                    pLift->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
                    mState = LiftMoverStates::eMovingDone_5;
                }
                break;

            case LiftMoverStates::eStartMovingUp_3:
                if (!pLift)
                {
                    return;
                }

                if (pLift->vOnAFloorLiftMoverCanUse())
                {
                    pLift->Move(FP_FromInteger(0), mLiftSpeed, 0);
                    if (mLiftSpeed < FP_FromInteger(0))
                    {
                        if (pLift->vOnTopFloor())
                        {
                            mState = LiftMoverStates::eMovingDown_2;
                        }
                    }

                    if (mLiftSpeed > FP_FromInteger(0))
                    {
                        if (pLift->vOnBottomFloor())
                        {
                            mState = LiftMoverStates::eMovingDown_2;
                        }
                    }
                }
                else
                {
                    mState = LiftMoverStates::eMovingUp_4;
                    pLift->vKeepOnMiddleFloor();
                }
                break;

            case LiftMoverStates::eMovingUp_4:
                if (!pLift)
                {
                    return;
                }

                if (pLift->vOnAFloorLiftMoverCanUse())
                {
                    pLift->Move(FP_FromInteger(0), FP_FromInteger(0), 0);

                    mState = LiftMoverStates::eInactive_0;
                    mLiftSpeed = -mLiftSpeed;
                }
                else
                {
                    pLift->Move(FP_FromInteger(0), mLiftSpeed, 0);
                }
                break;

            case LiftMoverStates::eMovingDone_5:
                if (!SwitchStates_Get(mLiftMoverSwitchId))
                {
                    mState = LiftMoverStates::eStartMovingUp_3;
                    mLiftSpeed = -mLiftSpeed;
                }
                break;

            default:
                break;
        }

        if (EventGet(kEventDeathReset))
        {
            SetDead(true);
        }
    }
}

LiftMover::~LiftMover()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

s32 LiftMover::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<LiftMoverSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eLiftMover;
    pState->mTlvId = mTlvId;
    pState->mState = mState;
    return sizeof(LiftMoverSaveState);
}

LiftPoint* LiftMover::GetLiftPoint()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eLiftPoint)
        {
            LiftPoint* pLift = static_cast<LiftPoint*>(pObj);
            if (mTargetLiftPointId == pLift->field_278_lift_point_id)
            {
                mTargetLiftId = pObj->mBaseGameObjectId;
                return pLift;
            }
        }
    }
    return nullptr;
}
