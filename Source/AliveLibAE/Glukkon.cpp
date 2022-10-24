#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../relive_lib/Shadow.hpp"
#include "MusicController.hpp"
#include "SwitchStates.hpp"
#include "Spark.hpp"
#include "Slig.hpp" //used for renderWithGlowingEyes() only - TODO refactor
#include "ParticleBurst.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Slurg.hpp"
#include "DeathFadeOut.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Movie.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "GameSpeak.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Bullet.hpp"
#include "Sound/Midi.hpp"
#include "Grid.hpp"
#include "Camera.hpp"

const TGlukkonMotionFn sGlukkonMotionTable[25] = {
    &Glukkon::M_Idle_0_442D10,
    &Glukkon::M_Walk_1_442D30,
    &Glukkon::M_Turn_2_442F10,
    &Glukkon::M_KnockBack_3_442F40,
    &Glukkon::M_Jump_4_443030,
    &Glukkon::M_JumpToFall_5_4434C0,
    &Glukkon::M_WalkToFall_6_4434E0,
    &Glukkon::M_Fall_7_443510,
    &Glukkon::M_DeathFall_8_443760,
    &Glukkon::M_Land_9_443790,
    &Glukkon::M_ChantShake_10_443B50,
    &Glukkon::M_Speak1_11_4437D0,
    &Glukkon::M_Speak2_12_4438F0,
    &Glukkon::M_LongLaugh_13_443930,
    &Glukkon::M_BeginWalk_14_443950,
    &Glukkon::M_EndWalk_15_443970,
    &Glukkon::M_StandToJump_16_4439B0,
    &Glukkon::M_JumpToStand_17_4439D0,
    &Glukkon::M_WalkToJump_18_443A00,
    &Glukkon::M_JumpToWalk_19_443A30,
    &Glukkon::M_KnockBackStandBegin_20_442FC0,
    &Glukkon::M_GetShot_21_443A60,
    &Glukkon::M_KnockBackStandEnd_22_443010,
    &Glukkon::M_Speak3_23_443910,
    &Glukkon::M_EndSingleStep_24_443990};

const TGlukkonBrainFn sGlukkon_brain_table_5544A0[6] = {
    &Glukkon::Brain_0_Calm_WalkAround_440B40,
    &Glukkon::Brain_1_Panic_4412F0,
    &Glukkon::Brain_2_Slapped_441720,
    &Glukkon::Brain_3_PlayerControlled_441A30,
    &Glukkon::Brain_4_Death_442010,
    &Glukkon::Brain_5_WaitToSpawn_442490};

const AnimId sGlukkonsAnimIdTable[4][25] = {
    {AnimId::Glukkon_Normal_Idle, AnimId::Glukkon_Normal_Walk, AnimId::Glukkon_Normal_Turn, AnimId::Glukkon_Normal_KnockBack, AnimId::Glukkon_Normal_Jump,
     AnimId::Glukkon_Normal_JumpToFall, AnimId::Glukkon_Normal_WalkToFall, AnimId::Glukkon_Normal_Fall, AnimId::Glukkon_Normal_DeathFall, AnimId::Glukkon_Normal_Land,
     AnimId::Glukkon_Normal_ChantShake, AnimId::Glukkon_Normal_Speak1, AnimId::Glukkon_Normal_Speak2, AnimId::Glukkon_Normal_LongLaugh, AnimId::Glukkon_Normal_BeginWalk,
     AnimId::Glukkon_Normal_EndWalk, AnimId::Glukkon_Normal_StandToJump, AnimId::Glukkon_Normal_JumpToStand, AnimId::Glukkon_Normal_WalkToJump, AnimId::Glukkon_Normal_JumpToWalk,
     AnimId::Glukkon_Normal_KnockBackStandBegin, AnimId::Glukkon_Normal_GetShot, AnimId::Glukkon_Normal_KnockBackStandEnd, AnimId::Glukkon_Normal_Speak3, AnimId::Glukkon_Normal_EndSingleStep},

    {AnimId::Glukkon_Aslik_Idle, AnimId::Glukkon_Aslik_Walk, AnimId::Glukkon_Aslik_Turn, AnimId::Glukkon_Aslik_KnockBack, AnimId::Glukkon_Aslik_Jump,
     AnimId::Glukkon_Aslik_JumpToFall, AnimId::Glukkon_Aslik_WalkToFall, AnimId::Glukkon_Aslik_Fall, AnimId::Glukkon_Aslik_DeathFall, AnimId::Glukkon_Aslik_Land,
     AnimId::Glukkon_Aslik_ChantShake, AnimId::Glukkon_Aslik_Speak1, AnimId::Glukkon_Aslik_Speak2, AnimId::Glukkon_Aslik_LongLaugh, AnimId::Glukkon_Aslik_BeginWalk,
     AnimId::Glukkon_Aslik_EndWalk, AnimId::Glukkon_Aslik_StandToJump, AnimId::Glukkon_Aslik_JumpToStand, AnimId::Glukkon_Aslik_WalkToJump, AnimId::Glukkon_Aslik_JumpToWalk,
     AnimId::Glukkon_Aslik_KnockBackStandBegin, AnimId::Glukkon_Aslik_GetShot, AnimId::Glukkon_Aslik_KnockBackStandEnd, AnimId::Glukkon_Aslik_Speak3, AnimId::Glukkon_Aslik_EndSingleStep},

    {AnimId::Glukkon_Dripik_Idle, AnimId::Glukkon_Dripik_Walk, AnimId::Glukkon_Dripik_Turn, AnimId::Glukkon_Dripik_KnockBack, AnimId::Glukkon_Dripik_Jump,
     AnimId::Glukkon_Dripik_JumpToFall, AnimId::Glukkon_Dripik_WalkToFall, AnimId::Glukkon_Dripik_Fall, AnimId::Glukkon_Dripik_DeathFall, AnimId::Glukkon_Dripik_Land,
     AnimId::Glukkon_Dripik_ChantShake, AnimId::Glukkon_Dripik_Speak1, AnimId::Glukkon_Dripik_Speak2, AnimId::Glukkon_Dripik_LongLaugh, AnimId::Glukkon_Dripik_BeginWalk,
     AnimId::Glukkon_Dripik_EndWalk, AnimId::Glukkon_Dripik_StandToJump, AnimId::Glukkon_Dripik_JumpToStand, AnimId::Glukkon_Dripik_WalkToJump, AnimId::Glukkon_Dripik_JumpToWalk,
     AnimId::Glukkon_Dripik_KnockBackStandBegin, AnimId::Glukkon_Dripik_GetShot, AnimId::Glukkon_Dripik_KnockBackStandEnd, AnimId::Glukkon_Dripik_Speak3, AnimId::Glukkon_Dripik_EndSingleStep},

    {AnimId::Glukkon_Phleg_Idle, AnimId::Glukkon_Phleg_Walk, AnimId::Glukkon_Phleg_Turn, AnimId::Glukkon_Phleg_KnockBack, AnimId::Glukkon_Phleg_Jump,
     AnimId::Glukkon_Phleg_JumpToFall, AnimId::Glukkon_Phleg_WalkToFall, AnimId::Glukkon_Phleg_Fall, AnimId::Glukkon_Phleg_DeathFall, AnimId::Glukkon_Phleg_Land,
     AnimId::Glukkon_Phleg_ChantShake, AnimId::Glukkon_Phleg_Speak1, AnimId::Glukkon_Phleg_Speak2, AnimId::Glukkon_Phleg_LongLaugh, AnimId::Glukkon_Phleg_BeginWalk,
     AnimId::Glukkon_Phleg_EndWalk, AnimId::Glukkon_Phleg_StandToJump, AnimId::Glukkon_Phleg_JumpToStand, AnimId::Glukkon_Phleg_WalkToJump, AnimId::Glukkon_Phleg_JumpToWalk,
     AnimId::Glukkon_Phleg_KnockBackStandBegin, AnimId::Glukkon_Phleg_GetShot, AnimId::Glukkon_Phleg_KnockBackStandEnd, AnimId::Glukkon_Phleg_Speak3, AnimId::Glukkon_Phleg_EndSingleStep}};

const TintEntry kGlukkonTints_5546B4[18] = {
    {EReliveLevelIds::eMines, 137u, 137u, 137u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 137u, 137u, 137u},
    {EReliveLevelIds::eMudancheeVault, 137u, 137u, 137u},
    {EReliveLevelIds::eFeeCoDepot, 137u, 137u, 137u},
    {EReliveLevelIds::eBarracks, 137u, 137u, 137u},
    {EReliveLevelIds::eMudancheeVault_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eBonewerkz, 137u, 137u, 137u},
    {EReliveLevelIds::eBrewery, 137u, 137u, 137u},
    {EReliveLevelIds::eBrewery_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eBarracks_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eBonewerkz_Ender, 137u, 137u, 137u},
    {EReliveLevelIds::eNone, 137u, 137u, 137u}};

