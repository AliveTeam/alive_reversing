#include "stdafx.h"
#include "ParamiteWebLine.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"

ParamiteWebLine* ParamiteWebLine::ctor_4E1FC0(Path_ParamiteWebLine* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930( 0);
    SetVTable(&field_108_anim_flare, 0x544290);
    SetVTable(this, 0x547FA0);
    field_4_typeId = Types::eWebLine_146;
    field_100 = tlvInfo;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, kWebResID);
    Animation_Init_424E10(148, 5, 16u, ppRes, 1, 1u);

    if (pTlv->field_10_scale)
    {
        field_20_animation.field_C_render_layer = 5;
        field_20_animation.field_14_scale = FP_FromDouble(0.7);
        field_CC_sprite_scale = FP_FromDouble(0.7);
        field_D6_scale = 0;
        field_F6_piece_length = 7;
        field_1A2 = 1;
        field_D0_r = 50;
        field_D2_g = 50;
        field_D4_b = 200;
    }
    else
    {
        field_20_animation.field_C_render_layer = 24;
        field_20_animation.field_14_scale = FP_FromInteger(1);
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_F6_piece_length = 15;
        field_1A2 = 2;
        field_D0_r = 10;
        field_D2_g = 10;
        field_D4_b = 10;
    }

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos + FP_FromInteger(20),
        field_BC_ypos + FP_FromInteger(20),
        &pLine, &hitX, &hitY, 0x100))
    {
        field_B8_xpos = FP_FromInteger(pLine->field_0_rect.x);

        const FP screenTop = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
        if (FP_FromInteger(pLine->field_0_rect.y) >= screenTop)
        {
            field_F8_top = pLine->field_0_rect.y;
        }
        else
        {
            field_F8_top = FP_GetExponent(screenTop);
        }

        const FP screenBottom = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(240);
        if (FP_FromInteger(pLine->field_0_rect.h) <= screenBottom)
        {
            field_FA_bottom = pLine->field_0_rect.h;
        }
        else
        {
            field_FA_bottom = FP_GetExponent(screenBottom);
        }
        field_BC_ypos = FP_FromInteger(field_FA_bottom);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    field_F4_anim_segment_count = (field_F6_piece_length + field_FA_bottom - field_F8_top) / field_F6_piece_length;
    field_FC = reinterpret_cast<AnimationUnknown*>(malloc_non_zero_4954F0(sizeof(AnimationUnknown) * field_F4_anim_segment_count));
    for (int i = 0; i < field_F4_anim_segment_count; i++)
    {
        new (&field_FC[i]) AnimationUnknown(); // We have memory but no constructor was called.. so use placement new to get a constructed instance
        SetVTable(&field_FC[i], 0x5447CC);

        field_FC[i].field_4_flags.Set(AnimFlags::eBit3_Render);
        field_FC[i].field_68_anim_ptr = &field_20_animation;
        field_FC[i].field_C_render_layer = field_20_animation.field_C_render_layer;
        field_FC[i].field_6C_scale = field_CC_sprite_scale;
        field_FC[i].field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_FC[i].field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    }

    field_104 = 0;
    field_106_wobble_pos = field_F8_top;

    BYTE** ppFlareRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kOmmflareResID, 0, 0);
    if (field_108_anim_flare.Init_40A030(1632, gObjList_animations_5C1A24, this, 39, 21u, ppFlareRes, 1u, 0, 0))
    {
        field_108_anim_flare.field_8_r = 100;
        field_108_anim_flare.field_9_g = 100;
        field_108_anim_flare.field_A_b = 100;

        field_108_anim_flare.field_C_render_layer = field_20_animation.field_C_render_layer;
        field_108_anim_flare.field_14_scale = FP_FromDouble(0.3);
        field_108_anim_flare.field_B_render_mode = 1;

        field_108_anim_flare.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_108_anim_flare.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_108_anim_flare.field_4_flags.Set(AnimFlags::eBit7_SwapXY);

        field_1A0 = field_F8_top;
        field_1A4_delay_counter = Math_RandomRange_496AB0(0, 10);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }
    return this;
}

void ParamiteWebLine::Wobble_4E29D0(short /*ypos*/)
{
    NOT_IMPLEMENTED();
}

void ParamiteWebLine::VScreenChanged()
{
    return vScreenChanged_4E2BC0();
}

BaseGameObject* ParamiteWebLine::VDestructor(signed int flags)
{
    return vdtor_4E2460(flags);
}

ParamiteWebLine* ParamiteWebLine::vdtor_4E2460(signed int flags)
{
    dtor_4E2490();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ParamiteWebLine::dtor_4E2490()
{
    SetVTable(this, 0x547FA0);
    Mem_Free_495560(field_FC);
    field_108_anim_flare.vCleanUp_40C630();
    Path::TLV_Reset_4DB8E0(field_100, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void ParamiteWebLine::vScreenChanged_4E2BC0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}
