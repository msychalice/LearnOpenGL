#include "../Common/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos):
	m_pos(pos),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_right(glm::normalize(glm::cross(m_front, m_up))),
	m_yaw(-90.0f),	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	m_pitch(0.0f),
	m_fov(45.0f)
{
	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::processKeyboardInput(GLFWwindow *window, float deltaTime)
{
	float speed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_pos += speed * m_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_pos -= speed * m_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_pos -= m_right * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_pos += m_right * speed;
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	updateCameraVectors();
}

void Camera::processScrollEvent(float xoffset, float yoffset)
{
	if (m_fov >= 1.0f && m_fov <= 45.0f)
		m_fov -= yoffset;
	if (m_fov <= 1.0f)
		m_fov = 1.0f;
	if (m_fov >= 45.0f)
		m_fov = 45.0f;
}

glm::mat4 Camera::getLookatMatrix()const
{
	return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix(float screenWidth, float screenHeight)const
{
	return glm::perspective(glm::radians(m_fov), screenWidth / screenHeight, 0.1f, 100.0f);
}

glm::vec3 Camera::getPos()const
{
	return m_pos;
}
glm::vec3 Camera::getFront()const
{
	return m_front;
}
