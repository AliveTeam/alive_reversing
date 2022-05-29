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

    field_DC_bApplyShadows |= 2u;

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_SligGreeterFarts_Half_14;
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_SligGreeterFarts_33;
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }


    mFlags.Set(BaseGameObject::eCanExplode_Bit7);

    if (pTlv->field_14_start_direction == XDirection_short::eLeft_0)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }

    field_134_speed = FP_FromInteger(pTlv->field_12_motion_detector_speed);
    field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
    field_12E_bDontSetDestroyed = 1;
    field_118_tlvInfo = tlvInfo;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(24),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale ? 1 : 16))
    {
        field_BC_ypos = hitY;
    }

    auto pMotionDetctor = ae_new<MotionDetector>();
    if (pMotionDetctor)
    {
        pMotionDetctor->ctor_4683B0(0, 0, this);
        field_11C_motionDetectorId = pMotionDetctor->field_8_object_id;
    }

    field_140_targetOnRight = 0;
    field_13E_targetOnLeft = 0;

    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(70, 210);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    field_12C_timesShot = 0;

    field_E0_pShadow = ae_new<Shadow>();

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_130_bChasing = 0;
}

void Greeter::VUpdate()
{
    vUpdate_4469B0();
}

void Greeter::VScreenChanged()
{
    vScreenChanged_447DD0();
}

void Greeter::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_447DB0(pFrom);
}

s16 Greeter::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_447C20(pFrom);
}

s32 Greeter::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_446400(reinterpret_cast<Greeter_State*>(pSaveBuffer));
}

s32 CC Greeter::CreateFromSaveState_446040(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Greeter_State*>(pBuffer);
    auto pTlv = static_cast<Path_Greeter*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_28_tlvInfo));

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
        pGreeter->field_B8_xpos = pState->field_C_xpos;
        pGreeter->field_BC_ypos = pState->field_10_ypos;
        pGreeter->field_C4_velx = pState->field_14_velx;
        pGreeter->field_C8_vely = pState->field_18_vely;

        pGreeter->field_C0_path_number = pState->field_8_path_number;
        pGreeter->field_C2_lvl_number = pState->field_A_lvl_number;
        pGreeter->field_CC_sprite_scale = pState->field_1C_sprite_scale;

        pGreeter->field_D0_r = pState->field_2_r;
        pGreeter->field_D2_g = pState->field_4_g;
        pGreeter->field_D4_b = pState->field_6_b;

        pGreeter->field_20_animation.field_92_current_frame = pState->field_20_current_frame;
        pGreeter->field_20_animation.field_E_frame_change_counter = pState->field_22_frame_change_counter;

        pGreeter->mFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);

        pGreeter->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

        if (IsLastFrame(&pGreeter->field_20_animation))
        {
            pGreeter->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
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

        auto pDetector = static_cast<MotionDetector*>(sObjectIds.Find_449CF0(pGreeter->field_11C_motionDetectorId));

        auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_449CF0(pDetector->field_F8_laser_id));
        pLaser->field_B8_xpos = pState->field_4C_motion_laser_xpos;
    }

    return sizeof(Greeter_State);
}

s32 Greeter::vGetSaveState_446400(Greeter_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = AETypes::eGreeter_64;

    pState->field_C_xpos = field_B8_xpos;
    pState->field_10_ypos = field_BC_ypos;
    pState->field_14_velx = field_C4_velx;
    pState->field_18_vely = field_C8_vely;

    pState->field_8_path_number = field_C0_path_number;
    pState->field_A_lvl_number = field_C2_lvl_number;
    pState->field_1C_sprite_scale = field_CC_sprite_scale;

    pState->field_2_r = field_D0_r;
    pState->field_4_g = field_D2_g;
    pState->field_6_b = field_D4_b;

    pState->field_20_current_frame = field_20_animation.field_92_current_frame;
    pState->field_22_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_25_bDrawable = mFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
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

    auto pMotionDetector = static_cast<MotionDetector*>(sObjectIds.Find_449CF0(field_11C_motionDetectorId));
    auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds.Find_449CF0(pMotionDetector->field_F8_laser_id));
    pState->field_4C_motion_laser_xpos = pLaser->field_B8_xpos;

    return sizeof(Greeter_State);
}

