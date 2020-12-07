#pragma once

#include "FunctionFwd.hpp"
#include "Sound/Sound.hpp"
#include "Io.hpp"

struct ProgAtr
{
    BYTE field_0_num_tones;
    char field_1_vol;
    char field_2_priority;
    char field_3_mode;
    char field_4_pan;
    char field_5_reserved0;
    __int16 field_6_attr;
    int field_8_reserved1;
    int field_C_reserved2;
};
ALIVE_ASSERT_SIZEOF(ProgAtr, 0x10);

struct VabHeader
{
    int field_0_form;
    int field_4_version;
    int field_8_id;
    int field_C_file_size;
    __int16 field_10_reserved0;
    __int16 field_12_num_progs;
    __int16 field_14_num_tones;
    unsigned __int16 field_16_num_vags;
    char field_18_master_vol;
    char field_19_master_pan;
    char field_1A_attr1;
    char field_1B_attr2;
    int field_1C_reserved1;
    ProgAtr field_20_progs[128];
};
ALIVE_ASSERT_SIZEOF(VabHeader, 0x820);

EXPORT __int16 CC SsVabOpenHead_4FC620(VabHeader* pVabHeader);

struct VabBodyRecord
{
    int field_0_length_or_duration;
    int field_4_unused;
    DWORD field_8_fileOffset;
};

struct Converted_Vag
{
    WORD field_0_adsr_attack;
    WORD field_2_adsr_sustain_level;
    WORD field_4_adsr_decay;
    WORD field_6_adsr_release;
    BYTE field_8_min;
    BYTE field_9_max;
    __int16 field_A_shift_cen;
    BYTE field_C;
    BYTE field_D_vol;
    BYTE field_E_priority;
    BYTE field_F_prog;
    BYTE field_10_vag;
    char field_11_pad;
};
ALIVE_ASSERT_SIZEOF(Converted_Vag, 0x12);

constexpr int kMaxVabs = 4;

struct ConvertedVagTable
{
    Converted_Vag table[kMaxVabs][128][16]; // 16 = max tones, 128 = max progs
};
ALIVE_ASSERT_SIZEOF(ConvertedVagTable, 147456);

struct SoundEntryTable
{
    SoundEntry table[kMaxVabs][256];
};
ALIVE_ASSERT_SIZEOF(SoundEntryTable, 36864);

struct VabUnknown
{
    char field_0[4][128];
};
ALIVE_ASSERT_SIZEOF(VabUnknown, 512);

const int kNumChannels = 24;

// Only exposed for SND_PlayEx_4EF740!
struct MIDI_ADSR_State
{
    unsigned __int8 field_0_seq_idx;
    unsigned __int8 field_1_program;
    unsigned __int8 field_2_note_byte1;
    char field_3_state;
    unsigned __int16 field_4_attack;
    unsigned __int16 field_6_sustain;
    unsigned __int16 field_8_decay;
    unsigned __int16 field_A_release;
    unsigned __int16 field_C;
    unsigned __int8 field_E_ref_count;
    char field_F_pad;
};
ALIVE_ASSERT_SIZEOF(MIDI_ADSR_State, 0x10);

struct MIDI_Channel
{
    int field_0_sound_buffer_field_4;
    int field_4_priority;
    int field_8_left_vol;
    int field_C_vol; // pan vol?
    float field_10_freq;
    int field_14_time;
    int field_18_rightVol;
    MIDI_ADSR_State field_1C_adsr;
};
ALIVE_ASSERT_SIZEOF(MIDI_Channel, 0x2C);

struct MidiChannels
{
    MIDI_Channel channels[kNumChannels];
};
ALIVE_ASSERT_SIZEOF(MidiChannels, 1056);

struct MIDI_ProgramVolume
{
    char field_0_program;
    char field_1_left_vol;
    char field_2_right_vol;
};
ALIVE_ASSERT_SIZEOF(MIDI_ProgramVolume, 3);

struct MIDI_SeqSong
{
    BYTE* field_0_seq_data;
    unsigned int field_4_time;
    int field_8_playTimeStamp;
    int field_C_volume;
    int field_10_quaterNoteRes;
    int field_14_tempo;
    int field_18_repeatCount;
    BYTE* field_1C_pSeqData;
    void* field_20_fn_ptr; // read but never written
    BYTE* field_24_loop_start;
    __int16 field_seq_idx;
    unsigned char field_2A_running_status;
    char field_2B_repeatMode; // TODO: Check
    char field_2C_loop_count;
    char field_2D_pad;
    __int16 field_2E_seqAccessNum; // never written in a meaningful way
    char field_30_timeSignatureBars;
    char field_31_timeSignatureBars2; // bug: maybe they should have assigned beats instead? but never read anyway
    MIDI_ProgramVolume field_32_progVols[16];
    char field_62_pad;
    char field_63_pad;
};
ALIVE_ASSERT_SIZEOF(MIDI_SeqSong, 100);

struct MidiSeqSongsTable
{
    MIDI_SeqSong table[32];
};

ALIVE_ASSERT_SIZEOF(MidiSeqSongsTable, 3200);

