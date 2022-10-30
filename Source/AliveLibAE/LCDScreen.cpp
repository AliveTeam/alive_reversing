#include "stdafx.h"
#include "LCDScreen.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "StringFormatters.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../AliveLibCommon/PathDataExtensionsTypes.hpp"
#include "Renderer/IRenderer.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "Input.hpp"

// TODO: Remove spaces and add them at runtime.
static const char_type* sLCDMessageTable_555768[101] = {
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
    "                               To alert a Mudokon, mSay hello by pressing " kHello ".   Then, talk to Mudokons by pressing " kHello ", " kFollowMe ", " kWait ", " kWork ", " kAnger ", " kAllYa ", " kSorry " or " kStopIt ".   Experiment!",
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


static const StringTable* sPerLvlMessages[static_cast<u32>(LevelIds::eCredits_16) + 1][99] = {};

void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId)
{
    sPerLvlMessages[static_cast<u32>(lvl)][pathId] = &msgs;
}

class LCDMessages final
{
public:
    const char_type* GetMessage(EReliveLevelIds lvlId, u32 pathId, u32 msgId) const
    {
        const StringTable* pTable = sPerLvlMessages[static_cast<u32>(MapWrapper::ToAE(lvlId))][pathId];
        if (pTable && pTable->mStringCount > 0)
        {
            if (msgId < pTable->mStringCount)
            {
                return pTable->mStrings[msgId].string_ptr;
            }
            else
            {
                LOG_WARNING("LCD message out of bounds, using first message for id: %d", msgId);
                return pTable->mStrings[0].string_ptr;
            }
        }

        if (msgId < ALIVE_COUNTOF(sLCDMessageTable_555768))
        {
            return sLCDMessageTable_555768[msgId];
        }
        else
        {
            LOG_WARNING("LCD message out of bounds using original message table id: %d", msgId);
            return sLCDMessageTable_555768[0];
        }
    }
};
static LCDMessages gLCDMessages;

LCDScreen::LCDScreen(relive::Path_LCDScreen* params, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mBaseGameObjectTlvInfo = tlvId;

    field_2C0_tlv = *params;

    field_2AA_message_1_id = params->mMessageId1;
    field_2B0_message_2_id = params->mMessageId2;
    field_2B2_toggle_message_switch_id = static_cast<u16>(params->mToggleMessageSwitchId);
    field_2BC_tlv_item_info = tlvId;

    mFontContext.LoadFontType(FontType::LcdFont);

    mPal1 = ResourceManagerWrapper::LoadPal(PalId::LedFont_1);
    mPal2 = ResourceManagerWrapper::LoadPal(PalId::LedFont_2);

    field_60_font.Load(60, mPal1, &mFontContext);

    IRenderer::PalRecord rec;
    rec.depth = 16;
    
    field_98_pal_rect.x = rec.x;
    field_98_pal_rect.y = rec.y;
    field_98_pal_rect.w = rec.depth;
    field_98_pal_rect.h = 1;

    // TODO FIX
    //IRenderer::GetRenderer()->PalSetData(rec, sLCDScreen_Palette2);

    if (SwitchStates_Get(field_2B2_toggle_message_switch_id))
    {
        field_A0_message = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, field_2B0_message_2_id);
    }
    else
    {
        field_A0_message = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, field_2AA_message_1_id);
    }

    //field_A0_message = "                               THIS IS A TEST";

    String_FormatString(field_A0_message, field_A8_message_buffer, 512, 1);
    field_A0_message = field_A8_message_buffer;
    field_A4_message_cutoff_ptr = nullptr;
    field_2AC_x_offset = 0;
    sFontDrawScreenSpace = 1;
    field_2AE_character_width = static_cast<u16>(field_60_font.MeasureCharacterWidth(*field_A0_message) + 2);
    sFontDrawScreenSpace = 0;
    field_2B4_show_random_message = 1;
    field_2B6_message_rand_min_id = params->mMessageRandMinId;
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
    field_2B8_message_rand_max_id = params->mMessageRandMaxId;
    field_2A8_play_sound_toggle = 0;
    gObjListDrawables->Push_Back(this);
}

void LCDScreen::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

#if LCD_PS1_SPEED
    field_2AC_x_offset += 6;
#else
    field_2AC_x_offset += 3;
