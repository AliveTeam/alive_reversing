#pragma once

#include "FunctionFwd.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

class BaseBomb : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseBomb * ctor_423E70(FP x, FP y, int unused, FP scale);
    
private:
    EXPORT void vUpdate_424180();
    EXPORT BaseBomb* vdtor_424130(signed int flags);
    EXPORT void dtor_424160();

private:
    FP field_f4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xF8);
