#include "stdafx_ao.h"
#include "Function.hpp"
#include "AbilityRing.hpp"
#include "BaseAliveGameObject.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Math.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "PossessionFlicker.hpp"
#undef min
#undef max

namespace AO {

static int MinDistance(int screenX, int screenY, int width1, int height1, int width2, int height2)
{
    const int d1 = Math_Distance_451270(screenX, screenY, width1, height1);
    const int d2 = Math_Distance_451270(screenX, screenY, width2, height2);
    if (d2 <= d1)
    {
        return d1;
    }
    else
    {
        return d2;
    }
}

BaseGameObject* AbilityRing::dtor_455E50()
{
    SetVTable(this, 0x4BC090);

    if (field_278_pTarget_obj)
    {
        field_278_pTarget_obj->field_C_refCount--;
    }

    ResourceManager::FreeResource_455550(field_18_ppRes);
    gObjList_drawables_504618->Remove_Item(this);
    return dtor_487DF0();
}

AbilityRing* AbilityRing::ctor_455860(FP xpos, FP ypos, __int16 type)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC090);

    field_4_typeId = Types::eAbilityRing_69;
    field_278_pTarget_obj = nullptr;
    gObjList_drawables_504618->Push_Back(this);
    field_6_flags.Set(Options::eDrawable_Bit4);

    // TODO: OG issue - using frame counter as res id again
    field_18_ppRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_Wave, gnFrameCount_507670, sizeof(AbilityRing_PolyBuffer) * 64);

    if (field_18_ppRes)
    {
        field_14_pRes = reinterpret_cast<AbilityRing_PolyBuffer*>(*field_18_ppRes);

        field_23C_xpos = xpos;
        field_240_ypos = ypos;

 
        field_25E_screenX = FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
        field_260_screenY = FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos));

        field_262_screenXPos = FP_GetExponent(xpos) - field_25E_screenX;
        field_264_screenYPos = FP_GetExponent(ypos) - field_260_screenY;

        const int d1 = MinDistance(field_262_screenXPos, field_264_screenYPos, gPsxDisplay_504C78.field_0_width, 0, 0, 0);
        const int d2 = MinDistance(field_262_screenXPos, field_264_screenYPos, gPsxDisplay_504C78.field_0_width, gPsxDisplay_504C78.field_2_height, 0, gPsxDisplay_504C78.field_2_height);

        if (d1 <= d2)
        {
            field_25C_fade = static_cast<short>(MinDistance(field_262_screenXPos, field_264_screenYPos, gPsxDisplay_504C78.field_0_width, gPsxDisplay_504C78.field_2_height, 0, gPsxDisplay_504C78.field_2_height));
        }
        else
        {
            field_25C_fade = static_cast<short>(MinDistance(field_262_screenXPos, field_264_screenYPos, gPsxDisplay_504C78.field_0_width, 0, 0, 0));
        }

        field_274_ring_type = type;

        switch (field_274_ring_type)
        {
        case 1:
            for (PSX_RECT& r : field_3C_collide_rects)
            {
                r = {};
            }
            // Fall through

        case 2:
            field_258_ring_thickness = FP_FromInteger(8);
            field_24C_speed = FP_FromInteger(6);
            field_248_right = FP_FromInteger(6);
            field_244_left = FP_FromInteger(0);
            field_266_r = 80;
            field_268_g = 0;
            field_26A_b = 0;
            SFX_Play_43AD70(SoundEffect::IngameTransition_107, 0, 0);
            break;

        case 3:
            field_258_ring_thickness = FP_FromInteger(8);
            field_24C_speed = FP_FromInteger(6);
            field_248_right = FP_FromInteger(350);
            field_244_left = FP_FromInteger(342);
            field_266_r = 80;
            field_268_g = 0;
            field_26A_b = 0;
            break;

        case 0:
        case 4:
            field_278_pTarget_obj = sActiveHero_507678;
            field_278_pTarget_obj->field_C_refCount++;
            // Fall through

        case 5:
        case 6:
            field_258_ring_thickness = FP_FromInteger(5);
            field_24C_speed = FP_FromInteger(4);
            field_248_right = FP_FromInteger(4);
            field_244_left = FP_FromInteger(0);
            field_25C_fade = 50;
            switch (type)
            {
            case 0:
                field_266_r = 255;
                field_268_g = 0;
                field_26A_b = 0;
                break;
            case 4:
                field_266_r = 0;
                field_268_g = 0;
                field_26A_b = 255;
                break;
            case 5:
                field_266_r = 0;
                field_268_g = 0;
                field_26A_b = 80;
                break;
            case 6:
                field_266_r = 255;
                field_268_g = 128;
                field_26A_b = 64;
                break;
            default:
                break;
            }
            break;

        default:
            break;

        }

        field_272_path = gMap_507BA8.field_2_current_path;
        field_10_layer = Layer::eLayer_39;
        field_270_level = gMap_507BA8.field_0_current_level;
        field_26C_semiTrans = 1;
        field_26E_tPageMode = TPageAbr::eBlend_1;

        field_250_scaleX = FP_FromDouble(1.0999); // TODO: Matching ?? 0x11999
        field_254_scaleY = FP_FromInteger(1);

        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                Poly_F4* pPoly = &field_14_pRes[x].mPolys[y];
                PolyF4_Init(pPoly);
                SetRGB0(pPoly, field_266_r & 255, field_268_g & 255, field_26A_b & 255);
                Poly_Set_SemiTrans_498A40(&pPoly->mBase.header, field_26C_semiTrans);
            }
            Init_SetTPage_495FB0(&field_1C_primSetTPage[y], 0, 0, PSX_getTPage_4965D0(TPageMode::e16Bit_2, field_26E_tPageMode, 0, 0));
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;
}

