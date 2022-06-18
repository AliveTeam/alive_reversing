#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "MusicController.hpp"
#include "SwitchStates.hpp"
#include "Spark.hpp"
#include "Slig.hpp" //used for renderWithGlowingEyes() only - TODO refactor
#include "ParticleBurst.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "ObjectIds.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Slurg.hpp"
#include "SlamDoor.hpp"
#include "DeathFadeOut.hpp"
#include "ScreenManager.hpp"
#include "Movie.hpp"
#include "DDCheat.hpp"
#include "PsxDisplay.hpp"
#include "Sound/Sound.hpp"
#include "MainMenu.hpp"
#include "GameSpeak.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Bullet.hpp"
#include "Sound/Midi.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

#define MAKE_STRINGS(VAR) #VAR,
const char_type* const sGlukkonMotionNames[25] = {
    GLUKKON_MOTIONS_ENUM(MAKE_STRINGS)};

const TGlukkonMotionFn sGlukkon_motion_table_5544C0[25] = {
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

const AnimId sGlukkonsFrameTableOffsetTable_554524[4][25] = {
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
    auto pTlv = static_cast<Path_Glukkon*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pSaveState->field_44_tlvInfo));

    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eFeeCoDepot:
        case EReliveLevelIds::eFeeCoDepot_Ender:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGlukAslikResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("ASLIK.BND", 0);
            }
            break;
        case EReliveLevelIds::eBarracks:
        case EReliveLevelIds::eBarracks_Ender:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGlukDripikResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("DRIPIK.BND", 0);
            }
            break;
        case EReliveLevelIds::eBonewerkz:
        case EReliveLevelIds::eBonewerkz_Ender:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGlukPhlegResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("PHLEG.BND", 0);
            }
            break;
        default:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGlkbasicResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("GLUKKON.BND", 0);
            }
            break;
    }

    auto pGlukkon = relive_new Glukkon(pTlv, pSaveState->field_44_tlvInfo);
    if (pGlukkon)
    {
        pGlukkon->SetType(FromAE(pSaveState->field_8E_type_id));
        pGlukkon->mBaseGameObjectTlvInfo = pSaveState->field_4_object_id;
        if (pSaveState->field_40_bIsActiveChar)
        {
            sControlledCharacter_5C1B8C = pGlukkon;
        }

        pGlukkon->BaseAliveGameObjectPathTLV = nullptr;
        pGlukkon->BaseAliveGameObjectCollisionLine = nullptr;

        pGlukkon->mBaseAnimatedWithPhysicsGameObject_XPos = pSaveState->field_8_xpos;
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_YPos = pSaveState->field_C_ypos;

        pGlukkon->mBaseAnimatedWithPhysicsGameObject_VelX = pSaveState->field_10_xvel;
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_VelY = pSaveState->field_14_yvel;

        pGlukkon->field_1D8_falling_velx_scale_factor = pSaveState->field_58_falling_velx_scale_factor;
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_PathNumber = pSaveState->field_18_path;
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pSaveState->field_1A_level);
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pSaveState->field_1C_sprite_scale;

        pGlukkon->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pSaveState->mRingRed, pSaveState->mRingGreen, pSaveState->mRingBlue);

        pGlukkon->field_1A0_red = pSaveState->mRingRed;
        pGlukkon->field_1A2_green = pSaveState->mRingGreen;
        pGlukkon->field_1A4_blue = pSaveState->mRingBlue;

        pGlukkon->mCurrentMotion = pSaveState->field_28_current_motion;

        GlukkonTypes glukType = pGlukkon->field_1A8_tlvData.field_22_glukkon_type;
        if (glukType > GlukkonTypes::StoryPhleg_3)
        {
            glukType = GlukkonTypes::Normal_0;
        }

        const AnimRecord& animRec = AnimRec(sGlukkonsFrameTableOffsetTable_554524[static_cast<s32>(glukType)][pSaveState->field_28_current_motion]);
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

        pGlukkon->mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame = pSaveState->field_2A_current_frame;
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pSaveState->field_2C_frame_change_counter;
        pGlukkon->mBaseGameObjectFlags.Set(BaseGameObject::Options::eDrawable_Bit4, pSaveState->field_2F_drawable & 1);
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pSaveState->field_26_flipX & 1);
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pSaveState->field_2E_render & 1);

        if (IsLastFrame(&pGlukkon->mBaseAnimatedWithPhysicsGameObject_Anim))
        {
            pGlukkon->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pGlukkon->mHealth = pSaveState->field_30_health;
        pGlukkon->mCurrentMotion = pSaveState->field_34_current_motion;
        pGlukkon->mNextMotion = pSaveState->field_36_next_motion;
        pGlukkon->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pSaveState->field_38_last_line_ypos);
        pGlukkon->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
        pGlukkon->field_1D4_timer = pSaveState->field_54_timer;
        pGlukkon->BaseAliveGameObjectCollisionLineType = pSaveState->field_3A_line_type;
        pGlukkon->field_214_tlv_info = pSaveState->field_44_tlvInfo;
        pGlukkon->SetBrain(sGlukkon_brain_table_5544A0[pSaveState->field_48_brain_state_idx]);
        pGlukkon->field_210_brain_sub_state = pSaveState->field_50_brain_sub_state;
        pGlukkon->field_1E2_prevent_depossession = pSaveState->field_5E_prevent_depossession;
        pGlukkon->field_1E4_level = MapWrapper::FromAE(pSaveState->field_60_level);
        pGlukkon->field_1E6_path = pSaveState->field_62_path;
        pGlukkon->field_1E8_camera = pSaveState->field_64_camera;
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
        pGlukkon->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed, pSaveState->field_8C_can_be_possessed);
    }

    return sizeof(Glukkon_SaveState);
}

const static TGlukkonBrainFn sGlukkonBrainTable[6] = 
{
    &Glukkon::Brain_0_Calm_WalkAround_440B40,
    &Glukkon::Brain_1_Panic_4412F0,
    &Glukkon::Brain_2_Slapped_441720,
    &Glukkon::Brain_3_PlayerControlled_441A30,
    &Glukkon::Brain_4_Death_442010,
    &Glukkon::Brain_5_WaitToSpawn_442490
};

void Glukkon::SetBrain(TGlukkonBrainFn fn)
{
    field_20C_brain_state_fn = fn;
}

bool Glukkon::BrainIs(TGlukkonBrainFn fn)
{
    return field_20C_brain_state_fn == fn;
}

