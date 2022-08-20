#include "stdafx_ao.h"
#include "Function.hpp"
#include "UXB.hpp"
#include "Sfx.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "BaseBomb.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include <math.h>

namespace AO {

UXB::UXB(Path_UXB* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eUXB;

    const AnimRecord rec = AO::AnimRec(AnimId::UXB_Active);
    u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::UXB_Active, ppRes2);

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

    if (pTlv->mScale == Scale_short::eHalf_1)
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

    if (pTlv->field_1_unknown) // Stores the activated/deactivated state for UXB
    {
        if (pTlv->mStartState == UXBStartState::eOn)
        {
            u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0);
            mFlashAnim.LoadPal(ppRes, 0);
            mIsRed = 0;

            mFlashAnim.Set_Animation_Data(AnimId::Bomb_RedGreenTick, nullptr);

            if (gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                SfxPlayMono(SoundEffect::GreenTick_3, 35, 0);
            }

            mAnim.Set_Animation_Data(AnimId::UXB_Disabled, nullptr);

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
        if (pTlv->mStartState == UXBStartState::eOn)
        {
            mStartingState = UXBState::eDelay;
        }
        else
        {
            u8** ppPal = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0);
            mFlashAnim.LoadPal(ppPal, 0);
            mIsRed = 0;

            mFlashAnim.Set_Animation_Data(AnimId::Bomb_RedGreenTick, nullptr);

            mAnim.Set_Animation_Data(AnimId::UXB_Disabled, nullptr);

            mStartingState = UXBState::eDeactivated;
            mCurrentState = UXBState::eDeactivated;
        }
    }

    mXPos = FP_FromInteger(pTlv->mTopLeft.x + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y + 24);

    mTlvInfo = tlvInfo;
    mNextStateTimer = sGnFrame;
    mDisabledResources = static_cast<s16>(pTlv->mDisabledResources);

    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 1, 0);

    if (!(mDisabledResources & 1))
    {
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
    }

    if (!(mDisabledResources & 4))
    {
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 1, 0);
    }

    if (!(mDisabledResources & 2))
    {
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 1, 0);
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mIsRed = 0;
        mRGB.SetRGB(80, 90, 110);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 1, 0);
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 1, 0);
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
    const AnimRecord& tickRec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, tickRec.mResourceId, 1, 0);
    if (mFlashAnim.Init(
            AnimId::Bomb_RedGreenTick,
            this,
            ppRes))
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

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0));

    if (!(mDisabledResources & 1))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0));
    }

    if (!(mDisabledResources & 4))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 0, 0));
    }

    if (!(mDisabledResources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 0, 0));

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

    switch (pFrom->mBaseGameObjectTypeId)
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
                mFlashAnim.Set_Animation_Data(AnimId::Bomb_RedGreenTick, nullptr);
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    SfxPlayMono(SoundEffect::GreenTick_3, 35, 0);
                }
                mAnim.Set_Animation_Data(AnimId::UXB_Toggle, nullptr);
                mCurrentState = UXBState::eDeactivated;
                mNextStateTimer = sGnFrame + 10;
            }
        }
        else
        {
            mCurrentState = UXBState::eDelay;
            mBaseGameObjectUpdateDelay = 6;
            mAnim.Set_Animation_Data(AnimId::UXB_Active, nullptr);
            if (gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                SfxPlayMono(SoundEffect::RedTick_4, 35, 0);
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
                mFlashAnim.Set_Animation_Data(AnimId::Bomb_Flash, nullptr);
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
                        mFlashAnim.LoadPal(ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kGrenflshAOResID, 0, 0), 0);
                        mIsRed = 0;
                    }
                }
                else
                {
                    const FrameInfoHeader* pFrameInfo = mFlashAnim.Get_FrameHeader(-1);

                    const FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*mFlashAnim.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

                    mFlashAnim.LoadPal(
                        mFlashAnim.field_20_ppBlock,
                        pFrameHeader->field_0_clut_offset);

                    mIsRed = 1;

                    mPatternIndex++;

                    if (mPatternIndex >= mPatternLength)
                    {
                        mPatternIndex = 0;
                    }

                    // Single out a single digit, and use that digit as the new amount of red blinks before a green one.
                    mRedBlinkCount = (mPattern / static_cast<s32>(pow(10, mPatternLength - mPatternIndex - 1))) % 10;
                }

                mFlashAnim.Set_Animation_Data(AnimId::Bomb_RedGreenTick, nullptr);
                
                if (mIsRed)
                {
                    if (gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            mXPos,
                            mYPos,
                            0))
                    {
                        SfxPlayMono(SoundEffect::RedTick_4, 35, 0);
                    }
                }
                else if (gMap.Is_Point_In_Current_Camera(
                             mCurrentLevel,
                             mCurrentPath,
                             mXPos,
                             mYPos,
                             0))
                {
                    SfxPlayMono(SoundEffect::GreenTick_3, 35, 0);
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
