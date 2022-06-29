#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slog.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "PsxDisplay.hpp"
#include "Blood.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "SwitchStates.hpp"
#include "PlatformBase.hpp"
#include "CameraSwapper.hpp"
#include "SnoozeParticle.hpp"
#include "Shadow.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "Particle.hpp"
#include "GameSpeak.hpp"
#include "Grid.hpp"

void Slog_ForceLink()
{ }

namespace AO {

using TSlogMotionFunction = decltype(&Slog::Motion_0_Idle_4742E0);

const TSlogMotionFunction sSlogMotionTable_4CFD30[] = {
    &Slog::Motion_0_Idle_4742E0,
    &Slog::Motion_1_Walk_4743F0,
    &Slog::Motion_2_Run_4749A0,
    &Slog::Motion_3_TurnAround_474C70,
    &Slog::Motion_4_Fall_4750C0,
    &Slog::Motion_5_Unknown_474070,
    &Slog::Motion_6_MoveHeadUpwards_474220,
    &Slog::Motion_7_SlideTurn_474DB0,
    &Slog::Motion_8_StopRunning_474EC0,
    &Slog::Motion_9_StartWalking_474690,
    &Slog::Motion_10_EndWalking_4747D0,
    &Slog::Motion_11_Land_475AB0,
    &Slog::Motion_12_Unknown_475B50,
    &Slog::Motion_13_StartFastBarking_475B70,
    &Slog::Motion_14_EndFastBarking_475BB0,
    &Slog::Motion_15_AngryBark_475290,
    &Slog::Motion_16_Sleeping_4752E0,
    &Slog::Motion_17_MoveHeadDownwards_475510,
    &Slog::Motion_18_WakeUp_475460,
    &Slog::Motion_19_JumpForwards_475610,
    &Slog::Motion_20_JumpUpwards_475890,
    &Slog::Motion_21_Eating_475900,
    &Slog::Motion_22_Dying_475A90,
    &Slog::Motion_23_Scratch_475550,
    &Slog::Motion_24_Growl_475590,
};

using TSlogBrainFunction = decltype(&Slog::Brain_0_ListeningToSlig_472450);

const TSlogBrainFunction sSlogBrainTable[] = {
    &Slog::Brain_0_ListeningToSlig_472450,
    &Slog::Brain_1_Idle_4719C0,
    &Slog::Brain_2_ChasingAbe_470F50,
    &Slog::Brain_3_Dead_4721B0};

const AnimId sSlogAnimIdTable_4CFD98[25] = {
    AnimId::Slog_Idle,
    AnimId::Slog_Walk,
    AnimId::Slog_Run,
    AnimId::Slog_TurnAround,
    AnimId::Slog_Fall,
    AnimId::Slog_Idle,
    AnimId::Slog_MoveHeadUpwards,
    AnimId::Slog_SlideTurn,
    AnimId::Slog_StopRunning,
    AnimId::Slog_StartWalking,
    AnimId::Slog_EndWalking,
    AnimId::Slog_Land,
    AnimId::Slog_AO_M_12_Unknown,
    AnimId::Slog_StartFastBarking,
    AnimId::Slog_EndFastBarking,
    AnimId::Slog_AngryBark,
    AnimId::Slog_Sleeping,
    AnimId::Slog_MoveHeadDownwards,
    AnimId::Slog_WakeUp,
    AnimId::Slog_JumpForwards,
    AnimId::Slog_JumpUpwards,
    AnimId::Slog_Eating,
    AnimId::Slog_Dying,
    AnimId::Slog_Scratch,
    AnimId::Slog_Growl};

ALIVE_VAR(1, 0x9F11C8, s16, gNumSlogs_9F11C8, 0);
ALIVE_VAR(1, 0x9F11C4, u8, sSlogRndSeed_9F11C4, 0);

static u8 Slog_NextRandom()
{
    return sRandomBytes_4BBE30[sSlogRndSeed_9F11C4++];
}

Slog::Slog(Path_Slog* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    if (pTlv->field_18_scale == Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
    }

    Init();

    field_138_tlvInfo = tlvInfo;
    field_114_brain_idx = 1;
    field_176 = 1;

    field_158_wake_up_anger = pTlv->field_1E_wake_up_anger;

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1A_start_direction == XDirection_short::eLeft_0);

    field_15A_total_anger = pTlv->field_1E_wake_up_anger + pTlv->field_20_bark_anger;
    field_15C_chase_anger = field_15A_total_anger + pTlv->field_22_chase_anger;
    field_17E_asleep = pTlv->field_1C_asleep;
    field_170 = pTlv->field_24_chase_delay;
    field_168_anger_switch_id = pTlv->field_28_anger_switch_id;

    if (pTlv->field_1C_asleep == Choice_short::eYes_1)
    {
        mCurrentMotion = eSlogMotions::Motion_16_Sleeping_4752E0;
        field_13C_res_idx = 1;
        const AnimRecord& rec = AO::AnimRec(AnimId::Slog_Sleeping);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, field_184_resources[1]);
    }
    else
    {
        mCurrentMotion = eSlogMotions::Motion_0_Idle_4742E0;
    }
}

Slog::Slog(FP xpos, FP ypos, FP scale)
    : BaseAliveGameObject()
{
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    Init();

    field_116_brain_sub_state = 0;

    field_10C_pTarget = sControlledCharacter_50767C;
    sControlledCharacter_50767C->mBaseGameObjectRefCount++;
    field_17E_asleep = Choice_short::eNo_0;
    field_158_wake_up_anger = 0;

    field_168_anger_switch_id = 0;
    mCurrentMotion = 0;
    field_138_tlvInfo = 0xFFFF;
    field_114_brain_idx = 2;
    field_15A_total_anger = 10;
    field_15C_chase_anger = 20;
}

Slog::~Slog()
{
    if (field_10C_pTarget)
    {
        field_10C_pTarget->mBaseGameObjectRefCount--;
        field_10C_pTarget = nullptr;
    }

    if (field_14C_pSlig)
    {
        field_14C_pSlig->mBaseGameObjectRefCount--;
        field_14C_pSlig = nullptr;
    }

    if (field_16C_pUnknown)
    {
        field_16C_pUnknown->mBaseGameObjectRefCount--;
        field_16C_pUnknown = nullptr;
    }

    if (field_138_tlvInfo != 0xFFFF)
    {
        if (mHealth <= FP_FromInteger(0))
        {
            gMap.TLV_Reset(field_138_tlvInfo, -1, 0, 1);
        }
        else
        {
            gMap.TLV_Reset(field_138_tlvInfo, -1, 0, 0);
        }
    }

    for (s32 i = 0; i < ALIVE_COUNTOF(field_184_resources); i++)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock != field_184_resources[i])
        {
            if (field_184_resources[i])
            {
                ResourceManager::FreeResource_455550(field_184_resources[i]);
                field_184_resources[i] = nullptr;
            }
        }
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);

    if (!field_178_bShot)
    {
        gNumSlogs_9F11C8--;
    }

    MusicController::ClearObject(this);
}

