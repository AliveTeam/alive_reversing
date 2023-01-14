#pragma once

#include <GL/glew.h>

#include "../../../relive_lib/Types.hpp"

struct SDL_Window;

class GLFramebuffer final
{
public:
    GLFramebuffer(u32 width, u32 height);
    ~GLFramebuffer();

    void BindAsSourceTextureTo(GLenum texUnit);
    void BindAsTarget();
    u32 GetHeight();
    u32 GetWidth();

    static void BindScreenAsTarget(SDL_Window* wnd, s32* outWidth = nullptr, s32* outHeight = nullptr);

private:
    GLuint mFramebufferGLId = 0;
    GLuint mTextureGLId = 0;
    u32 mHeight = 0;
    u32 mWidth = 0;
};
