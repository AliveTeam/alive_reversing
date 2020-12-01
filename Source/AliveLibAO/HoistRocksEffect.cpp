#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoistRocksEffect.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "Collisions.hpp"

namespace AO {

void HoistParticle::VUpdate()
{
    VUpdate_431BD0();
}

BaseGameObject* HoistParticle::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

HoistParticle* HoistParticle::ctor_431B00(FP xpos, FP ypos, FP scale, int frameTableOffset)
{
    ctor_417C10();
    SetVTable(this, 0x4BB288);
    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kDrpRockResID, 1, 0);
    int maxH = 7;
    if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
    {
        maxH = 5;
    }
    Animation_Init_417FD0(frameTableOffset, maxH, 4, ppRes, 1);

    field_BC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 25;
    }
    else
    {
        field_10_anim.field_C_layer = 6;
    }

    field_E4_bHitGround = 0;
    return this;
}

void HoistParticle::VUpdate_431BD0()
{
    if (field_B8_vely >= (field_BC_sprite_scale * FP_FromInteger(10)))
    {
        if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
    else
    {
        field_B8_vely += (field_BC_sprite_scale * FP_FromDouble(0.6));
    }

    const FP oldY = field_AC_ypos;
    field_AC_ypos += field_B8_vely;

    if (field_E4_bHitGround == 0)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_504C6C->RayCast_40C410(
            field_A8_xpos,
            oldY,
            field_A8_xpos,
            field_AC_ypos,
            &pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
        {
            field_AC_ypos = hitY;
            field_B8_vely = (field_B8_vely * FP_FromDouble(-0.3));
            field_E4_bHitGround = 1;
        }
    }
}

void HoistRocksEffect::VUpdate()
{
    VUpdate_431860();
}

void HoistRocksEffect::VScreenChanged_431AF0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void HoistRocksEffect::VScreenChanged()
{
    VScreenChanged_431AF0();
}

HoistRocksEffect* HoistRocksEffect::vdtor_431CF0(signed int flags)
{
    dtor_431A90();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* HoistRocksEffect::VDestructor(signed int flags)
{
    return vdtor_431CF0(flags);
}

BaseGameObject* HoistRocksEffect::dtor_431A90()
{
    SetVTable(this, 0x4BB270);
    gMap_507BA8.TLV_Reset_446870(field_18_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

HoistRocksEffect* HoistRocksEffect::ctor_431820(Path_Hoist* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB270);
    field_18_tlvInfo = tlvInfo;
    field_10_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);
    field_14_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    return this;
}

void HoistRocksEffect::VUpdate_431860()
{
    const auto rnd = Math_RandomRange_450F20(1, 4) - 2;
    if (rnd)
    {
        if (rnd == 1)
        {
            auto pHoistParticle = ao_new<HoistParticle>();
            if (pHoistParticle)
            {
                int frameTableOffset = 176;
                if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
                {
                    frameTableOffset = 260;
                }
                pHoistParticle->ctor_431B00(
                    field_10_xpos + FP_FromInteger(Math_RandomRange_450F20(-8, 8)),
                    field_14_ypos + FP_FromInteger(Math_RandomRange_450F20(-4, 4)),
                    FP_FromInteger(1),
                    frameTableOffset);
            }
            field_8_update_delay = Math_RandomRange_450F20(30, 50);
        }
        else
        {
            auto pHoistParticle = ao_new<HoistParticle>();
            if (pHoistParticle)
            {
                int frameTableOffset = 188;
                if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
                {
                    frameTableOffset = 276;
                }
                pHoistParticle->ctor_431B00(
                    field_10_xpos + FP_FromInteger(Math_RandomRange_450F20(-8, 8)),
                    field_14_ypos + FP_FromInteger(Math_RandomRange_450F20(-4, 4)),
                    FP_FromInteger(1),
                    frameTableOffset);
            }
            field_8_update_delay = Math_RandomRange_450F20(5, 10);
        }
    }
    else
    {
        auto pHoistParticle = ao_new<HoistParticle>();
        if (pHoistParticle)
        {
            int frameTableOffset = 164;
            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                frameTableOffset = 240;
            }
            pHoistParticle->ctor_431B00(
                field_10_xpos + FP_FromInteger(Math_RandomRange_450F20(-8, 8)),
                field_14_ypos + FP_FromInteger(Math_RandomRange_450F20(-4, 4)),
                FP_FromInteger(1),
                frameTableOffset);
        }
        field_8_update_delay = Math_RandomRange_450F20(10, 20);
    }
}

}
