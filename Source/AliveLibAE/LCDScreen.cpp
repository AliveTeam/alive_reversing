#include "stdafx.h"
#include "LCDScreen.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "StringFormatters.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/PathDataExtensionsTypes.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "Input.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"

static const char_type* sLCDMessageTable[101] = {
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
    mMessageId2 = pTlv->mMessageId2;
    mToggleMessageSwitchId = static_cast<u16>(pTlv->mToggleMessageSwitchId);
    mTlvId = tlvId;

    mFontContext.LoadFontType(FontType::LcdFont);

    mPal1 = ResourceManagerWrapper::LoadPal(PalId::LedFont_1);
    mPal2 = ResourceManagerWrapper::LoadPal(PalId::LedFont_2);

    mFont.Load(60, mPal1, &mFontContext);

    if (SwitchStates_Get(mToggleMessageSwitchId))
    {
        mActiveMessage = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId2);
    }
    else
    {
        mActiveMessage = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId1);
    }

    String_FormatString(mActiveMessage, mMessageBuffer, 1);
    mActiveMessage = mMessageBuffer;
    mMessageCutoffPtr = nullptr;
    mOffsetX = 0;

    SetDrawable(true);

    mMessageRandMinId = pTlv->mMessageRandMinId;
    mMessageRandMaxId = pTlv->mMessageRandMaxId;

    gFontDrawScreenSpace = true;
    mCharacterWidth = mFont.MeasureCharacterWidth(mActiveMessage[0]) + 2;
    gFontDrawScreenSpace = false;

    mShowRandomMessage = true;

    mPlayLetterSound = false;
    gObjListDrawables->Push_Back(this);
}

void LCDScreen::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
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
            if (mShowRandomMessage)
            {
                mShowRandomMessage = false;
                mActiveMessage = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, Math_RandomRange(mMessageRandMinId, mMessageRandMaxId));

                // Change pal
                mFont.mFontContext->mFntResource.mCurPal = mPal2.mPal;
            }
            else
            {
                mShowRandomMessage = true;
                if (SwitchStates_Get(mToggleMessageSwitchId))
                {
                    mActiveMessage = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId2);
                }
                else
                {
                    mActiveMessage = gLCDMessages.GetMessage(gMap.mCurrentLevel, gMap.mCurrentPath, mMessageId1);
                }

                // Change pal
                mFont.mFontContext->mFntResource.mCurPal = mPal1.mPal;
            }

            String_FormatString(mActiveMessage, mMessageBuffer, 1);
            mActiveMessage = mMessageBuffer;
        }

        gFontDrawScreenSpace = true;
        mCharacterWidth = mFont.MeasureCharacterWidth(mActiveMessage[0]) + 2;
        gFontDrawScreenSpace = false;
    }

    gFontDrawScreenSpace = true;

    auto screenLeft = mTlvTopLeft.x - FP_GetExponent(gScreenManager->CamXPos());
    auto screenRight = mTlvBottomRight.x - FP_GetExponent(gScreenManager->CamXPos());

    const char_type* slicedText = mFont.SliceText(
        mActiveMessage,
        PCToPsxX(screenLeft) - mOffsetX,
        FP_FromInteger(1),
        screenRight);
    gFontDrawScreenSpace = false;
    if (slicedText != mMessageCutoffPtr)
    {
        mPlayLetterSound = !mPlayLetterSound;
        mMessageCutoffPtr = slicedText;
        if (*slicedText != ' ')
        {
            if (mPlayLetterSound)
            {
                SfxPlayMono(relive::SoundEffects::LCDScreen, 0);
            }
        }
    }
}

void LCDScreen::VRender(OrderingTable& ot)
{
    if (gNumCamSwappers == 0)
    {
        const s32 screenX = mTlvTopLeft.x - FP_GetExponent(gScreenManager->CamXPos());
        const s32 screenY = ((mTlvTopLeft.y + mTlvBottomRight.y) / 2) - (FP_GetExponent(gScreenManager->CamYPos())) - 7;
        const s32 screenXWorld = PsxToPCX(screenX);
        const s32 maxWidth = mTlvBottomRight.x - FP_GetExponent(gScreenManager->CamXPos());

        PSX_RECT clipRect = {
            0,
            0,
            640,
            240};

        mPrimClippers[0].SetRect(clipRect);
        ot.Add(Layer::eLayer_RopeWebDrillMeatSaw_24, &mPrimClippers[0]);

        gFontDrawScreenSpace = true;
        mFont.DrawString(
            ot,
            mActiveMessage,
            static_cast<s16>(screenXWorld - mOffsetX),
            static_cast<s16>(screenY),
            relive::TBlendModes::eBlend_1,
            1,
            0,
            Layer::eLayer_RopeWebDrillMeatSaw_24,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            maxWidth,
            gDisableFontFlicker != false ? 0 : 40);
        gFontDrawScreenSpace = false;

        clipRect = {
            static_cast<s16>(screenXWorld),
            static_cast<s16>(screenY - 12),
            static_cast<s16>(PsxToPCX(maxWidth - screenX)),
            48};

        mPrimClippers[1].SetRect(clipRect);
        ot.Add(Layer::eLayer_RopeWebDrillMeatSaw_24, &mPrimClippers[1]);
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
