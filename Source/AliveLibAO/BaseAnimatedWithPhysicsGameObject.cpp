#include "stdafx_ao.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"

START_NS_AO

FP CC ScaleToGridSize_41FA30(FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return {};
}

BaseAnimatedWithPhysicsGameObject* BaseAnimatedWithPhysicsGameObject::ctor_417C10()
{
    ctor_487E10(1);

    SetVTable(&field_10_anim, 0x4BA2B8);
    SetVTable(this, 0x4BAA38);

    field_CC_bApplyShadows |= 1u;
    field_B0_path_number = gMap_507BA8.field_2_current_path;
    field_B2_lvl_number = gMap_507BA8.field_0_current_level;

    field_B4_velx = FP_FromInteger(0);
    field_A8_xpos = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_AC_ypos = FP_FromInteger(0);
    field_CA_xOffset = 0;
    field_D0_pShadow = nullptr;
    field_C4_b = 105;
    field_C2_g = 105;
    field_C0_r = 105;
    field_BC_sprite_scale = FP_FromInteger(1);
    field_C6_scale = 1;
    field_C8_yOffset = 5;

    field_6_flags.Clear(Options::eCanExplode_Bit7);
    field_6_flags.Clear(Options::eInteractive_Bit8);

    field_6_flags.Set(Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    field_6_flags.Set(Options::eDrawable_Bit4);

    return this;
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init_417FD0(int /*frameTableOffset*/, int /*maxW*/, int /*maxH*/, BYTE** /*ppAnimData*/, __int16 /*a6*/)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::VRender_417DA0(int** /*ot*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* BaseAnimatedWithPhysicsGameObject::VDestructor(signed int flags)
{
    return Vdtor_4189C0(flags);
}

BaseAnimatedWithPhysicsGameObject* BaseAnimatedWithPhysicsGameObject::Vdtor_4189C0(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BaseAnimatedWithPhysicsGameObject::VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn)
{
    VOnCollisionWith_418080(xy, wh, pObjList, startingPointIdx, pFn);
}


PSX_RECT* BaseAnimatedWithPhysicsGameObject::VGetBoundingRect(PSX_RECT* pRect, int pointIdx)
{
    return VGetBoundingRect_418120(pRect, pointIdx);
}

__int16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    return VIsObjNearby_418330(radius, pOtherObj);
}

__int16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VIsObj_GettingNear_On_X_418390(pOther);
}

__int16 BaseAnimatedWithPhysicsGameObject::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VIsFacingMe_4183F0(pOther);
}

__int16 BaseAnimatedWithPhysicsGameObject::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VOnSameYLevel_418450(pOther);
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType(Types typeToFind)
{
    VStackOnObjectsOfType_418930(typeToFind);
}

void BaseAnimatedWithPhysicsGameObject::VOnPickUpOrSlapped()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

__int16 BaseAnimatedWithPhysicsGameObject::VOnSameYLevel_418450(BaseAnimatedWithPhysicsGameObject* pOther)
{
    // Get bounding rects
    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    PSX_RECT theirRect = {};
    pOther->VGetBoundingRect(&theirRect, 1);

    // Get mid Y of each
    const int theirMidY = (theirRect.h + theirRect.y) / 2;
    const int ourMidY = (ourRect.h + ourRect.y) / 2;

    if (theirMidY <= ourRect.h && theirMidY >= ourRect.y)
    {
        return TRUE;
    }

    if (ourMidY <= theirRect.h && ourMidY >= theirRect.y)
    {
        return TRUE;
    }

    return FALSE;
}

__int16 BaseAnimatedWithPhysicsGameObject::VIsFacingMe_4183F0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->field_A8_xpos == field_A8_xpos
        && pOther->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are in the same spot as us, so they can only be facing us if they are NOT facing the same way.
        // This seems strange but its what causes muds to keep changing direction if you turn while you are stood in the same grid as them.
        return TRUE;
    }
    else if (pOther->field_A8_xpos > field_A8_xpos && !field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the right of us and facing left
        return TRUE;
    }
    else if (pOther->field_A8_xpos < field_A8_xpos && field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the left of using and facing right
        return TRUE;
    }

    return FALSE;
}

__int16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X_418390(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->field_A8_xpos < field_A8_xpos && pOther->field_B4_velx > field_B4_velx)
    {
        // Its before our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    if (pOther->field_A8_xpos > field_A8_xpos && pOther->field_B4_velx < field_B4_velx)
    {
        // Its after our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    // Not heading our way
    return FALSE;
}

__int16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby_418330(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    FP x_abs = FP_Abs(pOtherObj->field_A8_xpos - field_A8_xpos);
    if (x_abs > FP_FromInteger(400))
    {
        x_abs = x_abs + ScaleToGridSize_41FA30(field_BC_sprite_scale) - FP_FromInteger(656);
    }
    return x_abs <= radius;
}

void BaseAnimatedWithPhysicsGameObject::VOnCollisionWith_418080(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn)
{
    if (pObjList)
    {
        for (int i=0; i<pObjList->Size(); i++)
        {
            BaseGameObject* pObjIter = pObjList->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->field_6_flags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5))
            {
                if (pObjIter->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
                {
                    auto pObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObjIter);

                    PSX_RECT bRect = {};
                    pObj->VGetBoundingRect(&bRect, startingPointIdx);
                    if (xy.field_0_x <= bRect.w &&
                        wh.field_0_x >= bRect.x &&
                        wh.field_2_y >= bRect.y &&
                        xy.field_2_y <= bRect.h)
                    {
                        if (!(this->*pFn)(pObj))
                        {
                            break;
                        }
                    }
                }
            }
        };
    }
}

PSX_RECT* BaseAnimatedWithPhysicsGameObject::VGetBoundingRect_418120(PSX_RECT* pRect, int pointIdx)
{
    const FrameInfoHeader* pAnimFrameHeader = field_10_anim.Get_FrameHeader_403A00(-1);

    PSX_RECT rect = {};
    // Normally this data is 3 points, one that is the frame offset and then 2 that make up the bounding rect.
    // So usually pointIdx is 1. However the way the data is structured it could be anything to treat any index
    // into the array of points as a unique bounding rectangle. Also it appears there can be more than 3 points
    // supported in the data too.
    rect.x = pAnimFrameHeader->field_8_data.points[pointIdx].x;
    rect.y = pAnimFrameHeader->field_8_data.points[pointIdx].y;
    rect.w = pAnimFrameHeader->field_8_data.points[pointIdx + 1].x;
    rect.h = pAnimFrameHeader->field_8_data.points[pointIdx + 1].y;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        std::swap(rect.x, rect.w);
        rect.x = -rect.x;
        rect.w = -rect.w;
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit6_FlipY))
    {
        std::swap(rect.y, rect.h);
        rect.y = -rect.y;
        rect.h = -rect.h;
    }

    rect.x = FP_GetExponent((FP_FromInteger(rect.x) * field_BC_sprite_scale));
    rect.y = FP_GetExponent((FP_FromInteger(rect.y) * field_BC_sprite_scale));
    rect.w = FP_GetExponent((FP_FromInteger(rect.w) * field_BC_sprite_scale));
    rect.h = FP_GetExponent((FP_FromInteger(rect.h) * field_BC_sprite_scale));

    const short xpos = FP_GetExponent(field_A8_xpos);
    const short ypos = FP_GetExponent(field_AC_ypos);

    rect.x += xpos;
    rect.y += ypos;
    rect.w += xpos;
    rect.h += ypos;

    *pRect = rect;
    return pRect;
}

void BaseAnimatedWithPhysicsGameObject::SetTint_418750(const TintEntry* /*pTintArray*/, LevelIds /*levelId*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* BaseAnimatedWithPhysicsGameObject::dtor_417D10()
{
    SetVTable(this, 0x4BAA38);

    if (!field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        if (field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
        {
            gObjList_drawables_504618->Remove_Item(this);
            field_10_anim.vCleanUp();
        }

        if (field_D0_pShadow)
        {
            field_D0_pShadow->dtor_462030();
            ao_delete_free_447540(field_D0_pShadow);
        }
    }
    return dtor_487DF0();
}

__int16 BaseAnimatedWithPhysicsGameObject::SetBaseAnimPaletteTint_4187C0(TintEntry* /*pTintArray*/, LevelIds /*level_id*/, int /*resourceID*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType_418930(Types /*typeToFind*/)
{
    NOT_IMPLEMENTED();
}


EXPORT CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera_417CC0()
{
    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);
    return gMap_507BA8.Rect_Location_Relative_To_Active_Camera_4448C0(&rect, 0);
}

END_NS_AO
