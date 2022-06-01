#include "stdafx.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "BaseAliveGameObject.hpp"
#include "AnimResources.hpp"
#include <gmock/gmock.h>
#include "Sfx.hpp"
#include "Particle.hpp"

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject(s16 resourceArraySize)
    : BaseGameObject(TRUE, resourceArraySize)
{
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_B8_xpos = FP_FromInteger(0);
    field_BC_ypos = FP_FromInteger(0);

    field_D4_b = 127;
    field_D2_g = 127;
    field_D0_r = 127;

    mFlags.Clear(BaseGameObject::eInteractive_Bit8);
    mFlags.Clear(BaseGameObject::eCanExplode_Bit7);

    mFlags.Set(BaseGameObject::eDrawable_Bit4);
    mFlags.Set(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5);

    field_DC_bApplyShadows &= ~2;
    field_DC_bApplyShadows |= 1;

    field_C0_path_number = gMap.mCurrentPath;
    field_C2_lvl_number = gMap.mCurrentLevel;

    field_CC_sprite_scale = FP_FromInteger(1);

    field_D6_scale = 1;
    field_D8_yOffset = 0;
    field_DA_xOffset = 0;

    field_E0_pShadow = nullptr;
}

BaseAnimatedWithPhysicsGameObject::~BaseAnimatedWithPhysicsGameObject()
{
    if (!mFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        if (mFlags.Get(BaseGameObject::eDrawable_Bit4))
        {
            gObjList_drawables_5C1124->Remove_Item(this);
            field_20_animation.vCleanUp_40C630();
        }

        delete field_E0_pShadow;
    }
}

void BaseAnimatedWithPhysicsGameObject::VUpdate()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::VRender(PrimHeader** ppOt)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        // Only render if in the active level, path and camera
        if (gMap.mCurrentPath == field_C0_path_number
            && gMap.mCurrentLevel == field_C2_lvl_number
            && Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
        {
            field_20_animation.field_14_scale = field_CC_sprite_scale;

            s16 r = field_D0_r;
            s16 g = field_D2_g;
            s16 b = field_D4_b;

            PSX_RECT boundingRect = {};
            vGetBoundingRect_424FD0(&boundingRect, 1);

            if (field_DC_bApplyShadows & 1)
            {
                ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                    FP_GetExponent(field_B8_xpos),         // Left side
                    (boundingRect.y + boundingRect.h) / 2, // Middle of Height
                    field_D6_scale,
                    &r,
                    &g,
                    &b);
            }

            field_20_animation.field_8_r = static_cast<u8>(r);
            field_20_animation.field_9_g = static_cast<u8>(g);
            field_20_animation.field_A_b = static_cast<u8>(b);

            field_20_animation.vRender_40B820(
                FP_GetExponent((FP_FromInteger(field_DA_xOffset) + field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x)),
                FP_GetExponent((FP_FromInteger(field_D8_yOffset) + field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y)),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            field_20_animation.Get_Frame_Rect_409E10(&frameRect);
            pScreenManager_5BB5F4->InvalidateRect_40EC90(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_5BB5F4->field_3A_idx);

            if (field_E0_pShadow)
            {
                field_E0_pShadow->Calculate_Position_4ACA50(
                    field_B8_xpos,
                    field_BC_ypos,
                    &frameRect,
                    field_CC_sprite_scale,
                    field_D6_scale);
                field_E0_pShadow->Render_4ACE60(ppOt);
            }
        }
    }
}


void BaseAnimatedWithPhysicsGameObject::Animation_Init(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, s16 bAddToDrawableList, u8 bOwnsPalData)
{
    FrameTableOffsetExists(frameTableOffset, true, maxW, maxH);
    if (field_20_animation.Init_40A030(
            frameTableOffset,
            gObjList_animations_5C1A24,
            this,
            static_cast<s16>(maxW),
            static_cast<s16>(maxH),
            ppAnimData,
            bOwnsPalData,
            0,
            0))
    {
        if (field_CC_sprite_scale == FP_FromInteger(1))
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_27;
        }
        else
        {
            field_20_animation.field_C_render_layer = Layer::eLayer_8;
            field_D6_scale = 0;
        }

        bool added = true;
        if (bAddToDrawableList)
        {
            added = gObjList_drawables_5C1124->Push_Back_40CAF0(this) ? true : false;
        }

        if (added)
        {
            field_20_animation.field_B_render_mode = TPageAbr::eBlend_0;

            // TODO: Double check this logic
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        }
        else
        {
            mFlags.Set(BaseGameObject::eDead);
            mFlags.Set(BaseGameObject::eListAddFailed_Bit1);
        }
    }
    else
    {
        mFlags.Set(BaseGameObject::eDead);
        mFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void BaseAnimatedWithPhysicsGameObject::vOnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn)
{
    OnCollisionWith_424EE0(xy, wh, pObjList, startingPointIdx, pFn);
}

PSX_RECT* BaseAnimatedWithPhysicsGameObject::vGetBoundingRect_424FD0(PSX_RECT* pRect, s32 pointIdx)
{
    return GetBoundingRect_424FD0(pRect, pointIdx);
}

