#include "stdafx_ao.h"
#include "Function.hpp"
#include "Rock.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "Grenade.hpp"
#include "ThrowableArray.hpp"
#include "Math.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Particle.hpp"
#include "Grid.hpp"

namespace AO {

Rock::Rock(FP xpos, FP ypos, s16 count)
    : BaseThrowable()
{
    mBaseGameObjectTypeId = ReliveTypes::eRock;

    field_10E_bDead = 0;

    const AnimRecord rec = AO::AnimRec(AnimId::Rock);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    field_11C_xpos = xpos;

    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    field_120_ypos = ypos;

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    field_10C_count = count;
    field_110_state = States::eNone_0;

    u8** ppPal = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAberockAOResID, 0, 0);
    if (ppPal)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(ppPal, 0);
    }
    else
    {
        const FrameInfoHeader* pFrameInfo = mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(-1);

        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

        mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(
            mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock,
            pFrameHeader->field_0_clut_offset);
    }

    field_118_vol = 0;

    mShadow = relive_new Shadow();
}

Rock::~Rock()
{
    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (gpThrowableArray_50E26C)
        {
            gpThrowableArray_50E26C->Remove(field_10C_count >= 1u ? field_10C_count : 1);
        }
    }
}

void Rock::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    switch (field_110_state)
    {
        case States::eFallingOutOfRockSack_1:
            InTheAir();
            break;

        case States::eRolling_2:
            if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_VelX) >= FP_FromInteger(1))
            {
                if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX += FP_FromDouble(0.01);
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX -= FP_FromDouble(0.01);
                }

                field_114_pLine->MoveOnLine(
                    &mBaseAnimatedWithPhysicsGameObject_XPos,
                    &mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_VelX);

                if (!field_114_pLine)
                {
                    field_110_state = States::eBouncing_4;
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
                }
            }
            else
            {
                const s16 x_exp = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) > 1)
                {
                    field_114_pLine = field_114_pLine->MoveOnLine(
                        &mBaseAnimatedWithPhysicsGameObject_XPos,
                        &mBaseAnimatedWithPhysicsGameObject_YPos,
                        mBaseAnimatedWithPhysicsGameObject_VelX);
                    if (!field_114_pLine)
                    {
                        field_110_state = States::eBouncing_4;
                        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
                    }
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                    mCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
                    mCollectionRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));

                    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                    mCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    mCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;
                    field_110_state = States::eOnGround_3;
                    field_124_shimmer_timer = sGnFrame;
                }
            }
            break;

        case States::eOnGround_3:
            if (static_cast<s32>(sGnFrame) > field_124_shimmer_timer)
            {
                New_Shiny_Particle_4199A0(
                    (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(1)) + mBaseAnimatedWithPhysicsGameObject_XPos,
                    (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-7)) + mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);
                field_124_shimmer_timer = (Math_NextRandom() % 16) + sGnFrame + 60;
            }
            break;

        case States::eBouncing_4:
        {
            InTheAir();
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(
                xy,
                wh,
                gBaseGameObjects,
                1,
                (TCollisionCallBack) &Rock::OnCollision);

            if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(30))
            {
                field_110_state = States::eFallingOutOfWorld_5;
            }
        }
        break;

        case States::eFallingOutOfWorld_5:
            mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromInteger(1);
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            if (!gMap.Is_Point_In_Current_Camera(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    0))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;
        default:
            return;
    }
}

//TODO Identical to AE - merge
void Rock::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mPath
        || gMap.mCurrentLevel != gMap.mLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

//TODO Identical to AE - merge
void Rock::VThrow(FP velX, FP velY)
{
    mBaseAnimatedWithPhysicsGameObject_VelX = velX;
    mBaseAnimatedWithPhysicsGameObject_VelY = velY;

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    if (field_10C_count == 0)
    {
        field_110_state = States::eBouncing_4;
    }
    else
    {
        field_110_state = States::eFallingOutOfRockSack_1;
    }
}

s16 Rock::VCanThrow()
{
    return field_110_state == States::eBouncing_4;
}

void Rock::InTheAir()
{
    field_11C_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_120_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromInteger(1);

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    u16 result = 0;
    mBaseAnimatedWithPhysicsGameObject_XPos = CamX_VoidSkipper(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_VelX, 8, &result);
    mBaseAnimatedWithPhysicsGameObject_YPos = CamY_VoidSkipper(mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelY, 8, &result);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            field_11C_xpos,
            field_120_ypos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &field_114_pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1) ? 0x70 : 0x07))
    {
        switch (field_114_pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
                {
                    if (field_110_state != States::eBouncing_4 || mBaseAnimatedWithPhysicsGameObject_VelY >= FP_FromInteger(5))
                    {
                        if (field_110_state != States::eFallingOutOfRockSack_1 || mBaseAnimatedWithPhysicsGameObject_VelY >= FP_FromInteger(1))
                        {
                            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                            mBaseAnimatedWithPhysicsGameObject_VelY = (-mBaseAnimatedWithPhysicsGameObject_VelY / FP_FromInteger(2));
                            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
                            s32 vol = 20 * (4 - field_118_vol);
                            if (vol < 40)
                            {
                                vol = 40;
                            }
                            SfxPlayMono(SoundEffect::RockBounce_31, vol, 0);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            field_118_vol++;
                        }
                        else
                        {
                            field_110_state = States::eRolling_2;
                            if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(1))
                            {
                                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(1);
                            }

                            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(-1))
                            {
                                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(-1);
                            }
                        }
                    }
                    else
                    {
                        field_110_state = States::eFallingOutOfWorld_5;
                    }
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;

            default:
                return;
        }
    }
}

//TODO Identical to AE - merge
void Rock::BounceHorizontally( FP hitX, FP hitY )
{
    mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    s32 vol = 20 * (4 - field_118_vol);
    if (vol < 40)
    {
        vol = 40;
    }

    SfxPlayMono(SoundEffect::RockBounce_31, vol, 0);
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AE - merge
s16 Rock::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    if (field_11C_xpos < FP_FromInteger(bRect.x) || field_11C_xpos > FP_FromInteger(bRect.w))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos -= mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YPos -= mBaseAnimatedWithPhysicsGameObject_VelY;
        mBaseAnimatedWithPhysicsGameObject_VelY = (-mBaseAnimatedWithPhysicsGameObject_VelY / FP_FromInteger(2));
    }

    pObj->VOnThrowableHit(this);

    SfxPlayMono(SoundEffect::RockBounceOnMine_29, 80, 0);
    return 0;
}

s16 Rock::VIsFalling()
{
    // Same as meat falling func - compiler seems to have made them both
    // use the same func, or should it go in the base ??
    return field_110_state == States::eFallingOutOfWorld_5;
}

void Rock::VTimeToExplodeRandom()
{
    // Empty ?
}

} // namespace AO