s16 Slog::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    switch (pFrom->mBaseGameObjectTypeId)
    {
        case ReliveTypes::eBullet:
        {
            auto pBullet = static_cast<Bullet*>(pFrom);

            if (pBullet->field_20_x_distance <= FP_FromInteger(0))
            {
                relive_new Blood(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    pBullet->field_1C_ypos,
                    -FP_FromInteger(24),
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    50);
            }
            else
            {
                relive_new Blood(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    pBullet->field_1C_ypos,
                    FP_FromInteger(24),
                    FP_FromInteger(0),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                    50);
            }

            Sfx(9);

            mHealth = FP_FromInteger(0);
            field_114_brain_idx = 3;
            field_134 = 2;
            field_11C_timer = sGnFrame + 90;
            mCurrentMotion = eSlogMotions::Motion_22_Dying_475A90;
            field_13C_res_idx = 3;
            const AnimRecord& rec = AO::AnimRec(AnimId::Slog_Dying);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, field_184_resources[3]);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            gNumSlogs_9F11C8--;
            field_178_bShot = 1;
            return 1;
        }

        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eExplosion:
        {
            Sfx(9);
            mHealth = FP_FromInteger(0);
            relive_new Gibs(
                GibType::Slog_2,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_VelX,
                mBaseAnimatedWithPhysicsGameObject_VelY,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            const PSX_RECT bRect = VGetBoundingRect();
            relive_new Blood(
                FP_FromInteger((bRect.w + bRect.x) / 2),
                FP_FromInteger((bRect.h + bRect.y) / 2),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                50);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return 1;
        }

        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eSlig:
            if (field_17C_res)
            {
                return 1;
            }
            field_17C_res = 1;
            Sfx(9);
            break;

        case ReliveTypes::eZBall:
            return 1;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eRollingBall:
            Slog::Sfx(9);
            [[fallthrough]];

        case ReliveTypes::eElectrocute:
        {
            mHealth = FP_FromInteger(0);
            field_114_brain_idx = 3;
            field_134 = 2;
            field_11C_timer = sGnFrame + 90;
            mCurrentMotion = eSlogMotions::Motion_22_Dying_475A90;
            field_13C_res_idx = 3;
            const AnimRecord& rec = AO::AnimRec(AnimId::Slog_Dying);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, field_184_resources[3]);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            break;
        }

        default:
            Sfx(9);
            break;
    }
    return 1;
}

void Slog::VOnTlvCollision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType32 == TlvTypes::DeathDrop_5)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mHealth = FP_FromInteger(0);
        }
        pTlv = gMap.TLV_Get_At_446060(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    }
}

void Slog::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const s16 old_motion = mCurrentMotion;
    const auto oldBrain = sSlogBrainTable[field_114_brain_idx];
    field_116_brain_sub_state = (this->*sSlogBrainTable[field_114_brain_idx])();

    if (oldBrain != sSlogBrainTable[field_114_brain_idx])
    {
        //LOG_INFO("brain changed from " << GetOriginalFn(oldBrain, sSlogBrainTable).fnName << " to " << GetOriginalFn(sSlog_fns_brain_4CFE00[field_114_brain_idx], sSlogBrainTable).fnName);
    }

    if (showDebugCreatureInfo_5076E0)
    {
        DDCheat::DebugStr("Slog:  Motion=%d  BrainState=%d\n", mCurrentMotion, field_116_brain_sub_state);
    }

    const FP old_x = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP old_y = mBaseAnimatedWithPhysicsGameObject_YPos;
    (this->*sSlogMotionTable_4CFD30[mCurrentMotion])();

    if (old_x != mBaseAnimatedWithPhysicsGameObject_XPos || old_y != mBaseAnimatedWithPhysicsGameObject_YPos)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);

        VOnTlvCollision(BaseAliveGameObjectPathTLV);
    }

    if (old_motion == mCurrentMotion)
    {
        if (field_118_always_zero)
        {
            mCurrentMotion = mPreviousMotion;
            SetAnimFrame();
            field_118_always_zero = 0;
        }
        return;
    }

    SetAnimFrame();

    if (mCurrentMotion == eSlogMotions::Motion_5_Unknown_474070)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
    }
}

void Slog::SetAnimFrame()
{
    u8** ppRes = ResBlockForMotion(mCurrentMotion);
    const AnimRecord& rec = AO::AnimRec(sSlogAnimIdTable_4CFD98[mCurrentMotion]);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, ppRes);
}

u8** Slog::ResBlockForMotion(s16 motion)
{
    if (motion < eSlogMotions::Motion_15_AngryBark_475290)
    {
        field_13C_res_idx = 0;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < eSlogMotions::Motion_19_JumpForwards_475610)
    {
        field_13C_res_idx = 1;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < eSlogMotions::Motion_22_Dying_475A90)
    {
        field_13C_res_idx = 2;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < eSlogMotions::Motion_23_Scratch_475550)
    {
        field_13C_res_idx = 3;
        return field_184_resources[field_13C_res_idx];
    }

    if (motion < 25)
    {
        field_13C_res_idx = 4;
        return field_184_resources[field_13C_res_idx];
    }
    else
    {
        field_13C_res_idx = 0;
        return field_184_resources[field_13C_res_idx];
    }
}

void Slog::MoveOnLine()
{
    const FP xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mBaseAnimatedWithPhysicsGameObject_XPos,
            &mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_VelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (mLiftPoint)
            {
                if (BaseAliveGameObjectCollisionLine->field_8_type != eLineTypes::eDynamicCollision_32 &&
                    BaseAliveGameObjectCollisionLine->field_8_type != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    mLiftPoint->VRemove(this);
                    mLiftPoint->mBaseGameObjectRefCount--;
                    mLiftPoint = nullptr;
                }
            }
            else
            {
                if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes ::eDynamicCollision_32 ||
                    BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    PSX_RECT rect = VGetBoundingRect();
                    rect.y += 5;
                    rect.h += 5;
                    VOnCollisionWith(
                        {rect.x, rect.y},
                        {rect.w, rect.h},
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                }
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            field_12C = FP_FromInteger(0);
            mCurrentMotion = eSlogMotions::Motion_4_Fall_4750C0;
            mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_VelX + xpos;
        }
    }
    else
    {
        field_12C = FP_FromInteger(0);
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        mCurrentMotion = eSlogMotions::Motion_4_Fall_4750C0;
    }
}

const TintEntry sSlogTints_4CFE10[3] = {
    {EReliveLevelIds::eStockYards, 48u, 48u, 48u},
    {EReliveLevelIds::eStockYardsReturn, 48u, 48u, 48u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

void Slog::Init()
{
    field_184_resources[0] = nullptr;
    field_184_resources[1] = nullptr;
    field_184_resources[2] = nullptr;
    field_184_resources[3] = nullptr;
    field_184_resources[4] = nullptr;

    field_184_resources[0] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDogbasicAOResID, 1, 0);
    field_184_resources[1] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDogrstnAOResID, 1, 0);
    field_184_resources[2] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDogattkAOResID, 1, 0);
    field_184_resources[3] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDogknfdAOResID, 1, 0);
    field_184_resources[4] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kDogidleAOResID, 1, 0);

    const AnimRecord& rec = AO::AnimRec(AnimId::Slog_Idle);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_184_resources[0], 1);

    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mBaseAnimatedWithPhysicsGameObject_Anim.mFnPtrArray = kSlog_Anim_Frame_Fns_4CEBF4;
    field_11C_timer = 0;
    field_120 = 0;
    field_124 = -1;
    field_116_brain_sub_state = 0;
    mNextMotion = -1;
    field_EC = 3;
    field_13C_res_idx = 0;
    mBaseGameObjectTypeId = ReliveTypes::eSlog;
    mLiftPoint = nullptr;
    field_118_always_zero = 0;
    field_134 = 2;
    field_17A = 1;
    field_17C_res = 0;
    field_14C_pSlig = 0;
    field_10C_pTarget = 0;

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit6);

    field_178_bShot = 0;
    field_16C_pUnknown = nullptr;

    SetTint(sSlogTints_4CFE10, gMap.mCurrentLevel);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Slog_34;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Slog_Half_15;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }

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
            mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1) ? 0x01 : 0x10)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    MapFollowMe_401D30(FALSE);

    mShadow = relive_new Shadow();

    gNumSlogs_9F11C8++;
}

