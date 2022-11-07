#pragma once

#include <GL/glew.h>
#include <SDL_opengl.h>

#include "logger.hpp"

#include <string>

class GLShader final
{
public:
    GLShader();

    bool LoadSource(const char_type* vertex_Source, const char_type* fragment_Source);
    bool LoadFromFile(const char_type* vertex_Path, const char_type* fragment_Path);

    GLuint GetProgramID();
    GLuint GetAttributeLocation(const char_type* attr);

    void UniformVec2(const char_type* name, f32 x, f32 y);
    void UniformVec3(const char_type* name, f32 x, f32 y, f32 z);
    void UniformVec4(const char_type* name, f32 x, f32 y, f32 z, f32 w);
    void Uniform1i(const char_type* name, GLint v);
    void Uniform1iv(const char_type* name, GLsizei count, const GLint* value);

    void Use();
    void UnUse();

    void Free();

private:
    GLuint mVertexID = 0;
    GLuint mFragmentID = 0;
    GLuint mProgramID = 0;
    GLenum mShaderType = 0;

    GLuint CompileShader(const char_type* source, GLenum shaderType);
};

extern const char_type* gShader_PassthruVSH;
extern const char_type* gShader_PassthruIntVSH;
extern const char_type* gShader_PassthruFSH;
extern const char_type* gShader_PassthruFilterFSH;
extern const char_type* gShader_PsxVSH;
extern const char_type* gShader_PsxFSH;
