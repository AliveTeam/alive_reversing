#include "stdafx.h"
#include "MineCar.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Sound/Midi.hpp"
#include "../relive_lib/ShadowZone.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Function.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "ParticleBurst.hpp"
#include "ScreenShake.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"

const FP mineCarHeightUnscaled = FP_FromInteger(60);
const FP mineCarWidthUnscaled = FP_FromInteger(12);

void MineCar::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Car_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Car_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Car_Shake_A));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Car_Shake_B));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Car_Tread_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Car_Tread_Move_A));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Car_Tread_Move_B));
}

MineCar::MineCar(relive::Path_MineCar* pTlv, const Guid& tlvId, s32 /*a4*/, s32 /*a5*/, s32 /*a6*/)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eMineCar);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::Mine_Car_Open));

    field_11E_scale = pTlv->mScale;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    field_1BE_unused = 0;
    mAnim.mRenderLayer = Layer::eLayer_Shadow_26;
    mSpriteScale = FP_FromInteger(1);
    mScale = Scale::Fg;

    if (field_11E_scale != relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        mScale = Scale::Bg;
    }

    MapFollowMe(TRUE);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    field_118_tlvInfo = tlvId;
    field_11C_state = MineCarStates::eParkedWithoutAbe_0;

    LoadAnimation(&mTreadAnim);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    mShadow = relive_new Shadow();

    field_1C0_unused = 0;
    field_1C2_falling_counter = 0;

    // What was pressed before we started to move
    field_1D4_previous_input = static_cast<s16>(sInputKey_ThrowItem);

    // Set to a key that keeps the car moving, for example if moving right and we hit a right wall that a car
    // can travel "up" then we set this key to "up" such that holding down "right" automatically moves the car up.
    field_1D6_continue_move_input = static_cast<s16>(sInputKey_ThrowItem);

    field_1CC_spawned_path = gMap.mCurrentPath;
    field_1CE_spawned_camera = gMap.mCurrentCamera;
    field_1D0_sound_channels_mask = 0;
    field_1C4_velx_index = 0;
}

const AnimId sMineCarAnimIdTable[7] = {
    AnimId::Mine_Car_Closed,
    AnimId::Mine_Car_Open,
    AnimId::Mine_Car_Shake_A,
    AnimId::Mine_Car_Shake_B,
    AnimId::Mine_Car_Tread_Idle,
    AnimId::Mine_Car_Tread_Move_A,
    AnimId::Mine_Car_Tread_Move_B};

s32 MineCar::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const MineCar_SaveState*>(pBuffer);
    auto pTlv = static_cast<relive::Path_MineCar*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4C_tlvInfo));

    auto pMineCar = relive_new MineCar(pTlv, pState->field_4C_tlvInfo, 0, 0, 0);
    if (pMineCar)
    {
        if (pState->field_5A_bAbeInCar)
        {
            sControlledCharacter = pMineCar;
        }

        pMineCar->BaseAliveGameObjectPathTLV = nullptr;
        pMineCar->BaseAliveGameObjectCollisionLine = nullptr;

        pMineCar->mXPos = pState->field_4_xpos;
        pMineCar->mYPos = pState->field_8_ypos;

        pMineCar->mVelX = pState->field_C_velx;
        pMineCar->mVelY = pState->field_10_vely;

        pMineCar->mCurrentPath = pState->field_18_path_number;
        pMineCar->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_1A_lvl_number);

        pMineCar->mSpriteScale = pState->field_14_sprite_scale;

        pMineCar->mRGB.SetRGB(pState->field_1C_r, pState->field_1E_g, pState->field_20_b);

        pMineCar->mCurrentMotion = pState->field_28_current_motion;

        // TODO: replace Set_Animation_Data sMineCarAnimIdTable[] with the actual anim id's
        switch (pState->field_24_frame_table)
        {
        case 10860:
            pMineCar->mAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[6]));
            break;
        case 10884:
            pMineCar->mAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[1]));
            break;
        case 10896:
            pMineCar->mAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[4]));
            break;
        case 10908:
            pMineCar->mAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[0]));
            break;
        case 10920:
            pMineCar->mAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[5]));
            break;
        case 10944:
            pMineCar->mAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[2]));
            break;
        case 10972:
            pMineCar->mAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[3]));
            break;
        default:
            break;
        }

        pMineCar->mAnim.mCurrentFrame = pState->field_2A_current_anim_frame;


        pMineCar->mAnim.mFrameChangeCounter = pState->field_2C_frame_change_counter;

        pMineCar->mAnim.mFlags.Set(AnimFlags::eFlipX, pState->field_22_xFlip & 1);
        pMineCar->mAnim.mFlags.Set(AnimFlags::eRender, pState->field_2E_render & 1);

        pMineCar->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_drawable & 1);

        if (IsLastFrame(&pMineCar->mAnim))
        {
            pMineCar->mAnim.mFlags.Set(AnimFlags::eIsLastFrame);
        }

        // TODO: replace Set_Animation_Data sMineCarAnimIdTable[] with the actual anim id's
        switch (pState->field_38_frame_table_offset2)
        {
        case 10860:
            pMineCar->mTreadAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[6]));
            break;
        case 10884:
            pMineCar->mTreadAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[1]));
            break;
        case 10896:
            pMineCar->mTreadAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[4]));
            break;
        case 10908:
            pMineCar->mTreadAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[0]));
            break;
        case 10920:
            pMineCar->mTreadAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[5]));
            break;
        case 10944:
            pMineCar->mTreadAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[2]));
            break;
        case 10972:
            pMineCar->mTreadAnim.Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[3]));
            break;
        default:
            break;
        }

        pMineCar->mTreadAnim.mCurrentFrame = pState->field_2A_current_anim_frame;


        // TODO: OG Bug same flags but save state saves 2 sets one for each anim ??
        pMineCar->mTreadAnim.mFlags.Set(AnimFlags::eFlipX, pState->field_22_xFlip & 1);
        pMineCar->mTreadAnim.mFlags.Set(AnimFlags::eRender, pState->field_2E_render & 1);


        pMineCar->mTreadAnim.mFrameChangeCounter = pState->field_2C_frame_change_counter;

        pMineCar->mHealth = pState->field_3C_health;
        pMineCar->mCurrentMotion = pState->field_40_current_motion;

        pMineCar->mNextMotion = pState->field_42_next_motion;

        pMineCar->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_44_last_line_ypos);

        pMineCar->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);

        pMineCar->BaseAliveGameObjectCollisionLineType = pState->field_46_collision_line_type;
        pMineCar->field_118_tlvInfo = pState->field_4C_tlvInfo;

        pMineCar->field_11C_state = pState->field_50_state;
        pMineCar->field_1BC_turn_direction = pState->field_52_turn_direction;
        pMineCar->field_1BE_unused = pState->field_54_unused;
        pMineCar->field_1C0_unused = pState->field_56_unused;

        pMineCar->field_1C2_falling_counter = pState->field_58_falling_counter;

        pMineCar->field_1C8_frame_mod_16 = pState->field_5C_frame_mod_16;

        pMineCar->field_1CC_spawned_path = pState->field_60_spawned_path;
        pMineCar->field_1CE_spawned_camera = pState->field_62_spanwed_camera;

        pMineCar->field_1D4_previous_input = pState->field_64_throw_item_key1;
        pMineCar->field_1D6_continue_move_input = pState->field_66_continue_move_input;

        if (pMineCar->mVelX < (ScaleToGridSize(FP_FromInteger(1)) / FP_FromInteger(4)))
        {
            pMineCar->field_1C4_velx_index = 7;
        }
    }

    return sizeof(MineCar_SaveState);
}

