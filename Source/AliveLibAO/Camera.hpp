#pragma once

#include "../relive_lib/DynamicArray.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

class Camera final
{
public:
    Camera();
    ~Camera();

    static void On_Loaded_4447A0(Camera* pThis);

    DynamicArrayT<u8*> field_0_array;
    u8** field_C_ppBits;
    s32 field_10_resId;
    s16 field_14_cam_x;
    s16 field_16_cam_y;
    s16 field_18_path;
    EReliveLevelIds field_1A_level;
    s16 field_1C;
    char_type field_1E_fileName[18];
    s16 field_30_flags;
    s16 field_32;
};
ALIVE_ASSERT_SIZEOF(Camera, 0x34);

}