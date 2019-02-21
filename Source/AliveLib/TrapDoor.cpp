#include "stdafx.h"
#include "TrapDoor.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Path.hpp"
#include "stdlib.hpp"

struct TrapDoor_Data
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    __int16 field_10;
    __int16 field_12_frame_table_offset;
};

const TrapDoor_Data sTrapDoorData_547B78[18] =
{
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 5316, 5328, 5340, 5368, 77, 41 },
    { 5316, 5328, 5340, 5368, 77, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 5316, 5328, 5340, 5368, 77, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 5316, 5328, 5340, 5368, 77, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 4800, 4788, 4840, 4812, 72, 41 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 }
};

const TintEntry sTrapDoorTints_5639AC[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};



BaseGameObject* TrapDoor::VDestructor(signed int flags)
{
    return vdtor_4DD8A0(flags);
}

void TrapDoor::VUpdate()
{
    vUpdate_4DDA90();
}

void TrapDoor::VRender(int** pOrderingTable)
{
    vRender_4DDDD0(pOrderingTable);
}

void TrapDoor::VScreenChanged()
{
    vScreenChanged_4DDE40();
}

int TrapDoor::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_4DE050(reinterpret_cast<TrapDoor_State*>(pSaveBuffer));
}

PSX_RECT* TrapDoor::vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx)
{
    return vGetBoundingRect_4DD870(pRect, pointIdx);
}


EXPORT TrapDoor* TrapDoor::ctor_4DD570(Path_TrapDoor* pTlv, Map* pMap, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x547CE0);
    field_4_typeId = Types::eTrapDoor_142;
    field_C_objectId = tlvInfo;

    field_12C &= ~1u;

    field_130_stay_open_time2 = field_13C_stay_open_time;
    field_134_switch_idx = pTlv->field_10_id;
    field_138_switch_state = pTlv->field_12_start_state;

    const int levelIdx = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);

    int frameTableOffset = 0;
    if (field_138_switch_state == SwitchStates_Get_466020(field_134_switch_idx))
    {
        field_136_state = TrapDoorState::eState_2;
        frameTableOffset = sTrapDoorData_547B78[levelIdx].field_0;
    }
    else
    {
        field_136_state = TrapDoorState::eState_0;
        frameTableOffset = sTrapDoorData_547B78[levelIdx].field_4;
    }

    field_13E_set_switch_on_dead = pTlv->field_14_self_closing;
    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID);

    AddDynamicCollision_4971C0(
        sTrapDoorData_547B78[levelIdx].field_4,
        sTrapDoorData_547B78[levelIdx].field_10,
        sTrapDoorData_547B78[levelIdx].field_12_frame_table_offset,
        ppRes,
        pTlv,
        pMap,
        tlvInfo);

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 26;
    }
    else
    {
        field_20_animation.field_C_render_layer = 7;
        field_124_pCollisionLine->field_8_type = 36;
    }

    SetTint_425600(sTrapDoorTints_5639AC, gMap_5C3030.sCurrentLevelId_5C3030);

    field_140_x = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_144_y = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_20_animation.Set_Animation_Data_409C80(frameTableOffset, 0);

    if (pTlv->field_1A_direction)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_11C = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - field_B8_xpos);
    field_11E = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - field_B8_xpos);
    field_13A_xOff = pTlv->field_1C_anim_offset;

    if (field_136_state == TrapDoorState::eState_2)
    {
        Open_4DD960();
    }

    field_148_bounding_rect.x = pTlv->field_8_top_left.field_0_x;
    field_148_bounding_rect.y = pTlv->field_8_top_left.field_2_y;

    field_148_bounding_rect.w = pTlv->field_C_bottom_right.field_0_x;
    field_148_bounding_rect.h = pTlv->field_C_bottom_right.field_2_y;

    field_DC_bApplyShadows |= 2u;
    field_13C_stay_open_time = pTlv->field_1E_stay_open_time;

    return this;
}

signed int CC TrapDoor::CreateFromSaveState_4DDED0(const BYTE* pData)
{
    auto pState = reinterpret_cast<const TrapDoor_State*>(pData);
    auto pTlv = static_cast<Path_TrapDoor*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_8_tlvInfo));

    switch (gMap_5C3030.sCurrentLevelId_5C3030)
    {
    case LevelIds::eMudomoVault_3:
    case LevelIds::eMudancheeVault_4:
    case LevelIds::eMudancheeVault_Ender_7:
    case LevelIds::eMudomoVault_Ender_11:
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("VLTSTRAP.BAN", nullptr);
        }
        break;

    default:
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kP6c1trapResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("TRAPDOOR.BAN", nullptr);
        }
        break;
    }

    auto pTrapDoor = alive_new<TrapDoor>();
    if (pTrapDoor)
    {
        pTrapDoor->ctor_4DD570(pTlv, nullptr, pState->field_8_tlvInfo);
        pTrapDoor->field_130_stay_open_time2 = pState->field_4_open_time;
        pTrapDoor->field_136_state = pState->field_2_state;

        if (pState->field_2_state == TrapDoorState::eState_3 || pState->field_2_state == TrapDoorState::eState_1)
        {
            pTrapDoor->Open_4DD960();
        }
    }

    return sizeof(TrapDoor_State);
}

