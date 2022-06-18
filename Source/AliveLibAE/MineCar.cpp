#include "stdafx.h"
#include "MineCar.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Sound/Midi.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"
#include "Function.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "ParticleBurst.hpp"
#include "ScreenShake.hpp"
#include "Grid.hpp"

const FP mineCarHeightUnscaled = FP_FromInteger(60);
const FP mineCarWidthUnscaled = FP_FromInteger(12);

MineCar::MineCar(Path_MineCar* pTlv, s32 tlvInfo, s32 /*a4*/, s32 /*a5*/, s32 /*a6*/)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eMineCar);

    const AnimRecord& rec = AnimRec(AnimId::Mine_Car_Open);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_11E_scale = pTlv->field_10_scale;
    field_120_max_damage = pTlv->field_12_max_damage;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    mApplyShadows |= 2u;
    field_1BE_unused = 0;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_26;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
    mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;

    if (field_11E_scale != Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }

    MapFollowMe(TRUE);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    field_118_tlvInfo = tlvInfo;
    field_11C_state = MineCarStates::eParkedWithoutAbe_0;

    LoadAnimation(&field_124_anim);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    mShadow = relive_new Shadow();

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    field_1C0_unused = 0;
    field_1C2_falling_counter = 0;

    // What was pressed before we started to move
    field_1D4_previous_input = static_cast<s16>(sInputKey_ThrowItem_5550F4);

    // Set to a key that keeps the car moving, for example if moving right and we hit a right wall that a car
    // can travel "up" then we set this key to "up" such that holding down "right" automatically moves the car up.
    field_1D6_continue_move_input = static_cast<s16>(sInputKey_ThrowItem_5550F4);

    field_1CC_spawned_path = gMap.mCurrentPath;
    field_1CE_spawned_camera = gMap.mCurrentCamera;
    field_1D0_sound_channels_mask = 0;
    field_1C4_velx_index = 0;
}

const AnimId sMineCarFrameTable[7] = {
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
    auto pTlv = static_cast<Path_MineCar*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_4C_tlvInfo));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kBayrollResID_6013, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("BAYROLL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeCarResId, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABECAR.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("METAL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
    }

    auto pMineCar = relive_new MineCar(pTlv, pState->field_4C_tlvInfo, 0, 0, 0);
    if (pMineCar)
    {
        if (pState->field_5A_bAbeInCar)
        {
            sControlledCharacter_5C1B8C = pMineCar;
        }

        pMineCar->BaseAliveGameObjectPathTLV = nullptr;
        pMineCar->BaseAliveGameObjectCollisionLine = nullptr;

        pMineCar->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_4_xpos;
        pMineCar->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_8_ypos;

        pMineCar->mBaseAnimatedWithPhysicsGameObject_VelX = pState->field_C_velx;
        pMineCar->mBaseAnimatedWithPhysicsGameObject_VelY = pState->field_10_vely;

        pMineCar->mBaseAnimatedWithPhysicsGameObject_PathNumber = pState->field_18_path_number;
        pMineCar->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pState->field_1A_lvl_number);

        pMineCar->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pState->field_14_sprite_scale;

        pMineCar->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pState->field_1C_r, pState->field_1E_g, pState->field_20_b);

        pMineCar->mCurrentMotion = pState->field_28_current_motion;

        s32 remapped1 = 0;
        switch (pState->field_24_frame_table)
        {
        case 10860:
            remapped1 = 6;
            break;
        case 10884:
            remapped1 = 1;
            break;
        case 10896:
            remapped1 = 4;
            break;
        case 10908:
            remapped1 = 0;
            break;
        case 10920:
            remapped1 = 5;
            break;
        case 10944:
            remapped1 = 2;
            break;
        case 10972:
            remapped1 = 3;
            break;
        default:
            break;
        }

        const AnimRecord& animRec = AnimRec(sMineCarFrameTable[remapped1]);
        pMineCar->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        pMineCar->mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame = pState->field_2A_current_anim_frame;


        pMineCar->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pState->field_2C_frame_change_counter;

        pMineCar->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_22_xFlip & 1);
        pMineCar->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2E_render & 1);

        pMineCar->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_drawable & 1);

        if (IsLastFrame(&pMineCar->mBaseAnimatedWithPhysicsGameObject_Anim))
        {
            pMineCar->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        s32 remapped2 = 0;
        switch (pState->field_38_frame_table_offset2)
        {
        case 10860:
            remapped2 = 6;
            break;
        case 10884:
            remapped2 = 1;
            break;
        case 10896:
            remapped2 = 4;
            break;
        case 10908:
            remapped2 = 0;
            break;
        case 10920:
            remapped2 = 5;
            break;
        case 10944:
            remapped2 = 2;
            break;
        case 10972:
            remapped2 = 3;
            break;
        default:
            break;
        }

        const AnimRecord& animRec2 = AnimRec(sMineCarFrameTable[remapped2]);
        pMineCar->field_124_anim.Set_Animation_Data(animRec2.mFrameTableOffset, nullptr);
        pMineCar->field_124_anim.field_92_current_frame = pState->field_2A_current_anim_frame;


        // TODO: OG Bug same flags but save state saves 2 sets one for each anim ??
        pMineCar->field_124_anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_22_xFlip & 1);
        pMineCar->field_124_anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2E_render & 1);


        pMineCar->field_124_anim.mFrameChangeCounter = pState->field_2C_frame_change_counter;

        pMineCar->mHealth = pState->field_3C_health;
        pMineCar->mCurrentMotion = pState->field_40_current_motion;

        pMineCar->mNextMotion = pState->field_42_next_motion;

        pMineCar->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_44_last_line_ypos);

        pMineCar->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);

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

        if (pMineCar->mBaseAnimatedWithPhysicsGameObject_VelX < (ScaleToGridSize(FP_FromInteger(1)) / FP_FromInteger(4)))
        {
            pMineCar->field_1C4_velx_index = 7;
        }
    }

    return sizeof(MineCar_SaveState);
}

