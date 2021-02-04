#include "stdafx_ao.h"
#include "Function.hpp"
#include "SnoozeParticle.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "CameraSwapper.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "Sfx.hpp"

namespace AO {

const short xPositionDeltaEntries_4CF8E0[39] =
{
    1,
    0,
    0,
    1,
    0,
    0,
    1,
    0,
    1,
    0,
    1,
    1,
    1,
    0,
    1,
    0,
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    -1,
    0,
    -1,
    0,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    -1,
    0,
    0,
    0,
    0
};

BaseGameObject* SnoozeParticle::VDestructor(signed int flags)
{
    return Vdtor_464CE0(flags);
}

BaseGameObject* SnoozeParticle::dtor_4644A0()
{
    SetVTable(this, 0x4BCA18);

    if (field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
    {
        gObjList_drawables_504618->Remove_Item(this);
    }
    return dtor_487DF0();
}

void SnoozeParticle::VScreenChanged_464CD0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SnoozeParticle::VScreenChanged()
{
    VScreenChanged_464CD0();
}

SnoozeParticle* SnoozeParticle::ctor_464320(FP xpos, FP ypos, Layer layer, FP scale)
{
    ctor_487E10(1);

    field_6_flags.Set(Options::eDrawable_Bit4);

    SetVTable(this, 0x4BCA18);

    field_4_typeId = Types::eSnoozParticle_87;
    gObjList_drawables_504618->Push_Back(this);

    field_10_x_start = xpos;
    field_14_y_start = ypos;
    field_18_x = xpos;
    field_1C_y = ypos;

    field_24_dy = FP_FromDouble(0.15);
    field_20_dx = FP_FromInteger(1);

    field_24_dy = ((field_24_dy * FP_FromInteger(Math_NextRandom())) / FP_FromInteger(256));
    field_24_dy += FP_FromDouble(0.35);
    field_24_dy = field_24_dy * FP_FromInteger(-1);

    field_30_layer = layer;

    field_28_scale = (scale * FP_FromDouble(0.4));

    field_2C_scale_dx = FP_FromDouble(0.30);
    field_2C_scale_dx = field_2C_scale_dx / (FP_FromInteger(20) / -field_24_dy);

    field_32_r = 0;
    field_34_g = 0;
    field_36_b = 0;
    field_1D4_state = SnoozeParticleState::Rising_0;
    field_3A_count_down = 1;
    field_38_idx = Math_NextRandom() % 36;
    field_20_dx = FP_FromInteger(xPositionDeltaEntries_4CF8E0[field_38_idx]);
    field_38_idx++;
    return this;
}

SnoozeParticle* SnoozeParticle::Vdtor_464CE0(signed int flags)
{
    dtor_4644A0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void SnoozeParticle::VUpdate()
{
    VUpdate_464500();
}

void SnoozeParticle::VUpdate_464500()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!sNumCamSwappers_507668)
    {
        switch (field_1D4_state)
        {
        case SnoozeParticleState::Rising_0:
            if (field_1C_y >= field_14_y_start - FP_FromInteger(20))
            {
                if (field_32_r < 70 &&
                    field_34_g < 70 &&
                    field_36_b < 20)
                {
                    field_32_r += 14;
                    field_34_g += 14;
                    field_36_b += 4;
                }

                field_28_scale += field_2C_scale_dx;

                if (field_38_idx > 36)
                {
                    field_38_idx = 0;
                }

                const FP idx_toFP = FP_FromInteger(xPositionDeltaEntries_4CF8E0[field_38_idx]);
                field_20_dx = idx_toFP;
                field_38_idx += 1;
                field_18_x += idx_toFP;
                field_1C_y += field_24_dy;
            }
            else
            {
                field_1D4_state = SnoozeParticleState::BlowingUp_2;
            }
            break;

        case SnoozeParticleState::Unused_1:
            break;

        case SnoozeParticleState::BlowingUp_2:
            field_32_r /= 2;
            field_34_g /= 2;
            field_36_b /= 2;
            field_18_x += field_20_dx;
            field_1C_y += field_24_dy;

            if (field_3A_count_down > 0)
            {
                field_3A_count_down--;
            }
            else
            {
                SFX_Play_43AD70(SoundEffect::ZPop_5, 0, 0);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
    }
}


void SnoozeParticle::VRender(PrimHeader** ppOt)
{
    VRender_464620(ppOt);
}

//Identical to AE
const PSX_Point explosionVerts[6][2] =
{
    { { -3, -4 }, { -6, -7 } },
    { { 3, -4 }, { 6, -7 } },
    { { 4, -1 }, { 7, -1 } },
    { { -4, 1 }, { -7, 1 } },
    { { -3, 4 }, { -6, 7 } },
    { { 3, 4 }, { 6, 7 } }
};
const __int16 zVerts[8] =
{
    -4,
    -4,
    4,
    -4,
    -4,
    4,
    4,
    4
};

void SnoozeParticle::VRender_464620(PrimHeader** ppOt)
{
    //Identical to AE except xInScreen, yInScreen are offset by pScreenManager_4FF7C8 positions
    PSX_RECT rectToInvalidate = {};
    FP_Point* pCamPos = pScreenManager_4FF7C8->field_10_pCamPos;
    const __int16 bufIdx = gPsxDisplay_504C78.field_A_buffer_index;

    if (field_1D4_state == SnoozeParticleState::BlowingUp_2)
    {
        const __int16 xInScreen = FP_GetExponent(field_18_x - pCamPos->field_0_x) + pScreenManager_4FF7C8->field_14_xpos;
        const __int16 yInScreen = FP_GetExponent(field_1C_y - pCamPos->field_4_y) + pScreenManager_4FF7C8->field_16_ypos;

        for (int i = 0; i < ALIVE_COUNTOF(explosionVerts); i++)
        {
            Line_G2* pZExplosionLine = &field_94_lines[bufIdx][i];
            LineG2_Init(pZExplosionLine);

            const int scaledLineRelativeStartX = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].field_0_x) * field_28_scale);
            const int scaledLineRelativeStartY = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].field_2_y) * field_28_scale);
            const int scaledLineRelativeEndX = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].field_0_x) * field_28_scale);
            const int scaledLineRelativeEndY = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].field_2_y) * field_28_scale);
            SetXY0(pZExplosionLine,
                static_cast<__int16>(PsxToPCX(xInScreen + scaledLineRelativeStartX, 11)),
                static_cast<__int16>(yInScreen + scaledLineRelativeStartY)
            );
            SetXY1(pZExplosionLine,
                static_cast<__int16>(PsxToPCX(xInScreen + scaledLineRelativeEndX, 11)),
                static_cast<__int16>(yInScreen + scaledLineRelativeEndY)
            );

            SetRGB0(pZExplosionLine,
                static_cast<BYTE>(field_32_r / 2),
                static_cast<BYTE>(field_34_g / 2),
                static_cast<BYTE>(field_36_b / 2)
            );
            SetRGB1(pZExplosionLine,
                static_cast<BYTE>(field_32_r),
                static_cast<BYTE>(field_34_g),
                static_cast<BYTE>(field_36_b)
            );

            Poly_Set_SemiTrans_498A40(&pZExplosionLine->mBase.header, 1);
            OrderingTable_Add_498A80(OtLayer(ppOt, field_30_layer), &pZExplosionLine->mBase.header);
        }
        rectToInvalidate.x = static_cast<__int16>(PsxToPCX(xInScreen - 8, 0));
        rectToInvalidate.w = static_cast<__int16>(PsxToPCX(xInScreen + 8, 0));
        rectToInvalidate.y = static_cast<__int16>(yInScreen - 8);
        rectToInvalidate.h = static_cast<__int16>(yInScreen + 8);
    }
    else
    {
        Line_G4* pZLine = &field_3C_lines[bufIdx];
        LineG4_Init(pZLine);

        const __int16 xInScreen = FP_GetExponent(field_18_x - pCamPos->field_0_x) + pScreenManager_4FF7C8->field_14_xpos;
        const __int16 yInScreen = FP_GetExponent(field_1C_y - pCamPos->field_4_y) + pScreenManager_4FF7C8->field_16_ypos;

        const __int16 RectX_v_Psx = xInScreen + FP_GetExponent(FP_FromInteger(zVerts[0]) * field_28_scale);
        const __int16 RectW_v_Psx = xInScreen + FP_GetExponent(FP_FromInteger(zVerts[5]) * field_28_scale);

        const __int16 rectX_v = PsxToPCX(RectX_v_Psx, 11);
        const __int16 rectY_v = FP_GetExponent(FP_FromInteger(zVerts[1]) * field_28_scale) + yInScreen;
        const __int16 rectW_v = PsxToPCX(RectW_v_Psx, 11);
        const __int16 rectH_v = yInScreen + FP_GetExponent(FP_FromInteger(zVerts[7]) * field_28_scale);

        SetXY0(pZLine,
            rectX_v,
            rectY_v
        );
        SetXY1(pZLine,
            static_cast<__int16>(PsxToPCX(xInScreen + FP_GetExponent(FP_FromInteger(zVerts[2]) * field_28_scale), 11)),
            yInScreen + FP_GetExponent(FP_FromInteger(zVerts[3]) * field_28_scale)
        );
        SetXY2(pZLine,
            static_cast<__int16>(PsxToPCX(xInScreen + FP_GetExponent(FP_FromInteger(zVerts[4]) * field_28_scale), 11)),
            yInScreen + FP_GetExponent(FP_FromInteger(zVerts[5]) * field_28_scale)
        );
        SetXY3(pZLine,
            rectW_v,
            rectH_v
        );

        SetRGB0(pZLine,
            static_cast<BYTE>(field_32_r * 8 / 10),
            static_cast<BYTE>(field_34_g * 8 / 10),
            static_cast<BYTE>(field_36_b * 8 / 10)
        );
        SetRGB1(pZLine,
            static_cast<BYTE>(field_32_r),
            static_cast<BYTE>(field_34_g),
            static_cast<BYTE>(field_36_b)
        );
        SetRGB2(pZLine,
            static_cast<BYTE>(field_32_r * 7 / 10),
            static_cast<BYTE>(field_34_g * 7 / 10),
            static_cast<BYTE>(field_36_b * 7 / 10)
        );
        SetRGB3(pZLine,
            static_cast<BYTE>(field_32_r / 2),
            static_cast<BYTE>(field_34_g / 2),
            static_cast<BYTE>(field_36_b / 2)
        );

        Poly_Set_SemiTrans_498A40(&pZLine->mBase.header, 1);
        OrderingTable_Add_498A80(OtLayer(ppOt, field_30_layer), &pZLine->mBase.header);
        rectToInvalidate.x = rectX_v;
        rectToInvalidate.y = rectY_v;
        rectToInvalidate.w = rectW_v;
        rectToInvalidate.h = rectH_v;
    }
    Prim_SetTPage* thisTPage = &field_1B4_tPage[bufIdx];
    const int tPage = PSX_getTPage_4965D0(0, 1, 0, 0);
    Init_SetTPage_495FB0(thisTPage, 1, 0, tPage);
    OrderingTable_Add_498A80(OtLayer(ppOt, field_30_layer), &thisTPage->mBase);

    pScreenManager_4FF7C8->InvalidateRect_406E40(
        rectToInvalidate.x, rectToInvalidate.y,
        rectToInvalidate.w, rectToInvalidate.h,
        pScreenManager_4FF7C8->field_2E_idx
    );
}

}
