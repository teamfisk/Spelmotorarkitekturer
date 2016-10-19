#pragma once

class InstanceData;
#include "Model.h"
#include <vector>
#include <tuple>

class Renderer
{
public:
	Renderer();
	~Renderer();

//	void AddToRender(Model * model, InstanceData* instancedata);
	void Render(Model * model, const glm::mat4x4& worldMat, GLuint programHandle);
private:
	std::vector<std::tuple<Model*, InstanceData*>> renderList;
};

