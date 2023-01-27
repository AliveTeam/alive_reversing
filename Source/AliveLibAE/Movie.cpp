#include "stdafx.h"
#include "Movie.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Psx.hpp"
#include "stdlib.hpp"
#include "Text.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/Sound/Sound.hpp"
#include "../relive_lib/Masher.hpp"
#include "VGA.hpp"
#include "GameAutoPlayer.hpp"
#include "Game.hpp"
#include "../relive_lib/Renderer/IRenderer.hpp"

// Inputs on the controller that can be used for aborting skippable movies
const u32 MOVIE_SKIPPER_GAMEPAD_INPUTS = (InputCommands::eUnPause_OrConfirm | InputCommands::eBack | InputCommands::ePause);

// Tells whether reverb was enabled before starting the FMV
static bool wasReverbEnabled = false;

static SoundEntry sFmvSoundEntry = {};

Masher* Masher_Alloc(
    const char_type* pFileName,
    Masher_Header** ppMasherHeader,
    Masher_VideoHeader** ppMasherVideoHeader,
    Masher_AudioHeader** ppMasherAudioHeader,
    s32* errCode)
{
    Masher* pMasher = relive_new Masher;
    if (pMasher)
    {
        *errCode = pMasher->Init(pFileName);
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

void Masher_DeAlloc(Masher* pMasher)
{
    relive_delete pMasher;
}

s32 Masher_ReadNextFrame(Masher* pMasher)
{
    return pMasher->ReadNextFrame();
}

void Masher_DecodeVideoFrame(Masher* pMasher, RGBA32* pSurface)
{
    pMasher->VideoFrameDecode(pSurface);
}

static bool sHasAudio = false;
static s32 sFmvAudioSampleOffset = 0;
static s32 sFmvNumReadFrames = 0;
static Masher_Header* sMasher_Header = nullptr;
static Masher_VideoHeader* sMasher_VideoHeader = nullptr;
static Masher_AudioHeader* sMasher_AudioHeader = nullptr;
static bool sNoAudioOrAudioError = false;
static Masher* sMasherInstance = nullptr;
static s32 sFmvSingleAudioFrameSizeInSamples = 0;
static s32 sCurrentAudioOffset = 0;
static s32 sFmvNumPlayedAudioFrames = 0;
static s32 sOldBufferPlayPos = 0;

s8 DDV_StartAudio()
{
    if (!sHasAudio)
    {
        return 1;
    }

    #if USE_SDL2_SOUND
    wasReverbEnabled = gReverbEnabled;

    // disable reverb for cutscenes - it gets re-enabled in DeInit
    gReverbEnabled = false;
    #endif

    u32 audioBufferStartOffset = 0;
    sFmvAudioSampleOffset = 0;

    // Keep reading frames till we have >= number of interleaved so that we have 1 full frame
    if (sFmvNumReadFrames < sMasher_AudioHeader->field_10_num_frames_interleave)
    {
        while (Masher::ReadNextFrameToMemory_4EAC30(sMasherInstance))
        {
            if (!sNoAudioOrAudioError)
            {
                void* pDecompressedAudioBuffer = Masher::GetDecompressedAudioFrame_4EAC60(sMasherInstance);
                if (GetSoundAPI().mSND_LoadSamples(
                        &sFmvSoundEntry,
                        sFmvAudioSampleOffset,
                        (u8*) pDecompressedAudioBuffer,
                        sFmvSingleAudioFrameSizeInSamples))
                {
                    sNoAudioOrAudioError = true;
                }
            }

            sFmvAudioSampleOffset += sFmvSingleAudioFrameSizeInSamples;
            audioBufferStartOffset = sFmvAudioSampleOffset;
            sFmvNumReadFrames++;
            
            if (sFmvNumReadFrames >= sMasher_AudioHeader->field_10_num_frames_interleave)
            {
                break;
            }
        }
    }

    if (sFmvNumReadFrames >= sMasher_AudioHeader->field_10_num_frames_interleave)
    {
        // Update the offset to the size of the first demuxed frame
        sCurrentAudioOffset = audioBufferStartOffset;
        if (!sNoAudioOrAudioError)
        {
            // Sound entry is created and populated with 1 frame, play it
            if (FAILED(SND_PlayEx(&sFmvSoundEntry, 116, 116, 1.0, 0, 1, 100)))
            {
                sNoAudioOrAudioError = true;
            }
        }
        sFmvNumPlayedAudioFrames = 0;
        sOldBufferPlayPos = 0;
        return 1;
    }

    return 0;
}

static Masher* Open_DDV(const char_type* pMovieName)
{
    s32 errCode = 0;
    Masher* pMasher = Masher_Alloc(
        pMovieName,
        &sMasher_Header,
        &sMasher_VideoHeader,
        &sMasher_AudioHeader,
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

s8 DDV_Play_Impl(const char_type* pMovieName)
{
    if (!*pMovieName)
    {
        return 1;
    }

    sMasherInstance = Open_DDV(pMovieName);
    if (!sMasherInstance)
    {
        return 0;
    }

    while (AreMovieSkippingInputsHeld())
    {
        SYS_EventsPump();
    }

    sHasAudio = ((u32) sMasher_Header->field_4_contains >> 1) & 1;
    sFmvSingleAudioFrameSizeInSamples = sMasher_AudioHeader->field_C_single_audio_frame_size;
    const auto fmv_sound_entry_size = sFmvSingleAudioFrameSizeInSamples * (sMasher_AudioHeader->field_10_num_frames_interleave + 6);

    sNoAudioOrAudioError = false;
    if (sHasAudio && sMasher_AudioHeader->field_0_audio_format)
    {
        if (GetSoundAPI().mSND_New(
                &sFmvSoundEntry,
                fmv_sound_entry_size,
                sMasher_AudioHeader->field_4_samples_per_second,
                (sMasher_AudioHeader->field_0_audio_format & 2) != 0 ? 16 : 8,
                (sMasher_AudioHeader->field_0_audio_format & 1) | 6)
            < 0)
        {
            // SND_New failed
            sFmvSoundEntry.field_4_pDSoundBuffer = nullptr;
            sNoAudioOrAudioError = true;
        }
    }
    else
    {
        // Source DDV has no audio
        sNoAudioOrAudioError = true;
    }

    // NOTE: Call to Masher_Tables_Init_4EA880 as the whole masher code for audio has been replaced
    sFmvNumReadFrames = 0;

    //Bitmap tmpBmp = {};
    //BMP_New_4F1990(&tmpBmp, 640, 480, 15, 0);

    CamResource fmvFrame;
    fmvFrame.mData.mWidth = 640;
    fmvFrame.mData.mHeight = 240;
    fmvFrame.mData.mPixels = std::make_shared<std::vector<u8>>();
    fmvFrame.mData.mPixels->resize(fmvFrame.mData.mWidth * fmvFrame.mData.mHeight * sizeof(RGBA32));

    Poly_FT4 polyFT4 = {};
    PolyFT4_Init(&polyFT4);
    SetXYWH(&polyFT4, 0, 0, 640, 240);
    polyFT4.mCam = &fmvFrame;

    if (DDV_StartAudio() && Masher_ReadNextFrame(sMasherInstance) && Masher_ReadNextFrame(sMasherInstance))
    {
        const s32 movieStartTimeStamp_5CA244 = SYS_GetTicks();
        for (;;)
        {
            fmvFrame.mUniqueId = {};

            sFmvNumReadFrames++;

            // Lock the back buffer

            // Decode the video frame to the bitmap pixel buffer
            //SDL_LockSurface(tmpBmp.field_0_pSurface);
            Masher_DecodeVideoFrame(sMasherInstance, reinterpret_cast<RGBA32*>(fmvFrame.mData.mPixels->data()));

            //SDL_UnlockSurface(tmpBmp.field_0_pSurface);

            if (!sNoAudioOrAudioError)
            {
                void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(sMasherInstance);
                if (pDecompressedAudioFrame)
                {
                    // Push new samples into the buffer
                    if (GetSoundAPI().mSND_LoadSamples(&sFmvSoundEntry, sFmvAudioSampleOffset, (u8*)pDecompressedAudioFrame, sFmvSingleAudioFrameSizeInSamples) < 0)
                    {
                        // Reload with data fail
                        sNoAudioOrAudioError = true;
                    }
                }
                else
                {
                    if (GetSoundAPI().mSND_Clear(&sFmvSoundEntry, sFmvAudioSampleOffset, sFmvSingleAudioFrameSizeInSamples) < 0)
                    {
                        // Reload with silence on failure or no data
                        sNoAudioOrAudioError = true;
                    }
                }

                sFmvAudioSampleOffset += sFmvSingleAudioFrameSizeInSamples;

                // Loop back to the start of the audio buffer
                if (sFmvAudioSampleOffset >= fmv_sound_entry_size)
                {
                    sFmvAudioSampleOffset = 0;
                }
            }

            // Check for quitting video every 15 frames
            if (sFmvNumReadFrames > 15)
            {
                if (AreMovieSkippingInputsHeld())
                {
                    // User quit video playback
                    if (sFmvSoundEntry.field_4_pDSoundBuffer)
                    {
                        GetSoundAPI().mSND_Free(&sFmvSoundEntry);
                    }

                    Render_DDV_Frame(&polyFT4);

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

            const s32 bMoreFrames = Masher_ReadNextFrame(sMasherInstance); // read audio and video frame
            if (sNoAudioOrAudioError)
            {
                while ((s32)(SYS_GetTicks() - movieStartTimeStamp_5CA244) <= (1000 * sFmvNumReadFrames / sMasher_Header->field_8_frame_rate))
                {
                    // Wait for the amount of time the frame would take to display at the given framerate
                }
            }
            else
            {
                // Sync on where the audio playback is up to
                sCurrentAudioOffset += sFmvSingleAudioFrameSizeInSamples;
                const u32 soundBufferPlayPos = SND_Get_Sound_Entry_Pos_4EF620(&sFmvSoundEntry);
                if ((s32)(sOldBufferPlayPos - soundBufferPlayPos) > fmv_sound_entry_size / 2)
                {
                     sFmvNumPlayedAudioFrames++;
                }

                sOldBufferPlayPos = soundBufferPlayPos;

                const s32 maxAudioSyncTimeWait = 1000 * sFmvNumReadFrames / sMasher_Header->field_8_frame_rate + 2000;
                if (sCurrentAudioOffset >= 0)
                {
                    s32 counter = 0;
                    for (;;)
                    {
                        const u32 fmv_cur_audio_pos = SND_Get_Sound_Entry_Pos_4EF620(&sFmvSoundEntry);
                        const s32 fmv_audio_left_to_play = sOldBufferPlayPos - fmv_cur_audio_pos;
                        if (fmv_audio_left_to_play > fmv_sound_entry_size / 2)
                        {
                            sFmvNumPlayedAudioFrames++;
                        }

                        sOldBufferPlayPos = fmv_cur_audio_pos;
                        
                        counter++;

                        const s32 kTotalAudioToPlay = sFmvSingleAudioFrameSizeInSamples
                            * sMasher_AudioHeader->field_10_num_frames_interleave
                            + fmv_cur_audio_pos
                            + (fmv_sound_entry_size * sFmvNumPlayedAudioFrames);

                        if (counter > 10000)
                        {
                            counter = 0;
                            if ((s32)(SYS_GetTicks() - movieStartTimeStamp_5CA244) > maxAudioSyncTimeWait)
                            {
                                // TODO: Unknown failure case
                                sNoAudioOrAudioError = true;
                                break;
                            }
                        }

                        if (sCurrentAudioOffset < kTotalAudioToPlay)
                        {
                            break;
                        }
                    }
                }
            }

            SYS_EventsPump();
            PSX_VSync(0);

            if (!bMoreFrames)
            {
                // End of stream
                Render_DDV_Frame(&polyFT4);

                break;
            }
        }
    }

    if (sFmvSoundEntry.field_4_pDSoundBuffer)
    {
        GetSoundAPI().mSND_Free(&sFmvSoundEntry);
        sFmvSoundEntry.field_4_pDSoundBuffer = nullptr;
    }

    Masher_DeAlloc(sMasherInstance);
    sMasherInstance = nullptr;
    //Bmp_Free_4F1950(&tmpBmp);

    return 1;
}

s8 DDV_Play(const char_type* pDDVName)
{
    gMovieSoundEntry = &sFmvSoundEntry;
    const s8 ret = DDV_Play_Impl(pDDVName);
    gMovieSoundEntry = nullptr;
    return ret;
}

s16 sMovie_Kill_SEQs_563A88 = 1;
s32 gMovieRefCount = 0;

void Movie::VScreenChanged()
{
    // Null sub 0x4E02A0
}

void Movie::Init()
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eMovie);

    ++gMovieRefCount;

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
        SetDead(true);
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
    PSX_VSync(2);

    --gMovieRefCount;

    #if USE_SDL2_SOUND
    gReverbEnabled = wasReverbEnabled;
    #endif

    SetDead(true);
}

bool AreMovieSkippingInputsHeld()
{
    if (Input().IsJoyStickEnabled())
    {
        // OG bugfix - previously controllers couldn't skip movies
        return (Input_Read_Pad(sCurrentControllerIndex) & MOVIE_SKIPPER_GAMEPAD_INPUTS) != 0;
    }
    else
    {
        return Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN);
    }
}