s16 Slog::ToNextMotion()
{
    switch (mNextMotion)
    {
        case eSlogMotions::Motion_3_TurnAround_474C70:
            mCurrentMotion = eSlogMotions::Motion_3_TurnAround_474C70;
            mNextMotion = -1;
            return 1;

        case eSlogMotions::Motion_1_Walk_4743F0:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            }

            if (!WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(9)))
            {
                mCurrentMotion = eSlogMotions::Motion_9_StartWalking_474690;
                mNextMotion = -1;
                return 1;
            }

            ToIdle();
            return 0;

        case eSlogMotions::Motion_2_Run_4749A0:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
            }

            if (!WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4)))
            {
                mCurrentMotion = eSlogMotions::Motion_2_Run_4749A0;
                mNextMotion = -1;
                return 1;
            }

            ToIdle();
            return 0;
    }
    return 0;
}

void Slog::ToIdle()
{
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit6_FlipY);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit7_SwapXY);

    MapFollowMe_401D30(FALSE);
    field_12C = FP_FromInteger(0);
    ;
    field_130 = 0;
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mCurrentMotion = eSlogMotions::Motion_0_Idle_4742E0;
    mNextMotion = -1;

    field_128 = 60 * Slog_NextRandom() / 256 + sGnFrame + 120;
}

void Slog::ToJump()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (FP_FromDouble(-10.18) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (FP_FromDouble(10.18) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY = (FP_FromInteger(-8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    mCurrentMotion = eSlogMotions::Motion_19_JumpForwards_475610;

    VOnTrapDoorOpen();

    BaseAliveGameObjectCollisionLine = nullptr;

    Sfx(8);

    if (gMap.GetDirection(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_8, this, 0, 0);
    }
}

SfxDefinition sSlogSfx_4CFE40[20] = {
    {0, 12, 38, 30, 0, 0, 0},
    {0, 12, 39, 30, 0, 0, 0},
    {0, 12, 40, 100, -256, 0, 0},
    {0, 12, 41, 60, 0, 0, 0},
    {0, 12, 42, 50, 0, 0, 0},
    {0, 12, 40, 100, -256, 0, 0},
    {0, 12, 44, 90, 0, 0, 0},
    {0, 12, 60, 100, 0, 0, 0},
    {0, 18, 45, 100, -256, 0, 0},
    {0, 18, 46, 127, -127, 127, 0},
    {0, 18, 47, 70, 0, 0, 0},
    {0, 18, 48, 70, 0, 0, 0},
    {0, 18, 49, 30, 0, 0, 0},
    {0, 22, 61, 45, 0, 0, 0},
    {0, 22, 62, 45, 0, 0, 0},
    {0, 3, 59, 67, 0, 0, 0},
    {0, 22, 33, 45, 0, 0, 0},
    {0, 22, 34, 45, 0, 0, 0},
    {0, 22, 35, 45, 0, 0, 0},
    {0, 22, 36, 45, 0, 0, 0}};

void Slog::Sfx(s32 soundId)
{
    s32 volumeLeft = 0;
    s32 volumeRight = 0;

    const SfxDefinition& sndDef = sSlogSfx_4CFE40[static_cast<s32>(soundId)];
    const auto defaultSndIdxVol = sndDef.field_C_default_volume;
    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap.GetDirection(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);
    PSX_RECT worldRect;
    gMap.Get_Camera_World_Rect(direction, &worldRect);
    volumeLeft = volumeRight;
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
        {
            break;
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            FP percentHowFar = (FP_FromInteger(worldRect.w) - mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            break;
        }
        default:
            return;
    }
    SFX_SfxDefinition_Play_477330(&sndDef,
                                  static_cast<s16>(volumeLeft),
                                  static_cast<s16>(volumeRight),
                                  static_cast<s16>(sndDef.field_E_pitch_min),
                                  static_cast<s16>(sndDef.field_10_pitch_max));
}

s16 Slog::IsPlayerNear()
{
    if (FP_Abs(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) >= (FP_FromInteger(100) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) || FP_Abs(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) >= (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) || sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale)
    {
        return 0;
    }
    return 1;
}

BaseAliveGameObject* Slog::FindAbeMudOrSlig()
{
    BaseAliveGameObject* pResult = nullptr;
    FP minDist = FP_FromInteger(gPsxDisplay_504C78.field_0_width);

    PSX_RECT bRect = VGetBoundingRect();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        bRect.x -= gPsxDisplay_504C78.field_0_width;
    }
    else
    {
        bRect.w += gPsxDisplay_504C78.field_0_width;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != field_14C_pSlig && pObj != this)
        {
            if (pObj->mBaseGameObjectTypeId == ReliveTypes::eAbe || pObj->mBaseGameObjectTypeId == ReliveTypes::eMudokon || pObj->mBaseGameObjectTypeId == ReliveTypes::eSlig)
            {
                const PSX_RECT objRect = pObj->VGetBoundingRect();

                if (objRect.x <= bRect.w
                    && objRect.w >= bRect.x
                    && objRect.h >= bRect.y
                    && objRect.y <= bRect.h)
                {
                    const FP xd = FP_Abs(pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos);
                    if (xd < minDist)
                    {
                        pResult = pObj;
                        minDist = xd;
                    }
                }
            }
        }
    }
    return pResult;
}

void Slog::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel
        || gMap.mCurrentPath != gMap.mPath
        || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        if (field_10C_pTarget)
        {
            field_10C_pTarget->mBaseGameObjectRefCount--;
            field_10C_pTarget = nullptr;
        }

        if (field_14C_pSlig)
        {
            field_14C_pSlig->mBaseGameObjectRefCount--;
            field_14C_pSlig = nullptr;
        }

        if (field_16C_pUnknown)
        {
            field_16C_pUnknown->mBaseGameObjectRefCount--;
            field_16C_pUnknown = nullptr;
        }
    }
}

void Slog::VRender(PrimHeader** ppOt)
{
    if (mBaseGameObjectUpdateDelay == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Slog::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    field_156 += field_15C_chase_anger;
}

s16 Slog::HandleEnemyStopper()
{
    FP xpos = {};
    if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
    {
        xpos = mBaseAnimatedWithPhysicsGameObject_XPos + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2));
    }
    else
    {
        xpos = mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2));
    }

    auto pStopper = static_cast<Path_EnemyStopper*>(gMap.TLV_Get_At_446260(
        FP_GetExponent(xpos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(xpos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::EnemyStopper_79));

    if (!pStopper)
    {
        return 0;
    }

    if (pStopper->field_18_direction != (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) ? Path_EnemyStopper::StopDirection::Right_1 : Path_EnemyStopper::StopDirection::Left_0))
    {
        return 0;
    }

    if (!SwitchStates_Get(pStopper->field_1A_switch_id))
    {
        return 1;
    }

    return 0;
}

void Slog::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }
}

