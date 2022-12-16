#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "HoistRocksEffect.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Math.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Path.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "Map.hpp"

namespace AO {

HoistParticle::HoistParticle(FP xpos, FP ypos, FP scale, AnimId animId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mXPos = xpos;
    mYPos = ypos;

    /*u16 maxW = 7;
    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        maxW = 5;
    }
    */

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    Animation_Init(GetAnimRes(animId));
    // TODO: Add way to override anim height
    //Animation_Init(frameTableOffset, maxW, 4, ppRes);


    SetSpriteScale(scale);

    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
    }

    mHitGround = false;
}

void HoistParticle::VUpdate()
{
    if (mVelY >= (GetSpriteScale() * FP_FromInteger(10)))
    {
        if (!gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            SetDead(true);
        }
    }
    else
    {
        mVelY += (GetSpriteScale() * FP_FromDouble(0.6));
    }

    const FP oldY = mYPos;
    mYPos += mVelY;

    if (!mHitGround)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions->Raycast(
                mXPos,
                oldY,
                mXPos,
                mYPos,
                &pLine,
                &hitX,
                &hitY,
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
        {
            mYPos = hitY;
            mVelY = (mVelY * FP_FromDouble(-0.3));
            mHitGround = true;
        }
    }
}

void HoistRocksEffect::VScreenChanged()
{
    SetDead(true);
}

HoistRocksEffect::~HoistRocksEffect()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

HoistRocksEffect::HoistRocksEffect(relive::Path_Hoist* pTlv, const Guid& tlvInfo)
    : BaseGameObject(true, 0), mTlvId(tlvInfo)
{
    mTlvXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mTlvYPos = FP_FromInteger(pTlv->mTopLeftY);
}

void HoistRocksEffect::VUpdate()
{
    const auto rnd = Math_RandomRange(1, 4) - 2;
    if (rnd)
    {
        if (rnd == 1)
        {
            AnimId hoistRock = AnimId::AO_HoistRock2;
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                hoistRock = AnimId::RuptureFarms_HoistRock2;
            }

            const FP y = mTlvYPos + FP_FromInteger(Math_RandomRange(-4, 4));
            const FP x = mTlvXPos + FP_FromInteger(Math_RandomRange(-8, 8));
            relive_new HoistParticle(
                x, y,
                FP_FromInteger(1),
                hoistRock);

            SetUpdateDelay(Math_RandomRange(30, 50));
        }
        else
        {
            AnimId hoistRock = AnimId::AO_HoistRock3;
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                hoistRock = AnimId::RuptureFarms_HoistRock3;
            }

            const FP y = mTlvYPos + FP_FromInteger(Math_RandomRange(-4, 4));
            const FP x = mTlvXPos + FP_FromInteger(Math_RandomRange(-8, 8));
            relive_new HoistParticle(
                x, y,
                FP_FromInteger(1),
                hoistRock);

            SetUpdateDelay(Math_RandomRange(5, 10));
        }
    }
    else
    {
        AnimId hoistRock = AnimId::AO_HoistRock1;
        if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
        {
            hoistRock = AnimId::RuptureFarms_HoistRock1;
        }

        const FP y = mTlvYPos + FP_FromInteger(Math_RandomRange(-4, 4));
        const FP x = mTlvXPos + FP_FromInteger(Math_RandomRange(-8, 8));
        relive_new HoistParticle(
            x, y,
            FP_FromInteger(1),
            hoistRock);

        SetUpdateDelay(Math_RandomRange(10, 20));
    }
}

} // namespace AO
