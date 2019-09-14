#include "stdafx.h"
#include "LaughingGas.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"

ALIVE_VAR(1, 0x5BC214, int, gGasInstanceCount_5BC214, 0);
ALIVE_VAR(1, 0x5C1BA4, short, gLaughingGasOn_5C1BA4, FALSE);


LaughingGas* LaughingGas::ctor_432400(__int16 layer, int /*notUsed*/, Path_LaughingGas* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545000);

    gGasInstanceCount_5BC214++;

    field_4_typeId = Types::eLaughingGas_31;
    Path_LaughingGas_Data* pData = &field_48_tlv_data;
    pData->field_0_is_laughin_gas = pTlv->field_10_is_laughing_gas;
    pData->field_2_gas_id = pTlv->field_12_gas_id;

    pData->field_4_red_percent = pTlv->field_14_red_percent;
    pData->field_6_blue_percent = pTlv->field_18_blue_percent;
    pData->field_8_green_percent = pTlv->field_18_blue_percent;

    if (field_48_tlv_data.field_0_is_laughin_gas)
    {
        field_36 = 1;
    }
    else
    {
        field_36 = 0;
    }

    if (SwitchStates_Get_466020(field_48_tlv_data.field_2_gas_id))
    {
        field_34 = 1;
        field_54 = FP_FromInteger(1);
    }
    else
    {
        field_34 = 0;
        field_54 = FP_FromInteger(0);
    }

    field_24_tlvInfo = tlvInfo;
    field_58_layer = layer;
    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable);

    field_28 = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
    field_2C = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

    field_2A = FP_GetExponent(((FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x) *  FP_FromInteger(40)) / FP_FromInteger(23));
    field_2E = FP_GetExponent(((FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x) * FP_FromInteger(40)) / FP_FromInteger(23));
    
    field_31F8_count1 = (field_2E - field_2A) / 4;
    field_31FC_count2 = (field_2C - field_28 + 2) / 2;

    field_19C_pMem = malloc_non_zero_4954F0(2 * field_31FC_count2 * field_31F8_count1);
    
    Init_432980(); // also inits field_5C_prims
    VUpdate();

    return this;
}

void LaughingGas::dtor_432B80()
{
    SetVTable(this, 0x545000);
    Path::TLV_Reset_4DB8E0(field_24_tlvInfo, -1, 0, 0);
    gObjList_drawables_5C1124->Remove_Item(this);
    gLaughingGasOn_5C1BA4 = FALSE;
    gGasInstanceCount_5BC214--;
    Mem_Free_495540(field_19C_pMem);
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* LaughingGas::VDestructor(signed int flags)
{
    return vdtor_432670(flags);
}

void LaughingGas::VScreenChanged()
{
    vScreenChanged_432DE0();
}

void LaughingGas::VUpdate()
{
    vUpdate_432C40();
}

void LaughingGas::VRender(int** pOt)
{
    vRender_432D10(pOt);
}

void LaughingGas::vScreenChanged_432DE0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

void LaughingGas::Init_432980()
{
    NOT_IMPLEMENTED();
}

LaughingGas* LaughingGas::vdtor_432670(signed int flags)
{
    dtor_432B80();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void LaughingGas::vRender_432D10(int** pOt)
{
    NOT_IMPLEMENTED();

    if (field_54 > FP_FromDouble(0.1) || !field_36)
    {
        PrimHeader* pHack = (PrimHeader*)&field_5C_prims;
        pHack = pHack;

        if (field_19C_pMem)
        {
            if (sbDisplayRenderFrame_55EF8C)
            {
                DoRender_432740();
            }
            OrderingTable_Add_4F8AA0(&pOt[field_58_layer], (PrimHeader*)&field_5C_prims);
        }
        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, gPsxDisplay_5C1130.field_0_width, gPsxDisplay_5C1130.field_2_height);
    }
}

void LaughingGas::DoRender_432740()
{
    NOT_IMPLEMENTED();
}

bool LaughingGas::CounterOver_432DA0()
{
    return field_54 >= FP_FromDouble(0.3);
}

void LaughingGas::vUpdate_432C40()
{
    NOT_IMPLEMENTED();

    PrimHeader* pHack = (PrimHeader*)&field_5C_prims;
    pHack = pHack;
}
