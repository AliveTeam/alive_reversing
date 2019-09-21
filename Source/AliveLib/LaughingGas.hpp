#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"

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

private:
    int field_20;
    int field_24_tlvInfo;
    __int16 field_28;
    __int16 field_2A;
    __int16 field_2C;
    __int16 field_2E;
    int field_30;
    __int16 field_34;
    __int16 field_36;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    Path_LaughingGas_Data field_48_tlv_data;
    FP field_54;
    __int16 field_58_layer;
    __int16 field_5A;
    int field_5C_prims;
    int field_60;
    char field_64;
    char field_65;
    char field_66;
    char field_67;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    float field_10C_ary6[6][6];
    void* field_19C_pMem;
    int field_1A0[2252];
    int field_24D0_p28Stru[842];
    int field_31F8_count1;
    int field_31FC_count2;
};
ALIVE_ASSERT_SIZEOF(LaughingGas, 0x3200);

ALIVE_VAR_EXTERN(int, gGasInstanceCount_5BC214);
ALIVE_VAR_EXTERN(short, gLaughingGasOn_5C1BA4);
