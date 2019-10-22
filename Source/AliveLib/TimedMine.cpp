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
#include "BaseBomb.hpp"
#include "LiftPoint.hpp"

static TintEntry sTimedMineTint_550EB8[1] = { { -1, 127u, 127u, 127u } };

void TimedMine::VUpdate()
{
    Update_410A80();
}

void TimedMine::VRender(int ** pOrderingTable)
{
    Render_410CD0(pOrderingTable);
}

BaseGameObject* TimedMine::VDestructor(signed int flags)
{
    return vdtor_4108B0(flags);
}

void TimedMine::VScreenChanged()
{
    vScreenChanged_411270();
}

__int16 TimedMine::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_410FA0(pFrom);
}

void TimedMine::VOnThrowableHit(BaseGameObject* pFrom)
{
    vOnThrowableHit_410F00(pFrom);
}

void TimedMine::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_410E30();
}

TimedMine* TimedMine::ctor_410600(Path_TimedMine* pPath, TlvItemInfoUnion tlv)
{
    ctor_408240(0);

    SetVTable(this, 0x5442A8);
    SetVTable(&field_124_animation, 0x544290);

    field_4_typeId = Types::eTimedMine_or_MovingBomb_10;

    Animation_Init_424E10(836, 35, 0x11u, BaseGameObject::Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kBombResID), TRUE, TRUE);

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

    field_B8_xpos = FP_FromInteger((pPath->field_8_top_left.field_0_x + pPath->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pPath->field_8_top_left.field_2_y);

    FP hitY = {};
    FP hitX = {};

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 0x10))
    {
        field_BC_ypos = hitY;
    }

    field_11C_tlv = tlv.all;
    field_120_gnframe = sGnFrame_5C1B84;
    SetBaseAnimPaletteTint_425690(sTimedMineTint_550EB8, gMap_5C3030.sCurrentLevelId_5C3030, kBombResID);

    const FP gridSnap = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    field_6_flags.Set(Options::eInteractive);
    field_DC_bApplyShadows |= 2u;

    field_E4_collection_rect.x = field_B8_xpos - (gridSnap / FP_FromDouble(2.0));
    field_E4_collection_rect.y = field_BC_ypos - gridSnap;
    field_E4_collection_rect.w = (gridSnap / FP_FromDouble(2.0)) + field_B8_xpos;
    field_E4_collection_rect.h = field_BC_ypos;

    field_110_id = -1;

    return this;
}

void TimedMine::Update_410A80()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(Options::eDead);
    }

    if (!field_1C4_flags.Get(TimedMine_Flags_1C4::e1C4_Bit0))
    {
        StickToLiftPoint_411100();
    }

    if (pPlatform && pPlatform->vOnAnyFloor_461920())
    {
        field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
        field_E4_collection_rect.w = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2)) + field_B8_xpos;
        field_E4_collection_rect.h = field_BC_ypos;
        field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (field_118_armed == 1)
    {
        if (sGnFrame_5C1B84 > field_1BC + field_1C0)
        {
            field_1BC = sGnFrame_5C1B84;
            const CameraPos soundDir = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);
            SFX_Play_46FC20(2u, 50, soundDir);

            // TODO: Modulus ?
            if (((field_120_gnframe - sGnFrame_5C1B84) & 0xFFFFFFF8) >= 144)
            {
                field_1C0 = 18;
            }
            else
            {
                field_1C0 = (field_120_gnframe - sGnFrame_5C1B84) >> 3;
            }
        }

        if (sGnFrame_5C1B84 >= field_120_gnframe)
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

        PSX_RECT frameRect = {};
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
    if (pAnimation->Init_40A030(544, gObjList_animations_5C1A24, this, 36, 0x15u, Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kBombflshResID), 1, 0, 0))
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

void TimedMine::StickToLiftPoint_411100()
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos - FP_FromInteger(20),
        field_B8_xpos, field_BC_ypos + FP_FromInteger(20),
        &pLine, &hitX, &hitY,
        (field_D6_scale == 1) ? 15 : 240))
    {
        if (pLine->field_8_type == 32 || pLine->field_8_type == 36)
        {
            if (ObjList_5C1B78)
            {
                for (int i=0; i < ObjList_5C1B78->Size(); i++)
                {
                    BaseGameObject* pObj = ObjList_5C1B78->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->field_4_typeId == Types::eLiftPoint_78)
                    {
                        PSX_RECT bRect = {};
                        auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                        pLiftPoint->vGetBoundingRect_424FD0(&bRect, 1);
                        if (FP_GetExponent(field_B8_xpos) > bRect.x &&
                            FP_GetExponent(field_B8_xpos) < bRect.w &&
                            FP_GetExponent(field_BC_ypos) < bRect.h)
                        {
                            pLiftPoint->VAdd(this);
                            field_110_id = pObj->field_8_object_id;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void TimedMine::dtor_410970()
{
    SetVTable(this, 0x5442A8);

    auto pPlatform = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (field_118_armed != 1 || sGnFrame_5C1B84 < field_120_gnframe)
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlv, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_11C_tlv, -1, 0, 1);
    }

    field_124_animation.vCleanUp_40C630();

    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
    }

    field_6_flags.Clear(BaseGameObject::eInteractive);
    dtor_4080B0();
}

TimedMine* TimedMine::vdtor_4108B0(signed int flags)
{
    dtor_410970();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void TimedMine::vScreenChanged_411270()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    if (field_118_armed != 1)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

__int16 TimedMine::vTakeDamage_410FA0(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
    case Types::eAbe_69:
    case Types::eType_104:
    case Types::eExplosion_109:
    case Types::eShrykull_121:
    {
        field_6_flags.Set(BaseGameObject::eDead);
        auto pExplosion = alive_new<BaseBomb>();
        if (pExplosion)
        {
            pExplosion->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
        }
        field_118_armed = 1;
        field_120_gnframe = sGnFrame_5C1B84;
        return 1;
    }
    default:
        return 0;
    }
}

void TimedMine::vOnThrowableHit_410F00(BaseGameObject* /*pHitBy*/)
{
    auto pExplosion = alive_new<BaseBomb>();
    if (pExplosion)
    {
        pExplosion->ctor_423E70(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
    }
    field_118_armed = 1;
    field_6_flags.Set(BaseGameObject::eDead);
    field_120_gnframe = sGnFrame_5C1B84;
}

void TimedMine::vOnPickUpOrSlapped_410E30()
{
    if (field_118_armed != 1)
    {
        field_118_armed = 1;
        if ((signed int)(field_11A_explode_timeout & 0xFFFC) >= 72)
        {
            field_1C0 = 18;
        }
        else
        {
            field_1C0 = field_11A_explode_timeout >> 2;
        }
        field_1BC = sGnFrame_5C1B84;
        field_20_animation.Set_Animation_Data_409C80(848, 0);
        field_120_gnframe = sGnFrame_5C1B84 + field_11A_explode_timeout;
        field_124_animation.Set_Animation_Data_409C80(556, 0);
        SFX_Play_46FA90(2u, 0);
    }
}
