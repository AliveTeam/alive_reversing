#include "stdafx.h"
#include "ZapLine.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Game.hpp"

EXPORT ZapLine* ZapLine::ctor_4CC690(FP x1, FP y1, FP x2, FP y2, __int16 aliveTime, signed __int16 type, __int16 layer)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    field_12A_type = type;
    SetVTable(this, 0x5478A0);
    field_4_typeId = Types::eZapLine_135;

    int frameTable = 0;
    if (type == 1)
    {
        field_130_count_per_part = 20;
        field_12E_part_count = 12;
        frameTable = 276;
        field_12C_tPageAbr = 3;
    }
    else if (type == 0)
    {
        field_130_count_per_part = 10;
        field_12E_part_count = 28;
        frameTable = 288;
        field_12C_tPageAbr = 1;
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSplineResID);
    Animation_Init_424E10(frameTable, 14, 9, ppRes, 1, 1u);

    field_132_pSprts_count = field_12E_part_count * field_130_count_per_part;
    field_20_animation.field_4_flags.Set(AnimFlags::eBit25_bDecompressDone); // HIBYTE |= 1
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_C_render_layer = layer;
    
    field_F8_ppRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::ResourceType::Resource_Spline, 0, sizeof(ZapLineSprites) * field_132_pSprts_count);// Spln (spline?)
    
    field_134_pSprts = reinterpret_cast<ZapLineSprites*>(*field_F8_ppRes);
    field_138_buf1 = reinterpret_cast<PSX_Point*>(malloc_non_zero_4954F0(sizeof(PSX_Point) * field_132_pSprts_count));
    field_13C_buf2 = reinterpret_cast<ZapPoint*>(malloc_non_zero_4954F0(sizeof(ZapPoint) * field_130_count_per_part));
    field_140_buf3 = reinterpret_cast<FP_Point*>(malloc_non_zero_4954F0(sizeof(FP_Point) * field_12E_part_count));
    
    field_128_max_alive_time = aliveTime;
    
    field_B8_xpos = x1;
    field_BC_ypos = y1;

    field_F4_state = State::State_0;
    field_126_alive_timer = 0;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
    {
        field_124_tPageMode = 1;
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
    {
        field_124_tPageMode = 2;
    }
    else
    {
        field_124_tPageMode = 0; // 4 bit
    }

    BYTE u0 = field_20_animation.field_84_vram_rect.x & 0x3F;
    if (field_124_tPageMode == 1)
    {
        u0 = 2 * u0;
    }
    else if (!field_124_tPageMode)
    {
        u0 = 4 * u0;
    }

    auto pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[field_20_animation.Get_FrameHeader_40B730(-1)->field_0_frame_header_offset]);

    const BYTE frameW = pFrameHeader->field_4_width;
    const BYTE frameH = pFrameHeader->field_5_height;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < field_12E_part_count; j++)
        {
            for (int k = 0; k < field_130_count_per_part; k++)
            {
                Prim_Sprt* pSprt = &field_134_pSprts[(j * field_130_count_per_part) + k].field_0_sprts[i];
                Sprt_Init_4F8910(pSprt);

                Poly_Set_SemiTrans_4F8A60(&pSprt->mBase.header, 1);
                Poly_Set_Blending_4F8A20(&pSprt->mBase.header, 1);

                SetClut(pSprt, static_cast<short>(PSX_getClut_4F6350(field_20_animation.field_8C_pal_vram_xy.field_0_x, field_20_animation.field_8C_pal_vram_xy.field_2_y)));

                SetUV0(pSprt, u0, field_20_animation.field_84_vram_rect.y & 0xFF);
                pSprt->field_14_w = frameW - 1;
                pSprt->field_16_h = frameH - 1;
            }
        }
    }

    sub_4CCAD0(x1, y1, x2, y2);

    return this;
}

BaseGameObject* ZapLine::VDestructor(signed int flags)
{
    return vdtor_4CCAA0(flags);
}

void ZapLine::VUpdate()
{
    vUpdate_4CD790();
}

void ZapLine::VRender(int** pOrderingTable)
{
    vRender_4CD8C0(pOrderingTable);
}


void ZapLine::VScreenChanged()
{
    vScreenChanged_4CDBE0();
}

