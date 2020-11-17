#include "stdafx_ao.h"
#include "Function.hpp"
#include "Well.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Leaf.hpp"
#include "Events.hpp"
#include "PsxDisplay.hpp"
#include "DDCheat.hpp"

namespace AO {

ALIVE_VAR(1, 0x4D1488, BYTE, sWellRndSeed_4D1488, 4);

static __int16 Well_NextRandom()
{
    return sRandomBytes_4BBE30[sWellRndSeed_4D1488++];
}


BaseGameObject* Well::VDestructor(signed int flags)
{
    return vdtor_48B4D0(flags);
}

BaseGameObject* Well::dtor_48B460()
{
    SetVTable(this, 0x4BD730);
    if (field_E4_tlvInfo != -1)
    {
        gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    }
    return dtor_417D10();
}

void Well::VScreenChanged_48B450()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Well::VScreenChanged()
{
    VScreenChanged_48B450();
}

void Well::VRender_48B3D0(int** ppOt)
{
    field_A8_xpos += FP_FromInteger(gTweak_X_5076D8);
    field_AC_ypos += FP_FromInteger(gTweak_Y_5076DC);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    field_A8_xpos -= FP_FromInteger(gTweak_X_5076D8);
    field_AC_ypos -= FP_FromInteger(gTweak_Y_5076DC);
}

void Well::VRender(int** ppOt)
{
    VRender_48B3D0(ppOt);
}

void Well::VUpdate_48B270()
{
    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    }

    if (field_100_emit_leaves)
    {
        // Always on or has been enabled?
        if (!field_E8_trigger_id || SwitchStates_Get(field_E8_trigger_id))
        {
            // Random chance of leaves emitting
            if (Well_NextRandom() < 10)
            {
                auto pLeaf = ao_new<Leaf>();
                if (pLeaf)
                {
                    pLeaf->ctor_48B4F0(
                        field_F8_leaf_xpos,
                        field_FC_leaf_ypos,
                        field_F0_exit_x,
                        field_F4_exit_y,
                        field_EC_scale);
                }

                if (field_F4_exit_y > FP_FromInteger(0))
                {
                    if (pLeaf)
                    {
                        if (field_EC_scale == FP_FromDouble(0.5))
                        {
                            pLeaf->field_10_anim.field_C_layer = 3;
                        }
                        else
                        {
                            pLeaf->field_10_anim.field_C_layer = 22;
                        }
                    }
                }
            }
        }
    }
}

void Well::VUpdate()
{
    VUpdate_48B270();
}

void Well::WellExpress_Init_48B110(Path_Well_Express* pTlv, FP /*xpos*/, FP ypos)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, pTlv->field_1E_res_id, 1, 0);
    if (ppRes)
    {
        auto pHeader = reinterpret_cast<AnimHeader*>(*ppRes);
        Animation_Init_417FD0(
            pHeader->field_4_frame_table_offset,
            pHeader->field_0_max_w,
            pHeader->field_2_max_h,
            ppRes,
            1);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_CC_bApplyShadows &= ~1u;
    }
    else
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_6_flags.Clear(Options::eDrawable_Bit4);
    }

    if (pTlv->field_18_scale)
    {
        field_10_anim.field_C_layer = 4;
        field_EC_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_10_anim.field_C_layer = 23;
        field_EC_scale = FP_FromInteger(1);
    }

    field_E8_trigger_id = pTlv->field_1A_trigger_id;
    field_F0_exit_x = FP_FromInteger(pTlv->field_20_exit_x) / FP_FromInteger(100);
    field_F4_exit_y = FP_FromInteger(pTlv->field_22_exit_y) / FP_FromInteger(100);

    field_100_emit_leaves = pTlv->field_34_emit_leaves;

    if (field_100_emit_leaves)
    {
        field_F8_leaf_xpos = FP_FromInteger(pTlv->field_36_leaf_x);
        if (!FP_GetExponent(field_F8_leaf_xpos))
        {
            field_F8_leaf_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x
                + (PsxToPCX(pTlv->field_14_bottom_right.field_0_x -
                    pTlv->field_10_top_left.field_0_x, +11)
                    / 2));
        }

        field_FC_leaf_ypos = FP_FromInteger(pTlv->field_38_leaf_y);
        if (!FP_GetExponent(field_FC_leaf_ypos))
        {
            field_FC_leaf_ypos = ypos;
        }
    }
}

void Well::WellLocal_Init_48AFA0(Path_Well_Local* pTlv, FP /*xpos*/, FP ypos)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, pTlv->field_1E_res_id, 1, 0);
    if (ppRes)
    {
        auto pHeader = reinterpret_cast<AnimHeader*>(*ppRes);
        Animation_Init_417FD0(
            pHeader->field_4_frame_table_offset,
            pHeader->field_0_max_w,
            pHeader->field_2_max_h,
            ppRes,
            1);
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_CC_bApplyShadows &= ~1u;
    }
    else
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_6_flags.Clear(Options::eDrawable_Bit4);
    }

    if (pTlv->field_18_scale)
    {
        field_10_anim.field_C_layer = 4;
        field_EC_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_10_anim.field_C_layer = 23;
        field_EC_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    field_E8_trigger_id = pTlv->field_1A_trigger_id;
    field_F0_exit_x = FP_FromInteger(pTlv->field_20_exit_x) / FP_FromInteger(100);
    field_F4_exit_y = FP_FromInteger(pTlv->field_22_exit_y) / FP_FromInteger(100);

    field_100_emit_leaves = pTlv->field_2C_emit_leaves;

    if (field_100_emit_leaves)
    {
        field_F8_leaf_xpos = FP_FromInteger(pTlv->field_2E_leaf_x);
        if (!FP_GetExponent(field_F8_leaf_xpos))
        {
            field_F8_leaf_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x +
                (PsxToPCX(
                    pTlv->field_14_bottom_right.field_0_x -
                    pTlv->field_10_top_left.field_0_x, 11)
                    / 2));
        }

        field_FC_leaf_ypos = FP_FromInteger(pTlv->field_30_leaf_y);
        if (!FP_GetExponent(field_FC_leaf_ypos))
        {
            field_FC_leaf_ypos = ypos;
        }
    }
}

Well* Well::ctor_48AEE0(Path_Well_Base* pTlv, FP xpos, FP ypos, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BD730);

    field_E4_tlvInfo = tlvInfo;
    field_4_typeId = Types::eWell_101;

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    field_AC_ypos = ypos;
    field_A8_xpos = xpos;

    if (pTlv->field_4_type == TlvTypes::LocalWell_11)
    {
        WellLocal_Init_48AFA0(static_cast<Path_Well_Local*>(pTlv), xpos, ypos);
    }
    else
    {
        WellExpress_Init_48B110(static_cast<Path_Well_Express*>(pTlv), xpos, ypos);
    }
    return this;
}

Well* Well::vdtor_48B4D0(signed int flags)
{
    dtor_48B460();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}

