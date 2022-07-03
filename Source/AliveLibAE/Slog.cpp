#include "stdafx.h"
#include "Slog.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "MusicController.hpp"
#include "Collisions.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "DDCheat.hpp"
#include "Shadow.hpp"
#include "Map.hpp"
#include "Sound/Midi.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Bone.hpp"
#include "Sfx.hpp"
#include "Blood.hpp"
#include "SnoozeParticle.hpp"
#include "Gibs.hpp"
#include "Bullet.hpp"
#include "Particle.hpp"
#include "GameSpeak.hpp"
#include "SwitchStates.hpp"
#include "PsxDisplay.hpp"
#include "Mudokon.hpp"
#include "Grid.hpp"
#include "Function.hpp"

ALIVE_VAR(1, 0xBAF7F2, s16, sSlogCount, 0);

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

const SfxDefinition sSlogSFXList[19] = {
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
    field_134_last_event_index = -1;

    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    Init();

    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);
    field_12C_tlvInfo = 0xFFFF;
    field_120_brain_state_idx = 2;
    field_122_brain_state_result = 0;

    BaseAliveGameObject* pTarget = FindTarget(0, 0);
    if (!pTarget)
    {
        pTarget = sControlledCharacter_5C1B8C;
    }
    field_118_target_id = pTarget->field_8_object_id;

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

Slog::Slog(Path_Slog* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(5)
{
    field_134_last_event_index = -1;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(1);
    }

    mBaseGameObjectTlvInfo = tlvInfo;

    Init();

    field_160_flags.Clear(Flags_160::eBit9_MovedOffScreen);
    field_160_flags.Set(Flags_160::eBit2_ListenToSligs);
    field_160_flags.Set(Flags_160::eBit7_Asleep, pTlv->field_14_asleep == Choice_short::eYes_1);
    field_160_flags.Clear(Flags_160::eBit5_CommandedToAttack);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_direction == XDirection_short::eLeft_0);

    field_12C_tlvInfo = tlvInfo;
    mBaseGameObjectTlvInfo = tlvInfo;
    field_120_brain_state_idx = 1;
    field_118_target_id = -1;
    field_144_wake_up_anger = pTlv->field_16_wake_up_anger;
    field_146_total_anger = pTlv->field_16_wake_up_anger + pTlv->field_18_bark_anger;
    field_148_chase_anger = field_146_total_anger + pTlv->field_1A_chase_anger;
    field_158_chase_delay = pTlv->field_1C_chase_delay;
    field_154_anger_switch_id = pTlv->field_20_anger_switch_id;
    field_156_bone_eating_time = pTlv->field_22_bone_eating_time;

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

    pState->field_8_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_10_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_14_vely = mBaseAnimatedWithPhysicsGameObject_VelY;

    pState->field_50_falling_velx_scale_factor = field_128_falling_velx_scale_factor;

    pState->field_18_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_1A_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pState->field_1C_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    pState->mRingRed = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pState->mRingGreen = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pState->mRingBlue = mBaseAnimatedWithPhysicsGameObject_RGB.b;

    pState->field_26_bAnimFlipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_anim_cur_frame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    pState->field_2C_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;
    pState->field_2F_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2E_bRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
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
    pState->field_74_flags.Set(Slog_State::eBit2_Possessed, sControlledCharacter_5C1B8C == this); // Lol can't be possessed anyway so ??
    pState->field_40_tlvInfo = field_12C_tlvInfo;
    pState->field_40_tlvInfo = field_12C_tlvInfo;
    pState->field_44_obj_id = -1;

    if (field_118_target_id != -1)
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
    pState->field_54_obj_id = -1;

    if (field_138_listening_to_slig_id != -1)
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
    pState->field_6C_bone_id = -1;

    if (field_15C_bone_id != -1)
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
    pState->field_74_flags.Set(Slog_State::eBit2_Possessed, sControlledCharacter_5C1B8C == this); // Can never happen so is always 0
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
    AnimId::Slog_Growl};


