#include "stdafx.h"
#include "Quicksave.hpp"

#define Quicksave_IMPL true

void Quicksave_ForceLink() { }

ALIVE_FUNC_NOT_IMPL(0x4C9170, void CC(), Quicksave_LoadActive_4C9170);
ALIVE_VAR(1, 0xBAF7F8, Quicksave, sActiveQuicksaveData_BAF7F8, {});