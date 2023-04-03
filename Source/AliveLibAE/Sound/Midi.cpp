#include "stdafx.h"
#include "Sound/Midi.hpp"
#include "Function.hpp"
#include "Sound/Sound.hpp"
#include "PathData.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "LvlArchive.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "Sys.hpp"
#include "Io.hpp"

#include "Sfx.hpp"
#include "PathData.hpp"

#include "PsxSpuApi.hpp"
#include "AmbientSound.hpp"

#if !AUDIO_SPU_EMULATION
EXPORT void CC SFX_SetPitch_4CA510(const SfxDefinition* pSfx, s32 channelsBits, s16 pitch);

const s32 kSeqTableSizeAE = 144;

static TSNDStopAll sSNDStopAllCallBack = nullptr;
static TSNDRestart sSNDRestartCallBack = nullptr;

ALIVE_VAR(1, 0xBB2354, SeqIds, sSeq_Ids_word_BB2354, {});
ALIVE_VAR(1, 0xbb2e3e, u16, sSnd_ReloadAbeResources_BB2E3E, 0);
ALIVE_VAR(1, 0xbb2e38, OpenSeqHandle*, sSeqDataTable_BB2E38, nullptr);
ALIVE_VAR(1, 0xbb2e3c, s16, sSeqsPlaying_count_word_BB2E3C, 0);
ALIVE_VAR(1, 0xbb2e34, SoundBlockInfo*, sLastLoadedSoundBlockInfo_BB2E34, nullptr);
ALIVE_VAR(1, 0x560f58, s16, sSFXPitchVariationEnabled_560F58, true);
ALIVE_VAR(1, 0x560f40, s16, sNeedToHashSeqNames_560F40, 1);

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
const SoundBlockInfo soundBlock = {"MONK.VH", "MONK.VB", -1, nullptr};

ALIVE_VAR(1, 0x560F48, SoundBlockInfo, sMonkVh_Vb_560F48, soundBlock);

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

    virtual SoundBlockInfo*& sLastLoadedSoundBlockInfo() override
    {
        return sLastLoadedSoundBlockInfo_BB2E34;
    }

    virtual s16& sSFXPitchVariationEnabled() override
    {
        return sSFXPitchVariationEnabled_560F58;
    }

    virtual s16& sNeedToHashSeqNames() override
    {
        return sNeedToHashSeqNames_560F40;
    }

    virtual SoundBlockInfo& sMonkVh_Vb() override
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
        return ResourceManager::GetLoadedResource_49C2A0(type, resourceID, addUseCount, bLock);
    }

    virtual s16 FreeResource(u8** handle) override
    {
        return ResourceManager::FreeResource_49C330(handle);
    }

    virtual u8** Allocate_New_Locked_Resource(u32 type, u32 id, u32 size) override
    {
        return ResourceManager::Allocate_New_Locked_Resource_49BF40(type, id, size);
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
};

static AEMidiVars sAEMidiVars;
static IMidiVars* spMidiVars = &sAEMidiVars; // Default to AE vars

EXPORT IMidiVars* GetMidiVars()
{
    return spMidiVars;
}

EXPORT void SetMidiApiVars(IMidiVars* pVars)
{
    spMidiVars = pVars;
}

EXPORT void CC SND_Free_All_VABS_4C9EB0()
{
    SoundBlockInfo* pIter = GetMidiVars()->sLastLoadedSoundBlockInfo();
    while (pIter && pIter->field_4_vab_body_name)
    {
        GetMidiVars()->FreeResource_Impl(pIter->field_C_pVabHeader);
        pIter->field_C_pVabHeader = nullptr;
        SsVabClose_4FC5B0(pIter->field_8_vab_id);
        pIter->field_8_vab_id = -1;
        pIter++;
    }
    GetMidiVars()->sLastLoadedSoundBlockInfo() = nullptr;
}

