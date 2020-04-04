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

EXPORT void SND_Stop_All_Seqs_4CA850();
EXPORT void CC SFX_SetPitch_4CA510(const SfxDefinition* pSfx, int channelsBits, __int16 pitch);

const int kSeqTableSize = 144;

struct SeqIds
{
    __int16 ids[16];
};

ALIVE_VAR(1, 0xBB2354, SeqIds, sSeq_Ids_word_BB2354, {});
ALIVE_VAR(1, 0xbb2e3e, WORD, sSnd_ReloadAbeResources_BB2E3E, 0);
ALIVE_VAR(1, 0xbb2e38, SeqDataRecord *, sSeqDataTable_BB2E38, nullptr);
ALIVE_VAR(1, 0xbb2e3c, __int16, sSeqsPlaying_count_word_BB2E3C, 0);
ALIVE_VAR(1, 0xbb2e34, SoundBlockInfo *, sLastLoadedSoundBlockInfo_BB2E34, nullptr);
ALIVE_VAR(1, 0x560f58, __int16, sSFXPitchVariationEnabled_560F58, true);
ALIVE_VAR(1, 0x560f40, short, sNeedToHashSeqNames_560F40, 1);

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
const SoundBlockInfo soundBlock = { "MONK.VH", "MONK.VB", -1, nullptr };

ALIVE_VAR(1, 0x560F48, SoundBlockInfo, sMonkVh_Vb_560F48, soundBlock);


EXPORT void CC SND_Free_All_VABS_4C9EB0()
{
    SoundBlockInfo* pIter = sLastLoadedSoundBlockInfo_BB2E34;
    while (pIter && pIter->field_4_vab_body_name)
    {
        ResourceManager::FreeResource_Impl_49C360(pIter->field_C_pVabHeader);
        pIter->field_C_pVabHeader = nullptr;
        SsVabClose_4FC5B0(pIter->field_8_vab_id);
        pIter->field_8_vab_id = -1;
        pIter++;
    }
    sLastLoadedSoundBlockInfo_BB2E34 = nullptr;
}

EXPORT void CC SND_Free_All_Seqs_4C9F40()
{
    for (int i = 0; i < kSeqTableSize; i++)
    {
        if (sSeqDataTable_BB2E38[i].field_C_ppSeq_Data)
        {
            BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Seq, sSeqDataTable_BB2E38[i].field_4_generated_res_id, 0, 0);
            ResourceManager::FreeResource_49C330(ppRes);
            sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = nullptr;
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
    BYTE** ppVabHeader = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_VabHeader, vabId, headerSize);
    pSoundBlockInfo->field_C_pVabHeader = *ppVabHeader;
    sLvlArchive_5BC520.Read_File_4330A0(pVabHeaderFile, *ppVabHeader);
    pResourceManager_5C1BB0->LoadingLoop_465590(0);

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
        vabBodySize = pVabBodyFile->field_10_num_sectors << 11;
    }

    // Load the VB file data
    BYTE** ppVabBody = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_VabBody, vabId, vabBodySize);
    if (!ppVabBody)
    {
        // Maybe filed due to OOM cause its huge, free the abe resources and try again
        if (!sSnd_ReloadAbeResources_BB2E3E)
        {
            sSnd_ReloadAbeResources_BB2E3E = TRUE;
            sActiveHero_5C1B68->Free_Resources_44D420();
        }

        // Compact/reclaim any other memory we can too
        ResourceManager::Reclaim_Memory_49C470(0);

        // If it fails again there is no recovery, in either case caller will restore abes resources
        ppVabBody = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_VabBody, vabId, vabBodySize);
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
    ResourceManager::FreeResource_49C330(ppVabBody);
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
    memset(&sSeq_Ids_word_BB2354, -1, sizeof(SeqIds));
    sSeqsPlaying_count_word_BB2E3C = 0;
}

// SND_SetMono_NoRefs_4CA310

// SND_SetStereo_NoRefs_4CA330

EXPORT void SND_Shutdown_4CA280()
{
    SND_Reset_4C9FB0();

    if (sMonkVh_Vb_560F48.field_8_vab_id >= 0)
    {
        ResourceManager::FreeResource_Impl_49C360(sMonkVh_Vb_560F48.field_C_pVabHeader);
        sMonkVh_Vb_560F48.field_C_pVabHeader = nullptr;

        SsVabClose_4FC5B0(sMonkVh_Vb_560F48.field_8_vab_id);
        sMonkVh_Vb_560F48.field_8_vab_id = -1;
    }

    SsSetMVol_4FC360(0, 0);
    SsUtReverbOff_4FE350();
    SsUtSetReverbDepth_4FE380(0, 0);
    SsEnd_4FC350();

    // TODO: PSX calls a func that just does SpuQuit() here and nothing else
    SsExt_StopPlayingSamples();

    SsExt_CloseAllVabs();

    SND_SsQuit_4EFD50();
}


EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, int reverb)
{
    SoundBlockInfo* pSoundBlockInfoIter = pSoundBlockInfo;
    sSnd_ReloadAbeResources_BB2E3E = FALSE;
    if (sLastLoadedSoundBlockInfo_BB2E34 != pSoundBlockInfo)
    {
        SsUtReverbOff_4FE350();
        SsUtSetReverbDepth_4FE380(0, 0);
        SpuClearReverbWorkArea_4FA690(4);

        if (sMonkVh_Vb_560F48.field_8_vab_id < 0)
        {
            SND_VAB_Load_4C9FE0(&sMonkVh_Vb_560F48, 32);
        }

        sLastLoadedSoundBlockInfo_BB2E34 = pSoundBlockInfo;

        __int16 vabId = 0;
        while (SND_VAB_Load_4C9FE0(pSoundBlockInfoIter, vabId))
        {
            ++vabId;
            ++pSoundBlockInfoIter;
        }

        // Put abes resources back if we had to unload them to fit the VB in memory
        if (sSnd_ReloadAbeResources_BB2E3E)
        {
            ResourceManager::Reclaim_Memory_49C470(0);
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
        sfxDef->field_1_program | (sLastLoadedSoundBlockInfo_BB2E34[sfxDef->field_0_block_idx].field_8_vab_id << 8),
        sfxDef->field_2_note << 8,
        volume,
        volume);

    if (!sSFXPitchVariationEnabled_560F58)
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
    if (!sSFXPitchVariationEnabled_560F58)
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
        sfxDef->field_1_program | (sLastLoadedSoundBlockInfo_BB2E34[sfxDef->field_0_block_idx].field_8_vab_id << 8),
        sfxDef->field_2_note << 8,
        volLeft,
        volRight);

    if (!sSFXPitchVariationEnabled_560F58)
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
            SsUtKeyOffV_4FE010(static_cast<short>(i));
        }
    }
}

EXPORT void SND_Stop_All_Seqs_4CA850()
{
    // TODO: Why is there 16 of these but 32 of sMidiStruct2Ary32_C13400? Seems like they should match in size
    sSeqsPlaying_count_word_BB2E3C = 0;
    for (short i = 0; i < 16; i++)
    {
        if (sSeq_Ids_word_BB2354.ids[i] >= 0)
        {
            if (SsIsEos_4FDA80(i, 0))
            {
                SsSeqStop_4FD9C0(i);
            }
            SsSeqClose_4FD8D0(i);
            sSeqDataTable_BB2E38[sSeq_Ids_word_BB2354.ids[i]].field_A_id = -1;
            sSeq_Ids_word_BB2354.ids[i] = -1;
        }
    }
}


EXPORT void SND_Seq_Stop_4CA8E0()
{
    for (short i = 0; i < 16; i++)
    {
        if (sSeq_Ids_word_BB2354.ids[i] >= 0)
        {
            if (!SsIsEos_4FDA80(i, 0))
            {
                SsSeqClose_4FD8D0(i);
                sSeqDataTable_BB2E38[sSeq_Ids_word_BB2354.ids[i]].field_A_id = -1;
                sSeq_Ids_word_BB2354.ids[i] = -1;
                sSeqsPlaying_count_word_BB2E3C--;
            }
        }
    }
}

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4CA960(unsigned __int16 idx, __int16 repeatCount, __int16 bDontStop)
{
    SeqDataRecord& rec = sSeqDataTable_BB2E38[idx];
    if (!rec.field_C_ppSeq_Data)
    {
        return 0;
    }

    if (rec.field_A_id < 0)
    {
        if (sSeqsPlaying_count_word_BB2E3C >= 16)
        {
            // Note: Inlined in psx
            SND_Seq_Stop_4CA8E0();
            if (sSeqsPlaying_count_word_BB2E3C >= 16)
            {
                return 0;
            }
        }

        const int vabId = sLastLoadedSoundBlockInfo_BB2E34[rec.field_8_sound_block_idx].field_8_vab_id;
        rec.field_A_id = SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data, static_cast<short>(vabId));

        sSeq_Ids_word_BB2354.ids[rec.field_A_id] = idx;
        sSeqsPlaying_count_word_BB2E3C++;
    }
    else if (SsIsEos_4FDA80(rec.field_A_id, 0))
    {
        if (!bDontStop)
        {
            return 0;
        }
        SsSeqStop_4FD9C0(rec.field_A_id);
    }

    // Clamp vol
    __int16 clampedVol = rec.field_9;
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

    SsSeqSetVol_4FDAC0(rec.field_A_id, clampedVol, clampedVol);
    if (repeatCount)
    {
        SsSeqPlay_4FD900(rec.field_A_id, 1, repeatCount);
    }
    else
    {
        SsSeqPlay_4FD900(rec.field_A_id, 1, 0);
    }

    return 1;
}


