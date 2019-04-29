#include "stdafx.h"
#include "SlapLock.hpp"
#include "Function.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"

SlapLock* SlapLock::ctor_43DC80(Path_SlapLock* pTlv, int tlvInfo)
{
    NOT_IMPLEMENTED();

    ctor_408240(0);
    SetVTable(this, 0x545224);

    field_4_typeId = Types::eLockedSoul_61;
    field_118_pTlv = pTlv;
    field_11C_tlvInfo = tlvInfo;
    field_C_objectId = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 1053); // TODO: Id
    Animation_Init_424E10(7068, 58, 44, ppRes, 1, 1);

    if (field_118_pTlv->field_10_scale)
    {
        field_20_animation.field_C_render_layer = 6;
    }
    else
    {
        field_20_animation.field_C_render_layer = 25;
    }

    field_120_state = 0;
    field_124_timer1 = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + 25;
    field_134_id = -1;
    field_138_possesion_flicker_id = -1;
    const FP midX = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_128_midX = midX;
    field_B8_xpos = midX;

    const FP ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_12C_midY = ypos;
    field_BC_ypos = ypos;

    field_130_has_ghost = field_118_pTlv->field_18_has_ghost;

    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // TODO: Requires the layout of object eSlapLock_OrbWhirlWind_60
       // if (pObj->field_4_typeId == Types::eSlapLock_OrbWhirlWind_60 && pObj[2].field_4_typeId == field_118_pTlv->field_14_target_tomb_id2)
        {
            field_130_has_ghost = 0;
        }
    }

    if (SwitchStates_Get_466020(pTlv->field_14_target_tomb_id2))
    {
        field_130_has_ghost = 0;
    }

    if (pTlv->field_1_unknown == 0)
    {
        return this;
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_124_timer1 = sGnFrame_5C1B84 + 60;
    field_13C_timer2 = sGnFrame_5C1B84 + 30;

    if (field_118_pTlv->field_1A_has_powerup != 0)
    {
        field_120_state = 4;
    }
    else
    {
        field_120_state = 3;
    }

    return this;
}

void SlapLock::dtor_43DF00()
{
    SetVTable(this, 0x545224);
    Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

SlapLock* SlapLock::vdtor_43DED0(signed int flags)
{
    dtor_43DF00();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void SlapLock::vScreenChanged_43E840()
{
    if (field_120_state == 5 || field_120_state == 6)
    {
        GiveInvisiblity_43E880();
    }
    field_6_flags.Set(BaseGameObject::eDead);
}

void SlapLock::GiveInvisiblity_43E880()
{
    field_118_pTlv = static_cast<Path_SlapLock*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_11C_tlvInfo));
    if (sActiveHero_5C1B68)
    {
        sActiveHero_5C1B68->field_176 = field_118_pTlv->field_1C_powerup_id;
        sActiveHero_5C1B68->field_16C_bHaveShrykull = 0;
        sActiveHero_5C1B68->field_16E_bHaveInvisiblity = 1;
        sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 200000;
    }
}
