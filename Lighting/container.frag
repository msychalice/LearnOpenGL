
#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
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

uniform Light light;  
uniform Material material;

void main()
{

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)); 

	float theta = dot(lightDir, normalize(-light.direction));

	if(theta > light.outerCutOff)
	{
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = (vec3(texture(material.diffuse, TexCoords)) * diff) * light.diffuse;

		vec3 viewDir = normalize(-FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular; 

		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

		vec3 result = (ambient + (diffuse + specular) * intensity) * attenuation;
		FragColor = vec4(result, 1.0);
	}
	else
	{
		FragColor = vec4(ambient * attenuation, 1.0);
	}
} 
