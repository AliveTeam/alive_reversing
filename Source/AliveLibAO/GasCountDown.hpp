#pragma once

#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "FunctionFwd.hpp"
#include "Map.hpp"

namespace AO {

struct Path_GasCountDown final : public Path_TLV
{
    s16 field_18_start_id;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasCountDown, 0x1C);

class GasCountDown final : public BaseGameObject
{
public:
    EXPORT GasCountDown* ctor_40BF60(Path_GasCountDown* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_40C050();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_40C3F0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_40C3C0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40C0E0();

    void DealDamage();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_40C2F0(PrimHeader** ppOt);

    FontContext field_10_font_context;
    AliveFont field_20_font;
    s32 field_58_tlvInfo;
    s16 field_5C_xpos;
    s16 field_5E_ypos;
    u16 field_60_switch_id;
    s16 field_62_time_left;
};
ALIVE_ASSERT_SIZEOF(GasCountDown, 0x64);

ALIVE_VAR_EXTERN(s16, gGasOn_4FF888);
ALIVE_VAR_EXTERN(s32, sGasTimer_507700);

} // namespace AO