void Greeter::vScreenChanged_447DD0()
{
    BaseGameObject::VScreenChanged();

    if (sControlledCharacter_5C1B8C)
    {
        const FP xDistFromPlayer = FP_Abs(sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos);
        if (xDistFromPlayer > FP_FromInteger(356))
        {
            mFlags.Set(BaseGameObject::eDead);
            return;
        }

        const FP yDistFromPlayer = FP_Abs(sControlledCharacter_5C1B8C->field_BC_ypos - field_BC_ypos);
        if (yDistFromPlayer > FP_FromInteger(240))
        {
            mFlags.Set(BaseGameObject::eDead);
            return;
        }
    }
}

Greeter::~Greeter()
{
    if (field_12E_bDontSetDestroyed)
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }

    BaseGameObject* pMotionDetector = sObjectIds.Find_449CF0(field_11C_motionDetectorId);
    if (pMotionDetector)
    {
        pMotionDetector->mFlags.Set(BaseGameObject::eDead);
    }
}

EXPORT void Greeter::BlowUp_447E50()
{
    field_10C_health = FP_FromInteger(0);

    ae_new<Explosion>(
        field_B8_xpos,
        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
        field_CC_sprite_scale,
        0);

    ae_new<Gibs>(
        GibType::Metal_5,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(50),
        FP_FromInteger(0),
        FP_FromInteger(0),
        field_CC_sprite_scale,
        0);

    mFlags.Set(BaseGameObject::eDead);
    field_12E_bDontSetDestroyed = 0;
}

void Greeter::ChangeDirection_447BD0()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_1_PatrolTurn;
    field_C4_velx = FP_FromInteger(0);
    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Turn);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    field_124_last_turn_time = sGnFrame_5C1B84;
}

void Greeter::BounceBackFromShot_447B10()
{
    field_13C_brain_state = GreeterBrainStates::eBrain_5_Knockback;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = FP_FromInteger(-2);
    }
    else
    {
        field_C4_velx = FP_FromInteger(2);
    }

    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;

    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Hit);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

    const CameraPos soundDirection = gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);
    SFX_Play_46FC20(SoundEffect::GreeterKnockback_121, 0, soundDirection, field_CC_sprite_scale);
}

void Greeter::HandleRollingAlong_447860()
{
    for (Path_TLV* pTlv = field_138_pTlv; pTlv;
         pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv,
                                                      field_C4_velx + field_B8_xpos + field_C4_velx,
                                                      field_C8_vely + field_BC_ypos + field_C8_vely,
                                                      field_C4_velx + field_B8_xpos + field_C4_velx,
                                                      field_C8_vely + field_BC_ypos + field_C8_vely))
    {
        switch (pTlv->field_4_type.mType)
        {
            case TlvTypes::DeathDrop_4:
                BlowUp_447E50();
                break;

            case TlvTypes::ScrabLeftBound_43:
                if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection_447BD0();
                }
                break;

            case TlvTypes::ScrabRightBound_44:
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
                {
                    ChangeDirection_447BD0();
                }
                break;

            case TlvTypes::EnemyStopper_47:
                if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
                {
                    ChangeDirection_447BD0();
                }
                break;

            default:
                continue;
        }
    }

    if (field_13C_brain_state == GreeterBrainStates::eBrain_0_Patrol)
    {
        if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_408E90(0, 1)) || WallHit_408750(field_CC_sprite_scale * FP_FromInteger(40), field_C4_velx * FP_FromInteger(3)) || (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) && Check_IsOnEndOfLine_408E90(1, 1)))
        {
            ChangeDirection_447BD0();
        }
    }

    if (field_13C_brain_state == GreeterBrainStates::eBrain_4_Chase)
    {
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(40), field_C4_velx * FP_FromInteger(3))) // TODO: OG bug, raw * 3 here ??
        {
            BounceBackFromShot_447B10();
        }
    }
}

