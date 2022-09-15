#include "stdafx.h"
#include "Slog.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "MusicController.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Events.hpp"
#include "Abe.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Map.hpp"
#include "Sound/Midi.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Bone.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "SnoozeParticle.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "../relive_lib/Particle.hpp"
#include "GameSpeak.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Mudokon.hpp"
#include "Grid.hpp"
#include "Function.hpp"
#include "AnimationCallBacks.hpp"

ALIVE_VAR(1, 0xBAF7F2, s16, sSlogCount, 0);

const AnimId sSlogAnimIdTable[24] = {
    AnimId::Slog_Idle,
    AnimId::Slog_Walk,
    AnimId::Slog_Run,
    AnimId::Slog_TurnAround,
    AnimId::Slog_Fall,
    AnimId::Slog_MoveHeadUpwards,
    AnimId::Slog_StopRunning,
    AnimId::Slog_SlideTurn,
    AnimId::Slog_StartWalking,
    AnimId::Slog_EndWalking,
    AnimId::Slog_Land,
    AnimId::Slog_Unused,
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
    AnimId::Slog_Growl };

const TSlogMotionFn sSlogMotionTable[24] = {
    &Slog::Motion_0_Idle,
    &Slog::Motion_1_Walk,
    &Slog::Motion_2_Run,
    &Slog::Motion_3_TurnAround,
    &Slog::Motion_4_Fall,
    &Slog::Motion_5_MoveHeadUpwards,
    &Slog::Motion_6_StopRunning,
    &Slog::Motion_7_SlideTurn,
    &Slog::Motion_8_StartWalking,
    &Slog::Motion_9_EndWalking,
    &Slog::Motion_10_Land,
    &Slog::Motion_11_Unused,
    &Slog::Motion_12_StartFastBarking,
    &Slog::Motion_13_EndFastBarking,
    &Slog::Motion_14_AngryBark,
    &Slog::Motion_15_Sleeping,
    &Slog::Motion_16_MoveHeadDownwards,
    &Slog::Motion_17_WakeUp,
    &Slog::Motion_18_JumpForwards,
    &Slog::Motion_19_JumpUpwards,
    &Slog::Motion_20_Eating,
    &Slog::Motion_21_Dying,
    &Slog::Motion_22_Scratch,
    &Slog::Motion_23_Growl};

const TSlogBrainFn sSlogBrainTable[4] = {
    &Slog::Brain_0_ListeningToSlig,
    &Slog::Brain_1_Idle,
    &Slog::Brain_2_ChasingAbe,
    &Slog::Brain_3_Death};

const relive::SfxDefinition sSlogSFXList[19] = {
    {0u, 12u, 38u, 30u, 0, 0},
    {0u, 12u, 39u, 30u, 0, 0},
    {0u, 12u, 40u, 100u, -256, 0},
    {0u, 12u, 41u, 60u, 0, 0},
    {0u, 12u, 42u, 50u, 0, 0},
    {0u, 12u, 40u, 100u, -256, 0},
    {0u, 12u, 44u, 90u, 0, 0},
    {0u, 12u, 60u, 100u, 0, 0},
    {0u, 12u, 45u, 100u, -256, 0},
    {0u, 12u, 46u, 127u, -127, 127},
    {0u, 12u, 47u, 70u, 0, 0},
    {0u, 12u, 48u, 70u, 0, 0},
    {0u, 12u, 49u, 30u, 0, 0},
    {0u, 12u, 40u, 120u, 300, 400},
    {0u, 12u, 61u, 45u, 0, 0},
    {0u, 12u, 62u, 45u, 0, 0},
    {0u, 3u, 59u, 67u, 0, 0},
    {0u, 12u, 33u, 45u, 0, 127},
    {0u, 12u, 33u, 40u, -127, 0},
};

Slog::Slog(FP xpos, FP ypos, FP scale, s16 bListenToSligs, s16 chaseDelay)
    : BaseAliveGameObject(5)
{
    LoadAnimations();
    field_134_last_event_index = -1;

    mYPos = ypos;
    mXPos = xpos;

    mSpriteScale = scale;

    Init();

    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);
    field_12C_tlvInfo = Guid{};
    field_120_brain_state_idx = 2;
    field_122_brain_state_result = 0;

    BaseAliveGameObject* pTarget = FindTarget(0, 0);
    if (!pTarget)
    {
        pTarget = sControlledCharacter;
    }
    field_118_target_id = pTarget->mBaseGameObjectId;

    field_160_flags.Clear(Flags_160::eBit2_ListenToSligs);
    field_160_flags.Clear(Flags_160::eBit7_Asleep);
    field_160_flags.Clear(Flags_160::eBit9_MovedOffScreen);

    field_160_flags.Set(Flags_160::eBit2_ListenToSligs, bListenToSligs & 1);

    field_144_wake_up_anger = 0;
    field_158_chase_delay = chaseDelay;
    field_154_anger_switch_id = 0;
    SetCurrentMotion(eSlogMotions::Motion_0_Idle);
    field_146_total_anger = 10;
    field_148_chase_anger = 20;
    field_156_bone_eating_time = 60;
}

void Slog::LoadAnimations()
{
    for (auto& animId : sSlogAnimIdTable)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Slog::Slog(relive::Path_Slog* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(5)
{
    LoadAnimations();
    field_134_last_event_index = -1;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromDouble(0.5);
    }
    else
    {
        mSpriteScale = FP_FromDouble(1);
    }

    mBaseGameObjectTlvInfo = tlvId;

    Init();

    field_160_flags.Clear(Flags_160::eBit9_MovedOffScreen);
    field_160_flags.Set(Flags_160::eBit2_ListenToSligs);
    field_160_flags.Set(Flags_160::eBit7_Asleep, pTlv->mAsleep == relive::reliveChoice::eYes);
    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pTlv->mFacing == relive::reliveXDirection::eLeft);

    field_12C_tlvInfo = tlvId;
    mBaseGameObjectTlvInfo = tlvId;
    field_120_brain_state_idx = 1;
    field_118_target_id = Guid{};
    field_144_wake_up_anger = pTlv->mWakeUpAnger;
    field_146_total_anger = pTlv->mWakeUpAnger + pTlv->mBarkAnger;
    field_148_chase_anger = field_146_total_anger + pTlv->mChaseAnger;
    field_158_chase_delay = pTlv->mChaseDelay;
    field_154_anger_switch_id = pTlv->mAngerSwitchId;
    field_156_bone_eating_time = pTlv->mBoneEatingTime;

    if (field_160_flags.Get(Flags_160::eBit7_Asleep))
    {
        SetCurrentMotion(eSlogMotions::Motion_15_Sleeping);
        SetAnimFrame();
    }
    else
    {
        SetCurrentMotion(eSlogMotions::Motion_0_Idle);
    }

    VUpdate();
}

ALIVE_VAR(1, 0xBAF7F0, u8, sSlogRandomIdx_BAF7F0, 0);

