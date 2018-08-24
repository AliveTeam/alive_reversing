#include "stdafx.h"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"

void BaseAliveGameObject::VRender(int** pOrderingTable)
{
    Render_424B90(pOrderingTable);
}

void BaseAliveGameObject::Render_424B90(int** /*pOrderingTable*/)
{
    NOT_IMPLEMENTED();
}

int BaseAliveGameObject::Vnull_408F90()
{
    return vnull_408F90();
}

unsigned __int16 BaseAliveGameObject::Vnull_408F70()
{
    return vnull_408F70();
}

__int16 BaseAliveGameObject::Vsub_4081C0(__int16 a2)
{
    return vsub_4081C0(a2);
}

int BaseAliveGameObject::Vsub_408320(__int16 a2, __int16 a3, int a4)
{
    return vsub_408320(a2, a3, a4);
}

__int16 BaseAliveGameObject::Vsub_408730(int arg0)
{
    return vsub_408730(arg0);
}

signed __int16 BaseAliveGameObject::Vnull_4087F0(int a2a)
{
    return vnull_4087F0(a2a);
}

char BaseAliveGameObject::Vsub_408A40(__int16 a2)
{
    return vsub_408A40(a2);
}

int BaseAliveGameObject::Vsub_408FD0(__int16 a2)
{
    return vsub_408FD0(a2);
}

int BaseAliveGameObject::Vnull_4081F0()
{
    return vnull_4081F0();
}

// =======

int BaseAliveGameObject::vnull_408F90()
{
    // Empty
    return 0;
}

unsigned __int16 BaseAliveGameObject::vnull_408F70()
{
    // Empty
    return 0;
}

__int16 BaseAliveGameObject::vsub_4081C0(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int BaseAliveGameObject::vsub_408320(__int16 /*a2*/, __int16 /*a3*/, int /*a4*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 BaseAliveGameObject::vsub_408730(int /*arg0*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

signed __int16 BaseAliveGameObject::vnull_4087F0(int /*a2a*/)
{
    // Empty
    return 0;
}

char BaseAliveGameObject::vsub_408A40(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int BaseAliveGameObject::vsub_408FD0(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int BaseAliveGameObject::vnull_4081F0()
{
    // Empty
    return 0;
}

void BaseAliveGameObject::SetColorLUT_425600(TintEntry * pTintArray, __int16 level_id)
{
    while (pTintArray->field_0_level != level_id)
    {
        if (pTintArray->field_0_level == level_id || pTintArray->field_0_level == -1)
        {
            break;
        }
        pTintArray++;
    }

    field_D0_r = pTintArray->field_1_r;
    field_D2_g = pTintArray->field_2_g;
    field_D4_b = pTintArray->field_3_b;
}
