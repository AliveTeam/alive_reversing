#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "Events.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "LCDScreen.hpp"
#include "VRam.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "StringFormatters.hpp"
#include "Primitives_common.hpp"
#include "Input.hpp"

namespace AO {

const BYTE sLCDScreen_Palette_4C75A8[32] =
{
    0u,
    0u,
    1u,
    128u,
    1u,
    132u,
    32u,
    132u,
    33u,
    128u,
    32u,
    132u,
    33u,
    132u,
    101u,
    206u,
    57u,
    231u,
    140u,
    177u,
    96u,
    142u,
    100u,
    206u,
    101u,
    206u,
    215u,
    152u,
    20u,
    161u,
    24u,
    216u
};

const BYTE sLCDScreen_Palette2_4C7588[32] =
{
    0u,
    0u,
    1u,
    128u,
    1u,
    132u,
    32u,
    132u,
    33u,
    128u,
    32u,
    132u,
    33u,
    132u,
    5u,
    132u,
    57u,
    231u,
    140u,
    177u,
    19u,
    148u,
    100u,
    206u,
    101u,
    206u,
    215u,
    152u,
    20u,
    161u,
    24u,
    216u
};

const char* sLCDMessageTable_4C7420[90] =
{
    "",
    "                               The profits justify the means.",
    "                               You are who you eat.",
    "                               Only 1,236 work related accidents this month. Keep up the good work!",
    "                               Don't play with your food, unless it plays with you first.",
    "                               Venerate Authority.",
    "                               Lead Mudokons to bird portals.",
    "                               You are not the only one who can hide in shadows.",
    "                               When a Slig shouts, Look Out!, the wise Mudokon hits the deck.",
    "                               Decapitation toll cut in half!",
    "                               Work hard, die young, win valuable prizes!",
    "                               A little hard work never killed anyone important.",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "                               To hoist, stand under the ledge and press \x10 (up).",
    "                               To jump, press \x0c.   To run and jump, press \x0c while you run using \x0a.",
    "                               To sneak past the sleeping Slig, hold \x0b while you move.",
    "                               To pull the lever, stand next to it and press \x0d.",
    "                               To use the ring, stand under it, and press \x10, just like a hoist.",
    "                               To ride the platform, stand in the center of it, and press \x11 (down).",
    "                               To enter the door, stand in front of it and press \x10.",
    "                               To deactivate a bomb, crouch next to it by pressing \x11.   Then carefully press \x0d when the bomb blinks green.  Press \x0d again to reactivate it.",
    "                               Sneak to shadows by holding \x0b while you move.   Stand still in shadows while Slig is facing you, or you will be seen.  Sneak behind Slig when it walks away from you.",
    "                               To get a grenade from the Boom Machine, stand in front of it and press \x10.   To pick up a grenade, crouch next to it and press \x0d.   To throw the grenade, hold \x0e, then press any directional button.   Different directions will throw at different angles.",
    "                               It is possible to throw while crouching.",
    "                               To alert a Mudokon, say hello by holding \x08 and pressing \x0c.   Then, talk to Mudokons by holding \x08 or \x09 and pressing the \x0c, \x0d, \x0e, or \x0f button.   Experiment!",
    "                               Possess Sligs by chanting.   Chant by holding \x08 and \x09 at the same time or pressing (0) (zero) on the keyboard.",
    "                               To help Mudokons escape from slavery in Rupture Farms, chant when you see a circle of birds.   To chant, hold \x08 and \x09 at the same time or pressing (0) on the keyboard.",
    "                               To roll, first crouch by pressing \x11.   Then roll left or right by pressing \x12 or \x13.",
    "                               To run, hold \x0a while you press \x12 (left) or \x13 (right).",
    "                               To avoid the alarm, stand still when the Motion Detectors touch you.",
    "                               Use Slig to talk your way past the security door.",
    "                               Sligs can talk to Slogs.",
    "                               Use lever to operate meat saw.",
    "                               Lead Mudokons to bird portals, then chant to rescue them.  Chant by holding \x08 and \x09.",
    "",
    "",
    "",
    "",
    "                               You're almost free, Abe!   You must get through the stockyards.   Good luck ...  and watch out for those Scrabs!   They haven't been fed in awhile.",
    "                               Mudokons entering this door will be slaughtered and packaged as disgusting yet yummy novelty meat products.",
    "                               Do it, Abe ...  do it!!!",
    "                               Your first task is to escape from Rupture Farms. Watch out for Sligs!",
    "                               Run, Abe!   The Sligs have orders to shoot you on sight!",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

LCDScreen* LCDScreen::ctor_433F60(Path_LCDScreen* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB468);

    field_2BC_tlv = *pTlv;

    field_2B8_tlv_item_info = tlvInfo;

    field_2AC_message_1_id = pTlv->field_18_message_1_id;

    field_50_font_context.LoadFontType_41C040(2);
    field_60_font.ctor_41C170(60, sLCDScreen_Palette_4C75A8, &field_50_font_context);
    Pal_Allocate_4476F0(&field_98_pal_rect, 16u);

    const PSX_RECT palSize = { field_98_pal_rect.x, field_98_pal_rect.y,16, 1 };
    PSX_LoadImage16_4962A0(&palSize, sLCDScreen_Palette2_4C7588);

    if (Input_JoyStickEnabled() || field_2AC_message_1_id != 62)
    {
        String_FormatString_450DC0(sLCDMessageTable_4C7420[field_2AC_message_1_id], field_AC_message_buffer);
    }
    else
    {
        strcpy(field_AC_message_buffer,
            "                               To alert a Mudokon, say hello by holding (1) on the keyboard.   Then, talk to"
            " Mudokons by using the keyboard numbers (1) thru (8).   Experiment!");
    }

    field_A0_message = field_AC_message_buffer;
    field_A4_message_cutoff_ptr = 0;
    field_2B0_x_offset = 0;

    field_6_flags.Set(Options::eDrawable_Bit4);

    field_2D8_message_rand_min = pTlv->field_1A_message_rand_min;
    pad_2DA = pTlv->field_1C_message_rand_max;

    field_2DC_message_rand_max = pTlv->field_1C_message_rand_max;

    sFontDrawScreenSpace_508BF4 = 1;
    field_2B4_character_width = field_60_font.MeasureWidth_41C200(field_AC_message_buffer[0]);
    sFontDrawScreenSpace_508BF4 = 0;

    field_2D4 = 0;
    gObjList_drawables_504618->Push_Back(this);

    return this;
}

BaseGameObject* LCDScreen::dtor_434100()
{
    SetVTable(this, 0x4BB468);
    Pal_Free_447870({ field_98_pal_rect.x, field_98_pal_rect.y }, field_98_pal_rect.w);
    gObjList_drawables_504618->Remove_Item(this);
    gMap_507BA8.TLV_Reset_446870(field_2B8_tlv_item_info, -1, 0, 0);
    field_60_font.dtor_41C130();
    field_50_font_context.dtor_41C110();
    return dtor_487DF0();
}

BaseGameObject* LCDScreen::VDestructor(signed int flags)
{
    return Vdtor_434630(flags);
}

BaseGameObject* LCDScreen::Vdtor_434630(signed int flags)
{
    dtor_434100();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void LCDScreen::VScreenChanged()
{
    VScreenChanged_434620();
}

void LCDScreen::VScreenChanged_434620()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}


void LCDScreen::VUpdate()
{
    VUpdate_4341B0();
}

void LCDScreen::VUpdate_4341B0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

#if LCD_PS1_SPEED
    field_2B0_x_offset += 6;
#else
    field_2B0_x_offset += 3;
#endif

