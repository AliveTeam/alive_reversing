#pragma once

#include "../../../relive_lib/Sys_common.hpp"

typedef void* SDL_GLContext;

class GLContext final
{
public:
    GLContext(TWindowHandleType window);
    ~GLContext();

private:
    SDL_GLContext mContext = nullptr;
};
