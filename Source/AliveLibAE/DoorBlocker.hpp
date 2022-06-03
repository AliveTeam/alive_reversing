#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_DoorBlocker final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_switch_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorBlocker, 0x14);

class DoorBlocker final : public BaseAliveGameObject
{
public:
    DoorBlocker(Path_DoorBlocker* pTlv, s32 tlvInfo);
    ~DoorBlocker();

    virtual void VUpdate() override;

private:
    s16 field_118_bDone;
    s16 field_11A_switch_id;
    s32 field_11C_tlvInfo;
    s16 field_120;
    s16 field_122;
};
ALIVE_ASSERT_SIZEOF(DoorBlocker, 0x124);
