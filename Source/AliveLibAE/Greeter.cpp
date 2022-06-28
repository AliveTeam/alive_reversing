#include "stdafx.h"
#include "Greeter.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "ObjectIds.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Electrocute.hpp"
#include "ZapLine.hpp"
#include "Events.hpp"
#include "MotionDetector.hpp"
#include "Function.hpp"
#include "Bullet.hpp"

Greeter::Greeter(Path_Greeter* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eGreeter);
    const AnimRecord& rec = AnimRec(AnimId::Greeter_Moving);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }


    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    if (pTlv->field_14_start_direction == XDirection_short::eLeft_0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }

    field_134_speed = FP_FromInteger(pTlv->field_12_motion_detector_speed);
    field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
    field_12E_bDontSetDestroyed = 1;
    field_118_tlvInfo = tlvInfo;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

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
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    auto pMotionDetctor = relive_new MotionDetector(nullptr, 0, this);
    if (pMotionDetctor)
    {
        field_11C_motionDetectorId = pMotionDetctor->field_8_object_id;
    }

    field_140_targetOnRight = 0;
    field_13E_targetOnLeft = 0;

    field_128_timer = sGnFrame + Math_RandomRange(70, 210);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    field_12C_timesShot = 0;

    mShadow = relive_new Shadow();

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_130_bChasing = 0;
}

s32 Greeter::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Greeter_State*>(pBuffer);
    auto pTlv = static_cast<Path_Greeter*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_28_tlvInfo));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMflareResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MFLARE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMotionResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MOTION.BAN", nullptr);
    }
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGreeterResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("GREETER.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SPLINE.BAN", nullptr);
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

    auto pGreeter = relive_new Greeter(pTlv, pState->field_28_tlvInfo);
    if (pGreeter)
    {
        pGreeter->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_C_xpos;
        pGreeter->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_10_ypos;
        pGreeter->mBaseAnimatedWithPhysicsGameObject_VelX = pState->field_14_velx;
        pGreeter->mBaseAnimatedWithPhysicsGameObject_VelY = pState->field_18_vely;

        pGreeter->mBaseAnimatedWithPhysicsGameObject_PathNumber = pState->field_8_path_number;
        pGreeter->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pState->field_A_lvl_number);
        pGreeter->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pState->field_1C_sprite_scale;

        pGreeter->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pState->field_2_r, pState->field_4_g, pState->field_6_b);

        pGreeter->mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame = pState->field_20_current_frame;
        pGreeter->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pState->field_22_frame_change_counter;

        pGreeter->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);

        pGreeter->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

        if (IsLastFrame(&pGreeter->mBaseAnimatedWithPhysicsGameObject_Anim))
        {
            pGreeter->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pGreeter->field_118_tlvInfo = pState->field_28_tlvInfo;
        pGreeter->field_120_unused = pState->field_2C_unused;
        pGreeter->field_124_last_turn_time = pState->field_30_last_turn_time;
        pGreeter->field_128_timer = pState->field_34_timer;
        pGreeter->field_12C_timesShot = pState->field_38_timesShot;
        pGreeter->field_12E_bDontSetDestroyed = pState->field_3A_bDontSetDestroyed;
        pGreeter->field_130_bChasing = pState->field_3C_bChasing;
        pGreeter->field_134_speed = pState->field_40_speed;
        pGreeter->field_13C_brain_state = pState->field_44_brain_state;
        pGreeter->field_13E_targetOnLeft = pState->field_46_targetOnLeft;
        pGreeter->field_140_targetOnRight = pState->field_48_targetOnRight;

        auto pDetector = static_cast<MotionDetector*>(sObjectIds.Find_Impl(pGreeter->field_11C_motionDetectorId));

        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(pDetector->field_F8_laser_id));
        pLaser->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_4C_motion_laser_xpos;
    }

    return sizeof(Greeter_State);
}

