#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform bool gamma;

void main()
{           
	vec3 lightColor = vec3(0.3);
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(-fs_in.FragPos);
    float spec = 0.0;
	// Blinn Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	//// Phong
	//vec3 reflectDir = reflect(-lightDir, normal);
	//spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    vec3 specular = spec * lightColor; // assuming bright white light color

	// simple attenuation
    float max_distance = 1.5;
    float dist = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (gamma ? dist * dist : dist);
	diffuse *= attenuation;
	specular *= attenuation;

    FragColor = vec4(color * (diffuse + specular), 1.0);
//	if(gamma)
//	{
//		float gamma = 2.2;
//		FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
//	}
}