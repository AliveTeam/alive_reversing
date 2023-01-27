#include "AnimationCallBacks.hpp"
#include "Abe.hpp"    // For anim call back table
#include "Slig.hpp"   // For slig frame call back
#include "Fleech.hpp" // For fleech frame call back
#include "../relive_lib/Events.hpp"
#include "Slurg.hpp"
#include "Slog.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/Collisions.hpp"

void Animation_OnFrame_Common_Null(BaseGameObject*, u32&, const IndexedPoint&)
{

}

void Animation_OnFrame_Null(BaseGameObject*, u32&, const IndexedPoint&)
{

}

void Animation_OnFrame_Common(BaseGameObject* pObjPtr, u32&, const IndexedPoint& point)
{
    auto pObj = static_cast<BaseAliveGameObject*>(pObjPtr);
    AnimResource ppAnimData = ResourceManagerWrapper::LoadAnimation(AnimId::Dust_Particle);

    FP xOff = {};
    if (pObj->GetAnimation().GetFlipX())
    {
        xOff = -(pObj->GetSpriteScale() * FP_FromInteger(point.mPoint.x));
    }
    else
    {
        xOff = (pObj->GetSpriteScale() * FP_FromInteger(point.mPoint.x));
    }

    FP xpos = xOff + pObj->mXPos;
    FP ypos = (pObj->GetSpriteScale() * FP_FromInteger(point.mPoint.y)) + pObj->mYPos + FP_FromInteger(25);
    if (!pObj->BaseAliveGameObjectCollisionLine)
    {
        return;
    }

    const s8 count = gSlurgStepWatchPointsCount[gSlurgStepWatchPointsIdx];
    if (count < 5)
    {
        Slurg_Step_Watch_Points* pPoints = &gSlurgStepWatchPoints[gSlurgStepWatchPointsIdx];
        pPoints->mPoints[count].x = FP_GetExponent(xpos);
        pPoints->mPoints[count].y = pObj->BaseAliveGameObjectCollisionLine->mRect.y - 5;
        gSlurgStepWatchPointsCount[gSlurgStepWatchPointsIdx] = count + 1;
    }

    if (!IsActiveHero(pObj))
    {
        return;
    }

    if (pObj->GetSpriteScale() == FP_FromDouble(0.5))
    {
        ypos -= FP_FromInteger(14);
    }

    if (pObj->mCurrentMotion == eAbeMotions::Motion_71_Knockback_455090 && pObj->GetSpriteScale() == FP_FromDouble(0.5))
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
        pPartical->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

        if (pObj->GetScale() == Scale::Fg)
        {
            pPartical->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
        }
        else
        {
            pPartical->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        }

        pPartical->mRGB.SetRGB(45, 35, 5);

        switch (pObj->mCurrentMotion)
        {
            case eAbeMotions::Motion_1_WalkLoop_44FBA0:
                pPartical->SetSpriteScale(FP_FromDouble(0.3) * pObj->GetSpriteScale());
                break;

            case eAbeMotions::Motion_40_SneakLoop_450550:
                pPartical->SetSpriteScale(FP_FromInteger(0));
                break;

            case eAbeMotions::Motion_71_Knockback_455090:
                pPartical->SetSpriteScale(FP_FromInteger(1) * pObj->GetSpriteScale());
                break;

            default:
                pPartical->SetSpriteScale(FP_FromDouble(0.6) * pObj->GetSpriteScale());
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
        ALIVE_FATAL("object type in Animation_OnFrame_FlyingSlig isn't flying slig %d", static_cast<u32>(pObj->Type()));
    }

    if (pObj->mHealth <= FP_FromInteger(0))
    {
        return;
    }

    // flying slig: kVaporResID
    FP xOff = {};
    if (pObj->GetAnimation().GetFlipX())
    {
        xOff = -(pObj->GetSpriteScale() * FP_FromInteger(point.mPoint.x));
    }
    else
    {
        xOff = (pObj->GetSpriteScale() * FP_FromInteger(point.mPoint.x));
    }

    FP xpos = xOff + pObj->mXPos;
    FP ypos = (pObj->GetSpriteScale() * (FP_FromInteger(point.mPoint.y) + FP_FromInteger(25))) + pObj->mYPos;

    if (EventGet(kEventDeathReset))
    {
        pObj->SetDead(true);
    }

    auto pParticle = relive_new Particle(xpos, ypos, pObj->GetAnimRes(AnimId::Vaporize_Particle));
    if (pParticle)
    {
        pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
        pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
        pParticle->mRGB.SetRGB(64, 64, 64);
        pParticle->SetSpriteScale(FP_FromDouble(0.6) * pObj->GetSpriteScale());
    }
}

