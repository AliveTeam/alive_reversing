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
#include "Function.hpp"

struct MotionDetector : public BaseAnimatedWithPhysicsGameObject
{
    EXPORT MotionDetector* ctor_4683B0(int /*pTlv*/, int /*tlvInfo*/, BaseAnimatedWithPhysicsGameObject* /*pOwner*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    int field_E4_not_used[4];
    int field_F4;
    int field_F8_laser_id;
    int field_FC_owner_id;
    __int16 field_100;
    __int16 field_102;
    __int16 field_104;
    __int16 field_106;
    __int16 field_108;
    __int16 field_10A;
    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    __int16 field_124;
    __int16 field_126;
    __int16 field_128;
    __int16 field_12A;
    __int16 field_12C;
    __int16 field_12E;
    __int16 field_130;
    __int16 field_132;
    __int16 field_134;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    __int16 field_14E;
    __int16 field_150;
    __int16 field_152;
    __int16 field_154;
    __int16 field_156;
    __int16 field_158;
    __int16 field_15A;
    __int16 field_15C;
    __int16 field_15E;
    __int16 field_160;
    __int16 field_162;
    __int16 field_164;
    __int16 field_166;
    __int16 field_168;
    __int16 field_16A;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    int field_174;
    __int16 field_178;
    __int16 field_17A;
};
ALIVE_ASSERT_SIZEOF(MotionDetector, 0x17C);

EXPORT Greeter* Greeter::ctor_4465B0(Path_Greeter* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x54566C);

    field_4_typeId = Types::eGreeter_64;
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kGreeterResID);
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


    field_6_flags.Set(BaseGameObject::eCanExplode);

    if (pTlv->field_14_direction == 0)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }

    field_134_speed = FP_FromInteger(pTlv->field_12_motion_detector_speed);
    field_13C_state = 0;
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

    auto pMotionDetctor = alive_new<MotionDetector>();
    if (pMotionDetctor)
    {
        pMotionDetctor->ctor_4683B0(0, 0, this);
        field_11C_motionDetectorId = pMotionDetctor->field_8_object_id;
    }

    field_140 = 0;
    field_13E = 0;
    
    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(70, 210);

    Add_Resource_4DC130(ResourceManager::Resource_Animation, 365);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 301);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 25);

    field_12C = 0;

    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_130 = 0;

    return this;
}

Greeter* Greeter::vdtor_4468B0(signed int flags)
{
    dtor_4468E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
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
            field_6_flags.Set(BaseGameObject::eDead);
            return;
        }

        const FP yDistFromPlayer = FP_Abs(sControlledCharacter_5C1B8C->field_BC_ypos  - field_BC_ypos);
        if (yDistFromPlayer > FP_FromInteger(240))
        {
            field_6_flags.Set(BaseGameObject::eDead);
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
        pMotionDetector->field_6_flags.Set(BaseGameObject::eDead);
    }
    dtor_4080B0();
}

EXPORT void Greeter::BlowUp_447E50()
{
    field_10C_health = FP_FromInteger(0);
    
    auto pExplosion = alive_new<Explosion>();
    if (pExplosion)
    {
        pExplosion->ctor_4A1200(
            field_B8_xpos,
            field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
            field_CC_sprite_scale,
            0);
    }

    auto pGibs = alive_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(
            5,
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(50),
            FP_FromInteger(0),
            FP_FromInteger(0),
            field_CC_sprite_scale,
            0);
    }

    field_6_flags.Set(BaseGameObject::eDead);
    field_12E = 0;
}

void Greeter::ChangeDirection_447BD0()
{
    field_13C_state = 1;
    field_C4_velx = FP_FromInteger(0);
    field_20_animation.Set_Animation_Data_409C80(50072, nullptr);
    field_124_last_turn_time = sGnFrame_5C1B84;
}

void Greeter::BounceBackFromShot_447B10()
{
    field_13C_state = 5;
   
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = FP_FromInteger(-2);
    }
    else
    {
        field_C4_velx = FP_FromInteger(2);
    }

    field_13E = 0;
    field_140 = 0;

    field_20_animation.Set_Animation_Data_409C80(50236, nullptr);

    const CameraPos soundDirection = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);
    SFX_Play_46FC20(121, 0, soundDirection, field_CC_sprite_scale);
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
            if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))  && field_13C_state == 0)
            {
                ChangeDirection_447BD0();
            }
            break;

        case ScrabRightBound_44:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && field_13C_state == 0)
            {
                ChangeDirection_447BD0();
            }
            break;

        case EnemyStopper_47:
            if (field_13C_state != 7)
            {
                ChangeDirection_447BD0();
            }
            break;

        default:
            continue;
        }
    }

    if (field_13C_state == 0)
    {
        if ((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) && Check_Collision_Unknown_408E90(0, 1)) ||
            Raycast_408750(field_CC_sprite_scale * FP_FromInteger(40), field_C4_velx * FP_FromInteger(3)) ||
            (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) && Check_Collision_Unknown_408E90(1, 1)))
        {
            ChangeDirection_447BD0();
        }
    }

    if (field_13C_state == 4)
    {
        if (Raycast_408750(field_CC_sprite_scale * FP_FromInteger(40), field_C4_velx * FP_FromInteger(3))) // TODO: OG bug, raw * 3 here ??
        {
            BounceBackFromShot_447B10();
        }
    }
}

EXPORT signed __int16 Greeter::vTakeDamage_447C20(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead) || FP_GetExponent(field_10C_health) == 0)
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eGrinder_30:
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
        LOG_WARNING("This might be wrong - greeter default damage case, can this only be a slig ??");
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
    }
}

void Greeter::vsub_447DB0(BaseGameObject* /*pFrom*/)
{
    BounceBackFromShot_447B10();
}

void Greeter::ZapTarget_447320(FP xpos, FP ypos, BaseAliveGameObject* pTarget)
{
    auto pScreenShake = alive_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4ACF70(0, 0);
    }

    auto pZapLine = alive_new<ZapLine>();
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

    auto pZapLine2 = alive_new<ZapLine>();
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

    auto pZapLine3 = alive_new<ZapLine>();
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

    auto pParticleBurst = alive_new<ParticleBurst>();
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

    auto pParticleBurst2 = alive_new<ParticleBurst>();
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

    auto pElectrocute = alive_new<Electrocute>();
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

    SFX_Play_46FC20(49, 0, soundDirection, field_CC_sprite_scale);
    SFX_Play_46FC20(50, 0, soundDirection, field_CC_sprite_scale);
    
    RandomishSpeak_447A70(3);

    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(160, 200);
    field_13E = 0;
    field_140 = 0;
}

void Greeter::RandomishSpeak_447A70(__int16 effect)
{
    field_13C_state = 2;
    field_C4_velx = FP_FromInteger(0);
    field_20_animation.Set_Animation_Data_409C80(50104, nullptr);
    field_120 = sGnFrame_5C1B84 + 25;

    if (effect == 1000)
    {
        Sfx_Slig_4C04F0(static_cast<int>(sGnFrame_5C1B84 % 4), 100, 700, this);
    }
    else
    {
        Sfx_Slig_4C04F0(effect, 100, 700, this);
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

        if (pObj->field_4_typeId == BaseGameObject::Types::eMudokon_110)
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