s32 Slog::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Slog_State*>(pSaveBuffer);
    pState->field_0_type = AETypes::eSlog_126;

    pState->field_4_objectId = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;
    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_50_falling_velx_scale_factor = field_128_falling_velx_scale_factor;

    pState->field_18_path_number = mCurrentPath;
    pState->field_1A_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_1C_sprite_scale = mSpriteScale;

    pState->mRingRed = mRGB.r;
    pState->mRingGreen = mRGB.g;
    pState->mRingBlue = mRGB.b;

    pState->field_26_bAnimFlipX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_anim_cur_frame = static_cast<s16>(mAnim.mCurrentFrame);
    pState->field_2C_frame_change_counter = static_cast<s16>(mAnim.mFrameChangeCounter);
    pState->field_2F_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2E_bRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = mHealth;
    pState->field_34_current_motion = mCurrentMotion;
    pState->field_36_next_motion = mNextMotion;
    pState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_3A_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->field_3A_line_type = -1;
    }

    pState->field_3C_id = BaseAliveGameObject_PlatformId;
    pState->field_74_flags.Set(Slog_State::eBit2_Possessed, sControlledCharacter == this); // Lol can't be possessed anyway so ??
    pState->field_40_tlvInfo = field_12C_tlvInfo;
    pState->field_44_obj_id = Guid{};

    if (field_118_target_id != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(field_118_target_id);
        if (pObj)
        {
            pState->field_44_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pState->field_48_state_idx = field_120_brain_state_idx;
    pState->field_4A_brain_state_result = field_122_brain_state_result;
    pState->field_4C_timer = field_124_timer;
    pState->field_50_falling_velx_scale_factor = field_128_falling_velx_scale_factor;
    pState->field_40_tlvInfo = field_12C_tlvInfo;
    pState->field_54_obj_id = Guid{};

    if (field_138_listening_to_slig_id != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(field_138_listening_to_slig_id);
        if (pObj)
        {
            pState->field_54_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pState->field_58_has_woofed = field_132_has_woofed;
    pState->field_5A_waiting_counter = field_13C_waiting_counter;
    pState->field_5C_response_index = field_13E_response_index;
    pState->field_5E_response_part = field_140_response_part;
    pState->field_60_anger_level = field_142_anger_level;
    pState->field_62_jump_counter = field_15A_jump_counter;
    pState->field_64_scratch_timer = field_14C_scratch_timer;
    pState->field_68_growl_timer = field_150_growl_timer;
    pState->field_6C_bone_id = Guid{};

    if (field_15C_bone_id != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(field_15C_bone_id);
        if (pObj)
        {
            pState->field_6C_bone_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pState->field_70_jump_delay = field_158_chase_delay;
    pState->field_72_slog_random_index = sSlogRandomIdx_BAF7F0;

    pState->field_74_flags.Set(Slog_State::eBit1_BitingTarget, field_11C_biting_target & 1);
    pState->field_74_flags.Set(Slog_State::eBit2_Possessed, sControlledCharacter == this); // Can never happen so is always 0
    pState->field_74_flags.Set(Slog_State::eBit3_Asleep, field_160_flags.Get(Flags_160::eBit8_Asleep));
    pState->field_74_flags.Set(Slog_State::eBit4_MovedOffScreen, field_160_flags.Get(Flags_160::eBit9_MovedOffScreen));
    pState->field_74_flags.Set(Slog_State::eBit5_StopRunning, field_160_flags.Get(Flags_160::eBit1_StopRunning));
    pState->field_74_flags.Set(Slog_State::eBit6_Shot, field_160_flags.Get(Flags_160::eBit3_Shot));
    pState->field_74_flags.Set(Slog_State::eBit7_Hungry, field_160_flags.Get(Flags_160::eBit4_Hungry));
    pState->field_74_flags.Set(Slog_State::eBit8_CommandedToAttack, field_160_flags.Get(Flags_160::eBit5_CommandedToAttack));
    pState->field_74_flags.Set(Slog_State::eBit9_HitByAbilityRing, field_160_flags.Get(Flags_160::eBit6_HitByAbilityRing));
    pState->field_74_flags.Set(Slog_State::eBit10_ListenToSligs, field_160_flags.Get(Flags_160::eBit2_ListenToSligs));

    return sizeof(Slog_State);
}

s32 Slog::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Slog_State*>(pBuffer);
    auto pTlv = static_cast<relive::Path_Slog*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_40_tlvInfo));

    Slog* pSlog = nullptr;
    if (pState->field_40_tlvInfo == Guid{})
    {
        pSlog = relive_new Slog(pState->field_8_xpos,
                                  pState->field_C_ypos,
                                  pState->field_1C_sprite_scale, pState->field_74_flags.Get(Slog_State::eBit10_ListenToSligs), pState->field_70_jump_delay);

        pSlog->mBaseGameObjectTlvInfo = pState->field_4_objectId;
    }
    else
    {
        pSlog = relive_new Slog(pTlv, pState->field_40_tlvInfo);
    }

    pSlog->BaseAliveGameObjectPathTLV = nullptr;
    pSlog->BaseAliveGameObjectCollisionLine = nullptr;
    pSlog->BaseAliveGameObject_PlatformId = pState->field_3C_id;
    pSlog->mXPos = pState->field_8_xpos;
    pSlog->mYPos = pState->field_C_ypos;
    pSlog->mVelX = pState->field_10_velx;
    pSlog->mVelY = pState->field_14_vely;
    pSlog->field_128_falling_velx_scale_factor = pState->field_50_falling_velx_scale_factor;
    pSlog->mCurrentPath = pState->field_18_path_number;
    pSlog->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_1A_lvl_number);
    pSlog->mSpriteScale = pState->field_1C_sprite_scale;
    pSlog->mRGB.SetRGB(pState->mRingRed, pState->mRingGreen, pState->mRingBlue);

    pSlog->SetCurrentMotion(pState->field_28_current_motion);
    pSlog->mAnim.Set_Animation_Data(pSlog->GetAnimRes(sSlogAnimIdTable[pSlog->mCurrentMotion]));

    pSlog->mAnim.mCurrentFrame = pState->field_2A_anim_cur_frame;
    pSlog->mAnim.mFrameChangeCounter = pState->field_2C_frame_change_counter;

    pSlog->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bAnimFlipX & 1);
    pSlog->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

    pSlog->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_bDrawable & 1);

    if (IsLastFrame(&pSlog->mAnim))
    {
        pSlog->mAnim.mFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pSlog->mHealth = pState->field_30_health;
    pSlog->mCurrentMotion = pState->field_34_current_motion;
    pSlog->mNextMotion = pState->field_36_next_motion;
    pSlog->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);
    pSlog->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
    pSlog->BaseAliveGameObjectCollisionLineType = pState->field_3A_line_type;
    pSlog->field_12C_tlvInfo = pState->field_40_tlvInfo;
    pSlog->field_118_target_id = pState->field_44_obj_id;
    pSlog->field_120_brain_state_idx = pState->field_48_state_idx;
    pSlog->field_122_brain_state_result = pState->field_4A_brain_state_result;
    pSlog->field_124_timer = pState->field_4C_timer;
    pSlog->field_128_falling_velx_scale_factor = pState->field_50_falling_velx_scale_factor;
    pSlog->field_12C_tlvInfo = pState->field_40_tlvInfo;
    pSlog->field_138_listening_to_slig_id = pState->field_54_obj_id;
    pSlog->field_132_has_woofed = pState->field_58_has_woofed;
    pSlog->field_13C_waiting_counter = pState->field_5A_waiting_counter;
    pSlog->field_13E_response_index = pState->field_5C_response_index;
    pSlog->field_140_response_part = pState->field_5E_response_part;
    pSlog->field_142_anger_level = pState->field_60_anger_level;
    pSlog->field_15A_jump_counter = pState->field_62_jump_counter;
    pSlog->field_14C_scratch_timer = pState->field_64_scratch_timer;
    pSlog->field_150_growl_timer = pState->field_68_growl_timer;
    pSlog->field_158_chase_delay = pState->field_70_jump_delay;
    pSlog->field_15C_bone_id = pState->field_6C_bone_id;
    sSlogRandomIdx_BAF7F0 = pState->field_72_slog_random_index;


    pSlog->field_11C_biting_target = pState->field_74_flags.Get(Slog_State::eBit1_BitingTarget);
    // bit2 never read
    pSlog->field_160_flags.Set(Flags_160::eBit8_Asleep, pState->field_74_flags.Get(Slog_State::eBit3_Asleep));
    pSlog->field_160_flags.Set(Flags_160::eBit9_MovedOffScreen, pState->field_74_flags.Get(Slog_State::eBit4_MovedOffScreen));
    pSlog->field_160_flags.Set(Flags_160::eBit1_StopRunning, pState->field_74_flags.Get(Slog_State::eBit5_StopRunning));
    pSlog->field_160_flags.Set(Flags_160::eBit3_Shot, pState->field_74_flags.Get(Slog_State::eBit6_Shot));
    pSlog->field_160_flags.Set(Flags_160::eBit4_Hungry, pState->field_74_flags.Get(Slog_State::eBit7_Hungry));
    pSlog->field_160_flags.Set(Flags_160::eBit5_CommandedToAttack, pState->field_74_flags.Get(Slog_State::eBit8_CommandedToAttack));
    pSlog->field_160_flags.Set(Flags_160::eBit6_HitByAbilityRing, pState->field_74_flags.Get(Slog_State::eBit9_HitByAbilityRing));
    pSlog->field_160_flags.Set(Flags_160::eBit2_ListenToSligs, pState->field_74_flags.Get(Slog_State::eBit10_ListenToSligs));

    if (pSlog->field_160_flags.Get(Flags_160::eBit3_Shot))
    {
        sSlogCount--;
    }

    return sizeof(Slog_State);
}

void Slog::Motion_0_Idle()
{
    if (!ToNextMotion())
    {
        if (GetNextMotion() == eSlogMotions::Motion_18_JumpForwards)
        {
            ToJump();
        }
        else if (GetNextMotion() == eSlogMotions::m1)
        {
            if (!mCurrentMotion)
            {
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    SND_SEQ_PlaySeq(SeqId::Empty_13, 1, 0);
                }

                if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
                {
                    if (MusicController::static_GetMusicType(0, 0, 0) == MusicController::MusicTypes::eSlogChaseTension_6)
                    {
                        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChaseTension_6, this, 0, 0);
                    }
                    else
                    {
                        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogTension_5, this, 0, 0);
                    }
                }
            }
        }
        else
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }
}

const FP sSlogWalkVelXTable_5475EC[18] = {
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

void Slog::Motion_1_Walk()
{
    FP velX = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogWalkVelXTable_5475EC[mAnim.mCurrentFrame];
    }
    else
    {
        velX = sSlogWalkVelXTable_5475EC[mAnim.mCurrentFrame];
    }

    mVelX = (mSpriteScale * velX);

    if (CollisionCheck(mSpriteScale * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        ToIdle();

        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mXPos += mVelX + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
        }
        else
        {
            mXPos -= (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) - mVelX;
        }
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eSlogMotions::Motion_1_Walk)
        {
            if (mAnim.mCurrentFrame == 2 || mAnim.mCurrentFrame == 11)
            {
                if (GetNextMotion() == eSlogMotions::Motion_0_Idle)
                {
                    SetCurrentMotion(eSlogMotions::Motion_9_EndWalking);
                    SetNextMotion(eSlogMotions::m1);
                }
                if (GetNextMotion() == eSlogMotions::Motion_3_TurnAround)
                {
                    SetCurrentMotion(eSlogMotions::Motion_9_EndWalking);
                }
            }
            else if (mAnim.mCurrentFrame == 5 || mAnim.mCurrentFrame == 14)
            {
                Sfx(SlogSound::SlowStep_18);

                if (!field_160_flags.Get(Flags_160::eBit8_Asleep))
                {
                    field_160_flags.Set(Flags_160::eBit8_Asleep);
                    MapFollowMe(FALSE);
                }

                if (GetNextMotion() == eSlogMotions::Motion_2_Run)
                {
                    SetCurrentMotion(eSlogMotions::Motion_2_Run);
                    SetNextMotion(eSlogMotions::m1);
                }
            }
            else
            {
                field_160_flags.Clear(Flags_160::eBit8_Asleep);
            }
        }
    }
}

static u8 Slog_NextRandom()
{
    return sRandomBytes_546744[sSlogRandomIdx_BAF7F0++];
}


const FP sSlogRunVelXTable_547634[8] = {
    FP_FromDouble(8.625900268554688),
    FP_FromDouble(3.38677978515625),
    FP_FromDouble(3.5192413330078125),
    FP_FromDouble(5.1022491455078125),
    FP_FromDouble(6.7406005859375),
    FP_FromDouble(7.8218231201171875),
    FP_FromDouble(8.39404296875),
    FP_FromDouble(6.532867431640625)};

