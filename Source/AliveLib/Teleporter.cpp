#include "stdafx.h"
#include "Teleporter.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Map.hpp"

Teleporter* Teleporter::ctor_4DC1E0(Path_Teleporter* pTlv, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x547AE0);

    field_4C_pTlv = pTlv; // TODO: Don't think this is used, and it can become a dangling ptr?
    field_34_mTlvData = pTlv->field_10_data;
    field_20_tlvInfo = tlvInfo;

    field_24_global_y1 = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));
    field_28_global_y2 = FP_GetExponent((FP_FromInteger(pTlv->field_C_bottom_right.field_2_y) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y));
    field_26_global_x1 = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));
    field_2A_global_x2 = FP_GetExponent((FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x));

    field_2C_switch_state = SwitchStates_Get_466020(field_34_mTlvData.field_1A_trigger_id);

    field_54 = 0;

    field_32_bDestroySelf = 0;
    field_30_state = 0;
    field_50_objId = -1;

    return this;
}

BaseGameObject* Teleporter::VDestructor(signed int flags)
{
    return vdtor_4DC350(flags);
}

void Teleporter::VUpdate()
{
    vUpdate_4DC400();
}

Teleporter* Teleporter::vdtor_4DC350(signed int flags)
{
    dtor_4DC380();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Teleporter::dtor_4DC380()
{
    SetVTable(this, 0x547AE0);
    Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    BaseGameObject_dtor_4DBEC0();
}

void Teleporter::vScreenChanged_4DCE80()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    field_32_bDestroySelf = 1;
}

void Teleporter::vUpdate_4DC400()
{
    NOT_IMPLEMENTED();
}
