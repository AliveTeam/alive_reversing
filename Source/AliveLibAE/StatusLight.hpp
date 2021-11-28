#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Path.hpp"

struct Path_StatusLight final : public Path_TLV
{
    s16 field_10_id;
    Scale_short field_12_scale;
    s16 field_14_id1;
    s16 field_16_id2;
    s16 field_18_id3;
    s16 field_1A_id4;
    s16 field_1C_id5;
    Choice_short field_1E_bIgnore_grid_snapping;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_StatusLight, 0x20);

class StatusLight final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT StatusLight* ctor_4D5250(Path_StatusLight* pTlv, u32 tlvInfo);

private:
    EXPORT void vUpdate_4D5530();
    EXPORT StatusLight* vdtor_4D5500(s32 flags);
    EXPORT void dtor_4D5790();
    EXPORT void vScreenChanged_4D5820();

private:
    u32 field_F4_tlvInfo;
    s16 field_F8_input_id;
    s16 field_FA_id1;
    s16 field_FC_id2;
    s16 field_FE_id3;
    s16 field_100_id4;
    s16 field_102_id5;
    Choice_short field_104_bIgnore_grid_snapping;
    FP field_108_xpos;
    FP field_10C_ypos;
};
ALIVE_ASSERT_SIZEOF(StatusLight, 0x110);