void MineCar::LoadAnimation(Animation* pAnim)
{
    if (pAnim->Init(GetAnimRes(AnimId::Mine_Car_Tread_Idle), this))
    {
        pAnim->mRenderLayer = mAnim.mRenderLayer;
        pAnim->mFlags.Clear(AnimFlags::eBlending);
        pAnim->field_14_scale = mSpriteScale;
        pAnim->mRed = 128;
        pAnim->mGreen = 128;
        pAnim->mBlue = 128;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void MineCar::VStopAudio()
{
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
}

void MineCar::VScreenChanged()
{
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
    BaseGameObject::VScreenChanged();
}

MineCar::~MineCar()
{
    Path::TLV_Reset(field_118_tlvInfo, -1, 0, 1);
    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }
    mTreadAnim.VCleanUp();

    // Don't point to a deleted object
    if (sControlledCharacter == this)
    {
        sControlledCharacter = nullptr;
    }
}

bool MineCar::CheckRoofCollision(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;
    return sCollisions->Raycast(
        mXPos + hitX,
        mYPos,
        mXPos + hitX,
        mYPos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        mScale == Scale::Fg ? CollisionMask(eCeiling_3) : CollisionMask(eBackgroundCeiling_7)
    );
}

bool MineCar::CheckFloorCollision(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions->Raycast(
            mXPos + hitX,
            mYPos - FP_FromInteger(4),
            mXPos + hitX,
            mYPos + hitY,
            &pPathLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgFloor : kBgFloor
        )
    )
    {
        return FALSE;
    }

    if (mYPos > hitY)
    {
        mYPos = hitY;
    }

    return TRUE;
}

void MineCar::VRender(PrimHeader** ppOt)
{
    if (mAnim.mFlags.Get(AnimFlags::eRender))
    {
        s16 r = mRGB.g;
        s16 g = mRGB.r;
        s16 b = mRGB.b;

        const PSX_RECT bRect = VGetBoundingRect();

        if (mVisualFlags.Get(VisualFlags::eApplyShadowZoneColour))
        {
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mXPos),
                (bRect.h + bRect.y) / 2,
                mScale,
                &r,
                &g,
                &b);
        }

        mTreadAnim.mRed = static_cast<u8>(r);
        mTreadAnim.mGreen = static_cast<u8>(g);
        mTreadAnim.mBlue = static_cast<u8>(b);

        if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos + FP_FromInteger(30), mYPos, 0) || gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos - (mSpriteScale * FP_FromInteger(60)), 0) || gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos - FP_FromInteger(30), mYPos, 0))
        {
            mTreadAnim.VRender(
                FP_GetExponent(mXPos - pScreenManager->CamXPos()),
                FP_GetExponent(mYPos - pScreenManager->CamYPos()),
                ppOt,
                0,
                0);
        }

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void MineCar::Stop()
{
    field_11C_state = MineCarStates::eParkedWithAbe_1;

    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }

    SfxPlayMono(relive::SoundEffects::MinecarStop, 127, mSpriteScale);
    
    mAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Closed));
    mTreadAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Tread_Idle));

    field_1C4_velx_index = 0;

    mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));
}

