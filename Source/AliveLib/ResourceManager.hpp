#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"

class Camera;

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

    class RawHandle
    {
    public:
        RawHandle() : mResource(nullptr) { }
        RawHandle(BYTE* res) : mResource(res) {}

        Header* GetHeader()
        {
            return reinterpret_cast<Header*>((mResource - sizeof(Header)));
        }

        bool Valid() const { return mResource != nullptr; }

    private:
        BYTE* mResource = nullptr;
    };

    class BaseHandle
    {
    public:
        BaseHandle() : mResource(nullptr) { }
        BaseHandle(BYTE** res) : mResource(res) {}

        Header* GetHeader()
        {
            return reinterpret_cast<Header*>((*mResource - sizeof(Header)));
        }

        RawHandle ToRawHandle()
        {
            return RawHandle(*mResource);
        }

        bool Valid() const { return mResource != nullptr; }
        void Clear() { mResource = nullptr; }
    protected:
        BYTE** mResource = nullptr;
    };

    template<class T>
    class Handle : public BaseHandle
    {
    public:
        using BaseHandle::BaseHandle;

        T operator ->()
        {
            // TODO: Can assert on GetHeader()->field_8_type matching T
            return reinterpret_cast<T>(*mResource);
        }

        T operator & ()
        {
            return reinterpret_cast<T>(*mResource);
        }
    };

    // TODO
    virtual void VDestructor(signed int);
    EXPORT void dtor_4649B0(signed int flags);
    EXPORT void dtor_4649E0();

    ResourceManager();
    EXPORT void Ctor_464910();

    EXPORT void Shutdown_465610();
    EXPORT void LoadingLoop_465590(__int16 bShowLoadingIcon);
    
    using TLoaderFn = std::add_pointer<void CC(Camera*)>::type;
    EXPORT void LoadResourceFile_465460(const char* filename, Camera* pCam, Camera* a4, TLoaderFn pFn, __int16 a6);
    EXPORT void vLoadFile_StateMachine_464A70();

    EXPORT static signed __int16 __cdecl LoadResourceFile_49C170(const char *pFileName, Camera* a2);
    EXPORT static void* CC GetLoadedResource_49C2A0(DWORD type, int resourceID, unsigned __int16 addUseCount, __int16 a4);
    EXPORT static signed __int16 CC FreeResource_49C330(BaseHandle handle);
    EXPORT static signed __int16 CC FreeResource_Impl_49C360(RawHandle handle);
    
    // Helper to avoid casting raw types
    template<class T>
    static Handle<T> CC Allocate_New_Block_49BFB0_T(int sizeBytes, int allocMethod)
    {
        return Allocate_New_Block_49BFB0(sizeBytes, allocMethod);
    }

    EXPORT static BYTE** CC Allocate_New_Block_49BFB0(int sizeBytes, int allocMethod);
private:
    struct ResourceManager_FilePartRecord_18;

    struct ResourceManager_FileRecord_1C
    {
        char* field_0_fileName;
        int field_4;
        int field_8;
        int field_C;
        DynamicArrayT<ResourceManager_FilePartRecord_18> field_10_file_sections_dArray;

        EXPORT void dtor_464EA0();
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord_1C, 0x1C);

    struct ResourceManager_FilePartRecord_18
    {
        int field_0_type;
        int field_4_id;
        Camera *field_8_pCamera;
        Camera* field_C_fn_arg_pCamera;
        TLoaderFn field_10_pFn;
        __int16 field_14;
        __int16 field_16;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord_18, 0x18);

    DynamicArrayT<ResourceManager_FileRecord_1C> field_20_files_dArray;
    ResourceManager_FileRecord_1C* field_2C_pFileItem;
    int field_30_start_sector;
    int field_34_num_sectors;
    BYTE** field_38_ppRes;
    void* field_3C_pLoadingHeader;
    char field_40_seek_attempts;
    char field_41; // pad ?
    __int16 field_42_state;
    CdlLOC field_44_cdLoc;
    DynamicArray field_48_dArray;
};
ALIVE_ASSERT_SIZEOF(ResourceManager, 0x54);

ALIVE_VAR_EXTERN(ResourceManager*, pResourceManager_5C1BB0);
ALIVE_VAR_EXTERN(int, sManagedMemoryUsedSize_AB4A04);
