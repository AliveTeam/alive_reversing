#include "stdafx.h"
#include "Greeter.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "AirExplosion.hpp"
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
#include "Map.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"
#include "Math.hpp"

void Greeter::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Greeter_Turn));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Greeter_Hit));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Greeter_Speak));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Greeter_Moving));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Greeter_Chase));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Greeter_Falling));
}

Greeter::Greeter(relive::Path_Greeter* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eGreeter);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Greeter_Moving));

    SetDoPurpleLightEffect(true);

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBat_Half_14);
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }


    SetCanExplode(true);

    if (pTlv->mFacing == relive::reliveXDirection::eLeft)
    {
        GetAnimation().SetFlipX(true);
    }
    else
    {
        GetAnimation().SetFlipX(false);
    }

    field_134_speed = FP_FromInteger(pTlv->mMotionDetectorSpeed);
    mBrainState = GreeterBrainStates::eBrain_0_Patrol;
    field_12E_bDontSetDestroyed = 1;
    mTlvId = tlvId;

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
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
    }

    auto pMotionDetctor = relive_new MotionDetector(nullptr, Guid{}, this);
    if (pMotionDetctor)
    {
        field_11C_motionDetectorId = pMotionDetctor->mBaseGameObjectId;
    }

    field_140_targetOnRight = 0;
    field_13E_targetOnLeft = 0;

    field_128_timer = sGnFrame + Math_RandomRange(70, 210);

    mTimesShot = 0;

    CreateShadow();

    SetCanSetOffExplosives(true);
    mChasing = false;
}

s32 Greeter::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const GreeterSaveState*>(pBuffer);
    auto pTlv = static_cast<relive::Path_Greeter*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvId));

    auto pGreeter = relive_new Greeter(pTlv, pState->mTlvId);
    if (pGreeter)
    {
        pGreeter->mXPos = pState->field_C_xpos;
        pGreeter->mYPos = pState->field_10_ypos;
        pGreeter->mVelX = pState->field_14_velx;
        pGreeter->mVelY = pState->field_18_vely;

        pGreeter->mCurrentPath = pState->field_8_path_number;
        pGreeter->mCurrentLevel = pState->field_A_lvl_number;
        pGreeter->SetSpriteScale(pState->field_1C_sprite_scale);

        pGreeter->mRGB.SetRGB(pState->field_2_r, pState->field_4_g, pState->field_6_b);

        pGreeter->GetAnimation().SetCurrentFrame(pState->mCurrentFrame);
        pGreeter->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

        pGreeter->SetDrawable(pState->mDrawable & 1);

        pGreeter->GetAnimation().SetRender(pState->mAnimRender & 1);

        if (IsLastFrame(&pGreeter->GetAnimation()))
        {
            pGreeter->GetAnimation().SetIsLastFrame(true);
        }

        pGreeter->mTlvId = pState->mTlvId;
        pGreeter->field_124_last_turn_time = pState->field_30_last_turn_time;
        pGreeter->field_128_timer = pState->field_34_timer;
        pGreeter->mTimesShot = pState->mTimesShot;
        pGreeter->field_12E_bDontSetDestroyed = pState->field_3A_bDontSetDestroyed;
        pGreeter->mChasing = pState->mChasing;
        pGreeter->field_134_speed = pState->field_40_speed;
        pGreeter->mBrainState = pState->mBrainState;
        pGreeter->field_13E_targetOnLeft = pState->field_46_targetOnLeft;
        pGreeter->field_140_targetOnRight = pState->field_48_targetOnRight;

        auto pDetector = static_cast<MotionDetector*>(sObjectIds.Find_Impl(pGreeter->field_11C_motionDetectorId));

        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(pDetector->mLaserId));
        pLaser->mXPos = pState->mMotionLaserXPos;
    }

    return sizeof(GreeterSaveState);
}

s32 Greeter::VGetSaveState(u8* pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return 0;
    }

    auto pState = reinterpret_cast<GreeterSaveState*>(pSaveBuffer);

    pState->field_0_type = ReliveTypes::eGreeter;

    pState->field_C_xpos = mXPos;
    pState->field_10_ypos = mYPos;
    pState->field_14_velx = mVelX;
    pState->field_18_vely = mVelY;

    pState->field_8_path_number = mCurrentPath;
    pState->field_A_lvl_number = mCurrentLevel;
    pState->field_1C_sprite_scale = GetSpriteScale();

    pState->field_2_r = mRGB.r;
    pState->field_4_g = mRGB.g;
    pState->field_6_b = mRGB.b;

    pState->mCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    pState->mDrawable = GetDrawable();
    pState->mAnimRender = GetAnimation().GetRender();
    pState->mTlvId = mTlvId;
    pState->field_30_last_turn_time = field_124_last_turn_time;
    pState->field_34_timer = field_128_timer;

    pState->mTimesShot = mTimesShot;
    pState->field_3A_bDontSetDestroyed = field_12E_bDontSetDestroyed;
    pState->mChasing = mChasing;

    pState->field_40_speed = field_134_speed;
    pState->mBrainState = mBrainState;
    pState->field_46_targetOnLeft = field_13E_targetOnLeft;
    pState->field_48_targetOnRight = field_140_targetOnRight;

    auto pMotionDetector = static_cast<MotionDetector*>(sObjectIds.Find_Impl(field_11C_motionDetectorId));
    auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_Impl(pMotionDetector->mLaserId));
    pState->mMotionLaserXPos = pLaser->mXPos;

    return sizeof(GreeterSaveState);
}

