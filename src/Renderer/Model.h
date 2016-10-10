#pragma once
#include <vector>
#include <GL\glew.h>
#include <string.h>

#include "Resource.h"
#include "VertexFormat.h"


class Model :
	public Resource
{
public:
	Model(std::string& filename);
	virtual ~Model();
	
	GLenum GetVAO() const;
	GLenum GetVBO() const;
	const void* GetIndices() const;
	GLsizei GetIndicesCount() const;
	GLsizei GetTriangleCount() const;
	GLenum GetIndexType() const;	
private:
	std::vector<VertexFormat> vertexData;
	void* indexData; // Jag stör mig allt mer på att man måste deklarera typ innan man vet vilken typ man har ... 
	GLenum vbo; // Vertex Buffer Object handle
	GLenum vao; // Vertex Array Object handle
	GLenum indexVBO;
	GLenum indexType;
	GLuint indexDataBytes;
};

