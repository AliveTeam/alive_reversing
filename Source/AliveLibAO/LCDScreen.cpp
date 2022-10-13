#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "LCDScreen.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "StringFormatters.hpp"
#include "Primitives_common.hpp"
#include "Input.hpp"
#include "../AliveLibAE/Renderer/IRenderer.hpp"
#include "../AliveLibCommon/PathDataExtensionsTypes.hpp"
#include "PathData.hpp"
#include "Path.hpp"

namespace AO {

static const char_type* sLCDMessageTable_4C7420[90] = {
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
    "                               To hoist, stand under the ledge and press " kAO_Up " (up).",
    "                               To jump, press " kAO_Jump_Or_Hello ".   To run and jump, press " kAO_Jump_Or_Hello " while you run using " kAO_Run ".",
    "                               To sneak past the sleeping Slig, hold " kAO_Sneak " while you move.",
    "                               To pull the lever, stand next to it and press " kAO_Action ".",
    "                               To use the ring, stand under it, and press " kAO_Up ", just like a hoist.",
    "                               To ride the platform, stand in the center of it, and press " kAO_Down " (down).",
    "                               To enter the door, stand in front of it and press " kAO_Up ".",
    "                               To deactivate a bomb, crouch next to it by pressing " kAO_Down ".   Then carefully press " kAO_Action "  when the bomb blinks green.  Press " kAO_Action " again to reactivate it.",
    "                               Sneak to shadows by holding " kAO_Sneak " while you move.   Stand still in shadows while Slig is facing you, or you will be seen.  Sneak behind Slig when it walks away from you.",
    "                               To get a grenade from the Boom Machine, stand in front of it and press " kAO_Up ".   To pick up a grenade, crouch next to it and press " kAO_Action ".   To throw the grenade, hold " kAO_Throw ", then press any directional button.   Different directions will throw at different angles.",
    "                               It is possible to throw while crouching.",
    "                               To alert a Mudokon, mSay hello by holding " kAO_Speak1 " and pressing " kAO_Jump_Or_Hello ".   Then, talk to Mudokons by holding " kAO_Speak1 " or " kAO_Speak2 " and pressing the " kAO_Jump_Or_Hello ", " kAO_Action ", " kAO_Throw ", or " kAO_Crouch " button.   Experiment!",
    "                               Possess Sligs by chanting.   Chant by holding " kAO_Speak1 " and " kAO_Speak2 " at the same time or pressing (0) (zero) on the keyboard.",
    "                               To help Mudokons escape from slavery in Rupture Farms, chant when you see a circle of birds.   To chant, hold " kAO_Speak1 " and " kAO_Speak2 " at the same time or pressing (0) on the keyboard.",
    "                               To roll, first crouch by pressing " kAO_Down ".   Then roll left or right by pressing " kAO_Left " or " kAO_Right ".",
    "                               To run, hold " kAO_Run " while you press " kAO_Left " (left) or " kAO_Right " (right).",
    "                               To avoid the alarm, stand still when the Motion Detectors touch you.",
    "                               Use Slig to talk your way past the security door.",
    "                               Sligs can talk to Slogs.",
    "                               Use lever to operate meat saw.",
    "                               Lead Mudokons to bird portals, then chant to rescue them.  Chant by holding " kAO_Speak1 " and " kAO_Speak2 ".",
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
    ""};

static const StringTable* sPerLvlMessages[static_cast<u32>(LevelIds::eDesertEscape_15) + 1][99] = {};

void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId)
{
    sPerLvlMessages[static_cast<u32>(lvl)][pathId] = &msgs;
}

