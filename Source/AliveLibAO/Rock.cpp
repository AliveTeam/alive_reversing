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

    const AnimRecord& rec = AO::AnimRec(AnimId::Rock);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    field_11C_xpos = xpos;

    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    field_120_ypos = ypos;

    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);

    field_10C_count = count;
    field_110_state = States::eNone_0;

    u8** ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAberockAOResID, 0, 0);
    if (ppPal)
    {
        field_10_anim.LoadPal(ppPal, 0);
    }
    else
    {
        const FrameInfoHeader* pFrameInfo = field_10_anim.Get_FrameHeader(-1);

        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_10_anim.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

        field_10_anim.LoadPal(
            field_10_anim.field_20_ppBlock,
            pFrameHeader->field_0_clut_offset);
    }

    field_118_vol = 0;

    field_D0_pShadow = ao_new<Shadow>();
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
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    switch (field_110_state)
    {
        case States::eFallingOutOfRockSack_1:
            InTheAir();
            break;

        case States::eRolling_2:
            if (FP_Abs(field_B4_velx) >= FP_FromInteger(1))
            {
                if (field_B4_velx < FP_FromInteger(0))
                {
                    field_B4_velx += FP_FromDouble(0.01);
                }
                else
                {
                    field_B4_velx -= FP_FromDouble(0.01);
                }

                field_114_pLine->MoveOnLine(
                    &mBaseAnimatedWithPhysicsGameObject_XPos,
                    &mBaseAnimatedWithPhysicsGameObject_YPos,
                    field_B4_velx);

                if (!field_114_pLine)
                {
                    field_110_state = States::eBouncing_4;
                    field_10_anim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
                }
            }
            else
            {
                const s16 x_exp = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(field_BC_sprite_scale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) > 1)
                {
                    field_114_pLine = field_114_pLine->MoveOnLine(
                        &mBaseAnimatedWithPhysicsGameObject_XPos,
                        &mBaseAnimatedWithPhysicsGameObject_YPos,
                        field_B4_velx);
                    if (!field_114_pLine)
                    {
                        field_110_state = States::eBouncing_4;
                        field_10_anim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
                    }
                }
                else
                {
                    field_B4_velx = FP_FromInteger(0);
                    mBaseAliveGameObjectCollectionRect.x = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
                    mBaseAliveGameObjectCollectionRect.w = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));

                    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

                    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                    mBaseAliveGameObjectCollectionRect.y = mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(field_BC_sprite_scale);
                    mBaseAliveGameObjectCollectionRect.h = mBaseAnimatedWithPhysicsGameObject_YPos;
                    field_110_state = States::eOnGround_3;
                    field_124_shimmer_timer = sGnFrame;
                }
            }
            break;

        case States::eOnGround_3:
            if (static_cast<s32>(sGnFrame) > field_124_shimmer_timer)
            {
                New_Shiny_Particle_4199A0(
                    (field_BC_sprite_scale * FP_FromInteger(1)) + mBaseAnimatedWithPhysicsGameObject_XPos,
                    (field_BC_sprite_scale * FP_FromInteger(-7)) + mBaseAnimatedWithPhysicsGameObject_YPos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);
                field_124_shimmer_timer = (Math_NextRandom() % 16) + sGnFrame + 60;
            }
            break;

        case States::eBouncing_4:
        {
            InTheAir();
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(
                xy,
                wh,
                gBaseGameObjects,
                1,
                (TCollisionCallBack) &Rock::OnCollision);

            if (field_B8_vely > FP_FromInteger(30))
            {
                field_110_state = States::eFallingOutOfWorld_5;
            }
        }
        break;

        case States::eFallingOutOfWorld_5:
            field_B8_vely += FP_FromInteger(1);
            mBaseAnimatedWithPhysicsGameObject_XPos += field_B4_velx;
            mBaseAnimatedWithPhysicsGameObject_YPos += field_B8_vely;
            if (!gMap.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
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
    field_B4_velx = velX;
    field_B8_vely = velY;

    field_10_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

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

    if (field_B8_vely > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    field_B8_vely += FP_FromInteger(1);

    mBaseAnimatedWithPhysicsGameObject_XPos += field_B4_velx;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_B8_vely;

    u16 result = 0;
    mBaseAnimatedWithPhysicsGameObject_XPos = CamX_VoidSkipper(mBaseAnimatedWithPhysicsGameObject_XPos, field_B4_velx, 8, &result);
    mBaseAnimatedWithPhysicsGameObject_YPos = CamY_VoidSkipper(mBaseAnimatedWithPhysicsGameObject_YPos, field_B8_vely, 8, &result);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast(
            field_11C_xpos,
            field_120_ypos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &field_114_pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromInteger(1) ? 0x70 : 0x07))
    {
        switch (field_114_pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                if (field_B8_vely > FP_FromInteger(0))
                {
                    if (field_110_state != States::eBouncing_4 || field_B8_vely >= FP_FromInteger(5))
                    {
                        if (field_110_state != States::eFallingOutOfRockSack_1 || field_B8_vely >= FP_FromInteger(1))
                        {
                            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                            field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
                            field_B4_velx = (field_B4_velx / FP_FromInteger(2));
                            s32 vol = 20 * (4 - field_118_vol);
                            if (vol < 40)
                            {
                                vol = 40;
                            }
                            SFX_Play_Mono(SoundEffect::RockBounce_31, vol, 0);
                            Event_Broadcast(kEventNoise, this);
                            Event_Broadcast(kEventSuspiciousNoise, this);
                            field_118_vol++;
                        }
                        else
                        {
                            field_110_state = States::eRolling_2;
                            if (field_B4_velx >= FP_FromInteger(0) && field_B4_velx < FP_FromInteger(1))
                            {
                                field_B4_velx = FP_FromInteger(1);
                            }

                            if (field_B4_velx < FP_FromInteger(0) && field_B4_velx > FP_FromInteger(-1))
                            {
                                field_B4_velx = FP_FromInteger(-1);
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
                if (field_B4_velx < FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (field_B4_velx > FP_FromInteger(0))
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
    field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
    mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    s32 vol = 20 * (4 - field_118_vol);
    if (vol < 40)
    {
        vol = 40;
    }

    SFX_Play_Mono(SoundEffect::RockBounce_31, vol, 0);
    Event_Broadcast(kEventNoise, this);
    Event_Broadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AE - merge
s16 Rock::OnCollision(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

    if (field_11C_xpos < FP_FromInteger(bRect.x) || field_11C_xpos > FP_FromInteger(bRect.w))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos -= field_B4_velx;
        field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YPos -= field_B8_vely;
        field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
    }

    pObj->VOnThrowableHit(this);

    SFX_Play_Mono(SoundEffect::RockBounceOnMine_29, 80, 0);
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
