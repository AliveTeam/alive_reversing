#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"

struct Path_WellBase;
struct Path_WellLocal;
struct Path_WellExpress;

class Well final : public BaseGameObject
{
public:
    Well(Path_WellBase* pTlv, FP xpos, FP ypos, s32 tlvInfo);
    ~Well();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void WellExpress_Init(Path_WellExpress* pTlv, FP /*xpos*/, FP ypos);
    void WellLocal_Init(Path_WellLocal* pTlv, FP /*xpos*/, FP ypos);

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
