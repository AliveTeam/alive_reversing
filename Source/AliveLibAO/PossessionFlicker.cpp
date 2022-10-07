#include "stdafx_ao.h"
#include "PossessionFlicker.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

void PossessionFlicker::VScreenChanged()
{
    if (field_10_pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_10_pObj->mBaseGameObjectRefCount--;
        field_10_pObj = nullptr;
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

PossessionFlicker::~PossessionFlicker()
{
    if (field_10_pObj)
    {
        field_10_pObj->mAnim.mRenderMode = TPageAbr::eBlend_0;

        field_10_pObj->mRGB.r = field_1E_old_r;
        field_10_pObj->mRGB.g = field_20_old_g;
        field_10_pObj->mRGB.b = field_22_old_b;

        field_10_pObj->mBaseGameObjectRefCount--;
    }
}

PossessionFlicker::PossessionFlicker(BaseAliveGameObject* pToApplyFlicker, s32 duration, s32 r, s32 g, s32 b)
    : BaseGameObject(TRUE, 0)
{
    field_10_pObj = pToApplyFlicker;
    field_10_pObj->mBaseGameObjectRefCount++;

    field_18_r = static_cast<s16>(r);
    field_1A_g = static_cast<s16>(g);
    field_1C_b = static_cast<s16>(b);

    field_1E_old_r = pToApplyFlicker->mRGB.r;
    field_20_old_g = pToApplyFlicker->mRGB.g;
    field_22_old_b = pToApplyFlicker->mRGB.b;

    pToApplyFlicker->mAnim.mRenderMode = TPageAbr::eBlend_1;

    field_14_time_to_flicker = sGnFrame + duration;
}


void PossessionFlicker::VUpdate()
{
    bool bFlicker = false;
    if (field_10_pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_10_pObj->mBaseGameObjectRefCount--;
        field_10_pObj = nullptr;
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        bFlicker = false;
    }
    else
    {
        bFlicker = true;
    }

    if (bFlicker)
    {
        if (sGnFrame % 2)
        {
            field_10_pObj->mRGB.r = field_1E_old_r;
            field_10_pObj->mRGB.g = field_20_old_g;
            field_10_pObj->mRGB.b = field_22_old_b;
        }
        else
        {
            field_10_pObj->mRGB.r = field_18_r;
            field_10_pObj->mRGB.g = field_1A_g;
            field_10_pObj->mRGB.b = field_1C_b;
        }

        if (static_cast<s32>(sGnFrame) > field_14_time_to_flicker)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

} // namespace AO
