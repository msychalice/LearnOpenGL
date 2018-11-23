
#version 330 core
layout(location = 0) in vec3 aPos; // the position variable has attribute position 0
layout(location = 1) in vec3 aColor; // the color variable has attribute position 1
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor; // specify a color output to the fragment shader
out vec3 posColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
	ourColor = aColor; // set the output variable to a dark-red color
	posColor = aPos;
	texCoord = aTexCoord;
}
