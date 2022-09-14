#include "stdafx.h"
#include "Rock.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Particle.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"
#include <assert.h>

Rock::Rock(FP xpos, FP ypos, s16 count)
    : BaseThrowable(0)
{
    SetType(ReliveTypes::eRock);

    mBaseThrowableDead = 0;

    // Note: Loaded check removed
    LoadRockTypes_49AB30(mCurrentLevel, mCurrentPath);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Rock));
    Animation_Init(GetAnimRes(AnimId::Rock));

    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mXPos = xpos;
    mYPos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mBaseThrowableCount = count;
    field_11C_state = RockStates::eNone_0;

    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::Rock));

    //if (ppPal)
    {
        // TODO: I think this only existed in certain lvls, will need a way to know
        // which pal to use per lvl/path
        mAnim.LoadPal(GetPalRes(PalId::Rock));
    }
    //else
    {
        mAnim.ReloadPal();
    }

    field_11E_volume = 0;

    mShadow = relive_new Shadow();
}

void Rock::VTimeToExplodeRandom()
{
    // Calls actual implementation of 0x411490 which is empty.
}

//TODO Identical to AO - merge
void Rock::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mNextPath
        || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

bool Rock::VIsFalling()
{
    return field_11C_state == RockStates::eFallingOutOfWorld_5;
}

bool Rock::VCanThrow()
{
    return field_11C_state == RockStates::eBouncing_4;
}

Rock::~Rock()
{
    if (!gInfiniteThrowables && !mBaseThrowableDead)
    {
        if (gpThrowableArray)
        {
            gpThrowableArray->Remove(mBaseThrowableCount >= 1 ? mBaseThrowableCount : 1);
        }
    }
}

//TODO Identical to AO - merge
void Rock::VThrow(FP velX, FP velY)
{
    mVelX = velX;
    mVelY = velY;

    mAnim.mFlags.Set(AnimFlags::eBit3_Render);

    if (mBaseThrowableCount == 0)
    {
        field_11C_state = RockStates::eBouncing_4;
    }
    else
    {
        field_11C_state = RockStates::eFallingOutOfRockSack_1;
    }
}

void Rock::InTheAir()
{
    field_120_xpos = mXPos;
    field_124_ypos = mYPos;

    if (mVelY > FP_FromInteger(30))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mVelY += FP_FromDouble(1.01);

    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            field_120_xpos,
            field_124_ypos,
            mXPos,
            mYPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgFloorOrCeiling : kBgFloorOrCeiling)
        == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mVelY <= FP_FromInteger(0))
                {
                    break;
                }

                if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    return;
                }

                if (field_11C_state == RockStates::eBouncing_4 && mVelY < FP_FromInteger(5))
                {
                    field_11C_state = RockStates::eFallingOutOfWorld_5;
                    return;
                }

                if (field_11C_state == RockStates::eFallingOutOfRockSack_1 && mVelY < FP_FromInteger(1))
                {
                    field_11C_state = RockStates::eRolling_2;

                    if (mVelX >= FP_FromInteger(0) && mVelX < FP_FromInteger(1))
                    {
                        mVelX = FP_FromInteger(1);
                    }

                    if (mVelX > FP_FromInteger(-1) && mVelX < FP_FromInteger(0))
                    {
                        mVelX = FP_FromInteger(-1);
                    }

                    mYPos = hitY;
                    return;
                }
                else
                {
                    mYPos = hitY;
                    mVelX = (mVelX / FP_FromInteger(2));
                    mVelY = (-mVelY / FP_FromInteger(2));

                    s16 vol = 20 * (4 - field_11E_volume);
                    if (vol < 40)
                    {
                        vol = 40;
                    }

                    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                    field_11E_volume++;
                }
                break;

            case eLineTypes::eCeiling_3:
            case eLineTypes::eBackgroundCeiling_7:
                if (mVelY < FP_FromInteger(0))
                {
                    mYPos = hitY;
                    mVelY = (-mVelY / FP_FromInteger(2));
                    s16 vol = 20 * (4 - field_11E_volume);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
                    EventBroadcast(kEventNoise, this);
                    EventBroadcast(kEventSuspiciousNoise, this);
                }
                break;
        }
    }

    if (sCollisions->Raycast(field_120_xpos, field_124_ypos, mXPos, mYPos, &BaseAliveGameObjectCollisionLine, &hitX, &hitY, mScale == Scale::Fg ? kFgWalls : kBgWalls) == 1)
    {
        switch (BaseAliveGameObjectCollisionLine->mLineType)
        {
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mVelX < FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mVelX > FP_FromInteger(0))
                {
                    BounceHorizontally( hitX, hitY );
                }
                break;
        }
    }
}

