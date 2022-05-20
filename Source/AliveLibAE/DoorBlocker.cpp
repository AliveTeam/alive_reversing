#include "stdafx.h"
#include "DoorBlocker.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

DoorBlocker* DoorBlocker::ctor_41F0A0(Path_DoorBlocker* pTlv, s32 tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x544A1C);

    mFlags.Set(BaseGameObject::eCanExplode_Bit7);

    field_11A_switch_id = pTlv->field_12_switch_id;

    const AnimRecord& rec = AnimRec(AnimId::Door_Lock_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_118_bDone &= ~1u;

    SetType(AETypes::eDoorLock_34);
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_11C_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_Half_7;
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_26;
        field_D6_scale = 1;
    }

    if (SwitchStates_Get_466020(field_11A_switch_id))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    field_DC_bApplyShadows |= 2u;

    return this;
}

void DoorBlocker::dtor_41F310()
{
    SetVTable(this, 0x544A1C);
    Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

void DoorBlocker::VUpdate()
{
    vUpdate_41F250();
}

BaseGameObject* DoorBlocker::VDestructor(s32 flags)
{
    return vdtor_41F220(flags);
}

void DoorBlocker::vUpdate_41F250()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (!mFlags.Get(BaseGameObject::eDead))
    {
        if (field_118_bDone & 1)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mFlags.Set(BaseGameObject::eDead);
            }
        }
        else if (SwitchStates_Get_466020(field_11A_switch_id))
        {
            SFX_Play(SoundEffect::DoorEffect_57, 100, 900);
            SFX_Play(SoundEffect::DoorEffect_57, 100, -100);
            const AnimRecord& animRec = AnimRec(AnimId::Door_Lock_Open);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, 0);
            field_118_bDone |= 1u;
        }
    }
}

DoorBlocker* DoorBlocker::vdtor_41F220(s32 flags)
{
    dtor_41F310();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}