class LCDMessages final
{
public:
    const char_type* GetMessage(EReliveLevelIds lvlId, u32 pathId, u32 msgId) const
    {
        const StringTable* pTable = sPerLvlMessages[static_cast<u32>(MapWrapper::ToAO(lvlId))][pathId];
        if (pTable && pTable->mStringCount > 0)
        {
            if (msgId < pTable->mStringCount)
            {
                return pTable->mStrings[msgId].string_ptr;
            }
            else
            {
                LOG_WARNING("LCD message out of bounds, using first message for id: " << msgId);
                return pTable->mStrings[0].string_ptr;
            }
        }

        if (msgId < ALIVE_COUNTOF(sLCDMessageTable_4C7420))
        {
            return sLCDMessageTable_4C7420[msgId];
        }
        else
        {
            LOG_WARNING("LCD message out of bounds using original message table id: " << msgId);
            return sLCDMessageTable_4C7420[0];
        }
    }
};
static LCDMessages gLCDMessages;

LCDScreen::LCDScreen(relive::Path_LCDScreen* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    field_2BC_tlv = pTlv;

    field_2B8_tlv_item_info = tlvId;

    field_2AC_message_1_id = pTlv->mMessageId1;

    field_50_font_context.LoadFontType(FontType::LcdFont);

    mPal1 = ResourceManagerWrapper::LoadPal(PalId::LedFont_1);
    mPal2 = ResourceManagerWrapper::LoadPal(PalId::LedFont_2);

    field_60_font.Load(60, mPal1, &field_50_font_context);

    IRenderer::PalRecord rec;
    rec.depth = 16;
    if (!IRenderer::GetRenderer()->PalAlloc(rec))
    {
        LOG_ERROR("PalAlloc failure");
    }

    field_98_pal_rect.x = rec.x;
    field_98_pal_rect.y = rec.y;
    field_98_pal_rect.w = rec.depth;
    field_98_pal_rect.h = 1;

    //IRenderer::GetRenderer()->PalSetData(rec, sLCDScreen_Palette2_4C7588);

    if (Input_JoyStickEnabled() || field_2AC_message_1_id != 62)
    {
        String_FormatString(gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, field_2AC_message_1_id), field_AC_message_buffer);
    }
    else
    {
        strcpy(field_AC_message_buffer,
               "                               To alert a Mudokon, mSay hello by holding (1) on the keyboard.   Then, talk to"
               " Mudokons by using the keyboard numbers (1) thru (8).   Experiment!");
    }

    field_A0_message = field_AC_message_buffer;
    field_A4_message_cutoff_ptr = 0;
    field_2B0_x_offset = 0;

    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    field_2D8_message_rand_min = pTlv->mMessageRandMinId;
    field_2DC_message_rand_max = pTlv->mMessageRandMaxId;

    sFontDrawScreenSpace_508BF4 = 1;
    field_2B4_character_width = field_60_font.MeasureCharacterWidth(field_AC_message_buffer[0]);
    sFontDrawScreenSpace_508BF4 = 0;

    field_2D4 = 0;
    gObjListDrawables->Push_Back(this);
}

LCDScreen::~LCDScreen()
{
    IRenderer::GetRenderer()->PalFree(IRenderer::PalRecord{field_98_pal_rect.x, field_98_pal_rect.y, field_98_pal_rect.w});

    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(field_2B8_tlv_item_info, -1, 0, 0);
}

void LCDScreen::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void LCDScreen::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

#if LCD_PS1_SPEED
    field_2B0_x_offset += 6;
#else
    field_2B0_x_offset += 3;
