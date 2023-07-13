#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

class DeathGas final : public BaseGameObject
{
public:
    DeathGas(Layer layer, s32 amount);
    ~DeathGas();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

private:
    s16 mTotal = 0;
    s16 mAmount = 0;
    bool mDone = false;
    Layer mLayer = Layer::eLayer_0;
};

} // namespace AO
