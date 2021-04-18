#include "stdafx.h"
#include "LCDScreen.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "Sound/Midi.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "Renderer/IRenderer.hpp"

unsigned char sLCDScreen_Palette[] =
{
    0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
    0x20, 0x84, 0x21, 0x84, 0x65, 0xCE, 0x65, 0x8C, 0x8C, 0xB1,
    0x60, 0x8E, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
    0x18, 0xD8
};

unsigned char sLCDScreen_Palette2[] =
{
    0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
    0x20, 0x84, 0x21, 0x84, 0x05, 0x84, 0x65, 0x8C, 0x8C, 0xB1,
    0x13, 0x94, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
    0x18, 0xD8
};




// TODO: Remove spaces and add them at runtime.
const char *sLCDMessageTable_555768[101] =
{
    "",
    "                               SoulStorm Mining Company is an equal opportunity employer.",
    "                               Work! Do it!",
    "                               You break it, you bought it.",
    "                               Drink cool, clear, depressing SoulStorm Brew.",
    "                               Depressed ... ? Then work harder!",
    "                               Need a (permanent) vacation?  Consult a Slig supervisor.",
    "                               Smuggling bones in your pants is grounds for immediate termination!",
    "                               Molluck's MouthLube --  Say it, don't spray it!",
    "                               FeeCo Food Extractors...Eat Twice...Feels Nice...One Price!",
    "                               SoulStorm Brew...Our spew is your brew!",
    "                               Scrab Cakes  MMMM MMMM  They'll cost ya an arm and a leg!",
    "                               New and improved SoulStorm Brew...Twice the flavor...twice the bones...twice the price!",
    "                               If yer blue, drink Brew!",
    "                               SoulStorm Brew: Just drink it!",
    "                               Feeco Depot: Where will we send you today?",
    "                               Wanted: Bullet catchers and grenade testers. Apply in person to General Dripik. Qualified recruits only.",
    "                               AzGasCo: Growing one fart at a time.",
    "                               Find the secret prize inside every specially marked package of RuptureFarms mystery meat!",
    "                               New from RuptureFarms: Weenie babies! Sausage has never been this much fun!",
    "                               Wanted: Mudokon Fleech flusher. No experience necessary.",
    "                               Vykkers Labs. Molding the future out of you.",
    "                               Phlegs Bonepaste...good to the last glop.",
    "                               Moolah Ticker...SolstrmBrw UP...FeeCo UP Bonwrkz UP...Rptfrms DOWN! DOWN! DOWN!",
    "                               Blowco Bonesaws. Perfect powder - every time.",
    "                               Keep yer employees in the dark with the all- new Eyestitch 2000. The ultimate blinding machine",
    "                               FeeCo FatVats. When you got serious fat, you need a serious vat.",
    "                               Vykkers Lab reports SoulStorm Brew totally safe! Dont think, drink!",
    "                               Legz Iz Uz : if you can find a better leg, buy it!",
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
    "                               To enter the door, stand in front of it and press " kUp ".",
    "                               To enter the well, stand in front of it and press " kUp ".",
    "                               To hoist, stand under the falling debris and press " kUp " (up arrow).",
    "                               To hop, press " kJump ".",
    "                               To sneak past the sleeping Slig, hold " kSneak " while you move.",
    "                               To pull the lever, stand next to it and press " kAction ".",
    "                               To use the ring, stand under it, and press " kUp ", just like a hoist.",
    "                               To ride the platform, stand in the center of it, and press " kUp " (up).",
    "                               To enter the well, stand in front of it and press " kUp ".",
    "                               To deactivate a bomb, crouch next to it by pressing" kDown ".   Then carefully press " kAction " when the bomb blinks green.  Press " kAction " again to reactivate it.",
    "                               Sneak to shadows by holding " kSneak " while you move.   Stand still in shadows while Slig is facing you, or you will be seen.  Sneak behind Slig when it walks away from you.",
    "                               To get a rock from the rock sack, stand under it and press " kUp ".   To pick up a rock, crouch next to it and press " kAction ".   To throw the rock, hold " kThrow ", then press any directional button.   Different directions will throw at different angles.",
    "                               Practice throwing here. You can stand on the arrow and hit every mine.   It is possible to throw while crouching.",
    "                               To alert a Mudokon, say hello by pressing " kHello ".   Then, talk to Mudokons by pressing " kHello ", " kFollowMe ", " kWait ", " kWork ", " kAnger ", " kAllYa ", " kSorry " or " kStopIt ".   Experiment!",
    "                               Possess Sligs by chanting.   Chant by holding " kChant ".",
    "                               To help Mudokons escape from slavery chant when you see a circle of birds.   To chant, hold " kChant ".",
    "                               To roll, first crouch by pressing " kDown ".   Then roll left or right by pressing " kLeft " or " kRight ".",
    "                               To walk, press " kLeft " (left) or " kRight " (right). To run, hold " kRun " while you move.",
    "                               To avoid the alarm, stand still when the Motion Detectors touch you.",
    "                               Use Slig to talk your way past the security door.",
    "                               Sligs can talk to Slogs.",
    "                               Use lever to operate bone saw.",
    "                               Lead Mudokons to bird portals, then chant to rescue them.  Chant by holding " kChant ".",
    "                               To turn the wheel, stand in front of it and press " kUp ".",
    "                               After saying hello to the Mudokon, tell it you are sorry by standing next to it and pressing " kSorry ".",
    "                               Execute a roll while running by pressing " kFart " while you run.",
    "                               Stand on top of the arrow and press " kDown " (down arrow)",
    "                               Sligs ahead ... be quiet.",
    "                               Laughing gas makes Mudokons slap-happy.",
    "                               The only way to make a Mudokon calm down is to slap it.   To slap a Mudokon, stand next to it and press " kAnger ".",
    "                               Sneak behind the Slig as it walks away from you.",
    "                               You will need help to turn all the wheels at once.   Position a Mudokon in front of a wheel, then tell it to work by pressing " kWork ".   You may need to slap the Mudokon to get it to calm down.",
    "                               If you sneak, any Mudokons following you will also sneak.",
    "                               Say hello to all the Mudokons at once by pressing " kAllYa ".   Then tell them to follow you by pressing " kFollowMe ".",
    "                               Try bouncing rocks off the wall.",
    "                               Blind Mudokons keep walking until you tell them to wait. Tell them to wait by pressing " kWait ".",
    "                               Only a fool or a murderer would pull this lever.",
    "                               You can make a possessed Slig blast stuff in the foreground by pressing " kThrow " while holding " kDown ".",
    "                               Press " kJump " to hop.",
    "                               To enter the mine car, stand in front of it and press " kUp ". To exit the mine car, press " kAction ". To make the mine car drop from a ceiling, press " kAction ".",
    "",
    "",
    "",
    "",
    "",
    "",
    "                               To run, hold " kRun " while you move.",
    "                               If you are silent and still in shadows, you cannot be seen.",
    "",
    "",
    "",
    "",
};

