#version 450

// UBO
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

// Vertex attributes
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uint inSamplerIndex;
layout(location = 4) in uint inPalIndex;
layout(location = 5) in uint inDrawType;
layout(location = 6) in uint inisShaded;
layout(location = 7) in uint inblendMode;
layout(location = 8) in uint inisSemiTrans;

// Shader outputs
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) flat out uint outSamplerIndex;
layout(location = 3) flat out uint outPalIndex;
layout(location = 4) flat out uint outDrawType;
layout(location = 5) flat out uint outisShaded;
layout(location = 6) flat out uint outblendMode;
layout(location = 7) flat out uint outisSemiTrans;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    outSamplerIndex = inSamplerIndex;
    outPalIndex = inPalIndex;
    outDrawType = inDrawType;
    outisShaded = inisShaded;
    outblendMode = inblendMode;
    outisSemiTrans = inisSemiTrans;
}
