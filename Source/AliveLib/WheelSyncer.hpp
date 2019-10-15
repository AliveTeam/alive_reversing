#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"

struct Path_WheelSyncer : public Path_TLV
{
    __int16 field_10_id1;
    __int16 field_12_id2;
    __int16 field_14_trigger_id;
    __int16 field_16_action;
    __int16 field_18_id3;
    __int16 field_1A_id4;
    __int16 field_1C_id5;
    __int16 field_1E_id6;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WheelSyncer, 0x20);

class WheelSyncer : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT WheelSyncer* ctor_466090(Path_WheelSyncer* pTlv, DWORD tlvInfo);
private:
    EXPORT void vUpdate_4661D0();
    EXPORT WheelSyncer* vdtor_466120(signed int flags);
    EXPORT void dtor_466150();
    EXPORT void vScreenChanged_466310();
private:
    __int16 field_20_id1;
    __int16 field_22_id2;
    __int16 field_24_trigger_id;
    __int16 field_26_id3;
    __int16 field_28_id4;
    __int16 field_2A_id5;
    __int16 field_2C_id6;
    __int16 field_2E_action;
    DWORD field_30_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(WheelSyncer, 0x34);