Glukkon::Glukkon(Path_Glukkon* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    ;
    field_1EC_unused = -1;

    field_1A8_tlvData = *pTlv;

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_214_tlv_info = tlvInfo;

    switch (field_1A8_tlvData.field_22_glukkon_type)
    {
        case GlukkonTypes::Normal_0:
        case GlukkonTypes::Normal_4:
        case GlukkonTypes::Normal_5:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kGlukkonResID_801);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Normal_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
            break;
        }

        case GlukkonTypes::StoryAslik_1:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAslikResID_803);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Aslik_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
            break;
        }

        case GlukkonTypes::StoryDripik_2:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kDripikResID_805);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Dripik_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
            break;
        }

        case GlukkonTypes::StoryPhleg_3:
        {
            Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kPhlegResID_807);
            const AnimRecord& rec = AnimRec(AnimId::Glukkon_Phleg_Idle);
            u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
            Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
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
    if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1 || field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryPhleg_3)
    {
        eyeIndices[0] = 63;
    }
    else
    {
        eyeIndices[0] = 62;
    }
    renderWithGlowingEyes(ot, this, &field_118_pPalAlloc[0], ALIVE_COUNTOF(field_118_pPalAlloc),
                          &field_198_pal_rect, field_1A0_red, field_1A2_green, field_1A4_blue, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

s32 Glukkon::VGetSaveState(u8* pSaveBuffer)
{
    Glukkon_SaveState* pSaveState = reinterpret_cast<Glukkon_SaveState*>(pSaveBuffer);

    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }
    pSaveState->field_0_id = AETypes::eGlukkon_67;
    pSaveState->field_4_object_id = mBaseGameObjectTlvInfo;
    pSaveState->field_8_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pSaveState->field_C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pSaveState->field_10_xvel = mBaseAnimatedWithPhysicsGameObject_VelX;
    pSaveState->field_14_yvel = mBaseAnimatedWithPhysicsGameObject_VelY;
    pSaveState->field_18_path = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pSaveState->field_1A_level = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pSaveState->field_1C_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    pSaveState->mRingRed = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pSaveState->mRingGreen = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pSaveState->mRingBlue = mBaseAnimatedWithPhysicsGameObject_RGB.b;
    pSaveState->field_28_current_motion = mCurrentMotion;
    pSaveState->field_2A_current_frame = mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame;
    pSaveState->field_2C_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;
    pSaveState->field_26_flipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pSaveState->field_2E_render = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pSaveState->field_2F_drawable = mBaseGameObjectFlags.Get(BaseGameObject::Options::eDrawable_Bit4);
    pSaveState->field_30_health = mHealth;
    pSaveState->field_34_current_motion = mCurrentMotion;
    pSaveState->field_36_next_motion = mNextMotion;
    pSaveState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    if (BaseAliveGameObjectCollisionLine)
    {
        pSaveState->field_3A_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }
    else
    {
        pSaveState->field_3A_line_type = -1;
    }
    pSaveState->field_40_bIsActiveChar = this == static_cast<Glukkon*>(sControlledCharacter_5C1B8C);
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

    pSaveState->field_50_brain_sub_state = field_210_brain_sub_state;
    pSaveState->field_54_timer = field_1D4_timer;
    pSaveState->field_58_falling_velx_scale_factor = field_1D8_falling_velx_scale_factor;
    pSaveState->field_5E_prevent_depossession = field_1E2_prevent_depossession;
    pSaveState->field_60_level = MapWrapper::ToAE(field_1E4_level);
    pSaveState->field_62_path = field_1E6_path;
    pSaveState->field_64_camera = field_1E8_camera;
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
    pSaveState->field_8C_can_be_possessed = mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit3_Can_Be_Possessed);
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
        switch (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame)
        {
            case 0:
            case 9:
                if (sControlledCharacter_5C1B8C != this || mHealth <= FP_FromInteger(0))
                {
                    if (mNextMotion == eGlukkonMotions::M_Jump_4_443030)
                    {
                        SetAnim(eGlukkonMotions::M_WalkToJump_18_443A00);
                    }
                }
                else if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::Enum::eRun)
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
                if (sControlledCharacter_5C1B8C != this || mHealth <= FP_FromInteger(0))
                {
                    if (mNextMotion == eGlukkonMotions::M_Idle_0_442D10 || mNextMotion == eGlukkonMotions::M_Turn_2_442F10 || mNextMotion == eGlukkonMotions::M_Speak1_11_4437D0 || mNextMotion == eGlukkonMotions::M_Speak2_12_4438F0 || mNextMotion == eGlukkonMotions::M_Speak3_23_443910 || mNextMotion == eGlukkonMotions::M_LongLaugh_13_443930)
                    {
                        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame != 8)
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
                    if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::Enum::eLeft)) || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & InputCommands::Enum::eRight)) || !(sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & (InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
                    {
                        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 8)
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        ToStand();
    }
}

void Glukkon::M_KnockBack_3_442F40()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0)
    {
        PlaySound(2, this);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        SlowDown(FP_FromDouble(0.35));
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame >= 10)
    {
        JumpHelper();
        return;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0)
    {
        SFX_Play_Pitch(SoundEffect::PickupItem_28, 50, -900);
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        if (pPlatform)
        {
            pPlatform->VRemove(this);
            BaseAliveGameObject_PlatformId = -1;
        }
        BaseAliveGameObjectCollisionLine = nullptr;
    }

    mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * sGlukkonVelY_5453DC[mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame]);

    FP velXTableValue = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        velXTableValue = -sGlukkonJumpVelX_54539C[mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame];
    }
    else
    {
        velXTableValue = sGlukkonJumpVelX_54539C[mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame];
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * velXTableValue);

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX) || WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(2), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        field_1D8_falling_velx_scale_factor = FP_FromDouble(0.35);
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
        }
        SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
        MapFollowMe(TRUE);
        return;
    }

    if (sControlledCharacter_5C1B8C == this)
    {
        SetActiveCameraDelayedFromDir();
    }

    Bool32 bCollision = FALSE;
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0 || !bCollision)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 9 && !BaseAliveGameObjectCollisionLine)
        {
            SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
        }
        JumpHelper();
        return;
    }

    switch (pLine->field_8_type)
    {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            PlaySound(1, this);
            mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
            BaseAliveGameObjectCollisionLine = pLine;
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            MapFollowMe(TRUE);
            GetOnPlatforms();
            break;

        case 1u:
        case 2u:
            mBaseAnimatedWithPhysicsGameObject_VelX = (-mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2));
            break;

        default:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 9 && !BaseAliveGameObjectCollisionLine)
            {
                SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
            }
            JumpHelper();
            return;
    }
}

