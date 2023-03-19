#include "stdafx_ao.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "ResourceManager.hpp"
#include "AnimResources.hpp"
#include "Particle.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "../AliveLibCommon/BaseGameAutoPlayer.hpp"

namespace AO {

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

void BaseAnimatedWithPhysicsGameObject::Animation_Init_417FD0(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, s16 bAddToDrawableList)
{
    FrameTableOffsetExists(frameTableOffset, false, maxW, maxH);
    const auto init = field_10_anim.Init_402D20(
        frameTableOffset,
        gObjList_animations_505564,
        this,
        static_cast<u16>(maxW),
        static_cast<u16>(maxH),
        ppAnimData,
        1, 0, 0);

    if (init)
    {
        if (field_BC_sprite_scale == FP_FromInteger(1))
        {
            field_10_anim.field_C_layer = Layer::eLayer_27;
        }
        else
        {
            field_10_anim.field_C_layer = Layer::eLayer_8;
            field_C6_scale = 0;
        }

        bool added = true;
        if (bAddToDrawableList)
        {
            added = gObjList_drawables_504618->Push_Back(this) ? true : false;
        }

        if (added)
        {
            field_10_anim.field_B_render_mode = TPageAbr::eBlend_0;
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        }
        else
        {
            field_6_flags.Set(Options::eListAddFailed_Bit1);
            gBaseGameObject_list_9F2DF0->Remove_Item(this);
        }
    }
    else
    {
        if (GetGameAutoPlayer().IsPlaying() || GetGameAutoPlayer().IsRecording())
        {
            std::string errMsg = "Animation init failed! frame table offset " + std::to_string(frameTableOffset);
            ALIVE_FATAL(errMsg.c_str());
        }
        field_6_flags.Set(Options::eListAddFailed_Bit1);
        field_6_flags.Set(Options::eDead_Bit3);
    }
}


CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera()
{
    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);
    return gMap_507BA8.Rect_Location_Relative_To_Active_Camera_4448C0(&rect, 0);
}

void BaseAnimatedWithPhysicsGameObject::DeathSmokeEffect(bool bPlaySound)
{
    // note: mudokons used % 4
    if (!(gnFrameCount_507670 % 5))
    {
        New_Smoke_Particles_419A80(
            field_A8_xpos + (FP_FromInteger(Math_RandomRange_450F20(-24, 24)) * field_BC_sprite_scale),
            field_AC_ypos - FP_FromInteger(6),
            field_BC_sprite_scale / FP_FromInteger(2),
            2,
            0);

        if (bPlaySound == true)
        {
            SFX_Play_43AE60(SoundEffect::Vaporize_96, 25, FP_GetExponent(FP_FromInteger(2200) * field_BC_sprite_scale));
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::VRender_417DA0(PrimHeader** ppOt)
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        // Only render if in the active level, path and camera
        if (gMap_507BA8.field_2_current_path == field_B0_path_number && gMap_507BA8.field_0_current_level == field_B2_lvl_number && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            field_10_anim.field_14_scale = field_BC_sprite_scale;

            s16 r = field_C0_r;
            s16 g = field_C2_g;
            s16 b = field_C4_b;

            PSX_RECT boundingRect = {};
            VGetBoundingRect(&boundingRect, 1);

            if (field_CC_bApplyShadows & 1)
            {
                ShadowZone::ShadowZones_Calculate_Colour_435FF0(
                    FP_GetExponent(field_A8_xpos),         // Left side
                    (boundingRect.y + boundingRect.h) / 2, // Middle of Height
                    field_C6_scale,
                    &r,
                    &g,
                    &b);
            }

            field_10_anim.field_8_r = static_cast<u8>(r);
            field_10_anim.field_9_g = static_cast<u8>(g);
            field_10_anim.field_A_b = static_cast<u8>(b);

            field_10_anim.vRender(
                FP_GetExponent(field_A8_xpos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos + field_CA_xOffset)) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                FP_GetExponent(field_AC_ypos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + field_C8_yOffset)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            field_10_anim.Get_Frame_Rect_402B50(&frameRect);
            pScreenManager_4FF7C8->InvalidateRect_406E40(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_4FF7C8->field_2E_idx);

            if (field_D0_pShadow)
            {
                field_D0_pShadow->Calculate_Position_462040(
                    field_A8_xpos,
                    field_AC_ypos,
                    &frameRect,
                    field_BC_sprite_scale);
                field_D0_pShadow->Render_462410(ppOt);
            }
        }
    }
}

BaseGameObject* BaseAnimatedWithPhysicsGameObject::VDestructor(s32 flags)
{
    return Vdtor_4189C0(flags);
}

BaseAnimatedWithPhysicsGameObject* BaseAnimatedWithPhysicsGameObject::Vdtor_4189C0(s32 flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}


void BaseAnimatedWithPhysicsGameObject::VRender(PrimHeader** ppOt)
{
    VRender_417DA0(ppOt);
}

void BaseAnimatedWithPhysicsGameObject::VOnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn)
{
    VOnCollisionWith_418080(xy, wh, pObjList, startingPointIdx, pFn);
}


PSX_RECT* BaseAnimatedWithPhysicsGameObject::VGetBoundingRect(PSX_RECT* pRect, s32 pointIdx)
{
    return VGetBoundingRect_418120(pRect, pointIdx);
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    return VIsObjNearby_418330(radius, pOtherObj);
}

