#include "stdafx_ao.h"
#include "Movie.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"

namespace AO {

ALIVE_VAR(1, 0x9F309C, s32, sMovie_ref_count_9F309C, 0);

EXPORT void CC Get_fmvs_sectors_44FEB0(const s8* /*pMovieName1*/, const s8* /*pMovieName2*/, const s8* /*pMovieName3*/, u32* /*pMovie1Sector*/, u32* /*pMovie2Sector*/, u32* /*pMovie3Sector*/)
{
    NOT_IMPLEMENTED();
}

Movie* Movie::ctor_489C90(s32 id, s32 /*pos*/, s8 bUnknown, s32 /*flags*/, s16 volume)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD6E0);


    field_6_flags.Set(Options::eSurviveDeathReset_Bit9);
    field_6_flags.Set(Options::eUpdateDuringCamSwap_Bit10);

    /*
    // TODO: FIX MOI
    PSX_Pos_To_CdLoc_49B340(pos, &loc);

    word_9F3064 = 2;

    sMovie_ref_count_9F309C = v7 + 1;
    LOWORD(v7) = *(_WORD*)&loc.field_0_minute;
    LOBYTE(v7) = loc.field_2_sector;
    field_35 = BYTE1(sMovie_ref_count_9F309C);
    field_36 = sMovie_ref_count_9F309C;
    field_34_min = sMovie_ref_count_9F309C;
    */

    field_28 = id;
    field_4_typeId = Types::eMovie_100;
    field_2C = 0;
    field_10_flags = 4 * (bUnknown & 1) | (field_10_flags & ~7);
    field_3C = 1;

    field_12 = volume;

    ResourceManager::Reclaim_Memory_455660(0);

    return this;
}

BaseGameObject* Movie::VDestructor(s32 flags)
{
    return Vdtor_475FB0(flags);
}

BaseGameObject* Movie::Vdtor_475FB0(s32 flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Movie::VScreenChanged()
{
    // Empty
}

void Movie::VUpdate()
{
    VUpdate_489EA0();
}

void Movie::VUpdate_489EA0()
{
}

}
