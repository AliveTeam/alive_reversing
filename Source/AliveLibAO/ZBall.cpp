#include "stdafx_ao.h"
#include "ZBall.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "AnimationCallBacks.hpp"
#include "Path.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Map.hpp"

namespace AO {

ZBall* gCenterZBall = nullptr;
ZBall* gOutZBall = nullptr;

// TODO: Pass the whole object because this decides to read 2 points as a rect
void Animation_OnFrame_ZBallSmacker(::BaseGameObject* pObj, u32& idx, const IndexedPoint& points)
{
    auto pZBall = static_cast<ZBall*>(pObj);
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        ::BaseGameObject* pBase = gBaseGameObjects->ItemAt(i);
        if (!pBase)
        {
            break;
        }

        if (pBase->GetIsBaseAliveGameObject())
        {
            // If the object is within the ZBall rect then smack it
            auto pAliveObj = static_cast<::BaseAliveGameObject*>(pBase);

            const PSX_RECT bRect = pAliveObj->VGetBoundingRect();

             const IndexedPoint* pPoints = &points;

            if (bRect.x <= (FP_GetExponent(pZBall->mXPos) + pPoints[1].mPoint.x) &&
                bRect.w >= (FP_GetExponent(pZBall->mXPos) + pPoints[0].mPoint.x) &&
                bRect.h >= (FP_GetExponent(pZBall->mYPos) + pPoints[0].mPoint.y) &&
                bRect.y <= (FP_GetExponent(pZBall->mYPos) + pPoints[1].mPoint.y))
            {
                pAliveObj->VTakeDamage(pZBall);
            }
        }
    }

    // TODO: Debug this and make sure it actually does skip 2 and not 1
    // skip an extra point
    idx++;
}

ZBall::ZBall(relive::Path_ZBall* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eZBall);

    mRGB.SetRGB(128, 128, 128);

    switch (pTlv->mSpeed)
    {
        case relive::Path_ZBall::Speed::eNormal:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Swinging_Ball_Normal));
            Animation_Init(GetAnimRes(AnimId::Swinging_Ball_Normal));
            break;
        }

        case relive::Path_ZBall::Speed::eFast:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Swinging_Ball_Fast));
            Animation_Init(GetAnimRes(AnimId::Swinging_Ball_Fast));
            break;
        }

        case relive::Path_ZBall::Speed::eSlow:
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Swinging_Ball_Slow));
            Animation_Init(GetAnimRes(AnimId::Swinging_Ball_Slow));
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
                GetAnimation().SetFrame(6u);
                gCenterZBall = this;
                mSoundPitch = -800;
                break;

            case relive::Path_ZBall::StartPos::eOut:
                GetAnimation().SetFrame(0);
                gOutZBall = this;
                mSoundPitch = -400;
                break;

            case relive::Path_ZBall::StartPos::eIn:
                GetAnimation().SetFrame(13u);
                mSoundPitch = 0;
                break;
        }

        GetAnimation().VDecode();
    }

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }

    mTlvInfo = tlvId;
    GetAnimation().SetFnPtrArray(gZBall_Anim_Frame_Fns);
}

void ZBall::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    if (gCenterZBall == this || gOutZBall == this)
    {
        if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 13)
        {
            SFX_Play_Pitch(relive::SoundEffects::ZBall, 50, mSoundPitch);
        }
    }

    if (gCenterZBall == this)
    {
        if (GetAnimation().GetCurrentFrame() == 3 || GetAnimation().GetCurrentFrame() == 16)
        {
            SFX_Play_Pitch(relive::SoundEffects::SackWobble, 40, mSoundPitch - 2400);
        }
    }

    if (GetAnimation().GetCurrentFrame() <= 6 || GetAnimation().GetCurrentFrame() >= 19)
    {
        if (GetSpriteScale() == FP_FromInteger(1))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        }
    }
    else if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_22);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeWell_Half_3);
    }

    mFrameAbove12 = GetAnimation().GetCurrentFrame() >= 13;

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        SetDead(true);
        if (mTlvInfo.IsValid())
        {
            Path::TLV_Reset(mTlvInfo);
        }
    }
}

} // namespace AO