void Slog::Motion_2_Run()
{
    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_7, this, 0, 0);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = (mSpriteScale * -sSlogRunVelXTable_547634[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = (mSpriteScale * sSlogRunVelXTable_547634[mAnim.mCurrentFrame]);
    }

    if (CollisionCheck(mSpriteScale * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eSlogMotions::Motion_2_Run)
        {
            if (Slog_NextRandom() < 35u)
            {
                Sfx(SlogSound::CautiousWoof_5);
            }

            if (mAnim.mCurrentFrame == 4 || mAnim.mCurrentFrame == 7)
            {
                Sfx(SlogSound::FastStep_17);

                if (!field_160_flags.Get(Flags_160::eBit8_Asleep))
                {
                    field_160_flags.Set(Flags_160::eBit8_Asleep);
                    MapFollowMe(FALSE);
                }

                if (GetNextMotion() == eSlogMotions::Motion_0_Idle)
                {
                    SetCurrentMotion(eSlogMotions::Motion_6_StopRunning);
                    SetNextMotion(eSlogMotions::m1);
                }
                else if (GetNextMotion() == eSlogMotions::Motion_18_JumpForwards)
                {
                    ToJump();
                    SetNextMotion(eSlogMotions::m1);
                }
                else if (GetNextMotion() != eSlogMotions::m1)
                {
                    SetCurrentMotion(mNextMotion);
                    SetNextMotion(eSlogMotions::m1);
                }
            }
            else
            {
                field_160_flags.Clear(Flags_160::eBit8_Asleep);
            }
        }
    }
}

void Slog::Motion_3_TurnAround()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle();
    }
}

void Slog::Motion_4_Fall()
{
    mVelY += (mSpriteScale * FP_FromDouble(1.8));
    if (mVelY > (mSpriteScale * FP_FromInteger(20)))
    {
        mVelY = (mSpriteScale * FP_FromInteger(20));
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (mVelX > (FP_FromInteger(8) * mSpriteScale))
        {
            mVelX = (FP_FromInteger(8) * mSpriteScale);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        if (mVelX < -(FP_FromInteger(8) * mSpriteScale))
        {
            mVelX = -(FP_FromInteger(8) * mSpriteScale);
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (mSpriteScale * field_128_falling_velx_scale_factor);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (mSpriteScale * field_128_falling_velx_scale_factor);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    const FP xposBeforeChange = mXPos;
    const FP yposBeforeChange = mYPos;

    mXPos += mVelX;
    mYPos += mVelY;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            xposBeforeChange,
            yposBeforeChange - (mSpriteScale * FP_FromInteger(20)),
            mXPos,
            mYPos,
            &pLine, &hitX, &hitY, CollisionMask(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32)))
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
                mXPos = hitX;
                MapFollowMe(FALSE);
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(FP_GetExponent(mYPos) + 5)};
                    VOnCollisionWith(
                        xy,
                        wh,
                        ObjList_5C1B78,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                }
                SetCurrentMotion(eSlogMotions::Motion_10_Land);
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mYPos = hitY;
                mXPos = hitX - mVelX;
                MapFollowMe(FALSE);
                mVelX = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Slog::Motion_5_MoveHeadUpwards()
{
    if (mAnim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::IdleWoof_2);
        field_132_has_woofed = 1;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange(0, 100) < 30)
        {
            Sfx(SlogSound::IdleGrrAlt_4);
        }
        ToIdle();
    }
}

const FP sSlogStopRunningVelX_547658[10] = {
    FP_FromDouble(3.468994140625),
    FP_FromDouble(4.5489959716796875),
    FP_FromDouble(8.642990112304688),
    FP_FromDouble(9.31298828125),
    FP_FromDouble(6.4949951171875),
    FP_FromDouble(4.631988525390625),
    FP_FromDouble(3.9169921875),
    FP_FromDouble(4.334991455078125),
    FP_FromDouble(3.6609954833984375),
    FP_FromDouble(1.2819976806640625)};

void Slog::Motion_6_StopRunning()
{
    if (mAnim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::Skid_14);
    }

    FP velX = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogStopRunningVelX_547658[mAnim.mCurrentFrame];
    }
    else
    {
        velX = sSlogStopRunningVelX_547658[mAnim.mCurrentFrame];
    }

    mVelX = (mSpriteScale * velX);

    if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        MoveOnLine();
        if (!mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }
    }

    ToIdle();
}

const FP sSlogSlideTurnVelXTable_547684[25] = {
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
    FP_FromDouble(-4.89166259765625)};


void Slog::Motion_7_SlideTurn()
{
    FP velX = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogSlideTurnVelXTable_547684[mAnim.mCurrentFrame];
    }
    else
    {
        velX = sSlogSlideTurnVelXTable_547684[mAnim.mCurrentFrame];
    }

    mVelX = (mSpriteScale * velX);
    if (CollisionCheck(mSpriteScale * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(FALSE);

                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
                    mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(3));
                }
                else
                {
                    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
                    mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(3));
                }
                SetCurrentMotion(eSlogMotions::Motion_2_Run);
            }
        }
    }
}

void Slog::Motion_8_StartWalking()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_1_Walk);
    }

    MoveOnLine();
}

void Slog::Motion_9_EndWalking()
{
    MoveOnLine();

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_10_Land()
{
    if (mAnim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::Landing_16);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_11_Unused()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_12_StartFastBarking);
    }
}

void Slog::Motion_12_StartFastBarking()
{
    if (mAnim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::IdleWoof_2);
        field_132_has_woofed = 1;
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(eSlogMotions::Motion_13_EndFastBarking);
        }
    }
}

void Slog::Motion_13_EndFastBarking()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_14_AngryBark()
{
    if (mAnim.mCurrentFrame == 0 || mAnim.mCurrentFrame == 6)
    {
        Sfx(SlogSound::CautiousWoof_5);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }
}

void Slog::Motion_15_Sleeping()
{
    if (GetNextMotion() != eSlogMotions::m1 && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(mNextMotion);
        SetNextMotion(eSlogMotions::m1);
        return;
    }

    bool createParticle = false;
    if (static_cast<s32>(sGnFrame) % 60)
    {
        if (!(static_cast<s32>((sGnFrame - 20)) % 60))
        {
            Sfx(SlogSound::YawnEnd_11);
            createParticle = true;
        }
    }
    else
    {
        Sfx(SlogSound::YawnStart_10);
        createParticle = true;
    }

    if (createParticle)
    {
        if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
        {
            FP xOff = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -(mSpriteScale * FP_FromInteger(18));
            }
            else
            {
                xOff = (mSpriteScale * FP_FromInteger(18));
            }
            relive_new SnoozeParticle(xOff + mXPos,
                                                          (mSpriteScale * FP_FromInteger(-13)) + mYPos,
                                                          mAnim.mRenderLayer, mAnim.field_14_scale);
        }
    }
}

void Slog::Motion_16_MoveHeadDownwards()
{
    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
}

void Slog::Motion_17_WakeUp()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSnoozeParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->mState = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
            Sfx(SlogSound::IdleGrrAlt_4);
        }
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogTension_5, this, 0, 0);
    }
}

void Slog::Motion_18_JumpForwards()
{
    mVelY += (mSpriteScale * FP_FromDouble(1.8));

    const FP k20Scaled = mSpriteScale * FP_FromInteger(20);
    if (mVelY > k20Scaled)
    {
        mVelY = k20Scaled;
    }

    const FP oldXPos = mXPos;
    const FP oldYPos = mYPos;

    mXPos += mVelX;
    mYPos += mVelY;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(oldXPos, oldYPos - k20Scaled, mXPos, mYPos, &pLine, &hitX, &hitY, CollisionMask(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32)
) == 1)
    {
        switch (pLine->mLineType) // TODO: line enum
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                if (mVelY > FP_FromInteger(0))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    SetNextMotion(eSlogMotions::m1);
                    SetCurrentMotion(eSlogMotions::Motion_2_Run);
                    mYPos = hitY;
                    mVelY = FP_FromInteger(0);
                }
                break;

            case 1u:
            case 5u:
                if (mVelX < FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(2));
                }
                break;

            case 2u:
            case 6u:
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = (-mVelX / FP_FromInteger(2));
                }
                break;

            default:
                break;
        }
    }

    if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(2))
    {
        field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
        BaseAliveGameObjectLastLineYPos = mYPos;
        SetCurrentMotion(eSlogMotions::Motion_4_Fall);
    }
}

void Slog::Motion_19_JumpUpwards()
{
    const PSX_RECT bRect = VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            FP_FromInteger(bRect.x),
            FP_FromInteger(bRect.y),
            FP_FromInteger(bRect.w),
            FP_FromInteger(bRect.h),
            &pLine, &hitX, &hitY, CollisionMask(eCeiling_3)))
    {
        if (mAnim.mCurrentFrame < 12)
        {
            mAnim.SetFrame(24 - mAnim.mCurrentFrame);
        }
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChaseTension_6, this, 0, 0);
    }

    if (mAnim.mCurrentFrame == 5)
    {
        if (field_160_flags.Get(Flags_160::eBit4_Hungry) && field_118_target_id == sActiveHero->mBaseGameObjectId && sActiveHero->mScale == mScale && (sActiveHero->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold_455DF0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold_454410))
        {
            Sfx(SlogSound::HungryYip_13);
        }
        else
        {
            Sfx(SlogSound::JumpBite_6);
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_0_Idle);
        SetNextMotion(eSlogMotions::m1);
    }
}

