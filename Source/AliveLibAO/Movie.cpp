#include "stdafx_ao.h"
#include "Movie.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "PSXADPCMDecoder.h"
#include "PSXMDECDecoder.h"
#include "Masher.hpp"
#include "../AliveLibAE/Io.hpp"
#include "bmp.hpp"
#include "Sys.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibAE/Input.hpp"

#define NO_WAVE
#include "Sound/Sound.hpp"

namespace AO {

ALIVE_VAR(1, 0x9F309C, int, sMovie_ref_count_9F309C, 0);

struct MovieName
{
    char mName[64];
};

struct MovieQueue
{
    MovieName mNames[3];
};

ALIVE_VAR(1, 0x508B08, MovieQueue, sMovieNames_508B08, {});
ALIVE_VAR(1, 0x508C0C, BYTE, sMovieNameIdx_508C0C, 0);

SoundEntry fmv_sound_entry;

const int kSingleAudioFrameSize = 2016*2;  // todo: nuke
const int kNumFramesInterleave = 5; // todo: nuke

const int kXaFrameDataSize = 2016;
const int kNumAudioChannels = 2;
const int kBytesPerSample = 2;
class PsxStr
{
public:


    std::vector<unsigned char> mDemuxBuffer;
    PSXMDECDecoder mMdec;
    PSXADPCMDecoder mAdpcm;
    void* mFile = nullptr;
    std::vector<u8> pixelBuffer;
    int mFrameW = 0;
    int mFrameH = 0;

    int mSoundPos = 0; // 2016*20
    bool mSoundPlaying = false;
    std::array<s16, (kXaFrameDataSize* kNumAudioChannels* kBytesPerSample) / 2> outPtr;

    bool DecodeAudioAndVideo()
    {
        if (mDemuxBuffer.empty())
        {
            mDemuxBuffer.resize(1024 * 1024);
        }

        //for (;;)
        {
            PsxStrHeader w;
            if (!GetMovieIO().mIO_Read(mFile, &w, sizeof(w)))
            {
                // EOF
                return false;
            }

            // PC sector must start with "MOIR" if video, else starts with "VALE" if audio
            const auto kMoir = 0x52494f4d;
            const auto kVale = 0x454c4156; // VALE

          
            const auto kAkik = 0x4b494b41;  // AKIK
          
            if (w.mSectorType == kMoir)
            {
                if (w.mAkikMagic != kAkik)
                {
                    abort();
                }

                const u16 frameW = w.mWidth;
                const u16 frameH = w.mHeight;

                uint32_t bytes_to_copy = w.mFrameDataLen - w.mSectorNumberInFrame * kXaFrameDataSize;
                if (bytes_to_copy > 0)
                {
                    if (bytes_to_copy > kXaFrameDataSize)
                    {
                        bytes_to_copy = kXaFrameDataSize;
                    }

                    memcpy(mDemuxBuffer.data() + w.mSectorNumberInFrame * kXaFrameDataSize, w.frame, bytes_to_copy);
                }

                if (w.mSectorNumberInFrame == w.mNumSectorsInFrame - 1)
                {
                    // Always resize as its possible for a stream to change its frame size to be smaller or larger
                    // this happens in the AE PSX MI.MOV streams
                    pixelBuffer.resize(frameW * frameH * 4); // 4 bytes per pixel

                    mFrameW = frameW;
                    mFrameH = frameH;

                    mMdec.DecodeFrameToABGR32((uint16_t*)pixelBuffer.data(), (uint16_t*)mDemuxBuffer.data(), frameW, frameH);
                    //mVideoBuffer.push_back(Frame{ mFrameCounter++, frameW, frameH, pixelBuffer });
                    return true;
                }
            }
            else if (w.mSectorType == kVale)
            {
                mAdpcm.DecodeFrameToPCM(outPtr, (uint8_t*)&w.mAkikMagic);
                return true;
            }
            else
            {
                abort();
            }
        }
        return true;
    }

#pragma pack(push)
#pragma pack(1)
    struct PsxVideoFrameHeader
    {
        unsigned short int mNumMdecCodes;
        unsigned short int m3800Magic;
        unsigned short int mQuantizationLevel;
        unsigned short int mVersion;
    };

    struct PsxStrHeader
    {
        // these 2 make up the 8 byte subheader?
        unsigned int mSectorType; // AKIK
        unsigned int mSectorNumber;

        // The 4 "unknown" / 0x80010160 in psx data is replaced by "AKIK" in PC data
        unsigned int mAkikMagic;

        unsigned short int mSectorNumberInFrame;
        unsigned short int mNumSectorsInFrame;
        unsigned int mFrameNum;
        unsigned int mFrameDataLen;
        unsigned short int mWidth;
        unsigned short int mHeight;

        PsxVideoFrameHeader mVideoFrameHeader;
        unsigned int mNulls;