EXPORT __int16 CC SND_SEQ_Play_4CAB10(unsigned __int16 idx, __int16 a2, __int16 volLeft, __int16 volRight)
{
    LOG_INFO("PLAY SEQ " << idx);

    SeqDataRecord& rec = sSeqDataTable_BB2E38[idx];
    if (!rec.field_C_ppSeq_Data)
    {
        return 0;
    }

    // SEQ isn't in use
    if (rec.field_A_id < 0)
    {
        // Too many playing
        if (sSeqsPlaying_count_word_BB2E3C >= 16)
        {
            // Stop any SEQs that are done
            SND_Seq_Stop_4CA8E0();

            // If none where done then can't continue
            if (sSeqsPlaying_count_word_BB2E3C >= 16)
            {
                return 0;
            }
        }

        // Open the SEQ
        const short vabId = static_cast<short>(sLastLoadedSoundBlockInfo_BB2E34[rec.field_8_sound_block_idx].field_8_vab_id);
        rec.field_A_id = SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data, vabId);

        // Index into the IDS via the seq ID and map it to the index
        sSeq_Ids_word_BB2354.ids[rec.field_A_id] = idx;
        sSeqsPlaying_count_word_BB2E3C++;
    }
    else if (SsIsEos_4FDA80(rec.field_A_id, 0))
    {
        SsSeqStop_4FD9C0(rec.field_A_id);
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

    SsSeqSetVol_4FDAC0(rec.field_A_id, clampedVolLeft, clampedVolRight);

    if (a2)
    {
        SsSeqPlay_4FD900(rec.field_A_id, 1, a2);
    }
    else
    {
        SsSeqPlay_4FD900(rec.field_A_id, 1, 0);
    }

    return 1;
}


EXPORT int CC SND_SsIsEos_DeInlined_4CACD0(unsigned __int16 idx)
{
    SeqDataRecord* pRec = &sSeqDataTable_BB2E38[idx];
    if (pRec->field_A_id != -1 && pRec->field_C_ppSeq_Data)
    {
        return SsIsEos_4FDA80(pRec->field_A_id, 0) != 0;
    }
    return 0;
}


EXPORT void CC SND_SEQ_SetVol_4CAD20(int idx, __int16 volLeft, __int16 volRight)
{
    unsigned __int16 limitedIdx = idx & 0xFFFF;
    if (sSeqDataTable_BB2E38[limitedIdx].field_A_id != -1
        && sSeqDataTable_BB2E38[limitedIdx].field_C_ppSeq_Data
        && SND_SsIsEos_DeInlined_4CACD0(limitedIdx))
    {
        SsSeqSetVol_4FDAC0(sSeqDataTable_BB2E38[limitedIdx].field_A_id, volLeft, volRight);
    }
}


EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx)
{
    if (sSeqDataTable_BB2E38[idx].field_A_id != -1 && sSeqDataTable_BB2E38[idx].field_C_ppSeq_Data)
    {
        if (SsIsEos_4FDA80(sSeqDataTable_BB2E38[idx].field_A_id, 0))
        {
            SsSeqStop_4FD9C0(sSeqDataTable_BB2E38[idx].field_A_id);
        }
    }
}

EXPORT void CC SND_Load_Seqs_4CAED0(SeqDataRecord* pSeqTable, const char* bsqFileName)
{
    if (pSeqTable && bsqFileName)
    {
        sSeqDataTable_BB2E38 = pSeqTable;

        // Generate resource ids from hashing the name if we haven't already
        if (sNeedToHashSeqNames_560F40)
        {
            for (int i = 0; i < kSeqTableSize; i++)
            {
                sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = nullptr;
                sSeqDataTable_BB2E38[i].field_A_id = -1;
                sSeqDataTable_BB2E38[i].field_4_generated_res_id = ResourceManager::SEQ_HashName_49BE30(sSeqDataTable_BB2E38[i].field_0_mName);
            }
            sNeedToHashSeqNames_560F40 = 0;
        }

        // Load the BSQ
        ResourceManager::Reclaim_Memory_49C470(0);
        ResourceManager::LoadResourceFile_49C170(bsqFileName, nullptr);

        // Get a pointer to each SEQ
        for (int i = 0; i < kSeqTableSize; i++)
        {
            BYTE** ppSeq = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Seq, sSeqDataTable_BB2E38[i].field_4_generated_res_id, 1, 1);
            if (ppSeq)
            {
                sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = *ppSeq;
            }
            else
            {
                sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = nullptr;
            }
        }
    }
}

EXPORT char CC SND_Seq_Table_Valid_4CAFE0()
{
    return sSeqDataTable_BB2E38 != 0;
}

EXPORT void CC SND_StopAll_4CB060()
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

EXPORT void CC SND_Restart_4CB0E0()
{
    MusicController::EnableMusic_47FE10(TRUE);
    BackgroundMusic::Play_4CB030();
    Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
}

// Next -> Background music object

// Next -> Scoped seq object

// Next -> SND_Init_Ambiance_4CB480, SND_Reset_Ambiance_4CB4B0, Start_Sounds_for_TLV_4CB530, Start_Slig_sounds_4CB980
// Stop_slig_sounds_4CBA70 Path::Start_Sounds_For_Objects_In_Camera_4CBAF0, Start_Sounds_For_Objects_In_Near_Cameras_4CBB60
