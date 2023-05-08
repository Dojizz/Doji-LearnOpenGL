#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 Pos;
in vec2	TexCoords;

struct Material {
	sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{
	FragColor = vec4(texture(material.texture_diffuse1, TexCoords));
}