s32 Greeter::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Greeter_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eGreeter_64;

    pState->field_C_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_10_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_14_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_18_vely = mBaseAnimatedWithPhysicsGameObject_VelY;

    pState->field_8_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_A_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pState->field_1C_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    pState->field_2_r = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pState->field_4_g = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pState->field_6_b = mBaseAnimatedWithPhysicsGameObject_RGB.b;

    pState->field_20_current_frame = mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame;
    pState->field_22_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;
    pState->field_25_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->field_28_tlvInfo = field_118_tlvInfo;
    pState->field_2C_unused = field_120_unused;
    pState->field_30_last_turn_time = field_124_last_turn_time;
    pState->field_34_timer = field_128_timer;

    pState->field_38_timesShot = field_12C_timesShot;
    pState->field_3A_bDontSetDestroyed = field_12E_bDontSetDestroyed;
    pState->field_3C_bChasing = field_130_bChasing;

    pState->field_40_speed = field_134_speed;
    pState->field_44_brain_state = field_13C_brain_state;
    pState->field_46_targetOnLeft = field_13E_targetOnLeft;
    pState->field_48_targetOnRight = field_140_targetOnRight;

    auto pMotionDetector = static_cast<MotionDetector*>(sObjectIds.Find_Impl(field_11C_motionDetectorId));
    auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(pMotionDetector->field_F8_laser_id));
    pState->field_4C_motion_laser_xpos = pLaser->mBaseAnimatedWithPhysicsGameObject_XPos;

    return sizeof(Greeter_State);
}

void Greeter::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (sControlledCharacter_5C1B8C)
    {
        const FP xDistFromPlayer = FP_Abs(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos);
        if (xDistFromPlayer > FP_FromInteger(356))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;
        }

        const FP yDistFromPlayer = FP_Abs(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos);
        if (yDistFromPlayer > FP_FromInteger(240))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;
        }
    }
}

Greeter::~Greeter()
{
    if (field_12E_bDontSetDestroyed)
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 1);
    }

    BaseGameObject* pMotionDetector = sObjectIds.Find_Impl(field_11C_motionDetectorId);
    if (pMotionDetector)
    {
        pMotionDetector->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Greeter::BlowUp()
{
    mHealth = FP_FromInteger(0);

    relive_new Explosion(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5)),
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        0);

    relive_new Gibs(
        GibType::Metal_5,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(50),
        FP_FromInteger(0),
        FP_FromInteger(0),
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        0);

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    field_12E_bDontSetDestroyed = 0;
}

void Greeter::ChangeDirection()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_1_PatrolTurn;
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Turn);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    field_124_last_turn_time = sGnFrame;
}

void Greeter::BounceBackFromShot()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_5_Knockback;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(-2);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(2);
    }

    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;

    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Hit);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

    const CameraPos soundDirection = gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    SFX_Play_Camera(SoundEffect::GreeterKnockback_121, 0, soundDirection, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
}

void Greeter::HandleRollingAlong()
{
    for (Path_TLV* pTlv = field_138_pTlv; pTlv;
         pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv,
                                                      mBaseAnimatedWithPhysicsGameObject_VelX + mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
                                                      mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY,
                                                      mBaseAnimatedWithPhysicsGameObject_VelX + mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
                                                      mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos + mBaseAnimatedWithPhysicsGameObject_VelY))
    {
        switch (pTlv->field_4_type.mType)
        {
            case TlvTypes::DeathDrop_4:
                BlowUp();
                break;

            case TlvTypes::ScrabLeftBound_43:
                if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection();
                }
                break;

            case TlvTypes::ScrabRightBound_44:
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection();
                }
                break;

            case TlvTypes::EnemyStopper_47:
                if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
                {
                    ChangeDirection();
                }
                break;

            default:
                continue;
        }
    }

    if (field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
    {
        if ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine(0, 1)) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(40), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(3)) || (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) && Check_IsOnEndOfLine(1, 1)))
        {
            ChangeDirection();
        }
    }

    if (field_13C_brain_state == GreeterBrainStates::eBrain_4_Chase)
    {
        if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(40), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(3))) // TODO: OG bug, raw * 3 here ??
        {
            BounceBackFromShot();
        }
    }
}

s16 Greeter::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead) || FP_GetExponent(mHealth) == 0)
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        if (static_cast<Bullet*>(pFrom)->field_30_x_distance <= FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
        }

        if (++field_12C_timesShot <= 10)
        {
            BounceBackFromShot();
        }
        else
        {
            BlowUp();
        }
        return 1;

        case ReliveTypes::eDrill:
        case ReliveTypes::eElectricWall:
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eExplosion:
            BlowUp();
            return 1;

        case ReliveTypes::eSlamDoor:
            BounceBackFromShot();
            return 1;

        case ReliveTypes::eElectrocute:
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            BlowUp();
            return 1;

        default:
            BlowUp();
            return 1;
    }
}

