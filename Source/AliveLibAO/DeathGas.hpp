#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

class DeathGas final : public BaseGameObject
{
public:
    DeathGas(Layer layer, s32 amount);
    ~DeathGas();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    s16 field_10_total = 0;
    s16 field_12_unused = 0;
    s16 field_14_amount = 0;
    s16 field_16_flag = 0;
    Layer field_18_layer = Layer::eLayer_0;
    s16 field_1A = 0;
    s16 field_1C = 0;
    s16 field_1E = 0;
};

} // namespace AO