ZapLine* ZapLine::vdtor_4CCAA0(signed int flags)
{
    dtor_4CCCB0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ZapLine::sub_4CCAD0(FP x1, FP y1, FP x2, FP y2)
{
    field_11C_x1 = FP_GetExponent(x1 - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    field_11E_y1 = FP_GetExponent(y1 - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
    field_120_x2 = FP_GetExponent(x2 - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    field_122_y2 = FP_GetExponent(y2 - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

    field_11C_x1 = PsxToPCX(field_11C_x1, 11);
    field_120_x2 = PsxToPCX(field_120_x2, 11); 

    __int16 xOff = 0;
    __int16 yOff = 0;
    field_20_animation.Get_Frame_Offset_40C480(&xOff, &yOff);

    field_11C_x1 = FP_GetExponent(FP_FromInteger(xOff) + FP_FromInteger(field_11C_x1));
    field_11E_y1 = FP_GetExponent(FP_FromInteger(yOff) + FP_FromInteger(field_11E_y1));
    field_120_x2 = FP_GetExponent(FP_FromInteger(xOff) + FP_FromInteger(field_120_x2));
    field_122_y2 = FP_GetExponent(FP_FromInteger(yOff) + FP_FromInteger(field_122_y2));
}

void ZapLine::dtor_4CCCB0()
{
    SetVTable(this, 0x5478A0);
    ResourceManager::FreeResource_49C330(field_F8_ppRes);
    Mem_Free_495560(field_138_buf1);
    Mem_Free_495560(field_13C_buf2);
    Mem_Free_495560(field_140_buf3);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void ZapLine::vScreenChanged_4CDBE0()
{
    if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ZapLine::State_0_2_4_4CCD50()
{
    int v1 = 0;
    if (field_126_alive_timer >= 8)
    {
        const int remainingAliveTime = field_128_max_alive_time - field_126_alive_timer;
        if (remainingAliveTime >= 8)
        {
            v1 = 4;
        }
        else
        {
            v1 = (remainingAliveTime / 4) + 3;
        }
    }
    else
    {
        v1 = field_126_alive_timer / 4 + 3;
    }

    int v5 = 1 << v1;
    int v6 = 1 << (v1 - 1);

    field_140_buf3[0].field_0_x = FP_FromInteger(field_11C_x1);
    field_140_buf3[0].field_4_y = FP_FromInteger(field_11E_y1);

    field_140_buf3[field_12E_part_count - 1].field_0_x = FP_FromInteger(field_120_x2);
    field_140_buf3[field_12E_part_count - 1].field_4_y = FP_FromInteger(field_122_y2);

    int angExtra = 0;
    if ((sGnFrame_5C1B84 / 8) & 1)
    {
        angExtra = 0;
    }
    else
    {
        angExtra = 128;
    }

    const FP xDiff = FP_FromInteger(field_120_x2 - field_11C_x1) / FP_FromInteger(field_12E_part_count);
    const FP xDiffDiv = -xDiff * FP_FromDouble(1.5);

    const FP yDiff = FP_FromInteger(field_122_y2 - field_11E_y1) / FP_FromInteger(field_12E_part_count);
    const FP yDiffDiv = yDiff * FP_FromDouble(1.5);


    // First and last done above
    for (int i = 1; i < field_12E_part_count-1; i++)
    {
        const BYTE ang = static_cast<BYTE>(angExtra + 18 * i);
        field_140_buf3[i].field_0_x = 
            FP_FromInteger(Math_NextRandom() % v5) + (Math_Cosine_496CD0(ang) * xDiffDiv) + FP_FromInteger(field_11C_x1) + (FP_FromInteger(i) * xDiff) - FP_FromInteger(v6);

        field_140_buf3[i].field_4_y = 
            FP_FromInteger(Math_NextRandom() % v5) + (Math_Cosine_496CD0(ang) * yDiffDiv) + FP_FromInteger(field_11E_y1) + (FP_FromInteger(i) * yDiff) - FP_FromInteger(v6);
    }

    field_144_rects[0].x = 0;
    field_144_rects[0].y = 0;
    field_144_rects[0].w = gPsxDisplay_5C1130.field_0_width;
    field_144_rects[0].h = gPsxDisplay_5C1130.field_2_height;

    field_144_rects[1].x = 0;
    field_144_rects[1].y = 0;
    field_144_rects[1].w = gPsxDisplay_5C1130.field_0_width;
    field_144_rects[1].h = gPsxDisplay_5C1130.field_2_height;
}

void ZapLine::State_0_2_4_4CD110()
{
    field_140_buf3[0].field_0_x = FP_FromInteger(field_11C_x1);
    field_140_buf3[0].field_4_y = FP_FromInteger(field_11E_y1);
    field_140_buf3[field_12E_part_count - 1].field_0_x = FP_FromInteger(field_120_x2);
    field_140_buf3[field_12E_part_count - 1].field_4_y = FP_FromInteger(field_122_y2);
    
    const FP x2Diff = FP_FromInteger(field_120_x2 - field_11C_x1) / FP_FromInteger(field_12E_part_count);
    const FP y2Diff = FP_FromInteger(field_122_y2 - field_11E_y1) / FP_FromInteger(field_12E_part_count);

    const FP y2DiffDiv = -y2Diff * FP_FromDouble(0.1);
    const FP x2DiffDiv = x2Diff * FP_FromDouble(0.1);

    for (int i = 1; i < field_12E_part_count - 1; i++)
    {
        const FP rnd = FP_FromInteger(Math_NextRandom() % 32 - 16);
        field_140_buf3[i].field_0_x = (y2DiffDiv * rnd) + FP_FromInteger(field_11C_x1) + (FP_FromInteger(i) * x2Diff);
        field_140_buf3[i].field_4_y = (x2DiffDiv * rnd) + FP_FromInteger(field_11E_y1) + (FP_FromInteger(i) * y2Diff);
    }
}

void ZapLine::State_0_4CD340()
{
    FP acc = FP_FromInteger(0);
    const FP delta = FP_FromInteger(1) / FP_FromInteger(field_130_count_per_part);
    for (int i = 0; i < field_130_count_per_part; i++)
    {
        const FP accSqrd = (acc * acc);
        field_13C_buf2[i].field_0 = accSqrd - FP_FromRaw(2 * acc.fpValue) + FP_FromInteger(1);
        field_13C_buf2[i].field_4 = -FP_FromRaw(2 * accSqrd.fpValue) + FP_FromRaw(2 * acc.fpValue) + FP_FromInteger(1);
        field_13C_buf2[i].field_8 = accSqrd;
        acc += delta;
    }
}

void ZapLine::sub_4CD400(int idx1, int idx2, int idx3, __int16 idx4)
{
    const FP y1 = field_140_buf3[idx1].field_4_y;
    const FP x1 = field_140_buf3[idx1].field_0_x;

    const FP y2 = field_140_buf3[idx2].field_4_y;
    const FP x2 = field_140_buf3[idx2].field_0_x;

    const FP x3 = field_140_buf3[idx3].field_0_x;
    const FP y3 = field_140_buf3[idx3].field_4_y;

    for (int i = 0; i < field_130_count_per_part; i++)
    {
        auto pItem = &field_138_buf1[i + (idx4 * field_130_count_per_part)];

        pItem->field_0_x = FP_GetExponent(
         FP_FromRaw((
            (field_13C_buf2[i].field_8 * x3) + 
            (field_13C_buf2[i].field_4 * x2) + 
            (field_13C_buf2[i].field_0 * x1)).fpValue >> 1));

        pItem->field_2_y = FP_GetExponent(
            FP_FromRaw((
            (field_13C_buf2[i].field_8 * y3) +
            (field_13C_buf2[i].field_4 * y2) + 
            (field_13C_buf2[i].field_0 * y1)).fpValue >> 1));
    }
}

void ZapLine::State_2_4_4CD650()
{
    for (int i = 0; i < field_12E_part_count; i++)
    {
        for (int j = 0; j < field_130_count_per_part; j++)
        {
            const auto pPoint = &field_138_buf1[j + (i * field_130_count_per_part)];
            Prim_Sprt* pSprt = &field_134_pSprts->field_0_sprts[j + (i * field_130_count_per_part)];
            SetXY0(&pSprt[0], pPoint->field_0_x, pPoint->field_2_y);
            SetXY0(&pSprt[1], pPoint->field_0_x, pPoint->field_2_y);
        }
    }
}

void ZapLine::State_1_3_4CD5D0()
{
    for (short i = 0; i < field_12E_part_count; i++)
    {
        if (i == 0)
        {
            // First item
            sub_4CD400(0, 0, 1, 0);
        }
        else
        {
            const short lastIdx = field_12E_part_count - 1;
            if (i == lastIdx)
            {
                // Last item
                sub_4CD400(field_12E_part_count - 2, lastIdx, lastIdx, field_12E_part_count - 1);
            }
            else
            {
                // Other items
                sub_4CD400(i - 1, i, i + 1, i);
            }
        }
    }
}

void ZapLine::vUpdate_4CD790()
{
    field_126_alive_timer++;

    switch (field_F4_state)
    {
    case State::State_0:
        State_0_4CD340();
        if (field_12A_type == 1)
        {
            State_0_2_4_4CD110();
        }
        else if (field_12A_type == 0)
        {
            State_0_2_4_4CCD50();
        }
        field_F4_state = State::State_1;
        break;

    case State::State_1:
        State_1_3_4CD5D0();
        field_F4_state = State::State_2;
        break;

    case State::State_2:
    case State::State_4:
        State_2_4_4CD650();
        if (field_126_alive_timer >= field_128_max_alive_time && field_12A_type != 1)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        if (field_12A_type == 1)
        {
            State_0_2_4_4CD110();
        }
        else if (field_12A_type == 0)
        {
            State_0_2_4_4CCD50();
        }
        field_F4_state = State::State_3;
        break;

    case State::State_3:
        State_1_3_4CD5D0();
        field_F4_state = State::State_4;
        break;
    }
}

void ZapLine::vRender_4CD8C0(int** pOt)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0)
        && field_F4_state > State::State_2)
    {
        const auto bufferIdx = gPsxDisplay_5C1130.field_C_buffer_index;

        for (int i = 0; i < field_12E_part_count; i++)
        {
            for (int j = 0; j < field_130_count_per_part; j++)
            {
                Prim_Sprt* pSprt = &field_134_pSprts->field_0_sprts[j + (i * field_130_count_per_part)];
                OrderingTable_Add_4F8AA0(
                    &pOt[field_20_animation.field_C_render_layer],
                    &pSprt[bufferIdx].mBase.header);
            }
        }

        const int calcTPage = PSX_getTPage_4F60E0(
            static_cast<char>(field_124_tPageMode),
            static_cast<char>(field_12C_tPageAbr),
            field_20_animation.field_84_vram_rect.x,
            field_20_animation.field_84_vram_rect.y);

        Prim_SetTPage* pTPage = &field_FC_tPage_p8[bufferIdx];
        Init_SetTPage_4F5B60(pTPage, 0, 0, calcTPage);
        OrderingTable_Add_4F8AA0(&pOt[field_20_animation.field_C_render_layer], &pTPage->mBase);

        PSX_RECT* pRect = &field_144_rects[bufferIdx];
        pRect->x = 32767;
        pRect->w = -32767;
        pRect->y = 32767;
        pRect->h = -32767;

        for (int i = 0; i < field_12E_part_count; i++)
        {
            const PSX_Point* pPoint = &field_138_buf1[i*field_130_count_per_part];
            for (int j = 0; j < field_130_count_per_part; j++)
            {
                if (pPoint->field_0_x < pRect->x)
                {
                    pRect->x = pPoint->field_0_x;
                }

                if (pPoint->field_0_x > pRect->w)
                {
                    pRect->w = pPoint->field_0_x;
                }

                if (pPoint->field_2_y < pRect->y)
                {
                    pRect->y = pPoint->field_2_y;
                }

                if (pPoint->field_2_y > pRect->h)
                {
                    pRect->h = pPoint->field_2_y;
                }
            }
        }

        pRect->x -= 25;
        pRect->w += 25;
        pRect->y -= 25;
        pRect->h += 25;
        const PSX_RECT* pRectToUse = &field_144_rects[gPsxDisplay_5C1130.field_C_buffer_index];
        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            pRectToUse->x,
            pRectToUse->y,
            pRectToUse->w,
            pRectToUse->h,
            pScreenManager_5BB5F4->field_3A_idx);
    }

}
