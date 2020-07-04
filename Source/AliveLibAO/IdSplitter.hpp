#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

struct Path_IdSplitter : public Path_TLV
{
    __int16 field_18_source_id;
    unsigned __int16 field_1A_delay;
    __int16 field_1C_ids[4];
};
ALIVE_ASSERT_SIZEOF(Path_IdSplitter, 0x24);

class IdSplitter : public BaseGameObject
{
public:
    EXPORT IdSplitter* ctor_479B40(Path_IdSplitter* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_479BE0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT IdSplitter* Vdtor_479E00(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_479DB0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_479C40();

    unsigned __int16 field_10_source_id;

    enum class State : __int16
    {
        eState_0 = 0,
        eState_1 = 1,
        eState_2 = 2,
        eState_3 = 3,
    };
    State field_12_state;
    __int16 field_14_ids[4];
    int field_1C_tlvInfo;
    int field_20_delay_timer;
    int field_24_delay;
    __int16 field_28_source_switch_value;
    __int16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(IdSplitter, 0x2C);


END_NS_AO

