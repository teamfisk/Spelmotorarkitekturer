#include "Model.h"

Model::Model(std::string& filename)
{
	// Determine smallest indextype possible during load.
	//indexType = GL_UNSIGNED_SHORT
}

Model::~Model()
{

}

GLenum Model::GetVAO() const
{
	return vao;
}

GLenum Model::GetVBO() const
{
	return vbo;
}

const void* Model::GetIndices() const
{
	return indexData;
}

GLsizei Model::GetIndicesCount() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

GLsizei Model::GetTriangleCount() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

GLenum Model::GetIndexType() const
{
	return indexType;
}


