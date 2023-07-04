#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "Path.hpp" // only for Choice_short

// TODO: These can be combined
struct Path_LaughingGas_Data final
{
    Choice_short field_0_bLaughing_gas;
    s16 field_2_laughing_gas_switch_id;
    s16 field_4_red_percent;
    s16 field_6_blue_percent;
    s16 field_8_green_percent;
    s16 field_A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LaughingGas_Data, 0xC);

class LaughingGas final : public BaseGameObject
{
public:
    LaughingGas(Layer layer, s32 notUsed, relive::Path_LaughingGas* pTlv, const Guid& tlvId);
    ~LaughingGas();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(BasePrimitive** ppOt) override;

private:
    void Init();
    void DoRender();
    s16 CounterOver();
    f32 Calc_X(f32* a2, s32 xIndex);
    f32 Calc_Y(f32* a2, s32 yIndex);
    void sub_4328A0();

private:
    Guid field_24_tlvInfo;
    s16 field_28_y = 0;
    s16 field_2A_x = 0;
    s16 field_2C_h = 0;
    s16 field_2E_w = 0;
    s16 field_34_bEnabled = 0;
    Choice_short field_36_bLaughing_gas = Choice_short::eNo_0;
    Path_LaughingGas_Data field_48_tlv_data = {};
    FP field_54_amount_on = {};
    Layer field_58_layer = Layer::eLayer_0;

    Prim_GasEffect field_5C_prim = {};

    f32 field_7C_gas_y[6][6] = {};
    f32 field_10C_gas_x[6][6] = {};

    u16* field_19C_pMem = nullptr;

    struct Data final
    {
        f32 array_4[7];
    };

    Data field_1A0_x_data[321] = {};  // (640+2)/2
    Data field_24D0_y_data[121] = {}; // (480+4)/4

    s32 field_31F8_w_count = 0;
    s32 field_31FC_h_count = 0;
};

extern s32 gGasInstanceCount_5BC214;
extern s16 gLaughingGasOn_5C1BA4;