void Slog::Motion_0_Idle_4742E0()
{
    if (!ToNextMotion())
    {
        if (mNextMotion == eSlogMotions::Motion_19_JumpForwards_475610)
        {
            ToJump();
        }
        else if (mNextMotion == -1)
        {
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                        mBaseAnimatedWithPhysicsGameObject_PathNumber,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        0))
                {
                    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_17, 1, 0);
                }

                if (gMap.GetDirection(
                        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                        mBaseAnimatedWithPhysicsGameObject_PathNumber,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos)
                        >= CameraPos::eCamCurrent_0
                    && gMap.GetDirection(
                           mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                           mBaseAnimatedWithPhysicsGameObject_PathNumber,
                           mBaseAnimatedWithPhysicsGameObject_XPos,
                           mBaseAnimatedWithPhysicsGameObject_YPos)
                           >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::GetAbmientAndMusicInfo(nullptr, nullptr, nullptr) == MusicController::MusicTypes::eSlogChaseTension)
                    {
                        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChaseTension, this, 0, 0);
                    }
                    else
                    {
                        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogTension, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
        }
    }
}

const FP sSlogWalkVelXTable_4BCC28[18] = {
    FP_FromDouble(1.3329315185546875),
    FP_FromDouble(2.4870452880859375),
    FP_FromDouble(2.537445068359375),
    FP_FromDouble(1.974761962890625),
    FP_FromDouble(3.015594482421875),
    FP_FromDouble(3.10400390625),
    FP_FromDouble(2.758575439453125),
    FP_FromDouble(3.1327056884765625),
    FP_FromDouble(3.646148681640625),
    FP_FromDouble(2.3465118408203125),
    FP_FromDouble(2.222015380859375),
    FP_FromDouble(2.1777801513671875),
    FP_FromDouble(2.2137908935546875),
    FP_FromDouble(2.0020904541015625),
    FP_FromDouble(2.413543701171875),
    FP_FromDouble(2.185516357421875),
    FP_FromDouble(4.1008453369140625),
    FP_FromDouble(4.35284423828125)};

void Slog::Motion_1_Walk_4743F0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -sSlogWalkVelXTable_4BCC28[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * sSlogWalkVelXTable_4BCC28[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }

    if (WallHit_401930(
            mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20),
            mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle();

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eSlogMotions::Motion_1_Walk_4743F0)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 11)
            {
                if (mNextMotion == eSlogMotions::Motion_0_Idle_4742E0)
                {
                    mCurrentMotion = eSlogMotions::Motion_10_EndWalking_4747D0;
                    mNextMotion = -1;
                }

                if (mNextMotion == eSlogMotions::Motion_3_TurnAround_474C70)
                {
                    ToIdle();
                    mCurrentMotion = eSlogMotions::Motion_3_TurnAround_474C70;
                }
            }
            else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 14)
            {
                if (!field_126_movedOffScreen)
                {
                    field_126_movedOffScreen = 1;
                    MapFollowMe_401D30(FALSE);
                }

                if (mNextMotion == eSlogMotions::Motion_2_Run_4749A0)
                {
                    mCurrentMotion = eSlogMotions::Motion_2_Run_4749A0;
                    mNextMotion = -1;
                }
            }
            else
            {
                field_126_movedOffScreen = 0;
            }
        }
    }
}

const FP sSlogRunVelXTable_4BCC70[9] = {
    FP_FromDouble(8.625900268554688),
    FP_FromDouble(3.38677978515625),
    FP_FromDouble(3.5192413330078125),
    FP_FromDouble(5.1022491455078125),
    FP_FromDouble(6.7406005859375),
    FP_FromDouble(7.8218231201171875),
    FP_FromDouble(8.39404296875),
    FP_FromDouble(6.532867431640625),
    FP_FromInteger(32000)};

void Slog::Motion_2_Run_4749A0()
{
    if (gMap.GetDirection(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_8, this, 0, 0);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -sSlogRunVelXTable_4BCC70[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * sSlogRunVelXTable_4BCC70[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eSlogMotions::Motion_2_Run_4749A0)
        {
            if (Slog_NextRandom() < 35u)
            {
                Sfx(5);
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
            {
                Sfx(16);
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
            {
                if (!field_126_movedOffScreen)
                {
                    field_126_movedOffScreen = 1;
                    MapFollowMe_401D30(FALSE);
                }

                if (mNextMotion == eSlogMotions::Motion_0_Idle_4742E0)
                {
                    mCurrentMotion = eSlogMotions::Motion_7_SlideTurn_474DB0;
                    mNextMotion = -1;
                }
                else if (mNextMotion == eSlogMotions::Motion_19_JumpForwards_475610)
                {
                    ToJump();
                    mNextMotion = -1;
                }
                else if (mNextMotion != -1)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = -1;
                }
            }
            else
            {
                field_126_movedOffScreen = 0;
            }
        }
    }
}

void Slog::Motion_3_TurnAround_474C70()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle();
    }
}

void Slog::Motion_4_Fall_4750C0()
{
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX -= mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_12C;

        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX += mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_12C;
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};

    if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                MapFollowMe_401D30(FALSE);

                if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes ::eDynamicCollision_32 ||
                    BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) + 5;
                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                }
                mCurrentMotion = eSlogMotions::Motion_11_Land_475AB0;
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX - mBaseAnimatedWithPhysicsGameObject_VelX;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                MapFollowMe_401D30(FALSE);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Slog::Motion_5_Unknown_474070()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        mCurrentMotion = mPreviousMotion;
        if (mLiftPoint)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((BaseAliveGameObjectCollisionLine->field_0_rect.x + BaseAliveGameObjectCollisionLine->field_0_rect.w) / 2);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->field_0_rect.y);
        }
    }
}

void Slog::Motion_6_MoveHeadUpwards_474220()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(2);
        field_144 = 1;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange(0, 100) < 30)
        {
            Sfx(4);
        }

        ToIdle();
    }
}

const FP sSlogSlideTurnVelXTable_4BCC98[11] = {
    FP_FromDouble(3.468994140625),
    FP_FromDouble(4.5489959716796875),
    FP_FromDouble(8.642990112304688),
    FP_FromDouble(9.31298828125),
    FP_FromDouble(6.4949951171875),
    FP_FromDouble(4.631988525390625),
    FP_FromDouble(3.9169921875),
    FP_FromDouble(4.334991455078125),
    FP_FromDouble(3.6609954833984375),
    FP_FromDouble(1.2819976806640625),
    FP_FromDouble(32000)};

void Slog::Motion_7_SlideTurn_474DB0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(13);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -sSlogSlideTurnVelXTable_4BCC98[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * sSlogSlideTurnVelXTable_4BCC98[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }


    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX) || mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();
    }
}

const FP sSlogStopRunningVelX_4BCCC8[] = {
    FP_FromDouble(17.197998046875),
    FP_FromDouble(11.907989501953125),
    FP_FromDouble(8.52899169921875),
    FP_FromDouble(7.33599853515625),
    FP_FromDouble(4.168212890625),
    FP_FromDouble(5.3128204345703125),
    FP_FromDouble(3.81121826171875),
    FP_FromDouble(1.503692626953125),
    FP_FromDouble(0.5045166015625),
    FP_FromDouble(-0.2426605224609375),
    FP_FromDouble(-0.5961456298828125),
    FP_FromDouble(-1.2430877685546875),
    FP_FromDouble(-4.89166259765625),
    FP_FromDouble(32000)};

