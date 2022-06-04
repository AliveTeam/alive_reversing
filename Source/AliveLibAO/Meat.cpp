#include "stdafx_ao.h"
#include "Function.hpp"
#include "Meat.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "ThrowableArray.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "Events.hpp"
#include "LiftPoint.hpp"
#include "Collisions.hpp"
#include "Throwable.hpp"
#include "CameraSwapper.hpp"
#include "Particle.hpp"
#include "Grid.hpp"

namespace AO {

MeatSack::MeatSack(Path_MeatSack* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eMeatStack;

    const AnimRecord& rec = AO::AnimRec(AnimId::MeatSack_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10C_tlvInfo = tlvInfo;
    field_CC_bApplyShadows &= ~1u;

    field_110_bDoMeatSackIdleAnim = 0;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_118_velX = FP_FromRaw(pTlv->field_1A_x_vel << 8);

    // Throw the meat up into the air as it falls from the sack
    field_11C_velY = -FP_FromRaw(pTlv->field_1C_y_vel << 8);

    if (pTlv->field_18_meat_fall_direction == XDirection_short::eLeft_0)
    {
        field_118_velX = -field_118_velX;
    }

    if (pTlv->field_1E_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.mRenderLayer = Layer::eLayer_8;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.mRenderLayer = Layer::eLayer_27;
        field_C6_scale = 1;
    }

    field_112_num_items = pTlv->field_20_amount_of_meat;

    field_D0_pShadow = ao_new<Shadow>();
}

MeatSack::~MeatSack()
{
    gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
}

void MeatSack::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_10_anim.field_92_current_frame == 2)
    {
        if (field_114_bPlayWobbleSound)
        {
            if (Math_NextRandom() < 40u || field_116_always_0)
            {
                field_114_bPlayWobbleSound = 0;
                field_116_always_0 = 0;
                SFX_Play_Pitch(SoundEffect::SackWobble_34, 24, Math_RandomRange_450F20(-2400, -2200), 0);
            }
        }
    }
    else
    {
        field_114_bPlayWobbleSound = 1;
    }

    if (field_110_bDoMeatSackIdleAnim == 1)
    {
        if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            const AnimRecord& rec = AO::AnimRec(AnimId::MeatSack_Idle);
            field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
            field_110_bDoMeatSackIdleAnim = 0;
        }
        return;
    }

    PSX_RECT abeRect = {};
    sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    if (RectsOverlap(ourRect, abeRect))
    {
        if (field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
        {
            const AnimRecord& MeatSackHitRec = AO::AnimRec(AnimId::MeatSack_Hit);
            if (!gpThrowableArray_50E26C)
            {
                gpThrowableArray_50E26C = ao_new<ThrowableArray>();
            }

            if (gpThrowableArray_50E26C)
            {
                if (gpThrowableArray_50E26C->field_10_count > 0)
                {
                    field_10_anim.Set_Animation_Data(MeatSackHitRec.mFrameTableOffset, nullptr);
                    field_110_bDoMeatSackIdleAnim = 1;
                    return;
                }

                gpThrowableArray_50E26C->Add(field_112_num_items);
            }

            auto pMeat = ao_new<Meat>(
                field_A8_xpos,
                field_AC_ypos - FP_FromInteger(30),
                field_112_num_items);
            if (pMeat)
            {
                pMeat->VThrow(field_118_velX, field_11C_velY);
                pMeat->field_BC_sprite_scale = field_BC_sprite_scale;
            }

            SFX_Play_Mono(SoundEffect::SackHit_30, 0, 0);
            Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, nullptr);
            field_10_anim.Set_Animation_Data(MeatSackHitRec.mFrameTableOffset, nullptr);
            field_110_bDoMeatSackIdleAnim = 1;
            return;
        }
    }
}

void MeatSack::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

Meat::Meat(FP xpos, FP ypos, s16 count)
    : BaseThrowable()
{
    field_10E_bDead = 0;

    mBaseGameObjectTypeId = ReliveTypes::eMeat;

    const AnimRecord& rec = AO::AnimRec(AnimId::Meat);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    field_114_xpos = xpos;
    field_118_ypos = ypos;

    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_11C_timer = 0;
    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);

    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_120_deadtimer = sGnFrame + 600;
    field_124_pLine = 0;
    field_10C_count = count;
    field_110_state = 0;

    field_D0_pShadow = ao_new<Shadow>();
}

