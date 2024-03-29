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

BaseGameObject* HoistParticle::VDestructor(s32 flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

HoistParticle* HoistParticle::ctor_431B00(FP xpos, FP ypos, FP scale, s32 frameTableOffset)
{
    ctor_417C10();
    SetVTable(this, 0x4BB288);
    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kHoistRocksAOResID, 1, 0);
    s32 maxW = 7;
    if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
    {
        maxW = 5;
    }
    Animation_Init_417FD0(frameTableOffset, maxW, 4, ppRes, 1);

    field_BC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_25;
    }
    else
    {
        field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_Half_6;
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

HoistRocksEffect* HoistRocksEffect::vdtor_431CF0(s32 flags)
{
    dtor_431A90();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* HoistRocksEffect::VDestructor(s32 flags)
{
    return vdtor_431CF0(flags);
}

BaseGameObject* HoistRocksEffect::dtor_431A90()
{
    SetVTable(this, 0x4BB270);
    gMap_507BA8.TLV_Reset_446870(field_18_tlvInfo, -1, 0, 0);
    return dtor_487DF0();
}

HoistRocksEffect* HoistRocksEffect::ctor_431820(Path_Hoist* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BB270);
    field_18_tlvInfo = tlvInfo;
    field_10_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    field_14_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
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
                const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock2);
                s32 frameTableOffset = normalHoist.mFrameTableOffset;
                if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
                {
                    const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock2);
                    frameTableOffset = ruptureHoist.mFrameTableOffset;
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
                const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock3);
                s32 frameTableOffset = normalHoist.mFrameTableOffset;
                if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
                {
                    const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock3);
                    frameTableOffset = ruptureHoist.mFrameTableOffset;
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
            const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock1);
            s32 frameTableOffset = normalHoist.mFrameTableOffset;
            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock1);
                frameTableOffset = ruptureHoist.mFrameTableOffset;
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

} // namespace AO
