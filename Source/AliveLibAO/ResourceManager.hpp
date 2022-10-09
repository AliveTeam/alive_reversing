#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"
#include "../AliveLibCommon/AnimResources.hpp"
#include "Factory.hpp"

namespace AO {

class BaseAliveGameObject;
class Camera;
class LoadingFile;

void Game_ShowLoadingIcon_445EB0();

// TODO Argument type is always Camera* ?
using TLoaderFn = void(CC*)(void*);

class ResourceManager final
{
public:
    enum ResourceType : u32
    {
        Resource_PBuf = 0x66754250,
        Resource_CHNK = 0x4B4E4843,
        Resource_DecompressionBuffer = 0x66754244,
        Resource_VLC = 0x20434C56,
        Resource_Animation = 0x6D696E41,
        Resource_VabHeader = 0x48424156,
        Resource_VabBody = 0x42424156,
        Resource_Font = 0x746E6F46,
        Resource_Path = 0x68746150,
        Resource_Palt = 0x746C6150,
        Resource_FG1 = 0x20314746,
        Resource_Bits = 0x73746942,
        Resource_Blood = 0x64756C42,
        Resource_Sprx = 0x78727053,
        Resource_FntP = 0x50746E46,
        Resource_3DGibs = 0x65444433,
        Resource_HintFly = 0x796C4648,
        Resource_Spline = 0x6e6c7053,
        Resource_Wave = 0x65766157,
        Resource_Free = 0x65657246,
        Resource_Pend = 0x646E6550,
        Resource_End = 0x21646E45,
        Resource_Plbk = 0x6B626C50,
        Resource_Play = 0x79616C50,
    };

    enum ResourceHeaderFlags : s16
    {
        eLocked = 0x1,
        eOnlyAHeader = 0x2,
        eNeverFree = 0x4
    };

    struct Header final
    {
        u32 field_0_size;
        s16 field_4_ref_count;
        s16 field_6_flags;
        u32 mLineType;
        u32 field_C_id;
    };
    ALIVE_ASSERT_SIZEOF(Header, 0x10);

    struct ResourcesToLoadList_Entry final
    {
        u32 field_0_type;
        u32 field_4_res_id;
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList_Entry, 0x8);

    // TODO: Replace/combine with CompileTimeResourceList when everything is decompiled
    struct ResourcesToLoadList final
    {
        s32 field_0_count;
        ResourcesToLoadList_Entry field_4_items[1];
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList, 12);

    struct ResourceManager_FilePartRecord final
    {
        s32 field_0_type;
        s32 field_4_res_id;
        Camera* field_8_pCamera;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord, 0xc);

    class ResourceManager_FileRecord final
    {
    public:
        ResourceManager_FileRecord()
            : field_10_file_sections_dArray(10)
        {

        }

        const char_type* field_0_fileName = nullptr;
        ResourcesToLoadList* field_4_pResourcesToLoadList = nullptr;
        u32 mLineType = 0;
        u32 field_C_resourceId = 0;
        DynamicArrayT<ResourceManager_FilePartRecord> field_10_file_sections_dArray;
        LoadingFile* field_1C_pGameObjFileRec = nullptr;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord, 0x20);

    struct ResourceHeapItem final
    {
        u8* field_0_ptr;
        ResourceHeapItem* field_4_pNext;
    };
    ALIVE_ASSERT_SIZEOF(ResourceHeapItem, 0x8);

    enum BlockAllocMethod : s32
    {
        eFirstMatching = 0,
        eNearestMatching = 1,
        eLastMatching = 2
    };

    static s32 SEQ_HashName_454EA0(const char_type* seqFileName);
    static void LoadingLoop_41EAD0(s16 bShowLoadingIcon);
};

ALIVE_VAR_EXTERN(s16, bHideLoadingIcon_5076A0);
ALIVE_VAR_EXTERN(s32, loading_ticks_5076A4);
ALIVE_VAR_EXTERN(s32, gFilesPending_507714);
ALIVE_VAR_EXTERN(s16, bLoadingAFile_50768C);
ALIVE_VAR_EXTERN(DynamicArrayT<ResourceManager::ResourceManager_FileRecord>*, ObjList_5009E0);

ALIVE_VAR_EXTERN(u32, sManagedMemoryUsedSize_9F0E48);
ALIVE_VAR_EXTERN(u32, sPeakedManagedMemUsage_9F0E4C);

} // namespace AO
