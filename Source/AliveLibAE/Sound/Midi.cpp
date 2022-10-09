#include "../stdafx.h"
#include "Midi.hpp"
#include "Function.hpp"
#include "Sound.hpp"
#include "../PathData.hpp"
#include "../ResourceManager.hpp"
#include "../Game.hpp"
#include "../LvlArchive.hpp"
#include "../stdlib.hpp"
#include "../Abe.hpp"
#include "../MusicController.hpp"
#include "../BackgroundMusic.hpp"
#include "../Sys.hpp"
#include "../Io.hpp"

#include "../Sfx.hpp"
#include "../PathData.hpp"

#include "PsxSpuApi.hpp"
#include "../relive_lib/BinaryPath.hpp"
#include "../AliveLibAE/AmbientSound.hpp"

void SFX_SetPitch_4CA510(const relive::SfxDefinition& pSfx, s32 channelsBits, s16 pitch);

const s32 kSeqTableSizeAE = 144;

static TSNDStopAll sSNDStopAllCallBack = nullptr;
static TSNDRestart sSNDRestartCallBack = nullptr;

ALIVE_VAR(1, 0xBB2354, SeqIds, sSeq_Ids_word_BB2354, {});
ALIVE_VAR(1, 0xbb2e3e, u16, sSnd_ReloadAbeResources_BB2E3E, 0);
ALIVE_VAR(1, 0xbb2e38, OpenSeqHandle*, sSeqDataTable_BB2E38, nullptr);
ALIVE_VAR(1, 0xbb2e3c, s16, sSeqsPlaying_count_word_BB2E3C, 0);
ALIVE_VAR(1, 0x560f58, s16, sSFXPitchVariationEnabled_560F58, true);
ALIVE_VAR(1, 0x560f40, s16, sNeedToHashSeqNames_560F40, 1);

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
const PathSoundInfo soundBlock = {"MONK.VH", "MONK.VB", {}, {}, {}};

ALIVE_VAR(1, 0x560F48, PathSoundInfo, sMonkVh_Vb_560F48, soundBlock);

class AEMidiVars final : public IMidiVars
{
public:
    virtual SeqIds& sSeq_Ids_word() override
    {
        return sSeq_Ids_word_BB2354;
    }

    virtual u16& sSnd_ReloadAbeResources() override
    {
        return sSnd_ReloadAbeResources_BB2E3E;
    }

    virtual OpenSeqHandle*& sSeqDataTable() override
    {
        return sSeqDataTable_BB2E38;
    }

    virtual s16& sSeqsPlaying_count_word() override
    {
        return sSeqsPlaying_count_word_BB2E3C;
    }

    virtual PathSoundInfo*& sLastLoadedSoundBlockInfo() override
    {
        return mLastLoadedSoundBlockInfo;
    }

    virtual s16& sSFXPitchVariationEnabled() override
    {
        return sSFXPitchVariationEnabled_560F58;
    }

    virtual s16& sNeedToHashSeqNames() override
    {
        return sNeedToHashSeqNames_560F40;
    }

    virtual PathSoundInfo& sMonkVh_Vb() override
    {
        return sMonkVh_Vb_560F48;
    }

    virtual s32 MidiTableSize() override
    {
        return kSeqTableSizeAE;
    }

    virtual s16 FreeResource_Impl(u8* handle) override
    {
        return ResourceManager::FreeResource_Impl_49C360(handle);
    }

    virtual u8** GetLoadedResource(u32 type, u32 resourceID, u16 addUseCount, u16 bLock) override
    {
        return ResourceManager::GetLoadedResource(type, resourceID, addUseCount, bLock);
    }

    virtual s16 FreeResource(u8** handle) override
    {
        return ResourceManager::FreeResource_49C330(handle);
    }

    virtual u8** Allocate_New_Locked_Resource(u32 type, u32 id, u32 size) override
    {
        return ResourceManager::Allocate_New_Locked_Resource(type, id, size);
    }

    virtual void LoadingLoop(s16 bShowLoadingIcon) override
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);
    }

    virtual void Reclaim_Memory(u32 size) override
    {
        ResourceManager::Reclaim_Memory_49C470(size);
    }

    virtual u8** Alloc_New_Resource(u32 type, u32 id, u32 size) override
    {
        return ResourceManager::Alloc_New_Resource_49BED0(type, id, size);
    }

    virtual s16 LoadResourceFile(const char_type* pFileName, Camera* pCamera) override
    {
        return ResourceManager::LoadResourceFile_49C170(pFileName, pCamera);
    }