void Glukkon::JumpHelper()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame != 15)
    {
        return;
    }

    if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
    {
        const auto input_pressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;

        if ((mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eLeft)) || (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && (input_pressed & InputCommands::Enum::eRight)))
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::M_Fall_7_443510()
{
    if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_1D8_falling_velx_scale_factor);
        if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        }
    }
    else if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * field_1D8_falling_velx_scale_factor) + mBaseAnimatedWithPhysicsGameObject_VelX;
        if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
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
        switch (pLine->field_8_type)
        {
            case 0:
            case 4:
            case 32:
            case 36:
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

                GetOnPlatforms();

                if (hitY - BaseAliveGameObjectLastLineYPos > (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(7)))
                {
                    SetAnim(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    field_210_brain_sub_state = 0;
                }
                else if (mCurrentMotion != eGlukkonMotions::M_KnockBack_3_442F40)
                {
                    SetAnim(eGlukkonMotions::M_Land_9_443790, TRUE);
                }
                break;

            case 1u:
            case 2u:
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(2);
                break;

            default:
                return;
        }
    }
}

void Glukkon::M_DeathFall_8_443760()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0)
    {
        SFX_Play_Mono(SoundEffect::KillEffect_64, 85);
    }
}

void Glukkon::M_Land_9_443790()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 0)
    {
        Glukkon::PlaySound(1, this);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2 && field_1EA_speak != GlukkonSpeak::None)
    {
        if (gMap.Is_Point_In_Current_Camera_4810D0(
                mBaseAnimatedWithPhysicsGameObject_LvlNumber,
                mBaseAnimatedWithPhysicsGameObject_PathNumber,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            if (field_1FC)
            {
                Event_Broadcast(kEventGlukkonUnknown, this);
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
                    pEventSystem_5BC11C->PushEvent(evToBePushed);
                }
            }

            PlaySound_GameSpeak(field_1EA_speak, 0, 0, 0);

            if (field_1EA_speak == GlukkonSpeak::Help_6)
            {
                SwitchStates_Do_Operation(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetTrue_0);
            }
        }
        field_1EA_speak = GlukkonSpeak::None;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Glukkon::HandleInput();
    }
}

void Glukkon::M_WalkToJump_18_443A00()
{
    DoMovement();
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim(eGlukkonMotions::M_Jump_4_443030);
    }
}

void Glukkon::M_JumpToWalk_19_443A30()
{
    DoMovement();
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        SetAnim(eGlukkonMotions::M_Walk_1_442D30);
    }
}

void Glukkon::M_KnockBackStandBegin_20_442FC0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
        {
            field_210_brain_sub_state = 2;
            const FP shotXVel = FP_FromInteger(20) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
            if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0) ? FP_FromInteger(0) : shotXVel;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -shotXVel;
            }
        }
    }

    if (static_cast<s32>(sGnFrame) > field_200_knockback_delay_after_getting_shot_timer)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0));
        SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
    }
}

