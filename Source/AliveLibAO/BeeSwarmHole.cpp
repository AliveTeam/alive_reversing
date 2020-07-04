#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeSwarmHole.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"

START_NS_AO

BeeSwarmHole* BeeSwarmHole::ctor_4782B0(Path_BeeSwarmHole* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BCD60);

    field_10_tlvInfo = tlvInfo;
    field_4_typeId = Types::eSligSpawner_91;

    field_1C_interval_timer = 0;

    field_14_rect.x = pTlv->field_10_top_left.field_0_x;
    field_14_rect.y = pTlv->field_10_top_left.field_2_y;

    field_14_rect.w = pTlv->field_14_bottom_right.field_0_x;
    field_14_rect.h = pTlv->field_14_bottom_right.field_2_y;

    field_20_interval = pTlv->field_1A_interval;
    field_22_id = pTlv->field_1C_id;
    field_24_movement_type = pTlv->field_1E_movement_type;

    field_26_num_bees = pTlv->field_20_size;
    field_28_chase_time = pTlv->field_22_chase_time;
    field_2A_speed = pTlv->field_24_speed;

    return this;
}

void BeeSwarmHole::VUpdate()
{
    VUpdate_478320();
}

void BeeSwarmHole::VUpdate_478320()
{
    if (gMap_507BA8.Rect_Location_Relative_To_Active_Camera_4448C0(&field_14_rect, 0) != CameraPos::eCamCurrent_0)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);
    }

    if (static_cast<int>(gnFrameCount_507670) < field_1C_interval_timer)
    {
        return;
    }

    field_1C_interval_timer = static_cast<int>(gnFrameCount_507670) + field_20_interval;
    
    bool bBallFillingHole = false;
    for (int idx= 0; idx < gBaseGameObject_list_9F2DF0->Size(); idx++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(idx);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == Types::eRollingBall_72)
        {
            BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObjIter);
            PSX_RECT bRect = {};
            pAliveObj->VGetBoundingRect(&bRect, 1);
            if (field_14_rect.x >= bRect.x && field_14_rect.x <= bRect.w)
            {
                if (field_14_rect.y >= bRect.y && field_14_rect.y <= bRect.h)
                {
                    bBallFillingHole = true;
                    break;
                }
            }
        }
    }

    if (!bBallFillingHole)
    {
        const FP speed = FP_FromRaw(field_2A_speed << 8);
        auto pSwarm = ao_new<BeeSwarm>();
        if (pSwarm)
        {
            pSwarm->ctor_47FC60(
                FP_FromInteger(field_14_rect.x),
                FP_FromInteger(field_14_rect.y),
                speed,
                field_26_num_bees,
                field_28_chase_time);
        }

        switch (field_24_movement_type)
        {
        case Path_BeeSwarmHole::MovementType::eHover_0:
            // Idle around like a chav near a victim
            break;

        case Path_BeeSwarmHole::MovementType::eAttack_1:
            pSwarm->Chase_47FEB0(sActiveHero_507678);
            break;

        case Path_BeeSwarmHole::MovementType::eFollowPath_2:
        {
            PathLine* pLine = nullptr;
            FP targetX = {};
            FP targetY = {};
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                FP_FromInteger(field_14_rect.x),
                FP_FromInteger(field_14_rect.y),
                FP_FromInteger(field_14_rect.w),
                FP_FromInteger(field_14_rect.h),
                &pLine,
                &targetX,
                &targetY,
                0x100) == 1)
            {
                pSwarm->FollowLine_47FF10(pLine, targetX, targetY, speed + FP_FromInteger(1));
            }
        }
            break;

        default:
            break;
        }
    }
}

BaseGameObject* BeeSwarmHole::VDestructor(signed int flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
