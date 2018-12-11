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
#include "Sys.hpp"
#include "Sound.hpp"
#include "Masher.hpp"

ALIVE_VAR(1, 0x5ca208, SoundEntry, sDDV_SoundEntry_5CA208, {});

Masher * CC Masher_Alloc_4EAB80(
    const char* pFileName, 
    Masher_Header** ppMasherHeader, 
    Masher_VideoHeader **ppMasherVideoHeader, 
    Masher_AudioHeader** ppMasherAudioHeader, 
    int* errCode)
{
    Masher* pMasher = reinterpret_cast<Masher*>(malloc_4954D0(sizeof(Masher)));
    if (pMasher)
    {
        *errCode = pMasher->Init_4E6770(pFileName);
        if (*errCode)
        {
            pMasher->dtor_4E6AB0();
            Mem_Free_495540(pMasher);
            return nullptr;
        }
        else
        {
            *ppMasherHeader = &pMasher->field_4_ddv_header;
            *ppMasherVideoHeader = &pMasher->field_14_video_header;
            *ppMasherAudioHeader = &pMasher->field_2C_audio_header;
            return nullptr;
        }
    }
    else
    {
        *errCode = 2;
        return nullptr;
    }
}

EXPORT void CC Masher_DeAlloc_4EAC00(Masher* pMasher)
{
    if (pMasher)
    {
        pMasher->dtor_4E6AB0();
        Mem_Free_495540(pMasher);
    }
}

