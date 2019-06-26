#include "stdafx.h"
#include "Throwable.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "Sfx.hpp"
#include "Events.hpp"

ALIVE_VAR(1, 0x5c1bde, WORD, gInfiniteGrenades_5C1BDE, 0);

void BaseThrowable::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_4114D0();
}

void BaseThrowable::VToDead_4114B0()
{
    vToDead_4114B0();
}

EXPORT void BaseThrowable::vToDead_4114B0()
{
    NOT_IMPLEMENTED();
}

EXPORT void BaseThrowable::vOnPickUpOrSlapped_4114D0()
{
    NOT_IMPLEMENTED();
}

// ====================================================================

void Rock::VScreenChanged()
{
    vScreenChanged_49F030();
}

int Rock::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_49F9A0(pSaveBuffer);
}

EXPORT void Rock::vScreenChanged_49F030()
{
    NOT_IMPLEMENTED();
}

EXPORT int Rock::vGetSaveState_49F9A0(BYTE* /*pSaveBuffer*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT BaseThrowable* CCSTD Make_Throwable_49AF30(FP /*xpos*/, FP /*ypos*/, FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

TintEntry stru_550EC0[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


BoneBag* BoneBag::ctor_4125C0(Path_BoneBag* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x5443B0);
    field_4_typeId = Types::eBoneBag_12;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 590);
    // TODO: Super super OWI hack, WTF?? Figure out exactly what this is patching in the animation
    *((WORD *)*ppRes + 4374) = 0;

    Animation_Init_424E10(8748, 64, 60, ppRes, 1, 1);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    SetTint_425600(&stru_550EC0[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_11C = 0;
    field_118_tlvInfo = tlvInfo;
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x)  / 2);
    field_DC_bApplyShadows &= ~1u;
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_124 =  pTlv->field_12_x_vel << 8;
    field_128 = -256 * pTlv->field_14_y_vel; // TODO: << 8 negated ??
    
    if (!pTlv->field_10_side)
    {
        field_124 = -field_124;
    }

    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_11E_count = pTlv->field_18_num_bones;
    field_120 = 1;
    field_122 = 1;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

void BoneBag::vScreenChanged_412BF0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

BoneBag* BoneBag::vdtor_4127C0(signed int flags)
{
    dtor_4127F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void BoneBag::dtor_4127F0()
{
    SetVTable(this, 0x5443B0);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

Grenade* Grenade::ctor_447F70(FP xpos, FP ypos, __int16 numGrenades, __int16 a5, __int16 a6, int a7)
{
    ctor_408240(0);
    SetVTable(this, 0x5456E0);

    field_11A = 0;

    Init_448110(xpos, ypos);

    field_118 = numGrenades;
    field_132 = a5;

    if (a5)
    {
        field_120_state = 8;
        field_11A = 1;
    }
    else if (numGrenades)
    {
        field_120_state = a5;
    }
    else
    {
        field_120_state = 3;
        field_122_explode_timer = 90;
    }

    field_138 = a7;
    field_130 = a6;

    return this;
}

BaseGameObject* Grenade::VDestructor(signed int flags)
{
    return vdtor_4480E0(flags);
}

void Grenade::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_449390();
}

void Grenade::VThrow_49E460(FP velX, FP velY)
{
    vThrow_4482E0(velX, velY);
}

BOOL Grenade::VCanThrow_49E350()
{
    return vCanThrow_49A5F0();
}

BOOL Grenade::VIsFalling_49E330()
{
    return vIsFalling_49A610();
}

void Grenade::VTimeToExplodeRandom_411490()
{
    vTimeToExplodeRandom_4480A0();
}

__int16 Grenade::VGetCount_448080()
{
    return vGetCount_448080();
}

void Grenade::Init_448110(FP xpos, FP ypos)
{
    field_4_typeId = Types::eGrenade_65;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 6005, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID);
    Animation_Init_424E10(1252, 17, 11, ppRes, 1, 1);
    field_6_flags.Clear(BaseGameObject::eInteractive);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_20_animation.field_B_render_mode = 0;

    field_11C_explosion_id = -1;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_128_xpos = xpos;
    field_12C_ypos = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_118 = 0;
    field_124 = 0;
    field_134 = 0;
}

void Grenade::vOnTrapDoorOpen_449390()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        if (field_120_state == 1 || field_120_state == 2)
        {
            field_120_state = 0;
        }
        else if (field_120_state != 6)
        {
            field_120_state = 4;
        }
    }
}

