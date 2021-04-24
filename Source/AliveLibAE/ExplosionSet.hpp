#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

struct Path_ExplosionSet : public Path_TLV
{
    Choice_short field_10_bStart_enabled;
    s16 field_12_switch_id;
    Choice_short field_14_big_rocks;
    s16 field_16_start_delay;
    XDirection_short field_18_direction;
    s16 field_1A_start_delay;
    s16 field_1C_grid_spacing;
    s16 field_1E_increasing_grid_spacing;
    Scale_short field_20_scale;
    s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ExplosionSet, 0x24);

class ExplosionSet : public BaseGameObject
{
public:
    EXPORT ExplosionSet* ctor_414CA0();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    EXPORT void Start_415300();

    EXPORT void Init_4151D0(Path_ExplosionSet* pTlv);



private:
    EXPORT ExplosionSet* vdtor_414D80(s32 flags);

    EXPORT void dtor_414DB0();

    EXPORT void vScreenChanged_415190();

    EXPORT void vRender_4150D0(PrimHeader** ppOt);

    EXPORT void vUpdate_414E30();

private:
    Prim_ScreenOffset field_20[2];
    s16 field_40;
    s16 field_42;
    s16 field_44_start_delay_counter;
    s16 field_46_spacing_multiplicator;
    PSX_RECT field_48_tlv_rect;
    FP field_50_scale;
    s16 field_54_switch_id;
    s16 field_56_delay;
    s16 field_58_grid_spacing;
    s16 field_5A_increasing_grid_spacing;

    enum Flags_5C
    {
        eBit1_big_rocks = 0x1,
        eBit2_flipX = 0x2,
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
    s16 field_5E;
};
ALIVE_ASSERT_SIZEOF(ExplosionSet, 0x60);

ALIVE_VAR_EXTERN(ExplosionSet*, pExplosionSet_5BBF68);
ALIVE_VAR_EXTERN(short, bEnabled_5C1BB6);