EXPORT void CC SND_Free_All_Seqs_4C9F40()
{
    for (s32 i = 0; i < GetMidiVars()->MidiTableSize(); i++)
    {
        if (GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data)
        {
            u8** ppRes = GetMidiVars()->GetLoadedResource(ResourceManager::Resource_Seq, GetMidiVars()->sSeqDataTable()[i].field_4_generated_res_id, 0, 0);
            GetMidiVars()->FreeResource(ppRes);
            GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data = nullptr;
        }
    }
}

EXPORT void SND_Reset_4C9FB0()
{
    SND_Stop_All_Seqs_4CA850();
    SND_Free_All_Seqs_4C9F40();
    SND_Free_All_VABS_4C9EB0();
    SsSetMVol_4FC360(100, 100);
}


EXPORT s16 CC SND_VAB_Load_4C9FE0(SoundBlockInfo* pSoundBlockInfo, s16 vabId)
{
    // Fail if no file name
    if (!pSoundBlockInfo->field_0_vab_header_name)
    {
        return 0;
    }

    // Find the VH file record
    s32 headerSize = 0;
    LvlFileRecord* pVabHeaderFile = sLvlArchive_5BC520.Find_File_Record_433160(pSoundBlockInfo->field_0_vab_header_name);
    if (sbEnable_PCOpen_5CA4B0)
    {
        headerSize = pVabHeaderFile->field_14_file_size;
    }
    else
    {
        headerSize = pVabHeaderFile->field_10_num_sectors << 11;
    }

    // Load the VH file data
    u8** ppVabHeader = GetMidiVars()->Allocate_New_Locked_Resource(ResourceManager::Resource_VabHeader, vabId, headerSize);
    pSoundBlockInfo->field_C_pVabHeader = *ppVabHeader;
    sLvlArchive_5BC520.Read_File_4330A0(pVabHeaderFile, *ppVabHeader);
    GetMidiVars()->LoadingLoop(0);

    // Find the VB file record
    LvlFileRecord* pVabBodyFile = sLvlArchive_5BC520.Find_File_Record_433160(pSoundBlockInfo->field_4_vab_body_name);
    if (!pVabBodyFile)
    {
        // For some reason its acceptable to assume we have a VH with no VB, but the VH must always exist, this happens for MONK.VB
        return 0;
    }

    s32 vabBodySize = 0;
    if (sbEnable_PCOpen_5CA4B0)
    {
        vabBodySize = pVabBodyFile->field_14_file_size;
    }
    else
    {
        vabBodySize = pVabBodyFile->field_10_num_sectors << 11; // TODO * 4096 ?
    }

    // Load the VB file data
    u8** ppVabBody = GetMidiVars()->Alloc_New_Resource(ResourceManager::Resource_VabBody, vabId, vabBodySize);
    if (!ppVabBody)
    {
        // Maybe filed due to OOM cause its huge, free the abe resources and try again
        if (!GetMidiVars()->sSnd_ReloadAbeResources())
        {
            GetMidiVars()->sSnd_ReloadAbeResources() = TRUE;
            sActiveHero_5C1B68->Free_Resources_44D420();
        }

        // Compact/reclaim any other memory we can too
        GetMidiVars()->Reclaim_Memory(0);

        // If it fails again there is no recovery, in either case caller will restore abes resources
        ppVabBody = GetMidiVars()->Alloc_New_Resource(ResourceManager::Resource_VabBody, vabId, vabBodySize);
        if (!ppVabBody)
        {
            return 0;
        }
    }

    // Now we can read the actual VB data
    sLvlArchive_5BC520.Read_File_4330A0(pVabBodyFile, *ppVabBody);

    // Convert the records in the header to internal representation
    pSoundBlockInfo->field_8_vab_id = SsVabOpenHead_4FC620(reinterpret_cast<VabHeader*>(pSoundBlockInfo->field_C_pVabHeader));

    // Load actual sample data

    SsVabTransBody_4FC840(reinterpret_cast<VabBodyRecord*>(*ppVabBody), static_cast<s16>(pSoundBlockInfo->field_8_vab_id));

    SsVabTransCompleted_4FE060(SS_WAIT_COMPLETED);

    // Now the sound samples are loaded we don't need the VB data anymore
    GetMidiVars()->FreeResource(ppVabBody);
    return 1;
}

