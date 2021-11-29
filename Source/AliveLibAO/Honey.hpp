#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Honey final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Honey* ctor_431E30(FP xpos, FP ypos);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_431EF0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    s32 field_D4[4];
    s32 field_E4_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(Honey, 0xE8);

} // namespace AO