void Greeter::VScreenChanged()
{
    BaseGameObject::VScreenChanged();

    if (sControlledCharacter)
    {
        const FP xDistFromPlayer = FP_Abs(sControlledCharacter->mXPos - mXPos);
        if (xDistFromPlayer > FP_FromInteger(356))
        {
            SetDead(true);
            return;
        }

        const FP yDistFromPlayer = FP_Abs(sControlledCharacter->mYPos - mYPos);
        if (yDistFromPlayer > FP_FromInteger(240))
        {
            SetDead(true);
            return;
        }
    }
}

Greeter::~Greeter()
{
    if (field_12E_bDontSetDestroyed)
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
    }

    BaseGameObject* pMotionDetector = sObjectIds.Find_Impl(field_11C_motionDetectorId);
    if (pMotionDetector)
    {
        pMotionDetector->SetDead(true);
    }
}

void Greeter::BlowUp()
{
    mHealth = FP_FromInteger(0);

    relive_new AirExplosion(
        mXPos,
        mYPos - (GetSpriteScale() * FP_FromInteger(5)),
        GetSpriteScale(),
        0);

    relive_new Gibs(
        GibType::Metal_5,
        mXPos,
        mYPos + FP_FromInteger(50),
        FP_FromInteger(0),
        FP_FromInteger(0),
        GetSpriteScale(),
        0);

    SetDead(true);
    field_12E_bDontSetDestroyed = 0;
}

void Greeter::ChangeDirection()
{
    mBrainState = GreeterBrainStates::eBrain_1_PatrolTurn;
    mVelX = FP_FromInteger(0);
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Turn));
    field_124_last_turn_time = sGnFrame;
}

void Greeter::BounceBackFromShot()
{
    mBrainState = GreeterBrainStates::eBrain_5_Knockback;

    if (GetAnimation().GetFlipX())
    {
        mVelX = FP_FromInteger(-2);
    }
    else
    {
        mVelX = FP_FromInteger(2);
    }

    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;

    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Hit));

    const CameraPos soundDirection = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);
    SFX_Play_Camera(relive::SoundEffects::GreeterKnockback, 0, soundDirection, GetSpriteScale());
}

void Greeter::HandleRollingAlong()
{
    for (relive::Path_TLV* pTlv = field_138_pTlv; pTlv;
         pTlv = gPathInfo->TlvGetAt(pTlv,
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
                if (!(GetAnimation().GetFlipX()) && mBrainState == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection();
                }
                break;

            case ReliveTypes::eScrabRightBound:
                if (GetAnimation().GetFlipX() && mBrainState == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection();
                }
                break;

            case ReliveTypes::eEnemyStopper:
                if (mBrainState != GreeterBrainStates::eBrain_7_Fall)
                {
                    ChangeDirection();
                }
                break;

            default:
                continue;
        }
    }

    if (mBrainState == GreeterBrainStates::eBrain_0_Patrol)
    {
        if ((GetAnimation().GetFlipX() && Check_IsOnEndOfLine(0, 1)) || WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX * FP_FromInteger(3)) || (!(GetAnimation().GetFlipX()) && Check_IsOnEndOfLine(1, 1)))
        {
            ChangeDirection();
        }
    }

    if (mBrainState == GreeterBrainStates::eBrain_4_Chase)
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX * FP_FromInteger(3))) // TODO: OG bug, raw * 3 here ??
        {
            BounceBackFromShot();
        }
    }
}

s16 Greeter::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead() || FP_GetExponent(mHealth) == 0)
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        if (static_cast<Bullet*>(pFrom)->mXDistance <= FP_FromInteger(0))
        {
            GetAnimation().SetFlipX(true);
        }
        else
        {
            GetAnimation().SetFlipX(false);
        }

        if (++mTimesShot <= 10)
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
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eAirExplosion:
            BlowUp();
            return 1;

        case ReliveTypes::eSlamDoor:
            BounceBackFromShot();
            return 1;

        case ReliveTypes::eElectrocute:
            GetAnimation().SetRender(false);
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

