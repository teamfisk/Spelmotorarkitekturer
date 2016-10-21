#include "CameraEndlessRunner.h"
#include "OpenGL.h"


CameraEndlessRunner::CameraEndlessRunner()
	: Camera()
{
	up = glm::vec3(0.0, 1.0, 0.0);
	right = glm::vec3(0.0, 0.0, 1.0);
}


CameraEndlessRunner::~CameraEndlessRunner()
{
}

void CameraEndlessRunner::Update()
{
	// glfwGetTime is called only once, the first time this function is called	
	if (firstTime) {
		lastTime = glfwGetTime();
		firstTime = false;
	}

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	GLFWwindow* window = glfwGetCurrentContext();
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition -= right * deltaTime * speed;
	}

	float FoV = initialFoV - 1.0f * scrollWheelY;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = glm::perspective(FoV, aspectRatio, 0.1f, 150.0f);
	// Camera matrix

	viewMatrix = lookAt(
		cameraPosition,           // Camera is here
		cameraPosition + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void CameraEndlessRunner::SetDirection(glm::vec3 dir)
{
	this->direction = dir;
}