s32 Glukkon::CreateFromSaveState(const u8* pData)
{
    const Glukkon_SaveState* pSaveState = reinterpret_cast<const Glukkon_SaveState*>(pData);
    auto pTlv = static_cast<relive::Path_Glukkon*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->field_44_tlvInfo));

    auto pGlukkon = relive_new Glukkon(pTlv, pSaveState->field_44_tlvInfo);
    if (pGlukkon)
    {
        pGlukkon->SetType(FromAE(pSaveState->field_8E_type_id));
        pGlukkon->mBaseGameObjectTlvInfo = pSaveState->field_4_object_id;
        if (pSaveState->field_40_bIsActiveChar)
        {
            sControlledCharacter = pGlukkon;
        }

        pGlukkon->BaseAliveGameObjectPathTLV = nullptr;
        pGlukkon->BaseAliveGameObjectCollisionLine = nullptr;

        pGlukkon->mXPos = pSaveState->field_8_xpos;
        pGlukkon->mYPos = pSaveState->field_C_ypos;

        pGlukkon->mVelX = pSaveState->field_10_xvel;
        pGlukkon->mVelY = pSaveState->field_14_yvel;

        pGlukkon->field_1D8_falling_velx_scale_factor = pSaveState->field_58_falling_velx_scale_factor;
        pGlukkon->mCurrentPath = pSaveState->field_18_path;
        pGlukkon->mCurrentLevel = MapWrapper::FromAESaveData(pSaveState->field_1A_level);
        pGlukkon->SetSpriteScale(pSaveState->field_1C_sprite_scale);

        pGlukkon->mRGB.SetRGB(pSaveState->mRingRed, pSaveState->mRingGreen, pSaveState->mRingBlue);

        pGlukkon->field_1A0_red = pSaveState->mRingRed;
        pGlukkon->field_1A2_green = pSaveState->mRingGreen;
        pGlukkon->field_1A4_blue = pSaveState->mRingBlue;

        pGlukkon->mCurrentMotion = pSaveState->field_28_current_motion;

        relive::Path_Glukkon::GlukkonTypes glukType = pGlukkon->field_1A8_tlvData.mGlukkonType;
        // TODO: This should really be a switch rather than comparing values
        if (glukType > relive::Path_Glukkon::GlukkonTypes::eStoryPhleg)
        {
            glukType = relive::Path_Glukkon::GlukkonTypes::eNormal;
        }

        pGlukkon->GetAnimation().Set_Animation_Data(pGlukkon->GetAnimRes(sGlukkonsAnimIdTable[static_cast<s32>(glukType)][pSaveState->field_28_current_motion]));

        pGlukkon->GetAnimation().SetCurrentFrame(pSaveState->field_2A_current_frame);
        pGlukkon->GetAnimation().SetFrameChangeCounter(pSaveState->field_2C_frame_change_counter);
        pGlukkon->mBaseGameObjectFlags.Set(BaseGameObject::Options::eDrawable_Bit4, pSaveState->field_2F_drawable & 1);
        pGlukkon->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pSaveState->field_26_flipX & 1);
        pGlukkon->GetAnimation().mFlags.Set(AnimFlags::eRender, pSaveState->field_2E_render & 1);

        if (IsLastFrame(&pGlukkon->GetAnimation()))
        {
            pGlukkon->GetAnimation().mFlags.Set(AnimFlags::eIsLastFrame);
        }

        pGlukkon->mHealth = pSaveState->field_30_health;
        pGlukkon->mCurrentMotion = pSaveState->field_34_current_motion;
        pGlukkon->mNextMotion = pSaveState->field_36_next_motion;
        pGlukkon->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pSaveState->field_38_last_line_ypos);
        pGlukkon->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);
        pGlukkon->field_1D4_timer = pSaveState->field_54_timer;
        pGlukkon->BaseAliveGameObjectCollisionLineType = pSaveState->field_3A_line_type;
        pGlukkon->field_214_tlv_info = pSaveState->field_44_tlvInfo;
        pGlukkon->SetBrain(sGlukkon_brain_table_5544A0[pSaveState->field_48_brain_state_idx]);
        pGlukkon->mBrainSubState = pSaveState->field_50_brain_sub_state;
        pGlukkon->mPreventDepossession = pSaveState->field_5E_prevent_depossession;
        pGlukkon->mAbeLevel = MapWrapper::FromAESaveData(pSaveState->field_60_level);
        pGlukkon->mAbePath = pSaveState->field_62_path;
        pGlukkon->mAbeCamera = pSaveState->field_64_camera;
        pGlukkon->field_1EA_speak = pSaveState->field_66_speak;
        pGlukkon->field_1E0_gamespeak_pitch = pSaveState->field_68_gamespeak_pitch;
        pGlukkon->field_1DC_previous_ypos = pSaveState->field_6C_previous_ypos;
        pGlukkon->field_1F0_randomish_speak_timer = pSaveState->field_70_randomish_speak_timer;
        pGlukkon->field_1F4_turn_or_help_timer = pSaveState->field_74_turn_or_help_timer;
        pGlukkon->field_1F8_panic_timer = pSaveState->field_78_panic_timer;
        pGlukkon->field_1FC = pSaveState->field_7C;
        pGlukkon->field_200_knockback_delay_after_getting_shot_timer = pSaveState->field_80_knockback_delay_after_getting_shot_timer;
        pGlukkon->field_204_getting_shot_timer = pSaveState->field_84_getting_shot_timer;
        pGlukkon->field_208_obj_id = pSaveState->field_88_obj_id;
        pGlukkon->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed, pSaveState->field_8C_can_be_possessed);
    }

    return sizeof(Glukkon_SaveState);
}

void Glukkon::SetBrain(TGlukkonBrainFn fn)
{
    mBrainState = fn;
}

bool Glukkon::BrainIs(TGlukkonBrainFn fn)
{
    return mBrainState == fn;
}

void Glukkon::LoadAnimations()
{
    for (s32 i = 0; i < 4; i++)
    {
        for (s32 j = 0; j < 25; j++)
        {
            auto& animId = sGlukkonsAnimIdTable[i][j];
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

Glukkon::Glukkon(relive::Path_Glukkon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    field_1EC_unused = -1;

    LoadAnimations();

    field_1A8_tlvData = *pTlv;

    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanSetOffExplosives);

    field_214_tlv_info = tlvId;

    switch (field_1A8_tlvData.mGlukkonType)
    {
        case relive::Path_Glukkon::GlukkonTypes::eNormal:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Normal_Idle));
            break;
        }

        case relive::Path_Glukkon::GlukkonTypes::eStoryAslik:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Aslik_Idle));
            break;
        }

        case relive::Path_Glukkon::GlukkonTypes::eStoryDripik:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Dripik_Idle));
            break;
        }

        case relive::Path_Glukkon::GlukkonTypes::eStoryPhleg:
        {
            Animation_Init(GetAnimRes(AnimId::Glukkon_Phleg_Idle));
            break;
        }

        default:
            break;
    }

    Init();
}

void Glukkon::VRender(PrimHeader** ot)
{
    s16 eyeIndices[1];
    if (field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik || field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryPhleg)
    {
        eyeIndices[0] = 63;
    }
    else
    {
        eyeIndices[0] = 62;
    }
    renderWithGlowingEyes(ot, this, &field_118_pPalAlloc[0], ALIVE_COUNTOF(field_118_pPalAlloc),
                          field_1A0_red, field_1A2_green, field_1A4_blue, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

s32 Glukkon::VGetSaveState(u8* pSaveBuffer)
{
    Glukkon_SaveState* pSaveState = reinterpret_cast<Glukkon_SaveState*>(pSaveBuffer);

    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        return 0;
    }
    pSaveState->field_0_id = AETypes::eGlukkon_67;
    pSaveState->field_4_object_id = mBaseGameObjectTlvInfo;
    pSaveState->field_8_xpos = mXPos;
    pSaveState->field_C_ypos = mYPos;
    pSaveState->field_10_xvel = mVelX;
    pSaveState->field_14_yvel = mVelY;
    pSaveState->field_18_path = mCurrentPath;
    pSaveState->field_1A_level = MapWrapper::ToAE(mCurrentLevel);
    pSaveState->field_1C_sprite_scale = GetSpriteScale();
    pSaveState->mRingRed = mRGB.r;
    pSaveState->mRingGreen = mRGB.g;
    pSaveState->mRingBlue = mRGB.b;
    pSaveState->field_28_current_motion = mCurrentMotion;
    pSaveState->field_2A_current_frame = static_cast<u16>(GetAnimation().GetCurrentFrame());
    pSaveState->field_2C_frame_change_counter = static_cast<u16>(GetAnimation().GetFrameChangeCounter());
    pSaveState->field_26_flipX = GetAnimation().mFlags.Get(AnimFlags::eFlipX);
    pSaveState->field_2E_render = GetAnimation().mFlags.Get(AnimFlags::eRender);
    pSaveState->field_2F_drawable = mBaseGameObjectFlags.Get(BaseGameObject::Options::eDrawable_Bit4);
    pSaveState->field_30_health = mHealth;
    pSaveState->field_34_current_motion = mCurrentMotion;
    pSaveState->field_36_next_motion = mNextMotion;
    pSaveState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    if (BaseAliveGameObjectCollisionLine)
    {
        pSaveState->field_3A_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pSaveState->field_3A_line_type = -1;
    }
    pSaveState->field_40_bIsActiveChar = this == static_cast<Glukkon*>(sControlledCharacter);
    pSaveState->field_44_tlvInfo = field_214_tlv_info;

    pSaveState->field_48_brain_state_idx = 0;

    s16 idx = 0;
    for (auto& fn : sGlukkon_brain_table_5544A0)
    {
        if (BrainIs(fn))
        {
            pSaveState->field_48_brain_state_idx = idx;
            break;
        }
        idx++;
    }

    pSaveState->field_50_brain_sub_state = mBrainSubState;
    pSaveState->field_54_timer = field_1D4_timer;
    pSaveState->field_58_falling_velx_scale_factor = field_1D8_falling_velx_scale_factor;
    pSaveState->field_5E_prevent_depossession = mPreventDepossession;
    pSaveState->field_60_level = MapWrapper::ToAE(mAbeLevel);
    pSaveState->field_62_path = mAbePath;
    pSaveState->field_64_camera = mAbeCamera;
    pSaveState->field_66_speak = field_1EA_speak;
    pSaveState->field_68_gamespeak_pitch = field_1E0_gamespeak_pitch;
    pSaveState->field_6C_previous_ypos = field_1DC_previous_ypos;
    pSaveState->field_70_randomish_speak_timer = field_1F0_randomish_speak_timer;
    pSaveState->field_74_turn_or_help_timer = field_1F4_turn_or_help_timer;
    pSaveState->field_78_panic_timer = field_1F8_panic_timer;
    pSaveState->field_7C = field_1FC;
    pSaveState->field_80_knockback_delay_after_getting_shot_timer = field_200_knockback_delay_after_getting_shot_timer;
    pSaveState->field_84_getting_shot_timer = field_204_getting_shot_timer;
    pSaveState->field_88_obj_id = field_208_obj_id;
    pSaveState->field_8C_can_be_possessed = mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eCanBePossessed);
    pSaveState->field_8E_type_id = ToAE(Type());

    return sizeof(Glukkon_SaveState);
}

void Glukkon::M_Idle_0_442D10()
{
    HandleInput();
}

