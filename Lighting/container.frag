
#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 direction;
	vec3 position;
	float cutOff;
	float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos; 
in vec2 TexCoords;
out vec4 FragColor;

uniform DirLight dirLight;  
#define POINT_LIGHT_NUM 4  
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform SpotLight spotLight;  
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir); 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir); 

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(-FragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < POINT_LIGHT_NUM; i++)
	{
		result += CalcPointLight(pointLights[i], norm, viewDir);
	}

	result += CalcSpotLight(spotLight, norm, viewDir);

	FragColor = vec4(result, 1.0);
} 

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (vec3(texture(material.diffuse, TexCoords)) * diff) * light.diffuse;

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular; 


	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 lightDir = normalize(light.position - FragPos);

	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)); 

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (vec3(texture(material.diffuse, TexCoords)) * diff) * light.diffuse;

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular; 

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 lightDir = normalize(light.position - FragPos);

	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)); 

	float theta = dot(lightDir, normalize(-light.direction));

	if(theta > light.outerCutOff)
	{
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = (vec3(texture(material.diffuse, TexCoords)) * diff) * light.diffuse;

		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular; 

		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

		return (ambient + diffuse + specular) * intensity * attenuation;
	}
	else
	{
		return ambient * attenuation;
	}
}