void MineCar::Move(AnimId animId, FP velX, FP velY, InputCommands::Enum input, MineCarDirs turnDirection, s8 bChangeDirection)
{
    mAnim.Set_Animation_Data(GetAnimRes(animId));

    field_11C_state = MineCarStates::eMoving_2;
    field_1C8_frame_mod_16 = static_cast<s32>(sGnFrame) % 16;

    if (!field_1D0_sound_channels_mask)
    {
        field_1D0_sound_channels_mask = SfxPlayMono(relive::SoundEffects::MinecarMovement, 127, mSpriteScale);
    }

    mTreadAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Tread_Move_A));

    mVelX = velX;
    mVelY = velY;

    if (Input().isPressed(input))
    {
        field_1D4_previous_input = static_cast<s16>(input);
    }

    field_1BC_turn_direction = turnDirection;

    mTreadAnim.mFlags.Set(AnimFlags::eLoopBackwards, bChangeDirection);
}

s16 MineCar::IsBlocked(MineCarDirs a2, s32 /*a3*/)
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);
    const FP mineCarHeight = mSpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mSpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    switch (a2)
    {
        case MineCarDirs::eDown_0:
        {
            if (!CheckRoofCollision(FP_FromInteger(4) - mineCarWidthAdjusted, -(mineCarHeight) + FP_FromInteger(1)))
            {
                if (!CheckRoofCollision(mineCarWidthAdjusted - FP_FromInteger(4), -(mineCarHeight + FP_FromInteger(1))))
                {
                    return FALSE;
                }
            }
        }
        break;

        case MineCarDirs::eUp_3:
        {
            if (!CheckFloorCollision(FP_FromInteger(4) - mineCarWidthAdjusted, FP_FromInteger(1)))
            {
                if (
                    !CheckFloorCollision(mineCarWidthAdjusted - FP_FromInteger(4), FP_FromInteger(1)) &&
                    !CheckFloorCollision(-FP_FromInteger(10), FP_FromInteger(1)) &&
                    !CheckFloorCollision(FP_FromInteger(10), FP_FromInteger(1))
                )
                {
                    return FALSE;
                }
            }
        }
        break;
    }

    return TRUE;
}

s16 MineCar::FollowDirection()
{
    const FP mineCarHeight = mSpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mSpriteScale * mineCarWidthUnscaled;
    const FP stepSize = ScaleToGridSize(mSpriteScale);

    const FP halfHeight = mineCarHeight * FP_FromDouble(0.5);
    const FP step = mineCarWidth + stepSize;

    // If we're moving horizontally and hit a wall...
    //
    if (
        (mVelX > FP_FromInteger(0) && WallHit(halfHeight, step + FP_FromInteger(1))) ||
        (mVelX < FP_FromInteger(0) && WallHit(halfHeight, -step))
    )
    {
        // ...continue vertically
        //
        if (field_1BC_turn_direction == MineCarDirs::eUp_3)
        {
            field_1D6_continue_move_input = (u16) sInputKey_Up;
        }
        else
        {
            field_1D6_continue_move_input = (u16) sInputKey_Down;
        }

        return TRUE;
    }

    // If we're moving vertically and hit and floor or roof...
    //
    if (
        (
            CheckFloorCollision(FP_FromInteger(0), FP_FromInteger(1)) &&
            mVelY > FP_FromInteger(0)
        ) ||
        (
            CheckRoofCollision(FP_FromInteger(0), -mineCarHeight) &&
            mVelY < FP_FromInteger(0)
        )
    )
    {
        // ...continue horizontally
        //
        if (field_1BC_turn_direction == MineCarDirs::eLeft_2)
        {
            field_1D6_continue_move_input = (u16) sInputKey_Left;
        }
        else
        {
            field_1D6_continue_move_input = (u16) sInputKey_Right;
        }

        return TRUE;
    }

    switch (field_1BC_turn_direction)
    {
        case MineCarDirs::eDown_0:
        {
            const bool bRoofRight =
                CheckRoofCollision(FP_FromInteger(4) - step, -mineCarHeight) ||
                CheckRoofCollision(step + FP_FromInteger(2), -mineCarHeight);
            const bool bRoofLeft =
                CheckRoofCollision(-(step + FP_FromInteger(2)), -mineCarHeight) ||
                CheckRoofCollision(step - FP_FromInteger(4),    -mineCarHeight);

            if (
                (mVelX > FP_FromInteger(0) && !bRoofRight) ||
                (mVelX < FP_FromInteger(0) && !bRoofLeft)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Up;
                return TRUE;
            }
        }
        
        break;

        case MineCarDirs::eRight_1:
        {
            const bool bLeftWall1 =
                WallHit(mineCarHeight - FP_FromInteger(2), -(step + FP_FromInteger(4))) ||
                WallHit(-FP_FromInteger(2),                -(step + FP_FromInteger(4)));
            const bool bLeftWall2 =
                WallHit(mineCarHeight + FP_FromInteger(1), -(step + FP_FromInteger(4))) ||
                WallHit(FP_FromInteger(1),                 -(step + FP_FromInteger(4)));

            if (
                (mVelY > FP_FromInteger(0) && !bLeftWall1) ||
                (mVelY < FP_FromInteger(0) && !bLeftWall2)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Left;
                return TRUE;
            }
        }
        
        break;

        case MineCarDirs::eLeft_2:
        {
            const bool bRightWall1 =
                WallHit(mineCarHeight - FP_FromInteger(2), step + FP_FromInteger(4)) ||
                WallHit(-FP_FromInteger(2),                step + FP_FromInteger(4));
            const bool bRightWall2 =
                WallHit(mineCarHeight + FP_FromInteger(1), step + FP_FromInteger(4)) ||
                WallHit(FP_FromInteger(1),                 step + FP_FromInteger(4));

            if (
                (mVelY > FP_FromInteger(0) && !bRightWall1) ||
                (mVelY < FP_FromInteger(0) && !bRightWall2)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Right;
                return TRUE;
            }
        }
        
        break;

        case MineCarDirs::eUp_3:
        {
            const bool bFloorRight =
                CheckFloorCollision(FP_FromInteger(4) - step, FP_FromInteger(4)) ||
                CheckFloorCollision(step + FP_FromInteger(2), FP_FromInteger(4));

            const bool bFloorLeft =
                CheckFloorCollision(-(step + FP_FromInteger(2)), FP_FromInteger(4)) ||
                CheckFloorCollision(step - FP_FromInteger(4), FP_FromInteger(4));

            if (
                (mVelX > FP_FromInteger(0) && !bFloorRight) ||
                (mVelX < FP_FromInteger(0) && !bFloorLeft)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Down;
                return TRUE;
            }
        }
        
        break;
    }

    return FALSE;
}

