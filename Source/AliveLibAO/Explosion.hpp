#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class Explosion final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Explosion* ctor_458B80(FP xpos, FP ypos, FP scale);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_459470();

    EXPORT void VUpdate_458D00();

    EXPORT void DealBlastDamage_459160(PSX_RECT* pRect);

    

    s32 field_D4_padding[4];
    FP field_E4_explosion_size;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0xE8);

} // namespace AO
