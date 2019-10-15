#pragma once

#include "FunctionFwd.hpp"
#include "Primitives.hpp"
#include "BaseGameObject.hpp"

class ScreenShake : public BaseGameObject
{
public:
    EXPORT ScreenShake* ctor_4ACF70(__int16 a2, __int16 a3);
   
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOt) override;

private:
    EXPORT void dtor_4AD060();
    EXPORT void vUpdate_4AD0E0();
    EXPORT BaseGameObject* vdtor_4AD030(signed int flags);
    EXPORT void vRender_4AD120(int** /*pOt*/);


private:
    Prim_ScreenOffset field_20[2];
    __int16 field_40;
    __int16 field_42;
    __int16 field_44;
    __int16 field_46;
    int field_48;
};
ALIVE_ASSERT_SIZEOF(ScreenShake, 0x4C);
