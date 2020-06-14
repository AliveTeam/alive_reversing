#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class GameSpeak : public BaseGameObject
{
public:
    EXPORT GameSpeak* ctor_40F990();

    EXPORT GameSpeak* Vdtor_40FB50(signed int /*flags*/);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    __int16 field_10;
    __int16 field_12;
    int field_14;
    int field_18_idx;
    char field_1C[32];
};
ALIVE_ASSERT_SIZEOF(GameSpeak, 0x3C);

END_NS_AO