void Slog::Motion_20_Eating()
{
    SND_SEQ_Stop(SeqId::Empty_13);
    if (mAnim.mCurrentFrame == 0)
    {
        mAnim.mFlags.Clear(AnimFlags::eBit19_LoopBackwards);
        if (GetNextMotion() != eSlogMotions::m1 && GetNextMotion() != eSlogMotions::Motion_20_Eating)
        {
            SetCurrentMotion(eSlogMotions::Motion_0_Idle);
            return;
        }
    }

    if (mAnim.mCurrentFrame == 3 && !mAnim.mFlags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Eating1, relive::SoundEffects::Eating2), 100);
        relive_new Blood(((mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(-25) : FP_FromInteger(25)) * mSpriteScale + mXPos,
                      mYPos - (FP_FromInteger(4) * mSpriteScale),
                      FP_FromInteger(0), FP_FromInteger(0),
                      mSpriteScale, 12);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange(0, 100) < 85)
        {
            if (static_cast<s32>(sGnFrame) > field_150_growl_timer && Math_RandomRange(0, 100) < 60)
            {
                field_150_growl_timer = sGnFrame + 16;
                Sfx(SlogSound::IdleGrrr_3);
            }
            mAnim.mFlags.Set(AnimFlags::eBit19_LoopBackwards);
        }
    }

    if (mAnim.mCurrentFrame == 0)
    {
        mAnim.mFlags.Clear(AnimFlags::eBit19_LoopBackwards);
    }
}

void Slog::Motion_21_Dying()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_4_Fall();
        SetCurrentMotion(eSlogMotions::Motion_21_Dying);
    }
}

void Slog::Motion_22_Scratch()
{
    if (mAnim.mCurrentFrame == 4)
    {
        SND_SEQ_PlaySeq(SeqId::Scratching_12, 1, 1);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }
}

void Slog::Motion_23_Growl()
{
    if (mAnim.mCurrentFrame == 3)
    {
        if (field_132_has_woofed)
        {
            Sfx(SlogSound::IdleGrrr_3);
            mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
            field_132_has_woofed = 0;
            field_150_growl_timer = sGnFrame + 12;
        }
    }

    if (static_cast<s32>(sGnFrame) > field_150_growl_timer)
    {
        mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }
}

const eSlogMotions sSlogResponseMotion_560930[3][10] = {
    {eSlogMotions::Motion_3_TurnAround,
     eSlogMotions::Motion_2_Run,
     eSlogMotions::Motion_7_SlideTurn,
     eSlogMotions::m1,
     eSlogMotions::Motion_7_SlideTurn,
     eSlogMotions::m1,
     eSlogMotions::Motion_6_StopRunning,
     eSlogMotions::m2,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle},
    {eSlogMotions::Motion_12_StartFastBarking,
     eSlogMotions::Motion_2_Run,
     eSlogMotions::Motion_6_StopRunning,
     eSlogMotions::m2,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle},
    {eSlogMotions::Motion_12_StartFastBarking,
     eSlogMotions::m2,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle,
     eSlogMotions::Motion_0_Idle}};

s16 Slog::Brain_0_ListeningToSlig()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_138_listening_to_slig_id));

    // TODO: OG bug - return never used?
    //sObjectIds.Find_449CF0(field_118);

    if (!pObj || pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_142_anger_level = 0;
        field_138_listening_to_slig_id = Guid{};
        field_118_target_id = Guid{};
        field_120_brain_state_idx = 1;
        return 0;
    }

    FP gridSize1Directed = ScaleToGridSize(mSpriteScale) * FP_FromInteger(1);
    if (pObj->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize1Directed = -gridSize1Directed;
    }

    const FP xpos1GridAHead = gridSize1Directed + pObj->mXPos;
    switch (field_122_brain_state_result)
    {
        case 0:
            return Brain_ListeningToSlig_State_0_Init();
        case 1:
            return Brain_ListeningToSlig_State_1_Idle(xpos1GridAHead);
        case 2:
            return Brain_ListeningToSlig_State_2_Listening(xpos1GridAHead, pObj);
        case 3:
            return Brain_ListeningToSlig_State_3_Walking(xpos1GridAHead);
        case 4:
            return Brain_ListeningToSlig_State_4_Running(xpos1GridAHead);
        case 5:
            return Brain_ListeningToSlig_State_5_Waiting();
        case 6:
            return Brain_ListeningToSlig_State_6_Responding();
        default:
            return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ListeningToSlig_State_6_Responding()
{
    if (static_cast<s32>(sGnFrame) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }

    field_124_timer = sGnFrame + 10;
    SetNextMotion(sSlogResponseMotion_560930[field_13E_response_index][field_140_response_part++]);

    if (GetNextMotion() == eSlogMotions::m2)
    {
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }
    else
    {
        return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ListeningToSlig_State_5_Waiting()
{
    if (static_cast<s32>(sGnFrame) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }
    field_13C_waiting_counter--;
    SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
    return 2;
}

s16 Slog::Brain_ListeningToSlig_State_4_Running(const FP xpos1GridAHead)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        return 2;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_2_Run)
    {
        return field_122_brain_state_result;
    }

    if (Facing(xpos1GridAHead))
    {
        if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(3)))
        {
            return field_122_brain_state_result;
        }
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }
    else
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
        return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ListeningToSlig_State_3_Walking(const FP xpos1GridAHead)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        return 2;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_1_Walk)
    {
        return field_122_brain_state_result;
    }

    if (!Facing(xpos1GridAHead))
    {
        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return 2;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(4)))
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
        return 4;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(3)))
    {
        return field_122_brain_state_result;
    }

    SetNextMotion(eSlogMotions::Motion_0_Idle);
    return 2;
}

s16 Slog::Brain_ListeningToSlig_State_2_Listening(const FP xpos1GridAHead, BaseAliveGameObject* pObj)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }

    GameSpeakEvents speakValue = GameSpeakEvents::eNone_m1;
    if (field_134_last_event_index == pEventSystem_5BC11C->field_28_last_event_index)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            speakValue = GameSpeakEvents::eNone_m1;
        }
        else
        {
            speakValue = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_134_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        speakValue = pEventSystem_5BC11C->field_20_last_event;
    }

    switch (speakValue)
    {
        case GameSpeakEvents::Slig_LookOut_6:
            DelayedResponse(2);
            return 6;

        case GameSpeakEvents::Slig_Hi_27:
            field_13C_waiting_counter++;
            if (static_cast<s32>(sGnFrame) % 2)
            {
                field_13C_waiting_counter++;
            }
            [[fallthrough]];

        case GameSpeakEvents::Slig_HereBoy_28:
            field_124_timer = sGnFrame - Math_NextRandom() % 8 + 15;
            field_13C_waiting_counter++;
            break;

        case GameSpeakEvents::Slig_GetEm_29:
        {
            auto pTarget = FindTarget(1, 0);
            if (pTarget)
            {
                field_138_listening_to_slig_id = Guid{};
                field_160_flags.Set(Flags_160::eBit5_CommandedToAttack);
                field_118_target_id = pTarget->mBaseGameObjectId;
                field_120_brain_state_idx = 2;
                return 0;
            }

            FP gridSize = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mSpriteScale);
            }

            if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                DelayedResponse(1);
                return 6;
            }
            break;
        }

        case GameSpeakEvents::eUnknown_33:
            DelayedResponse(0);
            return 6;

        default:
            break;
    }

    if (!(Math_NextRandom() % 128))
    {
        field_13C_waiting_counter++;
    }

    if (field_13C_waiting_counter > 0)
    {
        field_124_timer += Math_NextRandom() % 16;
        return 5;
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(4)))
    {
        if (!Facing(xpos1GridAHead))
        {
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return field_122_brain_state_result;
        }

        FP gridSize2 = {};
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize2 = -ScaleToGridSize(mSpriteScale);
        }
        else
        {
            gridSize2 = ScaleToGridSize(mSpriteScale);
        }

        if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), gridSize2 * FP_FromInteger(2)))
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
            return 4;
        }
    }

    if (FP_Abs(xpos1GridAHead - mXPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(1)))
    {
        if (!Facing(xpos1GridAHead))
        {
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return field_122_brain_state_result;
        }

        FP gridSize3 = {};
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            gridSize3 = -ScaleToGridSize(mSpriteScale);
        }
        else
        {
            gridSize3 = ScaleToGridSize(mSpriteScale);
        }

        if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), gridSize3 * FP_FromInteger(2)))
        {
            SetNextMotion(eSlogMotions::Motion_1_Walk);
            return 3;
        }
    }

    if (pObj->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) != mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
    }

    return field_122_brain_state_result;
}

s16 Slog::Brain_ListeningToSlig_State_1_Idle(const FP xpos1GridAHead)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }

    if (!Facing(xpos1GridAHead))
    {
        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return field_122_brain_state_result;
    }

    if (static_cast<s32>(sGnFrame) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }
    SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
    return 2;
}

s16 Slog::Brain_ListeningToSlig_State_0_Init()
{
    SetNextMotion(eSlogMotions::Motion_0_Idle);
    field_13C_waiting_counter = 0;
    field_124_timer = sGnFrame + 15;
    return 1;
}

