#include "stdafx.h"
#include "VGA.hpp"
#include "Function.hpp"

#define VGA_IMPL true

void VGA_ForceLink() {}

#ifdef BEHAVIOUR_CHANGE_FORCE_WINDOW_MODE
signed int CC VGA_FullScreenSet_4F31F0(char bFullScreen)
{
    return 0;
}
ALIVE_FUNC_IMPLEX(0x4F31F0, VGA_FullScreenSet_4F31F0, VGA_IMPL);
#endif
