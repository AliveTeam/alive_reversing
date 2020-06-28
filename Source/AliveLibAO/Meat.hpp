#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

struct Path_MeatSack : public Path_TLV
{
    __int16 field_18_side;
    unsigned __int16 field_1A_x_vel;
    __int16 field_1C_y_vel;
    unsigned __int16 field_1E_scale;
    __int16 field_20_amount_of_meat;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_MeatSack, 0x24);


class MeatSack : public BaseAliveGameObject
{
public:
    EXPORT MeatSack* ctor_4390F0(Path_MeatSack* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MeatSack* Vdtor_439550(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_439540();

    EXPORT BaseGameObject* dtor_439250();

    int field_10C_tlvInfo;
    __int16 field_110;
    __int16 field_112_num_items;
    __int16 field_114;
    __int16 field_116;
    FP field_118_velX;
    FP field_11C_velY;
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x120);

END_NS_AO