s32 Slog::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Slog_State*>(pBuffer);
    auto pTlv = static_cast<Path_Slog*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_40_tlvInfo));
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kDogbasicResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLOG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("DOGKNFD.BAN", nullptr);
    }

    Slog* pSlog = nullptr;
    if (pState->field_40_tlvInfo == 0xFFFF)
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
    pSlog->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_8_xpos;
    pSlog->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_C_ypos;
    pSlog->mBaseAnimatedWithPhysicsGameObject_VelX = pState->field_10_velx;
    pSlog->mBaseAnimatedWithPhysicsGameObject_VelY = pState->field_14_vely;
    pSlog->field_128_falling_velx_scale_factor = pState->field_50_falling_velx_scale_factor;
    pSlog->mBaseAnimatedWithPhysicsGameObject_PathNumber = pState->field_18_path_number;
    pSlog->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pState->field_1A_lvl_number);
    pSlog->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pState->field_1C_sprite_scale;
    pSlog->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pState->mRingRed, pState->mRingGreen, pState->mRingBlue);

    pSlog->SetCurrentMotion(pState->field_28_current_motion);
    u8** ppRes = pSlog->ResBlockForMotion(pState->field_28_current_motion);
    pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sSlogAnimIdTable[pSlog->mCurrentMotion], ppRes);

    pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame = pState->field_2A_anim_cur_frame;
    pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pState->field_2C_frame_change_counter;

    pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bAnimFlipX & 1);
    pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

    pSlog->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_bDrawable & 1);

    if (IsLastFrame(&pSlog->mBaseAnimatedWithPhysicsGameObject_Anim))
    {
        pSlog->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
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
                if (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
                {
                    SND_SEQ_PlaySeq(SeqId::Empty_13, 1, 0);
                }

                if (gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) >= CameraPos::eCamCurrent_0)
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogWalkVelXTable_5475EC[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame];
    }
    else
    {
        velX = sSlogWalkVelXTable_5475EC[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame];
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * velX);

    if (CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4)))
    {
        ToIdle();

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX + (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos -= (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(2)) - mBaseAnimatedWithPhysicsGameObject_VelX;
        }
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eSlogMotions::Motion_1_Walk)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 2 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 11)
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
            else if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 14)
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
    if (gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eIntenseChase_7, this, 0, 0);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * -sSlogRunVelXTable_547634[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * sSlogRunVelXTable_547634[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame]);
    }

    if (CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4)))
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

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 7)
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToIdle();
    }
}

void Slog::Motion_4_Fall()
{
    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8));
    if (mBaseAnimatedWithPhysicsGameObject_VelY > (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20));
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX > (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        if (mBaseAnimatedWithPhysicsGameObject_VelX < -(FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = -(FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX -= (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_128_falling_velx_scale_factor);
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_128_falling_velx_scale_factor);
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }

    const FP xposBeforeChange = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP yposBeforeChange = mBaseAnimatedWithPhysicsGameObject_YPos;

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            xposBeforeChange,
            yposBeforeChange - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)),
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &pLine, &hitX, &hitY, CollisionMask(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32)))
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                MapFollowMe(FALSE);
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) + 5)};
                    VOnCollisionWith(
                        xy,
                        wh,
                        ObjList_5C1B78,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
                }
                SetCurrentMotion(eSlogMotions::Motion_10_Land);
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX - mBaseAnimatedWithPhysicsGameObject_VelX;
                MapFollowMe(FALSE);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                break;

            default:
                return;
        }
    }
}

void Slog::Motion_5_MoveHeadUpwards()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::IdleWoof_2);
        field_132_has_woofed = 1;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::Skid_14);
    }

    FP velX = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogStopRunningVelX_547658[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame];
    }
    else
    {
        velX = sSlogStopRunningVelX_547658[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame];
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * velX);

    if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4)))
    {
        MoveOnLine();
        if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -sSlogSlideTurnVelXTable_547684[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame];
    }
    else
    {
        velX = sSlogSlideTurnVelXTable_547684[mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame];
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * velX);
    if (CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4)))
    {
        ToIdle();
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eSlogMotions::Motion_7_SlideTurn)
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe(FALSE);

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                    mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
                }
                else
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                    mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
                }
                SetCurrentMotion(eSlogMotions::Motion_2_Run);
            }
        }
    }
}

void Slog::Motion_8_StartWalking()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_1_Walk);
    }

    MoveOnLine();
}

void Slog::Motion_9_EndWalking()
{
    MoveOnLine();

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_10_Land()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::Landing_16);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_11_Unused()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_12_StartFastBarking);
    }
}

