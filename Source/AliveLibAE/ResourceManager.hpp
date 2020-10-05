#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Camera.hpp"
#include "AnimResources.hpp"

EXPORT void CC Game_ShowLoadingIcon_482D80();

class ResourceManager : public BaseGameObject
{
public:
    enum ResourceType : DWORD
    {
        Resource_VLC = 0x20434C56,
        Resource_End = 0x21646E45,
        Resource_CHNK = 0x4B4E4843,
        Resource_SEQp = 0x53455170,
        Resource_Pend = 0x646E6550,
        Resource_Free = 0x65657246,
        Resource_PBuf = 0x66754250,
        Resource_Path = 0x68746150,
        Resource_Animation = 0x6D696E41,
        Resource_Demo = 0x6F6D6544,
        Resource_Bits = 0x73746942,
        Resource_Palt = 0x746C6150,
        Resource_Font = 0x746E6F46,
        Resource_FntP = 0x50746E46,
        Resource_Seq = 0x20716553,
        Resource_VabHeader = 0x48424156,
        Resource_VabBody = 0x42424156,
        Resource_FG1 = 0x20314746,
        Resource_NxtP = 0x5074784E,
        Resource_Sprx = 0x78727053,
        Resource_Blood = 0x64756C42,
        Resource_Water = 0x72746157,
        Resource_DecompressionBuffer = 0x66754244,
        Resource_Rope = 0x65706F52,
        Resource_Wave = 0x65766157,
        Resource_3DGibs = 0x65444433,
        Resource_Web = 0x20626557,
        Resource_Spline = 0x6e6c7053, // Went with Spline as in ZapLine.cpp.
    };

    enum ResourceHeaderFlags : __int16
    {
        eLocked = 0x1,
        eOnlyAHeader = 0x2,
        eNeverFree = 0x4
    };

    struct Header
    {
        DWORD field_0_size;
        __int16 field_4_ref_count;
        __int16 field_6_flags;
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
        int field_0_count;
        ResourcesToLoadList_Entry field_4_items[1];
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList, 12);


    using TLoaderFn = std::add_pointer<void CC(Camera*)>::type;

    struct ResourceManager_FilePartRecord_18
    {
        DWORD field_0_type;
        DWORD field_4_id;
        Camera *field_8_pCamera;
        Camera* field_C_fn_arg_pCamera;
        TLoaderFn field_10_pFn;
        __int16 field_14_bAddUseCount;
        __int16 field_16_padding;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord_18, 0x18);

    struct ResourceManager_FileRecord
    {
        char* field_0_fileName;
        ResourcesToLoadList* field_4_pResourcesToLoadList;
        DWORD field_8_type;
        DWORD field_C_id;
        DynamicArrayT<ResourceManager_FilePartRecord_18> field_10_file_sections_dArray;

        EXPORT void dtor_464EA0();
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord, 0x1C);

    EXPORT ResourceManager* ctor_464910();
    EXPORT BaseGameObject* vdtor_4649B0(signed int flags);
    EXPORT void dtor_4649E0();
    EXPORT void vLoadFile_StateMachine_464A70();
    EXPORT void OnResourceLoaded_464CE0();
    EXPORT void LoadResource_464EE0(const char* pFileItem, DWORD type, DWORD resourceID, Camera* pCamera, Camera* pFnArg, TLoaderFn pFn, __int16 bAddUseCount);
    EXPORT void LoadResourcesFromList_465150(const char* pFileName, ResourcesToLoadList* pTypeAndIdList, Camera* pCamera, Camera* pFnArg, TLoaderFn pFn, __int16 addUseCount);
    EXPORT void LoadResourceFile_465460(const char* filename, Camera* pCam, Camera* a4, TLoaderFn pFn, __int16 a6);
    EXPORT void LoadingLoop_465590(__int16 bShowLoadingIcon);
    EXPORT void Shutdown_465610();
    EXPORT void Free_Resources_For_Camera_4656F0(const Camera* pCamera);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_464EC0();

    ResourceManager();

    struct ResourceHeapItem
    {
        BYTE* field_0_ptr;
        ResourceHeapItem* field_4_pNext;
    };
    ALIVE_ASSERT_SIZEOF(ResourceHeapItem, 0x8);

