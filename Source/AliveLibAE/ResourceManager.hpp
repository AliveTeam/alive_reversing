#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/AddPointer.hpp"
#include "../relive_lib/Psx.hpp"

class Camera;

void Game_ShowLoadingIcon_482D80();

class ResourceManager final : public BaseGameObject
{
public:
    enum ResourceType : u32
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
        Resource_Pxtd = 0x50787464,   // Added for relive path extension blocks
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
        u32 mResourceType;
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


    using TLoaderFn = AddPointer_t<void CC(Camera*)>;

    struct ResourceManager_FilePartRecord_18 final
    {
        u32 field_0_type;
        u32 field_4_id;
        Camera* field_8_pCamera;
        Camera* field_C_fn_arg_pCamera;
        TLoaderFn field_10_pFn;
        s16 field_14_bAddUseCount;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord_18, 0x18);

    struct ResourceManager_FileRecord final
    {
        ResourceManager_FileRecord()
            : field_10_file_sections_dArray(3)
        {

        }
        char_type* field_0_fileName = nullptr;
        ResourcesToLoadList* field_4_pResourcesToLoadList = nullptr;
        u32 mResourceType = 0;
        u32 field_C_id = 0;
        DynamicArrayT<ResourceManager_FilePartRecord_18> field_10_file_sections_dArray;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord, 0x1C);

    ResourceManager();
    ~ResourceManager();
    void vLoadFile_StateMachine_464A70();
    void LoadingLoop(s16 bShowLoadingIcon);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void VScreenChanged_464EC0();

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

    static s32 SEQ_HashName_49BE30(const char_type* seqFileName);


private:
    enum LoadingStates : s16
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
    ResourceManager_FileRecord* field_2C_pFileItem = nullptr;
    s32 field_30_start_sector = 0;
    s32 field_34_num_sectors = 0;
    u8** field_38_ppRes = nullptr;
    Header* field_3C_pLoadingHeader = nullptr;
    s8 field_40_seek_attempts = 0;
    LoadingStates field_42_state = LoadingStates::State_Wait_For_Load_Request;
    CdlLOC field_44_cdLoc = {};
    DynamicArrayT<u8*> field_48_dArray ;
};
ALIVE_ASSERT_SIZEOF(ResourceManager, 0x54);

extern ResourceManager* pResourceManager;
extern u32 sManagedMemoryUsedSize_AB4A04;
extern u32 sPeakedManagedMemUsage_AB4A08;
extern s16 bHideLoadingIcon_5C1BAA;
extern s32 loading_ticks_5C1BAC;
