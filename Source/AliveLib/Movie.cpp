#include "stdafx.h"
#include "Movie.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "stdlib.hpp"
#include "CameraSwapper.hpp"
#include "ResourceManager.hpp"
#include "Text.hpp"
#include "MainMenu.hpp"
#include "Sound/Midi.hpp"
#include "Sys.hpp"
#include "Sound/Sound.hpp"
#include "Masher.hpp"
#include "DDraw.hpp"
#include "VGA.hpp"

ALIVE_VAR(1, 0x5ca208, SoundEntry, sDDV_SoundEntry_5CA208, {});

EXPORT Masher * CC Masher_Alloc_4EAB80(
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
            return pMasher;
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

EXPORT int CC Masher_ReadNextFrame_4EAC20(Masher* pMasher)
{
    return pMasher->sub_4E6B30();
}

EXPORT void CC Masher_MMX_Decode_4EAC40(Masher* pMasher, void* pSurface)
{
    pMasher->MMX_Decode_4E6C60((BYTE*)pSurface);
}

ALIVE_VAR(1, 0x5CA234, bool, bHasAudio_5CA234, false);
ALIVE_VAR(1, 0x5CA238, int, sampleOffsetPos_5CA238, 0);
ALIVE_VAR(1, 0x5CA23C, int, sFrameInterleaveNum_5CA23C, 0);
ALIVE_VAR(1, 0x5CA1E4, Masher_Header*, pMasher_header_5CA1E4, nullptr);
ALIVE_VAR(1, 0x5CA204, Masher_VideoHeader*, pMasher_video_header_5CA204, nullptr);
ALIVE_VAR(1, 0x5CA1E0, Masher_AudioHeader*, pMasher_audio_header_5CA1E0, nullptr);
ALIVE_VAR(1, 0x5CA1F4, bool, bNoAudio_5CA1F4, false);
ALIVE_VAR(1, 0x5CA1EC, Masher*, pMasherInstance_5CA1EC, nullptr);
ALIVE_VAR(1, 0x5CA240, int, gMasher_single_audio_frame_size_5CA240, 0);
ALIVE_VAR(1, 0x5CA1F0, int, total_audio_offset_5CA1F0, 0);
ALIVE_VAR(1, 0x5CA1FC, int, dword_5CA1FC, 0);
ALIVE_VAR(1, 0x5CA22C, int, oldBufferPlayPos_5CA22C, 0);

EXPORT char CC DDV_StartAudio_493DF0()
{
    if (!bHasAudio_5CA234)
    {
        return 1;
    }

    unsigned int audioBufferStartOffset = 0;
    sampleOffsetPos_5CA238 = 0;

    if (sFrameInterleaveNum_5CA23C < pMasher_audio_header_5CA1E0->field_10_num_frames_interleave)
    {
        while (Masher::sub_4EAC30(pMasherInstance_5CA1EC))
        {
            if (!bNoAudio_5CA1F4)
            {
                void* pAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(pMasherInstance_5CA1EC);
                if (SND_Reload_4EF1C0(
                    &sDDV_SoundEntry_5CA208,
                    sampleOffsetPos_5CA238,
                    (unsigned char*)pAudioFrame,
                    gMasher_single_audio_frame_size_5CA240))
                {
                    bNoAudio_5CA1F4 = 1;
                }
            }
            audioBufferStartOffset = sampleOffsetPos_5CA238 + gMasher_single_audio_frame_size_5CA240;
            ++sFrameInterleaveNum_5CA23C;
            sampleOffsetPos_5CA238 += gMasher_single_audio_frame_size_5CA240;
            if (sFrameInterleaveNum_5CA23C >= pMasher_audio_header_5CA1E0->field_10_num_frames_interleave)
            {
                break;
            }
        }
    }

    if (sFrameInterleaveNum_5CA23C >= pMasher_audio_header_5CA1E0->field_10_num_frames_interleave)
    {
        total_audio_offset_5CA1F0 = audioBufferStartOffset;
        if (!bNoAudio_5CA1F4)
        {
            if (FAILED(SND_PlayEx_4EF740(&sDDV_SoundEntry_5CA208, 116, 116, 1.0, 0, 1, 100)))
            {
                bNoAudio_5CA1F4 = 1;
            }
        }
        dword_5CA1FC = 0;
        oldBufferPlayPos_5CA22C = 0;
        return 1;
    }

    return 0;
}

#if USE_SDL2
EXPORT void DDV_493F30()
{
    // Do nothing
}

EXPORT void CC DD_Flip_4940F0()
{
    // Do nothing
}
#else
EXPORT void DDV_493F30()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC DD_Flip_4940F0()
{
    NOT_IMPLEMENTED();
}
#endif

static Masher* Open_DDV(const char* pMovieName)
{
    char pFileName[256] = {};
    strcpy(pFileName, sCdEmu_Path1_C14620);
    strcat(pFileName, pMovieName);

    // Replace STR with DDV
    strcpy(strstr(pFileName, ".STR"), ".DDV");

    int errCode = 0;

    Masher* pMasher = Masher_Alloc_4EAB80(
        pFileName,
        &pMasher_header_5CA1E4,
        &pMasher_video_header_5CA204,
        &pMasher_audio_header_5CA1E0,
        &errCode);

    if (errCode)
    {
#if! _WIN32
        const size_t len = strlen(pFileName);
        for (size_t i=0; i<len; i++)
        {
            pFileName[i] = static_cast<char>(tolower(pFileName[i]));
        }

        pMasher = Masher_Alloc_4EAB80(
                pFileName,
                &pMasher_header_5CA1E4,
                &pMasher_video_header_5CA204,
                &pMasher_audio_header_5CA1E0,
                &errCode);
#endif
    }

    if (errCode)
    {
#if BEHAVIOUR_CHANGE_SUB_DATA_FOLDERS
        // Load movies from a sub folder called "movies"
        strcpy(pFileName, "");
        strcat(pFileName, "movies\\");
        strcat(pFileName, pMovieName);

        strcpy(strstr(pFileName, ".STR"), ".ddv");

        pMasher = Masher_Alloc_4EAB80(
            pFileName,
            &pMasher_header_5CA1E4,
            &pMasher_video_header_5CA204,
            &pMasher_audio_header_5CA1E0,
            &errCode);

        if (!errCode)
        {
            return pMasher;
        }
#endif

        strcpy(pFileName, sCdEmu_Path2_C144C0);
        strcat(pFileName, "movies\\");
        strcat(pFileName, pMovieName);

        strcpy(strstr(pFileName, ".STR"), ".ddv");

        pMasher = Masher_Alloc_4EAB80(
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

            char curCdDriveLetter = sCdRomDrives_5CA488[0];
            if (sCdRomDrives_5CA488[0])
            {
                char* pCdDriveIter = sCdRomDrives_5CA488;
                while (*pCdDriveIter)
                {
                    pFileName[0] = curCdDriveLetter;

                    if (curCdDriveLetter != sCdEmu_Path2_C144C0[0])
                    {
                        pMasher = Masher_Alloc_4EAB80(
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
                }

                if (errCode)
                {
                    return nullptr;
                }
                curCdDriveLetter = pFileName[0];
            }
            else
            {
                if (errCode)
                {
                    return nullptr;
                }
            }
            sCdEmu_Path2_C144C0[0] = curCdDriveLetter;
        }
    }
    return pMasher;
}

static void Render_DDV_Frame(Bitmap& tmpBmp)
{
    // Copy into the emulated vram - when FMV ends the "screen" still have the last video frame "stick"
    // giving us a nice seamless transistion.
    SDL_Rect bufferSize = { 0,0, 640, 240 };
    SDL_BlitScaled(tmpBmp.field_0_pSurface, nullptr, sPsxVram_C1D160.field_0_pSurface, &bufferSize);

    if (sPsxEMU_show_vram_BD1465)
    {
        VGA_CopyToFront_4F3710(&sPsxVram_C1D160, nullptr);
    }
    else
    {
        // Copy to full window/primary buffer
        VGA_CopyToFront_4F3710(&tmpBmp, nullptr);
    }
}

EXPORT char CC DDV_Play_Impl_4932E0(const char* pMovieName)
{
    if (!*pMovieName)
    {
        return 1;
    }

    pMasherInstance_5CA1EC = Open_DDV(pMovieName);
    if (!pMasherInstance_5CA1EC)
    {
        return 0;
    }

    while (Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN))
    {
        SYS_EventsPump_494580();
    }

    bHasAudio_5CA234 = ((unsigned int)pMasher_header_5CA1E4->field_4_contains >> 1) & 1;
    gMasher_single_audio_frame_size_5CA240 = pMasher_audio_header_5CA1E0->field_C_single_audio_frame_size;
    const auto sampleLength = gMasher_single_audio_frame_size_5CA240 * (pMasher_audio_header_5CA1E0->field_10_num_frames_interleave + 6);

    bNoAudio_5CA1F4 = 0;
    if (bHasAudio_5CA234 && pMasher_audio_header_5CA1E0->field_0_audio_format)
    {
        if (SND_New_4EEFF0(
            &sDDV_SoundEntry_5CA208,
            sampleLength,
            pMasher_audio_header_5CA1E0->field_4_samples_per_second,
            (pMasher_audio_header_5CA1E0->field_0_audio_format & 2) != 0 ? 16 : 8,
            (pMasher_audio_header_5CA1E0->field_0_audio_format & 1) | 6) < 0)
        {
            // New fail
            sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer = nullptr;
            bNoAudio_5CA1F4 = 1;
        }
    }
    else
    {
        // Source DDV has no audio
        bNoAudio_5CA1F4 = 1;
    }

    // NOTE: Call to Masher_Tables_Init_4EA880 as the whole masher code for audio has been replaced
    sFrameInterleaveNum_5CA23C = 0;

#if USE_SDL2
    Bitmap tmpBmp = {};
    BMP_New_4F1990(&tmpBmp, 640, 480, 15, 0);
#endif

    if (DDV_StartAudio_493DF0() && Masher_ReadNextFrame_4EAC20(pMasherInstance_5CA1EC) && Masher_ReadNextFrame_4EAC20(pMasherInstance_5CA1EC))
    {
        const int dword_5CA244 = SYS_GetTicks();
        for (;;)
        {
            sFrameInterleaveNum_5CA23C++;

            // Lock the back buffer
#if USE_SDL2
            SDL_LockSurface(tmpBmp.field_0_pSurface);
            Masher_MMX_Decode_4EAC40(pMasherInstance_5CA1EC, tmpBmp.field_0_pSurface->pixels);
            SDL_UnlockSurface(tmpBmp.field_0_pSurface);
#else
            DDSURFACEDESC surfaceDesc = {};
            surfaceDesc.dwSize = sizeof(DDSURFACEDESC);
            HRESULT hr = sDD_surface_backbuffer_BBC3CC->Lock(nullptr, &surfaceDesc, 1, 0);
            if (hr == DDERR_SURFACELOST)
            {
                if (SUCCEEDED(sDD_surface_backbuffer_BBC3CC->Restore()))
                {
                    hr = sDD_surface_backbuffer_BBC3CC->Lock(nullptr, &surfaceDesc, 1, 0);
                }
            }
            // Decompress the frame and "render" it into the back buffer
            Masher_MMX_Decode_4EAC40(pMasherInstance_5CA1EC, FAILED(hr) ? nullptr : surfaceDesc.lpSurface);       
            // Unlock the back buffer

            if (SUCCEEDED(hr))
            {
                sDD_surface_backbuffer_BBC3CC->Unlock(0);
            }
#endif

            if (!bNoAudio_5CA1F4)
            {
                void* pDecompressedAudioFrame = (BYTE *)Masher::GetDecompressedAudioFrame_4EAC60(pMasherInstance_5CA1EC);
                if (pDecompressedAudioFrame)
                {
                    if (SND_Reload_4EF1C0(&sDDV_SoundEntry_5CA208, sampleOffsetPos_5CA238, (unsigned char*)pDecompressedAudioFrame, gMasher_single_audio_frame_size_5CA240) < 0)
                    {
                        // Reload with data fail
                        bNoAudio_5CA1F4 = 1;
                    }
                }
                else if (SND_Reload_4EF350(&sDDV_SoundEntry_5CA208, sampleOffsetPos_5CA238, gMasher_single_audio_frame_size_5CA240) < 0)
                {
                    // Reload with silence fail
                    bNoAudio_5CA1F4 = 1;
                }

                sampleOffsetPos_5CA238 += gMasher_single_audio_frame_size_5CA240;
                if (sampleOffsetPos_5CA238 >= sampleLength)
                {
                    sampleOffsetPos_5CA238 = 0;
                }
            }

            if (sFrameInterleaveNum_5CA23C > 15)
            {
                if (Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN))
                {
                    // User quit video playback
                    if (sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer)
                    {
                        SND_Free_4EFA30(&sDDV_SoundEntry_5CA208);
                    }

                    DDV_493F30();
#if USE_SDL2
                    Render_DDV_Frame(tmpBmp);
#else
                    DD_Flip_4F15D0();
#endif
                    DD_Flip_4940F0();

                    while (Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN))
                    {
                        SYS_EventsPump_494580();
                    }

                    break;
                }
            }
            else
            {
                // TODO: Almost looks like an OG bug but this has some internal side effect
                Input_IsVKPressed_4EDD40(VK_ESCAPE);
                Input_IsVKPressed_4EDD40(VK_RETURN);
            }

#if USE_SDL2
            Render_DDV_Frame(tmpBmp);
#else
            DD_Flip_4F15D0();
#endif

            const int bMoreFrames = Masher_ReadNextFrame_4EAC20(pMasherInstance_5CA1EC); // read audio and video frame
            if (bNoAudio_5CA1F4)
            {
                while ((signed int)(SYS_GetTicks() - dword_5CA244) <= (1000 * sFrameInterleaveNum_5CA23C / pMasher_header_5CA1E4->field_8_frame_rate))
                {
                    // Wait for the amount of time the frame would take to display at the given framerate
                }
            }
            else
            {
                // Sync on where the audio playback is up to
                total_audio_offset_5CA1F0 += gMasher_single_audio_frame_size_5CA240;
                const DWORD soundBufferPlayPos = SND_Get_Sound_Entry_Pos_4EF620(&sDDV_SoundEntry_5CA208);
                if ((signed int)(oldBufferPlayPos_5CA22C - soundBufferPlayPos) > sampleLength / 2)
                {
                     dword_5CA1FC++;
                }

                oldBufferPlayPos_5CA22C = soundBufferPlayPos;

                const int maxWait = 1000 * sFrameInterleaveNum_5CA23C / pMasher_header_5CA1E4->field_8_frame_rate + 2000;
                if (total_audio_offset_5CA1F0 >= 0)
                {
                    int counter = 0;
                    for (;;)
                    {
                        const unsigned int soundPlayingPos = SND_Get_Sound_Entry_Pos_4EF620(&sDDV_SoundEntry_5CA208);
                        const int remainderLen = oldBufferPlayPos_5CA22C - soundPlayingPos;
                        if (remainderLen > sampleLength / 2)
                        {
                            dword_5CA1FC++;
                        }

                        oldBufferPlayPos_5CA22C = soundPlayingPos;
                        
                        ++counter;

                        const int dword_5CA200 = gMasher_single_audio_frame_size_5CA240
                            * pMasher_audio_header_5CA1E0->field_10_num_frames_interleave
                            + soundPlayingPos
                            + sampleLength * dword_5CA1FC;

                        if (counter > 10000)
                        {
                            counter = 0;
                            if ((signed int)(SYS_GetTicks() - dword_5CA244) > maxWait)
                            {
                                // TODO: Unknown failure case
                                bNoAudio_5CA1F4 = 1;
                                break;
                            }
                        }

                        if (total_audio_offset_5CA1F0 < dword_5CA200)
                        {
                            break;
                        }
                    }
                }
            }

            SYS_EventsPump_494580();
            PSX_VSync_4F6170(0);

            if (!bMoreFrames)
            {
                // End of stream
                DDV_493F30();
#if USE_SDL2
                Render_DDV_Frame(tmpBmp);
#else
                DD_Flip_4F15D0();
#endif
                break;
            }
        }
    }
    else
    {
        DD_Flip_4940F0();
    }

    if (sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer)
    {
        SND_Free_4EFA30(&sDDV_SoundEntry_5CA208);
        sDDV_SoundEntry_5CA208.field_4_pDSoundBuffer = nullptr;
    }

    Masher_DeAlloc_4EAC00(pMasherInstance_5CA1EC);
    pMasherInstance_5CA1EC = nullptr;
#if USE_SDL2
    Bmp_Free_4F1950(&tmpBmp);
#endif
    return 1;
}

EXPORT char CC DDV_Play_493210(const char* pDDVName)
{
    sMovieSoundEntry_5CA230 = &sDDV_SoundEntry_5CA208;
    const char ret = DDV_Play_Impl_4932E0(pDDVName);
    sMovieSoundEntry_5CA230 = nullptr;
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
    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap_Bit10);

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

        if (++sMovieNameIdx_5CA4C4 == ALIVE_COUNTOF(sMovieNames_5CA348.mNames))
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

    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}
