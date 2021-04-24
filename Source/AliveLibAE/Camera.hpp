#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"

enum class LevelIds : s16;

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
    s16 field_14_xpos;
    s16 field_16_ypos;
    s16 field_18_path;
    LevelIds field_1A_level;
    s16 field_1C_camera_number;
    char field_1E_cam_name[16];
    s16 field_2E_padding;
    s16 field_30_flags;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(Camera, 0x32);