void Slog::Motion_12_StartFastBarking()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        Sfx(SlogSound::IdleWoof_2);
        field_132_has_woofed = 1;
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(eSlogMotions::Motion_13_EndFastBarking);
        }
    }
}

void Slog::Motion_13_EndFastBarking()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void Slog::Motion_14_AngryBark()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0 || mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 6)
    {
        Sfx(SlogSound::CautiousWoof_5);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }
}

void Slog::Motion_15_Sleeping()
{
    if (GetNextMotion() != eSlogMotions::m1 && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
        if (gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
        {
            FP xOff = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(18));
            }
            else
            {
                xOff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(18));
            }
            relive_new SnoozeParticle(xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
                                                          (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(-13)) + mBaseAnimatedWithPhysicsGameObject_YPos,
                                                          mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer, mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale);
        }
    }
}

void Slog::Motion_16_MoveHeadDownwards()
{
    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
            Sfx(SlogSound::IdleGrrAlt_4);
        }
    }

    if (gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogTension_5, this, 0, 0);
    }
}

void Slog::Motion_18_JumpForwards()
{
    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.8));

    const FP k20Scaled = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20);
    if (mBaseAnimatedWithPhysicsGameObject_VelY > k20Scaled)
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = k20Scaled;
    }

    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP oldYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(oldXPos, oldYPos - k20Scaled, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, &pLine, &hitX, &hitY, CollisionMask(eCeiling_3, eWallRight_2, eWallLeft_1, eFloor_0, eDynamicCollision_32)
) == 1)
    {
        switch (pLine->mLineType) // TODO: line enum
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    SetNextMotion(eSlogMotions::m1);
                    SetCurrentMotion(eSlogMotions::Motion_2_Run);
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                }
                break;

            case 1u:
            case 5u:
                if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
                }
                break;

            case 2u:
            case 6u:
                if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
                }
                break;

            default:
                break;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_YPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(2))
    {
        field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame < 12)
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(24 - mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame);
        }
    }

    if (gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSlogChaseTension_6, this, 0, 0);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 5)
    {
        if (field_160_flags.Get(Flags_160::eBit4_Hungry) && field_118_target_id == sActiveHero->field_8_object_id && sActiveHero->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale && (sActiveHero->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold_455DF0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold_454410))
        {
            Sfx(SlogSound::HungryYip_13);
        }
        else
        {
            Sfx(SlogSound::JumpBite_6);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetCurrentMotion(eSlogMotions::Motion_0_Idle);
        SetNextMotion(eSlogMotions::m1);
    }
}

void Slog::Motion_20_Eating()
{
    SND_SEQ_Stop(SeqId::Empty_13);
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards);
        if (GetNextMotion() != eSlogMotions::m1 && GetNextMotion() != eSlogMotions::Motion_20_Eating)
        {
            SetCurrentMotion(eSlogMotions::Motion_0_Idle);
            return;
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3 && !mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit19_LoopBackwards))
    {
        SfxPlayMono(static_cast<SoundEffect>(Math_RandomRange(SoundEffect::Eating1_65, SoundEffect::Eating2_66)), 100);
        relive_new Blood(((mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) != 0 ? FP_FromInteger(-25) : FP_FromInteger(25)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale + mBaseAnimatedWithPhysicsGameObject_XPos,
                      mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(4) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                      FP_FromInteger(0), FP_FromInteger(0),
                      mBaseAnimatedWithPhysicsGameObject_SpriteScale, 12);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (Math_RandomRange(0, 100) < 85)
        {
            if (static_cast<s32>(sGnFrame) > field_150_growl_timer && Math_RandomRange(0, 100) < 60)
            {
                field_150_growl_timer = sGnFrame + 16;
                Sfx(SlogSound::IdleGrrr_3);
            }
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit19_LoopBackwards);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit19_LoopBackwards);
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        SND_SEQ_PlaySeq(SeqId::Scratching_12, 1, 1);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SetCurrentMotion(mNextMotion);
            SetNextMotion(eSlogMotions::m1);
        }
    }
}