Meat::~Meat()
{
    if (!field_10E_bDead)
    {
        if (gpThrowableArray_50E26C)
        {
            gpThrowableArray_50E26C->Remove(field_10C_count >= 1u ? field_10C_count : 1);
        }
    }
}

void Meat::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mPath || gMap.mCurrentLevel != gMap.mLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Meat::VThrow(FP velX, FP velY)
{
    field_10_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    field_B4_velx = velX;
    field_B8_vely = velY;

    if (field_10C_count == 0)
    {
        field_110_state = 2;
    }
    else
    {
        field_110_state = 1;
    }
}

s16 Meat::VCanThrow()
{
    return field_110_state == 2;
}

Bool32 Meat::VCanEatMe()
{
    return field_110_state != 0;
}

s16 Meat::VIsFalling()
{
    return field_110_state == 5;
}

void Meat::VTimeToExplodeRandom()
{
    // Empty
}

void Meat::InTheAir()
{
    field_114_xpos = field_A8_xpos;
    field_118_ypos = field_AC_ypos;

    if (field_B8_vely < FP_FromInteger(18))
    {
        field_B8_vely += FP_FromInteger(1);
    }

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    u16 result = 0;
    const FP xVoidSkip = CamX_VoidSkipper(field_A8_xpos, field_B4_velx, 8, &result);
    field_A8_xpos = xVoidSkip;

    if (result)
    {
        field_114_xpos = xVoidSkip - field_B4_velx;
    }

    const FP yVoidSkip = CamY_VoidSkipper(field_AC_ypos, field_B8_vely, 8, &result);
    field_AC_ypos = yVoidSkip;

    if (result)
    {
        field_118_ypos = yVoidSkip - field_B8_vely;
    }

    FP hitX = {};
    FP hitY = {};

    const s16 CollisionRaycast = sCollisions_DArray_504C6C->RayCast(
        field_114_xpos,
        field_118_ypos,
        xVoidSkip,
        yVoidSkip,
        &field_124_pLine,
        &hitX,
        &hitY,
        field_BC_sprite_scale != FP_FromInteger(1) ? 0x70 : 7);


    if (CollisionRaycast == 1)
    {
        switch (field_124_pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                if (field_B8_vely > FP_FromInteger(0))
                {
                    field_110_state = 3;

                    field_A8_xpos = FP_FromInteger(SnapToXGrid(field_BC_sprite_scale, FP_GetExponent(hitX)));
                    field_AC_ypos = hitY;

                    field_B8_vely = FP_FromInteger(0);
                    field_B4_velx = FP_FromInteger(0);

                    SFX_Play_Pitch(SoundEffect::MeatBounce_43, 0, -650, 0);
                    Event_Broadcast(kEventNoise_0, this);
                    Event_Broadcast(kEventSuspiciousNoise_10, this);
                    AddToPlatform();
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (field_B4_velx >= FP_FromInteger(0))
                {
                    field_124_pLine = nullptr;
                    break;
                }

                field_B4_velx = (-field_B4_velx / FP_FromInteger(2));

                SFX_Play_Pitch(SoundEffect::MeatBounce_43, 0, -650, 0);
                Event_Broadcast(kEventNoise_0, this);
                Event_Broadcast(kEventSuspiciousNoise_10, this);

                if (field_B8_vely >= FP_FromInteger(0))
                {
                    field_124_pLine = nullptr;
                    break;
                }

                field_B8_vely = FP_FromInteger(0);
                field_124_pLine = nullptr;
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (field_B4_velx > FP_FromInteger(0))
                {
                    field_B4_velx = (-field_B4_velx / FP_FromInteger(4));

                    SFX_Play_Pitch(SoundEffect::MeatBounce_43, 0, -650, 0);
                    Event_Broadcast(kEventNoise_0, this);
                    Event_Broadcast(kEventSuspiciousNoise_10, this);

                    if (field_B8_vely < FP_FromInteger(0))
                    {
                        field_B8_vely = FP_FromInteger(0);
                    }
                }

                field_124_pLine = nullptr;
                break;

            default:
                return;
        }
    }
}

void Meat::VUpdate()
{
    if (sNumCamSwappers_507668 == 0)
    {
        if (Event_Get(kEventDeathReset_4))
        {
            mBaseGameObjectFlags.Set(Options::eDead);
        }

        // TODO: states enum
        switch (field_110_state)
        {
            case 1:
                InTheAir();
                break;

            case 2:
            {
                InTheAir();
                PSX_RECT bRect = {};

                VGetBoundingRect(&bRect, 1);
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};

                VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Meat::OnCollision);

                if (field_AC_ypos > FP_FromInteger(gMap.field_D4_pPathData->field_A_bBottom))
                {
                    mBaseGameObjectFlags.Set(Options::eDead);
                }
                break;
            }

            case 3:
                if (FP_Abs(field_B4_velx) < FP_FromInteger(1))
                {
                    field_10_anim.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                }

                if (FP_Abs(field_B4_velx) >= FP_FromDouble(0.5))
                {
                    if (field_B4_velx <= FP_FromInteger(0))
                    {
                        field_B4_velx += FP_FromDouble(0.01);
                    }
                    else
                    {
                        field_B4_velx -= FP_FromDouble(0.01);
                    }

                    field_124_pLine = field_124_pLine->MoveOnLine(&field_A8_xpos, &field_AC_ypos, field_B4_velx);
                    if (!field_124_pLine)
                    {
                        field_110_state = 2;
                        field_10_anim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
                    }
                }
                else
                {
                    field_B4_velx = FP_FromInteger(0);
                    mBaseAliveGameObjectCollectionRect.x = field_A8_xpos - ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2);
                    mBaseAliveGameObjectCollectionRect.y = field_AC_ypos - ScaleToGridSize(field_BC_sprite_scale);
                    mBaseAliveGameObjectCollectionRect.w = field_A8_xpos + ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2);
                    mBaseAliveGameObjectCollectionRect.h = field_AC_ypos;

                    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
                    field_110_state = 4;
                }
                break;

            case 4:
                if (gMap.Is_Point_In_Current_Camera_4449C0(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos, 0))
                {
                    field_120_deadtimer = sGnFrame + 600;
                }

                if (static_cast<s32>(sGnFrame) > field_11C_timer)
                {
                    New_Shiny_Particle_4199A0(
                        field_A8_xpos + field_BC_sprite_scale,
                        field_AC_ypos + (field_BC_sprite_scale * FP_FromInteger(-7)),
                        FP_FromDouble(0.3),
                        Layer::eLayer_Foreground_36);
                    field_11C_timer = Math_NextRandom() % 16 + sGnFrame + 60;
                }
                if (field_120_deadtimer < static_cast<s32>(sGnFrame))
                {
                    mBaseGameObjectFlags.Set(Options::eDead);
                }
                break;

            case 5:
                field_B8_vely += FP_FromInteger(1);
                field_A8_xpos += field_B4_velx;
                field_AC_ypos += field_B8_vely;

                if (!gMap.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        field_A8_xpos,
                        field_B8_vely + field_AC_ypos,
                        0))
                {
                    mBaseGameObjectFlags.Set(Options::eDead);
                }
                break;

            default:
                return;
        }
    }
}

s16 Meat::OnCollision(BaseAliveGameObject* pHit)
{
    if (!pHit->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    if (pHit->mBaseGameObjectTypeId == ReliveTypes::eMine || pHit->mBaseGameObjectTypeId == ReliveTypes::eUXB || pHit->mBaseGameObjectTypeId == ReliveTypes::eTimedMine)
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pHit->VGetBoundingRect(&bRect, 1);

    if (field_114_xpos < FP_FromInteger(bRect.x) || field_114_xpos > FP_FromInteger(bRect.w))
    {
        field_A8_xpos -= field_B4_velx;
        field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
    }
    else
    {
        field_AC_ypos -= field_B8_vely;
        field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
    }

    pHit->VOnThrowableHit(this);
    SFX_Play_Pitch(SoundEffect::MeatBounce_43, 0, -650, 0);
    return 0;
}

void Meat::AddToPlatform()
{
    BaseAddToPlatform();
}

void Meat::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
        if (field_110_state == 3 || field_110_state == 4)
        {
            field_110_state = 1;
        }
    }
}

s16 Meat::VGetCount()
{
    if (field_110_state == 4 && field_10C_count == 0)
    {
        return 1;
    }

    return field_10C_count;
}

} // namespace AO
