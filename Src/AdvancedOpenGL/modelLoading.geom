#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
} gs_in[];

out vec2 TexCoords; 
out vec3 Normal;
out vec3 FragPos;

uniform float time;
uniform mat4 projection;

vec4 getNewPosition(vec4 position, vec3 direction)
{
    return position + vec4(direction, 0.0);
} 

vec3 getNewNormal(vec3 normal, vec3 direction)
{
	return vec3(inverse(projection) * vec4(direction, 1.0)) + normal;
}

vec3 getNewFragPos(vec3 fragPos, vec3 direction)
{
	return vec3(inverse(projection) * vec4(direction, 1.0)) + fragPos;
}

vec3 getDirection()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 normal = normalize(cross(a, b));
	float magnitude = 2.0;
	return normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
}

void main() {
    vec3 direction = getDirection();

    gl_Position = getNewPosition(gl_in[0].gl_Position, direction);
    TexCoords = gs_in[0].TexCoords;
    Normal = getNewNormal(gs_in[0].Normal, direction);
    FragPos = getNewFragPos(gs_in[0].FragPos, direction);
    EmitVertex();
    gl_Position = getNewPosition(gl_in[1].gl_Position, direction);
    TexCoords = gs_in[1].TexCoords;
    Normal = getNewNormal(gs_in[1].Normal, direction);
    FragPos = getNewFragPos(gs_in[1].FragPos, direction);
    EmitVertex();
    gl_Position = getNewPosition(gl_in[2].gl_Position, direction);
    TexCoords = gs_in[2].TexCoords;
    Normal = getNewNormal(gs_in[2].Normal, direction);
    FragPos = getNewFragPos(gs_in[2].FragPos, direction);
    EmitVertex();
    EndPrimitive();
}  
