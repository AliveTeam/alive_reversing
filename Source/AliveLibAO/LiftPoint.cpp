#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "ResourceManager.hpp"
#include "Rope.hpp"
#include "Game.hpp"

START_NS_AO

struct LiftPointCoord
{
    int field_0;
    int field_4;
};

struct LiftPointData
{
    int field_0_maxW_Platform;
    int field_4_maxH_platform;
    int field_8_platform_frame_table_offset;
    int field_C_lift_wheel_frame_table_offset;
    int field_10_pulley_frame_table_offset;
    int field_14_maxW_lift_wheel_and_pulley;
    int field_18_maxW_lift_wheel_and_pulley;
};

const LiftPointData stru_4BB480[16] =
{
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1260, 117, 19, 24204, 24240, 124, 37 },
    { 1524, 114, 25, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1524, 114, 25, 17108, 17144, 62, 46 },
    { 1472, 116, 20, 21900, 21936, 73, 55 },
    { 1472, 116, 20, 21900, 21936, 73, 55 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1260, 117, 19, 24204, 24240, 124, 37 },
    { 1260, 117, 19, 24204, 24240, 124, 37 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1472, 116, 20, 21900, 21936, 73, 55 }
};

const LiftPointCoord stru_4BB640[16] =
{
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 2, 1 },
    { 2, 1 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 2, 1 }
};



LiftPoint* LiftPoint::ctor_434710(Path_LiftPoint* pTlv, Map* pPath, int tlvInfo)
{
    ctor_401090();

    SetVTable(&field_13C_lift_wheel, 0x4BA2B8);
    SetVTable(&field_1D4_pulley_anim, 0x4BA2B8);
    SetVTable(this, 0x4BB6C0);
    field_4_typeId = Types::eLiftPoint_51;

    pTlv->field_1_unknown = 3;

    BYTE** ppLiftRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1032, 1, 0);
    if (pTlv->field_20_scale)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    const int idx = static_cast<int>(gMap_507BA8.field_0_current_level);
    AddDynamicCollision_4512C0(
        stru_4BB480[idx].field_0_maxW_Platform,
        stru_4BB480[idx].field_4_maxH_platform,
        stru_4BB480[idx].field_8_platform_frame_table_offset,
        ppLiftRes,
        pTlv,
        pPath,
        tlvInfo);

    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 25;
    }
    else
    {
        field_10_anim.field_C_layer = 6;
        field_120_pCollisionLine->field_8_type = 36;
    }
    
    const FP oldX = field_A8_xpos;
    MapFollowMe_401D30(1);
    const FP newX = field_A8_xpos;
    
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    const auto xMovedBy = FP_GetExponent(newX - oldX);
    field_118 -= xMovedBy;
    field_11A -= xMovedBy;

    BYTE** ppLiftWheelRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1001, 1, 0);
    if (field_13C_lift_wheel.Init_402D20(
        stru_4BB480[idx].field_C_lift_wheel_frame_table_offset,
        gObjList_animations_505564,
        this,
        static_cast<short>(stru_4BB480[idx].field_14_maxW_lift_wheel_and_pulley),
        static_cast<short>(stru_4BB480[idx].field_18_maxW_lift_wheel_and_pulley),
        ppLiftWheelRes,
        1,
        0,
        0))
    {
        if (pTlv->field_20_scale)
        {
            field_13C_lift_wheel.field_C_layer = 6;
        }
        else
        {
            field_13C_lift_wheel.field_C_layer = 25;
        }

        field_13C_lift_wheel.field_14_scale = field_BC_sprite_scale;

        field_27A_flags.Clear(Flags::eBit6_bMoveToFloorLevel);
        field_27A_flags.Clear(Flags::eBit4_bBottomFloor);
        field_27A_flags.Clear(Flags::eBit3_bMiddleFloor);
        field_27A_flags.Clear(Flags::eBit2_bTopFloor);
 
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit2_Animate);

        field_12C_bMoving &= ~1u;
        field_274_ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 53, 1, 0);
        field_B4_velx = FP_FromInteger(0);
        field_13C_lift_wheel.field_8_r = 128;
        field_13C_lift_wheel.field_9_g = 128;
        field_13C_lift_wheel.field_A_b = 128;
        field_13C_lift_wheel.field_B_render_mode = 0;
        field_10C = 1;

        field_B8_vely = FP_FromInteger(0);
        field_130_stop_type = 4;

        auto pRopeMem = ao_new<Rope>();
        if (pRopeMem)
        {
            pRopeMem->ctor_458520(
                FP_GetExponent(field_A8_xpos + (FP_FromInteger(13) * field_BC_sprite_scale) + FP_FromInteger(stru_4BB640[idx].field_4)),
                0,
                FP_GetExponent(field_AC_ypos + (FP_FromInteger(25) * field_BC_sprite_scale)),
                field_BC_sprite_scale);
        }

        field_138_pRope1 = pRopeMem;

        auto pRopeMem2 = ao_new<Rope>();
        if (pRopeMem2)
        {
            pRopeMem2->ctor_458520(
                FP_GetExponent(field_A8_xpos + (FP_FromInteger(-10) * field_BC_sprite_scale) + FP_FromInteger(stru_4BB640[idx].field_0)),
                0,
                FP_GetExponent(field_AC_ypos + (FP_FromInteger(25) * field_BC_sprite_scale)),
                field_BC_sprite_scale);
            pRopeMem2->field_C_refCount++;
            field_134_pRope2 = pRopeMem2;

        }

        field_134_pRope2->field_F2_bottom = FP_GetExponent((FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_120_pCollisionLine->field_0_rect.y));
        field_138_pRope1->field_F2_bottom = FP_GetExponent((FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_120_pCollisionLine->field_0_rect.y));

        const FP v29 = FP_FromRaw(FP_GetExponent((field_AC_ypos * FP_FromDouble(1.5)) * field_BC_sprite_scale) % FP_FromInteger(field_134_pRope2->field_E6_rope_length).fpValue);
        field_134_pRope2->field_AC_ypos = FP_NoFractional(field_AC_ypos + v29 + (FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_134_pRope2->field_E6_rope_length));
        field_138_pRope1->field_AC_ypos = FP_NoFractional(field_AC_ypos + v29 - (FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_138_pRope1->field_E6_rope_length));

        field_27A_flags.Clear(Flags::eBit5);

        CreatePulleyIfExists_435AE0(0, 0);

        field_278_point_id = pTlv->field_18_id;
        field_130_stop_type = pTlv->field_1E_lift_point_stop_type;

        switch (field_130_stop_type)
        {
        case 0:
            field_27A_flags.Set(Flags::eBit2_bTopFloor);
            break;

        case 1:
            field_27A_flags.Set(Flags::eBit4_bBottomFloor);
            break;

        case 2:
            field_27A_flags.Set(Flags::eBit3_bMiddleFloor);
            break;
        }

        field_27A_flags.Clear(Flags::eBit8_KeepOnMiddleFloor);
        field_27A_flags.Set(Flags::eBit7);
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }
    return this;
}