s16 Slog::Brain_1_Idle()
{
    BaseGameObject* pTarget = sObjectIds.Find_Impl(field_118_target_id);

    // OG dead code - return never used
    //sObjectIds.Find_449CF0(field_138_bottom_right);

    if (pTarget && pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_118_target_id = Guid{};
    }

    if (field_134_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
    {
        field_134_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter->Type() == ReliveTypes::eSlig)
        {
            field_120_brain_state_idx = 0;
            field_118_target_id = Guid{};
            field_138_listening_to_slig_id = sControlledCharacter->mBaseGameObjectId;
            return 0;
        }
    }

    if (SwitchStates_Get(field_154_anger_switch_id))
    {
        field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);
        field_120_brain_state_idx = 2;
        return 0;
    }

    switch (field_122_brain_state_result)
    {
        case 0:
            if (GetCurrentMotion() != eSlogMotions::Motion_15_Sleeping && GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
            {
                SetNextMotion(eSlogMotions::Motion_0_Idle);
                return field_122_brain_state_result;
            }

            if (field_160_flags.Get(Flags_160::eBit7_Asleep))
            {
                field_142_anger_level = 0;
                return 1;
            }
            else
            {
                field_142_anger_level = field_144_wake_up_anger;
                return 4;
            }
            break;

        case 1:
            if (IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, EventScale::Full))
            {
                field_142_anger_level++;
            }

            if (IsEventInRange(kEventSpeaking, mXPos, mYPos, EventScale::Full))
            {
                field_142_anger_level += Slog_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame) % 16))
            {
                // Calm down a bit
                if (field_142_anger_level > 0)
                {
                    field_142_anger_level--;
                }
            }

            if (field_142_anger_level <= field_144_wake_up_anger)
            {
                return field_122_brain_state_result;
            }

            SetNextMotion(eSlogMotions::Motion_17_WakeUp);
            return 2;

        case 2:
            if (GetCurrentMotion() != eSlogMotions::Motion_17_WakeUp)
            {
                return field_122_brain_state_result;
            }

            SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
            field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
            field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
            return 4;

        case 3:
            if (GetCurrentMotion() != eSlogMotions::Motion_16_MoveHeadDownwards)
            {
                if (GetNextMotion() != eSlogMotions::Motion_16_MoveHeadDownwards)
                {
                    SetNextMotion(eSlogMotions::Motion_16_MoveHeadDownwards);
                }

                return field_122_brain_state_result;
            }

            SetNextMotion(eSlogMotions::Motion_15_Sleeping);
            return 1;

        case 4:
            if (IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, EventScale::Full))
            {
                field_142_anger_level++;
            }

            if (IsEventInRange(kEventSpeaking, mXPos, mYPos, EventScale::Full))
            {
                field_142_anger_level += Slog_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame) % 32))
            {
                if (field_142_anger_level)
                {
                    if (field_160_flags.Get(Flags_160::eBit7_Asleep))
                    {
                        field_142_anger_level--;
                    }
                }
            }

            if (PlayerOrNakedSligNear())
            {
                field_142_anger_level += 2;
            }

            if (!(Slog_NextRandom() % 64) && GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
                return field_122_brain_state_result;
            }

            if (static_cast<s32>(sGnFrame) > field_150_growl_timer && GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
                SetCurrentMotion(eSlogMotions::Motion_23_Growl);
                SetNextMotion(eSlogMotions::Motion_0_Idle);
                sGnFrame = sGnFrame;
            }

            if (static_cast<s32>(sGnFrame) > field_14C_scratch_timer && GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
                SetCurrentMotion(eSlogMotions::Motion_22_Scratch);
                SetNextMotion(eSlogMotions::Motion_0_Idle);
            }

            if (field_142_anger_level > field_146_total_anger)
            {
                SetNextMotion(eSlogMotions::Motion_14_AngryBark);
                field_142_anger_level = field_142_anger_level + Slog_NextRandom() % 8;
                return 5;
            }

            if (field_142_anger_level >= field_144_wake_up_anger)
            {
                return field_122_brain_state_result;
            }

            SetNextMotion(eSlogMotions::Motion_16_MoveHeadDownwards);
            return 3;

        case 5:
            if (IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, EventScale::Full))
            {
                field_142_anger_level++;
            }

            if (IsEventInRange(kEventSpeaking, mXPos, mYPos, EventScale::Full))
            {
                field_142_anger_level += Math_NextRandom() % 8 + 15;
            }

            if (!(static_cast<s32>(sGnFrame) % 2))
            {
                if (field_142_anger_level > 0)
                {
                    field_142_anger_level--;
                }
            }

            if (PlayerOrNakedSligNear())
            {
                field_142_anger_level += 2;
            }

            if (field_142_anger_level >= field_146_total_anger)
            {
                if (field_142_anger_level <= field_148_chase_anger)
                {
                    return field_122_brain_state_result;
                }
                else
                {
                    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);
                    field_120_brain_state_idx = 2;
                    return 0;
                }
            }
            else
            {
                SetCurrentMotion(eSlogMotions::Motion_0_Idle);
                field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
                field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
                return 4;
            }
            break;

        default:
            return field_122_brain_state_result;
    }
}

s16 Slog::Brain_2_ChasingAbe()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_118_target_id));
    if (field_160_flags.Get(Flags_160::eBit2_ListenToSligs))
    {
        if (field_134_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
        {
            field_134_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
            if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter->Type() == ReliveTypes::eSlig)
            {
                field_120_brain_state_idx = 0;
                field_118_target_id = Guid{};
                field_138_listening_to_slig_id = sControlledCharacter->mBaseGameObjectId;
                return 0;
            }
        }
    }

    bool updateTarget = false;
    if (!pTarget)
    {
        if (field_118_target_id != Guid{})
        {
            field_118_target_id = Guid{};
            field_142_anger_level = 0;
            field_120_brain_state_idx = 1;
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            return 0;
        }
        updateTarget = true;
    }

    if (updateTarget || !field_160_flags.Get(Flags_160::eBit5_CommandedToAttack) || pTarget->mSpriteScale == FP_FromDouble(0.5))
    {
        if (!field_11C_biting_target)
        {
            pTarget = FindTarget(0, 0);
            if (!pTarget)
            {
                pTarget = FindTarget(0, 1);
                if (!pTarget)
                {
                    pTarget = sControlledCharacter;
                    if (sControlledCharacter->mSpriteScale == FP_FromDouble(0.5))
                    {
                        field_118_target_id = Guid{};
                        field_142_anger_level = 0;
                        field_120_brain_state_idx = 1;
                        SetNextMotion(eSlogMotions::Motion_0_Idle);
                        return 0;
                    }
                }
            }
            field_118_target_id = pTarget->mBaseGameObjectId;
        }
    }

    switch (field_122_brain_state_result)
    {
        case 0:
            return Brain_ChasingAbe_State_0_Init();
        case 1:
            return Brain_ChasingAbe_State_1_Waiting();
        case 2:
            return Brain_ChasingAbe_State_2_Thinking(pTarget);
        case 3:
            return Brain_ChasingAbe_State_3_GrowlOrScratch(pTarget);
        case 4:
            return Brain_ChasingAbe_State_4_LungingAtTarget(pTarget);
        case 7:
            return Brain_ChasingAbe_State_7_EatingTarget(pTarget);
        case 8:
            return Brain_ChasingAbe_State_8_ToIdle();
        case 9:
            return Brain_ChasingAbe_State_9_Falling();
        case 10:
            return Brain_ChasingAbe_State_10_HungryForBone();
        case 11:
            return Brain_ChasingAbe_State_11_ChasingAfterBone();
        case 12:
            return Brain_ChasingAbe_State_12_WalkingToBone();
        case 13:
            return Brain_ChasingAbe_State_13_EatingBone();
        case 14:
            return Brain_ChasingAbe_State_14_CheckingIfBoneNearby();
        case 15:
            return Brain_ChasingAbe_State_15_ChasingAfterTarget(pTarget);
        case 16:
            return Brain_ChasingAbe_State_16_JumpingUpwards();
        case 17:
            return Brain_ChasingAbe_State_17_WaitingToChase(pTarget);
        case 18:
            return Brain_ChasingAbe_State_18_WaitingToJump(pTarget);
        case 19:
            return Brain_ChasingAbe_State_19_AboutToCollide(pTarget);
        case 20:
            return Brain_ChasingAbe_State_20_Collided(pTarget);
        default:
            return field_122_brain_state_result;
    }
}

s16 Slog::Brain_ChasingAbe_State_19_AboutToCollide(BaseAliveGameObject* pTarget)
{
    FP gridSize = {};

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize(mSpriteScale);
    }
    else
    {
        gridSize = ScaleToGridSize(mSpriteScale);
    }

    if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), FP_FromInteger(2) * gridSize))
    {
        return 2;
    }

    if (pTarget)
    {
        if (VIsObjNearby(ScaleToGridSize(mSpriteScale), pTarget))
        {
            if (pTarget->mHealth > FP_FromInteger(0) && VOnSameYLevel(pTarget))
            {
                SetNextMotion(eSlogMotions::Motion_18_JumpForwards);
                return 4;
            }
        }
    }

    if (!(Slog_NextRandom() % 64))
    {
        SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
        return field_122_brain_state_result;
    }
    return Brain_ChasingAbe_State_20_Collided(pTarget);
}

s16 Slog::Brain_ChasingAbe_State_18_WaitingToJump(BaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }

    field_15A_jump_counter -= 20;

    if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(3), pTarget))
    {
        return 15;
    }
    SetNextMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_17_WaitingToChase(BaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        if (!VIsFacingMe(pTarget))
        {
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return field_122_brain_state_result;
        }
        else
        {
            SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
        }
    }

    if (static_cast<s32>(sGnFrame) <= field_124_timer)
    {
        return field_122_brain_state_result;
    }

    if (!VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(3), pTarget))
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
    }
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_16_JumpingUpwards()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }
    field_15A_jump_counter += Slog_NextRandom() % 64;
    return 15;
}

s16 Slog::Brain_ChasingAbe_State_15_ChasingAfterTarget(BaseAliveGameObject* pTarget)
{
    if (mVelX > FP_FromInteger(0) && HandleEnemyStopper())
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        field_160_flags.Set(Flags_160::eBit1_StopRunning, mVelX < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 20;
    }

    if (!VIsFacingMe(pTarget) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
    }

    if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(3), pTarget))
    {
        if (pTarget->mSpriteScale == FP_FromInteger(1) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
        {
            if (VIsFacingMe(pTarget))
            {
                SetNextMotion(eSlogMotions::Motion_6_StopRunning);
            }
        }
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
    }

    if (mYPos >= pTarget->mYPos + FP_FromInteger(50))
    {
        auto pBone = FindBone();
        if (pBone)
        {
            field_15C_bone_id = pBone->mBaseGameObjectId;
            return 11;
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_4_Fall)
        {
            return 9;
        }

        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return field_122_brain_state_result;
        }

        if (VIsFacingMe(pTarget))
        {
            if (field_15A_jump_counter < 100 && VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(3), pTarget))
            {
                SetNextMotion(eSlogMotions::Motion_19_JumpUpwards);
                return 16;
            }

            if (Math_RandomRange(0, 100) < 20)
            {
                SetCurrentMotion(eSlogMotions::Motion_23_Growl);
            }

            SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
            return 18;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return field_122_brain_state_result;
    }

    field_124_timer = sGnFrame + field_158_chase_delay;
    return 17;
}

