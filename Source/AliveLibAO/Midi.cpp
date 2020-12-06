#include "stdafx_ao.h"

#include "config.h"

#include "Midi.hpp"
#include "Function.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "ResourceManager.hpp"
#include "LvlArchive.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "AmbientSound.hpp"
#include "Sound.hpp"

#include "Sfx.hpp"
#include "../AliveLibAE/Sfx.hpp"

#ifdef _WIN32
#define NO_WAVE
#endif

struct IO_FileHandleType;

#include "../AliveLibAE/Sound/PsxSpuApi.hpp"
#include "../AliveLibAE/Sound/Midi.hpp"
#include "../AliveLibAE/Sound/Sound.hpp"
#include "../AliveLibAE/PathData.hpp"


namespace AO {

const int kSeqTableSizeAO = 164;

ALIVE_VAR(1, 0x9F1DC4, WORD, sSnd_ReloadAbeResources_9F1DC4, 0);
//ALIVE_VAR(1, 0x9F1DB8, SoundBlockInfo *, sLastLoadedSoundBlockInfo_9F1DB8, nullptr);

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
//const SoundBlockInfo soundBlock = { "MONK.VH", "MONK.VB", -1, nullptr };
//ALIVE_VAR(1, 0x4D0008, SoundBlockInfo, sMonkVh_Vb_4D0008, soundBlock);

ALIVE_VAR(1, 0xA8918E, __int16, sGlobalVolumeLevel_right_A8918E, 0);
ALIVE_VAR(1, 0xA8918C, __int16, sGlobalVolumeLevel_left_A8918C, 0);
ALIVE_VAR(1, 0xABF8C0, VabUnknown, s512_byte_ABF8C0, {});
ALIVE_ARY(1, 0xA9289C, BYTE, kMaxVabs, sVagCounts_A9289C, {});
ALIVE_ARY(1, 0xA92898, BYTE, kMaxVabs, sProgCounts_A92898, {});
ALIVE_ARY(1, 0xABF8A0, VabHeader*, 4, spVabHeaders_ABF8A0, {});
ALIVE_VAR(1, 0xA9B8A0, ConvertedVagTable, sConvertedVagTable_A9B8A0, {});
ALIVE_VAR(1, 0xA928A0, SoundEntryTable, sSoundEntryTable16_A928A0, {});
ALIVE_VAR(1, 0xAC07C0, MidiChannels, sMidi_Channels_AC07C0, {});
ALIVE_VAR(1, 0xABFB40, MidiSeqSongsTable, sMidiSeqSongs_ABFB40, {});
ALIVE_VAR(1, 0xA89198, int, sMidi_Inited_dword_A89198, 0); 
ALIVE_VAR(1, 0xA89194, unsigned int, sMidiTime_A89194, 0);
ALIVE_VAR(1, 0xA89190, char, sbDisableSeqs_A89190, 0);
ALIVE_VAR(1, 0x4E8FD8, DWORD, sLastTime_4E8FD8, 0xFFFFFFFF);
ALIVE_VAR(1, 0xA8919C, BYTE, sControllerValue_A8919C, 0);

class AOPsxSpuApiVars : public IPsxSpuApiVars
{
public:
    virtual __int16& sGlobalVolumeLevel_right() override
    {
        return sGlobalVolumeLevel_right_A8918E;
    }

    virtual __int16& sGlobalVolumeLevel_left() override
    {
        return sGlobalVolumeLevel_left_A8918C;
    }

    virtual VabUnknown& s512_byte() override
    {
        return s512_byte_ABF8C0;
    }

    virtual BYTE* sVagCounts() override
    {
        return sVagCounts_A9289C;
    }

    virtual BYTE* sProgCounts() override
    {
        return sProgCounts_A92898;
    }

    virtual VabHeader** spVabHeaders() override
    {
        return spVabHeaders_ABF8A0;
    }

    virtual ConvertedVagTable& sConvertedVagTable() override
    {
        return sConvertedVagTable_A9B8A0;
    }

    virtual SoundEntryTable& sSoundEntryTable16() override
    {
        return sSoundEntryTable16_A928A0;
    }

    virtual MidiChannels& sMidi_Channels() override
    {
        return sMidi_Channels_AC07C0;
    }

    virtual MidiSeqSongsTable& sMidiSeqSongs() override
    {
        return sMidiSeqSongs_ABFB40;
    }

    virtual int& sMidi_Inited_dword() override
    {
        return sMidi_Inited_dword_A89198;
    }

    virtual unsigned int& sMidiTime() override
    {
        return sMidiTime_A89194;
    }

