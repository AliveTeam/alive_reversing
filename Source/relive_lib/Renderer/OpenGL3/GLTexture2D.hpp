#pragma once

#include <GL/glew.h>

#include "../../../relive_lib/Types.hpp"

class GLTexture2D final
{
public:
    GLTexture2D(u32 width, u32 height, GLenum format);
    ~GLTexture2D();

    void BindTo(GLenum texUnit);
    u32 GetHeight();
    u32 GetWidth();
    bool IsValid();
    void LoadImage(const void* pixels, GLenum type = GL_UNSIGNED_BYTE);
    void LoadSubImage(GLint x, GLint y, GLsizei width, GLsizei height, const void* pixels, GLenum type = GL_UNSIGNED_BYTE);

private:
    GLenum mFormat = 0;
    GLuint mGLId = 0;
    u32 mHeight = 0;
    u32 mWidth = 0;

    void SetPixelUnpacking();
};
