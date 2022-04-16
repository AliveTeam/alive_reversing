#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Sound/Sound.hpp"
#include "Io.hpp"

struct ProgAtr final
{
    u8 field_0_num_tones;
    s8 field_1_vol;
    s8 field_2_priority;
    s8 field_3_mode;
    s8 field_4_pan;
    s8 field_5_reserved0;
    s16 field_6_attr;
    s32 field_8_reserved1;
    s32 field_C_reserved2;
};
ALIVE_ASSERT_SIZEOF(ProgAtr, 0x10);

struct VabHeader final
{
    s32 field_0_form;
    s32 field_4_version;
    s32 field_8_id;
    s32 field_C_file_size;
    s16 field_10_reserved0;
    s16 field_12_num_progs;
    s16 field_14_num_tones;
    u16 field_16_num_vags;
    s8 field_18_master_vol;
    s8 field_19_master_pan;
    s8 field_1A_attr1;
    s8 field_1B_attr2;
    s32 field_1C_reserved1;
    ProgAtr field_20_progs[128];
};
ALIVE_ASSERT_SIZEOF(VabHeader, 0x820);

EXPORT s16 CC SsVabOpenHead_4FC620(VabHeader* pVabHeader);

struct VabBodyRecord final
{
    s32 field_0_length_or_duration;
    s32 field_4_unused;
    u32 field_8_fileOffset;
};

struct Converted_Vag final
{
    u16 field_0_adsr_attack;
    u16 field_2_adsr_sustain_level;
    u16 field_4_adsr_decay;
    u16 field_6_adsr_release;
    u8 field_8_min;
    u8 field_9_max;
    s16 field_A_shift_cen;
    u8 field_C;
    u8 field_D_vol;
    u8 field_E_priority;
    u8 field_F_prog;
    u8 field_10_vag;
    s8 field_11_pad;
};
ALIVE_ASSERT_SIZEOF(Converted_Vag, 0x12);

constexpr s32 kMaxVabs = 4;

struct ConvertedVagTable final
{
    Converted_Vag table[kMaxVabs][128][16]; // 16 = max tones, 128 = max progs
};
ALIVE_ASSERT_SIZEOF(ConvertedVagTable, 147456);

struct SoundEntryTable final
{
    SoundEntry table[kMaxVabs][256];
};
ALIVE_ASSERT_SIZEOF(SoundEntryTable, 36864);

struct VabUnknown final
{
    s8 field_0[4][128];
};
ALIVE_ASSERT_SIZEOF(VabUnknown, 512);

const s32 kNumChannels = 24;

// Only exposed for SND_PlayEx_4EF740!
struct MIDI_ADSR_State final
{
    u8 field_0_seq_idx;
    u8 field_1_program;
    u8 field_2_note_byte1;
    s8 field_3_state;
    u16 field_4_attack;
    u16 field_6_sustain;
    u16 field_8_decay;
    u16 field_A_release;
    u16 field_C;
    u8 field_E_ref_count;
    s8 field_F_pad;
};
ALIVE_ASSERT_SIZEOF(MIDI_ADSR_State, 0x10);

struct MIDI_Channel final
{
    s32 field_0_sound_buffer_field_4;
    s32 field_4_priority;
    s32 field_8_left_vol;
    s32 field_C_vol; // pan vol?
    f32 field_10_freq;
    s32 field_14_time;
    s32 field_18_rightVol;
    MIDI_ADSR_State field_1C_adsr;
};
ALIVE_ASSERT_SIZEOF(MIDI_Channel, 0x2C);

struct MidiChannels final
{
    MIDI_Channel channels[kNumChannels];
};
ALIVE_ASSERT_SIZEOF(MidiChannels, 1056);

struct MIDI_ProgramVolume final
{
    s8 field_0_program;
    s8 field_1_left_vol;
    s8 field_2_right_vol;
};
ALIVE_ASSERT_SIZEOF(MIDI_ProgramVolume, 3);

struct MIDI_SeqSong final
{
    u8* field_0_seq_data;
    u32 field_4_time;
    s32 field_8_playTimeStamp;
    s32 field_C_volume;
    s32 field_10_quaterNoteRes;
    s32 field_14_tempo;
    s32 field_18_repeatCount;
    u8* field_1C_pSeqData;
    void* field_20_fn_ptr; // read but never written
    u8* field_24_loop_start;
    s16 field_seq_idx;
    u8 field_2A_running_status;
    s8 field_2B_repeatMode; // TODO: Check
    s8 field_2C_loop_count;
    s8 field_2D_pad;
    s16 field_2E_seqAccessNum; // never written in a meaningful way
    s8 field_30_timeSignatureBars;
    s8 field_31_timeSignatureBars2; // bug: maybe they should have assigned beats instead? but never read anyway
    MIDI_ProgramVolume field_32_progVols[16];
    s8 field_62_pad;
    s8 field_63_pad;
};
ALIVE_ASSERT_SIZEOF(MIDI_SeqSong, 100);

struct MidiSeqSongsTable final
{
    MIDI_SeqSong table[32];
};

