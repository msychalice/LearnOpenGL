
#version 330 core
in vec2 TexCoords;

uniform sampler2D texture1;
out vec4 FragColor;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
//    FragColor = vec4(vec3(depth), 1.0);			//线性depth值
//    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);	//非线性depth值
	vec4 texColor = texture(texture1, TexCoords);
	if(texColor.a < 0.1)
		discard;
    FragColor = texColor;
}