#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

class ScreenShake : public BaseGameObject
{
public:
    EXPORT ScreenShake* ctor_4624D0(__int16 enableShakeEvent);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4625B0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ScreenShake* Vdtor_462770(signed int flags);

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_4625E0(int** ppOt);

    EXPORT BaseGameObject* dtor_462550();

    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    __int16 field_30_shakeNumber;
    __int16 field_32_enableShakeEvent;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x34);

END_NS_AO

