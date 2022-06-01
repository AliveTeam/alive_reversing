#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Honey final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Honey(FP xpos, FP ypos);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 field_D4[4];
    s32 field_E4_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(Honey, 0xE8);

} // namespace AO
