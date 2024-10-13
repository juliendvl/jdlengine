#version 450 core

struct Material
{
	sampler2D albedo;
};
uniform Material material;

layout(location = 2) in vec2 fragUV; 

layout(location = 0) out vec4 outColor;


void main()
{
	outColor = texture(material.albedo, fragUV);
}
