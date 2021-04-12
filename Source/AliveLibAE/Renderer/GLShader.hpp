#pragma once

#include <GL/glew.h>
#include <SDL_opengl.h>

#define GLM_FORCE_SILENT_WARNINGS

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string shaderReadFile(const char* filePath);

class GLShader
{
public:
    GLShader();

    bool LoadSource(const std::string& vertex_Source, const std::string& fragment_Source);
    bool LoadFromFile(const char* vertex_Path, const char* fragment_Path);

    GLuint GetProgramID();
    GLuint GetAttributeLocation(const char* attr);

    void UniformMatrix4fv(const char* name, glm::mat4 matrix);
    void UniformVec2(const char* name, glm::vec2 vector);
    void UniformVec3(const char* name, glm::vec3 vector);
    void UniformVec4(const char* name, glm::vec4 vector);
    void Uniform1i(const char* name, GLint v);

    void Use();
    void UnUse();

    void Free();

private:
    GLuint mVertexID;
    GLuint mFragmentID;
    GLuint mProgramID;
    GLenum mShaderType;

    GLuint CompileShader(const char* source, GLenum shaderType);
};