#endif

    if (field_2B0_x_offset > field_2B4_character_width)
    {
        field_2B0_x_offset -= field_2B4_character_width;
        char_type* pMsg = field_A0_message;
        field_A0_message++;
        if (!*pMsg)
        {
            field_A0_message = field_AC_message_buffer;
            field_2D4++;
            if (field_2D4 == 1)
            {
                const auto rangedRandom = Math_RandomRange(
                    field_2D8_message_rand_min,
                    field_2DC_message_rand_max);

                if (Input_JoyStickEnabled() || rangedRandom != 62)
                {
                    String_FormatString(gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, rangedRandom), field_AC_message_buffer);
                }
                else
                {
                    strcpy(
                        field_AC_message_buffer,
                        "                               To alert a Mudokon, mSay hello by holding (1) on the keyboard.   Then, talk to Mudokons by using the keyboard numbers (1) thru (8).   Experiment!");
                }

                 // Change pal
                field_60_font.field_34_font_context->field_C_resource_id.mCurPal = mPal2.mPal;
            }
            else
            {
                field_2D4 = 0;

                if (Input_JoyStickEnabled() || field_2AC_message_1_id != 62)
                {
                    String_FormatString(
                        gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, field_2AC_message_1_id),
                        field_AC_message_buffer);
                }
                else
                {
                    strcpy(
                        field_AC_message_buffer,
                        "                               To alert a Mudokon, mSay hello by holding (1) on the keyboard.   Then, talk to Mudokons by using the keyboard numbers (1) thru (8).   Experiment!");
                }

                // Change pal
                field_60_font.field_34_font_context->field_C_resource_id.mCurPal = mPal1.mPal;
            }

            // TODO
            //auto palSwap = field_98_pal_rect;
            //field_98_pal_rect = field_60_font.field_28_palette_rect;
            //field_60_font.field_28_palette_rect = palSwap;
        }
        sFontDrawScreenSpace_508BF4 = 1;
        field_2B4_character_width = field_60_font.MeasureCharacterWidth(*field_A0_message);
    }
    sFontDrawScreenSpace_508BF4 = 1;

    auto screenLeft = field_2BC_tlv->mTopLeftX - FP_GetExponent(pScreenManager->mCamPos->x);
    auto screenRight = field_2BC_tlv->mBottomRightX - FP_GetExponent(pScreenManager->mCamPos->x);

    const char_type* slicedText = field_60_font.SliceText(
        field_A0_message,
        PsxToPCX(screenLeft - pScreenManager->mCamXOff, 11) - field_2B0_x_offset,
        FP_FromInteger(1),
        screenRight - pScreenManager->mCamXOff);
    sFontDrawScreenSpace_508BF4 = 0;
    if (slicedText != field_A4_message_cutoff_ptr)
    {
        field_A4_message_cutoff_ptr = slicedText;
        if (*slicedText != ' ')
        {
            SfxPlayMono(relive::SoundEffects::LCDScreen, 0);
        }
    }
}

void LCDScreen::VRender(PrimHeader** ppOt)
{
    if (gNumCamSwappers == 0)
    {
        const FP_Point* camPos = pScreenManager->mCamPos;

        auto endY = field_2BC_tlv->mTopLeftY + field_2BC_tlv->mBottomRightY;
        auto endX = pScreenManager->mCamXOff + field_2BC_tlv->mBottomRightX;

        const s32 screenX = field_2BC_tlv->mTopLeftX - FP_GetExponent(camPos->x - FP_FromInteger(pScreenManager->mCamXOff));
        const s32 screenY = endY / 2 - FP_GetExponent(camPos->y - FP_FromInteger(pScreenManager->mCamYOff)) - 7;
        const s32 maxWidth = FP_GetExponent(FP_FromInteger(endX) - camPos->x);

        PSX_RECT clipRect = {
            0,
            0,
            640,
            static_cast<s16>(gPsxDisplay.mHeight)};

        auto* pClippers = &field_10_prim_clippers[0][gPsxDisplay.mBufferIndex];
        Init_PrimClipper(
            pClippers,
            &clipRect);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_BeforeWell_22), &pClippers->mBase);

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
        field_60_font.DrawString(
            ppOt,
            field_A0_message,
            static_cast<s16>(PsxToPCX(screenX, 11) - field_2B0_x_offset),
            static_cast<s16>(screenY),
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            maxWidth,
            fontFlickerAmount);
        sFontDrawScreenSpace_508BF4 = 0;

        PSX_RECT clipRect2 = {};

        clipRect2.x = static_cast<s16>(PsxToPCX(screenX, 11));
        clipRect2.y = static_cast<s16>(screenY - 12);
        clipRect2.w = static_cast<s16>(PsxToPCX(maxWidth - screenX, 51));
        clipRect2.h = 48;

        auto* clipper = &field_10_prim_clippers[1][gPsxDisplay.mBufferIndex];
        Init_PrimClipper(clipper, &clipRect2);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_BeforeWell_22), &clipper->mBase);
    }
}

} // namespace AO