// TODO: PSX has VSyncCallback here 0x800592dc

EXPORT s32 CC MIDI_Play_Single_Note_4CA1B0(s32 vabIdAndProgram, s32 note, s32 leftVol, s32 rightVol)
{
    // NOTE: word_BB2E40 is used as a guard here, but it is never read anywhere else
    return SsVoKeyOn_4FCF10(vabIdAndProgram, note, static_cast<u16>(leftVol), static_cast<u16>(rightVol));
}

EXPORT void CC SND_Init_4CA1F0()
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

EXPORT void SND_Shutdown_4CA280()
{
    SND_Reset_4C9FB0();

    if (GetMidiVars()->sMonkVh_Vb().field_8_vab_id >= 0)
    {
        GetMidiVars()->FreeResource_Impl(GetMidiVars()->sMonkVh_Vb().field_C_pVabHeader);
        GetMidiVars()->sMonkVh_Vb().field_C_pVabHeader = nullptr;

        SsVabClose_4FC5B0(GetMidiVars()->sMonkVh_Vb().field_8_vab_id);
        GetMidiVars()->sMonkVh_Vb().field_8_vab_id = -1;
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


EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, s32 reverb)
{
    SoundBlockInfo* pSoundBlockInfoIter = pSoundBlockInfo;
    GetMidiVars()->sSnd_ReloadAbeResources() = FALSE;
    if (GetMidiVars()->sLastLoadedSoundBlockInfo() != pSoundBlockInfo)
    {
        SsUtReverbOff_4FE350();
        SsUtSetReverbDepth_4FE380(0, 0);
        SpuClearReverbWorkArea_4FA690(4);

        if (GetMidiVars()->sMonkVh_Vb().field_8_vab_id < 0)
        {
            SND_VAB_Load_4C9FE0(&GetMidiVars()->sMonkVh_Vb(), 32);
        }

        GetMidiVars()->sLastLoadedSoundBlockInfo() = pSoundBlockInfo;

        s16 vabId = 0;
        while (SND_VAB_Load_4C9FE0(pSoundBlockInfoIter, vabId))
        {
            ++vabId;
            ++pSoundBlockInfoIter;
        }

        // Put abes resources back if we had to unload them to fit the VB in memory
        if (GetMidiVars()->sSnd_ReloadAbeResources())
        {
            GetMidiVars()->Reclaim_Memory(0);
            sActiveHero_5C1B68->Load_Basic_Resources_44D460();
        }

        SsUtSetReverbDepth_4FE380(reverb, reverb);
        SsUtReverbOn_4FE340();
    }
}


s32 CC SFX_SfxDefinition_Play_4CA420(const SfxDefinition* sfxDef, s16 volume, s16 pitch_min, s16 pitch_max)
{
    if (!volume)
    {
        volume = sfxDef->field_3_default_volume;
    }

    if (pitch_min == 0x7FFF)
    {
        pitch_min = sfxDef->field_4_pitch_min;
    }

    if (pitch_max == 0x7FFF)
    {
        pitch_max = sfxDef->field_6_pitch_max;
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
        sfxDef->field_1_program | (GetMidiVars()->sLastLoadedSoundBlockInfo()[sfxDef->field_0_block_idx].field_8_vab_id << 8),
        sfxDef->field_2_note << 8,
        volume,
        volume);

    if (!GetMidiVars()->sSFXPitchVariationEnabled())
    {
        return 0;
    }

    if (pitch_min || pitch_max)
    {
        // Note: Inlined in psx
        SFX_SetPitch_4CA510(sfxDef, midiHandle, Math_RandomRange_496AB0(pitch_min, pitch_max));
    }

    return midiHandle;
}


EXPORT void CC SFX_SetPitch_4CA510(const SfxDefinition* pSfx, s32 channelsBits, s16 pitch)
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
            SsUtChangePitch_4FDF70(i, program, vabId, pSfx->field_2_note, 0, static_cast<s16>(static_cast<s32>(pSfx->field_2_note) + v3), v4);
        }
    }
}