void Glukkon::M_Walk_1_442D30()
{
    if (DoMovement())
    {
        switch (GetAnimation().GetCurrentFrame())
        {
            case 0:
            case 9:
                if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
                {
                    if (mNextMotion == eGlukkonMotions::M_Jump_4_443030)
                    {
                        SetAnim(eGlukkonMotions::M_WalkToJump_18_443A00);
                    }
                }
                else if (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::Enum::eRun)
                {
                    SetAnim(eGlukkonMotions::M_WalkToJump_18_443A00, TRUE);
                }

                MapFollowMe(TRUE);
                return;

            case 2:
            case 6:
            case 10:
            case 14:
                PlaySound(0, this);
                field_212_currentWalkPitch++;
                return;

            case 8:
            case 17:
                if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
                {
                    if (mNextMotion == eGlukkonMotions::M_Idle_0_442D10 || mNextMotion == eGlukkonMotions::M_Turn_2_442F10 || mNextMotion == eGlukkonMotions::M_Speak1_11_4437D0 || mNextMotion == eGlukkonMotions::M_Speak2_12_4438F0 || mNextMotion == eGlukkonMotions::M_Speak3_23_443910 || mNextMotion == eGlukkonMotions::M_LongLaugh_13_443930)
                    {
                        if (GetAnimation().GetCurrentFrame() != 8)
                        {
                            SetAnim(eGlukkonMotions::M_EndWalk_15_443970);
                        }
                        else
                        {
                            SetAnim(eGlukkonMotions::M_EndSingleStep_24_443990);
                        }
                    }
                }
                else
                {
                    if ((mVelX > FP_FromInteger(0) && (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::Enum::eLeft)) || (mVelX < FP_FromInteger(0) && (Input().mPads[sCurrentControllerIndex].mPressed & InputCommands::Enum::eRight)) || !(Input().mPads[sCurrentControllerIndex].mPressed & (InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
                    {
                        if (GetAnimation().GetCurrentFrame() == 8)
                        {
                            SetAnim(eGlukkonMotions::M_EndSingleStep_24_443990, TRUE);
                        }
                        else
                        {
                            SetAnim(eGlukkonMotions::M_EndWalk_15_443970, TRUE);
                        }
                    }
                }
                break;

            default:
                return;
        }
    }
}

void Glukkon::M_Turn_2_442F10()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        ToStand();
    }
}

void Glukkon::M_KnockBack_3_442F40()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        PlaySound(2, this);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        SlowDown(FP_FromDouble(0.35));
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            if (mHealth > FP_FromInteger(0))
            {
                SetAnim(eGlukkonMotions::M_KnockBackStandBegin_20_442FC0, TRUE);
            }
        }
    }
    else
    {
        M_Fall_7_443510();
    }
}

const FP sGlukkonVelY_5453DC[10] = {
    FP_FromDouble(-10.011077880859375),
    FP_FromDouble(-10.583099365234375),
    FP_FromDouble(-7.2048492431640625),
    FP_FromDouble(-4.6619873046875),
    FP_FromInteger(-3),
    FP_FromInteger(3),
    FP_FromDouble(4.4048004150390625),
    FP_FromDouble(7.0737457275390625),
    FP_FromDouble(10.93603515625),
    FP_FromDouble(15.046417236328125)};

const FP sGlukkonJumpVelX_54539C[10] = {
    FP_FromDouble(10.335205078125),
    FP_FromDouble(21.649551391601562),
    FP_FromDouble(18.9296875),
    FP_FromDouble(12.507583618164062),
    FP_FromDouble(10.808151245117188),
    FP_FromDouble(7.7117919921875),
    FP_FromDouble(5.19439697265625),
    FP_FromDouble(4.020263671875),
    FP_FromDouble(2.5957489013671875),
    FP_FromDouble(0.920867919921875)};

void Glukkon::M_Jump_4_443030()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    if (GetAnimation().GetCurrentFrame() >= 10)
    {
        JumpHelper();
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -900);
        BaseAliveGameObjectLastLineYPos = mYPos;
        if (pPlatform)
        {
            pPlatform->VRemove(this);
            BaseAliveGameObject_PlatformId = Guid{};
        }
        BaseAliveGameObjectCollisionLine = nullptr;
    }

    mVelY = (GetSpriteScale() * sGlukkonVelY_5453DC[GetAnimation().GetCurrentFrame()]);

    FP velXTableValue = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        velXTableValue = -sGlukkonJumpVelX_54539C[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        velXTableValue = sGlukkonJumpVelX_54539C[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * velXTableValue);

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX) || WallHit(GetSpriteScale() * FP_FromInteger(2), mVelX))
    {
        mVelY = FP_FromInteger(0);
        field_1D8_falling_velx_scale_factor = FP_FromDouble(0.35);
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        else
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
        MapFollowMe(TRUE);
        return;
    }

    if (sControlledCharacter == this)
    {
        SetActiveCameraDelayedFromDir();
    }

    bool bCollision = FALSE;
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (BaseAliveGameObjectCollisionLine)
    {
        bCollision = FALSE;
    }
    else
    {
        bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromInteger(0));
    }

    if (GetAnimation().GetCurrentFrame() == 0 || !bCollision)
    {
        if (GetAnimation().GetCurrentFrame() == 9 && !BaseAliveGameObjectCollisionLine)
        {
            SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
        }
        JumpHelper();
        return;
    }

    switch (pLine->mLineType)
    {
        case eLineTypes::eFloor_0:
        case eLineTypes::eBackgroundFloor_4:
        case eLineTypes::eDynamicCollision_32:
        case eLineTypes::eBackgroundDynamicCollision_36:
            PlaySound(1, this);
            mXPos = hitX;
            BaseAliveGameObjectCollisionLine = pLine;
            mYPos = hitY;
            MapFollowMe(TRUE);
            GetOnPlatforms();
            break;

        case eLineTypes::eWallLeft_1:
        case eLineTypes::eWallRight_2:
            mVelX = (-mVelX / FP_FromInteger(2));
            break;

        default:
            if (GetAnimation().GetCurrentFrame() == 9 && !BaseAliveGameObjectCollisionLine)
            {
                SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
            }
            JumpHelper();
            return;
    }
}

void Glukkon::JumpHelper()
{
    if (GetAnimation().GetCurrentFrame() != 15)
    {
        return;
    }

    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        const auto input_pressed = Input().mPads[sCurrentControllerIndex].mPressed;

        if ((mVelX > FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eLeft)) || (mVelX < FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eRight)))
        {
            // Direction changed
            SetAnim(eGlukkonMotions::M_JumpToStand_17_4439D0, TRUE);
        }
        else if (input_pressed & (InputCommands::Enum::eLeft | InputCommands::Enum::eRight))
        {
            if (!(input_pressed & InputCommands::Enum::eRun))
            {
                // Still moving but not running, so start walking
                SetAnim(eGlukkonMotions::M_JumpToWalk_19_443A30, TRUE);
            }
        }
        else if (!(input_pressed & InputCommands::Enum::eHop))
        {
            // Not changing direction or trying to move at all, back to standing
            SetAnim(eGlukkonMotions::M_JumpToStand_17_4439D0, TRUE);
        }
    }
    else
    {
        if (mNextMotion != 1)
        {
            if (mNextMotion == eGlukkonMotions::M_Idle_0_442D10 || mNextMotion == eGlukkonMotions::M_Turn_2_442F10 || mNextMotion == eGlukkonMotions::M_Speak1_11_4437D0 || mNextMotion == eGlukkonMotions::M_Speak2_12_4438F0 || mNextMotion == eGlukkonMotions::M_Speak3_23_443910 || mNextMotion == eGlukkonMotions::M_LongLaugh_13_443930)
            {
                // Leaving jump to a motion that requires standing
                SetAnim(eGlukkonMotions::M_JumpToStand_17_4439D0);
            }
        }
        else
        {
            // No next motion so just stand
            SetAnim(eGlukkonMotions::M_JumpToWalk_19_443A30, TRUE);
        }
    }

    MapFollowMe(TRUE);
    return;
}

void Glukkon::M_JumpToFall_5_4434C0()
{
    M_WalkToFall_6_4434E0();
}

void Glukkon::M_WalkToFall_6_4434E0()
{
    M_Fall_7_443510();

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::M_Fall_7_443510()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * field_1D8_falling_velx_scale_factor);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * field_1D8_falling_velx_scale_factor) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled_441A30))
    {
        SetActiveCameraDelayedFromDir();
    }

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case 0:
            case 4:
            case 32:
            case 36:
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
                mXPos = hitX;
                mVelY = FP_FromInteger(0);

                GetOnPlatforms();

                if (hitY - BaseAliveGameObjectLastLineYPos > (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(7)))
                {
                    SetAnim(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    mBrainSubState = 0;
                }
                else if (mCurrentMotion != eGlukkonMotions::M_KnockBack_3_442F40)
                {
                    SetAnim(eGlukkonMotions::M_Land_9_443790, TRUE);
                }
                break;

            case 1u:
            case 2u:
                mXPos = hitX;
                mVelX = -mVelX / FP_FromInteger(2);
                break;

            default:
                return;
        }
    }
}

void Glukkon::M_DeathFall_8_443760()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 85);
    }
}

void Glukkon::M_Land_9_443790()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Glukkon::PlaySound(1, this);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        HandleInput();
    }
}

void Glukkon::M_ChantShake_10_443B50()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        M_Fall_7_443510();
    }
}

void Glukkon::M_Speak1_11_4437D0()
{
    if (GetAnimation().GetCurrentFrame() == 2 && field_1EA_speak != GlukkonSpeak::None)
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            if (field_1FC)
            {
                EventBroadcast(kEventGlukkonUnknown, this);
                field_1FC = 0;
            }

            if (BrainIs(&Glukkon::Brain_3_PlayerControlled_441A30))
            {
                GameSpeakEvents evToBePushed;
                switch (field_1EA_speak)
                {
                    case GlukkonSpeak::Hey_0:
                        evToBePushed = GameSpeakEvents::Glukkon_Hey_36;
                        break;
                    case GlukkonSpeak::DoIt_1:
                        evToBePushed = GameSpeakEvents::Glukkon_DoIt_37;
                        break;
                    case GlukkonSpeak::StayHere_2:
                        evToBePushed = GameSpeakEvents::Glukkon_StayHere_38;
                        break;
                    case GlukkonSpeak::Commere_3:
                        evToBePushed = GameSpeakEvents::Glukkon_Commere_39;
                        break;
                    case GlukkonSpeak::AllOYa_4:
                        evToBePushed = GameSpeakEvents::Glukkon_AllOYa_40;
                        break;
                    case GlukkonSpeak::Heh_5:
                        evToBePushed = GameSpeakEvents::Glukkon_Heh_41;
                        break;
                    case GlukkonSpeak::Help_6:
                        evToBePushed = GameSpeakEvents::Glukkon_Help_42;
                        break;
                    case GlukkonSpeak::Laugh_7:
                        evToBePushed = GameSpeakEvents::Glukkon_Laugh_43;
                        break;
                    case GlukkonSpeak::KillEm_8:
                        evToBePushed = GameSpeakEvents::Glukkon_KillEm_44;
                        break;
                    case GlukkonSpeak::Unused_9:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_45;
                        break;
                    case GlukkonSpeak::Unused_10:
                        evToBePushed = GameSpeakEvents::Glukkon_Unknown_46;
                        break;
                    case GlukkonSpeak::What_11:
                        evToBePushed = GameSpeakEvents::Glukkon_What_47;
                        break;
                    default:
                        evToBePushed = GameSpeakEvents::eUnknown_35; //GlukkonSpeak::None
                        break;
                }
                if (evToBePushed != GameSpeakEvents::eUnknown_35)
                {
                    gEventSystem->PushEvent(evToBePushed);
                }
            }

            PlaySound_GameSpeak(field_1EA_speak, 0, 0, 0);

            if (field_1EA_speak == GlukkonSpeak::Help_6)
            {
                SwitchStates_Do_Operation(field_1A8_tlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetTrue);
            }
        }
        field_1EA_speak = GlukkonSpeak::None;
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        HandleInput();
    }
}

