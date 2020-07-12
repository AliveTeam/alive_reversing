#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

struct Path_GasEmitter : public Path_TLV
{

};

class GasEmitter : public BaseGameObject
{
public:
    EXPORT GasEmitter* ctor_41D760(Path_GasEmitter* pTlv, int tlvInfo);

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_41D8D0();
    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT GasEmitter* Vdtor_41D910(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41D900();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41D7D0();

    EXPORT BaseGameObject* dtor_41D840();

    int field_10_tlvInfo;
    int field_14_emit_power;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_fp_not_used;
};
ALIVE_ASSERT_SIZEOF(GasEmitter, 0x24);

END_NS_AO

