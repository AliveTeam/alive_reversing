#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "Primitives.hpp"

// TODO: These can be combined
struct Path_LaughingGas_Data
{
    Choice_short field_0_bLaughing_gas;
    s16 field_2_laughing_gas_id;
    s16 field_4_red_percent;
    s16 field_6_blue_percent;
    s16 field_8_green_percent;
    s16 field_A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas_Data, 0xC);

struct Path_LaughingGas : public Path_TLV
{
    Choice_short field_10_bLaughing_gas;
    s16 field_12_laughing_gas_id;
    s16 field_14_red_percent;
    s16 field_16_green_percent;
    s16 field_18_blue_percent;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas, 0x1C);


class LaughingGas : public BaseGameObject
{
public:
    EXPORT LaughingGas* ctor_432400(Layer layer, s32 notUsed, Path_LaughingGas* pTlv, s32 tlvInfo);

    EXPORT void dtor_432B80();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT void vScreenChanged_432DE0();

    EXPORT void Init_432980();

    EXPORT LaughingGas* vdtor_432670(s32 flags);

    EXPORT void vRender_432D10(PrimHeader** ppOt);

    EXPORT void DoRender_432740();

    EXPORT s16 CounterOver_432DA0();

    EXPORT void vUpdate_432C40();

    EXPORT f32 Calc_X_4326A0(f32* a2, s32 xIndex);
    EXPORT f32 Calc_Y_4326F0(f32* a2, s32 yIndex);

    EXPORT void sub_4328A0();

private:
    s32 field_20_padding;
    s32 field_24_tlvInfo;
    s16 field_28_y;
    s16 field_2A_x;
    s16 field_2C_h;
    s16 field_2E_w;
    s32 field_30_padding;
    s16 field_34_bEnabled;
    Choice_short field_36_bLaughing_gas;
    s32 field_38_padding;
    s32 field_3C_padding;
    s32 field_40_padding;
    s32 field_44_padding;
    Path_LaughingGas_Data field_48_tlv_data;
    FP field_54_amount_on;
    Layer field_58_layer;
    s16 field_5A_padding;

    Prim_GasEffect field_5C_prim;

    f32 field_7C_gas_y[6][6];
    f32 field_10C_gas_x[6][6];

    u16* field_19C_pMem;

    struct Data
    {
        f32 array_4[7];
    };

    Data field_1A0_x_data[321];  // (640+2)/2
    Data field_24D0_y_data[121]; // (480+4)/4

    s32 field_31F8_w_count;
    s32 field_31FC_h_count;
};
ALIVE_ASSERT_SIZEOF(LaughingGas, 0x3200);

ALIVE_VAR_EXTERN(s32, gGasInstanceCount_5BC214);
ALIVE_VAR_EXTERN(s16, gLaughingGasOn_5C1BA4);
