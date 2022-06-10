#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Primitives.hpp"
#include "../relive_lib/MapWrapper.hpp"

enum class LevelIds : s16;

#pragma pack(push)
#pragma pack(2)
class Camera final // TODO: May actually just be "ResourceList" ?
{
public:
    Camera();
    ~Camera();

public:
    void Free();

    static void On_Loaded(Camera* pCamera);

    DynamicArrayT<u8*> field_0;
    u8** field_C_pCamRes;
    s32 field_10_camera_resource_id;
    s16 mCamXOff;
    s16 mCamYOff;
    s16 field_18_path;
    EReliveLevelIds field_1A_level;
    s16 field_1C_camera_number;
    char_type field_1E_cam_name[16];
    s16 field_30_flags;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(Camera, 0x32);