void Glukkon::M_Speak2_12_4438F0()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_LongLaugh_13_443930()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_BeginWalk_14_443950()
{
    M_JumpToWalk_19_443A30();
}

void Glukkon::M_EndWalk_15_443970()
{
    M_JumpToStand_17_4439D0();
}

void Glukkon::M_StandToJump_16_4439B0()
{
    M_WalkToJump_18_443A00();
}

void Glukkon::M_JumpToStand_17_4439D0()
{
    DoMovement();
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        Glukkon::HandleInput();
    }
}

void Glukkon::M_WalkToJump_18_443A00()
{
    DoMovement();
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::M_Jump_4_443030);
    }
}

void Glukkon::M_JumpToWalk_19_443A30()
{
    DoMovement();
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::M_Walk_1_442D30);
    }
}

void Glukkon::M_KnockBackStandBegin_20_442FC0()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        PlaySound_GameSpeak(GlukkonSpeak::Heh_5, 0, 0, 0);
        SetAnim(eGlukkonMotions::M_KnockBackStandEnd_22_443010, TRUE);
    }
}

void Glukkon::M_GetShot_21_443A60()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        M_Fall_7_443510();
        if (mCurrentMotion != eGlukkonMotions::M_GetShot_21_443A60)
        {
            SetAnim(eGlukkonMotions::M_GetShot_21_443A60, TRUE);
        }
    }

    if (static_cast<s32>(sGnFrame) >= field_204_getting_shot_timer)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eRender))
        {
            mBrainSubState = 2;
            const FP shotXVel = FP_FromInteger(20) * GetSpriteScale();
            if (mVelX >= FP_FromInteger(0))
            {
                mVelX = mVelX <= FP_FromInteger(0) ? FP_FromInteger(0) : shotXVel;
            }
            else
            {
                mVelX = -shotXVel;
            }
        }
    }

    if (static_cast<s32>(sGnFrame) > field_200_knockback_delay_after_getting_shot_timer)
    {
        GetAnimation().mFlags.Set(AnimFlags::eFlipX, mVelX > FP_FromInteger(0));
        SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
    }
}

void Glukkon::M_KnockBackStandEnd_22_443010()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetAnim(eGlukkonMotions::M_Idle_0_442D10, TRUE);
    }
}

void Glukkon::M_Speak3_23_443910()
{
    M_Speak1_11_4437D0();
}

void Glukkon::M_EndSingleStep_24_443990()
{
    M_JumpToStand_17_4439D0();
}

s16 Glukkon::Brain_0_Calm_WalkAround_440B40()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    LiftPoint* pLiftPoint = nullptr;
    if (pObj && pObj->Type() == ReliveTypes::eLiftPoint)
    {
        pLiftPoint = static_cast<LiftPoint*>(pObj);
        if (!pLiftPoint->vOnAnyFloor() && mBrainSubState != 7)
        {
            mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
            return 7;
        }
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        return 8;
    }

    if (sActiveHero->mHealth < FP_FromInteger(0))
    {
        Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death_442010);
        return 6;
    }

    BaseAnimatedWithPhysicsGameObject* pEvent17 = nullptr;

    switch (mBrainSubState)
    {
        case 0:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }

            if (ShouldPanic(FALSE))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }

            pEvent17 = IsEventInRange(
                kEventGlukkonUnknown,
                mXPos,
                mYPos,
                AsEventScale(GetScale()));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame + 20;
                return 9;
            }

            if (field_1A8_tlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                {
                    mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                    return 2;
                }
                mNextMotion = eGlukkonMotions::M_BeginWalk_14_443950;
                return 1;
            }
            else
            {
                mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
                return 1;
            }
            break;

        case 1:
            if (ShouldPanic(FALSE))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.mScreamHelpDelay;
                }

                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }
            else
            {
                auto pEvent17_1 = IsEventInRange(
                    kEventGlukkonUnknown,
                    mXPos,
                    mYPos,
                    AsEventScale(GetScale()));
                if (pEvent17_1 && pEvent17_1 != this)
                {
                    field_1D4_timer = sGnFrame + 20;
                    return 9;
                }

                if (field_1A8_tlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
                {
                    if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                    {
                        if (static_cast<s32>(sGnFrame) <= field_1F0_randomish_speak_timer)
                        {
                            mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
                            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
                            return 4;
                        }
                        else
                        {
                            field_1F0_randomish_speak_timer = sGnFrame + 120;
                            SpeakRandomish();
                            return 3;
                        }
                    }
                }
                else
                {
                    if (Math_NextRandom() < 5 && static_cast<s32>(sGnFrame) > field_1F4_turn_or_help_timer)
                    {
                        field_1F4_turn_or_help_timer = sGnFrame + 120;
                        mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                        return 2;
                    }
                }

                if (Math_NextRandom() >= 5 || static_cast<s32>(sGnFrame) <= field_1F0_randomish_speak_timer)
                {
                    return mBrainSubState;
                }

                field_1F0_randomish_speak_timer = sGnFrame + 120;
                SpeakRandomish();
                return 6;
            }
            break;

        case 2:
        case 8:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }
            return 0;

        case 3:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
            {
                return mBrainSubState;
            }
            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
            return 4;

        case 4:
            if (ShouldPanic(FALSE))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }

            pEvent17 = IsEventInRange(
                kEventGlukkonUnknown,
                mXPos,
                mYPos,
                AsEventScale(GetScale()));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame + 20;
                return 9;
            }
            else
            {
                if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                {
                    return mBrainSubState;
                }
                mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                return 2;
            }
            break;

        case 5:
            if (ShouldPanic(FALSE))
            {
                if (IsEventInRange(
                        kEventAbeOhm,
                        mXPos,
                        mYPos,
                        AsEventScale(GetScale())))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.mScreamHelpDelay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }
            else
            {
                auto pEvent17_3 = IsEventInRange(
                    kEventGlukkonUnknown,
                    mXPos,
                    mYPos,
                    AsEventScale(GetScale()));
                if (pEvent17_3 && pEvent17_3 != this)
                {
                    field_1D4_timer = sGnFrame + 20;
                    return 9;
                }
                else
                {
                    if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                    {
                        return mBrainSubState;
                    }
                    return 0;
                }
            }
            break;

        case 6:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }
            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
            return 5;

        case 7:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor())
                {
                    return mBrainSubState;
                }
                return 0;
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
                return 0;
            }
            break;

        case 9:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return mBrainSubState;
            }
            field_1FC = 1;
            Speak(GlukkonSpeak::Heh_5);
            return 6;

        default:
            return mBrainSubState;
    }
}

s16 Glukkon::Brain_1_Panic_4412F0()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint && pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->vOnAnyFloor() && mBrainSubState != 6)
    {
        mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
        return 6;
    }

    if (!BaseAliveGameObjectCollisionLine)
    {
        return 7;
    }

    if (sActiveHero->mHealth < FP_FromInteger(0))
    {
        Glukkon::Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death_442010);
        return 6;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer || mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }
            field_1F8_panic_timer = sGnFrame;
            Speak(GlukkonSpeak::Help_6);
            return 4;

        case 1:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }

            if (ShouldPanic(TRUE))
            {
                field_1F8_panic_timer = sGnFrame;
            }
            else if (static_cast<s32>(sGnFrame) - field_1F8_panic_timer > field_1A8_tlvData.mToCalmDelay)
            {
                SwitchStates_Do_Operation(field_1A8_tlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
                return 0;
            }

            if (field_1A8_tlvData.mBehavior != relive::Path_Glukkon::Behavior::eIgnoreWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                {
                    mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                    return 3;
                }
                mNextMotion = eGlukkonMotions::M_BeginWalk_14_443950;
            }
            else
            {
                mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
            }
            return 2;

        case 2:
            if (field_1A8_tlvData.mBehavior == relive::Path_Glukkon::Behavior::eCheckForWalls)
            {
                if (Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 1) || PathBlocked(mVelX, 1))
                {
                    Glukkon::Speak(GlukkonSpeak::Help_6);
                    return 5;
                }
            }
            else
            {
                if (Math_NextRandom() < 5u && static_cast<s32>(sGnFrame) > field_1F4_turn_or_help_timer)
                {
                    field_1F4_turn_or_help_timer = sGnFrame + 120;
                    Glukkon::Speak(GlukkonSpeak::Help_6);
                    return 5;
                }
            }
            if (Math_NextRandom() >= 0xAu || static_cast<s32>(sGnFrame) <= field_1F0_randomish_speak_timer)
            {
                return mBrainSubState;
            }
            Glukkon::Speak(GlukkonSpeak::Help_6);
            return 4;

        case 3:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }
            return 1;

        case 4:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }
            field_1F0_randomish_speak_timer = sGnFrame + 60;
            return 1;

        case 5:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
            {
                return mBrainSubState;
            }
            mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
            return 3;

        case 6:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor())
                {
                    return mBrainSubState;
                }
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
            }
            return 1;

        case 7:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }
            return 1;

        default:
            return mBrainSubState;
    }
}

