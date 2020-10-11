#include "stdafx_ao.h"
#include "Function.hpp"
#include "DeathGas.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Primitives.hpp"

START_NS_AO

struct GasPolys
{
    Poly_G4 polys[2][4][4][2];
};

struct Data_Byte
{
    BYTE data[2][5][5];
};

ALIVE_ARY(1, 0x5008E8, Prim_SetTPage, 2, gGasTPages_5008E8, {});

ALIVE_VAR(1, 0x4FFDE8, GasPolys, prims_dword_4FFDE8, {});

ALIVE_VAR(1, 0x4FFDB0, Data_Byte, sbyte_1_4FFDB0, {});
ALIVE_VAR(1, 0x5008B0, Data_Byte, sbyte_2_5008B0, {});
ALIVE_VAR(1, 0x5009D0, int, gDeathGasCount_5009D0, 0);

DeathGas* DeathGas::ctor_41CF40(__int16 layer, __int16 amount)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BAF08);

    gDeathGasCount_5009D0++;

    field_4_typeId = Types::eDeathFadeOut_80; // wot moment
    gObjList_drawables_504618->Push_Back(this);
    field_6_flags.Set(Options::eDrawable_Bit4);
    field_16_flag = 0;

    Init_SetTPage_495FB0(&gGasTPages_5008E8[0], 0, 1, PSX_getTPage_4965D0(2, 1, 0, 0));
    Init_SetTPage_495FB0(&gGasTPages_5008E8[1], 0, 1, PSX_getTPage_4965D0(2, 1, 0, 0));

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                for (int l = 0; l < 4; l++)
                {
                    Poly_G4* pPoly = &prims_dword_4FFDE8.polys[i][k][l][j];
                    PolyG4_Init_498890(pPoly);
                    Poly_Set_SemiTrans_498A40(&pPoly->mBase.header, TRUE);
                }
            }
        }
    }

    field_18_layer = layer;
    field_10_total = 0;
    field_12_unused = 0;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                sbyte_1_4FFDB0.data[i][j][k] = Math_NextRandom();
                sbyte_2_5008B0.data[i][j][k] = (Math_NextRandom() & 3) + 2;
            }
        }
    }

    field_14_amount = amount;
    return this;
}

BaseGameObject* DeathGas::VDestructor(signed int flags)
{
    return Vdtor_41D740(flags);
}

BaseGameObject* DeathGas::dtor_41D0E0()
{
    SetVTable(this, 0x4BAF08);
    gObjList_drawables_504618->Remove_Item(this);
    gDeathGasCount_5009D0--;
    return dtor_487DF0();
}

void DeathGas::VScreenChanged_41D700()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0() ||
        !sActiveHero_507678)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void DeathGas::VScreenChanged()
{
    VScreenChanged_41D700();
}

void DeathGas::VUpdate_41D150()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!field_16_flag)
    {
        field_10_total += field_14_amount;
        if (field_10_total > 255)
        {
            field_10_total = 255;
        }
    }
}

void DeathGas::VUpdate()
{
    VUpdate_41D150();
}

DeathGas* DeathGas::Vdtor_41D740(signed int flags)
{
    dtor_41D0E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void DeathGas::VRender(int** ppOt)
{
    VRender_41D190(ppOt);
}

void DeathGas::VRender_41D190(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO
