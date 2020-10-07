#include "stdafx_ao.h"
#include "Function.hpp"
#include "TrapDoor.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Collisions.hpp"

START_NS_AO

struct TrapDoor_Data
{
    int field_0;
    int field_4_maxW;
    int field_8;
    int field_C;
    __int16 field_10_maxH;
    __int16 field_12_frame_table_offset;
};
ALIVE_ASSERT_SIZEOF(TrapDoor_Data, 0x14);


const TrapDoor_Data sTrapDoorData_4BD4A0[16] =
{
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 4700, 4688, 4740, 4712, 72, 41 },
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 1976, 1964, 2012, 1988, 99, 56 },
    { 1976, 1964, 2012, 1988, 99, 56 },
    { 1976, 1964, 2012, 1988, 99, 56 },
    { 1976, 1964, 2012, 1988, 99, 56 },
    { 4700, 4688, 4740, 4712, 72, 41 },
    { 4700, 4688, 4740, 4712, 72, 41 },
    { 4644, 4632, 4680, 4656, 155, 59 },
    { 1976, 1964, 2012, 1988, 99, 56 }
};

void TrapDoor::VUpdate()
{
    VUpdate_4883E0();
}

void TrapDoor::VRender_4886F0(int** ppOt)
{
    field_A8_xpos += FP_FromInteger(field_13A_xOff);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    field_A8_xpos -= FP_FromInteger(field_13A_xOff);
}

void TrapDoor::VRender(int** ppOt)
{
    VRender_4886F0(ppOt);
}

void TrapDoor::VRemove(BaseAliveGameObject* pObj)
{
    VRemove_4886E0(pObj);
}

void TrapDoor::VRemove_4886E0(BaseAliveGameObject* pObj)
{
    PlatformBase::VRemove_451680(pObj);
}

void TrapDoor::VAdd(BaseAliveGameObject* pObj)
{
    VAdd_4886D0(pObj);
}

void TrapDoor::VAdd_4886D0(BaseAliveGameObject* pObj)
{
    PlatformBase::VAdd_4515D0(pObj);
}

void TrapDoor::VScreenChanged_488740()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path ||
        gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);

        if (field_13C_set_switch_on_dead)
        {
            SwitchStates_Set(field_134_switch_idx, field_138_switch_state == 0);
        }
    }
}

void TrapDoor::VScreenChanged()
{
    VScreenChanged_488740();
}

PSX_RECT* TrapDoor::VGetBoundingRect_4887B0(PSX_RECT* pRect, int /*pointIdx*/)
{
    *pRect = field_148_bounding_rect;
    return pRect;
}

PSX_RECT* TrapDoor::VGetBoundingRect(PSX_RECT* pRect, int pointIdx)
{
    return VGetBoundingRect_4887B0(pRect, pointIdx);
}

TrapDoor* TrapDoor::Vdtor_4887D0(signed int flags)
{
    dtor_4882A0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* TrapDoor::VDestructor(signed int flags)
{
    return Vdtor_4887D0(flags);
}

BaseGameObject* TrapDoor::dtor_4882A0()
{
    SetVTable(this, 0x4BD5E0);
    gMap_507BA8.TLV_Reset_446870(field_128_tlvInfo, -1, 0, 0);
    return dtor_451490();
}

void TrapDoor::Open()
{
    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find alive objects..
        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // That are on this trap door
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (pAliveObj->field_F8_pLiftPoint == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened
                if (field_120_pCollisionLine == pAliveObj->field_F4_pLine)
                {
                    pAliveObj->field_F4_pLine = nullptr;
                }
            }
        }
    }

    Rect_Clear_40C920(&field_120_pCollisionLine->field_0_rect);
    field_120_pCollisionLine = nullptr;
    ObjListPlatforms_50766C->Remove_Item(this);
}

