#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

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

    // TODO
    virtual void VDestructor(signed int);
    EXPORT void dtor_4649B0(signed int flags);
    EXPORT void dtor_4649E0();

    ResourceManager();
    EXPORT void Ctor_464910();

    EXPORT void Shutdown_465610();
    EXPORT void sub_465590(int a1);
    EXPORT static signed __int16 __cdecl LoadResourceFile_49C170(const char *pFileName, Camera* a2);

    EXPORT static void* CC GetLoadedResource_49C2A0(DWORD type, int resourceID, unsigned __int16 addUseCount, __int16 a4);

private:
    struct ResourceManager_FileRecord_1C
    {
        char* field_0_fileName;
        int field_4;
        int field_8;
        int field_C;
        DynamicArray field_10_file_sections_dArray;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord_1C, 0x1C);

    struct ResourceManager_FilePartRecord_18
    {
        int field_0;
        int field_4;
        Camera *field_8_pCamera;
        int field_C;
        int field_10;
        __int16 field_14;
        __int16 field_16;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord_18, 0x18);

    DynamicArray field_20_files_dArray;
    ResourceManager_FileRecord_1C* field_2C_pFileItem;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    char field_40;
    char field_41;
    __int16 field_42_state;
    int field_44;
    DynamicArray field_48_dArray;
};
ALIVE_ASSERT_SIZEOF(ResourceManager, 0x54);

ALIVE_VAR_EXTERN(ResourceManager*, pResourceManager_5C1BB0);