EXPORT s32 CC SND_4CA5D0(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
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
        s16 randomValue = Math_RandomRange_496AB0(min, max);

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

s32 CC SFX_SfxDefinition_Play_4CA700(const SfxDefinition* sfxDef, s16 volLeft, s16 volRight, s16 pitch_min, s16 pitch_max)
{
    if (pitch_min == 0x7FFF)
    {
        pitch_min = sfxDef->field_4_pitch_min;
    }

    if (pitch_max == 0x7FFF)
    {
        pitch_max = sfxDef->field_6_pitch_max;
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
        sfxDef->field_1_program | (GetMidiVars()->sLastLoadedSoundBlockInfo()[sfxDef->field_0_block_idx].field_8_vab_id << 8),
        sfxDef->field_2_note << 8,
        volLeft,
        volRight);

    if (!GetMidiVars()->sSFXPitchVariationEnabled())
    {
        return 0;
    }

    if (pitch_min || pitch_max)
    {
        SFX_SetPitch_4CA510(sfxDef, midiHandle, Math_RandomRange_496AB0(pitch_min, pitch_max));
    }

    return midiHandle;
}

EXPORT void CC SND_Stop_Channels_Mask_4CA810(u32 bitMask)
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

EXPORT void SND_Stop_All_Seqs_4CA850()
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


EXPORT void SND_Seq_Stop_4CA8E0()
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

EXPORT s16 CC SND_SEQ_PlaySeq_4CA960(u16 idx, s16 repeatCount, s16 bDontStop)
{
    OpenSeqHandle& rec = GetMidiVars()->sSeqDataTable()[idx];
    if (!rec.field_C_ppSeq_Data)
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

        const s32 vabId = GetMidiVars()->sLastLoadedSoundBlockInfo()[rec.field_8_sound_block_idx].field_8_vab_id;
        rec.field_A_id_seqOpenId = SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data, static_cast<s16>(vabId));

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


EXPORT s16 CC SND_SEQ_Play_4CAB10(u16 idx, s16 repeatCount, s16 volLeft, s16 volRight)
{
    OpenSeqHandle& rec = GetMidiVars()->sSeqDataTable()[idx];
    if (!rec.field_C_ppSeq_Data)
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
        const s16 vabId = static_cast<s16>(GetMidiVars()->sLastLoadedSoundBlockInfo()[rec.field_8_sound_block_idx].field_8_vab_id);
        rec.field_A_id_seqOpenId = SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data, vabId);

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


EXPORT s32 CC SND_SsIsEos_DeInlined_4CACD0(u16 idx)
{
    OpenSeqHandle* pRec = &GetMidiVars()->sSeqDataTable()[idx];
    if (pRec->field_A_id_seqOpenId != -1 && pRec->field_C_ppSeq_Data)
    {
        return SsIsEos_4FDA80(pRec->field_A_id_seqOpenId, 0) != 0;
    }
    return 0;
}


EXPORT void CC SND_SEQ_SetVol_4CAD20(s32 idx, s16 volLeft, s16 volRight)
{
    u16 limitedIdx = idx & 0xFFFF;
    if (GetMidiVars()->sSeqDataTable()[limitedIdx].field_A_id_seqOpenId != -1
        && GetMidiVars()->sSeqDataTable()[limitedIdx].field_C_ppSeq_Data
        && SND_SsIsEos_DeInlined_4CACD0(limitedIdx))
    {
        SsSeqSetVol_4FDAC0(GetMidiVars()->sSeqDataTable()[limitedIdx].field_A_id_seqOpenId, volLeft, volRight);
    }
}


EXPORT void CC SND_SEQ_Stop_4CAE60(u16 idx)
{
    if (GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId != -1 && GetMidiVars()->sSeqDataTable()[idx].field_C_ppSeq_Data)
    {
        if (SsIsEos_4FDA80(GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId, 0))
        {
            SsSeqStop_4FD9C0(GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId);
        }
    }
}

EXPORT void CC SND_Load_Seqs_Impl(OpenSeqHandle* pSeqTable, const char_type* bsqFileName)
{
    if (pSeqTable && bsqFileName)
    {
        GetMidiVars()->sSeqDataTable() = pSeqTable;

        // Generate resource ids from hashing the name if we haven't already
        if (GetMidiVars()->sNeedToHashSeqNames())
        {
            for (s32 i = 0; i < GetMidiVars()->MidiTableSize(); i++)
            {
                GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data = nullptr;
                GetMidiVars()->sSeqDataTable()[i].field_A_id_seqOpenId = -1;
                GetMidiVars()->sSeqDataTable()[i].field_4_generated_res_id = ResourceManager::SEQ_HashName_49BE30(GetMidiVars()->sSeqDataTable()[i].field_0_mBsqName);
            }
            GetMidiVars()->sNeedToHashSeqNames() = FALSE;
        }

        // Load the BSQ
        GetMidiVars()->Reclaim_Memory(0);
        GetMidiVars()->LoadResourceFile(bsqFileName, nullptr);

        // Get a pointer to each SEQ
        for (s32 i = 0; i < GetMidiVars()->MidiTableSize(); i++)
        {
            u8** ppSeq = GetMidiVars()->GetLoadedResource(ResourceManager::Resource_Seq, GetMidiVars()->sSeqDataTable()[i].field_4_generated_res_id, 1, 1);
            if (ppSeq)
            {
                GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data = *ppSeq;
            }
            else
            {
                GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data = nullptr;
            }
        }
    }
}

EXPORT void CC SND_Load_Seqs_4CAED0(OpenSeqHandle* pSeqTable, const char_type* bsqFileName)
{
    SND_Load_Seqs_Impl(pSeqTable, bsqFileName);
}

EXPORT s8 CC SND_Seq_Table_Valid_4CAFE0()
{
    return GetMidiVars()->sSeqDataTable() != 0;
}

// So AO can redirect SND_StopAll_4CB060 to its own func when called from SYS_ funcs
EXPORT void SND_StopAll_SetCallBack(TSNDStopAll cb)
{
    sSNDStopAllCallBack = cb;
}

EXPORT void CC SND_StopAll_4CB060()
{
    if (sSNDStopAllCallBack)
    {
        sSNDStopAllCallBack();
    }
    else
    {
        MusicController::EnableMusic_47FE10(FALSE);
        BackgroundMusic::Stop_4CB000();
        SND_Reset_Ambiance_4CB4B0();
        SND_Stop_All_Seqs_4CA850();
        for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (!pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                pObj->VStopAudio();
            }
        }
        SsUtAllKeyOff_4FDFE0(0);
    }
}


