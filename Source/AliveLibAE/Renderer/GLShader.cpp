#include <Sys_common.hpp>
#include "GLShader.hpp"

GLShader::GLShader()
{
}

static std::string shaderReadFile(const char_type* filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        LOG_ERROR("Could not read file " << filePath << ". File does not exist.");
        return std::string();
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

void printProgramLog(GLuint program)
{
    //Make sure name is shader
    if (glIsProgram(program))
    {
        //Program log length
        s32 infoLogLength = 0;
        s32 maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        std::vector<char_type> infoLog(maxLength + 1);

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog.data());
        if (infoLogLength > 0)
        {
            ALIVE_FATAL(infoLog.data());
        }
    }
    else
    {
        LOG_ERROR("GL Program passed is NOT a program.");
    }
}

bool GLShader::LoadSource(const char_type* vertex_Source, const char_type* fragment_Source)
{
    // Check if our handles are already created
    // so we can delete them

    if (mProgramID != 0)
    {
        glDeleteProgram(mProgramID);
    }

    if (mVertexID != 0)
    {
        glDeleteShader(mVertexID);
    }

    if (mFragmentID != 0)
    {
        glDeleteShader(mFragmentID);
    }

    mProgramID = glCreateProgram();

    mVertexID = CompileShader(vertex_Source, GL_VERTEX_SHADER);
    mFragmentID = CompileShader(fragment_Source, GL_FRAGMENT_SHADER);

    // Attach our compiled shaders to our main program
    glAttachShader(mProgramID, mVertexID);
    glAttachShader(mProgramID, mFragmentID);
    glLinkProgram(mProgramID);

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &programSuccess);

    if (programSuccess != GL_TRUE)
    {
        LOG_ERROR("Failed to compile OpenGL Shader program");
        printProgramLog(mProgramID);
        return false;
    }

    return true;
}

bool GLShader::LoadFromFile(const char_type* vertex_Path, const char_type* fragment_Path)
{
    return LoadSource(shaderReadFile(vertex_Path).c_str(), shaderReadFile(fragment_Path).c_str());
}

GLuint GLShader::GetProgramID()
{
    return mProgramID;
}

GLuint GLShader::GetAttributeLocation(const char_type* attr)
{
    return glGetAttribLocation(mProgramID, attr);
}

void GLShader::UniformMatrix4fv(const char_type* name, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void GLShader::UniformVec2(const char_type* name, glm::vec2 vector)
{
    glUniform2f(glGetUniformLocation(mProgramID, name), vector.x, vector.y);
}

void GLShader::UniformVec3(const char_type* name, glm::vec3 vector)
{
    glUniform3f(glGetUniformLocation(mProgramID, name), vector.x, vector.y, vector.z);
}

void GLShader::UniformVec4(const char_type* name, glm::vec4 vector)
{
    glUniform4f(glGetUniformLocation(mProgramID, name), vector.x, vector.y, vector.z, vector.w);
}

void GLShader::Uniform1i(const char_type* name, GLint v)
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
        s32 infoLogLength = 0;
        s32 maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        std::vector<char_type> infoLog(maxLength + 1);

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog.data());
        if (infoLogLength > 0)
        {
            ALIVE_FATAL(infoLog.data());
        }
    }
    else
    {
        LOG_ERROR("Shader " << shader << " is not a shader");
    }
}

GLuint GLShader::CompileShader(const char_type* source, GLenum shaderType)
{
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &source, nullptr);
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

void GLShader::Free()
{
    if (mVertexID != 0)
    {
        glDeleteShader(mVertexID);
    }

    if (mFragmentID != 0)
    {
        glDeleteShader(mFragmentID);
    }

    if (mProgramID != 0)
    {
        glDeleteProgram(mProgramID);
    }
}


/*
 _____ _   _   ___ ______ ___________  _____
/  ___| | | | / _ \|  _  \  ___| ___ \/  ___|
\ `--.| |_| |/ /_\ \ | | | |__ | |_/ /\ `--.
 `--. \  _  ||  _  | | | |  __||    /  `--. \
/\__/ / | | || | | | |/ /| |___| |\ \ /\__/ /
\____/\_| |_/\_| |_/___/ \____/\_| \_|\____/

*/

const char_type* gShader_PassthruVSH = R"(
#version 330 core

layout (location = 0) in vec2 vsPos;
layout (location = 1) in vec2 vsUV;

out vec2 fsUV;

uniform vec2 vsTexSize;


void main()
{
    gl_Position.x = ((vsPos.x / 640) * 2) - 1;
    gl_Position.y = (1 - ((vsPos.y / 240) * 2));

    // Pass-thru
    fsUV = vsUV / vsTexSize;
}
)";

const char_type* gShader_PassthruFSH = R"(
#version 330 core

in vec2 fsUV;

out vec4 outColor;

uniform sampler2D TextureSampler;


