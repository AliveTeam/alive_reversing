#include "stdafx.h"
#include "TorturedMudokon.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "Flash.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "Abe.hpp"

TorturedMudokon::TorturedMudokon(Path_TorturedMudokon* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eTorturedMud);
    mTlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Tortured_Mudokon);
    mTorturedMudRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    if (mTorturedMudRes)
    {
        mXPos = FP_FromInteger(pTlv->mTopLeft.x);
        mYPos = FP_FromInteger(pTlv->mTopLeft.y);
        Animation_Init(AnimId::Tortured_Mudokon, mTorturedMudRes);
        mAnim.SetFrame(Math_RandomRange(0, mAnim.Get_Frame_Count() - 1));
        mKillSwitchId = pTlv->mKillSwitchId;
        mReleaseSwitchId = pTlv->mReleaseSwitchId;
        mState = TorturedMudokonState::eBeingTortured_0;
        SetupTearsAnimation(&mTearsAnim);
        SetupZapAnimation(&mZapAnim);
        mPainSoundPitch = Math_RandomRange(800, 1000);
        mFlashColourTimer = sGnFrame + 100;
        mFlashColourCounter = 0;
    }
    else
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void TorturedMudokon::SetupTearsAnimation(Animation* pAnim)
{
    const AnimRecord& rec = AnimRec(AnimId::Tortured_Mudokon_Tears);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    if (pAnim->Init(AnimId::Tortured_Mudokon_Tears, this, ppRes))
    {
        pAnim->mRenderLayer = mAnim.mRenderLayer;
        pAnim->field_14_scale = mSpriteScale;
        pAnim->mRed = 128;
        pAnim->mGreen = 128;
        pAnim->mBlue = 128;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void TorturedMudokon::SetupZapAnimation(Animation* pAnim)
{
    const AnimRecord& rec = AnimRec(AnimId::Electric_Wall);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    if (pAnim->Init(AnimId::Electric_Wall, this, ppRes))
    {
        // TODO: clean this up
        const s32 layerM1 = static_cast<s32>(mAnim.mRenderLayer) - 1;
        pAnim->mRenderLayer = static_cast<Layer>(layerM1);
        pAnim->field_14_scale = mSpriteScale;
        pAnim->mRed = 128;
        pAnim->mGreen = 128;
        pAnim->mBlue = 128;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void TorturedMudokon::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void TorturedMudokon::VRender(PrimHeader** ppOt)
{
    mTearsAnim.VRender(
        FP_GetExponent(mXPos - pScreenManager->CamXPos()),
        FP_GetExponent(mYPos - pScreenManager->CamYPos()),
        ppOt,
        0,
        0);
    if (mTearsAnim.mFlags.Get(AnimFlags::eBit3_Render))
    {
        mZapAnim.VRender(
            FP_GetExponent(mXPos - pScreenManager->CamXPos()),
            FP_GetExponent(mYPos - pScreenManager->CamYPos()),
            ppOt,
            0,
            0);
    }

    PSX_RECT rect = {};
    mTearsAnim.Get_Frame_Rect(&rect);
    pScreenManager->InvalidateRectCurrentIdx(
        rect.x,
        rect.y,
        rect.w,
        rect.h);

    mZapAnim.Get_Frame_Rect(&rect);
    pScreenManager->InvalidateRectCurrentIdx(
        rect.x,
        rect.y,
        rect.w,
        rect.h);

    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

TorturedMudokon::~TorturedMudokon()
{
    if (mState != TorturedMudokonState::eReleased_2)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }

    mTearsAnim.VCleanUp();
    mZapAnim.VCleanUp();
}

void TorturedMudokon::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (mFlashColourTimer == static_cast<s32>(sGnFrame))
    {
        mFlashColourCounter++;
        mFlashColourTimer = sGnFrame + 100;
        if (mFlashColourCounter == 4)
        {
            mFlashColourCounter = 0;
        }
    }

    switch (mState)
    {
        case TorturedMudokonState::eBeingTortured_0:
            if (SwitchStates_Get(mKillSwitchId))
            {
                mState = TorturedMudokonState::eKilled_1;
                mAnim.Set_Animation_Data(AnimId::Tortured_Mudokon_Zap, nullptr);
            }
            break;

        case TorturedMudokonState::eKilled_1:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;

        case TorturedMudokonState::eReleased_2:
            return;

        default:
            break;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mAnim.mFrameChangeCounter == mAnim.mFrameDelay)
        {
            mZapAnim.mFlags.Clear(AnimFlags::eBit3_Render);
            if (!Math_RandomRange(0, 8))
            {
                Mudokon_SFX(MudSounds::eNoSad_22, 100, Math_RandomRange(mPainSoundPitch, mPainSoundPitch + 100), 0);
            }
        }
    }

    if (mAnim.mCurrentFrame == 6)
    {
        if (Math_RandomRange(0, 2))
        {
            mAnim.mCurrentFrame = 0;
        }
    }

    u8 rgbBase = 0;
    switch (mFlashColourCounter)
    {
        case 0:
            rgbBase = static_cast<u8>((mFlashColourTimer & 0xFF) - sGnFrame);
            break;

        case 1:
            rgbBase = 0;
            break;

        case 2:
            rgbBase = static_cast<u8>(sGnFrame - (mFlashColourTimer & 0xFF) + 100);
            break;

        case 3:
            rgbBase = 100;
            break;
    }

    if (mAnim.mCurrentFrame == 6 && mAnim.mFrameChangeCounter == mAnim.mFrameDelay)
    {
        relive_new Flash(Layer::eLayer_Above_FG1_39, rgbBase + 50, rgbBase + 50, rgbBase + 110, TPageAbr::eBlend_1, 1);
        mZapAnim.mFlags.Set(AnimFlags::eBit3_Render);
        SfxPlayMono(SoundEffect::ElectricZap_39, 70);
        const s16 sndRnd = Math_RandomRange(0, 3) - 1;
        if (sndRnd)
        {
            if (sndRnd == 1)
            {
                Mudokon_SFX(MudSounds::eHurt1_16, 127, Math_RandomRange(mPainSoundPitch, mPainSoundPitch + 100), 0);
            }
        }
        else
        {
            Mudokon_SFX(MudSounds::eHurt2_9, 127, Math_RandomRange(mPainSoundPitch, mPainSoundPitch + 100), 0);
        }
    }

    if (mAnim.mCurrentFrame >= 7 && !Math_RandomRange(0, 10))
    {
        relive_new Flash(Layer::eLayer_Above_FG1_39, rgbBase + 10, rgbBase + 10, rgbBase + 50, TPageAbr::eBlend_1, 1);
    }

    if (SwitchStates_Get(mReleaseSwitchId))
    {
        mState = TorturedMudokonState::eReleased_2;
        mAnim.Set_Animation_Data(AnimId::Tortured_Mudokon_Released, nullptr);
        mTearsAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        mZapAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        Path_TLV* pTlv = sPathInfo->TLV_From_Offset_Lvl_Cam(mTlvInfo);
        if (pTlv)
        {
            pTlv->mTlvState = 1;
        }
    }
}
