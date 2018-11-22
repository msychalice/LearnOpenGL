
#version 330 core
out vec4 FragColor;

in vec3 ourColor; // the input variable from the vertex shader (same name and same type)  
in vec3 posColor;
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec4 uniformColor;

void main()
{
	//FragColor = vec4(ourColor, 1.0);
	//FragColor = vec4(posColor, 1.0);
	FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0) + uniformColor;
}
