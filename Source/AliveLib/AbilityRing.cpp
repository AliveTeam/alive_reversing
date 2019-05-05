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

EXPORT AbilityRing * CC AbilityRing::Factory_482F80(FP xpos, FP ypos, RingTypes type, FP scale)
{
    auto pRing = alive_new<AbilityRing>();
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

    field_4_typeId = Types::eType_104;
    field_288_target_obj_id = -1;
    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable);

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
            field_26C = static_cast<short>(MinDistance(field_272_screenXPos, field_274_screenYPos, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height, 0, gPsxDisplay_5C1130.field_2_height));
        }
        else
        {
            field_26C = static_cast<short>(MinDistance(field_272_screenXPos, field_274_screenYPos, gPsxDisplay_5C1130.field_0_width, 0, 0, 0));
        }

        field_284_ring_type = ringType;

        switch (field_284_ring_type)
        {
        case RingTypes::eExplosive_Emit_1:
        case RingTypes::eHealing_Emit_12:
            for (PSX_RECT& r : field_4C)
            {
                r = {};
            }
            // Fall through

        case RingTypes::eExplosive_Emit_Effect_2:
        case RingTypes::eInvisible_Pulse_Emit_9:
        case RingTypes::eHealing_Emit_Effect_11:
            field_268 = FP_FromInteger(8);
            field_25C_speed = FP_FromInteger(6);
            field_258 = FP_FromInteger(6);
            field_254 = FP_FromInteger(0);

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

            SFX_Play_46FA90(84, 0);
            break;

        case RingTypes::eExplosive_Give_3:
        case RingTypes::eInvisible_Pulse_Give_10:
        case RingTypes::eHealing_Give_13:
            field_268 = FP_FromInteger(8);
            field_25C_speed = FP_FromInteger(6);
            field_258 = FP_FromInteger(350);
            field_254 = FP_FromInteger(342);
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
            field_268 = FP_FromInteger(5);
            field_25C_speed = FP_FromInteger(4);
            field_258 = FP_FromInteger(4);
            field_254 = FP_FromInteger(0);
            field_26C = 50;

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

        field_282_path = gMap_5C3030.sCurrentPathId_5C3032;
        field_20_layer = 39;
        field_280_level = gMap_5C3030.sCurrentLevelId_5C3030;
        field_27C_prims = 1;
        field_27E_tPageMode = 1;

        if (field_284_ring_type == RingTypes::eShrykull_Pulse_Orange_6 && scale == FP_FromDouble(0.5))
        {
            field_20_layer = 16;
        }

        field_260 = 0x11999;
        field_264 = 0x10000;
        field_268 = (field_268 * scale);
        field_25C_speed = (field_25C_speed * scale);
        field_254 = (field_254 * scale);
        field_258 = (field_258 * scale);

        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                Poly_F4* pPoly = &field_24_pRes[x].mPolys[y];
                PolyF4_Init_4F8830(pPoly);
                SetRGB0(pPoly, field_276_r & 0xFF, field_278_g & 0xFF, field_27A_b & 0xFF);
                Poly_Set_SemiTrans_4F8A60(&pPoly->mBase.header, field_27C_prims);
            }
            Init_SetTPage_4F5B60(&field_2C_primSetTPage[y], 0, 0, PSX_getTPage_4F60E0(2, static_cast<char>(field_27E_tPageMode), 0, 0));
        }
        field_28C_count = 64;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
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

void AbilityRing::VSetTarget(BaseGameObject* pTarget)
{
    vSetTarget_49D140(pTarget);
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
        Mem_Free_495540(this);
    }
    return this;
}

void AbilityRing::vUpdate_49D160()
{
    if (field_290)
    {
        field_290 = 0;
        field_288_target_obj_id = Find_Flags_4DC170(field_288_target_obj_id);
    }

    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_288_target_obj_id));
    if (pTarget)
    {
        if (pTarget->field_6_flags.Get(BaseGameObject::eDead))
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
        field_258 = field_25C_speed + field_258;
        field_254 = field_258 - field_268;
        if (field_258 - field_268 < FP_FromInteger(0))
        {
            field_254 = FP_FromInteger(0);
        }

        if (FP_GetExponent(field_254) <= field_26C)
        {
            field_276_r = (field_276_r >> 1) + (field_276_r >> 2);
            field_278_g = (field_278_g >> 1) + (field_278_g >> 2);
            field_27A_b = (field_27A_b >> 1) + (field_27A_b >> 2);
            for (int j = 0; j < 2; j++)
            {
                for (int i = 0; i < 64; i++)
                {
                    SetRGB0(&field_24_pRes[i].mPolys[j], field_276_r & 0xFF, field_278_g & 0xFF, field_27A_b & 0xFF);
                }
            }
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
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
        field_258 = field_25C_speed + field_258;
        field_254 = field_258 - field_268;
        if (field_258 - field_268 < FP_FromInteger(0))
        {
            field_254 = FP_FromInteger(0);
        }

        if (FP_GetExponent(field_254) > field_26C)
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        break;

    case RingTypes::eExplosive_Give_3:
    case RingTypes::eInvisible_Pulse_Give_10:
    case RingTypes::eHealing_Give_13:
        field_258 = field_258 - field_25C_speed;
        field_254 = field_258 - field_268;
        if (field_258 - field_268 < FP_FromInteger(0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
            field_254 = FP_FromInteger(0);
            SFX_Play_46FA90(0x54u, 0);
            if (field_284_ring_type == RingTypes::eExplosive_Give_3)
            {
                auto v26 = alive_new<PossessionFlicker>();
                if (v26)
                {
                    v26->ctor_4319E0(sActiveHero_5C1B68, 8, 255, 128, 128);
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
        field_4C[i].x += field_26E_screenX;
        field_4C[i].y += field_270_screenY;
        field_4C[i].w += field_26E_screenX;
        field_4C[i].h += field_270_screenY;
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

        if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
        {
            for (int j = 0; j < field_28C_count; j++)
            {
                if (field_4C[j].x <= bRect.w &&
                    field_4C[j].w >= bRect.x &&
                    field_4C[j].h >= bRect.y &&
                    field_4C[j].y <= bRect.h)
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
                        if (pObj->field_114_flags.Get(Flags_114::e114_Bit3_Can_Be_Possessed))
                        {
                            if (pObj->field_10C_health > FP_FromInteger(0))
                            {
                                pObj->Vnull_408F70();
                            }
                        }
                    }
                }
            }
        }
    }
}