EXPORT char CC DDV_Play_Impl_4932E0(const char* pMovieName)
{
    NOT_IMPLEMENTED();

    if (!*pMovieName)
    {
        return 1;
    }

    char pFileName[256] = {};
    strcpy(pFileName, sCdEmu_Path1_C14620);
    strcat(pFileName, pMovieName);

    // Replace STR with DDV
    strcpy(strstr(pFileName, ".STR"), ".ddv");

    Masher_Header* pMasher_header_5CA1E4 = nullptr;
    Masher_VideoHeader* pMasher_video_header_5CA204 = nullptr;
    Masher_AudioHeader* pMasher_audio_header_5CA1E0 = nullptr;
    int errCode = 0;

    Masher* pMasherInstance_5CA1EC = Masher_Alloc_4EAB80(
        pFileName,
        &pMasher_header_5CA1E4,
        &pMasher_video_header_5CA204,
        &pMasher_audio_header_5CA1E0,
        &errCode);

    if (errCode)
    {
        strcpy(pFileName, sCdEmu_Path2_C144C0);
        strcat(pFileName, "movies\\");
        strcat(pFileName, pMovieName);

        strcpy(strstr(pFileName, ".STR"), ".ddv");

        pMasherInstance_5CA1EC = Masher_Alloc_4EAB80(
            pFileName,
            &pMasher_header_5CA1E4,
            &pMasher_video_header_5CA204,
            &pMasher_audio_header_5CA1E0,
            &errCode);

        if (errCode)
        {
            strcpy(pFileName, sCdEmu_Path3_C145A0);
            strcat(pFileName, "movies\\");
            strcat(pFileName, pMovieName);
            strcpy(strstr(pFileName, ".STR"), ".ddv");
            /*
            curCdDriveLetter = sCdRomDrives_5CA488[0];
            pFileName[0] = sCdRomDrives_5CA488[0];
            if (sCdRomDrives_5CA488[0])
            {
                pCdDriveIter = sCdRomDrives_5CA488;
                while (1)
                {
                    if (curCdDriveLetter != sCdEmu_Path2_C144C0[0])
                    {
                        pMasherInstance_5CA1EC = Masher_Alloc_4EAB80(
                            pFileName,
                            &pMasher_header_5CA1E4,
                            &pMasher_video_header_5CA204,
                            &pMasher_audio_header_5CA1E0,
                            &errCode);
                        if (!errCode)
                        {
                            break;
                        }
                    }
                    curCdDriveLetter = (pCdDriveIter++)[1];
                    pFileName[0] = curCdDriveLetter;
                    if (!curCdDriveLetter)
                    {
                        goto LABEL_9;
                    }
                }
                curCdDriveLetter = pFileName[0];
            }
            else
            {
            LABEL_9:
                if (errCode)
                {
                    return 0;
                }
            }
            sCdEmu_Path2_C144C0[0] = curCdDriveLetter;
            */
        }
    }

    while (Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN))
    {
        SYS_EventsPump_494580();
    }


    auto bHasAudio_5CA234 = ((unsigned int)pMasher_header_5CA1E4->field_4_contains >> 1) & 1;
    auto gMasher_single_audio_frame_size_5CA240 = pMasher_audio_header_5CA1E0->field_C_single_audio_frame_size;
    auto framesInterleavePlus6 = pMasher_audio_header_5CA1E0->field_10_num_frames_interleave + 6;
    auto bAudioAllocated_5CA1F4 = 0;
    auto sampleLength = gMasher_single_audio_frame_size_5CA240 * framesInterleavePlus6;
    //auto dword_5CA1F8 = sampleLength;

    if (bHasAudio_5CA234 && pMasher_audio_header_5CA1E0->field_0_audio_format)
    {
        SND_New_4EEFF0(
            &sDDV_SoundEntry_5CA208,
            sampleLength,
            pMasher_audio_header_5CA1E0->field_4_samples_per_second,
            (pMasher_audio_header_5CA1E0->field_0_audio_format & 2) != 0 ? 16 : 8,
            pMasher_audio_header_5CA1E0->field_0_audio_format & 1 | 6);
    }
    {
        sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer = nullptr;
        bAudioAllocated_5CA1F4 = 1;
    }
    
    /*
    surfaceDesc_1.dwSize = 108;
    sDD_Surface2_BBC3CC->lpVtbl->GetSurfaceDesc(sDD_Surface2_BBC3CC, &surfaceDesc_1);
    pitch_bytes = surfaceDesc_1.lPitch;
    if (sScreenMode_BD146D == 1)
    {
        v9 = 6;
        pitch_bytes = 2 * surfaceDesc_1.lPitch;
    }
    else
    {
        v9 = 10 - (byte_55EF88 != 0);
    }
    if (sRedShift_C215C4 == 10)
    {
        if (sGreenShift_C1D180 != 5 || sBlueShift_C19140)
        {
            return 1;
        }
        Masher_Tables_Init_4EA880(pitch_bytes, 1, v9);
    }
    else if (sRedShift_C215C4 == 11)
    {
        if (sGreenShift_C1D180 != 6 || sBlueShift_C19140)
        {
            return 1;
        }
        Masher_Tables_Init_4EA880(pitch_bytes, 2, v9);
    }
    else
    {
        if (sRedShift_C215C4)
        {
            return 1;
        }
        if (sGreenShift_C1D180 == 5)
        {
            if (sBlueShift_C19140 != 10)
            {
                return 1;
            }
            Masher_Tables_Init_4EA880(pitch_bytes, 3, v9);
        }
        else
        {
            if (sGreenShift_C1D180 != 6 || sBlueShift_C19140 != 11)
            {
                return 1;
            }
            Masher_Tables_Init_4EA880(pitch_bytes, 4, v9);
        }
    }*/

    /*
    dword_5CA23C = 0;
    LOBYTE(len) = 0;
    if (!DDV_493DF0() || !Masher::sub_4EAC20(pMasherInstance_5CA1EC) || !Masher::sub_4EAC20(pMasherInstance_5CA1EC))
    {
    LABEL_83:
        DD_Flip_4940F0(len);
        goto LABEL_84;
    }
    dword_5CA244 = timeGetTime();
    v23 = dword_5CA23C + 2;
    while (1)
    {
        v10 = dword_5CA23C++ + 1;
        if ((_BYTE)len && v10 >= v23)
        {
            goto do_no_mmx;
        }
        surfaceDesc.dwSize = 108;
        hr = sDD_Surface2_BBC3CC->lpVtbl->Lock(sDD_Surface2_BBC3CC, 0, &surfaceDesc, 1, 0);
        if (hr == DDERR_SURFACELOST)
        {
            if (sDD_Surface2_BBC3CC->lpVtbl->Restore(sDD_Surface2_BBC3CC))
            {
                goto do_no_mmx;
            }
            hr = sDD_Surface2_BBC3CC->lpVtbl->Lock(sDD_Surface2_BBC3CC, 0, &surfaceDesc, 1, 0);
        }
        if (hr)
        {
        do_no_mmx:
            jMasher::Decode_4EAC50(pMasherInstance_5CA1EC);
            goto skip_mmx;
        }
        if (sScreenMode_BD146D == 1 && dword_5CA23C < v23)
        {
            v12 = 240;
            v13 = (char *)surfaceDesc.lpSurface + surfaceDesc.lPitch;
            do
            {
                --v12;
                memset(v13, 0, 0x500u);
                v13 += 2 * surfaceDesc.lPitch;
            } while (v12);
        }
        Masher_MMX_Decode_4EAC40(pMasherInstance_5CA1EC, (int)surfaceDesc.lpSurface);
        sDD_Surface2_BBC3CC->lpVtbl->Unlock(sDD_Surface2_BBC3CC, 0);
    skip_mmx:
        if (bAudioAllocated_5CA1F4)
        {
            goto LABEL_61;
        }
        v14 = (BYTE *)Masher::sub_4EAC60(pMasherInstance_5CA1EC);
        if (v14)
        {
            if (!SND_Reload_4EF1C0(&sDDV_SoundEntry_5CA208, dword_5CA238, v14, gMasher_single_audio_frame_size_5CA240))
            {
                goto LABEL_59;
            }
        }
        else if (!SND_Reload_4EF350(
            &sDDV_SoundEntry_5CA208,
            (unsigned int)dword_5CA238,
            gMasher_single_audio_frame_size_5CA240))
        {
            goto LABEL_59;
        }
        bAudioAllocated_5CA1F4 = 1;
    LABEL_59:
        dword_5CA238 += gMasher_single_audio_frame_size_5CA240;
        if ((signed int)dword_5CA238 >= dword_5CA1F8)
        {
            dword_5CA238 = 0;
        }
    LABEL_61:
        if (dword_5CA23C > 15)
        {
            break;
        }
        Input_IsVKPressed_4EDD40(VK_ESCAPE);
        Input_IsVKPressed_4EDD40(VK_RETURN);
    LABEL_66:
        if (!(_BYTE)len)
        {
            DD_Flip_4F15D0();
        }
        v25 = Masher::sub_4EAC20(pMasherInstance_5CA1EC);
        if (bAudioAllocated_5CA1F4)
        {
            LOBYTE(len) = 0;
            while ((signed int)(timeGetTime() - dword_5CA244) <= 1000
                * dword_5CA23C
                / pMasher_header_5CA1E4->field_8_frame_rate)
            {
                ;
            }
        }
        else
        {
            dword_5CA1F0 += gMasher_single_audio_frame_size_5CA240;
            v15 = SND_Get_Sound_Entry_Pos_4EF620(&sDDV_SoundEntry_5CA208);
            v16 = dword_5CA1FC;
            if ((signed int)(dword_5CA22C - v15) > dword_5CA1F8 / 2)
            {
                v16 = dword_5CA1FC++ + 1;
            }
            dword_5CA22C = v15;
            dword_5CA200 = gMasher_single_audio_frame_size_5CA240
                * pMasher_audio_header_5CA1E0->field_10_num_frames_interleave
                + v15
                + dword_5CA1F8 * v16;
            LOBYTE(len) = dword_5CA1F0 + 2 * gMasher_single_audio_frame_size_5CA240 + gMasher_single_audio_frame_size_5CA240 < dword_5CA200;
            v17 = 0;
            v22 = dword_5CA1F0 + 2 * gMasher_single_audio_frame_size_5CA240 + gMasher_single_audio_frame_size_5CA240 < dword_5CA200;
            v18 = 1000 * dword_5CA23C / pMasher_header_5CA1E4->field_8_frame_rate + 2000;
            if (dword_5CA1F0 >= (unsigned int)dword_5CA200)
            {
                while (1)
                {
                    v19 = SND_Get_Sound_Entry_Pos_4EF620(&sDDV_SoundEntry_5CA208);
                    v20 = v19;
                    len = dword_5CA22C - v19;
                    v21 = dword_5CA1FC;
                    if (len > dword_5CA1F8 / 2)
                    {
                        v21 = dword_5CA1FC++ + 1;
                    }
                    dword_5CA22C = v20;
                    ++v17;
                    dword_5CA200 = gMasher_single_audio_frame_size_5CA240
                        * pMasher_audio_header_5CA1E0->field_10_num_frames_interleave
                        + v20
                        + dword_5CA1F8 * v21;
                    if (v17 > 10000)
                    {
                        v17 = 0;
                        if ((signed int)(timeGetTime() - dword_5CA244) > v18)
                        {
                            break;
                        }
                    }
                    if (dword_5CA1F0 < (unsigned int)dword_5CA200)
                    {
                        LOBYTE(len) = v22;
                        goto LABEL_81;
                    }
                }
                LOBYTE(len) = v22;
                bAudioAllocated_5CA1F4 = 1;
            }
        }
    LABEL_81:
        SYS_EventsPump_494580();
        PSX_VSync_4F6170(0);
        if (!v25)
        {
            DDV_493F30();
            goto LABEL_83;
        }
    }
    */

    /*
    if (!Input_IsVKPressed_4EDD40(VK_ESCAPE) && !Input_IsVKPressed_4EDD40(VK_RETURN))
    {
        goto LABEL_66;
    }
    if (sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer)
    {
        SND_Free_4EFA30(&sDDV_SoundEntry_5CA208);
    }
    DDV_493F30();
    DD_Flip_4F15D0();
    DD_Flip_4940F0(len);
    while (Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN))
    {
        SYS_EventsPump_494580();
    }
LABEL_84:
*/
    
    /*
    if (sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer)
    {
        SND_Free_4EFA30(&sDDV_SoundEntry_5CA208);
        sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer = nullptr;
    }
    */

    Masher_DeAlloc_4EAC00(pMasherInstance_5CA1EC);
    pMasherInstance_5CA1EC = nullptr;
    return 1;
}

EXPORT char CC DDV_Play_493210(const char* pDDVName)
{
    sSoundEntry_5CA230 = &sDDV_SoundEntry_5CA208;
    const char ret = DDV_Play_Impl_4932E0(pDDVName);
    sSoundEntry_5CA230 = nullptr;
    return ret;
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
