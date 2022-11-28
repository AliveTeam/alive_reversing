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
    field_14_tlvInfo = tlvId;
    mTargetLift = nullptr;
    SetType(ReliveTypes::eLiftMover);

    field_10_lift_mover_switch_id = pTlv->mLiftMoverSwitchId;
    mTargetLiftPointId = pTlv->mTargetLiftPointId;

    if (pTlv->mMoveDirection == relive::Path_LiftMover::YDirection::eUp)
    {
        mLiftSpeed = FP_FromInteger(-4);
    }
    else
    {
        mLiftSpeed = FP_FromInteger(4);
    }

    mState = 0;
}

LiftMover::~LiftMover()
{
    if (mTargetLift)
    {
        mTargetLift->mBaseGameObjectRefCount--;
        mTargetLift = nullptr;
    }
    Path::TLV_Reset(field_14_tlvInfo, -1, 0, 0);
}

void LiftMover::VUpdate()
{
    if (mTargetLift && mTargetLift->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        mTargetLift->mBaseGameObjectRefCount--;
        mTargetLift = nullptr;
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    // NOTE: Isn't null checked, could easily crash later :)
    auto pLiftPoint = static_cast<LiftPoint*>(mTargetLift);

    switch (mState)
    {
        case 0:
            if (SwitchStates_Get(field_10_lift_mover_switch_id))
            {
                if (pLiftPoint)
                {
                    mState = 1;
                }
                else
                {
                    // Find the lift point
                    mTargetLift = FindLiftPointWithId(mTargetLiftPointId);

                    if (!mTargetLift)
                    {
                        // Load lift point objects (I guess in case for some reason it got unloaded ??)
                        // AE doesn't do this.
                        for (s16 y = 0; y < gMap.field_26_max_cams_y; y++)
                        {
                            for (s16 x = 0; x < gMap.field_24_max_cams_x; x++)
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
                        mState = 1;
                    }
                }
            }
            break;

        case 1:
            if (!pLiftPoint->OnAnyFloor())
            {
                pLiftPoint->field_27A_flags.Set(LiftPoint::Flags::eBit8_KeepOnMiddleFloor);
                mState = 2;
            }
            else
            {
                pLiftPoint->Move(FP_FromInteger(0), mLiftSpeed, 0);

                if ((mLiftSpeed > FP_FromInteger(0) && pLiftPoint->OnBottomFloor()) || (mLiftSpeed < FP_FromInteger(0) && pLiftPoint->OnTopFloor()))
                {
                    mState = 2;
                }
            }
            break;

        case 2:
            if (!pLiftPoint->OnAFloorLiftMoverCanUse())
            {
                pLiftPoint->Move(FP_FromInteger(0), mLiftSpeed, 0);
            }
            else
            {
                pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
                mState = 5;
            }
            break;

        case 3:
            if (pLiftPoint->OnAFloorLiftMoverCanUse())
            {
                pLiftPoint->Move(FP_FromInteger(0), mLiftSpeed, 0);

                if ((mLiftSpeed > FP_FromInteger(0) && pLiftPoint->OnBottomFloor()) || (mLiftSpeed < FP_FromInteger(0) && pLiftPoint->OnTopFloor()))
                {
                    mState = 2;
                }
            }
            else
            {
                pLiftPoint->field_27A_flags.Set(LiftPoint::Flags::eBit8_KeepOnMiddleFloor);
                mState = 4;
            }
            break;

        case 4:
            if (pLiftPoint->OnAFloorLiftMoverCanUse())
            {
                pLiftPoint->Move(FP_FromInteger(0), FP_FromInteger(0), 0);
                mState = 0;
                mLiftSpeed = -mLiftSpeed;
            }
            else
            {
                pLiftPoint->Move(FP_FromInteger(0), mLiftSpeed, 0);
            }
            break;

        case 5:
            if (!SwitchStates_Get(field_10_lift_mover_switch_id))
            {
                mState = 3;
                mLiftSpeed = -mLiftSpeed;
            }
            break;

        default:
            break;
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

LiftPoint* LiftMover::FindLiftPointWithId(s16 id)
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pItem = gBaseAliveGameObjects->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->Type() == ReliveTypes::eLiftPoint)
        {
            auto pLiftPoint = static_cast<LiftPoint*>(pItem);
            if (pLiftPoint->field_278_lift_point_id == id)
            {
                return pLiftPoint;
            }
        }
    }
    return nullptr;
}

} // namespace AO
