#include "stdafx.h"
#include "ShadowZone.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Events.hpp"

BaseGameObject* ShadowZone::VDestructor(signed int flags)
{
    return vdtor_463A70(flags);
}

void ShadowZone::VUpdate()
{
    vUpdate_463C40();
}

void ShadowZone::VScreenChanged()
{
    vScreenChanged_463CC0();
}

ShadowZone* ShadowZone::ctor_463900(Path_ShadowZone* pTlv, Path* /*pPath*/, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545E10); // vTbl_ShadowZone_545E10
    
    sShadowZone_dArray_5C1B80->Push_Back(this);
    
    field_26_path = gMap_5C3030.field_2_current_path;
    field_24_level = gMap_5C3030.field_0_current_level;
    
    field_20_tlvInfo = tlvInfo;

    field_30_mid_x = (pTlv->field_C_bottom_right.field_0_x - pTlv->field_8_top_left.field_0_x) / 2;
    field_32_mid_y = (pTlv->field_C_bottom_right.field_2_y - pTlv->field_8_top_left.field_2_y) / 2;
    
    field_28_centre_x = field_30_mid_x + pTlv->field_8_top_left.field_0_x;
    field_2C_centre_mid_x = field_30_mid_x - 75;
    if (field_2C_centre_mid_x < 0)
    {
        field_2C_centre_mid_x = 0;
    }

    field_2A_center_y = field_32_mid_y + pTlv->field_8_top_left.field_2_y;
    field_2E_centre_mid_y = field_32_mid_y - 75;
    if (field_2E_centre_mid_y < 0)
    {
        field_2E_centre_mid_y = 0;
    }

    field_38_r = FP_FromInteger(pTlv->field_14_r);
    field_3C_g = FP_FromInteger(pTlv->field_16_g);
    field_40_b = FP_FromInteger(pTlv->field_18_b);

    field_34_id = pTlv->field_1A_id;
    field_36_scale = pTlv->field_1C_scale;

    return this;
}

EXPORT void CC ShadowZone::ShadowZones_Calculate_Colour_463CE0(int xpos, int ypos, __int16 scale, short* r, short* g, short* b)
{
    for (int idx = 0; idx < sShadowZone_dArray_5C1B80->Size(); idx++)
    {
        ShadowZone* pShadow = sShadowZone_dArray_5C1B80->ItemAt(idx);
        if (!pShadow)
        {
            break;
        }

        if (pShadow->ApplysToScale_463C70(scale))
        {
            FP amount = {};
            // TODO: This was probably a reference, refactor later
            pShadow->GetColourAmount_463AA0(&amount, static_cast<short>(xpos), static_cast<short>(ypos));

            *r = FP_GetExponent(FP_FromInteger(*r) + (pShadow->field_38_r * amount));
            *b = FP_GetExponent(FP_FromInteger(*b) + (pShadow->field_40_b * amount));

            // NOTE: Never seems to be enabled, a debugging feature so instead of being hidden
            // in the shadow zones you appear green.
            //if (!word_5C1BE2)
            {
                *g = FP_GetExponent(FP_FromInteger(*g) + (pShadow->field_3C_g * amount));
            }
            
        }
    }

    // Clamp min/max
    if (*r > 255)
    {
        *r = 255;
    }

    if (*r < 0)
    {
        *r = 0;
    }

    if (*g > 255)
    {
        *g = 255;
    }

    if (*g < 0)
    {
        *g = 0;
    }

    if (*b > 255)
    {
        *b = 255;
    }

    if (*b < 0)
    {
        *b = 0;
    }
}

void ShadowZone::dtor_463BB0()
{
    SetVTable(this, 0x545E10); // vTbl_ShadowZone_545E10
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    sShadowZone_dArray_5C1B80->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

ShadowZone* ShadowZone::vdtor_463A70(signed int flags)
{
    dtor_463BB0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void ShadowZone::vScreenChanged_463CC0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

__int16 ShadowZone::ApplysToScale_463C70(__int16 scale)
{
    if (field_36_scale == ShadowZoneScale::eBoth_0)
    {
        return 1;
    }
    else if (field_36_scale == ShadowZoneScale::eFull_2 && scale == 1)
    {
        return 1;
    }
    else if (field_36_scale == ShadowZoneScale::eHalf_1 && scale == 0)
    {
        return 1;
    }
    return 0;
}

void ShadowZone::vUpdate_463C40()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

FP* ShadowZone::GetColourAmount_463AA0(FP* pOut, __int16 xpos, __int16 ypos)
{
    const int deltaX = abs(xpos - field_28_centre_x);
    const int deltaY = abs(ypos - field_2A_center_y);

    if (deltaX > field_30_mid_x || deltaY > field_32_mid_y)
    {
        // Not in range
        *pOut = FP_FromInteger(0);
    }
    else
    {
        if (deltaX > field_2C_centre_mid_x)
        {
            // Partly in range
            const FP midXDistance = FP_FromInteger(field_30_mid_x - field_2C_centre_mid_x);
            const FP value = midXDistance - FP_FromInteger(deltaX - field_2C_centre_mid_x);

            *pOut = value / midXDistance;
        }
        else
        {
            // Fully in range
            *pOut = FP_FromInteger(1);
        }
    }
    return pOut;
}
