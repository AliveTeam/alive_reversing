#include <GL/glew.h>

#include "../AliveLibCommon/FatalError.hpp"
#include "../AliveLibCommon/Types.hpp"
#include "GLDebug.hpp"
#include "GLShaderProgram.hpp"

GLShaderProgram::GLShaderProgram(GLShader& vertexShader, GLShader& fragmentShader)
{
    mGLId = GL_VERIFY(glCreateProgram());

    vertexShader.AttachTo(mGLId);
    fragmentShader.AttachTo(mGLId);

    GL_VERIFY(glLinkProgram(mGLId));

    // Check successful compile
    GLint infoLogLength;
    char_type infoLog[GL_INFOLOG_MAX_LENGTH];

    GL_VERIFY(glGetProgramInfoLog(mGLId, GL_INFOLOG_MAX_LENGTH, &infoLogLength, infoLog));

    if (infoLogLength)
    {
        ALIVE_FATAL(infoLog);
    }

    vertexShader.DetachFrom(mGLId);
    fragmentShader.DetachFrom(mGLId);
}

GLShaderProgram::~GLShaderProgram()
{
    if (mGLId)
    {
        GL_VERIFY(glDeleteProgram(mGLId));
    }
}


void GLShaderProgram::Uniform1i(const char_type* uniform, GLint x)
{
    GL_VERIFY(glUniform1i(GetUniformLocation(uniform), x));
}

void GLShaderProgram::Uniform1iv(const char_type* uniform, GLsizei count, const GLint* data)
{
    GL_VERIFY(glUniform1iv(GetUniformLocation(uniform), count, data));
}

void GLShaderProgram::UniformVec2(const char_type* uniform, GLfloat x, GLfloat y)
{
    GL_VERIFY(glUniform2f(GetUniformLocation(uniform), x, y));
}

void GLShaderProgram::UniformVec3(const char_type* uniform, GLfloat x, GLfloat y, GLfloat z)
{
    GL_VERIFY(glUniform3f(GetUniformLocation(uniform), x, y, z));
}

void GLShaderProgram::UniformVec4(const char_type* uniform, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GL_VERIFY(glUniform4f(GetUniformLocation(uniform), x, y, z, w));
}

void GLShaderProgram::Use()
{
    GL_VERIFY(glUseProgram(mGLId));
}


GLuint GLShaderProgram::GetUniformLocation(const char_type* uniform)
{
    return GL_VERIFY(glGetUniformLocation(mGLId, uniform));
}