void MineCar::LoadAnimation(Animation* pAnim)
{
    const AnimRecord& rec = AnimRec(AnimId::Mine_Car_Tread_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);

    if (pAnim->Init(rec.mFrameTableOffset, gAnimations, this, rec.mMaxW, rec.mMaxH, ppRes))
    {
        pAnim->mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
        pAnim->mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        pAnim->field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
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
    field_124_anim.VCleanUp();
}

Bool32 MineCar::CheckRoofCollision(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;
    return sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos + hitX,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_XPos + hitX,
        mBaseAnimatedWithPhysicsGameObject_YPos + hitY,
        &pPathLine,
        &hitX,
        &hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eCeiling_3) : CollisionMask(eBackgroundCeiling_7)
    );
}

Bool32 MineCar::CheckFloorCollision(FP hitX, FP hitY)
{
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos + hitX,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(4),
            mBaseAnimatedWithPhysicsGameObject_XPos + hitX,
            mBaseAnimatedWithPhysicsGameObject_YPos + hitY,
            &pPathLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor
        )
    )
    {
        return FALSE;
    }

    if (mBaseAnimatedWithPhysicsGameObject_YPos > hitY)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    return TRUE;
}

void MineCar::VRender(PrimHeader** ppOt)
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        s16 r = mBaseAnimatedWithPhysicsGameObject_RGB.g;
        s16 g = mBaseAnimatedWithPhysicsGameObject_RGB.r;
        s16 b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

        const PSX_RECT bRect = VGetBoundingRect();

        if (mApplyShadows & 1)
        {
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                (bRect.h + bRect.y) / 2,
                mBaseAnimatedWithPhysicsGameObject_Scale,
                &r,
                &g,
                &b);
        }

        field_124_anim.mRed = static_cast<u8>(r);
        field_124_anim.mGreen = static_cast<u8>(g);
        field_124_anim.mBlue = static_cast<u8>(b);

        if (gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(30), mBaseAnimatedWithPhysicsGameObject_YPos, 0) || gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)), 0) || gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(30), mBaseAnimatedWithPhysicsGameObject_YPos, 0))
        {
            field_124_anim.VRender(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos()),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos()),
                ppOt,
                0,
                0);
        }

        PSX_RECT frameRect = {};
        field_124_anim.Get_Frame_Rect(&frameRect);
        pScreenManager->InvalidateRectCurrentIdx(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h);
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void MineCar::Stop()
{
    const AnimRecord& animRec = AnimRec(AnimId::Mine_Car_Closed);
    const AnimRecord& animRec2 = AnimRec(AnimId::Mine_Car_Tread_Idle);

    field_11C_state = MineCarStates::eParkedWithAbe_1;

    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }

    SFX_Play_Mono(SoundEffect::MinecarStop_101, 127, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    field_124_anim.Set_Animation_Data(animRec2.mFrameTableOffset, nullptr);

    field_1C4_velx_index = 0;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));
}