void Glukkon::M_KnockBackStandEnd_22_443010()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (gMap.GetDirection_4811A0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    auto pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    LiftPoint* pLiftPoint = nullptr;
    if (pObj && pObj->Type() == ReliveTypes::eLiftPoint)
    {
        pLiftPoint = static_cast<LiftPoint*>(pObj);
        if (!pLiftPoint->vOnAnyFloor() && field_210_brain_sub_state != 7)
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

    IBaseAnimatedWithPhysicsGameObject* pEvent17 = nullptr;

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }

            if (ShouldPanic(FALSE))
            {
                if (Event_Is_Event_In_Range(
                        kEventAbeOhm,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.field_16_scream_help_delay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }

            pEvent17 = Event_Is_Event_In_Range(
                kEventGlukkonUnknown,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame + 20;
                return 9;
            }

            if (field_1A8_tlvData.field_14_behaviour == Path_Glukkon::Behavior::eCheckForWalls_1)
            {
                if (Check_IsOnEndOfLine(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked(mBaseAnimatedWithPhysicsGameObject_VelX, 1))
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
                if (Event_Is_Event_In_Range(
                        kEventAbeOhm,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.field_16_scream_help_delay;
                }

                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }
            else
            {
                auto pEvent17_1 = Event_Is_Event_In_Range(
                    kEventGlukkonUnknown,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale));
                if (pEvent17_1 && pEvent17_1 != this)
                {
                    field_1D4_timer = sGnFrame + 20;
                    return 9;
                }

                if (field_1A8_tlvData.field_14_behaviour == Path_Glukkon::Behavior::eCheckForWalls_1)
                {
                    if (Check_IsOnEndOfLine(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked(mBaseAnimatedWithPhysicsGameObject_VelX, 1))
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
                    return field_210_brain_sub_state;
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
                return field_210_brain_sub_state;
            }
            return 0;

        case 3:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
            {
                return field_210_brain_sub_state;
            }
            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
            return 4;

        case 4:
            if (ShouldPanic(FALSE))
            {
                if (Event_Is_Event_In_Range(
                        kEventAbeOhm,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.field_16_scream_help_delay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }

            pEvent17 = Event_Is_Event_In_Range(
                kEventGlukkonUnknown,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale));
            if (pEvent17 && pEvent17 != this)
            {
                field_1D4_timer = sGnFrame + 20;
                return 9;
            }
            else
            {
                if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                {
                    return field_210_brain_sub_state;
                }
                mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
                return 2;
            }
            break;

        case 5:
            if (ShouldPanic(FALSE))
            {
                if (Event_Is_Event_In_Range(
                        kEventAbeOhm,
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos,
                        AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
                {
                    field_1D4_timer = sGnFrame + 10;
                }
                else
                {
                    field_1D4_timer = sGnFrame + field_1A8_tlvData.field_16_scream_help_delay;
                }
                Speak(GlukkonSpeak::Hey_0);
                SetBrain(&Glukkon::Brain_1_Panic_4412F0);
                return 0;
            }
            else
            {
                auto pEvent17_3 = Event_Is_Event_In_Range(
                    kEventGlukkonUnknown,
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos,
                    AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale));
                if (pEvent17_3 && pEvent17_3 != this)
                {
                    field_1D4_timer = sGnFrame + 20;
                    return 9;
                }
                else
                {
                    if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
                    {
                        return field_210_brain_sub_state;
                    }
                    return 0;
                }
            }
            break;

        case 6:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            field_1D4_timer = sGnFrame + Math_RandomRange(30, 120);
            return 5;

        case 7:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor())
                {
                    return field_210_brain_sub_state;
                }
                return 0;
            }
            else
            {
                BaseAliveGameObject_PlatformId = -1;
                return 0;
            }
            break;

        case 9:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return field_210_brain_sub_state;
            }
            field_1FC = 1;
            Speak(GlukkonSpeak::Heh_5);
            return 6;

        default:
            return field_210_brain_sub_state;
    }
}

s16 Glukkon::Brain_1_Panic_4412F0()
{
    if (gMap.GetDirection_4811A0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint && pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->vOnAnyFloor() && field_210_brain_sub_state != 6)
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

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer || mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            field_1F8_panic_timer = sGnFrame;
            Speak(GlukkonSpeak::Help_6);
            return 4;

        case 1:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }

            if (ShouldPanic(TRUE))
            {
                field_1F8_panic_timer = sGnFrame;
            }
            else if (static_cast<s32>(sGnFrame) - field_1F8_panic_timer > field_1A8_tlvData.field_1A_to_calm_delay)
            {
                SwitchStates_Do_Operation(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
                return 0;
            }

            if (field_1A8_tlvData.field_14_behaviour != Path_Glukkon::Behavior::eIgnoreWalls_0)
            {
                if (Check_IsOnEndOfLine(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked(mBaseAnimatedWithPhysicsGameObject_VelX, 1))
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
            if (field_1A8_tlvData.field_14_behaviour == Path_Glukkon::Behavior::eCheckForWalls_1)
            {
                if (Check_IsOnEndOfLine(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 1) || PathBlocked(mBaseAnimatedWithPhysicsGameObject_VelX, 1))
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
                return field_210_brain_sub_state;
            }
            Glukkon::Speak(GlukkonSpeak::Help_6);
            return 4;

        case 3:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            return 1;

        case 4:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            field_1F0_randomish_speak_timer = sGnFrame + 60;
            return 1;

        case 5:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
            {
                return field_210_brain_sub_state;
            }
            mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
            return 3;

        case 6:
            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor())
                {
                    return field_210_brain_sub_state;
                }
            }
            else
            {
                BaseAliveGameObject_PlatformId = -1;
            }
            return 1;

        case 7:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            return 1;

        default:
            return field_210_brain_sub_state;
    }
}

s16 Glukkon::Brain_2_Slapped_441720()
{
    if (gMap.GetDirection_4811A0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
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

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (mCurrentMotion)
            {
                return field_210_brain_sub_state;
            }

            if (ShouldPanic(TRUE))
            {
                field_1F8_panic_timer = sGnFrame;
            }
            else if ((s32)(sGnFrame - field_1F8_panic_timer) > field_1A8_tlvData.field_1A_to_calm_delay)
            {
                SwitchStates_Do_Operation(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
                SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);

                if (FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_0_x) >= mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        return 2;
                    }
                    if (FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_0_x) >= mBaseAnimatedWithPhysicsGameObject_XPos)
                    {
                        return 0;
                    }
                }

                if (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
                {
                    return 0;
                }
                return 2;
            }

            if ((BaseAliveGameObjectCollisionLine && Check_IsOnEndOfLine(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 4)) || PathBlocked(mBaseAnimatedWithPhysicsGameObject_VelX, 0))
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
            else if (static_cast<s32>(sGnFrame - field_1F8_panic_timer) > field_1A8_tlvData.field_1A_to_calm_delay)
            {
                mNextMotion = eGlukkonMotions::M_Idle_0_442D10;
                return 0;
            }

            if (!BaseAliveGameObjectCollisionLine || (!Check_IsOnEndOfLine(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), 4) && !PathBlocked(mBaseAnimatedWithPhysicsGameObject_VelX, 0)))
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= field_1F0_randomish_speak_timer)
                {
                    return field_210_brain_sub_state;
                }
                Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Help_6, 0, 0, 0);
                field_1F0_randomish_speak_timer = sGnFrame + 40;
                return field_210_brain_sub_state;
            }

            mNextMotion = eGlukkonMotions::M_Turn_2_442F10;
            return 2;

        case 2:
        case 3:
            if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10)
            {
                return field_210_brain_sub_state;
            }
            return 0;

        default:
            return field_210_brain_sub_state;
    }
}

