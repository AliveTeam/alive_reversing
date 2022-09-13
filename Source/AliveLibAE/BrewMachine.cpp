#include "stdafx.h"
#include "BrewMachine.hpp"
#include "../relive_lib/Events.hpp"
#include "Function.hpp"
#include "Map.hpp"

// TODO: Should be const but can't be due to mlgs hacks in Font obj
u8 fontPalette_550F08[32] = {
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
    101u,
    140u,
    140u,
    177u,
    19u,
    148u,
    100u,
    206u,
    101u,
    206u,
    215u,
    152u,
    20u,
    161u,
    24u,
    216u};

BrewMachine::BrewMachine(relive::Path_BrewMachine* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBrewMachine);

    mFontContext.LoadFontType_433400(2);
    mFont.ctor_433590(3, fontPalette_550F08, &mFontContext);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BrewMachine_Button));
    Animation_Init(GetAnimRes(AnimId::BrewMachine_Button));

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mTlvInfo = tlvId;
    mAnim.mRenderLayer = Layer::eLayer_Well_23;
    mRemainingBrewCount = pTlv->mBrewCount;

    const u8 savedBrewCount = pTlv->mTlvSpecificMeaning;
    if (savedBrewCount == 0)
    {
        mTotalBrewCount = mRemainingBrewCount;
    }
    else if (savedBrewCount > 30)
    {
        mTotalBrewCount = 0;
    }
    else
    {
        mTotalBrewCount = savedBrewCount;
    }

    mTextX = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftX + 5) - pScreenManager->CamXPos()));
    mTextY = FP_GetExponent((FP_FromInteger(pTlv->mTopLeftY + 10) - pScreenManager->CamYPos()));
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mBrewMachineCamera = gMap.mCurrentCamera;
}

BrewMachine::~BrewMachine()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    mFont.dtor_433540();
    mFontContext.dtor_433510();
}

void BrewMachine::VUpdate()
{
    relive::Path_BrewMachine* pTlv = static_cast<relive::Path_BrewMachine*>(sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo));
    if (mTotalBrewCount > 0)
    {
        pTlv->mTlvSpecificMeaning = static_cast<u8>(mTotalBrewCount);
    }
    else
    {
        pTlv->mTlvSpecificMeaning = 50;
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BrewMachine::VRender(PrimHeader** ppOt)
{
    if (gMap.mCurrentCamera == mBrewMachineCamera)
    {
        char_type text[12] = {};
        sprintf(text, "%02d", mTotalBrewCount);
        const s32 textWidth = mFont.MeasureTextWidth(text);
        s16 flickerAmount = 50;
        if (sDisableFontFlicker_5C9304)
        {
            flickerAmount = 0;
        }

        mFont.DrawString_4337D0(
            ppOt,
            text,
            mTextX,
            mTextY,
            TPageAbr::eBlend_1,
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

        const s32 v5 = 5 * textWidth;
        pScreenManager->InvalidateRectCurrentIdx(
            PsxToPCX(mTextX),
            mTextY,
            PsxToPCX(8 * v5),
            16);
    }

    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}