void MineCar::RunThingsOver()
{
    PSX_RECT ourRect = VGetBoundingRect();
    ourRect.h += 6;

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // You can't run yourself over with a mine car it seems.
            if (pObj->Type() != ReliveTypes::eAbe)
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                if (pAliveObj->mSpriteScale == mSpriteScale ||
                   (pAliveObj->Type() == ReliveTypes::eSlog && mSpriteScale != FP_FromDouble(0.5)))
                {
                    const PSX_RECT targetRect = pAliveObj->VGetBoundingRect();
                    if (PSX_Rects_overlap_no_adjustment(&ourRect, &targetRect))
                    {
                        // Get run over by the mine car
                        pAliveObj->VTakeDamage(this);
                    }
                }
            }
        }
    }
}

s16 MineCar::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    return !mBaseGameObjectFlags.Get(BaseGameObject::eDead);
}

s32 MineCar::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<MineCar_SaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eMineCar_89;

    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;

    pState->field_C_velx = mVelX;
    pState->field_10_vely = mVelY;

    pState->field_18_path_number = mCurrentPath;
    pState->field_1A_lvl_number = MapWrapper::ToAE(mCurrentLevel);

    pState->field_14_sprite_scale = mSpriteScale;

    pState->field_1C_r = mRGB.r;
    pState->field_1E_g = mRGB.g;
    pState->field_20_b = mRGB.b;

    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_current_anim_frame = static_cast<s16>(mAnim.mCurrentFrame);
    pState->field_2C_frame_change_counter = static_cast<s16>(mAnim.mFrameChangeCounter);

    pState->field_2F_drawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_22_xFlip = mAnim.mFlags.Get(AnimFlags::eFlipX);
    pState->field_2E_render = mAnim.mFlags.Get(AnimFlags::eRender);

    // this makes no sense because we convert pState->field_24_frame_table back to the actual offset in CreateFromSaveState
    switch (mAnim.mAnimRes.mId)
    {
        case AnimId::Mine_Car_Tread_Move_B:
            pState->field_24_frame_table = 10860;
            break;
        case AnimId::Mine_Car_Open:
            pState->field_24_frame_table = 10884;
            break;
        case AnimId::Mine_Car_Tread_Idle:
            pState->field_24_frame_table = 10896;
            break;
        case AnimId::Mine_Car_Closed:
            pState->field_24_frame_table = 10908;
            break;
        case AnimId::Mine_Car_Tread_Move_A:
            pState->field_24_frame_table = 10920;
            break;
        case AnimId::Mine_Car_Shake_A:
            pState->field_24_frame_table = 10944;
            break;
        case AnimId::Mine_Car_Shake_B:
            pState->field_24_frame_table = 10972;
            break;
        default:
            break;
    }

    pState->field_34_unused = static_cast<s16>(mTreadAnim.mCurrentFrame);
    pState->field_36_unused = static_cast<s16>(mTreadAnim.mFrameChangeCounter);

    pState->field_32_unused = mTreadAnim.mFlags.Get(AnimFlags::eFlipX);
    pState->field_30_unused = mTreadAnim.mFlags.Get(AnimFlags::eRender);
    
    switch (mTreadAnim.mAnimRes.mId)
    {
        case AnimId::Mine_Car_Tread_Move_B:
            pState->field_38_frame_table_offset2 = 10860;
            break;
        case AnimId::Mine_Car_Open:
            pState->field_38_frame_table_offset2 = 10884;
            break;
        case AnimId::Mine_Car_Tread_Idle:
            pState->field_38_frame_table_offset2 = 10896;
            break;
        case AnimId::Mine_Car_Closed:
            pState->field_38_frame_table_offset2 = 10908;
            break;
        case AnimId::Mine_Car_Tread_Move_A:
            pState->field_38_frame_table_offset2 = 10920;
            break;
        case AnimId::Mine_Car_Shake_A:
            pState->field_38_frame_table_offset2 = 10944;
            break;
        case AnimId::Mine_Car_Shake_B:
            pState->field_38_frame_table_offset2 = 10972;
            break;
        default:
            break;
    }

    pState->field_3C_health = mHealth;
    pState->field_42_next_motion = mNextMotion;

    pState->field_40_current_motion = mCurrentMotion;

    pState->field_44_last_line_ypos = FP_GetExponent(FP_Abs(BaseAliveGameObjectLastLineYPos));

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_46_collision_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->field_46_collision_line_type = -1;
    }

    pState->field_5A_bAbeInCar = (this == sControlledCharacter);
    pState->field_4C_tlvInfo = field_118_tlvInfo;
    pState->field_50_state = field_11C_state;

    pState->field_52_turn_direction = field_1BC_turn_direction;
    pState->field_54_unused = field_1BE_unused;
    pState->field_56_unused = field_1C0_unused;

    pState->field_58_falling_counter = field_1C2_falling_counter;
    pState->field_5C_frame_mod_16 = field_1C8_frame_mod_16;

    pState->field_60_spawned_path = field_1CC_spawned_path;
    pState->field_62_spanwed_camera = field_1CE_spawned_camera;

    pState->field_64_throw_item_key1 = field_1D4_previous_input;
    pState->field_66_continue_move_input = field_1D6_continue_move_input;

    return sizeof(MineCar_SaveState);
}

