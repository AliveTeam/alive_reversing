#include "stdafx.h"
#include "Spark.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"

BaseGameObject* Spark::VDestructor(signed int flags)
{
    return vdtor_4CBE30(flags);
}

void Spark::VUpdate()
{
    vUpdate_4CBEF0();
}

void Spark::VRender(int** pOrderingTable)
{
    vRender_4CC050(pOrderingTable);
}

void Spark::VScreenChanged()
{
    vScreenChange_4CC4A0();
}

Spark* Spark::ctor_4CBBB0(FP xpos, FP ypos, FP scale, unsigned __int8 count, __int16 min, __int16 max, __int16 type)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    field_6_flags.Set(BaseGameObject::eDrawable);

    SetVTable(this, 0x54783C); // vTbl_Spark_54783C
    field_4_typeId = Types::eNone_0;

    gObjList_drawables_5C1124->Push_Back(this);

    field_64_type = type;
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
            pSparkIter->field_14_radius = FP_FromInteger(0);
            pSparkIter->field_18_len = FP_FromInteger(Math_RandomRange_496AB0(2, 4));
        }

        field_60_timer = sGnFrame_5C1B84 + 3;

        if (field_64_type == 1)
        {
            // Much bigger longer lasting sparks - uses chant particles
            New_Chant_Particle_426BE0(field_40_xpos, field_44_ypos - FP_FromInteger(4), scale, 0);
        }
        else
        {
            // Normal drill/grinder type sparks
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
                pSpark->field_0_x0 = pSpark->field_14_radius * Math_Sine_496DD0(pSpark->field_10_ang);
                pSpark->field_4_y0 = pSpark->field_14_radius * Math_Cosine_496CD0(pSpark->field_10_ang);
                pSpark->field_8_x1 = (pSpark->field_14_radius + pSpark->field_18_len) * Math_Sine_496DD0(pSpark->field_10_ang);;
                pSpark->field_C_y1 = (pSpark->field_14_radius + pSpark->field_18_len) * Math_Cosine_496CD0(pSpark->field_10_ang);
                pSpark->field_14_radius = pSpark->field_18_len + FP_FromInteger(Math_RandomRange_496AB0(2, 5));
                pSpark->field_18_len = pSpark->field_18_len + FP_FromInteger(2);
            }
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
}

void Spark::vRender_4CC050(int** ppOt)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        sActiveHero_5C1B68->field_C2_lvl_number,
        sActiveHero_5C1B68->field_C0_path_number,
        field_40_xpos,
        field_44_ypos,
        0))
    {
        PSX_Point xy = { 32767, 32767 };
        PSX_Point wh = { -32767, -32767 };

        const int xOrg = FP_GetExponent(field_40_xpos) - FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
        const int yOrg = FP_GetExponent(field_44_ypos) - FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

        for (int i = 0; i < field_5C_count; i++)
        {
            SparkRes* pSpark = &field_58_pRes[i];

            Line_G2* pPrim = &pSpark->field_1C_pLineG2s[gPsxDisplay_5C1130.field_C_buffer_index];
            LineG2_Init(pPrim);

            const int y0 = yOrg + FP_GetExponent(pSpark->field_4_y0 * field_48_scale);
            const int y1 = yOrg + FP_GetExponent(pSpark->field_C_y1 * field_48_scale);
            const int x0 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_0_x0 * field_48_scale));
            const int x1 = PsxToPCX(xOrg + FP_GetExponent(pSpark->field_8_x1 * field_48_scale));

            SetXY0(pPrim, static_cast<short>(x0), static_cast<short>(y0));
            SetXY1(pPrim, static_cast<short>(x1), static_cast<short>(y1));

            SetRGB0(pPrim,
                static_cast<BYTE>(field_4C_r / 2),
                static_cast<BYTE>(field_4E_g / 2),
                static_cast<BYTE>(field_50_b / 2));

            SetRGB1(pPrim,
                static_cast<BYTE>(field_4C_r),
                static_cast<BYTE>(field_4E_g),
                static_cast<BYTE>(field_50_b));

            Poly_Set_SemiTrans_4F8A60(&pPrim->mBase.header, TRUE);
            OrderingTable_Add_4F8AA0(&ppOt[field_52_layer], &pPrim->mBase.header);

            // TODO: Can be refactored much further - looks like min/max stuff
            short v25 = static_cast<short>(x1);
            short maxX = xy.field_0_x;
            if (v25 <= xy.field_0_x)
            {
                maxX = v25;
            }

            if (x0 <= maxX)
            {
                xy.field_0_x = static_cast<short>(x0);
            }
            else if (v25 <= xy.field_0_x)
            {
                xy.field_0_x = v25;
            }

            short v14 = v25;
            if (v25 <= wh.field_0_x)
            {
                v14 = wh.field_0_x;
            }

            if (x0 <= v14)
            {
                if (v25 > wh.field_0_x)
                {
                    wh.field_0_x = v25;
                }
            }
            else
            {
                wh.field_0_x = static_cast<short>(x0);
            }

            short v15 = xy.field_2_y;
            if (y1 <= xy.field_2_y)
            {
                v15 = static_cast<short>(y1);
            }
            if (y0 <= v15)
            {
                xy.field_2_y = static_cast<short>(y0);
            }
            else if (y1 <= xy.field_2_y)
            {
                xy.field_2_y = static_cast<short>(y1);
            }

            short v16 = static_cast<short>(y1);
            if (y1 <= wh.field_2_y)
            {
                v16 = wh.field_2_y;
            }

            if (y0 <= v16)
            {
                if (y1 > wh.field_2_y)
                {
                    wh.field_2_y = static_cast<short>(y1);
                }
            }
            else
            {
                wh.field_2_y = static_cast<short>(y0);
            }
        }

        Prim_SetTPage* pTPage = &field_20_tPage[gPsxDisplay_5C1130.field_C_buffer_index];
        Init_SetTPage_4F5B60(pTPage, 1, 0, PSX_getTPage_4F60E0(0, 1, 0, 0));
        OrderingTable_Add_4F8AA0(&ppOt[field_52_layer], &pTPage->mBase);
        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            xy.field_0_x,
            xy.field_2_y,
            wh.field_0_x,
            wh.field_2_y,
            pScreenManager_5BB5F4->field_3A_idx);
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
