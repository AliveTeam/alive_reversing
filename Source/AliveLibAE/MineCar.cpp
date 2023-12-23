#include "stdafx.h"
#include "MineCar.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/Function.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/ParticleBurst.hpp"
#include "../relive_lib/GameObjects/ScreenShake.hpp"
#include "../relive_lib/Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/Psx.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "QuikSave.hpp"

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

    SetDoPurpleLightEffect(true);
    GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_26);
    SetSpriteScale(FP_FromInteger(1));
    SetScale(Scale::Fg);

    if (field_11E_scale != relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_Half_7);
        SetScale(Scale::Bg);
    }

    MapFollowMe(true);

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    field_118_tlvInfo = tlvId;
    field_11C_state = MineCarStates::eParkedWithoutAbe_0;

    LoadAnimation(&mTreadAnim);

    SetCanExplode(true);

    CreateShadow();

    field_1C2_falling_counter = 0;

    // What was pressed before we started to move
    field_1D4_previous_input = static_cast<s16>(InputCommands::eThrowItem);

    // Set to a key that keeps the car moving, for example if moving right and we hit a right wall that a car
    // can travel "up" then we set this key to "up" such that holding down "right" automatically moves the car up.
    field_1D6_continue_move_input = static_cast<s16>(InputCommands::eThrowItem);

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

void MineCar::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<MineCarSaveState>();
    auto pTlv = static_cast<relive::Path_MineCar*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_4C_tlvInfo));

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
        pMineCar->mCurrentLevel = pState->field_1A_lvl_number;

        pMineCar->SetSpriteScale(pState->field_14_sprite_scale);

        pMineCar->mRGB.SetRGB(pState->field_1C_r, pState->field_1E_g, pState->field_20_b);

        pMineCar->mCurrentMotion = pState->field_28_current_motion;

        // TODO: replace Set_Animation_Data sMineCarAnimIdTable[] with the actual anim id's
        switch (pState->field_24_frame_table)
        {
        case 10860:
            pMineCar->GetAnimation().Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[6]));
            break;
        case 10884:
            pMineCar->GetAnimation().Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[1]));
            break;
        case 10896:
            pMineCar->GetAnimation().Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[4]));
            break;
        case 10908:
            pMineCar->GetAnimation().Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[0]));
            break;
        case 10920:
            pMineCar->GetAnimation().Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[5]));
            break;
        case 10944:
            pMineCar->GetAnimation().Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[2]));
            break;
        case 10972:
            pMineCar->GetAnimation().Set_Animation_Data(pMineCar->GetAnimRes(sMineCarAnimIdTable[3]));
            break;
        default:
            break;
        }

        pMineCar->GetAnimation().SetCurrentFrame(pState->field_2A_current_anim_frame);


        pMineCar->GetAnimation().SetFrameChangeCounter(pState->field_2C_frame_change_counter);

        pMineCar->GetAnimation().SetFlipX(pState->field_22_xFlip & 1);
        pMineCar->GetAnimation().SetRender(pState->field_2E_render & 1);

        pMineCar->SetDrawable(pState->field_2F_drawable & 1);

        if (IsLastFrame(&pMineCar->GetAnimation()))
        {
            pMineCar->GetAnimation().SetIsLastFrame(true);
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

        pMineCar->mTreadAnim.SetCurrentFrame(pState->field_2A_current_anim_frame);


        // TODO: OG Bug same flags but save state saves 2 sets one for each anim ??
        pMineCar->mTreadAnim.SetFlipX(pState->field_22_xFlip & 1);
        pMineCar->mTreadAnim.SetRender(pState->field_2E_render & 1);


        pMineCar->mTreadAnim.SetFrameChangeCounter(pState->field_2C_frame_change_counter);

        pMineCar->mHealth = pState->field_3C_health;
        pMineCar->mCurrentMotion = pState->field_40_current_motion;

        pMineCar->mNextMotion = pState->field_42_next_motion;

        pMineCar->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_44_last_line_ypos);

        pMineCar->SetRestoredFromQuickSave(true);

        pMineCar->BaseAliveGameObjectCollisionLineType = pState->field_46_collision_line_type;
        pMineCar->field_118_tlvInfo = pState->field_4C_tlvInfo;

        pMineCar->field_11C_state = pState->field_50_state;
        pMineCar->field_1BC_turn_direction = pState->field_52_turn_direction;

        pMineCar->field_1C2_falling_counter = pState->field_58_falling_counter;

        pMineCar->field_1C8_frame_mod_16 = pState->field_5C_frame_mod_16;

        pMineCar->field_1CC_spawned_path = pState->field_60_spawned_path;
        pMineCar->field_1CE_spawned_camera = pState->field_62_spawned_camera;

        pMineCar->field_1D4_previous_input = pState->field_64_throw_item_key1;
        pMineCar->field_1D6_continue_move_input = pState->field_66_continue_move_input;

        if (pMineCar->mVelX < (ScaleToGridSize(FP_FromInteger(1)) / FP_FromInteger(4)))
        {
            pMineCar->field_1C4_velx_index = 7;
        }
    }
}

