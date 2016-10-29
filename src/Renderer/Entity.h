#pragma once
#include "OpenGL.h"
#include "ResourceManager/ResourceHandle.h"
#include "Model.h"


class Entity
{
public:
	Entity(const glm::mat4x4& worldMatrix, const ResourceHandle<Model>& modelHandle, const ResourceHandle<Texture>& textureHandle);

	glm::mat4x4 worldMatrix;
	ResourceHandle<Model> modelHandle;
	ResourceHandle<Texture> textureHandle;
};

Entity::Entity(const glm::mat4x4& worldMatrix, const ResourceHandle<Model>& modelHandle, const ResourceHandle<Texture>& textureHandle)
{
	this->worldMatrix = worldMatrix;
	this->modelHandle = modelHandle;
	this->textureHandle = textureHandle;
}