void MineCar::Move(u16 frameTabeOffset, FP velX, FP velY, InputCommands::Enum input, MineCarDirs turnDirection, s8 bChangeDirection)
{
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(frameTabeOffset, nullptr);

    field_11C_state = MineCarStates::eMoving_2;
    field_1C8_frame_mod_16 = static_cast<s32>(sGnFrame) % 16;

    if (!field_1D0_sound_channels_mask)
    {
        field_1D0_sound_channels_mask = SFX_Play_Mono(SoundEffect::MinecarMovement_100, 127, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    const AnimRecord& animRec2 = AnimRec(AnimId::Mine_Car_Tread_Move_A);

    field_124_anim.Set_Animation_Data(animRec2.mFrameTableOffset, nullptr);

    mBaseAnimatedWithPhysicsGameObject_VelX = velX;
    mBaseAnimatedWithPhysicsGameObject_VelY = velY;

    if (sInputObject_5BD4E0.isPressed(input))
    {
        field_1D4_previous_input = static_cast<s16>(input);
    }

    field_1BC_turn_direction = turnDirection;

    field_124_anim.mAnimFlags.Set(AnimFlags::eBit19_LoopBackwards, bChangeDirection);
}

s16 MineCar::IsBlocked(MineCarDirs a2, s32 /*a3*/)
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP mineCarHeight = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarWidthUnscaled;
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
    const FP mineCarHeight = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarWidthUnscaled;
    const FP stepSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    const FP halfHeight = mineCarHeight * FP_FromDouble(0.5);
    const FP step = mineCarWidth + stepSize;

    // If we're moving horizontally and hit a wall...
    //
    if (
        (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && WallHit(halfHeight, step + FP_FromInteger(1))) ||
        (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && WallHit(halfHeight, -step))
    )
    {
        // ...continue vertically
        //
        if (field_1BC_turn_direction == MineCarDirs::eUp_3)
        {
            field_1D6_continue_move_input = (u16) sInputKey_Up_5550D8;
        }
        else
        {
            field_1D6_continue_move_input = (u16) sInputKey_Down_5550DC;
        }

        return TRUE;
    }

    // If we're moving vertically and hit and floor or roof...
    //
    if (
        (
            CheckFloorCollision(FP_FromInteger(0), FP_FromInteger(1)) &&
            mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0)
        ) ||
        (
            CheckRoofCollision(FP_FromInteger(0), -mineCarHeight) &&
            mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0)
        )
    )
    {
        // ...continue horizontally
        //
        if (field_1BC_turn_direction == MineCarDirs::eLeft_2)
        {
            field_1D6_continue_move_input = (u16) sInputKey_Left_5550D4;
        }
        else
        {
            field_1D6_continue_move_input = (u16) sInputKey_Right_5550D0;
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
                (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && !bRoofRight) ||
                (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && !bRoofLeft)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Up_5550D8;
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
                (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0) && !bLeftWall1) ||
                (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0) && !bLeftWall2)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Left_5550D4;
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
                (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0) && !bRightWall1) ||
                (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0) && !bRightWall2)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Right_5550D0;
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
                (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && !bFloorRight) ||
                (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && !bFloorLeft)
            )
            {
                field_1D6_continue_move_input = (u16) sInputKey_Down_5550DC;
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

                if (pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale ||
                   (pAliveObj->Type() == ReliveTypes::eSlog && mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5)))
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

    pState->field_4_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_8_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    pState->field_C_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_10_vely = mBaseAnimatedWithPhysicsGameObject_VelY;

    pState->field_18_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_1A_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);

    pState->field_14_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    pState->field_1C_r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pState->field_1E_g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pState->field_20_b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_current_anim_frame = mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame;
    pState->field_2C_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;

    pState->field_2F_drawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_22_xFlip = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_2E_render = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);


    switch (mBaseAnimatedWithPhysicsGameObject_Anim.field_18_frame_table_offset)
    {
        case 20788: // Mine_Car_Tread_Move_B
            pState->field_24_frame_table = 10860;
            break;
        case 20812: // Mine_Car_Open
            pState->field_24_frame_table = 10884;
            break;
        case 20824: // Mine_Car_Tread_Idle
            pState->field_24_frame_table = 10896;
            break;
        case 20836: // Mine_Car_Closed
            pState->field_24_frame_table = 10908;
            break;
        case 20848: // Mine_Car_Tread_Move_A
            pState->field_24_frame_table = 10920;
            break;
        case 20872: // Mine_Car_Shake_A
            pState->field_24_frame_table = 10944;
            break;
        case 20900: // Mine_Car_Shake_B
            pState->field_24_frame_table = 10972;
            break;
        default:
            break;
    }

    pState->field_34_unused = field_124_anim.field_92_current_frame;
    pState->field_36_unused = field_124_anim.mFrameChangeCounter;

    pState->field_32_unused = field_124_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_30_unused = field_124_anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
    
    switch (field_124_anim.field_18_frame_table_offset)
    {
        case 20788: // Mine_Car_Tread_Move_B
            pState->field_38_frame_table_offset2 = 10860;
            break;
        case 20812: // Mine_Car_Open
            pState->field_38_frame_table_offset2 = 10884;
            break;
        case 20824: // Mine_Car_Tread_Idle
            pState->field_38_frame_table_offset2 = 10896;
            break;
        case 20836: // Mine_Car_Closed
            pState->field_38_frame_table_offset2 = 10908;
            break;
        case 20848: // Mine_Car_Tread_Move_A
            pState->field_38_frame_table_offset2 = 10920;
            break;
        case 20872: // Mine_Car_Shake_A
            pState->field_38_frame_table_offset2 = 10944;
            break;
        case 20900: // Mine_Car_Shake_B
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
        pState->field_46_collision_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }
    else
    {
        pState->field_46_collision_line_type = -1;
    }

    pState->field_5A_bAbeInCar = (this == sControlledCharacter_5C1B8C);
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
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

        if (BaseAliveGameObjectCollisionLineType != -1)
        {
            sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
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

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

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

    if (sInputObject_5BD4E0.isPressed(sInputKey_DoAction_5550E4))
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

    const FP mineCarWidth = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (
        !sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos - mineCarWidthAdjusted,
            mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            mBaseAnimatedWithPhysicsGameObject_XPos + mineCarWidthAdjusted,
            mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eMineCarWall_12) : CollisionMask(eBackgroundMineCarWall_15)
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
        sActiveHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale
    )
    {
        const AnimRecord& animRec = AnimRec(AnimId::Mine_Car_Closed);

        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_11C_state = MineCarStates::eParkedWithAbe_1;
        sControlledCharacter_5C1B8C = this;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_35;
        field_124_anim.mRenderLayer = Layer::eLayer_BombMineCar_35;

        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombMineCar_Half_16;
            field_124_anim.mRenderLayer = Layer::eLayer_BombMineCar_Half_16;
        }

        SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, 500, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
}

