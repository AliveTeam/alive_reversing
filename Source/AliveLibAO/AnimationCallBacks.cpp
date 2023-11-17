#include "Slig.hpp"
#include "Dove.hpp"
#include "Bullet.hpp"
#include "../relive_lib/GameObjects/BulletShell.hpp"
#include "Dove.hpp"
#include "Slog.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Abe.hpp"
#include "Throwable.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

void Animation_OnFrame_Slig(::BaseGameObject* pObj, u32&, const IndexedPoint& pData)
{
    auto pSlig = static_cast<Slig*>(pObj);
    if (pSlig->UpdateDelay() != 0)
    {
        return;
    }

    BulletType bulletType = BulletType::ePossessedSlig_0;
    if (pSlig->GetPossessed())
    {
        pSlig->mPreventDepossession |= 1u;
        bulletType = BulletType::ePossessedSlig_0;
    }
    else
    {
        bulletType = BulletType::eNormalBullet_1;
    }

    const FP xOff = pSlig->GetSpriteScale() * FP_FromInteger(pData.mPoint.x);
    const FP yOff = pSlig->GetSpriteScale() * FP_FromInteger(pData.mPoint.y);
    if (pSlig->GetAnimation().GetFlipX())
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mXPos,
            yOff + pSlig->mYPos,
            FP_FromInteger(-640),
            pSlig->GetSpriteScale(),
            0);

        New_ShootingFire_Particle(
            pSlig->mXPos - xOff,
            pSlig->mYPos + yOff,
            1,
            pSlig->GetSpriteScale());

        relive_new BulletShell(
            pSlig->mXPos,
            pSlig->mYPos + yOff,
            0,
            pSlig->GetSpriteScale());
    }
    else
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mXPos,
            yOff + pSlig->mYPos,
            FP_FromInteger(640),
            pSlig->GetSpriteScale(),
            0);

        New_ShootingFire_Particle(
            pSlig->mXPos + xOff,
            pSlig->mYPos + yOff,
            0,
            pSlig->GetSpriteScale());

        relive_new BulletShell(
            pSlig->mXPos,
            pSlig->mYPos + yOff,
            1,
            pSlig->GetSpriteScale());
    }

    if (pSlig->GetSpriteScale() == FP_FromDouble(0.5))
    {
        SfxPlayMono(relive::SoundEffects::SligShoot, 85);
    }
    else
    {
        SfxPlayMono(relive::SoundEffects::SligShoot, 0);
    }

    EventBroadcast(kEventShooting, pSlig);
    EventBroadcast(kEventLoudNoise, pSlig);

    Dove::All_FlyAway(false);
}

void Animation_OnFrame_ZBallSmacker(::BaseGameObject* pObj, u32& idx, const IndexedPoint& pData);

void Slog_OnFrame(::BaseGameObject* pObj, u32&, const IndexedPoint& pData)
{
    auto pSlog = static_cast<Slog*>(pObj);
    if (pSlog->mTarget)
    {
        const PSX_RECT targetRect = pSlog->mTarget->VGetBoundingRect();
        const PSX_RECT slogRect = pSlog->VGetBoundingRect();

        if (RectsOverlap(slogRect, targetRect))
        {
            if (pSlog->mTarget->GetSpriteScale() == pSlog->GetSpriteScale() && !pSlog->mBitingTarget)
            {
                if (pSlog->mTarget->VTakeDamage(pSlog))
                {
                    FP blood_xpos = {};
                    if (pSlog->GetAnimation().GetFlipX())
                    {
                        blood_xpos = pSlog->mXPos - (pSlog->GetSpriteScale() * FP_FromInteger(pData.mPoint.x));
                    }
                    else
                    {
                        blood_xpos = pSlog->mXPos + (pSlog->GetSpriteScale() * FP_FromInteger(pData.mPoint.x));
                    }

                    const FP blood_ypos = (pSlog->GetSpriteScale() * FP_FromInteger(pData.mPoint.y)) + pSlog->mYPos;

                    relive_new Blood(
                        blood_xpos,
                        blood_ypos - FP_FromInteger(8),
                        (pSlog->mVelX * FP_FromInteger(2)),
                        FP_FromInteger(0),
                        pSlog->GetSpriteScale(),
                        50);

                    pSlog->mBitingTarget = 1;

                    SfxPlayMono(relive::SoundEffects::SlogBite, 0);
                }
            }
        }
    }
}

static const FP_Point sThrowVelocities[6] = {
    {FP_FromInteger(3), FP_FromInteger(-14)},
    {FP_FromInteger(10), FP_FromInteger(-10)},
    {FP_FromInteger(15), FP_FromInteger(-8)},
    {FP_FromInteger(10), FP_FromInteger(3)},
    {FP_FromInteger(10), FP_FromInteger(-4)},
    {FP_FromInteger(4), FP_FromInteger(-3)}};

void Abe_OnFrame(::BaseGameObject* pObj, u32&, const IndexedPoint& pData)
{
    auto pAbe = static_cast<Abe*>(pObj);

    FP xVel = sThrowVelocities[pAbe->mThrowDirection].x * pAbe->GetSpriteScale();
    const FP yVel = sThrowVelocities[pAbe->mThrowDirection].y * pAbe->GetSpriteScale();

    FP directed_x = {};
    if (sActiveHero->GetAnimation().GetFlipX())
    {
        xVel = -xVel;
        directed_x = -(pAbe->GetSpriteScale() * FP_FromInteger(pData.mPoint.x));
    }
    else
    {
        directed_x = (pAbe->GetSpriteScale() * FP_FromInteger(pData.mPoint.x));
    }

    FP data_y = FP_FromInteger(pData.mPoint.y);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (gCollisions->Raycast(
            pAbe->mXPos,
            pAbe->mYPos + data_y,
            pAbe->mXPos + directed_x,
            pAbe->mYPos + data_y,
            &pLine,
            &hitX,
            &hitY,
            pAbe->GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls))
    {
        directed_x = hitX - pAbe->mXPos;
        xVel = -xVel;
    }

    if (sActiveHero->mThrowable)
    {
        sActiveHero->mThrowable->mXPos = directed_x + sActiveHero->mXPos;
        BaseThrowable* pThrowable = sActiveHero->mThrowable;
        pThrowable->mYPos = (pAbe->GetSpriteScale() * data_y) + sActiveHero->mYPos;
        pThrowable->VThrow(xVel, yVel);
        pThrowable->SetSpriteScale(pAbe->GetSpriteScale());
        sActiveHero->mThrowable = nullptr;
    }
}

TFrameCallBackType gAbe_Anim_Frame_Fns[] = {Abe_OnFrame};
TFrameCallBackType gSlig_Anim_Frame_Fns[] = {Animation_OnFrame_Slig};
TFrameCallBackType gSlog_Anim_Frame_Fns[] = {Slog_OnFrame};
TFrameCallBackType gZBall_Anim_Frame_Fns[] = {Animation_OnFrame_ZBallSmacker};

}