s16 Slog::Brain_ChasingAbe_State_14_CheckingIfBoneNearby()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(field_15C_bone_id));
    if (!pBone)
    {
        return 0;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (pBone->VIsFalling())
        {
            field_15C_bone_id = Guid{};
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            return 2;
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
        {
            FP gridSize = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mSpriteScale);
            }

            if (CollisionCheck(mSpriteScale * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            }
            else
            {
                SetNextMotion(eSlogMotions::Motion_1_Walk);
            }
        }

        if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(1), pBone))
        {
            return field_122_brain_state_result;
        }
        return 12;
    }

    field_15C_bone_id = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_13_EatingBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling())
    {
        field_15C_bone_id = Guid{};
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (VIsFacingMe(pBone))
        {
            if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
            {
                SetNextMotion(eSlogMotions::Motion_20_Eating);
            }

            if (field_124_timer > static_cast<s32>(sGnFrame))
            {
                return field_122_brain_state_result;
            }

            pBone->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            field_15C_bone_id = Guid{};
            return 2;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return 12;
    }

    field_15C_bone_id = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_12_WalkingToBone()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle && GetCurrentMotion() != eSlogMotions::Motion_1_Walk)
    {
        return field_122_brain_state_result;
    }

    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling())
    {
        field_15C_bone_id = Guid{};
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        SetNextMotion(eSlogMotions::Motion_1_Walk);
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (VIsFacingMe(pBone))
        {
            if (!VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromDouble(1.5), pBone))
            {
                SetNextMotion(eSlogMotions::Motion_1_Walk);
            }

            if (!VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromDouble(1.5), pBone) || pBone->mVelX > FP_FromInteger(0))
            {
                return field_122_brain_state_result;
            }

            SetNextMotion(eSlogMotions::Motion_0_Idle);
            field_124_timer = sGnFrame + field_156_bone_eating_time;
            return 13;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return field_122_brain_state_result;
    }

    field_15C_bone_id = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_11_ChasingAfterBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling())
    {
        field_15C_bone_id = Guid{};
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (!VIsFacingMe(pBone) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
        {
            SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
        }

        if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(4), pBone))
        {
            if (FP_Abs(mYPos - pBone->mYPos) < FP_FromInteger(50) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
            {
                SetNextMotion(eSlogMotions::Motion_6_StopRunning);
                return 12;
            }
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_1_Walk)
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_4_Fall)
        {
            field_15C_bone_id = Guid{};
            return 9;
        }

        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return field_122_brain_state_result;
        }

        if (VIsFacingMe(pBone))
        {
            FP gridSize = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mSpriteScale);
            }

            // TODO: Same check twice ??
            if (CollisionCheck(mSpriteScale * FP_FromInteger(20), FP_FromInteger(4) * gridSize))
            {
                FP gridSize2 = {};
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    gridSize2 = -ScaleToGridSize(mSpriteScale);
                }
                else
                {
                    gridSize2 = ScaleToGridSize(mSpriteScale);
                }

                if (CollisionCheck(mSpriteScale * FP_FromInteger(20), FP_FromInteger(1) * gridSize2))
                {
                    if (Slog_NextRandom() % 32)
                    {
                        return field_122_brain_state_result;
                    }
                    SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
                    return field_122_brain_state_result;
                }

                SetNextMotion(eSlogMotions::Motion_1_Walk);
                return 12;
            }

            SetNextMotion(eSlogMotions::Motion_2_Run);
            return field_122_brain_state_result;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return field_122_brain_state_result;
    }

    field_15C_bone_id = Guid{};
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_20_Collided(BaseAliveGameObject* pTarget)
{
    auto pBone = FindBone();
    if (pBone)
    {
        field_15C_bone_id = pBone->mBaseGameObjectId;
        return 11;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }

    if (field_160_flags.Get(Flags_160::eBit1_StopRunning))
    {
        if (pTarget->mXPos > mXPos)
        {
            return 2;
        }
    }
    else
    {
        if (pTarget->mXPos < mXPos)
        {
            return 2;
        }
    }

    if (static_cast<s32>(sGnFrame) > field_150_growl_timer)
    {
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        SetCurrentMotion(eSlogMotions::Motion_23_Growl);
        SetNextMotion(eSlogMotions::Motion_0_Idle);
    }

    if (static_cast<s32>(sGnFrame) <= field_14C_scratch_timer)
    {
        return field_122_brain_state_result;
    }

    field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
    SetCurrentMotion(eSlogMotions::Motion_22_Scratch);
    SetNextMotion(eSlogMotions::Motion_0_Idle);
    return field_122_brain_state_result;
}

s16 Slog::Brain_ChasingAbe_State_10_HungryForBone()
{
    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        SetNextMotion(eSlogMotions::Motion_19_JumpUpwards);
    }

    if (static_cast<s32>(sGnFrame) <= field_124_timer)
    {
        auto pBone = FindBone();
        if (pBone)
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
            field_15C_bone_id = pBone->mBaseGameObjectId;
            return 11;
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_6_StopRunning)
        {
            return field_122_brain_state_result;
        }

        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return field_122_brain_state_result;
        }

        if (Slog_NextRandom() % 16)
        {
            return field_122_brain_state_result;
        }

        SetNextMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
        return field_122_brain_state_result;
    }

    field_160_flags.Clear(Flags_160::eBit4_Hungry);
    SetNextMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_9_Falling()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }
    SetCurrentMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_8_ToIdle()
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }
    field_142_anger_level = 0;
    field_120_brain_state_idx = 1;
    field_11C_biting_target = 0;
    return 0;
}

s16 Slog::Brain_ChasingAbe_State_7_EatingTarget(BaseAliveGameObject* pTarget)
{
    if (static_cast<s32>(sGnFrame) <= field_124_timer && pTarget->mAnim.mFlags.Get(AnimFlags::eBit3_Render))
    {
        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return field_122_brain_state_result;
        }

        SetCurrentMotion(eSlogMotions::Motion_20_Eating);
        return field_122_brain_state_result;
    }

    SetNextMotion(eSlogMotions::Motion_0_Idle);
    return 8;
}

s16 Slog::Brain_ChasingAbe_State_4_LungingAtTarget(BaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        if (pTarget->mHealth > FP_FromInteger(0))
        {
            field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
            return 1;
        }

        if (FP_Abs(pTarget->mXPos - mXPos) > ScaleToGridSize(mSpriteScale) * FP_FromInteger(2))
        {
            field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
            return 1;
        }

        if (FP_Abs(pTarget->mYPos - mYPos) > ScaleToGridSize(mSpriteScale) * FP_FromInteger(2))
        {
            field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
            return 1;
        }

        if (VIsFacingMe(pTarget))
        {
            SetCurrentMotion(eSlogMotions::Motion_20_Eating);
            SetNextMotion(eSlogMotions::m1);
        }
        else
        {
            SetCurrentMotion(eSlogMotions::Motion_3_TurnAround);
            SetNextMotion(eSlogMotions::Motion_20_Eating);
        }

        field_124_timer = sGnFrame + 90;
        return 7;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_4_Fall)
    {
        return field_122_brain_state_result;
    }

    return 9;
}

s16 Slog::Brain_ChasingAbe_State_3_GrowlOrScratch(BaseAliveGameObject* pTarget)
{
    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return field_122_brain_state_result;
    }

    if (Slog_NextRandom() % 64)
    {
        if (static_cast<s32>(sGnFrame) > field_150_growl_timer)
        {
            field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
            SetCurrentMotion(eSlogMotions::Motion_23_Growl);
            SetNextMotion(eSlogMotions::Motion_0_Idle);
        }

        if (static_cast<s32>(sGnFrame) > field_14C_scratch_timer)
        {
            field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
            SetCurrentMotion(eSlogMotions::Motion_22_Scratch);
            SetNextMotion(eSlogMotions::Motion_0_Idle);
        }

        if (pTarget->mSpriteScale != FP_FromInteger(1))
        {
            return field_122_brain_state_result;
        }
        return 2;
    }

    SetCurrentMotion(eSlogMotions::Motion_5_MoveHeadUpwards);
    return field_122_brain_state_result;
}

s16 Slog::Brain_ChasingAbe_State_2_Thinking(BaseAliveGameObject* pTarget)
{
    if (mVelX > FP_FromInteger(0) && HandleEnemyStopper())
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        field_160_flags.Set(Flags_160::eBit1_StopRunning, mVelX < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 20;
    }

    if (CollisionCheck(mSpriteScale * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        field_160_flags.Set(Flags_160::eBit1_StopRunning, mVelX < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 19;
    }

    if (!VIsFacingMe(pTarget) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
    }

    if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(3), pTarget))
    {
        if (VOnSameYLevel(pTarget))
        {
            if (VIsFacingMe(pTarget))
            {
                if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), pTarget->mXPos - mXPos) && !field_160_flags.Get(Flags_160::eBit9_MovedOffScreen))
                {
                    if (pTarget->mHealth <= FP_FromInteger(0))
                    {
                        SetNextMotion(eSlogMotions::Motion_0_Idle);
                    }
                    else
                    {
                        SetNextMotion(eSlogMotions::Motion_18_JumpForwards);
                    }
                }
            }
        }
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
    }

    auto pBone = FindBone();
    if (pBone)
    {
        field_15C_bone_id = pBone->mBaseGameObjectId;
        return 11;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        if (VIsFacingMe(pTarget))
        {
            FP gridSizeDirected = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(mSpriteScale);
            }

            if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), FP_FromInteger(2) * gridSizeDirected))
            {
                if (pTarget->mHealth > FP_FromInteger(0))
                {
                    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->mXPos - mXPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->mYPos - mYPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
                    return 1;
                }
                field_124_timer = sGnFrame + 90;
                return 7;
            }

            field_160_flags.Set(Flags_160::eBit1_StopRunning, mAnim.mFlags.Get(AnimFlags::eBit5_FlipX));

            field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
            field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
            return 19;
        }
        SetCurrentMotion(eSlogMotions::Motion_3_TurnAround);
    }

    if (field_160_flags.Get(Flags_160::eBit4_Hungry) && IsActiveHero(pTarget) && pTarget->mScale == mScale && (sActiveHero->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold_455DF0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold_454410))
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        field_124_timer = sGnFrame + 90;
        return 10;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_4_Fall)
    {
        return 9;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_18_JumpForwards)
    {
        return 4;
    }

    if (mYPos <= pTarget->mYPos + FP_FromInteger(50))
    {
        if (pTarget->mSpriteScale != FP_FromDouble(0.5))
        {
            return field_122_brain_state_result;
        }
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
        return 3;
    }

    return 15;
}

