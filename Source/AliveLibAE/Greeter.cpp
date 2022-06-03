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
    SetType(AETypes::eGreeter_64);
    const AnimRecord& rec = AnimRec(AnimId::Greeter_Moving);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mApplyShadows |= 2u;

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
        mSpriteScale = FP_FromDouble(0.5);
        mScale = 0;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
        mSpriteScale = FP_FromInteger(1);
        mScale = 1;
    }


    mGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    if (pTlv->field_14_start_direction == XDirection_short::eLeft_0)
    {
        mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mAnim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }

    field_134_speed = FP_FromInteger(pTlv->field_12_motion_detector_speed);
    field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
    field_12E_bDontSetDestroyed = 1;
    field_118_tlvInfo = tlvInfo;

    mXPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &mCollisionLine,
            &hitX,
            &hitY,
            mScale ? 1 : 16))
    {
        mYPos = hitY;
    }

    auto pMotionDetctor = ae_new<MotionDetector>(nullptr, 0, this);
    if (pMotionDetctor)
    {
        field_11C_motionDetectorId = pMotionDetctor->field_8_object_id;
    }

    field_140_targetOnRight = 0;
    field_13E_targetOnLeft = 0;

    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange(70, 210);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    field_12C_timesShot = 0;

    mShadow = ae_new<Shadow>();

    mAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_130_bChasing = 0;
}

s32 Greeter::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Greeter_State*>(pBuffer);
    auto pTlv = static_cast<Path_Greeter*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_28_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMflareResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MFLARE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMotionResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("MOTION.BAN", nullptr);
    }
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGreeterResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("GREETER.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSplineResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SPLINE.BAN", nullptr);
    }
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("METAL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("EXPLO2.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
    }

    auto pGreeter = ae_new<Greeter>(pTlv, pState->field_28_tlvInfo);
    if (pGreeter)
    {
        pGreeter->mXPos = pState->field_C_xpos;
        pGreeter->mYPos = pState->field_10_ypos;
        pGreeter->mVelX = pState->field_14_velx;
        pGreeter->mVelY = pState->field_18_vely;

        pGreeter->mPathNumber = pState->field_8_path_number;
        pGreeter->mLvlNumber = pState->field_A_lvl_number;
        pGreeter->mSpriteScale = pState->field_1C_sprite_scale;

        pGreeter->mRed = pState->field_2_r;
        pGreeter->mGreen = pState->field_4_g;
        pGreeter->mBlue = pState->field_6_b;

        pGreeter->mAnim.field_92_current_frame = pState->field_20_current_frame;
        pGreeter->mAnim.mFrameChangeCounter = pState->field_22_frame_change_counter;

        pGreeter->mGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);

        pGreeter->mAnim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

        if (IsLastFrame(&pGreeter->mAnim))
        {
            pGreeter->mAnim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
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
    if (mAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Greeter_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eGreeter_64;

    pState->field_C_xpos = mXPos;
    pState->field_10_ypos = mYPos;
    pState->field_14_velx = mVelX;
    pState->field_18_vely = mVelY;

    pState->field_8_path_number = mPathNumber;
    pState->field_A_lvl_number = mLvlNumber;
    pState->field_1C_sprite_scale = mSpriteScale;

    pState->field_2_r = mRed;
    pState->field_4_g = mGreen;
    pState->field_6_b = mBlue;

    pState->field_20_current_frame = mAnim.field_92_current_frame;
    pState->field_22_frame_change_counter = mAnim.mFrameChangeCounter;
    pState->field_25_bDrawable = mGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = mAnim.mAnimFlags.Get(AnimFlags::eBit3_Render);
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
            mGameObjectFlags.Set(BaseGameObject::eDead);
            return;
        }

        const FP yDistFromPlayer = FP_Abs(sControlledCharacter->mYPos - mYPos);
        if (yDistFromPlayer > FP_FromInteger(240))
        {
            mGameObjectFlags.Set(BaseGameObject::eDead);
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
        pMotionDetector->mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Greeter::BlowUp()
{
    mHealth = FP_FromInteger(0);

    ae_new<Explosion>(
        mXPos,
        mYPos - (mSpriteScale * FP_FromInteger(5)),
        mSpriteScale,
        0);

    ae_new<Gibs>(
        GibType::Metal_5,
        mXPos,
        mYPos + FP_FromInteger(50),
        FP_FromInteger(0),
        FP_FromInteger(0),
        mSpriteScale,
        0);

    mGameObjectFlags.Set(BaseGameObject::eDead);
    field_12E_bDontSetDestroyed = 0;
}

void Greeter::ChangeDirection()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_1_PatrolTurn;
    mVelX = FP_FromInteger(0);
    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Turn);
    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    field_124_last_turn_time = sGnFrame_5C1B84;
}

