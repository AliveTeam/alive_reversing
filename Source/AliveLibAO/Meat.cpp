#include "stdafx_ao.h"
#include "Function.hpp"
#include "Meat.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Game.hpp"
#include "ThrowableArray.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "../relive_lib/Events.hpp"
#include "LiftPoint.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Throwable.hpp"
#include "CameraSwapper.hpp"
#include "Particle.hpp"
#include "Grid.hpp"

namespace AO {

MeatSack::MeatSack(relive::Path_MeatSack* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eMeatSack;

    const AnimRecord rec = AO::AnimRec(AnimId::MeatSack_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::MeatSack_Idle, ppRes);

    field_10C_tlvInfo = tlvId;
    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    field_110_bDoMeatSackIdleAnim = 0;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    field_118_velX = FP_FromRaw(pTlv->mVelX << 8);

    // Throw the meat up into the air as it falls from the sack
    field_11C_velY = -FP_FromRaw(pTlv->mVelY << 8);

    if (pTlv->mMeatFallDirection == relive::reliveXDirection::eLeft)
    {
        field_118_velX = -field_118_velX;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_8;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    field_112_num_items = pTlv->mMeatAmount;

    mShadow = relive_new Shadow();
}

MeatSack::~MeatSack()
{
    Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
}

void MeatSack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mAnim.mCurrentFrame == 2)
    {
        if (field_114_bPlayWobbleSound)
        {
            if (Math_NextRandom() < 40u || field_116_always_0)
            {
                field_114_bPlayWobbleSound = 0;
                field_116_always_0 = 0;
                SFX_Play_Pitch(SoundEffect::SackWobble_34, 24, Math_RandomRange(-2400, -2200), 0);
            }
        }
    }
    else
    {
        field_114_bPlayWobbleSound = 1;
    }

