#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

union OffLevelOrDx
{
    LevelIds level;
    s16 dx;
};

struct Path_WellBase : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_switch_id;
    s16 field_1C_other_well_id;
    s16 field_1E_anim_id;
    s16 field_20_exit_x;
    s16 field_22_exit_y;
    OffLevelOrDx field_24_off_level_or_dx;
    s16 field_26_off_path_or_dy;
};
ALIVE_ASSERT_SIZEOF(Path_WellBase, 0x28);

struct Path_WellExpress final : public Path_WellBase
{
    s16 field_28_off_camera;
    s16 field_2A_off_well_id;
    LevelIds field_2C_on_level;
    s16 field_2E_on_path;
    s16 field_30_on_camera;
    s16 field_32_on_well_id;
    s16 field_34_emit_leaves;
    s16 field_36_leaf_x;
    s16 field_38_leaf_y;
    s16 field_3A_movie_id;
};
ALIVE_ASSERT_SIZEOF(Path_WellExpress, 0x3C);

struct Path_WellLocal final : public Path_WellBase
{
    s16 field_28_on_dx;
    s16 field_2A_on_dy;
    s16 field_2C_bEmit_leaves;
    s16 field_2E_leaf_x;
    s16 field_30_leaf_y;
    s16 field_32_pad;
};

ALIVE_ASSERT_SIZEOF(Path_WellLocal, 0x34);

class Well final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Well* ctor_48AEE0(Path_WellBase* pTlv, FP xpos, FP ypos, s32 tlvInfo);

    EXPORT void WellLocal_Init_48AFA0(Path_WellLocal* pTlv, FP xpos, FP ypos);

    EXPORT void WellExpress_Init_48B110(Path_WellExpress* pTlv, FP xpos, FP ypos);

    virtual void VUpdate() override;

    EXPORT void VUpdate_48B270();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_48B3D0(PrimHeader** ppOt);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_48B450();

    EXPORT BaseGameObject* dtor_48B460();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Well* vdtor_48B4D0(s32 flags);

    s32 field_D4[4];
    s32 field_E4_tlvInfo;
    s16 field_E8_switch_id;
    // pad
    FP field_EC_scale;
    FP field_F0_exit_x;
    FP field_F4_exit_y;
    FP field_F8_leaf_xpos;
    FP field_FC_leaf_ypos;
    s16 field_100_emit_leaves;
    s16 field_102_pad;
};
ALIVE_ASSERT_SIZEOF(Well, 0x104);

} // namespace AO
