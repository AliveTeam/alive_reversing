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

    if (field_120_pCollisionLine)
    {
        Rect_Clear(&field_120_pCollisionLine->mRect);
    }

    if (field_114_pRollingBallShaker)
    {
        field_114_pRollingBallShaker->field_32_bKillMe = TRUE;
        field_114_pRollingBallShaker->mBaseGameObjectRefCount--;
        field_114_pRollingBallShaker = nullptr;
    }

    u8** pRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 0, 0);
    ResourceManager::FreeResource_455550(pRes);
}

RollingBall::RollingBall(Path_RollingBall* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eRollingBall;
    
    const AnimRecord rec = AO::AnimRec(AnimId::Stone_Ball);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallPortalClip_Half_31;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallPortalClip_12;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }

    if (pTlv->field_1A_roll_direction == XDirection_short::eLeft_0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    field_110_release_switch_id = pTlv->field_1C_release_switch_id;
    field_118_speed = FP_FromRaw(pTlv->field_1E_speed << 8);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_118_speed = -FP_FromRaw(pTlv->field_1E_speed << 8);
    }

    field_11C_acceleration = FP_FromRaw(pTlv->field_20_acceleration << 8);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    FP castX = {};
    FP castY = {};

    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &castX,
            &castY,
            // yeah this should be simplified, WTF!
            mBaseAnimatedWithPhysicsGameObject_SpriteScale - FP_FromDouble(0.5) != FP_FromInteger(0) ? kFgFloor : kBgFloor)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = castY;
    }

    MapFollowMe_401D30(TRUE);
    field_10C_tlvInfo = tlvInfo;
    field_112_state = States::eInactive_0;
    field_114_pRollingBallShaker = nullptr;
    field_120_pCollisionLine = nullptr;

    mShadow = relive_new Shadow();

    // Looks strange, it just bumps the res ref count
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 1, 0);

    if (!SwitchStates_Get(field_110_release_switch_id))
    {
        return;
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple && gMap.mCurrentPath == 2)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(2522);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(1300);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombRollingBall_35;
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
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromDouble(2.5);
                field_112_state = States::eStartRolling_1;
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(15608, 0);
                field_114_pRollingBallShaker = relive_new RollingBallShaker();
                if (field_114_pRollingBallShaker)
                {
                    field_114_pRollingBallShaker->mBaseGameObjectRefCount++;
                }
            }
            else if (!gMap.Is_Point_In_Current_Camera(
                         mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                         mBaseAnimatedWithPhysicsGameObject_PathNumber,
                         mBaseAnimatedWithPhysicsGameObject_XPos,
                         mBaseAnimatedWithPhysicsGameObject_YPos,
                         0))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            return;

        case States::eStartRolling_1:
        {
            if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame % 3))
            {
                SfxPlayMono(static_cast<SoundEffect>(Math_RandomRange(SoundEffect::RollingBallNoise1_71, SoundEffect::RollingBallNoise2_72)), 0, 0);
            }

            Accelerate();

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromInteger(0)))
            {
                if (pLine->mLineType != eLineTypes::eFloor_0 && pLine->mLineType != eLineTypes::eBackgroundFloor_4)
                {
                    return;
                }

                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
                field_112_state = States::eRolling_2;
            }
            return;
        }

        case States::eRolling_2:
        {
            if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame % 3))
            {
                SfxPlayMono(static_cast<SoundEffect>(Math_RandomRange(SoundEffect::RollingBallNoise1_71, SoundEffect::RollingBallNoise2_72)), 0, 0);
            }

            Accelerate();

            BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_VelX);

            u16 result = 0;
            CamX_VoidSkipper(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_VelX, 50, &result);
            if (result == 1 || result == 2)
            {
                MapFollowMe_401D30(0);
            }

            CrushThingsInTheWay();

            if (EventGet(kEventDeathReset))
            {
                field_114_pRollingBallShaker->mBaseGameObjectRefCount--;
                field_114_pRollingBallShaker->field_32_bKillMe = 1;
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                field_114_pRollingBallShaker = nullptr;
            }
            else if (!BaseAliveGameObjectCollisionLine)
            {
                field_112_state = States::eFallingAndHittingWall_3;

                field_114_pRollingBallShaker->mBaseGameObjectRefCount--;
                field_114_pRollingBallShaker->field_32_bKillMe = 1;
                field_114_pRollingBallShaker = nullptr;

                mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
                BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            }
            return;
        }

        case States::eFallingAndHittingWall_3:
        {
            if (WallHit_401930(FP_FromInteger(30), mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                relive_new ParticleBurst(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(30),
                    150,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    BurstType::eFallingRocks_0);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_1, 1);

                relive_new ScreenShake(0);

                mBaseGameObjectFlags.Set(Options::eDead);

                const CameraPos direction = gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
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

            if (EventGet(kEventDeathReset))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
                CrushThingsInTheWay();
                return;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (!InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
            {
                if (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(240))
                {
                    if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple
                        && gMap.mCurrentPath == 2
                        && !sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
                    {
                        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
                        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(2522);
                        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(1300);
                        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BombRollingBall_35;
                        field_112_state = States::eCrushedBees_4;
                        CrushThingsInTheWay();
                        return;
                    }
                    mBaseGameObjectFlags.Set(Options::eDead);
                }
                CrushThingsInTheWay();
                return;
            }

            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            mBaseAnimatedWithPhysicsGameObject_VelY = (-mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(0.8));

            relive_new ScreenShake(0);

            const CameraPos direction = gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
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
            if (mBaseAnimatedWithPhysicsGameObject_LvlNumber != gMap.mCurrentLevel || mBaseAnimatedWithPhysicsGameObject_PathNumber != gMap.mCurrentPath || EventGet(kEventDeathReset))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            return;

        default:
            return;
    }
}

void RollingBall::Accelerate()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX > field_118_speed)
        {
            mBaseAnimatedWithPhysicsGameObject_VelX -= field_11C_acceleration;
            mBaseAnimatedWithPhysicsGameObject_VelY = (-mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.5));
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX < field_118_speed)
        {
            mBaseAnimatedWithPhysicsGameObject_VelX += field_11C_acceleration;
            mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.5));
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

        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            const PSX_RECT bOurRect = VGetBoundingRect();

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            const PSX_RECT bObjRect = pAliveObj->VGetBoundingRect();

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
