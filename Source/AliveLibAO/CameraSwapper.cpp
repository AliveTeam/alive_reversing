#include "stdafx_ao.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "PsxDisplay.hpp"
#include "Movie.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Psx.hpp"
#include "ScreenClipper.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507668, short, sNumCamSwappers_507668, 0);

CameraSwapper* CameraSwapper::ctor_48C540(BYTE** ppBits, int movieId, int movieFlag, char movieFlags, __int16 flags, __int16 volume)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);
    
    Init_48C830(ppBits, CameraSwapEffects::eEffect5_1_FMV);
    
    PSX_ResetCallBack_49AFB0();

    auto pMovie = ao_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_489C90(movieFlag, movieId, movieFlags, flags, volume);
    }

    field_3C_movie_bPutDispEnv = flags;
    return this;
}

CameraSwapper* CameraSwapper::ctor_48C5E0(BYTE** ppBits, int moviePos1, int movieId1, int moviePos2, int movieFlag1, char movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlag2_1, __int16 movieFlags2_1, __int16 movieVol2)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);

    Init_48C830(ppBits, CameraSwapEffects::eEffect9_2_FMV);
    
    PSX_ResetCallBack_49AFB0();

    auto pMovie = ao_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_489C90(movieId1, moviePos1, movieFlags1, movieVol1, movieFlag2);
    }

    field_14_movie_id_3 = movieFlag1;
    field_10_movie_pos_3 = moviePos2;
    field_34_movie_vol_3 = movieVol2;
    field_32_movie_flags_3 = movieFlags2_1;
    field_30_movie_flag_3 = movieFlag2_1;

    field_3C_movie_bPutDispEnv = movieVol1;

    return this;
}

EXPORT CameraSwapper* CameraSwapper::ctor_48C6B0(BYTE** ppBits, int moviePos1, int movieIds1, int moviePos2, int movieId2, int moviePos3, int movieId3, char movieFlag1, __int16 movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlags2, __int16 movieVol2, __int16 movieFlag3, __int16 movieFlags3, __int16 movieVol3)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);

    Init_48C830(ppBits, CameraSwapEffects::eEffect10_3_FMV);

    PSX_ResetCallBack_49AFB0();
    auto pMovie = ao_new<Movie>();
    if (pMovie)
    {
        pMovie->ctor_489C90(movieIds1, moviePos1, movieFlag1, movieFlags1, movieVol1);
    }

    field_18_movie_pos_2 = moviePos2;
    field_1C_movie_id_2 = movieId2;
    field_38_movie_flags_2 = movieFlags2;
    field_36_movie_flag_2 = movieFlag2;
    field_3A_movie_vol_2 = movieVol2;

    field_14_movie_id_3 = movieId3;
    field_10_movie_pos_3 = moviePos3;
    field_30_movie_flag_3 = movieFlag3;
    field_34_movie_vol_3 = movieVol3;
    field_32_movie_flags_3 = movieFlags3;

    field_3C_movie_bPutDispEnv = movieFlags1;

    return this;
}

BaseGameObject* CameraSwapper::VDestructor(signed int flags)
{
    return Vdtor_48D380(flags);
}


void CameraSwapper::VScreenChanged()
{
    // Empty
}

CameraSwapper* CameraSwapper::Vdtor_48D380(signed int flags)
{
    dtor_48CE00();

    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

CameraSwapper* CameraSwapper::ctor_48C7A0(BYTE** ppBits, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BD7C8);

    field_40_ypos_converted = ypos;
    field_3E_xpos_converted = PsxToPCX(xpos);
    Init_48C830(ppBits, changeEffect);

    return this;
}


void CameraSwapper::Init_48C830(BYTE** /*ppCamRes*/, CameraSwapEffects /*changeEffect*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* CameraSwapper::dtor_48CE00()
{
    SetVTable(this, 0x4BD7C8);

    sNumCamSwappers_507668--;

    if (field_24_pSubObject)
    {
        field_24_pSubObject->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (sMap_bDoPurpleLightEffect_507C9C)
    {
        gMap_507BA8.RemoveObjectsWithPurpleLight_4440D0(0);
        sMap_bDoPurpleLightEffect_507C9C = FALSE;
    }

    BackgroundMusic::Play_4762B0();
    MusicController::EnableMusic_443900(1);
    gMap_507BA8.Start_Sounds_For_Objects_In_Near_Cameras_4467D0();

    return dtor_487DF0();
}

void CameraSwapper::VUpdate()
{
    VUpdate_48CEA0();
}

void CameraSwapper::VUpdate_48CEA0()
{
    NOT_IMPLEMENTED();
}

END_NS_AO