void Slog::Motion_23_Growl()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3)
    {
        if (field_132_has_woofed)
        {
            Sfx(SlogSound::IdleGrrr_3);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
            field_132_has_woofed = 0;
            field_150_growl_timer = sGnFrame + 12;
        }
    }

    if (static_cast<s32>(sGnFrame) > field_150_growl_timer)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
    }

    if (GetNextMotion() != eSlogMotions::m1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
        field_138_listening_to_slig_id = -1;
        field_118_target_id = -1;
        field_120_brain_state_idx = 1;
        return 0;
    }

    FP gridSize1Directed = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(1);
    if (pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize1Directed = -gridSize1Directed;
    }

    const FP xpos1GridAHead = gridSize1Directed + pObj->mBaseAnimatedWithPhysicsGameObject_XPos;
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
        if (FP_Abs(xpos1GridAHead - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3)))
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

    if (FP_Abs(xpos1GridAHead - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
    {
        SetNextMotion(eSlogMotions::Motion_2_Run);
        return 4;
    }

    if (FP_Abs(xpos1GridAHead - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3)))
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
                field_138_listening_to_slig_id = -1;
                field_160_flags.Set(Flags_160::eBit5_CommandedToAttack);
                field_118_target_id = pTarget->field_8_object_id;
                field_120_brain_state_idx = 2;
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

            if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
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

    if (FP_Abs(xpos1GridAHead - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4)))
    {
        if (!Facing(xpos1GridAHead))
        {
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return field_122_brain_state_result;
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

        if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize2 * FP_FromInteger(2)))
        {
            SetNextMotion(eSlogMotions::Motion_2_Run);
            return 4;
        }
    }

    if (FP_Abs(xpos1GridAHead - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(1)))
    {
        if (!Facing(xpos1GridAHead))
        {
            SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            return field_122_brain_state_result;
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

        if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize3 * FP_FromInteger(2)))
        {
            SetNextMotion(eSlogMotions::Motion_1_Walk);
            return 3;
        }
    }

    if (pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) != mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
    //sObjectIds.Find_449CF0(field_138);

    if (pTarget && pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        field_118_target_id = -1;
    }

    if (field_134_last_event_index != pEventSystem_5BC11C->field_28_last_event_index)
    {
        field_134_last_event_index = pEventSystem_5BC11C->field_28_last_event_index;
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter_5C1B8C->Type() == ReliveTypes::eSlig)
        {
            field_120_brain_state_idx = 0;
            field_118_target_id = -1;
            field_138_listening_to_slig_id = sControlledCharacter_5C1B8C->field_8_object_id;
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
            if (IsEventInRange(kEventSuspiciousNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, EventScale::Full))
            {
                field_142_anger_level++;
            }

            if (IsEventInRange(kEventSpeaking, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, EventScale::Full))
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
            if (IsEventInRange(kEventSuspiciousNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, EventScale::Full))
            {
                field_142_anger_level++;
            }

            if (IsEventInRange(kEventSpeaking, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, EventScale::Full))
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
            if (IsEventInRange(kEventSuspiciousNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, EventScale::Full))
            {
                field_142_anger_level++;
            }

            if (IsEventInRange(kEventSpeaking, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, EventScale::Full))
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
            if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::Slig_HereBoy_28 && sControlledCharacter_5C1B8C->Type() == ReliveTypes::eSlig)
            {
                field_120_brain_state_idx = 0;
                field_118_target_id = -1;
                field_138_listening_to_slig_id = sControlledCharacter_5C1B8C->field_8_object_id;
                return 0;
            }
        }
    }

    bool updateTarget = false;
    if (!pTarget)
    {
        if (field_118_target_id != -1)
        {
            field_118_target_id = -1;
            field_142_anger_level = 0;
            field_120_brain_state_idx = 1;
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            return 0;
        }
        updateTarget = true;
    }

    if (updateTarget || !field_160_flags.Get(Flags_160::eBit5_CommandedToAttack) || pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        if (!field_11C_biting_target)
        {
            pTarget = FindTarget(0, 0);
            if (!pTarget)
            {
                pTarget = FindTarget(0, 1);
                if (!pTarget)
                {
                    pTarget = sControlledCharacter_5C1B8C;
                    if (sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
                    {
                        field_118_target_id = -1;
                        field_142_anger_level = 0;
                        field_120_brain_state_idx = 1;
                        SetNextMotion(eSlogMotions::Motion_0_Idle);
                        return 0;
                    }
                }
            }
            field_118_target_id = pTarget->field_8_object_id;
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), FP_FromInteger(2) * gridSize))
    {
        return 2;
    }

    if (pTarget)
    {
        if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale), pTarget))
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

    if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pTarget))
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

    if (!VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pTarget))
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
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && HandleEnemyStopper())
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        field_160_flags.Set(Flags_160::eBit1_StopRunning, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 20;
    }

    if (!VIsFacingMe(pTarget) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
    }

    if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pTarget))
    {
        if (pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
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

    if (mBaseAnimatedWithPhysicsGameObject_YPos >= pTarget->mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(50))
    {
        auto pBone = FindBone();
        if (pBone)
        {
            field_15C_bone_id = pBone->field_8_object_id;
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
            if (field_15A_jump_counter < 100 && VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pTarget))
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

    if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBone->mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (pBone->VIsFalling())
        {
            field_15C_bone_id = -1;
            SetNextMotion(eSlogMotions::Motion_0_Idle);
            return 2;
        }

        if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
        {
            FP gridSize = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            if (CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), gridSize * FP_FromInteger(2)))
            {
                SetNextMotion(eSlogMotions::Motion_3_TurnAround);
            }
            else
            {
                SetNextMotion(eSlogMotions::Motion_1_Walk);
            }
        }

        if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(1), pBone))
        {
            return field_122_brain_state_result;
        }
        return 12;
    }

    field_15C_bone_id = -1;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_13_EatingBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling())
    {
        field_15C_bone_id = -1;
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBone->mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
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
            field_15C_bone_id = -1;
            return 2;
        }

        SetNextMotion(eSlogMotions::Motion_3_TurnAround);
        return 12;
    }

    field_15C_bone_id = -1;
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
        field_15C_bone_id = -1;
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        SetNextMotion(eSlogMotions::Motion_1_Walk);
    }

    if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBone->mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (VIsFacingMe(pBone))
        {
            if (!VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(1.5), pBone))
            {
                SetNextMotion(eSlogMotions::Motion_1_Walk);
            }

            if (!VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(1.5), pBone) || pBone->mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
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

    field_15C_bone_id = -1;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_11_ChasingAfterBone()
{
    auto pBone = static_cast<Bone*>(sObjectIds.Find_Impl(field_15C_bone_id));
    if (!pBone || pBone->VIsFalling())
    {
        field_15C_bone_id = -1;
        SetNextMotion(eSlogMotions::Motion_0_Idle);
        return 2;
    }

    if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBone->mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
    {
        if (!VIsFacingMe(pBone) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
        {
            SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
        }

        if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4), pBone))
        {
            if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBone->mBaseAnimatedWithPhysicsGameObject_YPos) < FP_FromInteger(50) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
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
            field_15C_bone_id = -1;
            return 9;
        }

        if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
        {
            return field_122_brain_state_result;
        }

        if (VIsFacingMe(pBone))
        {
            FP gridSize = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSize = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            // TODO: Same check twice ??
            if (CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), FP_FromInteger(4) * gridSize))
            {
                FP gridSize2 = {};
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    gridSize2 = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                else
                {
                    gridSize2 = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }

                if (CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), FP_FromInteger(1) * gridSize2))
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

    field_15C_bone_id = -1;
    return 2;
}

