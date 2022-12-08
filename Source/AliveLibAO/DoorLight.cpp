#include "stdafx_ao.h"
#include "Function.hpp"
#include "DoorLight.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "MusicTrigger.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "CameraSwapper.hpp"
#include "Path.hpp"
#include "../AliveLibAE/FixedPoint.hpp"

namespace AO {

static s32 sNextDoorLightUpdate = -1;
static s32 sDoorLightUpdateTimer = 0;

DoorLight::DoorLight(relive::Path_LightEffect* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mTlvId = tlvId;
    mWidth = pTlv->mSize;
    mHeight = pTlv->mSize;

    SetType(ReliveTypes::eNone);
    mHasSwitchId = false;
    mSwitchId = pTlv->mSwitchId;
    mSwitchState = SwitchStates_Get(pTlv->mSwitchId);

    s32 xOff = 0;
    switch (pTlv->mType)
    {
        case relive::Path_LightEffect::Type::GoldGlow:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::GoldGlow));
            Animation_Init(GetAnimRes(AnimId::GoldGlow));
            break;
        }

        case relive::Path_LightEffect::Type::GreenGlow:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::GreenGlow));
            Animation_Init(GetAnimRes(AnimId::GreenGlow));
            break;
        }

        case relive::Path_LightEffect::Type::FlintGlow:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FlintGlow));
            Animation_Init(GetAnimRes(AnimId::FlintGlow));
            mHasSwitchId = true;
            break;
        }

        case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
        {
            mWidth = 0;
            mHeight = 0;
            if (SwitchStates_Get(pTlv->mSwitchId))
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::GreenDoorLight));
                Animation_Init(GetAnimRes(AnimId::GreenDoorLight));
            }
            else
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RedDoorLight));
                Animation_Init(GetAnimRes(AnimId::RedDoorLight));
                xOff = 6;
            }
            break;
        }

        case relive::Path_LightEffect::Type::Switchable_RedGreenHubLight:
        {
            mWidth = 0;
            mHeight = 0;
            if (SwitchStates_Get(pTlv->mSwitchId))
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::GreenHubLight));
                Animation_Init(GetAnimRes(AnimId::GreenHubLight));
            }
            else
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RedHubLight));
                Animation_Init(GetAnimRes(AnimId::RedHubLight));
            }
            break;
        }

        default:
            break;
    }

    GetAnimation().SetFlipX(pTlv->mDirection == relive::reliveXDirection::eLeft);

    if (sNextDoorLightUpdate < 0)
    {
        sNextDoorLightUpdate = sGnFrame;
        sDoorLightUpdateTimer = sNextDoorLightUpdate + Math_RandomRange(30, 45);
    }

    GetAnimation().SetIgnorePosOffset(true);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_3);

    GetAnimation().SetBlending(false);
    GetAnimation().SetSemiTrans(true);

    if (GetAnimation().GetFlipX())
    {
        mXPos = FP_FromInteger(pTlv->mTopLeftX - xOff);
    }
    else
    {
        mXPos = FP_FromInteger(xOff + pTlv->mTopLeftX);
    }

    SetSpriteScale(FP_FromInteger(1));
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
}

DoorLight::~DoorLight()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void DoorLight::VScreenChanged()
{
    SetDead(true);
    sNextDoorLightUpdate = -1;
}

void DoorLight::VUpdate()
{
    if (static_cast<s32>(sGnFrame) > sDoorLightUpdateTimer)
    {
        sNextDoorLightUpdate = sGnFrame + Math_RandomRange(6, 20);
        sDoorLightUpdateTimer = sNextDoorLightUpdate + Math_RandomRange(30, 45);
        mRGB.SetRGB(32, 32, 32);
    }
    else if (static_cast<s32>(sGnFrame) >= sNextDoorLightUpdate)
    {
        const FP lightAngle = (FP_FromInteger(128) * FP_FromInteger(sGnFrame - sNextDoorLightUpdate) / FP_FromInteger(sDoorLightUpdateTimer - sNextDoorLightUpdate));

        const FP lightAngleCosine = -Math_Cosine(FP_GetExponent(lightAngle) & 0xFF);
        const s32 rgbVal = FP_GetExponent(FP_FromInteger(255) * lightAngleCosine) + 32;

        u8 rgb = 0;
        if (rgbVal <= 255)
        {
            rgb = rgbVal & 0xFF;
        }
        else
        {
            rgb = 255;
        }

        if (mHasSwitchId)
        {
            if (SwitchStates_Get(mSwitchId))
            {
                if (mSwitchState == 0)
                {
                    mSwitchState = 1;

                    if (sControlledCharacter == sActiveHero)
                    {
                        relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 15);
                    }
                    else
                    {
                        SND_SEQ_Play_477760(SeqId::eSaveTriggerMusic_45, 1, 127, 127);
                    }
                }
                mRGB.SetRGB(32, rgb, 32);
            }
            else
            {
                mRGB.SetRGB(rgb, 32, 32);
            }
        }
        else
        {
            mRGB.SetRGB(rgb, rgb, rgb);
        }
    }
}

void DoorLight::VRender(PrimHeader** ppOt)
{
    if (gNumCamSwappers == 0)
    {
        const FP xpos = FP_FromInteger(pScreenManager->mCamXOff) + mXPos - pScreenManager->mCamPos->x;
        const FP ypos = FP_FromInteger(pScreenManager->mCamYOff) + mYPos - pScreenManager->mCamPos->y;

        GetAnimation().SetRGB(mRGB.r, mRGB.g, mRGB.b);

        GetAnimation().VRender(
            FP_GetExponent(FP_FromInteger((FP_GetExponent(xpos) - mWidth / 2))),
            FP_GetExponent(FP_FromInteger((FP_GetExponent(ypos) - mHeight / 2))),
            ppOt,
            mWidth,
            mHeight);
    }
}

} // namespace AO