void Grenade::vThrow_4482E0(FP velX, FP velY)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (field_132 == 0)
    {
        if (!field_118)
        {
            field_120_state = 4;
        }
    }
    else
    {
        field_120_state = 9;
    }
}

BOOL Grenade::vCanThrow_49A5F0()
{
    NOT_IMPLEMENTED();
    return 0;
}

BOOL Grenade::vIsFalling_49A610()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Grenade::vTimeToExplodeRandom_4480A0()
{
    field_122_explode_timer -= Math_NextRandom() % 16;
}

__int16 Grenade::vGetCount_448080()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Grenade::BlowUp_4483C0(__int16 bSmallExplosion)
{
    auto pExplosion = alive_new<Explosion>();
    if (pExplosion)
    {
        pExplosion->ctor_4A1200(
            field_B8_xpos,
            field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
            field_CC_sprite_scale,
            bSmallExplosion);
        field_11C_explosion_id = pExplosion->field_8_object_id;
    }
   
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_120_state = 6;
    
    auto pGibs = alive_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(5, field_B8_xpos, field_BC_ypos, FP_FromInteger(0) , FP_FromInteger(5), field_CC_sprite_scale,  bSmallExplosion);
    }
}

void Grenade::dtor_448220()
{
    SetVTable(this, 0x5456E0);

    if (!gInfiniteGrenades_5C1BDE && !this->field_11A)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove_49AA00(field_118 >= 1 ? field_118 : 1);
        }
    }

    dtor_4080B0();
}

