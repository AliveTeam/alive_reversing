#version 330 core

#define PI 3.1415926538

out vec4 vFrag;

in vec3 m_Color;
in vec2 m_TexCoord;

uniform sampler2D m_Sprite;
uniform sampler2D m_Palette;

uniform bool m_PaletteEnabled = true;
uniform int m_PaletteDepth = 0;
uniform bool m_Textured = true;
uniform bool m_Dithered = false;
uniform int m_DitherWidth = 0;
uniform int m_DitherHeight = 0;
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