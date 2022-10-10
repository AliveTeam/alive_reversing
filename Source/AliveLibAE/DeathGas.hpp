#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Layer.hpp"

extern s32 gDeathGasCount_5BD24C;

class DeathGas final : public BaseGameObject
{
public:
    DeathGas(Layer layer, s32 amount);
    ~DeathGas();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    s16 field_20_total = 0;
    s16 field_22_unused = 0; // Gets increased by two every vUpdate_43C300. Isn't used anywhere else -- Nemin (6/7/2020)
    s16 field_24_amount = 0;
    s16 field_26_flag = 0;
    Layer field_28_layer = Layer::eLayer_0;
};
