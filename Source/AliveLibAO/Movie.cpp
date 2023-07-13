#include "stdafx_ao.h"
#include "Movie.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/PSXADPCMDecoder.h"
#include "../relive_lib/PSXMDECDecoder.h"
#include "../relive_lib/Masher.hpp"
#include "../AliveLibAE/Io.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/Psx.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibAE/Input.hpp"
#include "../relive_lib/Sound/SDLSoundBuffer.hpp"
#include "GameAutoPlayer.hpp"
#include "Midi.hpp"

#define NO_WAVE
#include "../relive_lib/Sound/Sound.hpp"

namespace AO {

s32 gMovieRefCount = 0;

static SoundEntry sFmvSoundEntry;


static const int kXaFrameDataSize = 2016;

static const int num_frames_interleave = 5; // maybe 20 ?? AE uses 5
static const int fmv_single_audio_frame_size_in_samples = 2016; // AE uses 2940
static const auto fmv_sound_entry_size = fmv_single_audio_frame_size_in_samples * (num_frames_interleave + 6);
static const int kSamplesPerSecond = 37800; // AE uses 44100
static const int kFmvFrameRate = 15;

static bool sNoAudioOrAudioError = false;
static int sFmvAudioSampleOffset = 0;
static bool bStartedPlayingSound = false;

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

     std::vector<s16> outPtr;

    bool DecodeAudioAndVideo()
    {
        if (mDemuxBuffer.empty())
        {
            mDemuxBuffer.resize(1024 * 1024);
        }

        outPtr.clear();

        for (;;)
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

                    mMdec.DecodeFrameToRGBA32((uint16_t*)pixelBuffer.data(), (uint16_t*)mDemuxBuffer.data(), frameW, frameH);
                    //mVideoBuffer.push_back(Frame{ mFrameCounter++, frameW, frameH, pixelBuffer });
                    return true;
                }
            }
            else if (w.mSectorType == kVale)
            {
                outPtr.resize(2016*2);
                mAdpcm.DecodeFrameToPCM(outPtr, (uint8_t*)&w.mAkikMagic);

                if (!sNoAudioOrAudioError)
                {
                    // Push new samples into the buffer
                    if (GetSoundAPI().mSND_LoadSamples(&sFmvSoundEntry, sFmvAudioSampleOffset, (unsigned char*)outPtr.data(), fmv_single_audio_frame_size_in_samples) < 0)
                    {
                        // Reload with data fail
                        sNoAudioOrAudioError = true;
                    }

                    sFmvAudioSampleOffset += fmv_single_audio_frame_size_in_samples;

                    // Loop back to the start of the audio buffer
                    if (sFmvAudioSampleOffset >= fmv_sound_entry_size)
                    {
                        sFmvAudioSampleOffset = 0;
                    }

                    // If this is the first time then start to play the buffer
                    if (!bStartedPlayingSound && !sNoAudioOrAudioError)
                    {
                        // TODO: PSX Version would allow some sounds / bg music to play over certain FMV transitions
                        SND_StopAll();

                        bStartedPlayingSound = true;
                        if (FAILED(SND_PlayEx(&sFmvSoundEntry, 116, 116, 1.0, 0, 1, 100)))
                        {
                            sNoAudioOrAudioError = true;
                        }
                    }
                }

                //return true;
            }
            else if (w.mSectorType == 0x10101010)
            {
                // end of stream ??
                return false;
            }
            else
            {
                abort();
            }
        }
//        return true;
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

Movie::Movie(const char_type* pFmvName)
    : BaseGameObject(true, 0)
    , mFmvName(pFmvName)
{
    LOG_INFO("Create movie %s", mFmvName);
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eMovie);

    IO_Init_494230(); // Set up IO funcs

    gMovieRefCount++;
}

Movie::~Movie()
{
    LOG_INFO("Destroy movie %s", mFmvName);
    gMovieRefCount--;
}

void Movie::VScreenChanged()
{
    // Empty
}

static void Render_Str_Frame()
{
    // Copy into the emulated vram - when FMV ends the "screen" still have the last video frame "stick"
    // giving us a nice seamless transition.
    //SDL_Rect bufferSize = { 0,0, 640, 240 };
    //SDL_BlitScaled(tmpBmp.field_0_pSurface, nullptr, GetPsxVram().field_0_pSurface, &bufferSize);

    // Copy to full window/primary buffer
    VGA_EndFrame();
}

class TempSurface final
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

            wholeImage = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
            if (!wholeImage)
            {
                ALIVE_FATAL("CreateRGBSurface failed: %s", SDL_GetError());
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



void Movie::VUpdate()
{
    // AE_IMPLEMENTED();

    if (GetGameAutoPlayer().IsPlaying() || GetGameAutoPlayer().IsRecording())
    {
        SetDead(true);
        return;
    }

    // Open the file
    void* hMovieFile = GetMovieIO().mIO_Open(mFmvName);

    // Bail if failed to open
    if (!hMovieFile)
    {
        SetDead(true);
        return;
    }

    PsxStr psxStr;
    psxStr.mFile = hMovieFile;

    sNoAudioOrAudioError = false;
    sFmvAudioSampleOffset = 0;
    bStartedPlayingSound = false;

    if (GetSoundAPI().mSND_New(
        &sFmvSoundEntry,
        fmv_sound_entry_size,
        kSamplesPerSecond,
        16,
        7) < 0)
    {
        // SND_New failed
        sFmvSoundEntry.field_4_pDSoundBuffer = nullptr;
        sNoAudioOrAudioError = true;
    }

    int fmv_num_read_frames = 0;

    TempSurface tempSurface;
    //Bitmap tmpBmp = {};

    // Till EOF decoding loop
    const auto movieStartTimeStamp = SYS_GetTicks();
    while (psxStr.DecodeAudioAndVideo())
    {
        fmv_num_read_frames++;

        //
        // Video frame logic
        //
        if (!psxStr.pixelBuffer.empty())
        {
            // Create if it hasn't or recreate if w/h changed
            if (tempSurface.InitIf(psxStr.mFrameW, psxStr.mFrameH))
            {
                // Ditto
                /*
                if (tmpBmp.field_0_pSurface)
                {
                    Bmp_Free_4F1950(&tmpBmp);
                    tmpBmp = {};
                }
                BMP_New_4F1990(&tmpBmp, psxStr.mFrameW, psxStr.mFrameH, 15, 0);*/
            }

            // Copy decoded frame to tempSurface
            tempSurface.SetPixels(psxStr.pixelBuffer);

            // Copy temp surface to tmpBmp (colour depth conversion)
            //tempSurface.BlitScaledTo(tmpBmp.field_0_pSurface);
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

        Render_Str_Frame();

        const int maxAudioSyncTimeWait = 1000 * fmv_num_read_frames / kFmvFrameRate - 200;

        while ((signed int) (SYS_GetTicks() - movieStartTimeStamp) <= maxAudioSyncTimeWait)
        {
            // Wait for the amount of time the frame would take to display at the given framerate
        }

        SYS_EventsPump();
        PSX_VSync(VSyncMode::UncappedFps);
    }

    if (sFmvSoundEntry.field_4_pDSoundBuffer)
    {
        sFmvSoundEntry.field_4_pDSoundBuffer->Stop();

        GetSoundAPI().mSND_Free(&sFmvSoundEntry);
        sFmvSoundEntry.field_4_pDSoundBuffer = nullptr;
    }

    //Bmp_Free_4F1950(&tmpBmp);

    GetMovieIO().mIO_Close(hMovieFile);

    SetDead(true);
}

} // namespace AO