private:
    PathSoundInfo* mLastLoadedSoundBlockInfo = nullptr;
};

static AEMidiVars sAEMidiVars;
static IMidiVars* spMidiVars = &sAEMidiVars; // Default to AE vars

IMidiVars* GetMidiVars()
{
    return spMidiVars;
}

void SetMidiApiVars(IMidiVars* pVars)
{
    spMidiVars = pVars;
}

void SND_Free_All_VABS_4C9EB0()
{
    PathSoundInfo* pIter = GetMidiVars()->sLastLoadedSoundBlockInfo();
    if (pIter)
    {
        SsVabClose_4FC5B0(pIter->mVabId);
        pIter->mVabId = -1;
    }
    GetMidiVars()->sLastLoadedSoundBlockInfo() = nullptr;
}

void SND_Free_All_Seqs_4C9F40()
{
    for (s32 i = 0; i < GetMidiVars()->MidiTableSize(); i++)
    {
        GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data.clear();
    }
}

void SND_Reset()
{
    SND_Stop_All_Seqs();
    SND_Free_All_Seqs_4C9F40();
    SND_Free_All_VABS_4C9EB0();
    SsSetMVol_4FC360(100, 100);
}


s16 SND_VAB_Load_4C9FE0(PathSoundInfo& pSoundBlockInfo)
{
    // Load the VH file data
    pSoundBlockInfo.mVhFileData = ResourceManagerWrapper::LoadFile(pSoundBlockInfo.mVhFile.c_str(), GetMap().mNextLevel);
    GetMidiVars()->LoadingLoop(0);

    // Load the VB file data
    std::vector<u8> vbFileData = ResourceManagerWrapper::LoadFile(pSoundBlockInfo.mVbFile.c_str(), GetMap().mNextLevel);

    // Convert the records in the header to internal representation
    pSoundBlockInfo.mVabId = SsVabOpenHead_4FC620(reinterpret_cast<VabHeader*>(pSoundBlockInfo.mVhFileData.data()));

    // Load actual sample data (copied, hence vec goes out of scope after this)
    SsVabTransBody_4FC840(reinterpret_cast<VabBodyRecord*>(vbFileData.data()), static_cast<s16>(pSoundBlockInfo.mVabId));

    SsVabTransCompleted_4FE060(SS_WAIT_COMPLETED);

    return 1;
}

// TODO: PSX has VSyncCallback here 0x800592dc

s32 MIDI_Play_Single_Note_4CA1B0(s32 vabIdAndProgram, s32 note, s32 leftVol, s32 rightVol)
{
    // NOTE: word_BB2E40 is used as a guard here, but it is never read anywhere else
    return SsVoKeyOn_4FCF10(vabIdAndProgram, note, static_cast<u16>(leftVol), static_cast<u16>(rightVol));
}

void SND_Init()
{
    SSInit_4FC230();
    SsSetTableSize_4FE0B0(nullptr, 16, 1);
    SsUtSetReverbDepth_4FE380(0, 0);
    SsUtSetReverbType_4FE360(4);
    SsUtReverbOn_4FE340();
    SsSetTickMode_4FDC20(4096);
    VSyncCallback_4F8C40(SND_CallBack_4020A4);
    SpuInitHot_4FC320();
    SsSetMVol_4FC360(100, 100);
    memset(&GetMidiVars()->sSeq_Ids_word(), -1, sizeof(SeqIds));
    GetMidiVars()->sSeqsPlaying_count_word() = 0;
}

// SND_SetMono_NoRefs_4CA310

// SND_SetStereo_NoRefs_4CA330

void SND_Shutdown()
{
    SND_Reset();

    if (GetMidiVars()->sMonkVh_Vb().mVabId >= 0)
    {
        SsVabClose_4FC5B0(GetMidiVars()->sMonkVh_Vb().mVabId);
        GetMidiVars()->sMonkVh_Vb().mVabId = -1;
    }

    SsSetMVol_4FC360(0, 0);
    SsUtReverbOff_4FE350();
    SsUtSetReverbDepth_4FE380(0, 0);
    SsEnd_4FC350();

    // TODO: PSX calls a func that just does SpuQuit() here and nothing else
    SsExt_StopPlayingSamples();

    SsExt_CloseAllVabs();

    GetSoundAPI().SND_SsQuit();
}


