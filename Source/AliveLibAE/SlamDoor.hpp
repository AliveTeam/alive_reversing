#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Factory.hpp"
#include "Path.hpp"

void SlamDoor_ForceLink();

enum SlamDoor_Flags_118
{
    e118_Bit1_bClosed = 0x1,
    e118_Bit2_Open = 0x2,
    e118_Bit3_bLastFrame = 0x4,
    e118_Bit4_Inverted = 0x8,
    e118_Bit5_Delete = 0x10,
};

struct Path_SlamDoor final : public Path_TLV
{
    Choice_short field_10_bStart_closed;
    Scale_short field_12_scale;
    s16 field_14_switch_id;
    Choice_short field_16_bStart_inverted;
    Choice_short field_18_bDelete;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlamDoor, 0x1C);

class SlamDoor final : public BaseAliveGameObject
{
public:
    
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VUpdate() override;

    virtual void VOnThrowableHit(BaseGameObject*) override
    { }

    EXPORT SlamDoor* ctor_4AF700(Path_SlamDoor* tlv_params, TlvItemInfoUnion tlvInfo);
    EXPORT static s32 CC CreateFromSaveState_4C08B0(const u8*);

private:
    EXPORT void dtor_4B0620();
    EXPORT SlamDoor* vdtor_4AFD20(s32 flags);
    EXPORT void vUpdate_4AFD50();
    EXPORT s32 vGetSaveState_4C09D0(u8* pSaveBuffer);
    EXPORT void ClearInsideSlamDoor_4B0530(BaseAliveGameObject* pObj, s16 xPosition, s16 width);

private:
    BitField16<SlamDoor_Flags_118> field_118_flags;
    s16 field_11A_padding;
    PathLine* field_11C_pCollisionLine_6_2;
    PathLine* field_120_pCollisionLine_5_1;
    s16 field_124_x1;
    s16 field_126_y1;
    s16 field_128_switch_id;
    s16 field_12A_padding;
    TlvItemInfoUnion field_12C_tlvInfo;
    s32 field_130_padding;
};
ALIVE_ASSERT_SIZEOF(SlamDoor, 0x134);
