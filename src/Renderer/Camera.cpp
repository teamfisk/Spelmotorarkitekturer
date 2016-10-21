#include "Camera.h"

using namespace glm;

Camera::Camera()
{
	cameraPosition = vec3(0.0f, 0.0f, 0.0f);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = 90.0f;
	speed = 3.0f;
	mouseSpeed = 0.005f;
	scrollWheelY = 0.0f;
	firstTime = true;
	cameraMouseControlEnabled = true;
	aspectRatio = 4.0f / 3.0f;
}

Camera::~Camera()
{

}

void Camera::Update(){
	// glfwGetTime is called only once, the first time this function is called	
	if (firstTime) {
		lastTime = glfwGetTime();
		firstTime = false;
	}

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	GLFWwindow * window = glfwGetCurrentContext();
	glfwGetCursorPos(window, &xpos, &ypos);
	if (cameraMouseControlEnabled) {
		// Reset mouse position for next frame
		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(windowWidth / 2 - xpos);
		verticalAngle += mouseSpeed * float(windowHeight / 2 - ypos);
	}
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	right = vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	up = cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		cameraPosition -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		cameraPosition += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		cameraPosition -= right * deltaTime * speed;
	}

	float FoV = initialFoV - 1.0f * scrollWheelY;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = perspective(FoV, aspectRatio, 0.1f, 150.0f);
	// Camera matrix

	viewMatrix = lookAt(
		cameraPosition,           // Camera is here
		cameraPosition + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}