s16 Slog::Brain_ChasingAbe_State_20_Collided(BaseAliveGameObject* pTarget)
{
    auto pBone = FindBone();
    if (pBone)
    {
        field_15C_bone_id = pBone->field_8_object_id;
        return 11;
    }

    if (GetCurrentMotion() != eSlogMotions::Motion_0_Idle)
    {
        return field_122_brain_state_result;
    }

    if (field_160_flags.Get(Flags_160::eBit1_StopRunning))
    {
        if (pTarget->mBaseAnimatedWithPhysicsGameObject_XPos > mBaseAnimatedWithPhysicsGameObject_XPos)
        {
            return 2;
        }
    }
    else
    {
        if (pTarget->mBaseAnimatedWithPhysicsGameObject_XPos < mBaseAnimatedWithPhysicsGameObject_XPos)
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
            field_15C_bone_id = pBone->field_8_object_id;
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
    if (static_cast<s32>(sGnFrame) <= field_124_timer && pTarget->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
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

        if (FP_Abs(pTarget->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) > ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2))
        {
            field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
            return 1;
        }

        if (FP_Abs(pTarget->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) > ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2))
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

        if (pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1))
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
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && HandleEnemyStopper())
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        field_160_flags.Set(Flags_160::eBit1_StopRunning, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 20;
    }

    if (CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4)))
    {
        SetNextMotion(eSlogMotions::Motion_6_StopRunning);
        field_160_flags.Set(Flags_160::eBit1_StopRunning, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));
        field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
        field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
        return 19;
    }

    if (!VIsFacingMe(pTarget) && GetCurrentMotion() == eSlogMotions::Motion_2_Run)
    {
        SetNextMotion(eSlogMotions::Motion_7_SlideTurn);
    }

    if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(3), pTarget))
    {
        if (VOnSameYLevel(pTarget))
        {
            if (VIsFacingMe(pTarget))
            {
                if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), pTarget->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) && !field_160_flags.Get(Flags_160::eBit9_MovedOffScreen))
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
        field_15C_bone_id = pBone->field_8_object_id;
        return 11;
    }

    if (GetCurrentMotion() == eSlogMotions::Motion_0_Idle)
    {
        if (VIsFacingMe(pTarget))
        {
            FP gridSizeDirected = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridSizeDirected = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                gridSizeDirected = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), FP_FromInteger(2) * gridSizeDirected))
            {
                if (pTarget->mHealth > FP_FromInteger(0))
                {
                    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
                    return 1;
                }

                if (FP_Abs(pTarget->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2)))
                {
                    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
                    return 1;
                }
                field_124_timer = sGnFrame + 90;
                return 7;
            }

            field_160_flags.Set(Flags_160::eBit1_StopRunning, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX));

            field_14C_scratch_timer = Math_NextRandom() % 32 + sGnFrame + 120;
            field_150_growl_timer = Math_NextRandom() % 32 + sGnFrame + 60;
            return 19;
        }
        SetCurrentMotion(eSlogMotions::Motion_3_TurnAround);
    }

    if (field_160_flags.Get(Flags_160::eBit4_Hungry) && IsActiveHero(pTarget) && pTarget->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale && (sActiveHero->mCurrentMotion == eAbeMotions::Motion_104_RockThrowStandingHold_455DF0 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_107_RockThrowCrouchingHold_454410))
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

    if (mBaseAnimatedWithPhysicsGameObject_YPos <= pTarget->mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(50))
    {
        if (pTarget->mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5))
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
    field_15C_bone_id = -1;
    field_124_timer = Math_RandomRange(1, 3) + sGnFrame + field_158_chase_delay;
    Sfx(SlogSound::AttackGrowl_8);
    return 1;
}