s16 Glukkon::Brain_2_Slapped_441720()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    if (mCurrentMotion == eGlukkonMotions::M_Fall_7_443510)
    {
        return 3;
    }

    if (sActiveHero->mHealth < FP_FromInteger(0))
    {
        Glukkon::Speak(GlukkonSpeak::Laugh_7);
        SetBrain(&Glukkon::Brain_4_Death_442010);
        return 6;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (mCurrentMotion)
            {
                return mBrainSubState;
            }

            if (ShouldPanic(TRUE))
            {
                field_1F8_panic_timer = sGnFrame;
            }
            else if ((s32)(sGnFrame - field_1F8_panic_timer) > field_1A8_tlvData.mToCalmDelay)
            {
                SwitchStates_Do_Operation(field_1A8_tlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);

                if (FP_FromInteger(field_1A8_tlvData.mTopLeftX) >= mXPos)
                {
                    if (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
                    {
                        return 2;
                    }
                    if (FP_FromInteger(field_1A8_tlvData.mTopLeftX) >= mXPos)
                    {
                        return 0;
                    }
                }

                if (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
                {
                    return 0;
                }
                return 2;
            }

            if ((BaseAliveGameObjectCollisionLine && Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 4)) || PathBlocked(mVelX, 0))
            {
                mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                return 2;
            }
            else
            {
                mNextMotion = eGlukkonMotions::M_Jump_4_443030;
                return 1;
            }
            break;

        case 1:
            if (ShouldPanic(TRUE))
            {
                field_1F8_panic_timer = sGnFrame;
            }
            else if (static_cast<s32>(sGnFrame - field_1F8_panic_timer) > field_1A8_tlvData.mToCalmDelay)
            {
                mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
                return 0;
            }

            if (!BaseAliveGameObjectCollisionLine || (!Check_IsOnEndOfLine(GetAnimation().mFlags.Get(AnimFlags::eFlipX), 4) && !PathBlocked(mVelX, 0)))
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= field_1F0_randomish_speak_timer)
                {
                    return mBrainSubState;
                }
                Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Help_6, 0, 0, 0);
                field_1F0_randomish_speak_timer = sGnFrame + 40;
                return mBrainSubState;
            }

            mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
            return 2;

        case 2:
        case 3:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return mBrainSubState;
            }
            return 0;

        default:
            return mBrainSubState;
    }
}

s16 Glukkon::Brain_3_PlayerControlled_441A30()
{
    auto pDeathFadeOut = static_cast<DeathFadeOut*>(sObjectIds.Find_Impl(field_208_obj_id));
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case 0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return mBrainSubState;
            }
            ToStand();
            return 1;

        case 1:
        {
            if ((field_1A8_tlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryAslik &&
                field_1A8_tlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryDripik &&
                field_1A8_tlvData.mGlukkonType != relive::Path_Glukkon::GlukkonTypes::eStoryPhleg) ||
                !SwitchStates_Get(field_1A8_tlvData.mPlayMovieSwitchId))
            {
                if (Input_IsChanting_45F260() && mCurrentMotion != eGlukkonMotions::M_Jump_4_443030 && !mPreventDepossession)
                {
                    field_1D4_timer = sGnFrame + 30;
                    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
                    SetAnim(10, TRUE);
                    return 2;
                }
                return mBrainSubState;
            }

            if (field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik)
            {
                sVisitedFeecoEnder_5C1C06 = 1;
            }
            else if (field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryDripik)
            {
                sVisitedBarracks_5C1C04 = 1;
            }
            else
            {
                sVisitedBonewerks_5C1C02 = 1;
            }

            auto pDeathFadeOutMem = relive_new DeathFadeOut(Layer::eLayer_FadeFlash_40, 1, 0, 8, TPageAbr::eBlend_2);
            if (pDeathFadeOutMem)
            {
                field_208_obj_id = pDeathFadeOutMem->mBaseGameObjectId;
            }
        }
            return 3;

        case 2:
            if (Input_IsChanting_45F260()
                || ((field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik || field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryDripik || field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryPhleg)
                    && SwitchStates_Get(field_1A8_tlvData.mPlayMovieSwitchId)))
            {
                if (!(static_cast<s32>(sGnFrame) % 4))
                {
                    const FP xRand = FP_FromInteger(Math_RandomRange(-20, 20));
                    const FP yRand = FP_FromInteger(Math_RandomRange(20, 50));
                    New_TintChant_Particle(
                        (GetSpriteScale() * xRand) + mXPos,
                        mYPos - (GetSpriteScale() * yRand),
                        GetSpriteScale(),
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame) > field_1D4_timer || sActiveHero->mHealth <= FP_FromInteger(0))
                {
                    mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::ePossessed);
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    mBrainSubState = 2;
                    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                }
                return mBrainSubState;
            }
            ToStand();
            return 1;

        case 3:
            if (pDeathFadeOut && !pDeathFadeOut->field_7E_bDone)
            {
                return mBrainSubState;
            }

            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->Type() == ReliveTypes::eSlig)
                {
                    pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }

            SetUpdateDelay(2);
            return 4;

        case 4:
        {
            pScreenManager->DisableRendering();
            sLevelId_dword_5CA408 = static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel));

            const FmvInfo* pFmvRec = Path_Get_FMV_Record(gMap.mCurrentLevel, field_1A8_tlvData.mMovieId);
            relive_new Movie(pFmvRec->field_0_pName);
        }
            return 5;

        case 5:
            if (sMovie_ref_count_BB4AE4)
            {
                return mBrainSubState;
            }
            gPsxDisplay.PutCurrentDispEnv();
            pScreenManager->DecompressCameraToVRam(gMap.field_2C_camera_array[0]->field_C_pCamRes);
            if (pDeathFadeOut)
            {
                pDeathFadeOut->Init(Layer::eLayer_FadeFlash_40, 0, 1, 8);
            }
            return 6;

        case 6:
            GetSoundAPI().SND_Restart();
            pScreenManager->EnableRendering();
            field_1D4_timer = sGnFrame + 30;
            SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
            SetAnim(10, TRUE);
            return 2;

        default:
            return mBrainSubState;
    }
}

static GibType AsGibType(relive::Path_Glukkon::GlukkonTypes glukkonType)
{
    switch (glukkonType)
    {
        case relive::Path_Glukkon::GlukkonTypes::eNormal:
            return GibType::Glukkon_6;

        case relive::Path_Glukkon::GlukkonTypes::eStoryAslik:
            return GibType::Aslik_7;

        case relive::Path_Glukkon::GlukkonTypes::eStoryDripik:
            return GibType::Dripik_8;

        case relive::Path_Glukkon::GlukkonTypes::eStoryPhleg:
            return GibType::Phleg_9;
        default:
            return GibType::Glukkon_6;
    }
}

s16 Glukkon::Brain_4_Death_442010()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case 0:
            if (mCurrentMotion != eGlukkonMotions::M_DeathFall_8_443760 || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            else
            {
                mHealth = FP_FromInteger(0);
                field_1D4_timer = sGnFrame + 90;
                return 1;
            }
            break;

        case 1:
            if (static_cast<s32>(sGnFrame) > field_1D4_timer)
            {
                ToDead();
                return mBrainSubState;
            }
            else
            {
                SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));

                mRGB.r -= 2;
                mRGB.g -= 2;
                mRGB.b -= 2;

                DeathSmokeEffect(true);

                return mBrainSubState;
            }
            break;

        case 2:
        {
            relive_new Gibs(
                AsGibType(field_1A8_tlvData.mGlukkonType),
                mXPos,
                mYPos,
                mVelX,
                mVelY,
                GetSpriteScale(),
                0);

            relive_new Blood(
                mXPos,
                mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale(),
                20);

            New_Smoke_Particles(
                mXPos,
                mYPos - (FP_FromInteger(30) * GetSpriteScale()),
                GetSpriteScale(),
                3,
                RGB16{128, 128, 128});

            SfxPlayMono(relive::SoundEffects::KillEffect, 128, GetSpriteScale());
            SfxPlayMono(relive::SoundEffects::FallingItemHit, 90, GetSpriteScale());

            GetAnimation().mFlags.Clear(AnimFlags::eAnimate);
            GetAnimation().mFlags.Clear(AnimFlags::eRender);

            SetAnim(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);

            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            field_1D4_timer = sGnFrame + 40;
        }
            return 3;

        case 3:
            if (static_cast<s32>(sGnFrame) > field_1D4_timer)
            {
                ToDead();
            }
            return mBrainSubState;

        case 4:
        case 5:
            if (!BaseAliveGameObjectCollisionLine || mCurrentMotion != eGlukkonMotions::M_KnockBack_3_442F40 || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            else
            {
                field_1D4_timer = sGnFrame + 90;
                return 1;
            }
            break;

        default:
            return mBrainSubState;
    }
}

const PSX_Point v00554768[8] = {
    {-25, -5},
    {-30, -22},
    {-25, -39},
    {-10, -51},
    {6, -39},
    {9, -23},
    {5, -6},
    {0, 0}};


s16 Glukkon::Brain_5_WaitToSpawn_442490()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (mBrainSubState == 0)
    {
        if (!SwitchStates_Get(field_1A8_tlvData.mSpawnSwitchId))
        {
            return mBrainSubState;
        }
        field_1D4_timer = sGnFrame + field_1A8_tlvData.mSpawnDelay;
        return 1;
    }
    else if (mBrainSubState == 2)
    {
        if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
        {
            return mBrainSubState;
        }
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
        mBrainSubState = 0;
        return mBrainSubState;
    }
    else if (mBrainSubState == 1)
    {
        if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
        {
            return mBrainSubState;
        }

        mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
        mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed);

        SetType(ReliveTypes::eGlukkon);

        if (field_1A8_tlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFullSpawnEffects)
        {
            SFX_Play_Pitch(relive::SoundEffects::Zap1, 60, -300);

            for (const auto& p : v00554768)
            {
                const s16 sparkX = FP_GetExponent(FP_FromInteger(p.x) + mXPos + FP_FromInteger(13));
                const s16 sparkY = FP_GetExponent(mYPos + FP_FromInteger(p.y) - FP_FromInteger(11));
                relive_new Spark(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, SparkType::eBigChantParticle_1);
            }

            const PSX_RECT bRect = VGetBoundingRect();

            New_DestroyOrCreateObject_Particle(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2) + (GetSpriteScale() * FP_FromInteger(60)),
                GetSpriteScale());

            relive_new ParticleBurst(
                mXPos,
                mYPos - FP_FromInteger(18),
                6,
                FP_FromInteger(1),
                BurstType::eBigRedSparks_3,
                9);

            Speak(GlukkonSpeak::Heh_5);
            return 2;
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::GlukkonSpawn, 0);
            Speak(GlukkonSpeak::Heh_5);
            return 2;
        }
    }
    return mBrainSubState;
}

