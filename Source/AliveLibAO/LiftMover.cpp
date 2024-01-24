#include "stdafx_ao.h"
#include "LiftMover.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Factory.hpp"
#include "Path.hpp"

namespace AO {

LiftMover::LiftMover(relive::Path_LiftMover* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvId = tlvId;
    mTargetLift = nullptr;
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
}

LiftMover::~LiftMover()
{
    if (mTargetLift)
    {
        mTargetLift->mBaseGameObjectRefCount--;
        mTargetLift = nullptr;
    }
    Path::TLV_Reset(mTlvId);
}

void LiftMover::VUpdate()
{
    if (mTargetLift && mTargetLift->GetDead())
    {
        mTargetLift->mBaseGameObjectRefCount--;
        mTargetLift = nullptr;
        SetDead(true);
        return;
    }

    // NOTE: Isn't null checked, could easily crash later :)
    auto pLift = static_cast<LiftPoint*>(mTargetLift);

    switch (mState)
    {
        case LiftMoverStates::eInactive_0:
            if (SwitchStates_Get(mLiftMoverSwitchId))
            {
                if (pLift)
                {
                    mState = LiftMoverStates::eStartMovingDown_1;
                }
                else
                {
                    // Find the lift point
                    mTargetLift = FindLiftPointWithId(mTargetLiftPointId);

                    if (!mTargetLift)
                    {
                        // Load lift point objects (I guess in case for some reason it got unloaded ??)
                        // AE doesn't do this.
                        for (s16 y = 0; y < gMap.mMaxCamsY; y++)
                        {
                            for (s16 x = 0; x < gMap.mMaxCamsX; x++)
                            {
                                gMap.Loader(x, y, LoadMode::ConstructObject_0, ReliveTypes::eLiftPoint);
                            }
                        }

                        // And have another look now that we might have just loaded it in
                        mTargetLift = FindLiftPointWithId(mTargetLiftPointId);
                    }

                    if (mTargetLift)
                    {
                        mTargetLift->mBaseGameObjectRefCount++;
                        mState = LiftMoverStates::eStartMovingDown_1;
                    }
                }
            }
            break;

        case LiftMoverStates::eStartMovingDown_1:
            if (!pLift->OnAnyFloor())
            {
                pLift->mKeepOnMiddleFloor = true;
                mState = LiftMoverStates::eMovingDown_2;
            }
            else
            {
                pLift->Move(FP_FromInteger(0), mLiftSpeed);

                if ((mLiftSpeed > FP_FromInteger(0) && pLift->OnBottomFloor()) || (mLiftSpeed < FP_FromInteger(0) && pLift->OnTopFloor()))
                {
                    mState = LiftMoverStates::eMovingDown_2;
                }
            }
            break;

        case LiftMoverStates::eMovingDown_2:
            if (!pLift->OnAFloorLiftMoverCanUse())
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
            if (pLift->OnAFloorLiftMoverCanUse())
            {
                pLift->Move(FP_FromInteger(0), mLiftSpeed);

                if ((mLiftSpeed > FP_FromInteger(0) && pLift->OnBottomFloor()) || (mLiftSpeed < FP_FromInteger(0) && pLift->OnTopFloor()))
                {
                    mState = LiftMoverStates::eMovingDown_2;
                }
            }
            else
            {
                pLift->mKeepOnMiddleFloor = true;
                mState = LiftMoverStates::eMovingUp_4;
            }
            break;

        case LiftMoverStates::eMovingUp_4:
            if (pLift->OnAFloorLiftMoverCanUse())
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

LiftPoint* LiftMover::FindLiftPointWithId(s16 id)
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
            if (pLift->mLiftPointId == id)
            {
                return pLift;
            }
        }
    }
    return nullptr;
}

} // namespace AO
