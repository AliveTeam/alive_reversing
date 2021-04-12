#version 330 core
layout (location = 0) in vec3 aPos;layout (location = 1) in vec3 aColor;layout (location = 2) in vec2 aTexCoord;out vec3 m_Color;out vec2 m_TexCoord;uniform mat4 m_MVP;
void main(){	gl_Position = m_MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);		m_Color = aColor;	m_TexCoord = aTexCoord;}