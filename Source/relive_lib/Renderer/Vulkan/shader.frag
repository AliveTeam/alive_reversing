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
layout(location = 4) flat in uint drawType;

layout(location = 0) out vec4 outColor;

vec4 PixelToPalette(float v)
{
    return texture(texPalette, vec2(v, palIndex / 255.0f));
}


vec4 TextureHack()
{
    vec4 ret = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    switch(samplerIndex)
    {
    case 0: ret = textureLod(texSpriteSheets[0], fragTexCoord, 0.0f); break;
    case 1: ret = textureLod(texSpriteSheets[1], fragTexCoord, 0.0f); break;
    case 2: ret = textureLod(texSpriteSheets[2], fragTexCoord, 0.0f); break;
    case 3: ret = textureLod(texSpriteSheets[3], fragTexCoord, 0.0f); break;
    case 4: ret = textureLod(texSpriteSheets[4], fragTexCoord, 0.0f); break;
    case 5: ret = textureLod(texSpriteSheets[5], fragTexCoord, 0.0f); break;
    case 6: ret = textureLod(texSpriteSheets[6], fragTexCoord, 0.0f); break;
    case 7: ret = textureLod(texSpriteSheets[7], fragTexCoord, 0.0f); break;
    case 8: ret = textureLod(texSpriteSheets[8], fragTexCoord, 0.0f); break;
    }
    return ret;
}

void main()
{
    // palIndex
    if (drawType == 2)
    {
        outColor = TextureHack();
    }
    else
    {
        float texelSprite = TextureHack().r;
        outColor = PixelToPalette(texelSprite);
    }
}