s16 BaseAnimatedWithPhysicsGameObject::vIsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject* pObj)
{
    return IsObjNearby_4253B0(radius, pObj);
}

s16 BaseAnimatedWithPhysicsGameObject::vIsObj_GettingNear_425420(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return IsObj_GettingNear_On_X_425420(pObj);
}

s16 BaseAnimatedWithPhysicsGameObject::vIsFacingMe_4254A0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return IsFacingMe_4254A0(pOther);
}

s16 BaseAnimatedWithPhysicsGameObject::vOnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return OnSameYLevel_425520(pOther);
}

void BaseAnimatedWithPhysicsGameObject::vStackOnObjectsOfType_425840(AETypes typeToFind)
{
    StackOnObjectsOfType_425840(typeToFind);
}

void BaseAnimatedWithPhysicsGameObject::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_408180();
}

void BaseAnimatedWithPhysicsGameObject::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_4081A0(pFrom);
}

PSX_RECT* BaseAnimatedWithPhysicsGameObject::GetBoundingRect_424FD0(PSX_RECT* pRect, s32 pointIdx)
{
    const FrameInfoHeader* pAnimFrameHeader = field_20_animation.Get_FrameHeader_40B730(-1);

    PSX_RECT rect = {};
    // Normally this data is 3 points, one that is the frame offset and then 2 that make up the bounding rect.
    // So usually pointIdx is 1. However the way the data is structured it could be anything to treat any index
    // into the array of points as a unique bounding rectangle. Also it appears there can be more than 3 points
    // supported in the data too.
    rect.x = pAnimFrameHeader->field_8_data.points[pointIdx].x;
    rect.y = pAnimFrameHeader->field_8_data.points[pointIdx].y;
    rect.w = pAnimFrameHeader->field_8_data.points[pointIdx + 1].x;
    rect.h = pAnimFrameHeader->field_8_data.points[pointIdx + 1].y;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        std::swap(rect.x, rect.w);
        rect.x = -rect.x;
        rect.w = -rect.w;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit6_FlipY))
    {
        std::swap(rect.y, rect.h);
        rect.y = -rect.y;
        rect.h = -rect.h;
    }

    rect.x = FP_GetExponent((FP_FromInteger(rect.x) * field_CC_sprite_scale));
    rect.y = FP_GetExponent((FP_FromInteger(rect.y) * field_CC_sprite_scale));
    rect.w = FP_GetExponent((FP_FromInteger(rect.w) * field_CC_sprite_scale));
    rect.h = FP_GetExponent((FP_FromInteger(rect.h) * field_CC_sprite_scale));

    const s16 xpos = FP_GetExponent(field_B8_xpos);
    const s16 ypos = FP_GetExponent(field_BC_ypos);

    rect.x += xpos;
    rect.y += ypos;
    rect.w += xpos;
    rect.h += ypos;

    *pRect = rect;
    return pRect;
}

s16 BaseAnimatedWithPhysicsGameObject::IsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject* pObj)
{
    FP distance = pObj->field_B8_xpos - field_B8_xpos;

    if (distance < FP_FromInteger(0))
    {
        distance = field_B8_xpos - pObj->field_B8_xpos;
    }

    return distance <= radius;
}

s16 BaseAnimatedWithPhysicsGameObject::IsObj_GettingNear_On_X_425420(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->field_B8_xpos < field_B8_xpos && pOther->field_C4_velx > field_C4_velx)
    {
        // Its before our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    if (pOther->field_B8_xpos > field_B8_xpos && pOther->field_C4_velx < field_C4_velx)
    {
        // Its after our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    // Not heading our way
    return FALSE;
}

// Muds use this to face "away" from Abe when stood on the same grid block. Also used to follow Abe in the correct direction etc.
s16 BaseAnimatedWithPhysicsGameObject::IsFacingMe_4254A0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->field_B8_xpos == field_B8_xpos
        && pOther->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are in the same spot as us, so they can only be facing us if they are NOT facing the same way.
        // This seems strange but its what causes muds to keep changing direction if you turn while you are stood in the same grid as them.
        return TRUE;
    }
    else if (pOther->field_B8_xpos > field_B8_xpos && !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the right of us and facing left
        return TRUE;
    }
    else if (pOther->field_B8_xpos < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the left of using and facing right
        return TRUE;
    }

    return FALSE;
}

// This is how Scrabs, Fleeches (and probably other stuff) know you are on the same "floor"
s16 BaseAnimatedWithPhysicsGameObject::OnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther)
{
    // Get bounding rects
    PSX_RECT ourRect = {};
    GetBoundingRect_424FD0(&ourRect, 1);

    PSX_RECT theirRect = {};
    pOther->GetBoundingRect_424FD0(&theirRect, 1);

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

void BaseAnimatedWithPhysicsGameObject::StackOnObjectsOfType_425840(AETypes typeToFind)
{
    // For some reason this isn't const in the real game
    const s16 kData[6] = {
        0, 3, -3, 6, -6, 2};

    s32 data_idx = 0;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == typeToFind && pObj != this)
        {
            data_idx++;
            if (data_idx >= ALIVE_COUNTOF(kData))
            {
                data_idx = 0;
            }
        }
    }

    field_DA_xOffset = FP_GetExponent(FP_FromInteger(kData[data_idx]) * field_CC_sprite_scale);
}

