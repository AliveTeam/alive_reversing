#include "AnimationCallBacks.hpp"
#include "Slig.hpp"
#include "Dove.hpp"
#include "Bullet.hpp"
#include "BulletShell.hpp"
#include "Dove.hpp"
#include "Slog.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "Particle.hpp"
#include "Abe.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Throwable.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

s32 Animation_OnFrame_Slig(BaseGameObject* pObj, s16* pData)
{
    auto pSlig = static_cast<Slig*>(pObj);
    if (pSlig->mBaseGameObjectUpdateDelay != 0)
    {
        return 1;
    }

    BulletType bulletType = BulletType::ePossessedSlig_0;
    if (pSlig->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit2_bPossesed))
    {
        pSlig->field_254_prevent_depossession |= 1u;
        bulletType = BulletType::ePossessedSlig_0;
    }
    else
    {
        bulletType = BulletType::eNormalBullet_1;
    }

    const FP xOff = pSlig->mSpriteScale * FP_FromInteger(pData[0]);
    const FP yOff = pSlig->mSpriteScale * FP_FromInteger(pData[1]);
    if (pSlig->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mXPos,
            yOff + pSlig->mYPos,
            FP_FromInteger(-640),
            pSlig->mSpriteScale,
            0);

        New_ShootingFire_Particle_419720(
            pSlig->mXPos - xOff,
            pSlig->mYPos + yOff,
            1,
            pSlig->mSpriteScale);

        relive_new BulletShell(
            pSlig->mXPos,
            pSlig->mYPos + yOff,
            0,
            pSlig->mSpriteScale);
    }
    else
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mXPos,
            yOff + pSlig->mYPos,
            FP_FromInteger(640),
            pSlig->mSpriteScale,
            0);

        New_ShootingFire_Particle_419720(
            pSlig->mXPos + xOff,
            pSlig->mYPos + yOff,
            0,
            pSlig->mSpriteScale);

        relive_new BulletShell(
            pSlig->mXPos,
            pSlig->mYPos + yOff,
            1,
            pSlig->mSpriteScale);
    }

    if (pSlig->mSpriteScale == FP_FromDouble(0.5))
    {
        SfxPlayMono(SoundEffect::SligShoot_6, 85);
    }
    else
    {
        SfxPlayMono(SoundEffect::SligShoot_6, 0);
    }

    EventBroadcast(kEventShooting, pSlig);
    EventBroadcast(kEventLoudNoise, pSlig);

    Dove::All_FlyAway();

    return 1;
}

s32 Animation_OnFrame_ZBallSmacker(BaseGameObject* pObj, s16* pData);

s32 Slog_OnFrame(BaseGameObject* pObj, s16* pData)
{
    auto pSlog = static_cast<Slog*>(pObj);
    if (pSlog->field_10C_pTarget)
    {
        const PSX_RECT targetRect = pSlog->field_10C_pTarget->VGetBoundingRect();
        const PSX_RECT slogRect = pSlog->VGetBoundingRect();

        if (RectsOverlap(slogRect, targetRect))
        {
            if (pSlog->field_10C_pTarget->mSpriteScale == pSlog->mSpriteScale && !pSlog->field_110)
            {
                if (pSlog->field_10C_pTarget->VTakeDamage(pSlog))
                {
                    FP blood_xpos = {};
                    if (pSlog->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        blood_xpos = pSlog->mXPos - (pSlog->mSpriteScale * FP_FromInteger(pData[0]));
                    }
                    else
                    {
                        blood_xpos = pSlog->mXPos + (pSlog->mSpriteScale * FP_FromInteger(pData[0]));
                    }

                    const FP blood_ypos = (pSlog->mSpriteScale * FP_FromInteger(pData[1])) + pSlog->mYPos;

                    relive_new Blood(
                        blood_xpos,
                        blood_ypos - FP_FromInteger(8),
                        (pSlog->mVelX * FP_FromInteger(2)),
                        FP_FromInteger(0),
                        pSlog->mSpriteScale,
                        50);

                    pSlog->field_110 = 1;

                    SfxPlayMono(SoundEffect::SlogBite_39, 0);
                }
            }
        }
    }

    return 1;
}

const FP_Point kAbeVelTable_4C6608[6] = {
    {FP_FromInteger(3), FP_FromInteger(-14)},
    {FP_FromInteger(10), FP_FromInteger(-10)},
    {FP_FromInteger(15), FP_FromInteger(-8)},
    {FP_FromInteger(10), FP_FromInteger(3)},
    {FP_FromInteger(10), FP_FromInteger(-4)},
    {FP_FromInteger(4), FP_FromInteger(-3)}};

s32 Abe_OnFrame(BaseGameObject* pObj, s16* pData)
{
    auto pAbe = static_cast<Abe*>(pObj);

    FP xVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].x * pAbe->mSpriteScale;
    const FP yVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].y * pAbe->mSpriteScale;

    FP directed_x = {};
    if (sActiveHero->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xVel = -xVel;
        directed_x = -(pAbe->mSpriteScale * FP_FromInteger(pData[0]));
    }
    else
    {
        directed_x = (pAbe->mSpriteScale * FP_FromInteger(pData[0]));
    }

    FP data_y = FP_FromInteger(pData[1]);

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
            pAbe->mSpriteScale != FP_FromDouble(0.5) ? kFgWalls : kBgWalls))
    {
        directed_x = hitX - pAbe->mXPos;
        xVel = -xVel;
    }

    if (sActiveHero->field_198_pThrowable)
    {
        sActiveHero->field_198_pThrowable->mXPos = directed_x + sActiveHero->mXPos;
        BaseThrowable* pThrowable = sActiveHero->field_198_pThrowable;
        pThrowable->mYPos = (pAbe->mSpriteScale * data_y) + sActiveHero->mYPos;
        pThrowable->VThrow(xVel, yVel);
        pThrowable->mSpriteScale = pAbe->mSpriteScale;
        sActiveHero->field_198_pThrowable = nullptr;
    }
    return 1;
}

TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[] = {Abe_OnFrame};
TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[] = {Animation_OnFrame_Slig};
TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[] = {Slog_OnFrame};
TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[] = {Animation_OnFrame_ZBallSmacker};

}
