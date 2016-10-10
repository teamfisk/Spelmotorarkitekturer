#pragma once

#include <string>
#include <vector>
#include "VertexFormat.h"

void LoadObjFile(std::string& dir, std::string& fileName, void* vertexData, std::size_t& vertexDataBytes, void* indexData, std::size_t& indexDataBytes);
void LoadObjFile(const char* dir, const char* fileName, void* vertexData, std::size_t& vertexDataBytes, void* indexData, std::size_t& indexDataBytes);

struct MeshMaterialData
{
	std::string name;
	unsigned int illum;
	float	Kd[3];
	float	Ka[3];
	float	Tf[3];
	std::string	map_Kd;
	std::string bump;
	std::string disp;
	std::string occlusion;
	float	Ni;
};

struct VertexGroup
{
	std::string name;
	std::vector<VertexFormat> vertices;
	MeshMaterialData materialData;
};
/*
class Mesh
{

	//GLuint bufferHandle[1];
	//GLuint VAOHandle[1];
	//bool buffersInitialized = false;

	std::vector<VertexGroup> groups;

	void InitBuffers();

public:
	//~Mesh();

	void Draw();

	void LoadMaterialData(std::string filepath, std::vector<MeshMaterialData> & data);
	void LoadFromObjFile(std::string dir, std::string filename);

	MeshMaterialData GetGroupMaterialData(int groupIndex) { return groups[groupIndex].materialData; };
	int GetVertexCount(int groupIndex) { return groups[groupIndex].vertices.size(); };
	int GetVertexDataSize(int groupIndex) { return groups[groupIndex].vertices.size() * sizeof(VertexData); };
	const VertexData * GetVertexData(int groupIndex) { return groups[groupIndex].vertices.data(); };
}; 
*/