#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

ALIVE_VAR_EXTERN(int, gDeathGasCount_5BD24C);

class DeathGas : public BaseGameObject
{
public:
    EXPORT DeathGas* ctor_43C030(__int16 layer, __int16 amount);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(int** pOrderingTable) override;

private:
    EXPORT DeathGas* vdtor_43C240(signed int flags);

    EXPORT void dtor_43C270();

    EXPORT void vScreenChanged_43CA50();

    EXPORT void vUpdate_43C300();

    EXPORT void vRender_43C350(int** pOt);
private:
    __int16 field_20_total;
    __int16 field_22_unused; // Gets increased by two every vUpdate_43C300. Isn't used anywhere else -- Nemin (6/7/2020)
    __int16 field_24_amount;
    __int16 field_26_flag;
    __int16 field_28_layer;
    __int16 field_2A_padding;
    __int16 field_2C_padding;
    __int16 field_2E_padding;
};
ALIVE_ASSERT_SIZEOF(DeathGas, 0x30);