#endif

    if (field_2AC_x_offset > field_2AE_character_width)
    {
        field_2AC_x_offset -= field_2AE_character_width;
        s8 lastChar = *field_A0_message;
        field_A0_message++; // Offset s8 index

        if (lastChar == 0)
        {
            if (field_2B4_show_random_message == 1)
            {
                field_2B4_show_random_message = 0;
                field_A0_message = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, Math_RandomRange(field_2B6_message_rand_min_id, field_2B8_message_rand_max_id));

                // Change pal
                field_60_font.field_34_FontContext->field_C_resource_id.mCurPal = mPal2.mPal;
            }
            else
            {
                field_2B4_show_random_message = 1;
                if (SwitchStates_Get(field_2B2_toggle_message_switch_id))
                {
                    field_A0_message = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, field_2B0_message_2_id);
                }
                else
                {
                    field_A0_message = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, field_2AA_message_1_id);
                }

                // Change pal
                field_60_font.field_34_FontContext->field_C_resource_id.mCurPal = mPal1.mPal;
            }

            String_FormatString(field_A0_message, field_A8_message_buffer, 512, 1);
            field_A0_message = field_A8_message_buffer;

            // TODO: Fix
            //auto palSwap = field_98_pal_rect;
            //field_98_pal_rect = field_60_font.field_28_palette_rect;
            //field_60_font.field_28_palette_rect = palSwap;



        }

        sFontDrawScreenSpace = 1;
        field_2AE_character_width = static_cast<u16>(field_60_font.MeasureCharacterWidth(*field_A0_message) + 2);
        sFontDrawScreenSpace = 0;
    }

    auto screenLeft = field_2C0_tlv.mTopLeftX - FP_GetExponent(pScreenManager->CamXPos());
    auto screenRight = field_2C0_tlv.mBottomRightX - FP_GetExponent(pScreenManager->CamXPos());

    sFontDrawScreenSpace = 1;
    auto slicedText = field_60_font.SliceText(
        field_A0_message,
        PCToPsxX(screenLeft) - field_2AC_x_offset,
        FP_FromInteger(1),
        screenRight);
    sFontDrawScreenSpace = 0;
    if (slicedText != field_A4_message_cutoff_ptr)
    {
        field_2A8_play_sound_toggle = !field_2A8_play_sound_toggle;
        field_A4_message_cutoff_ptr = slicedText;
        if (*slicedText != ' ')
        {
            if (field_2A8_play_sound_toggle)
            {
                SfxPlayMono(relive::SoundEffects::LCDScreen, 0);
            }
        }
    }
}

void LCDScreen::VRender(PrimHeader** ppOt)
{
    if (gNumCamSwappers == 0)
    {
        const s32 screenX = field_2C0_tlv.mTopLeftX - FP_GetExponent(pScreenManager->CamXPos());
        const s32 screenY = ((field_2C0_tlv.mTopLeftY + field_2C0_tlv.mBottomRightY) / 2 - FP_GetExponent(pScreenManager->CamYPos())) - 7;
        const s32 screenXWorld = PsxToPCX(screenX);
        const s32 maxWidth = field_2C0_tlv.mBottomRightX - FP_GetExponent(pScreenManager->CamXPos());

        PSX_RECT clipRect = {
            0,
            0,
            640,
            240};

        Init_PrimClipper(&field_20_prim_clippers[0][gPsxDisplay.mBufferIndex], &clipRect);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_RopeWebDrillMeatSaw_24), &field_20_prim_clippers[0][gPsxDisplay.mBufferIndex].mBase);

        sFontDrawScreenSpace = 1;
        field_60_font.DrawString(
            ppOt,
            field_A0_message,
            static_cast<s16>(screenXWorld - field_2AC_x_offset),
            static_cast<s16>(screenY),
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_RopeWebDrillMeatSaw_24,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            maxWidth,
            sDisableFontFlicker != 0 ? 0 : 40);
        sFontDrawScreenSpace = 0;

        clipRect = {
            static_cast<s16>(screenXWorld),
            static_cast<s16>(screenY - 12),
            static_cast<s16>(PsxToPCX(maxWidth - screenX)),
            48};

        auto* clipper = &field_20_prim_clippers[1][gPsxDisplay.mBufferIndex];
        Init_PrimClipper(clipper, &clipRect);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_RopeWebDrillMeatSaw_24), &clipper->mBase);
    }
}

void LCDScreen::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

LCDScreen::~LCDScreen()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(field_2BC_tlv_item_info, -1, 0, 0);
}