void Glukkon::Init()
{
    GetAnimation().mFlags.Set(AnimFlags::eAnimate);
    GetAnimation().mFlags.Set(AnimFlags::eRender);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetTint(&kGlukkonTints_5546B4[0], gMap.mCurrentLevel);
    mXPos = FP_FromInteger((field_1A8_tlvData.mTopLeftX + field_1A8_tlvData.mBottomRightX) / 2);
    mYPos = FP_FromInteger(field_1A8_tlvData.mTopLeftY);

    if (field_1A8_tlvData.mFacing == relive::Path_Glukkon::Facing::eLeft)
    {
        GetAnimation().mFlags.Set(AnimFlags::eFlipX);
    }

    if (field_1A8_tlvData.mSpawnSwitchId)
    {
        if (field_1A8_tlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFacingLeft)
        {
            mXPos -= ScaleToGridSize(GetSpriteScale());
            GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
        }
        else if (field_1A8_tlvData.mSpawnType == relive::Path_Glukkon::SpawnType::eFacingRight)
        {
            mXPos += ScaleToGridSize(GetSpriteScale());
            GetAnimation().mFlags.Set(AnimFlags::eFlipX);
        }
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eCanBePossessed);
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        SetBrain(&Glukkon::Brain_5_WaitToSpawn_442490);
        mBrainSubState = 0;
        SetType(ReliveTypes::eNone);
    }
    else
    {
        mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed);
        SetType(ReliveTypes::eGlukkon);
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
        mBrainSubState = 0;
    }

    if (field_1A8_tlvData.mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
    }
    else if (field_1A8_tlvData.mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(79),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    SetAnim(0, TRUE);

    field_208_obj_id = Guid{};
    BaseAliveGameObject_PlatformId = Guid{};
    field_1D4_timer = 0;
    mHealth = FP_FromInteger(1);
    field_1F0_randomish_speak_timer = 0;
    field_1F4_turn_or_help_timer = 0;
    field_1F8_panic_timer = 0;
    field_1EA_speak = GlukkonSpeak::None;
    field_1E0_gamespeak_pitch = 0;
    field_1FC = 0;
    field_200_knockback_delay_after_getting_shot_timer = 0;
    field_204_getting_shot_timer = 0;
    field_1DC_previous_ypos = mYPos;

    if (!GetShadow())
    {
        CreateShadow();
    }
}

Glukkon::~Glukkon()
{
    if (mHealth <= FP_FromInteger(0))
    {
        Path::TLV_Reset(field_214_tlv_info, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_214_tlv_info, -1, 0, 0);
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4); // Seems wrong to do this here ??

    if (this == sControlledCharacter)
    {
        sControlledCharacter = sActiveHero;
    }
}

void Glukkon::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eRestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eRestoredFromQuickSave);
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

            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();

                PSX_Point xy = {bRect.x, bRect.y};
                PSX_Point wh = {bRect.w, bRect.h};
                xy.y += 5;
                wh.y += 5;

                OnCollisionWith(xy, wh, gPlatformsArray);
            }
        }
        field_208_obj_id = BaseGameObject::RefreshId(field_208_obj_id);
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!Input_IsChanting_45F260())
        {
            mPreventDepossession = 0;
        }

        const auto oldMotion = mCurrentMotion;

        mBrainSubState = (this->*mBrainState)();

        const FP oldXPos = mXPos;
        const FP oldYPos = mYPos;

        (this->*sGlukkonMotionTable[mCurrentMotion])();

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != mCurrentMotion)
        {
            //LOG_INFO("Glukkon: Old motion = " << sGlukkonMotionNames[oldMotion] << " new motion = " << sGlukkonMotionNames[mCurrentMotion]);
        }

        if (oldXPos != mXPos || oldYPos != mYPos)
        {
            relive::Path_TLV* pTlv = sPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(pTlv);
        }

        Update_Slurg_WatchPoints();

        if (sControlledCharacter == this && BaseAliveGameObject_PlatformId != Guid{})
        {
            mVelY = mYPos - field_1DC_previous_ypos;
            SetActiveCameraDelayedFromDir();
        }
        field_1DC_previous_ypos = mYPos;
    }
}

void Glukkon::VPossessed()
{
    SwitchStates_Do_Operation(field_1A8_tlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::ePossessed);
    mPreventDepossession = 1;
    SetAnim(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);
    SetBrain(&Glukkon::Brain_3_PlayerControlled_441A30);
    mBrainSubState = 0;
    field_1D4_timer = sGnFrame + 35;
    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;
}

void Glukkon::Update_Slurg_WatchPoints()
{
    if (mCurrentMotion == eGlukkonMotions::M_Walk_1_442D30 || (mCurrentMotion == eGlukkonMotions::M_Jump_4_443030 && GetAnimation().GetCurrentFrame() > 8))
    {
        if (sGnFrame & 1)
        {
            const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
                pPoints->mPoints[count].x = FP_GetExponent(mXPos);
                pPoints->mPoints[count].y = BaseAliveGameObjectCollisionLine->mRect.y - 5;
                sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
            }
        }
    }
}

void Glukkon::SetAnim(s16 currentMotion, s16 bClearNextMotion)
{
    s32 typeIndex = static_cast<s32>(field_1A8_tlvData.mGlukkonType);
    if (typeIndex > 3)
    {
        typeIndex = 0;
    }

    GetAnimation().Set_Animation_Data(GetAnimRes(sGlukkonsAnimIdTable[typeIndex][currentMotion]));

    mCurrentMotion = currentMotion;
    if (bClearNextMotion)
    {
        mNextMotion = -1;
    }
}

void Glukkon::Speak(GlukkonSpeak speak)
{
    switch (speak)
    {
        case GlukkonSpeak::Hey_0:
        case GlukkonSpeak::DoIt_1:
        case GlukkonSpeak::Heh_5:
        case GlukkonSpeak::Help_6:
            mNextMotion = eGlukkonMotions::M_Speak1_11_4437D0;
            field_1EA_speak = speak;
            break;

        case GlukkonSpeak::StayHere_2:
            mNextMotion = eGlukkonMotions::M_Speak3_23_443910;
            field_1EA_speak = speak;
            break;

        case GlukkonSpeak::Commere_3:
        case GlukkonSpeak::AllOYa_4:
        case GlukkonSpeak::KillEm_8:
        case GlukkonSpeak::Unused_9:
        case GlukkonSpeak::Unused_10:
            mNextMotion = eGlukkonMotions::M_Speak2_12_4438F0;
            field_1EA_speak = speak;
            break;

        case GlukkonSpeak::Laugh_7:
            mNextMotion = eGlukkonMotions::M_LongLaugh_13_443930;
            field_1EA_speak = speak;
            break;

        default:
            field_1EA_speak = speak;
            break;
    }
}

void Glukkon::HandleInput()
{
    MapFollowMe(TRUE);

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled_441A30) && mBrainSubState == 1 && !(mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eTeleporting)))
    {
        const auto inputHeld = Input().mPads[sCurrentControllerIndex].mHeld;
        const auto matchButtons = InputCommands::Enum::eGameSpeak1 | InputCommands::Enum::eGameSpeak2 | InputCommands::Enum::eGameSpeak3 | InputCommands::Enum::eGameSpeak4 | InputCommands::Enum::eGameSpeak5 | InputCommands::Enum::eGameSpeak6 | InputCommands::Enum::eGameSpeak7 | InputCommands::Enum::eGameSpeak8 | InputCommands::Enum::eChant;

        if (inputHeld & matchButtons)
        {
            field_1EA_speak = GlukkonSpeak::None;

            if (inputHeld & InputCommands::Enum::eGameSpeak1)
            {
                field_1EA_speak = GlukkonSpeak::Hey_0;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak2)
            {
                field_1EA_speak = GlukkonSpeak::Commere_3;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak3)
            {
                field_1EA_speak = GlukkonSpeak::StayHere_2;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak4)
            {
                field_1EA_speak = GlukkonSpeak::DoIt_1;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak5)
            {
                field_1EA_speak = GlukkonSpeak::KillEm_8;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak7)
            {
                field_1EA_speak = GlukkonSpeak::Help_6;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak8)
            {
                field_1EA_speak = GlukkonSpeak::Laugh_7;
            }
            else if (inputHeld & InputCommands::Enum::eGameSpeak6)
            {
                field_1EA_speak = GlukkonSpeak::AllOYa_4;
            }

            if (field_1EA_speak != GlukkonSpeak::None)
            {
                Speak(field_1EA_speak);
            }
        }
        else
        {
            const auto inputPressed = Input().mPads[sCurrentControllerIndex].mPressed;
            if (inputPressed & InputCommands::Enum::eRight)
            {
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                }
                else
                {
                    mNextMotion = eGlukkonMotions::M_BeginWalk_14_443950;
                }
            }
            else if (inputPressed & InputCommands::Enum::eLeft)
            {
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    mNextMotion = eGlukkonMotions::M_BeginWalk_14_443950;
                }
                else
                {
                    mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                }
            }

            if (inputPressed & InputCommands::Enum::eRun)
            {
                if (mNextMotion == eGlukkonMotions::M_Walk_1_442D30)
                {
                    mNextMotion = eGlukkonMotions::M_Jump_4_443030;
                }
            }

            if (inputPressed & InputCommands::Enum::eHop)
            {
                mNextMotion = eGlukkonMotions::M_Jump_4_443030;
            }
        }
    }

    switch (mNextMotion)
    {
        case -1:
            if (mCurrentMotion == eGlukkonMotions::M_Idle_0_442D10)
            {
                return;
            }
            mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
            SetAnim(mNextMotion, TRUE);
            break;

        case eGlukkonMotions::M_Idle_0_442D10:
        case eGlukkonMotions::M_Turn_2_442F10:
        case eGlukkonMotions::M_Speak1_11_4437D0:
        case eGlukkonMotions::M_Speak2_12_4438F0:
        case eGlukkonMotions::M_LongLaugh_13_443930:
        case eGlukkonMotions::M_Speak3_23_443910:
            SetAnim(mNextMotion, TRUE);
            break;

        case eGlukkonMotions::M_Walk_1_442D30:
        case eGlukkonMotions::M_BeginWalk_14_443950:
            if (mCurrentMotion != eGlukkonMotions::M_Walk_1_442D30)
            {
                field_212_currentWalkPitch = 0;
            }
            [[fallthrough]];

        case eGlukkonMotions::M_Jump_4_443030:
        case eGlukkonMotions::M_StandToJump_16_4439B0:
        {
            FP xOff = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                xOff = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                xOff = ScaleToGridSize(GetSpriteScale());
            }
            if (!WallHit(GetSpriteScale() * FP_FromInteger(50), xOff))
            {
                SetAnim(mNextMotion, TRUE);
            }
        }
        break;

        default:
            return;
    }
}

