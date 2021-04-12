#include "GLShader.hpp"

GLShader::GLShader()
{
    
}

void printProgramLog(GLuint program)
{
    //Make sure name is shader
    if (glIsProgram(program))
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //Print Log
            LOG_WARNING(infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        LOG_WARNING("Name " << program << " is not a program");
    }
}

bool GLShader::LoadSource(const std::string& vertex_Source, const std::string&  fragment_Source)
{
    // Check if our handles are already created
    // so we can delete them

    if (mProgramID != 0)
        glDeleteProgram(mProgramID);
    if (mVertexID != 0)
        glDeleteShader(mVertexID);
    if (mFragmentID != 0)
        glDeleteShader(mFragmentID);

    mProgramID = glCreateProgram();

    
    mVertexID = CompileShader(vertex_Source.c_str(), GL_VERTEX_SHADER);
    mFragmentID = CompileShader(fragment_Source.c_str(), GL_FRAGMENT_SHADER);

    // Attach our compiled shaders to our main program
    glAttachShader(mProgramID, mVertexID);
    glAttachShader(mProgramID, mFragmentID);
    glBindFragDataLocation(mProgramID, 0, "vFragColor");
    glLinkProgram(mProgramID);

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE)
    {
        LOG_ERROR("Error linking program " << mProgramID);
        printProgramLog(mProgramID);
        return false;
    }

    return true;
}

bool GLShader::LoadFromFile(const char* vertex_Path, const char* fragment_Path)
{
    return LoadSource(shaderReadFile(vertex_Path), shaderReadFile(fragment_Path));
}

GLuint GLShader::GetProgramID()
{
    return mProgramID;
}

GLuint GLShader::GetAttributeLocation(const char* attr)
{
    return glGetAttribLocation(mProgramID, attr);
}

void GLShader::UniformMatrix4fv(const char * name, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void GLShader::UniformVec2(const char* name, glm::vec2 vector)
{
    glUniform2f(glGetUniformLocation(mProgramID, name), vector.x, vector.y);
}

void GLShader::UniformVec3(const char* name, glm::vec3 vector)
{
    glUniform3f(glGetUniformLocation(mProgramID, name), vector.x, vector.y, vector.z);
}

void GLShader::UniformVec4(const char* name, glm::vec4 vector)
{
    glUniform4f(glGetUniformLocation(mProgramID, name), vector.x, vector.y, vector.z, vector.w);
}

void GLShader::Uniform1i(const char* name, GLint v)
{
    glUniform1i(glGetUniformLocation(mProgramID, name), v);
}

void GLShader::Use()
{
    glUseProgram(mProgramID);
}

void GLShader::UnUse()
{
    glUseProgram(0);
}

void printShaderLog(GLuint shader)
{
    //Make sure name is shader
    if (glIsShader(shader))
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //Print Log
            LOG_WARNING(infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        LOG_ERROR("Shader " << shader << " is not a shader");
    }
}

GLuint GLShader::CompileShader(const char * source, GLenum shaderType)
{
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);

    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE)
    {
        LOG_ERROR("Unable to compile fragment shader " << shaderID);
        printShaderLog(shaderID);
        return false;
    }

    return shaderID;
}

std::string shaderReadFile(const char* filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        LOG_ERROR("Could not read file " << filePath << ". File does not exist.");
        return false;
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

void GLShader::Free()
{
    if (mVertexID != 0)
        glDeleteShader(mVertexID);

    if (mFragmentID != 0)
        glDeleteShader(mFragmentID);

    if (mProgramID != 0)
        glDeleteProgram(mProgramID);
}