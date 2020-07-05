#include "stdafx_ao.h"
#include "Function.hpp"
#include "RollingBall.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "RollingBallShaker.hpp"
#include "Shadow.hpp"
#include "SwitchStates.hpp"

START_NS_AO

void RollingBall::VUpdate()
{
    VUpdate_457AF0();
}

RollingBall* RollingBall::Vdtor_458490(signed int flags)
{
    dtor_458230();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* RollingBall::VDestructor(signed int flags)
{
    return Vdtor_458490(flags);
}

BaseGameObject* RollingBall::dtor_458230()
{
    SetVTable(this, 0x4BC180);
    if (field_112_state)
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }

    if (field_120_pCollisionLine)
    {
        Rect_Clear_40C920(&field_120_pCollisionLine->field_0_rect);
    }

    if (field_114_pRollingBallShaker)
    {
        field_114_pRollingBallShaker->field_32_bKillMe = TRUE;
        field_114_pRollingBallShaker->field_C_refCount--;
        field_114_pRollingBallShaker = nullptr;
    }

    BYTE** pRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 0, 0);
    ResourceManager::FreeResource_455550(pRes);
    return dtor_401000();
}

RollingBall* RollingBall::ctor_4578C0(Path_RollingBall* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BC180);
    field_4_typeId = Types::eRollingBall_72;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2002, 1, 0);
    Animation_Init_417FD0(
        15596,
        131,
        64,
        ppRes,
        1);

    field_10_anim.field_C_layer = 31;

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 12;
        field_C6_scale = 0;
    }

    if (!pTlv->field_1A_roll_direction)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_110_release_switch_id = pTlv->field_1C_release;
    field_118_speed = FP_FromRaw(pTlv->field_1E_speed << 8);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_118_speed = -FP_FromRaw(pTlv->field_1E_speed << 8);
    }

    field_11C = FP_FromRaw(pTlv->field_20_acceleration << 8);

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    FP castX = {};
    FP castY = {};

    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_AC_ypos + FP_FromInteger(24),
        &field_F4_pLine,
        &castX,
        &castY,
        // yeah this should be simplified, WTF! 
        field_BC_sprite_scale - FP_FromDouble(0.5) != FP_FromInteger(0) ? 1 : 0x10) == 1)
    {
        field_AC_ypos = castY;
    }

    MapFollowMe_401D30(TRUE);
    field_10C_tlvInfo = tlvInfo;
    field_112_state = 0;
    field_114_pRollingBallShaker = nullptr;
    field_120_pCollisionLine = nullptr;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    // Looks strange, it just bumps the res ref count
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 1, 0);

    if (!SwitchStates_Get(field_110_release_switch_id))
    {
        return this;
    }

    if (gMap_507BA8.field_0_current_level == LevelIds::eForestTemple_4 && gMap_507BA8.field_2_current_path == 2)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_A8_xpos = FP_FromInteger(2522);
        field_AC_ypos = FP_FromInteger(1300);
        field_10_anim.field_C_layer = 35;
        field_112_state = 4;
    }

    return this;
}

void RollingBall::VUpdate_457AF0()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

