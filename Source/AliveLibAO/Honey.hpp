#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

namespace AO {

class Honey final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Honey(FP xpos, FP ypos);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    Guid mTlvInfo;
};
ALIVE_ASSERT_SIZEOF(Honey, 0xE8);

} // namespace AO