void MineCar::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eRestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eRestoredFromQuickSave);

        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            sCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(20),
                mXPos,
                mYPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType))
            );

            BaseAliveGameObjectCollisionLineType = 0;
        }
        else
        {
            BaseAliveGameObjectCollisionLine = nullptr;
            BaseAliveGameObjectCollisionLineType = 0;
        }
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    switch (field_11C_state)
    {
        case MineCarStates::eParkedWithoutAbe_0:
            State_0_ParkedWithoutAbe();
            break;

        case MineCarStates::eParkedWithAbe_1:
            State_1_ParkedWithAbe();
            break;

        case MineCarStates::eMoving_2:
            State_2_Moving();
            break;

        case MineCarStates::eFalling_3:
            State_3_Falling();
            break;

        default:
            break;
    }

    if (Input().isPressed(sInputKey_DoAction))
    {
        if (
            (
                field_1BC_turn_direction != MineCarDirs::eUp_3 &&
                !IsBlocked(MineCarDirs::eUp_3, 0)
            ) ||
            !IsBlocked(MineCarDirs::eUp_3, 0)
        )
        {
            if (field_11C_state != MineCarStates::eParkedWithoutAbe_0)
            {
                field_11C_state = MineCarStates::eFalling_3;
            }
        }
    }

    if (
        field_1BC_turn_direction != MineCarDirs::eUp_3 ||
        IsBlocked(MineCarDirs::eUp_3, 0)
    )
    {
        return;
    }

    const FP mineCarWidth = mSpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions->Raycast(
            mXPos - mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            mXPos + mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? CollisionMask(eMineCarWall_12) : CollisionMask(eBackgroundMineCarWall_15)
        )
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
    }
}

void MineCar::State_0_ParkedWithoutAbe()
{
    const PSX_RECT carRect = VGetBoundingRect();
    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();

    if (
        sActiveHero->mCurrentMotion == eAbeMotions::Motion_117_InMineCar_4587C0 &&
        PSX_Rects_overlap_4FA0B0(&carRect, &abeRect) &&
        sActiveHero->mSpriteScale == mSpriteScale
    )
    {
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Closed));
        field_11C_state = MineCarStates::eParkedWithAbe_1;
        sControlledCharacter = this;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;
        mTreadAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;

        if (mSpriteScale == FP_FromDouble(0.5))
        {
            mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
            mTreadAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
        }

        SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 500, mSpriteScale);
    }
}