s16 Glukkon::Brain_3_PlayerControlled_441A30()
{
    auto pDeathFadeOut = static_cast<DeathFadeOut*>(sObjectIds.Find_Impl(field_208_obj_id));
    if (gMap.GetDirection_4811A0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
            {
                return field_210_brain_sub_state;
            }
            ToStand();
            return 1;

        case 1:
        {
            if ((field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::StoryAslik_1 &&
                field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::StoryDripik_2 &&
                field_1A8_tlvData.field_22_glukkon_type != GlukkonTypes::StoryPhleg_3) ||
                !SwitchStates_Get(field_1A8_tlvData.field_26_play_movie_switch_id))
            {
                if (Input_IsChanting_45F260() && mCurrentMotion != eGlukkonMotions::M_Jump_4_443030 && !field_1E2_prevent_depossession)
                {
                    field_1D4_timer = sGnFrame + 30;
                    SFX_Play_Mono(SoundEffect::PossessEffect_17, 0);
                    SetAnim(10, TRUE);
                    return 2;
                }
                return field_210_brain_sub_state;
            }

            if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1)
            {
                sVisitedFeecoEnder_5C1C06 = 1;
            }
            else if (field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryDripik_2)
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
                field_208_obj_id = pDeathFadeOutMem->field_8_object_id;
            }
        }
            return 3;

        case 2:
            if (Input_IsChanting_45F260()
                || ((field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1 || field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryDripik_2 || field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryPhleg_3)
                    && SwitchStates_Get(field_1A8_tlvData.field_26_play_movie_switch_id)))
            {
                if (!(static_cast<s32>(sGnFrame) % 4))
                {
                    const FP xRand = FP_FromInteger(Math_RandomRange(-20, 20));
                    const FP yRand = FP_FromInteger(Math_RandomRange(20, 50));
                    New_TintChant_Particle(
                        (mBaseAnimatedWithPhysicsGameObject_SpriteScale * xRand) + mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * yRand),
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame) > field_1D4_timer || sActiveHero->mHealth <= FP_FromInteger(0))
                {
                    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    field_210_brain_sub_state = 2;
                    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                }
                return field_210_brain_sub_state;
            }
            ToStand();
            return 1;

        case 3:
            if (pDeathFadeOut && !pDeathFadeOut->field_7E_bDone)
            {
                return field_210_brain_sub_state;
            }

            for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
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
            pScreenManager->EnableRendering();
            sLevelId_dword_5CA408 = static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel));

            const FmvInfo* pFmvRec = Path_Get_FMV_Record(gMap.mCurrentLevel, field_1A8_tlvData.field_28_movie_to_play_fmvID);

            u32 pos = 0;
            Get_fmvs_sectors(pFmvRec->field_0_pName, 0, 0, &pos, 0, 0);

            relive_new Movie(pFmvRec->field_4_id, pos, pFmvRec->field_6_flags & 1, pFmvRec->field_8_flags, pFmvRec->field_A_volume);
        }
            return 5;

        case 5:
            if (sMovie_ref_count_BB4AE4)
            {
                return field_210_brain_sub_state;
            }
            gPsxDisplay_5C1130.PutCurrentDispEnv_41DFA0();
            pScreenManager->DecompressCameraToVRam((u16**) gMap.field_2C_camera_array[0]->field_C_pCamRes); // TODO: Cast hack
            if (pDeathFadeOut)
            {
                pDeathFadeOut->Init(Layer::eLayer_FadeFlash_40, 0, 1, 8);
            }
            return 6;

        case 6:
            GetSoundAPI().SND_Restart();
            pScreenManager->DisableRendering();
            field_1D4_timer = sGnFrame + 30;
            SFX_Play_Mono(SoundEffect::PossessEffect_17, 0);
            SetAnim(10, TRUE);
            return 2;

        default:
            return field_210_brain_sub_state;
    }
}

static GibType AsGibType(GlukkonTypes glukkonType)
{
    switch (glukkonType)
    {
        case GlukkonTypes::Normal_0:
            return GibType::Glukkon_6;

        case GlukkonTypes::StoryAslik_1:
            return GibType::Aslik_7;

        case GlukkonTypes::StoryDripik_2:
            return GibType::Dripik_8;

        case GlukkonTypes::StoryPhleg_3:
            return GibType::Phleg_9;

        case GlukkonTypes::Normal_4:
            ALIVE_FATAL("Didn't expect glukkon type 4 for gibs!");
            //return GibType::Fleech_10; // TODO: This can't be correct

        case GlukkonTypes::Normal_5:
            ALIVE_FATAL("Didn't expect glukkon type 5 for gibs!");
            //return GibType::Glukkon_6; // TODO: Was out of range

        default:
            return GibType::Glukkon_6;
    }
}

