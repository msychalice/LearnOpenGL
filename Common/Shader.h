#pragma once

#include <string>
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();
	//reads and builds the shader
	bool load(const GLchar* vertexPath, const GLchar* fragmentPath);
	// use/activate the shader
	bool use();
	// utility uniform functions
	void setVector3fv(const GLchar* name, const glm::vec3& vec) const;
	void setVector3fv(const GLchar* name, float x, float y, float z) const;
	void setMatrix3fv(const GLchar* name, const glm::mat3& mat) const;
	void setMatrix4fv(const GLchar* name, const glm::mat4& mat) const;
	GLuint getId()const;

private:
	//特别需要注意的是传入的shader文件编码格式需要时ASCII 
	static void openFile(const char* fileName, string& shader);

private:
	// the program ID
	GLuint m_uId;
}; 