void Slog::Motion_8_StopRunning_474EC0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -sSlogStopRunningVelX_4BCCC8[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * sSlogStopRunningVelX_4BCCC8[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eSlogMotions::Motion_8_StopRunning_474EC0)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_401D30(FALSE);

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
                    mCurrentMotion = eSlogMotions::Motion_2_Run_4749A0;
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    mCurrentMotion = eSlogMotions::Motion_2_Run_4749A0;
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                }
            }
        }
    }
}

void Slog::Motion_9_StartWalking_474690()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSlogMotions::Motion_1_Walk_4743F0;
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle();

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
    }
    else
    {
        MoveOnLine();
    }
}

void Slog::Motion_10_EndWalking_4747D0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        MapFollowMe_401D30(0);

        if (!ToNextMotion())
        {
            ToIdle();
        }
    }

    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        ToIdle();

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
    }
    else
    {
        MoveOnLine();
    }
}

void Slog::Motion_11_Land_475AB0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(15);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_12_Unknown_475B50()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mCurrentMotion = eSlogMotions::Motion_13_StartFastBarking_475B70;
    }
}

void Slog::Motion_13_StartFastBarking_475B70()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(2);
        field_144 = 1;
    }

    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = eSlogMotions::Motion_14_EndFastBarking_475BB0;
        }
    }
}

void Slog::Motion_14_EndFastBarking_475BB0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mCurrentMotion = eSlogMotions::Motion_0_Idle_4742E0;
    }
}

void Slog::Motion_15_AngryBark_475290()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 6)
    {
        Sfx(5);
    }

    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
        }
    }
}

void Slog::Motion_16_Sleeping_4752E0()
{
    if (mNextMotion != -1 && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
        return;
    }

    bool bSpawnParticle = false;
    if (sGnFrame % 60)
    {
        if (!((sGnFrame - 20) % 60))
        {
            Sfx(11);
            if (gMap.Is_Point_In_Current_Camera(
                    mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                    mBaseAnimatedWithPhysicsGameObject_PathNumber,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    0))
            {
                bSpawnParticle = true;
            }
        }
    }
    else
    {
        Sfx(10);
        if (gMap.Is_Point_In_Current_Camera(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            bSpawnParticle = true;
        }
    }

    if (bSpawnParticle)
    {
        relive_new SnoozeParticle(
            mBaseAnimatedWithPhysicsGameObject_XPos
                + ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(-18) : FP_FromInteger(18)),
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(13),
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer,
            mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale);
    }
}

void Slog::Motion_17_MoveHeadDownwards_475510()
{
    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
        }
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
}

void Slog::Motion_18_WakeUp_475460()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectTypeId == ReliveTypes::eSnoozParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->field_1D4_state = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
            Sfx(4);
        }
    }

    if (gMap.GetDirection(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogTension, this, 0, 0);
    }
}

void Slog::Motion_19_JumpForwards_475610()
{
    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8));

    if (mBaseAnimatedWithPhysicsGameObject_VelY > (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20));
    }

    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP ypos1 = mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20));

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            oldXPos,
            ypos1,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 6 : 0x60)
        == 1)
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackgroundWallLeft_5:
                if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
                    mBaseAnimatedWithPhysicsGameObject_XPos = oldXPos;
                }
                break;

            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallRight_6:
                if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
                    mBaseAnimatedWithPhysicsGameObject_XPos = oldXPos;
                }
                break;
            default:
                break;
        }
    }

    if (sCollisions->Raycast(
            oldXPos,
            ypos1,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 1 : 0x10))
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    mNextMotion = -1;
                    mCurrentMotion = eSlogMotions::Motion_2_Run_4749A0;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                }
                break;
            default:
                break;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(2))
    {
        field_12C = FP_FromInteger(0);
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        mCurrentMotion = eSlogMotions::Motion_4_Fall_4750C0;
    }
}

void Slog::Motion_20_JumpUpwards_475890()
{
    if (gMap.GetDirection(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChaseTension, this, 0, 0);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5)
    {
        Sfx(6);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mCurrentMotion = eSlogMotions::Motion_0_Idle_4742E0;
        mNextMotion = -1;
    }
}

void Slog::Motion_21_Eating_475900()
{
    SND_Seq_Stop_477A60(SeqId::Unknown_17);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame != 0
        || (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards),
            mNextMotion == -1)
        || mNextMotion == eSlogMotions::Motion_21_Eating_475900)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3 && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit19_LoopBackwards))
        {
            SfxPlayMono(static_cast<SoundEffect>(Math_RandomRange(SoundEffect::Eating1_79, SoundEffect::Eating2_80)), 100, 0);
            const FP bloodYPos = mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(4) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            const FP bloodXPos = ((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -FP_FromInteger(25) : FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            relive_new Blood(
                bloodXPos + mBaseAnimatedWithPhysicsGameObject_XPos,
                bloodYPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                12);
        }

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (Math_RandomRange(0, 100) < 85)
            {
                if (static_cast<s32>(sGnFrame) > field_164_timer && Math_RandomRange(0, 100) < 60)
                {
                    field_164_timer = sGnFrame + 16;
                    Sfx(3);
                }
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit19_LoopBackwards);
            }
        }

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards);
        }
    }
    else
    {
        mCurrentMotion = eSlogMotions::Motion_0_Idle_4742E0;
    }
}

void Slog::Motion_22_Dying_475A90()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_4_Fall_4750C0();
        mCurrentMotion = eSlogMotions::Motion_22_Dying_475A90;
    }
}

void Slog::Motion_23_Scratch_475550()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_16, 1, 1);
    }

    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
        }
    }
}

void Slog::Motion_24_Growl_475590()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3)
    {
        if (field_144)
        {
            Sfx(3);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
            field_144 = 0;
            field_164_timer = sGnFrame + 12;
        }
    }

    if (static_cast<s32>(sGnFrame) > field_164_timer)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
    }

    if (mNextMotion != -1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
        }
    }
}

const s16 sSlogResponseMotion_4CFCF0[3][10] = {
    {eSlogMotions::Motion_3_TurnAround_474C70,
     eSlogMotions::Motion_2_Run_4749A0,
     eSlogMotions::Motion_8_StopRunning_474EC0,
     -1,
     eSlogMotions::Motion_8_StopRunning_474EC0,
     -1,
     eSlogMotions::Motion_7_SlideTurn_474DB0,
     -2,
     0,
     0},
    {eSlogMotions::Motion_13_StartFastBarking_475B70,
     eSlogMotions::Motion_2_Run_4749A0,
     eSlogMotions::Motion_7_SlideTurn_474DB0,
     -2,
     0,
     0,
     0,
     0,
     0,
     0},
    {eSlogMotions::Motion_13_StartFastBarking_475B70,
     -2,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0}};

