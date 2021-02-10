#include "stdafx.h"
#include "AbilityRing.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Sfx.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "PossessionFlicker.hpp"
#include "stdlib.hpp"

AbilityRing * CC AbilityRing::Factory_482F80(FP xpos, FP ypos, RingTypes type, FP scale)
{
    auto pRing = ae_new<AbilityRing>();
    pRing->ctor_49C730(xpos, ypos, type, scale);
    return pRing;
}

struct AbilityRing_PolyBuffer
{
    Poly_F4 mPolys[2];
};
ALIVE_ASSERT_SIZEOF(AbilityRing_PolyBuffer, 56);

static int MinDistance(int screenX, int screenY, int width1, int height1, int width2, int height2)
{
    const int d1 = Math_Distance_496EB0(screenX, screenY, width1, height1);
    const int d2 = Math_Distance_496EB0(screenX, screenY, width2, height2);
    if (d2 <= d1)
    {
        return d1;
    }
    else
    {
        return d2;
    }
}

AbilityRing* AbilityRing::ctor_49C730(FP xpos, FP ypos, RingTypes ringType, FP scale)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x546AB0);

    field_4_typeId = Types::eAbilityRing_104;
    field_288_target_obj_id = -1;
    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);

     // TODO: OG issue - using frame counter as res id again
    field_28_ppRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Wave, sGnFrame_5C1B84, sizeof(AbilityRing_PolyBuffer) * 64);
    if (field_28_ppRes)
    {
        field_24_pRes = reinterpret_cast<AbilityRing_PolyBuffer*>(*field_28_ppRes);
        
        field_24C_xpos = xpos;
        field_250_ypos = ypos;

        field_26E_screenX = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
        field_270_screenY = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

        field_272_screenXPos = FP_GetExponent(xpos) - field_26E_screenX;
        field_274_screenYPos = FP_GetExponent(ypos) - field_270_screenY;

        const int d1 = MinDistance(field_272_screenXPos, field_274_screenYPos, gPsxDisplay_5C1130.field_0_width, 0, 0, 0);
        const int d2 = MinDistance(field_272_screenXPos, field_274_screenYPos, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 0, gPsxDisplay_5C1130.field_2_height);
        if (d1 <= d2)
        {
            field_26C_fade = static_cast<short>(MinDistance(field_272_screenXPos, field_274_screenYPos, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 0, gPsxDisplay_5C1130.field_2_height));
        }
        else
        {
            field_26C_fade = static_cast<short>(MinDistance(field_272_screenXPos, field_274_screenYPos, gPsxDisplay_5C1130.field_0_width, 0, 0, 0));
        }

        field_284_ring_type = ringType;

        switch (field_284_ring_type)
        {
        case RingTypes::eExplosive_Emit_1:
        case RingTypes::eHealing_Emit_12:
            for (PSX_RECT& r : field_4C_collide_rects)
            {
                r = {};
            }
            // Fall through

        case RingTypes::eExplosive_Emit_Effect_2:
        case RingTypes::eInvisible_Pulse_Emit_9:
        case RingTypes::eHealing_Emit_Effect_11:
            field_268_ring_thickness = FP_FromInteger(8);
            field_25C_speed = FP_FromInteger(6);
            field_258_right = FP_FromInteger(6);
            field_254_left = FP_FromInteger(0);

            if (field_284_ring_type == RingTypes::eInvisible_Pulse_Emit_9)
            {
                field_276_r = 0;
                field_278_g = 255;
                field_27A_b = 32;
            }
            else
            {
                if (ringType != RingTypes::eHealing_Emit_Effect_11 && ringType != RingTypes::eHealing_Emit_12)
                {
                    field_276_r = 80;
                    field_278_g = 0;
                    field_27A_b = 0;
                }
                else
                {
                    field_276_r = 255;
                    field_278_g = 255;
                    field_27A_b = 32;
                }
            }

            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 0);
            break;

        case RingTypes::eExplosive_Give_3:
        case RingTypes::eInvisible_Pulse_Give_10:
        case RingTypes::eHealing_Give_13:
            field_268_ring_thickness = FP_FromInteger(8);
            field_25C_speed = FP_FromInteger(6);
            field_258_right = FP_FromInteger(350);
            field_254_left = FP_FromInteger(342);
            if (ringType == RingTypes::eInvisible_Pulse_Give_10)
            {
                field_276_r = 0;
                field_278_g = 255;
                field_27A_b = 32;
            }
            else if (ringType == RingTypes::eHealing_Give_13)
            {
                field_27A_b = 32;
                field_276_r = 255;
                field_278_g = 255;
            }
            else
            {
                field_276_r = 80;
                field_278_g = 0;
                field_27A_b = 0;
            }
            break;

        case RingTypes::eExplosive_Pulse_0:
        case RingTypes::eShrykull_Pulse_Small_4:
        case RingTypes::eInvisible_Pulse_Small_7:
        case RingTypes::eHealing_Pulse_14:
            vSetTarget_49D140(sActiveHero_5C1B68);
            // Fall through

        case RingTypes::eShrykull_Pulse_Large_5:
        case RingTypes::eShrykull_Pulse_Orange_6:
        case RingTypes::eInvisible_Pulse_Large_8:
            field_268_ring_thickness = FP_FromInteger(5);
            field_25C_speed = FP_FromInteger(4);
            field_258_right = FP_FromInteger(4);
            field_254_left = FP_FromInteger(0);
            field_26C_fade = 50;

            switch (field_284_ring_type)
            {
            case RingTypes::eExplosive_Pulse_0:
                field_276_r = 255;
                field_278_g = 0;
                field_27A_b = 0;
                break;

            case RingTypes::eShrykull_Pulse_Small_4:
                field_276_r = 0;
                field_278_g = 0;
                field_27A_b = 255;
                break;

            case RingTypes::eShrykull_Pulse_Large_5:
                field_276_r = 0;
                field_278_g = 0;
                field_27A_b = 80;
                break;

            case RingTypes::eShrykull_Pulse_Orange_6:
                field_276_r = 255;
                field_278_g = 128;
                field_27A_b = 64;
                break;

            case RingTypes::eInvisible_Pulse_Small_7:
            case RingTypes::eInvisible_Pulse_Large_8:
                field_276_r = 0;
                field_278_g = 255;
                field_27A_b = 0;
                break;

            case RingTypes::eHealing_Pulse_14:
                field_27A_b = 32;
                field_276_r = 255;
                field_278_g = 255;
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }

        field_282_path = gMap_5C3030.field_2_current_path;
        field_20_layer = Layer::eLayer_39;
        field_280_level = gMap_5C3030.field_0_current_level;
        field_27C_semiTrans = 1;
        field_27E_tPageMode = TPageAbr::eBlend_1;

        if (field_284_ring_type == RingTypes::eShrykull_Pulse_Orange_6 && scale == FP_FromDouble(0.5))
        {
            field_20_layer = Layer::eLayer_16;
        }

        field_260_scaleX = FP_FromDouble(1.0999); // TODO: Matching ?? 0x11999
        field_264_scaleY = FP_FromInteger(1);
        field_268_ring_thickness = (field_268_ring_thickness * scale);
        field_25C_speed = (field_25C_speed * scale);
        field_254_left = (field_254_left * scale);
        field_258_right = (field_258_right * scale);

        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                Poly_F4* pPoly = &field_24_pRes[x].mPolys[y];
                PolyF4_Init_4F8830(pPoly);
                SetRGB0(pPoly, field_276_r & 255, field_278_g & 255, field_27A_b & 255);
                Poly_Set_SemiTrans_4F8A60(&pPoly->mBase.header, field_27C_semiTrans);
            }
            Init_SetTPage_4F5B60(&field_2C_primSetTPage[y], 0, 0, PSX_getTPage_4F60E0(TPageMode::e16Bit_2, field_27E_tPageMode, 0, 0));
        }
        field_28C_count = 64;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return this;
}

