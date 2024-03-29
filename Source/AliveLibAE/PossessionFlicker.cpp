#include "stdafx.h"
#include "PossessionFlicker.hpp"
#include "BaseAliveGameObject.hpp"
#include "ObjectIds.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

BaseGameObject* PossessionFlicker::VDestructor(s32 flags)
{
    return vdtor_431AD0(flags);
}

void PossessionFlicker::VUpdate()
{
    vUpdate_431BC0();
}

void PossessionFlicker::VScreenChanged()
{
    vScreenChanged_431C80();
}

PossessionFlicker* PossessionFlicker::ctor_4319E0(BaseAliveGameObject* pToApplyFlicker, s32 duration, s16 r, s16 g, s16 b)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x544FC8); // vTbl_PossessionFlicker_544FC8

    SetType(AETypes::ePossessionFlicker_51);
    field_30_obj_id = pToApplyFlicker->field_8_object_id;

    // Check if another PossessionFlicker is already applying flicker to pToApplyFlicker
    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != this && pObj->Type() == AETypes::ePossessionFlicker_51 && static_cast<PossessionFlicker*>(pObj)->ObjectId() == field_30_obj_id)
        {
            // It is to don't store the id, first update will destroy this object
            field_30_obj_id = -1;
            return this;
        }
    }

    field_24_r = r;
    field_26_g = g;
    field_28_b = b;

    field_2A_old_r = pToApplyFlicker->field_D0_r;
    field_2C_old_g = pToApplyFlicker->field_D2_g;
    field_2E_old_b = pToApplyFlicker->field_D4_b;

    field_20_time_to_flicker = duration + sGnFrame_5C1B84;

    pToApplyFlicker->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;

    return this;
}

s32 PossessionFlicker::ObjectId() const
{
    return field_30_obj_id;
}

PossessionFlicker* PossessionFlicker::vdtor_431AD0(s32 flags)
{
    dtor_431B00();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void PossessionFlicker::dtor_431B00()
{
    SetVTable(this, 0x544FC8); // vTbl_PossessionFlicker_544FC8

    // Restore the original non flickering colour
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_30_obj_id));
    if (pToApplyFlicker)
    {
        pToApplyFlicker->field_20_animation.field_B_render_mode = TPageAbr::eBlend_0;
        pToApplyFlicker->field_D0_r = field_2A_old_r;
        pToApplyFlicker->field_D2_g = field_2C_old_g;
        pToApplyFlicker->field_D4_b = field_2E_old_b;
    }

    BaseGameObject_dtor_4DBEC0();
}

void PossessionFlicker::vScreenChanged_431C80()
{
    if (!sObjectIds_5C1B70.Find_449CF0(field_30_obj_id))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void PossessionFlicker::vUpdate_431BC0()
{
    BaseAnimatedWithPhysicsGameObject* pToApplyFlicker = static_cast<BaseAnimatedWithPhysicsGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_30_obj_id));
    if (!pToApplyFlicker || static_cast<s32>(sGnFrame_5C1B84) > field_20_time_to_flicker)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
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
