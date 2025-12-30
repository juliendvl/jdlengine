#version 450

layout(set = 0, binding = 0) uniform GlobalUniformBuffer {
    mat4 V;
    mat4 P;
    vec3 cameraPosition;
} globalUbo;

layout(push_constant) uniform PushConstantData {
    mat4 M;
} pushData;

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragColor;


void main()
{
    gl_Position = globalUbo.P * globalUbo.V * pushData.M * vec4(inVertex, 1.0);
    fragColor = inNormal;
}
