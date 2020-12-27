#pragma once

#include "FunctionFwd.hpp"
#include "Types.hpp"

struct Movie_IO
{
    void(CC* mIO_Close)(void* pHandle);
    BOOL(CC* mIO_Wait)(void* pHandle);
    BOOL(CC* mIO_Seek)(void* pHandle, DWORD offset, DWORD origin);
    void* (CC* mIO_Open)(const char* pFileName);
    BOOL(CC* mIO_Read)(void* pHandle, void* pBuffer, DWORD size);
};

Movie_IO& GetMovieIO();

struct Masher_Header
{
    int field_0_ddv_version;
    int field_4_contains;
    int field_8_frame_rate;
    int field_C_number_of_frames;
};
ALIVE_ASSERT_SIZEOF(Masher_Header, 16);

struct Masher_VideoHeader
{
    int field_0_unknown;
    DWORD field_4_width;
    DWORD field_8_height;
    int field_C_max_audio_frame_size;
    int field_10_max_video_frame_size;
    int field_14_key_frame_rate;
};
ALIVE_ASSERT_SIZEOF(Masher_VideoHeader, 24);

struct Masher_AudioHeader
{
    int field_0_audio_format;
    int field_4_samples_per_second;
    int field_8_max_audio_frame_size;
    int field_C_single_audio_frame_size;
    int field_10_num_frames_interleave;
};
ALIVE_ASSERT_SIZEOF(Masher_AudioHeader, 20);


class AudioDecompressor
{
public:

    s32 mUsedBits = 0;
    u32 mWorkBits = 0;
    s32 mAudioNumChannels = 0;
    u16* mAudioFrameDataPtr = nullptr;

    static u8 gSndTbl_byte_62EEB0[256];

    AudioDecompressor();
    static s32 GetSoundTableValue(s16 tblIndex);
    s16 sub_408F50(s16 a1);
    s32 ReadNextAudioWord(s32 value);
    s32 SndRelated_sub_409650();
    s16 NextSoundBits(u16 numBits);
    bool SampleMatches(s16& sample, s16 bits);
    template<class T>
    void decode_generic(T* outPtr, s32 numSamplesPerFrame, bool isLast);
    void decode_8bit_audio_frame(u8* outPtr, s32 numSamplesPerFrame, bool isLast);
    void decode_16bit_audio_frame(u16* outPtr, s32 numSamplesPerFrame, bool isLast);
    u16* SetupAudioDecodePtrs(u16 *rawFrameBuffer);
    void SetChannelCount(s32 channelCount);
    static void init_Snd_tbl();
};

class Masher
{
public:
    // Same as 0x523FA0 in MGSI.exe
    int Init_4E6770(const char* movieFileName);

    // Same as 0x524214 in MGSI.exe
    void dtor_4E6AB0();

    // Same as 0x52427C in MGSI.exe
    int sub_4E6B30();

    // Same as 0x52897C in MSGI.exe
    static int CC sub_4EAC30(Masher* pMasher);

    // Same as 0x528985 in MGSI.exe
    void Decode_4EA670();
    void MMX_Decode_4E6C60(BYTE* pPixelBuffer);

    // Same as 0x52B015 in MGSI.exe
    static void CC DDV_SND_4ECFD0(int numChannels, int bitsPerSample);

    // Same as 0x52B028 in MGSI.exe
    static void CC DDV_SND_4ECFF0(int* pMasherFrame, BYTE* pDecodedFrame, int frameSize);

    // Same as 0x52899C in MGSI.exe
    static void* CC GetDecompressedAudioFrame_4EAC60(Masher* pMasher);
private:
    struct Macroblock_RGB_Struct
    {
        unsigned char Red;
        unsigned char Green;
        unsigned char Blue;
        unsigned char A;
    };

    static int To1d(int x, int y);

    static unsigned char Clamp(f32 v);


    static void SetElement(int x, int y, int width, int height, u16* ptr, u16 value, bool doubleWidth, bool doubleHeight);

    static uint16_t rgb888torgb565(Macroblock_RGB_Struct& rgb888Pixel);

    static void ConvertYuvToRgbAndBlit(u16* pixelBuffer, int xoff, int yoff, int width, int height, bool doubleWidth, bool doubleHeight);


    void* field_0_file_handle;
public:
    Masher_Header field_4_ddv_header;
    Masher_VideoHeader field_14_video_header;
    Masher_AudioHeader field_2C_audio_header;
private:
    void* field_40_video_frame_to_decode;
    unsigned short *field_44_decoded_frame_data_buffer;
    int* field_48_sound_frame_to_decode;
    void *field_4C_decoded_audio_buffer;
    int field_50_num_channels;
    int field_54_bits_per_sample;
    int field_58_macro_blocks_x;
    int field_5C_macro_blocks_y;
    bool field_60_bHasAudio;
    bool field_61_bHasVideo;
    int field_64_audio_frame_idx;
    int field_68_frame_number;
    int field_6C_frame_num;
    int* field_70_frame_sizes_array;
    int* field_74_pCurrentFrameSize;
    int field_78_padding;
    int field_7C_padding;
    int* field_80_raw_frame_data;
    int field_84_max_frame_size;
    int field_88_audio_data_offset;
    void* field_8C_macro_block_buffer;
    int field_90_64_or_0;
};
ALIVE_ASSERT_SIZEOF(Masher, 0x94);
