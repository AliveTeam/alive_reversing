#include "stdafx.h"
#include "Mine.hpp"
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


void Mine_ForceLink() {
}

ALIVE_VAR(0, 0x5C3008, Mine*, sMineSFXOwner_5C3008, nullptr);

//signed int UXB::IsColliding_4DF630()
//{
//    PSX_RECT uxbBound;
//    vGetBoundingRect_424FD0(&uxbBound, 1);
//
//    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
//    {
//        BaseAliveGameObject * pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
//
//        if (!pObj)
//        {
//            break;
//        }
//
//        // e114_Bit6 May be "can set off explosives?"
//        if (pObj->field_114_flags.Get(e114_Bit6_SetOffExplosives) && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
//        {
//            PSX_RECT objBound;
//            pObj->vGetBoundingRect_424FD0(&objBound, 1);
//
//            int objX = FP_GetExponent(pObj->field_B8_xpos);
//            int objY = FP_GetExponent(pObj->field_BC_ypos);
//
//            if (objX > uxbBound.x &&
//                objX < uxbBound.w &&
//                objY < uxbBound.h + 5 &&
//                uxbBound.x <= objBound.w &&
//                uxbBound.w >= objBound.x &&
//                uxbBound.h >= objBound.y &&
//                uxbBound.y <= objBound.h &&
//                pObj->field_CC_sprite_scale == field_CC_sprite_scale)
//            {
//                return 1;
//            }
//        }
//    }
//
//    return 0;
//}

void Mine::VUpdate()
{
    Update_46B5D0();
}

void Mine::VRender(int ** pOrderingTable)
{
    Render_46B7A0(pOrderingTable);
}

void Mine::VDestructor(signed int /*flags*/)
{

}

void Mine::VScreenChanged()
{
    ScreenChanged_46BAE0();
}

Mine * Mine::ctor_46B120(Path_Mine * pPath, TlvItemInfoUnion tlv)
{
    ctor_408240(0);

    SetVTable(this, 0x546164);
    SetVTable(&field_124_animation, 0x544290);

    field_4_typeId = eMine;

    Animation_Init_424E10(784, 38, 0xDu, Add_Resource_4DC130(ResourceManager::Resource_Animation, 1036), 1, 1u);

    field_118 = 0;
    field_6_flags.Set(Options::eInteractive);
    field_6_flags.Set(Options::eCanExplode);

    if (pPath->field_14_scale)
    {
        if (pPath->field_14_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = 16;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1);
        field_20_animation.field_C_render_layer = 35;
        field_D6_scale = 1;
    }
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
    field_11C_tlv = tlv;
    field_120_gnframe = sGnFrame_5C1B84;
    field_124_animation.Init_40A030(400, gObjList_animations_5C1A24, this, 36, 8u, Add_Resource_4DC130(ResourceManager::Resource_Animation, kMineflshResID), 1u, 0, 0);

    field_124_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_124_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);

    field_124_animation.field_C_render_layer = field_20_animation.field_C_render_layer;
    field_124_animation.field_14_scale = field_CC_sprite_scale;
    field_124_animation.field_8_r = 128;
    field_124_animation.field_9_g = 128;
    field_124_animation.field_A_b = 128;

    field_11A_disabled_resources = pPath->field_16_disabled_resources;

    field_1BC_flags.Clear(Mine_Flags_1BC::e1BC_Bit1);
    if (pPath->field_18_persists_offscreen & 1)
    {
        field_1BC_flags.Set(Mine_Flags_1BC::e1BC_Bit1);
    }

    Add_Resource_4DC130(ResourceManager::Resource_Animation, kAbebombResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, kDebrisID00);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, kBgexpldResID);

    if (!(field_11A_disabled_resources & 1))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, kAbeblowResID);
    }
    if (!(field_11A_disabled_resources & 2))
    {
        Add_Resource_4DC130(ResourceManager::Resource_Animation, kSlogBlowResID );
    }

    FP gridSnap = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    field_E4 = field_B8_xpos - (gridSnap / FP_FromDouble(2.0));
    field_EC = (gridSnap / FP_FromDouble(2.0)) + field_B8_xpos;
    field_6_flags.Set(Options::eInteractive);
    field_DC_bApplyShadows |= 2u;
    field_E8 = field_BC_ypos - gridSnap;
    field_F0_prev_base = field_BC_ypos;

    return this;
}

void Mine::Update_46B5D0()
{
    const bool onScreen = gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0);

    if (field_118)
    {
        if (field_118 == 1 && sGnFrame_5C1B84 >= field_120_gnframe)
        {
            auto explosion = alive_new<BaseBomb>();
            if (explosion)
            {
                explosion->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
            }
            field_6_flags.Set(Options::eDead);
        }
    }
    else
    {
        if (field_20_animation.field_92_current_frame == 1
            && (!sMineSFXOwner_5C3008 || sMineSFXOwner_5C3008 == this))
        {
            if (onScreen)
            {
                SFX_Play_46FA90(3u, 35, 0x10000);
                sMineSFXOwner_5C3008 = this;
            }
            else
            {
                sMineSFXOwner_5C3008 = nullptr;
            }
        }
        if (Mine::IsColliding_46B8C0())
        {
            field_118 = 1;
            field_120_gnframe = sGnFrame_5C1B84;
        }
    }
    if (field_118 != 1)
    {
        BaseGameObject * pEventObj = Event_Get_422C00(kEventDeathReset);
        if (pEventObj || field_C2_lvl_number != gMap_5C3030.sCurrentLevelId_5C3030 ||
            field_C0_path_number != gMap_5C3030.sCurrentPathId_5C3032)
        {
            field_6_flags.Set(Options::eDead);
        }
    }
}

void Mine::Render_46B7A0(int ** pOt)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
        {
            this->field_124_animation.vRender_40B820(FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                FP_GetExponent(FP_FromInteger(field_D8_yOffset) + field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                pOt,
                0,
                0);

            Render_424B90(pOt);
        }
    }
}

void Mine::ScreenChanged_46BAE0()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId
        || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId
        || !field_1BC_flags.Get(Mine_Flags_1BC::e1BC_Bit1))
    {
        field_6_flags.Set(Options::eDead);
    }
}

bool Mine::IsColliding_46B8C0()
{
    PSX_RECT mineBound;
    vGetBoundingRect_424FD0(&mineBound, 1);

    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject * pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        // e114_Bit6 May be "can set off explosives?"
        if (pObj->field_114_flags.Get(e114_Bit6_SetOffExplosives) && pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            PSX_RECT objBound;
            pObj->vGetBoundingRect_424FD0(&objBound, 1);

            int objX = FP_GetExponent(pObj->field_B8_xpos);
            int objY = FP_GetExponent(pObj->field_BC_ypos);

            if (objX > mineBound.x &&
                objX < mineBound.w &&
                objY < mineBound.h + 12 &&
                mineBound.x <= objBound.w &&
                mineBound.w >= objBound.x &&
                mineBound.h >= objBound.y &&
                mineBound.y <= objBound.h &&
                pObj->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                return 1;
            }
        }
    }

    return 0;
}

