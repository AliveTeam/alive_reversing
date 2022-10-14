#include "Slig.hpp"
#include "Dove.hpp"
#include "Bullet.hpp"
#include "BulletShell.hpp"
#include "Dove.hpp"
#include "Slog.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Particle.hpp"
#include "Abe.hpp"
#include "Throwable.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

void Animation_OnFrame_Slig(::BaseGameObject* pObj, u32&, const IndexedPoint& pData)
{
    auto pSlig = static_cast<Slig*>(pObj);
    if (pSlig->UpdateDelay() != 0)
    {
        return;
    }

    BulletType bulletType = BulletType::ePossessedSlig_0;
    if (pSlig->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit2_bPossesed))
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
    if (pSlig->GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

    Dove::All_FlyAway();
}

void Animation_OnFrame_ZBallSmacker(::BaseGameObject* pObj, u32& idx, const IndexedPoint& pData);

void Slog_OnFrame(::BaseGameObject* pObj, u32&, const IndexedPoint& pData)
{
    auto pSlog = static_cast<Slog*>(pObj);
    if (pSlog->field_10C_pTarget)
    {
        const PSX_RECT targetRect = pSlog->field_10C_pTarget->VGetBoundingRect();
        const PSX_RECT slogRect = pSlog->VGetBoundingRect();

        if (RectsOverlap(slogRect, targetRect))
        {
            if (pSlog->field_10C_pTarget->GetSpriteScale() == pSlog->GetSpriteScale() && !pSlog->field_110)
            {
                if (pSlog->field_10C_pTarget->VTakeDamage(pSlog))
                {
                    FP blood_xpos = {};
                    if (pSlog->GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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

                    pSlog->field_110 = 1;

                    SfxPlayMono(relive::SoundEffects::SlogBite, 0);
                }
            }
        }
    }
}

const FP_Point kAbeVelTable_4C6608[6] = {
    {FP_FromInteger(3), FP_FromInteger(-14)},
    {FP_FromInteger(10), FP_FromInteger(-10)},
    {FP_FromInteger(15), FP_FromInteger(-8)},
    {FP_FromInteger(10), FP_FromInteger(3)},
    {FP_FromInteger(10), FP_FromInteger(-4)},
    {FP_FromInteger(4), FP_FromInteger(-3)}};

void Abe_OnFrame(::BaseGameObject* pObj, u32&, const IndexedPoint& pData)
{
    auto pAbe = static_cast<Abe*>(pObj);

    FP xVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].x * pAbe->GetSpriteScale();
    const FP yVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].y * pAbe->GetSpriteScale();

    FP directed_x = {};
    if (sActiveHero->GetAnimation().mFlags.Get(AnimFlags::eFlipX))
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
    if (sCollisions->Raycast(
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

    if (sActiveHero->field_198_pThrowable)
    {
        sActiveHero->field_198_pThrowable->mXPos = directed_x + sActiveHero->mXPos;
        BaseThrowable* pThrowable = sActiveHero->field_198_pThrowable;
        pThrowable->mYPos = (pAbe->GetSpriteScale() * data_y) + sActiveHero->mYPos;
        pThrowable->VThrow(xVel, yVel);
        pThrowable->SetSpriteScale(pAbe->GetSpriteScale());
        sActiveHero->field_198_pThrowable = nullptr;
    }
}

TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[] = {Abe_OnFrame};
TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[] = {Animation_OnFrame_Slig};
TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[] = {Slog_OnFrame};
TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[] = {Animation_OnFrame_ZBallSmacker};

}
