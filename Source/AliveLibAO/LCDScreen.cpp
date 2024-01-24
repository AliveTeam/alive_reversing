#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Function.hpp"
#include "LCDScreen.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "StringFormatters.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Input.hpp"
#include "../relive_lib/PathDataExtensionsTypes.hpp"
#include "PathData.hpp"
#include "Path.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Game.hpp"

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
    mBaseGameObjectTlvInfo = tlvId;

    mTlvTopLeft.x = pTlv->mTopLeftX;
    mTlvTopLeft.y = pTlv->mTopLeftY;

    mTlvBottomRight.x = pTlv->mBottomRightX;
    mTlvBottomRight.y = pTlv->mBottomRightY;

    mMessageId1 = pTlv->mMessageId1;
    mTlvId = tlvId;

    mFontContext.LoadFontType(FontType::LcdFont);

    mPal1 = ResourceManagerWrapper::LoadPal(PalId::LedFont_1);
    mPal2 = ResourceManagerWrapper::LoadPal(PalId::LedFont_2);

    mFont.Load(60, mPal1, &mFontContext);

    if (Input().IsJoyStickEnabled() || mMessageId1 != 62)
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
    mMessageCutoffPtr = nullptr;
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
        const s8 lastChar = *mActiveMessage;
        mActiveMessage++;

        if (lastChar == 0)
        {
            mShowRandomMessage++; // TODO: wot? must be a bug?
            if (mShowRandomMessage == 1) // TODO: Double wot
            {
                const auto rangedRandom = Math_RandomRange(
                    mMessageRandMinId,
                    mMessageRandMaxId);

                if (Input().IsJoyStickEnabled() || rangedRandom != 62)
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

                if (Input().IsJoyStickEnabled() || mMessageId1 != 62)
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

            mActiveMessage = mMessageBuffer;
        }

        gFontDrawScreenSpace = true;
        mCharacterWidth = mFont.MeasureCharacterWidth(mActiveMessage[0]);
        gFontDrawScreenSpace = false;
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

void LCDScreen::VRender(OrderingTable& ot)
{
    if (gNumCamSwappers == 0)
    {
        const s32 screenX = mTlvTopLeft.x - FP_GetExponent(gScreenManager->mCamPos->x - FP_FromInteger(gScreenManager->mCamXOff));
        const s32 screenY = (mTlvTopLeft.y + mTlvBottomRight.y) / 2 - FP_GetExponent(gScreenManager->mCamPos->y - FP_FromInteger(gScreenManager->mCamYOff)) - 7;
        const s32 maxWidth = FP_GetExponent(FP_FromInteger(gScreenManager->mCamXOff + mTlvBottomRight.x) - gScreenManager->mCamPos->x);

        PSX_RECT clipRect = {
            0,
            0,
            640,
            static_cast<s16>(gPsxDisplay.mHeight)};

        mPrimClippers[0].SetRect(clipRect);
        ot.Add(Layer::eLayer_BeforeWell_22, &mPrimClippers[0]);

        gFontDrawScreenSpace = true;
        mFont.DrawString(
            ot,
            mActiveMessage,
            static_cast<s16>(PsxToPCX(screenX, 11) - mOffsetX),
            static_cast<s16>(screenY),
            relive::TBlendModes::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            maxWidth,
            gDisableFontFlicker != false ? 0 : 40);
        gFontDrawScreenSpace = false;

        clipRect = {
            static_cast<s16>(PsxToPCX(screenX, 11)),
            static_cast<s16>(screenY - 12),
            static_cast<s16>(PsxToPCX(maxWidth - screenX, 51)),
            48};

        mPrimClippers[1].SetRect(clipRect);
        ot.Add(Layer::eLayer_BeforeWell_22, &mPrimClippers[1]);
    }
}

void LCDScreen::VScreenChanged()
{
    SetDead(true);
}

LCDScreen::~LCDScreen()
{
    gObjListDrawables->Remove_Item(this);
    Path::TLV_Reset(mTlvId);
}

} // namespace AO
