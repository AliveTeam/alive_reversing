#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"

namespace AO {

class BaseAliveGameObject;
class PalleteOverwriter;

class Electrocute : public BaseGameObject
{
public:
    EXPORT Electrocute* ctor_48D3A0(BaseAliveGameObject* pTargetObj, __int16 bExtraOverwriter);

    EXPORT BaseGameObject* dtor_48D480();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Electrocute* Vdtor_48D8E0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_48D8B0();

    EXPORT void Stop_48D510();

    virtual void VUpdate() override;

    EXPORT void VUpdate_48D5C0();

    BaseAliveGameObject* field_10_obj_target;
    __int16 field_14_overwriter_count;
    __int16 field_16_pad;
    PalleteOverwriter* field_18_pPalOverwriters[3];
    __int16 field_24_extraOverwriter;
    __int16 field_26_pad;
    WORD* field_28_pPalData;
    __int16 field_2C_r;
    __int16 field_2E_g;
    __int16 field_30_b;
    enum class States : __int16
    {
        eSetNewColour_0 = 0,
        eAlphaFadeout_1 = 1,
        eHandleDamage_2 = 2,
        eKillElectrocute_3 = 3
    };
    States field_32_state;
    int field_34_pad;
    PSX_RECT field_38_pal_rect;
};
ALIVE_ASSERT_SIZEOF(Electrocute, 0x40);

}

