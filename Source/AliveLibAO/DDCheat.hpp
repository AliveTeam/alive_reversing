#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class DDCheat : public BaseGameObject
{
public:
    static EXPORT void ClearProperties_4095B0();

    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
};
ALIVE_ASSERT_SIZEOF(DDCheat, 0x28);

END_NS_AO