void Greeter::BounceBackFromShot()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_5_Knockback;

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = FP_FromInteger(-2);
    }
    else
    {
        mVelX = FP_FromInteger(2);
    }

    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;

    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Hit);
    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

    const CameraPos soundDirection = gMap.GetDirection_4811A0(mLvlNumber, mPathNumber, mXPos, mYPos);
    SFX_Play_Camera(SoundEffect::GreeterKnockback_121, 0, soundDirection, mSpriteScale);
}

void Greeter::HandleRollingAlong()
{
    for (Path_TLV* pTlv = field_138_pTlv; pTlv;
         pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv,
                                                      mVelX + mXPos + mVelX,
                                                      mVelY + mYPos + mVelY,
                                                      mVelX + mXPos + mVelX,
                                                      mVelY + mYPos + mVelY))
    {
        switch (pTlv->field_4_type.mType)
        {
            case TlvTypes::DeathDrop_4:
                BlowUp();
                break;

            case TlvTypes::ScrabLeftBound_43:
                if (!(mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection();
                }
                break;

            case TlvTypes::ScrabRightBound_44:
                if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
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
        if ((mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine(0, 1)) || WallHit(mSpriteScale * FP_FromInteger(40), mVelX * FP_FromInteger(3)) || (!(mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) && Check_IsOnEndOfLine(1, 1)))
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
    if (mGameObjectFlags.Get(BaseGameObject::eDead) || FP_GetExponent(mHealth) == 0)
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case AETypes::eBullet_15:
        if (static_cast<Bullet*>(pFrom)->field_30_x_distance <= FP_FromInteger(0))
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }

        if (++field_12C_timesShot <= 10)
        {
            BounceBackFromShot_447B10();
        }
        else
        {
            BlowUp_447E50();
        }
        return 1;

        case AETypes::eDrill_30:
        case AETypes::eElectricWall_39:
            if (static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)->mAnim.field_10_frame_delay <= 0)
            {
                mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
            else
            {
                mAnim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
            }

            if (++field_12C_timesShot <= 10)
            {
                BounceBackFromShot_447B10();
            }
            else
            {
                BlowUp_447E50();
            }
            return 1;

        case AETypes::eBaseBomb_46:
        case AETypes::eRockSpawner_48:
        case AETypes::eMeatSaw_86:
        case AETypes::eMineCar_89:
        case AETypes::eNeverSet_107:
        case AETypes::eExplosion_109:
            BlowUp();
            return 1;

        case AETypes::eSlamDoor_122:
            BounceBackFromShot();
            return 1;

        case AETypes::eElectrocute_150:
            mAnim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            BlowUp();
            return 1;

        default:
            BlowUp_447E50();
            return 1;
    }
}

void Greeter::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    BounceBackFromShot();
}

