#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Psx.hpp"
#include "AnimResources.hpp"
#include "Factory.hpp"

namespace AO {

class BaseAliveGameObject;
class Camera;
class ResourceManager_FileRecord_Unknown;

EXPORT void CC Game_ShowLoadingIcon_445EB0();

// TODO Argument type is always Camera* ?
using TLoaderFn = void(CC*)(void*);

class ResourceManager
{
public:
    enum ResourceType : DWORD
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

    struct Header
    {
        DWORD field_0_size;
        s16 field_4_ref_count;
        s16 field_6_flags;
        DWORD field_8_type;
        DWORD field_C_id;
    };
    ALIVE_ASSERT_SIZEOF(Header, 0x10);

    struct ResourcesToLoadList_Entry
    {
        DWORD field_0_type;
        DWORD field_4_res_id;
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList_Entry, 0x8);

    // TODO: Replace/combine with CompileTimeResourceList when everything is decompiled
    struct ResourcesToLoadList
    {
        s32 field_0_count;
        ResourcesToLoadList_Entry field_4_items[1];
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList, 12);

    struct ResourceManager_FilePartRecord
    {
        s32 field_0_type;
        s32 field_4_res_id;
        Camera* field_8_pCamera;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord, 0xc);

    class ResourceManager_FileRecord
    {
    public:
        void dtor_447510()
        {
            field_10_file_sections_dArray.dtor_404440();
        }

        const char* field_0_fileName;
        ResourcesToLoadList* field_4_pResourcesToLoadList;
        DWORD field_8_type;
        DWORD field_C_resourceId;
        DynamicArrayT<ResourceManager_FilePartRecord> field_10_file_sections_dArray;
        ResourceManager_FileRecord_Unknown* field_1C_pGameObjFileRec;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord, 0x20);

    struct ResourceHeapItem
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

    static EXPORT s32 CC SEQ_HashName_454EA0(const char* seqFileName);

    EXPORT static void CC Init_454DA0();

    static ResourceHeapItem* Push_List_Item();

    static void Pop_List_Item(ResourceHeapItem* pListItem);

    static ResourceHeapItem* Split_block(ResourceHeapItem* pItem, s32 size);

    static EXPORT void CC On_Loaded_446C10(ResourceManager_FileRecord* pLoaded);

    static EXPORT s16 CC Move_Resources_To_DArray_455430(u8** ppRes, DynamicArrayT<u8*>* pArray);

    static u8** Alloc_New_Resource_Impl(DWORD type, DWORD id, DWORD size, bool locked, BlockAllocMethod allocType);

    static EXPORT u8** CC Alloc_New_Resource_454F20(DWORD type, DWORD id, DWORD size);

    static EXPORT u8** CC Allocate_New_Block_454FE0(DWORD sizeBytes, BlockAllocMethod allocMethod);

    static EXPORT s16 CC FreeResource_Impl_4555B0(u8* handle);

    static EXPORT void CC Increment_Pending_Count_4557A0();

    static EXPORT void CC Decrement_Pending_Count_4557B0();

    static EXPORT s32 CC Is_Resources_Pending_4557C0();

    static EXPORT void CC WaitForPendingResources_41EA60(BaseGameObject* pObj);

    static EXPORT void CC Reclaim_Memory_455660(DWORD sizeToReclaim);

    static EXPORT s16 CC FreeResource_455550(u8** ppRes);

    static EXPORT void CC LoadResource_446C90(const char* pFileName, DWORD type, DWORD resourceId, LoadMode loadMode, s16 bDontLoad = FALSE);

  
    static EXPORT u8** CC GetLoadedResource_4554F0(DWORD type, DWORD resourceId, s16 addUseCount, s16 bLock);

    static void CheckResourceIsLoaded(DWORD type, ResourceID resourceId);
    static void CheckResourceIsLoaded(DWORD type, std::initializer_list<ResourceID>& resourceIds);

    static EXPORT void CC LoadingLoop_41EAD0(s16 bShowLoadingIcon);

    static s16 CC LoadResourceFileWrapper(const char* filename, Camera* pCam);

    static EXPORT s16 CC LoadResourceFile_455270(const char* filename, Camera* pCam, BlockAllocMethod allocMethod = BlockAllocMethod::eFirstMatching);

    static EXPORT u8** CC Allocate_New_Locked_Resource_454F80(DWORD type, DWORD id, DWORD size);

    static EXPORT void CC Set_Header_Flags_4557D0(u8** ppRes, s16 flags);

    static EXPORT void CC Clear_Header_Flags_4557F0(u8** ppRes, s16 flags);

    template<class T, class Y>
    static EXPORT ResourceManager_FileRecord_Unknown* CC LoadResourceFile(const char* pFileName, T pOnLoadFn, Y* pOnLoadFnArgument, Y* pCamera2 = nullptr)
    {
        // TODO: Change the camera types to void*'s
        return LoadResourceFile_4551E0(pFileName, reinterpret_cast<TLoaderFn>(pOnLoadFn), reinterpret_cast<Camera*>(pOnLoadFnArgument), reinterpret_cast<Camera*>(pCamera2));
    }

    static EXPORT ResourceManager_FileRecord_Unknown* CC LoadResourceFile_4551E0(const char* pFileName, TLoaderFn fnOnLoad, Camera* pCamera1, Camera* pCamera2);

    static EXPORT void CC Free_Resource_Of_Type_455810(DWORD type);

    static EXPORT void CC Free_Resources_For_Camera_447170(Camera* pCamera);

    static EXPORT void CC LoadResourcesFromList_446E80(const char* pFileName, ResourcesToLoadList* pList, LoadMode loadMode, s16);

    static EXPORT Header* CC Get_Header_455620(u8** ppRes);
};

ALIVE_VAR_EXTERN(s16, bHideLoadingIcon_5076A0);
ALIVE_VAR_EXTERN(s32, loading_ticks_5076A4);
ALIVE_VAR_EXTERN(s32, gFilesPending_507714);
ALIVE_VAR_EXTERN(s16, bLoadingAFile_50768C);
ALIVE_VAR_EXTERN(DynamicArrayT<ResourceManager::ResourceManager_FileRecord>*, ObjList_5009E0);

ALIVE_VAR_EXTERN(DWORD, sManagedMemoryUsedSize_9F0E48);
ALIVE_VAR_EXTERN( DWORD, sPeakedManagedMemUsage_9F0E4C);

}
