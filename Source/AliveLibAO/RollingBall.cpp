#include "stdafx_ao.h"
#include "Function.hpp"
#include "RollingBall.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "RollingBallShaker.hpp"
#include "Shadow.hpp"
#include "SwitchStates.hpp"
#include "ParticleBurst.hpp"
#include "Flash.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "Grid.hpp"

namespace AO {

RollingBall::~RollingBall()
{
    if (field_112_state != States::eInactive_0)
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
    }

    if (mCollisionLine)
    {
        Rect_Clear(&mCollisionLine->field_0_rect);
    }

    if (field_114_pRollingBallShaker)
    {
        field_114_pRollingBallShaker->field_32_bKillMe = TRUE;
        field_114_pRollingBallShaker->field_C_refCount--;
        field_114_pRollingBallShaker = nullptr;
    }

    u8** pRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 0, 0);
    ResourceManager::FreeResource_455550(pRes);
}

RollingBall::RollingBall(Path_RollingBall* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mTypeId = Types::eRollingBall_72;
    
    const AnimRecord& rec = AO::AnimRec(AnimId::Stone_Ball);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mAnim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallPortalClip_Half_31;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallPortalClip_12;
        mScale = 0;
    }

    if (pTlv->field_1A_roll_direction == XDirection_short::eLeft_0)
    {
        mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    field_110_release_switch_id = pTlv->field_1C_release_switch_id;
    field_118_speed = FP_FromRaw(pTlv->field_1E_speed << 8);

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_118_speed = -FP_FromRaw(pTlv->field_1E_speed << 8);
    }

    field_11C_acceleration = FP_FromRaw(pTlv->field_20_acceleration << 8);

    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    FP castX = {};
    FP castY = {};

    if (sCollisions_DArray_504C6C->RayCast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &mCollisionLine,
            &castX,
            &castY,
            // yeah this should be simplified, WTF!
            mSpriteScale - FP_FromDouble(0.5) != FP_FromInteger(0) ? 1 : 0x10)
        == 1)
    {
        mYPos = castY;
    }

    MapFollowMe_401D30(TRUE);
    field_10C_tlvInfo = tlvInfo;
    field_112_state = States::eInactive_0;
    field_114_pRollingBallShaker = nullptr;
    mCollisionLine = nullptr;

    mShadow = ao_new<Shadow>();

    // Looks strange, it just bumps the res ref count
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 1, 0);

    if (!SwitchStates_Get(field_110_release_switch_id))
    {
        return;
    }

    if (gMap.mCurrentLevel == LevelIds::eForestTemple_4 && gMap.mCurrentPath == 2)
    {
        mAnim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        mXPos = FP_FromInteger(2522);
        mYPos = FP_FromInteger(1300);
        mAnim.mRenderLayer = Layer::eLayer_BombRollingBall_35;
        field_112_state = States::eCrushedBees_4;
    }
}