void MineCar::LoadAnimation(Animation* pAnim)
{
    if (pAnim->Init(GetAnimRes(AnimId::Mine_Car_Tread_Idle), this))
    {
        pAnim->SetRenderLayer(GetAnimation().GetRenderLayer());
        pAnim->SetBlending(false);
        pAnim->SetSpriteScale(GetSpriteScale());
        pAnim->SetRGB(128, 128, 128);
    }
    else
    {
        SetListAddFailed(true);
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
    return gCollisions->Raycast(
        mXPos + hitX,
        mYPos,
        mXPos + hitX,
        mYPos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        GetScale() == Scale::Fg ? CollisionMask(eCeiling_3) : CollisionMask(eBackgroundCeiling_7)
    );
}

bool MineCar::CheckFloorCollision(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;

    if (
        !gCollisions->Raycast(
            mXPos + hitX,
            mYPos - FP_FromInteger(4),
            mXPos + hitX,
            mYPos + hitY,
            &pPathLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor
        )
    )
    {
        return false;
    }

    if (mYPos > hitY)
    {
        mYPos = hitY;
    }

    return true;
}

void MineCar::VRender(OrderingTable& ot)
{
    if (GetAnimation().GetRender())
    {
        s16 r = mRGB.g;
        s16 g = mRGB.r;
        s16 b = mRGB.b;

        const PSX_RECT bRect = VGetBoundingRect();

        if (GetApplyShadowZoneColour())
        {
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mXPos),
                (bRect.h + bRect.y) / 2,
                GetScale(),
                &r,
                &g,
                &b);
        }

        mTreadAnim.SetRGB(r, g, b);

        if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos + FP_FromInteger(30), mYPos, 0) || gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos - (GetSpriteScale() * FP_FromInteger(60)), 0) || gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos - FP_FromInteger(30), mYPos, 0))
        {
            mTreadAnim.VRender(
                FP_GetExponent(mXPos - gScreenManager->CamXPos()),
                FP_GetExponent(mYPos - gScreenManager->CamYPos()),
                ot,
                0,
                0);
        }

        BaseAnimatedWithPhysicsGameObject::VRender(ot);
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

    SfxPlayMono(relive::SoundEffects::MinecarStop, 127, GetSpriteScale());
    
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Closed));
    mTreadAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Tread_Idle));

    field_1C4_velx_index = 0;

    mXPos = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos)));
}