void MineCar::State_1_ParkedWithAbe()
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);
    const FP mineCarHeight = mSpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mSpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    VCheckCollisionLineStillValid(10);

    // Abe exits minecar
    //
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_117_InMineCar_4587C0)
    {
        sActiveHero->mXPos = mXPos;
        sActiveHero->mYPos = mYPos;
        field_11C_state = MineCarStates::eParkedWithoutAbe_0;
        
        mTreadAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Tread_Idle));
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Open));

        sControlledCharacter = sActiveHero;
        field_1CC_spawned_path = gMap.mCurrentPath;
        field_1CE_spawned_camera = gMap.mCurrentCamera;

        sActiveHero->VCheckCollisionLineStillValid(10);

        SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 500, mSpriteScale);

        if (mSpriteScale == FP_FromDouble(0.5))
        {
            mAnim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
            mTreadAnim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_Shadow_26;
            mTreadAnim.mRenderLayer = Layer::eLayer_Shadow_26;
        }

        return;
    }

    // Peform any movements
    //
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    // Horizontal movements
    //
    const FP rayCastY1 = mYPos - mineCarHeight;
    const FP rayCastY2 = mYPos;
    const FP velX = kGridSize / FP_FromInteger(4);
    const FP velY = FP_FromInteger(0);

    InputCommands::Enum inputKey = sInputKey_Right;

    if (
        (
            Input().isPressed(inputKey) ||
            (
                Input().isPressed(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) sInputKey_Left &&
                field_1BC_turn_direction != MineCarDirs::eUp_3 &&
                field_1BC_turn_direction != MineCarDirs::eDown_0
            )
        ) &&
        !(
            WallHit(mineCarHeight * FP_FromDouble(0.5), mineCarWidthAdjusted + FP_FromInteger(1)) &&
            WallHit(mineCarHeight - FP_FromInteger(8), mineCarWidthAdjusted + FP_FromInteger(1))
        )
    )
    {
        const FP rayCastX = mXPos + velX;
        
        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(2);
        const FP hitX2 = FP_FromInteger(4) - mineCarWidthAdjusted;
        if (
            HandleState1Move(
                &MineCar::CheckFloorCollision,
                hitX,
                FP_FromInteger(4),
                hitX2,
                AnimId::Mine_Car_Shake_A,
                MineCarDirs::eUp_3,
                FALSE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                CollisionMask(eMineCarFloor_11),
                CollisionMask(eBackgroundMineCarFloor_14),
                velX,
                velY,
                inputKey,
                false,
                false
            ) ||
            HandleState1Move(
                &MineCar::CheckRoofCollision,
                hitX,
                -mineCarHeight,
                hitX2,
                AnimId::Mine_Car_Shake_A,
                MineCarDirs::eDown_0,
                TRUE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                CollisionMask(eMineCarCeiling_13),
                CollisionMask(eBackgroundMineCarCeiling_16),
                velX,
                velY,
                inputKey,
                false,
                false
            )
        )
        {
            return;
        }
    }
    else
    {
        if (WallHit(mineCarHeight * FP_FromDouble(0.5), mineCarWidthAdjusted + FP_FromInteger(1)))
        {
            if (Input().isPressed(inputKey))
            {
                if (field_1BC_turn_direction != MineCarDirs::eLeft_2 && !(static_cast<s32>(sGnFrame) % 6))
                {
                    SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, mSpriteScale);
                }
            }
        }
    }

    inputKey = sInputKey_Left;

    if (
        (
            Input().isPressed(inputKey) ||
            (
                Input().isPressed(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) sInputKey_Right &&
                field_1BC_turn_direction != MineCarDirs::eUp_3 &&
                field_1BC_turn_direction != MineCarDirs::eDown_0
            )
        ) &&
        !(
            WallHit(mineCarHeight * FP_FromDouble(0.5), -mineCarWidthAdjusted) &&
            WallHit(mineCarHeight - FP_FromInteger(8), -mineCarWidthAdjusted)
        )
    )
    {
        const FP rayCastX = mXPos - velX;
        
        const FP hitX = mineCarWidthAdjusted - FP_FromInteger(4);
        const FP hitX2 = -(mineCarWidthAdjusted + FP_FromInteger(2));

        if (
            HandleState1Move(
                &MineCar::CheckFloorCollision,
                hitX,
                FP_FromInteger(4),
                hitX2,
                AnimId::Mine_Car_Shake_B,
                MineCarDirs::eUp_3,
                TRUE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                CollisionMask(eMineCarFloor_11),
                CollisionMask(eBackgroundMineCarFloor_14),
                -velX,
                velY,
                inputKey,
                false,
                false
            ) ||
            HandleState1Move(
                &MineCar::CheckRoofCollision,
                hitX,
                -mineCarHeight,
                hitX2,
                AnimId::Mine_Car_Shake_B,
                MineCarDirs::eDown_0,
                FALSE,
                rayCastX,
                rayCastY1,
                rayCastX,
                rayCastY2,
                CollisionMask(eMineCarCeiling_13),
                CollisionMask(eBackgroundMineCarCeiling_16),
                -velX,
                velY,
                inputKey,
                false,
                false
            )
        )
        {
            return;
        }
    }
    else
    {
        if (Input().isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eRight_1 && !(static_cast<s32>(sGnFrame) % 6))
            {
                SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, mSpriteScale);
            }
        }
    }

    HandleUpDown();
}

bool MineCar::HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3,
                               AnimId animId, MineCarDirs mineCarDir, const s8 bChangeDir, FP rayCastX1, FP rayCastY1, FP rayCastX2, FP rayCastY2, const CollisionMask ModelMask1, const CollisionMask ModelMask2,
                               FP velX, FP velY, InputCommands::Enum key, bool isVertical, bool verticalFlipXCond)
{
    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};

    if (
        sCollisions->Raycast(
            rayCastX1,
            rayCastY1,
            rayCastX2,
            rayCastY2,
            &pPathLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? ModelMask1 : ModelMask2
        )
    )
    {
        if ((this->*func)(mineCarFPFuncArg1, mineCarFPFuncArg2) || (this->*func)(mineCarFPFuncArg3, mineCarFPFuncArg2))
        {
            if (
                !isVertical ||
                (
                    (verticalFlipXCond  && hitX > mXPos) ||
                    (!verticalFlipXCond && hitX < mXPos)
                )
            )
            {
                Move(animId, velX, velY, key, mineCarDir, bChangeDir);
                return true;
            }
        }
    }

    return false;
}

