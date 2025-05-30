#version 450

layout(push_constant) uniform ModelMatrix {
    mat4 matrix;
} model;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragNormal;

void main()
{
    gl_Position = model.matrix * vec4(inPosition, 1.0);
    fragNormal = inNormal;
}
