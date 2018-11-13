#include "stdafx.h"
#include "StatsSign.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "DebugHelpers.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "MainMenu.hpp"
#include "Map.hpp"

void StatsSign_ForceLink()
{

}

unsigned char sStatsSignFontPalette_55CF8C[] =
{
    0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
    0x20, 0x84, 0x21, 0x84, 0x65, 0xCE, 0x65, 0x8C, 0x8C, 0xB1,
    0x13, 0x94, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
    0x18, 0xD8
};

__int16 sMudsInLevelCount_55CFAC[15] =
{ 0, 75, 10, 5, 14, 26, 49, 14, 31, 90, 90, 5, 26, 49, 31 };


ALIVE_VAR(1, 0x5C1BC4, __int16, sMudokonsInArea_5C1BC4, 0);
ALIVE_VAR(1, 0x5C1A20, char, sStatsSignCurrentArea_5C1A20, 0);


void StatsSign::VDestructor(signed int flags)
{
    vdtor_47B7B0(flags);
}

void StatsSign::VUpdate()
{
    vUpdate_47B8D0();
}

void StatsSign::VRender(int** pOrderingTable)
{
    vRender_47B900(pOrderingTable);
}

void StatsSign::VScreenChanged()
{
    vScreenChanged_47BC40();
}

StatsSign * StatsSign::ctor_47B600(Path_StatsSign * params, TlvItemInfoUnion a3)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x54635C);

    field_100_objectId = a3.all;

    if (!sFontType2LoadCount_5BC5E8)
    {
        sFont2Context_5BC5D8.LoadFontType_433400(2);
    }

    ++sFontType2LoadCount_5BC5E8;

    field_20_font1.ctor_433590(3, sStatsSignFontPalette_55CF8C, &sFont2Context_5BC5D8);
    field_58_font2.ctor_433590(3, sStatsSignFontPalette_55CF8C, &sFont2Context_5BC5D8);
    field_90_font3.ctor_433590(3, sStatsSignFontPalette_55CF8C, &sFont2Context_5BC5D8);
    field_C8_font4.ctor_433590(3, sStatsSignFontPalette_55CF8C, &sFont2Context_5BC5D8);

    field_6_flags.Set(eDrawable);
    gObjList_drawables_5C1124->Push_Back_40CAF0(this);
    field_104_position_x = (FP(static_cast<int>(params->field_0_mBase.field_8_top_left.field_0_x)) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x).GetExponent();
    field_106_position_y = (FP(static_cast<int>(params->field_0_mBase.field_8_top_left.field_2_y)) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y).GetExponent();
    sMudokonsInArea_5C1BC4 = params->field_10_number_of_muds;
    field_108_is_hidden = static_cast<__int16 >(params->field_14_hidden);
    sStatsSignCurrentArea_5C1A20 = static_cast<char>(params->field_12_zulag_number);
    return this;
}

void StatsSign::dtor_47B7E0()
{
    SetVTable(this, 0x54635C);
    gObjList_drawables_5C1124->Remove_Item(this);
    Path::TLV_Reset_4DB8E0(field_100_objectId, -1, 0, 0);

    if (!--sFontType2LoadCount_5BC5E8)
    {
        sFont2Context_5BC5D8.dtor_433510();
    }

    field_C8_font4.dtor_433540();
    field_90_font3.dtor_433540();
    field_58_font2.dtor_433540();
    field_20_font1.dtor_433540();
    BaseGameObject_dtor_4DBEC0();
}


void StatsSign::vdtor_47B7B0(signed int flags)
{
    dtor_47B7E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void StatsSign::vUpdate_47B8D0()
{ 
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(eDead);
    }
}

// Todo: clean up
void StatsSign::vRender_47B900(int ** pOt)
{
    if (!field_108_is_hidden)
    {
        char text[12] = {};
        sprintf(text, "%3d", (unsigned __int16)sMudsInLevelCount_55CFAC[gMap_5C3030.sCurrentLevelId_5C3030]);
        int maxWidth = field_90_font3.MeasureWidth_433700(text);

        __int16 flickerAmount = 50; // ax
        if (sDisableFontFlicker_5C9304)
        {
            flickerAmount = 0;
        }

        // Muds In This Level
        field_90_font3.DrawString_4337D0(
            pOt,
            text,
            field_104_position_x - maxWidth + 33,
            field_106_position_y,
            1,
            1,
            0,
            22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + maxWidth,
            flickerAmount);
        const __int16 mudsLeftInArea = sMudokonsInArea_5C1BC4 - sSavedKilledMudsPerPath_5C1B50.mData[sStatsSignCurrentArea_5C1A20];
        field_10A_muds_left_in_area = mudsLeftInArea;

        // Muds in this Area
        sprintf(text, "%3d", mudsLeftInArea);
        const int font4Width = field_C8_font4.MeasureWidth_433700(text);
        field_C8_font4.DrawString_4337D0(
            pOt,
            text,
            field_104_position_x - font4Width + 33,
            field_106_position_y + 16,
            1,
            1,
            0,
            22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + font4Width,
            flickerAmount);
        maxWidth = max(font4Width, maxWidth);

        // Saved Mudokons
        sprintf(text, "%3d", sRescuedMudokons_5C1BC2);
        const int font2Width = field_58_font2.MeasureWidth_433700(text);
        field_58_font2.DrawString_4337D0(
            pOt,
            text,
            field_104_position_x - font2Width + 33,
            field_106_position_y + 32,
            1,
            1,
            0,
            22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + font2Width,
            flickerAmount);
        maxWidth = max(font2Width, maxWidth);

        // Killed mudokons
        sprintf(text, "%3d", sKilledMudokons_5C1BC0);
        const int font1Width = field_20_font1.MeasureWidth_433700(text);
        field_20_font1.DrawString_4337D0(
            pOt,
            text,
            field_104_position_x - font1Width + 33,
            field_106_position_y + 48,
            1,
            1,
            0,
            22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + font1Width,
            flickerAmount);
        maxWidth = max(font1Width, maxWidth);

        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            static_cast<int>(field_104_position_x / 0.575f),
            field_106_position_y,
            static_cast<int>(maxWidth / 0.575f),
            64,
            pScreenManager_5BB5F4->field_3A_idx);
    }
}

void StatsSign::vScreenChanged_47BC40()
{
    field_6_flags.Set(eDead);
}