void Greeter::ZapTarget(FP xpos, FP ypos, IBaseAliveGameObject* pTarget)
{
    relive_new ScreenShake(0, 0);

    relive_new ZapLine(
        mXPos,
        mYPos - (FP_FromInteger(20) * GetSpriteScale()),
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
        mYPos - (FP_FromInteger(50) * GetSpriteScale()),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesElumMuds_28);

    relive_new ParticleBurst(
        xpos,
        ypos,
        10,
        GetSpriteScale(),
        BurstType::eBigRedSparks_3,
        11);

    relive_new ParticleBurst(
        mXPos,
        mYPos - (FP_FromInteger(10) * GetSpriteScale()),
        10,
        GetSpriteScale(),
        BurstType::eBigRedSparks_3,
        11);

    pTarget->SetElectrocuted(true);

    relive_new Electrocute(pTarget, true, true);

    pTarget->VTakeDamage(this);

    const CameraPos soundDirection = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    SFX_Play_Camera(relive::SoundEffects::Zap1, 0, soundDirection, GetSpriteScale());
    SFX_Play_Camera(relive::SoundEffects::Zap2, 0, soundDirection, GetSpriteScale());

    RandomishSpeak(GreeterSpeak::eLaugh_3);

    field_128_timer = sGnFrame + Math_RandomRange(160, 200);
    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;
}

void Greeter::RandomishSpeak(GreeterSpeak effect)
{
    mBrainState = GreeterBrainStates::eBrain_2_Speak;
    mVelX = FP_FromInteger(0);
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Speak));

    if (effect == GreeterSpeak::eRandomized_1000)
    {
        const s32 randomSpeak = static_cast<s32>(sGnFrame % 4);
        // Will be one of: Hi_0, HereBoy_1,  GetHim_2 or Laugh_3
        Slig_GameSpeak_SFX(static_cast<SligSpeak>(randomSpeak), 100, 700, this);
    }
    else
    {
        Slig_GameSpeak_SFX(static_cast<SligSpeak>(effect), 100, 700, this);
    }
}

bool Greeter::ZapIsNotBlocked(IBaseAliveGameObject* pUs, IBaseAliveGameObject* pThem)
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
               pUs->GetScale() == Scale::Fg ? kFgWalls : kBgWalls)
        == 1;
}

