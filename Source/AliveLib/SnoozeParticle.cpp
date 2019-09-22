#include "stdafx.h"
#include "SnoozeParticle.hpp"
#include "Events.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

const __int16 word_560408[39] =
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
    -3
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

const PSX_Point explosionVerts[6][2] =
{
    { { -3, -4 }, { -6, -7 } },
    { { 3, -4 }, { 6, -7 } },
    { { 4, -1 }, { 7, -1 } },
    { { -4, 1 }, { -7, 1 } },
    { { -3, 4 }, { -6, 7 } },
    { { 3, 4 }, { 6, 7 } }
};

SnoozeParticle* SnoozeParticle::ctor_4B06F0(FP xpos, FP ypos, __int16 layer, FP scale)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_6_flags.Set(BaseGameObject::eDrawable);
    
    SetVTable(this, 0x5472FC);

    field_4_typeId = Types::eSnoozParticle_124;
    gObjList_drawables_5C1124->Push_Back(this);

    field_20_x_start = xpos;
    field_24_y_start = ypos;
    field_28_x = xpos;
    field_2C_y = ypos;

    // Interesting calc.. ??
    field_34_dy = (FP_FromDouble(0.15) * FP_FromInteger(Math_NextRandom())) / FP_FromInteger(256);
    field_34_dy += FP_FromDouble(0.35);
    field_34_dy = (field_34_dy + FP_FromDouble(0.35)) * FP_FromInteger(-1);

    field_40_layer = layer;

    field_38_scale = scale * FP_FromDouble(0.4);

    field_3C_scale_dx = FP_FromDouble(0.30);
    field_3C_scale_dx = field_3C_scale_dx / (FP_FromInteger(20) / -field_34_dy);

    field_42_r = 0;
    field_44_g = 0;
    field_46_b = 0;
    field_1E4_state = SnoozeParticleState::Init_0;
    field_4A_count_down = 1;
    field_48_idx = Math_NextRandom() % 36;
    field_30_dx = FP_FromDouble(word_560408[field_48_idx]);
    field_48_idx++;
    return this;
}

void SnoozeParticle::dtor_4B0900()
{
    SetVTable(this, 0x5472FC);
    if (field_6_flags.Get(BaseGameObject::eDrawable))
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }
    BaseGameObject_dtor_4DBEC0();
}

