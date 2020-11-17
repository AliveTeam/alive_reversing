#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Honey : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Honey* ctor_431E30(FP xpos, FP ypos);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_431EF0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    int field_E4_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(Honey, 0xE8);

}