s16 Glukkon::Brain_4_Death_442010()
{
    if (gMap.GetDirection_4811A0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (field_210_brain_sub_state)
    {
        case 0:
            if (mCurrentMotion != eGlukkonMotions::M_DeathFall_8_443760 || !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_210_brain_sub_state;
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
                return field_210_brain_sub_state;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.008);

                mBaseAnimatedWithPhysicsGameObject_RGB.r -= 2;
                mBaseAnimatedWithPhysicsGameObject_RGB.g -= 2;
                mBaseAnimatedWithPhysicsGameObject_RGB.b -= 2;

                DeathSmokeEffect(true);

                return field_210_brain_sub_state;
            }
            break;

        case 2:
        {
            relive_new Gibs(
                AsGibType(field_1A8_tlvData.field_22_glukkon_type),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_VelX,
                mBaseAnimatedWithPhysicsGameObject_VelY,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                0);

            relive_new Blood(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                20);

            New_Smoke_Particles(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                3,
                128u,
                128u,
                128u);

            SFX_Play_Mono(SoundEffect::KillEffect_64, 128, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            SFX_Play_Mono(SoundEffect::FallingItemHit_47, 90, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

            SetAnim(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);

            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            field_1D4_timer = sGnFrame + 40;
        }
            return 3;

        case 3:
            if (static_cast<s32>(sGnFrame) > field_1D4_timer)
            {
                ToDead();
            }
            return field_210_brain_sub_state;

        case 4:
        case 5:
            if (!BaseAliveGameObjectCollisionLine || mCurrentMotion != eGlukkonMotions::M_KnockBack_3_442F40 || !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_210_brain_sub_state;
            }
            else
            {
                field_1D4_timer = sGnFrame + 90;
                return 1;
            }
            break;

        default:
            return field_210_brain_sub_state;
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
    if (gMap.GetDirection_4811A0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (field_210_brain_sub_state == 0)
    {
        if (!SwitchStates_Get(field_1A8_tlvData.field_1C_spawn_switch_id))
        {
            return field_210_brain_sub_state;
        }
        field_1D4_timer = sGnFrame + field_1A8_tlvData.field_20_spawn_delay;
        return 1;
    }
    else if (field_210_brain_sub_state == 2)
    {
        if (mCurrentMotion != eGlukkonMotions::M_Idle_0_442D10 || field_1EA_speak != GlukkonSpeak::None)
        {
            return field_210_brain_sub_state;
        }
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
        field_210_brain_sub_state = 0;
        return field_210_brain_sub_state;
    }
    else if (field_210_brain_sub_state == 1)
    {
        if (static_cast<s32>(sGnFrame) <= field_1D4_timer)
        {
            return field_210_brain_sub_state;
        }

        mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);

        SetType(ReliveTypes::eGlukkon);

        if (field_1A8_tlvData.field_1E_spawn_type == Path_Glukkon::SpawnType::eFullSpawnEffects_3)
        {
            SFX_Play_Pitch(SoundEffect::Zap1_49, 60, -300);

            for (const auto& p : v00554768)
            {
                const s16 sparkX = FP_GetExponent(FP_FromInteger(p.field_0_x) + mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(13));
                const s16 sparkY = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(p.field_2_y) - FP_FromInteger(11));
                relive_new Spark(FP_FromInteger(sparkX), FP_FromInteger(sparkY), FP_FromInteger(1), 9, -31, 159, SparkType::eBigChantParticle_1);
            }

            const PSX_RECT bRect = VGetBoundingRect();

            New_DestroyOrCreateObject_Particle(
                FP_FromInteger((bRect.x + bRect.w) / 2),
                FP_FromInteger((bRect.y + bRect.h) / 2) + (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60)),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            relive_new ParticleBurst(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(18),
                6,
                FP_FromInteger(1),
                BurstType::eBigRedSparks_3,
                9);

            Speak(GlukkonSpeak::Heh_5);
            return 2;
        }
        else
        {
            SFX_Play_Mono(SoundEffect::GlukkonSpawn_112, 0);
            Speak(GlukkonSpeak::Heh_5);
            return 2;
        }
    }
    return field_210_brain_sub_state;
}

void Glukkon::Init()
{
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetTint(&kGlukkonTints_5546B4[0], gMap.mCurrentLevel);
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((field_1A8_tlvData.field_8_top_left.field_0_x + field_1A8_tlvData.field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(field_1A8_tlvData.field_8_top_left.field_2_y);

    if (field_1A8_tlvData.field_12_start_direction == Path_Glukkon::StartDirection::eLeft_1)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    if (field_1A8_tlvData.field_1C_spawn_switch_id)
    {
        if (field_1A8_tlvData.field_1E_spawn_type == Path_Glukkon::SpawnType::eFacingLeft_1)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos -= ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
        }
        else if (field_1A8_tlvData.field_1E_spawn_type == Path_Glukkon::SpawnType::eFacingRight_2)
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
        }
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        SetBrain(&Glukkon::Brain_5_WaitToSpawn_442490);
        field_210_brain_sub_state = 0;
        SetType(ReliveTypes::eNone);
    }
    else
    {
        mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
        SetType(ReliveTypes::eGlukkon);
        SetBrain(&Glukkon::Brain_0_Calm_WalkAround_440B40);
        field_210_brain_sub_state = 0;
    }

    if (field_1A8_tlvData.field_10_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
    }
    else if (field_1A8_tlvData.field_10_scale == Scale_short::eFull_0)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(79),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    SetAnim(0, TRUE);

    field_208_obj_id = -1;
    BaseAliveGameObject_PlatformId = -1;
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
    field_1DC_previous_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    if (!mShadow)
    {
        mShadow = relive_new Shadow();
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

    if (this == sControlledCharacter_5C1B8C)
    {
        sControlledCharacter_5C1B8C = sActiveHero;
    }
}

void Glukkon::VUpdate()
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

            if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eBackgroundDynamicCollision_36)
            {
                const PSX_RECT bRect = VGetBoundingRect();

                PSX_Point xy = {bRect.x, bRect.y};
                PSX_Point wh = {bRect.w, bRect.h};
                xy.field_2_y += 5;
                wh.field_2_y += 5;

                VOnCollisionWith(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
            }
        }
        field_208_obj_id = BaseGameObject::RefreshId(field_208_obj_id);
    }

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!Input_IsChanting_45F260())
        {
            field_1E2_prevent_depossession = 0;
        }

        const auto oldMotion = mCurrentMotion;

        field_210_brain_sub_state = (this->*field_20C_brain_state_fn)();

        const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
        const FP oldYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

        (this->*sGlukkon_motion_table_5544C0[mCurrentMotion])();

        // TODO: This is extra debug logging to figure out the motion names
        if (oldMotion != mCurrentMotion)
        {
            //LOG_INFO("Glukkon: Old motion = " << sGlukkonMotionNames[oldMotion] << " new motion = " << sGlukkonMotionNames[mCurrentMotion]);
        }

        if (oldXPos != mBaseAnimatedWithPhysicsGameObject_XPos || oldYPos != mBaseAnimatedWithPhysicsGameObject_YPos)
        {
            Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);
            VOn_TLV_Collision(pTlv);
        }

        Update_Slurg_WatchPoints();

        if (sControlledCharacter_5C1B8C == this && BaseAliveGameObject_PlatformId != -1)
        {
            mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_YPos - field_1DC_previous_ypos;
            SetActiveCameraDelayedFromDir();
        }
        field_1DC_previous_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    }
}

void Glukkon::VPossessed()
{
    SwitchStates_Do_Operation(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    field_1E2_prevent_depossession = 1;
    SetAnim(eGlukkonMotions::M_ChantShake_10_443B50, TRUE);
    SetBrain(&Glukkon::Brain_3_PlayerControlled_441A30);
    field_210_brain_sub_state = 0;
    field_1D4_timer = sGnFrame + 35;
    field_1E4_level = gMap.mCurrentLevel;
    field_1E6_path = gMap.mCurrentPath;
    field_1E8_camera = gMap.mCurrentCamera;
}

void Glukkon::Update_Slurg_WatchPoints()
{
    if (mCurrentMotion == eGlukkonMotions::M_Walk_1_442D30 || (mCurrentMotion == eGlukkonMotions::M_Jump_4_443030 && mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame > 8))
    {
        if (sGnFrame & 1)
        {
            const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
                pPoints->field_0_points[count].field_0_xPos = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
                pPoints->field_0_points[count].field_2_yPos = BaseAliveGameObjectCollisionLine->field_0_rect.y - 5;
                sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
            }
        }
    }
}

void Glukkon::SetAnim(s16 currentMotion, s16 bClearNextMotion)
{
    s32 typeIndex = static_cast<s32>(field_1A8_tlvData.field_22_glukkon_type);
    if (typeIndex > 3)
    {
        typeIndex = 0;
    }

    const AnimRecord& animRec = AnimRec(sGlukkonsFrameTableOffsetTable_554524[typeIndex][currentMotion]);
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

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

    if (BrainIs(&Glukkon::Brain_3_PlayerControlled_441A30) && field_210_brain_sub_state == 1 && !(mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit10_Teleporting)))
    {
        const auto inputHeld = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;
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
            const auto inputPressed = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed;
            if (inputPressed & InputCommands::Enum::eRight)
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
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
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                xOff = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            if (!WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), xOff))
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
    if (IsLineOfSightBetween(this, sControlledCharacter_5C1B8C)
        && !(sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
        && !BaseAliveGameObject::IsInInvisibleZone(sControlledCharacter_5C1B8C)
        && !Event_Get(kEventResetting)
        && gMap.Is_Point_In_Current_Camera_4810D0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0)
        && gMap.Is_Point_In_Current_Camera_4810D0(
            sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_PathNumber,
            sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos,
            sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos,
            0)
        && (panicEvenIfNotFacingMe || VIsFacingMe(sControlledCharacter_5C1B8C)))
    {
        return 1;
    }

    // Panic if Abe is chanting
    if (Event_Is_Event_In_Range(
            kEventAbeOhm,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
    {
        return 1;
    }

    // Panic if an alarm is on
    if (Event_Get(kEventAlarm))
    {
        return 1;
    }

    // Panic if player is speaking (I guess it can be rather alarming?)
    auto pSpeakEvent = Event_Is_Event_In_Range(
        kEventSpeaking,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale));
    return pSpeakEvent && pSpeakEvent == sControlledCharacter_5C1B8C;
}

