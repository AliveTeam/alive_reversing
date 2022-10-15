#include "GLShader.hpp"
#include "../AliveLibCommon/FatalError.hpp"

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

void GLShader::UniformVec2(const char_type* name, f32 x, f32 y)
{
    glUniform2f(glGetUniformLocation(mProgramID, name), x, y);
}

void GLShader::UniformVec3(const char_type* name, f32 x, f32 y, f32 z)
{
    glUniform3f(glGetUniformLocation(mProgramID, name), x, y, z);
}

void GLShader::UniformVec4(const char_type* name, f32 x, f32 y, f32 z, f32 w)
{
    glUniform4f(glGetUniformLocation(mProgramID, name), x, y, z, w);
}

void GLShader::Uniform1i(const char_type* name, GLint v)
{
    glUniform1i(glGetUniformLocation(mProgramID, name), v);
}

void GLShader::Uniform1iv(const char_type* name, GLsizei count, const GLint* value)
{
    glUniform1iv(glGetUniformLocation(mProgramID, name), count, value);
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
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec2 vsPos;
layout (location = 1) in vec2 vsUV;

out vec2 fsUV;

uniform vec2 vsViewportSize;
uniform vec2 vsTexSize;


void main()
{
    gl_Position.x = ((vsPos.x / vsViewportSize.x) * 2) - 1;
    gl_Position.y = (1 - ((vsPos.y / vsViewportSize.y) * 2));
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    // Pass-thru
    fsUV = vsUV / vsTexSize;
}
)";

const char_type* gShader_PassthruFSH = R"(
#version 140

in vec2 fsUV;

out vec4 outColor;

uniform sampler2D TextureSampler;


void main()
{
    outColor = texture(TextureSampler, fsUV);
}
)";

const char_type* gShader_PsxVSH = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in ivec2 vsPos;
layout (location = 1) in uvec3 vsShadeColor;
layout (location = 2) in uvec4 vsUV;
layout (location = 3) in uvec4 vsFlags;
layout (location = 4) in uvec2 vsTexIndexing;

out vec3  fsShadeColor;
out vec2  fsUV;
flat out uvec4 fsFlags;
flat out uvec2 fsTexIndexing;

uniform vec2 vsViewportSize;


void main()
{
    gl_Position.x = ((vsPos.x / vsViewportSize.x) * 2) - 1;
    gl_Position.y = (1 - ((vsPos.y / vsViewportSize.y) * 2));
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    
    fsShadeColor = vsShadeColor;
    fsFlags = vsFlags;
    fsTexIndexing = vsTexIndexing;

    // This is basically for if there is no texture in this batch
    if (vsUV.z == 0u || vsUV.w == 0u)
    {
        fsUV = vec2(0.0);
    }
    else
    {
        fsUV = vec2(vsUV.xy) / vsUV.zw;
    }
}
)";

const char_type* gShader_PsxFSH = R"(
#version 140

in vec3 fsShadeColor;
in vec2 fsUV;
flat in uvec4 fsFlags;
flat in uvec2 fsTexIndexing;

out vec4 outColor;

uniform sampler2D texPalette;
uniform sampler2D texGas;
uniform sampler2D texCamera;
uniform sampler2D texFG1Masks[4];
uniform sampler2D texSpriteSheets[8];

const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
const int BLEND_MODE_ONE_DST_ADD_ONE_SRC   = 1;
const int BLEND_MODE_ONE_DST_SUB_ONE_SRC   = 2;
const int BLEND_MODE_ONE_DST_ADD_QRT_SRC   = 3;

const int DRAW_FLAT        = 0;
const int DRAW_DEFAULT_FT4 = 1;
const int DRAW_CAM         = 2;
const int DRAW_FG1         = 3;
const int DRAW_GAS         = 4;

const vec2 frameSize = vec2(640.0, 240.0);


vec4 PixelToPalette(float v)
{
    return texture(texPalette, vec2(v, fsTexIndexing.x / 255.0));
}