EXPORT BaseGameObject* TrapDoor::vdtor_4DD8A0(signed int flags)
{
    dtor_4DD8D0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

EXPORT void TrapDoor::vUpdate_4DDA90()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    const signed __int16 mode = gMap_5C3030.sub_4811A0(field_C2_lvl_number, field_C0_path_number, field_140_x, field_144_y);

    switch (field_136_state)
    {
    case TrapDoorState::eState_0:
        if (SwitchStates_Get_466020(field_134_switch_idx) == field_138_switch_state)
        {
            Open_4DD960();
            field_136_state = TrapDoorState::eState_1;
            field_20_animation.Set_Animation_Data_409C80(sTrapDoorData_547B78[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_8, 0);

            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMines_1 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_Ender_14 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_5 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_Ender_12 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_6 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_Ender_13 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_9 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_Ender_10)
            {
                SFX_Play_46FC20(0x50u, 45, mode, 0x10000);
                SFX_Play_46FC20(0x4Cu, 90, mode, 0x10000);
            }
        }
        break;

    case TrapDoorState::eState_1:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_46FC20(0x2Bu, 70, mode, 0x10000);
            field_136_state = TrapDoorState::eState_2;
            field_130_stay_open_time2 = field_13C_stay_open_time;
        }
        break;

    case TrapDoorState::eState_2:
        field_130_stay_open_time2--;

        if (field_13E_set_switch_on_dead && field_130_stay_open_time2 + 1 <= 0 || SwitchStates_Get_466020(field_134_switch_idx) != field_138_switch_state)
        {
            field_20_animation.Set_Animation_Data_409C80(sTrapDoorData_547B78[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_C, 0);
            
            field_136_state = TrapDoorState::eState_3;

            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMines_1 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_Ender_14 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_5 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_Ender_12 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_6 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_Ender_13 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_9 ||
                gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_Ender_10)
            {
                SFX_Play_46FC20(0x4Eu, 60, mode, 0x10000);
                SFX_Play_46FC20(0x4Du, 90, mode, 0x10000);
            }
        }
        break;

    case TrapDoorState::eState_3:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_46FC20(0x2Au, 70, mode, 0x10000);
            Add_To_Collisions_Array_4DDA20();
            field_136_state = TrapDoorState::eState_0;
            SwitchStates_Set_465FF0(field_134_switch_idx, field_138_switch_state == 0);
        }
        break;

    default:
        return;
    }
}

EXPORT void TrapDoor::vRender_4DDDD0(int **ot)
{
    field_B8_xpos += FP_FromInteger(field_13A_xOff);
    BaseAliveGameObject::VRender(ot);
    field_B8_xpos -= FP_FromInteger(field_13A_xOff);
}

EXPORT void TrapDoor::vScreenChanged_4DDE40()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
        if (field_13E_set_switch_on_dead)
        {
            SwitchStates_Set_465FF0(field_134_switch_idx, field_138_switch_state == 0);
        }
    }
}

EXPORT int TrapDoor::vGetSaveState_4DE050(TrapDoor_State* pState)
{
    pState->field_0_type = Types::eTrapDoor_142;
    pState->field_4_open_time = field_130_stay_open_time2;
    pState->field_2_state = field_136_state;
    pState->field_8_tlvInfo = field_128_tlvInfo;
    return sizeof(TrapDoor_State);
}

EXPORT PSX_RECT* TrapDoor::vGetBoundingRect_4DD870(PSX_RECT* pRect, int /*not_used*/)
{
    *pRect = field_148_bounding_rect;
    return pRect;
}

void TrapDoor::VAdd(BaseAliveGameObject* pObj)
{
    vAdd_4DDD90(pObj);
}

void TrapDoor::VRemove(BaseAliveGameObject* pObj)
{
    vRemove_4DDDB0(pObj);
}

void TrapDoor::vAdd_4DDD90(BaseAliveGameObject* pObj)
{
    PlatformBase::VAdd(pObj);
}

void TrapDoor::vRemove_4DDDB0(BaseAliveGameObject* pObj)
{
    PlatformBase::VRemove(pObj);
}

void TrapDoor::Add_To_Collisions_Array_4DDA20()
{
    field_124_pCollisionLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
        field_148_bounding_rect.x,
        field_148_bounding_rect.y,
        field_148_bounding_rect.w,
        field_148_bounding_rect.y,
        32);

    if (field_CC_sprite_scale != FP_FromInteger(1))
    {
        field_124_pCollisionLine->field_8_type = 36;
    }

    ObjList_5C1B78->Push_Back(this);
}

void TrapDoor::Open_4DD960()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find alive objects..
        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject))
        {
            // That are on this trap door
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (sObjectIds_5C1B70.Find_449CF0(pAliveObj->field_110_id) == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened
                if (field_124_pCollisionLine == pAliveObj->field_100_pCollisionLine)
                {
                    pAliveObj->field_100_pCollisionLine = nullptr;
                }
            }
        }
    }

    Rect_Clear_418040(&field_124_pCollisionLine->field_0_rect);
    field_124_pCollisionLine = nullptr;
    ObjList_5C1B78->Remove_Item(this);
}

void TrapDoor::dtor_4DD8D0()
{
    SetVTable(this, 0x547CE0);
    Path::TLV_Reset_4DB8E0(field_128_tlvInfo, -1, 0, 0);
    dtor_4973E0();
}