BaseGameObject* AbilityRing::VDestructor(signed int flags)
{
    return vdtor_49D080(flags);
}

void AbilityRing::VUpdate()
{
    vUpdate_49D160();
}

void AbilityRing::VRender(PrimHeader** ppOt)
{
    vRender_49D790(ppOt);
}

void AbilityRing::VScreenChanged()
{
    vScreenChanged_49DE70();
}

int AbilityRing::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_49E070(reinterpret_cast<AbilityRing_State*>(pSaveBuffer));
}

void AbilityRing::VSetTarget(BaseGameObject* pTarget)
{
    vSetTarget_49D140(pTarget);
}

int CC AbilityRing::CreateFromSaveState_49DF90(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const AbilityRing_State*>(pBuffer);
    auto pRing = ae_new<AbilityRing>();
    if (pRing)
    {
        pRing->ctor_49C730(pState->field_4_xpos, pState->field_8_ypos, pState->field_C_ring_type, pState->field_10_scale);
    }
    pRing->field_276_r = pState->field_20_r;
    pRing->field_278_g = pState->field_22_g;
    pRing->field_27A_b = pState->field_24_b;
    pRing->field_258_right = pState->field_18_right;
    pRing->field_28C_count = pState->field_1C_count;
    pRing->field_288_target_obj_id = pState->field_14_obj_id;
    pRing->field_290_bFindingTarget = TRUE;
    return sizeof(AbilityRing_State);
}

