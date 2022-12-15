#include "stdafx_ao.h"
#include "Function.hpp"
#include "FlintLockFire.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "MusicTrigger.hpp"
#include "Sfx.hpp"
#include "Midi.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

struct FlintLockFireData final
{
    AnimId mGourdAnimId;
    AnimId mHammersDisabledAnimId;
    AnimId mHammersActivatingAnimId;
    bool mIsFire;
};

static const FlintLockFireData sFlintLockFireData[] = {
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::FlintLock_Gourd, AnimId::FlintLock_Hammers_Disabled, AnimId::FlintLock_Hammers_Activating, true},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::FlintLock_Gourd, AnimId::FlintLock_Hammers_Disabled, AnimId::FlintLock_Hammers_Activating, true},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::FlintLock_Gourd, AnimId::FlintLock_Hammers_Disabled, AnimId::FlintLock_Hammers_Activating, true},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::None, AnimId::None, AnimId::None, false},
    {AnimId::None, AnimId::None, AnimId::None, false}};


void FlintLockFire::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FlintLock_Gourd));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FlintLock_Hammers_Disabled));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FlintLock_Hammers_Activating));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Fire));
}

void FlintLockFire::VScreenChanged()
{
    SetDead(true);
}

void FlintLockFire::VStopAudio()
{
    if (mFireSound)
    {
        SND_Stop_Channels_Mask(mFireSound);
        mFireSound = 0;
    }
}

FlintLockFire::~FlintLockFire()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
    mGourdAnim.VCleanUp();

    if (sFlintLockFireData[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].mIsFire)
    {
        mFire1Anim.VCleanUp();
        mFire2Anim.VCleanUp();
        if (mFireSound)
        {
            SND_Stop_Channels_Mask(mFireSound);
        }
    }
}

FlintLockFire::FlintLockFire(relive::Path_FlintLockFire* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eFlintLockFire);

    LoadAnimations();

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    Animation_Init(GetAnimRes(sFlintLockFireData[cur_lvl].mHammersDisabledAnimId));
    GetAnimation().SetSemiTrans(true);

    mGourdAnim.Init(
        GetAnimRes(sFlintLockFireData[cur_lvl].mGourdAnimId),
        this);

    mGourdAnim.SetRenderMode(TPageAbr::eBlend_0);
    mGourdAnim.SetAnimate(false);
    mGourdAnim.SetSemiTrans(true);

    if (sFlintLockFireData[cur_lvl].mIsFire)
    {
        mFire1Anim.Init(GetAnimRes(AnimId::Fire), this);
        mFire1Anim.SetRenderMode(TPageAbr::eBlend_0);
        mFire1Anim.SetAnimate(false);
        mFire1Anim.SetRender(false);
        mFire1Anim.SetSemiTrans(true);

        mFire2Anim.Init(GetAnimRes(AnimId::Fire), this);
        mFire2Anim.SetRenderMode(TPageAbr::eBlend_0);
        mFire2Anim.SetAnimate(false);
        mFire2Anim.SetRender(false);
        mFire2Anim.SetSemiTrans(true);
        mFire2Anim.SetFlipX(true);
        mFire2Anim.SetFrame(3u);
    }

    mState = States::eDisabled_0;
    mFireSound = 0;
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mTlvId = tlvId;
    mSwitchId = pTlv->mSwitchId;

    Layer layer = Layer::eLayer_0;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        layer = Layer::eLayer_BeforeShadow_Half_6;
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        layer = Layer::eLayer_BeforeShadow_25;
        SetScale(Scale::Fg);
    }

    GetAnimation().SetRenderLayer(layer);
    mGourdAnim.SetRenderLayer(layer);

    if (sFlintLockFireData[cur_lvl].mIsFire)
    {
        mFire1Anim.SetRenderLayer(layer);
        mFire2Anim.SetRenderLayer(layer);
    }

    if (SwitchStates_Get(pTlv->mSwitchId))
    {
        mState = States::eActivated_2;
        GetAnimation().Set_Animation_Data(GetAnimRes(sFlintLockFireData[cur_lvl].mHammersActivatingAnimId));
        GetAnimation().SetFrame(GetAnimation().Get_Frame_Count() - 1);
        GetAnimation().VDecode();
        GetAnimation().SetSemiTrans(true);
        mGourdAnim.SetAnimate(true);

        if (sFlintLockFireData[cur_lvl].mIsFire)
        {
            mFire1Anim.SetAnimate(true);
            mFire1Anim.SetRender(true);

            mFire2Anim.SetAnimate(true);
            mFire2Anim.SetRender(true);

            mFireSound = SfxPlayMono(relive::SoundEffects::Fire, 0);
        }
    }
}