    if (field_2B0_x_offset > field_2B4_character_width)
    {
        field_2B0_x_offset -= field_2B4_character_width;
        char* pMsg = field_A0_message;
        field_A0_message++;
        if (!*pMsg)
        {
            field_A0_message = field_AC_message_buffer;
            field_2D4++;
            if (field_2D4 == 1)
            {
                const auto rangedRandom = Math_RandomRange_450F20(
                    field_2D8_message_rand_min,
                    field_2DC_message_rand_max
                );

                if (Input_JoyStickEnabled() || rangedRandom != 62)
                {
                    String_FormatString_450DC0(sLCDMessageTable_4C7420[rangedRandom], field_AC_message_buffer);
                }
                else
                {
                    strcpy(
                        field_AC_message_buffer,
                        "                               To alert a Mudokon, say hello by holding (1) on the keyboard.   Then, talk to Mudokons by using the keyboard numbers (1) thru (8).   Experiment!"
                    );
                }
            }
            else
            {
                field_2D4 = 0;

                if (Input_JoyStickEnabled() || field_2AC_message_1_id != 62)
                {
                    String_FormatString_450DC0(
                        sLCDMessageTable_4C7420[field_2AC_message_1_id],
                        field_AC_message_buffer
                    );
                }
                else
                {
                    strcpy(
                        field_AC_message_buffer,
                        "                               To alert a Mudokon, say hello by holding (1) on the keyboard.   Then, talk to Mudokons by using the keyboard numbers (1) thru (8).   Experiment!"
                    );
                }
            }
            auto palSwap = field_98_pal_rect;
            field_98_pal_rect = field_60_font.field_28_palette_rect;
            field_60_font.field_28_palette_rect = palSwap;
        }
        sFontDrawScreenSpace_508BF4 = 1;
        field_2B4_character_width = field_60_font.MeasureWidth_41C200(*field_A0_message);
    }
    sFontDrawScreenSpace_508BF4 = 1;

