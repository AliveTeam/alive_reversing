#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseGameObject.hpp"

class ScreenShake : public BaseGameObject
{
public:
    EXPORT ScreenShake* ctor_4ACF70(__int16 enableShakeEvent, __int16 softerShakes);
   
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOt) override;

private:
    EXPORT void dtor_4AD060();
    EXPORT void vUpdate_4AD0E0();
    EXPORT BaseGameObject* vdtor_4AD030(signed int flags);
    EXPORT void vRender_4AD120(int** /*pOt*/);

private:
    Prim_ScreenOffset field_20_screenOffset[2];
    __int16 field_40_shakeNumber;
    __int16 field_42_enableShakeEvent;
    __int16 field_44_softerShakes;

    __int16 field_46; // Apparently not used
    int field_48;     // Apparently not used
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x4C);
