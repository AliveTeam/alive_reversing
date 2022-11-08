#pragma once

#include <GL/glew.h>

#include "../AliveLibCommon/Types.hpp"

class GLTexture2D final
{
public:
    GLTexture2D(u32 width, u32 height, GLenum format, bool initNow = false);
    ~GLTexture2D();

    void BindTo(GLenum texUnit);
    void LoadImage(const void* data);
    void LoadSubImage(GLint x, GLint y, GLsizei width, GLsizei height, const void* pixels);

private:
    GLenum mFormat;
    bool mInitialized;
    GLuint mGLId;
    u32 mHeight;
    u32 mWidth;


    void Initialize();
    void SetPixelUnpacking();
};
