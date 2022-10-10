#pragma once

#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"

enum class LevelIds : s16;

#pragma pack(push)
#pragma pack(2)
class Camera final // TODO: May actually just be "ResourceList" ?
{
public:
    Camera();
    ~Camera();
    void LoadFG1();

public:
    void Free();

    static void On_Loaded(Camera* pCamera);

    DynamicArrayT<u8*> field_0;
    CamResource field_C_pCamRes;
    s32 field_10_camera_resource_id = 0;
    s16 mCamXOff = 0;
    s16 mCamYOff = 0;
    s16 field_18_path = 0;
    EReliveLevelIds field_1A_level = EReliveLevelIds::eNone;
    s16 field_1C_camera_number = 0;
    char_type field_1E_cam_name[16] = {};
    s16 field_30_flags = 0;
};
#pragma pack(pop)
