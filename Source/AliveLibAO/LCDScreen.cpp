#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Function.hpp"
#include "LCDScreen.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "StringFormatters.hpp"
#include "../relive_lib/Primitives_common.hpp"
#include "Input.hpp"
#include "../relive_lib/PathDataExtensionsTypes.hpp"
#include "PathData.hpp"
#include "Path.hpp"
#include "Map.hpp"

namespace AO {

static const char_type* sLCDMessageTable[90] = {
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
    "                               To alert a Mudokon, say hello by holding " kAO_Speak1 " and pressing " kAO_Jump_Or_Hello ".   Then, talk to Mudokons by holding " kAO_Speak1 " or " kAO_Speak2 " and pressing the " kAO_Jump_Or_Hello ", " kAO_Action ", " kAO_Throw ", or " kAO_Crouch " button.   Experiment!",
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

static const StringTable* sPerLvlMessagesLCD[static_cast<u32>(LevelIds::eDesertEscape_15) + 1][99] = {};

void SetLcdMessagesForLvl(const StringTable& msgs, LevelIds lvl, u32 pathId)
{
    sPerLvlMessagesLCD[static_cast<u32>(lvl)][pathId] = &msgs;
}

class LCDMessages final
{
public:
    const char_type* GetMessage(EReliveLevelIds lvlId, u32 pathId, u32 msgId) const
    {
        const StringTable* pTable = sPerLvlMessagesLCD[static_cast<u32>(MapWrapper::ToAO(lvlId))][pathId];
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

        if (msgId < ALIVE_COUNTOF(sLCDMessageTable))
        {
            return sLCDMessageTable[msgId];
        }
        else
        {
            LOG_WARNING("LCD message out of bounds using original message table id: %d", msgId);
            return sLCDMessageTable[0];
        }
    }
};
static LCDMessages gLCDMessages;

LCDScreen::LCDScreen(relive::Path_LCDScreen* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvTopLeft.x = pTlv->mTopLeftX;
    mTlvTopLeft.y = pTlv->mTopLeftY;

    mTlvBottomRight.x = pTlv->mBottomRightX;
    mTlvBottomRight.y = pTlv->mBottomRightY;

    mTlvId = tlvId;

    mMessageId1 = pTlv->mMessageId1;

    mFontContext.LoadFontType(FontType::LcdFont);

    mPal1 = ResourceManagerWrapper::LoadPal(PalId::LedFont_1);
    mPal2 = ResourceManagerWrapper::LoadPal(PalId::LedFont_2);

    mFont.Load(60, mPal1, &mFontContext);

    if (Input_JoyStickEnabled() || mMessageId1 != 62)
    {
        String_FormatString(gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId1), mMessageBuffer);
    }
    else
    {
        strcpy(mMessageBuffer,
               "                               To alert a Mudokon, say hello by holding (1) on the keyboard.   Then, talk to"
               " Mudokons by using the keyboard numbers (1) thru (8).   Experiment!");
    }

    mActiveMessage = mMessageBuffer;
    mMessageCutoffPtr = 0;
    mOffsetX = 0;

    SetDrawable(true);

    mMessageRandMinId = pTlv->mMessageRandMinId;
    mMessageRandMaxId = pTlv->mMessageRandMaxId;

    gFontDrawScreenSpace = true;
    mCharacterWidth = mFont.MeasureCharacterWidth(mMessageBuffer[0]);
    gFontDrawScreenSpace = false;

    mShowRandomMessage = 0;
    gObjListDrawables->Push_Back(this);
}

LCDScreen::~LCDScreen()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void LCDScreen::VScreenChanged()
{
    SetDead(true);
}

void LCDScreen::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

#if LCD_PS1_SPEED
    mOffsetX += 6;
#else
    mOffsetX += 3;
#endif

