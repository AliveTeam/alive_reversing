#include "AnimationCallBacks.hpp"
#include "Abe.hpp"    // For anim call back table
#include "Slig.hpp"   // For slig frame call back
#include "Fleech.hpp" // For fleech frame call back
#include "../relive_lib/Events.hpp"
#include "Slurg.hpp"
#include "Slog.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibCommon/FatalError.hpp"

void Animation_OnFrame_Common_Null_455F40(BaseGameObject*, u32&, const IndexedPoint&)
{

}

void Animation_OnFrame_Null_455F60(BaseGameObject*, u32&, const IndexedPoint&)
{

}

void Animation_OnFrame_Common_4561B0(BaseGameObject* pObjPtr, u32&, const IndexedPoint& point)
{
    auto pObj = static_cast<BaseAliveGameObject*>(pObjPtr);
    AnimResource ppAnimData = ResourceManagerWrapper::LoadAnimation(AnimId::Dust_Particle);

    FP xOff = {};
    if (pObj->mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        xOff = -(pObj->mSpriteScale * FP_FromInteger(point.mPoint.x));
    }
    else
    {
        xOff = (pObj->mSpriteScale * FP_FromInteger(point.mPoint.x));
    }

    FP xpos = xOff + pObj->mXPos;
    FP ypos = (pObj->mSpriteScale * FP_FromInteger(point.mPoint.y)) + pObj->mYPos + FP_FromInteger(25);
    if (!pObj->BaseAliveGameObjectCollisionLine)
    {
        return;
    }

    const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
    if (count < 5)
    {
        Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
        pPoints->mPoints[count].x = FP_GetExponent(xpos);
        pPoints->mPoints[count].y = pObj->BaseAliveGameObjectCollisionLine->mRect.y - 5;
        sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
    }

    if (!IsActiveHero(pObj))
    {
        return;
    }

    if (pObj->mSpriteScale == FP_FromDouble(0.5))
    {
        ypos -= FP_FromInteger(14);
    }

    if (pObj->mCurrentMotion == eAbeMotions::Motion_71_Knockback_455090 && pObj->mSpriteScale == FP_FromDouble(0.5))
    {
        ypos += FP_FromInteger(5);
    }

    if (pObj->mCurrentMotion == eAbeMotions::Motion_1_WalkLoop_44FBA0)
    {
        ypos -= FP_FromInteger(5);
    }

    auto pPartical = relive_new Particle(xpos, ypos, ppAnimData);
    if (pPartical)
    {
        pPartical->mAnim.mRenderMode = TPageAbr::eBlend_1;

        if (pObj->mScale == Scale::Fg)
        {
            pPartical->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pPartical->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pPartical->mRGB.SetRGB(45, 35, 5);

        switch (pObj->mCurrentMotion)
        {
            case eAbeMotions::Motion_1_WalkLoop_44FBA0:
                pPartical->mSpriteScale = FP_FromDouble(0.3) * pObj->mSpriteScale;
                break;

            case eAbeMotions::Motion_40_SneakLoop_450550:
                pPartical->mSpriteScale = FP_FromInteger(0);
                break;

            case eAbeMotions::Motion_71_Knockback_455090:
                pPartical->mSpriteScale = FP_FromInteger(1) * pObj->mSpriteScale;
                break;

            default:
                pPartical->mSpriteScale = FP_FromDouble(0.6) * pObj->mSpriteScale;
                break;
        }
    }
    return;
}

void Animation_OnFrame_FlyingSlig(BaseGameObject* pObjPtr, u32&, const IndexedPoint& point)
{
    auto pObj = static_cast<BaseAliveGameObject*>(pObjPtr);

    if (pObj->Type() != ReliveTypes::eFlyingSlig)
    {
        LOG_ERROR("object type in Animation_OnFrame_FlyingSlig isn't flying slig");
        ALIVE_FATAL("got wrong type id");
    }

    if (pObj->mHealth <= FP_FromInteger(0))
    {
        return;
    }

    // flying slig: kVaporResID
    FP xOff = {};
    if (pObj->mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        xOff = -(pObj->mSpriteScale * FP_FromInteger(point.mPoint.x));
    }
    else
    {
        xOff = (pObj->mSpriteScale * FP_FromInteger(point.mPoint.x));
    }

    FP xpos = xOff + pObj->mXPos;
    FP ypos = (pObj->mSpriteScale * (FP_FromInteger(point.mPoint.y) + FP_FromInteger(25))) + pObj->mYPos;

    if (EventGet(kEventDeathReset))
    {
        pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    auto pParticle = relive_new Particle(xpos, ypos, pObj->GetAnimRes(AnimId::Vaporize_Particle));
    if (pParticle)
    {
        pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
        pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        pParticle->mRGB.SetRGB(64, 64, 64);
        pParticle->mSpriteScale = FP_FromDouble(0.6) * pObj->mSpriteScale;
    }
}

void Animation_OnFrame_Slog_4C3030(BaseGameObject* pObjPtr, u32&, const IndexedPoint& point)
{
    auto pSlog = static_cast<Slog*>(pObjPtr);
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(pSlog->field_118_target_id));
    if (!pTarget)
    {
        return;
    }

    if ((pTarget->Type() == ReliveTypes::eAbe && pTarget->mCurrentMotion == eAbeMotions::Motion_68_ToOffScreenHoist_454B80) || pSlog->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        return;
    }

    const PSX_RECT bTargetRect = pTarget->VGetBoundingRect();
    const PSX_RECT bSlogRect = pSlog->VGetBoundingRect();

    if (bSlogRect.x > bTargetRect.w || bSlogRect.w < bTargetRect.x || bSlogRect.h < bTargetRect.y || bSlogRect.y > bTargetRect.h || pTarget->mSpriteScale != FP_FromInteger(1) || pSlog->field_11C_biting_target)
    {
        return;
    }

    if (!pTarget->VTakeDamage(pSlog))
    {
        return;
    }

    FP bloodX = {};
    if (pSlog->mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        bloodX = pSlog->mXPos - (pSlog->mSpriteScale * FP_FromInteger(point.mPoint.x));
    }
    else
    {
        bloodX = (pSlog->mSpriteScale * FP_FromInteger(point.mPoint.x)) + pSlog->mXPos;
    }

    const FP bloodY = (pSlog->mSpriteScale * FP_FromInteger(point.mPoint.y)) + pSlog->mYPos;

    relive_new Blood(
        bloodX,
        bloodY - FP_FromInteger(8),
        pSlog->mVelX * FP_FromInteger(2),
        FP_FromInteger(0),
        pSlog->mSpriteScale,
        50);

    pSlog->field_11C_biting_target = 1;
    SfxPlayMono(relive::SoundEffects::SlogBite, 0);

    return;
}

// TODO: Array is possibly bigger, called by AnimationEx::Invoke_CallBacks
TFrameCallBackType kAbe_Anim_Frame_Fns_55EF98[5] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_Abe_455F80,
    Animation_OnFrame_Common_4561B0,
    Animation_OnFrame_FlyingSlig};

TFrameCallBackType kSlig_Anim_Frame_Fns_55EFAC[4] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_Slig_4C0600,
    Animation_OnFrame_Common_4561B0};

TFrameCallBackType kSlog_Anim_Frame_Fns_55EFBC[2] = {
    Animation_OnFrame_Slog_4C3030,
    Animation_OnFrame_Null_455F60};

TFrameCallBackType kFlyingSlig_Anim_Frames_Fns_55EFC4[3] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_FlyingSlig};

TFrameCallBackType kFleech_Anim_Frame_Fns_55EFD0[3] = {
    Animation_OnFrame_Common_Null_455F40,
    Animation_OnFrame_Null_455F60,
    Animation_OnFrame_Fleech_449A60};