    virtual BOOL& sSoundDatIsNull() override
    {
        return mSoundDatIsNull;
    }

    virtual char& sbDisableSeqs() override
    {
        return sbDisableSeqs_A89190;
    }

    virtual DWORD& sLastTime() override
    {
        return sLastTime_4E8FD8;
    }

    virtual DWORD& sMidi_WaitUntil() override
    {
        // Always 0 in AO cause it dont exist 
        return mMidi_WaitUntil;
    }

    virtual IO_FileHandleType& sSoundDatFileHandle() override
    {
        // Should never be called
        throw std::logic_error("The method or operation is not implemented.");
    }

    virtual BYTE& sControllerValue() override
    {
        return sControllerValue_A8919C;
    }
private:
    BOOL mSoundDatIsNull = FALSE; // Pretend we have sounds dat opened so AE funcs work
    DWORD mMidi_WaitUntil = 0;
};

static AOPsxSpuApiVars sAoSpuVars;

EXPORT void CC SND_Reset_476BA0()
{
    AE_IMPLEMENTED();
    SND_Reset_4C9FB0();
}

EXPORT void CC SsUtAllKeyOff_49EDE0(int mode)
{
    SsUtAllKeyOff_4FDFE0(mode);
}

VabBodyRecord* IterateVBRecords(VabBodyRecord* ret, int i_3)
{
    for (int i = 0; i < i_3; i++)
    {
        ret = (VabBodyRecord*)((char*)ret
            + ret->field_0_length_or_duration
            + 8);
    }
    return ret;
}

// Loads vab body sample data to memory

// TODO: Need to test this with dll/og as it might be wrong
EXPORT void CC SsVabTransBody_49D3E0(VabBodyRecord* pVabBody, __int16 vabId)
{
    AE_IMPLEMENTED();

    if (vabId < 0)
    {
        return;
    }

    VabHeader* pVabHeader = GetSpuApiVars()->spVabHeaders()[vabId];
    const int vagCount = GetSpuApiVars()->sVagCounts()[vabId];

    for (int i = 0; i < vagCount; i++)
    {
        SoundEntry* pEntry = &GetSpuApiVars()->sSoundEntryTable16().table[vabId][i];

        if (!(i & 7))
        {
            SsSeqCalledTbyT_4FDC80();
        }

        memset(pEntry, 0, sizeof(SoundEntry));

        int sampleLen = -1;
        if (pVabHeader && i >= 0)
        {
            sampleLen = (8 * IterateVBRecords(pVabBody, i)->field_0_length_or_duration) / 16;
        }

        if (sampleLen > 0)
        {
            VabBodyRecord* v10 = nullptr;
            if (pVabHeader && i >= 0)
            {
                v10 = IterateVBRecords(pVabBody, i);
            }

            const BYTE unused_field = v10->field_4_unused != 0 ? 0 : 4;
            for (int prog = 0; prog < 128; prog++)
            {
                for (int tone = 0; tone < 16; tone++)
                {
                    auto pVag = &GetSpuApiVars()->sConvertedVagTable().table[vabId][prog][tone];
                    if (pVag->field_10_vag == i)
                    {
                        pVag->field_C = unused_field;

                        if (!(unused_field & 4) && !pVag->field_0_adsr_attack && pVag->field_6_adsr_release)
                        {
                            pVag->field_6_adsr_release = 0;
                        }
                    }
                }
            }

            if (!SND_New_4EEFF0(pEntry, sampleLen, 44100, 16u, 0))
            {
                auto pTempBuffer = (DWORD*)malloc(sampleLen * pEntry->field_1D_blockAlign);
                if (pTempBuffer)
                {
                    DWORD* pSrcVB = nullptr;
                    if (pVabHeader && i >= 0)
                    {
                        pSrcVB = &IterateVBRecords(pVabBody, i)->field_8_fileOffset;
                    }
      
                    int sampleLen2 = -1;
                    if (pVabHeader && i >= 0)
                    {
                        sampleLen2 = (8 * IterateVBRecords(pVabBody, i)->field_0_length_or_duration) / 16;
                    }

                    const int len = (16 * sampleLen2) / 8;
                    memcpy(pTempBuffer, pSrcVB, len);

                    if (sampleLen2)
                    {
                        SND_Load_4EF680(pEntry, pTempBuffer, sampleLen2);
                    }

                    free(pTempBuffer);
                }
            }
        }
    }
}

EXPORT signed __int16 CC SND_VAB_Load_476CB0(SoundBlockInfo* pSoundBlockInfo, __int16 vabId)
{
    AE_IMPLEMENTED();

    // Fail if no file name
    if (!pSoundBlockInfo->field_0_vab_header_name)
    {
        return 0;
    }

    // Find the VH file record
    LvlFileRecord* pVabHeaderFile = sLvlArchive_4FFD60.Find_File_Record_41BED0(pSoundBlockInfo->field_0_vab_header_name);
    
    int headerSize = pVabHeaderFile->field_10_num_sectors << 11;

    BYTE** ppVabHeader = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_VabHeader, vabId, headerSize);

