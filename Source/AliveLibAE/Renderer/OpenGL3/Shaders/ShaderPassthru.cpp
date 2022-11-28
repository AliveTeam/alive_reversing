#include "../AliveLibCommon/Types.hpp"
#include "../GLShader.hpp"


//
// TODO: Consolidate Passthru shader to single VS and FS
//       Filtering can be done via uniform, bin integer VS
//


const char_type* gShader_PassthruVSH = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec2 vsPos;
layout (location = 1) in vec2 vsUV;

out vec2 fsUV;

uniform vec2 vsViewportSize;


void main()
{
    gl_Position.x = ((vsPos.x / vsViewportSize.x) * 2) - 1;
    gl_Position.y = (1 - ((vsPos.y / vsViewportSize.y) * 2));
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    // Pass-thru
    fsUV = vsUV;
}
)";

const char_type* gShader_PassthruFSH = R"(
#version 140

in vec2 fsUV;

out vec4 outColor;

uniform bool fsFlipUV;
uniform vec2 fsTexSize;
uniform sampler2D texTextureData;


void main()
{
    vec2 scaledUV = fsUV / fsTexSize;

    if (fsFlipUV)
    {
        scaledUV.y = 1.0 - scaledUV.y;
    }

    outColor = texture(texTextureData, scaledUV);
}
)";

const char_type* gShader_PassthruFilterFSH = R"(
#version 140

in vec2 fsUV;

out vec4 outColor;

uniform vec2 fsTexSize;
uniform sampler2D texTextureData;


int get888FromNormalized(in vec3 rgbInput)
{
    int rValue = int(ceil(rgbInput.r * 255.0f));
    int gValue = int(ceil(rgbInput.g * 255.0f));
    int bValue = int(ceil(rgbInput.b * 255.0f));

    rValue = rValue << 16;
    gValue = gValue << 8;

    return rValue | gValue | bValue;
}

vec3 getNormalizedFrom888(in int rgbInput)
{
    float rValue = float((rgbInput >> 16) & 0xFF) / 255.0f;
    float gValue = float((rgbInput >> 8) & 0xFF) / 255.0f;
    float bValue = float(rgbInput & 0xFF) / 255.0f;

    return vec3(rValue, gValue, bValue);
}

vec2 getScaledUV(in vec2 coord)
{
    return coord / fsTexSize;
}

void main()
{
    bool scanline = int(mod(gl_FragCoord.y, 2.0f)) > 0;

    if (scanline)
    {
        vec4 aboveTexel = texture(texTextureData, getScaledUV(vec2(fsUV.x, fsUV.y + 1.0)));
        vec4 belowTexel = texture(texTextureData, getScaledUV(fsUV));

        int aboveTexel888 = get888FromNormalized(aboveTexel.rgb);
        int belowTexel888 = get888FromNormalized(belowTexel.rgb);

        // Do the bit rotation stuff
        int pixelResult =
            (((aboveTexel888 & 0xF8F8F8) + (belowTexel888 & 0xF8F8F8)) >> 1) |
            (belowTexel888 & 0xF8F8F8) << 23;

        pixelResult = pixelResult & 0xFFFFFF;

        outColor = vec4(getNormalizedFrom888(pixelResult), 1.0);
    }
    else
    {
        outColor = texture(texTextureData, getScaledUV(fsUV));
    }
}
)";
