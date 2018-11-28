#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public :
	Camera(glm::vec3 pos);

	void processKeyboardInput(GLFWwindow *window, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset);
	void processScrollEvent(float xoffset, float yoffset);

	glm::mat4 getLookatMatrix()const;
	glm::mat4 getProjectionMatrix(float screenWidth, float screenHeight)const;

	glm::vec3 getPos()const;

private:
	void updateCameraVectors(); // Calculates the front vector from the Camera's (updated) Euler Angles

	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	float m_yaw;
	float m_pitch;
	float m_fov;
};
