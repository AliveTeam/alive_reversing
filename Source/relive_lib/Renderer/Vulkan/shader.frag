#version 450

// TODO use layout(set = 0, binding = X) and allocate more descriptor sets

layout(binding = 1) uniform sampler2D texPalette;
layout(binding = 2) uniform sampler2D texGas;
layout(binding = 3) uniform sampler2D texCamera;
layout(binding = 4) uniform sampler2D texFG1Masks[4];
layout(binding = 8) uniform sampler2D texSpriteSheets[9];


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint samplerIndex;
layout(location = 3) flat in uint palIndex;

layout(location = 0) out vec4 outColor;

vec4 PixelToPalette(float v)
{
    return texture(texPalette, vec2(v, palIndex / 255.0f));
}

void main()
{
    // palIndex
    float texelSprite = texture(texSpriteSheets[samplerIndex], fragTexCoord).r;

    outColor = PixelToPalette(texelSprite);
}
