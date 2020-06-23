#include "stdafx_ao.h"
#include "Collisions.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Psx_common.hpp"

START_NS_AO

ALIVE_VAR(1, 0x504C6C, Collisions*, sCollisions_DArray_504C6C, nullptr);

EXPORT void Collisions::dtor_40CFB0()
{
    ao_delete_free_447540(field_0);
}

Collisions* Collisions::ctor_40CF30(const CollisionInfo* /*pCollisionInfo*/, const BYTE* /*ppPathData*/)
{
    NOT_IMPLEMENTED();
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

END_NS_AO