void SND_Load_VABS(PathSoundInfo& info, s32 reverb)
{
    GetMidiVars()->sSnd_ReloadAbeResources() = FALSE;
    if (GetMidiVars()->sLastLoadedSoundBlockInfo() != &info)
    {
        SsUtReverbOff_4FE350();
        SsUtSetReverbDepth_4FE380(0, 0);
        SpuClearReverbWorkArea_4FA690(4);

        if (GetMidiVars()->sMonkVh_Vb().mVabId < 0)
        {
            // TODO: Fix me
            SND_VAB_Load_4C9FE0(GetMidiVars()->sMonkVh_Vb());
        }

        GetMidiVars()->sLastLoadedSoundBlockInfo() = &info;

        SND_VAB_Load_4C9FE0(info);

        // Put abes resources back if we had to unload them to fit the VB in memory
        if (GetMidiVars()->sSnd_ReloadAbeResources())
        {
            GetMidiVars()->Reclaim_Memory(0);
            sActiveHero->Load_Basic_Resources_44D460();
        }

        SsUtSetReverbDepth_4FE380(reverb, reverb);
        SsUtReverbOn_4FE340();
    }
}


s32 SFX_SfxDefinition_Play_Mono(const relive::SfxDefinition& sfxDef, s32 volume, s32 pitch_min, s32 pitch_max)
{
    if (!volume)
    {
        volume = sfxDef.field_C_default_volume;
    }

    if (pitch_min == 0x7FFF)
    {
        pitch_min = sfxDef.field_E_pitch_min;
    }

    if (pitch_max == 0x7FFF)
    {
        pitch_max = sfxDef.field_10_pitch_max;
    }

    if (volume < 1)
    {
        volume = 1;
    }
    else if (volume >= 127)
    {
        volume = 127;
    }

    // Note: Inlined in psx
    auto midiHandle = MIDI_Play_Single_Note_4CA1B0(
        sfxDef.field_4_program | (GetMidiVars()->sLastLoadedSoundBlockInfo()[sfxDef.field_0_block_idx].mVabId << 8),
        sfxDef.field_8_note << 8,
        volume,
        volume);

    if (!GetMidiVars()->sSFXPitchVariationEnabled())
    {
        return 0;
    }

    if (pitch_min || pitch_max)
    {
        // Note: Inlined in psx
        SFX_SetPitch_4CA510(sfxDef, midiHandle, Math_RandomRange(static_cast<s16>(pitch_min), static_cast<s16>(pitch_max)));
    }

    return midiHandle;
}


void SFX_SetPitch_4CA510(const relive::SfxDefinition& pSfx, s32 channelsBits, s16 pitch)
{
    s32 v3 = 0;
    s16 v4 = 0;

    if (pitch >= 0)
    {
        v3 = (pitch >> 7) & 0xFFFF;
        v4 = pitch & 127;
    }
    else
    {
        v3 = -1 - (-pitch >> 7);
        v4 = 127 - (-(s8) pitch & 127);
    }

    for (s16 i = 0; i < 24; i++) // TODO: use kNumChannels
    {
        if ((1 << i) & channelsBits)
        {
            const s16 vabId = 0;   // Not used by target func
            const s16 program = 0; // Not used by target func
            SsUtChangePitch_4FDF70(i, program, vabId, static_cast<s16>(pSfx.field_8_note), 0, static_cast<s16>(static_cast<s32>(pSfx.field_8_note) + v3), v4);
        }
    }
}

