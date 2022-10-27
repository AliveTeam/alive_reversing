#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Types.hpp"

struct Movie_IO final
{
    void(CC* mIO_Close)(void* pHandle);
    bool(CC* mIO_Wait)
    (void* pHandle);
    bool(CC* mIO_Seek)
    (void* pHandle, u32 offset, u32 origin);
    void*(CC* mIO_Open)(const char_type* pFileName);
    bool(CC* mIO_Read)
    (void* pHandle, void* pBuffer, u32 size);
};

Movie_IO& GetMovieIO();

struct Masher_Header final
{
    s32 field_0_ddv_version;
    s32 field_4_contains;
    s32 field_8_frame_rate;
    s32 field_C_number_of_frames;
};
ALIVE_ASSERT_SIZEOF(Masher_Header, 16);

struct Masher_VideoHeader final
{
    s32 field_0_unknown;
    u32 field_4_width;
    u32 field_8_height;
    s32 field_C_max_audio_frame_size;
    s32 field_10_max_video_frame_size;
    s32 field_14_key_frame_rate;
};
ALIVE_ASSERT_SIZEOF(Masher_VideoHeader, 24);

struct Masher_AudioHeader final
{
    s32 field_0_audio_format;
    s32 field_4_samples_per_second;
    s32 field_8_max_audio_frame_size;
    s32 field_C_single_audio_frame_size;
    s32 field_10_num_frames_interleave;
};
ALIVE_ASSERT_SIZEOF(Masher_AudioHeader, 20);


class AudioDecompressor final
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
    template <class T>
    void decode_generic(T* outPtr, s32 numSamplesPerFrame, bool isLast);
    void decode_8bit_audio_frame(u8* outPtr, s32 numSamplesPerFrame, bool isLast);
    void decode_16bit_audio_frame(u16* outPtr, s32 numSamplesPerFrame, bool isLast);
    u16* SetupAudioDecodePtrs(u16* rawFrameBuffer);
    void SetChannelCount(s32 channelCount);
    static void init_Snd_tbl();
};

class Masher final
{
public:
    // Same as 0x523FA0 in MGSI.exe
    s32 Init_4E6770(const char_type* movieFileName);

    // Same as 0x524214 in MGSI.exe
    ~Masher();

    // Same as 0x52427C in MGSI.exe
    s32 ReadNextFrame_4E6B30();

    // Same as 0x52897C in MSGI.exe
    static s32 ReadNextFrameToMemory_4EAC30(Masher* pMasher);

    // Same as 0x528985 in MGSI.exe
    void Decode_4EA670();
    void VideoFrameDecode_4E6C60(u8* pPixelBuffer);

    // Same as 0x52B015 in MGSI.exe
    static void DDV_Set_Channels_And_BitsPerSample_4ECFD0(s32 numChannels, s32 bitsPerSample);

    // Same as 0x52B028 in MGSI.exe
    static void DDV_DecompressAudioFrame_4ECFF0(s32* pMasherFrame, BYTE* pDecodedFrame, s32 frameSize);

    // Same as 0x52899C in MGSI.exe
    static void* GetDecompressedAudioFrame_4EAC60(Masher* pMasher);

private:
    struct Macroblock_RGB_Struct final
    {
        u8 Red;
        u8 Green;
        u8 Blue;
        u8 A;
    };

    static s32 To1d(s32 x, s32 y);

    static u8 Clamp(f32 v);


    static void SetElement(s32 x, s32 y, s32 width, s32 height, u16* ptr, u16 value, bool doubleWidth, bool doubleHeight);

    static void ConvertYuvToRgbAndBlit(u16* pixelBuffer, s32 xoff, s32 yoff, s32 width, s32 height, bool doubleWidth, bool doubleHeight);


    void* field_0_file_handle;

public:
    Masher_Header field_4_ddv_header;
    Masher_VideoHeader field_14_video_header;
    Masher_AudioHeader field_2C_audio_header;

private:
    void* field_40_video_frame_to_decode;
    u16* field_44_decoded_frame_data_buffer;
    s32* field_48_sound_frame_to_decode;
    void* field_4C_decoded_audio_buffer;
    s32 field_50_num_channels;
    s32 field_54_bits_per_sample;
    s32 field_58_macro_blocks_x;
    s32 field_5C_macro_blocks_y;
    bool field_60_bHasAudio;
    bool field_61_bHasVideo;
    s32 field_64_audio_frame_idx;
    s32 field_68_frame_number;
    s32 field_6C_frame_num;
    s32* field_70_frame_sizes_array;
    s32* field_74_pCurrentFrameSize;
    s32* field_80_raw_frame_data;
    s32 field_84_max_frame_size;
    s32 field_88_audio_data_offset;
    void* field_8C_macro_block_buffer;
    s32 field_90_64_or_0;
};
ALIVE_ASSERT_SIZEOF(Masher, 0x94);
