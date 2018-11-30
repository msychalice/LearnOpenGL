
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Common/Camera.h"
#include "../Common/Shader.h"

using namespace std;

float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
Camera camera(cameraPos);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, float deltaTime)
{
	camera.processKeyboardInput(window, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processScrollEvent((float)xoffset, (float)yoffset);
}

bool loadImage(GLuint& texture, const char * fileName, GLenum format)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);	// should be called before stbi_load
	GLubyte *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return false;
	}
	stbi_image_free(data);
	return true;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLuint screenWidth = 800;
	GLuint screenHeight = 600;


	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, screenWidth, screenHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
	};
	GLuint indices[] = {  // note that we start from 0!
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// ..:: Initialization code :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader shaderContainer;
	shaderContainer.load("container.vert", "container.frag");
	Shader shaderLight;
	shaderLight.load("container.vert", "light.frag");

	GLuint diffuseMap;
	loadImage(diffuseMap, "../Resources/Textures/container2.png", GL_RGBA);
	GLuint specularMap;
	loadImage(specularMap, "../Resources/Textures/container2_specular.png", GL_RGBA);

	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightAmbientColor(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuseColor(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecularColor(1.0f, 1.0f, 1.0f);
	glm::vec3 containerPos(0.0f, 0.0f, 0.0f);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//lightPos.y = 0.0f;
		//lightPos.x = cos((float)glfwGetTime()) * 3.0f;
		//lightPos.z = sin((float)glfwGetTime()) * 3.0f;

		// input
		processInput(window, deltaTime);

		// rendering commands here
		//
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw container
		shaderContainer.use();


		glm::mat4 view = camera.getLookatMatrix();
		glm::mat4 projection = camera.getProjectionMatrix((float)screenWidth, (float)screenHeight);
		glm::mat4 model;
		model = glm::translate(model, containerPos);
		//model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		glm::vec3 lightPosInView = glm::vec3(view * glm::vec4(lightPos, 1.0f));
		shaderContainer.setVector3fv("light.position", lightPosInView);

		//glm::vec3 lightColor;
		//lightColor.x = sin((float)glfwGetTime() * 2.0f);
		//lightColor.y = sin((float)glfwGetTime() * 0.7f);
		//lightColor.z = sin((float)glfwGetTime() * 1.3f);
		//glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

		shaderContainer.setVector3fv("light.ambient", lightAmbientColor);
		shaderContainer.setVector3fv("light.diffuse", lightDiffuseColor);
		shaderContainer.setVector3fv("light.specular", lightSpecularColor);

		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(view * model)));
		shaderContainer.setMatrix3fv("normalMatrix", normalMatrix);
		shaderContainer.setMatrix4fv("model", model);
		shaderContainer.setMatrix4fv("view", view);
		shaderContainer.setMatrix4fv("projection", projection);

		//shaderContainer.setVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
		//shaderContainer.setVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
		//shaderContainer.setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
		shaderContainer.setInt("material.diffuse", 0);
		shaderContainer.setInt("material.specular", 1);
		shaderContainer.setFloat("material.shininess", 64.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//draw light 
		shaderLight.use();

		shaderLight.setVector3fv("lightColor", lightDiffuseColor);
		shaderLight.setMatrix4fv("view", view);
		shaderLight.setMatrix4fv("projection", projection);
		glm::mat4 modelLight;
		modelLight = glm::translate(modelLight, lightPos);
		modelLight = glm::scale(modelLight, glm::vec3(0.2f));
		shaderLight.setMatrix4fv("model", modelLight);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		// check and call events and swap the buffers
		glfwPollEvents();
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}