s32 SND_MIDI(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
{
    s32 volClamped = 0;
    if (vol < 10)
    {
        volClamped = 10;
    }
    else
    {
        volClamped = vol;
        if (vol >= 127)
        {
            volClamped = 127;
        }
    }

    // Note: Inlined in psx
    const s32 channelBits = MIDI_Play_Single_Note_4CA1B0(vabId | ((s16) program << 8), note << 8, volClamped, volClamped);
    if (!GetMidiVars()->sSFXPitchVariationEnabled())
    {
        return 0;
    }

    if (min || max)
    {
        s16 randomValue = Math_RandomRange(min, max);

        s32 v9;  // edi
        s16 v10; // bx
        if (randomValue >= 0)
        {
            v9 = (randomValue >> 7) & 0xFFFF;
            v10 = randomValue & 127;
        }
        else
        {
            v9 = -1 - (-randomValue >> 7);
            v10 = 127 - (-(s8) randomValue & 127);
        }

        for (s16 i = 0; i < 24; i++) // TODO: Use kNumChannels
        {
            if ((1 << i) & channelBits)
            {
                SsUtChangePitch_4FDF70(i, program, vabId, static_cast<s16>(note), 0, static_cast<s16>(v9 + note), v10);
            }
        }
    }
    return channelBits;
}

s32 SFX_SfxDefinition_Play_Stereo(const relive::SfxDefinition& sfxDef, s16 volLeft, s16 volRight, s16 pitch_min, s16 pitch_max)
{
    if (pitch_min == 0x7FFF)
    {
        pitch_min = sfxDef.field_E_pitch_min;
    }

    if (pitch_max == 0x7FFF)
    {
        pitch_max = sfxDef.field_10_pitch_max;
    }

    if (volLeft < 10)
    {
        volLeft = 10;
    }
    else if (volLeft >= 127)
    {
        volLeft = 127;
    }

    if (volRight < 10)
    {
        volRight = 10;
    }
    else if (volRight >= 127)
    {
        volRight = 127;
    }

    // Note: Inlined in psx
    auto midiHandle = MIDI_Play_Single_Note_4CA1B0(
        sfxDef.field_4_program | (GetMidiVars()->sLastLoadedSoundBlockInfo()[sfxDef.field_0_block_idx].mVabId << 8),
        sfxDef.field_8_note << 8,
        volLeft,
        volRight);

    if (!GetMidiVars()->sSFXPitchVariationEnabled())
    {
        return 0;
    }

    if (pitch_min || pitch_max)
    {
        SFX_SetPitch_4CA510(sfxDef, midiHandle, Math_RandomRange(pitch_min, pitch_max));
    }

    return midiHandle;
}

void SND_Stop_Channels_Mask(u32 bitMask)
{
    for (s32 i = 0; i < 24; i++) // TODO: Constant
    {
        // Does the index match a bit in the bitmask?
        if ((1 << i) & bitMask)
        {
            // Turn it off
            GetSpuApiVars()->SsUtKeyOffV(static_cast<s16>(i));
        }
    }
}

void SND_Stop_All_Seqs()
{
    // TODO: Why is there 16 of these but 32 of sMidiStruct2Ary32_C13400? Seems like they should match in size
    GetMidiVars()->sSeqsPlaying_count_word() = 0;
    for (s16 i = 0; i < 16; i++)
    {
        if (GetMidiVars()->sSeq_Ids_word().ids[i] >= 0)
        {
            if (SsIsEos_4FDA80(i, 0))
            {
                SsSeqStop_4FD9C0(i);
            }
            SsSeqClose_4FD8D0(i);
            GetMidiVars()->sSeqDataTable()[GetMidiVars()->sSeq_Ids_word().ids[i]].field_A_id_seqOpenId = -1;
            GetMidiVars()->sSeq_Ids_word().ids[i] = -1;
        }
    }
}


void SND_Seq_Stop_4CA8E0()
{
    for (s16 i = 0; i < 16; i++)
    {
        if (GetMidiVars()->sSeq_Ids_word().ids[i] >= 0)
        {
            if (!SsIsEos_4FDA80(i, 0))
            {
                SsSeqClose_4FD8D0(i);
                GetMidiVars()->sSeqDataTable()[GetMidiVars()->sSeq_Ids_word().ids[i]].field_A_id_seqOpenId = -1;
                GetMidiVars()->sSeq_Ids_word().ids[i] = -1;
                GetMidiVars()->sSeqsPlaying_count_word()--;
            }
        }
    }
}

s16 SND_SEQ_PlaySeq(u16 idx, s16 repeatCount, s16 bDontStop)
{
    OpenSeqHandle& rec = GetMidiVars()->sSeqDataTable()[idx];
    if (rec.field_C_ppSeq_Data.empty())
    {
        return 0;
    }

    if (rec.field_A_id_seqOpenId < 0)
    {
        if (GetMidiVars()->sSeqsPlaying_count_word() >= 16)
        {
            // Note: Inlined in psx
            SND_Seq_Stop_4CA8E0();
            if (GetMidiVars()->sSeqsPlaying_count_word() >= 16)
            {
                return 0;
            }
        }

        const s32 vabId = GetMidiVars()->sLastLoadedSoundBlockInfo()[rec.field_8_sound_block_idx].mVabId;
        rec.field_A_id_seqOpenId = SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data.data(), static_cast<s16>(vabId));

        GetMidiVars()->sSeq_Ids_word().ids[rec.field_A_id_seqOpenId] = idx;
        GetMidiVars()->sSeqsPlaying_count_word()++;
    }
    else if (SsIsEos_4FDA80(rec.field_A_id_seqOpenId, 0))
    {
        if (!bDontStop)
        {
            return 0;
        }
        SsSeqStop_4FD9C0(rec.field_A_id_seqOpenId);
    }

    // Clamp vol
    s16 clampedVol = rec.field_9_volume;
    if (clampedVol <= 10)
    {
        clampedVol = 10;
    }
    else
    {
        if (clampedVol >= 127)
        {
            clampedVol = 127;
        }
    }

    SsSeqSetVol_4FDAC0(rec.field_A_id_seqOpenId, clampedVol, clampedVol);
    if (repeatCount)
    {
        SsSeqPlay_4FD900(rec.field_A_id_seqOpenId, 1, repeatCount);
    }
    else
    {
        SsSeqPlay_4FD900(rec.field_A_id_seqOpenId, 1, 0);
    }

    return 1;
}