void main()
{
    outColor = texture(TextureSampler, fsUV);
}
)";

const char_type* gShader_PsxVSH = R"(
#version 330 core

layout (location = 0) in vec3 vsPos;
layout (location = 1) in vec3 vsShadeColor;
layout (location = 2) in vec2 vsUV;

out vec3 fsShadeColor;
out vec2 fsUV;


void main()
{
    gl_Position.x = ((vsPos.x / 640) * 2) - 1;
    gl_Position.y = (1 - ((vsPos.y / 240) * 2));
    
    fsShadeColor = vsShadeColor;
    fsUV = vsUV;
}
)";

const char_type* gShader_PsxFSH = R"(
#version 330 core

in vec3 fsShadeColor;
in vec2 fsUV;

out vec4 outColor;

uniform sampler2D texTextureData;
uniform sampler2D texAdditionalData;
uniform sampler2D texFramebufferData;

uniform int  fsDrawType;
uniform bool fsIsSemiTrans;
uniform bool fsIsShaded;
uniform int  fsBlendMode;

const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
const int BLEND_MODE_ONE_DST_ADD_ONE_SRC   = 1;
const int BLEND_MODE_ONE_DST_SUB_ONE_SRC   = 2;
const int BLEND_MODE_ONE_DST_ADD_QRT_SRC   = 3;

const int DRAW_FLAT        = 0;
const int DRAW_DEFAULT_FT4 = 1;
const int DRAW_CAM         = 2;
const int DRAW_FG1         = 3;

const vec2 frameSize = vec2(640.0, 240.0);


vec4 PixelToPalette(float v)
{
    return texture(texAdditionalData, vec2(v, 0.5f));
}

vec3 checker(in vec2 uv)
{
    float checkSize = 2;
    float fmodResult = mod(floor(checkSize * uv.x) + floor(checkSize * uv.y), 2.0);
    float fin = max(sign(fmodResult), 0.0);
    return vec3(fin, fin, fin);
}

vec3 handle_blending(in vec3 src)
{
    vec3 texelFb = texture(texFramebufferData, gl_FragCoord.xy / frameSize).rgb;
    vec3 res = vec3(0.0);

    switch (fsBlendMode)
    {
        case BLEND_MODE_HALF_DST_ADD_HALF_SRC:
            res = (texelFb * 0.5) + (src * 0.5);
            break;

        case BLEND_MODE_ONE_DST_ADD_ONE_SRC:
            res = texelFb + src;
            break;

        case BLEND_MODE_ONE_DST_SUB_ONE_SRC:
            res = texelFb - src;
            break;

        case BLEND_MODE_ONE_DST_ADD_QRT_SRC:
            res = texelFb + (src * 0.25);
            break;
    }

    return res;
}

vec3 handle_shading(in vec3 texelT)
{
    vec3 texelP = texelT;

    if (fsIsShaded)
    {
        texelP = clamp((texelT * (fsShadeColor / 255.0)) / 0.5f, 0.0f, 1.0f);
    }

    return texelP;
}

void draw_flat()
{
    outColor.rgb = handle_blending(fsShadeColor / 255.0);
    outColor.rgb = clamp(outColor.rgb, vec3(0.0), vec3(1.0));
    outColor.a = 1.0;
}

void draw_default_ft4()
{
    vec4 texelPal = PixelToPalette(texture(texTextureData, fsUV).r);
    vec3 texelShaded = handle_shading(texelPal.rgb);

    if (texelPal == vec4(0.0, 0.0, 0.0, 0.0))
    {
        outColor.a = 0.0;
        return;
    }
    else
    {
        if (fsIsSemiTrans)
        {
            if (texelPal.a == 1.0)
            {
                outColor.rgb = handle_blending(texelShaded);
                outColor.rgb = clamp(outColor.rgb, vec3(0.0), vec3(1.0));
            }
            else
            {
                outColor.rgb = texelShaded;
            }
        }
        else
        {
            outColor.rgb = texelShaded;
        }
    }

    outColor.a = 1.0;
}

void draw_cam()
{
    outColor = texture(texTextureData, fsUV);

    outColor.rgb = handle_shading(outColor.rgb);
}

void draw_fg1()
{
    vec4 mask = texture(texAdditionalData, fsUV);

    outColor = texture(texTextureData, fsUV);

    if (mask.rgb == vec3(0.0, 0.0, 0.0))
    {
        outColor.a = 0.0;
    }

    outColor.rgb = handle_shading(outColor.rgb);
}

void main()
{
    switch (fsDrawType)
    {
        case DRAW_FLAT:
            draw_flat();
            break;

        case DRAW_DEFAULT_FT4:
            draw_default_ft4();
            break;

        case DRAW_CAM:
            draw_cam();
            break;

        case DRAW_FG1:
            draw_fg1();
            break;
    }
} 
)";