void AbilityRing::vSetTarget_49D140(BaseGameObject* pTarget)
{
    field_288_target_obj_id = pTarget->field_8_object_id;
}

void AbilityRing::dtor_49D0B0()
{
    SetVTable(this, 0x546AB0);
    ResourceManager::FreeResource_49C330(field_28_ppRes);
    gObjList_drawables_5C1124->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

AbilityRing* AbilityRing::vdtor_49D080(signed int flags)
{
    dtor_49D0B0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void AbilityRing::vUpdate_49D160()
{
    if (field_290_bFindingTarget)
    {
        field_290_bFindingTarget = FALSE;
        field_288_target_obj_id = Find_Flags_4DC170(field_288_target_obj_id);
    }

    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_288_target_obj_id));
    if (pTarget)
    {
        if (pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_288_target_obj_id = -1;
        }
        else
        {
            field_26E_screenX = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
            field_270_screenY = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
            PSX_RECT bRect = {};
            pTarget->vGetBoundingRect_424FD0(&bRect, 1);
            field_272_screenXPos = (bRect.x + bRect.w) / 2 - field_26E_screenX;
            field_274_screenYPos = (bRect.y + bRect.h) / 2 - field_270_screenY;
        }
    }

    switch (field_284_ring_type)
    {
    case RingTypes::eExplosive_Pulse_0:
    case RingTypes::eShrykull_Pulse_Small_4:
    case RingTypes::eShrykull_Pulse_Orange_6:
    case RingTypes::eInvisible_Pulse_Small_7:
    case RingTypes::eInvisible_Pulse_Large_8:
    case RingTypes::eHealing_Pulse_14:
        field_258_right += field_25C_speed;
        field_254_left = field_258_right - field_268_ring_thickness;
        if (field_254_left < FP_FromInteger(0))
        {
            field_254_left = FP_FromInteger(0);
        }

        if (FP_GetExponent(field_254_left) <= field_26C_fade)
        {
            field_276_r = (field_276_r >> 1) + (field_276_r >> 2);
            field_278_g = (field_278_g >> 1) + (field_278_g >> 2);
            field_27A_b = (field_27A_b >> 1) + (field_27A_b >> 2);
            for (int j = 0; j < 2; j++)
            {
                for (int i = 0; i < 64; i++)
                {
                    SetRGB0(&field_24_pRes[i].mPolys[j], field_276_r & 255, field_278_g & 255, field_27A_b & 255);
                }
            }
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        return;

    case RingTypes::eExplosive_Emit_1:
    case RingTypes::eHealing_Emit_12:
        if (field_284_ring_type == RingTypes::eHealing_Emit_12)
        {
            CollideWithObjects_49D5E0(FALSE);
        }
        else
        {
            CollideWithObjects_49D5E0(TRUE);
        }
        // Fall through

    case RingTypes::eExplosive_Emit_Effect_2:
    case RingTypes::eInvisible_Pulse_Emit_9:
    case RingTypes::eHealing_Emit_Effect_11:
    case RingTypes::eShrykull_Pulse_Large_5:
        field_258_right += field_25C_speed;
        field_254_left = field_258_right - field_268_ring_thickness;
        if (field_254_left < FP_FromInteger(0))
        {
            field_254_left = FP_FromInteger(0);
        }

        if (FP_GetExponent(field_254_left) > field_26C_fade)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        break;

    case RingTypes::eExplosive_Give_3:
    case RingTypes::eInvisible_Pulse_Give_10:
    case RingTypes::eHealing_Give_13:
        field_258_right -= field_25C_speed;
        field_254_left = field_258_right - field_268_ring_thickness;
        if (field_254_left < FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_254_left = FP_FromInteger(0);
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 0);
            if (field_284_ring_type == RingTypes::eExplosive_Give_3)
            {
                auto pPossessionFlicker = ae_new<PossessionFlicker>();
                if (pPossessionFlicker)
                {
                    pPossessionFlicker->ctor_4319E0(sActiveHero_5C1B68, 8, 255, 128, 128);
                }
            }
        }
        break;

    default:
        return;
    }
}