SnoozeParticle* SnoozeParticle::vdtor_4B08D0(signed int flags)
{
    dtor_4B0900();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void SnoozeParticle::Update_4B0980()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    if (!sNum_CamSwappers_5C1B66)
    {
        if (field_1E4_state != SnoozeParticleState::Init_0)
        {
            int previousState = (__int16) field_1E4_state - 1;
            if (!previousState)
            {
                field_6_flags.Set(BaseGameObject::eDead);
                return;
            }
            if (previousState != 1)
            {
                return;
            }
            field_42_r = field_42_r >> 1;
            field_44_g = field_44_g >> 1;
            field_46_b = field_46_b >> 1;
            field_28_x += field_30_dx;
            field_2C_y += field_34_dy;

            if (field_4A_count_down)
            {
                field_4A_count_down = field_4A_count_down - 1;
            }
            else
            {
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }
        else if (field_2C_y >= field_24_y_start - FP_FromInteger(20))
        {
            if (field_42_r < 70)
            {
                if (field_44_g < 70)
                {
                    if (field_46_b < 20)
                    {
                        field_42_r += 14;
                        field_44_g += 14;
                        field_46_b += 4;
                    }
                }
            }
            field_38_scale += field_3C_scale_dx;
            if (field_48_idx > 36)
            {
                field_48_idx = 0;
            }
            FP field_48_idx_toFP = FP_FromInteger(word_560408[field_48_idx]);
            field_30_dx = field_48_idx_toFP;
            field_28_x += field_48_idx_toFP;
            field_2C_y += field_34_dy;
            field_48_idx += 1;
        }
        else
        {
            field_1E4_state = SnoozeParticleState::BlowingUp_2;
            SFX_Play_46FA90(4, 0, field_38_scale);
        }
    }
}

void SnoozeParticle::Render_4B0AF0(int **pOt)
{
    PSX_RECT rectToInvalidate = {};
    FP_Point *pCamPos = pScreenManager_5BB5F4->field_20_pCamPos;
    __int16 bufIdx = gPsxDisplay_5C1130.field_C_buffer_index;

    if (field_1E4_state == SnoozeParticleState::BlowingUp_2)
    {
        int x_diff = FP_GetExponent(field_28_x - pCamPos->field_0_x);
        int y_diff = FP_GetExponent(field_2C_y - pCamPos->field_4_y);

        for (int i = 0; i < ALIVE_COUNTOF(explosionVerts); i++)
        {
            Line_G2* pZExplosionLine = &field_A4_G2_lines[bufIdx][i];
            pZExplosionLine->mBase.header.header.mNormal.field_4_num_longs = 4;
            pZExplosionLine->mBase.header.header.mNormal.field_5_unknown = byte_BD146C;
            int scaledLineRelativeStartX = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].field_0_x) * field_38_scale);
            int scaledLineRelativeStartY = FP_GetExponent(FP_FromInteger(explosionVerts[i][0].field_2_y) * field_38_scale);
            int scaledLineRelativeEndX = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].field_0_x) * field_38_scale);
            int scaledLineRelativeEndY = FP_GetExponent(FP_FromInteger(explosionVerts[i][1].field_2_y) * field_38_scale);
            int v11 = 40 * ( x_diff + scaledLineRelativeStartX ) + 11;
            int v12 = 40 * ( x_diff + scaledLineRelativeEndX) + 11;
            pZExplosionLine->mBase.vert.x = ( __int16 ) ( ( ( unsigned int ) ( v11 + ( ( unsigned __int64 ) ( -1307163959i64 * v11 ) >> 32 ) ) >> 31 ) + ( ( int ) ( v11 + ( ( unsigned __int64 ) ( -1307163959i64 * v11 ) >> 32 ) ) >> 4 ) );
            pZExplosionLine->mBase.vert.y = ( __int16 ) ( y_diff + scaledLineRelativeStartY );
            pZExplosionLine->mVerts[0].mVert.x = ( __int16 ) ( ( ( unsigned int ) ( v12 + ( ( unsigned __int64 ) ( -1307163959i64 * v12 ) >> 32 ) ) >> 31 ) + ( ( int ) ( v12 + ( ( unsigned __int64 ) ( -1307163959i64 * v12 ) >> 32 ) ) >> 4 ) );
            pZExplosionLine->mVerts[0].mVert.y = ( __int16 ) ( y_diff + scaledLineRelativeEndY );

            pZExplosionLine->mBase.header.rgb_code.r = ( BYTE ) (field_42_r / 2);
            pZExplosionLine->mBase.header.rgb_code.g = ( BYTE ) (field_44_g / 2);
            pZExplosionLine->mBase.header.rgb_code.b = ( BYTE ) (field_46_b / 2);
            pZExplosionLine->mBase.header.rgb_code.code_or_pad = 80;
            pZExplosionLine->mVerts[0].mRgb.r = ( BYTE ) field_42_r;
            pZExplosionLine->mVerts[0].mRgb.g = ( BYTE ) field_44_g;
            pZExplosionLine->mVerts[0].mRgb.b = ( BYTE ) field_46_b;

            Poly_Set_SemiTrans_4F8A60( &pZExplosionLine->mBase.header, 1);
            OrderingTable_Add_4F8AA0(&pOt[field_40_layer], &pZExplosionLine->mBase.header);
        }
        rectToInvalidate.x = ( __int16 ) ( ( ( unsigned int ) ( 8 * ( 5 * ( __int16 ) x_diff - 40 ) + ( ( unsigned __int64 ) ( -10457311672i64 * ( 5 * ( __int16 ) x_diff - 40 ) ) >> 32 ) ) >> 31 ) + ( ( signed int ) ( 8 * ( 5 * ( __int16 ) x_diff - 40 ) + ( ( unsigned __int64 ) ( -10457311672i64 * ( 5 * ( __int16 ) x_diff - 40 ) ) >> 32 ) ) >> 4 ) );
        unsigned int rectW_partial_v = ( int ) ( 8 * ( 5 * ( __int16 ) x_diff + 40 ) + ( ( unsigned __int64 ) ( -10457311672i64 * ( 5 * ( __int16 ) x_diff + 40 ) ) >> 32 ) ) >> 4;
        rectToInvalidate.w = ( __int16 ) ( ( rectW_partial_v >> 31 ) + rectW_partial_v );
        rectToInvalidate.y = ( __int16 ) ( y_diff - 8 );
        rectToInvalidate.h = ( __int16 ) ( y_diff + 8 );
    }
    else
    {
        __int16 xDiff = FP_GetExponent(field_28_x - FP_FromInteger(FP_GetExponent(pCamPos->field_0_x)));
        __int16 yDiff = FP_GetExponent(field_2C_y - FP_FromInteger(FP_GetExponent(pCamPos->field_4_y)));

        Line_G4* pZLine = &field_4C_G4_lines[bufIdx];
        pZLine->mBase.header.header.mNormal.field_4_num_longs = 9;
        pZLine->mBase.header.header.mNormal.field_5_unknown = byte_BD146C;
        pZLine->mBase.header.rgb_code.code_or_pad = 92;
        pZLine->field_28_pad = 0x55555555;

        __int16 rectX_v = ( 40 * ( xDiff + FP_GetExponent(FP_FromInteger(zVerts[0]) * field_38_scale) ) + 11 ) / 23;
        __int16 rectY_v = FP_GetExponent(FP_FromInteger(zVerts[1]) * field_38_scale) + yDiff;
        __int16 rectW_v = ( 40 * ( xDiff + FP_GetExponent(FP_FromInteger(zVerts[5]) * field_38_scale) ) + 11 ) / 23;
        __int16 rectH_v = yDiff + FP_GetExponent(FP_FromInteger(zVerts[7]) * field_38_scale);

        pZLine->mBase.vert.x = rectX_v;
        pZLine->mBase.vert.y = rectY_v;

        __int16 v20 = xDiff + FP_GetExponent(FP_FromInteger(zVerts[2]) * field_38_scale);
        pZLine->mVerts[0].mVert.x = ( ( unsigned int ) ( 40 * v20 + 11 + ( ( unsigned __int64 ) ( -1307163959i64 * ( 40 * v20 + 11 ) ) >> 32 ) ) >> 31 ) + ( ( int ) ( 40 * v20 + 11 + ( ( unsigned __int64 ) ( -1307163959i64 * ( 40 * v20 + 11 ) ) >> 32 ) ) >> 4 );
        pZLine->mVerts[0].mVert.y = yDiff + FP_GetExponent(FP_FromInteger(zVerts[3]) * field_38_scale);
        __int16 v22 = xDiff + FP_GetExponent(FP_FromInteger(zVerts[4]) * field_38_scale);
        unsigned int v33 = 40 * v22 + 11 + ( ( unsigned __int64 ) ( -1307163959i64 * ( 40 * v22 + 11 ) ) >> 32 );
        pZLine->mVerts[1].mVert.x = ( __int16 ) ( ( v33 >> 31 ) + ( ( signed int ) v33 >> 4 ) );
        pZLine->mVerts[1].mVert.y = yDiff + FP_GetExponent(FP_FromInteger(zVerts[5]) * field_38_scale);;

        pZLine->mVerts[2].mVert.x = rectW_v;
        pZLine->mVerts[2].mVert.y = rectH_v;

        pZLine->mBase.header.rgb_code.r = (BYTE ) ( field_42_r * 8 / 10 );
        pZLine->mBase.header.rgb_code.g = (BYTE ) (field_44_g * 8 / 10);
        pZLine->mBase.header.rgb_code.b = (BYTE ) (field_46_b * 8 / 10);
        pZLine->mVerts[0].mRgb.r = ( BYTE) field_42_r;
        pZLine->mVerts[0].mRgb.g = ( BYTE) field_44_g;
        pZLine->mVerts[0].mRgb.b = ( BYTE) field_46_b;
        pZLine->mVerts[1].mRgb.r = ( BYTE) (field_42_r * 7 / 10);
        pZLine->mVerts[1].mRgb.g = ( BYTE) (field_44_g * 7 / 10);
        pZLine->mVerts[1].mRgb.b = ( BYTE) (field_46_b * 7 / 10);
        pZLine->mVerts[2].mRgb.r = ( BYTE) (field_42_r * 5 / 10 );
        pZLine->mVerts[2].mRgb.g = ( BYTE) (field_44_g * 5 / 10 );
        pZLine->mVerts[2].mRgb.b = (BYTE) (field_46_b * 5 / 10 );
        Poly_Set_SemiTrans_4F8A60(&pZLine->mBase.header, 1);
        OrderingTable_Add_4F8AA0(&pOt[field_40_layer], &pZLine->mBase.header);
        rectToInvalidate.x = rectX_v;
        rectToInvalidate.y = rectY_v;
        rectToInvalidate.w = rectW_v;
        rectToInvalidate.h = rectH_v;
    }
    Prim_SetTPage* thisTPage = &field_1C4_tPage[bufIdx];
    int tPage = PSX_getTPage_4F60E0(0, 1, 0, 0);
    Init_SetTPage_4F5B60(thisTPage, 1, 0, tPage);
    OrderingTable_Add_4F8AA0(&pOt[field_40_layer], &thisTPage->mBase);

    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        rectToInvalidate.x, rectToInvalidate.y,
        rectToInvalidate.w, rectToInvalidate.h,
        pScreenManager_5BB5F4->field_3A_idx
    );
}

void SnoozeParticle::vScreenChanged_4B1300()
{
    field_6_flags.Set(BaseGameObject::eDead);
}
