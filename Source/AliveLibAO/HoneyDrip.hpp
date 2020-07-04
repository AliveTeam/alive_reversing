#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

class HoneyDrip : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT HoneyDrip* ctor_431D10(FP xpos, FP ypos);

    virtual void VUpdate() override;

    EXPORT void VUpdate_431DC0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_D4[4];
    __int16 field_E4_bSoundPlaying;
    __int16 field_E6;
};
ALIVE_ASSERT_SIZEOF(HoneyDrip, 0xE8);

END_NS_AO
