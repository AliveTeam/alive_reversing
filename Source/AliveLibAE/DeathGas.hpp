#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Layer.hpp"

extern s32 gDeathGasCount;

class DeathGas final : public BaseGameObject
{
public:
    DeathGas(Layer layer, s32 amount);
    ~DeathGas();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    s16 mTotal = 0;
    s16 mAmount = 0;
    bool mDone = false;
    Layer mLayer = Layer::eLayer_0;
};
