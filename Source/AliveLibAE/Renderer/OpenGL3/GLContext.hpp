#pragma once

#include <SDL.h>

#include "../AliveLibCommon/Sys_common.hpp"

class GLContext final
{
public:
    GLContext(TWindowHandleType window);
    ~GLContext();

private:
    SDL_GLContext mContext = nullptr;
};
