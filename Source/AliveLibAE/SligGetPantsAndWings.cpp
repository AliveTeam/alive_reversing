#include "stdafx.h"
#include "SligGetPantsAndWings.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Slig.hpp"

SligGetPantsAndWings* SligGetPantsAndWings::ctor_465BF0(Path_TLV* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x545ED8);

    field_4_typeId = Types::eSligGetPantsOrWings_80;
    field_F8_tlvInfo = tlvInfo;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kUnknownResID_448);
    Animation_Init_424E10(4352, 37, 70u, ppRes, 1, 1);

    // HACK: See header for details
    auto pHack = static_cast<Path_Slig*>(pTlv);
    if (pHack->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 6;
    }
    else if (pHack->field_10_scale == 0)
    {
        field_20_animation.field_C_render_layer = 25;
    }

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    return this;
}

BaseGameObject* SligGetPantsAndWings::VDestructor(signed int flags)
{
    return vdtor_465D10(flags);
}

void SligGetPantsAndWings::VUpdate()
{
    vUpdate_465DD0();
}


void SligGetPantsAndWings::VScreenChanged()
{
    vScreenChanged_465EE0();
}

void SligGetPantsAndWings::vUpdate_465DD0()
{
    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_F8_tlvInfo);
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_F4_state)
    {
        case State::eWaiting_0:
            if (pTlv->field_1_unknown)
            {
                field_F4_state = State::eActive_1;
                field_FC_timer = sGnFrame_5C1B84 + 8;
            }
            break;

        case State::eActive_1:
            if (static_cast<int>(sGnFrame_5C1B84) > field_FC_timer)
            {
                field_F4_state = State::eFinished_2;
                SFX_Play_46FA90(SoundEffect::NakedSligTransformEnd_92, 0);
                field_20_animation.Set_Animation_Data_409C80(4364, nullptr);
            }
            break;

        case State::eFinished_2:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_F4_state = State::eWaiting_0;
                field_20_animation.Set_Animation_Data_409C80(4352, nullptr);
                pTlv->field_1_unknown = 0;
            }
            break;
    }
}

SligGetPantsAndWings* SligGetPantsAndWings::vdtor_465D10(signed int flags)
{
    dtor_465D40();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void SligGetPantsAndWings::vScreenChanged_465EE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void SligGetPantsAndWings::dtor_465D40()
{
    SetVTable(this, 0x545ED8);
    Path::TLV_Reset_4DB8E0(field_F8_tlvInfo, 0, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}
