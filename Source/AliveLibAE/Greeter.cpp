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

EXPORT Greeter* Greeter::ctor_4465B0(Path_Greeter* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x54566C);

    field_4_typeId = Types::eGreeter_64;
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kGreeterResID);
    Animation_Init_424E10(50028, 65, 60, ppRes, 1, 1);

    field_DC_bApplyShadows |= 2u;

    if (pTlv->field_10_scale)
    {
        field_20_animation.field_C_render_layer = 14;
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_20_animation.field_C_render_layer = 33;
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }


    field_6_flags.Set(BaseGameObject::eCanExplode_Bit7);

    if (pTlv->field_14_direction == 0)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }

    field_134_speed = FP_FromInteger(pTlv->field_12_motion_detector_speed);
    field_13C_state = GreeterStates::eState_0_Patrol;
    field_12E = 1;
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

    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kMetalResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    field_12C = 0;

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_130 = 0;

    return this;
}

BaseGameObject* Greeter::VDestructor(signed int flags)
{
    return vdtor_4468B0(flags);
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

signed __int16 Greeter::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_447C20(pFrom);
}

int Greeter::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_446400(reinterpret_cast<Greeter_State*>(pSaveBuffer));
}

int CC Greeter::CreateFromSaveState_446040(const BYTE* pBuffer)
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
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMetalResID, FALSE, FALSE))
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

    auto pGreeter = ae_new<Greeter>();
    pGreeter->ctor_4465B0(pTlv, pState->field_28_tlvInfo);

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

    pGreeter->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_25_bDrawable & 1);

    pGreeter->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_24_bAnimRender & 1);

    if (IsLastFrame(&pGreeter->field_20_animation))
    {
        pGreeter->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pGreeter->field_118_tlvInfo = pState->field_28_tlvInfo;
    pGreeter->field_120 = pState->field_2C;
    pGreeter->field_124_last_turn_time = pState->field_30_last_turn_time;
    pGreeter->field_128_timer = pState->field_34_timer;
    pGreeter->field_12C = pState->field_38;
    pGreeter->field_12E = pState->field_3A;
    pGreeter->field_130 = pState->field_3C;
    pGreeter->field_134_speed = pState->field_40_speed;
    pGreeter->field_13C_state = pState->field_44_state;
    pGreeter->field_13E_targetOnLeft = pState->field_46_targetOnLeft;
    pGreeter->field_140_targetOnRight = pState->field_48_targetOnRight;

    auto pDetector = static_cast<MotionDetector*>(sObjectIds_5C1B70.Find_449CF0(pGreeter->field_11C_motionDetectorId));

    auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds_5C1B70.Find_449CF0(pDetector->field_F8_laser_id));
    pLaser->field_B8_xpos = pState->field_4C_motion_laser_xpos;

    return sizeof(Greeter_State);
}

int Greeter::vGetSaveState_446400(Greeter_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = Types::eGreeter_64;

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
    pState->field_25_bDrawable = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_24_bAnimRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_28_tlvInfo = field_118_tlvInfo;
    pState->field_2C = field_120;
    pState->field_30_last_turn_time = field_124_last_turn_time;
    pState->field_34_timer = field_128_timer;

    pState->field_38 = field_12C;
    pState->field_3A = field_12E;
    pState->field_3C = field_130;

    pState->field_40_speed = field_134_speed;
    pState->field_44_state = field_13C_state;
    pState->field_46_targetOnLeft = field_13E_targetOnLeft;
    pState->field_48_targetOnRight = field_140_targetOnRight;
    
    auto pMotionDetector = static_cast<MotionDetector*>(sObjectIds_5C1B70.Find_449CF0(field_11C_motionDetectorId));
    auto pLaser = static_cast<MotionDetectorLaser*>(sObjectIds_5C1B70.Find_449CF0(pMotionDetector->field_F8_laser_id));
    pState->field_4C_motion_laser_xpos = pLaser->field_B8_xpos;

    return sizeof(Greeter_State);
}

Greeter* Greeter::vdtor_4468B0(signed int flags)
{
    dtor_4468E0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Greeter::vScreenChanged_447DD0()
{
    BaseGameObject::VScreenChanged();

    if (sControlledCharacter_5C1B8C)
    {
        const FP xDistFromPlayer = FP_Abs(sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos);
        if (xDistFromPlayer > FP_FromInteger(356))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        const FP yDistFromPlayer = FP_Abs(sControlledCharacter_5C1B8C->field_BC_ypos  - field_BC_ypos);
        if (yDistFromPlayer > FP_FromInteger(240))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }
    }
}