bool dither()
{
    return mod(gl_FragCoord.x + mod(gl_FragCoord.y, 2.0), 2.0) > 0.0;
}

vec3 handle_shading(in vec3 texelT)
{
    bool isShaded = int(fsFlags.z) > 0;
    vec3 texelP = texelT;

    if (isShaded)
    {
        texelP = clamp((texelT * (fsShadeColor / 255.0)) / 0.5f, 0.0f, 1.0f);
    }

    return texelP;
}

vec4 handle_final_color(in vec4 src, in bool doShading)
{
    int blendMode = int(fsFlags.w);
    bool isSemiTrans = int(fsFlags.y) > 0;
    vec4 ret = src;

    if (src == vec4(0.0))
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }

    if (doShading)
    {
        ret.rgb = handle_shading(src.rgb);
    }

    if (isSemiTrans && src.a == 1.0)
    {
        switch (blendMode)
        {
            case BLEND_MODE_HALF_DST_ADD_HALF_SRC:
                ret = vec4(ret.rgb * 0.5, 0.5);
                break;

            case BLEND_MODE_ONE_DST_ADD_ONE_SRC:
            case BLEND_MODE_ONE_DST_SUB_ONE_SRC:
                ret = vec4(ret.rgb, 1.0);
                break;

            case BLEND_MODE_ONE_DST_ADD_QRT_SRC:
                ret = vec4(ret.rgb * 0.25, 1.0);
                break;
        }
    }
    else
    {
        ret = vec4(ret.rgb, 0.0);
    }

    return ret;
}

void draw_flat()
{
    outColor.rgb = fsShadeColor / 255.0;

    outColor = handle_final_color(vec4(outColor.rgb, 1.0), false);
}

void draw_default_ft4()
{
    float texelSprite = 0.0;

    switch (fsTexIndexing.y)
    {
        case 0u:
            texelSprite = texture(texSpriteSheets[0], fsUV).r;
            break;

        case 1u:
            texelSprite = texture(texSpriteSheets[1], fsUV).r;
            break;

        case 2u:
            texelSprite = texture(texSpriteSheets[2], fsUV).r;
            break;

        case 3u:
            texelSprite = texture(texSpriteSheets[3], fsUV).r;
            break;

        case 4u:
            texelSprite = texture(texSpriteSheets[4], fsUV).r;
            break;

        case 5u:
            texelSprite = texture(texSpriteSheets[5], fsUV).r;
            break;

        case 6u:
            texelSprite = texture(texSpriteSheets[6], fsUV).r;
            break;

        case 7u:
            texelSprite = texture(texSpriteSheets[7], fsUV).r;
            break;
    }

    vec4 texelPal = PixelToPalette(texelSprite);

    outColor = handle_final_color(texelPal, true);
}

void draw_cam()
{
    outColor = texture(texCamera, fsUV);

    outColor = vec4(outColor.rgb, 0.0);
}

void draw_fg1()
{
    vec4 mask = vec4(0.0);

    switch (fsTexIndexing.y)
    {
        case 0u:
            mask = texture(texFG1Masks[0], fsUV);
            break;

        case 1u:
            mask = texture(texFG1Masks[1], fsUV);
            break;

        case 2u:
            mask = texture(texFG1Masks[2], fsUV);
            break;

        case 3u:
            mask = texture(texFG1Masks[3], fsUV);
            break;
    }

    outColor = vec4(texture(texCamera, fsUV).rgb, 0.0);

    if (mask.rgb == vec3(0.0))
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void draw_gas()
{
    vec4 texelGas = texture(texGas, fsUV);

    if (dither())
    {
        outColor = texelGas * 0.5;
    }
    else
    {
        outColor = vec4(vec3(0.0), 1.0);
    }
}

void main()
{
    int drawType = int(fsFlags.x);

    switch (drawType)
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

        case DRAW_GAS:
            draw_gas();
            break;
    }
} 
)";
