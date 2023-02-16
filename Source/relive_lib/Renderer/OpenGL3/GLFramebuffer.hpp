#pragma once

#include <GL/glew.h>

#include "../../../relive_lib/Types.hpp"

struct SDL_Window;

class GLFramebuffer final
{
public:
    GLFramebuffer(s32 width, s32 height);
    ~GLFramebuffer();

    void BindAsSourceTextureTo(GLenum texUnit);
    void BindAsTarget();
    s32 GetHeight();
    s32 GetWidth();
    void Resize(s32 newWidth, s32 newHeight);

    static void BindScreenAsTarget(SDL_Window* wnd, s32* outWidth = nullptr, s32* outHeight = nullptr);

private:
    void CreateGLObjects();
    void DestroyGLObjects();

private:
    GLuint mFramebufferGLId = 0;
    GLuint mTextureGLId = 0;
    s32 mHeight = 0;
    s32 mWidth = 0;
};