s16 Slog::Brain_0_ListeningToSlig_472450()
{
    if (!field_14C_pSlig)
    {
        return 99;
    }

    if (field_14C_pSlig->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_14C_pSlig->mBaseGameObjectRefCount--;
        field_14C_pSlig = nullptr;

        if (field_10C_pTarget)
        {
            field_10C_pTarget->mBaseGameObjectRefCount--;
            field_10C_pTarget = nullptr;
        }

        field_156 = 0;
        field_114_brain_idx = 1;
        return 0;
    }

    FP gridScale = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    FP scaled1Directed = (gridScale * FP_FromInteger(1));
    if (field_14C_pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        scaled1Directed = -scaled1Directed;
    }

    u16 result = 0;
    const FP xSkip = CamX_VoidSkipper(scaled1Directed + field_14C_pSlig->mBaseAnimatedWithPhysicsGameObject_XPos, scaled1Directed, 0, &result);

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;
    switch (field_116_brain_sub_state)
    {
        case 0:
            mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
            field_150_waiting_counter = 0;
            field_11C_timer = sGnFrame + 15;
            field_134 = 0;
            return 1;

        case 1:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                return field_116_brain_sub_state;
            }

            if (!Facing(xSkip))
            {
                mNextMotion = eSlogMotions::Motion_3_TurnAround_474C70;
                return field_116_brain_sub_state;
            }

            if (static_cast<s32>(sGnFrame) <= field_11C_timer)
            {
                return field_116_brain_sub_state;
            }

            mNextMotion = eSlogMotions::Motion_6_MoveHeadUpwards_474220;
            return 2;

        case 2:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                return field_116_brain_sub_state;
            }

            if (field_148 == pEventSystem_4FF954->field_18_last_event_index)
            {
                if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
                {
                    speak = GameSpeakEvents::eNone_m1;
                }
                else
                {
                    speak = GameSpeakEvents::eSameAsLast_m2;
                }
            }
            else
            {
                field_148 = pEventSystem_4FF954->field_18_last_event_index;
                speak = pEventSystem_4FF954->field_10_last_event;
            }
            // Down to game speak handler
            break;

        case 3:
            if (mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                return 2;
            }

            if (mCurrentMotion != eSlogMotions::Motion_1_Walk_4743F0)
            {
                return field_116_brain_sub_state;
            }

            if (!Facing(xSkip))
            {
                mNextMotion = eSlogMotions::Motion_3_TurnAround_474C70;
                return 2;
            }

            if (FP_Abs(xSkip - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
            {
                mNextMotion = eSlogMotions::Motion_2_Run_4749A0;
                return 4;
            }

            if (FP_Abs(xSkip - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(1)))
            {
                return field_116_brain_sub_state;
            }

            mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
            return 2;

        case 4:
            if (mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                return 2;
            }

            if (mCurrentMotion != eSlogMotions::Motion_2_Run_4749A0)
            {
                return field_116_brain_sub_state;
            }

            if (!Facing(xSkip))
            {
                mNextMotion = eSlogMotions::Motion_8_StopRunning_474EC0;
                return field_116_brain_sub_state;
            }

            if (FP_Abs(xSkip - mBaseAnimatedWithPhysicsGameObject_XPos) <= (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3)))
            {
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
                return 2;
            }

            return field_116_brain_sub_state;

        case 5:
            if (static_cast<s32>(sGnFrame) <= field_11C_timer)
            {
                return field_116_brain_sub_state;
            }
            field_150_waiting_counter--;
            mNextMotion = eSlogMotions::Motion_6_MoveHeadUpwards_474220;
            return 2;

        case 6:
            if (static_cast<s32>(sGnFrame) <= field_11C_timer)
            {
                return field_116_brain_sub_state;
            }

            if (field_180)
            {
                return 2;
            }

            field_11C_timer = sGnFrame + 10;
            mNextMotion = sSlogResponseMotion_4CFCF0[field_152_response_index][field_154_response_part++];

            if (mNextMotion == -2)
            {
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
                return 2;
            }
            return field_116_brain_sub_state;

        default:
            return field_116_brain_sub_state;
    }

    switch (speak)
    {
        case GameSpeakEvents::Slig_LookOut_6:
            DelayedResponse(2);
            return 6;

        case GameSpeakEvents::Slig_Hi_23:
            field_150_waiting_counter++;
            if (static_cast<s32>(sGnFrame) % 2)
            {
                field_150_waiting_counter++;
            }
            [[fallthrough]];
        case GameSpeakEvents::Slig_HereBoy_24:
            field_150_waiting_counter++;
            field_11C_timer = sGnFrame - (Math_NextRandom() % 8) + 15;
            break;

        case GameSpeakEvents::eUnknown_25:
        {
            field_10C_pTarget = FindAbeMudOrSlig();
            if (field_10C_pTarget)
            {
                field_10C_pTarget->mBaseGameObjectRefCount++;

                field_14C_pSlig->mBaseGameObjectRefCount--;
                field_14C_pSlig = nullptr;

                field_114_brain_idx = 2;
                return 0;
            }

            FP gridSize = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            if (!WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize))
            {
                DelayedResponse(1);
                return 6;
            }
            break;
        }

        case GameSpeakEvents::eUnknown_29:
            DelayedResponse(0);
            return 6;

        default:
            break;
    }


    if (!(Math_NextRandom() % 128))
    {
        field_150_waiting_counter++;
    }

    if (field_150_waiting_counter)
    {
        field_11C_timer += Math_NextRandom() % 16;
        return 5;
    }

    if (FP_Abs(xSkip - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
    {
        if (!Facing(xSkip))
        {
            mNextMotion = eSlogMotions::Motion_3_TurnAround_474C70;
            return field_116_brain_sub_state;
        }

        FP gridSize2 = {};
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize2 = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
        else
        {
            gridSize2 = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }

        if (!WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize2))
        {
            mNextMotion = eSlogMotions::Motion_2_Run_4749A0;
            return 4;
        }
    }

    if (FP_Abs(xSkip - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(1)))
    {
        if (!Facing(xSkip))
        {
            mNextMotion = eSlogMotions::Motion_3_TurnAround_474C70;
            return field_116_brain_sub_state;
        }

        FP gridSize3 = {};
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize3 = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
        else
        {
            gridSize3 = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }

        if (!WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize3))
        {
            mNextMotion = eSlogMotions::Motion_1_Walk_4743F0;
            return 3;
        }
    }

    if (field_14C_pSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return eSlogMotions::Motion_3_TurnAround_474C70;
    }

    return field_116_brain_sub_state;
}

