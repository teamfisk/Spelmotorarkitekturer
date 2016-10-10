#include "Model.h"
#include <string.h>

Model::Model(std::string& filename)
{
	void* vertexData = nullptr;
	void* indexData = nullptr;
	GLsizei vertexDataBytes;
	//LoadObjFile(filename, vertexData, vertexDataBytes, indexData, indexDataBytes);

	if (indexData == nullptr) // no indices?
	{
		throw std::logic_error("The method or operation is not implemented.");
	}
	else
	{	
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexDataBytes, vertexData, GL_STATIC_DRAW);
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(VertexFormat::pos)));		
		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataBytes, indexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
	
	// Determine smallest indextype possible during load.
	auto vertexCount = vertexDataBytes / sizeof(VertexFormat);
	if (vertexCount <= std::numeric_limits<unsigned char>::max())
	{
		indexType = GL_UNSIGNED_BYTE;
	}
	else if (vertexCount <= std::numeric_limits<unsigned short>::max())
	{
		indexType = GL_UNSIGNED_SHORT;
	}
	else if(vertexCount <= std::numeric_limits<unsigned int>::max())
	{
		indexType = GL_UNSIGNED_INT;
	}
	else
	{
		std::string s = "Too many vertices! Max vertexCount: ";
		s += std::numeric_limits<unsigned int>::max() + " vertexCount: " + vertexCount;
		throw std::exception(s.c_str());
	}	
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


