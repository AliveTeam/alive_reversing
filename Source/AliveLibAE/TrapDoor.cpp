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

struct TrapDoor_Data final
{
    AnimId field_0_open;
    AnimId field_4_closed;
    AnimId field_8_opening;
    AnimId field_C_closing;
    //s16 field_10_maxW;
    //s16 field_12_maxH;
};

const TrapDoor_Data sTrapDoorData_547B78[18] = {
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing}};

const TintEntry sTrapDoorTints_5639AC[18] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};



BaseGameObject* TrapDoor::VDestructor(s32 flags)
{
    return vdtor_4DD8A0(flags);
}

void TrapDoor::VUpdate()
{
    vUpdate_4DDA90();
}

void TrapDoor::VRender(PrimHeader** ppOt)
{
    vRender_4DDDD0(ppOt);
}

void TrapDoor::VScreenChanged()
{
    vScreenChanged_4DDE40();
}

s32 TrapDoor::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_4DE050(reinterpret_cast<TrapDoor_State*>(pSaveBuffer));
}

PSX_RECT* TrapDoor::vGetBoundingRect_424FD0(PSX_RECT* pRect, s32 pointIdx)
{
    return vGetBoundingRect_4DD870(pRect, pointIdx);
}


EXPORT TrapDoor* TrapDoor::ctor_4DD570(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x547CE0);
    SetType(AETypes::eTrapDoor_142);
    field_C_objectId = tlvInfo;

    field_12C_unused &= ~1u;

    field_130_stay_open_time2 = field_13C_stay_open_time;
    field_134_switch_id = pTlv->field_10_switch_id;
    field_138_switch_state = pTlv->field_12_start_state;

    const s32 levelIdx = static_cast<s32>(gMap_5C3030.field_0_current_level);

    s32 frameTableOffset = 0;
    const AnimRecord& closedRec = AnimRec(sTrapDoorData_547B78[levelIdx].field_4_closed);
    if (field_138_switch_state == SwitchStates_Get_466020(field_134_switch_id))
    {
        field_136_state = TrapDoorState::eOpen_2;
        const AnimRecord& openRec = AnimRec(sTrapDoorData_547B78[levelIdx].field_0_open);
        frameTableOffset = openRec.mFrameTableOffset;
    }
    else
    {
        field_136_state = TrapDoorState::eClosed_0;
        frameTableOffset = closedRec.mFrameTableOffset;
    }

    field_13E_self_closing = pTlv->field_14_self_closing;
    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID);

    AddDynamicCollision_4971C0(
        closedRec.mFrameTableOffset,
        closedRec.mMaxW,
        closedRec.mMaxH,
        ppRes,
        pTlv,
        pMap,
        tlvInfo);

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_26;
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_Shadow_Half_7;
        field_124_pCollisionLine->field_8_type = eLineTypes::eUnknown_36;
    }

    SetTint_425600(sTrapDoorTints_5639AC, gMap_5C3030.field_0_current_level);

    field_140_x = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_144_y = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_20_animation.Set_Animation_Data_409C80(frameTableOffset, 0);

    if (pTlv->field_1A_direction == XDirection_short::eRight_1) // TODO: check if this is the correct direction
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_11C_x_offset = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - field_B8_xpos);
    field_11E_width_offset = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - field_B8_xpos);
    field_13A_xOff = pTlv->field_1C_xOff;

    if (field_136_state == TrapDoorState::eOpen_2)
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

s32 CC TrapDoor::CreateFromSaveState_4DDED0(const u8* pData)
{
    auto pState = reinterpret_cast<const TrapDoor_State*>(pData);
    auto pTlv = static_cast<Path_TrapDoor*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_8_tlvInfo));

    switch (gMap_5C3030.field_0_current_level)
    {
        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
        case LevelIds::eMudomoVault_Ender_11:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("VLTSTRAP.BAN", nullptr);
            }
            break;

        default:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("TRAPDOOR.BAN", nullptr);
            }
            break;
    }

    auto pTrapDoor = ae_new<TrapDoor>();
    if (pTrapDoor)
    {
        pTrapDoor->ctor_4DD570(pTlv, nullptr, pState->field_8_tlvInfo);
        pTrapDoor->field_130_stay_open_time2 = pState->field_4_open_time;
        pTrapDoor->field_136_state = pState->field_2_state;

        if (pState->field_2_state == TrapDoorState::eClosing_3 || pState->field_2_state == TrapDoorState::eOpening_1)
        {
            pTrapDoor->Open_4DD960();
        }
    }

    return sizeof(TrapDoor_State);
}

