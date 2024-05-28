#include "stdafx.h"
#include "IBirdPortal.hpp"
#include "Dove.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "../Sound/Midi.hpp"
#include "../ObjectIds.hpp"
#include "ScreenClipper.hpp"
#include "ScreenManager.hpp"
#include "../PsxDisplay.hpp"
#include "BirdPortalTerminator.hpp"
#include "../FatalError.hpp"
#include "../AliveLibAE/QuikSave.hpp"

IBirdPortal::IBirdPortal()
    : BaseGameObject(true, 0)
{

}

void IBirdPortal::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Sparks));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BirdPortal_Flash));
}

void IBirdPortal::CreateDovesAndShrykullNumber()
{
    for (u8 i = 0; i < ALIVE_COUNTOF(mDoveIds); i++)
    {
        auto pDove = relive_new Dove(AnimId::Dove_Flying, mXPos, mYPos, mSpriteScale);

        mDovesExist = true;
        if (mPortalType == relive::Path_BirdPortal::PortalType::eAbe)
        {
            pDove->AsAlmostACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
        }
        else
        {
            pDove->AsACircle(mXPos, mYPos + (mSpriteScale * FP_FromInteger(30)), 42 * i);
        }
        pDove->SetSpriteScale(mSpriteScale);
        mDoveIds[i] = pDove->mBaseGameObjectId;
    }

    if (mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
    {
        const Layer indicatorLayer = mSpriteScale != FP_FromDouble(0.5) ? Layer::eLayer_27 : Layer::eLayer_8;
        auto pIndicator = relive_new ThrowableTotalIndicator(
            mXPos,
            mYPos + FP_FromInteger(10),
            indicatorLayer,
            mSpriteScale,
            mMudCountForShrykull,
            0);
        if (pIndicator)
        {
            mThrowableTotalIndicator = pIndicator->mBaseGameObjectId;
        }
    }
}

void IBirdPortal::VScreenChanged()
{
    if (mState <= PortalStates::IdlePortal_1 || mState >= PortalStates::KillPortalClipper_21 || ((GetMap().LevelChanged() || GetMap().PathChanged()) && (mState != PortalStates::AbeInsidePortal_16 || mPortalType != relive::Path_BirdPortal::PortalType::eAbe || GetMap().mNextLevel != mExitLevel || GetMap().mNextPath != mExitPath)))
    {
        SetDead(true);
    }
    BaseGameObject* pTerminator1 = sObjectIds.Find_Impl(mTerminatorId1);
    BaseGameObject* pTerminator2 = sObjectIds.Find_Impl(mTerminatorId2);
    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);

    if (GetDead())
    {
        if (pTerminator1)
        {
            pTerminator1->SetDead(true);
        }

        if (pTerminator2)
        {
            pTerminator2->SetDead(true);
        }

        if (pClipper1)
        {
            pClipper1->SetDead(true);
        }

        if (pClipper2)
        {
            pClipper2->SetDead(true);
        }

        mTerminatorId1 = Guid{};
        mTerminatorId2 = Guid{};
        mScreenClipperId1 = Guid{};
        mScreenClipperId2 = Guid{};
    }
    else if (mSfxPlaying)
    {
        SND_Stop_Channels_Mask(mSfxPlaying);
        mSfxPlaying = 0;
    }
}

void IBirdPortal::VStopAudio()
{
    if (mSfxPlaying)
    {
        SND_Stop_Channels_Mask(mSfxPlaying);
        mSfxPlaying = 0;
    }
}

void IBirdPortal::DestroyPortalClippers()
{
    BaseGameObject* pClipper1 = sObjectIds.Find_Impl(mScreenClipperId1);
    BaseGameObject* pClipper2 = sObjectIds.Find_Impl(mScreenClipperId2);

    mScreenClipperId1 = Guid{};
    mScreenClipperId2 = Guid{};

    if (pClipper1)
    {
        pClipper1->SetDead(true);
        if (pClipper2)
        {
            pClipper2->SetDead(true);
        }
    }
}

bool IBirdPortal::IsActivePortalState()
{
    return mState == PortalStates::ActivePortal_6;
}

bool IBirdPortal::IsAbeInsidePortalState()
{
    return mState == PortalStates::AbeInsidePortal_16;
}

bool IBirdPortal::IsAbExittingPortalState()
{
    return mState == PortalStates::PortalExit_AbeExitting_20;
}

void IBirdPortal::RemovePortalClipperDelayed()
{
    mState = PortalStates::KillPortalClipper_21;
    mTimer = MakeTimer(30);
}

void IBirdPortal::MudSaved()
{
    mMudCountForShrykull--;
}

void IBirdPortal::CreateTerminators()
{
    auto pTerminator1 = relive_new BirdPortalTerminator(mXPos, mYPos, mSpriteScale, mPortalType);
    if (pTerminator1)
    {
        mTerminatorId1 = pTerminator1->mBaseGameObjectId;
    }

    auto pTerminator2 = relive_new BirdPortalTerminator(mXPos, mYPos, mSpriteScale, mPortalType);
    if (pTerminator2)
    {
        mTerminatorId2 = pTerminator2->mBaseGameObjectId;
    }
}

bool IBirdPortal::ClipPortal(bool bIgnoreClipping)
{
    if (bIgnoreClipping && mState != PortalStates::ActivePortal_6)
    {
        return false;
    }

    if (mScreenClipperId1 != Guid{})
    {
        return true;
    }

    const s16 portalX = static_cast<s16>(PsxToPCX(FP_GetExponent(mXPos - gScreenManager->CamXPos()), 11));

    PSX_Point xy = {};
    PSX_Point wh = {};
    if (mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft)
    {
        xy.x = 0;
        xy.y = 0;

        wh.x = portalX;
        wh.y = 240;
    }
    else
    {
        xy.x = portalX;
        xy.y = 0;

        wh.x = 640;
        wh.y = 240;
    }

    // Clip objects entering portal?
    auto pClipper1 = relive_new ScreenClipper(xy, wh, Layer::eLayer_0);
    if (pClipper1)
    {
        mScreenClipperId1 = pClipper1->mBaseGameObjectId;
        if (mSpriteScale == FP_FromInteger(1))
        {
            pClipper1->mOtLayer = Layer::eLayer_BirdPortal_29;
        }
        else
        {
            pClipper1->mOtLayer = Layer::eLayer_BirdPortal_Half_10;
        }
    }

    // Clip whole screen when "in" the portal?
    auto pClipper2 = relive_new ScreenClipper(PSX_Point{0, 0}, PSX_Point{640, 240}, Layer::eLayer_0);
    if (pClipper2)
    {
        mScreenClipperId2 = pClipper2->mBaseGameObjectId;
        if (mSpriteScale == FP_FromInteger(1))
        {
            pClipper2->mOtLayer = Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31;
        }
        else
        {
            pClipper2->mOtLayer = Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12;
        }
    }

    return true;
}
