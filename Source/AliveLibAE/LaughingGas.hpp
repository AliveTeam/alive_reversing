#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "Primitives.hpp"

// TODO: These can be combined
struct Path_LaughingGas_Data
{
    Choice_short field_0_bLaughing_gas;
    __int16 field_2_laughing_gas_id;
    __int16 field_4_red_percent;
    __int16 field_6_blue_percent;
    __int16 field_8_green_percent;
    __int16 field_A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas_Data, 0xC);

struct Path_LaughingGas : public Path_TLV
{
    Choice_short field_10_bLaughing_gas;
    __int16 field_12_laughing_gas_id;
    __int16 field_14_red_percent;
    __int16 field_16_green_percent;
    __int16 field_18_blue_percent;
    __int16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas, 0x1C);


class LaughingGas : public BaseGameObject
{
public:
    EXPORT LaughingGas* ctor_432400(Layer layer, int notUsed, Path_LaughingGas* pTlv, int tlvInfo);

    EXPORT void dtor_432B80();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT void vScreenChanged_432DE0();

    EXPORT void Init_432980();

    EXPORT LaughingGas* vdtor_432670(signed int flags);

    EXPORT void vRender_432D10(PrimHeader** ppOt);

    EXPORT void DoRender_432740();

    EXPORT __int16 CounterOver_432DA0();

    EXPORT void vUpdate_432C40();

    EXPORT float Calc_X_4326A0(float* a2, int xIndex);
    EXPORT float Calc_Y_4326F0(float* a2, int yIndex);

    EXPORT void sub_4328A0();
private:
    int field_20_padding;
    int field_24_tlvInfo;
    __int16 field_28_y;
    __int16 field_2A_x;
    __int16 field_2C_h;
    __int16 field_2E_w;
    int field_30_padding;
    __int16 field_34_bEnabled;
    Choice_short field_36_bLaughing_gas;
    int field_38_padding;
    int field_3C_padding;
    int field_40_padding;
    int field_44_padding;
    Path_LaughingGas_Data field_48_tlv_data;
    FP field_54_amount_on;
    Layer field_58_layer;
    __int16 field_5A_padding;

    Prim_GasEffect field_5C_prim;

    float field_7C_gas_y[6][6];
    float field_10C_gas_x[6][6];

    WORD* field_19C_pMem;

    struct Data
    {
        float array_4[7];
    };

    Data field_1A0_x_data[321]; // (640+2)/2
    Data field_24D0_y_data[121]; // (480+4)/4

    int field_31F8_w_count;
    int field_31FC_h_count;
};
ALIVE_ASSERT_SIZEOF(LaughingGas, 0x3200);

ALIVE_VAR_EXTERN(int, gGasInstanceCount_5BC214);
ALIVE_VAR_EXTERN(short, gLaughingGasOn_5C1BA4);