s16 Slog::Brain_3_Death()
{
    field_138_listening_to_slig_id = -1;
    field_118_target_id = -1;

    if (field_124_timer < static_cast<s32>(sGnFrame + 80))
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.023);
        mBaseAnimatedWithPhysicsGameObject_RGB.r -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.g -= 2;
        mBaseAnimatedWithPhysicsGameObject_RGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_124_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale < FP_FromInteger(0))
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
    u8** ppRes = ResBlockForMotion(mCurrentMotion);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sSlogAnimIdTable[mCurrentMotion], ppRes);
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
    const AnimRecord& rec = AnimRec(AnimId::Slog_Idle);
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kDogrstnResID, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kDogattkResID, 1, 0));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kDogknfdResID, 1, 0));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kDogidleResID, 1, 0));
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_10_resources_array.ItemAt(0), 1);

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_160_flags.Clear(Flags_160::eBit3_Shot);
    field_160_flags.Clear(Flags_160::eBit6_HitByAbilityRing);
    field_160_flags.Set(Flags_160::eBit4_Hungry);

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mBaseAnimatedWithPhysicsGameObject_Anim.mFnPtrArray = kSlog_Anim_Frame_Fns_55EFBC;
    field_124_timer = 0;
    field_122_brain_state_result = 0;
    SetNextMotion(eSlogMotions::m1);
    field_130_motion_resource_block_index = 0;
    BaseAliveGameObject_PlatformId = -1;
    field_138_listening_to_slig_id = -1;
    field_118_target_id = -1;
    field_15C_bone_id = -1;
    SetTint(&sSlogTints_560A48[0], gMap.mCurrentLevel);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SlogFleech_34;

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_YOffset = 1;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YOffset = 2;
    }

    mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine, &hitX, &hitY, CollisionMask(eFloor_0, eDynamicCollision_32))
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
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
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                &BaseAliveGameObjectCollisionLine,
                &mBaseAnimatedWithPhysicsGameObject_XPos,
                &mBaseAnimatedWithPhysicsGameObject_YPos,
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

    if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos) > FP_FromInteger(750) || FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos) > FP_FromInteger(390))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        if (mHealth > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
        }

        const auto oldMotion = mCurrentMotion;
        field_122_brain_state_result = (this->*sSlogBrainTable[field_120_brain_state_idx])();
        if (sDDCheat_ShowAI_Info_5C1BD8)
        {
            DDCheat::DebugStr("Slog:  Motion=%d  BrainState=%d\n", mCurrentMotion, field_122_brain_state_result);
        }

        const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
        const FP oldYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

        (this->*sSlogMotionTable[mCurrentMotion])();

        if (oldXPos != mBaseAnimatedWithPhysicsGameObject_XPos || oldYPos != mBaseAnimatedWithPhysicsGameObject_YPos)
        {
            BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TlvGetAt(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
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
    field_118_target_id = -1;
    field_138_listening_to_slig_id = -1;
    field_15C_bone_id = -1;

    if (field_12C_tlvInfo != 0xFFFF)
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
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    SetCurrentMotion(eSlogMotions::Motion_0_Idle);
    SetNextMotion(eSlogMotions::m1);
}

const SfxDefinition getSfxDef(SlogSound effectId)
{
    return sSlogSFXList[static_cast<s32>(effectId)];
}

void Slog::Sfx(SlogSound effectId)
{
    s16 volumeLeft = 0;
    s16 volumeRight = 0;
    const SfxDefinition effectDef = getSfxDef(effectId);


    const CameraPos direction = gMap.GetDirection(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);
    PSX_RECT pRect = {};
    gMap.Get_Camera_World_Rect(direction, &pRect);

    const s16 defaultSndIdxVol = effectDef.field_3_default_volume;
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
            const FP percentHowFar = (FP_FromInteger(pRect.w) - mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
        }
        break;
        case CameraPos::eCamRight_4:
        {
            const FP percentHowFar = (mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
        }
        break;
        default:
            return;
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        SFX_SfxDefinition_Play_Stereo(
            &effectDef,
            volumeLeft,
            volumeRight,
            effectDef.field_4_pitch_min + 1524,
            effectDef.field_6_pitch_max + 1524);
    }
    else
    {
        SFX_SfxDefinition_Play_Stereo(
            &effectDef,
            volumeLeft,
            volumeRight,
            effectDef.field_4_pitch_min,
            effectDef.field_6_pitch_max);
    }
}

void Slog::ToJump()
{
    mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-10.18) : FP_FromDouble(10.18)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(-8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    VOnTrapDoorOpen();

    SetCurrentMotion(eSlogMotions::Motion_18_JumpForwards);
    BaseAliveGameObjectCollisionLine = nullptr;

    Sfx(SlogSound::AttackGrowl_8);

    if (gMap.GetDirection(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos) >= CameraPos::eCamCurrent_0)
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
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(9));
            }

            if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(9)) * FP_FromInteger(2)))
            {
                SetCurrentMotion(eSlogMotions::Motion_8_StartWalking);
                SetNextMotion(eSlogMotions::m1);
                return 1;
            }
            ToIdle();
            return 0;

        case eSlogMotions::Motion_2_Run:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(3));
            }

            if (!CollisionCheck(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20), mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4)))
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
    return sCollisions->Raycast(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - hitY, mBaseAnimatedWithPhysicsGameObject_XPos + hitX, mBaseAnimatedWithPhysicsGameObject_YPos - hitY, &pLine, &hitX, &hitY, CollisionMask(eWallRight_2, eWallLeft_1)) != 0;
}