    pSoundBlockInfo->field_C_pVabHeader = *ppVabHeader;
    sLvlArchive_4FFD60.Read_File_41BE40(pVabHeaderFile, *ppVabHeader);
    
    // Find the VB file record
    LvlFileRecord* pVabBodyFile = sLvlArchive_4FFD60.Find_File_Record_41BED0(pSoundBlockInfo->field_4_vab_body_name);
    if (!pVabBodyFile)
    {
        // For some reason its acceptable to assume we have a VH with no VB, but the VH must always exist, this happens for MONK.VB
        return 0;
    }

    int vabBodySize = pVabBodyFile->field_10_num_sectors << 11;

    // Load the VB file data
    BYTE** ppVabBody = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_VabBody, vabId, vabBodySize);
    if (!ppVabBody)
    {
        // Maybe filed due to OOM cause its huge, free the abe resources and try again
        if (!sSnd_ReloadAbeResources_9F1DC4)
        {
            sSnd_ReloadAbeResources_9F1DC4 = TRUE;
            sActiveHero_507678->Free_Resources_422870();
        }

        // Compact/reclaim any other memory we can too
        ResourceManager::Reclaim_Memory_455660(0);

        // If it fails again there is no recovery, in either case caller will restore abes resources
        ppVabBody = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_VabBody, vabId, vabBodySize);
        if (!ppVabBody)
        {
            return 0;
        }
    }

    sLvlArchive_4FFD60.Read_File_41BE40(pVabBodyFile, *ppVabBody);
    pSoundBlockInfo->field_8_vab_id = SsVabOpenHead_4FC620(reinterpret_cast<VabHeader*>(pSoundBlockInfo->field_C_pVabHeader));
    SsVabTransBody_49D3E0(reinterpret_cast<VabBodyRecord*>(*ppVabBody), static_cast<short>(pSoundBlockInfo->field_8_vab_id));
    SsVabTransCompleted_4FE060(SS_WAIT_COMPLETED);

    // Now the sound samples are loaded we don't need the VB data anymore
    ResourceManager::FreeResource_455550(ppVabBody);
    return 1;
}

EXPORT void CC SND_Load_VABS_477040(SoundBlockInfo* pSoundBlockInfo, int reverb)
{
    AE_IMPLEMENTED();

    SoundBlockInfo* pSoundBlockInfoIter = pSoundBlockInfo;
    sSnd_ReloadAbeResources_9F1DC4 = FALSE;
    if (GetLastLoadedSoundBlockInfo() != reinterpret_cast<::SoundBlockInfo*>(pSoundBlockInfo))
    {
        SsUtReverbOff_4FE350();
        SsUtSetReverbDepth_4FE380(0, 0);
        SpuClearReverbWorkArea_4FA690(4);

        if (GetMonkVb().field_8_vab_id < 0)
        {
            SND_VAB_Load_476CB0(reinterpret_cast<SoundBlockInfo*>(&GetMonkVb()), 32);
        }

        SetLastLoadedSoundBlockInfo(reinterpret_cast<::SoundBlockInfo*>(pSoundBlockInfo));

        __int16 vabId = 0;
        while (SND_VAB_Load_476CB0(pSoundBlockInfoIter, vabId))
        {
            ++vabId;
            ++pSoundBlockInfoIter;
        }

        if (sSnd_ReloadAbeResources_9F1DC4)
        {
            ResourceManager::Reclaim_Memory_455660(0);
            Abe::Load_Basic_Resources_4228A0();
        }

        SsUtSetReverbDepth_4FE380(reverb, reverb);
        SsUtReverbOn_4FE340();
    }
}

