#pragma once

#include <GL/glew.h>
#include <SDL.h>

#include "../AliveLibCommon/Types.hpp"

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
    GLuint mFramebufferGLId;
    GLuint mTextureGLId;
    u32 mHeight;
    u32 mWidth;
};
