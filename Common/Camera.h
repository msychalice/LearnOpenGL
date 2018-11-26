#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public :
	Camera();

	void processKeyboardInput(GLFWwindow *window, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset);
	void processScrollEvent(float xoffset, float yoffset);

	glm::mat4 getLookatMatrix()const;
	glm::mat4 getProjectionMatrix(float screenWidth, float screenHeight)const;

private:
	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;
	float m_yaw;
	float m_pitch;
	float m_fov;
	float speed;
};
