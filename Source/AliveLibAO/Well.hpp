#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_WellBase : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_trigger_id;
    __int16 field_1C_well_id;
    __int16 field_1E_res_id;
    __int16 field_20_exit_x;
    __int16 field_22_exit_y;
    LevelIds field_24_off_level_or_dx;
    __int16 field_26_off_path_or_dy;
};
ALIVE_ASSERT_SIZEOF(Path_WellBase, 0x28);

struct Path_WellExpress : public Path_WellBase
{
    __int16 field_28_off_camera;
    __int16 field_2A_off_well_id;
    LevelIds field_2C_on_level;
    __int16 field_2E_on_path;
    __int16 field_30_on_camera;
    __int16 field_32_on_well_id;
    __int16 field_34_emit_leaves;
    __int16 field_36_leaf_x;
    __int16 field_38_leaf_y;
    __int16 field_3A_movie_id;
};
ALIVE_ASSERT_SIZEOF(Path_WellExpress, 0x3C);

struct Path_WellLocal : public Path_WellBase
{
    __int16 field_28_on_dx;
    __int16 field_2A_on_dy;
    __int16 field_2C_emit_leaves;
    __int16 field_2E_leaf_x;
    __int16 field_30_leaf_y;
    __int16 field_32_pad;
};

ALIVE_ASSERT_SIZEOF(Path_WellLocal, 0x34);

class Well : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Well* ctor_48AEE0(Path_WellBase* pTlv, FP xpos, FP ypos, int tlvInfo);

    EXPORT void WellLocal_Init_48AFA0(Path_WellLocal* pTlv, FP xpos, FP ypos);

    EXPORT void WellExpress_Init_48B110(Path_WellExpress* pTlv, FP xpos, FP ypos);

    virtual void VUpdate() override;

    EXPORT void VUpdate_48B270();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_48B3D0(PrimHeader** ppOt);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_48B450();

    EXPORT BaseGameObject* dtor_48B460();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Well* vdtor_48B4D0(signed int flags);

    int field_D4[4];
    int field_E4_tlvInfo;
    short field_E8_trigger_id;
    // pad
    FP field_EC_scale;
    FP field_F0_exit_x;
    FP field_F4_exit_y;
    FP field_F8_leaf_xpos;
    FP field_FC_leaf_ypos;
    __int16 field_100_emit_leaves;
    __int16 field_102_pad;
};
ALIVE_ASSERT_SIZEOF(Well, 0x104);

}