void Greeter::dtor_4468E0()
{
    SetVTable(this, 0x54566C);

    if (field_12E)
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }

    BaseGameObject* pMotionDetector = sObjectIds_5C1B70.Find_449CF0(field_11C_motionDetectorId);
    if (pMotionDetector)
    {
        pMotionDetector->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    dtor_4080B0();
}

EXPORT void Greeter::BlowUp_447E50()
{
    field_10C_health = FP_FromInteger(0);
    
    auto pExplosion = ae_new<Explosion>();
    if (pExplosion)
    {
        pExplosion->ctor_4A1200(
            field_B8_xpos,
            field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
            field_CC_sprite_scale,
            0);
    }

    auto pGibs = ae_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(
            GibType::Metal_5,
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(50),
            FP_FromInteger(0),
            FP_FromInteger(0),
            field_CC_sprite_scale,
            0);
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    field_12E = 0;
}

void Greeter::ChangeDirection_447BD0()
{
    field_13C_state = GreeterStates::eState_1_PatrolTurn;
    field_C4_velx = FP_FromInteger(0);
    field_20_animation.Set_Animation_Data_409C80(50072, nullptr);
    field_124_last_turn_time = sGnFrame_5C1B84;
}

void Greeter::BounceBackFromShot_447B10()
{
    field_13C_state = GreeterStates::eState_5_Knockback;
   
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

    field_20_animation.Set_Animation_Data_409C80(50236, nullptr);

    const CameraPos soundDirection = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);
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
        switch (pTlv->field_4_type)
        {
        case DeathDrop_4:
            BlowUp_447E50();
            break;

        case ScrabLeftBound_43: 
            if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))  && field_13C_state == GreeterStates::eState_0_Patrol)
            {
                ChangeDirection_447BD0();
            }
            break;

        case ScrabRightBound_44:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && field_13C_state == GreeterStates::eState_0_Patrol)
            {
                ChangeDirection_447BD0();
            }
            break;

        case EnemyStopper_47:
            if (field_13C_state != GreeterStates::eState_7_Fall)
            {
                ChangeDirection_447BD0();
            }
            break;

        default:
            continue;
        }
    }

    if (field_13C_state == GreeterStates::eState_0_Patrol)
    {
        if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_IsOnEndOfLine_408E90(0, 1)) ||
            WallHit_408750(field_CC_sprite_scale * FP_FromInteger(40), field_C4_velx * FP_FromInteger(3)) ||
            (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) && Check_IsOnEndOfLine_408E90(1, 1)))
        {
            ChangeDirection_447BD0();
        }
    }

    if (field_13C_state == GreeterStates::eState_4_Chase)
    {
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(40), field_C4_velx * FP_FromInteger(3))) // TODO: OG bug, raw * 3 here ??
        {
            BounceBackFromShot_447B10();
        }
    }
}

EXPORT signed __int16 Greeter::vTakeDamage_447C20(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3) || FP_GetExponent(field_10C_health) == 0)
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eBullet_15:
        if (static_cast<Bullet*>(pFrom)->field_30 <= FP_FromInteger(0))
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }

        if (++field_12C <= 10)
        {
            BounceBackFromShot_447B10();
        }
        else
        {
            BlowUp_447E50();
        }
        return 1;

    case Types::eGrinder_30:
    case Types::eElectricWall_39:
        if (static_cast<BaseAnimatedWithPhysicsGameObject*>(pFrom)->field_20_animation.field_10_frame_delay <= 0)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
        }

        if (++field_12C <= 10)
        {
            BounceBackFromShot_447B10();
        }
        else
        {
            BlowUp_447E50();
        }
        return 1;

    case Types::eBaseBomb_46:
    case Types::eRockSpawner_48:
    case Types::eType_86:
    case Types::eMineCar_89:
    case Types::eType_107:
    case Types::eExplosion_109:
        BlowUp_447E50();
        return 1;

    case Types::eSlamDoor_122:
        BounceBackFromShot_447B10();
        return 1;

    case Types::eElectrocute_150:
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        BlowUp_447E50();
        return 1;

    default:
        return 1;
    }
}

void Greeter::vOnThrowableHit_447DB0(BaseGameObject* /*pFrom*/)
{
    BounceBackFromShot_447B10();
}