IBaseAliveGameObject* Greeter::GetMudToZap()
{
    for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMudokon)
        {
            const PSX_RECT bRect = pObj->VGetBoundingRect();

            const FP xMid = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP yMid = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (xMid - mXPos < (GetSpriteScale() * FP_FromInteger(60)) && mXPos - xMid < (GetSpriteScale() * FP_FromInteger(60)) && yMid - (mYPos - FP_FromInteger(4)) < (GetSpriteScale() * FP_FromInteger(60)) && mYPos - FP_FromInteger(4) - yMid < (GetSpriteScale() * FP_FromInteger(60)) && !(sActiveHero->GetElectrocuted()) && !ZapIsNotBlocked(this, pObj))
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
        SetDead(true);
    }

    switch (mBrainState)
    {
        case GreeterBrainStates::eBrain_0_Patrol:
            if (!((sGnFrame - field_124_last_turn_time) % 14))
            {
                const CameraPos soundDirection = gMap.GetDirection(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos);
                SFX_Play_Camera(relive::SoundEffects::WheelSqueak, 10, soundDirection, GetSpriteScale());
            }

            mVelY = FP_FromInteger(0);
            if ((GetAnimation().GetFlipX()) == 0)
            {
                mVelX = -(GetSpriteScale() * FP_FromInteger(3));
                if (field_13E_targetOnLeft)
                {
                    RandomishSpeak(GreeterSpeak::eHi_0);
                    mBrainState = GreeterBrainStates::eBrain_3_ChaseSpeak;
                }
                else if (field_140_targetOnRight)
                {
                    ChangeDirection();
                    mBrainState = GreeterBrainStates::eBrain_6_ToChase;
                }
            }
            else
            {
                mVelX = (GetSpriteScale() * FP_FromInteger(3));
                if (field_140_targetOnRight)
                {
                    RandomishSpeak(GreeterSpeak::eHi_0);
                    mBrainState = GreeterBrainStates::eBrain_3_ChaseSpeak;
                }
                else if (field_13E_targetOnLeft)
                {
                    ChangeDirection();
                    mBrainState = GreeterBrainStates::eBrain_6_ToChase;
                }
            }

            if (static_cast<s32>(sGnFrame) > field_128_timer)
            {
                RandomishSpeak(GreeterSpeak::eRandomized_1000);
            }
            break;

        case GreeterBrainStates::eBrain_1_PatrolTurn:
            if (GetAnimation().GetIsLastFrame())
            {
                mBrainState = GreeterBrainStates::eBrain_0_Patrol;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Moving));
                mVelY = FP_FromInteger(0);
                field_13E_targetOnLeft = 0;
                field_140_targetOnRight = 0;
                if (GetAnimation().GetFlipX())
                {
                    GetAnimation().SetFlipX(false);
                }
                else
                {
                    GetAnimation().SetFlipX(true);
                }
            }
            break;

        case GreeterBrainStates::eBrain_2_Speak:
            if (GetAnimation().GetIsLastFrame())
            {
                mChasing = false;
                mBrainState = GreeterBrainStates::eBrain_0_Patrol;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Moving));
                mVelY = FP_FromInteger(0);
                field_128_timer = sGnFrame + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_3_ChaseSpeak:
            if (GetAnimation().GetIsLastFrame())
            {
                mChasing = true;
                mBrainState = GreeterBrainStates::eBrain_4_Chase;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Chase));
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
                SFX_Play_Camera(relive::SoundEffects::WheelSqueak, 10, soundDirection2, GetSpriteScale());
            }

            mVelX = -(GetSpriteScale() * FP_FromInteger(5));
            if (GetAnimation().GetFlipX())
            {
                mVelX = GetSpriteScale() * FP_FromInteger(5);
            }

            const PSX_RECT bRect = sActiveHero->VGetBoundingRect();

            const FP midX = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP midY = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (midX - mXPos >= (GetSpriteScale() * FP_FromInteger(60)) || mXPos - midX >= (GetSpriteScale() * FP_FromInteger(60)) || midY - (mYPos - FP_FromInteger(4)) >= (GetSpriteScale() * FP_FromInteger(60)) || mYPos - FP_FromInteger(4) - midY >= (GetSpriteScale() * FP_FromInteger(60)) || sActiveHero->GetElectrocuted() || sActiveHero->CantBeDamaged_44BAB0() || ZapIsNotBlocked(this, sActiveHero))
            {
                IBaseAliveGameObject* pGonnaZapYa = GetMudToZap();
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
            if (WallHit(GetSpriteScale() * FP_FromInteger(40), FP_FromRaw(3 * mVelX.fpValue))) // TODO: OG bug, why * 3 and not * FP 3??
            {
                mVelX = FP_FromInteger(0);
            }

            if (GetAnimation().GetIsLastFrame())
            {
                RandomishSpeak(GreeterSpeak::eWhat_9);
                field_128_timer = sGnFrame + Math_RandomRange(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_6_ToChase:
            if (GetAnimation().GetIsLastFrame())
            {
                RandomishSpeak(GreeterSpeak::eHi_0);
                mBrainState = GreeterBrainStates::eBrain_3_ChaseSpeak;
                if (GetAnimation().GetFlipX())
                {
                    GetAnimation().SetFlipX(false);
                }
                else
                {
                    GetAnimation().SetFlipX(true);
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

                SFX_Play_Camera(relive::SoundEffects::GreeterLand, 0, soundDirection3, GetSpriteScale());
                if (mVelY > -FP_FromInteger(1))
                {
                    mVelY = FP_FromInteger(0);
                    if (!mChasing)
                    {
                        mBrainState = GreeterBrainStates::eBrain_0_Patrol;
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Moving));
                    }
                    else
                    {
                        mBrainState = GreeterBrainStates::eBrain_4_Chase;
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Chase));
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
        if (mBrainState != GreeterBrainStates::eBrain_7_Fall)
        {
            const FP xpos = mVelX
                          + mVelX
                          + mVelX
                          + mVelX
                          + mXPos;

            const FP ypos = mVelY
                          + mYPos
                          + mVelY;

            field_138_pTlv = gPathInfo->TlvGetAt(nullptr, xpos, ypos, xpos, ypos);
            HandleRollingAlong();
        }
    }

    bool collisionCheck = true;
    if (mBrainState == GreeterBrainStates::eBrain_7_Fall)
    {
        field_138_pTlv = gPathInfo->TlvGetAt(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
        HandleRollingAlong();
        if (mBrainState == GreeterBrainStates::eBrain_7_Fall)
        {
            collisionCheck = false;
        }
    }

    if (collisionCheck)
    {
        if (Check_IsOnEndOfLine(0, 0))
        {
            mBrainState = GreeterBrainStates::eBrain_7_Fall;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Greeter_Falling));
        }
    }

    if (mBrainState != GreeterBrainStates::eBrain_7_Fall)
    {
        mXPos += mVelX;
        mYPos += mVelY;
    }
}
