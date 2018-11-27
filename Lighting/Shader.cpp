#include "../Common/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

//特别需要注意的是传入的shader文件编码格式需要时ASCII 
void Shader::openFile(const char* fileName, string& shader)
{
	ifstream in(fileName);
	stringstream ss;
	ss << in.rdbuf();
	shader = ss.str();
	in.close();
}

Shader::Shader():m_uId(0) {}

Shader::~Shader()
{
	if (m_uId != 0)
	{
		glDeleteProgram(m_uId);
		m_uId = 0;
	}
}

bool Shader::load(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// vertex shader
	string sVertexShader;
	openFile(vertexPath, sVertexShader);
	const GLchar* vertexShaderSource = sVertexShader.c_str();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(vertexShader);
		return false;
	}

	//fragment shader
	string sFragmentShader;
	openFile(fragmentPath, sFragmentShader);
	const GLchar* fragmentShaderSource = sFragmentShader.c_str();

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return false;
	}

	m_uId = glCreateProgram();
	if (m_uId == 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << std::endl;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return false;
	}
	glAttachShader(m_uId, vertexShader);
	glAttachShader(m_uId, fragmentShader);
	glLinkProgram(m_uId);

	glGetProgramiv(m_uId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_uId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
		glDeleteProgram(m_uId);
		m_uId = 0;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

bool Shader::use()
{
	if (m_uId == 0)
		return false;

	glUseProgram(m_uId);
	return true;
}

void Shader::setBool(const string &name, bool value) const
{

}

void Shader::setInt(const string &name, int value) const
{

}

void Shader::setFloat(const string &name, float value) const
{

}

GLuint Shader::getId()const
{
	return m_uId;
}

