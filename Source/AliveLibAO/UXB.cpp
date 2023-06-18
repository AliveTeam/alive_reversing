#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "UXB.hpp"
#include "Sfx.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "GroundExplosion.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Grid.hpp"
#include <math.h>
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Map.hpp"
#include "Path.hpp"

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

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_0);

    SetInteractive(true);
    mCurrentState = UXBState::eDelay;

    mPatternLength = pTlv->mPatternLength;
    if (pTlv->mPatternLength < 1 || pTlv->mPatternLength > 4)
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
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        SetScale(Scale::Fg);
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

            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Disabled));

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

            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Disabled));

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

    const FP gridSnap = ScaleToGridSize(GetSpriteScale());
    SetInteractive(true);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromInteger(2));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = mXPos + (gridSnap / FP_FromInteger(2));
    mCollectionRect.h = mYPos;
}

void UXB::InitBlinkAnim()
{
    if (mFlashAnim.Init(GetAnimRes(AnimId::Bomb_RedGreenTick), this))
    {
        mFlashAnim.SetSemiTrans(true);
        mFlashAnim.SetBlending(true);

        mFlashAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
        mFlashAnim.SetSpriteScale(GetSpriteScale());
        mFlashAnim.SetRGB(128, 128, 128);
        mFlashAnim.SetRenderMode(TPageAbr::eBlend_1);
    }
    else
    {
        SetListAddFailed(true);
    }
}

void UXB::VOnPickUpOrSlapped()
{
    if (mCurrentState != UXBState::eExploding)
    {
        if (mCurrentState != UXBState::eDeactivated || mNextStateTimer > sGnFrame)
        {
            if (mRedBlinkCount)
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = MakeTimer(2);
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
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Toggle));
                mCurrentState = UXBState::eDeactivated;

                mNextStateTimer = MakeTimer(10);
            }
        }
        else
        {
            mCurrentState = UXBState::eDelay;
            SetUpdateDelay(6);
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::UXB_Active));
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

UXB::~UXB()
{
    if (mCurrentState != UXBState::eExploding || sGnFrame < mNextStateTimer)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }

    mFlashAnim.VCleanUp();

    SetInteractive(false);
}

void UXB::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        if (mStartingState == UXBState::eDeactivated && mCurrentState != UXBState::eDeactivated)
        {
            Path::TLV_Reset(mTlvInfo, 1, 1, 0);
            SetDead(true);
        }
        else if (mStartingState != UXBState::eDelay || mCurrentState != UXBState::eDeactivated)
        {
            Path::TLV_Reset(mTlvInfo, 0, 1, 0);
            SetDead(true);
        }
        else
        {
            Path::TLV_Reset(mTlvInfo, 1, 1, 0);
            SetDead(true);
        }
    }
}

bool UXB::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return false;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbe:
            if (mCurrentState == UXBState::eDeactivated)
            {
                return false;
            }
            break;

        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eAirExplosion:
        case ReliveTypes::eShrykull:
            break;

        default:
            return false;
    }

    SetDead(true);

    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());

    mCurrentState = UXBState::eExploding;
    mNextStateTimer = sGnFrame;

    return true;
}

void UXB::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
    mCurrentState = UXBState::eExploding;
    SetDead(true);
    mNextStateTimer = sGnFrame;
}

void UXB::VUpdate()
{
    switch (mCurrentState)
    {
        case UXBState::eDelay:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = MakeTimer(2);
            }
            else if (mNextStateTimer <= sGnFrame)
            {
                mCurrentState = UXBState::eActive;
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_Flash));
                mNextStateTimer = MakeTimer(2);
            }
            break;

        case UXBState::eActive:
            if (IsColliding())
            {
                mCurrentState = UXBState::eExploding;
                mNextStateTimer = MakeTimer(2);
            }
            else if (mNextStateTimer <= sGnFrame)
            {
                mFlashAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_RedGreenTick));

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
                mNextStateTimer = MakeTimer(10); // UXB change color delay
            }
            break;

        case UXBState::eExploding:
            if (sGnFrame >= mNextStateTimer)
            {
                relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
                SetDead(true);
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
                    Path::TLV_Reset(mTlvInfo, 0, 1, 0);
                }
                else
                {
                    Path::TLV_Reset(mTlvInfo, 1, 1, 0);
                }
            }
            else
            {
                Path::TLV_Reset(mTlvInfo, 1, 1, 0);
            }
            SetDead(true);
        }
    }
}

s16 UXB::IsColliding()
{
    const PSX_RECT uxbBound = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetCanSetOffExplosives())
        {
            if (pObj->GetAnimation().GetRender())
            {
                const PSX_RECT objBound = pObj->VGetBoundingRect();

                const s32 objX = FP_GetExponent(pObj->mXPos);
                const s32 objY = FP_GetExponent(pObj->mYPos);

                if (objX > uxbBound.x && objX < uxbBound.w && objY < uxbBound.h + 5 && uxbBound.x <= objBound.w && uxbBound.w >= objBound.x && uxbBound.h >= objBound.y && uxbBound.y <= objBound.h && pObj->GetSpriteScale() == GetSpriteScale())
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
                           + FP_FromInteger(gScreenManager->mCamXOff)
                           - gScreenManager->mCamPos->x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(gScreenManager->mCamYOff) - FP_NoFractional(GetSpriteScale() * FP_FromInteger(12)))
                           - gScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

} // namespace AO
