#include "Model.h"
#include <string>

template <class T>
void SetAttribPointer(GLuint attributeIndex, GLuint numElements, unsigned int long long sizeOfT)
{
	glEnableVertexAttribArray(attributeIndex);
	glVertexAttribPointer(attributeIndex, numElements, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeOfT));
}
/*
�ndskade att det h�r funkade ...
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
Model::Model(std::shared_ptr<ResourceBundle::Block> block)
{
	m_RawModel = ResourceManager::Load<RawModelAssimp>(block->Path());	
	m_IndexCount = m_RawModel->m_Indices.size();
					
	if (m_RawModel->m_Indices.size() == 0) {
		throw std::logic_error("The method or operation is not implemented.");
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

void Model::Finalize()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_RawModel->GetVertexBytes(), m_RawModel->m_Vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	using Vertex = RawModelAssimp::Vertex;
	auto stride = sizeof(Vertex);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(Vertex, Vertex::Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(Vertex, Vertex::Tangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(Vertex, Vertex::BiNormal));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(Vertex, Vertex::TextureCoords));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(Vertex, Vertex::BoneIndices));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offsetof(Vertex, Vertex::BoneWeights));
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_RawModel->GetIndexBytes(), m_RawModel->m_Indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_RawModel.Release();
}

GLenum Model::GetVAO() const
{
	return vao;
}

GLenum Model::GetVBO() const
{
	return vbo;
}

GLsizei Model::GetIndicesCount() const
{
	return m_IndexCount;
}

GLsizei Model::GetTriangleCount() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

GLenum Model::GetIndexType() const
{
	return indexType;
}

size_t Model::GetMemoryUsage() const
{	
	return sizeof(Model);
}

GLenum Model::GetIndexBuffer()
{
	return indexVBO;
}

