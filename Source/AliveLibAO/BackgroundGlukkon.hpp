#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

struct Path_Glukkon : public Path_TLV
{
    __int16 field_18_scale_percent;
    unsigned __int16 field_1A_pal_id;
    __int16 field_1C_target_id;
    __int16 field_1E_voice_adjust;
};
ALIVE_ASSERT_SIZEOF(Path_Glukkon, 0x20);

class BackgroundGlukkon : public BaseAliveGameObject
{
public:
    EXPORT BackgroundGlukkon* ctor_41DBD0(Path_Glukkon* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_41DCE0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BackgroundGlukkon* Vdtor_41E0F0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41E0E0();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_41DF80(BaseGameObject* pFrom);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41DD60();

    int field_10C_tlvInfo;
    __int16 field_110_state;
    __int16 field_112_pad;
    int field_114_timer2;
    int field_118_timer1;
    int field_11C_voice_adjust;
    __int16 field_120_target_id;
    __int16 field_122;
};
ALIVE_ASSERT_SIZEOF(BackgroundGlukkon, 0x124);

}
