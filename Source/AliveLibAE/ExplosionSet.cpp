#include "stdafx.h"
#include "ExplosionSet.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/AirExplosion.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "FallingItem.hpp"
#include "../relive_lib/Grid.hpp"

ExplosionSet* gExplosionSet = nullptr;
bool gExplosionSetEnabled = false;

ExplosionSet::ExplosionSet()
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eExplosionSet);

    if (gExplosionSet)
    {
        SetDead(true);
    }
    else
    {
        gExplosionSet = this;
        SetDrawable(true);
        gExplosionSetEnabled = false;
        gObjListDrawables->Push_Back(this);
    }
}

void ExplosionSet::Start()
{
    gExplosionSetEnabled = true;
}

void ExplosionSet::Init(relive::Path_ExplosionSet* pTlv)
{
    mTlvRect.x = pTlv->mTopLeftX;
    mTlvRect.y = pTlv->mTopLeftY;
    mTlvRect.w = pTlv->Width();
    mTlvRect.h = pTlv->Height();

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }

    mStartDelay = pTlv->mStartDelay;

    mSpawnAssets = pTlv->mSpawnAssets == relive::reliveChoice::eYes ? true : false;
    mFlipX = pTlv->mStartDirection == relive::reliveXDirection::eRight ? true : false;
    mAssetInterval = pTlv->mAssetInterval;
    mGridSpacing = FP_GetExponent(FP_FromInteger(pTlv->mGridSpacing) * ScaleToGridSize(mSpriteScale));
    mIncreasingGridSpacing = FP_GetExponent(FP_FromInteger(pTlv->mIncreasingGridSpacing) * ScaleToGridSize(mSpriteScale));
    mSwitchId = pTlv->mSwitchId;

    if (!gExplosionSetEnabled)
    {
        gExplosionSetEnabled = static_cast<s16>(pTlv->mStartEnabled);
    }

    mActive = true;
}

ExplosionSet::~ExplosionSet()
{
    gObjListDrawables->Remove_Item(this);
    gExplosionSet = nullptr;
}

void ExplosionSet::VScreenChanged()
{
    if (!gMap.LevelChanged() && !gMap.PathChanged())
    {
        mActive = false;
    }
    else
    {
        SetDead(true);
    }
}

struct Point2 final
{
    s8 x, y;
};

static const Point2 sScreenShakeOffsets[12] = {
    {0, -2},
    {-2, 0},
    {2, -2},
    {-2, 2},
    {0, -3},
    {-3, 0},
    {3, -3},
    {-3, 3},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}};

void ExplosionSet::VRender(OrderingTable& ot)
{
    if (gExplosionSetEnabled)
    {
        mScreenOffset.SetOffset(sScreenShakeOffsets[mScreenShakeIdx].x, sScreenShakeOffsets[mScreenShakeIdx].y);
        ot.Add(Layer::eLayer_0, &mScreenOffset);
    }
}

void ExplosionSet::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!gExplosionSetEnabled)
    {
        if (mActive && mSwitchId > 0 && SwitchStates_Get(mSwitchId))
        {
            gExplosionSetEnabled = true;
        }
        return;
    }

    mScreenShakeIdx += mScreenShakeIdxModifier;

    if (mScreenShakeIdx >= 7 || mScreenShakeIdx <= 0)
    {
        mScreenShakeIdxModifier = -mScreenShakeIdxModifier;
    }

    if (mActive && mSpawnAssets)
    {
        if (mStartDelay > 0)
        {
            mStartDelay--;
            return;
        }

        s16 xpos = 0;
        if (mFlipX)
        {
            xpos = mTlvRect.w + mTlvRect.x - (mSpacingMultiplicator * mIncreasingGridSpacing) - mGridSpacing;
            if (xpos <= mTlvRect.x)
            {
                xpos = mTlvRect.w + mTlvRect.x - mGridSpacing;
                mSpacingMultiplicator = 0;
            }
        }
        else
        {
            xpos = mGridSpacing + mTlvRect.x + (mSpacingMultiplicator * mIncreasingGridSpacing);
            if (xpos >= mTlvRect.x + mTlvRect.w)
            {
                xpos = mTlvRect.x + mGridSpacing;
                mSpacingMultiplicator = 0;
            }
        }

        relive_new FallingItem(xpos, mTlvRect.y, mSpriteScale < FP_FromInteger(1), 0, 0, 1, 0);

        mSpacingMultiplicator++;
        mStartDelay = mAssetInterval;

        if (gMap.mCurrentLevel == EReliveLevelIds::eMines && Math_RandomRange(1, 5) >= 4)
        {
            const FP explodeX = FP_FromInteger(Math_RandomRange(mTlvRect.y + 20, mTlvRect.y + 230));
            const FP explodeY = FP_FromInteger(Math_RandomRange(mTlvRect.x, xpos));
            relive_new AirExplosion(explodeY, explodeX, mSpriteScale, 0);
        }
    }
}
