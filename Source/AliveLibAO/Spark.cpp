#include "stdafx_ao.h"
#include "Function.hpp"
#include "Spark.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "Animation.hpp"
#include "Particle.hpp"

START_NS_AO

Spark* Spark::ctor_477B70(FP xpos, FP ypos, FP scale, unsigned __int8 count, __int16 min, __int16 max)
{
    ctor_487E10(1);
    field_6_flags.Set(Options::eDrawable_Bit4);

    SetVTable(this, 0x4BCD48);
    field_4_typeId = Types::eNone_0;
    
    gObjList_drawables_504618->Push_Back(this);

    field_30_xpos = xpos;
    field_34_ypos = ypos;
    field_38_scale = scale;

    if (scale == FP_FromDouble(0.5))
    {
        field_42_layer = 17;
    }
    else
    {
        field_42_layer = 36;
    }

    field_40_b = 127;
    field_3C_r = 31;
    field_3E_g = 31;

    field_4C_count = count;

    field_44_ppSprxRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_Sprx, 0, sizeof(SparkRes) * count);
    if (field_44_ppSprxRes)
    {
        field_48_pRes = reinterpret_cast<SparkRes*>(*field_44_ppSprxRes);
        for (int idx = 0; idx < field_4C_count; idx++)
        {
            SparkRes* pSparkIter = &field_48_pRes[idx];
            int randAng = 0;
            if (min >= 0)
            {
                randAng = Math_RandomRange_450F20(min, max);
            }
            else
            {
                randAng = min + Math_RandomRange_450F20(0, max - min);
            }
            pSparkIter->field_10_ang = static_cast<BYTE>(randAng);
            pSparkIter->field_14_radius = FP_FromInteger(0);
            pSparkIter->field_18_len = FP_FromInteger(Math_RandomRange_450F20(2, 4));
        }

        field_50_timer = gnFrameCount_507670 + 3;

        BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 312, 1, 0);
        auto pParticle = ao_new<Particle>();
        if (pParticle)
        {
            pParticle->ctor_478880(xpos, ypos - FP_FromInteger(4), 1532, 38, 21, ppRes);

            pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);

            pParticle->field_10_anim.field_B_render_mode = 1;
            pParticle->field_10_anim.field_8_r = 128;
            pParticle->field_10_anim.field_9_g = 128;
            pParticle->field_10_anim.field_A_b = 128;

            if (scale == FP_FromInteger(1))
            {
                pParticle->field_10_anim.field_C_layer = 36;
            }
            else
            {
                pParticle->field_10_anim.field_C_layer = 17;
            }

            pParticle->field_BC_sprite_scale = scale;
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;
}

END_NS_AO
