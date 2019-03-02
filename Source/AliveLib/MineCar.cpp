#include "stdafx.h"
#include "MineCar.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "Function.hpp"

MineCar* MineCar::ctor_46BC80(Path_MineCar* pTlv, int tlvInfo, int /*a4*/, int /*a5*/, int /*a6*/)
{
    ctor_408240(0);
    SetVTable(this, 0x544290);
    SetVTable(&field_124_anim, 0x5461FC);
    field_4_typeId = Types::eMineCar_89;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6013); // TODO: Id
    Animation_Init_424E10(
        20812,
        128,
        62,
        ppRes,
        1,
        1);

    field_11E_scale = pTlv->field_10_scale;
    field_120_max_damage = pTlv->field_12_max_damage;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_DC_bApplyShadows |= 2u;
    field_1BE = 0;
    field_20_animation.field_C_render_layer = 26;
    field_CC_sprite_scale = FP_FromInteger(1);
    field_D6_scale = 1;

    if (field_11E_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 7;
        field_D6_scale = 0;
    }

    MapFollowMe_408D10(TRUE);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }
    
    field_118_tlvInfo = tlvInfo;
    field_11C_state = 0;

    LoadAnimation_46BF80(&field_124_anim);

    field_6_flags.Set(BaseGameObject::eCanExplode);

    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    Add_Resource_4DC130(ResourceManager::Resource_Animation, 113);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 365);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 301);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 25);

    field_1C0 = 0;
    field_1C2 = 0;

    field_1D4_throw_item_key1 = static_cast<short>(sInputKey_ThrowItem_5550F4);
    field_1D6_throw_item_key2 = static_cast<short>(sInputKey_ThrowItem_5550F4);

    field_1CC_spawned_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_1CE_spawned_camera = gMap_5C3030.sCurrentCamId_5C3034;
    field_1D0_sound_channels_mask = 0;
    field_1C4 = 0;

    return this;
}

void MineCar::LoadAnimation_46BF80(Animation* pAnim)
{
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6013);
    if (pAnim->Init_40A030(20824, gObjList_animations_5C1A24, this, 130, 62u, ppRes, 1, 0, 0))
    {
        pAnim->field_C_render_layer = field_20_animation.field_C_render_layer;
        pAnim->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        pAnim->field_14_scale = field_CC_sprite_scale;
        pAnim->field_8_r = 128;
        pAnim->field_9_g = 128;
        pAnim->field_A_b = 128;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }
}

void MineCar::vStopAudio_46F9C0()
{
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
}

void MineCar::vScreenChanged_46F800()
{
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
    BaseGameObject::VScreenChanged();
}

MineCar* MineCar::vdtor_46BF50(signed int flags)
{
    dtor_46F2A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void MineCar::dtor_46F2A0()
{
    SetVTable(this, 0x5461FC);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
    field_124_anim.vCleanUp_40C630();
    dtor_4080B0();
}

BOOL MineCar::CheckCollision_46F6B0(FP hitX, FP hitY)
{
    PathLine *pPathLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(
        hitX + field_B8_xpos,
        field_BC_ypos,
        hitX + field_B8_xpos,
        field_BC_ypos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 8 : 128) != 0;
}

__int16 MineCar::CheckCollision_46F730(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;

    if (!sCollisions_DArray_5C1128->Raycast_417A60(
        hitX + field_B8_xpos,
        field_BC_ypos - FP_FromInteger(4),
        hitX + field_B8_xpos,
        field_BC_ypos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16))
    {
        return 0;
    }

    if (field_BC_ypos > hitY)
    {
        field_BC_ypos = hitY;
    }

    return 1;
}

void MineCar::vUpdate_46C010()
{
    NOT_IMPLEMENTED();
}
