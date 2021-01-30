#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

class DeathGas : public BaseGameObject
{
public:
    EXPORT DeathGas* ctor_41CF40(__int16 layer, __int16 amount);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41D150();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41D700();

    EXPORT BaseGameObject* dtor_41D0E0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT DeathGas* Vdtor_41D740(signed int flags);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_41D190(PrimHeader** ppOt);

    __int16 field_10_total;
    __int16 field_12_unused;
    __int16 field_14_amount;
    __int16 field_16_flag;
    __int16 field_18_layer;
    __int16 field_1A;
    __int16 field_1C;
    __int16 field_1E;
};
ALIVE_ASSERT_SIZEOF(DeathGas, 0x20);

}
