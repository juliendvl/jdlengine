#version 450

layout(set = 1, binding = 0) uniform MaterialData {
    vec4 baseColorFactor;
} materialData;

layout(set = 1, binding = 1) uniform sampler2D baseColorTexture;

layout(location = 0) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = materialData.baseColorFactor * texture(baseColorTexture, fragUV);
}