EXPORT BaseGameObject* TrapDoor::vdtor_4DD8A0(s32 flags)
{
    dtor_4DD8D0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

EXPORT void TrapDoor::vUpdate_4DDA90()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const CameraPos direction = gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_140_x, field_144_y);

    switch (field_136_state)
    {
        case TrapDoorState::eClosed_0:
            if (SwitchStates_Get_466020(field_134_switch_id) == field_138_switch_state)
            {
                Open_4DD960();
                field_136_state = TrapDoorState::eOpening_1;
                const AnimRecord& openingRec = AnimRec(sTrapDoorData_547B78[static_cast<s32>(gMap_5C3030.field_0_current_level)].field_8_opening);
                field_20_animation.Set_Animation_Data_409C80(openingRec.mFrameTableOffset, 0);

                if (gMap_5C3030.field_0_current_level == LevelIds::eMines_1 || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8 || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_Ender_14 || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5 || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_Ender_12 || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6 || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_Ender_13 || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9 || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_Ender_10)
                {
                    SFX_Play_46FC20(SoundEffect::IndustrialTrigger_80, 45, direction);
                    SFX_Play_46FC20(SoundEffect::IndustrialNoise1_76, 90, direction);
                }
            }
            break;

        case TrapDoorState::eOpening_1:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                SFX_Play_46FC20(SoundEffect::TrapdoorOpen_43, 70, direction);
                field_136_state = TrapDoorState::eOpen_2;
                field_130_stay_open_time2 = field_13C_stay_open_time;
            }
            break;

        case TrapDoorState::eOpen_2:
            field_130_stay_open_time2--;

            if ((field_13E_self_closing == Choice_short::eYes_1 && field_130_stay_open_time2 + 1 <= 0) || SwitchStates_Get_466020(field_134_switch_id) != field_138_switch_state)
            {
                const AnimRecord& closingRec = AnimRec(sTrapDoorData_547B78[static_cast<s32>(gMap_5C3030.field_0_current_level)].field_C_closing);
                field_20_animation.Set_Animation_Data_409C80(closingRec.mFrameTableOffset, 0);

                field_136_state = TrapDoorState::eClosing_3;

                if (gMap_5C3030.field_0_current_level == LevelIds::eMines_1 || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_8 || gMap_5C3030.field_0_current_level == LevelIds::eBonewerkz_Ender_14 || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_5 || gMap_5C3030.field_0_current_level == LevelIds::eFeeCoDepot_Ender_12 || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_6 || gMap_5C3030.field_0_current_level == LevelIds::eBarracks_Ender_13 || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_9 || gMap_5C3030.field_0_current_level == LevelIds::eBrewery_Ender_10)
                {
                    SFX_Play_46FC20(SoundEffect::IndustrialNoise3_78, 60, direction);
                    SFX_Play_46FC20(SoundEffect::IndustrialNoise2_77, 90, direction);
                }
            }
            break;

        case TrapDoorState::eClosing_3:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                SFX_Play_46FC20(SoundEffect::TrapdoorClose_42, 70, direction);
                Add_To_Collisions_Array_4DDA20();
                field_136_state = TrapDoorState::eClosed_0;
                SwitchStates_Set_465FF0(field_134_switch_id, field_138_switch_state == 0);
            }
            break;

        default:
            return;
    }
}

EXPORT void TrapDoor::vRender_4DDDD0(PrimHeader** ppOt)
{
    field_B8_xpos += FP_FromInteger(field_13A_xOff);
    BaseAliveGameObject::VRender(ppOt);
    field_B8_xpos -= FP_FromInteger(field_13A_xOff);
}

EXPORT void TrapDoor::vScreenChanged_4DDE40()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        if (field_13E_self_closing == Choice_short::eYes_1)
        {
            SwitchStates_Set_465FF0(field_134_switch_id, field_138_switch_state == 0);
        }
    }
}

EXPORT s32 TrapDoor::vGetSaveState_4DE050(TrapDoor_State* pState)
{
    pState->field_0_type = AETypes::eTrapDoor_142;
    pState->field_4_open_time = field_130_stay_open_time2;
    pState->field_2_state = field_136_state;
    pState->field_8_tlvInfo = field_128_tlvInfo;
    return sizeof(TrapDoor_State);
}

EXPORT PSX_RECT* TrapDoor::vGetBoundingRect_4DD870(PSX_RECT* pRect, s32 /*not_used*/)
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
        field_124_pCollisionLine->field_8_type = eLineTypes::eUnknown_36;
    }

    ObjList_5C1B78->Push_Back(this);
}

void TrapDoor::Open_4DD960()
{
    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find alive objects.
        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // That are on this trap door.
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (sObjectIds_5C1B70.Find_449CF0(pAliveObj->field_110_id) == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened.
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
