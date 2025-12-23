#version 450

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragColor;


void main()
{
    gl_Position = vec4(inVertex, 1.0);
    fragColor = inNormal;
}
