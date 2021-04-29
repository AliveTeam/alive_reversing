#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

namespace AO {

struct Path_IdSplitter final : public Path_TLV
{
    s16 field_18_source_id;
    u16 field_1A_delay;
    s16 field_1C_id1;
    s16 field_1C_id2;
    s16 field_1C_id3;
    s16 field_1C_id4;
};
ALIVE_ASSERT_SIZEOF(Path_IdSplitter, 0x24);

class IdSplitter final : public BaseGameObject
{
public:
    EXPORT IdSplitter* ctor_479B40(Path_IdSplitter* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_479BE0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT IdSplitter* Vdtor_479E00(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_479DB0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_479C40();

    u16 field_10_source_id;

    enum class State : s16
    {
        eState_0 = 0,
        eState_1 = 1,
        eState_2 = 2,
        eState_3 = 3,
    };
    State field_12_state;
    s16 field_14_ids[4];
    s32 field_1C_tlvInfo;
    s32 field_20_delay_timer;
    s32 field_24_delay;
    s16 field_28_source_switch_value;
    s16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(IdSplitter, 0x2C);


} // namespace AO
