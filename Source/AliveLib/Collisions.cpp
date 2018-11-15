#include "stdafx.h"
#include "Collisions.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"


ALIVE_VAR(1, 0x5C1128, Collisions*, sCollisions_DArray_5C1128, nullptr);

Collisions* Collisions::ctor_418930(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes)
{
    field_8_item_count = pCollisionInfo->field_10_num_collision_items;
    field_4_current_item_count = static_cast<WORD>(pCollisionInfo->field_10_num_collision_items);

    // Up to 40 dynamic collisions, slam doors, trap doors, lift platforms etc.
    field_C_max_count = pCollisionInfo->field_10_num_collision_items + 40;

    // Allocate memory for collisions array
    field_0_pArray = reinterpret_cast<PathLine*>(malloc_non_zero_4954F0(field_C_max_count * sizeof(PathLine)));

    // Copy collision line data out of Path resource
    memcpy(field_0_pArray, &pPathRes[pCollisionInfo->field_C_collision_offset], field_4_current_item_count * sizeof(PathLine));

    // Init dynamic collisions positions to zeros
    for (int i = field_4_current_item_count; i < field_C_max_count; i++)
    {
        field_0_pArray[i].field_0_x1 = 0;
        field_0_pArray[i].field_2_y1 = 0;
        field_0_pArray[i].field_4_x2 = 0;
        field_0_pArray[i].field_6_y2 = 0;
    }
    return this;
}

void Collisions::dtor_4189F0()
{
    Mem_Free_495560(field_0_pArray);
}

void CC Collisions::Factory_4188A0(const CollisionInfo* pCollisionInfo, const BYTE* pPathRes)
{
    sCollisions_DArray_5C1128 = alive_new<Collisions>();
    if (sCollisions_DArray_5C1128)
    {
        sCollisions_DArray_5C1128->ctor_418930(pCollisionInfo, pPathRes);
    }
}

signed __int16 Collisions::Raycast_417A60(FP X1, FP Y1, FP X2, FP Y2, PathLine** ppLine, FP * hitX, FP * hitY, int modeMask)
{
    NOT_IMPLEMENTED();
}
