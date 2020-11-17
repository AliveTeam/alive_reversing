#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

namespace AO {

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

    Prim_ScreenOffset field_10_screenOffset[2];
    __int16 field_30_shakeNumber;
    __int16 field_32_enableShakeEvent;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x34);

}

