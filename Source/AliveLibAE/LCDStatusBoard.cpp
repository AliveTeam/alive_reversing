#include "stdafx.h"
#include "LCDStatusBoard.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "DebugHelpers.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "MainMenu.hpp"
#include "Map.hpp"
#include "Sys.hpp"

ALIVE_VAR(1, 0x5C1BC4, s16, sMudokonsInArea_5C1BC4, 0);
ALIVE_VAR(1, 0x5C1A20, s8, sZulagNumber_5C1A20, 0);

LCDStatusBoard::LCDStatusBoard(relive::Path_LCDStatusBoard* params, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    field_100_objectId = tlvId;

    mFontContext.LoadFontType_433400(FontType::LcdFont);

    
    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_StatusBoard);

    field_20_font1.ctor_433590(3, mPal, &mFontContext);
    field_58_font2.ctor_433590(3, mPal, &mFontContext);
    field_90_font3.ctor_433590(3, mPal, &mFontContext);
    field_C8_font4.ctor_433590(3, mPal, &mFontContext);

    mBaseGameObjectFlags.Set(eDrawable_Bit4);
    gObjListDrawables->Push_Back(this);
    field_104_position_x = FP_GetExponent(FP_FromInteger(static_cast<s32>(params->mTopLeftX)) - pScreenManager->CamXPos());
    field_106_position_y = FP_GetExponent(FP_FromInteger(static_cast<s32>(params->mTopLeftY)) - pScreenManager->CamYPos());
    sMudokonsInArea_5C1BC4 = params->mNumberOfMuds;
    field_108_is_hidden = static_cast<s16>(params->mHideBoard);
    sZulagNumber_5C1A20 = static_cast<s8>(params->mZulagNumber);
    if (sZulagNumber_5C1A20 > ALIVE_COUNTOF(sSavedKilledMudsPerZulag_5C1B50.mData))
    {
        LOG_ERROR("sZulagNumber_5C1A20 is " << sZulagNumber_5C1A20 << " max is 20");
        ALIVE_FATAL("sZulagNumber_5C1A20 out of bounds, don't set your zulag numbe to > 20");
    }
}

LCDStatusBoard::~LCDStatusBoard()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(field_100_objectId, -1, 0, 0);

    mFontContext.dtor_433510();

    field_C8_font4.dtor_433540();
    field_90_font3.dtor_433540();
    field_58_font2.dtor_433540();
    field_20_font1.dtor_433540();
}

void LCDStatusBoard::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(eDead);
    }
}

// Todo: clean up
void LCDStatusBoard::VRender(PrimHeader** ppOt)
{
    if (!field_108_is_hidden)
    {
        char_type text[12] = {};
        sprintf(text, "%3d", Path_GetMudsInLevel(gMap.mCurrentLevel, gMap.mCurrentPath));
        s32 maxWidth = field_90_font3.MeasureTextWidth(text);

        s16 flickerAmount = 50; // ax
        if (sDisableFontFlicker_5C9304)
        {
            flickerAmount = 0;
        }

        // Muds In This Level
        field_90_font3.DrawString_4337D0(
            ppOt,
            text,
            field_104_position_x - maxWidth + 33,
            field_106_position_y,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + maxWidth,
            flickerAmount);
        const s16 mudsLeftInArea = sMudokonsInArea_5C1BC4 - sSavedKilledMudsPerZulag_5C1B50.mData[sZulagNumber_5C1A20];
        field_10A_muds_left_in_area = mudsLeftInArea;

        // Muds in this Area
        sprintf(text, "%3d", mudsLeftInArea);
        const s32 font4Width = field_C8_font4.MeasureTextWidth(text);
        field_C8_font4.DrawString_4337D0(
            ppOt,
            text,
            field_104_position_x - font4Width + 33,
            field_106_position_y + 16,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + font4Width,
            flickerAmount);
        maxWidth = std::max(font4Width, maxWidth);

        // Saved Mudokons
        sprintf(text, "%3d", sRescuedMudokons_5C1BC2);
        const s32 font2Width = field_58_font2.MeasureTextWidth(text);
        field_58_font2.DrawString_4337D0(
            ppOt,
            text,
            field_104_position_x - font2Width + 33,
            field_106_position_y + 32,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + font2Width,
            flickerAmount);
        maxWidth = std::max(font2Width, maxWidth);

        // Killed mudokons
        sprintf(text, "%3d", sKilledMudokons_5C1BC0);
        const s32 font1Width = field_20_font1.MeasureTextWidth(text);
        field_20_font1.DrawString_4337D0(
            ppOt,
            text,
            field_104_position_x - font1Width + 33,
            field_106_position_y + 48,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromDouble(1.0),
            field_104_position_x + font1Width,
            flickerAmount);
        maxWidth = std::max(font1Width, maxWidth);
    }
}

void LCDStatusBoard::VScreenChanged()
{
    mBaseGameObjectFlags.Set(eDead);
}
