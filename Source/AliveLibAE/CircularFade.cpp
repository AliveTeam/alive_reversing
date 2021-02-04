#include "stdafx.h"
#include "CircularFade.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

BaseGameObject* CircularFade::VDestructor(signed int flags)
{
    return vdtor_4CE0D0(flags);
}

void CircularFade::VUpdate()
{
    vUpdate_4CE380();
}

void CircularFade::VRender(PrimHeader** ppOt)
{
    vRender_4CE3F0(ppOt);
}

void CircularFade::VScreenChanged()
{
    // null sub
}

int CircularFade::VFadeIn_4CE300(__int16 direction, char destroyOnDone)
{
    return vFadeIn_4CE300(direction, destroyOnDone);
}

int CircularFade::VDone_4CE0B0()
{
    return vDone_4CE0B0();
}

CircularFade* CircularFade::ctor_4CE100(FP xpos, FP ypos, FP scale, __int16 direction, char destroyOnDone)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

    SetVTable(this, 0x547904); // vTbl_CircularFade_547904

    if (direction)
    {
        field_1B8_fade_colour = 0;
    }
    else
    {
        field_1B8_fade_colour = 255;
    }

    vFadeIn_4CE300(direction, destroyOnDone);

    const BYTE fade_rgb = static_cast<BYTE>((field_1B8_fade_colour * 60) / 100);
    field_D4_b = fade_rgb;
    field_D2_g = fade_rgb;
    field_D0_r = fade_rgb;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSpotliteResID);
    Animation_Init_424E10(1536, 57, 32, ppRes, 1, 1u);

    field_DC_bApplyShadows &= ~1u;

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    field_CC_sprite_scale.fpValue = scale.fpValue * 2;
    field_20_animation.field_14_scale.fpValue = scale.fpValue * 2;

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_20_animation.field_B_render_mode = 2;
    field_20_animation.field_C_render_layer = Layer::eLayer_40;
    field_D0_r = field_1B8_fade_colour;
    field_D2_g = field_1B8_fade_colour;
    field_D4_b = field_1B8_fade_colour;

    Init_SetTPage_4F5B60(&field_198_tPages[0], 0, 0, PSX_getTPage_4F60E0(2, 2, 0, 0));
    Init_SetTPage_4F5B60(&field_198_tPages[1], 0, 0, PSX_getTPage_4F60E0(2, 2, 0, 0));
    return this;
}