// So AO can redirect SND_Restart_4CB0E0 to its own func when called from SYS_ funcs
EXPORT void SND_Restart_SetCallBack(TSNDRestart cb)
{
    sSNDRestartCallBack = cb;
}

EXPORT void CC SND_Restart_4CB0E0()
{
    if (sSNDRestartCallBack)
    {
        sSNDRestartCallBack();
    }
    else
    {
        MusicController::EnableMusic_47FE10(TRUE);
        BackgroundMusic::Play_4CB030();
        Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
    }
}

// Next -> Background music object

// Next -> Scoped seq object

// Next -> SND_Init_Ambiance_4CB480, SND_Reset_Ambiance_4CB4B0, Start_Sounds_for_TLV_4CB530, Start_Slig_sounds_4CB980
// Stop_slig_sounds_4CBA70 Path::Start_Sounds_For_Objects_In_Camera_4CBAF0, Start_Sounds_For_Objects_In_Near_Cameras_4CBB60
#else

#include "../../AliveLibCommon/audio/MidiPlayer.hpp"

class AEResourceProvider : public psx::ResourceProvider
{
public:
    psx::ResourceData* readFile(char_type* name)
    {
        LvlFileRecord* fileRecord = sLvlArchive_5BC520.Find_File_Record_433160(name);
        s32 size = fileRecord->field_10_num_sectors << 11;
        u8* data = new u8[size];
        sLvlArchive_5BC520.Read_File_4330A0(fileRecord, data);

        psx::ResourceData* resource = new psx::ResourceData();
        resource->data = data;
        resource->size = size;
        return resource;
    }