s16 Slog::Brain_ChasingAbe_State_1_Waiting()
{
    if (field_124_timer > static_cast<s32>(sGnFrame))
    {
        return field_122_brain_state_result;
    }
    SetNextMotion(eSlogMotions::Motion_2_Run);
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_0_Init()
{
    field_11C_biting_target = 0;
    field_15A_jump_counter = 0;
    field_15C_bone_id = Guid{};
    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
    Sfx(SlogSound::AttackGrowl_8);
    return 1;
}

s16 Slog::Brain_3_Death()
{
    field_138_listening_to_slig_id = Guid{};
    field_118_target_id = Guid{};

    if (field_124_timer < static_cast<s32>(sGnFrame + 80))
    {
        mSpriteScale -= FP_FromDouble(0.023);
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_124_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (mSpriteScale < FP_FromInteger(0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

u8** Slog::ResBlockForMotion(s16 motion)
{
    const auto slogMotion = static_cast<eSlogMotions>(motion);
    if (slogMotion < eSlogMotions::Motion_14_AngryBark)
    {
        field_130_motion_resource_block_index = 0;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (slogMotion < eSlogMotions::Motion_18_JumpForwards)
    {
        field_130_motion_resource_block_index = 1;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (slogMotion < eSlogMotions::Motion_21_Dying)
    {
        field_130_motion_resource_block_index = 2;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (slogMotion < eSlogMotions::Motion_22_Scratch)
    {
        field_130_motion_resource_block_index = 3;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }

    if (motion < 24) // last + 1
    {
        field_130_motion_resource_block_index = 4;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }
    else
    {
        field_130_motion_resource_block_index = 0;
        return field_10_resources_array.ItemAt(field_130_motion_resource_block_index);
    }
}

void Slog::SetAnimFrame()
{
    mAnim.Set_Animation_Data(GetAnimRes(sSlogAnimIdTable[mCurrentMotion]));
}

const TintEntry sSlogTints_560A48[] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

void Slog::Init()
{
    SetType(ReliveTypes::eSlog);
    Animation_Init(GetAnimRes(AnimId::Slog_Idle));

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_160_flags.Clear(Flags_160::eBit3_Shot);
    field_160_flags.Clear(Flags_160::eBit6_HitByAbilityRing);
    field_160_flags.Set(Flags_160::eBit4_Hungry);

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mAnim.mFnPtrArray = kSlog_Anim_Frame_Fns_55EFBC;
    field_124_timer = 0;
    field_122_brain_state_result = 0;
    SetNextMotion(eSlogMotions::m1);
    field_130_motion_resource_block_index = 0;
    BaseAliveGameObject_PlatformId = Guid{};
    field_138_listening_to_slig_id = Guid{};
    field_118_target_id = Guid{};
    field_15C_bone_id = Guid{};
    SetTint(&sSlogTints_560A48[0], gMap.mCurrentLevel);
    mAnim.mRenderLayer = Layer::eLayer_SlogFleech_34;

    if (mSpriteScale == FP_FromInteger(1))
    {
        mYOffset = 1;
    }
    else
    {
        mYOffset = 2;
    }

    mScale = Scale::Fg;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos, mYPos,
            mXPos, mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine, &hitX, &hitY, CollisionMask(eFloor_0, eDynamicCollision_32))
        == 1)
    {
        mYPos = hitY;
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(xy, wh, ObjList_5C1B78, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }

    MapFollowMe(FALSE);
    mShadow = relive_new Shadow();

    sSlogCount++;

    VStackOnObjectsOfType(ReliveTypes::eSlog);
}

void Slog::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            sCollisions->Raycast(
                mXPos,
                mYPos - FP_FromInteger(20),
                mXPos,
                mYPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mXPos,
                &mYPos,
                CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));
        }
        BaseAliveGameObjectCollisionLineType = 0;
        field_118_target_id = BaseGameObject::RefreshId(field_118_target_id);
        field_138_listening_to_slig_id = BaseGameObject::RefreshId(field_138_listening_to_slig_id);
        field_15C_bone_id = BaseGameObject::RefreshId(field_15C_bone_id);
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (FP_Abs(mXPos - sControlledCharacter->mXPos) > FP_FromInteger(750) || FP_Abs(mYPos - sControlledCharacter->mYPos) > FP_FromInteger(390))
    {
        mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        if (mHealth > FP_FromInteger(0))
        {
            mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);
        }

        const auto oldMotion = mCurrentMotion;
        field_122_brain_state_result = (this->*sSlogBrainTable[field_120_brain_state_idx])();
        if (sDDCheat_ShowAI_Info)
        {
            DDCheat::DebugStr("Slog:  Motion=%d  BrainState=%d\n", mCurrentMotion, field_122_brain_state_result);
        }

        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;

        (this->*sSlogMotionTable[mCurrentMotion])();

        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            BaseAliveGameObjectPathTLV = sPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion != mCurrentMotion)
        {
            SetAnimFrame();
        }
    }
}

Slog::~Slog()
{
    field_118_target_id = Guid{};
    field_138_listening_to_slig_id = Guid{};
    field_15C_bone_id = Guid{};

    if (field_12C_tlvInfo != Guid{})
    {
        Path::TLV_Reset(field_12C_tlvInfo, -1, 0, mHealth <= FP_FromInteger(0));
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (!field_160_flags.Get(Flags_160::eBit3_Shot))
    {
        sSlogCount--;
    }
}


void Slog::ToIdle()
{
    MapFollowMe(FALSE);
    field_128_falling_velx_scale_factor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetCurrentMotion(eSlogMotions::Motion_0_Idle);
    SetNextMotion(eSlogMotions::m1);
}

const relive::SfxDefinition& getSfxDef(SlogSound effectId)
{
    return sSlogSFXList[static_cast<s32>(effectId)];
}

void Slog::Sfx(SlogSound effectId)
{
    s16 volumeLeft = 0;
    s16 volumeRight = 0;
    const relive::SfxDefinition& effectDef = getSfxDef(effectId);


    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);
    PSX_RECT pRect = {};
    gMap.Get_Camera_World_Rect(direction, &pRect);

    const s16 defaultSndIdxVol = effectDef.field_C_default_volume;
    volumeRight = defaultSndIdxVol;
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volumeLeft = volumeRight;
            break;
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
            volumeRight = volumeLeft;
        }
        break;
        case CameraPos::eCamLeft_3:
        {
            const FP percentHowFar = (FP_FromInteger(pRect.w) - mXPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
        }
        break;
        case CameraPos::eCamRight_4:
        {
            const FP percentHowFar = (mXPos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
        }
        break;
        default:
            return;
    }

    if (mSpriteScale == FP_FromDouble(0.5))
    {
        SFX_SfxDefinition_Play_Stereo(
            effectDef,
            volumeLeft,
            volumeRight,
            effectDef.field_E_pitch_min + 1524,
            effectDef.field_10_pitch_max + 1524);
    }
    else
    {
        SFX_SfxDefinition_Play_Stereo(
            effectDef,
            volumeLeft,
            volumeRight,
            effectDef.field_E_pitch_min,
            effectDef.field_10_pitch_max);
    }
}

void Slog::ToJump()
{
    mVelX = (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-10.18) : FP_FromDouble(10.18)) * mSpriteScale;
    mVelY = FP_FromInteger(-8) * mSpriteScale;

    BaseAliveGameObjectLastLineYPos = mYPos;

    VOnTrapDoorOpen();

    SetCurrentMotion(eSlogMotions::Motion_18_JumpForwards);
    BaseAliveGameObjectCollisionLine = nullptr;

    Sfx(SlogSound::AttackGrowl_8);

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_7, this, 0, 0);
    }
}

s16 Slog::ToNextMotion()
{
    const auto slogMotion = static_cast<eSlogMotions>(mNextMotion);
    switch (slogMotion)
    {
        case eSlogMotions::Motion_3_TurnAround:
            SetCurrentMotion(eSlogMotions::Motion_3_TurnAround);
            SetNextMotion(eSlogMotions::m1);
            return 1;

        case eSlogMotions::Motion_1_Walk:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
            }
            else
            {
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
            }

            if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), (mVelX * FP_FromInteger(9)) * FP_FromInteger(2)))
            {
                SetCurrentMotion(eSlogMotions::Motion_8_StartWalking);
                SetNextMotion(eSlogMotions::m1);
                return 1;
            }
            ToIdle();
            return 0;

        case eSlogMotions::Motion_2_Run:
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(3));
            }
            else
            {
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(3));
            }

            if (!CollisionCheck(mSpriteScale * FP_FromInteger(20), mVelX * FP_FromInteger(4)))
            {
                SetCurrentMotion(eSlogMotions::Motion_2_Run);
                SetNextMotion(eSlogMotions::m1);
                return 1;
            }
            ToIdle();
            return 0;
    }
    return 0;
}