void Animation_OnFrame_Slog(BaseGameObject* pObjPtr, u32&, const IndexedPoint& point)
{
    auto pSlog = static_cast<Slog*>(pObjPtr);
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(pSlog->mTargetId));
    if (!pTarget)
    {
        return;
    }

    if ((pTarget->Type() == ReliveTypes::eAbe && pTarget->mCurrentMotion == eAbeMotions::Motion_68_ToOffScreenHoist_454B80) || pSlog->GetElectrocuted())
    {
        return;
    }

    const PSX_RECT bTargetRect = pTarget->VGetBoundingRect();
    const PSX_RECT bSlogRect = pSlog->VGetBoundingRect();

    if (bSlogRect.x > bTargetRect.w || bSlogRect.w < bTargetRect.x || bSlogRect.h < bTargetRect.y || bSlogRect.y > bTargetRect.h || pTarget->GetSpriteScale() != FP_FromInteger(1) || pSlog->mBitingTarget)
    {
        return;
    }

    if (!pTarget->VTakeDamage(pSlog))
    {
        return;
    }

    FP bloodX = {};
    if (pSlog->GetAnimation().GetFlipX())
    {
        bloodX = pSlog->mXPos - (pSlog->GetSpriteScale() * FP_FromInteger(point.mPoint.x));
    }
    else
    {
        bloodX = (pSlog->GetSpriteScale() * FP_FromInteger(point.mPoint.x)) + pSlog->mXPos;
    }

    const FP bloodY = (pSlog->GetSpriteScale() * FP_FromInteger(point.mPoint.y)) + pSlog->mYPos;

    relive_new Blood(
        bloodX,
        bloodY - FP_FromInteger(8),
        pSlog->mVelX * FP_FromInteger(2),
        FP_FromInteger(0),
        pSlog->GetSpriteScale(),
        50);

    pSlog->mBitingTarget = 1;
    SfxPlayMono(relive::SoundEffects::SlogBite, 0);

    return;
}

// TODO: Array is possibly bigger, called by AnimationEx::Invoke_CallBacks
TFrameCallBackType gAbe_Anim_Frame_Fns[5] = {
    Animation_OnFrame_Common_Null,
    Animation_OnFrame_Null,
    Animation_OnFrame_Abe,
    Animation_OnFrame_Common,
    Animation_OnFrame_FlyingSlig};

TFrameCallBackType gSlig_Anim_Frame_Fns[4] = {
    Animation_OnFrame_Common_Null,
    Animation_OnFrame_Null,
    Animation_OnFrame_Slig,
    Animation_OnFrame_Common};

TFrameCallBackType gSlog_Anim_Frame_Fns[2] = {
    Animation_OnFrame_Slog,
    Animation_OnFrame_Null};

TFrameCallBackType gFlyingSlig_Anim_Frames_Fns[3] = {
    Animation_OnFrame_Common_Null,
    Animation_OnFrame_Null,
    Animation_OnFrame_FlyingSlig};

TFrameCallBackType gFleech_Anim_Frame_Fns[3] = {
    Animation_OnFrame_Common_Null,
    Animation_OnFrame_Null,
    Animation_OnFrame_Fleech};