class IPsxSpuApiVars
{
public:
    virtual ~IPsxSpuApiVars() {}
    virtual __int16& sGlobalVolumeLevel_right() = 0;
    virtual __int16& sGlobalVolumeLevel_left() = 0;
    virtual VabUnknown& s512_byte() = 0;
    virtual BYTE* sVagCounts() = 0;
    virtual BYTE* sProgCounts() = 0;
    virtual VabHeader** spVabHeaders() = 0;
    virtual ConvertedVagTable& sConvertedVagTable() = 0;
    virtual SoundEntryTable& sSoundEntryTable16() = 0;
    virtual MidiChannels& sMidi_Channels() = 0;
    virtual MidiSeqSongsTable& sMidiSeqSongs() = 0;
    virtual int& sMidi_Inited_dword() = 0;
    virtual unsigned int& sMidiTime() = 0;
    virtual BOOL& sSoundDatIsNull() = 0;
    virtual char& sbDisableSeqs() = 0;
    virtual DWORD& sLastTime() = 0;
    virtual DWORD& sMidi_WaitUntil() = 0;
    virtual IO_FileHandleType& sSoundDatFileHandle() = 0;
    virtual BYTE& sControllerValue() = 0;
    virtual void MIDI_ParseMidiMessage(int idx) = 0;
    virtual void SsUtKeyOffV(int idx) = 0;
};

EXPORT void SetSpuApiVars(IPsxSpuApiVars* pVars);
EXPORT IPsxSpuApiVars* GetSpuApiVars();

EXPORT void CC SsVabTransBody_4FC840(VabBodyRecord* pVabBody, __int16 vabId);

#define SS_WAIT_COMPLETED 1
EXPORT void SsVabTransCompleted_4FE060(int immediateFlag);
EXPORT void CC SsVabClose_4FC5B0(int vabId);


EXPORT void CC SsSetMVol_4FC360(__int16 left, __int16 right);
EXPORT void CC SSInit_4FC230();
EXPORT void CC SpuInitHot_4FC320();
EXPORT void SsEnd_4FC350();

EXPORT void CC SsSetTableSize_4FE0B0(void* pTable, int sMax, int tMax);

EXPORT void SsUtSetReverbDepth_4FE380(int leftDepth, int rightDepth);
EXPORT void CC SsUtSetReverbType_4FE360(int);
EXPORT void SsUtReverbOn_4FE340();
EXPORT void SsUtReverbOff_4FE350();
EXPORT void SpuClearReverbWorkArea_4FA690(int reverbMode);

EXPORT void CC SsSetTickMode_4FDC20(int tickMode);
EXPORT int CC SsVoKeyOn_4FCF10(int vabIdAndProgram, int pitch, unsigned __int16 leftVol, unsigned __int16 rightVol);
EXPORT void CC SsUtAllKeyOff_4FDFE0(int mode);
EXPORT __int16 CC SsUtKeyOffV_4FE010(__int16 idx);
EXPORT __int16 CC SsUtChangePitch_4FDF70(__int16 voice, int /*vabId*/, int /*prog*/, __int16 old_note, __int16 old_fine, __int16 new_note, __int16 new_fine);

EXPORT __int16 CC SsSeqOpen_4FD6D0(BYTE* pSeqData, __int16 seqIdx);
EXPORT void CC SsSeqClose_4FD8D0(__int16 idx);
EXPORT void CC SsSeqStop_4FD9C0(__int16 idx);
EXPORT unsigned __int16 CC SsIsEos_4FDA80(__int16 idx, __int16 seqNum);
EXPORT void CC SsSeqSetVol_4FDAC0(__int16 idx, __int16 volLeft, __int16 volRight);
EXPORT void CC SsSeqPlay_4FD900(unsigned __int16 idx, char playMode, __int16 repeatCount);

EXPORT signed int CC MIDI_ParseMidiMessage_4FD100(int idx);
EXPORT int CC MIDI_Read_Var_Len_4FD0D0(MIDI_SeqSong* pMidiStru);
EXPORT BYTE CC MIDI_ReadByte_4FD6B0(MIDI_SeqSong* pData);
EXPORT void CC MIDI_SkipBytes_4FD6C0(MIDI_SeqSong* pData, int length);
EXPORT void CC MIDI_SetTempo_4FDB80(__int16 idx, __int16 kZero, __int16 tempo);
EXPORT int CC MIDI_PlayerPlayMidiNote_4FCE80(int vabId, int program, int note, int leftVol, int rightVol, int volume);
EXPORT signed int CC MIDI_Allocate_Channel_4FCA50(int not_used, int priority);

using TVSyncCallBackFn = void(CC *)();
EXPORT void CC VSyncCallback_4F8C40(TVSyncCallBackFn callBack);
EXPORT void CC SND_CallBack_4020A4(); // TODO: Naming??
EXPORT void CC SsSeqCalledTbyT_4FDC80();

// Most likely PC specific extensions that have been inlined
void SsExt_CloseAllVabs();

void SsExt_StopPlayingSamples();
