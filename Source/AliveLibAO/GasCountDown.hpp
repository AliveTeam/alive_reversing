#pragma once

#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "FunctionFwd.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_GasCountDown : public Path_TLV
{
    __int16 field_18_start_id;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GasCountDown, 0x1C);

class GasCountDown : public BaseGameObject
{
public:
    EXPORT GasCountDown* ctor_40BF60(Path_GasCountDown* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_40C050();

    virtual BaseGameObject* VDestructor(int flags) override;

    EXPORT BaseGameObject* Vdtor_40C3F0(int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_40C3C0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40C0E0();

    void DealDamage();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_40C2F0(int** ppOt);

    FontContext field_10_font_context;
    AliveFont field_20_font;
    int field_58_tlvInfo;
    __int16 field_5C_xpos;
    __int16 field_5E_ypos;
    unsigned __int16 field_60_switch_id;
    __int16 field_62_time_left;
};
ALIVE_ASSERT_SIZEOF(GasCountDown, 0x64);

ALIVE_VAR_EXTERN(short, gGasOn_4FF888);

END_NS_AO
