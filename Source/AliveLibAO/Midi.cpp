#include "stdafx_ao.h"
#include "Midi.hpp"
#include "Function.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "ResourceManager.hpp"
#include "LvlArchive.hpp"
#include "../AliveLibAE/Sound/PsxSpuApi.hpp"

namespace AO {

EXPORT void CC SND_Reset_476BA0()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x9F1DC4, WORD, sSnd_ReloadAbeResources_9F1DC4, 0);
ALIVE_VAR(1, 0x9F1DB8, SoundBlockInfo *, sLastLoadedSoundBlockInfo_9F1DB8, nullptr);

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
const SoundBlockInfo soundBlock = { "MONK.VH", "MONK.VB", -1, nullptr };
ALIVE_VAR(1, 0x4D0008, SoundBlockInfo, sMonkVh_Vb_4D0008, soundBlock);

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
    SsVabTransBody_4FC840(reinterpret_cast<VabBodyRecord*>(*ppVabBody), static_cast<short>(pSoundBlockInfo->field_8_vab_id));
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
    if (sLastLoadedSoundBlockInfo_9F1DB8 != pSoundBlockInfo)
    {
        SsUtReverbOff_4FE350();
        SsUtSetReverbDepth_4FE380(0, 0);
        SpuClearReverbWorkArea_4FA690(4);

        if (sMonkVh_Vb_4D0008.field_8_vab_id < 0)
        {
            SND_VAB_Load_476CB0(&sMonkVh_Vb_4D0008, 32);
        }

        sLastLoadedSoundBlockInfo_9F1DB8 = pSoundBlockInfo;

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

EXPORT void CC SND_Stop_Channels_Mask_4774A0(int /*mask*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandle* /*pSeqTable*/, const char* /*bsqFileName*/)
{
    NOT_IMPLEMENTED();
}

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4775A0(SeqId /*idx*/, int /*repeatCount*/, __int16 /*bDontStop*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC SND_Seq_Stop_477A60(SeqId /*idx*/)
{
    NOT_IMPLEMENTED();
}

EXPORT signed __int16 CC SND_SEQ_Play_477760(SeqId /*idx*/, int /*repeatCount*/, __int16 /*volLeft*/, __int16 /*volRight*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT __int16 CC SND_SsIsEos_DeInlined_477930(SeqId /*idx*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC SFX_SfxDefinition_Play_477330(const SfxDefinition* /*sfxDef*/, short /*volLeft*/, short /*volRight*/, short /*pitch_min*/, signed __int16 /*pitch_max*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC SFX_SfxDefinition_Play_4770F0(const SfxDefinition* /*sfxDef*/, signed int /*vol*/, int /*pitch_min*/, int /*pitch_max*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC SND_Init_476E40()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_Shutdown_476EC0()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_SEQ_SetVol_477970(SeqId /*idx*/, __int16 /*volLeft*/, __int16 /*volRight*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_StopAll_4762D0()
{
    NOT_IMPLEMENTED();
}

}
