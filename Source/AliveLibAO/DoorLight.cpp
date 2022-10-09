#include "stdafx_ao.h"
#include "Function.hpp"
#include "DoorLight.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "MusicTrigger.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "CameraSwapper.hpp"

namespace AO {

ALIVE_VAR(1, 0x4C30A8, s32, gNextDoorLightUpdate_4C30A8, -1);
ALIVE_VAR(1, 0x4FC8A4, s32, gDoorLightUpdateTimer_4FC8A4, 0);

DoorLight::DoorLight(relive::Path_LightEffect* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_E4_tlvInfo = tlvId;
    field_E8_width = pTlv->mSize;
    field_EA_height = pTlv->mSize;

    SetType(ReliveTypes::eNone);
    field_EC_bHasID = 0;
    field_F0_switch_id = pTlv->mSwitchId;
    field_EE_switch_value = SwitchStates_Get(pTlv->mSwitchId);

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
            field_EC_bHasID = 1;
            break;
        }

        case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
        {
            field_E8_width = 0;
            field_EA_height = 0;
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
            field_E8_width = 0;
            field_EA_height = 0;
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

    mAnim.mFlags.Set(AnimFlags::eFlipX, pTlv->mDirection == relive::reliveXDirection::eLeft);

    if (gNextDoorLightUpdate_4C30A8 < 0)
    {
        gNextDoorLightUpdate_4C30A8 = sGnFrame;
        gDoorLightUpdateTimer_4FC8A4 = gNextDoorLightUpdate_4C30A8 + Math_RandomRange(30, 45);
    }

    mAnim.mFlags.Set(AnimFlags::eIgnorePosOffset);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    mAnim.mRenderMode = TPageAbr::eBlend_3;

    mAnim.mFlags.Clear(AnimFlags::eBlending);
    mAnim.mFlags.Set(AnimFlags::eSemiTrans);

    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mXPos = FP_FromInteger(pTlv->mTopLeftX - xOff);
    }
    else
    {
        mXPos = FP_FromInteger(xOff + pTlv->mTopLeftX);
    }

    mSpriteScale = FP_FromInteger(1);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
}

DoorLight::~DoorLight()
{
    Path::TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

void DoorLight::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    gNextDoorLightUpdate_4C30A8 = -1;
}

void DoorLight::VUpdate()
{
    if (static_cast<s32>(sGnFrame) > gDoorLightUpdateTimer_4FC8A4)
    {
        gNextDoorLightUpdate_4C30A8 = sGnFrame + Math_RandomRange(6, 20);
        gDoorLightUpdateTimer_4FC8A4 = gNextDoorLightUpdate_4C30A8 + Math_RandomRange(30, 45);
        mRGB.SetRGB(32, 32, 32);
    }
    else if (static_cast<s32>(sGnFrame) >= gNextDoorLightUpdate_4C30A8)
    {
        const FP lightAngle = (FP_FromInteger(128) * FP_FromInteger(sGnFrame - gNextDoorLightUpdate_4C30A8) / FP_FromInteger(gDoorLightUpdateTimer_4FC8A4 - gNextDoorLightUpdate_4C30A8));

        const FP lightAngleCosine = -Math_Cosine_4510A0(FP_GetExponent(lightAngle) & 0xFF);
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

        if (field_EC_bHasID)
        {
            if (SwitchStates_Get(field_F0_switch_id))
            {
                if (field_EE_switch_value == 0)
                {
                    field_EE_switch_value = 1;

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
    if (sNumCamSwappers_507668 == 0)
    {
        const FP xpos = FP_FromInteger(pScreenManager->mCamXOff) + mXPos - pScreenManager->mCamPos->x;
        const FP ypos = FP_FromInteger(pScreenManager->mCamYOff) + mYPos - pScreenManager->mCamPos->y;

        mAnim.mRed = static_cast<u8>(mRGB.r);
        mAnim.mGreen = static_cast<u8>(mRGB.g);
        mAnim.mBlue = static_cast<u8>(mRGB.b);

        mAnim.VRender(
            FP_GetExponent(FP_FromInteger((FP_GetExponent(xpos) - field_E8_width / 2))),
            FP_GetExponent(FP_FromInteger((FP_GetExponent(ypos) - field_EA_height / 2))),
            ppOt,
            field_E8_width,
            field_EA_height);
    }
}

} // namespace AO