void MineCar::HandleUpDown()
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);
    const FP mineCarWidth = mSpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;
    const FP mineCarHeight = mSpriteScale * mineCarHeightUnscaled;
    const FP k5Scaled = FP_FromInteger(5) * mSpriteScale;

    const FP rayCastX1 = mXPos - mineCarWidthAdjusted;
    const FP rayCastX2 = mXPos + mineCarWidthAdjusted;
    
    const FP velX = FP_FromInteger(0);
    const FP velY = k5Scaled;

    InputCommands::Enum inputKey = sInputKey_Up;

    if (
        Input().isPressed(inputKey) ||
        (
            Input().isPressed(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) sInputKey_Down &&
            field_1BC_turn_direction != MineCarDirs::eLeft_2 &&
            field_1BC_turn_direction != MineCarDirs::eRight_1 &&
            !IsBlocked(MineCarDirs::eDown_0, 0)
        )
    )
    {
        const FP rayCastY = mYPos - velY - (mineCarWidthAdjusted * FP_FromDouble(0.5));

        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitY = FP_FromInteger(1);
        const FP hitY2 = mineCarHeight + FP_FromInteger(1);

        if (
            HandleState1Move(
                &MineCar::WallHit,
                hitY,
                hitX,
                hitY2,
                AnimId::Mine_Car_Closed,
                MineCarDirs::eLeft_2,
                FALSE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                CollisionMask(eMineCarWall_12),
                CollisionMask(eBackgroundMineCarWall_15),
                velX,
                -velY,
                inputKey,
                true,
                true
            ) ||
            HandleState1Move(
                &MineCar::WallHit,
                hitY,
                -hitX,
                hitY2,
                AnimId::Mine_Car_Closed,
                MineCarDirs::eRight_1,
                TRUE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                CollisionMask(eMineCarWall_12),
                CollisionMask(eBackgroundMineCarWall_15),
                velX,
                -velY,
                inputKey,
                true,
                false
            )
        )
        {
            return;
        }
    }
    else if (IsBlocked(MineCarDirs::eDown_0, 0))
    {
        if (Input().isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eDown_0)
            {
                if (!(static_cast<s32>(sGnFrame) % 6))
                {
                    SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, mSpriteScale);
                }
            }
        }
    }

    inputKey = sInputKey_Down;

    if (
        Input().isPressed(inputKey) ||
        (
            Input().isPressed(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) sInputKey_Up &&
            field_1BC_turn_direction != MineCarDirs::eLeft_2 &&
            field_1BC_turn_direction != MineCarDirs::eRight_1 &&
            !IsBlocked(MineCarDirs::eUp_3, 0)
        )
    )
    {
        const FP rayCastY = mYPos + velY;

        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitY = -FP_FromInteger(2);
        const FP hitY2 = mineCarHeight - FP_FromInteger(1);

        if (
            HandleState1Move(
                &MineCar::WallHit,
                hitY,
                hitX,
                hitY2,
                AnimId::Mine_Car_Closed,
                MineCarDirs::eLeft_2,
                TRUE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                CollisionMask(eMineCarWall_12),
                CollisionMask(eBackgroundMineCarWall_15),
                velX,
                velY,
                inputKey,
                true,
                true
            ) ||
            HandleState1Move(
                &MineCar::WallHit,
                hitY,
                -hitX,
                hitY2,
                AnimId::Mine_Car_Closed,
                MineCarDirs::eRight_1,
                FALSE,
                rayCastX1,
                rayCastY,
                rayCastX2,
                rayCastY,
                CollisionMask(eMineCarWall_12),
                CollisionMask(eBackgroundMineCarWall_15),
                velX,
                velY,
                inputKey,
                true,
                false
            )
        )
        {
            return;
        }
    }
    else if (IsBlocked(MineCarDirs::eUp_3, 0))
    {
        if (Input().isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eUp_3 && !(static_cast<s32>(sGnFrame) % 6))
            {
                SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, mSpriteScale);
            }
        }
    }
}

// TODO: Probably 7?
const FP velXTable_5461D8[9] = {
    FP_FromInteger(6),
    FP_FromInteger(5),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(3),
    FP_FromInteger(2),
    FP_FromInteger(1),
    FP_FromInteger(0),
    FP_FromInteger(0)};


