#include "stdafx.h"
#include "SligGetPantsAndWings.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Slig.hpp"

SligGetPantsAndWings* SligGetPantsAndWings::ctor_465BF0(Path_TLV* pTlv, s32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x545ED8);

    SetType(AETypes::eSligGetPantsOrWings_80);
    field_F8_tlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSligLocker_Closed);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    // HACK: See header for details
    auto pHack = static_cast<Path_Slig*>(pTlv);
    if (pHack->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else if (pHack->field_10_scale == Scale_short::eFull_0)
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_25;
    }

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    return this;
}

BaseGameObject* SligGetPantsAndWings::VDestructor(s32 flags)
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
        mFlags.Set(BaseGameObject::eDead);
    }

    switch (field_F4_state)
    {
        case State::eWaiting_0:
            if (pTlv->field_1_tlv_state)
            {
                field_F4_state = State::eActive_1;
                field_FC_timer = sGnFrame_5C1B84 + 8;
            }
            break;

        case State::eActive_1:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_FC_timer)
            {
                field_F4_state = State::eFinished_2;
                SFX_Play_46FA90(SoundEffect::NakedSligTransformEnd_92, 0);
                const AnimRecord& rec = AnimRec(AnimId::CrawlingSligLocker_Open);
                field_20_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, nullptr);
            }
            break;

        case State::eFinished_2:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_F4_state = State::eWaiting_0;
                const AnimRecord& rec = AnimRec(AnimId::CrawlingSligLocker_Closed);
                field_20_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, nullptr);
                pTlv->field_1_tlv_state = 0;
            }
            break;
    }
}

SligGetPantsAndWings* SligGetPantsAndWings::vdtor_465D10(s32 flags)
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
    mFlags.Set(BaseGameObject::eDead);
}

void SligGetPantsAndWings::dtor_465D40()
{
    SetVTable(this, 0x545ED8);
    Path::TLV_Reset_4DB8E0(field_F8_tlvInfo, 0, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}
