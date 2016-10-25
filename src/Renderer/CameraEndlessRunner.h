#pragma once
#include "Renderer/Camera.h"

class CameraEndlessRunner
	: public Camera
{
public:
	CameraEndlessRunner();
	~CameraEndlessRunner();

	void Update() override;
	void SetDirection(glm::vec3 dir);
};

