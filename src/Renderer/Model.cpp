#include "Model.h"
#include <string>

template <class T>
void SetAttribPointer(GLuint attributeIndex, GLuint numElements, unsigned int long long sizeOfT)
{
	glEnableVertexAttribArray(attributeIndex);
	glVertexAttribPointer(attributeIndex, numElements, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeOfT));
}
/*
Öndskade att det här funkade ...
GLenum CreateVAO(RawModelAssimp::Vertex someStruct)
{
	GLenum vao;
	glGenVertexArrays(1, &vao);
	for each member in someStruct
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,offsetof(someeStruct.member));
	}
	return vao;
}
*/
Model::Model(const std::string& path)	
{	
	auto handle = ResourceManager::Load<RawModelAssimp>(path, 0);	
	this->handle = handle;
	auto model = *handle;
					
	if (model->m_Indices.size() == 0) // no indices?
	{
		throw std::logic_error("The method or operation is not implemented.");
	}
	else
	{	
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, model->GetVertexBytes(), model->m_Vertices.data(), GL_STATIC_DRAW);		
				
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		auto stride = sizeof(RawModelAssimp::Vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);

		offsetof(RawModelAssimp::Vertex, RawModelAssimp::Vertex::Position);

		auto offset = sizeof(RawModelAssimp::Vertex::Normal);
		glEnableVertexAttribArray(1);		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);

		offset += sizeof(RawModelAssimp::Vertex::Tangent);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);

		offset += sizeof(RawModelAssimp::Vertex::BiNormal);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);

		offset += sizeof(RawModelAssimp::Vertex::TextureCoords);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);

		offset += sizeof(RawModelAssimp::Vertex::BoneIndices);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);

		offset += sizeof(RawModelAssimp::Vertex::BoneWeights);		
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset);
		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->GetIndexBytes(), model->m_Indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Determine smallest index type possible during load.
	/*
	auto vertexCount = model->m_Vertices.size();
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
		throw std::runtime_error(s.c_str());
	}
	*/
	indexType = GL_UNSIGNED_INT;	
}

Model::~Model()
{
	// Clean up OpenGL Handles.
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &indexVBO);
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
	return (*handle)->m_Indices.data();	
}

GLsizei Model::GetIndicesCount() const
{
	return (*handle)->m_Indices.size();
}

GLsizei Model::GetTriangleCount() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

GLenum Model::GetIndexType() const
{
	return indexType;
}


