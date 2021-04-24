#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class HoneyDrip : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT HoneyDrip* ctor_431D10(FP xpos, FP ypos);

    virtual void VUpdate() override;

    EXPORT void VUpdate_431DC0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    s16 field_E4_bSoundPlaying;
    s16 field_E6;
};
ALIVE_ASSERT_SIZEOF(HoneyDrip, 0xE8);

}
