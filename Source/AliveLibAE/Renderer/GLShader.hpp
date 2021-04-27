#pragma once

#include <GL/glew.h>
#include <SDL_opengl.h>

#define GLM_FORCE_SILENT_WARNINGS

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "logger.hpp"

#include <string>

class GLShader final
{
public:
    GLShader();

    bool LoadSource(const s8* vertex_Source, const s8* fragment_Source);
    bool LoadFromFile(const s8* vertex_Path, const s8* fragment_Path);

    GLuint GetProgramID();
    GLuint GetAttributeLocation(const s8* attr);

    void UniformMatrix4fv(const s8* name, glm::mat4 matrix);
    void UniformVec2(const s8* name, glm::vec2 vector);
    void UniformVec3(const s8* name, glm::vec3 vector);
    void UniformVec4(const s8* name, glm::vec4 vector);
    void Uniform1i(const s8* name, GLint v);

    void Use();
    void UnUse();

    void Free();

private:
    GLuint mVertexID = 0;
    GLuint mFragmentID = 0;
    GLuint mProgramID = 0;
    GLenum mShaderType = 0;

    GLuint CompileShader(const s8* source, GLenum shaderType);
};

extern const s8* gShader_TextureVSH;
extern const s8* gShader_TextureFSH;