void AbilityRing::CollideWithObjects_49D5E0(__int16 bDealDamage)
{
    for (int i = 0; i < field_28C_count; i++)
    {
        field_4C_collide_rects[i].x += field_26E_screenX;
        field_4C_collide_rects[i].y += field_270_screenY;
        field_4C_collide_rects[i].w += field_26E_screenX;
        field_4C_collide_rects[i].h += field_270_screenY;
    }

    for (int i=0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        PSX_RECT bRect = {};
        pObj->vGetBoundingRect_424FD0(&bRect, 1);

        if (!(pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
        {
            for (int j = 0; j < field_28C_count; j++)
            {
                if (field_4C_collide_rects[j].x <= bRect.w &&
                    field_4C_collide_rects[j].w >= bRect.x &&
                    field_4C_collide_rects[j].h >= bRect.y &&
                    field_4C_collide_rects[j].y <= bRect.h)
                {
                    if (bDealDamage)
                    {
                        if (bDealDamage == 1)
                        {
                            pObj->VTakeDamage_408730(this);
                        }
                    }
                    else if (pObj->field_4_typeId == Types::eMudokon_110)
                    {
                        if (pObj->field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed)) // TODO: Clearly this has another meaning for muds
                        {
                            if (pObj->field_10C_health > FP_FromInteger(0))
                            {
                                pObj->VPossessed_408F70();
                            }
                        }
                    }
                }
            }
        }
    }
}

void AbilityRing::vRender_49D790(PrimHeader** ppOt)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_280_level,
        field_282_path,
        field_24C_xpos,
        field_250_ypos,
        0)
        && !field_290_bFindingTarget)
    {
        short y3 = field_274_screenYPos;
        short y4  = field_274_screenYPos;

        short x3 = PsxToPCX(FP_GetExponent(FP_FromInteger(field_272_screenXPos) + (field_254_left * field_260_scaleX)), 11);
        short x4 = PsxToPCX(FP_GetExponent(FP_FromInteger(field_272_screenXPos) + (field_258_right * field_260_scaleX)), 11);
      
        BYTE angIncrement = 0;
        if (field_258_right <= FP_FromInteger(150))
        {
            if (field_258_right <= FP_FromInteger(50))
            {
                field_28C_count = 16;
                angIncrement = 16;
            }
            else
            {
                field_28C_count = 32;
                angIncrement = 8;
            }
        }
        else
        {
            field_28C_count = 64;
            angIncrement = 4;
        }

        BYTE ang = angIncrement;
        for (int i = 0; i < field_28C_count; i++)
        {
            const short x1 = (short)PsxToPCX(field_272_screenXPos + FP_GetExponent(field_254_left * Math_Sine_496DD0(ang) * field_260_scaleX), 11);
            const short x2 = (short)PsxToPCX(field_272_screenXPos + FP_GetExponent(field_258_right * Math_Sine_496DD0(ang) * field_260_scaleX), 11);

            const short y1 = field_274_screenYPos + FP_GetExponent(field_254_left * Math_Cosine_496CD0(ang) * field_264_scaleY);
            const short y2 = field_274_screenYPos + FP_GetExponent(field_258_right * Math_Cosine_496CD0(ang) * field_264_scaleY);

            const short x = std::min(std::min(x1, x3), std::min(x2, x4));
            const short y = std::min(std::min(y1, y3), std::min(y2, y4));
            const short w = std::max(std::max(x1, x3), std::max(x2, x4));
            const short h = std::max(std::max(y1, y3), std::max(y2, y4));

            const PSX_RECT rect = { x, y, w, h };
            if (rect.w < 0 || rect.x > 640 || rect.y > 240 || rect.h < 0)
            {
                //TODO untie from Render() into Update()
                field_4C_collide_rects[i].x = 0;
                field_4C_collide_rects[i].w = 0;
                field_4C_collide_rects[i].y = 0;
                field_4C_collide_rects[i].h = 0;
            }
            else
            {
                Poly_F4* pPoly = &field_24_pRes[i].mPolys[gPsxDisplay_5C1130.field_C_buffer_index];
                SetXY0(pPoly, x1, y1);
                SetXY1(pPoly, x2, y2);
                SetXY2(pPoly, x3, y3);
                SetXY3(pPoly, x4, y4);

                OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_layer), &pPoly->mBase.header);

                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                    rect.x,
                    rect.y,
                    rect.w,
                    rect.h,
                    pScreenManager_5BB5F4->field_3A_idx);

                field_4C_collide_rects[i] = rect;
                field_4C_collide_rects[i].x = PCToPsxX(field_4C_collide_rects[i].x, 20);
                field_4C_collide_rects[i].w = PCToPsxX(field_4C_collide_rects[i].w, 20);
            }

            x3 = x1;
            y3 = y1;
            x4 = x2;
            y4 = y2;

            ang += angIncrement;
        }
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_layer), &field_2C_primSetTPage[gPsxDisplay_5C1130.field_C_buffer_index].mBase);
    }
}