s16 Glukkon::ShouldPanic(s16 panicEvenIfNotFacingMe)
{
    if (IsLineOfSightBetween(this, sControlledCharacter)
        && !(sControlledCharacter->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
        && !BaseAliveGameObject::IsInInvisibleZone(sControlledCharacter)
        && !EventGet(kEventResetting)
        && gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0)
        && gMap.Is_Point_In_Current_Camera(
            sControlledCharacter->mCurrentLevel,
            sControlledCharacter->mCurrentPath,
            sControlledCharacter->mXPos,
            sControlledCharacter->mYPos,
            0)
        && (panicEvenIfNotFacingMe || VIsFacingMe(sControlledCharacter)))
    {
        return 1;
    }

    // Panic if Abe is chanting
    if (IsEventInRange(
            kEventAbeOhm,
            mXPos,
            mYPos,
            AsEventScale(GetScale())))
    {
        return 1;
    }

    // Panic if an alarm is on
    if (EventGet(kEventAlarm))
    {
        return 1;
    }

    // Panic if player is speaking (I guess it can be rather alarming?)
    auto pSpeakEvent = IsEventInRange(
        kEventSpeaking,
        mXPos,
        mYPos,
        AsEventScale(GetScale()));
    return pSpeakEvent && pSpeakEvent == sControlledCharacter;
}

s16 Glukkon::PathBlocked(FP /*a2*/, s16 checkBounds)
{
    FP gridSize = ScaleToGridSize(GetSpriteScale());
    if (mCurrentMotion == eGlukkonMotions::M_Jump_4_443030)
    {
        gridSize = (gridSize * FP_FromInteger(4));
    }

    relive::Path_EnemyStopper::StopDirection direction = relive::Path_EnemyStopper::StopDirection::Left;
    ReliveTypes boundType = ReliveTypes::eScrabLeftBound;
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        boundType = ReliveTypes::eScrabLeftBound;
        direction = relive::Path_EnemyStopper::StopDirection::Left;
        gridSize = -gridSize;
    }
    else
    {
        boundType = ReliveTypes::eScrabRightBound;
        direction = relive::Path_EnemyStopper::StopDirection::Right;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = sPathInfo->TLV_Get_At_4DB4B0(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos), // TODO Abs() ??
        FP_GetExponent(mXPos + gridSize),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eSlamDoor);

    auto pSlamDoorTlv = static_cast<relive::Path_SlamDoor*>(BaseAliveGameObjectPathTLV);

    if (pSlamDoorTlv && ((pSlamDoorTlv->mStartClosed == relive::reliveChoice::eYes && !SwitchStates_Get(pSlamDoorTlv->mSwitchId)) || (pSlamDoorTlv->mStartClosed == relive::reliveChoice::eNo && SwitchStates_Get(pSlamDoorTlv->mSwitchId))))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = sPathInfo->TLV_Get_At_4DB4B0(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + gridSize),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eEnemyStopper);

    auto pEnemyStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
    if (pEnemyStopper
        && (pEnemyStopper->mStopDirection == direction || pEnemyStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
        && SwitchStates_Get(pEnemyStopper->mSwitchId))
    {
        return 1;
    }

    if (!checkBounds)
    {
        return 0;
    }

    if (sPathInfo->TLV_Get_At_4DB4B0(
            FP_GetExponent(mXPos),
            FP_GetExponent(mYPos), // TODO: Abs() ??
            FP_GetExponent(mXPos + gridSize),
            FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
            boundType))
    {
        return 1;
    }

    return 0;
}

void Glukkon::SpeakRandomish()
{
    if (sGnFrame & 1)
    {
        if ((sGnFrame & 1) == 1)
        {
            Speak(GlukkonSpeak::Heh_5);
        }
    }
    else
    {
        Speak(GlukkonSpeak::Laugh_7);
    }
}

const FP sWalkData_545354[18] = {
    FP_FromInteger(0),
    FP_FromDouble(2.1124420166015625),
    FP_FromDouble(3.923065185546875),
    FP_FromDouble(2.1124420166015625),
    FP_FromDouble(0.816680908203125),
    FP_FromDouble(4.986083984375),
    FP_FromDouble(6.017730712890625),
    FP_FromDouble(2.9228973388671875),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(3.923095703125),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(0.816680908203125),
    FP_FromDouble(4.986114501953125),
    FP_FromDouble(6.0177154541015625),
    FP_FromDouble(2.9228973388671875),
    FP_FromDouble(2.1124114990234375),
    FP_FromDouble(3.923095703125),
    FP_FromDouble(2.1124114990234375)};


// These tables just contain all zeros
// TODO/NOTE: These are all pointless - the logic in 0x444190 will use 0 if there is no table
const FP sJumpToFallData_54542C[10] = {};
const FP sWalkToFallData_54541C[4] = {};
const FP sBeginWalkData_545454[6] = {};
const FP sEndWalkData_54546C[6] = {};
const FP sStandToJumpData_545484[6] = {};
const FP sJumpToStandData_54549C[6] = {};
const FP sWalkToJumpData_5454B4[6] = {};
const FP sJumpToWalkData_5454CC[26] = {};

const FP* motion_velx_table_5547C4[25] = {
    nullptr,                 // M_Idle_0_442D10
    sWalkData_545354,        // M_Walk_1_442D30
    nullptr,                 // M_Turn_2_442F10
    nullptr,                 // M_KnockBack_3_442F40
    sGlukkonJumpVelX_54539C, // M_Jump_4_443030
    sJumpToFallData_54542C,  // M_JumpToFall_5_4434C0
    sWalkToFallData_54541C,  // M_WalkToFall_6_4434E0
    nullptr,                 // M_Fall_7_443510
    nullptr,                 // M_DeathFall_8_443760
    nullptr,                 // M_Land_9_443790
    nullptr,                 // M_ChantShake_10_443B50
    nullptr,                 // M_Speak1_11_4437D0
    nullptr,                 // M_Speak2_12_4438F0
    nullptr,                 // M_LongLaugh_13_443930
    sBeginWalkData_545454,   // M_BeginWalk_14_443950
    sEndWalkData_54546C,     // M_EndWalk_15_443970
    sStandToJumpData_545484, // M_StandToJump_16_4439B0
    sJumpToStandData_54549C, // M_JumpToStand_17_4439D0
    sWalkToJumpData_5454B4,  // M_WalkToJump_18_443A00
    sJumpToWalkData_5454CC,  // M_JumpToWalk_19_443A30
    nullptr,                 // M_KnockBackStandBegin_20_442FC0
    nullptr,                 // M_GetShot_21_443A60
    nullptr,                 // M_KnockBackStandEnd_22_443010
    nullptr,                 // M_Speak3_23_443910
    nullptr                  // M_EndSingleStep_24_443990
};


s16 Glukkon::DoMovement()
{
    const FP* pTable = motion_velx_table_5547C4[mCurrentMotion];
    if (pTable)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = -pTable[GetAnimation().GetCurrentFrame()];
        }
        else
        {
            mVelX = pTable[GetAnimation().GetCurrentFrame()];
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
    }

    mVelX = mVelX * GetSpriteScale();

    if (WallHit(GetSpriteScale() * FP_FromInteger(50), mVelX))
    {
        field_1D8_falling_velx_scale_factor = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        else
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(6));
        }
        MapFollowMe(TRUE);
        SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
        return FALSE;
    }
    else
    {
        FollowLine();
        return BaseAliveGameObjectCollisionLine != nullptr;
    }
}

