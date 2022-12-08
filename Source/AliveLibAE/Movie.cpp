#include "stdafx.h"
#include "Movie.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "stdlib.hpp"
#include "Text.hpp"
#include "MainMenu.hpp"
#include "Sound/Midi.hpp"
#include "Sys.hpp"
#include "Sound/Sound.hpp"
#include "Masher.hpp"
#include "VGA.hpp"
#include "GameAutoPlayer.hpp"
#include "Game.hpp"
#include "Renderer/IRenderer.hpp"

// Inputs on the controller that can be used for aborting skippable movies
const u32 MOVIE_SKIPPER_GAMEPAD_INPUTS = (InputCommands::Enum::eUnPause_OrConfirm | InputCommands::Enum::eBack | InputCommands::Enum::ePause);

// Tells whether reverb was enabled before starting the FMV
static bool wasReverbEnabled = false;

SoundEntry fmv_sound_entry_5CA208 = {};

Masher* Masher_Alloc_4EAB80(
    const char_type* pFileName,
    Masher_Header** ppMasherHeader,
    Masher_VideoHeader** ppMasherVideoHeader,
    Masher_AudioHeader** ppMasherAudioHeader,
    s32* errCode)
{
    Masher* pMasher = relive_new Masher;
    if (pMasher)
    {
        *errCode = pMasher->Init_4E6770(pFileName);
        if (*errCode)
        {
            relive_delete pMasher;
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

void Masher_DeAlloc_4EAC00(Masher* pMasher)
{
    relive_delete pMasher;
}

s32 Masher_ReadNextFrame_4EAC20(Masher* pMasher)
{
    return pMasher->ReadNextFrame_4E6B30();
}

void Masher_DecodeVideoFrame_4EAC40(Masher* pMasher, RGBA32* pSurface)
{
    pMasher->VideoFrameDecode_4E6C60(pSurface);
}

bool bHasAudio_5CA234 = false;
s32 fmv_audio_sample_offset_5CA238 = 0;
s32 fmv_num_read_frames_5CA23C = 0;
Masher_Header* pMasher_header_5CA1E4 = nullptr;
Masher_VideoHeader* pMasher_video_header_5CA204 = nullptr;
Masher_AudioHeader* pMasher_audio_header_5CA1E0 = nullptr;
bool bNoAudioOrAudioError_5CA1F4 = false;
Masher* pMasherInstance_5CA1EC = nullptr;
s32 fmv_single_audio_frame_size_in_samples_5CA240 = 0;
s32 current_audio_offset_5CA1F0 = 0;
s32 fmv_num_played_audio_frames_5CA1FC = 0;
s32 oldBufferPlayPos_5CA22C = 0;

s8 DDV_StartAudio_493DF0()
{
    if (!bHasAudio_5CA234)
    {
        return 1;
    }

    #if USE_SDL2_SOUND
    wasReverbEnabled = gReverbEnabled;

    // disable reverb for cutscenes - it gets re-enabled in DeInit
    gReverbEnabled = false;
    #endif

    u32 audioBufferStartOffset = 0;
    fmv_audio_sample_offset_5CA238 = 0;

    // Keep reading frames till we have >= number of interleaved so that we have 1 full frame
    if (fmv_num_read_frames_5CA23C < pMasher_audio_header_5CA1E0->field_10_num_frames_interleave)
    {
        while (Masher::ReadNextFrameToMemory_4EAC30(pMasherInstance_5CA1EC))
        {
            if (!bNoAudioOrAudioError_5CA1F4)
            {
                void* pDecompressedAudioBuffer = Masher::GetDecompressedAudioFrame_4EAC60(pMasherInstance_5CA1EC);
                if (GetSoundAPI().SND_LoadSamples(
                        &fmv_sound_entry_5CA208,
                        fmv_audio_sample_offset_5CA238,
                        (u8*) pDecompressedAudioBuffer,
                        fmv_single_audio_frame_size_in_samples_5CA240))
                {
                    bNoAudioOrAudioError_5CA1F4 = 1;
                }
            }

            fmv_audio_sample_offset_5CA238 += fmv_single_audio_frame_size_in_samples_5CA240;
            audioBufferStartOffset = fmv_audio_sample_offset_5CA238;
            fmv_num_read_frames_5CA23C++;
            
            if (fmv_num_read_frames_5CA23C >= pMasher_audio_header_5CA1E0->field_10_num_frames_interleave)
            {
                break;
            }
        }
    }

    if (fmv_num_read_frames_5CA23C >= pMasher_audio_header_5CA1E0->field_10_num_frames_interleave)
    {
        // Update the offset to the size of the first demuxed frame
        current_audio_offset_5CA1F0 = audioBufferStartOffset;
        if (!bNoAudioOrAudioError_5CA1F4)
        {
            // Sound entry is created and populated with 1 frame, play it
            if (FAILED(SND_PlayEx_4EF740(&fmv_sound_entry_5CA208, 116, 116, 1.0, 0, 1, 100)))
            {
                bNoAudioOrAudioError_5CA1F4 = 1;
            }
        }
        fmv_num_played_audio_frames_5CA1FC = 0;
        oldBufferPlayPos_5CA22C = 0;
        return 1;
    }

    return 0;
}

void DDV_Null_493F30()
{
    // Do nothing
}

void DD_Null_Flip_4940F0()
{
    // Do nothing
}

static Masher* Open_DDV(const char_type* pMovieName)
{
    s32 errCode = 0;
    Masher* pMasher = Masher_Alloc_4EAB80(
        pMovieName,
        &pMasher_header_5CA1E4,
        &pMasher_video_header_5CA204,
        &pMasher_audio_header_5CA1E0,
        &errCode);

    if (errCode)
    {
        return nullptr;
    }
    return pMasher;
}

static void Render_DDV_Frame(Poly_FT4* poly)
{
    // Copy into the emulated vram - when FMV ends the "screen" still have the last video frame "stick"
    // giving us a nice seamless transistion.
    //SDL_Rect bufferSize = {0, 0, 640, 240};
    //SDL_BlitScaled(tmpBmp.field_0_pSurface, nullptr, sPsxVram_C1D160.field_0_pSurface, &bufferSize);

    IRenderer::GetRenderer()->Draw(*poly);

    // Copy to full window/primary buffer
    VGA_EndFrame();

    // TODO: PHAT hax just to make FMV do something for now
    IRenderer& renderer = *IRenderer::GetRenderer();

    renderer.StartFrame();
}

s8 DDV_Play_Impl_4932E0(const char_type* pMovieName)
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

    while (AreMovieSkippingInputsHeld())
    {
        SYS_EventsPump();
    }

    bHasAudio_5CA234 = ((u32) pMasher_header_5CA1E4->field_4_contains >> 1) & 1;
    fmv_single_audio_frame_size_in_samples_5CA240 = pMasher_audio_header_5CA1E0->field_C_single_audio_frame_size;
    const auto fmv_sound_entry_size = fmv_single_audio_frame_size_in_samples_5CA240 * (pMasher_audio_header_5CA1E0->field_10_num_frames_interleave + 6);

    bNoAudioOrAudioError_5CA1F4 = 0;
    if (bHasAudio_5CA234 && pMasher_audio_header_5CA1E0->field_0_audio_format)
    {
        if (GetSoundAPI().SND_New(
                &fmv_sound_entry_5CA208,
                fmv_sound_entry_size,
                pMasher_audio_header_5CA1E0->field_4_samples_per_second,
                (pMasher_audio_header_5CA1E0->field_0_audio_format & 2) != 0 ? 16 : 8,
                (pMasher_audio_header_5CA1E0->field_0_audio_format & 1) | 6)
            < 0)
        {
            // SND_New failed
            fmv_sound_entry_5CA208.field_4_pDSoundBuffer = nullptr;
            bNoAudioOrAudioError_5CA1F4 = 1;
        }
    }
    else
    {
        // Source DDV has no audio
        bNoAudioOrAudioError_5CA1F4 = 1;
    }

    // NOTE: Call to Masher_Tables_Init_4EA880 as the whole masher code for audio has been replaced
    fmv_num_read_frames_5CA23C = 0;

#if USE_SDL2
    //Bitmap tmpBmp = {};
    //BMP_New_4F1990(&tmpBmp, 640, 480, 15, 0);
#endif

    CamResource fmvFrame;
    fmvFrame.mData.mWidth = 640;
    fmvFrame.mData.mHeight = 240;
    fmvFrame.mData.mPixels = std::make_shared<std::vector<u8>>();
    fmvFrame.mData.mPixels->resize(fmvFrame.mData.mWidth * fmvFrame.mData.mHeight * sizeof(RGBA32));

    Poly_FT4 polyFT4 = {};
    PolyFT4_Init(&polyFT4);
    SetXYWH(&polyFT4, 0, 0, 640, 240);
    polyFT4.mCam = &fmvFrame;

    if (DDV_StartAudio_493DF0() && Masher_ReadNextFrame_4EAC20(pMasherInstance_5CA1EC) && Masher_ReadNextFrame_4EAC20(pMasherInstance_5CA1EC))
    {
        const s32 movieStartTimeStamp_5CA244 = SYS_GetTicks();
        for (;;)
        {
            fmvFrame.mUniqueId = {};

            fmv_num_read_frames_5CA23C++;

            // Lock the back buffer

            // Decode the video frame to the bitmap pixel buffer
            //SDL_LockSurface(tmpBmp.field_0_pSurface);
            Masher_DecodeVideoFrame_4EAC40(pMasherInstance_5CA1EC, reinterpret_cast<RGBA32*>(fmvFrame.mData.mPixels->data()));

            //SDL_UnlockSurface(tmpBmp.field_0_pSurface);

            if (!bNoAudioOrAudioError_5CA1F4)
            {
                void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(pMasherInstance_5CA1EC);
                if (pDecompressedAudioFrame)
                {
                    // Push new samples into the buffer
                    if (GetSoundAPI().SND_LoadSamples(&fmv_sound_entry_5CA208, fmv_audio_sample_offset_5CA238, (u8*)pDecompressedAudioFrame, fmv_single_audio_frame_size_in_samples_5CA240) < 0)
                    {
                        // Reload with data fail
                        bNoAudioOrAudioError_5CA1F4 = 1;
                    }
                }
                else
                {
                    if (GetSoundAPI().SND_Clear(&fmv_sound_entry_5CA208, fmv_audio_sample_offset_5CA238, fmv_single_audio_frame_size_in_samples_5CA240) < 0)
                    {
                        // Reload with silence on failure or no data
                        bNoAudioOrAudioError_5CA1F4 = 1;
                    }
                }

                fmv_audio_sample_offset_5CA238 += fmv_single_audio_frame_size_in_samples_5CA240;

                // Loop back to the start of the audio buffer
                if (fmv_audio_sample_offset_5CA238 >= fmv_sound_entry_size)
                {
                    fmv_audio_sample_offset_5CA238 = 0;
                }
            }

            // Check for quitting video every 15 frames
            if (fmv_num_read_frames_5CA23C > 15)
            {
                if (AreMovieSkippingInputsHeld())
                {
                    // User quit video playback
                    if (fmv_sound_entry_5CA208.field_4_pDSoundBuffer)
                    {
                        GetSoundAPI().SND_Free(&fmv_sound_entry_5CA208);
                    }

                    DDV_Null_493F30();

                    Render_DDV_Frame(&polyFT4);

                    DD_Null_Flip_4940F0();

                    while (AreMovieSkippingInputsHeld())
                    {
                        SYS_EventsPump();
                    }

                    break;
                }
            }
            else
            {
                // This clears the pressed state to avoid the above check stopping the FMV too early.
                // E.g user presses return before FMV starts, then after 15 frames it would quit without this call clearing the pressed flag.
                Input_IsVKPressed_4EDD40(VK_ESCAPE);
                Input_IsVKPressed_4EDD40(VK_RETURN);
            }

            Render_DDV_Frame(&polyFT4);

            const s32 bMoreFrames = Masher_ReadNextFrame_4EAC20(pMasherInstance_5CA1EC); // read audio and video frame
            if (bNoAudioOrAudioError_5CA1F4)
            {
                while ((s32)(SYS_GetTicks() - movieStartTimeStamp_5CA244) <= (1000 * fmv_num_read_frames_5CA23C / pMasher_header_5CA1E4->field_8_frame_rate))
                {
                    // Wait for the amount of time the frame would take to display at the given framerate
                }
            }
            else
            {
                // Sync on where the audio playback is up to
                current_audio_offset_5CA1F0 += fmv_single_audio_frame_size_in_samples_5CA240;
                const u32 soundBufferPlayPos = SND_Get_Sound_Entry_Pos_4EF620(&fmv_sound_entry_5CA208);
                if ((s32)(oldBufferPlayPos_5CA22C - soundBufferPlayPos) > fmv_sound_entry_size / 2)
                {
                     fmv_num_played_audio_frames_5CA1FC++;
                }

                oldBufferPlayPos_5CA22C = soundBufferPlayPos;

                const s32 maxAudioSyncTimeWait = 1000 * fmv_num_read_frames_5CA23C / pMasher_header_5CA1E4->field_8_frame_rate + 2000;
                if (current_audio_offset_5CA1F0 >= 0)
                {
                    s32 counter = 0;
                    for (;;)
                    {
                        const u32 fmv_cur_audio_pos = SND_Get_Sound_Entry_Pos_4EF620(&fmv_sound_entry_5CA208);
                        const s32 fmv_audio_left_to_play = oldBufferPlayPos_5CA22C - fmv_cur_audio_pos;
                        if (fmv_audio_left_to_play > fmv_sound_entry_size / 2)
                        {
                            fmv_num_played_audio_frames_5CA1FC++;
                        }

                        oldBufferPlayPos_5CA22C = fmv_cur_audio_pos;
                        
                        counter++;

                        const s32 kTotalAudioToPlay = fmv_single_audio_frame_size_in_samples_5CA240
                            * pMasher_audio_header_5CA1E0->field_10_num_frames_interleave
                            + fmv_cur_audio_pos
                            + (fmv_sound_entry_size * fmv_num_played_audio_frames_5CA1FC);

                        if (counter > 10000)
                        {
                            counter = 0;
                            if ((s32)(SYS_GetTicks() - movieStartTimeStamp_5CA244) > maxAudioSyncTimeWait)
                            {
                                // TODO: Unknown failure case
                                bNoAudioOrAudioError_5CA1F4 = 1;
                                break;
                            }
                        }

                        if (current_audio_offset_5CA1F0 < kTotalAudioToPlay)
                        {
                            break;
                        }
                    }
                }
            }

            SYS_EventsPump();
            PSX_VSync_4F6170(0);

            if (!bMoreFrames)
            {
                // End of stream
                DDV_Null_493F30();

                Render_DDV_Frame(&polyFT4);

                break;
            }
        }
    }
    else
    {
        DD_Null_Flip_4940F0();
    }

    if (fmv_sound_entry_5CA208.field_4_pDSoundBuffer)
    {
        GetSoundAPI().SND_Free(&fmv_sound_entry_5CA208);
        fmv_sound_entry_5CA208.field_4_pDSoundBuffer = nullptr;
    }

    Masher_DeAlloc_4EAC00(pMasherInstance_5CA1EC);
    pMasherInstance_5CA1EC = nullptr;
    //Bmp_Free_4F1950(&tmpBmp);

    return 1;
}

s8 DDV_Play(const char_type* pDDVName)
{
    sMovieSoundEntry_5CA230 = &fmv_sound_entry_5CA208;
    const s8 ret = DDV_Play_Impl_4932E0(pDDVName);
    sMovieSoundEntry_5CA230 = nullptr;
    return ret;
}

s16 sMovie_Kill_SEQs_563A88 = 1;
s32 sMovie_ref_count_BB4AE4 = 0;

void Movie::VScreenChanged()
{
    // Null sub 0x4E02A0
}

void Movie::Init()
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eMovie);

    ++sMovie_ref_count_BB4AE4;

    sMovie_Kill_SEQs_563A88 = 1;

    /*
    // TODO: never seemed to be used, compare with psx to check
    if (flags & 0x4000)
    {
        sMovie_Kill_SEQs_563A88 = 0;
    }
    */
}