s16 Glukkon::PathBlocked(FP /*a2*/, s16 checkBounds)
{
    FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    if (mCurrentMotion == eGlukkonMotions::M_Jump_4_443030)
    {
        gridSize = (gridSize * FP_FromInteger(4));
    }

    Path_EnemyStopper::StopDirection direction = Path_EnemyStopper::StopDirection::Left_0;
    TlvTypes boundType = TlvTypes::ScrabLeftBound_43;
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        boundType = TlvTypes::ScrabLeftBound_43;
        direction = Path_EnemyStopper::StopDirection::Left_0;
        gridSize = -gridSize;
    }
    else
    {
        boundType = TlvTypes::ScrabRightBound_44;
        direction = Path_EnemyStopper::StopDirection::Right_1;
    }

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos), // TODO Abs() ??
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + gridSize),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
        TlvTypes::SlamDoor_85);

    auto pSlamDoorTlv = static_cast<Path_SlamDoor*>(BaseAliveGameObjectPathTLV);

    if (pSlamDoorTlv && ((pSlamDoorTlv->field_10_bStart_closed == Choice_short::eYes_1 && !SwitchStates_Get(pSlamDoorTlv->field_14_switch_id)) || (pSlamDoorTlv->field_10_bStart_closed == Choice_short::eNo_0 && SwitchStates_Get(pSlamDoorTlv->field_14_switch_id))))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + gridSize),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
        TlvTypes::EnemyStopper_47);

    auto pEnemyStopper = static_cast<Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
    if (pEnemyStopper
        && (pEnemyStopper->field_10_stop_direction == direction || pEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2)
        && SwitchStates_Get(pEnemyStopper->field_12_switch_id))
    {
        return 1;
    }

    if (!checkBounds)
    {
        return 0;
    }

    if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos), // TODO: Abs() ??
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + gridSize),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = -pTable[mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame];
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = pTable[mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame];
        }
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    if (WallHit(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(50), mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        field_1D8_falling_velx_scale_factor = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = -(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromInteger(6));
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
    const FP prevXPos = mBaseAnimatedWithPhysicsGameObject_XPos;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->field_8_type != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->field_8_type != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = -1;
                    field_1D8_falling_velx_scale_factor = FP_FromDouble(0.35);
                }
            }
            else if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eBackgroundDynamicCollision_36)
            {
                GetOnPlatforms();
            }
        }
        else
        {
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

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

            mBaseAnimatedWithPhysicsGameObject_XPos = prevXPos + mBaseAnimatedWithPhysicsGameObject_VelX;
            field_1D8_falling_velx_scale_factor = FP_FromInteger(1);

            if (mCurrentMotion == eGlukkonMotions::M_KnockBack_3_442F40)
            {
                field_1D8_falling_velx_scale_factor = FP_FromDouble(0.67);
            }
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        SetAnim(eGlukkonMotions::M_Fall_7_443510, TRUE);
    }
}

void Glukkon::GetOnPlatforms()
{
    VOnCollisionWith(
        {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(5)), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5))},
        {FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(5)), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(5))},
        ObjList_5C1B78,
        1,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
}

SfxDefinition stepSfx_554840[3] = {
    {0u, 8u, 36u, 25u, 1524, 1905},
    {0u, 3u, 59u, 60u, 0, 254},
    {0u, 3u, 72u, 120u, 0, 254}};

void Glukkon::PlaySound(s32 sndIdx, Glukkon* pGlukkon)
{
    s32 volumeLeft, volumeRight;
    s32 defaultSndIdxVol = stepSfx_554840[sndIdx].field_3_default_volume;

    s16 pitch;
    if (sndIdx || !pGlukkon)
    {
        pitch = Math_RandomRange(stepSfx_554840[sndIdx].field_4_pitch_min, stepSfx_554840[sndIdx].field_6_pitch_max);
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

    if (pGlukkon->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else // Glukkon in background layer? TODO figure out if this does actually happen
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap.GetDirection_4811A0(
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_PathNumber,
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_XPos,
        pGlukkon->mBaseAnimatedWithPhysicsGameObject_YPos);
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
            FP percentHowFar = (FP_FromInteger(worldRect.w) - pGlukkon->mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            break;
        }
        case CameraPos::eCamRight_4:
        {
            FP percentHowFar = (pGlukkon->mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
            volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            break;
        }
        default:
            return;
    }

    if (pGlukkon->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5)) //TODO figure out if this does actually happen
    {
        volumeLeft = FP_GetExponent(FP_FromInteger(volumeLeft * 2) / FP_FromInteger(3));
        volumeRight = FP_GetExponent(FP_FromInteger(volumeRight * 2) / FP_FromInteger(3));
    }

    SFX_SfxDefinition_Play_Stereo(&stepSfx_554840[sndIdx], (s16) volumeLeft, (s16) volumeRight, pitch, pitch);
}

void Glukkon::ToDead()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        // When its a player controlled gluk go back to the screen the player is in
        sControlledCharacter_5C1B8C = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

        if (gMap.mLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                field_1E4_level,
                field_1E6_path,
                field_1E8_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    SwitchStates_Do_Operation(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);

    if ((field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryAslik_1 ||
        field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryDripik_2 ||
        field_1A8_tlvData.field_22_glukkon_type == GlukkonTypes::StoryPhleg_3) &&
        !SwitchStates_Get(field_1A8_tlvData.field_26_play_movie_switch_id))
    {
        // If an exec is dead trigger ze gas
        SwitchStates_Do_Operation(field_1A8_tlvData.field_24_death_switch_id, SwitchOp::eSetTrue_0);
    }

    if (field_1A8_tlvData.field_1C_spawn_switch_id == 0)
    {
        // Don't spawn again, dead
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_1A8_tlvData.field_1C_spawn_switch_id > 1)
        {
            // If above 1 then its an actual switch id
            SwitchStates_Do_Operation(field_1A8_tlvData.field_1C_spawn_switch_id, SwitchOp::eSetFalse_1);
        }

        // Spawn again if id is 1 or above
        Init();
    }
}

