#pragma once
#include "OpenGL.h"
#include "ResourceManager\ResourceHandle.h"
#include "Model.h"


class Entity
{
public:
	Entity(const glm::mat4x4& worldMatrix, const ResourceHandle<Model>& modelHandle);

	glm::mat4x4 worldMatrix;
	ResourceHandle<Model> modelHandle;
};

Entity::Entity(const glm::mat4x4& worldMatrix, const  ResourceHandle<Model>& modelHandle)
{
	this->worldMatrix = worldMatrix;
	this->modelHandle = modelHandle;
}