void MineCar::Move(AnimId animId, FP velX, FP velY, InputCommands input, MineCarDirs turnDirection, s8 bChangeDirection)
{
    GetAnimation().Set_Animation_Data(GetAnimRes(animId));

    field_11C_state = MineCarStates::eMoving_2;
    field_1C8_frame_mod_16 = static_cast<s32>(sGnFrame) % 16;

    if (!field_1D0_sound_channels_mask)
    {
        field_1D0_sound_channels_mask = SfxPlayMono(relive::SoundEffects::MinecarMovement, 127, GetSpriteScale());
    }

    mTreadAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Tread_Move_A));

    mVelX = velX;
    mVelY = velY;

    if (Input().IsAnyHeld(input))
    {
        field_1D4_previous_input = static_cast<s16>(input);
    }

    field_1BC_turn_direction = turnDirection;

    mTreadAnim.SetLoopBackwards(bChangeDirection);
}

s16 MineCar::IsBlocked(MineCarDirs a2, s32 /*a3*/)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP mineCarHeight = GetSpriteScale() * mineCarHeightUnscaled;
    const FP mineCarWidth = GetSpriteScale() * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    switch (a2)
    {
        case MineCarDirs::eDown_0:
        {
            if (!CheckRoofCollision(FP_FromInteger(4) - mineCarWidthAdjusted, -(mineCarHeight) + FP_FromInteger(1)))
            {
                if (!CheckRoofCollision(mineCarWidthAdjusted - FP_FromInteger(4), -(mineCarHeight + FP_FromInteger(1))))
                {
                    return false;
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
                    return false;
                }
            }
        }
        break;
    }

    return true;
}

s16 MineCar::FollowDirection()
{
    const FP mineCarHeight = GetSpriteScale() * mineCarHeightUnscaled;
    const FP mineCarWidth = GetSpriteScale() * mineCarWidthUnscaled;
    const FP stepSize = ScaleToGridSize(GetSpriteScale());

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
            field_1D6_continue_move_input = (u16) InputCommands::eUp;
        }
        else
        {
            field_1D6_continue_move_input = (u16) InputCommands::eDown;
        }

        return true;
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
            field_1D6_continue_move_input = (u16) InputCommands::eLeft;
        }
        else
        {
            field_1D6_continue_move_input = (u16) InputCommands::eRight;
        }

        return true;
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
                field_1D6_continue_move_input = (u16) InputCommands::eUp;
                return true;
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
                field_1D6_continue_move_input = (u16) InputCommands::eLeft;
                return true;
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
                field_1D6_continue_move_input = (u16) InputCommands::eRight;
                return true;
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
                field_1D6_continue_move_input = (u16) InputCommands::eDown;
                return true;
            }
        }
        
        break;
    }

    return false;
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

        if (pObj->GetIsBaseAliveGameObject())
        {
            // You can't run yourself over with a mine car it seems.
            if (pObj->Type() != ReliveTypes::eAbe)
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                if (pAliveObj->GetSpriteScale() == GetSpriteScale() ||
                   (pAliveObj->Type() == ReliveTypes::eSlog && GetSpriteScale() != FP_FromDouble(0.5)))
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

bool MineCar::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    return !GetDead();
}

