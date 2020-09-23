#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"

enum class LevelIds : __int16;

#pragma pack(push)
#pragma pack(2)
class Camera // TODO: May actually just be "ResourceList" ?
{
public:
    Camera();
public:
    EXPORT void ctor_480DD0();
    EXPORT void dtor_480E00();
    EXPORT static void CC On_Loaded_480ED0(Camera* pCamera);

    DynamicArrayT<BYTE*> field_0;
    BYTE** field_C_pCamRes;
    int field_10_camera_resource_id;
    __int16 field_14_xpos;
    __int16 field_16_ypos;
    __int16 field_18_path;
    LevelIds field_1A_level;
    __int16 field_1C_camera_number;
    char field_1E_cam_name[13];
    int field_2B_padding[3];
    __int16 field_2E_padding;
    __int16 field_30_flags;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(Camera, 0x32);