void MineCar::State_2_Moving()
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);
    const FP mineCarHeight = mSpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mSpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    sActiveHero->mXPos = mXPos;
    sActiveHero->mYPos = mYPos;

    if (!field_1D0_sound_channels_mask)
    {
        // Play the mine car moving sound
        //
        field_1D0_sound_channels_mask = SfxPlayMono(relive::SoundEffects::MinecarMovement, 127, mSpriteScale);
    }

    if (FollowDirection() && !field_1C4_velx_index)
    {
        Stop();
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions->Raycast(
            mXPos - mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            mXPos + mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? CollisionMask(eMineCarWall_12) : CollisionMask(eBackgroundMineCarWall_15)
        ) &&
        mVelY > FP_FromInteger(0)
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !sCollisions->Raycast(
            mXPos + mVelX,
            mYPos,
            mXPos + mVelX,
            mYPos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? CollisionMask(eMineCarCeiling_13) : CollisionMask(eBackgroundMineCarCeiling_16)
        ) &&
        field_1BC_turn_direction == MineCarDirs::eDown_0
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !sCollisions->Raycast(
            mXPos + mVelX,
            mYPos,
            mXPos + mVelX,
            mYPos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? CollisionMask(eMineCarFloor_11) : CollisionMask(eBackgroundMineCarFloor_14)
        ) &&
        field_1BC_turn_direction == MineCarDirs::eUp_3
    )
    {
        Stop();
        return;
    }

    if (field_1C4_velx_index)
    {
        if (field_1C4_velx_index >= 7)
        {
            Stop();
            return;
        }

        if (mVelX <= FP_FromInteger(0))
        {
            mVelX = -velXTable_5461D8[field_1C4_velx_index];
        }
        else
        {
            mVelX = velXTable_5461D8[field_1C4_velx_index];
        }

        if (++field_1C4_velx_index == 2)
        {
            ++mTreadAnim.mFrameDelay;
        }
    }

    if (Input().isPressed(field_1D4_previous_input))
    {
        mXPos += mVelX;
        mYPos += mVelY;

        SetActiveCameraDelayedFromDir();
        RunThingsOver();

        return;
    }

    if (
        field_1BC_turn_direction == MineCarDirs::eUp_3 ||
        field_1BC_turn_direction == MineCarDirs::eDown_0
    )
    {
        if (
            mXPos == FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos))) &&
            !field_1C4_velx_index
        )
        {
            if (mVelX <= FP_FromInteger(0))
            {
                mVelX = -velXTable_5461D8[0];
            }
            else
            {
                mVelX = velXTable_5461D8[0];
            }

            ++field_1C4_velx_index;
        }

        mXPos += mVelX;
        mYPos += mVelY;

        SetActiveCameraDelayedFromDir();
        RunThingsOver();

        return;
    }

    Stop();
}

void MineCar::State_3_Falling()
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);
    const FP mineCarHeight = mSpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mSpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    SetActiveCameraDelayedFromDir();
    field_1C2_falling_counter++;

    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (
            WallHit(mineCarHeight, mineCarWidthAdjusted) ||
            WallHit(FP_FromInteger(0), mineCarWidthAdjusted)
        )
        {
            mVelX = FP_FromInteger(0);

            relive_new ParticleBurst(
                sControlledCharacter->mXPos + mineCarHeight + kGridSize,
                sControlledCharacter->mYPos - ((mineCarHeight + kGridSize) * FP_FromDouble(0.5)),
                4u,
                mSpriteScale,
                BurstType::eBigRedSparks_3,
                9
            );

            SfxPlayMono(relive::SoundEffects::FallingItemHit, 80, mSpriteScale);
        }
    }

    if (mVelX < FP_FromInteger(0))
    {
        if (
            WallHit(mineCarHeight, -mineCarWidthAdjusted) ||
            WallHit(FP_FromInteger(0), -mineCarWidthAdjusted)
        )
        {
            mVelX = FP_FromInteger(0);

            relive_new ParticleBurst(
                sControlledCharacter->mXPos - (mineCarHeight + kGridSize),
                sControlledCharacter->mYPos - ((mineCarHeight + kGridSize) * FP_FromDouble(0.5)),
                4u,
                mSpriteScale,
                BurstType::eBigRedSparks_3,
                9
            );

            SfxPlayMono(relive::SoundEffects::FallingItemHit, 80, mSpriteScale);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        mYPos = hitY;
        BaseAliveGameObjectLastLineYPos = hitY;
        mVelY = (-mVelY * FP_FromDouble(0.2));

        relive_new ParticleBurst(
            sControlledCharacter->mXPos,
            sControlledCharacter->mYPos,
            5u,
            FP_FromInteger(1),
            BurstType::eBigRedSparks_3,
            9
        );

        if (field_1C2_falling_counter > 4)
        {
            SFX_Play_Pitch(relive::SoundEffects::MinecarStop, 127, 0, mSpriteScale);
            SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 127, 0, mSpriteScale);

            relive_new ScreenShake(FALSE, FALSE);
        }

        field_1C2_falling_counter = 0;

        if (mVelY > -FP_FromInteger(1))
        {
            SFX_Play_Pitch(relive::SoundEffects::MinecarStop, 120, 0, mSpriteScale);
            SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 70, -800, mSpriteScale);

            mVelY = FP_FromInteger(0);
            BaseAliveGameObjectCollisionLine = pPathLine;
            mYPos = hitY;

            if (FP_GetExponent(mVelX))
            {
                mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));
            }

            BaseAliveGameObjectLastLineYPos = mYPos;

            Stop();

            sActiveHero->BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine;
            sActiveHero->mXPos = mXPos;
            sActiveHero->mYPos = mYPos;
            field_1BC_turn_direction = MineCarDirs::eUp_3;
        }
    }

    RunThingsOver();

    return;
}
