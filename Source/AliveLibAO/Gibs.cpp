#include "stdafx_ao.h"
#include "Function.hpp"
#include "Gibs.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

START_NS_AO

const Gib_Data kGibData_4C30B0[6] =
{
    { 7208, 7248, 7288, 50, 25, 25 },
    { 5948, 6028, 5988, 44, 26, 423 },
    { 28376, 28416, 28416, 111, 63, 576 },
    { 7440, 7480, 7400, 72, 43, 217 },
    { 7208, 7248, 7288, 50, 25, 25 },
    { 3928, 3928, 3928, 37, 30, 365 }
};

TintEntry sAbeGibTints_4C6438[5] =
{
    { 5, 25u, 25u, 25u },
    { 6, 25u, 25u, 25u },
    { 8, 125u, 125u, 95u },
    { 9, 120u, 120u, 90u },
    { -1, 102u, 102u, 102u }
};

TintEntry sMudGibTints_4CD320[3] =
{
    { 5, 25u, 25u, 25u },
    { 6, 25u, 25u, 25u },
    { -1, 87u, 103u, 67u },
};


static FP GibRand(FP scale)
{
    return FP_FromRaw((Math_NextRandom() - 128) << 13) * scale;
}

Gibs* Gibs::ctor_407B20(int gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale)
{
    ctor_417C10();

    for (GibPart& part : field_F4_parts)
    {
        SetVTable(&part.field_18_anim, 0x4BA2B8);
    }
    SetVTable(this, 0x4BA280);

    field_E4_pGibData = &kGibData_4C30B0[gibType];
    BYTE** ppAnimData = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, field_E4_pGibData->field_14_resource_id, 1, 0);

    // The base class renders the head gib
    Animation_Init_417FD0(
        field_E4_pGibData->field_0_head,
        static_cast<short>(field_E4_pGibData->field_C_max_w),
        static_cast<short>(field_E4_pGibData->field_10_max_h),
        ppAnimData,
        1);

    field_BC_sprite_scale = scale;
    field_A8_xpos = xpos;
    field_AC_ypos = ypos + FP_FromInteger(2);
    
    field_F0_timer = gnFrameCount_507670 + 91;

    if (scale == FP_FromInteger(1))
    {
        field_E8_z = FP_FromInteger(0);
        field_10_anim.field_C_layer = 37;
        field_C6_scale = 1;
    }
    else if (scale == FP_FromDouble(0.5))
    {
        field_E8_z = FP_FromInteger(100);
        field_10_anim.field_C_layer = 17;
        field_C6_scale = 0;
    }
    else
    {
        // Not a valid scale
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_B4_velx = xOff + GibRand(scale);
    field_B8_vely = yOff + GibRand(scale);

    if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
    {
        field_EC_dz = -(GibRand(scale) / FP_FromInteger(2));
    }
    else
    {
        field_EC_dz = GibRand(scale) / FP_FromInteger(2);
    }

    BYTE** ppPal = nullptr;
    if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
    {
        // TODO: Enum for gib types, 0 = abe, 4 = mud
        if (gibType == 0 || gibType == 4)
        {
            ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 25, 0, 0);
        }
        else if (gibType == 2)
        {
            ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 576, 0, 0);
        }
    }

    if (gibType == 0)
    {
        SetTint_418750(sAbeGibTints_4C6438, gMap_507BA8.field_0_current_level);
      
    }
    else if (gibType == 4)
    {
        SetTint_418750(sMudGibTints_4CD320, gMap_507BA8.field_0_current_level);
    }

    field_5C4_parts_used_count = 7;

    GibPart* pPart = &field_F4_parts[0];
    for (short i = 0; i < field_5C4_parts_used_count; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            if (!pPart->field_18_anim.Init_402D20(
                field_E4_pGibData->field_4_arm,
                gObjList_animations_505564,
                this,
                static_cast<short>(field_E4_pGibData->field_C_max_w),
                static_cast<short>(field_E4_pGibData->field_10_max_h),
                ppAnimData,
                1,
                0,
                0))
            {
                field_5C4_parts_used_count = i;
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return this;
            }
        }
        else
        {
            // 2 body parts
            if (!pPart->field_18_anim.Init_402D20(
                field_E4_pGibData->field_8_body,
                gObjList_animations_505564,
                this,
                static_cast<short>(field_E4_pGibData->field_C_max_w),
                static_cast<short>(field_E4_pGibData->field_10_max_h),
                ppAnimData,
                1,
                0,
                0))
            {
                field_5C4_parts_used_count = i;
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return this;
            }
        }

        pPart->field_18_anim.field_C_layer = field_10_anim.field_C_layer;
        pPart->field_18_anim.field_14_scale = scale;

        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit17); // Else the gibs seem to kill meat grinders and other objects ??
        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

        pPart->field_18_anim.field_8_r = static_cast<BYTE>(field_C0_r);
        pPart->field_18_anim.field_9_g = static_cast<BYTE>(field_C2_g);
        pPart->field_18_anim.field_A_b = static_cast<BYTE>(field_C4_b);

        pPart->field_0_x = field_A8_xpos;
        pPart->field_4_y = field_AC_ypos;
        pPart->field_8_z = field_E8_z;

        pPart->field_C_dx = xOff + GibRand(scale);
        pPart->field_10_dy = yOff + GibRand(scale);

        if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
        {
            pPart->field_14_dz = -(GibRand(scale) / FP_FromInteger(2));
        }
        else
        {
            pPart->field_14_dz = GibRand(scale) / FP_FromInteger(2);
        }

        if (ppPal)
        {
            pPart->field_18_anim.LoadPal_403090(ppPal, 0);
        }

        pPart++;
    }

    return this;

}

void Gibs::VUpdate()
{
    VUpdate_4080C0();
}

void Gibs::VUpdate_4080C0()
{
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;
    field_E8_z += field_EC_dz;

    field_B8_vely += FP_FromDouble(0.25);

    if (field_E8_z + FP_FromInteger(100) < FP_FromInteger(15))
    {
        field_EC_dz = -field_EC_dz;
        field_E8_z += -field_EC_dz;
    }

    for (int i = 0; i < field_5C4_parts_used_count; i++)
    {
        field_F4_parts[i].field_0_x += field_F4_parts[i].field_C_dx;
        field_F4_parts[i].field_4_y += field_F4_parts[i].field_10_dy;
        field_F4_parts[i].field_8_z += field_F4_parts[i].field_14_dz;

        field_F4_parts[i].field_10_dy += FP_FromDouble(0.25);

        if (field_F4_parts[i].field_8_z + FP_FromInteger(100) < FP_FromInteger(15))
        {
            field_F4_parts[i].field_14_dz = -field_F4_parts[i].field_14_dz;
            field_F4_parts[i].field_8_z += -field_F4_parts[i].field_14_dz;
        }
    }

    if (static_cast<int>(gnFrameCount_507670) > field_F0_timer)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

BaseGameObject* Gibs::dtor_408040()
{
    SetVTable(this, 0x4BA280);

    for (short i = 0; i < field_5C4_parts_used_count; i++)
    {
        field_F4_parts[i].field_18_anim.vCleanUp();
    }
    return dtor_417D10();
}

BaseGameObject* Gibs::Vdtor_4083D0(unsigned int flags)
{
    dtor_408040();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Gibs::VDestructor(signed int flags)
{
    return Vdtor_4083D0(flags);
}

END_NS_AO
