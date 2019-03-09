#include "stdafx.h"
#include "ChantSuppressor.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "Function.hpp"

const TintEntry sChantOrbTints_55C1EC[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


ChantSuppressor* ChantSuppressor::ctor_466350(Path_ChantSuppressor* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x545F3C);
    
    field_4_typeId = Types::eAntiChant_83;

    field_6_flags.Set(BaseGameObject::eCanExplode);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMaimGameResID);
    Animation_Init_424E10(2228, 53, 28u, ppRes, 1, 1u);
    
    SetTint_425600(sChantOrbTints_55C1EC, gMap_5C3030.sCurrentLevelId_5C3030);

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_118_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 8;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 27;
    }

    field_DC_bApplyShadows |= 2u;
    field_11C_state = 0;
    field_124_sound_channels_mask = 0;

    return this;
}

ChantSuppressor* ChantSuppressor::vdtor_4664B0(char flags)
{
    dtor_4664E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ChantSuppressor::dtor_4664E0()
{
    SetVTable(this, 0x545F3C);

    if (field_124_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_124_sound_channels_mask);
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }

    dtor_4080B0();
}

void ChantSuppressor::vScreenChanged_466D20()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

signed __int16 ChantSuppressor::vTakeDamage_466BB0(BaseGameObject* pFrom)
{
    NOT_IMPLEMENTED();

    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    field_6_flags.Set(BaseGameObject::eDead);
    field_10C_health = FP_FromInteger(0);

    if (pFrom->field_4_typeId == Types::eMineCar_89 || pFrom->field_4_typeId == Types::eType_104 || pFrom->field_4_typeId == Types::eType_121)
    {
        /* TODO
        auto pExplosionMem = alive_new<Explosion>();
        if (pExplosionMem)
        {
            pExplosionMem->ctor_4A1200(
                field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
                field_CC_sprite_scale,
                0);
        }
        */

        auto pGibsMem = alive_new<Gibs>();
        if (pGibsMem)
        {
            pGibsMem->ctor_40FB40(
                5,
                field_B8_xpos,
                field_BC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                0);
        }
    }

    return 1;
}