    if (mOffsetX > mCharacterWidth)
    {
        mOffsetX -= mCharacterWidth;
        char_type* pMsg = mActiveMessage;
        mActiveMessage++;
        if (!*pMsg)
        {
            mActiveMessage = mMessageBuffer;
            mShowRandomMessage++;
            if (mShowRandomMessage == 1)
            {
                const auto rangedRandom = Math_RandomRange(
                    mMessageRandMinId,
                    mMessageRandMaxId);

                if (Input_JoyStickEnabled() || rangedRandom != 62)
                {
                    String_FormatString(gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, rangedRandom), mMessageBuffer);
                }
                else
                {
                    strcpy(
                        mMessageBuffer,
                        "                               To alert a Mudokon, say hello by holding (1) on the keyboard.   Then, talk to Mudokons by using the keyboard numbers (1) thru (8).   Experiment!");
                }

                 // Change pal
                mFont.mFontContext->mFntResource.mCurPal = mPal2.mPal;
            }
            else
            {
                mShowRandomMessage = 0;

                if (Input_JoyStickEnabled() || mMessageId1 != 62)
                {
                    String_FormatString(
                        gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId1),
                        mMessageBuffer);
                }
                else
                {
                    strcpy(
                        mMessageBuffer,
                        "                               To alert a Mudokon, say hello by holding (1) on the keyboard.   Then, talk to Mudokons by using the keyboard numbers (1) thru (8).   Experiment!");
                }

                // Change pal
                mFont.mFontContext->mFntResource.mCurPal = mPal1.mPal;
            }
        }
        gFontDrawScreenSpace = true;
        mCharacterWidth = mFont.MeasureCharacterWidth(*mActiveMessage);
    }
    gFontDrawScreenSpace = true;

    auto screenLeft = mTlvTopLeft.x - FP_GetExponent(gScreenManager->mCamPos->x);
    auto screenRight = mTlvBottomRight.x - FP_GetExponent(gScreenManager->mCamPos->x);

    const char_type* slicedText = mFont.SliceText(
        mActiveMessage,
        PsxToPCX(screenLeft - gScreenManager->mCamXOff, 11) - mOffsetX,
        FP_FromInteger(1),
        screenRight - gScreenManager->mCamXOff);
    gFontDrawScreenSpace = false;
    if (slicedText != mMessageCutoffPtr)
    {
        mMessageCutoffPtr = slicedText;
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
        const FP_Point* camPos = gScreenManager->mCamPos;

        auto endY = mTlvTopLeft.y + mTlvBottomRight.y;
        auto endX = gScreenManager->mCamXOff + mTlvBottomRight.x;

        const s32 screenX = mTlvTopLeft.x - FP_GetExponent(camPos->x - FP_FromInteger(gScreenManager->mCamXOff));
        const s32 screenY = endY / 2 - FP_GetExponent(camPos->y - FP_FromInteger(gScreenManager->mCamYOff)) - 7;
        const s32 maxWidth = FP_GetExponent(FP_FromInteger(endX) - camPos->x);

        PSX_RECT clipRect = {
            0,
            0,
            640,
            static_cast<s16>(gPsxDisplay.mHeight)};

        auto* pClippers = &mPrimClippers[0][gPsxDisplay.mBufferIndex];
        Init_PrimClipper(
            pClippers,
            &clipRect);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_BeforeWell_22), &pClippers->mBase);

        auto fontFlickerAmount = 50;
        if (gDisableFontFlicker)
        {
            fontFlickerAmount = 0;
        }
        if (fontFlickerAmount)
        {
            fontFlickerAmount = 40;
        }

        gFontDrawScreenSpace = true;
        mFont.DrawString(
            ppOt,
            mActiveMessage,
            static_cast<s16>(PsxToPCX(screenX, 11) - mOffsetX),
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
            static_cast<s16>(fontFlickerAmount));
        gFontDrawScreenSpace = false;

        PSX_RECT clipRect2 = {};

        clipRect2.x = static_cast<s16>(PsxToPCX(screenX, 11));
        clipRect2.y = static_cast<s16>(screenY - 12);
        clipRect2.w = static_cast<s16>(PsxToPCX(maxWidth - screenX, 51));
        clipRect2.h = 48;

        auto* clipper = &mPrimClippers[1][gPsxDisplay.mBufferIndex];
        Init_PrimClipper(clipper, &clipRect2);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_BeforeWell_22), &clipper->mBase);
    }
}

} // namespace AO