//TODO Identical to AO - merge
void Rock::BounceHorizontally( FP hitX, FP hitY )
{
    mVelX = (-mVelX / FP_FromInteger(2));
    mXPos = hitX;
    mYPos = hitY;
    s16 vol = 20 * (4 - field_11E_volume);
    if (vol < 40)
    {
        vol = 40;
    }
    SfxPlayMono(relive::SoundEffects::RockBounce, vol);
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AO - merge
s16 Rock::OnCollision(BaseAliveGameObject* pObj)
{
    if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
    {
        mXPos -= mVelX;
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        mYPos -= mVelY;
        mVelY = (-mVelY / FP_FromInteger(2));
    }

    pObj->VOnThrowableHit(this);

    SfxPlayMono(relive::SoundEffects::RockBounceOnMine, 80);
    return 0;
}

void Rock::VUpdate()
{
    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            sCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(10),
                mXPos,
                mYPos + FP_FromInteger(10),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));
        }
        BaseAliveGameObjectCollisionLineType = 0;
    }

    switch (field_11C_state)
    {
        case RockStates::eNone_0:
            break;

        case RockStates::eFallingOutOfRockSack_1:
            InTheAir();
            return;

        case RockStates::eRolling_2:
            if (FP_Abs(mVelX) >= FP_FromInteger(1))
            {
                if (mVelX <= FP_FromInteger(0))
                {
                    mVelX += FP_FromDouble(0.01);
                }
                else
                {
                    mVelX -= FP_FromDouble(0.01);
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }
            else
            {
                if (abs(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) <= 1)
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                    mCollectionRect.w = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) + mXPos;
                    mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    mCollectionRect.h = mYPos;
                    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                    field_11C_state = RockStates::eOnGround_3;
                    mAnim.mFlags.Clear(AnimFlags::eBit8_Loop);
                    field_128_shimmer_timer = sGnFrame;
                    return;
                }
                BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return;
            }

            mAnim.mFlags.Set(AnimFlags::eBit8_Loop);
            field_11C_state = RockStates::eBouncing_4;
            return;

        case RockStates::eOnGround_3:
            if (static_cast<s32>(sGnFrame) <= field_128_shimmer_timer || pObj)
            {
                return;
            }
            // The strange shimmering that rocks give off.
            New_TintShiny_Particle(
                (mSpriteScale * FP_FromInteger(1)) + mXPos,
                (mSpriteScale * FP_FromInteger(-7)) + mYPos,
                FP_FromDouble(0.3),
                Layer::eLayer_Foreground_36);
            field_128_shimmer_timer = (Math_NextRandom() % 16) + sGnFrame + 60;
            return;

        case RockStates::eBouncing_4:
        {
            InTheAir();
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(xy, wh,
                                    gBaseGameObjects,
                                    (TCollisionCallBack) &Rock::OnCollision);

            if (mVelY > FP_FromInteger(30))
            {
                field_11C_state = RockStates::eFallingOutOfWorld_5;
            }
        }
            return;

        case RockStates::eFallingOutOfWorld_5:
            mVelY += FP_FromDouble(1.01);
            mXPos += mVelX;
            mYPos = mVelY + mYPos;
            if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos + FP_FromInteger(240), 0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;
    }
}

s32 Rock::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<RockSaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eRock_105;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;

    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_1C_path_number = mCurrentPath;
    pState->field_1E_lvl_number = MapWrapper::ToAE(mCurrentLevel);

    pState->field_18_sprite_scale = mSpriteScale;

    pState->field_20_flags.Set(RockSaveState::eBit1_bRender, mAnim.mFlags.Get(AnimFlags::eBit3_Render));
    pState->field_20_flags.Set(RockSaveState::eBit2_bDrawable, mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));

    pState->field_20_flags.Set(RockSaveState::eBit3_bLoop, mAnim.mFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(RockSaveState::eBit4_bInteractive, mBaseGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_28_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->field_28_line_type = -1;
    }
    pState->field_24_id = BaseAliveGameObject_PlatformId;
    pState->field_2A_count = mBaseThrowableCount;
    pState->field_2C_state = field_11C_state;
    pState->field_2E_volume = field_11E_volume;
    pState->field_30_xpos = field_120_xpos;
    pState->field_34_ypos = field_124_ypos;
    return sizeof(RockSaveState);
}

s32 Rock::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const RockSaveState*>(pData);

    auto pRock = relive_new Rock(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pRock->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

    pRock->mXPos = pState->field_8_xpos;
    pRock->mYPos = pState->field_C_ypos;

    pRock->mCollectionRect.x = pRock->mXPos - (ScaleToGridSize(pRock->mSpriteScale) / FP_FromInteger(2));
    pRock->mCollectionRect.w = pRock->mXPos + (ScaleToGridSize(pRock->mSpriteScale) / FP_FromInteger(2));
    pRock->mCollectionRect.h = pRock->mYPos;
    pRock->mCollectionRect.y = pRock->mYPos - ScaleToGridSize(pRock->mSpriteScale);

    pRock->mVelX = pState->field_10_velx;
    pRock->mVelY = pState->field_14_vely;

    pRock->mCurrentPath = pState->field_1C_path_number;
    pRock->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_1E_lvl_number);

    pRock->mSpriteScale = pState->field_18_sprite_scale;
    pRock->mScale = pState->field_18_sprite_scale > FP_FromDouble(0.75) ? Scale::Fg : Scale::Bg;

    pRock->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(RockSaveState::eBit1_bRender));
    pRock->mAnim.mFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(RockSaveState::eBit3_bLoop));

    pRock->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(RockSaveState::eBit2_bDrawable));
    pRock->mBaseGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(RockSaveState::eBit4_bInteractive));

    pRock->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pRock->field_128_shimmer_timer = sGnFrame;

    pRock->BaseAliveGameObjectCollisionLineType = pState->field_28_line_type;

    pRock->mBaseThrowableCount = pState->field_2A_count;
    pRock->field_11C_state = pState->field_2C_state;

    pRock->field_11E_volume = pState->field_2E_volume;

    pRock->field_120_xpos = pState->field_30_xpos;
    pRock->field_124_ypos = pState->field_34_ypos;

    return sizeof(RockSaveState);
}
