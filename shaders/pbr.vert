#version 450 core

uniform mat4 M;

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

void main()
{
	gl_Position = M * vec4(inVertex, 1.0);
}