void MineCar::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    MineCarSaveState data = {};

    data.mType = ReliveTypes::eMineCar;

    data.field_4_xpos = mXPos;
    data.field_8_ypos = mYPos;

    data.field_C_velx = mVelX;
    data.field_10_vely = mVelY;

    data.field_18_path_number = mCurrentPath;
    data.field_1A_lvl_number = mCurrentLevel;

    data.field_14_sprite_scale = GetSpriteScale();

    data.field_1C_r = mRGB.r;
    data.field_1E_g = mRGB.g;
    data.field_20_b = mRGB.b;

    data.field_28_current_motion = mCurrentMotion;
    data.field_2A_current_anim_frame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    data.field_2C_frame_change_counter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());

    data.field_2F_drawable = GetDrawable();
    data.field_22_xFlip = GetAnimation().GetFlipX();
    data.field_2E_render = GetAnimation().GetRender();

    // this makes no sense because we convert data.field_24_frame_table back to the actual offset in CreateFromSaveState
    switch (GetAnimation().mAnimRes.mId)
    {
        case AnimId::Mine_Car_Tread_Move_B:
            data.field_24_frame_table = 10860;
            break;
        case AnimId::Mine_Car_Open:
            data.field_24_frame_table = 10884;
            break;
        case AnimId::Mine_Car_Tread_Idle:
            data.field_24_frame_table = 10896;
            break;
        case AnimId::Mine_Car_Closed:
            data.field_24_frame_table = 10908;
            break;
        case AnimId::Mine_Car_Tread_Move_A:
            data.field_24_frame_table = 10920;
            break;
        case AnimId::Mine_Car_Shake_A:
            data.field_24_frame_table = 10944;
            break;
        case AnimId::Mine_Car_Shake_B:
            data.field_24_frame_table = 10972;
            break;
        default:
            break;
    }
    
    switch (mTreadAnim.mAnimRes.mId)
    {
        case AnimId::Mine_Car_Tread_Move_B:
            data.field_38_frame_table_offset2 = 10860;
            break;
        case AnimId::Mine_Car_Open:
            data.field_38_frame_table_offset2 = 10884;
            break;
        case AnimId::Mine_Car_Tread_Idle:
            data.field_38_frame_table_offset2 = 10896;
            break;
        case AnimId::Mine_Car_Closed:
            data.field_38_frame_table_offset2 = 10908;
            break;
        case AnimId::Mine_Car_Tread_Move_A:
            data.field_38_frame_table_offset2 = 10920;
            break;
        case AnimId::Mine_Car_Shake_A:
            data.field_38_frame_table_offset2 = 10944;
            break;
        case AnimId::Mine_Car_Shake_B:
            data.field_38_frame_table_offset2 = 10972;
            break;
        default:
            break;
    }

    data.field_3C_health = mHealth;
    data.field_42_next_motion = mNextMotion;

    data.field_40_current_motion = mCurrentMotion;

    data.field_44_last_line_ypos = FP_GetExponent(FP_Abs(BaseAliveGameObjectLastLineYPos));

    if (BaseAliveGameObjectCollisionLine)
    {
        data.field_46_collision_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        data.field_46_collision_line_type = eLineTypes::eNone_m1;
    }

    data.field_5A_bAbeInCar = (this == sControlledCharacter);
    data.field_4C_tlvInfo = field_118_tlvInfo;
    data.field_50_state = field_11C_state;

    data.field_52_turn_direction = field_1BC_turn_direction;

    data.field_58_falling_counter = field_1C2_falling_counter;
    data.field_5C_frame_mod_16 = field_1C8_frame_mod_16;

    data.field_60_spawned_path = field_1CC_spawned_path;
    data.field_62_spawned_camera = field_1CE_spawned_camera;

    data.field_64_throw_item_key1 = field_1D4_previous_input;
    data.field_66_continue_move_input = field_1D6_continue_move_input;

    pSaveBuffer.Write(data);
}

void MineCar::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);

        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            gCollisions->Raycast(
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
        SetDead(true);
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

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

    if (Input().IsAnyHeld(InputCommands::eDoAction))
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

    const FP mineCarWidth = GetSpriteScale() * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (
        !gCollisions->Raycast(
            mXPos - mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            mXPos + mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? CollisionMask(eMineCarWall_12) : CollisionMask(eBackgroundMineCarWall_15)
        )
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
    }
}

void MineCar::State_0_ParkedWithoutAbe()
{
    const PSX_RECT carRect = VGetBoundingRect();
    const PSX_RECT abeRect = gAbe->VGetBoundingRect();

    if (
        gAbe->mCurrentMotion == eAbeMotions::Motion_117_InMineCar &&
        PSX_Rects_overlap_4FA0B0(&carRect, &abeRect) &&
        gAbe->GetSpriteScale() == GetSpriteScale()
    )
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Closed));
        field_11C_state = MineCarStates::eParkedWithAbe_1;
        sControlledCharacter = this;
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        mTreadAnim.SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);

        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
            mTreadAnim.SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
        }

        SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 500, GetSpriteScale());
    }
}

