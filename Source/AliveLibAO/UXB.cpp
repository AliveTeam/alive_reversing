#include "stdafx_ao.h"
#include "Function.hpp"
#include "UXB.hpp"
#include "Sfx.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "BaseBomb.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Grid.hpp"
#include <math.h>

namespace AO {

void UXB::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_RedGreenTick));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::UXB_Disabled));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::UXB_Toggle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::UXB_Active));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_Flash));
}

UXB::UXB(relive::Path_UXB* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eUXB);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::UXB_Active));
    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::GreenFlash));

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_0;

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mCurrentState = UXBState::eDelay;

    mPatternLength = pTlv->mPatternLength;
    if (pTlv->mPatternLength < 1u || pTlv->mPatternLength > 4u)
    {
        mPatternLength = 1;
    }

    mPattern = pTlv->mPattern;
    if (!pTlv->mPattern) // If no pattern set, go to a default one.
    {
        mPattern = 11111;
    }

    mPatternIndex = 0;

    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
    mRedBlinkCount = (mPattern / static_cast<s32>(pow(10, mPatternLength - 1))) % 10;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;
        mScale = Scale::Fg;
    }

    InitBlinkAnim();

    if (pTlv->mTlvSpecificMeaning) // Stores the activated/deactivated state for UXB
    {
        if (pTlv->mStartState == relive::Path_UXB::StartState::eOn)
        {
            mFlashAnim.LoadPal(GetPalRes(PalId::GreenFlash));
            mIsRed = 0;

            mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));

            if (gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                SfxPlayMono(relive::SoundEffects::GreenTick, 35);
            }

            mAnim.Set_Animation_Data(GetAnimRes(AnimId::UXB_Disabled));

            mCurrentState = UXBState::eDeactivated;
            mStartingState = UXBState::eDelay;
        }
        else
        {
            mStartingState = UXBState::eDeactivated;
        }
    }
    else
    {
        if (pTlv->mStartState == relive::Path_UXB::StartState::eOn)
        {
            mStartingState = UXBState::eDelay;
        }
        else
        {
            mFlashAnim.LoadPal(GetPalRes(PalId::GreenFlash));
            mIsRed = 0;

            mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));

            mAnim.Set_Animation_Data(GetAnimRes(AnimId::UXB_Disabled));

            mStartingState = UXBState::eDeactivated;
            mCurrentState = UXBState::eDeactivated;
        }
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);

    mTlvInfo = tlvId;
    mNextStateTimer = sGnFrame;

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mIsRed = 0;
        mRGB.SetRGB(80, 90, 110);
    }

    const FP gridSnap = ScaleToGridSize(mSpriteScale);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromInteger(2));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = mXPos + (gridSnap / FP_FromInteger(2));
    mCollectionRect.h = mYPos;
}

void UXB::InitBlinkAnim()
{
    if (mFlashAnim.Init(GetAnimRes(AnimId::Bomb_RedGreenTick), this))
    {
        mFlashAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
        mFlashAnim.mFlags.Set(AnimFlags::eBit16_bBlending);

        mFlashAnim.mRenderLayer = mAnim.mRenderLayer;
        mFlashAnim.field_14_scale = mSpriteScale;
        mFlashAnim.mRed = 128;
        mFlashAnim.mGreen = 128;
        mFlashAnim.mBlue = 128;
        mFlashAnim.mRenderMode = TPageAbr::eBlend_1;
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}


UXB::~UXB()
{
    if (mCurrentState != UXBState::eExploding || static_cast<s32>(sGnFrame) < mNextStateTimer)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }

    mFlashAnim.VCleanUp();

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
}

void UXB::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        if (mStartingState == UXBState::eDeactivated && mCurrentState != UXBState::eDeactivated)
        {
            Path::TLV_Reset(mTlvInfo, 1, 1u, 0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        else if (mStartingState != UXBState::eDelay || mCurrentState != UXBState::eDeactivated)
        {
            Path::TLV_Reset(mTlvInfo, 0, 1u, 0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        else
        {
            Path::TLV_Reset(mTlvInfo, 1, 1u, 0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

s16 UXB::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbe:
            if (mCurrentState == UXBState::eDeactivated)
            {
                return 0;
            }
            break;

        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eExplosion:
        case ReliveTypes::eShrykull:
            break;

        default:
            return 0;
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    relive_new BaseBomb(
        mXPos,
        mYPos,
        0,
        mSpriteScale);

    mCurrentState = UXBState::eExploding;
    mNextStateTimer = sGnFrame;

    return 1;
}

void UXB::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new BaseBomb(
        mXPos,
        mYPos,
        0,
        mSpriteScale);

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    mCurrentState = UXBState::eExploding;
    mNextStateTimer = sGnFrame;
}

void UXB::VOnPickUpOrSlapped()
{
    if (mCurrentState != UXBState::eExploding)
    {
        if (mCurrentState != UXBState::eDeactivated || mNextStateTimer > static_cast<s32>(sGnFrame))
        {
            if (mRedBlinkCount)
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = sGnFrame + 2;
            }
            else
            {
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    SfxPlayMono(relive::SoundEffects::GreenTick, 35);
                }
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::UXB_Toggle));
                mCurrentState = UXBState::eDeactivated;
                mNextStateTimer = sGnFrame + 10;
            }
        }
        else
        {
            mCurrentState = UXBState::eDelay;
            SetUpdateDelay(6);
            mAnim.Set_Animation_Data(GetAnimRes(AnimId::UXB_Active));
            if (gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                SfxPlayMono(relive::SoundEffects::RedTick, 35);
            }
        }
    }
}