        unsigned char frame[2296];
    };
#pragma pack(pop)

};

EXPORT void CC Get_fmvs_sectors_44FEB0(const char* pMovieName1, const char* pMovieName2, const char* pMovieName3, DWORD* pMovie1Sector, DWORD* pMovie2Sector, DWORD* pMovie3Sector)
{
    AE_IMPLEMENTED();

    sMovieNameIdx_508C0C = 0;

    if (pMovieName1)
    {
        strcpy(sMovieNames_508B08.mNames[0].mName, pMovieName1);
        *pMovie1Sector = 0x11111111;
    }

    if (pMovieName2)
    {
        strcpy(sMovieNames_508B08.mNames[1].mName, pMovieName2);
        *pMovie2Sector = 0x22222222;
    }

    if (pMovieName3)
    {
        strcpy(sMovieNames_508B08.mNames[2].mName, pMovieName3);
        *pMovie3Sector = 0x33333333;
    }
}

Movie* Movie::ctor_489C90(int id, int /*pos*/, char bUnknown, int /*flags*/, __int16 volume)
{
    AE_IMPLEMENTED();


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
    field_10_flags = 4 * (bUnknown & 1) | field_10_flags & ~7;
    field_3C = 1;

    field_12 = volume;

    ResourceManager::Reclaim_Memory_455660(0);

    IO_Init_494230(); // Set up IO funcs

    return this;
}

BaseGameObject* Movie::VDestructor(signed int flags)
{
    return Vdtor_475FB0(flags);
}

BaseGameObject* Movie::Vdtor_475FB0(signed int flags)
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


static void Render_Str_Frame(Bitmap& tmpBmp)
{
    // Copy into the emulated vram - when FMV ends the "screen" still have the last video frame "stick"
    // giving us a nice seamless transistion.
    SDL_Rect bufferSize = { 0,0, 640, 240 };
    SDL_BlitScaled(tmpBmp.field_0_pSurface, nullptr, GetPsxVram().field_0_pSurface, &bufferSize);

    //if (sPsxEMU_show_vram_BD1465)
    {
     //   VGA_CopyToFront_4F3710(&sPsxVram_C1D160, nullptr);
    }
   // else
    {
        // Copy to full window/primary buffer
        VGA_CopyToFront_4F3710(&tmpBmp, nullptr);
    }
}

class TempSurface
{
public:
    bool InitIf(int width, int height)
    {
        if (mFrameW != width || mFrameH != height)
        {
            FreeSurface();

            mFrameW = width;
            mFrameH = height;

            Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
#else
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;
#endif

            wholeImage = SDL_CreateRGBSurface(0, 320, 192, 32, rmask, gmask, bmask, amask);
            if (!wholeImage)
            {
                LOG_ERROR("CreateRGBSurface failed: " << SDL_GetError());
                ALIVE_FATAL("CreateRGBSurface failed");
            }
            return true;
        }
        return false;
    }

    void BlitScaledTo(SDL_Surface* pDst)
    {
        SDL_BlitScaled(wholeImage, nullptr, pDst, nullptr);
        //SDL_SaveBMP(wholeImage, "test.bmp");
    }

    void SetPixels(const std::vector<u8>& pixels)
    {
        SDL_LockSurface(wholeImage);
        memcpy(wholeImage->pixels, pixels.data(), mFrameW * mFrameH * 4);
        SDL_UnlockSurface(wholeImage);
    }

    ~TempSurface()
    {
        FreeSurface();
    }

private:
    void FreeSurface()
    {
        if (wholeImage)
        {
            SDL_FreeSurface(wholeImage);
            wholeImage = nullptr;
        }
    }