void MineCar::State_1_ParkedWithAbe()
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP mineCarHeight = GetSpriteScale() * mineCarHeightUnscaled;
    const FP mineCarWidth = GetSpriteScale() * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    VCheckCollisionLineStillValid(10);

    // Abe exits minecar
    //
    if (gAbe->mCurrentMotion != eAbeMotions::Motion_117_InMineCar)
    {
        gAbe->mXPos = mXPos;
        gAbe->mYPos = mYPos;
        field_11C_state = MineCarStates::eParkedWithoutAbe_0;
        
        mTreadAnim.Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Tread_Idle));
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Mine_Car_Open));

        sControlledCharacter = gAbe;
        field_1CC_spawned_path = gMap.mCurrentPath;
        field_1CE_spawned_camera = gMap.mCurrentCamera;

        gAbe->VCheckCollisionLineStillValid(10);

        SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 500, GetSpriteScale());

        if (GetSpriteScale() == FP_FromDouble(0.5))
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_Half_7);
            mTreadAnim.SetRenderLayer(Layer::eLayer_Shadow_Half_7);
        }
        else
        {
            GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_26);
            mTreadAnim.SetRenderLayer(Layer::eLayer_Shadow_26);
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

    InputCommands inputKey = InputCommands::eRight;

    if (
        (
            Input().IsAnyHeld(inputKey) ||
            (
                Input().IsAnyHeld(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) InputCommands::eLeft &&
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
                false,
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
                true,
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
            if (Input().IsAnyHeld(inputKey))
            {
                if (field_1BC_turn_direction != MineCarDirs::eLeft_2 && !(static_cast<s32>(sGnFrame) % 6))
                {
                    SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, GetSpriteScale());
                }
            }
        }
    }

    inputKey = InputCommands::eLeft;

    if (
        (
            Input().IsAnyHeld(inputKey) ||
            (
                Input().IsAnyHeld(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) InputCommands::eRight &&
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
                true,
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
                false,
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
        if (Input().IsAnyHeld(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eRight_1 && !(static_cast<s32>(sGnFrame) % 6))
            {
                SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, GetSpriteScale());
            }
        }
    }

    HandleUpDown();
}