BaseGameObject* AbilityRing::VDestructor(signed int flags)
{
    return Vdtor_456940(flags);
}

BaseGameObject* AbilityRing::Vdtor_456940(signed int flags)
{
    dtor_455E50();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

AbilityRing* CC AbilityRing::Factory_447590(FP xpos, FP ypos, __int16 type)
{
    auto pAbilityRing = ao_new<AbilityRing>();
    if (pAbilityRing)
    {
        pAbilityRing->ctor_455860(xpos, ypos, type);
    }
    return pAbilityRing;
}


void AbilityRing::SetTarget_455EC0(BaseAliveGameObject* pTarget)
{
    field_278_pTarget_obj = pTarget;
    field_278_pTarget_obj->field_C_refCount++;
}

void AbilityRing::VScreenChanged()
{
    VScreenChanged_4568D0();
}

void AbilityRing::VScreenChanged_4568D0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void AbilityRing::CollideWithObjects_456250()
{
    for (auto& rect : field_3C_collide_rects)
    {
        rect.x += field_25E_screenX;
        rect.y += field_260_screenY;
        rect.w += field_25E_screenX;
        rect.h += field_260_screenY;
    }

    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        PSX_RECT bRect = {};
        pObj->VGetBoundingRect(&bRect, 1);

        if (!(pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
        {
            for (auto& rect : field_3C_collide_rects)
            {
                if (rect.x <= bRect.w &&
                    rect.w >= bRect.x &&
                    rect.h >= bRect.y &&
                    rect.y <= bRect.h)
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
    }
}

void AbilityRing::VUpdate()
{
    VUpdate_455ED0();
}

void AbilityRing::VUpdate_455ED0()
{
    if (field_278_pTarget_obj)
    {
        if (field_278_pTarget_obj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_278_pTarget_obj->field_C_refCount--;
            field_278_pTarget_obj = nullptr;
        }
        else
        {
            field_25E_screenX = FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
            field_260_screenY = FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos));

            PSX_RECT bRect = {};
            field_278_pTarget_obj->VGetBoundingRect(&bRect, 1);

            field_262_screenXPos = (bRect.w + bRect.x) / 2 - field_25E_screenX;
            field_264_screenYPos = (bRect.h + bRect.y) / 2 - field_260_screenY;
        }
    }

    switch (field_274_ring_type)
    {
    case 0:
    case 4:
    case 6:
        field_248_right += field_24C_speed;
        field_244_left = field_248_right - field_258_ring_thickness;

        if (field_244_left < FP_FromInteger(0))
        {
            field_244_left = FP_FromInteger(0);
        }

        if (FP_GetExponent(field_244_left) <= field_25C_fade)
        {
            field_266_r = (field_266_r >> 1) + (field_266_r >> 2);
            field_268_g = (field_268_g >> 1) + (field_268_g >> 2);
            field_26A_b = (field_26A_b >> 1) + (field_26A_b >> 2);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 64; j++)
                {
                    SetRGB0(&field_14_pRes[j].mPolys[i], field_266_r & 255, field_268_g & 255, field_26A_b & 255);
                }
            }
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        return;

    case 1:
        CollideWithObjects_456250();
        // Fall through

    case 2:
        field_248_right += field_24C_speed;
        field_244_left = field_248_right - field_258_ring_thickness;

        if (field_244_left < FP_FromInteger(0))
        {
            field_244_left = FP_FromInteger(0);
        }

        if (FP_GetExponent(field_244_left) > field_25C_fade)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        break;

    case 3:
        field_248_right -= field_24C_speed;
        field_244_left = field_248_right - field_258_ring_thickness;
        if (field_244_left < FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_244_left = FP_FromInteger(0);
            SFX_Play_43AD70(SoundEffect::IngameTransition_107, 0, 0);
            auto pPossessionFlicker = ao_new<PossessionFlicker>();
            if (pPossessionFlicker)
            {
                pPossessionFlicker->ctor_41A8C0(sActiveHero_507678, 8, 255, 128, 128);
            }
        }
        break;

    case 5:
        field_248_right += field_24C_speed;
        field_244_left = field_248_right - field_258_ring_thickness;
        if (field_244_left >= FP_FromInteger(0))
        {
            if (FP_GetExponent(field_244_left) > field_25C_fade)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
        else
        {
            field_244_left = FP_FromInteger(0);
            if (field_25C_fade < 0)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
        break;
    default:
        return;
    }
}

void AbilityRing::VRender(PrimHeader** ppOt)
{
    VRender_456340(ppOt);
}

void AbilityRing::VRender_456340(PrimHeader** ppOt)
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_270_level,
        field_272_path,
        field_23C_xpos,
        field_240_ypos,
        0)
        )//&& !field_290_bFindingTarget) Missing part of the check from AE
    {
        short y3 = field_264_screenYPos;
        short y4 = field_264_screenYPos;

        short x3 = PsxToPCX(FP_GetExponent(FP_FromInteger(field_262_screenXPos) + (field_244_left * field_250_scaleX)), 11);
        short x4 = PsxToPCX(FP_GetExponent(FP_FromInteger(field_262_screenXPos) + (field_248_right * field_250_scaleX)), 11);

        //Not hardcoded in Exoddus
        BYTE ang = 4;
        auto count = 64;

        for (int i = 0; i < count; i++)
        {
            const short x1 = (short) PsxToPCX(field_262_screenXPos + FP_GetExponent(field_244_left * Math_Sine_451110(ang) * field_250_scaleX), 11);
            const short x2 = (short) PsxToPCX(field_262_screenXPos + FP_GetExponent(field_248_right * Math_Sine_451110(ang) * field_250_scaleX), 11);

            const short y1 = field_264_screenYPos + FP_GetExponent(field_244_left * Math_Cosine_4510A0(ang) * field_254_scaleY);
            const short y2 = field_264_screenYPos + FP_GetExponent(field_248_right * Math_Cosine_4510A0(ang) * field_254_scaleY);

            const short x = std::min(std::min(x1, x3), std::min(x2, x4));
            const short y = std::min(std::min(y1, y3), std::min(y2, y4));
            const short w = std::max(std::max(x1, x3), std::max(x2, x4));
            const short h = std::max(std::max(y1, y3), std::max(y2, y4));

            const PSX_RECT rect = { x, y, w, h };
            if (rect.w < 0 || rect.x > 640 || rect.y > 240 || rect.h < 0)
            {
                //TODO untie from Render() into Update()
                field_3C_collide_rects[i].x = 0;
                field_3C_collide_rects[i].w = 0;
                field_3C_collide_rects[i].y = 0;
                field_3C_collide_rects[i].h = 0;
            }
            else
            {
                Poly_F4* pPoly = &field_14_pRes[i].mPolys[gPsxDisplay_504C78.field_A_buffer_index];
                SetXY0(pPoly, x1, y1);
                SetXY1(pPoly, x2, y2);
                SetXY2(pPoly, x3, y3);
                SetXY3(pPoly, x4, y4);

                OrderingTable_Add_498A80(OtLayer(ppOt, field_10_layer), &pPoly->mBase.header);

                pScreenManager_4FF7C8->InvalidateRect_406E40(
                    rect.x,
                    rect.y,
                    rect.w,
                    rect.h,
                    pScreenManager_4FF7C8->field_2E_idx);

                field_3C_collide_rects[i] = rect;
                field_3C_collide_rects[i].x = PCToPsxX(field_3C_collide_rects[i].x, 20);
                field_3C_collide_rects[i].w = PCToPsxX(field_3C_collide_rects[i].w, 20);
            }

            x3 = x1;
            y3 = y1;
            x4 = x2;
            y4 = y2;

            ang += 4;
        }
        OrderingTable_Add_498A80(OtLayer(ppOt, field_10_layer), &field_1C_primSetTPage[gPsxDisplay_504C78.field_A_buffer_index].mBase);
    }
}

}
