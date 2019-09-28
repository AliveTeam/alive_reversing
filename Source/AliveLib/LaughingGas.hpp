#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "Primitives.hpp"

// TODO: These can be combined
struct Path_LaughingGas_Data
{
    __int16 field_0_is_laughin_gas;
    __int16 field_2_gas_id;
    __int16 field_4_red_percent;
    __int16 field_6_blue_percent;
    __int16 field_8_green_percent;
    __int16 field_A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas_Data, 0xC);

struct Path_LaughingGas : public Path_TLV
{
    __int16 field_10_is_laughing_gas;
    __int16 field_12_gas_id;
    __int16 field_14_red_percent;
    __int16 field_16_green_percent;
    __int16 field_18_blue_percent;
    __int16 field_1A;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas, 0x1C);


class LaughingGas : public BaseGameObject
{
public:
    EXPORT LaughingGas* ctor_432400(__int16 layer, int notUsed, Path_LaughingGas* pTlv, int tlvInfo);

    EXPORT void dtor_432B80();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(int** pOt) override;

private:
    EXPORT void vScreenChanged_432DE0();

    EXPORT void Init_432980();

    EXPORT LaughingGas* vdtor_432670(signed int flags);

    EXPORT void vRender_432D10(int** pOt);

    EXPORT void DoRender_432740();

    EXPORT __int16 CounterOver_432DA0();

    EXPORT void vUpdate_432C40();

    EXPORT float Calc_Y_4326A0(float* a2, int a3);
    EXPORT float Calc_X_4326F0(float* a2, int idx);

    EXPORT void sub_4328A0();
private:
    int field_20;
    int field_24_tlvInfo;
    PSX_RECT field_28_rect;
    int field_30;
    __int16 field_34_bEnabled;
    __int16 field_36_bGreen;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    Path_LaughingGas_Data field_48_tlv_data;
    FP field_54_amount_on;
    __int16 field_58_layer;
    __int16 field_5A;

    Prim_GasEffect field_5C_prim;

    float field_7C[6][6];
    float field_10C_ary6[6][6];

    short* field_19C_pMem;

    struct Data
    {
        float unknown;
        float array_4[4];
        float unknown2;
        float unknown3;
    };

    // TODO: Figure out why these seem to be indexed the wrong way around
    Data field_1A0_x_data[321]; // (640+2)/2
    Data field_24D0_y_data[121]; // (480+4)/4

    int field_31F8_h_count;
    int field_31FC_w_count;
};
ALIVE_ASSERT_SIZEOF(LaughingGas, 0x3200);

ALIVE_VAR_EXTERN(int, gGasInstanceCount_5BC214);
ALIVE_VAR_EXTERN(short, gLaughingGasOn_5C1BA4);
