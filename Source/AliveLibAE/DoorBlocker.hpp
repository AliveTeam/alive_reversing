#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_DoorBlocker : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorBlocker, 0x14);

class DoorBlocker : public BaseAliveGameObject
{
public:
    EXPORT DoorBlocker* ctor_41F0A0(Path_DoorBlocker* pTlv, int tlvInfo);
    EXPORT void dtor_41F310();

    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT void vUpdate_41F250();
    EXPORT DoorBlocker* vdtor_41F220(signed int flags);

private:
    s16 field_118_bDone;
    s16 field_11A_switch_id;
    int field_11C_tlvInfo;
    s16 field_120;
    s16 field_122;
};
ALIVE_ASSERT_SIZEOF(DoorBlocker, 0x124);
