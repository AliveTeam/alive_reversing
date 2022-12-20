#version 450
layout(binding = 1) uniform sampler2D texPalette;
layout(binding = 2) uniform sampler2D texGas;
layout(binding = 3) uniform sampler2D texCamera;
layout(binding = 4) uniform sampler2D texFG1Masks[4];
layout(binding = 8) uniform sampler2D texSpriteSheets[8];


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint samplerIndex;
layout(location = 3) flat in uint palIndex;

layout(location = 0) out vec4 outColor;

void main() 
{
    // palIndex
    vec4 pixel = texture(texSpriteSheets[0], fragTexCoord);

    outColor = texture(texSpriteSheets[samplerIndex], fragTexCoord);
}