Grenade* Grenade::vdtor_4480E0(signed int flags)
{
    dtor_448220();

    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

signed __int16 Grenade::TimeToBlowUp_448350()
{
    if (!(--field_122_explode_timer % 16))
    {
        SFX_Play_46FA90(2, 0);
    }

    if (field_122_explode_timer != 0)
    {
        return 0;
    }

    BlowUp_4483C0(0);
    return 1;
}

void Grenade::vUpdate_4489C0()
{
    NOT_IMPLEMENTED();

    auto pExplosion = sObjectIds_5C1B70.Find_449CF0(field_11C_explosion_id);

//    auto v3 = field_100_pCollisionLine;
    auto pCurLine = &field_100_pCollisionLine;
    auto v5 = (int)pExplosion;
    auto v44 = pExplosion;
//    auto v42 = v3;
    
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            *pCurLine = 0;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(10),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(10),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
            v5 = (int)v44;
        }
        field_104_collision_line_type = 0;
    }

    switch (field_120_state)
    {
    case 0:
        if (!InTheAir_4484F0(0))
        {
            field_E4 = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_EC = field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_F0_prev_base = field_BC_ypos;
            field_6_flags.Set(BaseGameObject::eInteractive);
            field_E8 = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_120_state = 1;
        }
        break;

    case 1:
        /*
        v15 = field_C4_velx;
        v16 = field_C4_velx;

        if (v15 < 0)
        {
            v16 = -v16;
        }

        if (v16 >= 0x10000)
        {
            v29 = v15 == 0;
            v30 = v15 < 0;
            v31 = field_C4_velx;
            if (v30 || v29)
            {
                v32 = v31 + 655;
            }
            else
            {
                v32 = v31 - 655;
            }
            field_C4_velx = v32;
            pLine = PathLine::MoveOnLine_418260(
                *pCurLine,
                &field_B8_xpos,
                &field_BC_ypos,
                v32);
            newLineId = field_110_id;
            *pCurLine = pLine;
            if (newLineId != -1 && v42 != pLine)
            {
                VOnTrapDoorOpen();
            }
            if (!*pCurLine)
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                field_120_state = 0;
            }
        }
        else
        {
            v17 = &field_B8_xpos;
            snappedX = SnapToXGrid_449930(field_CC_sprite_scale, field_B8_xpos / 0x10000);
            curX = field_B8_xpos / 0x10000;
            if (snappedX - curX >= 0)
            {
                deltaX = snappedX - curX;
            }
            else
            {
                deltaX = curX - snappedX;
            }
            if (deltaX > 1)
            {
                v27 = PathLine::MoveOnLine_418260(
                    *pCurLine,
                    &field_B8_xpos,
                    &field_BC_ypos,
                    field_C4_velx);
                v28 = field_110_id;
                *pCurLine = v27;
                if (v28 != -1 && v42 != v27)
                {
                    VOnTrapDoorOpen();
                }
                if (!*pCurLine)
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                    field_120_state = 4;
                }
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
                v21 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                v22 = Math_FixedPoint_Divide_496B70(v21, 0x20000);
                v23 = field_CC_sprite_scale;
                field_E4 = *v17 - v22;
                v24 = ScaleToGridSize_4498B0(v23);
                field_EC = *v17 + Math_FixedPoint_Divide_496B70(v24, 0x20000);
                v25 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
                v26 = field_BC_ypos;
                field_F0_prev_base = v26;
                LOBYTE(field_6_flags) |= 0x80u;
                field_E8 = v26 - v25;
                field_120_state = 2;
            }
        }
        */
        break;

    case 3:
        TimeToBlowUp_448350();
        break;

    case 4:
        if (InTheAir_4484F0(0))
        {
            if (!TimeToBlowUp_448350())
            {
                /*
                v35 = (int *)field_0_VTbl->VBaseAliveGameObject.field_1C_vGetBoundingRect_424FD0(
                    this,
                    &Y1,
                    1);
                v44 = (BaseGameObject *)*v35;
                v36 = field_0_VTbl;
                v45 = v35[1];
                HIWORD(v44) += 5;
                HIWORD(v45) += 5;
                ((void(*)(Grenade *, BaseGameObject *, int, DynamicArray *, signed int, signed __int16(*)(Grenade *, int)))v36->VBaseAliveGameObject.field_18_vOnCollisionWith_424EE0)(
                    this,
                    v44,
                    v45,
                    gBaseGameObject_list_BB47C4,
                    1,
                    Grenade::OnCollision2_401690);
                */
            }
        }
        else
        {
            field_120_state = 5;
        }
        break;

    case 5:
        /*
        v37 = field_C4_velx >> 1;
        v38 = *pCurLine;
        field_C4_velx = v37;
        v39 = PathLine::MoveOnLine_418260(
            v38,
            &field_B8_xpos,
            &field_BC_ypos,
            v37);
        *pCurLine = v39;
        if (!v39)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
            field_120_state = 4;
        }
        TimeToBlowUp_448350();
        */
        break;

    case 6:
        /*
        if (!v5 || *(_BYTE *)(v5 + 6) & 4)
        {
            field_120_state = 7;
            field_11C_explosion_id = -1;
        }*/
        break;

    case 7:
        field_6_flags.Set(BaseGameObject::eDead);
        break;

    case 9:
        /*
        sub_4484F0(1);
        v40 = (int *)field_0_VTbl->VBaseAliveGameObject.field_1C_vGetBoundingRect_424FD0(
            this,
            (PSX_RECT *)&v46,
            1);
        v44 = (BaseGameObject *)*v40;
        thisVTable = field_0_VTbl;
        v45 = v40[1];
        HIWORD(v44) += 5;
        HIWORD(v45) += 5;
        ((void(*)(Grenade *, BaseGameObject *, int, DynamicArray *, signed int, signed __int16(*)(Grenade *, int)))thisVTable->VTableGrenade.BaseAnimatedWithPhysicsGameObject__OnCollisionWith_424EE0)(
            this,
            v44,
            v45,
            gBaseGameObject_list_BB47C4,
            1,
            j_Grenade::OnCollision_402AD1);
        */

        if (field_134)
        {
            BlowUp_4483C0(1);
        }
        break;

    default:
        return;
    }
}

