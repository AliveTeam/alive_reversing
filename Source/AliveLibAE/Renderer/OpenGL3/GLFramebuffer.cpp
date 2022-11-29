#include <GL/glew.h>

#include "../AliveLibCommon/Types.hpp"
#include "GLDebug.hpp"
#include "GLFramebuffer.hpp"
#include "SDL.h"

GLFramebuffer::GLFramebuffer(u32 width, u32 height)
{
    static const GLenum fbTargets[1] = {GL_COLOR_ATTACHMENT0};

    mHeight = height;
    mWidth = width;

    // Create objects
    GL_VERIFY(glGenFramebuffers(1, &mFramebufferGLId));
    GL_VERIFY(glGenTextures(1, &mTextureGLId));

    // Init texture
    GL_VERIFY(glActiveTexture(GL_TEXTURE0));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mTextureGLId));

    GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0));

    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    // Init framebuffer
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferGLId));
    GL_VERIFY(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextureGLId, 0));

    GL_VERIFY(glDrawBuffers(1, fbTargets));
}

GLFramebuffer::~GLFramebuffer()
{
    if (mFramebufferGLId)
    {
        GL_VERIFY(glDeleteFramebuffers(1, &mFramebufferGLId));
        GL_VERIFY(glDeleteTextures(1, &mTextureGLId));
    }
}


void GLFramebuffer::BindAsSourceTextureTo(GLenum texUnit)
{
    GL_VERIFY(glActiveTexture(texUnit));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mTextureGLId));
}

void GLFramebuffer::BindAsTarget()
{
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferGLId));
    GL_VERIFY(glViewport(0, 0, mWidth, mHeight));
}

u32 GLFramebuffer::GetHeight()
{
    return mHeight;
}

u32 GLFramebuffer::GetWidth()
{
    return mWidth;
}


void GLFramebuffer::BindScreenAsTarget(SDL_Window* wnd, s32* outWidth, s32* outHeight)
{
    s32 viewportW, viewportH;

    SDL_GL_GetDrawableSize(wnd, &viewportW, &viewportH);

    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_VERIFY(glViewport(0, 0, viewportW, viewportH));

    if (outWidth)
    {
        *outWidth = viewportW;
    }

    if (outHeight)
    {
        *outHeight = viewportH;
    }
}
