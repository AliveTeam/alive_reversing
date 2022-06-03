#include "stdafx.h"
#include "Rock.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "Particle.hpp"
#include "Grid.hpp"
#include <assert.h>

Rock::Rock(FP xpos, FP ypos, s16 count)
    : BaseThrowable(0)
{
    SetType(AETypes::eRock_105);

    field_11A_bDead = 0;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAberockResID, 0, 0))
    {
        LoadRockTypes_49AB30(mLvlNumber, mPathNumber);
    }

    const AnimRecord& rec = AnimRec(AnimId::Rock);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
    mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    mAnim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    mXPos = xpos;
    mYPos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mCount = count;
    field_11C_state = RockStates::eNone_0;

    u8** ppPal = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, AEResourceID::kAberockResID, 0, 0);
    if (ppPal)
    {
        mAnim.Load_Pal(ppPal, 0);
    }
    else
    {
        const FrameInfoHeader* pFrameInfo = mAnim.Get_FrameHeader(-1);

        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*mAnim.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

        mAnim.Load_Pal(
            mAnim.field_20_ppBlock,
            pFrameHeader->field_0_clut_offset);
    }

    field_11E_volume = 0;

    mShadow = ae_new<Shadow>();
}

void Rock::VTimeToExplodeRandom()
{
    // Calls actual implementation of 0x411490 which is empty.
}

//TODO Identical to AO - merge
void Rock::VScreenChanged()
{
    if (gMap.mCurrentPath != gMap.mPath
        || gMap.mCurrentLevel != gMap.mLevel)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Bool32 Rock::VIsFalling()
{
    return field_11C_state == RockStates::eFallingOutOfWorld_5;
}

Bool32 Rock::VCanThrow()
{
    return field_11C_state == RockStates::eBouncing_4;
}

Rock::~Rock()
{
    if (!gInfiniteGrenades_5C1BDE && !field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove(mCount >= 1 ? mCount : 1);
        }
    }
}

//TODO Identical to AO - merge
void Rock::VThrow(FP velX, FP velY)
{
    mVelX = velX;
    mVelY = velY;

    mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    if (mCount == 0)
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
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mVelY += FP_FromDouble(1.01);

    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            field_120_xpos,
            field_124_ypos,
            mXPos,
            mYPos,
            &mCollisionLine,
            &hitX,
            &hitY,
            mScale == 1 ? 0x09 : 0x90)
        == 1)
    {
        switch (mCollisionLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                if (mVelY <= FP_FromInteger(0))
                {
                    break;
                }

                if (!gMap.Is_Point_In_Current_Camera_4810D0(mLvlNumber, mPathNumber, mXPos, mYPos, 0))
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

                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
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
                    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
                    Event_Broadcast(kEventNoise, this);
                    Event_Broadcast(kEventSuspiciousNoise, this);
                }
                break;
        }
    }

    if (sCollisions_DArray_5C1128->Raycast(field_120_xpos, field_124_ypos, mXPos, mYPos, &mCollisionLine, &hitX, &hitY, mScale == 1 ? 0x06 : 0x60) == 1)
    {
        switch (mCollisionLine->field_8_type)
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
    SFX_Play_Mono(SoundEffect::RockBounce_26, vol);
    Event_Broadcast(kEventNoise, this);
    Event_Broadcast(kEventSuspiciousNoise, this);
}

