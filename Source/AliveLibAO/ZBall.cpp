#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZBall.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
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

            if (bRect.x <= (FP_GetExponent(pZBall->mBaseAnimatedWithPhysicsGameObject_XPos) + pData[2]) && bRect.w >= (FP_GetExponent(pZBall->mBaseAnimatedWithPhysicsGameObject_XPos) + pData[0]) && bRect.h >= (FP_GetExponent(pZBall->mBaseAnimatedWithPhysicsGameObject_YPos) + pData[1]) && bRect.y <= (FP_GetExponent(pZBall->mBaseAnimatedWithPhysicsGameObject_YPos) + pData[3]))
            {
                pAliveObj->VTakeDamage(pZBall);
            }
        }
    }

    return 2;
}

ZBall::ZBall(Path_ZBall* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eZBall;

    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(128, 128, 128);

    switch (pTlv->mSpeed)
    {
        case Path_ZBall::Speed::eNormal_0:
        {
            const AnimRecord& rec1 = AO::AnimRec(AnimId::Swinging_Ball_Normal);
            u8** ppRes1 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec1.mResourceId, 1, 0);
            Animation_Init(AnimId::Swinging_Ball_Normal, ppRes1);
            break;
        }

        case Path_ZBall::Speed::eFast_1:
        {
            const AnimRecord& rec2 = AO::AnimRec(AnimId::Swinging_Ball_Fast);
            u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init(AnimId::Swinging_Ball_Fast, ppRes2);
            break;
        }

        case Path_ZBall::Speed::eSlow_2:
        {
            const AnimRecord& rec3 = AO::AnimRec(AnimId::Swinging_Ball_Slow);
            u8** ppRes3 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec3.mResourceId, 1, 0);
            Animation_Init(AnimId::Swinging_Ball_Slow, ppRes3);
            break;
        }

    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple)
    {
        switch (pTlv->mStartPos)
        {
            case Path_ZBall::StartPos::eCenter_0:
                mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(6u);
                gCenterZBall = this;
                mSoundPitch = -800;
                break;

            case Path_ZBall::StartPos::eOut_1:
                mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(0);
                gOutZBall = this;
                mSoundPitch = -400;
                break;

            case Path_ZBall::StartPos::eIn_2:
                mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(13u);
                mSoundPitch = 0;
                break;
        }

        mBaseAnimatedWithPhysicsGameObject_Anim.VDecode();
    }

    if (pTlv->mScale != Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }

    mTlvInfo = tlvInfo;
    mBaseAnimatedWithPhysicsGameObject_Anim.mFnPtrArray = kZBall_Anim_Frame_Fns_4CEBF8;
}

void ZBall::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (gCenterZBall == this || gOutZBall == this)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 13)
        {
            SFX_Play_Pitch(SoundEffect::ZBall_62, 50, mSoundPitch, nullptr);
        }
    }

    if (gCenterZBall == this)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 16)
        {
            SFX_Play_Pitch(SoundEffect::SackWobble_34, 40, mSoundPitch - 2400, nullptr);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame <= 6 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 19)
    {
        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_22;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeWell_Half_3;
    }

    mFrameAbove12 = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame >= 13;

    if (!gMap.Is_Point_In_Current_Camera(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
        if (mTlvInfo != -1)
        {
            gMap.TLV_Reset(mTlvInfo, -1, 0, 0);
        }
    }
}

} // namespace AO
