#include "stdafx.h"
#include "Quicksave.hpp"
#include "Function.hpp"

#define Quicksave_IMPL true

void Quicksave_ForceLink() { }

ALIVE_VAR(1, 0xBAF7F8, Quicksave, sActiveQuicksaveData_BAF7F8, {});

EXPORT void CC Quicksave_LoadActive_4C9170()
{
    NOT_IMPLEMENTED(0x4C9170);
}