void Slog::MoveOnLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = -1;
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                VOnCollisionWith(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            mBaseAnimatedWithPhysicsGameObject_XPos = oldXPos + mBaseAnimatedWithPhysicsGameObject_VelX;
        }
    }
    else
    {
        field_128_falling_velx_scale_factor = FP_FromDouble(0.3);
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
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
                if (gMap.Is_Point_In_Current_Camera(pBone->mBaseAnimatedWithPhysicsGameObject_LvlNumber, pBone->mBaseAnimatedWithPhysicsGameObject_PathNumber, pBone->mBaseAnimatedWithPhysicsGameObject_XPos, pBone->mBaseAnimatedWithPhysicsGameObject_YPos, 0) && pBone->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale)
                {
                    if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBone->mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(50) || pBone->VCanBeEaten())
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
    s32 local_array[10] = {};

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSlog)
        {
            auto pSlog = static_cast<Slog*>(pObj);
            if (pSlog->field_118_target_id != -1 && array_idx < ALIVE_COUNTOF(local_array))
            {
                local_array[array_idx++] = pSlog->field_118_target_id;
            }
        }

        if (pObj != pSligBeingListendTo && pObj != this && pObj->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale)
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

                            const FP xDelta = FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - pObj->mBaseAnimatedWithPhysicsGameObject_XPos);
                            if (xDelta < distanceToLastFoundObj)
                            {
                                s32 array_idx_iter = 0;
                                if (array_idx)
                                {
                                    // Something to do with finding who is the last attacker
                                    while (local_array[array_idx_iter] != pObj->field_8_object_id)
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
        BaseAliveGameObject_PlatformId = -1;
        SetCurrentMotion(eSlogMotions::Motion_4_Fall);
    }
}