void Greeter::ZapTarget(FP xpos, FP ypos, BaseAliveGameObject* pTarget)
{
    ae_new<ScreenShake>(0, 0);

    ae_new<ZapLine>(
        mXPos,
        mYPos - (FP_FromInteger(20) * mSpriteScale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    ae_new<ZapLine>(
        mXPos,
        mYPos,
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    ae_new<ZapLine>(
        mXPos,
        mYPos - (FP_FromInteger(50) * mSpriteScale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    ae_new<ParticleBurst>(
        xpos,
        ypos,
        10,
        mSpriteScale,
        BurstType::eBigRedSparks_3,
        11);

    ae_new<ParticleBurst>(
        mXPos,
        mYPos - (FP_FromInteger(10) * mSpriteScale),
        10,
        mSpriteScale,
        BurstType::eBigRedSparks_3,
        11);

    pTarget->mAliveGameObjectFlags.Set(Flags_114::e114_Bit7_Electrocuted);

    ae_new<Electrocute>(pTarget, TRUE, TRUE);

    pTarget->VTakeDamage(this);

    const CameraPos soundDirection = gMap.GetDirection_4811A0(
        mLvlNumber,
        mPathNumber,
        mXPos,
        mYPos);

    SFX_Play_Camera(SoundEffect::Zap1_49, 0, soundDirection, mSpriteScale);
    SFX_Play_Camera(SoundEffect::Zap2_50, 0, soundDirection, mSpriteScale);

    RandomishSpeak(GreeterSpeak::eLaugh_3);

    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange(160, 200);
    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;
}

void Greeter::RandomishSpeak(GreeterSpeak effect)
{
    field_13C_brain_state = GreeterBrainStates::eBrain_2_Speak;
    mVelX = FP_FromInteger(0);
    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Speak);
    mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    field_120_unused = sGnFrame_5C1B84 + 25;

    if (effect == GreeterSpeak::eRandomized_1000)
    {
        const s32 randomSpeak = static_cast<s32>(sGnFrame_5C1B84 % 4);
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
    PSX_RECT usRect = {};
    VGetBoundingRect(&usRect, 1);

    PSX_RECT bRectThem = {};
    pThem->VGetBoundingRect(&bRectThem, 1);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast(
               pUs->mXPos,
               FP_FromInteger(usRect.h + 0xFFE7),
               pThem->mXPos,
               FP_FromInteger(bRectThem.h + 0xFFE7),
               &pLine,
               &hitX,
               &hitY,
               pUs->mScale != 0 ? 6 : 0x60)
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

        if (pObj->Type() == AETypes::eMudokon_110)
        {
            PSX_RECT bRect = {};
            pObj->VGetBoundingRect(&bRect, 1);

            const FP xMid = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP yMid = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (xMid - mXPos < (mSpriteScale * FP_FromInteger(60)) && mXPos - xMid < (mSpriteScale * FP_FromInteger(60)) && yMid - (mYPos - FP_FromInteger(4)) < (mSpriteScale * FP_FromInteger(60)) && mYPos - FP_FromInteger(4) - yMid < (mSpriteScale * FP_FromInteger(60)) && !(sActiveHero->mAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted)) && !ZapIsNotBlocked(this, pObj))
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
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_13C_brain_state)
    {
        case GreeterBrainStates::eBrain_0_Patrol:
            if (!((sGnFrame_5C1B84 - field_124_last_turn_time) % 14))
            {
                const CameraPos soundDirection = gMap.GetDirection_4811A0(
                    mLvlNumber,
                    mPathNumber,
                    mXPos,
                    mYPos);
                SFX_Play_Camera(SoundEffect::WheelSqueak_31, 10, soundDirection, mSpriteScale);
            }

            mVelY = FP_FromInteger(0);
            if ((mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) == 0)
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

            if (static_cast<s32>(sGnFrame_5C1B84) > field_128_timer)
            {
                RandomishSpeak(GreeterSpeak::eRandomized_1000);
            }
            break;

        case GreeterBrainStates::eBrain_1_PatrolTurn:
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                mVelY = FP_FromInteger(0);
                field_13E_targetOnLeft = 0;
                field_140_targetOnRight = 0;
                if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mAnim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                }
            }
            break;

        case GreeterBrainStates::eBrain_2_Speak:
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 0;
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                mVelY = FP_FromInteger(0);
                field_128_timer = sGnFrame_5C1B84 + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_3_ChaseSpeak:
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 1;
                field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Chase);
                mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                mVelY = FP_FromInteger(0);
            }
            break;

        case GreeterBrainStates::eBrain_4_Chase:
        {
            if (!(sGnFrame_5C1B84 % 8))
            {
                const CameraPos soundDirection2 = gMap.GetDirection_4811A0(
                    mLvlNumber,
                    mPathNumber,
                    mXPos,
                    mYPos);
                SFX_Play_Camera(SoundEffect::WheelSqueak_31, 10, soundDirection2, mSpriteScale);
            }

            mVelX = -(mSpriteScale * FP_FromInteger(5));
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = mSpriteScale * FP_FromInteger(5);
            }

            PSX_RECT bRect = {};
            sActiveHero->VGetBoundingRect(&bRect, 1);

            const FP midX = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP midY = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (midX - mXPos >= (mSpriteScale * FP_FromInteger(60)) || mXPos - midX >= (mSpriteScale * FP_FromInteger(60)) || midY - (mYPos - FP_FromInteger(4)) >= (mSpriteScale * FP_FromInteger(60)) || mYPos - FP_FromInteger(4) - midY >= (mSpriteScale * FP_FromInteger(60)) || sActiveHero->mAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted) || sActiveHero->CantBeDamaged_44BAB0() || ZapIsNotBlocked(this, sActiveHero))
            {
                BaseAliveGameObject* pGonnaZapYa = GetMudToZap();
                if (pGonnaZapYa)
                {
                    PSX_RECT bZapRect = {};
                    pGonnaZapYa->VGetBoundingRect(&bZapRect, 1);

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

            if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak(GreeterSpeak::eWhat_9);
                field_128_timer = sGnFrame_5C1B84 + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_6_ToChase:
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak(GreeterSpeak::eHi_0);
                field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mAnim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
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
                mCollisionLine = pLine;
                mYPos = hitY;
                mLastLineYPos = hitY;

                const CameraPos soundDirection3 = gMap.GetDirection_4811A0(
                    mLvlNumber,
                    mPathNumber,
                    mXPos,
                    hitY);

                SFX_Play_Camera(SoundEffect::GreeterLand_120, 0, soundDirection3, mSpriteScale);
                if (mVelY > -FP_FromInteger(1))
                {
                    mVelY = FP_FromInteger(0);
                    if (!field_130_bChasing)
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                        const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                        mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
                    }
                    else
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                        const AnimRecord& animRec = AnimRec(AnimId::Greeter_Chase);
                        mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
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

            field_138_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(nullptr, xpos, ypos, xpos, ypos);
            HandleRollingAlong();
        }
    }

    bool collisionCheck = true;
    if (field_13C_brain_state == GreeterBrainStates::eBrain_7_Fall)
    {
        field_138_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
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
            const AnimRecord& animRec = AnimRec(AnimId::Greeter_Falling);
            mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        }
    }

    if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
    {
        mXPos += mVelX;
        mYPos += mVelY;
    }
}
