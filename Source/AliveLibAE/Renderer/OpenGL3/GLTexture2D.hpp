#pragma once

#include <GL/glew.h>

#include "../AliveLibCommon/Types.hpp"

class GLTexture2D final
{
public:
    GLTexture2D();
    GLTexture2D(u32 width, u32 height, GLenum format);
    GLTexture2D(const GLTexture2D& src);
    GLTexture2D(GLTexture2D&& src);
    ~GLTexture2D();

    GLTexture2D& operator=(GLTexture2D& src);
    GLTexture2D& operator=(GLTexture2D&& src);

    bool operator==(const GLTexture2D other);

    void BindTo(GLenum texUnit);
    u32 GetHeight();
    u32 GetWidth();
    bool IsValid();
    void LoadImage(const void* data);
    void LoadSubImage(GLint x, GLint y, GLsizei width, GLsizei height, const void* pixels);

private:
    GLenum mFormat = 0;
    GLuint mGLId = 0;
    u32 mHeight = 0;
    u32 mWidth = 0;

    bool mIsOriginal = false;


    void SetPixelUnpacking();
};
