#include "stdafx.h"
#include "LaughingGas.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "PsxRender.hpp"
#include "Events.hpp"
#include <math.h>

ALIVE_VAR(1, 0x5BC214, s32, gGasInstanceCount_5BC214, 0);
ALIVE_VAR(1, 0x5C1BA4, s16, gLaughingGasOn_5C1BA4, FALSE);

// On linux not using this random algorithm produces much bigger numbers
// which causes flickering in the gas rendering. Apparently this is the MSVC algorithm.
static s32 random_seed = 0;
static s32 gas_rand()
{
    random_seed = (random_seed * 214013 + 2531011) & 0xFFFFFFFF;
    return (random_seed >> 16) & 0x7FFF;
}

LaughingGas::LaughingGas(Layer layer, s32 /*notUsed*/, Path_LaughingGas* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    gGasInstanceCount_5BC214++;

    SetType(AETypes::eLaughingGas_31);
    Path_LaughingGas_Data* pData = &field_48_tlv_data;
    pData->field_0_bLaughing_gas = pTlv->field_10_bLaughing_gas;
    pData->field_2_laughing_gas_switch_id = pTlv->field_12_laughing_gas_switch_id;

    pData->field_4_red_percent = pTlv->field_14_red_percent;
    pData->field_6_blue_percent = pTlv->field_18_blue_percent;
    pData->field_8_green_percent = pTlv->field_18_blue_percent;


    if (field_48_tlv_data.field_0_bLaughing_gas == Choice_short::eYes_1)
    {
        field_36_bLaughing_gas = Choice_short::eYes_1;
    }
    else
    {
        field_36_bLaughing_gas = Choice_short::eNo_0;
    }

    if (SwitchStates_Get(field_48_tlv_data.field_2_laughing_gas_switch_id))
    {
        field_34_bEnabled = 1;
        field_54_amount_on = FP_FromInteger(1);
    }
    else
    {
        field_34_bEnabled = 0;
        field_54_amount_on = FP_FromInteger(0);
    }

    field_24_tlvInfo = tlvInfo;
    field_58_layer = layer;

    gObjList_drawables_5C1124->Push_Back(this);
    mFlags.Set(BaseGameObject::eDrawable_Bit4);

    field_28_y = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
    field_2A_x = FP_GetExponent(PsxToPCX(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));

    field_2C_h = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
    field_2E_w = FP_GetExponent(PsxToPCX(FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));

    field_31F8_w_count = (field_2E_w - field_2A_x) / 4;
    field_31FC_h_count = (field_2C_h - field_28_y + 2) / 2;

    field_19C_pMem = static_cast<u16*>(ae_malloc_non_zero_4954F0(sizeof(s16) * field_31FC_h_count * field_31F8_w_count));

    Init();
    VUpdate();
}

LaughingGas::~LaughingGas()
{
    Path::TLV_Reset(field_24_tlvInfo, -1, 0, 0);
    gObjList_drawables_5C1124->Remove_Item(this);
    gLaughingGasOn_5C1BA4 = FALSE;
    gGasInstanceCount_5BC214--;
    ae_delete_free_495540(field_19C_pMem);
}

void LaughingGas::VScreenChanged()
{
    mFlags.Set(BaseGameObject::eDead);
}

const f32 dword_551C58[7] = {1.0, 5.0, 10.0, 10.0, 5.0, 1.0, 0.0};

void LaughingGas::Init()
{
    for (s32 i = 0; i < field_31F8_w_count; i++)
    {
        const f32 v4 = (f32) i / (f32) field_31F8_w_count;
        f32 val1 = 1.0f;
        for (s32 j = 0; j < 7; j++)
        {
            field_1A0_x_data[i].array_4[j] = val1 * dword_551C58[j];
            val1 *= v4;
        }

        f32* pElem_ptr = &field_1A0_x_data[i].array_4[5];
        const f32 v1 = 1.0f - v4;
        f32 val2 = 1.0f;
        for (s32 j = 0; j < 6; j++)
        {
            const f32 calc2 = val2 * *pElem_ptr;
            *pElem_ptr = calc2;
            --pElem_ptr;
            val2 *= v1;
        }
    }

    for (s32 i = 0; i < field_31FC_h_count; i++)
    {
        const f32 v4 = (f32) i / (f32) field_31FC_h_count;
        f32 val1 = 1.0f;
        for (s32 j = 0; j < 7; j++)
        {
            field_24D0_y_data[i].array_4[j] = val1 * dword_551C58[j];
            val1 *= v4;
        }

        f32* pElem_ptr = &field_24D0_y_data[i].array_4[5];
        const f32 v1 = 1.0f - v4;
        f32 val2 = 1.0f;
        for (s32 j = 0; j < 6; j++)
        {
            const f32 calc2 = val2 * *pElem_ptr;
            *pElem_ptr = calc2;
            --pElem_ptr;
            val2 *= v1;
        }
    }

    for (s32 i = 0; i < 6; i++)
    {
        for (s32 j = 0; j < 6; j++)
        {
            field_10C_gas_x[i][j] = static_cast<f32>(gas_rand()) * 6.28f * (1.0f / 32767.0f);
        }
    }

    field_5C_prim.mPrimHeader.rgb_code.code_or_pad = PrimTypeCodes::eLaughingGas;
    field_5C_prim.x = field_2A_x;
    field_5C_prim.y = field_28_y;
    field_5C_prim.w = field_2E_w;
    field_5C_prim.h = field_2C_h;
    field_5C_prim.pData = field_19C_pMem;
}

