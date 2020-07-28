#include "stdafx_ao.h"
#include "Function.hpp"
#include "LCDScreen.hpp"
#include "VRam.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "StringFormatters.hpp"

START_NS_AO

const BYTE gLCDFontPal_4C75A8[32] =
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

const BYTE word_4C7588[16] =
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
    132u
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
    "",
    "",
    "                               To sneak past the sleeping Slig, hold \v while you move.",
    "                               To pull the lever, stand next to it and press \r.",
    "",
    "",
    "",
    "",
    "                               Sneak to shadows by holding \v while you move.   Stand still in shadows while Slig is facing you, or you will be seen.  Sneak behind Slig hen t  walks away from you.",
    "",
    "                               It is possible to throw while crouching.",
    "",
    "                               Possess Sligs by chanting.   Chant by holding \b and \t at the same time or pressing (0) (zero) on the keyboard.",
    "                               To help Mudokons escape from slavery in Rupture Farms, chant when you see a circle of birds.   To chant, hold \b and \t at the same time or     essing (0) on the keyboard.",
    "",
    "",
    "                               To avoid the alarm, stand still when the Motion Detectors touch you.",
    "                               Use Slig to talk your way past the security door.",
    "                               Sligs can talk to Slogs.",
    "                               Use lever to operate meat saw.",
    "                               Lead Mudokons to bird portals, then chant to rescue them.  Chant by holding \b and \t.",
    "",
    "",
    "",
    "",
    "                               You're almost free, Abe!   You must get through the stockyards.   Good luck ...  and watch out for those Scrabs!   They haven't been fed in     hile.",
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


ALIVE_VAR(1, 0x508A60, int, dword_508A60, 0);

LCDScreen* LCDScreen::ctor_433F60(Path_LCDScreen* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB468);

    field_2BC_tlv = *pTlv;

    field_2B8_tlv_item_info = tlvInfo;

    field_2AC_message_1_id = pTlv->field_18_message_1_id;

    field_50_font_context.LoadFontType_41C040(2);
    field_60_font.ctor_41C170(60, gLCDFontPal_4C75A8, &field_50_font_context);
    Pal_Allocate_4476F0(&field_98_pal_rect, 16u);

    PSX_RECT rect = {};
    rect.x = field_98_pal_rect.x;
    rect.y = field_98_pal_rect.y;
    rect.w = 16;
    rect.h = 1;
    PSX_LoadImage16_4962A0(&rect, word_4C7588);

    if (dword_508A60 || field_2AC_message_1_id != 62)
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
    field_2DA = pTlv->field_1C_message_rand_max;

    sFontDrawScreenSpace_508BF4 = 1;
    field_2B4_character_width = field_60_font.MeasureWidth_41C200(field_AC_message_buffer[0]);
    sFontDrawScreenSpace_508BF4 = 0;

    field_2D4 = 0;
    field_2DC_message_rand_max = pTlv->field_1C_message_rand_max;
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

END_NS_AO
