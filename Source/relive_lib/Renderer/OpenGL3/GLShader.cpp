#include <GL/glew.h>

#include "../../../relive_lib/FatalError.hpp"
#include "../../../relive_lib/Types.hpp"
#include "GLDebug.hpp"
#include "GLShader.hpp"

GLShader::GLShader(const char_type* source, GLenum kind)
{
    mGLId = GL_VERIFY(glCreateShader(kind));
    mKind = kind;

    GL_VERIFY(glShaderSource(mGLId, 1, &source, nullptr));
    GL_VERIFY(glCompileShader(mGLId));

    // Check successful compile
    GLint infoLogLength;
    char_type infoLog[GL_INFOLOG_MAX_LENGTH];

    GL_VERIFY(glGetShaderInfoLog(mGLId, GL_INFOLOG_MAX_LENGTH - 1, &infoLogLength, infoLog));

    if (infoLogLength)
    {
        ALIVE_FATAL(infoLog);
    }
}

GLShader::~GLShader()
{
    if (mGLId)
    {
        GL_VERIFY(glDeleteShader(mGLId));
    }
}


void GLShader::AttachTo(GLuint programId)
{
    GL_VERIFY(glAttachShader(programId, mGLId));
}

void GLShader::DetachFrom(GLuint programId)
{
    GL_VERIFY(glDetachShader(programId, mGLId));
}

GLenum GLShader::GetKind()
{
    return mKind;
}