ALIVE_ASSERT_SIZEOF(MidiSeqSongsTable, 3200);

class IPsxSpuApiVars
{
public:
    virtual ~IPsxSpuApiVars()
    { }
    virtual s16& sGlobalVolumeLevel_right() = 0;
    virtual s16& sGlobalVolumeLevel_left() = 0;
    virtual VabUnknown& s512_byte() = 0;
    virtual u8* sVagCounts() = 0;
    virtual u8* sProgCounts() = 0;
    virtual VabHeader** spVabHeaders() = 0;
    virtual ConvertedVagTable& sConvertedVagTable() = 0;
    virtual SoundEntryTable& sSoundEntryTable16() = 0;
    virtual MidiChannels& sMidi_Channels() = 0;
    virtual MIDI_SeqSong& sMidiSeqSongs(s32 idx) = 0;
    virtual s32& sMidi_Inited_dword() = 0;
    virtual u32& sMidiTime() = 0;
    virtual Bool32& sSoundDatIsNull() = 0;
    virtual s8& sbDisableSeqs() = 0;
    virtual u32& sLastTime() = 0;
    virtual u32& sMidi_WaitUntil() = 0;
    virtual IO_FileHandleType& sSoundDatFileHandle() = 0;
    virtual u8& sControllerValue() = 0;
    virtual void MIDI_ParseMidiMessage(s32 idx) = 0;
    virtual void SsUtKeyOffV(s32 idx) = 0;
};

EXPORT void SetSpuApiVars(IPsxSpuApiVars* pVars);
EXPORT IPsxSpuApiVars* GetSpuApiVars();

EXPORT void CC SsVabTransBody_4FC840(VabBodyRecord* pVabBody, s16 vabId);

#define SS_WAIT_COMPLETED 1
EXPORT void SsVabTransCompleted_4FE060(s32 immediateFlag);
EXPORT void CC SsVabClose_4FC5B0(s32 vabId);


EXPORT void CC SsSetMVol_4FC360(s16 left, s16 right);
EXPORT void CC SSInit_4FC230();
EXPORT void CC SpuInitHot_4FC320();
EXPORT void SsEnd_4FC350();

EXPORT void CC SsSetTableSize_4FE0B0(void* pTable, s32 sMax, s32 tMax);

EXPORT void SsUtSetReverbDepth_4FE380(s32 leftDepth, s32 rightDepth);
EXPORT void CC SsUtSetReverbType_4FE360(s32);
EXPORT void SsUtReverbOn_4FE340();
EXPORT void SsUtReverbOff_4FE350();
EXPORT void SpuClearReverbWorkArea_4FA690(s32 reverbMode);

EXPORT void CC SsSetTickMode_4FDC20(s32 tickMode);
EXPORT s32 CC SsVoKeyOn_4FCF10(s32 vabIdAndProgram, s32 pitch, u16 leftVol, u16 rightVol, s32 pan);
EXPORT void CC SsUtAllKeyOff_4FDFE0(s32 mode);
EXPORT s16 CC SsUtKeyOffV_4FE010(s16 idx);
EXPORT s16 CC SsUtChangePitch_4FDF70(s16 voice, s32 /*vabId*/, s32 /*prog*/, s16 old_note, s16 old_fine, s16 new_note, s16 new_fine);

EXPORT s16 CC SsSeqOpen_4FD6D0(u8* pSeqData, s16 seqIdx);
EXPORT void CC SsSeqClose_4FD8D0(s16 idx);
EXPORT void CC SsSeqStop_4FD9C0(s16 idx);
EXPORT u16 CC SsIsEos_4FDA80(s16 idx, s16 seqNum);
EXPORT void CC SsSeqSetVol_4FDAC0(s16 idx, s16 volLeft, s16 volRight);
EXPORT void CC SsSeqPlay_4FD900(u16 idx, s8 playMode, s16 repeatCount);

EXPORT s32 CC MIDI_ParseMidiMessage_4FD100(s32 idx);
EXPORT s32 CC MIDI_Read_Var_Len_4FD0D0(MIDI_SeqSong* pMidiStru);
EXPORT u8 CC MIDI_ReadByte_4FD6B0(MIDI_SeqSong* pData);
EXPORT void CC MIDI_SkipBytes_4FD6C0(MIDI_SeqSong* pData, s32 length);
EXPORT void CC MIDI_SetTempo_4FDB80(s16 idx, s16 kZero, s16 tempo);
EXPORT s32 CC MIDI_PlayerPlayMidiNote_4FCE80(s32 vabId, s32 program, s32 note, s32 leftVol, s32 rightVol, s32 volume);
EXPORT s32 CC MIDI_Allocate_Channel_4FCA50(s32 not_used, s32 priority);

using TVSyncCallBackFn = void(CC*)();
EXPORT void CC VSyncCallback_4F8C40(TVSyncCallBackFn callBack);
EXPORT void CC SND_CallBack_4020A4(); // TODO: Naming??
EXPORT void CC SsSeqCalledTbyT_4FDC80();

// Most likely PC specific extensions that have been inlined
void SsExt_CloseAllVabs();

void SsExt_StopPlayingSamples();