bool Slog::CollisionCheck(FP hitY, FP hitX)
{
    PathLine* pLine = nullptr;
    return sCollisions->Raycast(mXPos, mYPos - hitY, mXPos + hitX, mYPos - hitY, &pLine, &hitX, &hitY, CollisionMask(eWallRight_2, eWallLeft_1)) != 0;
}

void Slog::MoveOnLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    const FP oldXPos = mXPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = Guid{};
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                VOnCollisionWith(xy, wh, ObjList_5C1B78, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
            BaseAliveGameObjectLastLineYPos = mYPos;
            mXPos = oldXPos + mVelX;
        }
    }
    else
    {
        field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
        BaseAliveGameObjectLastLineYPos = mYPos;
        SetCurrentMotion(eSlogMotions::Motion_4_Fall);
    }
}

Bone* Slog::FindBone()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eBone)
        {
            auto pBone = static_cast<Bone*>(pObj);
            if (pBone->VCanThrow())
            {
                if (gMap.Is_Point_In_Current_Camera(pBone->mCurrentLevel, pBone->mCurrentPath, pBone->mXPos, pBone->mYPos, 0) && pBone->mScale == mScale)
                {
                    if (FP_Abs(mYPos - pBone->mYPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
                    {
                        return pBone;
                    }
                }
            }
        }
    }
    return nullptr;
}

BaseAliveGameObject* Slog::FindTarget(s16 bKillSligs, s16 bLookingUp)
{
    PSX_RECT bRect = VGetBoundingRect();

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        bRect.x -= 368;
    }
    else
    {
        bRect.w += 368;
    }

    if (bLookingUp)
    {
        bRect.x -= 368;
        bRect.w += 368;
        bRect.y -= gPsxDisplay.mHeight;
        bRect.h += gPsxDisplay.mHeight;
    }

    FP distanceToLastFoundObj = FP_FromInteger(gPsxDisplay.mWidth);
    auto pSligBeingListendTo = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_138_listening_to_slig_id));

    BaseAliveGameObject* pBestObj = nullptr;
    BaseAliveGameObject* pLastFoundObj = nullptr;

    s32 array_idx = 0;
    Guid local_array[10] = {};

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSlog)
        {
            auto pSlog = static_cast<Slog*>(pObj);
            if (pSlog->field_118_target_id != Guid{} && array_idx < ALIVE_COUNTOF(local_array))
            {
                local_array[array_idx++] = pSlog->field_118_target_id;
            }
        }

        if (pObj != pSligBeingListendTo && pObj != this && pObj->mScale == mScale)
        {
            switch (pObj->Type())
            {
                case ReliveTypes::eCrawlingSlig:
                case ReliveTypes::eFlyingSlig:
                case ReliveTypes::eGlukkon:
                case ReliveTypes::eAbe:
                case ReliveTypes::eMudokon:
                case ReliveTypes::eSlig:
                    if (bKillSligs || (!bKillSligs && (pObj->Type() == ReliveTypes::eAbe || pObj->Type() == ReliveTypes::eCrawlingSlig || pObj->Type() == ReliveTypes::eFlyingSlig || pObj->Type() == ReliveTypes::eGlukkon || (pObj->Type() == ReliveTypes::eMudokon && static_cast<Mudokon*>(pObj)->field_18E_brain_state == Mud_Brain_State::Brain_4_ListeningToAbe))))
                    {
                        const PSX_RECT objRect = pObj->VGetBoundingRect();

                        if (objRect.x <= bRect.w && objRect.w >= bRect.x && objRect.h >= bRect.y && objRect.y <= bRect.h)
                        {
                            pLastFoundObj = pObj;

                            const FP xDelta = FP_Abs(mXPos - pObj->mXPos);
                            if (xDelta < distanceToLastFoundObj)
                            {
                                s32 array_idx_iter = 0;
                                if (array_idx)
                                {
                                    // Something to do with finding who is the last attacker
                                    while (local_array[array_idx_iter] != pObj->mBaseGameObjectId)
                                    {
                                        if (++array_idx_iter >= array_idx)
                                        {
                                            distanceToLastFoundObj = xDelta;
                                            pBestObj = pObj;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    distanceToLastFoundObj = xDelta;
                                    pBestObj = pObj;
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (pBestObj)
    {
        return pBestObj;
    }

    if (pLastFoundObj)
    {
        return pLastFoundObj;
    }

    return nullptr;
}

void Slog::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        SetCurrentMotion(eSlogMotions::Motion_4_Fall);
    }
}

void Slog::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            mHealth = FP_FromInteger(0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        pTlv = sPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

s16 Slog::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            auto pBullet = static_cast<Bullet*>(pFrom);
            switch (pBullet->mBulletType)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                    if (pBullet->mXDistance <= FP_FromInteger(0))
                    {
                        relive_new Blood(mXPos,
                                                    pBullet->mYPos,
                                                    FP_FromInteger(-24),
                                                    FP_FromInteger(0),
                                                    mSpriteScale, 50);
                    }
                    else
                    {
                        relive_new Blood(mXPos,
                                                    pBullet->mYPos,
                                                    FP_FromInteger(24),
                                                    FP_FromInteger(0),
                                                    mSpriteScale, 50);
                    }
                    break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    relive_new Blood(mXPos,
                                                mYPos - (FP_FromInteger(20) * mSpriteScale),
                                                FP_FromInteger(0),
                                                FP_FromInteger(0),
                                                mSpriteScale, 50);
                    break;
                }

                default:
                    break;
            }

            Sfx(SlogSound::DeathWhine_9);
            mHealth = FP_FromInteger(0);
            field_120_brain_state_idx = 3;
            SetCurrentMotion(eSlogMotions::Motion_21_Dying);
            field_124_timer = sGnFrame + 90;
            SetAnimFrame();
            mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
            field_160_flags.Set(Flags_160::eBit3_Shot);
            sSlogCount--;
            break;
        }

        case ReliveTypes::eDrill:
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eExplosion:
        {
            Sfx(SlogSound::DeathWhine_9);
            mHealth = FP_FromInteger(0);
            relive_new Gibs(GibType::Slog_2, mXPos, mYPos, mVelX, mVelY, mSpriteScale, 0);

            const PSX_RECT bRect = VGetBoundingRect();
            relive_new Blood(FP_FromInteger((bRect.x + bRect.w) / 2),
                                        FP_FromInteger((bRect.y + bRect.h) / 2),
                                        FP_FromInteger(0),
                                        FP_FromInteger(0),
                                        mSpriteScale, 50);

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            break;
        }

        case ReliveTypes::eElectricWall:
            Sfx(SlogSound::DeathWhine_9);
            field_160_flags.Set(Flags_160::eBit13_Unused);
            break;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
            Sfx(SlogSound::DeathWhine_9);
            mHealth = FP_FromInteger(0);
            field_120_brain_state_idx = 3;
            SetCurrentMotion(eSlogMotions::Motion_21_Dying);
            field_124_timer = sGnFrame + 90;
            SetAnimFrame();
            mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
            break;

        case ReliveTypes::eAbilityRing:
            if (!field_160_flags.Get(Flags_160::eBit6_HitByAbilityRing))
            {
                field_160_flags.Set(Flags_160::eBit6_HitByAbilityRing);
                Sfx(SlogSound::DeathWhine_9);
            }
            break;

        case ReliveTypes::eElectrocute:
            mHealth = FP_FromInteger(0);
            field_120_brain_state_idx = 3;
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            break;

        default:
            return 1;
    }
    return 1;
}

void Slog::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    field_142_anger_level += field_148_chase_anger; // on throwable hit?
}

s16 Slog::PlayerOrNakedSligNear()
{
    const FP kMinXDist = FP_FromInteger(100) * mSpriteScale;
    const FP kMinYDist = FP_FromInteger(25) * mSpriteScale;

    // Is the player near?
    if (FP_Abs(sActiveHero->mXPos - mXPos) < kMinXDist && FP_Abs(sActiveHero->mYPos - mYPos) < kMinYDist && sActiveHero->mSpriteScale == FP_FromInteger(1))
    {
        return 1;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eCrawlingSlig)
        {
            // Is this naked slig near?
            if (FP_Abs(pObj->mXPos - mXPos) < kMinXDist && FP_Abs(pObj->mYPos - mYPos) < kMinYDist && pObj->mSpriteScale == mSpriteScale)
            {
                return 1;
            }
        }
    }
    return 0;
}

void Slog::DelayedResponse(s16 responseIdx)
{
    field_140_response_part = 0;
    field_13E_response_index = responseIdx;
    field_124_timer = sGnFrame + 10;
}

s16 Slog::HandleEnemyStopper()
{
    FP xPos = ScaleToGridSize(mSpriteScale) * FP_FromInteger(2);
    if (mVelX >= FP_FromInteger(0))
    {
        xPos += mXPos;
    }
    FP width = mXPos;
    if (mXPos <= xPos)
    {
        width = xPos;
    }
    FP xToUse = xPos;
    if (mXPos <= xPos)
    {
        xToUse = mXPos;
    }

    const auto stopperPath = static_cast<relive::Path_EnemyStopper*>(
        sPathInfo->TLV_Get_At_4DB4B0(
            FP_GetExponent(xToUse), FP_GetExponent(mYPos),
            FP_GetExponent(width), FP_GetExponent(mYPos), ReliveTypes::eEnemyStopper));

    return stopperPath != nullptr && stopperPath->mStopDirection == (mVelX > FP_FromInteger(0) ? relive::Path_EnemyStopper::StopDirection::Right : relive::Path_EnemyStopper::StopDirection::Left) && SwitchStates_Get(stopperPath->mSwitchId) > 0;
}

s16 Slog::Facing(FP xpos)
{
    if (mXPos < xpos && !mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    if (xpos < mXPos && mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        return TRUE;
    }

    return FALSE;
}
