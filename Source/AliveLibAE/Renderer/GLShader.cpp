#include "GLShader.hpp"

GLShader::GLShader()
{
    
}

static std::string shaderReadFile(const char* filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        LOG_ERROR("Could not read file " << filePath << ". File does not exist.");
        return std::string();
    }

    std::string line = "";
    while (!fileStream.eof()) {
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
        std::vector<char> infoLog(maxLength + 1);

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog.data());
        if (infoLogLength > 0)
        {
            //Print Log
            LOG_WARNING(infoLog.data());
        }
    }
    else
    {
        LOG_ERROR("GL Program passed is NOT a program.");
    }
}

bool GLShader::LoadSource(const char* vertex_Source, const char* fragment_Source)
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
    glBindFragDataLocation(mProgramID, 0, "vFragColor");
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

bool GLShader::LoadFromFile(const char* vertex_Path, const char* fragment_Path)
{
    return LoadSource(shaderReadFile(vertex_Path).c_str(), shaderReadFile(fragment_Path).c_str());
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
        s32 infoLogLength = 0;
        s32 maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        std::vector<char> infoLog(maxLength + 1);

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog.data());
        if (infoLogLength > 0)
        {
            //Print Log
            LOG_WARNING(infoLog.data());
        }
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

const char* gShader_TextureVSH = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 m_Color;
out vec2 m_TexCoord;

uniform mat4 m_MVP;

void main()
{
	gl_Position = m_MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	
	m_Color = aColor;
	m_TexCoord = aTexCoord;
}
)";

const char* gShader_TextureFSH = R"(
#version 330 core

#define PI 3.1415926538

out vec4 vFrag;

in vec3 m_Color;
in vec2 m_TexCoord;

uniform sampler2D m_Sprite;
uniform sampler2D m_Palette;

uniform bool m_PaletteEnabled = true;
uniform s32 m_PaletteDepth = 0;
uniform bool m_Textured = true;
uniform bool m_Dithered = false;
uniform s32 m_DitherWidth = 0;
uniform s32 m_DitherHeight = 0;
uniform bool m_Debug = false;
uniform bool m_FG1 = false;
uniform vec4 m_FG1Size;

const vec2 CamSize = vec2(640,240);

vec4 PixelToPalette(float v)
{
	return texture(m_Palette, vec2((v / m_PaletteDepth) * 256, 0.5f));
}

vec3 checker(in vec2 uv)
{
  float checkSize = 2;
  float fmodResult = mod(floor(checkSize * uv.x) + floor(checkSize * uv.y), 2.0);
  float fin = max(sign(fmodResult), 0.0);
  return vec3(fin, fin, fin);
}

void main()
{
if (m_Debug)
	{
		// vFrag = vec4(m_Color,1); // Colored Lines
		vFrag = vec4(1,1,1,1);
		return;
	}
	
	if (m_FG1)
	{
		vec2 bgCoord = (m_TexCoord.xy * (m_FG1Size.zw / CamSize)) + (m_FG1Size.xy / CamSize);
		
		vFrag = texture(m_Palette, bgCoord) * texture(m_Sprite, m_TexCoord).rrrr;
		// vFrag =  texture(m_Sprite, m_TexCoord).rrrr; // Debug
		return;
	}
	
	if (m_Textured)
	{
		if (m_PaletteEnabled)
		{
			vec4 palColored = PixelToPalette(texture(m_Sprite, m_TexCoord ).r);
			vFrag = palColored * vec4(m_Color, 1.0f);
		}
		else
		{
			vFrag = texture(m_Sprite, m_TexCoord) * vec4(m_Color, 1.0f);
		}
	}
	else
	{
		vFrag = vec4(m_Color, 1.0f);
	}
	
	//vFrag = vec4(1,1,1,1);
    
	//vFrag = texture(m_Sprite, m_TexCoord);
	
	//vFrag = texture(m_Palette, m_TexCoord);
	
	
	if (m_Dithered)
	{
		float v = max(0.5,checker(m_TexCoord * vec2(m_DitherWidth / 2, m_DitherHeight / 2)).r);
		vFrag.rgb *= vec3(v,v,v) * 0.5;
	}
} 
)";