//TODO Identical to AO - merge
s16 Rock::OnCollision(BaseAliveGameObject* pObj)
{
    if (!pObj->mGameObjectFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

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

    SFX_Play_Mono(SoundEffect::RockBounceOnMine_24, 80);
    return 0;
}

void Rock::VUpdate()
{
    auto pObj = sObjectIds.Find_Impl(mObjectId);
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (mCollisionLineType == -1)
        {
            mCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast(
                mXPos,
                mYPos - FP_FromInteger(10),
                mXPos,
                mYPos + FP_FromInteger(10),
                &mCollisionLine,
                &mXPos,
                &mYPos,
                1 << mCollisionLineType);
        }
        mCollisionLineType = 0;
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
                mCollisionLine = mCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }
            else
            {
                if (abs(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)) - FP_GetExponent(mXPos)) <= 1)
                {
                    mVelX = FP_FromInteger(0);
                    mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
                    mCollectionRect.w = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) + mXPos;
                    mGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8);
                    mCollectionRect.h = mYPos;
                    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
                    field_11C_state = RockStates::eOnGround_3;
                    mAnim.mAnimFlags.Clear(AnimFlags::eBit8_Loop);
                    field_128_shimmer_timer = sGnFrame_5C1B84;
                    return;
                }
                mCollisionLine = mCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
            }

            if (mCollisionLine)
            {
                return;
            }

            mAnim.mAnimFlags.Set(AnimFlags::eBit8_Loop);
            field_11C_state = RockStates::eBouncing_4;
            return;

        case RockStates::eOnGround_3:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_128_shimmer_timer || pObj)
            {
                return;
            }
            // The strange shimmering that rocks give off.
            New_TintShiny_Particle(
                (mSpriteScale * FP_FromInteger(1)) + mXPos,
                (mSpriteScale * FP_FromInteger(-7)) + mYPos,
                FP_FromDouble(0.3),
                Layer::eLayer_Foreground_36);
            field_128_shimmer_timer = (Math_NextRandom() % 16) + sGnFrame_5C1B84 + 60;
            return;

        case RockStates::eBouncing_4:
        {
            InTheAir();
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(xy, wh,
                                    gBaseGameObjects,
                                    1,
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
            if (!gMap.Is_Point_In_Current_Camera_4810D0(mLvlNumber, mPathNumber, mXPos, mYPos, 0) && !gMap.Is_Point_In_Current_Camera_4810D0(mLvlNumber, mPathNumber, mXPos, mYPos + FP_FromInteger(240), 0))
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;
    }
}

s32 Rock::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<RockSaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eRock_105;
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;

    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_1C_path_number = mPathNumber;
    pState->field_1E_lvl_number = mLvlNumber;

    pState->field_18_sprite_scale = mSpriteScale;

    pState->field_20_flags.Set(RockSaveState::eBit1_bRender, mAnim.mAnimFlags.Get(AnimFlags::eBit3_Render));
    pState->field_20_flags.Set(RockSaveState::eBit2_bDrawable, mGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4));

    pState->field_20_flags.Set(RockSaveState::eBit3_bLoop, mAnim.mAnimFlags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(RockSaveState::eBit4_bInteractive, mGameObjectFlags.Get(BaseGameObject::eInteractive_Bit8));

    if (mCollisionLine)
    {
        pState->field_28_line_type = mCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }
    pState->field_24_id = mObjectId;
    pState->field_2A_count = mCount;
    pState->field_2C_state = field_11C_state;
    pState->field_2E_volume = field_11E_volume;
    pState->field_30_xpos = field_120_xpos;
    pState->field_34_ypos = field_124_ypos;
    return sizeof(RockSaveState);
}

s32 Rock::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const RockSaveState*>(pData);

    auto pRock = ae_new<Rock>(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pRock->field_C_objectId = pState->field_4_obj_id;

    pRock->mXPos = pState->field_8_xpos;
    pRock->mYPos = pState->field_C_ypos;

    pRock->mCollectionRect.x = pRock->mXPos - (ScaleToGridSize(pRock->mSpriteScale) / FP_FromInteger(2));
    pRock->mCollectionRect.w = pRock->mXPos + (ScaleToGridSize(pRock->mSpriteScale) / FP_FromInteger(2));
    pRock->mCollectionRect.h = pRock->mYPos;
    pRock->mCollectionRect.y = pRock->mYPos - ScaleToGridSize(pRock->mSpriteScale);

    pRock->mVelX = pState->field_10_velx;
    pRock->mVelY = pState->field_14_vely;

    pRock->mPathNumber = pState->field_1C_path_number;
    pRock->mLvlNumber = pState->field_1E_lvl_number;

    pRock->mSpriteScale = pState->field_18_sprite_scale;
    pRock->mScale = pState->field_18_sprite_scale > FP_FromDouble(0.75);

    pRock->mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(RockSaveState::eBit1_bRender));
    pRock->mAnim.mAnimFlags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(RockSaveState::eBit3_bLoop));

    pRock->mGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(RockSaveState::eBit2_bDrawable));
    pRock->mGameObjectFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(RockSaveState::eBit4_bInteractive));

    pRock->mAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

    pRock->field_128_shimmer_timer = sGnFrame_5C1B84;

    pRock->mCollisionLineType = pState->field_28_line_type;

    pRock->mCount = pState->field_2A_count;
    pRock->field_11C_state = pState->field_2C_state;

    pRock->field_11E_volume = pState->field_2E_volume;

    pRock->field_120_xpos = pState->field_30_xpos;
    pRock->field_124_ypos = pState->field_34_ypos;

    return sizeof(RockSaveState);
}