void BaseAnimatedWithPhysicsGameObject::vOnPickUpOrSlapped_408180()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::vOnThrowableHit_4081A0(BaseGameObject* /*pFrom*/)
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::DealDamageRect_4247A0(const PSX_RECT* pRect)
{
    if (gBaseAliveGameObjects_5C1B7C)
    {
        auto min_x_w = pRect->w;
        if (pRect->x <= pRect->w)
        {
            min_x_w = pRect->x;
        }

        auto min_w_x = pRect->w;
        if (pRect->w <= pRect->x)
        {
            min_w_x = pRect->x;
        }

        auto min_y_h = pRect->h;
        if (pRect->y <= pRect->h)
        {
            min_y_h = pRect->y;
        }

        auto min_h_y = pRect->h;
        if (pRect->h <= pRect->y)
        {
            min_h_y = pRect->y;
        }

        const auto right = FP_GetExponent(field_B8_xpos) + min_x_w;
        const auto left = FP_GetExponent(field_B8_xpos) + min_w_x;
        const auto top = FP_GetExponent(field_BC_ypos) + min_y_h;
        const auto bottom = FP_GetExponent(field_BC_ypos) + min_h_y;

        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            const auto objXPos = FP_GetExponent(pObj->field_B8_xpos);
            const auto objYPos = FP_GetExponent(pObj->field_BC_ypos);

            if (objXPos >= right && objXPos <= left)
            {
                if (objYPos >= top && objYPos <= bottom)
                {
                    if (field_CC_sprite_scale == (pObj->field_CC_sprite_scale * FP_FromDouble(2.75)))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }
}

CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera_424A70()
{
    PSX_RECT rect = {};
    vGetBoundingRect_424FD0(&rect, 1);
    return gMap.Rect_Location_Relative_To_Active_Camera_480FE0(&rect);
}

void BaseAnimatedWithPhysicsGameObject::OnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, s32 startingPointIdx, TCollisionCallBack pFn)
{
    if (!pObjList)
    {
        return;
    }

    //LOG_INFO("X " << xy.field_0_x << " Y " << xy.field_2_y << " W " << wh.field_0_x << " H " << wh.field_2_y);

    for (s32 i = 0; i < pObjList->Size(); i++)
    {
        BaseGameObject* pElement = pObjList->ItemAt(i);
        if (!pElement)
        {
            break;
        }

        if (pElement->mFlags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5))
        {
            BaseAnimatedWithPhysicsGameObject* pObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pElement);
            if (pObj->mFlags.Get(BaseGameObject::eDrawable_Bit4))
            {
                PSX_RECT bRect = {};
                pObj->GetBoundingRect_424FD0(&bRect, startingPointIdx);
                if (xy.field_0_x <= bRect.w && xy.field_2_y <= bRect.h && wh.field_0_x >= bRect.x && wh.field_2_y >= bRect.y && field_D6_scale == pObj->field_D6_scale)
                {
                    if (!(this->*(pFn))(pObj))
                    {
                        break;
                    }
                }
            }
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::SetTint_425600(const TintEntry* pTintArray, LevelIds level_id)
{
    while (pTintArray->field_0_level != static_cast<s32>(level_id)) // TODO: HACK in static data its a byte which doesn't match the enum size
    {
        if (pTintArray->field_0_level == static_cast<s32>(level_id) || pTintArray->field_0_level == static_cast<s32>(LevelIds::eNone))
        {
            break;
        }
        pTintArray++;
    }

    field_D0_r = pTintArray->field_1_r;
    field_D2_g = pTintArray->field_2_g;
    field_D4_b = pTintArray->field_3_b;
}


void BaseAnimatedWithPhysicsGameObject::SetRGB(s16 r, s16 g, s16 b)
{
    field_D0_r = r;
    field_D2_g = g;
    field_D4_b = b;
}

void BaseAnimatedWithPhysicsGameObject::DeathSmokeEffect(bool bPlaySound)
{
    // note: mudokons used % 4
    if (!(sGnFrame_5C1B84 % 5))
    {
        New_Smoke_Particles_426C70(
            (FP_FromInteger(Math_RandomRange_496AB0(-24, 24)) * field_CC_sprite_scale) + field_B8_xpos,
            field_BC_ypos - FP_FromInteger(6),
            field_CC_sprite_scale / FP_FromInteger(2),
            2,
            128u, 128u, 128u);

        if (bPlaySound == true)
        {
            SFX_Play(SoundEffect::Vaporize_79, 25, FP_GetExponent((FP_FromInteger(2200) * field_CC_sprite_scale)));
        }
    }
}
