#include "stdafx.h"
#include "Greeter.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Electrocute.hpp"
#include "ZapLine.hpp"
#include "../relive_lib/Events.hpp"
#include "MotionDetector.hpp"
#include "Function.hpp"
#include "Bullet.hpp"

Greeter::Greeter(relive::Path_Greeter* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eGreeter);
    const AnimRecord& rec = AnimRec(AnimId::Greeter_Moving);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Greeter_Moving, ppRes);

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBat_Half_14;
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBats_33;
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }


    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    if (pTlv->mFacing == relive::reliveXDirection::eLeft)
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
    }

    field_134_speed = FP_FromInteger(pTlv->mMotionDetectorSpeed);
    field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
    field_12E_bDontSetDestroyed = 1;
    field_118_tlvInfo = tlvInfo;

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

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
            mScale == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
    }

    auto pMotionDetctor = relive_new MotionDetector(nullptr, 0, this);
    if (pMotionDetctor)
    {
        field_11C_motionDetectorId = pMotionDetctor->mBaseGameObjectId;
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
    auto pTlv = static_cast<relive::Path_Greeter*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_28_tlvInfo));

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
        pGreeter->mXPos = pState->field_C_xpos;
        pGreeter->mYPos = pState->field_10_ypos;
        pGreeter->mVelX = pState->field_14_velx;
        pGreeter->mVelY = pState->field_18_vely;

        pGreeter->mCurrentPath = pState->field_8_path_number;
        pGreeter->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_A_lvl_number);
        pGreeter->mSpriteScale = pState->field_1C_sprite_scale;

        pGreeter->mRGB.SetRGB(pState->field_2_r, pState->field_4_g, pState->field_6_b);

        pGreeter->mAnim.mCurrentFrame = pState->field_20_current_frame;
        pGreeter->mAnim.mFrameChangeCounter = pState->field_22_frame_change_counter;

        pGreeter->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);

        pGreeter->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

        if (IsLastFrame(&pGreeter->mAnim))
        {
            pGreeter->mAnim.mFlags.Set(AnimFlags::eBit18_IsLastFrame);
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
        pLaser->mXPos = pState->field_4C_motion_laser_xpos;
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

    pState->field_C_xpos = mXPos;
    pState->field_10_ypos = mYPos;
    pState->field_14_velx = mVelX;
    pState->field_18_vely = mVelY;

    pState->field_8_path_number = mCurrentPath;
    pState->field_A_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_1C_sprite_scale = mSpriteScale;

    pState->field_2_r = mRGB.r;
    pState->field_4_g = mRGB.g;
    pState->field_6_b = mRGB.b;

    pState->field_20_current_frame = mAnim.mCurrentFrame;
    pState->field_22_frame_change_counter = mAnim.mFrameChangeCounter;
    pState->field_25_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render);
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
    pState->field_4C_motion_laser_xpos = pLaser->mXPos;

    return sizeof(Greeter_State);
}

void Greeter::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (sControlledCharacter)
    {
        const FP xDistFromPlayer = FP_Abs(sControlledCharacter->mXPos - mXPos);
        if (xDistFromPlayer > FP_FromInteger(356))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;
        }

        const FP yDistFromPlayer = FP_Abs(sControlledCharacter->mYPos - mYPos);
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
        mXPos,
        mYPos - (mSpriteScale * FP_FromInteger(5)),
        mSpriteScale,
        0);

    relive_new Gibs(
        GibType::Metal_5,
        mXPos,
        mYPos + FP_FromInteger(50),
        FP_FromInteger(0),
        FP_FromInteger(0),
        mSpriteScale,
        0);

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    field_12E_bDontSetDestroyed = 0;
}

void Greeter::ChangeDirection()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_1_PatrolTurn;
    mVelX = FP_FromInteger(0);
    mAnim.Set_Animation_Data(AnimId::Greeter_Turn, nullptr);
    field_124_last_turn_time = sGnFrame;
}

void Greeter::BounceBackFromShot()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_5_Knockback;

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = FP_FromInteger(-2);
    }
    else
    {
        mVelX = FP_FromInteger(2);
    }

    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;

    mAnim.Set_Animation_Data(AnimId::Greeter_Hit, nullptr);

    const CameraPos soundDirection = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);
    SFX_Play_Camera(SoundEffect::GreeterKnockback_121, 0, soundDirection, mSpriteScale);
}

