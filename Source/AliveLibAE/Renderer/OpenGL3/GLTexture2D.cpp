#include <GL/glew.h>

#include "../AliveLibCommon/FatalError.hpp"
#include "../AliveLibCommon/Types.hpp"
#include "../relive_lib/data_conversion/rgb_conversion.hpp"
#include "GLDebug.hpp"
#include "GLTexture2D.hpp"

GLTexture2D::GLTexture2D(u32 width, u32 height, GLenum format, bool initNow)
{
    GL_VERIFY(glGenTextures(1, &mGLId));

    mFormat = format;
    mHeight = height;
    mWidth = width;

    if (initNow)
    {
        BindTo(GL_TEXTURE0);
        Initialize();

        GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, 0));
    }
}

GLTexture2D::~GLTexture2D()
{
    if (mGLId)
    {
        GL_VERIFY(glDeleteTextures(1, &mGLId));
        mGLId = 0;
    }
}


void GLTexture2D::BindTo(GLenum texUnit)
{
    GL_VERIFY(glActiveTexture(texUnit));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mGLId));
}

void GLTexture2D::LoadImage(const void *data)
{
    BindTo(GL_TEXTURE0);

    if (!mInitialized)
    {
        Initialize();
    }

    SetPixelUnpacking();
    GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, data));
}

void GLTexture2D::LoadSubImage(GLint x, GLint y, GLsizei width, GLsizei height, const void *pixels)
{
    BindTo(GL_TEXTURE0);

    if (!mInitialized)
    {
        Initialize();
    }

    SetPixelUnpacking();
    GL_VERIFY(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, mFormat, GL_UNSIGNED_BYTE, pixels));
}


void GLTexture2D::Initialize()
{
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
}

void GLTexture2D::SetPixelUnpacking()
{
    switch (mFormat)
    {
        case GL_RED:
            GL_VERIFY(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
            break;

        case GL_RGBA:
            GL_VERIFY(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
            break;

        default:
            ALIVE_FATAL("GLTexture2D: Texture format not implemented.");
    }
}
