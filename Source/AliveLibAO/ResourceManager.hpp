#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Psx.hpp"

START_NS_AO

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
        Resource_Animation = 0x6D696E41,
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

    struct ResourceManager_FilePartRecord
    {
        int field_0_ResId;
        int field_4_bAddUsecount;
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
        int field_8_resId;
        int field_C_addUseCount;
        DynamicArrayT<ResourceManager_FilePartRecord> field_10_file_sections_dArray;
        ResourceManager_FileRecord_Unknown* field_1C_pGameObjFileRec;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord, 0x20);

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

    static EXPORT int CC SEQ_HashName_454EA0(const char* seqFileName);

    EXPORT static void CC Init_454DA0();

    static EXPORT void CC On_Loaded_446C10(ResourceManager_FileRecord* pLoaded);

    static EXPORT __int16 CC Move_Resources_To_DArray_455430(BYTE** ppRes, DynamicArrayT<BYTE*>* pArray);

    static BYTE** Alloc_New_Resource_Impl(DWORD type, DWORD id, DWORD size, bool locked, BlockAllocMethod allocType);

    static EXPORT BYTE** CC Alloc_New_Resource_454F20(DWORD type, DWORD id, DWORD size);

    static EXPORT BYTE** CC Allocate_New_Block_454FE0(DWORD sizeBytes, BlockAllocMethod allocMethod);

    static EXPORT __int16 CC FreeResource_Impl_4555B0(BYTE* handle);

    static EXPORT void CC Increment_Pending_Count_4557A0();

    static EXPORT void CC Decrement_Pending_Count_4557B0();

    static EXPORT int CC Is_Resources_Pending_4557C0();

    static EXPORT void CC WaitForPendingResources_41EA60(BaseGameObject* pObj);

    static EXPORT void CC Reclaim_Memory_455660(DWORD sizeToReclaim);

    static EXPORT __int16 CC FreeResource_455550(BYTE** ppRes);

    static EXPORT void CC LoadResource_446C90(const char* pFileName, int type, int resourceId, __int16 loadMode, __int16 bDontLoad = FALSE);

    static EXPORT BYTE** CC GetLoadedResource_4554F0(DWORD type, DWORD resourceId, __int16 addUseCount, __int16 bLock);

    static EXPORT void CC LoadingLoop_41EAD0(__int16 bShowLoadingIcon);

    static EXPORT __int16 CC LoadResourceFile_455270(const char* filename, Camera* pCam, BlockAllocMethod allocMethod = BlockAllocMethod::eFirstMatching);

    static EXPORT BYTE** CC Allocate_New_Locked_Resource_454F80(DWORD type, DWORD id, DWORD size);

    static EXPORT void CC Set_Header_Flags_4557D0(BYTE** ppRes, __int16 flags);

    static EXPORT void CC Clear_Header_Flags_4557F0(BYTE** ppRes, __int16 flags);

    template<class T, class Y>
    static EXPORT ResourceManager_FileRecord_Unknown* CC LoadResourceFile(const char* pFileName, T pOnLoadFn, Y* pOnLoadFnArgument, Y* pCamera2 = nullptr)
    {
        // TODO: Change the camera types to void*'s
        return LoadResourceFile_4551E0(pFileName, reinterpret_cast<TLoaderFn>(pOnLoadFn), reinterpret_cast<Camera*>(pOnLoadFnArgument), reinterpret_cast<Camera*>(pCamera2));
    }

    static EXPORT ResourceManager_FileRecord_Unknown* CC LoadResourceFile_4551E0(const char* pFileName, TLoaderFn fnOnLoad, Camera* pCamera1, Camera* pCamera2);

    static EXPORT void CC Free_Resource_Of_Type_455810(DWORD type);

    static EXPORT void CC Free_Resources_For_Camera_447170(Camera* pCamera);

    static EXPORT void CC LoadResourcesFromList_446E80(const char* pFileName, ResourcesToLoadList* pList, __int16 loadMode, __int16);

    static EXPORT Header* CC Get_Header_455620(BYTE** ppRes);
};

ALIVE_VAR_EXTERN(short, bHideLoadingIcon_5076A0);
ALIVE_VAR_EXTERN(int, loading_ticks_5076A4);
ALIVE_VAR_EXTERN(int, gFilesPending_507714);
ALIVE_VAR_EXTERN(short, bLoadingAFile_50768C);
ALIVE_VAR_EXTERN(DynamicArrayT<ResourceManager::ResourceManager_FileRecord>*, ObjList_5009E0);

enum ResourceID
{
    kUnknownResID_200 = 200,
    kUnknownResID_216 = 216,
    kUnknownResID_230 = 230,
    kUnknownResID_301 = 301,
    kOmmflareResID_312 = 312,
    kZflashResID_344 = 344,
    kDeathFlareResID_349 = 349
};

END_NS_AO