EXPORT s16 Greeter::vTakeDamage_447C20(BaseGameObject* pFrom)
{
    if (mFlags.Get(BaseGameObject::eDead) || FP_GetExponent(field_10C_health) == 0)
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
        case AETypes::eBaseBomb_46:
        case AETypes::eRockSpawner_48:
        case AETypes::eMeatSaw_86:
        case AETypes::eMineCar_89:
        case AETypes::eNeverSet_107:
        case AETypes::eExplosion_109:
            BlowUp_447E50();
            return 1;

        case AETypes::eSlamDoor_122:
            BounceBackFromShot_447B10();
            return 1;

        case AETypes::eElectrocute_150:
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            BlowUp_447E50();
            return 1;

        default:
            BlowUp_447E50();
            return 1;
    }
}

void Greeter::vOnThrowableHit_447DB0(BaseGameObject* /*pFrom*/)
{
    BounceBackFromShot_447B10();
}

void Greeter::ZapTarget_447320(FP xpos, FP ypos, BaseAliveGameObject* pTarget)
{
    ae_new<ScreenShake>(0, 0);

    ae_new<ZapLine>(
        field_B8_xpos,
        field_BC_ypos - (FP_FromInteger(20) * field_CC_sprite_scale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    ae_new<ZapLine>(
        field_B8_xpos,
        field_BC_ypos,
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    ae_new<ZapLine>(
        field_B8_xpos,
        field_BC_ypos - (FP_FromInteger(50) * field_CC_sprite_scale),
        xpos,
        ypos,
        8,
        ZapLineType::eThick_0,
        Layer::eLayer_ZapLinesMuds_28);

    ae_new<ParticleBurst>(
        xpos,
        ypos,
        10,
        field_CC_sprite_scale,
        BurstType::eBigRedSparks_3,
        11);

    ae_new<ParticleBurst>(
        field_B8_xpos,
        field_BC_ypos - (FP_FromInteger(10) * field_CC_sprite_scale),
        10,
        field_CC_sprite_scale,
        BurstType::eBigRedSparks_3,
        11);

    pTarget->field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted);

    ae_new<Electrocute>(pTarget, TRUE, TRUE);

    pTarget->VTakeDamage_408730(this);

    const CameraPos soundDirection = gMap.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos);

    SFX_Play_46FC20(SoundEffect::Zap1_49, 0, soundDirection, field_CC_sprite_scale);
    SFX_Play_46FC20(SoundEffect::Zap2_50, 0, soundDirection, field_CC_sprite_scale);

    RandomishSpeak_447A70(GreeterSpeak::eLaugh_3);

    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(160, 200);
    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;
}

void Greeter::RandomishSpeak_447A70(GreeterSpeak effect)
{
    field_13C_brain_state = GreeterBrainStates::eBrain_2_Speak;
    field_C4_velx = FP_FromInteger(0);
    const AnimRecord& animRec = AnimRec(AnimId::Greeter_Speak);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
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

Bool32 Greeter::ZapIsNotBlocked_447240(BaseAliveGameObject* pUs, BaseAliveGameObject* pThem)
{
    PSX_RECT usRect = {};
    vGetBoundingRect_424FD0(&usRect, 1);

    PSX_RECT bRectThem = {};
    pThem->vGetBoundingRect_424FD0(&bRectThem, 1);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(
               pUs->field_B8_xpos,
               FP_FromInteger(usRect.h + 0xFFE7),
               pThem->field_B8_xpos,
               FP_FromInteger(bRectThem.h + 0xFFE7),
               &pLine,
               &hitX,
               &hitY,
               pUs->field_D6_scale != 0 ? 6 : 0x60)
        == 1;
}

BaseAliveGameObject* Greeter::GetMudToZap_447690()
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
            pObj->vGetBoundingRect_424FD0(&bRect, 1);

            const FP xMid = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP yMid = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (xMid - field_B8_xpos < (field_CC_sprite_scale * FP_FromInteger(60)) && field_B8_xpos - xMid < (field_CC_sprite_scale * FP_FromInteger(60)) && yMid - (field_BC_ypos - FP_FromInteger(4)) < (field_CC_sprite_scale * FP_FromInteger(60)) && field_BC_ypos - FP_FromInteger(4) - yMid < (field_CC_sprite_scale * FP_FromInteger(60)) && !(sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted)) && !ZapIsNotBlocked_447240(this, pObj))
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

void Greeter::vUpdate_4469B0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    switch (field_13C_brain_state)
    {
        case GreeterBrainStates::eBrain_0_Patrol:
            if (!((sGnFrame_5C1B84 - field_124_last_turn_time) % 14))
            {
                const CameraPos soundDirection = gMap.GetDirection_4811A0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_B8_xpos,
                    field_BC_ypos);
                SFX_Play_46FC20(SoundEffect::WheelSqueak_31, 10, soundDirection, field_CC_sprite_scale);
            }

            field_C8_vely = FP_FromInteger(0);
            if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) == 0)
            {
                field_C4_velx = -(field_CC_sprite_scale * FP_FromInteger(3));
                if (field_13E_targetOnLeft)
                {
                    RandomishSpeak_447A70(GreeterSpeak::eHi_0);
                    field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                }
                else if (field_140_targetOnRight)
                {
                    ChangeDirection_447BD0();
                    field_13C_brain_state = GreeterBrainStates::eBrain_6_ToChase;
                }
            }
            else
            {
                field_C4_velx = (field_CC_sprite_scale * FP_FromInteger(3));
                if (field_140_targetOnRight)
                {
                    RandomishSpeak_447A70(GreeterSpeak::eHi_0);
                    field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                }
                else if (field_13E_targetOnLeft)
                {
                    ChangeDirection_447BD0();
                    field_13C_brain_state = GreeterBrainStates::eBrain_6_ToChase;
                }
            }

            if (static_cast<s32>(sGnFrame_5C1B84) > field_128_timer)
            {
                RandomishSpeak_447A70(GreeterSpeak::eRandomized_1000);
            }
            break;

        case GreeterBrainStates::eBrain_1_PatrolTurn:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                field_C8_vely = FP_FromInteger(0);
                field_13E_targetOnLeft = 0;
                field_140_targetOnRight = 0;
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                }
            }
            break;

        case GreeterBrainStates::eBrain_2_Speak:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 0;
                field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                field_C8_vely = FP_FromInteger(0);
                field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_3_ChaseSpeak:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_130_bChasing = 1;
                field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                const AnimRecord& animRec = AnimRec(AnimId::Greeter_Chase);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                field_C8_vely = FP_FromInteger(0);
            }
            break;

        case GreeterBrainStates::eBrain_4_Chase:
        {
            if (!(sGnFrame_5C1B84 % 8))
            {
                const CameraPos soundDirection2 = gMap.GetDirection_4811A0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_B8_xpos,
                    field_BC_ypos);
                SFX_Play_46FC20(SoundEffect::WheelSqueak_31, 10, soundDirection2, field_CC_sprite_scale);
            }

            field_C4_velx = -(field_CC_sprite_scale * FP_FromInteger(5));
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromInteger(5);
            }

            PSX_RECT bRect = {};
            sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bRect, 1);

            const FP midX = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP midY = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (midX - field_B8_xpos >= (field_CC_sprite_scale * FP_FromInteger(60)) || field_B8_xpos - midX >= (field_CC_sprite_scale * FP_FromInteger(60)) || midY - (field_BC_ypos - FP_FromInteger(4)) >= (field_CC_sprite_scale * FP_FromInteger(60)) || field_BC_ypos - FP_FromInteger(4) - midY >= (field_CC_sprite_scale * FP_FromInteger(60)) || sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted) || sActiveHero_5C1B68->CantBeDamaged_44BAB0() || ZapIsNotBlocked_447240(this, sActiveHero_5C1B68))
            {
                BaseAliveGameObject* pGonnaZapYa = GetMudToZap_447690();
                if (pGonnaZapYa)
                {
                    PSX_RECT bZapRect = {};
                    pGonnaZapYa->vGetBoundingRect_424FD0(&bZapRect, 1);

                    ZapTarget_447320(
                        FP_FromInteger((bZapRect.x + bZapRect.w) / 2),
                        FP_FromInteger((bZapRect.y + bZapRect.h) / 2),
                        pGonnaZapYa);
                }
            }
            else
            {
                ZapTarget_447320(midX, midY, sActiveHero_5C1B68);
            }
        }
        break;

        case GreeterBrainStates::eBrain_5_Knockback:
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(40), FP_FromRaw(3 * field_C4_velx.fpValue))) // TODO: OG bug, why * 3 and not * FP 3??
            {
                field_C4_velx = FP_FromInteger(0);
            }

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak_447A70(GreeterSpeak::eWhat_9);
                field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(160, 200);
            }
            break;

        case GreeterBrainStates::eBrain_6_ToChase:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                RandomishSpeak_447A70(GreeterSpeak::eHi_0);
                field_13C_brain_state = GreeterBrainStates::eBrain_3_ChaseSpeak;
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                }
                else
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                }
            }
            break;

        case GreeterBrainStates::eBrain_7_Fall:
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            if (InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
            {
                field_C8_vely = -field_C8_vely * FP_FromDouble(0.4);
                field_100_pCollisionLine = pLine;
                field_BC_ypos = hitY;
                field_F8_LastLineYPos = hitY;

                const CameraPos soundDirection3 = gMap.GetDirection_4811A0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_B8_xpos,
                    hitY);

                SFX_Play_46FC20(SoundEffect::GreeterLand_120, 0, soundDirection3, field_CC_sprite_scale);
                if (field_C8_vely > -FP_FromInteger(1))
                {
                    field_C8_vely = FP_FromInteger(0);
                    if (!field_130_bChasing)
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_0_Patrol;
                        const AnimRecord& animRec = AnimRec(AnimId::Greeter_Moving);
                        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                    }
                    else
                    {
                        field_13C_brain_state = GreeterBrainStates::eBrain_4_Chase;
                        const AnimRecord& animRec = AnimRec(AnimId::Greeter_Chase);
                        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                    }
                }
            }
        }
        break;

        default:
            break;
    }

    if (FP_GetExponent(field_C4_velx) || FP_GetExponent(field_C8_vely))
    {
        if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
        {
            const FP xpos = field_C4_velx
                          + field_C4_velx
                          + field_C4_velx
                          + field_C4_velx
                          + field_B8_xpos;

            const FP ypos = field_C8_vely
                          + field_BC_ypos
                          + field_C8_vely;

            field_138_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(nullptr, xpos, ypos, xpos, ypos);
            HandleRollingAlong_447860();
        }
    }

    bool collisionCheck = true;
    if (field_13C_brain_state == GreeterBrainStates::eBrain_7_Fall)
    {
        field_138_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
        HandleRollingAlong_447860();
        if (field_13C_brain_state == GreeterBrainStates::eBrain_7_Fall)
        {
            collisionCheck = false;
        }
    }

    if (collisionCheck)
    {
        if (Check_IsOnEndOfLine_408E90(0, 0))
        {
            field_13C_brain_state = GreeterBrainStates::eBrain_7_Fall;
            const AnimRecord& animRec = AnimRec(AnimId::Greeter_Falling);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        }
    }

    if (field_13C_brain_state != GreeterBrainStates::eBrain_7_Fall)
    {
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;
    }
}
