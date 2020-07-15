#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

extern const Types word_4CF158[];

class BaseThrowable : public BaseAliveGameObject
{
public:
    __int16 field_10C_count;
    __int16 field_10E_bDead;
};
ALIVE_ASSERT_SIZEOF(BaseThrowable, 0x110);

END_NS_AO