    psx::ResourceData* readSeq(const char_type* fileName, const char_type* sequenceName)
    {
        psx::ResourceData* resource = new psx::ResourceData();
        resource->data = 0;
        resource->size = 0;

        if (!sequenceName || sequenceName == nullptr)
        {
            return resource;
        }

        // TODO - maybe this load should be stored in memory for faster access?
        //  Need to check underlying implementation
        if (mLoadedFile != fileName)
        {
            mLoadedFile = fileName;
            ResourceManager::LoadResourceFile_49C170(fileName, nullptr);
        }

        s32 hash = ResourceManager::SEQ_HashName_49BE30(sequenceName);
        u8** ppSeq = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Seq, hash, 1, 1);
        if (!ppSeq)
        {
            return resource;
        }

        u32 size = ResourceManager::Get_Header_49C410(ppSeq)->field_0_size;
        resource->data = *ppSeq;
        resource->size = size;
        return resource;
    }

    s32 sequenceCount()
    {
        return 144;
    }

private:
    const char_type* mLoadedFile = NULL;
};

/*
    AE is different than AO. AE reads sample data from a separate
    sounds.dat file whereas AO reads it inline of the vab body.
*/
class AESoundSampleParser : public psx::SoundSampleParser
{
public:
    std::vector<psx::Sample*> parseSamples(psx::VabHeader* vabHeader, u8* ppVabBody)
    {
        std::vector<psx::Sample*> samples;
        

        std::ifstream soundDatFile;
        soundDatFile.open("sounds.dat", std::ios::binary);
        if (!soundDatFile.is_open())
        {
            throw std::invalid_argument("Could not find sounds.dat");
        }
        soundDatFile.seekg(0, std::ios::end);
        std::streamsize fileSize = soundDatFile.tellg();
        soundDatFile.seekg(0, std::ios::beg);
        char* soundData = new char[(s32) fileSize + 1]; // Plus one, just in case interpolating tries to go that one byte further!

        if (soundDatFile.read(soundData, fileSize))
        {
            // Tada!
        }

        int pos = 0;
        for (s32 i = 0; i < vabHeader->field_16_num_vags; i++)
        {
            u32 size = *reinterpret_cast<u32*>(&ppVabBody[pos]);
            pos += sizeof(u32);

            u32 sampleRate = *reinterpret_cast<u32*>(&ppVabBody[pos]);
            pos += sizeof(u32);

            // In AO the body is at this part.
            // IN AE instead it's an offset in sounds.dat
            u32 offset = *reinterpret_cast<u32*>(&ppVabBody[pos]);
            pos += sizeof(u32);

            // Read data from sounds.dat
            u8* data = new u8[size];
            memcpy(data, &soundData[offset], size);

            psx::Sample* sample = new psx::Sample();
            sample->m_SampleBuffer = reinterpret_cast<s16*>(data);
            sample->i_SampleSize = size / 2;
            sample->sampleRate = 44100;        // non standard? Doesn't use sampleRate field?
            sample->loop = sampleRate > 44100; // non-standard?
            samples.push_back(sample);
        }

        soundDatFile.close();

        return samples;
    }
};

