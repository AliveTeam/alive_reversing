#version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D texSampler2;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint samplerIndex;

layout(location = 0) out vec4 outColor;

void main() 
{
    if (samplerIndex == 0)
    {
        outColor = texture(texSampler, fragTexCoord);
    }
    else
    {
        outColor = texture(texSampler2, fragTexCoord);
        //outColor = vec4(1.0f, 0.0f, 1.0f, 0.5f);
    }
}
