#include "stdafx.h"
#include "FootSwitch.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

const TintEntry sFootSwitchTints_5639F4[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};



const FootSwitch_Data sFootSwitchData_547D60[] =
{
    { 568, 588, 45, 11 },
    { 568, 588, 45, 11 },
    { 568, 588, 45, 11 },
    { 972, 992, 72, 18 },
    { 972, 992, 72, 18 },
    { 568, 588, 45, 11 },
    { 568, 588, 45, 11 },
    { 972, 992, 72, 18 },
    { 548, 568, 46, 11 },
    { 568, 588, 45, 11 },
    { 568, 588, 45, 11 },
    { 972, 992, 72, 18 },
    { 568, 588, 45, 11 },
    { 568, 588, 45, 11 },
    { 548, 568, 46, 11 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};



FootSwitch* FootSwitch::ctor_4DE090(Path_FootSwitch* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x547E38);

    field_4_typeId = Types::eFootSwitch_56;
    field_100_obj_id = -1;

    const int idx = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kTriggerResID);
    Animation_Init_424E10(
        sFootSwitchData_547D60[idx].field_0_frameTableOffset,
        sFootSwitchData_547D60[idx].field_8_maxH,
        sFootSwitchData_547D60[idx].field_A_frameTableOffset,
        ppRes,
        1,
        1u);

    field_20_animation.field_C_render_layer = 25;

    SetTint_425600(sFootSwitchTints_5639F4, gMap_5C3030.sCurrentLevelId_5C3030);

    field_FA_id = pTlv->field_10_id;

    if (pTlv->field_12_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 6;
    }

    field_FC_action = pTlv->field_14_action;
    field_FE_trigger_by = pTlv->field_16_trigger_by;
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_F8_state = 0;
    field_DC_bApplyShadows |= 2u;
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_104_bUnknown = 0;
    field_F4_tlvInfo = tlvInfo;
    field_106_bFindStander = 1;
    return this;
}

BaseGameObject* FootSwitch::VDestructor(signed int flags)
{
    return vdtor_4DE240(flags);
}

void FootSwitch::VUpdate()
{
    vUpdate_4DE270();
}

void FootSwitch::VScreenChanged()
{
    vScreenChanged_4DE650();
}

FootSwitch* FootSwitch::vdtor_4DE240(signed int flags)
{
    dtor_4DE670();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void FootSwitch::dtor_4DE670()
{
    SetVTable(this, 0x547E38);
    field_100_obj_id = -1;
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void FootSwitch::vScreenChanged_4DE650()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

void FootSwitch::vUpdate_4DE270()
{
    NOT_IMPLEMENTED();
}