void Greeter::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    BounceBackFromShot();
}

void Greeter::ZapTarget(FP xpos, FP ypos, BaseAliveGameObject* pTarget)
{
    relive_new ScreenShake(0, 0);

    relive_new ZapLine(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(20) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    relive_new ZapLine(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    relive_new ZapLine(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(50) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    relive_new ParticleBurst(
        xpos,
        ypos,
        10,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        BurstType::eBigRedSparks_3,
        11);

    relive_new ParticleBurst(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
        10,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
        BurstType::eBigRedSparks_3,
        11);

    pTarget->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit7_Electrocuted);

    relive_new Electrocute(pTarget, TRUE, TRUE);

    pTarget->VTakeDamage(this);

    const CameraPos soundDirection = gMap.GetDirection(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);

    SFX_Play_Camera(SoundEffect::Zap1_49, 0, soundDirection, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    SFX_Play_Camera(SoundEffect::Zap2_50, 0, soundDirection, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    RandomishSpeak(GreeterSpeak::eLaugh_3);

    field_128_timer = sGnFrame + Math_RandomRange(160, 200);
    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;
}

void Greeter::RandomishSpeak(GreeterSpeak effect)
{
    field_13C_brain_state = GreeterBrainStates::eBrain_2_Speak;
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Speak);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    field_120_unused = sGnFrame + 25;

    if (effect == GreeterSpeak::eRandomized_1000)
    {
        const s32 randomSpeak = static_cast<s32>(sGnFrame % 4);
        // Will be one of: Hi_0, HereBoy_1,  GetHim_2 or Laugh_3
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(randomSpeak), 100, 700, this);
    }
    else
    {
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(effect), 100, 700, this);
    }
}

Bool32 Greeter::ZapIsNotBlocked(BaseAliveGameObject* pUs, BaseAliveGameObject* pThem)
{
    const PSX_RECT usRect = VGetBoundingRect();
    const PSX_RECT bRectThem = pThem->VGetBoundingRect();

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    return sCollisions->Raycast(
               pUs->mBaseAnimatedWithPhysicsGameObject_XPos,
               FP_FromInteger(usRect.h + 0xFFE7),
               pThem->mBaseAnimatedWithPhysicsGameObject_XPos,
               FP_FromInteger(bRectThem.h + 0xFFE7),
               &pLine,
               &hitX,
               &hitY,
               pUs->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls)
        == 1;
}

BaseAliveGameObject* Greeter::GetMudToZap()
{
    for (s32 idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMudokon)
        {
            const PSX_RECT bRect = pObj->VGetBoundingRect();

            const FP xMid = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP yMid = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (xMid - mBaseAnimatedWithPhysicsGameObject_XPos < (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) && mBaseAnimatedWithPhysicsGameObject_XPos - xMid < (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) && yMid - (mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(4)) < (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) && mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(4) - yMid < (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) && !(sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted)) && !ZapIsNotBlocked(this, pObj))
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

void Greeter::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_13C_brain_state)
    {
        case GreeterBrainStates::eBrain_0_Patrol:
            if (!((sGnFrame - field_124_last_turn_time) % 14))
            {
                const CameraPos soundDirection = gMap.GetDirection(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos);
                SFX_Play_Camera(SoundEffect::WheelSqueak_31, 10, soundDirection, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            if ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) == 0)
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(3));
                if (field_13E_targetOnLeft)
                {
                    RandomishSpeak(GreeterSpeak::eHi_0);
                    field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                }
                else if (field_140_targetOnRight)
                {
                    ChangeDirection();
                    field_13C_brain_state = GreeterBrainStates::eBrain_6_ToChase;
                }
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(3));
                if (field_140_targetOnRight)
                {
                    RandomishSpeak(GreeterSpeak::eHi_0);
                    field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                }
                else if (field_13E_targetOnLeft)
                {
                    ChangeDirection();
                    field_13C_brain_state = GreeterBrainStates::eBrain_6_ToChase;
                }
            }

            if (static_cast<s32>(sGnFrame) > field_128_timer)
            {
                RandomishSpeak(GreeterSpeak::eRandomized_1000);
            }
            break;

        case GreeterBrainStates::eBrain_1_PatrolTurn:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_13E_targetOnLeft = 0;
                field_140_targetOnRight = 0;
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                }
            }
            break;

        case GreeterBrainStates::eBrain_2_Speak:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 0;
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_128_timer = sGnFrame + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_3_ChaseSpeak:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 1;
                field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Chase);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            }
            break;

        case GreeterBrainStates::eBrain_4_Chase:
        {
            if (!(sGnFrame % 8))
            {
                const CameraPos soundDirection2 = gMap.GetDirection(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos);
                SFX_Play_Camera(SoundEffect::WheelSqueak_31, 10, soundDirection2, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            mBaseAnimatedWithPhysicsGameObject_VelX = -(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5));
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5);
            }

            const PSX_RECT bRect = sActiveHero->VGetBoundingRect();

            const FP midX = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP midY = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (midX - mBaseAnimatedWithPhysicsGameObject_XPos >= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) || mBaseAnimatedWithPhysicsGameObject_XPos - midX >= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) || midY - (mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(4)) >= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) || mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(4) - midY >= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)) || sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted) || sActiveHero->CantBeDamaged_44BAB0() || ZapIsNotBlocked(this, sActiveHero))
            {
                BaseAliveGameObject* pGonnaZapYa = GetMudToZap();
                if (pGonnaZapYa)
                {
                    const PSX_RECT bZapRect = pGonnaZapYa->VGetBoundingRect();

                    ZapTarget(
                        FP_FromInteger((bZapRect.x + bZapRect.w) / 2),
                        FP_FromInteger((bZapRect.y + bZapRect.h) / 2),
                        pGonnaZapYa);
                }
            }
            else
            {
                ZapTarget(midX, midY, sActiveHero);
            }
        }
        break;

        case GreeterBrainStates::eBrain_5_Knockback:
            if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(40), FP_FromRaw(3 * mBaseAnimatedWithPhysicsGameObject_VelX.fpValue))) // TODO: OG bug, why * 3 and not * FP 3??
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak(GreeterSpeak::eWhat_9);
                field_128_timer = sGnFrame + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_6_ToChase:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak(GreeterSpeak::eHi_0);
                field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                }
            }
            break;

        case GreeterBrainStates::eBrain_7_Fall:
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = -mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(0.4);
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                BaseAliveGameObjectLastLineYPos = hitY;

                const CameraPos soundDirection3 = gMap.GetDirection(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    hitY);

                SFX_Play_Camera(SoundEffect::GreeterLand_120, 0, soundDirection3, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                if (mBaseAnimatedWithPhysicsGameObject_VelY > -FP_FromInteger(1))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                    if (!field_130_bChasing)
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                        const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    }
                    else
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                        const AnimRecord& animRec = AnimRec(AnimId::Greeter_Chase);
                        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    }
                }
            }
        }
        break;

        default:
            break;
    }

    if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX) || FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelY))
    {
        if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
        {
            const FP xpos = mBaseAnimatedWithPhysicsGameObject_VelX
                          + mBaseAnimatedWithPhysicsGameObject_VelX
                          + mBaseAnimatedWithPhysicsGameObject_VelX
                          + mBaseAnimatedWithPhysicsGameObject_VelX
                          + mBaseAnimatedWithPhysicsGameObject_XPos;

            const FP ypos = mBaseAnimatedWithPhysicsGameObject_VelY
                          + mBaseAnimatedWithPhysicsGameObject_YPos
                          + mBaseAnimatedWithPhysicsGameObject_VelY;

            field_138_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(nullptr, xpos, ypos, xpos, ypos);
            HandleRollingAlong();
        }
    }

    bool collisionCheck = true;
    if (field_13C_brain_state == GreeterBrainStates::eBrain_7_Fall)
    {
        field_138_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
        HandleRollingAlong();
        if (field_13C_brain_state == GreeterBrainStates::eBrain_7_Fall)
        {
            collisionCheck = false;
        }
    }

    if (collisionCheck)
    {
        if (Check_IsOnEndOfLine(0, 0))
        {
            field_13C_brain_state = GreeterBrainStates::eBrain_7_Fall;
            const AnimRecord& animRec = AnimRec(AnimId::Greeter_Falling);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }
    }

    if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
    }
}
