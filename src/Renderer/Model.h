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
public:
	Model(std::string& path);
	virtual ~Model();
	
	GLenum GetVAO() const;
	GLenum GetVBO() const;
	const void* GetIndices() const;
	GLsizei GetIndicesCount() const;
	GLsizei GetTriangleCount() const;
	GLenum GetIndexType() const;	
private:
	std::vector<VertexFormat> vertexData;
	void* indexData; // Jag st�r mig allt mer p� att man m�ste deklarera typ innan man vet vilken typ man har ... 
	GLenum vbo; // Vertex Buffer Object handle
	GLenum vao; // Vertex Array Object handle
	GLenum indexVBO;
	GLenum indexType;
	GLuint indexDataBytes;

	// Store as an object? Can't since there is no default constructor, and need to assign resource after construction.
	//ResourceHandle<RawModelAssimp>* rawModelAssimpHandle;
};