    enum BlockAllocMethod : int
    {
        eFirstMatching = 0,
        eNearestMatching = 1,
        eLastMatching = 2
    };

    EXPORT static void CC Init_49BCE0();
    EXPORT static ResourceHeapItem* CC Push_List_Item_49BD70();
    EXPORT static void CC Pop_List_Item_49BD90(ResourceHeapItem* pListItem);
    EXPORT static ResourceHeapItem* CC Split_block_49BDC0(ResourceHeapItem* pItem, int size);
    EXPORT static int CC SEQ_HashName_49BE30(const char* seqFileName);
    static BYTE** Alloc_New_Resource_Impl(DWORD type, DWORD id, DWORD size, bool locked, ResourceManager::BlockAllocMethod allocType);
    EXPORT static BYTE** CC Alloc_New_Resource_49BED0(DWORD type, DWORD id, DWORD size);
    EXPORT static BYTE** CC Allocate_New_Locked_Resource_49BF40(DWORD type, DWORD id, DWORD size);
    EXPORT static BYTE** CC Allocate_New_Block_49BFB0(int sizeBytes, BlockAllocMethod allocMethod);
    EXPORT static int CC LoadResourceFile_49C130(const char* filename, TLoaderFn pFn, Camera* a4, Camera* pCamera);
    EXPORT static signed __int16 CC LoadResourceFile_49C170(const char* pFileName, Camera* pCamera);
    EXPORT static signed __int16 CC Move_Resources_To_DArray_49C1C0(BYTE** ppRes, DynamicArrayT<BYTE*>* pArray);
    EXPORT static BYTE** CC GetLoadedResource_49C2A0(DWORD type, DWORD resourceID, unsigned __int16 addUseCount, unsigned __int16 bLock);
    EXPORT static void CC Inc_Ref_Count_49C310(BYTE **ppRes);
    EXPORT static signed __int16 CC FreeResource_49C330(BYTE** handle);
    EXPORT static signed __int16 CC FreeResource_Impl_49C360(BYTE* handle);
    EXPORT static Header* CC Get_Header_49C410(BYTE** ppRes);
    EXPORT static void CC Reclaim_Memory_49C470(unsigned int size);
    EXPORT static void CC Increment_Pending_Count_49C5F0();
    EXPORT static void CC Decrement_Pending_Count_49C610();
    EXPORT static void CC Set_Header_Flags_49C650(BYTE** ppRes, __int16 flags);
    EXPORT static void CC Free_Resource_Of_Type_49C6B0(DWORD type);
    EXPORT static void CC NoEffect_49C700();


private:

    enum LoadingStates : __int16
    {
        State_Wait_For_Load_Request = 0,
        State_Allocate_Memory_For_File = 1,
        State_Seek_To_File = 2,
        State_Read_Sectors_ASync = 3,
        State_Wait_For_Read_Complete = 4,
        State_File_Read_Completed = 5,
        State_Load_Completed = 6
    };

    DynamicArrayT<ResourceManager_FileRecord> field_20_files_pending_loading;
    ResourceManager_FileRecord* field_2C_pFileItem;
    int field_30_start_sector;
    int field_34_num_sectors;
    BYTE** field_38_ppRes;
    Header* field_3C_pLoadingHeader;
    char field_40_seek_attempts;
    LoadingStates field_42_state;
    CdlLOC field_44_cdLoc;
    DynamicArrayT<BYTE*> field_48_dArray;
};
ALIVE_ASSERT_SIZEOF(ResourceManager, 0x54);

ALIVE_VAR_EXTERN(ResourceManager*, pResourceManager_5C1BB0);
ALIVE_VAR_EXTERN(DWORD, sManagedMemoryUsedSize_AB4A04);
ALIVE_VAR_EXTERN(DWORD, sPeakedManagedMemUsage_AB4A08);
ALIVE_VAR_EXTERN(short, sbLoadingInProgress_5C1B96);
ALIVE_VAR_EXTERN(short, bHideLoadingIcon_5C1BAA);
ALIVE_VAR_EXTERN(int, loading_ticks_5C1BAC);
