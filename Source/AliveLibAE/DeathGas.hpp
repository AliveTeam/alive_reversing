#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
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
    Layer mLayer = Layer::eLayer_0;
    s32 mAmount = 0;
    s32 mTotal = 0;
    bool mDone = false;
};