void Greeter::ZapTarget_447320(FP xpos, FP ypos, BaseAliveGameObject* pTarget)
{
    auto pScreenShake = ae_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4ACF70(0, 0);
    }

    auto pZapLine = ae_new<ZapLine>();
    if (pZapLine)
    {
        pZapLine->ctor_4CC690(
            field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(20) * field_CC_sprite_scale),
            xpos,
            ypos,
            8,
            0,
            28);
    }

    auto pZapLine2 = ae_new<ZapLine>();
    if (pZapLine2)
    {
        pZapLine2->ctor_4CC690(
            field_B8_xpos,
            field_BC_ypos,
            xpos,
            ypos,
            8,
            0,
            28);
    }

    auto pZapLine3 = ae_new<ZapLine>();
    if (pZapLine3)
    {
        pZapLine3->ctor_4CC690(
            field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(50) * field_CC_sprite_scale),
            xpos,
            ypos,
            8,
            0,
            28);
    }

    auto pParticleBurst = ae_new<ParticleBurst>();
    if (pParticleBurst)
    {
        pParticleBurst->ctor_41CF50(
            xpos,
            ypos,
            10,
            field_CC_sprite_scale,
            BurstType::eBigRedSparks_3,
            11);
    }

    auto pParticleBurst2 = ae_new<ParticleBurst>();
    if (pParticleBurst2)
    {
        pParticleBurst2->ctor_41CF50(
            field_B8_xpos,
            field_BC_ypos - (FP_FromInteger(10) * field_CC_sprite_scale),
            10,
            field_CC_sprite_scale,
            BurstType::eBigRedSparks_3,
            11);
    }

    pTarget->field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted);

    auto pElectrocute = ae_new<Electrocute>();
    if (pElectrocute)
    {
        pElectrocute->ctor_4E5E80(pTarget, TRUE, TRUE);
    }

    pTarget->VTakeDamage_408730(this);

    const CameraPos soundDirection = gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos);

    SFX_Play_46FC20(SoundEffect::Zap1_49, 0, soundDirection, field_CC_sprite_scale);
    SFX_Play_46FC20(SoundEffect::Zap2_50, 0, soundDirection, field_CC_sprite_scale);
    
    RandomishSpeak_447A70(GreeterSpeak::Laugh_3);

    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(160, 200);
    field_13E_targetOnLeft = 0;
    field_140_targetOnRight = 0;
}

void Greeter::RandomishSpeak_447A70(GreeterSpeak effect)
{
    field_13C_state = GreeterStates::eState_2_Speak;
    field_C4_velx = FP_FromInteger(0);
    field_20_animation.Set_Animation_Data_409C80(50104, nullptr);
    field_120 = sGnFrame_5C1B84 + 25;

    if (effect == GreeterSpeak::Randomized_1000)
    {
        const int randomSpeak = static_cast<int>(sGnFrame_5C1B84 % 4);
        // Will be one of: Hi_0, HereBoy_1,  GetHim_2 or Laugh_3
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(randomSpeak), 100, 700, this);
    }
    else
    {
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(effect), 100, 700, this);
    }
}

BOOL Greeter::ZapIsNotBlocked_447240(BaseAliveGameObject* pUs, BaseAliveGameObject* pThem)
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
        pUs->field_D6_scale != 0 ? 6 : 0x60) == 1;
}

