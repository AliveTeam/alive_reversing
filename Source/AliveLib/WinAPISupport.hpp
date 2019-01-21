#pragma once

#if !USE_SDL2
#include <timeapi.h>
#else
unsigned long timeGetTime();
#endif