TrapDoor* TrapDoor::ctor_488010(Path_TrapDoor* pTlv, Map* pMap, int tlvInfo)
{
    ctor_401090();
    field_12C_flag &= ~1u;
    SetVTable(this, 0x4BD5E0);

    field_4_typeId = Types::eTrapDoor_98;
    field_134_switch_idx = pTlv->field_18_id;
    field_138_switch_state = pTlv->field_1A_start_state;

    const int cur_lvl = static_cast<int>(gMap_507BA8.field_0_current_level);

    int frame_table_offset_1 = 0;
    if (field_138_switch_state == SwitchStates_Get(pTlv->field_18_id))
    {
        field_136_state = 2;
        frame_table_offset_1 = sTrapDoorData_4BD4A0[cur_lvl].field_0;
    }
    else
    {
        field_136_state = 0;
        frame_table_offset_1 = sTrapDoorData_4BD4A0[cur_lvl].field_4_maxW;
    }

    field_13C_set_switch_on_dead = pTlv->field_1C_self_closing;
    if (pTlv->field_1E_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1004, 1, 0);

    AddDynamicCollision_4512C0(
        sTrapDoorData_4BD4A0[cur_lvl].field_4_maxW,
        sTrapDoorData_4BD4A0[cur_lvl].field_10_maxH,
        sTrapDoorData_4BD4A0[cur_lvl].field_12_frame_table_offset,
        ppRes,
        pTlv,
        pMap,
        tlvInfo);

    field_144_y = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_140_x = FP_FromInteger(pTlv->field_10_top_left.field_0_x);

    field_10_anim.Set_Animation_Data_402A40(frame_table_offset_1, 0);
    if (pTlv->field_22_direction)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_118 = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_0_x) - field_A8_xpos);
    field_11A = FP_GetExponent(FP_FromInteger(pTlv->field_14_bottom_right.field_0_x) - field_A8_xpos);
    field_13A_xOff = pTlv->field_24_anim_offset;

    if (field_136_state == 2)
    {
        // Inlined
        Open();
    }

    field_148_bounding_rect.x = pTlv->field_10_top_left.field_0_x;
    field_148_bounding_rect.y = pTlv->field_10_top_left.field_2_y;

    field_148_bounding_rect.w = pTlv->field_14_bottom_right.field_0_x;
    field_148_bounding_rect.h = pTlv->field_14_bottom_right.field_2_y;

    field_10C = 4;

    return this;
}

void TrapDoor::VUpdate_4883E0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const CameraPos direction = gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_140_x,
        field_144_y);

    switch (field_136_state)
    {
    case 0:
        if (SwitchStates_Get(field_134_switch_idx) == field_138_switch_state)
        {
            Open();
            field_136_state = 1;

            const int cur_lvl = static_cast<int>(gMap_507BA8.field_0_current_level);
            field_10_anim.Set_Animation_Data_402A40(
                sTrapDoorData_4BD4A0[cur_lvl].field_8,
                0);

            SFX_Play_43AED0(SoundEffect::Trapdoor_49, 70, direction);

            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
                gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12 ||
                gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                SFX_Play_43AED0(SoundEffect::IndustrialTrigger_97, 45, direction);
                SFX_Play_43AED0(SoundEffect::IndustrialNoise1_93, 90, direction);
            }
        }
        break;

    case 1:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_136_state = 2;
            field_130_stay_open_time = 20;
        }
        break;

    case 2:
        field_130_stay_open_time--;
        if (field_13C_set_switch_on_dead && !field_130_stay_open_time ||
            SwitchStates_Get(field_134_switch_idx) != SwitchStates_Get(field_138_switch_state))
        {
            const int cur_lvl = static_cast<int>(gMap_507BA8.field_0_current_level);

            field_10_anim.Set_Animation_Data_402A40(
                sTrapDoorData_4BD4A0[cur_lvl].field_C,
                0);
            field_136_state = 3;

            SFX_Play_43AED0(SoundEffect::Trapdoor_49, 70, direction);

            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
                gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12 ||
                gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                SFX_Play_43AED0(SoundEffect::IndustrialNoise3_95, 60, direction);
                SFX_Play_43AED0(SoundEffect::IndustrialNoise2_94, 90, direction);
            }
        }
        break;

    case 3:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_120_pCollisionLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line_40C8A0(
                field_148_bounding_rect.x,
                field_148_bounding_rect.y,
                field_148_bounding_rect.w,
                field_148_bounding_rect.y,
                32);
            ObjListPlatforms_50766C->Push_Back(this);
            field_136_state = 0;
            SwitchStates_Set(field_134_switch_idx, field_138_switch_state == 0);
        }
        break;

    default:
        return;
    }
}

END_NS_AO