BaseAliveGameObject* Greeter::GetMudToZap_447690()
{
    for (int idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eMudokon_110)
        {
            PSX_RECT bRect = {};
            pObj->vGetBoundingRect_424FD0(&bRect, 1);

            const FP xMid = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP yMid = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (xMid - field_B8_xpos < (field_CC_sprite_scale * FP_FromInteger(60)) &&
                field_B8_xpos - xMid < (field_CC_sprite_scale * FP_FromInteger(60)) &&
                yMid - (field_BC_ypos - FP_FromInteger(4)) < (field_CC_sprite_scale * FP_FromInteger(60))&&
                field_BC_ypos - FP_FromInteger(4) - yMid < (field_CC_sprite_scale * FP_FromInteger(60)) &&
                !(sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted)) && 
                !ZapIsNotBlocked_447240(this, pObj))
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
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_13C_state)
    {
    case GreeterStates::eState_0_Patrol:
        if (!((sGnFrame_5C1B84 - field_124_last_turn_time) % 14))
        {
            const CameraPos soundDirection = gMap_5C3030.GetDirection_4811A0(
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
                RandomishSpeak_447A70(GreeterSpeak::Hi_0);
                field_13C_state = GreeterStates::eState_3_ChaseSpeak;
            }
            else if (field_140_targetOnRight)
            {
                ChangeDirection_447BD0();
                field_13C_state = GreeterStates::eState_6_ToChase;
            }
        }
        else
        {
            field_C4_velx = (field_CC_sprite_scale * FP_FromInteger(3));
            if (field_140_targetOnRight)
            {
                RandomishSpeak_447A70(GreeterSpeak::Hi_0);
                field_13C_state = GreeterStates::eState_3_ChaseSpeak;
            }
            else if (field_13E_targetOnLeft)
            {
                ChangeDirection_447BD0();
                field_13C_state = GreeterStates::eState_6_ToChase;
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_128_timer)
        {
            RandomishSpeak_447A70(GreeterSpeak::Randomized_1000);
        }
        break;

    case GreeterStates::eState_1_PatrolTurn:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_13C_state = GreeterStates::eState_0_Patrol;
            field_20_animation.Set_Animation_Data_409C80(50028, 0);
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

    case GreeterStates::eState_2_Speak:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_130 = 0;
            field_13C_state = GreeterStates::eState_0_Patrol;
            field_20_animation.Set_Animation_Data_409C80(50028, 0);
            field_C8_vely = FP_FromInteger(0);
            field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(160, 200);
        }
        break;

    case GreeterStates::eState_3_ChaseSpeak:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_130 = 1;
            field_13C_state = GreeterStates::eState_4_Chase;
            field_20_animation.Set_Animation_Data_409C80(50144, 0);
            field_C8_vely = FP_FromInteger(0);
        }
        break;

    case GreeterStates::eState_4_Chase:
    {
        if (!(sGnFrame_5C1B84 % 8))
        {
            const CameraPos soundDirection2 = gMap_5C3030.GetDirection_4811A0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos);
            SFX_Play_46FC20(SoundEffect::WheelSqueak_31, 10, soundDirection2, field_CC_sprite_scale);
        }

        field_C4_velx = -(field_CC_sprite_scale * FP_FromInteger(5));
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_C4_velx = field_CC_sprite_scale* FP_FromInteger(5);
        }

        PSX_RECT bRect = {};
        sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bRect, 1);

        const FP midX = FP_FromInteger((bRect.x + bRect.w) / 2);
        const FP midY = FP_FromInteger((bRect.y + bRect.h) / 2);

        if (midX - field_B8_xpos >= (field_CC_sprite_scale * FP_FromInteger(60)) ||
            field_B8_xpos - midX >= (field_CC_sprite_scale * FP_FromInteger(60)) ||
            midY - (field_BC_ypos - FP_FromInteger(4)) >= (field_CC_sprite_scale * FP_FromInteger(60)) ||
            field_BC_ypos - FP_FromInteger(4) - midY >= (field_CC_sprite_scale * FP_FromInteger(60)) ||
            sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted) ||
            sActiveHero_5C1B68->CantBeDamaged_44BAB0() ||
            ZapIsNotBlocked_447240(this, sActiveHero_5C1B68))
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
    } break;

    case GreeterStates::eState_5_Knockback:
        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(40), FP_FromRaw(3 * field_C4_velx.fpValue))) // TODO: OG bug, why * 3 and not * FP 3??
        {
            field_C4_velx = FP_FromInteger(0);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            RandomishSpeak_447A70(GreeterSpeak::What_9);
            field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(160, 200);
        }
        break;

    case GreeterStates::eState_6_ToChase:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            RandomishSpeak_447A70(GreeterSpeak::Hi_0);
            field_13C_state = GreeterStates::eState_3_ChaseSpeak;
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

    case GreeterStates::eState_7_Fall:
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

            const CameraPos  soundDirection3 = gMap_5C3030.GetDirection_4811A0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                hitY);

            SFX_Play_46FC20(SoundEffect::GreeterLand_120, 0, soundDirection3, field_CC_sprite_scale);
            if (field_C8_vely > -FP_FromInteger(1))
            {
                field_C8_vely = FP_FromInteger(0);
                if (field_130 == 0)
                {
                    field_13C_state = GreeterStates::eState_0_Patrol;
                    field_20_animation.Set_Animation_Data_409C80(50028, nullptr);
                }
                else
                {
                    field_13C_state = GreeterStates::eState_4_Chase;
                    field_20_animation.Set_Animation_Data_409C80(50144, nullptr);
                }
            }
        }
    }  break;

    default:
        break;
    }

    if (FP_GetExponent(field_C4_velx) || FP_GetExponent(field_C8_vely))
    {
        if (field_13C_state != GreeterStates::eState_7_Fall)
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
    if (field_13C_state == GreeterStates::eState_7_Fall)
    {
        field_138_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
        HandleRollingAlong_447860();
        if (field_13C_state == GreeterStates::eState_7_Fall)
        {
            collisionCheck = false;
        }
    }

    if (collisionCheck)
    {
        if (Check_IsOnEndOfLine_408E90(0, 0))
        {
            field_13C_state = GreeterStates::eState_7_Fall;
            field_20_animation.Set_Animation_Data_409C80(50212, 0);
        }
    }

    if (field_13C_state != GreeterStates::eState_7_Fall)
    {
        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;
    }
}
