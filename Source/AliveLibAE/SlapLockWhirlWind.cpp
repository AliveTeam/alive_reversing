#include "stdafx.h"
#include "SlapLockWhirlWind.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "OrbWhirlWind.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Map.hpp"
#include "Path.hpp"

s32 SlapLockWhirlWind::CreateFromSaveState(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const SlapLockWhirlWindSaveState*>(pBuffer);
    SwitchStates_Do_Operation(pSaveState->mSwitchId, relive::reliveSwitchOp::eSetTrue);
    return sizeof(SlapLockWhirlWindSaveState);
}

SlapLockWhirlWind::SlapLockWhirlWind(s16 doorNumber, s16 switchId, FP xpos, FP ypos, FP scale)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eSlapLock_OrbWhirlWind);

    mSwitchId = switchId;

    bool bFoundTarget = false;
    for (s16 y = 0; y < gPathInfo->mCamsOnY; y++)
    {
        for (s16 x = 0; x < gPathInfo->mCamsOnX; x++)
        {
            relive::Path_Door* pDoorTlv = static_cast<relive::Path_Door*>(gPathInfo->Get_First_TLV_For_Offsetted_Camera(
                x - gMap.mCamIdxOnX,
                y - gMap.mCamIdxOnY));
            while (pDoorTlv)
            {
                if (pDoorTlv->mTlvType == ReliveTypes::eDoor && pDoorTlv->mDoorId == doorNumber)
                {
                    // For some reason once found we just keep on searching...
                    bFoundTarget = true;

                    mDoorX = FP_FromInteger((pDoorTlv->mTopLeftX + pDoorTlv->mBottomRightX) / 2);
                    mDoorY = FP_FromInteger((pDoorTlv->mTopLeftY + pDoorTlv->mBottomRightY) / 2);

                    if (pDoorTlv->mScale != relive::reliveScale::eFull)
                    {
                        mDoorSpriteScale = FP_FromDouble(0.5);
                    }
                    else
                    {
                        mDoorSpriteScale = FP_FromInteger(1);
                    }

                    mDoorY -= (FP_FromInteger(40) * mDoorSpriteScale);
                }
                pDoorTlv = static_cast<relive::Path_Door*>(gPathInfo->Next_TLV(pDoorTlv));
            }
        }
    }

    if (bFoundTarget)
    {
        auto pWhirlWind = relive_new OrbWhirlWind(xpos, ypos, scale, 1);
        if (pWhirlWind)
        {
            mOrbWhirlWindId = pWhirlWind->mBaseGameObjectId;
        }
        mState = 0;
        mTimer = sGnFrame + 70;
    }
    else
    {
        SetDead(true);
    }
}

s32 SlapLockWhirlWind::VGetSaveState(u8* pSaveBuffer)
{
    SlapLockWhirlWindSaveState* pSaveState = reinterpret_cast<SlapLockWhirlWindSaveState*>(pSaveBuffer);
    pSaveState->mType = ReliveTypes::eSlapLock_OrbWhirlWind;
    pSaveState->mSwitchId = mSwitchId;
    return sizeof(SlapLockWhirlWindSaveState);
}

void SlapLockWhirlWind::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));
        if (mState == 1)
        {
            if (!(static_cast<s32>(sGnFrame) % 10))
            {
                SFX_Play_Pitch(
                    relive::SoundEffects::FlyingSpirit2,
                    static_cast<s16>(127 - (static_cast<s32>(sGnFrame) - mTimer) / 2),
                    4 * (sGnFrame - mTimer));
            }

            if (!pWhirlWind || pWhirlWind->GetDead())
            {
                SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetTrue);
                SetDead(true);
            }
        }
        else if (mState == 0)
        {
            if (!(static_cast<s32>(sGnFrame) % 10))
            {
                if (static_cast<s32>(sGnFrame) % 20)
                {
                    SfxPlayMono(relive::SoundEffects::FlyingSpirit1, 0);
                }
                else
                {
                    SfxPlayMono(relive::SoundEffects::FlyingSpirit2, 0);
                }
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                if (pWhirlWind)
                {
                    pWhirlWind->ToSpin(mDoorX, mDoorY, mDoorSpriteScale, 0);
                }
                mState = 1;
            }
        }
    }
}

s16 SlapLockWhirlWind::SwitchId() const
{
    return mSwitchId;
}
