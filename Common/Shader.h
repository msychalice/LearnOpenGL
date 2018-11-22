#pragma once

#include <string>
#include <glad/glad.h> // include glad to get all the required OpenGL headers

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
	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value) const;
	void setFloat(const string &name, float value) const;
	GLuint getId()const;

private:
	//特别需要注意的是传入的shader文件编码格式需要时ASCII 
	static void openFile(const char* fileName, string& shader);

private:
	// the program ID
	GLuint m_uId;
}; 