    int mFrameW = 0;
    int mFrameH = 0;
    SDL_Surface* wholeImage = nullptr;
};

void Movie::VUpdate_489EA0()
{
    AE_IMPLEMENTED();

    void* hMovieFile = GetMovieIO().mIO_Open(sMovieNames_508B08.mNames[sMovieNameIdx_508C0C].mName);
    if (hMovieFile)
    {
        PsxStr psxStr;
        psxStr.mFile = hMovieFile;

        int bNoAudioOrAudioError = 0;
        const int num_frames_interleave = 1; // maybe 20 ??
        const int fmv_single_audio_frame_size_in_samples = 2016;
        const auto fmv_sound_entry_size = fmv_single_audio_frame_size_in_samples * (num_frames_interleave + 6);
        const int kSamplesPerSecond = 37800;
        int fmv_audio_sample_offset = 0;
        bool bStartedPlayingSound = false;
        const int kFmvFrameRate = 15; // TODO: or is it 30 ?
        int fmv_num_played_audio_frames = 0;
        int current_audio_offset = 0;
        int oldBufferPlayPos = 0;

        if (GetSoundAPI().SND_New(
            &fmv_sound_entry,
            fmv_sound_entry_size,
            kSamplesPerSecond,
            16,
            1) < 0)
        {
            // SND_New failed
            fmv_sound_entry.field_4_pDSoundBuffer = nullptr;
            bNoAudioOrAudioError = 1;
        }

        const auto movieStartTimeStamp = SYS_GetTicks();
        int fmv_num_read_frames = 0;

        TempSurface tempSurface;
        Bitmap tmpBmp = {};

        // Till EOF decoding loop
        while (psxStr.DecodeAudioAndVideo())
        {
            fmv_num_read_frames++;

            //
            // Video frame logic
            //
            if (!psxStr.pixelBuffer.empty())
            {
                // Create if it hasnt or recreate if w/h changed
                if (tempSurface.InitIf(psxStr.mFrameW, psxStr.mFrameH))
                {
                    // Ditto
                    if (tmpBmp.field_0_pSurface)
                    {
                        Bmp_Free_4F1950(&tmpBmp);
                        tmpBmp = {};
                    }
                    BMP_New_4F1990(&tmpBmp, psxStr.mFrameW, psxStr.mFrameH, 15, 0);
                }

                // Copy decoded frame to tempSurface
                tempSurface.SetPixels(psxStr.pixelBuffer);

                // Copy temp surface to tmpBmp (colour depth conversion)
                tempSurface.BlitScaledTo(tmpBmp.field_0_pSurface);
            }

            // 
            // Audio frame logic
            //
            if (!bNoAudioOrAudioError)
            {
                // Push new samples into the buffer
                if (GetSoundAPI().SND_LoadSamples(&fmv_sound_entry, fmv_audio_sample_offset, (unsigned char*)psxStr.outPtr.data(), fmv_single_audio_frame_size_in_samples) < 0)
                {
                    // Reload with data fail
                    bNoAudioOrAudioError = 1;
                }

                // If this is the first time then start to play the buffer
                if (!bStartedPlayingSound && !bNoAudioOrAudioError)
                {
                    bStartedPlayingSound = true;
                    if (FAILED(GetSoundAPI().SND_PlayEx(&fmv_sound_entry, 116, 116, 1.0, 0, 1, 100)))
                    {
                        bNoAudioOrAudioError = 1;
                    }

                    // TODO: Need to set current_audio_offset here ??
                    oldBufferPlayPos = 0;
                }

                fmv_audio_sample_offset += fmv_single_audio_frame_size_in_samples;

                // Loop back to the start of the audio buffer
                if (fmv_audio_sample_offset >= fmv_sound_entry_size)
                {
                    fmv_audio_sample_offset = 0;
                }
            }

            // Check for quitting video every 15 frames
            if (fmv_num_read_frames > 15)
            {
                // TODO: Not complete
                if (Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN))
                {
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

            Render_Str_Frame(tmpBmp);

            if (bNoAudioOrAudioError)
            {
                while ((signed int)(SYS_GetTicks() - movieStartTimeStamp) <= (1000 * fmv_num_read_frames / kFmvFrameRate))
                {
                    // Wait for the amount of time the frame would take to display at the given framerate
                }
            }
            else
            {
                // Sync on where the audio playback is up to
                current_audio_offset += fmv_single_audio_frame_size_in_samples;
                const DWORD soundBufferPlayPos = SND_Get_Sound_Entry_Pos_4EF620(&fmv_sound_entry);
                if ((signed int)(oldBufferPlayPos - soundBufferPlayPos) > fmv_sound_entry_size / 2)
                {
                    fmv_num_played_audio_frames++;
                }

                oldBufferPlayPos = soundBufferPlayPos;

                const int maxAudioSyncTimeWait = 1000 * fmv_num_read_frames / kFmvFrameRate + 2000;
                if (current_audio_offset >= 0)
                {
                    int counter = 0;
                    for (;;)
                    {
                        const unsigned int fmv_cur_audio_pos = SND_Get_Sound_Entry_Pos_4EF620(&fmv_sound_entry);
                        const int fmv_audio_left_to_play = oldBufferPlayPos - fmv_cur_audio_pos;
                        if (fmv_audio_left_to_play > fmv_sound_entry_size / 2)
                        {
                            fmv_num_played_audio_frames++;
                        }

                        oldBufferPlayPos = fmv_cur_audio_pos;

                        counter++;

                        const int kTotalAudioToPlay = fmv_single_audio_frame_size_in_samples
                            * num_frames_interleave
                            + fmv_cur_audio_pos
                            + (fmv_sound_entry_size * fmv_num_played_audio_frames);

                        if (counter > 10000)
                        {
                            counter = 0;
                            if ((signed int)(SYS_GetTicks() - movieStartTimeStamp) > maxAudioSyncTimeWait)
                            {
                                // TODO: Unknown failure case
                                bNoAudioOrAudioError = 1;
                                break;
                            }
                        }

                        if (current_audio_offset < kTotalAudioToPlay)
                        {
                            break;
                        }
                    }
                }
            }

            SYS_EventsPump_44FF90();
            PSX_VSync_496620(0);
        }

        if (fmv_sound_entry.field_4_pDSoundBuffer)
        {
            GetSoundAPI().SND_Free(&fmv_sound_entry);
            fmv_sound_entry.field_4_pDSoundBuffer = nullptr;
        }

        Bmp_Free_4F1950(&tmpBmp);

        GetMovieIO().mIO_Close(hMovieFile);
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

}