void UXB::VUpdate()
{
    switch (mCurrentState)
    {
        case UXBState::eDelay:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = sGnFrame + 2;
            }
            else if (mNextStateTimer <= static_cast<s32>(sGnFrame))
            {
                mCurrentState = UXBState::eActive;
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_Flash));
                mNextStateTimer = sGnFrame + 2;
            }
            break;

        case UXBState::eActive:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = sGnFrame + 2;
            }
            else if (mNextStateTimer <= static_cast<s32>(sGnFrame))
            {
                if (mRedBlinkCount)
                {
                    mRedBlinkCount--;
                    if (mRedBlinkCount == 0)
                    {
                        mFlashAnim.LoadPal(GetPalRes(PalId::GreenFlash));
                        mIsRed = 0;
                    }
                }
                else
                {
                    mFlashAnim.ReloadPal();

                    mIsRed = 1;

                    mPatternIndex++;

                    if (mPatternIndex >= mPatternLength)
                    {
                        mPatternIndex = 0;
                    }

                    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
                    mRedBlinkCount = (mPattern / static_cast<s32>(pow(10, mPatternLength - mPatternIndex - 1))) % 10;
                }

                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));
                
                if (mIsRed)
                {
                    if (gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            mXPos,
                            mYPos,
                            0))
                    {
                        SfxPlayMono(relive::SoundEffects::RedTick, 35);
                    }
                }
                else if (gMap.Is_Point_In_Current_Camera(
                             mCurrentLevel,
                             mCurrentPath,
                             mXPos,
                             mYPos,
                             0))
                {
                    SfxPlayMono(relive::SoundEffects::GreenTick, 35);
                }
                mCurrentState = UXBState::eDelay;
                mNextStateTimer = sGnFrame + 10; // UXB change color delay
            }
            break;

        case UXBState::eExploding:
            if (static_cast<s32>(sGnFrame) >= mNextStateTimer)
            {
                relive_new BaseBomb(
                    mXPos,
                    mYPos,
                    0,
                    mSpriteScale);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;
    }

    if (mCurrentState != UXBState::eExploding)
    {
        if (EventGet(kEventDeathReset))
        {
            if (mStartingState != UXBState::eDeactivated || mCurrentState == UXBState::eDeactivated)
            {
                if (mStartingState != UXBState::eDelay || mCurrentState != UXBState::eDeactivated)
                {
                    Path::TLV_Reset(mTlvInfo, 0, 1u, 0);
                }
                else
                {
                    Path::TLV_Reset(mTlvInfo, 1, 1u, 0);
                }
            }
            else
            {
                Path::TLV_Reset(mTlvInfo, 1, 1u, 0);
            }
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

s16 UXB::IsColliding()
{
    const PSX_RECT uxbBound = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
        {
            if (pObj->mAnim.mFlags.Get(AnimFlags::eBit3_Render))
            {
                const PSX_RECT objBound = pObj->VGetBoundingRect();

                const s32 objX = FP_GetExponent(pObj->mXPos);
                const s32 objY = FP_GetExponent(pObj->mYPos);

                if (objX > uxbBound.x && objX < uxbBound.w && objY < uxbBound.h + 5 && uxbBound.x <= objBound.w && uxbBound.w >= objBound.x && uxbBound.h >= objBound.y && uxbBound.y <= objBound.h && pObj->mSpriteScale == mSpriteScale)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void UXB::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mFlashAnim.VRender(
            FP_GetExponent(mXPos
                           + FP_FromInteger(pScreenManager->mCamXOff)
                           - pScreenManager->mCamPos->x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(pScreenManager->mCamYOff) - FP_NoFractional(mSpriteScale * FP_FromInteger(12)))
                           - pScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        mFlashAnim.Get_Frame_Rect(&rect);
        pScreenManager->InvalidateRectCurrentIdx(
            rect.x,
            rect.y,
            rect.w,
            rect.h);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

} // namespace AO
