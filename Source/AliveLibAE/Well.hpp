#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Path.hpp"

struct Path_WellBase;
struct Path_WellLocal;
struct Path_WellExpress;

class Well final : public BaseGameObject
{
public:
    EXPORT Well* ctor_4E2BE0(Path_WellBase* pTlv, FP xpos, FP ypos, s32 tlvInfo);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void WellExpress_Init_4E2E00(Path_WellExpress* pTlv, FP /*xpos*/, FP ypos);
    EXPORT void WellLocal_Init_4E2CD0(Path_WellLocal* pTlv, FP /*xpos*/, FP ypos);
    EXPORT Well* vdtor_4E2CA0(s32 flags);
    EXPORT void dtor_4E3090();
    EXPORT void vScreenChanged_4E3070();
    EXPORT void vUpdate_4E2F60();

private:
    s32 field_20_tlvInfo;
    s16 field_24_switch_id;
    s16 field_26_padding;
    FP field_28_scale;
    FP field_2C_exit_x;
    FP field_30_exit_y;
    FP field_34_leaf_xpos;
    FP field_38_leaf_ypos;
    Choice_short field_3C_bEmitLeaves;
    s16 field_3E_padding;
};
ALIVE_ASSERT_SIZEOF(Well, 0x40);