s16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VIsObj_GettingNear_On_X_418390(pOther);
}

s16 BaseAnimatedWithPhysicsGameObject::VIsFacingMe(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return VIsFacingMe_4183F0(pOther);
}

s16 BaseAnimatedWithPhysicsGameObject::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
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

s16 BaseAnimatedWithPhysicsGameObject::VOnSameYLevel_418450(BaseAnimatedWithPhysicsGameObject* pOther)
{
    // Get bounding rects
    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    PSX_RECT theirRect = {};
    pOther->VGetBoundingRect(&theirRect, 1);

    // Get mid Y of each
    const s32 theirMidY = (theirRect.h + theirRect.y) / 2;
    const s32 ourMidY = (ourRect.h + ourRect.y) / 2;

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

s16 BaseAnimatedWithPhysicsGameObject::VIsFacingMe_4183F0(BaseAnimatedWithPhysicsGameObject* pOther)
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

s16 BaseAnimatedWithPhysicsGameObject::VIsObj_GettingNear_On_X_418390(BaseAnimatedWithPhysicsGameObject* pOther)
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

s16 BaseAnimatedWithPhysicsGameObject::VIsObjNearby_418330(FP radius, BaseAnimatedWithPhysicsGameObject* pOtherObj)
{
    FP x_abs = FP_Abs(pOtherObj->field_A8_xpos - field_A8_xpos);
    if (x_abs > FP_FromInteger(400))
    {
        x_abs = x_abs + ScaleToGridSize_41FA30(field_BC_sprite_scale) - FP_FromInteger(656);
    }
    return x_abs <= radius;
}

void BaseAnimatedWithPhysicsGameObject::VOnCollisionWith_418080(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn)
{
    if (pObjList)
    {
        for (s32 i = 0; i < pObjList->Size(); i++)
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
                    if (xy.field_0_x <= bRect.w && wh.field_0_x >= bRect.x && wh.field_2_y >= bRect.y && xy.field_2_y <= bRect.h)
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

PSX_RECT* BaseAnimatedWithPhysicsGameObject::VGetBoundingRect_418120(PSX_RECT* pRect, s32 pointIdx)
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

    const s16 xpos = FP_GetExponent(field_A8_xpos);
    const s16 ypos = FP_GetExponent(field_AC_ypos);

    rect.x += xpos;
    rect.y += ypos;
    rect.w += xpos;
    rect.h += ypos;

    *pRect = rect;
    return pRect;
}

void BaseAnimatedWithPhysicsGameObject::SetTint_418750(const TintEntry* pTintArray, LevelIds level_id)
{
    while (pTintArray->field_0_level != static_cast<s32>(level_id))
    {
        if (pTintArray->field_0_level == static_cast<s32>(level_id) || pTintArray->field_0_level == static_cast<s32>(LevelIds::eNone))
        {
            break;
        }
        pTintArray++;
    }

    field_C0_r = pTintArray->field_1_r;
    field_C2_g = pTintArray->field_2_g;
    field_C4_b = pTintArray->field_3_b;
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

s16 BaseAnimatedWithPhysicsGameObject::SetBaseAnimPaletteTint_4187C0(const TintEntry* pTintArray, LevelIds lvl, s32 palId)
{
    const TintEntry* pIter = pTintArray;
    while (pIter->field_0_level != static_cast<u8>(lvl))
    {
        if (pIter->field_0_level == -1) // End of entries
        {
            return 0;
        }
        pIter++;
    }

    field_C0_r = pIter->field_1_r;
    field_C2_g = pIter->field_2_g;
    field_C4_b = pIter->field_3_b;

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, palId, 1, 0);
    if (!ppRes)
    {
        return 0;
    }
    field_10_anim.LoadPal_403090(ppRes, 0);
    ResourceManager::FreeResource_455550(ppRes);
    return 1;
}

void BaseAnimatedWithPhysicsGameObject::VStackOnObjectsOfType_418930(Types typeToFind)
{
    const s16 offsets[6] = {
        0, 3, -3, 6, -6, 2};

    s32 array_idx = 0;
    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == typeToFind && pObjIter != this)
        {
            array_idx++;
            if (array_idx >= ALIVE_COUNTOF(offsets))
            {
                array_idx = 0;
            }
        }
    }

    field_CA_xOffset = offsets[array_idx];
}


EXPORT CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera_417CC0()
{
    PSX_RECT rect = {};
    VGetBoundingRect(&rect, 1);
    return gMap_507BA8.Rect_Location_Relative_To_Active_Camera_4448C0(&rect, 0);
}


BaseAnimatedWithPhysicsGameObject::BetweenCamPos BaseAnimatedWithPhysicsGameObject::BetweenCameras_418500()
{
    // TODO: Try to understand how the hell these calcs are supposed to work
    const s32 xPosMinusHalfCameraSpace = FP_GetExponent(field_A8_xpos - FP_FromInteger(512 / 2));
    if (xPosMinusHalfCameraSpace / 512 % 2)
    {
        return BetweenCamPos::Left_1;
    }
    // TODO :This seems wrong, won't it always be false ??
    else if (FP_FromInteger(xPosMinusHalfCameraSpace % 512) > FP_FromInteger(640))
    {
        return BetweenCamPos::Right_2;
    }
    else
    {
        return BetweenCamPos::None_0;
    }
}

} // namespace AO