LCDScreen * LCDScreen::ctor_460680(Path_LCDScreen* params, TlvItemInfoUnion itemInfo)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x545AAC);

    field_C_objectId = itemInfo.all;

    field_2C0_tlv = *params;

    field_2AA_message_1_id = params->field_10_message_1_id;
    field_2B0_message_2_id = params->field_16_message_2_id;
    field_2B2_toggle_message_switch_id = static_cast<WORD>(params->field_18_toggle_message_switch_id);
    field_2BC_tlv_item_info = itemInfo;

    if (!sFontType2LoadCount_5BC5E8)
    {
        sFont2Context_5BC5D8.LoadFontType_433400(2);
    }
    sFontType2LoadCount_5BC5E8++;

    field_60_font.ctor_433590(60, sLCDScreen_Palette, &sFont2Context_5BC5D8);

    IRenderer::PalRecord rec;
    rec.depth = 16;
    if (!IRenderer::GetRenderer()->PalAlloc(rec))
    {
        LOG_ERROR("PalAlloc failed");
    }

    field_98_pal_rect.x = rec.x;
    field_98_pal_rect.y = rec.y;
    field_98_pal_rect.w = rec.depth;
    field_98_pal_rect.h = 1;

    IRenderer::GetRenderer()->PalSetData(rec, sLCDScreen_Palette2);

    if (SwitchStates_Get_466020(field_2B2_toggle_message_switch_id))
    {
        field_A0_message = sLCDMessageTable_555768[field_2B0_message_2_id];
    }
    else
    {
        field_A0_message = sLCDMessageTable_555768[field_2AA_message_1_id];
    }

    //field_A0_message = "                               THIS IS A TEST";

    String_FormatString_4969D0(field_A0_message, field_A8_message_buffer, 512, 1);
    field_A0_message = field_A8_message_buffer;
    field_A4_message_cutoff_ptr = nullptr;
    field_2AC_x_offset = 0;
    sFontDrawScreenSpace_5CA4B4 = 1;
    field_2AE_character_width = static_cast<WORD>(field_60_font.MeasureWidth_433630(*field_A0_message) + 2);
    sFontDrawScreenSpace_5CA4B4 = 0;
    field_2B4 = 0;
    field_2B6_message_rand_min_id = params->field_12_message_rand_min_id;
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
    field_2B8_message_rand_max_id = params->field_14_message_rand_max_id;
    field_2A8_play_sound_toggle = 0;
    gObjList_drawables_5C1124->Push_Back_40CAF0(this);

    return this;
}

