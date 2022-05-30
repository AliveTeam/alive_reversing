#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class BaseBomb final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BaseBomb(FP xpos, FP ypos, s32 /*unused*/, FP scale);

    void DealDamageRect(const PSX_RECT* pRect);

    virtual void VUpdate() override;

    s32 field_D4_padding[4];
    FP field_E4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xE8);

} // namespace AO
