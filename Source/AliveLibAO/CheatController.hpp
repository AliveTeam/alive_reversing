#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class CheatController : public BaseGameObject
{
public:
    EXPORT CheatController* ctor_40FBF0();

    EXPORT CheatController* Vdtor_40FCD0(signed int flags);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    __int16 field_10;
    __int16 field_12;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x14);

END_NS_AO
