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
    SetType(AETypes::ePossessionFlicker_51);
    field_30_obj_id = pToApplyFlicker->field_8_object_id;

    // Check if another PossessionFlicker is already applying flicker to pToApplyFlicker
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == AETypes::ePossessionFlicker_51 && static_cast<PossessionFlicker*>(pObj)->ObjectId() == field_30_obj_id)
        {
            // It is to don't store the id, first update will destroy this object
            field_30_obj_id = -1;
            return;
        }
    }

    field_24_r = static_cast<s16>(r);
    field_26_g = static_cast<s16>(g);
    field_28_b = static_cast<s16>(b);

    field_2A_old_r = pToApplyFlicker->field_D0_r;
    field_2C_old_g = pToApplyFlicker->field_D2_g;
    field_2E_old_b = pToApplyFlicker->field_D4_b;

    field_20_time_to_flicker = duration + sGnFrame_5C1B84;

    pToApplyFlicker->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
}

PossessionFlicker::~PossessionFlicker()
{
    // Restore the original non flickering colour
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_449CF0(field_30_obj_id));
    if (pToApplyFlicker)
    {
        pToApplyFlicker->field_20_animation.field_B_render_mode = TPageAbr::eBlend_0;
        pToApplyFlicker->field_D0_r = field_2A_old_r;
        pToApplyFlicker->field_D2_g = field_2C_old_g;
        pToApplyFlicker->field_D4_b = field_2E_old_b;
    }
}

s32 PossessionFlicker::ObjectId() const
{
    return field_30_obj_id;
}

void PossessionFlicker::VScreenChanged()
{
    if (!sObjectIds.Find_449CF0(field_30_obj_id))
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void PossessionFlicker::VUpdate()
{
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds.Find_449CF0(field_30_obj_id));
    if (!pToApplyFlicker || static_cast<s32>(sGnFrame_5C1B84) > field_20_time_to_flicker)
    {
        mFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (sGnFrame_5C1B84 % 2)
    {
        // Flicker to original colour
        pToApplyFlicker->field_D0_r = field_24_r;
        pToApplyFlicker->field_D2_g = field_26_g;
        pToApplyFlicker->field_D4_b = field_28_b;
    }
    else
    {
        // Flicker to new colour
        pToApplyFlicker->field_D0_r = field_2A_old_r;
        pToApplyFlicker->field_D2_g = field_2C_old_g;
        pToApplyFlicker->field_D4_b = field_2E_old_b;
    }
}
