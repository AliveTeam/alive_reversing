#include "stdafx.h"
#include "LiftMover.hpp"
#include "../relive_lib/Function.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "Path.hpp"
#include "QuikSave.hpp"

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

LiftMover::~LiftMover()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
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
        return;
    }

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
                pLift->Move(FP_FromInteger(0), mLiftSpeed);
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
                pLift->Move(FP_FromInteger(0), mLiftSpeed);
            }
            else
            {
                pLift->Move(FP_FromInteger(0), FP_FromInteger(0));
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
                pLift->Move(FP_FromInteger(0), mLiftSpeed);
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
                pLift->vKeepOnMiddleFloor();
                mState = LiftMoverStates::eMovingUp_4;
            }
            break;

        case LiftMoverStates::eMovingUp_4:
            if (!pLift)
            {
                return;
            }

            if (pLift->vOnAFloorLiftMoverCanUse())
            {
                pLift->Move(FP_FromInteger(0), FP_FromInteger(0));

                mState = LiftMoverStates::eInactive_0;
                mLiftSpeed = -mLiftSpeed;
            }
            else
            {
                pLift->Move(FP_FromInteger(0), mLiftSpeed);
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

LiftPoint* LiftMover::GetLiftPoint()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eLiftPoint)
        {
            auto pLift = static_cast<LiftPoint*>(pObj);
            if (mTargetLiftPointId == pLift->mLiftPointId)
            {
                mTargetLiftId = pObj->mBaseGameObjectId;
                return pLift;
            }
        }
    }
    return nullptr;
}

void LiftMover::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    LiftMoverSaveState data = {};

    data.mType = ReliveTypes::eLiftMover;
    data.mTlvId = mTlvId;
    data.mState = mState;
    pSaveBuffer.Write(data);
}

void LiftMover::CreateFromSaveState(SerializedObjectData& pData)
{
    const auto pState = pData.ReadTmpPtr<LiftMoverSaveState>();

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
}