s16 Slog::Brain_1_Idle_4719C0()
{
    if (field_10C_pTarget)
    {
        if (field_10C_pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_10C_pTarget->mBaseGameObjectRefCount--;
            field_10C_pTarget = nullptr;
        }
    }

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    if (field_148 == pEventSystem_4FF954->field_18_last_event_index)
    {
        if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
        {
            speak = GameSpeakEvents::eNone_m1;
        }
        else
        {
            speak = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        speak = pEventSystem_4FF954->field_10_last_event;
        field_148 = pEventSystem_4FF954->field_18_last_event_index;
    }

    if (speak == GameSpeakEvents::Slig_HereBoy_24)
    {
        // Listen to slig
        field_114_brain_idx = 0;
        field_14C_pSlig = sControlledCharacter_50767C;
        field_14C_pSlig->mBaseGameObjectRefCount++;

        if (field_10C_pTarget)
        {
            field_10C_pTarget = nullptr;
            field_10C_pTarget->mBaseGameObjectRefCount--;
        }
        return 0;
    }

    if (SwitchStates_Get(field_168_anger_switch_id))
    {
        // Slog triggered, not gonna listen to you anymore, you gotta die
        field_114_brain_idx = 2;
        return 0;
    }

    switch (field_116_brain_sub_state)
    {
        case 0:
            if (mCurrentMotion != eSlogMotions::Motion_16_Sleeping_4752E0 && mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
                return field_116_brain_sub_state;
            }
            else if (field_17E_asleep == Choice_short::eYes_1)
            {
                field_156 = 0;
                return 1;
            }
            field_156 = field_158_wake_up_anger;
            return 4;

        case 1:
            if (IsEventInRange(
                    kEventSuspiciousNoise,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                field_156++;
            }

            if (IsEventInRange(
                    kEventSpeaking,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                field_156 += (Slog_NextRandom() % 8) + 15;
            }

            if (!(sGnFrame % 16))
            {
                if (field_156)
                {
                    field_156--;
                }
            }

            if (field_156 <= field_158_wake_up_anger)
            {
                return field_116_brain_sub_state;
            }
            mNextMotion = eSlogMotions::Motion_18_WakeUp_475460;
            return 2;

        case 2:
            if (mCurrentMotion != eSlogMotions::Motion_18_WakeUp_475460)
            {
                return field_116_brain_sub_state;
            }
            mNextMotion = eSlogMotions::Motion_6_MoveHeadUpwards_474220;
            field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
            field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
            return 4;

        case 3:
            if (mCurrentMotion != eSlogMotions::Motion_17_MoveHeadDownwards_475510)
            {
                return field_116_brain_sub_state;
            }
            mNextMotion = eSlogMotions::Motion_16_Sleeping_4752E0;
            return 1;

        case 4:
            if (IsEventInRange(
                    kEventSuspiciousNoise,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                field_156++;
            }

            if (IsEventInRange(
                    kEventSpeaking,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                field_156 += (Slog_NextRandom() % 8) + 15;
            }

            if (!(sGnFrame % 32))
            {
                if (field_156)
                {
                    if (field_17E_asleep == Choice_short::eYes_1)
                    {
                        field_156--;
                    }
                }
            }

            if (IsPlayerNear())
            {
                field_156 += 2;
            }

            if (!(Slog_NextRandom() % 64) && mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                mCurrentMotion = eSlogMotions::Motion_6_MoveHeadUpwards_474220;
                return field_116_brain_sub_state;
            }

            if (static_cast<s32>(sGnFrame) > field_164_timer && mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                mCurrentMotion = eSlogMotions::Motion_24_Growl_475590;
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;

                field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
            }

            if (static_cast<s32>(sGnFrame) > field_160 && mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                mCurrentMotion = eSlogMotions::Motion_23_Scratch_475550;
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;

                field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
            }

            if (field_156 > field_15A_total_anger)
            {
                mNextMotion = eSlogMotions::Motion_15_AngryBark_475290;
                field_156 += Slog_NextRandom() % 8;
                return 5;
            }

            if (field_156 >= field_158_wake_up_anger)
            {
                return field_116_brain_sub_state;
            }
            mNextMotion = eSlogMotions::Motion_17_MoveHeadDownwards_475510;
            return 3;

        case 5:
            if (IsEventInRange(
                    kEventSuspiciousNoise,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                field_156++;
            }

            if (IsEventInRange(
                    kEventSpeaking,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                field_156 += (Slog_NextRandom() % 8) + 15;
            }

            if (!(sGnFrame % 2))
            {
                if (field_156)
                {
                    field_156--;
                }
            }

            if (IsPlayerNear())
            {
                field_156 += 2;
            }

            if (field_156 >= field_15A_total_anger)
            {
                if (field_156 > field_15C_chase_anger)
                {
                    field_114_brain_idx = 2;
                    return 0;
                }
                return field_116_brain_sub_state;
            }

            mCurrentMotion = eSlogMotions::Motion_0_Idle_4742E0;
            field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
            field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
            return 4;

        default:
            return field_116_brain_sub_state;
    }
}

s16 Slog::Brain_2_ChasingAbe_470F50()
{
    const s32 lastIdx = pEventSystem_4FF954->field_18_last_event_index;

    GameSpeakEvents speak = GameSpeakEvents::eNone_m1;

    if (field_148 == lastIdx)
    {
        if (pEventSystem_4FF954->field_10_last_event == GameSpeakEvents::eNone_m1)
        {
            speak = GameSpeakEvents::eNone_m1;
        }
        else
        {
            speak = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_148 = lastIdx;
        speak = pEventSystem_4FF954->field_10_last_event;
    }

    if (field_176 && speak == GameSpeakEvents::Slig_HereBoy_24)
    {
        // Listen to slig
        field_114_brain_idx = 0;
        field_14C_pSlig = sControlledCharacter_50767C;
        field_14C_pSlig->mBaseGameObjectRefCount++;

        if (field_10C_pTarget)
        {
            field_10C_pTarget->mBaseGameObjectRefCount--;
            field_10C_pTarget = nullptr;
        }
        return 0;
    }

    if (field_10C_pTarget && field_10C_pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        // Idle
        field_10C_pTarget->mBaseGameObjectRefCount--;
        field_10C_pTarget = nullptr;
        field_156 = 0;
        field_114_brain_idx = 1;
        mNextMotion = 0;
        return 0;
    }

    switch (field_116_brain_sub_state)
    {
        case 0:
            if (!field_10C_pTarget)
            {
                field_10C_pTarget = sActiveHero_507678;
                field_10C_pTarget->mBaseGameObjectRefCount++;
            }
            field_110 = 0;
            field_134 = 0;
            field_172 = 0;
            field_16C_pUnknown = nullptr;
            mNextMotion = eSlogMotions::Motion_2_Run_4749A0;
            Sfx(8);
            return 1;

        case 1:
        {
            if ((mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0) && HandleEnemyStopper()) || WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                mNextMotion = eSlogMotions::Motion_7_SlideTurn_474DB0;
                field_174 = mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0);
                field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
                field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
                return 13;
            }

            if (!VIsFacingMe(field_10C_pTarget) && mCurrentMotion == eSlogMotions::Motion_2_Run_4749A0)
            {
                mNextMotion = eSlogMotions::Motion_8_StopRunning_474EC0;
            }

            if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), field_10C_pTarget))
            {
                if (VOnSameYLevel(field_10C_pTarget))
                {
                    if (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                    {
                        if (VIsFacingMe(field_10C_pTarget))
                        {
                            mNextMotion = eSlogMotions::Motion_19_JumpForwards_475610;
                        }
                    }
                }
            }

            if (mCurrentMotion == eSlogMotions::Motion_8_StopRunning_474EC0)
            {
                mNextMotion = eSlogMotions::Motion_2_Run_4749A0;
            }

            if (mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                if (VIsFacingMe(field_10C_pTarget))
                {
                    FP scaleDirected = {};
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        scaleDirected = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    }
                    else
                    {
                        scaleDirected = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    }

                    if (WallHit_401930(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), FP_FromInteger(2) * scaleDirected))
                    {
                        field_174 = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
                        field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
                        field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
                        return 13;
                    }
                    mNextMotion = eSlogMotions::Motion_2_Run_4749A0;
                }
                else
                {
                    mCurrentMotion = eSlogMotions::Motion_3_TurnAround_474C70;
                }
            }

            if (mCurrentMotion == eSlogMotions::Motion_4_Fall_4750C0)
            {
                return 8;
            }

            if (mCurrentMotion == eSlogMotions::Motion_19_JumpForwards_475610)
            {
                return 3;
            }

            const FP k10Scaled = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10);
            if (mBaseAnimatedWithPhysicsGameObject_YPos <= field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_YPos + k10Scaled)
            {
                if (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                {
                    if (mBaseAnimatedWithPhysicsGameObject_YPos >= (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_YPos - (k10Scaled * FP_FromInteger(3))))
                    {
                        return field_116_brain_sub_state;
                    }
                }
            }
            else
            {
                if (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                {
                    return 9;
                }
            }

            field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
            field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
            return 2;
        }

        case 2:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
                return field_116_brain_sub_state;
            }

            if ((Slog_NextRandom() % 64) == 0)
            {
                mCurrentMotion = eSlogMotions::Motion_6_MoveHeadUpwards_474220;
                return field_116_brain_sub_state;
            }

            if (static_cast<s32>(sGnFrame) > field_164_timer)
            {
                field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;

                mCurrentMotion = eSlogMotions::Motion_24_Growl_475590;
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
            }

            if (static_cast<s32>(sGnFrame) > field_160)
            {
                field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;

                mCurrentMotion = eSlogMotions::Motion_23_Scratch_475550;
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
            }

            if (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale)
            {
                return field_116_brain_sub_state;
            }
            return 1;

        case 3:
            if (mCurrentMotion == eSlogMotions::Motion_2_Run_4749A0)
            {
                if (!field_110)
                {
                    return 1;
                }

                if (VIsObjNearby(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), field_10C_pTarget) || VIsFacingMe(field_10C_pTarget))
                {
                    mCurrentMotion = eSlogMotions::Motion_21_Eating_475900;
                    mNextMotion = -1;
                }
                else
                {
                    mCurrentMotion = eSlogMotions::Motion_3_TurnAround_474C70;
                    mNextMotion = eSlogMotions::Motion_21_Eating_475900;
                }
                field_11C_timer = sGnFrame + 90;
                return 6;
            }

            if (mCurrentMotion != eSlogMotions::Motion_4_Fall_4750C0)
            {
                return field_116_brain_sub_state;
            }

            return 8;

        case 6:
            if (static_cast<s32>(sGnFrame) <= field_11C_timer)
            {
                if (mCurrentMotion)
                {
                    return field_116_brain_sub_state;
                }
                mCurrentMotion = eSlogMotions::Motion_21_Eating_475900;
                return field_116_brain_sub_state;
            }

            mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;
            return 7;

        case 7:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                return field_116_brain_sub_state;
            }
            field_156 = 0;
            field_114_brain_idx = 1;
            return 0;

        case 8:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                return field_116_brain_sub_state;
            }
            mCurrentMotion = eSlogMotions::Motion_2_Run_4749A0;
            return 1;

        case 9:
            if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && HandleEnemyStopper())
            {
                mNextMotion = eSlogMotions::Motion_7_SlideTurn_474DB0;
                field_174 = mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0);
                field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
                field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
                return 13;
            }

            if (!VIsFacingMe(field_10C_pTarget) && mCurrentMotion == eSlogMotions::Motion_2_Run_4749A0)
            {
                mNextMotion = eSlogMotions::Motion_8_StopRunning_474EC0;
            }

            if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), field_10C_pTarget))
            {
                if (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale && mCurrentMotion == eSlogMotions::Motion_2_Run_4749A0)
                {
                    if (VIsFacingMe(field_10C_pTarget))
                    {
                        mNextMotion = eSlogMotions::Motion_7_SlideTurn_474DB0;
                    }
                }
            }

            if (mCurrentMotion == eSlogMotions::Motion_8_StopRunning_474EC0)
            {
                mNextMotion = eSlogMotions::Motion_2_Run_4749A0;
            }

            if (mBaseAnimatedWithPhysicsGameObject_YPos < ((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10)) + field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_YPos))
            {
                field_11C_timer = sGnFrame + field_170;
                return 11;
            }

            if (mCurrentMotion == eSlogMotions::Motion_4_Fall_4750C0)
            {
                return 8;
            }

            if (mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                if (VIsFacingMe(field_10C_pTarget))
                {
                    if (field_172 >= 100)
                    {
                        if (Math_RandomRange(0, 100) < 20)
                        {
                            mCurrentMotion = eSlogMotions::Motion_24_Growl_475590;
                        }
                        mNextMotion = eSlogMotions::Motion_6_MoveHeadUpwards_474220;
                        return 12;
                    }
                    else
                    {
                        mNextMotion = eSlogMotions::Motion_20_JumpUpwards_475890;
                        return 10;
                    }
                }
                else
                {
                    mNextMotion = eSlogMotions::Motion_3_TurnAround_474C70;
                }
                return field_116_brain_sub_state;
            }
            return field_116_brain_sub_state;

        case 10:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                return field_116_brain_sub_state;
            }

            field_172 += Slog_NextRandom() % 64;
            return 9;

        case 11:
            if (mCurrentMotion == eSlogMotions::Motion_0_Idle_4742E0)
            {
                if (!VIsFacingMe(field_10C_pTarget))
                {
                    mNextMotion = eSlogMotions::Motion_3_TurnAround_474C70;
                    return field_116_brain_sub_state;
                }

                mNextMotion = eSlogMotions::Motion_6_MoveHeadUpwards_474220;
            }

            if (static_cast<s32>(sGnFrame) <= field_11C_timer)
            {
                return field_116_brain_sub_state;
            }
            return 1;

        case 12:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                return field_116_brain_sub_state;
            }
            field_172 -= 20;
            return 9;

        case 13:
            if (mCurrentMotion != eSlogMotions::Motion_0_Idle_4742E0)
            {
                return field_116_brain_sub_state;
            }

            if (field_174)
            {
                if (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    return 1;
                }
            }
            else
            {
                if (field_10C_pTarget->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    return 1;
                }
            }

            if (static_cast<s32>(sGnFrame) > field_164_timer)
            {
                mCurrentMotion = eSlogMotions::Motion_24_Growl_475590;
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;

                field_164_timer = (Math_NextRandom() % 32) + sGnFrame + 60;
            }

            if (static_cast<s32>(sGnFrame) > field_160)
            {
                mCurrentMotion = eSlogMotions::Motion_23_Scratch_475550;
                mNextMotion = eSlogMotions::Motion_0_Idle_4742E0;

                field_160 = (Math_NextRandom() % 32) + sGnFrame + 120;
            }

            return field_116_brain_sub_state;

        default:
            break;
    }

    return field_116_brain_sub_state;
}

s16 Slog::Brain_3_Dead_4721B0()
{
    if (field_14C_pSlig)
    {
        field_14C_pSlig->mBaseGameObjectRefCount--;
        field_14C_pSlig = nullptr;
    }

    if (field_10C_pTarget)
    {
        field_10C_pTarget->mBaseGameObjectRefCount--;
        field_10C_pTarget = nullptr;
    }

    if (field_11C_timer < static_cast<s32>(sGnFrame) + 80)
    {
        mBaseAnimatedWithPhysicsGameObject_RGB.r -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.g -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.b -= 2;
        mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.023);
    }

    if (static_cast<s32>(sGnFrame) < field_11C_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    return 100;
}

s16 Slog::Facing(FP xpos)
{
    if (mBaseAnimatedWithPhysicsGameObject_XPos < xpos && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    if (xpos < mBaseAnimatedWithPhysicsGameObject_XPos && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    return FALSE;
}

void Slog::DelayedResponse(s16 responseIdx)
{
    field_152_response_index = responseIdx;
    field_154_response_part = 0;
    field_180 = 0;
    field_11C_timer = sGnFrame + 10;
}

} // namespace AO
