#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Layer.hpp"

namespace AO {

class DeathGas final : public BaseGameObject
{
public:
    DeathGas(Layer layer, s32 amount);
    ~DeathGas();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    s16 field_10_total;
    s16 field_12_unused;
    s16 field_14_amount;
    s16 field_16_flag;
    Layer field_18_layer;
    s16 field_1A;
    s16 field_1C;
    s16 field_1E;
};
ALIVE_ASSERT_SIZEOF(DeathGas, 0x20);

} // namespace AO
