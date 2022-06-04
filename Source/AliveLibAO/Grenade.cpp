#include "stdafx_ao.h"
#include "Function.hpp"
#include "Grenade.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Grid.hpp"

namespace AO {

ALIVE_VAR(1, 0x5076EC, s16, gInfiniteGrenades_5076EC, 0);

Grenade::Grenade(FP xpos, FP ypos, s16 numGrenades)
    : BaseThrowable()
{
    field_10E_bDead = 0;
    mBaseGameObjectTypeId = ReliveTypes::eGrenade;

    const AnimRecord& rec = AO::AnimRec(AnimId::Grenade);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
    field_10_anim.mRenderMode = TPageAbr::eBlend_0;

    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    field_120_xpos = xpos;

    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    field_124_ypos = ypos;

    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_10C_count = numGrenades;

    if (numGrenades > 0)
    {
        field_110_state = States::eFallingToBeCollected_0;
    }
    else
    {
        field_110_state = States::eCountingDown_3;
        field_112_explode_timer = 90;
    }

    field_118 = 0;
    field_11C = 0;
}

Grenade::~Grenade()
{
    if (field_11C)
    {
        field_11C->mBaseGameObjectRefCount--;
    }

    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (mLiftPoint)
        {
            mLiftPoint->VRemove(this);
            mLiftPoint->mBaseGameObjectRefCount--;
            mLiftPoint = nullptr;
        }

        if (gpThrowableArray_50E26C)
        {
            s16 count = 0;
            if (field_10C_count >= 1u)
            {
                count = field_10C_count;
            }
            else
            {
                count = 1;
            }
            gpThrowableArray_50E26C->Remove(count);
        }
    }
}

void Grenade::AddToPlatform()
{
    BaseAddToPlatform();
}

void Grenade::VTimeToExplodeRandom()
{
    field_112_explode_timer -= Math_NextRandom() % 16;
}

void Grenade::VThrow(FP velX, FP velY)
{
    field_10_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    field_B4_velx = velX;
    field_B8_vely = velY;

    if (field_10C_count == 0)
    {
        field_110_state = States::eFalling_4;
    }
}

void Grenade::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Grenade::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    switch (field_110_state)
    {
        case States::eFallingToBeCollected_0:
            if (!InTheAir())
            {
                mBaseAliveGameObjectCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
                mBaseAliveGameObjectCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_BC_sprite_scale);
                mBaseAliveGameObjectCollectionRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
                mBaseAliveGameObjectCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;

                mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
                field_110_state = States::eWaitToBeCollected_1;
            }
            break;

        case States::eWaitToBeCollected_1:
            if (field_B4_velx < FP_FromInteger(0))
            {
                field_B4_velx = FP_FromInteger(0);
            }

            if (FP_Abs(field_B4_velx) >= FP_FromInteger(1))
            {
                if (field_B4_velx <= FP_FromInteger(0))
                {
                    field_B4_velx += FP_FromDouble(0.01);
                }
                else
                {
                    field_B4_velx -= FP_FromDouble(0.01);
                }

                const auto oldLine = field_114_pCollisionLine;
                field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_B4_velx);
                if (mLiftPoint)
                {
                    if (field_114_pCollisionLine != oldLine)
                    {
                        mLiftPoint->VRemove(this);
                        mLiftPoint->mBaseGameObjectRefCount--;
                        mLiftPoint = nullptr;
                    }
                }

                if (!field_114_pCollisionLine)
                {
                    field_110_state = States::eFallingToBeCollected_0;
                }
            }
            else if (abs(SnapToXGrid(field_BC_sprite_scale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) - FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) > 1)
            {
                const auto oldLine = field_114_pCollisionLine;
                field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_B4_velx);
                if (mLiftPoint)
                {
                    if (field_114_pCollisionLine != oldLine)
                    {
                        mLiftPoint->VRemove(this);
                        mLiftPoint->mBaseGameObjectRefCount--;
                        mLiftPoint = nullptr;
                    }
                }

                if (!field_114_pCollisionLine)
                {
                    field_110_state = States::eFalling_4;
                }
            }
            else
            {
                field_B4_velx = FP_FromInteger(0);

                mBaseAliveGameObjectCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
                mBaseAliveGameObjectCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_BC_sprite_scale);
                mBaseAliveGameObjectCollectionRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
                mBaseAliveGameObjectCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;

                mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
                field_110_state = States::eDoesNothing_2;
            }
            break;

        case States::eDoesNothing_2:
            break;

        case States::eCountingDown_3:
            BlowUpAfterCountdown();
            break;

        case States::eFalling_4:
            if (InTheAir())
            {
                if (!BlowUpAfterCountdown())
                {
                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};

                    VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Grenade::OnCollision_BounceOff);
                }
            }
            else
            {
                field_110_state = States::eHitGround_5;
            }
            break;

        case States::eHitGround_5:
        {
            field_B4_velx = FP_FromRaw(field_B4_velx.fpValue / 2);

            const auto oldLine = field_114_pCollisionLine;
            field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_B4_velx);
            if (mLiftPoint)
            {
                if (field_114_pCollisionLine != oldLine)
                {
                    mLiftPoint->VRemove(this);
                    mLiftPoint->mBaseGameObjectRefCount--;
                    mLiftPoint = nullptr;
                }
            }

            if (!field_114_pCollisionLine)
            {
                field_110_state = States::eFalling_4;
            }

            BlowUpAfterCountdown();
            break;
        }

        case States::eWaitForExplodeEnd_6:
            if (field_11C->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_110_state = States::eExploded_7;
                field_11C->mBaseGameObjectRefCount--;
                field_11C = nullptr;
            }
            break;

        case States::eExploded_7:
            mBaseGameObjectFlags.Set(Options::eDead);
            break;

        default:
            return;
    }
}

