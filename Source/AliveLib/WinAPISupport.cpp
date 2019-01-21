#include "stdafx.h"
#include "WinAPISupport.hpp"
#include <SDL.h>

// win32 api wrappers

#if USE_SDL2

unsigned long timeGetTime()
{
	return SDL_GetTicks();
}

#endif