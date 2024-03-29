#include "stdafx.h"
#include "StatusLight.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Collisions.hpp"
#include "SwitchStates.hpp"
#include "DDCheat.hpp"
#include "stdlib.hpp"
#include "Grid.hpp"

BaseGameObject* StatusLight::VDestructor(s32 flags)
{
    return vdtor_4D5500(flags);
}

void StatusLight::VUpdate()
{
    vUpdate_4D5530();
}

void StatusLight::VScreenChanged()
{
    vScreenChanged_4D5820();
}

StatusLight* StatusLight::ctor_4D5250(Path_StatusLight* pTlv, u32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x5479A4); // vTbl_StatusLight_5479A4
    field_F4_tlvInfo = tlvInfo;
    field_F8_switch_id = pTlv->field_10_switch_id;

    if (pTlv->field_12_scale != Scale_short::eFull_0)
    {
        if (pTlv->field_12_scale == Scale_short::eHalf_1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = Layer::eLayer_8;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_27;
        field_D6_scale = 1;
    }

    field_FA_id1 = pTlv->field_14_id1;
    field_FC_id2 = pTlv->field_16_id2;
    field_FE_id3 = pTlv->field_18_id3;
    field_100_id4 = pTlv->field_1A_id4;
    field_102_id5 = pTlv->field_1C_id5;
    field_104_bIgnore_grid_snapping = pTlv->field_1E_bIgnore_grid_snapping;

    const AnimRecord& rec = AnimRec(AnimId::Status_Light_Red);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_DC_bApplyShadows &= ~1u;
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);

    if (field_104_bIgnore_grid_snapping == Choice_short::eNo_0)
    {
        field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
    }

    field_BC_ypos = FP_FromInteger((pTlv->field_8_top_left.field_2_y + pTlv->field_C_bottom_right.field_2_y) / 2);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(56)),
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale == 1 ? 241 : 16);

    if (field_104_bIgnore_grid_snapping == Choice_short::eNo_0)
    {
        if (bCollision)
        {
            field_BC_ypos = hitY - (field_CC_sprite_scale * FP_FromInteger(56));
        }
    }

    field_108_xpos = field_B8_xpos;
    field_10C_ypos = field_BC_ypos;
    return this;
}

void StatusLight::vUpdate_4D5530()
{
    // TODO: Document how this works

    field_B8_xpos = FP_FromInteger(sTweakX_5C1BD0) + field_108_xpos;
    field_BC_ypos = FP_FromInteger(sTweakY_5C1BD4) + field_10C_ypos;

    if (SwitchStates_Get_466020(field_F8_switch_id))
    {
        if ((!SwitchStates_Get_466020(field_FA_id1) && field_FA_id1) || (!SwitchStates_Get_466020(field_FC_id2) && field_FC_id2) || (!SwitchStates_Get_466020(field_FE_id3) && field_FE_id3) || (!SwitchStates_Get_466020(field_100_id4) && field_100_id4) || (!SwitchStates_Get_466020(field_102_id5) && field_102_id5)
            || (sGnFrame_5C1B84 % 8) >= 4)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }
        const AnimRecord& animRec = AnimRec(AnimId::Status_Light_Green);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    }
    else if (SwitchStates_Get_466020(field_FA_id1) || SwitchStates_Get_466020(field_FC_id2) || SwitchStates_Get_466020(field_FE_id3) || SwitchStates_Get_466020(field_100_id4) || SwitchStates_Get_466020(field_102_id5))
    {
        if ((sGnFrame_5C1B84 % 8) >= 4)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }
        const AnimRecord& animRec = AnimRec(AnimId::Status_Light_Red);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        const AnimRecord& animRec = AnimRec(AnimId::Status_Light_Red);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    }
}

StatusLight* StatusLight::vdtor_4D5500(s32 flags)
{
    dtor_4D5790();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void StatusLight::dtor_4D5790()
{
    SetVTable(this, 0x5479A4); // vTbl_StatusLight_5479A4
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void StatusLight::vScreenChanged_4D5820()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}
