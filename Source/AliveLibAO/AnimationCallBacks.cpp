#include "AnimationCallBacks.hpp"
#include "Slig.hpp"
#include "Dove.hpp"
#include "Bullet.hpp"
#include "BulletShell.hpp"
#include "Dove.hpp"
#include "Slog.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Particle.hpp"
#include "Abe.hpp"
#include "Primitives.hpp"
#include "Throwable.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

s16* Animation_OnFrame_Slig(BaseGameObject* pObj, s16* pData)
{
    auto pSlig = static_cast<Slig*>(pObj);
    if (pSlig->mBaseGameObjectUpdateDelay != 0)
    {
        return pData + 2;
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

    const FP xOff = pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]);
    const FP yOff = pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[1]);
    if (pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos,
            FP_FromInteger(-640),
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            0);

        New_ShootingFire_Particle_419720(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos - xOff,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            1,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        relive_new BulletShell(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        relive_new Bullet(
            pSlig,
            bulletType,
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            yOff + pSlig->mBaseAnimatedWithPhysicsGameObject_YPos,
            FP_FromInteger(640),
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            0);

        New_ShootingFire_Particle_419720(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos + xOff,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            0,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        relive_new BulletShell(
            pSlig->mBaseAnimatedWithPhysicsGameObject_XPos,
            pSlig->mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
            1,
            pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (pSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
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

    return pData + 2;
}

s16* Animation_OnFrame_ZBallSmacker(BaseGameObject* pObj, s16* pData);

s16* Slog_OnFrame(BaseGameObject* pObj, s16* pData)
{
    auto pSlog = static_cast<Slog*>(pObj);
    if (pSlog->field_10C_pTarget)
    {
        const PSX_RECT targetRect = pSlog->field_10C_pTarget->VGetBoundingRect();
        const PSX_RECT slogRect = pSlog->VGetBoundingRect();

        if (RectsOverlap(slogRect, targetRect))
        {
            if (pSlog->field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale && !pSlog->field_110)
            {
                if (pSlog->field_10C_pTarget->VTakeDamage(pSlog))
                {
                    FP blood_xpos = {};
                    if (pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        blood_xpos = pSlog->mBaseAnimatedWithPhysicsGameObject_XPos - (pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
                    }
                    else
                    {
                        blood_xpos = pSlog->mBaseAnimatedWithPhysicsGameObject_XPos + (pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
                    }

                    const FP blood_ypos = (pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[1])) + pSlog->mBaseAnimatedWithPhysicsGameObject_YPos;

                    relive_new Blood(
                        blood_xpos,
                        blood_ypos - FP_FromInteger(8),
                        (pSlog->mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(2)),
                        FP_FromInteger(0),
                        pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                        50);

                    pSlog->field_110 = 1;

                    SfxPlayMono(SoundEffect::SlogBite_39, 0);
                }
            }
        }
    }

    return pData + 2;
}

const FP_Point kAbeVelTable_4C6608[6] = {
    {FP_FromInteger(3), FP_FromInteger(-14)},
    {FP_FromInteger(10), FP_FromInteger(-10)},
    {FP_FromInteger(15), FP_FromInteger(-8)},
    {FP_FromInteger(10), FP_FromInteger(3)},
    {FP_FromInteger(10), FP_FromInteger(-4)},
    {FP_FromInteger(4), FP_FromInteger(-3)}};

s16* Abe_OnFrame(BaseGameObject* pObj, s16* pData)
{
    auto pAbe = static_cast<Abe*>(pObj);

    FP xVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].x * pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    const FP yVel = kAbeVelTable_4C6608[pAbe->field_19D_throw_direction].y * pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    FP directed_x = {};
    if (sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xVel = -xVel;
        directed_x = -(pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
    }
    else
    {
        directed_x = (pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(pData[0]));
    }

    FP data_y = FP_FromInteger(pData[1]);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            pAbe->mBaseAnimatedWithPhysicsGameObject_XPos,
            pAbe->mBaseAnimatedWithPhysicsGameObject_YPos + data_y,
            pAbe->mBaseAnimatedWithPhysicsGameObject_XPos + directed_x,
            pAbe->mBaseAnimatedWithPhysicsGameObject_YPos + data_y,
            &pLine,
            &hitX,
            &hitY,
            pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? kFgWalls : kBgWalls))
    {
        directed_x = hitX - pAbe->mBaseAnimatedWithPhysicsGameObject_XPos;
        xVel = -xVel;
    }

    if (sActiveHero_507678->field_198_pThrowable)
    {
        sActiveHero_507678->field_198_pThrowable->mBaseAnimatedWithPhysicsGameObject_XPos = directed_x + sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos;
        BaseThrowable* pThrowable = sActiveHero_507678->field_198_pThrowable;
        pThrowable->mBaseAnimatedWithPhysicsGameObject_YPos = (pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale * data_y) + sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos;
        pThrowable->VThrow(xVel, yVel);
        pThrowable->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pAbe->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        sActiveHero_507678->field_198_pThrowable = nullptr;
    }
    return pData + 2;
}

TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[] = {Abe_OnFrame};
TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[] = {Animation_OnFrame_Slig};
TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[] = {Slog_OnFrame};
TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[] = {Animation_OnFrame_ZBallSmacker};

}