bool MineCar::HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3,
                               AnimId animId, MineCarDirs mineCarDir, const s8 bChangeDir, FP rayCastX1, FP rayCastY1, FP rayCastX2, FP rayCastY2, const CollisionMask ModelMask1, const CollisionMask ModelMask2,
                               FP velX, FP velY, InputCommands key, bool isVertical, bool verticalFlipXCond)
{
    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};

    if (
        gCollisions->Raycast(
            rayCastX1,
            rayCastY1,
            rayCastX2,
            rayCastY2,
            &pPathLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? ModelMask1 : ModelMask2
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
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP mineCarWidth = GetSpriteScale() * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;
    const FP mineCarHeight = GetSpriteScale() * mineCarHeightUnscaled;
    const FP k5Scaled = FP_FromInteger(5) * GetSpriteScale();

    const FP rayCastX1 = mXPos - mineCarWidthAdjusted;
    const FP rayCastX2 = mXPos + mineCarWidthAdjusted;
    
    const FP velX = FP_FromInteger(0);
    const FP velY = k5Scaled;

    InputCommands inputKey = InputCommands::eUp;

    if (
        Input().IsAnyHeld(inputKey) ||
        (
            Input().IsAnyHeld(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) InputCommands::eDown &&
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
                false,
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
                true,
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
        if (Input().IsAnyHeld(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eDown_0)
            {
                if (!(static_cast<s32>(sGnFrame) % 6))
                {
                    SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, GetSpriteScale());
                }
            }
        }
    }

    inputKey = InputCommands::eDown;

    if (
        Input().IsAnyHeld(inputKey) ||
        (
            Input().IsAnyHeld(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) InputCommands::eUp &&
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
                true,
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
                false,
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
        if (Input().IsAnyHeld(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eUp_3 && !(static_cast<s32>(sGnFrame) % 6))
            {
                SfxPlayMono(relive::SoundEffects::MinecarStuck, 127, GetSpriteScale());
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
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP mineCarHeight = GetSpriteScale() * mineCarHeightUnscaled;
    const FP mineCarWidth = GetSpriteScale() * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    gAbe->mXPos = mXPos;
    gAbe->mYPos = mYPos;

    if (!field_1D0_sound_channels_mask)
    {
        // Play the mine car moving sound
        //
        field_1D0_sound_channels_mask = SfxPlayMono(relive::SoundEffects::MinecarMovement, 127, GetSpriteScale());
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
        !gCollisions->Raycast(
            mXPos - mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            mXPos + mineCarWidthAdjusted,
            mYPos + mVelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? CollisionMask(eMineCarWall_12) : CollisionMask(eBackgroundMineCarWall_15)
        ) &&
        mVelY > FP_FromInteger(0)
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !gCollisions->Raycast(
            mXPos + mVelX,
            mYPos,
            mXPos + mVelX,
            mYPos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? CollisionMask(eMineCarCeiling_13) : CollisionMask(eBackgroundMineCarCeiling_16)
        ) &&
        field_1BC_turn_direction == MineCarDirs::eDown_0
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !gCollisions->Raycast(
            mXPos + mVelX,
            mYPos,
            mXPos + mVelX,
            mYPos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? CollisionMask(eMineCarFloor_11) : CollisionMask(eBackgroundMineCarFloor_14)
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
            mTreadAnim.SetFrameDelay(mTreadAnim.GetFrameDelay() + 1);
        }
    }

    if (Input().IsAnyHeld(field_1D4_previous_input))
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
            mXPos == FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos))) &&
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
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP mineCarHeight = GetSpriteScale() * mineCarHeightUnscaled;
    const FP mineCarWidth = GetSpriteScale() * mineCarWidthUnscaled;
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
                GetSpriteScale(),
                BurstType::eBigRedSparks,
                9, true
            );

            SfxPlayMono(relive::SoundEffects::FallingItemHit, 80, GetSpriteScale());
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
                GetSpriteScale(),
                BurstType::eBigRedSparks,
                9, true
            );

            SfxPlayMono(relive::SoundEffects::FallingItemHit, 80, GetSpriteScale());
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
            BurstType::eBigRedSparks,
            9, true
        );

        if (field_1C2_falling_counter > 4)
        {
            SFX_Play_Pitch(relive::SoundEffects::MinecarStop, 127, 0, GetSpriteScale());
            SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 127, 0, GetSpriteScale());

            relive_new ScreenShake(false, false);
        }

        field_1C2_falling_counter = 0;

        if (mVelY > -FP_FromInteger(1))
        {
            SFX_Play_Pitch(relive::SoundEffects::MinecarStop, 120, 0, GetSpriteScale());
            SFX_Play_Pitch(relive::SoundEffects::FallingItemHit, 70, -800, GetSpriteScale());

            mVelY = FP_FromInteger(0);
            BaseAliveGameObjectCollisionLine = pPathLine;
            mYPos = hitY;

            if (FP_GetExponent(mVelX))
            {
                mXPos = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos)));
            }

            BaseAliveGameObjectLastLineYPos = mYPos;

            Stop();

            gAbe->BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine;
            gAbe->mXPos = mXPos;
            gAbe->mYPos = mYPos;
            field_1BC_turn_direction = MineCarDirs::eUp_3;
        }
    }

    RunThingsOver();

    return;
}
