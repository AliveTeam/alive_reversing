#include "stdafx.h"
#include "PossessionFlicker.hpp"
#include "BaseAliveGameObject.hpp"
#include "ObjectIds.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

PossessionFlicker::PossessionFlicker(BaseAliveGameObject* pToApplyFlicker, s32 duration, s32 r, s32 g, s32 b)
 : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::ePossessionFlicker);
    field_30_obj_id = pToApplyFlicker->field_8_object_id;

    // Check if another PossessionFlicker is already applying flicker to pToApplyFlicker
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == ReliveTypes::ePossessionFlicker && static_cast<PossessionFlicker*>(pObj)->ObjectId() == field_30_obj_id)
        {
            // It is to don't store the id, first update will destroy this object
            field_30_obj_id = -1;
            return;
        }
    }

    field_24_r = static_cast<s16>(r);
    field_26_g = static_cast<s16>(g);
    field_28_b = static_cast<s16>(b);

    field_2A_old_r = pToApplyFlicker->mRGB.r;
    field_2C_old_g = pToApplyFlicker->mRGB.g;
    field_2E_old_b = pToApplyFlicker->mRGB.b;

    field_20_time_to_flicker = duration + sGnFrame;

    pToApplyFlicker->mAnim.mRenderMode = TPageAbr::eBlend_1;
}

PossessionFlicker::~PossessionFlicker()
{
    // Restore the original non flickering colour
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_30_obj_id));
    if (pToApplyFlicker)
    {
        pToApplyFlicker->mAnim.mRenderMode = TPageAbr::eBlend_0;
        pToApplyFlicker->mRGB.SetRGB(field_2A_old_r, field_2C_old_g, field_2E_old_b);
    }
}

s32 PossessionFlicker::ObjectId() const
{
    return field_30_obj_id;
}

void PossessionFlicker::VScreenChanged()
{
    if (!sObjectIds.Find_Impl(field_30_obj_id))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void PossessionFlicker::VUpdate()
{
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_Impl(field_30_obj_id));
    if (!pToApplyFlicker || static_cast<s32>(sGnFrame) > field_20_time_to_flicker)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (sGnFrame % 2)
    {
        // Flicker to original colour
        pToApplyFlicker->mRGB.SetRGB(field_24_r, field_26_g, field_28_b);
    }
    else
    {
        // Flicker to new colour
        pToApplyFlicker->mRGB.SetRGB(field_2A_old_r, field_2C_old_g, field_2E_old_b);
    }
}