void LCDScreen::Update_460A00()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

#if LCD_PS1_SPEED
    field_2AC_x_offset += 6;
#else
    field_2AC_x_offset += 3;
#endif

    if (field_2AC_x_offset > field_2AE_character_width)
    {
        field_2AC_x_offset -= field_2AE_character_width;
        char lastChar = *field_A0_message;
        field_A0_message++; // Offset char index

        if (lastChar == 0)
        {
            if (++field_2B4 == 1)
            {
                field_A0_message = sLCDMessageTable_555768[Math_RandomRange_496AB0(field_2B6_message_rand_min_id, field_2B8_message_rand_max_id)];
            }
            else
            {
                field_2B4 = 0;
                if (SwitchStates_Get_466020(field_2B2_toggle_message_switch_id))
                {
                    field_A0_message = sLCDMessageTable_555768[field_2B0_message_2_id];
                }
                else
                {
                    field_A0_message = sLCDMessageTable_555768[field_2AA_message_1_id];
                }
            }

            String_FormatString_4969D0(field_A0_message, field_A8_message_buffer, 512, 1);
            field_A0_message = field_A8_message_buffer;

            auto palSwap = field_98_pal_rect;
            field_98_pal_rect = field_60_font.field_28_palette_rect;
            field_60_font.field_28_palette_rect = palSwap;
        }

        sFontDrawScreenSpace_5CA4B4 = 1;
        field_2AE_character_width = static_cast<WORD>(field_60_font.MeasureWidth_433630(*field_A0_message) + 2);
        sFontDrawScreenSpace_5CA4B4 = 0;
    }

    auto screenLeft = field_2C0_tlv.field_8_top_left.field_0_x - FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    auto screenRight = field_2C0_tlv.field_C_bottom_right.field_0_x - FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);

    sFontDrawScreenSpace_5CA4B4 = 1;
    auto slicedText = field_60_font.SliceText_433BD0(
        field_A0_message,
        PCToPsxX(screenLeft) - field_2AC_x_offset,
        FP_FromInteger(1),
        screenRight
    );
    sFontDrawScreenSpace_5CA4B4 = 0;
    if (slicedText != field_A4_message_cutoff_ptr)
    {
        field_2A8_play_sound_toggle = !field_2A8_play_sound_toggle;
        field_A4_message_cutoff_ptr = slicedText;
        if (*slicedText != ' ')
        {
            if (field_2A8_play_sound_toggle)
            {
                SFX_Play_46FA90(SoundEffect::LCDScreen_73, 0);
            }
        }
    }
}