void Greeter::HandleRollingAlong()
{
    for (relive::Path_TLV* pTlv = field_138_pTlv; pTlv;
         pTlv = sPathInfo->TlvGetAt(pTlv,
                                                      mVelX + mXPos + mVelX,
                                                      mVelY + mYPos + mVelY,
                                                      mVelX + mXPos + mVelX,
                                                      mVelY + mYPos + mVelY))
    {
        switch (pTlv->mTlvType)
        {
            case ReliveTypes::eDeathDrop:
                BlowUp();
                break;

            case ReliveTypes::eScrabLeftBound:
                if (!(mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection();
                }
                break;

            case ReliveTypes::eScrabRightBound:
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection();
                }
                break;

            case ReliveTypes::eEnemyStopper:
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
        if ((mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine(0, 1)) || WallHit(mSpriteScale * FP_FromInteger(40), mVelX * FP_FromInteger(3)) || (!(mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)) && Check_IsOnEndOfLine(1, 1)))
        {
            ChangeDirection();
        }
    }

    if (field_13C_brain_state == GreeterBrainStates::eBrain_4_Chase)
    {
        if (WallHit(mSpriteScale * FP_FromInteger(40), mVelX * FP_FromInteger(3))) // TODO: OG bug, raw * 3 here ??
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
        if (static_cast<Bullet*>(pFrom)->mXDistance <= FP_FromInteger(0))
        {
            mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
        }
        else
        {
            mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
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
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
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
        mXPos,
        mYPos - (FP_FromInteger(20) * mSpriteScale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesElumMuds_28);

    relive_new ZapLine(
        mXPos,
        mYPos,
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesElumMuds_28);

    relive_new ZapLine(
        mXPos,
        mYPos - (FP_FromInteger(50) * mSpriteScale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesElumMuds_28);

    relive_new ParticleBurst(
        xpos,
        ypos,
        10,
        mSpriteScale,
        BurstType::eBigRedSparks_3,
        11);

    relive_new ParticleBurst(
        mXPos,
        mYPos - (FP_FromInteger(10) * mSpriteScale),
        10,
        mSpriteScale,
        BurstType::eBigRedSparks_3,
        11);

    pTarget->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit7_Electrocuted);

    relive_new Electrocute(pTarget, TRUE, TRUE);

    pTarget->VTakeDamage(this);

    const CameraPos soundDirection = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    SFX_Play_Camera(SoundEffect::Zap1_49, 0, soundDirection, mSpriteScale);
    SFX_Play_Camera(SoundEffect::Zap2_50, 0, soundDirection, mSpriteScale);

    RandomishSpeak(GreeterSpeak::eLaugh_3);

    field_128_timer = sGnFrame + Math_RandomRange(160, 200);
    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;
}

void Greeter::RandomishSpeak(GreeterSpeak effect)
{
    field_13C_brain_state = GreeterBrainStates::eBrain_2_Speak;
    mVelX = FP_FromInteger(0);
    mAnim.Set_Animation_Data(AnimId::Greeter_Speak, nullptr);
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

bool Greeter::ZapIsNotBlocked(BaseAliveGameObject* pUs, BaseAliveGameObject* pThem)
{
    const PSX_RECT usRect = VGetBoundingRect();
    const PSX_RECT bRectThem = pThem->VGetBoundingRect();

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    return sCollisions->Raycast(
               pUs->mXPos,
               FP_FromInteger(usRect.h + 0xFFE7),
               pThem->mXPos,
               FP_FromInteger(bRectThem.h + 0xFFE7),
               &pLine,
               &hitX,
               &hitY,
               pUs->mScale == Scale::Fg ? kFgWalls : kBgWalls)
        == 1;
}

BaseAliveGameObject* Greeter::GetMudToZap()
{
    for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMudokon)
        {
            const PSX_RECT bRect = pObj->VGetBoundingRect();

            const FP xMid = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP yMid = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (xMid - mXPos < (mSpriteScale * FP_FromInteger(60)) && mXPos - xMid < (mSpriteScale * FP_FromInteger(60)) && yMid - (mYPos - FP_FromInteger(4)) < (mSpriteScale * FP_FromInteger(60)) && mYPos - FP_FromInteger(4) - yMid < (mSpriteScale * FP_FromInteger(60)) && !(sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted)) && !ZapIsNotBlocked(this, pObj))
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

void Greeter::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_13C_brain_state)
    {
        case GreeterBrainStates::eBrain_0_Patrol:
            if (!((sGnFrame - field_124_last_turn_time) % 14))
            {
                const CameraPos soundDirection = gMap.GetDirection(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos);
                SFX_Play_Camera(SoundEffect::WheelSqueak_31, 10, soundDirection, mSpriteScale);
            }

            mVelY = FP_FromInteger(0);
            if ((mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)) == 0)
            {
                mVelX = -(mSpriteScale * FP_FromInteger(3));
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
                mVelX = (mSpriteScale * FP_FromInteger(3));
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
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                mAnim.Set_Animation_Data(AnimId::Greeter_Moving, nullptr);
                mVelY = FP_FromInteger(0);
                field_13E_targetOnLeft = 0;
                field_140_targetOnRight = 0;
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
                }
            }
            break;

        case GreeterBrainStates::eBrain_2_Speak:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 0;
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                mAnim.Set_Animation_Data(AnimId::Greeter_Moving, nullptr);
                mVelY = FP_FromInteger(0);
                field_128_timer = sGnFrame + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_3_ChaseSpeak:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 1;
                field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                mAnim.Set_Animation_Data(AnimId::Greeter_Chase, nullptr);
                mVelY = FP_FromInteger(0);
            }
            break;

        case GreeterBrainStates::eBrain_4_Chase:
        {
            if (!(sGnFrame % 8))
            {
                const CameraPos soundDirection2 = gMap.GetDirection(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos);
                SFX_Play_Camera(SoundEffect::WheelSqueak_31, 10, soundDirection2, mSpriteScale);
            }

            mVelX = -(mSpriteScale * FP_FromInteger(5));
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = mSpriteScale * FP_FromInteger(5);
            }

            const PSX_RECT bRect = sActiveHero->VGetBoundingRect();

            const FP midX = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP midY = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (midX - mXPos >= (mSpriteScale * FP_FromInteger(60)) || mXPos - midX >= (mSpriteScale * FP_FromInteger(60)) || midY - (mYPos - FP_FromInteger(4)) >= (mSpriteScale * FP_FromInteger(60)) || mYPos - FP_FromInteger(4) - midY >= (mSpriteScale * FP_FromInteger(60)) || sActiveHero->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted) || sActiveHero->CantBeDamaged_44BAB0() || ZapIsNotBlocked(this, sActiveHero))
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
            if (WallHit(mSpriteScale * FP_FromInteger(40), FP_FromRaw(3 * mVelX.fpValue))) // TODO: OG bug, why * 3 and not * FP 3??
            {
                mVelX = FP_FromInteger(0);
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak(GreeterSpeak::eWhat_9);
                field_128_timer = sGnFrame + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_6_ToChase:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak(GreeterSpeak::eHi_0);
                field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
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
                mVelY = -mVelY * FP_FromDouble(0.4);
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
                BaseAliveGameObjectLastLineYPos = hitY;

                const CameraPos soundDirection3 = gMap.GetDirection(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    hitY);

                SFX_Play_Camera(SoundEffect::GreeterLand_120, 0, soundDirection3, mSpriteScale);
                if (mVelY > -FP_FromInteger(1))
                {
                    mVelY = FP_FromInteger(0);
                    if (!field_130_bChasing)
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                        mAnim.Set_Animation_Data(AnimId::Greeter_Moving, nullptr);
                    }
                    else
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                        mAnim.Set_Animation_Data(AnimId::Greeter_Chase, nullptr);
                    }
                }
            }
        }
        break;

        default:
            break;
    }

    if (FP_GetExponent(mVelX) || FP_GetExponent(mVelY))
    {
        if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
        {
            const FP xpos = mVelX
                          + mVelX
                          + mVelX
                          + mVelX
                          + mXPos;

            const FP ypos = mVelY
                          + mYPos
                          + mVelY;

            field_138_pTlv = sPathInfo->TlvGetAt(nullptr, xpos, ypos, xpos, ypos);
            HandleRollingAlong();
        }
    }

    bool collisionCheck = true;
    if (field_13C_brain_state == GreeterBrainStates::eBrain_7_Fall)
    {
        field_138_pTlv = sPathInfo->TlvGetAt(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
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
            mAnim.Set_Animation_Data(AnimId::Greeter_Falling, nullptr);
        }
    }

    if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
    {
        mXPos += mVelX;
        mYPos += mVelY;
    }
}