void LiftPoint::Move_435740(FP xSpeed, FP ySpeed, int /*not_used*/)
{
    field_B4_velx = xSpeed;
    field_B8_vely = ySpeed;
    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C_bMoving |= 1u;
    }
}

bool LiftPoint::OnTopFloor() const
{
    return field_27A_flags.Get(Flags::eBit2_bTopFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnBottomFloor() const
{
    return field_27A_flags.Get(Flags::eBit4_bBottomFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnMiddleFloor() const
{
    return field_27A_flags.Get(Flags::eBit3_bMiddleFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnAnyFloor() const
{
    return OnBottomFloor() || OnTopFloor() || OnMiddleFloor();
}

void LiftPoint::VUpdate()
{
    VUpdate_434D10();
}

void LiftPoint::VUpdate_434D10()
{
    NOT_IMPLEMENTED();
}

void LiftPoint::VRender(int** pOrderingTable)
{
    VRender_435780(pOrderingTable);
}

void LiftPoint::VRender_435780(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

void LiftPoint::VScreenChanged()
{
    VScreenChanged_435CC0();
}

void LiftPoint::VScreenChanged_435CC0()
{
    if (!field_27A_flags.Get(Flags::eBit5))
    {
        CreatePulleyIfExists_435AE0(0, -1);
    }

    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

EXPORT BaseGameObject* AO::LiftPoint::dtor_4355E0()
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* LiftPoint::VDestructor(signed int flags)
{
    return Vdtor_435D10(flags);
}

void LiftPoint::CreatePulleyIfExists_435AE0(int /*camX*/, int /*camY*/)
{
    NOT_IMPLEMENTED();
}

LiftPoint* LiftPoint::Vdtor_435D10(signed int flags)
{
    dtor_4355E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