s16 SND_SEQ_Play(u16 idx, s16 repeatCount, s16 volLeft, s16 volRight)
{
    OpenSeqHandle& rec = GetMidiVars()->sSeqDataTable()[idx];
    if (rec.field_C_ppSeq_Data.empty())
    {
        return 0;
    }

    // SEQ isn't in use
    if (rec.field_A_id_seqOpenId < 0)
    {
        // Too many playing
        if (GetMidiVars()->sSeqsPlaying_count_word() >= 16)
        {
            // Stop any SEQs that are done
            SND_Seq_Stop_4CA8E0();

            // If none where done then can't continue
            if (GetMidiVars()->sSeqsPlaying_count_word() >= 16)
            {
                return 0;
            }
        }

        // Open the SEQ
        const s16 vabId = static_cast<s16>(GetMidiVars()->sLastLoadedSoundBlockInfo()[rec.field_8_sound_block_idx].mVabId);
        rec.field_A_id_seqOpenId = SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data.data(), vabId);

        // Index into the IDS via the seq ID and map it to the index
        GetMidiVars()->sSeq_Ids_word().ids[rec.field_A_id_seqOpenId] = idx;
        GetMidiVars()->sSeqsPlaying_count_word()++;
    }
    else if (SsIsEos_4FDA80(rec.field_A_id_seqOpenId, 0))
    {
        SsSeqStop_4FD9C0(rec.field_A_id_seqOpenId);
    }

    // Clamp left
    s16 clampedVolLeft = 0;
    if (volLeft < 10)
    {
        clampedVolLeft = 10;
    }
    else if (volLeft >= 127)
    {
        clampedVolLeft = 127;
    }
    else
    {
        clampedVolLeft = volLeft;
    }

    // Clamp right
    s16 clampedVolRight = 0;
    if (volRight < 10)
    {
        clampedVolRight = 10;
    }
    else
    {
        clampedVolRight = volRight;
        if (volRight >= 127)
        {
            clampedVolRight = 127;
        }
    }

    SsSeqSetVol_4FDAC0(rec.field_A_id_seqOpenId, clampedVolLeft, clampedVolRight);

    if (repeatCount)
    {
        SsSeqPlay_4FD900(rec.field_A_id_seqOpenId, 1, repeatCount);
    }
    else
    {
        SsSeqPlay_4FD900(rec.field_A_id_seqOpenId, 1, 0);
    }

    return 1;
}


s32 SND_SsIsEos_DeInlined(u16 idx)
{
    OpenSeqHandle* pRec = &GetMidiVars()->sSeqDataTable()[idx];
    if (pRec->field_A_id_seqOpenId != -1 && !pRec->field_C_ppSeq_Data.empty())
    {
        return SsIsEos_4FDA80(pRec->field_A_id_seqOpenId, 0) != 0;
    }
    return 0;
}