void CircularFade::vRender_4CE3F0(PrimHeader** ppOt)
{
    const BYTE fade_rgb = static_cast<BYTE>((field_1B8_fade_colour * 60) / 100);

    field_D4_b = fade_rgb;
    field_D2_g = fade_rgb;
    field_D0_r = fade_rgb;
    field_20_animation.field_8_r = fade_rgb;
    field_20_animation.field_9_g = fade_rgb;
    field_20_animation.field_A_b = fade_rgb;

    field_20_animation.vRender_40B820(
        FP_GetExponent(FP_FromInteger(field_DA_xOffset) + field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
        FP_GetExponent(FP_FromInteger(field_D8_yOffset) + field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
        ppOt,
        0,
        0);

    PSX_RECT frameRect = {};
    field_20_animation.Get_Frame_Rect_409E10(&frameRect);

    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        frameRect.x,
        frameRect.y,
        frameRect.w,
        frameRect.h,
        pScreenManager_5BB5F4->field_3A_idx);

    --frameRect.h;
    --frameRect.w;

    if (frameRect.y < 0)
    {
        frameRect.y = 0;
    }

    if (frameRect.x < 0)
    {
        frameRect.x = 0;
    }

    if (frameRect.w >= 640)
    {
        frameRect.w = 639;
    }

    if (frameRect.h >= 240)
    {
        frameRect.h = 240;
    }

    const BYTE fadeColour = static_cast<BYTE>(field_1B8_fade_colour);

    Prim_Tile* pTile1 = &field_F8[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_Tile(pTile1);
    SetRGB0(pTile1, fadeColour, fadeColour, fadeColour);
    SetXY0(pTile1, 0, 0);
    pTile1->field_14_w = gPsxDisplay_5C1130.field_0_width;
    pTile1->field_16_h = frameRect.y;
    Poly_Set_SemiTrans_4F8A60(&pTile1->mBase.header, 1);
    OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_animation.field_C_render_layer), &pTile1->mBase.header);


    Prim_Tile* pTile2 = &field_120[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_Tile(pTile2);
    SetRGB0(pTile2, fadeColour, fadeColour, fadeColour);
    SetXY0(pTile2, 0, frameRect.y);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        pTile2->field_14_w = frameRect.x + 1;
    }
    else
    {
        pTile2->field_14_w = frameRect.x;
    }
    pTile2->field_16_h = frameRect.h - frameRect.y;
    Poly_Set_SemiTrans_4F8A60(&pTile2->mBase.header, 1);
    OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_animation.field_C_render_layer), &pTile2->mBase.header);

    Prim_Tile* pTile3 = &field_148[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_Tile(pTile3);
    SetRGB0(pTile3, fadeColour, fadeColour, fadeColour);
    SetXY0(pTile3, frameRect.w, frameRect.y);
    pTile3->field_14_w = gPsxDisplay_5C1130.field_0_width - frameRect.w;
    pTile3->field_16_h = frameRect.h - frameRect.y;
    Poly_Set_SemiTrans_4F8A60(&pTile3->mBase.header, 1);
    OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_animation.field_C_render_layer), &pTile3->mBase.header);

    Prim_Tile* pTile4 = &field_170[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_Tile(pTile4);
    SetRGB0(pTile4, fadeColour, fadeColour, fadeColour);
    SetXY0(pTile4, 0, frameRect.h);
    pTile4->field_14_w = gPsxDisplay_5C1130.field_0_width;
    pTile4->field_16_h = gPsxDisplay_5C1130.field_2_height - frameRect.h;
    Poly_Set_SemiTrans_4F8A60(&pTile4->mBase.header, 1);
    OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_animation.field_C_render_layer), &pTile4->mBase.header);

    OrderingTable_Add_4F8AA0(OtLayer(ppOt, field_20_animation.field_C_render_layer), &field_198_tPages[gPsxDisplay_5C1130.field_C_buffer_index].mBase);

    if (field_1B8_fade_colour < 255)
    {
        pScreenManager_5BB5F4->InvalidateRect_40EC10(
            0,
            0,
            gPsxDisplay_5C1130.field_0_width,
            gPsxDisplay_5C1130.field_2_height);
    }

    if ((field_1B8_fade_colour == 255 && field_F4_flags.Get(Flags::eBit1_FadeIn)) ||
        (field_1B8_fade_colour == 0 && !(field_F4_flags.Get(Flags::eBit1_FadeIn))))
    {
        if (!(field_F4_flags.Get(Flags::eBit2_Done)))
        {
            field_F4_flags.Set(Flags::eBit2_Done);
            --sNum_CamSwappers_5C1B66;
        }

        if (field_F4_flags.Get(Flags::eBit3_DestroyOnDone))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

void CircularFade::vUpdate_4CE380()
{
    if ((!field_F4_flags.Get(Flags::eBit4_NeverSet) && !field_F4_flags.Get(Flags::eBit2_Done)))
    {
        field_1B8_fade_colour += field_1BA_speed;
        if (field_F4_flags.Get(Flags::eBit1_FadeIn))
        {
            if (field_1B8_fade_colour > 255)
            {
                field_1B8_fade_colour = 255;
            }
        }
        else if (field_1B8_fade_colour < 0)
        {
            field_1B8_fade_colour = 0;
        }
    }
}

int CircularFade::vFadeIn_4CE300(__int16 direction, char destroyOnDone) // TODO: Likely no return
{
    sNum_CamSwappers_5C1B66++;

    field_F4_flags.Set(Flags::eBit1_FadeIn, direction);

    field_F4_flags.Clear(Flags::eBit2_Done);
    field_F4_flags.Clear(Flags::eBit4_NeverSet);


    field_F4_flags.Set(Flags::eBit3_DestroyOnDone, destroyOnDone);

    if (field_F4_flags.Get(Flags::eBit1_FadeIn))
    {
        field_1BA_speed = 12;
    }
    else
    {
        field_1BA_speed = -12;
    }
    return field_F4_flags.Raw().all;
}

int CircularFade::vDone_4CE0B0()
{
    return field_F4_flags.Get(Flags::eBit2_Done);
}

void CircularFade::dtor_4CE080()
{
    SetVTable(this, 0x547904); // vTbl_CircularFade_547904

    if (!(field_F4_flags.Get(Flags::eBit2_Done)))
    {
        --sNum_CamSwappers_5C1B66;
    }
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

CircularFade* CircularFade::vdtor_4CE0D0(signed int flags)
{
    dtor_4CE080();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

CircularFade* CC Make_Circular_Fade_4CE8C0(FP xpos, FP ypos, FP scale, __int16 direction, char destroyOnDone, char setBit8)
{
    auto pFade = ae_new<CircularFade>();
    if (!pFade)
    {
        return nullptr;
    }

    pFade->ctor_4CE100(xpos, ypos, scale, direction, destroyOnDone);

    if (setBit8)
    {
        pFade->field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    }
    else
    {
        pFade->field_6_flags.Clear(BaseGameObject::eSurviveDeathReset_Bit9);
    }

    return pFade;
}