void LaughingGas::VRender(PrimHeader** ppOt)
{
    if (field_54_amount_on > FP_FromDouble(0.1) || field_36_bLaughing_gas == Choice_short::eNo_0)
    {
        if (field_19C_pMem)
        {
            if (sbDisplayRenderFrame_55EF8C)
            {
                DoRender();
            }
            OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_58_layer), &field_5C_prim.mPrimHeader);
        }
        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);
    }
}

void LaughingGas::DoRender()
{
    f32 local_array[6];

    u16* memPtr = field_19C_pMem;
    s32 rgb_base = (1 << sRedShift_C215C4) + (1 << sGreenShift_C1D180);

    if (field_36_bLaughing_gas == Choice_short::eNo_0)
    {
        rgb_base = (1 << sBlueShift_C19140) + (1 << sRedShift_C215C4) + (1 << sGreenShift_C1D180);
    }

    for (s32 yCount = 0; yCount < field_31FC_h_count; ++yCount)
    {
        for (s32 p = 0; p < 6; p++)
        {
            local_array[p] = Calc_Y(&field_7C_gas_y[p][0], yCount);
        }

        for (s32 xCount = 0; xCount < field_31F8_w_count; ++memPtr)
        {
            f32 yValue = Calc_X(local_array, xCount);
            if (yValue > 0.0f)
            {
                if (yValue >= 3.0f)
                {
                    if (yValue > 31.0f)
                    {
                        yValue = 31.0f;
                    }
                }
                else
                {
                    yValue = 3.0f;
                }
            }
            else
            {
                yValue = 0.0f;
            }

            *memPtr = static_cast<u16>(rgb_base * (static_cast<u8>(yValue) & 30));
            ++xCount;
        }
    }
}

s16 LaughingGas::CounterOver()
{
    return field_54_amount_on >= FP_FromDouble(0.3);
}

void LaughingGas::VUpdate()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (SwitchStates_Get(field_48_tlv_data.field_2_laughing_gas_switch_id))
    {
        field_34_bEnabled = 1;
    }
    else
    {
        field_34_bEnabled = 0;
    }

    if (field_36_bLaughing_gas == Choice_short::eYes_1)
    {
        if (field_34_bEnabled)
        {
            if (field_54_amount_on < FP_FromInteger(1))
            {
                field_54_amount_on += FP_FromDouble(0.01);
            }
        }
        else
        {
            if (field_54_amount_on > FP_FromInteger(0))
            {
                field_54_amount_on -= FP_FromDouble(0.01);
            }
        }
    }

    if (field_36_bLaughing_gas == Choice_short::eYes_1)
    {
        gLaughingGasOn_5C1BA4 = 1;
        if (CounterOver())
        {
            sub_4328A0();
            return;
        }
    }

    gLaughingGasOn_5C1BA4 = 0;
    sub_4328A0();
}

f32 LaughingGas::Calc_X(f32* a2, s32 xIndex)
{
    f32 result = 0.0;
    f32* v4 = a2 + 1;

    for (s32 i = 0; i < 4; i++)
    {
        const f32 v5 = field_1A0_x_data[xIndex].array_4[i + 1];
        result += v5 * *v4;
        v4++;
    }

    return result;
}

f32 LaughingGas::Calc_Y(f32* a2, s32 yIndex)
{
    f32 result = 0.0;
    f32* v4 = a2 + 1;

    for (s32 i = 0; i < 4; i++)
    {
        const f32 v5 = field_24D0_y_data[yIndex].array_4[i + 1];
        result += v5 * *v4;
        v4++;
    }

    return result;
}

void LaughingGas::sub_4328A0()
{
    for (s32 i = 0; i < 4; i++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            const s32 rnd = gas_rand() - 8191;
            field_7C_gas_y[1 + i][1 + j] = (sin(field_10C_gas_x[1 + i][1 + j]) * 50.0f + 30.0f) * static_cast<f32>(FP_GetDouble(field_54_amount_on));
            field_10C_gas_x[1 + i][1 + j] += (f32)(rnd) *0.03f * (1.0f / 16383.0f);
        }
    }
}
