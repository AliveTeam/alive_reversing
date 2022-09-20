#pragma once

#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

class Camera final
{
public:
    Camera();
    ~Camera();

    void CreateFG1();

    static void On_Loaded_4447A0(Camera* pThis);

    DynamicArrayT<u8*> field_0_array;
    CamResource field_C_ppBits;
    s32 field_10_resId = 0;
    s16 field_14_cam_x = 0;
    s16 field_16_cam_y = 0;
    s16 field_18_path = 0;
    EReliveLevelIds field_1A_level = EReliveLevelIds::eNone;
    s16 field_1C = 0;
    char_type field_1E_fileName[18] = {};
    s16 field_30_flags = 0;
    s16 field_32 = 0;
    u32 field_1C_camera_number = 0;
};
ALIVE_ASSERT_SIZEOF(Camera, 0x34);

}