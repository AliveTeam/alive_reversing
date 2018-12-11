#include "stdafx.h"
#include "Movie.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "stdlib.hpp"
#include "CameraSwapper.hpp"
#include "ResourceManager.hpp"
#include "Text.hpp"
#include "MainMenu.hpp"
#include "Midi.hpp"

EXPORT char CC DDV_Play_493210(const char* /*pDDVName*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

ALIVE_VAR(1, 0x563a88, short, sMovie_Kill_SEQs_563A88, 1);
ALIVE_VAR(1, 0xbb4ab2, short, word_BB4AB2, 0);
ALIVE_VAR(1, 0xbb4ae4, int, sMovie_ref_count_BB4AE4, 0);
ALIVE_VAR(1, 0x5ca4c4, BYTE, sMovieNameIdx_5CA4C4, 0);

struct MovieName
{
    char mName[64];
};

struct MovieQueue
{
    MovieName mNames[3];
};

ALIVE_VAR(1, 0x5CA348, MovieQueue, sMovieNames_5CA348, {});

void CC Get_fmvs_sectors_494460(const char* pMovieName1, const char* pMovieName2, const char* pMovieName3, DWORD* pMovie1Sector, DWORD* pMovie2Sector, DWORD* pMovie3Sector)
{
    // NOTE: Unused globals that also had the "fake" sector number assigned have been omitted.
    sMovieNameIdx_5CA4C4 = 0;

    if (pMovieName1)
    {
        strcpy(sMovieNames_5CA348.mNames[0].mName, pMovieName1);
        *pMovie1Sector = 0x11111111;
    }

    if (pMovieName2)
    {
        strcpy(sMovieNames_5CA348.mNames[1].mName, pMovieName2);
        *pMovie2Sector = 0x22222222;
    }

    if (pMovieName3)
    {
        strcpy(sMovieNames_5CA348.mNames[2].mName, pMovieName3);
        *pMovie3Sector = 0x33333333;
    }
}

BaseGameObject* Movie::VDestructor(signed int flags)
{
    return vdtor_4DFE80(flags);
}

void Movie::VUpdate()
{
    vUpdate_4E0030();
}

void Movie::VScreenChanged()
{
    // Null sub 0x4E02A0
}

void Movie::Init_4DFF60(int id, CdlLOC* pCdPos, __int16 bUnknown, __int16 flags, __int16 vol)
{
    field_6_flags.Set(BaseGameObject::eBit08);
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);

    field_4_typeId = Types::eMovie_145;

    ++sMovie_ref_count_BB4AE4;

    field_3C = 0;

    if (bUnknown & 1)
    {
        field_20 |= 4; // TODO: Strongly type these flags
    }
    else
    {
        field_20 &= ~4;
    }

    field_38_param_1 = id;
    field_44_cd_loc_min = pCdPos->field_0_minute;
    field_45_cd_loc_sec = pCdPos->field_1_second;
    field_20 &= ~3;
    field_46 = pCdPos->field_2_sector;
    
    sMovie_Kill_SEQs_563A88 = 1;
    
    if (flags & 0x4000)
    {
        sMovie_Kill_SEQs_563A88 = 0;
    }
    word_BB4AB2 = 2;

    field_22_param5 = vol;

    ResourceManager::Reclaim_Memory_49C470(0);
}

Movie* Movie::ctor_4DFDE0(int id, DWORD pos, __int16 a4, __int16 a5, __int16 volume)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x547EF4); // vTbl_Movie_547EF4

    CdlLOC cdLoc = {};
    PSX_Pos_To_CdLoc_4FADD0(pos, &cdLoc);
    Init_4DFF60(id, &cdLoc, a4, a5, volume);
    return this;
}

void Movie::vUpdate_4E0030()
{
    if (sMovie_Kill_SEQs_563A88)
    {
        SND_StopAll_4CB060();
    }

    if (sMovieNameIdx_5CA4C4 >= 0)
    {
        while (!DDV_Play_493210(sMovieNames_5CA348.mNames[sMovieNameIdx_5CA4C4].mName))
        {
            if (word_5C1BA0)
            {
                break;
            }

            if (!Display_Full_Screen_Message_Blocking_465820(sPathData_559660.paths[sLevelId_dword_5CA408].field_1C_unused, 1))
            {
                break;
            }
        }

        if (++sMovieNameIdx_5CA4C4 == _countof(sMovieNames_5CA348.mNames))
        {
            sMovieNameIdx_5CA4C4 = 0;
        }
    }

    DeInit_4E0210();
}

BaseGameObject* Movie::vdtor_4DFE80(signed int flags)
{
    BaseGameObject_dtor_4DBEC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Movie::DeInit_4E0210()
{
    //YuvToRgb_4F8C60(1); // mdec reset
    //sub_4F92A0(0); // Clear a call back thats never used
    //sub_4FB440(); // Frees/resets some sound stuff guarded by always false conditions

    PSX_CdControlB_4FB320(9, 0, 0);
    PSX_VSync_4F6170(2);

    if (word_BB4AB2 & 1)
    {
        ResourceManager::FreeResource_49C330(field_2C_ppRes);
        field_2C_ppRes = nullptr;
    }

    sbLoadingInProgress_5C1B96 = FALSE;
    --sMovie_ref_count_BB4AE4;

    field_6_flags.Set(BaseGameObject::eDead);
}
