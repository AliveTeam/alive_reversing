#include "stdafx.h"
#include "Spark.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "Events.hpp"

Spark* Spark::ctor_4CBBB0(FP xpos, FP ypos, FP scale, unsigned __int8 count, __int16 min, __int16 max, __int16 a8)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_6_flags.Set(BaseGameObject::eDrawable);

    SetVTable(this, 0x54783C); // vTbl_Spark_54783C
    field_4_typeId = BaseGameObject::Types::eNone_0;

    gObjList_drawables_5C1124->Push_Back(this);

    field_64_ctor_arg = a8;
    field_40_xpos = xpos;
    field_44_ypos = ypos;
    field_48_scale = scale;

    if (scale == FP_FromDouble(0.5))
    {
        field_52_layer = 17;
    }
    else
    {
        field_52_layer = 36;
    }

    field_50_b = 127;
    field_4C_r = 31;
    field_4E_g = 31;

    field_5C_count = count;

    field_54_ppSprxRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Sprx, 0, sizeof(SparkRes) * count);
    if (field_54_ppSprxRes)
    {
        field_58_pRes = reinterpret_cast<SparkRes*>(*field_54_ppSprxRes);
        for (int idx = 0; idx < field_5C_count; idx++)
        {
            SparkRes* pSparkIter = &field_58_pRes[idx];
            int randAng = 0;
            if (min >= 0)
            {
                randAng = Math_RandomRange_496AB0(min, max);
            }
            else
            {
                randAng = min + Math_RandomRange_496AB0(0, max - min);
            }
            pSparkIter->field_10_ang = static_cast<BYTE>(randAng);
            pSparkIter->field_14_fp = FP_FromInteger(0);
            pSparkIter->field_18_fp = FP_FromInteger(Math_RandomRange_496AB0(2, 4));
        }

        field_60_timer = sGnFrame_5C1B84 + 3;

        if (field_64_ctor_arg == 1)
        {
            New_Particle_426BE0(field_40_xpos, field_44_ypos - FP_FromInteger(4), scale, 0);
        }
        else
        {
            BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID);
            auto pParticle = alive_new<Particle>();
            if (pParticle)
            {
                pParticle->ctor_4CC4C0(
                    field_40_xpos,
                    field_44_ypos,
                    1672,
                    39,
                    21,
                    ppRes);

                pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
                pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);

                pParticle->field_20_animation.field_B_render_mode = 1;
                pParticle->field_20_animation.field_8_r = 128;
                pParticle->field_20_animation.field_9_g = 128;
                pParticle->field_20_animation.field_A_b = 128;

                if (scale == FP_FromInteger(1))
                {
                    pParticle->field_20_animation.field_C_render_layer = 36;
                }
                else
                {
                    pParticle->field_20_animation.field_C_render_layer = 17;
                }

                pParticle->field_CC_sprite_scale = scale;
            }
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    return this;
}

void Spark::vUpdate_4CBEF0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sNum_CamSwappers_5C1B66 == 0)
    {
        if (static_cast<int>(sGnFrame_5C1B84) < field_60_timer)
        {
            if (static_cast<int>(sGnFrame_5C1B84) == field_60_timer - 1)
            {
                // Reduce spark count as time passes
                field_5C_count /= 3;
            }

            for (int idx = 0; idx < field_5C_count; idx++)
            {
                SparkRes* pSpark = &field_58_pRes[idx];
                pSpark->field_0_x0 = pSpark->field_14_fp * Math_Sine_496DD0(pSpark->field_10_ang);
                pSpark->field_4_y0 = pSpark->field_14_fp * Math_Cosine_496CD0(pSpark->field_10_ang);
                pSpark->field_8_x1 = (pSpark->field_14_fp + pSpark->field_18_fp) * Math_Sine_496DD0(pSpark->field_10_ang);;
                pSpark->field_C_y1 = (pSpark->field_14_fp + pSpark->field_18_fp) * Math_Cosine_496CD0(pSpark->field_10_ang);
                pSpark->field_14_fp = pSpark->field_18_fp + FP_FromInteger(Math_RandomRange_496AB0(2, 5));
                pSpark->field_18_fp = pSpark->field_18_fp + FP_FromInteger(2);
            }
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
}

void Spark::vScreenChange_4CC4A0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

Spark* Spark::vdtor_4CBE30(signed int flags)
{
    dtor_4CBE60();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Spark::dtor_4CBE60()
{
    SetVTable(this, 0x54783C); // vTbl_Spark_54783C

    if (field_6_flags.Get(BaseGameObject::eDrawable))
    {
        gObjList_drawables_5C1124->Remove_Item(this);
    }

    if (field_54_ppSprxRes)
    {
        ResourceManager::FreeResource_49C330(field_54_ppSprxRes);
    }

    BaseGameObject_dtor_4DBEC0();
}
