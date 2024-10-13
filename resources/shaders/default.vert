#version 450 core

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 2) out vec2 fragUV; 

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;


void main()
{
	gl_Position = P * V * M * vec4(inVertex, 1.0);
	fragUV = inUV;
}