void Grenade::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;

        if (field_110_state == States::eWaitToBeCollected_1 || field_110_state == States::eDoesNothing_2)
        {
            field_110_state = States::eFallingToBeCollected_0;
        }
        else if (field_110_state != States::eWaitForExplodeEnd_6)
        {
            field_110_state = States::eFalling_4;
        }
    }
}

s16 Grenade::InTheAir()
{
    field_120_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_124_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    field_B8_vely += FP_FromInteger(1);

    mBaseAnimatedWithPhysicsGameObject_XPos += field_B4_velx;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_B8_vely;

    u16 result = 0;
    mBaseAnimatedWithPhysicsGameObject_XPos = CamX_VoidSkipper(mBaseAnimatedWithPhysicsGameObject_XPos, field_B4_velx, 8, &result);
    mBaseAnimatedWithPhysicsGameObject_YPos = CamY_VoidSkipper(mBaseAnimatedWithPhysicsGameObject_YPos, field_B8_vely, 8, &result);

    FP hitX = {};
    FP hitY = {};
    const auto bHit = sCollisions_DArray_504C6C->RayCast(
        field_120_xpos,
        field_124_ypos,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        &field_114_pCollisionLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 1 : 0);

    result = bHit;

    if (bHit == 1)
    {
        if (field_B8_vely > FP_FromInteger(0))
        {
            if (field_B8_vely < FP_FromInteger(1))
            {
                if (!mLiftPoint)
                {
                    AddToPlatform();
                }
                return 0;
            }

            mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
            field_B4_velx = (field_B4_velx / FP_FromInteger(2));
            if (field_118 <= 4)
            {
                s16 vol = 75 - 20 * field_118;
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_Mono(SoundEffect::GrenadeBounce_82, vol);
                field_118++;
                Event_Broadcast(kEventNoise, this);
                Event_Broadcast(kEventSuspiciousNoise, this);
                Event_Broadcast(kEventSpeaking, this);
            }
        }
    }

    const auto v20 = sCollisions_DArray_504C6C->RayCast(
        field_120_xpos,
        field_124_ypos,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        &field_114_pCollisionLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 96);

    if (v20 == 1)
    {
        switch (field_114_pCollisionLine->field_8_type)
        {
            case 1:
            case 5:
                if (field_B4_velx < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                    field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                    s16 vol = 75 - 20 * field_118;
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::GrenadeBounce_82, vol, 0);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                    Event_Broadcast(kEventSpeaking, this);
                }
                break;

            case 2:
            case 6:
                if (field_B4_velx > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                    s16 vol = 75 - 20 * field_118;
                    if (vol < 40)
                    {
                        vol = 40;
                    }

                    SFX_Play_Mono(SoundEffect::GrenadeBounce_82, vol, 0);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                    Event_Broadcast(kEventSpeaking, this);
                }
                break;

            default:
                return 1;
        }
    }
    return 1;
}

s16 Grenade::OnCollision_BounceOff(BaseGameObject* pHit)
{
    if (!pHit->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);

    PSX_RECT bRect = {};
    pHit2->VGetBoundingRect(&bRect, 1);

    if (mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(bRect.x + 12) || mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(bRect.w - 12))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = field_120_xpos;
        field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = field_124_ypos;
        field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
    }

    pHit2->VOnThrowableHit(this);

    SFX_Play_Mono(SoundEffect::RockBounceOnMine_29, 0, 0);
    return 0;
}

s16 Grenade::BlowUpAfterCountdown()
{
    field_112_explode_timer--;
    const s16 timer = field_112_explode_timer;
    if (!(timer % 16))
    {
        SFX_Play_Mono(SoundEffect::GreenTick_3, 0, 0);
    }

    if (timer)
    {
        return 0;
    }

    auto pExplosion = ao_new<Explosion>(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (field_BC_sprite_scale * FP_FromInteger(5)),
        field_BC_sprite_scale);
    if (pExplosion)
    {
        field_10_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_11C = pExplosion;
        pExplosion->mBaseGameObjectRefCount++;
        field_110_state = States::eWaitForExplodeEnd_6;
    }

    ao_new<Gibs>(GibType::Metal_5, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, FP_FromInteger(0), FP_FromInteger(5), field_BC_sprite_scale);
    return 1;
}

s16 Grenade::VCanThrow()
{
    return FALSE;
}

s16 Grenade::VIsFalling()
{
    return FALSE;
}

} // namespace AO
