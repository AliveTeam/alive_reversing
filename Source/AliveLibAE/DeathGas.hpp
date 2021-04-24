#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Layer.hpp"

ALIVE_VAR_EXTERN(int, gDeathGasCount_5BD24C);

class DeathGas : public BaseGameObject
{
public:
    EXPORT DeathGas* ctor_43C030(Layer layer, s16 amount);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT DeathGas* vdtor_43C240(s32 flags);

    EXPORT void dtor_43C270();

    EXPORT void vScreenChanged_43CA50();

    EXPORT void vUpdate_43C300();

    EXPORT void vRender_43C350(PrimHeader** ppOt);
private:
    s16 field_20_total;
    s16 field_22_unused; // Gets increased by two every vUpdate_43C300. Isn't used anywhere else -- Nemin (6/7/2020)
    s16 field_24_amount;
    s16 field_26_flag;
    Layer field_28_layer;
    s16 field_2A_padding;
    s16 field_2C_padding;
    s16 field_2E_padding;
};
ALIVE_ASSERT_SIZEOF(DeathGas, 0x30);
