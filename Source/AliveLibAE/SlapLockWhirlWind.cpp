#include "stdafx.h"
#include "SlapLockWhirlWind.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "OrbWhirlWind.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Door.hpp"

s32 SlapLockWhirlWind::CreateFromSaveState(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const SlapLockWhirlWind_State*>(pBuffer);
    SwitchStates_Do_Operation(pSaveState->mSwitchId, SwitchOp::eSetTrue_0);
    return sizeof(SlapLockWhirlWind_State);
}

SlapLockWhirlWind::SlapLockWhirlWind(s16 doorNumber, s16 switchId, FP xpos, FP ypos, FP scale)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eSlapLock_OrbWhirlWind);

    mSwitchId = switchId;

    bool bFoundTarget = false;
    for (s16 y = 0; y < sPathInfo->mCamsOnY; y++)
    {
        for (s16 x = 0; x < sPathInfo->mCamsOnX; x++)
        {
            Path_Door* pDoorTlv = static_cast<Path_Door*>(sPathInfo->Get_First_TLV_For_Offsetted_Camera(
                x - gMap.mCamIdxOnX,
                y - gMap.mCamIdxOnY));
            while (pDoorTlv)
            {
                if (pDoorTlv->mTlvType32 == TlvTypes::Door_5 && pDoorTlv->mDoorId == doorNumber)
                {
                    // For some reason once found we just keep on searching...
                    bFoundTarget = true;

                    mDoorX = FP_FromInteger((pDoorTlv->mTopLeft.x + pDoorTlv->mBottomRight.x) / 2);
                    mDoorY = FP_FromInteger((pDoorTlv->mTopLeft.y + pDoorTlv->mBottomRight.y) / 2);

                    if (pDoorTlv->mScale != Scale_short::eFull_0)
                    {
                        mDoorSpriteScale = FP_FromDouble(0.5);
                    }
                    else
                    {
                        mDoorSpriteScale = FP_FromInteger(1);
                    }

                    mDoorY -= (FP_FromInteger(40) * mDoorSpriteScale);
                }
                pDoorTlv = static_cast<Path_Door*>(sPathInfo->Next_TLV(pDoorTlv));
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s32 SlapLockWhirlWind::VGetSaveState(u8* pSaveBuffer)
{
    SlapLockWhirlWind_State* pSaveState = reinterpret_cast<SlapLockWhirlWind_State*>(pSaveBuffer);
    pSaveState->mType = AETypes::eSlapLock_OrbWhirlWind_60;
    pSaveState->mSwitchId = mSwitchId;
    return sizeof(SlapLockWhirlWind_State);
}

void SlapLockWhirlWind::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(mOrbWhirlWindId));
        if (mState == 1)
        {
            if (!(static_cast<s32>(sGnFrame) % 10))
            {
                SFX_Play_Pitch(
                    SoundEffect::FlyingSpirit2_108,
                    static_cast<s16>(127 - (static_cast<s32>(sGnFrame) - mTimer) / 2),
                    4 * (sGnFrame - mTimer));
            }

            if (!pWhirlWind || pWhirlWind->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                SwitchStates_Do_Operation(mSwitchId, SwitchOp::eSetTrue_0);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        else if (mState == 0)
        {
            if (!(static_cast<s32>(sGnFrame) % 10))
            {
                if (static_cast<s32>(sGnFrame) % 20)
                {
                    SfxPlayMono(SoundEffect::FlyingSpirit1_107, 0);
                }
                else
                {
                    SfxPlayMono(SoundEffect::FlyingSpirit2_108, 0);
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
