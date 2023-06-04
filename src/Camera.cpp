#include "Camera.h"
#include <iostream>

Camera::Camera() {
	flag = 1;
	zoomFactor = 1;
	flashlightOn = 0;

	vel = 0.0f;
	jumpflag = true;
	releasedctrl = false;
	movingforward = false;

	cameraPosition = glm::vec3(0.0f, 1.25f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	fov = 70.0f;
	projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(fov), (float)16.0 / (float)9.0, 0.1f, 100.0f);

	cameraSpeed = 3.5f;
	cameraSens = 0.1;

	currentFrame = 0.0;
	deltaTime = 0.0;
	lastFrame = 0.0;

	yaw = 0.0;
	pitch = 0.0;
}

Camera::Camera(const float& speed, const float& sens, const float& f) {
	flag = 1;
	zoomFactor = 1;
	flashlightOn = 0;

	vel = 0.0f;
	jumpflag = true;
	releasedctrl = false;
	movingforward = false;

	cameraPosition = glm::vec3(0.0f, 1.25f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	fov = f;
	projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(fov), (float)16.0 / (float)9.0, 0.1f, 100.0f);

	cameraSpeed = speed;
	cameraSens = sens;

	currentFrame = 0.0;
	deltaTime = 0.0;
	lastFrame = 0.0;

	yaw = 0.0;
	pitch = 0.0;
}

void Camera::CameraUpdate(GLFWwindow* window, const float& deltaTime, Shader& shader) {
	actualSpeed = cameraSpeed * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		cameraPosition = glm::vec3(0.0f, 1.25f, 0.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		yaw = 0.0f;
		pitch = 0.0f;
	}
	if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)) {
		if (movingforward) {
			actualSpeed = actualSpeed * 1.23f;
		}
		if ((fov <= 80) && movingforward) {
			fov += deltaTime * 180.0f;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)) {
		releasedctrl = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z)) * actualSpeed;
		movingforward = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
		movingforward = false;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition -= glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z)) * actualSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * actualSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * actualSpeed;
	}
	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && jumpflag) {
		vel = 7.0f;
		cameraPosition.y += (vel * deltaTime) - (11 * deltaTime * deltaTime);
		jumpflag = false;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		if (!(cameraPosition.y <= 1.25)) {
			cameraPosition.y -= actualSpeed;
		}
	}

	if ((glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && flag) {
		zoomFactor = 4;
		cameraSens /= 8.0;
		flag = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && !flag) {
		zoomFactor = 1;
		cameraSens *= 8.0;
		flag = 1;
	}
	
	if (!(cameraPosition.y <= 1.25)) {
		cameraPosition.y += (vel * deltaTime) - (11.0f * deltaTime * deltaTime);
		if (cameraPosition.y < 1.25) {
			cameraPosition.y = 1.25;
		}
		vel = vel - (22.0f * deltaTime);
	}
	else {
		jumpflag = true;
	}

	if ((fov > 70.0f) && (releasedctrl || !movingforward)) {
		fov -= deltaTime * 160.0f;
	}
	else {
		releasedctrl = false;
	}

	projectionMatrix = glm::perspective(glm::radians(fov/(float)zoomFactor), (float)16.0 / (float)9.0, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	shader.SetUniform3fv("u_ViewPos", 1, &cameraPosition[0]);
	shader.SetUniformMat4f("u_View", viewMatrix);
	shader.SetUniformMat4f("u_Projection", projectionMatrix);
}

void Camera::MouseUpdate(double xoffset, double yoffset) {
	xoffset *= cameraSens;
	yoffset *= cameraSens;

	yaw += xoffset;
	pitch -= yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
	if (yaw < 0.0f) {
		yaw = 360.0f - yaw;
	}

	glm::vec3 direction;
	direction.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::ScrollUpdate(const double& scroll) {
	fov += 4.0 * scroll;
	if (fov > 140.0) {
		fov = 140.0;
	}
	if (fov < 10.0) {
		fov = 10.0;
	}
}