#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

class BaseBomb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseBomb* ctor_423E70(FP x, FP y, s32 unused, FP scale);

private:
    EXPORT void vUpdate_424180();
    EXPORT BaseBomb* vdtor_424130(s32 flags);
    EXPORT void dtor_424160();

private:
    FP field_f4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xF8);
