#include "stdafx_ao.h"
#include "Function.hpp"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Game.hpp"

START_NS_AO

struct ParticleBurst_Item
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_z;
    FP field_C_x_speed;
    FP field_10_y_speed;
    FP field_14_z_speed;
    AnimationUnknown field_18_anim;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst_Item, 0x88);

static inline FP Random_Speed(FP scale)
{
    return FP_FromRaw((Math_NextRandom() - 128) << 13) * scale;
}

ParticleBurst* ParticleBurst::ctor_40D0F0(FP xpos, FP ypos, __int16 particleCount, FP scale, BurstType type)
{
    ctor_417C10();
    SetVTable(this, 0x4BA480);
    field_4_typeId = Types::eParticleBurst_19;
    field_BC_sprite_scale = scale;

    field_E4_ppRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::ResourceType::Resource_3DGibs, 0, sizeof(ParticleBurst_Item) * particleCount);
    if (field_E4_ppRes)
    {
        field_E8_pRes = reinterpret_cast<ParticleBurst_Item*>(*field_E4_ppRes);
        for (int i = 0; i < particleCount; i++)
        {
            // Placement new each element
            new (&field_E8_pRes[i]) ParticleBurst_Item();
            SetVTable(&field_E8_pRes[i].field_18_anim, 0x4BA470);
        }

        field_F4_type = type;
        switch (type)
        {
        case BurstType::eType_0:
            Animation_Init_417FD0(8108, 71, 36, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 1, 0), 1);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            break;

        case BurstType::eType_1:
            Animation_Init_417FD0(2800, 47, 29, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 358, 1, 0), 1);
            scale = FP_FromDouble(0.4) * scale;
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            break;

        case BurstType::eType_2:
            Animation_Init_417FD0(6284, 70, 43, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 349, 1, 0), 1);
            field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            field_10_anim.field_B_render_mode = 1;
            break;

        case BurstType::eType_3:
            Animation_Init_417FD0(6284, 70, 43, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 349, 1, 0), 1);

            field_10_anim.field_B_render_mode = 1;
            field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

            field_10_anim.field_8_r = 254;
            field_10_anim.field_9_g = 148;
            field_10_anim.field_A_b = 18;
            break;

        case BurstType::eType_4:
            Animation_Init_417FD0(8908, 69, 30, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6014, 1, 0), 1);
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            break;

        default:
            break;

        }

        if (field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        else
        {
            if (field_BC_sprite_scale == FP_FromInteger(1))
            {
                field_C6_scale = 1;
                field_10_anim.field_C_layer = 39;
            }
            else
            {
                field_C6_scale = 0;
                field_10_anim.field_C_layer = 20;
            }

            field_EC_count = particleCount;
            field_F0_timer = gnFrameCount_507670 + 91;
            field_A8_xpos = xpos;
            field_AC_ypos = ypos;

            for (int i = 0; i < particleCount; i++)
            {
                field_E8_pRes[i].field_18_anim.field_68_anim_ptr = &field_10_anim;
                field_E8_pRes[i].field_18_anim.field_C_layer = field_10_anim.field_C_layer;
                field_E8_pRes[i].field_18_anim.field_6C_scale = FP_FromDouble(0.95) * field_BC_sprite_scale;

                field_E8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

                field_E8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans, field_10_anim.field_4_flags.Get(AnimFlags::eBit15_bSemiTrans));

                field_E8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending, field_10_anim.field_4_flags.Get(AnimFlags::eBit16_bBlending));

                if (type == BurstType::eType_2)
                {
                    if (i % 2)
                    {
                        field_E8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
                    }
                }

                field_E8_pRes[i].field_18_anim.field_8_r = field_10_anim.field_8_r;
                field_E8_pRes[i].field_18_anim.field_9_g = field_10_anim.field_9_g;
                field_E8_pRes[i].field_18_anim.field_A_b = field_10_anim.field_A_b;

                field_E8_pRes[i].field_0_x = xpos;
                field_E8_pRes[i].field_4_y = ypos;
                field_E8_pRes[i].field_8_z = FP_FromInteger(0);

                field_E8_pRes[i].field_C_x_speed = Random_Speed(scale);
                field_E8_pRes[i].field_10_y_speed = -Random_Speed(scale);
                // OG bug sign could be wrong here as it called random again to Abs() it!
                field_E8_pRes[i].field_14_z_speed = -FP_Abs(Random_Speed(scale));
            }

            if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
            {
                field_C4_b = 60;
                field_C2_g = 60;
                field_C0_r = 60;
            }
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;
}

BaseGameObject* ParticleBurst::dtor_40D5A0()
{
    SetVTable(this, 0x4BA480);
    if (field_E4_ppRes)
    {
        ResourceManager::FreeResource_455550(field_E4_ppRes);
    }
    return dtor_417D10();
}

BaseGameObject* ParticleBurst::VDestructor(signed int flags)
{
    return Vdtor_40DA40(flags);
}

ParticleBurst* ParticleBurst::Vdtor_40DA40(signed int flags)
{
    dtor_40D5A0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
