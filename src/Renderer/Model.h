#pragma once
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <GL/glew.h>
#include "ResourceManager/Resource.h"
#include "VertexFormat.h"
#include "RawModelAssimp.h"
#include "ResourceManager/ResourceHandle.h"

// Stores OpenGL buffer handles. Used with Render.Render(Model* model);
// Uses an RawModelAssimp to load and store the mesh data.

class Model :
	public Resource
{
	friend class ResourceManager;
public:
	Model(std::shared_ptr<ResourceBundle::Block> block);
	virtual ~Model();
	
	GLenum GetVAO() const;
	GLenum GetVBO() const;	
	GLsizei GetIndicesCount() const;
	GLsizei GetTriangleCount() const;
	GLenum GetIndexType() const;	
	size_t GetMemoryUsage() const;
	GLenum GetIndexBuffer();
private:
	GLenum vbo; // Vertex Buffer Object handle
	GLenum vao; // Vertex Array Object handle
	GLenum indexVBO;
	GLenum indexType;
	GLuint indexDataBytes;
	size_t m_IndexCount;
};

