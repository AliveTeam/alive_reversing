#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class BaseBomb final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BaseBomb* ctor_4173A0(FP xpos, FP ypos, s32 /*unused*/, FP scale);

    EXPORT void DealDamageRect_417A50(const PSX_RECT* pRect);

    virtual void VUpdate() override;

    EXPORT void VUpdate_417580();

    

    s32 field_D4_padding[4];
    FP field_E4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xE8);

} // namespace AO
