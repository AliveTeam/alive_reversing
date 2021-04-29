#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

struct Path_Glukkon final : public Path_TLV
{
    s16 field_18_scale_percent;
    u16 field_1A_pal_id;
    s16 field_1C_target_id;
    s16 field_1E_voice_adjust;
};
ALIVE_ASSERT_SIZEOF(Path_Glukkon, 0x20);

class BackgroundGlukkon final : public BaseAliveGameObject
{
public:
    EXPORT BackgroundGlukkon* ctor_41DBD0(Path_Glukkon* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_41DCE0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BackgroundGlukkon* Vdtor_41E0F0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41E0E0();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_41DF80(BaseGameObject* pFrom);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41DD60();

    s32 field_10C_tlvInfo;
    s16 field_110_state;
    s16 field_112_pad;
    s32 field_114_timer2;
    s32 field_118_timer1;
    s32 field_11C_voice_adjust;
    s16 field_120_target_id;
    s16 field_122_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundGlukkon, 0x124);

} // namespace AO
