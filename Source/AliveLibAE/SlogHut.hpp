#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

struct Path_SlogHut final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_switch_id;
    s16 field_14_Zzz_interval;
    s16 field_16_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogHut, 0x18);

class SlogHut final : public BaseGameObject
{
public:
    EXPORT SlogHut* ctor_4C4070(Path_SlogHut* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void dtor_4C4130();
    EXPORT SlogHut* vdtor_4C4100(s32 flags);
    EXPORT void vScreenChanged_4C4280();
    EXPORT void vUpdate_4C41B0();

private:
    FP field_20_xpos;
    FP field_24_ypos;
    FP field_28_scale;
    s32 field_2C_tlvInfo;
    s16 field_30_switch_id;
    s16 field_32_padding;
    s32 field_34_Zzz_timer;
    s16 field_38_Zzz_interval;
    s16 field_3A_padding;
};
ALIVE_ASSERT_SIZEOF(SlogHut, 0x3C);
