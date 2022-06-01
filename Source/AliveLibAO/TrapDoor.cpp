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

namespace AO {

struct TrapDoor_Data final
{
    AnimId field_0_open;
    AnimId field_4_closed;
    AnimId field_8_opening;
    AnimId field_C_closing;
    //s16 field_10_maxW;
    //s16 field_12_maxH;
};
//ALIVE_ASSERT_SIZEOF(TrapDoor_Data, 0x14);


const TrapDoor_Data sTrapDoorData_4BD4A0[16] = {
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // menu
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // rapture farms
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // lines
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest temple
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // stock yards
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // stock yards return
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // removed
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // desert
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // desert temple
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // credits
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // removed
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // board room
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // rupture farms return
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest chase
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}}; // desert escape


void TrapDoor::VRender(PrimHeader** ppOt)
{
    field_A8_xpos += FP_FromInteger(field_13A_xOff);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    field_A8_xpos -= FP_FromInteger(field_13A_xOff);
}

void TrapDoor::VRemove(BaseAliveGameObject* pObj)
{
    PlatformBase::VRemove_451680(pObj);
}

void TrapDoor::VAdd(BaseAliveGameObject* pObj)
{
    PlatformBase::VAdd_4515D0(pObj);
}

void TrapDoor::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mFlags.Set(BaseGameObject::eDead);

        if (field_13C_self_closing == Choice_short::eYes_1)
        {
            SwitchStates_Set(field_134_switch_id, field_138_switch_state == 0);
        }
    }
}

PSX_RECT* TrapDoor::VGetBoundingRect(PSX_RECT* pRect, s32 /*pointIdx*/)
{
    *pRect = field_148_bounding_rect;
    return pRect;
}

TrapDoor::~TrapDoor()
{
    gMap.TLV_Reset(field_128_tlvInfo, -1, 0, 0);
}

void TrapDoor::Open()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find alive objects..
        if (pObj->mFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
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

    Rect_Clear(&field_120_pCollisionLine->field_0_rect);
    field_120_pCollisionLine = nullptr;
    ObjListPlatforms_50766C->Remove_Item(this);
}

TrapDoor::TrapDoor(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo)
{
    field_12C_flag &= ~1u;

    field_4_typeId = Types::eTrapDoor_98;
    field_134_switch_id = pTlv->field_18_switch_id;
    field_138_switch_state = pTlv->field_1A_start_state;

    const s32 cur_lvl = static_cast<s32>(gMap.mCurrentLevel);

    s32 frame_table_offset_1 = 0;
    const AnimRecord& closedRec = AO::AnimRec(sTrapDoorData_4BD4A0[cur_lvl].field_4_closed);
    if (field_138_switch_state == SwitchStates_Get(pTlv->field_18_switch_id))
    {
        field_136_state = TrapDoorState::eOpen_2;
        const AnimRecord& openRec = AO::AnimRec(sTrapDoorData_4BD4A0[cur_lvl].field_0_open);
        frame_table_offset_1 = openRec.mFrameTableOffset;
    }
    else
    {
        field_136_state = TrapDoorState::eClosed_0;
        frame_table_offset_1 = closedRec.mFrameTableOffset;
    }

    field_13C_self_closing = pTlv->field_1C_self_closing;
    if (pTlv->field_1E_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, 1, 0);

    AddDynamicCollision_4512C0(
        closedRec.mFrameTableOffset,
        closedRec.mMaxW,
        closedRec.mMaxH,
        ppRes,
        pTlv,
        pMap,
        tlvInfo);

    field_144_y = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_140_x = FP_FromInteger(pTlv->field_10_top_left.field_0_x);

    field_10_anim.Set_Animation_Data(frame_table_offset_1, 0);
    if (pTlv->field_22_direction == XDirection_short::eRight_1)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_118_x_offset = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_0_x) - field_A8_xpos);
    field_11A_width_offset = FP_GetExponent(FP_FromInteger(pTlv->field_14_bottom_right.field_0_x) - field_A8_xpos);
    field_13A_xOff = pTlv->field_24_xOff;

    if (field_136_state == TrapDoorState::eOpen_2)
    {
        // Inlined
        Open();
    }

    field_148_bounding_rect.x = pTlv->field_10_top_left.field_0_x;
    field_148_bounding_rect.y = pTlv->field_10_top_left.field_2_y;

    field_148_bounding_rect.w = pTlv->field_14_bottom_right.field_0_x;
    field_148_bounding_rect.h = pTlv->field_14_bottom_right.field_2_y;

    field_10C = 4;
}

void TrapDoor::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    const CameraPos direction = gMap.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_140_x,
        field_144_y);

    switch (field_136_state)
    {
        case TrapDoorState::eClosed_0:
            if (SwitchStates_Get(field_134_switch_id) == field_138_switch_state)
            {
                Open();
                field_136_state = TrapDoorState::eOpening_1;

                const s32 cur_lvl = static_cast<s32>(gMap.mCurrentLevel);
                const AnimRecord& openingRec = AO::AnimRec(sTrapDoorData_4BD4A0[cur_lvl].field_8_opening);
                field_10_anim.Set_Animation_Data(
                    openingRec.mFrameTableOffset,
                    0);

                SFX_Play_Camera(SoundEffect::Trapdoor_49, 70, direction);

                if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eBoardRoom_12 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialTrigger_97, 45, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise1_93, 90, direction);
                }
            }
            break;

        case TrapDoorState::eOpening_1:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_136_state = TrapDoorState::eOpen_2;
                field_130_stay_open_time = 20;
            }
            break;

        case TrapDoorState::eOpen_2:
            field_130_stay_open_time--;
            if ((field_13C_self_closing == Choice_short::eYes_1 && !field_130_stay_open_time) || SwitchStates_Get(field_134_switch_id) != SwitchStates_Get(field_138_switch_state))
            {
                const s32 cur_lvl = static_cast<s32>(gMap.mCurrentLevel);
                const AnimRecord& closingRec = AO::AnimRec(sTrapDoorData_4BD4A0[cur_lvl].field_C_closing);
                field_10_anim.Set_Animation_Data(
                    closingRec.mFrameTableOffset,
                    0);
                field_136_state = TrapDoorState::eClosing_3;

                SFX_Play_Camera(SoundEffect::Trapdoor_49, 70, direction);

                if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eBoardRoom_12 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialNoise3_95, 60, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise2_94, 90, direction);
                }
            }
            break;

        case TrapDoorState::eClosing_3:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_120_pCollisionLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line(
                    field_148_bounding_rect.x,
                    field_148_bounding_rect.y,
                    field_148_bounding_rect.w,
                    field_148_bounding_rect.y,
                    eLineTypes::eUnknown_32
                );
                ObjListPlatforms_50766C->Push_Back(this);
                field_136_state = TrapDoorState::eClosed_0;
                SwitchStates_Set(field_134_switch_id, field_138_switch_state == 0);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