void FlintLockFire::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    switch (mState)
    {
        case States::eDisabled_0:
            if (SwitchStates_Get(mSwitchId))
            {
                mState = States::eActivating_1;
                GetAnimation().Set_Animation_Data(GetAnimRes(sFlintLockFireData[cur_lvl].mHammersActivatingAnimId));
            }
            break;

        case States::eActivating_1:
            if (sFlintLockFireData[cur_lvl].mIsFire)
            {
                if (GetAnimation().GetCurrentFrame() == 6)
                {
                    SfxPlayMono(relive::SoundEffects::FlintLock, 0);
                    SfxPlayMono(relive::SoundEffects::PostFlint, 0);
                }
            }

            if (GetAnimation().GetForwardLoopCompleted())
            {
                mState = States::eActivated_2;

                mGourdAnim.SetAnimate(true);
                if (sFlintLockFireData[cur_lvl].mIsFire)
                {
                    mFire1Anim.SetAnimate(true);
                    mFire1Anim.SetRender(true);

                    mFire2Anim.SetAnimate(true);
                    mFire2Anim.SetRender(true);

                    mFireSound = SfxPlayMono(relive::SoundEffects::Fire, 0);
                }

                relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 15);
            }
            break;

        case States::eActivated_2:
            if (sFlintLockFireData[cur_lvl].mIsFire)
            {
                if (!mFireSound)
                {
                    mFireSound = SfxPlayMono(relive::SoundEffects::Fire, 0);
                }
            }
            break;

        default:
            break;
    }
}

void FlintLockFire::VRender(PrimHeader** ppOt)
{
    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
    {
        const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        GetAnimation().SetSpriteScale(GetSpriteScale());
        mGourdAnim.SetSpriteScale(GetSpriteScale());

        if (sFlintLockFireData[cur_lvl].mIsFire)
        {
            mFire1Anim.SetSpriteScale((GetSpriteScale() * FP_FromDouble(1.33)));
            mFire2Anim.SetSpriteScale(GetSpriteScale());
        }

        s16 r = mRGB.r;
        s16 g = mRGB.g;
        s16 b = mRGB.b;

        const PSX_RECT bRect = VGetBoundingRect();

        if (GetApplyShadowZoneColour())
        {
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mXPos),
                FP_GetExponent(FP_FromInteger((bRect.y + bRect.h) / 2)),
                GetScale(),
                &r,
                &g,
                &b);
        }


        GetAnimation().SetRGB(r, g, b);
        mGourdAnim.SetRGB(r, g, b);

        if (sFlintLockFireData[cur_lvl].mIsFire)
        {
            mFire1Anim.SetRGB(r, g, b);
            mFire2Anim.SetRGB(r, g, b);

            mFire2Anim.VRender(
                FP_GetExponent(mXPos + (FP_FromInteger(gScreenManager->mCamXOff)) - gScreenManager->mCamPos->x),
                FP_GetExponent(mYPos + (FP_FromInteger(gScreenManager->mCamYOff + mYOffset - 28)) - gScreenManager->mCamPos->y),
                ppOt,
                0,
                0);

            mFire1Anim.VRender(
                FP_GetExponent(mXPos + (FP_FromInteger(gScreenManager->mCamXOff - 3)) - gScreenManager->mCamPos->x),
                FP_GetExponent(mYPos + (FP_FromInteger(gScreenManager->mCamYOff + mYOffset - 28)) - gScreenManager->mCamPos->y),
                ppOt,
                0,
                0);
        }

        GetAnimation().VRender(
            FP_GetExponent(mXPos + FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x),
            FP_GetExponent(mYPos + (FP_FromInteger(mYOffset + gScreenManager->mCamYOff)) - gScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        mGourdAnim.VRender(
            FP_GetExponent(mXPos
                           + FP_FromInteger(gScreenManager->mCamXOff)
                           - gScreenManager->mCamPos->x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(mYOffset + gScreenManager->mCamYOff))
                           - gScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        GetAnimation().Get_Frame_Rect(&frameRect);

        mGourdAnim.Get_Frame_Rect(&frameRect);

        if (sFlintLockFireData[cur_lvl].mIsFire)
        {
            mFire1Anim.Get_Frame_Rect(&frameRect);


            mFire2Anim.Get_Frame_Rect(&frameRect);

        }
    }
}

} // namespace AO