void LCDScreen::Render_460CB0(PrimHeader** ppOt)
{
    if (sNum_CamSwappers_5C1B66 == 0)
    {
        const FP_Point* camPos = pScreenManager_5BB5F4->field_20_pCamPos;
        const int screenX = field_2C0_tlv.field_8_top_left.field_0_x - FP_GetExponent(camPos->field_0_x);
        const int screenY = ((field_2C0_tlv.field_8_top_left.field_2_y + field_2C0_tlv.field_C_bottom_right.field_2_y) / 2 - FP_GetExponent(camPos->field_4_y)) - 7;
        const int screenXWorld = PsxToPCX(screenX);
        const int maxWidth = field_2C0_tlv.field_C_bottom_right.field_0_x - FP_GetExponent(camPos->field_0_x);

        PSX_RECT clipRect = {
            0,
            0,
            640,
            240
        };

        Init_PrimClipper_4F5B80(&field_20_prim_clippers[0][gPsxDisplay_5C1130.field_C_buffer_index], &clipRect);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, Layer::eLayer_24), &field_20_prim_clippers[0][gPsxDisplay_5C1130.field_C_buffer_index].mBase);

        sFontDrawScreenSpace_5CA4B4 = 1;
        field_60_font.DrawString_4337D0(
            ppOt,
            field_A0_message,
            static_cast<short>(screenXWorld - field_2AC_x_offset),
            static_cast<short>(screenY),
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_24,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            maxWidth,
            sDisableFontFlicker_5C9304 != 0 ? 0 : 40);
        sFontDrawScreenSpace_5CA4B4 = 0;

        clipRect = {
            static_cast<short>(screenXWorld),
            static_cast<short>(screenY - 12),
            static_cast<short>(PsxToPCX(maxWidth - screenX)),
            48
        };

        auto* clipper = &field_20_prim_clippers[1][gPsxDisplay_5C1130.field_C_buffer_index];
        Init_PrimClipper_4F5B80(clipper, &clipRect);
        OrderingTable_Add_4F8AA0(OtLayer(ppOt, Layer::eLayer_24), &clipper->mBase);

        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            screenXWorld,
            screenY,
            clipRect.w,
            24,
            pScreenManager_5BB5F4->field_3A_idx
        );
    }
}

void LCDScreen::vSetDead_460F10()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void LCDScreen::dtor_460920()
{
    SetVTable(this, 0x545AAC); // vTbl_LCDScreen_545AAC

    IRenderer::GetRenderer()->PalFree(IRenderer::PalRecord{ field_98_pal_rect.x, field_98_pal_rect.y, field_98_pal_rect.w });

    gObjList_drawables_5C1124->Remove_Item(this);
    Path::TLV_Reset_4DB8E0(field_2BC_tlv_item_info.all, -1, 0, 0);

    if (!--sFontType2LoadCount_5BC5E8)
    {
        sFont2Context_5BC5D8.dtor_433510();
    }
    field_60_font.dtor_433540();
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* LCDScreen::vdtor_4608F0(signed int flags)
{
    dtor_460920();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void LCDScreen::VUpdate()
{
    Update_460A00();
}

void LCDScreen::VRender(PrimHeader** ppOt)
{
    Render_460CB0(ppOt);
}

BaseGameObject* LCDScreen::VDestructor(signed int flags)
{
    return vdtor_4608F0(flags);
}

void LCDScreen::VScreenChanged()
{
    vSetDead_460F10();
}
