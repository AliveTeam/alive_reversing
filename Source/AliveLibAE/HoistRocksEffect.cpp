#include "stdafx.h"
#include "HoistRocksEffect.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"

const static int dword_5556E0[4] = { 204, 180, 192, 204 };
const static __int16 word_5556F0[12] = { 5, 0, 10, 0, 30, 0, 5, 0, 0, 0, 0, 0 };

BaseGameObject* HoistRocksEffect::VDestructor(signed int flags)
{
    return vdtor_45D430(flags);
}

void HoistRocksEffect::VUpdate()
{
    Update_45D460();
}

void HoistRocksEffect::VRender(PrimHeader** ppOt)
{
    Render_45D7B0(ppOt);
}

void HoistRocksEffect::VScreenChanged()
{
    vScreenChanged_45D790();
}

HoistRocksEffect* HoistRocksEffect::ctor_45D270(Path_Hoist* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    for (HoistRockParticle& particle : field_30_rocks)
    {
        SetVTable(&particle.field_10_mAnim, 0x544290); // gVtbl_animation_2a_544290
    }

    SetVTable(this, 0x545910); // vTbl_Hoist_545910

    field_24_tlvInfo = tlvInfo;

    field_20_xpos = (pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2;
    field_22_ypos = pTlv->field_8_top_left.field_2_y;
    field_28_timer = 0;

    if (pTlv->field_16_scale == Path_Hoist::Scale::eHalf)
    {
        field_2C_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_2C_scale = FP_FromDouble(1.0);
    }

    if (gObjList_drawables_5C1124->Push_Back(this))
    {
        field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
    }

    BYTE** ppAnimData = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDrpRockResID);
    for (HoistRockParticle& particle : field_30_rocks)
    {
        particle.field_10_mAnim.Init_40A030(
            204,
            gObjList_animations_5C1A24,
            this,
            7,
            4u,
            ppAnimData,
            1u,
            0,
            0);

        particle.field_10_mAnim.field_8_r = 255;
        particle.field_10_mAnim.field_9_g = 255;
        particle.field_10_mAnim.field_A_b = 255;

        if (pTlv->field_16_scale == Path_Hoist::Scale::eHalf)
        {
            particle.field_10_mAnim.field_C_render_layer = 6;
        }
        else
        {
            particle.field_10_mAnim.field_C_render_layer = 25;
        }

        particle.field_10_mAnim.field_B_render_mode = 0;
        particle.field_10_mAnim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        particle.field_10_mAnim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        particle.field_10_mAnim.field_14_scale = field_2C_scale;

        particle.field_0_state = 0;
    }
    return this;
}

HoistRocksEffect* HoistRocksEffect::vdtor_45D430(signed int flags)
{
    dtor_45D6D0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void HoistRocksEffect::dtor_45D6D0()
{
    SetVTable(this, 0x545910); // vTbl_Hoist_545910

    gObjList_drawables_5C1124->Remove_Item(this);

    for (HoistRockParticle& particle : field_30_rocks)
    {
        particle.field_10_mAnim.vCleanUp_40C630();
    }

    Path::TLV_Reset_4DB8E0(field_24_tlvInfo, -1, 0, 0);

    BaseGameObject_dtor_4DBEC0();
}

void HoistRocksEffect::Update_45D460()
{
    if (field_28_timer <= static_cast<int>(sGnFrame_5C1B84))
    {
        int idx = 0;
        while (field_30_rocks[idx].field_0_state != 0)
        {
            if (++idx >= 4)
            {
                break;
            }
        }

        if (idx < 4)
        {
            int randomXScaled = 0;
            if (field_2C_scale == FP_FromDouble(1.0))
            {
                randomXScaled = Math_RandomRange_496AB0(-8, 8);
            }
            else
            {
                randomXScaled = Math_RandomRange_496AB0(-4, 4);
            }

            field_30_rocks[idx].field_4_xpos = FP_FromInteger(field_20_xpos + randomXScaled);
            field_30_rocks[idx].field_8_ypos = FP_FromInteger(field_22_ypos + Math_RandomRange_496AB0(-4, 4));

            field_30_rocks[idx].field_C_yVel = FP_FromInteger(0);
            field_30_rocks[idx].field_0_state = 1;

            const int randomAnimAndUpdate = 2 * Math_RandomRange_496AB0(0, 3);
            field_30_rocks[idx].field_10_mAnim.Set_Animation_Data_409C80(dword_5556E0[randomAnimAndUpdate / 2], nullptr);
            field_28_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(word_5556F0[randomAnimAndUpdate], 2 * word_5556F0[randomAnimAndUpdate]);
        }
    }

    for (HoistRockParticle& particle : field_30_rocks)
    {
        if (particle.field_0_state)
        {
            if (particle.field_C_yVel >= FP_FromInteger(10))
            {
                if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    gMap_5C3030.field_0_current_level,
                    gMap_5C3030.field_2_current_path,
                    particle.field_4_xpos,
                    particle.field_8_ypos,
                    0))
                {
                    particle.field_0_state = 0;
                }
            }
            else
            {
                particle.field_C_yVel += FP_FromDouble(0.6);
            }

            particle.field_8_ypos += particle.field_C_yVel;

            if (particle.field_0_state == 1)
            {
                PathLine* pLine = nullptr;
                FP hitX = {};
                FP hitY = {};
                if (sCollisions_DArray_5C1128->Raycast_417A60(
                    particle.field_4_xpos,
                    particle.field_8_ypos - particle.field_C_yVel,
                    particle.field_4_xpos,
                    particle.field_8_ypos,
                    &pLine,
                    &hitX,
                    &hitY,
                    field_2C_scale > FP_FromDouble(0.5) ? 1 : 16))
                {
                    particle.field_8_ypos = hitY;
                    particle.field_C_yVel = particle.field_C_yVel * FP_FromDouble(-0.3);
                    particle.field_0_state = 2;
                }
            }
        }
    }
}

void HoistRocksEffect::Render_45D7B0(PrimHeader** ppOt)
{
    for (HoistRockParticle& particle : field_30_rocks)
    {
        if (particle.field_0_state)
        {
            particle.field_10_mAnim.vRender_40B820(
                FP_GetExponent(particle.field_4_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                FP_GetExponent(particle.field_8_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                ppOt,
                0,
                0);

            PSX_RECT frameRect = {};
            particle.field_10_mAnim.Get_Frame_Rect_409E10(&frameRect);
            pScreenManager_5BB5F4->InvalidateRect_40EC90(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_5BB5F4->field_3A_idx);
        }
    }
}

void HoistRocksEffect::vScreenChanged_45D790()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}
