#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 Pos;
in vec2	TexCoords;

uniform vec3 viewPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 lightColor;
	float innerSpotDirCos;
	float outerSpotDirCos;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec3 CalcPointLight(vec3 pos, vec3 normal, vec2 tex, Material m, PointLight l){
	vec3 lightDir = normalize(pos - l.position);
	float dist = length(lightDir);
	float attenuation = 1.f/(l.constant + l.linear * dist + l.quadratic * dist * dist);

	vec3 ambientLight = l.ambient * vec3(texture(m.diffuse, tex));
	vec3 diffuseLight = max(0, dot(-lightDir, normalize(normal))) * l.diffuse * vec3(texture(m.diffuse, tex));

	vec3 viewDir = normalize(viewPos - pos);
	vec3 refDir = reflect(lightDir, normalize(normal));
	vec3 specularLight = pow(max(0.0, dot(refDir, viewDir)), 32) * l.specular * vec3(texture(m.specular, tex));

	vec3 pointLightResult = ambientLight + diffuseLight + specularLight;
	pointLightResult *= attenuation;
	return pointLightResult;
}

vec3 CalcSpotLight(vec3 pos, vec2 tex, Material m, SpotLight l){
	vec3 lightDir = normalize(pos - l.position);
	float dist = length(lightDir);
	float attenuation = 1.f/(l.constant + l.linear * dist + l.quadratic * dist * dist);
	float fragDirCos = dot(lightDir, normalize(l.direction));
	float intensity = (fragDirCos - l.outerSpotDirCos)/(l.innerSpotDirCos - l.outerSpotDirCos);
	intensity = clamp(intensity, 0.0, 1.0);

	vec3 spotLightResult = l.lightColor * vec3(texture(m.diffuse, tex)) * intensity;
	spotLightResult *= attenuation;
	return spotLightResult;
}

void main()
{
	vec3 pointLightResult = CalcPointLight(Pos, Normal, TexCoords, material, pointLight);
	vec3 spotLightResult = CalcSpotLight(Pos, TexCoords, material, spotLight);

	vec3 result = spotLightResult + pointLightResult;
	FragColor = vec4(result, 1.0);
}

