#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"
#include "Layer.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_SlogHut final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_switch_id;
    s16 field_1C_Zzz_delay;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_SlogHut, 0x20);

class SlogHut final : public BaseGameObject
{
public:
    EXPORT SlogHut* ctor_472C80(Path_SlogHut* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_472CF0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT SlogHut* Vdtor_472E30(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_472E20();

    virtual void VUpdate() override;

    EXPORT void VUpdate_472D50();

    FP field_10_xpos;
    FP field_14_ypos;
    FP field_18_scale;
    s32 field_1C_tlvInfo;
    s16 field_20_switch_id;
    s16 field_22_padding;
    s32 field_24_timer;
    s16 field_28_Zzz_delay;
    s16 field_2A_padding;
};
ALIVE_ASSERT_SIZEOF(SlogHut, 0x2C);

} // namespace AO