    if (field_110_bDoMeatSackIdleAnim == 1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mAnim.Set_Animation_Data(AnimId::MeatSack_Idle, nullptr);
            field_110_bDoMeatSackIdleAnim = 0;
        }
        return;
    }

    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT ourRect = VGetBoundingRect();

    if (RectsOverlap(ourRect, abeRect))
    {
        if (mSpriteScale == sActiveHero->mSpriteScale)
        {
            if (!gpThrowableArray_50E26C)
            {
                gpThrowableArray_50E26C = relive_new ThrowableArray();
            }

            if (gpThrowableArray_50E26C)
            {
                if (gpThrowableArray_50E26C->field_10_count > 0)
                {
                    mAnim.Set_Animation_Data(AnimId::MeatSack_Hit, nullptr);
                    field_110_bDoMeatSackIdleAnim = 1;
                    return;
                }

                gpThrowableArray_50E26C->Add(field_112_num_items);
            }

            auto pMeat = relive_new Meat(
                mXPos,
                mYPos - FP_FromInteger(30),
                field_112_num_items);
            if (pMeat)
            {
                pMeat->VThrow(field_118_velX, field_11C_velY);
                pMeat->mSpriteScale = mSpriteScale;
            }

            SfxPlayMono(SoundEffect::SackHit_30, 0, 0);
            Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, nullptr);
            mAnim.Set_Animation_Data(AnimId::MeatSack_Hit, nullptr);
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

    const AnimRecord rec = AO::AnimRec(AnimId::Meat);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Meat, ppRes);

    mXPos = xpos;
    mYPos = ypos;

    field_114_xpos = xpos;
    field_118_ypos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    field_11C_timer = 0;
    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);

    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_120_deadtimer = sGnFrame + 600;
    field_124_pLine = 0;
    field_10C_count = count;
    field_110_state = 0;

    mShadow = relive_new Shadow();
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
    if (gMap.mCurrentPath != gMap.mNextPath || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Meat::VThrow(FP velX, FP velY)
{
    mAnim.mFlags.Set(AnimFlags::eBit3_Render);

    mVelX = velX;
    mVelY = velY;

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

bool Meat::VCanEatMe()
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
    field_114_xpos = mXPos;
    field_118_ypos = mYPos;

    if (mVelY < FP_FromInteger(18))
    {
        mVelY += FP_FromInteger(1);
    }

    mXPos += mVelX;
    mYPos += mVelY;

    u16 result = 0;
    const FP xVoidSkip = CamX_VoidSkipper(mXPos, mVelX, 8, &result);
    mXPos = xVoidSkip;

    if (result)
    {
        field_114_xpos = xVoidSkip - mVelX;
    }

    const FP yVoidSkip = CamY_VoidSkipper(mYPos, mVelY, 8, &result);
    mYPos = yVoidSkip;

    if (result)
    {
        field_118_ypos = yVoidSkip - mVelY;
    }

    FP hitX = {};
    FP hitY = {};

    const s16 CollisionRaycast = sCollisions->Raycast(
        field_114_xpos,
        field_118_ypos,
        xVoidSkip,
        yVoidSkip,
        &field_124_pLine,
        &hitX,
        &hitY,
        mSpriteScale == FP_FromInteger(1) ? kFgWallsOrFloor : kBgWallsOrFloor) ? 1 : 0;


    if (CollisionRaycast == 1)
    {
        switch (field_124_pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY > FP_FromInteger(0))
                {
                    field_110_state = 3;

                    mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(hitX)));
                    mYPos = hitY;

                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);

                    SFX_Play_Pitch(SoundEffect::MeatBounce_43, 0, -650, 0);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    AddToPlatform();
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX >= FP_FromInteger(0))
                {
                    field_124_pLine = nullptr;
                    break;
                }

                mVelX = (-mVelX / FP_FromInteger(2));

                SFX_Play_Pitch(SoundEffect::MeatBounce_43, 0, -650, 0);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);

                if (mVelY >= FP_FromInteger(0))
                {
                    field_124_pLine = nullptr;
                    break;
                }

                mVelY = FP_FromInteger(0);
                field_124_pLine = nullptr;
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(4));

                    SFX_Play_Pitch(SoundEffect::MeatBounce_43, 0, -650, 0);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);

                    if (mVelY < FP_FromInteger(0))
                    {
                        mVelY = FP_FromInteger(0);
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
        if (EventGet(kEventDeathReset))
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
                const PSX_RECT bRect = VGetBoundingRect();

                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};

                VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Meat::OnCollision);

                if (mYPos > FP_FromInteger(gMap.field_D4_pPathData->field_A_bBottom))
                {
                    mBaseGameObjectFlags.Set(Options::eDead);
                }
                break;
            }

            case 3:
                if (FP_Abs(mVelX) < FP_FromInteger(1))
                {
                    mAnim.mFlags.Clear(AnimFlags::eBit8_Loop);
                }

                if (FP_Abs(mVelX) >= FP_FromDouble(0.5))
                {
                    if (mVelX <= FP_FromInteger(0))
                    {
                        mVelX += FP_FromDouble(0.01);
                    }
                    else
                    {
                        mVelX -= FP_FromDouble(0.01);
                    }

                    field_124_pLine = field_124_pLine->MoveOnLine(&mXPos, &mYPos, mVelX);
                    if (!field_124_pLine)
                    {
                        field_110_state = 2;
                        mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
                    }
                }
                else
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - ScaleToGridSize(mSpriteScale) / FP_FromInteger(2);
                    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                    mCollectionRect.w = mXPos + ScaleToGridSize(mSpriteScale) / FP_FromInteger(2);
                    mCollectionRect.h = mYPos;

                    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
                    field_110_state = 4;
                }
                break;

            case 4:
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    field_120_deadtimer = sGnFrame + 600;
                }

                if (static_cast<s32>(sGnFrame) > field_11C_timer)
                {
                    New_Shiny_Particle_4199A0(
                        mXPos + mSpriteScale,
                        mYPos + (mSpriteScale * FP_FromInteger(-7)),
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
                mVelY += FP_FromInteger(1);
                mXPos += mVelX;
                mYPos += mVelY;

                if (!gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mVelY + mYPos,
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

    const PSX_RECT bRect = pHit->VGetBoundingRect();

    if (field_114_xpos < FP_FromInteger(bRect.x) || field_114_xpos > FP_FromInteger(bRect.w))
    {
        mXPos -= mVelX;
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        mYPos -= mVelY;
        mVelY = (-mVelY / FP_FromInteger(2));
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
