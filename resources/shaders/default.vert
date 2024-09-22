#version 450 core

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

uniform mat4 M;


void main()
{
	gl_Position = M * vec4(inVertex, 1.0);
}
