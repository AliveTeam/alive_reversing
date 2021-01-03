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

SoundEntry soundEntry;

const int kSingleAudioFrameSize = 2016*2;
const int kNumFramesInterleave = 5;

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

    bool Decode()
    {

        const int kXaFrameDataSize = 2016;
        const int kNumAudioChannels = 2;
        const int kBytesPerSample = 2;
        std::array<s16, (kXaFrameDataSize* kNumAudioChannels* kBytesPerSample) / 2> outPtr;

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


                //if (!noAudio)
                {
                    //  // Restore the lost buffer
//                    if (GetSoundAPI().SND_LoadSamples(pSnd, 0, pSnd->field_8_pSoundBuffer, pSnd->field_C_buffer_size_bytes / pSnd->field_1D_blockAlign) == 0)

                    // 2352
                    if (GetSoundAPI().SND_LoadSamples(&soundEntry, mSoundPos, (unsigned char*)outPtr.data(), (outPtr.size() * 2) / soundEntry.field_1D_blockAlign) < 0)
                    {

                    }

                    mSoundPos += 2016;

                    if (mSoundPos > (2016 * (kNumFramesInterleave+6)))
                    {
                        mSoundPos = 0;
                    }

                    return true;

                    /*
                    size_t consumedSrc = 0;
                    size_t wroteSamples = 0;
                    size_t inLenSampsPerChan = kXaFrameDataSize;
                    size_t outLenSampsPerChan = inLenSampsPerChan * 2;
                    std::vector<u8> tmp2(2352 * 4);

                    soxr_io_spec_t ioSpec = soxr_io_spec(
                        SOXR_INT16_I,   // In type
                        SOXR_INT16_I);  // Out type

                    soxr_oneshot(
                        37800,       // Input rate
                        44100,      // Output rate
                        2,          // Num channels
                        outPtr.data(),
                        inLenSampsPerChan,
                        &consumedSrc,
                        tmp2.data(),
                        outLenSampsPerChan,
                        &wroteSamples,
                        &ioSpec,    // IO spec
                        nullptr,    // Quality spec
                        nullptr     // Runtime spec
                    );


                    for (auto i = 0u; i < wroteSamples * 4; i++)
                    {
                        mAudioBuffer.push_back(tmp2[i]);
                    }
                    */
                }
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


void Movie::VUpdate_489EA0()
{
    AE_IMPLEMENTED();


    //if (!bLoadingAFile_50768C)
    {
      //  bLoadingAFile_50768C = TRUE;

        void* hMovie = GetMovieIO().mIO_Open(sMovieNames_508B08.mNames[sMovieNameIdx_508C0C].mName);
        if (hMovie)
        {
            PsxStr psxStr;
            psxStr.mFile = hMovie;

            Bitmap tmpBmp = {};
            BMP_New_4F1990(&tmpBmp, 320, 192, 15, 0);

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

            SDL_Surface* wholeImage = SDL_CreateRGBSurface(0, 320, 192, 32, rmask, gmask, bmask, amask);
            if (wholeImage == NULL)
            {
                fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
                exit(1);
            }

            if (GetSoundAPI().SND_New(
                &soundEntry,
                2016*(kNumFramesInterleave + 6), // OG passes  2016*20 =40320
                37800,
                16,
                1) < 0)
            {

            }

            if (FAILED(GetSoundAPI().SND_PlayEx(&soundEntry, 116, 116, 1.0, 0, 1, 100)))
            {

            }

            const auto movieStartTimeStamp = SYS_GetTicks();

            int dword_5CA1FC = 0;
            int total_audio_offset = 0;
            const int kFrameRate = 30;
            int oldBufferPlayPos = 0;
            while (psxStr.Decode())
            {
                if (!psxStr.pixelBuffer.empty())
                {
                    SDL_LockSurface(wholeImage);
                    memcpy(wholeImage->pixels, psxStr.pixelBuffer.data(), psxStr.mFrameW * psxStr.mFrameH * 4);
                    SDL_UnlockSurface(wholeImage);

                    SDL_BlitScaled(wholeImage, nullptr, tmpBmp.field_0_pSurface, nullptr);

                    //SDL_SaveBMP(wholeImage, "test.bmp");
                }

                //SDL_LockSurface(tmpBmp.field_0_pSurface);
                //SDL_UnlockSurface(tmpBmp.field_0_pSurface);

                Render_Str_Frame(tmpBmp);

                // Sync on where the audio playback is up to
                total_audio_offset += kSingleAudioFrameSize;

                const int sampleLength = kSingleAudioFrameSize * 26;

                const DWORD soundBufferPlayPos = SND_Get_Sound_Entry_Pos_4EF620(&soundEntry);
                if ((signed int)(oldBufferPlayPos - soundBufferPlayPos) > sampleLength / 2)
                {
                    dword_5CA1FC++;
                }

                oldBufferPlayPos = soundBufferPlayPos;

                const int maxWait = 1000 * kNumFramesInterleave / kFrameRate + 2000; // check max
                if (total_audio_offset >= 0)
                {
                    int counter = 0;
                    for (;;)
                    {
                        const unsigned int soundPlayingPos = SND_Get_Sound_Entry_Pos_4EF620(&soundEntry);
                        const int remainderLen = oldBufferPlayPos - soundPlayingPos;
                        if (remainderLen > sampleLength / 2)
                        {
                            dword_5CA1FC++;
                        }

                        oldBufferPlayPos = soundPlayingPos;

                        ++counter;

                        const int kTargetOff = 
                              kSingleAudioFrameSize
                            * kNumFramesInterleave
                            + soundPlayingPos
                            + sampleLength * dword_5CA1FC;

                        if (counter > 10000)
                        {
                            counter = 0;
                            if ((signed int)(SYS_GetTicks() - movieStartTimeStamp) > maxWait)
                            {
                                // TODO: Unknown failure case
                                //bNoAudio_5CA1F4 = 1;
                                break;
                            }
                        }

                        if (total_audio_offset < kTargetOff)
                        {
                            break;
                        }
                    }
                }


                SYS_EventsPump_44FF90();
                PSX_VSync_496620(0);

                if (Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN))
                {
                    break;
                }

            }

            GetSoundAPI().SND_Free(&soundEntry);

            SDL_FreeSurface(wholeImage);

            Bmp_Free_4F1950(&tmpBmp);

            GetMovieIO().mIO_Close(hMovie);
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

}
