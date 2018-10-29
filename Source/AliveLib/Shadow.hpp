#pragma once

#include <FunctionFwd.hpp>
#include "Animation.hpp"

class Shadow
{
public:
    EXPORT Shadow* ctor_4AC990();

    EXPORT void dtor_4ACA30();
private:
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14_flags;
    AnimationEx field_18_animation;
};
ALIVE_ASSERT_SIZEOF(Shadow, 0xB0);
