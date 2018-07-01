#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class Camera;

class ResourceManager : public BaseGameObject
{
public:
    // TODO
    virtual void VDestructor(signed int);
    EXPORT void dtor_4649B0(signed int flags);
    EXPORT void dtor_4649E0();

    ResourceManager();
    EXPORT void Ctor_464910();

    EXPORT void Shutdown_465610();
    EXPORT void sub_465590(int a1);
    EXPORT static signed __int16 __cdecl LoadResourceFile_49C170(const char *pFileName, int a2);

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