    auto screenLeft = field_2BC_tlv.field_10_top_left.field_0_x - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);
    auto screenRight = field_2BC_tlv.field_14_bottom_right.field_0_x - FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);

    const char *slicedText = field_60_font.SliceText_41C6C0(
        field_A0_message,
        PsxToPCX(screenLeft - pScreenManager_4FF7C8->field_14_xpos, 11) - field_2B0_x_offset,
        FP_FromInteger(1),
        screenRight - pScreenManager_4FF7C8->field_14_xpos
    );
    sFontDrawScreenSpace_508BF4 = 0;
    if (slicedText != field_A4_message_cutoff_ptr)
    {
        field_A4_message_cutoff_ptr = slicedText;
        if (*slicedText != ' ')
        {
            SFX_Play_43AD70(SoundEffect::LCDScreen_87, 0, 0);
        }
    }
}

void LCDScreen::VRender(int** ppOt)
{
    VRender_434400(ppOt);
}

void LCDScreen::VRender_434400(int** ppOt)
{
    if (sNumCamSwappers_507668 == 0)
    {
        const FP_Point* camPos = pScreenManager_4FF7C8->field_10_pCamPos;

        auto endY = field_2BC_tlv.field_10_top_left.field_2_y + field_2BC_tlv.field_14_bottom_right.field_2_y;
        auto endX = pScreenManager_4FF7C8->field_14_xpos + field_2BC_tlv.field_14_bottom_right.field_0_x;

        const int screenX = field_2BC_tlv.field_10_top_left.field_0_x - FP_GetExponent(camPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));
        const int screenY = endY / 2 - FP_GetExponent(camPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos)) - 7;
        const int maxWidth = FP_GetExponent(FP_FromInteger(endX) - camPos->field_0_x);

        PSX_RECT clipRect = {
            0,
            0,
            640,
            static_cast<short>(gPsxDisplay_504C78.field_2_height)
        };

        auto* pClippers = &field_10_prim_clippers[0][gPsxDisplay_504C78.field_A_buffer_index];
        Init_PrimClipper_495FD0(
            pClippers,
            &clipRect
        );
        OrderingTable_Add_498A80(&ppOt[22], &pClippers->mBase);

        auto fontFlickerAmount = 50;
        if (sDisableFontFlicker_5080E4)
        {
            fontFlickerAmount = 0;
        }
        if (fontFlickerAmount)
        {
            fontFlickerAmount = 40;
        }

        sFontDrawScreenSpace_508BF4 = 1;
        field_60_font.DrawString_41C360(
            ppOt,
            field_A0_message,
            static_cast<short>(PsxToPCX(screenX, 11) - field_2B0_x_offset),
            static_cast<short>(screenY),
            1,
            1,
            0,
            22,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            maxWidth,
            fontFlickerAmount
        );
        sFontDrawScreenSpace_508BF4 = 0;

        PSX_RECT clipRect2 = {};

        clipRect2.x = static_cast<short>(PsxToPCX(screenX, 11));
        clipRect2.y = static_cast<short>(screenY - 12);
        clipRect2.w = static_cast<short>(PsxToPCX(maxWidth - screenX, 51));
        clipRect2.h = 48;

        auto* clipper = &field_10_prim_clippers[1][gPsxDisplay_504C78.field_A_buffer_index];
        Init_PrimClipper_495FD0(clipper, &clipRect2);
        OrderingTable_Add_498A80(&ppOt[22], &clipper->mBase);

        pScreenManager_4FF7C8->InvalidateRect_406E40(
            clipRect2.x,
            clipRect2.y,
            clipRect2.w,
            24,
            pScreenManager_4FF7C8->field_2E_idx
        );
    }
}

}