Movie::Movie(const char_type* pName)
    : BaseGameObject(true, 0)
    , mName(pName)
{
    Init();
}

void Movie::VUpdate()
{
    if (GetGameAutoPlayer().IsPlaying() || GetGameAutoPlayer().IsRecording())
    {
        // Skip FMVs in rec/playback mode
        mBaseGameObjectFlags.Set(BaseGameObject::Options::eDead);
    }
    else
    {
        if (sMovie_Kill_SEQs_563A88)
        {
            SND_StopAll();
        }

        while (!DDV_Play(mName))
        {
            if (gAttract)
            {
                break;
            }

            if (!Display_Full_Screen_Message_Blocking(Path_Get_Unknown(MapWrapper::FromAE(static_cast<LevelIds>(sLevelId_dword_5CA408))), MessageType::eSkipMovie_1))
            {
                break;
            }
        }
    }
    DeInit();
}


void Movie::DeInit()
{
    PSX_VSync_4F6170(2);

    --sMovie_ref_count_BB4AE4;

    #if USE_SDL2_SOUND
    gReverbEnabled = wasReverbEnabled;
    #endif

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

bool AreMovieSkippingInputsHeld()
{
    if (sJoystickEnabled)
    {
        // OG bugfix - previously controllers couldn't skip movies
        return (Input_Read_Pad_4FA9C0(sCurrentControllerIndex) & MOVIE_SKIPPER_GAMEPAD_INPUTS) != 0;
    }
    else
    {
        return Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN);
    }
}
