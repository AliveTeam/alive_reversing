#include "stdafx.h"
#include "BrewMachine.hpp"
#include "../relive_lib/Events.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"

BrewMachine::BrewMachine(relive::Path_BrewMachine* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0),
    mTlvInfo(tlvId),
    mMaxBrewCount(pTlv->mBrewCount)
{
    SetType(ReliveTypes::eBrewMachine);

    mPal = ResourceManagerWrapper::LoadPal(PalId::LedFont_Red);

    mFontContext.LoadFontType(FontType::LcdFont);
    mFont.Load(3, mPal, &mFontContext);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BrewMachine_Button));
    Animation_Init(GetAnimRes(AnimId::BrewMachine_Button));

    SetApplyShadowZoneColour(false);
    GetAnimation().SetRenderLayer(Layer::eLayer_Well_23);

    const u8 savedBrewCount = pTlv->mTlvSpecificMeaning;
    if (savedBrewCount == 0)
    {
        mRemainingBrewCount = mMaxBrewCount;
    }
    else if (savedBrewCount > 30)
    {
        mRemainingBrewCount = 0;
    }
    else
    {
        mRemainingBrewCount = savedBrewCount;
    }

    mTextX = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftX + 5) - gScreenManager->CamXPos()));
    mTextY = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftY + 10) - gScreenManager->CamYPos()));
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mBrewMachineCamera = gMap.mCurrentCamera;
}

BrewMachine::~BrewMachine()
{
    Path::TLV_Reset(mTlvInfo);
}

void BrewMachine::VUpdate()
{
    relive::Path_BrewMachine* pTlv = static_cast<relive::Path_BrewMachine*>(gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));
    if (mRemainingBrewCount > 0)
    {
        pTlv->mTlvSpecificMeaning = static_cast<u8>(mRemainingBrewCount);
    }
    else
    {
        pTlv->mTlvSpecificMeaning = 50;
    }

    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }
}

void BrewMachine::VRender(OrderingTable& ot)
{
    if (gMap.mCurrentCamera == mBrewMachineCamera)
    {
        char_type text[12] = {};
        sprintf(text, "%02d", mRemainingBrewCount);
        const s32 textWidth = mFont.MeasureTextWidth(text);
        s16 flickerAmount = 50;
        if (gDisableFontFlicker)
        {
            flickerAmount = 0;
        }

        mFont.DrawString(
            ot,
            text,
            mTextX,
            mTextY,
            relive::TBlendModes::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            mTextX + textWidth,
            flickerAmount);
    }

    BaseAnimatedWithPhysicsGameObject::VRender(ot);
}
