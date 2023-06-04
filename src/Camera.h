#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"

class Camera {
private:
	int flag;
	int zoomFactor;

	glm::vec3 cameraPosition;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	float cameraSpeed;
	float cameraSens;
	float actualSpeed = 0.0f;
	float fov;

	float currentFrame;
	float deltaTime;
	float lastFrame;

	float yaw;
	float pitch;

	float vel;

	bool jumpflag;
	bool releasedctrl;
	bool movingforward;
public:
	int flashlightOn;

	Camera();
	Camera(const float& speed, const float& sens, const float& f);

	void CameraUpdate(GLFWwindow* window, const float& deltaTime, Shader& shader);
	void MouseUpdate(double xoffset, double yoffset);
	void ScrollUpdate(const double& scroll);

	inline glm::mat4 getView() const { return viewMatrix; };
	inline glm::mat4 getProjection() const { return projectionMatrix; };
	inline glm::vec3 getCameraPos() const { return cameraPosition; }
	inline glm::vec3 getCameraFront() const { return cameraFront; }
};