void SND_SEQ_SetVol(s32 idx, s16 volLeft, s16 volRight)
{
    u16 limitedIdx = idx & 0xFFFF;
    if (GetMidiVars()->sSeqDataTable()[limitedIdx].field_A_id_seqOpenId != -1
        && !GetMidiVars()->sSeqDataTable()[limitedIdx].field_C_ppSeq_Data.empty()
        && SND_SsIsEos_DeInlined(limitedIdx))
    {
        SsSeqSetVol_4FDAC0(GetMidiVars()->sSeqDataTable()[limitedIdx].field_A_id_seqOpenId, volLeft, volRight);
    }
}


void SND_SEQ_Stop(u16 idx)
{
    if (GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId != -1 && !GetMidiVars()->sSeqDataTable()[idx].field_C_ppSeq_Data.empty())
    {
        if (SsIsEos_4FDA80(GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId, 0))
        {
            SsSeqStop_4FD9C0(GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId);
        }
    }
}

static u32 GetTableIdxForName(const char_type* pName)
{
    for (s32 i = 0; i < GetMidiVars()->MidiTableSize(); i++)
    {
        auto curName = GetMidiVars()->sSeqDataTable()[i].field_0_mBsqName;
        if (strcmp(curName, pName) == 0)
        {
            return i;
        }
    }
    ALIVE_FATAL("Couldn't find seq name in the table");
}

void SND_Load_Seqs_Impl(OpenSeqHandle* pSeqTable, PathSoundInfo& info)
{
    if (pSeqTable)
    {
        GetMidiVars()->sSeqDataTable() = pSeqTable;

        // Generate resource ids from hashing the name if we haven't already
        if (GetMidiVars()->sNeedToHashSeqNames())
        {
            for (s32 i = 0; i < GetMidiVars()->MidiTableSize(); i++)
            {
                GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data.clear();
                GetMidiVars()->sSeqDataTable()[i].field_A_id_seqOpenId = -1;
                GetMidiVars()->sSeqDataTable()[i].field_4_generated_res_id = ResourceManager::SEQ_HashName_49BE30(GetMidiVars()->sSeqDataTable()[i].field_0_mBsqName);
            }
            GetMidiVars()->sNeedToHashSeqNames() = FALSE;
        }

        GetMidiVars()->Reclaim_Memory(0);

        // Get a pointer to each SEQ
        for (const auto& seqName : info.mSeqFiles)
        {
            auto buffer = ResourceManagerWrapper::LoadFile(seqName.c_str(), GetMap().mNextLevel);

            // We have to insert into the table at the position that matches the file name
            GetMidiVars()->sSeqDataTable()[GetTableIdxForName(seqName.c_str())].field_C_ppSeq_Data = buffer;
        }
    }
}

void SND_Load_Seqs(OpenSeqHandle* pSeqTable, PathSoundInfo& bsqFileName)
{
    SND_Load_Seqs_Impl(pSeqTable, bsqFileName);
}

s8 SND_Seq_Table_Valid()
{
    return GetMidiVars()->sSeqDataTable() != 0;
}

// So AO can redirect SND_StopAll_4CB060 to its own func when called from SYS_ funcs
void SND_StopAll_SetCallBack(TSNDStopAll cb)
{
    sSNDStopAllCallBack = cb;
}

void SND_StopAll()
{
    if (sSNDStopAllCallBack)
    {
        sSNDStopAllCallBack();
    }
    else
    {
        MusicController::static_EnableMusic(FALSE);
        BackgroundMusic::Stop();
        SND_Reset_Ambiance();
        SND_Stop_All_Seqs();
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (!pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                pObj->VStopAudio();
            }
        }
        SsUtAllKeyOff_4FDFE0(0);
    }
}


// So AO can redirect SND_Restart_4CB0E0 to its own func when called from SYS_ funcs
void SND_Restart_SetCallBack(TSNDRestart cb)
{
    sSNDRestartCallBack = cb;
}

void SND_Restart_4CB0E0()
{
    if (sSNDRestartCallBack)
    {
        sSNDRestartCallBack();
    }
    else
    {
        MusicController::static_EnableMusic(TRUE);
        BackgroundMusic::Play();
        Start_Sounds_For_Objects_In_Near_Cameras();
    }
}

// Next -> Background music object

// Next -> Scoped seq object

// Next -> SND_Init_Ambiance, SND_Reset_Ambiance, Start_Sounds_for_TLV, Start_Slig_sounds
// Stop_slig_sounds_4CBA70 Path::Start_Sounds_For_Objects_In_Camera_4CBAF0, Start_Sounds_For_Objects_In_Near_Cameras
