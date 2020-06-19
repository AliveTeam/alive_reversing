#pragma once
#include "FunctionFwd.hpp"

START_NS_AO;

class PauseMenu
{
public:
    int placeholder[154];

    EXPORT PauseMenu* ctor_48FB80();
};
ALIVE_ASSERT_SIZEOF(PauseMenu, 0x268);

ALIVE_VAR_EXTERN(PauseMenu*, pPauseMenu_5080E0);

END_NS_AO;