psx::MidiPlayer* player = new psx::MidiPlayer(new AEResourceProvider(), new AESoundSampleParser());

EXPORT void CC SND_StopAll_4CB060()
{
    MusicController::EnableMusic_47FE10(FALSE);
    BackgroundMusic::Stop_4CB000();
    player->SND_StopAll();
}

EXPORT void CC SND_Init_4CA1F0()
{
    player->SND_Init();
}

EXPORT void CC SND_Shutdown_4CA280()
{
    player->SND_Shutdown();
}

EXPORT void CC SND_Stop_Channels_Mask_4CA810(u32 bitMask)
{
    bitMask;
    player->SND_Stop_Channels_Mask(bitMask);
}

EXPORT void SND_Reset_4C9FB0()
{
    player->SND_Reset();
}

EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, s32 reverb)
{
    pSoundBlockInfo;
    reverb;
    player->SND_Load_VABS(reinterpret_cast<psx::SoundBlockInfo*>(pSoundBlockInfo), reverb);
}

EXPORT s32 CC SND_4CA5D0(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
{
    return player->SND(program, vabId, note, vol, min, max);
}

EXPORT void CC SND_Restart_4CB0E0()
{
    MusicController::EnableMusic_47FE10(TRUE);
    BackgroundMusic::Play_4CB030();
    Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
    player->SND_Restart();
}

EXPORT void CC SND_Load_Seqs_4CAED0(OpenSeqHandle* pSeqTable, const char_type* bsqFileName)
{
    player->SND_Load_Seqs(reinterpret_cast<psx::OpenSeqHandle*>(pSeqTable), bsqFileName);
}

EXPORT void CC SND_SEQ_Stop_4CAE60(u16 idx)
{
    player->SND_SEQ_Stop(idx);
}

EXPORT s8 CC SND_Seq_Table_Valid_4CAFE0()
{
    return player->SND_Seq_Table_Valid();
}

EXPORT s16 CC SND_SEQ_PlaySeq_4CA960(u16 idx, s16 repeatCount, s16 bDontStop)
{
    return player->SND_SEQ_PlaySeq(idx, repeatCount, bDontStop);
}

EXPORT void CC SND_SEQ_SetVol_4CAD20(s32 idx, s16 volLeft, s16 volRight)
{
    player->SND_SEQ_SetVol(idx, volLeft, volRight);
}

EXPORT s16 CC SND_SEQ_Play_4CAB10(u16 idx, s16 repeatCount, s16 volLeft, s16 volRight)
{
    return player->SND_SEQ_Play(idx, repeatCount, volLeft, volRight);
}

EXPORT s32 CC SND_SsIsEos_DeInlined_4CACD0(u16 idx)
{
    return player->SND_SsIsEos_DeInlined(idx);
}

EXPORT s32 CC SFX_SfxDefinition_Play_4CA700(const SfxDefinition* sfxDef, s16 volLeft, s16 volRight, s16 pitch_min, s16 pitch_max)
{
    return player->SFX_SfxDefinition_Play(reinterpret_cast<psx::SfxDefinition*>(const_cast<SfxDefinition*>(sfxDef)), volLeft, volRight, pitch_min, pitch_max);
}

EXPORT s32 CC SFX_SfxDefinition_Play_4CA420(const SfxDefinition* sfxDef, s16 volume, s16 pitch_min, s16 pitch_max)
{
    return player->SFX_SfxDefinition_Play(reinterpret_cast<psx::SfxDefinition*>(const_cast<SfxDefinition*>(sfxDef)), volume, pitch_min, pitch_max);
} 

#endif