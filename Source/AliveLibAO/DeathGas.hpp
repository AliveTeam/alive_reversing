#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Layer.hpp"

namespace AO {

class DeathGas final : public BaseGameObject
{
public:
    EXPORT DeathGas* ctor_41CF40(Layer layer, s16 amount);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41D150();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41D700();

    EXPORT BaseGameObject* dtor_41D0E0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT DeathGas* Vdtor_41D740(s32 flags);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_41D190(PrimHeader** ppOt);

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
