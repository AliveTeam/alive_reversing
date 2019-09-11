#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

struct Path_ExplosionSet : public Path_TLV
{
    __int16 field_10_start_instantly;
    __int16 field_12_id;
    __int16 field_14_big_rocks;
    __int16 field_16_start_delay;
    __int16 field_18_direction;
    __int16 field_1A_delay;
    __int16 field_1C_grid_spacing;
    __int16 field_1E_scale;
    __int16 field_20_scale;
    __int16 field_22;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ExplosionSet, 0x24);

class ExplosionSet : public BaseGameObject
{
public:
    EXPORT ExplosionSet* ctor_414CA0();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VUpdate() override;
    EXPORT void Start_415300();

    EXPORT void Init_4151D0(Path_ExplosionSet* pTlv);



private:
    EXPORT ExplosionSet* vdtor_414D80(signed int flags);

    EXPORT void dtor_414DB0();

    EXPORT void vScreenChanged_415190();

    EXPORT void vRender_4150D0(int** pOt);

    EXPORT void vUpdate_414E30();

private:
    Prim_ScreenOffset field_20[2];
    __int16 field_40;
    __int16 field_42;
    __int16 field_44_start_delay;
    __int16 field_46;
    PSX_RECT field_48_tlv_rect;
    FP field_50_scale;
    __int16 field_54_switch_id;
    __int16 field_56_delay;
    __int16 field_58_grid_spacing;
    __int16 field_5A_scale;

    enum Flags_5C
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15 = 0x4000,
        eBit16 = 0x8000,
    };
    BitField16<Flags_5C> field_5C_flags;
    __int16 field_5E;
};
ALIVE_ASSERT_SIZEOF(ExplosionSet, 0x60);

ALIVE_VAR_EXTERN(ExplosionSet*, pExplosionSet_5BBF68);
