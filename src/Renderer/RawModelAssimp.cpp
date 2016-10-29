#include "RawModelAssimp.h"

RawModelAssimp::RawModelAssimp(std::shared_ptr<ResourceBundle::Block> block)
{
	Assimp::Importer importer;
    char* data = new char[block->Size()];

	block->Read(data);
	const aiScene* scene = importer.ReadFileFromMemory(data, block->Size(), aiProcess_CalcTangentSpace | aiProcess_Triangulate);
	delete[] data;

	if (scene == nullptr) {
		LOG_ERROR("Failed to load model \"%s\"", block->Path().c_str());
		LOG_ERROR("Assimp error: %s", importer.GetErrorString());
        throw std::runtime_error("Failed to open model file.");
	}

	auto m = scene->mRootNode->mTransformation;
	m_Matrix = glm::mat4(
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4
	);
	m_Matrix = glm::transpose(m_Matrix);

	auto meshes = scene->mMeshes;

	// Pre-count vertices
	int numVertices = 0;
	int numIndices = 0;
	for (int i = 0; i < scene->mNumMeshes; ++i) {
		numVertices += meshes[i]->mNumVertices;

		// Faces
		for (int j = 0; j < meshes[i]->mNumFaces; ++j) {
			auto face = meshes[i]->mFaces[j];
			numIndices += face.mNumIndices;
		}
	}
	//LOG_DEBUG("Vertex count %i", numVertices);
	//LOG_DEBUG("Index count %i", numIndices);

	//LOG_DEBUG("Model has %i embedded textures", scene->mNumTextures);
    
	std::vector<std::tuple<std::string, glm::mat4>> boneInfo;
	std::map<std::string, int> boneNameMapping;

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		auto mesh = meshes[i];
		auto material = scene->mMaterials[mesh->mMaterialIndex];
		unsigned int indexOffset = m_Vertices.size();

		// Vertices, normals and texture coordinates
		for (int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			Vertex desc;

			// Position
			auto position = mesh->mVertices[vertexIndex];
			desc.Position = glm::vec3(position.x, position.y, position.z);

			// Normal
			if(mesh->mNormals != nullptr)
			{
				auto normal = mesh->mNormals[vertexIndex];			 
				desc.Normal = glm::vec3(normal.x, normal.y, normal.z);
			}

			//if (mesh->HasTangentsAndBitangents()) {
			//	// Tangent
			//	auto tangent = mesh->mTangents[vertexIndex];
			//	desc.Tangent = glm::vec3(tangent.x, tangent.y, tangent.z);

			//	// Bi-tangent
			//	auto bitangent = mesh->mBitangents[vertexIndex];
			//	desc.BiTangent = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
			//}

			// UV
			if (mesh->HasTextureCoords(0)) {
				auto uv = mesh->mTextureCoords[0][vertexIndex];
				desc.TextureCoords = glm::vec2(uv.x, uv.y);
			}
            
			m_Vertices.push_back(desc);
		}

		// Faces
		for (int j = 0; j < mesh->mNumFaces; ++j) {
			auto face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; ++k) {
				unsigned int index = face.mIndices[k];
				m_Indices.push_back(indexOffset + index);
			}
		}

		// Calculate normal mapping tangents
		for (int i = 0; i < m_Indices.size(); i += 3) {
			Vertex& v0 = m_Vertices[m_Indices[i]];
			Vertex& v1 = m_Vertices[m_Indices[i + 1]];
			Vertex& v2 = m_Vertices[m_Indices[i + 2]];

			glm::vec3 edge1 = v1.Position - v0.Position;
			glm::vec3 edge2 = v2.Position - v0.Position;

			float deltaU1 = v1.TextureCoords.x - v0.TextureCoords.x;
			float deltaV1 = v1.TextureCoords.y - v0.TextureCoords.y;
			float deltaU2 = v2.TextureCoords.x - v0.TextureCoords.x;
			float deltaV2 = v2.TextureCoords.y - v0.TextureCoords.y;

			float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

			glm::vec3 tangent;
			tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
			tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
			tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

			v0.Tangent += tangent;
			v1.Tangent += tangent;
			v2.Tangent += tangent;
		}
		for (auto& vertex : m_Vertices) {
			vertex.Tangent = glm::normalize(vertex.Tangent);
			vertex.BiNormal = glm::normalize(glm::cross(vertex.Tangent, glm::normalize(vertex.Normal)));
		}

		// Material info
		MaterialGroup matGroup;
		matGroup.StartIndex = indexOffset;
		matGroup.EndIndex = m_Indices.size() - 1;
		// Material shininess
		material->Get(AI_MATKEY_SHININESS, matGroup.Shininess);
        material->Get(AI_MATKEY_OPACITY, matGroup.Transparency);
		//LOG_DEBUG("Shininess: %f", matGroup.Shininess);
		// Diffuse texture
		//LOG_DEBUG("%i diffuse textures found", material->GetTextureCount(aiTextureType_DIFFUSE));
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			aiTextureMapping mapping;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path, &mapping);
			matGroup.TexturePath = std::string(path.C_Str());
		}
		// Normal map
		//LOG_DEBUG("%i normal maps found", material->GetTextureCount(aiTextureType_HEIGHT));
		if (material->GetTextureCount(aiTextureType_HEIGHT)) {
			aiString path;
			aiTextureMapping mapping;
			material->GetTexture(aiTextureType_HEIGHT, 0, &path, &mapping);
            matGroup.NormalMapPath = std::string(path.C_Str());
		}
		// Specular map
		//LOG_DEBUG("%i specular maps found", material->GetTextureCount(aiTextureType_SPECULAR));
		if (material->GetTextureCount(aiTextureType_SPECULAR)) {
			aiString path;
			aiTextureMapping mapping;
			material->GetTexture(aiTextureType_SPECULAR, 0, &path, &mapping);
            matGroup.SpecularMapPath = std::string(path.C_Str());
		}
		MaterialGroups.push_back(matGroup);

		// Bones
		std::map<int, std::vector<std::tuple<int, float>>> vertexWeights;
		for (int j = 0; j < mesh->mNumBones; ++j) {
			auto bone = mesh->mBones[j];
			std::string boneName = bone->mName.C_Str();

			auto mat = bone->mOffsetMatrix;
			glm::mat4 glmMat(mat.a1, mat.b1, mat.c1, mat.d1,
				mat.a2, mat.b2, mat.c2, mat.d2,
				mat.a3, mat.b3, mat.c3, mat.d3,
				mat.a4, mat.b4, mat.c4, mat.d4);

			int boneIndex;
			if (boneNameMapping.find(boneName) != boneNameMapping.end()) {
				boneIndex = boneNameMapping[boneName];
			} else {
				boneIndex = boneInfo.size();
				boneInfo.push_back(std::make_tuple(boneName, glmMat));
				boneNameMapping[boneName] = boneIndex;
			}

			for (int k = 0; k < bone->mNumWeights; ++k) {
				auto weight = bone->mWeights[k];
				unsigned int offsetVertexId = weight.mVertexId + indexOffset;
				vertexWeights[offsetVertexId].push_back(std::make_tuple(boneIndex, weight.mWeight));
			}
		}
		for (auto &pair : vertexWeights) {
			auto weights = pair.second;
			Vertex& desc = m_Vertices[pair.first];

			const int maxWeights = 8;
			if (weights.size() > maxWeights) {
				LOG_WARNING("Vertex weights (%i) greater than max weights per vertex (%i)", weights.size(), maxWeights);
			}
			for (int weightIndex = 0; weightIndex < weights.size() && weightIndex < maxWeights && weightIndex < 4; ++weightIndex) {
				std::tie(desc.BoneIndices[weightIndex], desc.BoneWeights[weightIndex]) = weights[weightIndex];
			}
		}

		//break;
	}
}

RawModelAssimp::~RawModelAssimp()
{
}

size_t RawModelAssimp::GetMemoryUsage() const
{	
	size_t s = 0;
	s += MaterialGroups.size() * sizeof(MaterialGroup);
	s += m_Vertices.size() * sizeof(Vertex);
	s += m_Indices.size() * sizeof(unsigned int);
	s += BoneIndices.size() * sizeof(glm::ivec2);
	s += BoneWeights.size() * sizeof(glm::vec2);
	s += Normals.size() * sizeof(glm::vec3);
	s += TangentNormals.size() * sizeof(glm::vec3);
	s += BiTangentNormals.size() * sizeof(glm::vec3);
	s += TextureCoords.size() * sizeof(glm::vec2);
	return s;
}
