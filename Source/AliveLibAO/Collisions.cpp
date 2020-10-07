#include "stdafx_ao.h"
#include "Collisions.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Psx_common.hpp"
#include "PathData.hpp"

START_NS_AO

ALIVE_VAR(1, 0x504C6C, Collisions*, sCollisions_DArray_504C6C, nullptr);

void CC Collisions::Factory_40CEC0(const CollisionInfo* pCollisionInfo, const BYTE* pPathData)
{
    sCollisions_DArray_504C6C = ao_new<Collisions>();
    if (sCollisions_DArray_504C6C)
    {
        sCollisions_DArray_504C6C->ctor_40CF30(pCollisionInfo, pPathData);
    }
}

void Collisions::dtor_40CFB0()
{
    ao_delete_free_447540(field_0_pArray);
}

Collisions* Collisions::ctor_40CF30(const CollisionInfo* pCollisionInfo, const BYTE* ppPathData)
{
    field_8_item_count = pCollisionInfo->field_10_num_collision_items;
    field_4_current_item_count = static_cast<WORD>(pCollisionInfo->field_10_num_collision_items);

    // Up to 20 dynamic collisions, slam doors, trap doors, lift platforms etc. (Half of AEs)
    field_C_max_count = pCollisionInfo->field_10_num_collision_items + 20;

    // Allocate memory for collisions array
    field_0_pArray = reinterpret_cast<PathLine*>(ao_new_malloc_447520(field_C_max_count * sizeof(PathLine)));

    // Copy collision line data out of Path resource
    memcpy(field_0_pArray, &ppPathData[pCollisionInfo->field_C_collision_offset], field_4_current_item_count * sizeof(PathLine));

    // Init dynamic collisions positions to zeros
    for (int i = field_4_current_item_count; i < field_C_max_count; i++)
    {
        field_0_pArray[i].field_0_rect.x = 0;
        field_0_pArray[i].field_0_rect.y = 0;
        field_0_pArray[i].field_0_rect.w = 0;
        field_0_pArray[i].field_0_rect.h = 0;
    }
    return this;
}

PathLine* Collisions::Add_Dynamic_Collision_Line_40C8A0(__int16 /*x1*/, __int16 /*y1*/, __int16 /*x2*/, __int16 /*y2*/, char /*mode*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

signed __int16 Collisions::RayCast_40C410(FP /*X1_16_16*/, FP /*Y1_16_16*/, FP /*X2_16_16*/, FP /*Y2_16_16*/, PathLine** /*ppLine*/, FP* /*hitX*/, FP* /*hitY*/, unsigned int /*modeMask*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

PSX_RECT* CCSTD Rect_Clear_40C920(PSX_RECT* pRect)
{
    pRect->x = 0;
    pRect->w = 0;
    pRect->y = 0;
    pRect->h = 0;
    return pRect;
}

PathLine* PathLine::MoveOnLine_40CA20(FP* /*xpos*/, FP* /*ypos*/, FP /*velX*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

END_NS_AO
