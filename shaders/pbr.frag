#version 450 core

struct Material
{
	vec4 baseColorFactor;
	sampler2D baseColorTexture;
};

uniform Material material;

layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = material.baseColorFactor * texture(material.baseColorTexture, fragUV);
}
