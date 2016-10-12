#ifndef RawModelAssimp_h__
#define RawModelAssimp_h__

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stack>
#include <vector>
#include <memory>
#include <map>

#include "ResourceManager/Resource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GLM.h"
#include "ResourceManager/ResourceManager.h"
#include "Texture.h"
#include "GLM.h"

class RawModelAssimp : public Resource
{
	friend class ResourceManager;

protected:
	RawModelAssimp(const std::string& fileName);

public:
	~RawModelAssimp();

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 BiNormal;
		glm::vec2 TextureCoords;
		glm::vec4 BoneIndices;
		glm::vec4 BoneWeights;
	};

	struct MaterialGroup
	{
		float Shininess;
		float Transparency;
		std::string TexturePath;
		std::shared_ptr<::Texture> Texture;
		std::string NormalMapPath;
		std::shared_ptr<::Texture> NormalMap;
		std::string SpecularMapPath;
		std::shared_ptr<::Texture> SpecularMap;
		unsigned int StartIndex;
		unsigned int EndIndex;
	};

	std::vector<MaterialGroup> MaterialGroups;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	glm::mat4 m_Matrix;

	size_t GetVertexBytes() { return m_Vertices.size() * sizeof(Vertex); }
	size_t GetIndexBytes() { return m_Vertices.size() * sizeof(unsigned int); }
private:
	std::vector<glm::ivec2> BoneIndices;
	std::vector<glm::vec2> BoneWeights;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec3> TangentNormals;
	std::vector<glm::vec3> BiTangentNormals;
	std::vector<glm::vec2> TextureCoords;
};

#endif
