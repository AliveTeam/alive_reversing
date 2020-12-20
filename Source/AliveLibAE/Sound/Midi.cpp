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


EXPORT void CC SFX_SetPitch_4CA510(const SfxDefinition* pSfx, int channelsBits, __int16 pitch);

const int kSeqTableSizeAE = 144;

static TSNDStopAll sSNDStopAllCallBack = nullptr;
static TSNDRestart sSNDRestartCallBack = nullptr;

ALIVE_VAR(1, 0xBB2354, SeqIds, sSeq_Ids_word_BB2354, {});
ALIVE_VAR(1, 0xbb2e3e, WORD, sSnd_ReloadAbeResources_BB2E3E, 0);
ALIVE_VAR(1, 0xbb2e38, OpenSeqHandle *, sSeqDataTable_BB2E38, nullptr);
ALIVE_VAR(1, 0xbb2e3c, __int16, sSeqsPlaying_count_word_BB2E3C, 0);
ALIVE_VAR(1, 0xbb2e34, SoundBlockInfo *, sLastLoadedSoundBlockInfo_BB2E34, nullptr);
ALIVE_VAR(1, 0x560f58, __int16, sSFXPitchVariationEnabled_560F58, true);
ALIVE_VAR(1, 0x560f40, short, sNeedToHashSeqNames_560F40, 1);

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
const SoundBlockInfo soundBlock = { "MONK.VH", "MONK.VB", -1, nullptr };

ALIVE_VAR(1, 0x560F48, SoundBlockInfo, sMonkVh_Vb_560F48, soundBlock);

class AEMidiVars : public IMidiVars
{
public:
    virtual SeqIds& sSeq_Ids_word() override
    {
        return sSeq_Ids_word_BB2354;
    }

    virtual WORD& sSnd_ReloadAbeResources() override
    {
        return sSnd_ReloadAbeResources_BB2E3E;
    }

    virtual OpenSeqHandle*& sSeqDataTable() override
    {
        return sSeqDataTable_BB2E38;
    }

    virtual __int16& sSeqsPlaying_count_word() override
    {
        return sSeqsPlaying_count_word_BB2E3C;
    }

    virtual SoundBlockInfo*& sLastLoadedSoundBlockInfo() override
    {
        return sLastLoadedSoundBlockInfo_BB2E34;
    }

    virtual __int16& sSFXPitchVariationEnabled() override
    {
        return sSFXPitchVariationEnabled_560F58;
    }

    virtual short& sNeedToHashSeqNames() override
    {
        return sNeedToHashSeqNames_560F40;
    }

    virtual SoundBlockInfo& sMonkVh_Vb() override
    {
        return sMonkVh_Vb_560F48;
    }

    virtual int MidiTableSize() override
    {
        return kSeqTableSizeAE;
    }

    virtual signed __int16 FreeResource_Impl(BYTE* handle) override
    {
        return ResourceManager::FreeResource_Impl_49C360(handle);
    }

    virtual BYTE** GetLoadedResource(DWORD type, DWORD resourceID, unsigned __int16 addUseCount, unsigned __int16 bLock) override
    {
        return ResourceManager::GetLoadedResource_49C2A0(type, resourceID, addUseCount, bLock);
    }

    virtual signed __int16 FreeResource(BYTE** handle) override
    {
        return ResourceManager::FreeResource_49C330(handle);
    }

    virtual BYTE** Allocate_New_Locked_Resource(DWORD type, DWORD id, DWORD size) override
    {
        return ResourceManager::Allocate_New_Locked_Resource_49BF40(type, id, size);
    }