void MineCar::State_1_ParkedWithAbe()
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP mineCarHeight = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    VCheckCollisionLineStillValid(10);

    // Abe exits minecar
    //
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_117_InMineCar_4587C0)
    {
        const AnimRecord& animRec = AnimRec(AnimId::Mine_Car_Open);
        const AnimRecord& animRec2 = AnimRec(AnimId::Mine_Car_Tread_Idle);

        sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos;
        sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        field_11C_state = MineCarStates::eParkedWithoutAbe_0;
        
        field_124_anim.Set_Animation_Data(animRec2.mFrameTableOffset, nullptr);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

        sControlledCharacter_5C1B8C = sActiveHero;
        field_1CC_spawned_path = gMap.mCurrentPath;
        field_1CE_spawned_camera = gMap.mCurrentCamera;

        sActiveHero->VCheckCollisionLineStillValid(10);

        SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, 500, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
            field_124_anim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_26;
            field_124_anim.mRenderLayer = Layer::eLayer_Shadow_26;
        }

        return;
    }

    // Peform any movements
    //
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    // Horizontal movements
    //
    const FP rayCastY1 = mBaseAnimatedWithPhysicsGameObject_YPos - mineCarHeight;
    const FP rayCastY2 = mBaseAnimatedWithPhysicsGameObject_YPos;
    const FP velX = kGridSize / FP_FromInteger(4);
    const FP velY = FP_FromInteger(0);

    InputCommands::Enum inputKey = sInputKey_Right_5550D0;

    if (
        (
            sInputObject_5BD4E0.isPressed(inputKey) ||
            (
                sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) sInputKey_Left_5550D4 &&
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
        const FP rayCastX = mBaseAnimatedWithPhysicsGameObject_XPos + velX;
        
        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(2);
        const FP hitX2 = FP_FromInteger(4) - mineCarWidthAdjusted;
        const u16 frameTableOffset = 20872u;

        if (
            HandleState1Move(
                &MineCar::CheckFloorCollision,
                hitX,
                FP_FromInteger(4),
                hitX2,
                frameTableOffset,
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
                frameTableOffset,
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
            if (sInputObject_5BD4E0.isPressed(inputKey))
            {
                if (field_1BC_turn_direction != MineCarDirs::eLeft_2 && !(static_cast<s32>(sGnFrame) % 6))
                {
                    SFX_Play_Mono(SoundEffect::MinecarStuck_102, 127, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
            }
        }
    }

    inputKey = sInputKey_Left_5550D4;

    if (
        (
            sInputObject_5BD4E0.isPressed(inputKey) ||
            (
                sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
                (u16) field_1D6_continue_move_input == inputKey &&
                field_1D4_previous_input != (u16) sInputKey_Right_5550D0 &&
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
        const FP rayCastX = mBaseAnimatedWithPhysicsGameObject_XPos - velX;
        
        const FP hitX = mineCarWidthAdjusted - FP_FromInteger(4);
        const FP hitX2 = -(mineCarWidthAdjusted + FP_FromInteger(2));
        const u16 frameTableOffset = 20900u;

        if (
            HandleState1Move(
                &MineCar::CheckFloorCollision,
                hitX,
                FP_FromInteger(4),
                hitX2,
                frameTableOffset,
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
                frameTableOffset,
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
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eRight_1 && !(static_cast<s32>(sGnFrame) % 6))
            {
                SFX_Play_Mono(SoundEffect::MinecarStuck_102, 127, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
        }
    }

    HandleUpDown();
}

bool MineCar::HandleState1Move(const mineCarFPFunc func, const FP mineCarFPFuncArg1, const FP mineCarFPFuncArg2, const FP mineCarFPFuncArg3,
                               u16 frameTableOffset, MineCarDirs mineCarDir, const s8 bChangeDir, FP rayCastX1, FP rayCastY1, FP rayCastX2, FP rayCastY2, const CollisionMask ModelMask1, const CollisionMask ModelMask2,
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
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? ModelMask1 : ModelMask2
        )
    )
    {
        if ((this->*func)(mineCarFPFuncArg1, mineCarFPFuncArg2) || (this->*func)(mineCarFPFuncArg3, mineCarFPFuncArg2))
        {
            if (
                !isVertical ||
                (
                    (verticalFlipXCond  && hitX > mBaseAnimatedWithPhysicsGameObject_XPos) ||
                    (!verticalFlipXCond && hitX < mBaseAnimatedWithPhysicsGameObject_XPos)
                )
            )
            {
                Move(frameTableOffset, velX, velY, key, mineCarDir, bChangeDir);
                return true;
            }
        }
    }

    return false;
}

void MineCar::HandleUpDown()
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP mineCarWidth = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;
    const FP mineCarHeight = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarHeightUnscaled;
    const FP k5Scaled = FP_FromInteger(5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    const FP rayCastX1 = mBaseAnimatedWithPhysicsGameObject_XPos - mineCarWidthAdjusted;
    const FP rayCastX2 = mBaseAnimatedWithPhysicsGameObject_XPos + mineCarWidthAdjusted;
    
    const FP velX = FP_FromInteger(0);
    const FP velY = k5Scaled;
    const u16 frameTableOffset = 20836u;

    InputCommands::Enum inputKey = sInputKey_Up_5550D8;

    if (
        sInputObject_5BD4E0.isPressed(inputKey) ||
        (
            sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) sInputKey_Down_5550DC &&
            field_1BC_turn_direction != MineCarDirs::eLeft_2 &&
            field_1BC_turn_direction != MineCarDirs::eRight_1 &&
            !IsBlocked(MineCarDirs::eDown_0, 0)
        )
    )
    {
        const FP rayCastY = mBaseAnimatedWithPhysicsGameObject_YPos - velY - (mineCarWidthAdjusted * FP_FromDouble(0.5));

        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitY = FP_FromInteger(1);
        const FP hitY2 = mineCarHeight + FP_FromInteger(1);

        if (
            HandleState1Move(
                &MineCar::WallHit,
                hitY,
                hitX,
                hitY2,
                frameTableOffset,
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
                frameTableOffset,
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
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eDown_0)
            {
                if (!(static_cast<s32>(sGnFrame) % 6))
                {
                    SFX_Play_Mono(SoundEffect::MinecarStuck_102, 127, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
            }
        }
    }

    inputKey = sInputKey_Down_5550DC;

    if (
        sInputObject_5BD4E0.isPressed(inputKey) ||
        (
            sInputObject_5BD4E0.isPressed(field_1D4_previous_input) &&
            (u16) field_1D6_continue_move_input == inputKey &&
            field_1D4_previous_input != (u16) sInputKey_Up_5550D8 &&
            field_1BC_turn_direction != MineCarDirs::eLeft_2 &&
            field_1BC_turn_direction != MineCarDirs::eRight_1 &&
            !IsBlocked(MineCarDirs::eUp_3, 0)
        )
    )
    {
        const FP rayCastY = mBaseAnimatedWithPhysicsGameObject_YPos + velY;

        const FP hitX = mineCarWidthAdjusted + FP_FromInteger(4);
        const FP hitY = -FP_FromInteger(2);
        const FP hitY2 = mineCarHeight - FP_FromInteger(1);

        if (
            HandleState1Move(
                &MineCar::WallHit,
                hitY,
                hitX,
                hitY2,
                frameTableOffset,
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
                frameTableOffset,
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
        if (sInputObject_5BD4E0.isPressed(inputKey))
        {
            if (field_1BC_turn_direction != MineCarDirs::eUp_3 && !(static_cast<s32>(sGnFrame) % 6))
            {
                SFX_Play_Mono(SoundEffect::MinecarStuck_102, 127, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP mineCarHeight = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (!field_1D0_sound_channels_mask)
    {
        // Play the mine car moving sound
        //
        field_1D0_sound_channels_mask = SFX_Play_Mono(SoundEffect::MinecarMovement_100, 127, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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
            mBaseAnimatedWithPhysicsGameObject_XPos - mineCarWidthAdjusted,
            mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            mBaseAnimatedWithPhysicsGameObject_XPos + mineCarWidthAdjusted,
            mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY - (mineCarWidthAdjusted * FP_FromDouble(0.5)),
            &pPathLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eMineCarWall_12) : CollisionMask(eBackgroundMineCarWall_15)
        ) &&
        mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0)
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
            mBaseAnimatedWithPhysicsGameObject_YPos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eMineCarCeiling_13) : CollisionMask(eBackgroundMineCarCeiling_16)
        ) &&
        field_1BC_turn_direction == MineCarDirs::eDown_0
    )
    {
        field_11C_state = MineCarStates::eFalling_3;
        return;
    }

    if (
        !sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
            mBaseAnimatedWithPhysicsGameObject_YPos - mineCarHeight,
            &pPathLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eMineCarFloor_11) : CollisionMask(eBackgroundMineCarFloor_14)
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

        if (mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = -velXTable_5461D8[field_1C4_velx_index];
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = velXTable_5461D8[field_1C4_velx_index];
        }

        if (++field_1C4_velx_index == 2)
        {
            ++field_124_anim.field_10_frame_delay;
        }
    }

    if (sInputObject_5BD4E0.isPressed(field_1D4_previous_input))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

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
            mBaseAnimatedWithPhysicsGameObject_XPos == FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos))) &&
            !field_1C4_velx_index
        )
        {
            if (mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -velXTable_5461D8[0];
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = velXTable_5461D8[0];
            }

            ++field_1C4_velx_index;
        }

        mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

        SetActiveCameraDelayedFromDir();
        RunThingsOver();

        return;
    }

    Stop();
}

void MineCar::State_3_Falling()
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP mineCarHeight = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarHeightUnscaled;
    const FP mineCarWidth = mBaseAnimatedWithPhysicsGameObject_SpriteScale * mineCarWidthUnscaled;
    const FP mineCarWidthAdjusted = mineCarWidth + kGridSize;

    SetActiveCameraDelayedFromDir();
    field_1C2_falling_counter++;

    if (field_1D0_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_1D0_sound_channels_mask);
        field_1D0_sound_channels_mask = 0;
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        if (
            WallHit(mineCarHeight, mineCarWidthAdjusted) ||
            WallHit(FP_FromInteger(0), mineCarWidthAdjusted)
        )
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);

            relive_new ParticleBurst(
                sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos + mineCarHeight + kGridSize,
                sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos - ((mineCarHeight + kGridSize) * FP_FromDouble(0.5)),
                4u,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                BurstType::eBigRedSparks_3,
                9
            );

            SFX_Play_Mono(SoundEffect::FallingItemHit_47, 80, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        if (
            WallHit(mineCarHeight, -mineCarWidthAdjusted) ||
            WallHit(FP_FromInteger(0), -mineCarWidthAdjusted)
        )
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);

            relive_new ParticleBurst(
                sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos - (mineCarHeight + kGridSize),
                sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos - ((mineCarHeight + kGridSize) * FP_FromDouble(0.5)),
                4u,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                BurstType::eBigRedSparks_3,
                9
            );

            SFX_Play_Mono(SoundEffect::FallingItemHit_47, 80, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pPathLine = nullptr;

    if (InAirCollision(&pPathLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
        BaseAliveGameObjectLastLineYPos = hitY;
        mBaseAnimatedWithPhysicsGameObject_VelY = (-mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(0.2));

        relive_new ParticleBurst(
            sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos,
            sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos,
            5u,
            FP_FromInteger(1),
            BurstType::eBigRedSparks_3,
            9
        );

        if (field_1C2_falling_counter > 4)
        {
            SFX_Play_Pitch(SoundEffect::MinecarStop_101, 127, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            SFX_Play_Pitch(SoundEffect::FallingItemHit_47, 127, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            relive_new ScreenShake(FALSE, FALSE);
        }

        field_1C2_falling_counter = 0;

        if (mBaseAnimatedWithPhysicsGameObject_VelY > -FP_FromInteger(1))
        {
            SFX_Play_Pitch(SoundEffect::MinecarStop_101, 120, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            SFX_Play_Pitch(SoundEffect::FallingItemHit_47, 70, -800, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            BaseAliveGameObjectCollisionLine = pPathLine;
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;

            if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));
            }

            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

            Stop();

            sActiveHero->BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine;
            sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos;
            sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            field_1BC_turn_direction = MineCarDirs::eUp_3;
        }
    }

    RunThingsOver();

    return;
}