EXPORT void CC SND_Stop_Channels_Mask_4774A0(int mask)
{
    AE_IMPLEMENTED();
    SND_Stop_Channels_Mask_4CA810(mask);
}

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandleAE* pSeqTable, const char* bsqFileName)
{
    AE_IMPLEMENTED();
    SND_Load_Seqs_Impl(
        reinterpret_cast<::OpenSeqHandle*>(pSeqTable),
        kSeqTableSizeAO,
        bsqFileName,
        reinterpret_cast<TReclaimMemoryFn>(ResourceManager::Reclaim_Memory_455660),
        reinterpret_cast<TLoadResourceFileFn>(ResourceManager::LoadResourceFileWrapper),
        reinterpret_cast<TGetLoadedResourceFn>(ResourceManager::GetLoadedResource_4554F0));
}

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4775A0(SeqId idx, int repeatCount, __int16 bDontStop)
{
    AE_IMPLEMENTED();
    return SND_SEQ_PlaySeq_4CA960(static_cast<unsigned __int16>(idx), static_cast<short>(repeatCount), bDontStop);
}

EXPORT void CC SND_Seq_Stop_477A60(SeqId idx)
{
    AE_IMPLEMENTED();
    SND_SEQ_Stop_4CAE60(static_cast<unsigned __int16>(idx));
}

EXPORT signed __int16 CC SND_SEQ_Play_477760(SeqId idx, int repeatCount, __int16 volLeft, __int16 volRight)
{
    AE_IMPLEMENTED();
    const auto ret = SND_SEQ_PlaySeq_4CA960(static_cast<unsigned __int16>(idx), static_cast<short>(repeatCount), 1); // TODO ??
    SsSeqSetVol_4FDAC0(static_cast<unsigned __int16>(idx), volLeft, volRight);
    return ret;
}

EXPORT __int16 CC SND_SsIsEos_DeInlined_477930(SeqId idx)
{
    AE_IMPLEMENTED();
    return static_cast<__int16>(SND_SsIsEos_DeInlined_4CACD0(static_cast<unsigned __int16>(idx)));
}

static ::SfxDefinition ToAeSfxDef(const SfxDefinition* sfxDef)
{
    ::SfxDefinition aeDef = {};
    aeDef.field_0_block_idx = static_cast<char>(sfxDef->field_0_block_idx);
    aeDef.field_1_program = static_cast<char>(sfxDef->field_4_program);
    aeDef.field_2_note = static_cast<char>(sfxDef->field_8_note);
    aeDef.field_3_default_volume = static_cast<char>(sfxDef->field_C_default_volume);
    aeDef.field_4_pitch_min = sfxDef->field_E_pitch_min;
    aeDef.field_6_pitch_max = sfxDef->field_10_pitch_max;
    return aeDef;
}

EXPORT int CC SFX_SfxDefinition_Play_477330(const SfxDefinition* sfxDef, short volLeft, short volRight, short pitch_min, signed __int16 pitch_max)
{
    AE_IMPLEMENTED();
    const ::SfxDefinition aeDef = ToAeSfxDef(sfxDef);
    return SFX_SfxDefinition_Play_4CA700(&aeDef, volLeft, volRight, pitch_min, pitch_max);
}

EXPORT int CC SFX_SfxDefinition_Play_4770F0(const SfxDefinition* sfxDef, int vol, int pitch_min, int pitch_max)
{
    AE_IMPLEMENTED();
    const ::SfxDefinition aeDef = ToAeSfxDef(sfxDef);
    return SFX_SfxDefinition_Play_4CA420(&aeDef, static_cast<short>(vol), static_cast<short>(pitch_min), static_cast<short>(pitch_max));
}

EXPORT void CC SND_Init_476E40()
{
    AE_IMPLEMENTED();
    SetSpuApiVars(&sAoSpuVars);
    SND_Init_4CA1F0();
    SND_Restart_SetCallBack(SND_Restart_476340);
    SND_StopAll_SetCallBack(SND_StopAll_4762D0);
}

EXPORT void CC SND_Shutdown_476EC0()
{
    NOT_IMPLEMENTED();
    SND_Shutdown_4CA280();
}

EXPORT void CC SND_SEQ_SetVol_477970(SeqId idx, __int16 volLeft, __int16 volRight)
{
    AE_IMPLEMENTED();
    SND_SEQ_SetVol_4CAD20(static_cast<unsigned __int16>(idx), volLeft, volRight);
}

EXPORT void CC SND_StopAll_4762D0()
{
    AE_IMPLEMENTED();

    MusicController::EnableMusic_443900(0);

    if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
    {
        SND_Seq_Stop_477A60(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8));
    }

    SND_Reset_Ambiance_4765E0();
    //SND_Stop_All_Seqs_4774D0();
    SND_Stop_All_Seqs_4CA850();

    for (int i=0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (!pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            pObj->VStopAudio();
        }
    }

    SsUtAllKeyOff_49EDE0(0);

}

}
