#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

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

bool Shader::load(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
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

	//geometry shader
	unsigned int geometryShader = 0;
	if (geometryPath != nullptr)
	{
		string sGeometryShader;
		openFile(geometryPath, sGeometryShader);
		const GLchar* geometryShaderSource = sGeometryShader.c_str();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
		glCompileShader(geometryShader);
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteShader(geometryShader);
			return false;
		}
	}

	m_uId = glCreateProgram();
	if (m_uId == 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << std::endl;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryShader > 0)
		{
			glDeleteShader(geometryShader);
		}
		return false;
	}
	glAttachShader(m_uId, vertexShader);
	glAttachShader(m_uId, fragmentShader);
	glAttachShader(m_uId, geometryShader);
	glLinkProgram(m_uId);

	glGetProgramiv(m_uId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_uId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
		glDeleteProgram(m_uId);
		m_uId = 0;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryShader > 0)
		{
			glDeleteShader(geometryShader);
		}
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryShader > 0)
	{
		glDeleteShader(geometryShader);
	}

	return true;
}

bool Shader::use()
{
	if (m_uId == 0)
		return false;

	glUseProgram(m_uId);
	return true;
}

void Shader::setVector3fv(const GLchar* name, const glm::vec3& vec) const
{
	glUniform3fv(glGetUniformLocation(m_uId, name), 1, glm::value_ptr(vec));
}
void Shader::setVector3f(const GLchar* name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_uId, name), x, y, z);
}
void Shader::setMatrix3fv(const GLchar* name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_uId, name), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setMatrix4fv(const GLchar* name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_uId, name), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setFloat(const GLchar* name, float f) const
{
	glUniform1f(glGetUniformLocation(m_uId, name), f);
}
void Shader::setInt(const GLchar* name, int i) const
{
	glUniform1i(glGetUniformLocation(m_uId, name), i);
}

GLuint Shader::getId()const
{
	return m_uId;
}