void RollingBall::VUpdate()
{
    switch (field_112_state)
    {
        case States::eInactive_0:
            if (SwitchStates_Get(field_110_release_switch_id))
            {
                mVelY = FP_FromDouble(2.5);
                field_112_state = States::eStartRolling_1;
                mAnim.Set_Animation_Data(15608, 0);
                field_114_pRollingBallShaker = ao_new<RollingBallShaker>();
                if (field_114_pRollingBallShaker)
                {
                    field_114_pRollingBallShaker->field_C_refCount++;
                }
            }
            else if (!gMap.Is_Point_In_Current_Camera_4449C0(
                         mLvlNumber,
                         mPathNumber,
                         mXPos,
                         mYPos,
                         0))
            {
                mGameObjectFlags.Set(Options::eDead);
            }
            return;

        case States::eStartRolling_1:
        {
            if (!(mAnim.field_92_current_frame % 3))
            {
                SFX_Play_Mono(static_cast<SoundEffect>(Math_RandomRange_450F20(SoundEffect::RollingBallNoise1_71, SoundEffect::RollingBallNoise2_72)), 0, 0);
            }

            Accelerate();

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromInteger(0)))
            {
                if (pLine->field_8_type != eLineTypes::eFloor_0 && pLine->field_8_type != eLineTypes::eBackgroundFloor_4)
                {
                    return;
                }

                mVelY = FP_FromInteger(0);
                mXPos = hitX;
                mYPos = hitY;
                mCollisionLine = pLine;
                field_112_state = States::eRolling_2;
            }
            return;
        }

        case States::eRolling_2:
        {
            if (!(mAnim.field_92_current_frame % 3))
            {
                SFX_Play_Mono(static_cast<SoundEffect>(Math_RandomRange_450F20(SoundEffect::RollingBallNoise1_71, SoundEffect::RollingBallNoise2_72)), 0, 0);
            }

            Accelerate();

            mCollisionLine = mCollisionLine->MoveOnLine(
                &mXPos,
                &mYPos,
                mVelX);

            u16 result = 0;
            CamX_VoidSkipper(mXPos, mVelX, 50, &result);
            if (result == 1 || result == 2)
            {
                MapFollowMe_401D30(0);
            }

            CrushThingsInTheWay();

            if (Event_Get(kEventDeathReset_4))
            {
                field_114_pRollingBallShaker->field_C_refCount--;
                field_114_pRollingBallShaker->field_32_bKillMe = 1;
                mGameObjectFlags.Set(BaseGameObject::eDead);
                field_114_pRollingBallShaker = nullptr;
            }
            else if (!mCollisionLine)
            {
                field_112_state = States::eFallingAndHittingWall_3;

                field_114_pRollingBallShaker->field_C_refCount--;
                field_114_pRollingBallShaker->field_32_bKillMe = 1;
                field_114_pRollingBallShaker = nullptr;

                mXPos += mVelX;
                mLastLineYPos = mYPos;
            }
            return;
        }

        case States::eFallingAndHittingWall_3:
        {
            if (WallHit_401930(FP_FromInteger(30), mVelX))
            {
                ao_new<ParticleBurst>(
                    mXPos,
                    mYPos - FP_FromInteger(30),
                    150,
                    mSpriteScale,
                    BurstType::eFallingRocks_0);

                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_1, 1);

                ao_new<ScreenShake>(0);

                mGameObjectFlags.Set(Options::eDead);

                const CameraPos direction = gMap.GetDirection(mLvlNumber, mPathNumber, mXPos, mYPos);
                SFX_Play_Camera(SoundEffect::IngameTransition_107, 50, direction);

                switch (direction)
                {
                    case CameraPos::eCamCurrent_0:
                        SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 60, 60);
                        break;

                    case CameraPos::eCamTop_1:
                    case CameraPos::eCamBottom_2:
                        SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 40, 40);
                        break;

                    case CameraPos::eCamLeft_3:
                        SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 40, 13);
                        break;

                    case CameraPos::eCamRight_4:
                        SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 13, 40);
                        break;

                    default:
                        return;
                }
                return;
            }

            if (Event_Get(kEventDeathReset_4))
            {
                mGameObjectFlags.Set(Options::eDead);
                CrushThingsInTheWay();
                return;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (!InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
            {
                if (mYPos - mLastLineYPos > FP_FromInteger(240))
                {
                    if (gMap.mCurrentLevel == LevelIds::eForestTemple_4
                        && gMap.mCurrentPath == 2
                        && !sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
                    {
                        mAnim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
                        mXPos = FP_FromInteger(2522);
                        mYPos = FP_FromInteger(1300);
                        mAnim.mRenderLayer = Layer::eLayer_BombRollingBall_35;
                        field_112_state = States::eCrushedBees_4;
                        CrushThingsInTheWay();
                        return;
                    }
                    mGameObjectFlags.Set(Options::eDead);
                }
                CrushThingsInTheWay();
                return;
            }

            mYPos = hitY;
            mLastLineYPos = mYPos;
            mVelY = (-mVelY * FP_FromDouble(0.8));

            ao_new<ScreenShake>(0);

            const CameraPos direction = gMap.GetDirection(mLvlNumber, mPathNumber, mXPos, mYPos);
            SFX_Play_Camera(SoundEffect::IngameTransition_107, 50, direction);

            switch (direction)
            {
                case CameraPos::eCamCurrent_0:
                    SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 60, 60);
                    CrushThingsInTheWay();
                    break;

                case CameraPos::eCamTop_1:
                case CameraPos::eCamBottom_2:
                    SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 40, 40);
                    CrushThingsInTheWay();
                    break;

                case CameraPos::eCamLeft_3:
                    SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 40, 13);
                    CrushThingsInTheWay();
                    break;

                case CameraPos::eCamRight_4:
                    SND_SEQ_Play_477760(SeqId::eExplosion1_21, 1, 13, 40);
                    CrushThingsInTheWay();
                    break;

                default:
                    CrushThingsInTheWay();
                    return;
            }
            return;
        }

        case States::eCrushedBees_4:
            if (mLvlNumber != gMap.mCurrentLevel || mPathNumber != gMap.mCurrentPath || Event_Get(kEventDeathReset_4))
            {
                mGameObjectFlags.Set(Options::eDead);
            }
            return;

        default:
            return;
    }
}

void RollingBall::Accelerate()
{
    if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (mVelX > field_118_speed)
        {
            mVelX -= field_11C_acceleration;
            mVelY = (-mVelX * FP_FromDouble(0.5));
        }
    }
    else
    {
        if (mVelX < field_118_speed)
        {
            mVelX += field_11C_acceleration;
            mVelY = (mVelX * FP_FromDouble(0.5));
        }
    }
}

void RollingBall::CrushThingsInTheWay()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            PSX_RECT bOurRect = {};
            VGetBoundingRect(&bOurRect, 1);

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            PSX_RECT bObjRect = {};
            pAliveObj->VGetBoundingRect(&bObjRect, 1);

            if (bOurRect.x <= bObjRect.w
                && bOurRect.w >= bObjRect.x
                && bOurRect.h >= bObjRect.y
                && bOurRect.y <= bObjRect.h)
            {
                pAliveObj->VTakeDamage(this);
            }
        }
    }
}

} // namespace AO
