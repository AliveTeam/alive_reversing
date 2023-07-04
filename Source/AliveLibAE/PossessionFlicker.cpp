#include "stdafx.h"
#include "PossessionFlicker.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Function.hpp"

PossessionFlicker::PossessionFlicker(IBaseAliveGameObject* pToApplyFlicker, s32 duration, s32 r, s32 g, s32 b)
 : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::ePossessionFlicker);
    mTargetObjId = pToApplyFlicker->mBaseGameObjectId;

    // Check if another PossessionFlicker is already applying flicker to pToApplyFlicker
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == ReliveTypes::ePossessionFlicker && static_cast<PossessionFlicker*>(pObj)->ObjectId() == mTargetObjId)
        {
            // It is so don't store the id as the first update will destroy this object
            mTargetObjId = Guid{};
            return;
        }
    }

    mNewRed = static_cast<s16>(r);
    mNewGreen = static_cast<s16>(g);
    mNewBlue = static_cast<s16>(b);

    mOldRed = pToApplyFlicker->mRGB.r;
    mOldGreen = pToApplyFlicker->mRGB.g;
    mOldBlue = pToApplyFlicker->mRGB.b;

    pToApplyFlicker->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);

    mTimeToFlicker = duration + sGnFrame;
}

PossessionFlicker::~PossessionFlicker()
{
    // Restore the original non flickering colour
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(mTargetObjId));
    if (pToApplyFlicker)
    {
        pToApplyFlicker->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);
        pToApplyFlicker->mRGB.SetRGB(mOldRed, mOldGreen, mOldBlue);
    }
}

Guid PossessionFlicker::ObjectId() const
{
    return mTargetObjId;
}

void PossessionFlicker::VScreenChanged()
{
    if (!sObjectIds.Find_Impl(mTargetObjId))
    {
        SetDead(true);
    }
}

void PossessionFlicker::VUpdate()
{
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(mTargetObjId));
    if (!pToApplyFlicker || static_cast<s32>(sGnFrame) > mTimeToFlicker)
    {
        SetDead(true);
        return;
    }

    if (sGnFrame % 2)
    {
        // Flicker to original colour
        pToApplyFlicker->mRGB.SetRGB(mNewRed, mNewGreen, mNewBlue);
    }
    else
    {
        // Flicker to new colour
        pToApplyFlicker->mRGB.SetRGB(mOldRed, mOldGreen, mOldBlue);
    }
}
