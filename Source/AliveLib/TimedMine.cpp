#include "stdafx.h"
#include "TimedMine.hpp"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "Midi.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"

TintEntry sTimedMineTint_550EB8[1] = { { -1, 127u, 127u, 127u } };


void TimedMine_ForceLink() {
}


void TimedMine::VUpdate()
{
    Update_410A80();
}

void TimedMine::VRender(int ** pOrderingTable)
{
    Render_410CD0(pOrderingTable);
}

BaseGameObject* TimedMine::VDestructor(signed int /*flags*/)
{
    abort();
}

void TimedMine::VScreenChanged()
{
    abort();
}

TimedMine * TimedMine::ctor_410600(Path_TimedMine * pPath, TlvItemInfoUnion tlv)
{
    ctor_408240(0);

    SetVTable(this, 0x5442A8);
    SetVTable(&field_124_animation, 0x544290);

    field_4_typeId = Types::eTimedMine_10;

    Animation_Init_424E10(836, 35, 0x11u, BaseGameObject::Add_Resource_4DC130(
        ResourceManager::Resource_Animation,
        kBombResID), 1, 1u);

    field_6_flags.Set(Options::eInteractive);

    field_1C4_flags.Clear(TimedMine_Flags_1C4::e1C4_Bit0);

    field_118_armed = 0;

    if (pPath->field_14_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 16;
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1.0);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 35;
    }

    InitBlinkAnimation_4108E0(&field_124_animation);

    field_11A_explode_timeout = pPath->field_16_timeout;

    const int v7 = pPath->field_0_mBase.field_8_top_left.field_0_x + pPath->field_0_mBase.field_C_bottom_right.field_0_x;
    field_B8_xpos = FP_FromInteger(v7 / 2);
    const FP v8 = FP_FromInteger(pPath->field_0_mBase.field_8_top_left.field_2_y);
    field_BC_ypos = v8;

    FP hitY;
    FP hitX;

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(v7 / 2),
        v8,
        FP_FromInteger(v7 / 2),
        v8 + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_BC_ypos = hitY;
    }

    field_11C_tlv = tlv.all;
    field_120_gnframe = sGnFrame_5C1B84;
    SetBaseAnimPaletteTint_425690(sTimedMineTint_550EB8, gMap_5C3030.sCurrentLevelId_5C3030, kBombResID);

    FP gridSnap = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    field_E4 = field_B8_xpos - (gridSnap / FP_FromDouble(2.0));
    field_EC = (gridSnap / FP_FromDouble(2.0)) + field_B8_xpos;
    field_6_flags.Set(Options::eInteractive);
    field_DC_bApplyShadows |= 2u;
    field_E8 = field_BC_ypos - gridSnap;
    field_F0_prev_base = field_BC_ypos;
    field_110_id = -1;

    return this;
}

void TimedMine::Update_410A80()
{
    NOT_IMPLEMENTED();

    BaseGameObject * v2 = sObjectIds_5C1B70.Find_449CF0(field_110_id);

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(Options::eDead);
    }
    if (!(field_1C4_flags.Get(TimedMine_Flags_1C4::e1C4_Bit0)))
    {
        sub_411100();
    }

    if (v2)
    {
        // TODO: Figure this crap out
        //if (!(*((unsigned __int16(__thiscall **)(BaseGameObject *))*((int*)&v2) + 32))(v2))
        //{
        //    /*v3 = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        //    field_0_base.field_E4 = field_0_base.field_0_mBase.field_B8_xpos
        //        - Math_FixedPoint_Divide_496B70(v3, 0x20000);
        //    v4 = ScaleToGridSize_4498B0(field_0_base.field_0_mBase.field_CC_sprite_scale);
        //    v5 = Math_FixedPoint_Divide_496B70(v4, 0x20000);
        //    v6 = field_0_base.field_0_mBase.field_CC_sprite_scale;
        //    field_0_base.field_EC = v5 + field_0_base.field_0_mBase.field_B8_xpos;
        //    v7 = ScaleToGridSize_4498B0(v6);
        //    v8 = field_0_base.field_0_mBase.field_BC_ypos;
        //    field_0_base.field_F0_prev_base = v8;
        //    field_0_base.field_E8 = v8 - v7;*/
        //}
    }
    if (field_118_armed == 1)
    {
        unsigned int v9 = sGnFrame_5C1B84;
        if (sGnFrame_5C1B84 > field_1BC + field_1C0)
        {
            FP v10 = field_BC_ypos;
            field_1BC = sGnFrame_5C1B84;
            const CameraPos soundDir = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, v10);
            SFX_Play_46FC20(2u, 50, soundDir);

            if (((field_120_gnframe - sGnFrame_5C1B84) & 0xFFFFFFF8) >= 144)
            {
                field_1C0 = 18;
            }
            else
            {
                field_1C0 = (field_120_gnframe - sGnFrame_5C1B84) >> 3;
            }
            v9 = sGnFrame_5C1B84;
        }
        if (v9 >= field_120_gnframe)
        {
            auto explosion = alive_new<BaseBomb>();
            if (explosion)
            {
                explosion->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
            }
            field_6_flags.Set(Options::eDead);
        }
    }
}

void TimedMine::Render_410CD0(int ** pOt)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0))
    {
        field_124_animation.vRender_40B820(
            FP_GetExponent((field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x)),
            FP_GetExponent((field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y - FP_NoFractional(field_CC_sprite_scale * FP_FromDouble(14)))),
            pOt,
            0,
            0);

        PSX_RECT frameRect;
        field_124_animation.Get_Frame_Rect_409E10(&frameRect);

        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_5BB5F4->field_3A_idx);

        Render_424B90(pOt);
    }
}

void TimedMine::InitBlinkAnimation_4108E0(Animation * pAnimation)
{
    if (pAnimation->Init_40A030(544, gObjList_animations_5C1A24, this, 36, 0x15u, Add_Resource_4DC130(ResourceManager::Resource_Animation, kBombflshResID), 1u, 0, 0))
    {
        pAnimation->field_C_render_layer = field_20_animation.field_C_render_layer;
        pAnimation->field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        pAnimation->field_4_flags.Set(AnimFlags::eBit16_bBlending);
        pAnimation->field_14_scale = field_CC_sprite_scale;
        pAnimation->field_8_r = 128;
        pAnimation->field_9_g = 128;
        pAnimation->field_A_b = 128;
        pAnimation->field_B_render_mode = 1;
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed);
    }
}

void TimedMine::sub_411100()
{
    NOT_IMPLEMENTED();
}
