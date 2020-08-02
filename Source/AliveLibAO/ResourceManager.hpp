#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Psx.hpp"

START_NS_AO

class BaseAliveGameObject;
class Camera;

EXPORT void CC Game_ShowLoadingIcon_445EB0();

class ResourceManager
{
public:
    struct ResourcesToLoadList_Entry
    {
        DWORD field_0_type;
        DWORD field_4_res_id;
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList_Entry, 0x8);


    enum ResourceHeaderFlags : __int16
    {
        eLocked = 0x1,
        eOnlyAHeader = 0x2,
        eNeverFree = 0x4
    };

    // TODO: Replace/combine with CompileTimeResourceList when everything is decompiled
    struct ResourcesToLoadList
    {
        int field_0_count;
        ResourcesToLoadList_Entry field_4_items[1];
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList, 12);

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

    class ResourceManager_FileRecord : public BaseGameObject
    {
    public:
        int field_10;
        int field_14_fn;
        int field_18;
        int field_1C_array;
        int field_20;
        int field_24;
        __int16 field_28_state;
        CdlLOC field_2A;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord, 0x30);

    using TLoaderFn = std::add_pointer<void CC(Camera*)>::type;

    static EXPORT void CC CancelPendingResourcesFor_41EA60(BaseGameObject* pObj);

    static EXPORT void CC Reclaim_Memory_455660(DWORD sizeToReclaim);

    static EXPORT __int16 CC FreeResource_455550(BYTE** ppRes);

    static EXPORT void CC LoadResource_446C90(const char* pFileName, int type, int resourceId, __int16 loadMode, __int16 bDontLoad = FALSE);

    static EXPORT BYTE** CC GetLoadedResource_4554F0(int type, int resourceId, __int16 addUseCount, __int16 bLock);

    static EXPORT void CC LoadingLoop_41EAD0(__int16 bShowLoadingIcon);

    static EXPORT __int16 CC LoadResourceFile_455270(const char* filename, Camera* pCam, int allocMethod);

    static EXPORT BYTE** CC Allocate_New_Locked_Resource_454F80(int type, int id, int size);

    static EXPORT void CC Set_Header_Flags_4557D0(BYTE** ppRes, __int16 flags);

    template<class T, class Y>
    static EXPORT ResourceManager_FileRecord* CC LoadResourceFile(const char* pFileName, T pOnLoadFn, Y* pOnLoadFnArgument, Y* pCamera2 = nullptr)
    {
        // TODO: Change the camera types to void*'s
        return LoadResourceFile_4551E0(pFileName, reinterpret_cast<TLoaderFn>(pOnLoadFn), reinterpret_cast<Camera*>(pOnLoadFnArgument), reinterpret_cast<Camera*>(pCamera2));
    }

    static EXPORT ResourceManager_FileRecord* CC LoadResourceFile_4551E0(const char* pFileName, TLoaderFn fnOnLoad, Camera* pCamera1, Camera* pCamera2);

    static EXPORT void CC Free_Resource_Of_Type_455810(int type);

    static EXPORT void CC Free_Resources_For_Camera_447170(Camera* pCamera);

    static EXPORT void CC LoadResourcesFromList_446E80(const char* pFileName, ResourcesToLoadList* pList, __int16 loadMode, __int16);

    static EXPORT Header* CC Get_Header_455620(BYTE** ppRes);
};

ALIVE_VAR_EXTERN(short, bHideLoadingIcon_5076A0);
ALIVE_VAR_EXTERN(int, loading_ticks_5076A4);

enum ResourceID
{
    kUnknownResID_200 = 200,
    kUnknownResID_216 = 216,
    kUnknownResID_230 = 230
};

END_NS_AO
