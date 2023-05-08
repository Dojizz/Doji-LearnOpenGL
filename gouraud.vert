#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec3 fragColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
	vec3 Pos = vec3(view * model * vec4(aPos, 1.0));

	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * lightColor;
	vec3 diffuseLight = max(0, dot(normalize(lightPos - Pos), normalize(Normal))) * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - Pos);
	vec3 lightDir = normalize(Pos - lightPos);
	vec3 refDir = reflect(lightDir, normalize(Normal));
	vec3 specularLight = pow(max(0.0, dot(refDir, viewDir)), 32) * lightColor;

	fragColor = (ambientLight + diffuseLight + specularLight) * objectColor;
}