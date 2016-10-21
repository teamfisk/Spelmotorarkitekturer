#pragma once
#include "OpenGL.h"
#include "GLM.h"

class Camera
{
protected:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 cameraPosition;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;
	float scrollWheelY;
	bool firstTime;
	double lastTime;

public:
	bool cameraMouseControlEnabled;
	float aspectRatio;

	Camera();
	~Camera();

	virtual void Update();

	glm::mat4 getViewMatrix()		{ return viewMatrix; }
	glm::mat4 getProjectionMatrix() { return projectionMatrix;}

	glm::vec3 getPosition()	 { return cameraPosition;}
	glm::vec3 getDirection() { return direction; }
	glm::vec3 getRight()	 { return right; }
	glm::vec3 getUp()		 { return up; }
	float getVerticalAngle() { return verticalAngle; }
	float getHorizontalAngle() { return horizontalAngle; }

	void setPosition(float x, float y, float z) { cameraPosition = glm::vec3(x, y, z); }
	void setPosition(glm::vec3 pos) { cameraPosition = pos; }
	void setVerticalAngle(float angle) { verticalAngle = angle; }
	void setHorizontalAngle(float angle) { horizontalAngle= angle; }
};
