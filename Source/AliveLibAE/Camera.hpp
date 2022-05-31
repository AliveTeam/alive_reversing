#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Primitives.hpp"

enum class LevelIds : s16;

#pragma pack(push)
#pragma pack(2)
class Camera final // TODO: May actually just be "ResourceList" ?
{
public:
    Camera();

public:
    void ctor_480DD0();
    void dtor_480E00();
    static void On_Loaded_480ED0(Camera* pCamera);

    DynamicArrayT<u8*> field_0;
    u8** field_C_pCamRes;
    s32 field_10_camera_resource_id;
    s16 field_14_xpos;
    s16 field_16_ypos;
    s16 field_18_path;
    LevelIds field_1A_level;
    s16 field_1C_camera_number;
    char_type field_1E_cam_name[16];
    s16 field_2E_padding;
    s16 field_30_flags;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(Camera, 0x32);
