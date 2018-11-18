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
    // THIS IMPLEMENTATION IS NOT CORRECT!
    // This is an entirely different method of raycasting.
    // It works, but its not 100% correct.
    // Flying sligs do not spawn and crash the game because of this.
    // But it is enough to get Abe + some physics stuff working.
    // This function should be reversed and implemented using the
    // base game code as reference.

    NOT_IMPLEMENTED();
        
    PathLine * nearestLine = nullptr;
    float nearestCollisionX = 0;
    float nearestCollisionY = 0;
    float nearestDistance = 0.f;
    bool hasCollided = false;
    bool firstCollision = true;

    if (!field_8_item_count)
    {
        *ppLine = nullptr;
        return false;
    }

    for (int i = 0; i < field_8_item_count; i++)
    {
        PathLine * currentLine = &field_0_pArray[i];

        // Game specific
        if (!(((1 << currentLine->field_8_mode) % 32) & modeMask)) // if (!(currentLine->Mode & mode))
            continue;

        bool segments_intersect = false;
        float intersectionX = 0;
        float intersectionY = 0;

        // Converting to fixed point only needed for real game.
        // Alive won't need this
        int line1p1x = FP_GetExponent(X1);
        int line1p1y = FP_GetExponent(Y1);
        int line1p2x = FP_GetExponent(X2);
        int line1p2y = FP_GetExponent(Y2);

        int line2p1x = currentLine->field_0_x1;
        int line2p1y = currentLine->field_2_y1;
        int line2p2x = currentLine->field_4_x2;
        int line2p2y = currentLine->field_6_y2;

        // Get the segments' parameters.
        int dx12 = line1p2x - line1p1x;
        int dy12 = line1p2y - line1p1y;
        int dx34 = line2p2x - line2p1x;
        int dy34 = line2p2y - line2p1y;

        // Solve for t1 and t2
        float denominator = (dy12 * dx34 - dx12 * dy34);
        float t1 = ((line1p1x - line2p1x) * dy34 + (line2p1y - line1p1y) * dx34) / denominator;

        if (isinf(t1))
            continue;

        float t2 = ((line2p1x - line1p1x) * dy12 + (line1p1y - line2p1y) * dx12) / -denominator;

        // Find the point of intersection.
        intersectionX = line1p1x + dx12 * t1;
        intersectionY = line1p1y + dy12 * t1;

        // The segments intersect if t1 and t2 are between 0 and 1.
        hasCollided = ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1));

        if (hasCollided)
        {
            float distance = sqrtf(powf((line1p1x - intersectionX), 2) + powf((line1p1y - intersectionY), 2));

            if (firstCollision || distance < nearestDistance)
            {
                nearestCollisionX = intersectionX;
                nearestCollisionY = intersectionY;
                nearestDistance = distance;
                nearestLine = currentLine;

                firstCollision = false;
            }
        }
    }

    if (nearestLine)
    {
        *hitX = FP_FromDouble(nearestCollisionX);
        *hitY = FP_FromDouble(nearestCollisionY);
        *ppLine = nearestLine;
        return true;
    }

    *ppLine = nullptr;
    return false;
}
