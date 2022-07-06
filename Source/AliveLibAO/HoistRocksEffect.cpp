#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoistRocksEffect.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "Collisions.hpp"

namespace AO {

void HoistParticle::VUpdate()
{
    VUpdate_431BD0();
}

HoistParticle::HoistParticle(FP xpos, FP ypos, FP scale, s32 frameTableOffset)
{
    mXPos = xpos;
    mYPos = ypos;
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kHoistRocksAOResID, 1, 0);
    u16 maxW = 7;
    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        maxW = 5;
    }
    Animation_Init(frameTableOffset, maxW, 4, ppRes);

    mSpriteScale = scale;

    if (scale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }

    field_E4_bHitGround = 0;
}

void HoistParticle::VUpdate_431BD0()
{
    if (mVelY >= (mSpriteScale * FP_FromInteger(10)))
    {
        if (!gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        mVelY += (mSpriteScale * FP_FromDouble(0.6));
    }

    const FP oldY = mYPos;
    mYPos += mVelY;

    if (field_E4_bHitGround == 0)
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
                mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
        {
            mYPos = hitY;
            mVelY = (mVelY * FP_FromDouble(-0.3));
            field_E4_bHitGround = 1;
        }
    }
}

void HoistRocksEffect::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

HoistRocksEffect::~HoistRocksEffect()
{
    gMap.TLV_Reset(field_18_tlvInfo, -1, 0, 0);
}

HoistRocksEffect::HoistRocksEffect(Path_Hoist* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_18_tlvInfo = tlvInfo;
    field_10_xpos = FP_FromInteger(pTlv->mTopLeft.x + 12);
    field_14_ypos = FP_FromInteger(pTlv->mTopLeft.y);
}

void HoistRocksEffect::VUpdate()
{
    const auto rnd = Math_RandomRange(1, 4) - 2;
    if (rnd)
    {
        if (rnd == 1)
        {
            const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock2);
            s32 frameTableOffset = normalHoist.mFrameTableOffset;
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock2);
                frameTableOffset = ruptureHoist.mFrameTableOffset;
            }
            relive_new HoistParticle(
                field_10_xpos + FP_FromInteger(Math_RandomRange(-8, 8)),
                field_14_ypos + FP_FromInteger(Math_RandomRange(-4, 4)),
                FP_FromInteger(1),
                frameTableOffset);

            mBaseGameObjectUpdateDelay = Math_RandomRange(30, 50);
        }
        else
        {
            const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock3);
            s32 frameTableOffset = normalHoist.mFrameTableOffset;
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock3);
                frameTableOffset = ruptureHoist.mFrameTableOffset;
            }
            relive_new HoistParticle(
                field_10_xpos + FP_FromInteger(Math_RandomRange(-8, 8)),
                field_14_ypos + FP_FromInteger(Math_RandomRange(-4, 4)),
                FP_FromInteger(1),
                frameTableOffset);

            mBaseGameObjectUpdateDelay = Math_RandomRange(5, 10);
        }
    }
    else
    {
        const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock1);
        s32 frameTableOffset = normalHoist.mFrameTableOffset;
        if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
        {
            const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock1);
            frameTableOffset = ruptureHoist.mFrameTableOffset;
        }
        relive_new HoistParticle(
            field_10_xpos + FP_FromInteger(Math_RandomRange(-8, 8)),
            field_14_ypos + FP_FromInteger(Math_RandomRange(-4, 4)),
            FP_FromInteger(1),
            frameTableOffset);

        mBaseGameObjectUpdateDelay = Math_RandomRange(10, 20);
    }
}

} // namespace AO