void AbilityRing::vScreenChanged_49DE70()
{
    if (field_284_ring_type == RingTypes::eHealing_Emit_12)
    {
        for (int i=0; i<gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_4_typeId == Types::eMudokon_110)
            {
                if (pObj->field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed))
                {
                    // Only heal alive muds in the same screen
                    if (pObj->Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0 && pObj->field_10C_health > FP_FromInteger(0))
                    {
                        pObj->VPossessed_408F70();
                    }
                }
            }
        }
    }
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

int AbilityRing::vGetSaveState_49E070(AbilityRing_State* pSaveState)
{
    pSaveState->field_0_type = Types::eAbilityRing_104;
    pSaveState->field_4_xpos = field_24C_xpos;
    pSaveState->field_8_ypos = field_250_ypos;
    pSaveState->field_C_ring_type = field_284_ring_type;

    if (field_20_layer == Layer::eLayer_39)
    {
        pSaveState->field_10_scale = FP_FromInteger(1);
    }
    else
    {
        pSaveState->field_10_scale = FP_FromDouble(0.5);
    }

    pSaveState->field_18_right = field_258_right;
    pSaveState->field_1C_count = field_28C_count;

    pSaveState->field_20_r = field_276_r;
    pSaveState->field_22_g = field_278_g;
    pSaveState->field_24_b = field_27A_b;

    pSaveState->field_14_obj_id = -1;
    if (field_288_target_obj_id == -1)
    {
        return sizeof(AbilityRing_State);
    }

    BaseGameObject* pTargetObj = sObjectIds_5C1B70.Find_449CF0(field_288_target_obj_id);
    if (pTargetObj)
    {
        pSaveState->field_14_obj_id = pTargetObj->field_C_objectId;
    }
    return sizeof(AbilityRing_State);
}
