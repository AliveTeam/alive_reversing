#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZBall.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "AnimationCallBacks.hpp"

namespace AO {

ALIVE_VAR(1, 0x9F1DCC, ZBall*, gCenterZBall, nullptr);
ALIVE_VAR(1, 0x9F1DD0, ZBall*, gOutZBall, nullptr);

s32 Animation_OnFrame_ZBallSmacker(BaseGameObject* pObj, s16* pData)
{
    auto pZBall = static_cast<ZBall*>(pObj);
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pBase = gBaseGameObjects->ItemAt(i);
        if (!pBase)
        {
            break;
        }

        if (pBase->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // If the object is within the ZBall rect then smack it
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pBase);

            const PSX_RECT bRect = pAliveObj->VGetBoundingRect();

            if (bRect.x <= (FP_GetExponent(pZBall->mXPos) + pData[2]) && bRect.w >= (FP_GetExponent(pZBall->mXPos) + pData[0]) && bRect.h >= (FP_GetExponent(pZBall->mYPos) + pData[1]) && bRect.y <= (FP_GetExponent(pZBall->mYPos) + pData[3]))
            {
                pAliveObj->VTakeDamage(pZBall);
            }
        }
    }

    return 2;
}

ZBall::ZBall(relive::Path_ZBall* pTlv, const TLVUniqueId& tlvId)
{
    mBaseGameObjectTypeId = ReliveTypes::eZBall;

    mRGB.SetRGB(128, 128, 128);

    switch (pTlv->mSpeed)
    {
        case relive::Path_ZBall::Speed::eNormal:
        {
            const AnimRecord& rec1 = AO::AnimRec(AnimId::Swinging_Ball_Normal);
            u8** ppRes1 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec1.mResourceId, 1, 0);
            Animation_Init(AnimId::Swinging_Ball_Normal, ppRes1);
            break;
        }

        case relive::Path_ZBall::Speed::eFast:
        {
            const AnimRecord& rec2 = AO::AnimRec(AnimId::Swinging_Ball_Fast);
            u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init(AnimId::Swinging_Ball_Fast, ppRes2);
            break;
        }

        case relive::Path_ZBall::Speed::eSlow:
        {
            const AnimRecord& rec3 = AO::AnimRec(AnimId::Swinging_Ball_Slow);
            u8** ppRes3 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec3.mResourceId, 1, 0);
            Animation_Init(AnimId::Swinging_Ball_Slow, ppRes3);
            break;
        }

    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple)
    {
        switch (pTlv->mStartPos)
        {
            case relive::Path_ZBall::StartPos::eCenter:
                mAnim.SetFrame(6u);
                gCenterZBall = this;
                mSoundPitch = -800;
                break;

            case relive::Path_ZBall::StartPos::eOut:
                mAnim.SetFrame(0);
                gOutZBall = this;
                mSoundPitch = -400;
                break;

            case relive::Path_ZBall::StartPos::eIn:
                mAnim.SetFrame(13u);
                mSoundPitch = 0;
                break;
        }

        mAnim.VDecode();
    }

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }

    mTlvInfo = tlvId;
    mAnim.mFnPtrArray = kZBall_Anim_Frame_Fns_4CEBF8;
}

void ZBall::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (gCenterZBall == this || gOutZBall == this)
    {
        if (mAnim.mCurrentFrame == 0 || mAnim.mCurrentFrame == 13)
        {
            SFX_Play_Pitch(SoundEffect::ZBall_62, 50, mSoundPitch, nullptr);
        }
    }

    if (gCenterZBall == this)
    {
        if (mAnim.mCurrentFrame == 3 || mAnim.mCurrentFrame == 16)
        {
            SFX_Play_Pitch(SoundEffect::SackWobble_34, 40, mSoundPitch - 2400, nullptr);
        }
    }

    if (mAnim.mCurrentFrame <= 6 || mAnim.mCurrentFrame >= 19)
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
    }
    else if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeWell_22;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
    }

    mFrameAbove12 = mAnim.mCurrentFrame >= 13;

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
        if (mTlvInfo.IsValid())
        {
            Path::TLV_Reset(mTlvInfo, -1, 0, 0);
        }
    }
}

} // namespace AO