void Slog::VOnTlvCollision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType32 == TlvTypes::DeathDrop_4)
        {
            mHealth = FP_FromInteger(0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        pTlv = sPath_dword_BB47C0->TlvGetAt(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
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
            switch (pBullet->field_20_type)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                    if (pBullet->field_30_x_distance <= FP_FromInteger(0))
                    {
                        relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                    pBullet->field_2C_ypos,
                                                    FP_FromInteger(-24),
                                                    FP_FromInteger(0),
                                                    mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);
                    }
                    else
                    {
                        relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                    pBullet->field_2C_ypos,
                                                    FP_FromInteger(24),
                                                    FP_FromInteger(0),
                                                    mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);
                    }
                    break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos,
                                                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(20) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                                                FP_FromInteger(0),
                                                FP_FromInteger(0),
                                                mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);
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
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
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
            relive_new Gibs(GibType::Slog_2, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX, mBaseAnimatedWithPhysicsGameObject_VelY, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);

            const PSX_RECT bRect = VGetBoundingRect();
            relive_new Blood(FP_FromInteger((bRect.x + bRect.w) / 2),
                                        FP_FromInteger((bRect.y + bRect.h) / 2),
                                        FP_FromInteger(0),
                                        FP_FromInteger(0),
                                        mBaseAnimatedWithPhysicsGameObject_SpriteScale, 50);

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
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
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
    const FP kMinXDist = FP_FromInteger(100) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    const FP kMinYDist = FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    // Is the player near?
    if (FP_Abs(sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) < kMinXDist && FP_Abs(sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) < kMinYDist && sActiveHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        return 1;
    }

    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eCrawlingSlig)
        {
            // Is this naked slig near?
            if (FP_Abs(pObj->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos) < kMinXDist && FP_Abs(pObj->mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_YPos) < kMinYDist && pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
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
    FP xPos = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2);
    if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
    {
        xPos += mBaseAnimatedWithPhysicsGameObject_XPos;
    }
    FP width = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (mBaseAnimatedWithPhysicsGameObject_XPos <= xPos)
    {
        width = xPos;
    }
    FP xToUse = xPos;
    if (mBaseAnimatedWithPhysicsGameObject_XPos <= xPos)
    {
        xToUse = mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    const auto stopperPath = static_cast<Path_EnemyStopper*>(
        sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(xToUse), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(width), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos), TlvTypes::EnemyStopper_47));

    return stopperPath != nullptr && stopperPath->field_10_stop_direction == (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) ? Path_EnemyStopper::StopDirection::Right_1 : Path_EnemyStopper::StopDirection::Left_0) && SwitchStates_Get(stopperPath->field_12_switch_id) > 0;
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
