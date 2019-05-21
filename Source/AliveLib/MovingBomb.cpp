#include "stdafx.h"
#include "MovingBomb.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"

TintEntry stru_55C734[4] =
{
    { 6u, 97u, 97u, 97u },
    { 10u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u }
};

ALIVE_VAR(1, 0x5C300C, MovingBomb*, dword_5C300C, nullptr);

MovingBomb* MovingBomb::ctor_46FD40(Path_MovingBomb* pTlv, int tlvInfo)
{
    ctor_408240(0);
    field_6_flags.Set(BaseGameObject::eCanExplode);

    SetVTable(this, 0x546270);
    field_4_typeId = Types::eTimedMine_or_MovingBomb_10;
    
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 3006);
    Animation_Init_424E10(17548, 76, 30, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = 0;
    field_118_state = 1;

    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 16;
    }
    else if (pTlv->field_16_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 35;
    }

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_124_speed = FP_FromRaw(pTlv->field_10_speed << 8);
    field_C4_velx = FP_FromRaw(pTlv->field_1C_start_speed << 8);
    field_128_switch_id = pTlv->field_12_id;
    field_120_timer = sGnFrame_5C1B84;
    field_11C_tlvInfo = tlvInfo;
    field_12C_max = 0;
    field_12A_min = 0;
    field_136_persist_offscreen = pTlv->field_1E_persist_offscreen;
    field_130_sound_channels = 0;

    if (pTlv->field_14_start_type)
    {
        field_118_state = 0;
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint_425600(&stru_55C734[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_134 = pTlv->field_1A_disable_resources;
    if (!(field_134 & 1))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 25);
    }

    Add_Resource_4DC130(ResourceManager::Resource_Animation, 301);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 365);
    Add_Resource_4DC130(ResourceManager::Resource_Palt, 25);
    Add_Resource_4DC130(ResourceManager::Resource_Palt, 576);
    
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos + FP_FromInteger(24),
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        0x100))
    {
        field_BC_ypos = hitY;
        field_B8_xpos = hitX;
    }

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

MovingBomb* MovingBomb::vdtor_470040(signed int flags)
{
    dtor_4700C0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void MovingBomb::dtor_4700C0()
{
    SetVTable(this, 0x546270);
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
    }

    if (field_118_state >= 6)
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlvInfo, -1, 0, 0);
    }

    if (dword_5C300C == this)
    {
        if (field_130_sound_channels)
        {
            SND_Stop_Channels_Mask_4CA810(field_130_sound_channels);
            field_130_sound_channels = 0;
        }
        dword_5C300C = 0;
    }
    dtor_4080B0();
}