__int16 Grenade::InTheAir_4484F0(__int16 a1)
{
    NOT_IMPLEMENTED();

    sObjectIds_5C1B70.Find_449CF0(field_110_id);

    field_128_xpos = field_B8_xpos;
    field_12C_ypos = field_BC_ypos;

    field_C8_vely += FP_FromInteger(1);
    field_B8_xpos += field_C4_velx;
    field_BC_ypos = field_C8_vely + field_BC_ypos;

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
        nullptr,
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos);

    if (field_FC_pPathTLV)
    {
        while (1)
        {
            if (field_FC_pPathTLV->field_4_type == TlvTypes::DeathDrop_4)
            {
                field_6_flags.Set(BaseGameObject::eDead);
                return 1;
            }

            field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                field_FC_pPathTLV,
                field_B8_xpos,
                field_BC_ypos,
                field_B8_xpos,
                field_BC_ypos);

            if (!field_FC_pPathTLV)
            {
                break;
            }
        }
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_128_xpos,
        field_12C_ypos,
        field_B8_xpos,
        field_BC_ypos,
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        if (field_C8_vely <= FP_FromInteger(0))
        {
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
            short v14 = 75 - 20 * field_124;
            if (v14 < 40)
            {
                v14 = 40;
            }
            SFX_Play_46FA90(68u, v14);
        }
        else
        {
            if (field_C8_vely < FP_FromInteger(1))
            {
                if (field_110_id == -1)
                {
                    AddToPlatform_449210();
                }
                return 0;
            }

            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
            field_C4_velx = (field_C4_velx / FP_FromInteger(2));

            if (a1)
            {
                field_134 = 1;
                return 1;
            }

            if (field_124 > 4)
            {
                goto LABEL_21;
            }

            short v12 = 75 - 20 * field_124;
            if (v12 < 40)
            {
                v12 = 40;
            }
            SFX_Play_46FA90(68u, v12);
            ++field_124;
        }
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
        Event_Broadcast_422BC0(kEventSpeaking, this);
    }

LABEL_21:
    auto v15 = -(field_D6_scale != 0);
    v15 = v15 & 0xA6; // TODO LOBYTE

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_128_xpos,
        field_12C_ypos,
        field_B8_xpos,
        field_BC_ypos,
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        v15 + 0x60) == 1)
    {
        switch (field_100_pCollisionLine->field_8_type)
        {
        case 1u:
        case 5u:
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_BC_ypos = hitY;
                field_B8_xpos = hitX;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                short vol = 75 - 20 * field_124;
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(68u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
            break;

        case 2u:
        case 6u:
            if (field_C4_velx > FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                short vol = 75 - 20 * field_124;
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(68u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
            break;

        default:
            return 1;
        }
    }

    return 1;
}

void Grenade::AddToPlatform_449210()
{
    NOT_IMPLEMENTED();
}

Bone* Bone::ctor_4112C0(FP xpos, FP ypos, __int16 countId)
{
    ctor_408240(0);
    field_11A = 0;
    SetVTable(this, 0x54431C);
    field_4_typeId = Types::eBone_11;
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 591, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 591);
    Animation_Init_424E10(456, 15, 9, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_120 = xpos;
    field_124 = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_6_flags.Clear(BaseGameObject::eInteractive);
    field_130 &= ~1u;

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_12C = sGnFrame_5C1B84 + 300;
    field_118_count_id = countId;
    field_11C_state = 0;
    field_11E = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

Bone* Bone::vdtor_411580(signed int flags)
{
    dtor_4115B0();

    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Bone::dtor_4115B0()
{
    SetVTable(this, 0x54431C);

    if (!gInfiniteGrenades_5C1BDE && !field_11A)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove_49AA00(field_118_count_id >= 1 ? field_118_count_id : 1);
        }
    }

    dtor_4080B0();
}

Meat* Meat::ctor_4694A0(FP xpos, FP ypos, __int16 a4)
{
    ctor_408240(0);
    SetVTable(this, 0x546040);

    field_11A = 0;
    field_4_typeId = Types::eMeat_84;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 2023, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 2023);
    Animation_Init_424E10(488, 17, 9, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_128 = 0;
    field_6_flags.Clear(BaseGameObject::eInteractive);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_12C = sGnFrame_5C1B84 + 600;
    field_130_pLine = 0;
    field_118 = a4;
    field_11C = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}
