#pragma once

#include <GL/glew.h>
#include <SDL_opengl.h>

#include "../../../relive_lib/Types.hpp"

class GLShader final
{
public:
    GLShader(const char_type* source, GLenum kind);
    ~GLShader();

    void AttachTo(GLuint programId);
    void DetachFrom(GLuint programId);
    GLenum GetKind();

private:
    GLuint mGLId;
    GLenum mKind;
};

extern const char_type* gShader_PassthruVSH;
extern const char_type* gShader_PassthruIntVSH;
extern const char_type* gShader_PassthruFSH;
extern const char_type* gShader_PassthruFilterFSH;
extern const char_type* gShader_PsxVSH;
extern const char_type* gShader_PsxFSH;
