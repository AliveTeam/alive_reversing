#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Factory.hpp"
#include "Path.hpp"

void SlamDoor_ForceLink();

enum SlamDoor_Flags_118
{
    e118_Bit1 = 0x1,
    e118_Bit2_Open = 0x2,
    e118_Bit3 = 0x4,
    e118_Bit4_Inverted = 0x8,
    e118_Bit5_Delete = 0x10,
};

struct Path_SlamDoor : public Path_TLV
{
    __int16 field_10_starts_shut;
    __int16 field_12_half_scale;
    __int16 field_14_id;
    __int16 field_16_inverted;
    __int16 field_18_delete;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlamDoor, 0x1C);

class SlamDoor : public BaseAliveGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;

    EXPORT SlamDoor* ctor_4AF700(Path_SlamDoor * tlv_params, TlvItemInfoUnion tlvInfo);
    EXPORT void dtor_4B0620();
    EXPORT SlamDoor * vdtor_4AFD20(signed int flags);
    EXPORT void vUpdate_4AFD50();

    EXPORT void ClearInsideSlamDoor_4B0530(BaseAliveGameObject *pObj, __int16 a3, __int16 a4);

public:
    __int16 field_116;
    BitField16<SlamDoor_Flags_118> field_118_flags;
    __int16 field_11A;
    PathLine *field_11C_pCollisionLine_6_2;
    PathLine *field_120_pCollisionLine_5_1;
    __int16 field_124_x1;
    __int16 field_126_y1;
    __int16 field_128_switch_id;
    __int16 field_12A;
    TlvItemInfoUnion field_12C_tlvInfo;
    int field_130;
};
ALIVE_ASSERT_SIZEOF(SlamDoor, 0x134);