    virtual void LoadingLoop(__int16 bShowLoadingIcon) override
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);
    }

    virtual void Reclaim_Memory(unsigned int size) override
    {
        ResourceManager::Reclaim_Memory_49C470(size);
    }

    virtual BYTE** Alloc_New_Resource(DWORD type, DWORD id, DWORD size) override
    {
        return ResourceManager::Alloc_New_Resource_49BED0(type, id, size);
    }

    virtual signed __int16 LoadResourceFile(const char* pFileName, Camera* pCamera) override
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
    for (int i = 0; i < GetMidiVars()->MidiTableSize(); i++)
    {
        if (GetMidiVars()->sSeqDataTable()[i].field_C_ppSeq_Data)
        {
            BYTE** ppRes = GetMidiVars()->GetLoadedResource(ResourceManager::Resource_Seq, GetMidiVars()->sSeqDataTable()[i].field_4_generated_res_id, 0, 0);
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


EXPORT signed __int16 CC SND_VAB_Load_4C9FE0(SoundBlockInfo* pSoundBlockInfo, __int16 vabId)
{
    // Fail if no file name
    if (!pSoundBlockInfo->field_0_vab_header_name)
    {
        return 0;
    }
    
    // Find the VH file record
    int headerSize = 0;
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
    BYTE** ppVabHeader = GetMidiVars()->Allocate_New_Locked_Resource(ResourceManager::Resource_VabHeader, vabId, headerSize);
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

    int vabBodySize = 0;
    if (sbEnable_PCOpen_5CA4B0)
    {
        vabBodySize = pVabBodyFile->field_14_file_size;
    }
    else
    {
        vabBodySize = pVabBodyFile->field_10_num_sectors << 11; // TODO * 4096 ?
    }

    // Load the VB file data
    BYTE** ppVabBody = GetMidiVars()->Alloc_New_Resource(ResourceManager::Resource_VabBody, vabId, vabBodySize);
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

    SsVabTransBody_4FC840(reinterpret_cast<VabBodyRecord*>(*ppVabBody), static_cast<short>(pSoundBlockInfo->field_8_vab_id));

    SsVabTransCompleted_4FE060(SS_WAIT_COMPLETED);

    // Now the sound samples are loaded we don't need the VB data anymore
    GetMidiVars()->FreeResource(ppVabBody);
    return 1;
}

// TODO: PSX has VSyncCallback here 0x800592dc

EXPORT int CC MIDI_Play_Single_Note_4CA1B0(int vabIdAndProgram, int note, int leftVol, int rightVol)
{
    // NOTE: word_BB2E40 is used as a guard here, but it is never read anywhere else
    return SsVoKeyOn_4FCF10(vabIdAndProgram, note, static_cast<unsigned short>(leftVol), static_cast<unsigned short>(rightVol));
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


EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, int reverb)
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

        __int16 vabId = 0;
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


int CC SFX_SfxDefinition_Play_4CA420(const SfxDefinition* sfxDef, __int16 volume, __int16 pitch_min, __int16 pitch_max)
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


EXPORT void CC SFX_SetPitch_4CA510(const SfxDefinition* pSfx, int channelsBits, __int16 pitch)
{
    int v3 = 0;
    __int16 v4 = 0;

    if (pitch >= 0)
    {
        v3 = (pitch >> 7) & 0xFFFF;
        v4 = pitch & 127;
    }
    else
    {
        v3 = -1 - (-pitch >> 7);
        v4 = 127 - (-(char)pitch & 127);
    }

    for (short i = 0; i < 24; i++) // TODO: use kNumChannels
    {
        if ((1 << i) & channelsBits)
        {
            const short vabId = 0; // Not used by target func
            const short program = 0; // Not used by target func
            SsUtChangePitch_4FDF70(i, program, vabId, pSfx->field_2_note, 0, static_cast<short>(static_cast<int>(pSfx->field_2_note) + v3), v4);
        }
    }
}

EXPORT int CC SND_4CA5D0(int program, int vabId, int note, __int16 vol, __int16 min, __int16 max)
{
    int volClamped = 0;
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
    const int channelBits = MIDI_Play_Single_Note_4CA1B0(vabId | ((signed __int16)program << 8), note << 8, volClamped, volClamped);
    if (!GetMidiVars()->sSFXPitchVariationEnabled())
    {
        return 0;
    }

    if (min || max)
    {
        __int16 randomValue = Math_RandomRange_496AB0(min, max);

        int v9; // edi
        __int16 v10; // bx
        if (randomValue >= 0)
        {
            v9 = (randomValue >> 7) & 0xFFFF;
            v10 = randomValue & 127;
        }
        else
        {
            v9 = -1 - (-randomValue >> 7);
            v10 = 127 - (-(char)randomValue & 127);
        }

        for (short i = 0; i < 24; i++) // TODO: Use kNumChannels
        {
            if ((1 << i) & channelBits)
            {
                SsUtChangePitch_4FDF70(i, program, vabId, static_cast<short>(note), 0, static_cast<short>(v9 + note), v10);
            }
        }
    }
    return channelBits;
}

int CC SFX_SfxDefinition_Play_4CA700(const SfxDefinition* sfxDef, __int16 volLeft, __int16 volRight, __int16 pitch_min, __int16 pitch_max)
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

EXPORT void CC SND_Stop_Channels_Mask_4CA810(DWORD bitMask)
{
    for (int i = 0; i < 24; i++) // TODO: Constant
    {
        // Does the index match a bit in the bitmask?
        if ((1 << i) & bitMask)
        {
            // Turn it off
            GetSpuApiVars()->SsUtKeyOffV(static_cast<short>(i));
        }
    }
}

EXPORT void SND_Stop_All_Seqs_4CA850()
{
    // TODO: Why is there 16 of these but 32 of sMidiStruct2Ary32_C13400? Seems like they should match in size
    GetMidiVars()->sSeqsPlaying_count_word() = 0;
    for (short i = 0; i < 16; i++)
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
    for (short i = 0; i < 16; i++)
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

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4CA960(unsigned __int16 idx, __int16 repeatCount, __int16 bDontStop)
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

        const int vabId = GetMidiVars()->sLastLoadedSoundBlockInfo()[rec.field_8_sound_block_idx].field_8_vab_id;
        rec.field_A_id_seqOpenId = SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data, static_cast<short>(vabId));

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
    __int16 clampedVol = rec.field_9_volume;
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


EXPORT __int16 CC SND_SEQ_Play_4CAB10(unsigned __int16 idx, __int16 repeatCount, __int16 volLeft, __int16 volRight)
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
        const short vabId = static_cast<short>(GetMidiVars()->sLastLoadedSoundBlockInfo()[rec.field_8_sound_block_idx].field_8_vab_id);
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
    signed __int16 clampedVolLeft = 0;
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
    signed __int16 clampedVolRight = 0;
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


EXPORT int CC SND_SsIsEos_DeInlined_4CACD0(unsigned __int16 idx)
{
    OpenSeqHandle* pRec = &GetMidiVars()->sSeqDataTable()[idx];
    if (pRec->field_A_id_seqOpenId != -1 && pRec->field_C_ppSeq_Data)
    {
        return SsIsEos_4FDA80(pRec->field_A_id_seqOpenId, 0) != 0;
    }
    return 0;
}


EXPORT void CC SND_SEQ_SetVol_4CAD20(int idx, __int16 volLeft, __int16 volRight)
{
    unsigned __int16 limitedIdx = idx & 0xFFFF;
    if (GetMidiVars()->sSeqDataTable()[limitedIdx].field_A_id_seqOpenId != -1
        && GetMidiVars()->sSeqDataTable()[limitedIdx].field_C_ppSeq_Data
        && SND_SsIsEos_DeInlined_4CACD0(limitedIdx))
    {
        SsSeqSetVol_4FDAC0(GetMidiVars()->sSeqDataTable()[limitedIdx].field_A_id_seqOpenId, volLeft, volRight);
    }
}


EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx)
{
    if (GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId != -1 && GetMidiVars()->sSeqDataTable()[idx].field_C_ppSeq_Data)
    {
        if (SsIsEos_4FDA80(GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId, 0))
        {
            SsSeqStop_4FD9C0(GetMidiVars()->sSeqDataTable()[idx].field_A_id_seqOpenId);
        }
    }
}

EXPORT void CC SND_Load_Seqs_Impl(OpenSeqHandle* pSeqTable, const char* bsqFileName)
{
    if (pSeqTable && bsqFileName)
    {
        GetMidiVars()->sSeqDataTable() = pSeqTable;

        // Generate resource ids from hashing the name if we haven't already
        if (GetMidiVars()->sNeedToHashSeqNames())
        {
            for (int i = 0; i < GetMidiVars()->MidiTableSize(); i++)
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
        for (int i = 0; i < GetMidiVars()->MidiTableSize(); i++)
        {
            BYTE** ppSeq = GetMidiVars()->GetLoadedResource(ResourceManager::Resource_Seq, GetMidiVars()->sSeqDataTable()[i].field_4_generated_res_id, 1, 1);
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

EXPORT void CC SND_Load_Seqs_4CAED0(OpenSeqHandle* pSeqTable, const char* bsqFileName)
{
    SND_Load_Seqs_Impl(pSeqTable, bsqFileName);
}

EXPORT char CC SND_Seq_Table_Valid_4CAFE0()
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
        for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
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