void Glukkon::FollowLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    const FP prevXPos = mXPos;

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
                    field_1D8_falling_velx_scale_factor = FP_FromDouble(0.35);
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                GetOnPlatforms();
            }
        }
        else
        {
            BaseAliveGameObjectLastLineYPos = mYPos;

            VOnTrapDoorOpen();

            if (mCurrentMotion == eGlukkonMotions::M_Walk_1_442D30 || mCurrentMotion == eGlukkonMotions::M_BeginWalk_14_443950 || mCurrentMotion == eGlukkonMotions::M_EndWalk_15_443970 || mCurrentMotion == eGlukkonMotions::M_WalkToJump_18_443A00)
            {
                SetAnim(eGlukkonMotions::M_WalkToFall_6_4434E0, TRUE);
            }
            else if (mCurrentMotion == eGlukkonMotions::M_Jump_4_443030 || mCurrentMotion == eGlukkonMotions::M_StandToJump_16_4439B0 || mCurrentMotion == eGlukkonMotions::M_JumpToStand_17_4439D0 || mCurrentMotion == eGlukkonMotions::M_JumpToWalk_19_443A30)
            {
                SetAnim(eGlukkonMotions::M_JumpToFall_5_4434C0, TRUE);
            }
            else
            {
                SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
            }

            mXPos = prevXPos + mVelX;
            field_1D8_falling_velx_scale_factor = FP_FromInteger(1);

            if (mCurrentMotion == eGlukkonMotions::M_KnockBack_3_442F40)
            {
                field_1D8_falling_velx_scale_factor = FP_FromDouble(0.67);
            }
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::GetOnPlatforms()
{
    OnCollisionWith(
        {FP_GetExponent(mXPos - FP_FromInteger(5)), FP_GetExponent(mYPos - FP_FromInteger(5))},
        {FP_GetExponent(mXPos + FP_FromInteger(5)), FP_GetExponent(mYPos + FP_FromInteger(5))},
        gPlatformsArray);
}

const relive::SfxDefinition stepSfx_554840[3] = {
    {0u, 8u, 36u, 25u, 1524, 1905},
    {0u, 3u, 59u, 60u, 0, 254},
    {0u, 3u, 72u, 120u, 0, 254}};

void Glukkon::PlaySound(s32 sndIdx, Glukkon* pGlukkon)
{
    s32 volumeLeft, volumeRight;
    s32 defaultSndIdxVol = stepSfx_554840[sndIdx].field_C_default_volume;

    s16 pitch;
    if (sndIdx || !pGlukkon)
    {
        pitch = Math_RandomRange(stepSfx_554840[sndIdx].field_E_pitch_min, stepSfx_554840[sndIdx].field_10_pitch_max);
    }
    else
    {
        s16 pitchCap = pGlukkon->field_212_currentWalkPitch;
        if (pitchCap > 12)
        {
            pitchCap = pitchCap % 4 + 12;
        }
        pitch = 127 * pitchCap;
    }

    if (pGlukkon->GetSpriteScale() == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else // Glukkon in background layer? TODO figure out if this does actually happen
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap.GetDirection(
        pGlukkon->mCurrentLevel,
        pGlukkon->mCurrentPath,
        pGlukkon->mXPos,
        pGlukkon->mYPos);
    PSX_RECT worldRect;
    gMap.Get_Camera_World_Rect(direction, &worldRect);
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
        {
            volumeLeft = volumeRight;
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
            FP percentHowFar = (FP_FromInteger(worldRect.w) - pGlukkon->mXPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (pGlukkon->mXPos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            break;
        }
        default:
            return;
    }

    if (pGlukkon->GetSpriteScale() == FP_FromDouble(0.5)) //TODO figure out if this does actually happen
    {
        volumeLeft = FP_GetExponent(FP_FromInteger(volumeLeft * 2) / FP_FromInteger(3));
        volumeRight = FP_GetExponent(FP_FromInteger(volumeRight * 2) / FP_FromInteger(3));
    }

    SFX_SfxDefinition_Play_Stereo(stepSfx_554840[sndIdx], (s16) volumeLeft, (s16) volumeRight, pitch, pitch);
}

void Glukkon::ToDead()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (sControlledCharacter == this)
    {
        // When its a player controlled gluk go back to the screen the player is in
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

        if (gMap.mNextLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                mAbeLevel,
                mAbePath,
                mAbeCamera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    SwitchStates_Do_Operation(field_1A8_tlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);

    if ((field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryAslik ||
        field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryDripik ||
        field_1A8_tlvData.mGlukkonType == relive::Path_Glukkon::GlukkonTypes::eStoryPhleg) &&
        !SwitchStates_Get(field_1A8_tlvData.mPlayMovieSwitchId))
    {
        // If an exec is dead trigger ze gas
        SwitchStates_Do_Operation(field_1A8_tlvData.mDeathSwitchId, relive::reliveSwitchOp::eSetTrue);
    }

    if (field_1A8_tlvData.mSpawnSwitchId == 0)
    {
        // Don't spawn again, dead
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_1A8_tlvData.mSpawnSwitchId > 1)
        {
            // If above 1 then its an actual switch id
            SwitchStates_Do_Operation(field_1A8_tlvData.mSpawnSwitchId, relive::reliveSwitchOp::eSetFalse);
        }

        // Spawn again if id is 1 or above
        Init();
    }
}

void Glukkon::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);

                PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, 0, 0);
                ToDead();

                // Muds love it when people DIE
                EventBroadcast(kEventMudokonComfort, this);
            }
        }

        pTlv = sPathInfo->TlvGetAt(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

const relive::SfxDefinition gameSpeak_554858[15] = {
    {0u, 8u, 66u, 127u, 0, 0},
    {0u, 8u, 64u, 127u, 0, 0},
    {0u, 8u, 67u, 127u, 0, 0},
    {0u, 8u, 61u, 127u, 0, 0},
    {0u, 8u, 63u, 127u, 0, 0},
    {0u, 8u, 70u, 127u, 0, 0},
    {0u, 8u, 65u, 127u, 0, 0},
    {0u, 8u, 69u, 127u, 0, 0},
    {0u, 8u, 71u, 127u, 0, 0},
    {0u, 8u, 60u, 127u, 0, 0},
    {0u, 8u, 60u, 127u, 0, 0},
    {0u, 8u, 62u, 127u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

void Glukkon::PlaySound_GameSpeak(GlukkonSpeak sndIdx, s16 volume, s16 pitch, Glukkon* pGlukkon)
{
    auto sndIdxShort = (s16) sndIdx;
    s16 calcedVolume = volume;
    if (!calcedVolume)
    {
        calcedVolume = gameSpeak_554858[sndIdxShort].field_C_default_volume;
    }
    if (pGlukkon)
    {
        if (pGlukkon->GetSpriteScale() == FP_FromDouble(0.5))
        {
            calcedVolume = FP_GetExponent(FP_FromInteger(calcedVolume * 2) / FP_FromInteger(3));
        }
    }
    SFX_SfxDefinition_Play_Mono(gameSpeak_554858[sndIdxShort], calcedVolume, pitch, pitch);
}

bool Glukkon::IsLineOfSightBetween(Glukkon* pGlukkon, BaseAliveGameObject* pOther)
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pathLine = nullptr;

    const PSX_RECT bRect = pOther->VGetBoundingRect();
    const FP X2 = FP_FromInteger((bRect.x + bRect.w) / 2);
    const FP Y2 = FP_FromInteger((bRect.y + bRect.y) / 2);

    return sCollisions->Raycast(
               pGlukkon->mXPos,
               pGlukkon->mYPos,
               X2,
               Y2,
               &pathLine,
               &hitX,
               &hitY,
               pGlukkon->GetScale() == Scale::Fg ? kFgFloorWallOrCeiling
               : kBgFloorWallOrCeiling)
        != 1;
}

void Glukkon::ToStand()
{
    field_1D8_falling_velx_scale_factor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetAnim(eGlukkonMotions::M_Idle_0_442D10);
    MapFollowMe(TRUE);
}

void Glukkon::SlowDown(FP speed)
{
    if (FP_GetExponent(mVelX))
    {
        FollowLine();
        if (mVelX <= FP_FromInteger(0))
        {
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = (GetSpriteScale() * speed) + mVelX;
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mVelX = mVelX - (GetSpriteScale() * speed);
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = FP_FromInteger(0);
            }
        }
    }
}

void Glukkon::VScreenChanged()
{
    BaseGameObject::VScreenChanged();
    SwitchStates_Do_Operation(field_1A8_tlvData.mHelpSwitchId, relive::reliveSwitchOp::eSetFalse);
    if (BrainIs(&Glukkon::Brain_5_WaitToSpawn_442490) && !mBrainSubState)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Glukkon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        SetAnim(eGlukkonMotions::M_WalkToFall_6_4434E0, TRUE);
    }
}

s16 Glukkon::VTakeDamage(BaseGameObject* pFrom)
{
    if (BrainIs(&Glukkon::Brain_4_Death_442010))
    {
        return 1;
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
                {
                    {
                        const FP yRand = (FP_FromInteger(Math_NextRandom() % 16)) - FP_FromInteger(8);
                        const FP xRand = FP_FromInteger(Math_NextRandom() & 0xF); // TODO: Might be wrong as was trying to make this abs() but result is unsigned anyway ??

                        const FP xPos = (GetSpriteScale() * (pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6)));
                        relive_new Blood(
                            xPos + mXPos,
                            mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                            ((pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(1) : FP_FromInteger(1)) * xRand + FP_FromInteger(16)),
                            yRand,
                            GetSpriteScale(),
                            12);
                    }

                    {
                        const FP xPos = (GetSpriteScale() * (pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(12) : FP_FromInteger(12)));
                        relive_new Blood(
                            xPos + mXPos,
                            mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                            pBullet->mXDistance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6),
                            FP_FromInteger(0),
                            GetSpriteScale(),
                            8);
                    }
                }
                break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    relive_new Blood(
                        mXPos,
                        mYPos - (FP_FromInteger(25) * GetSpriteScale()),
                        FP_FromInteger(0),
                        FP_FromInteger(0),
                        GetSpriteScale(),
                        25);
                }
                break;

                default:
                    break;
            }

            field_200_knockback_delay_after_getting_shot_timer = sGnFrame + 5;

            if (mCurrentMotion == eGlukkonMotions::M_GetShot_21_443A60)
            {
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
                return 1;
            }

            if (mCurrentMotion == eGlukkonMotions::M_KnockBack_3_442F40)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    mBrainSubState = 4;
                }
                mHealth = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
                return 1;
            }

            field_204_getting_shot_timer = sGnFrame + 20;
            SetAnim(eGlukkonMotions::M_GetShot_21_443A60, TRUE);

            SetBrain(&Glukkon::Brain_4_Death_442010);
            mBrainSubState = 4;

            if (pBullet->mXDistance >= FP_FromInteger(0))
            {
                mVelX = FP_FromDouble(0.001);
            }
            else
            {
                mVelX = -FP_FromDouble(0.001);
            }
            mHealth = FP_FromInteger(0);
            EventBroadcast(kEventMudokonComfort, this);
        }
        break;

        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eAirExplosion:
            SetBrain(&Glukkon::Brain_4_Death_442010);
            mBrainSubState = 2;
            EventBroadcast(kEventMudokonComfort, this);
            break;

        case ReliveTypes::eElectricWall:
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, field_1E0_gamespeak_pitch, this);
            return 1;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
            SetAnim(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
            SetBrain(&Glukkon::Brain_4_Death_442010);
            mBrainSubState = 0;
            EventBroadcast(kEventMudokonComfort, this);
            break;

        case ReliveTypes::eAbe:
            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_62_Punch_454750)
            {
                if (Math_NextRandom() <= 32u)
                {
                    Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, 0, 0);
                }
                else
                {
                    Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Hey_0, 0, 0, 0);
                }
                field_1F8_panic_timer = sGnFrame;
                SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
                SetBrain(&Glukkon::Brain_2_Slapped_441720);
                mBrainSubState = 0;
            }
            break;

        case ReliveTypes::eSlog:
            if (mCurrentMotion != eGlukkonMotions::M_KnockBack_3_442F40)
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&Glukkon::Brain_4_Death_442010);
                mBrainSubState = 5;
                Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
                EventBroadcast(kEventMudokonComfort, this);
                if (!VIsFacingMe(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
                {
                    GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
                }
                mVelX = FP_FromInteger(0);
                SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
            }
            break;

        case ReliveTypes::eElectrocute:
            GetAnimation().mFlags.Clear(AnimFlags::eRender);
            mHealth = FP_FromInteger(0);
            SetBrain(&Glukkon::Brain_4_Death_442010);
            mBrainSubState = 3;
            field_1D4_timer = sGnFrame + 1;
            EventBroadcast(kEventMudokonComfort, this);
            break;

        default:
            return 1;
    }

    return 1;
}