void Glukkon::VOn_TLV_Collision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);

                PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, 0, 0);
                ToDead();

                // Muds love it when people DIE
                Event_Broadcast(kEventMudokonComfort, this);
            }
        }

        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlv,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
    }
}

SfxDefinition gameSpeak_554858[15] = {
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
        calcedVolume = gameSpeak_554858[sndIdxShort].field_3_default_volume;
    }
    if (pGlukkon)
    {
        if (pGlukkon->mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
        {
            calcedVolume = FP_GetExponent(FP_FromInteger(calcedVolume * 2) / FP_FromInteger(3));
        }
    }
    SFX_SfxDefinition_Play_Mono(&gameSpeak_554858[sndIdxShort], calcedVolume, pitch, pitch);
}

Bool32 Glukkon::IsLineOfSightBetween(Glukkon* pGlukkon, BaseAliveGameObject* pOther)
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pathLine = nullptr;

    const PSX_RECT bRect = pOther->VGetBoundingRect();
    const FP X2 = FP_FromInteger((bRect.x + bRect.w) / 2);
    const FP Y2 = FP_FromInteger((bRect.y + bRect.y) / 2);

    return sCollisions->Raycast(
               pGlukkon->mBaseAnimatedWithPhysicsGameObject_XPos,
               pGlukkon->mBaseAnimatedWithPhysicsGameObject_YPos,
               X2,
               Y2,
               &pathLine,
               &hitX,
               &hitY,
               pGlukkon->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloorWallOrCeiling
               : kBgFloorWallOrCeiling)
        != 1;
}

void Glukkon::ToStand()
{
    field_1D8_falling_velx_scale_factor = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    SetAnim(eGlukkonMotions::M_Idle_0_442D10);
    MapFollowMe(TRUE);
}

void Glukkon::SlowDown(FP speed)
{
    if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX))
    {
        FollowLine();
        if (mBaseAnimatedWithPhysicsGameObject_VelX <= FP_FromInteger(0))
        {
            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * speed) + mBaseAnimatedWithPhysicsGameObject_VelX;
                if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
                {
                    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * speed);
            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            }
        }
    }
}

void Glukkon::VScreenChanged()
{
    BaseGameObject::VScreenChanged();
    SwitchStates_Do_Operation(field_1A8_tlvData.field_18_help_switch_id, SwitchOp::eSetFalse_1);
    if (BrainIs(&Glukkon::Brain_5_WaitToSpawn_442490) && !field_210_brain_sub_state)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Glukkon::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = -1;
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
            switch (pBullet->field_20_type)
            {
                case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
                case BulletType::eNormalBullet_2:
                {
                    {
                        const FP yRand = (FP_FromInteger(Math_NextRandom() % 16)) - FP_FromInteger(8);
                        const FP xRand = FP_FromInteger(Math_NextRandom() & 0xF); // TODO: Might be wrong as was trying to make this abs() but result is unsigned anyway ??

                        const FP xPos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6)));
                        relive_new Blood(
                            xPos + mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                            ((pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(1) : FP_FromInteger(1)) * xRand + FP_FromInteger(16)),
                            yRand,
                            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                            12);
                    }

                    {
                        const FP xPos = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * (pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(12) : FP_FromInteger(12)));
                        relive_new Blood(
                            xPos + mBaseAnimatedWithPhysicsGameObject_XPos,
                            mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                            pBullet->field_30_x_distance <= FP_FromInteger(0) ? -FP_FromInteger(6) : FP_FromInteger(6),
                            FP_FromInteger(0),
                            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                            8);
                    }
                }
                break;

                case BulletType::ePossessedSligZBullet_1:
                case BulletType::eZBullet_3:
                {
                    relive_new Blood(
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                        FP_FromInteger(0),
                        FP_FromInteger(0),
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale,
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
                Event_Broadcast(kEventMudokonComfort, this);
                return 1;
            }

            if (mCurrentMotion == eGlukkonMotions::M_KnockBack_3_442F40)
            {
                if (mHealth > FP_FromInteger(0))
                {
                    SetBrain(&Glukkon::Brain_4_Death_442010);
                    field_210_brain_sub_state = 4;
                }
                mHealth = FP_FromInteger(0);
                Event_Broadcast(kEventMudokonComfort, this);
                return 1;
            }

            field_204_getting_shot_timer = sGnFrame + 20;
            SetAnim(eGlukkonMotions::M_GetShot_21_443A60, TRUE);

            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 4;

            if (pBullet->field_30_x_distance >= FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromDouble(0.001);
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -FP_FromDouble(0.001);
            }
            mHealth = FP_FromInteger(0);
            Event_Broadcast(kEventMudokonComfort, this);
        }
        break;

        case ReliveTypes::eDrill:
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eExplosion:
            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 2;
            Event_Broadcast(kEventMudokonComfort, this);
            break;

        case ReliveTypes::eElectricWall:
            Glukkon::PlaySound_GameSpeak(GlukkonSpeak::Unused_9, 0, field_1E0_gamespeak_pitch, this);
            return 1;

        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eMineCar:
            SetAnim(eGlukkonMotions::M_DeathFall_8_443760, TRUE);
            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 0;
            Event_Broadcast(kEventMudokonComfort, this);
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
                field_210_brain_sub_state = 0;
            }
            break;

        case ReliveTypes::eSlog:
            if (mCurrentMotion != eGlukkonMotions::M_KnockBack_3_442F40)
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&Glukkon::Brain_4_Death_442010);
                field_210_brain_sub_state = 5;
                Environment_SFX_457A40(EnvironmentSfx::eKnockback_13, 0, 32767, this);
                Event_Broadcast(kEventMudokonComfort, this);
                if (!VIsFacingMe(static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)))
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
                }
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                SetAnim(eGlukkonMotions::M_KnockBack_3_442F40, TRUE);
            }
            break;

        case ReliveTypes::eElectrocute:
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            mHealth = FP_FromInteger(0);
            SetBrain(&Glukkon::Brain_4_Death_442010);
            field_210_brain_sub_state = 3;
            field_1D4_timer = sGnFrame + 1;
            Event_Broadcast(kEventMudokonComfort, this);
            break;

        default:
            return 1;
    }

    return 1;
}
