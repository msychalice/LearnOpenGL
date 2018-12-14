#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1; // reflection map
uniform samplerCube skybox;

void main()
{    
    vec3 I = normalize(FragPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 reflectiveColor = vec4(texture(skybox, R).rgb, 1.0);

    FragColor = texture(texture_height1, TexCoords) * reflectiveColor + texture(texture_diffuse1, TexCoords);
}