#include "OBJ.h"
#include <fstream>
#include <iostream>

using namespace std;

/*void Mesh::LoadFromObjFile(std::string dir, std::string filename)
{
	vector<vec3> tempPoints;
	vector<vec2> tempTexCoords;
	vector<vec3> tempNormals;
	vector<MeshMaterialData> tempMaterialData;

	int lastGroupIteration = 0;
	int iterationCount = 0;
	bool ignorePreviousGroup = false;

	string filePath = dir + filename;
	ifstream f(filePath);
	if (!f)
	{
		cout << "Can't open file: " << filePath << endl;
		return;
	}

	string str;
	while (!f.eof())
	{
		f >> str;
		if (str == "#")
		{
			f.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if (str == "v")
		{
			vec3 u;
			f >> u[0];
			f >> u[1];
			f >> u[2];
			tempPoints.push_back(u);
		}
		else if (str == "vt")
		{
			vec2 u;
			f >> u[0];
			f >> u[1];
			tempTexCoords.push_back(u);
		}
		else if (str == "vn")
		{
			vec3 n;
			f >> n[0];
			f >> n[1];
			f >> n[2];
			tempNormals.push_back(n);
		}
		else if (str == "f")
		{
			if (lastGroupIteration + 1 == iterationCount)
			{
				ignorePreviousGroup = false;
			}

			unsigned int indices[9];
			for (int i = 0; i < 9; i++)
			{
				f >> indices[i];
				indices[i] -= 1;
				f.ignore();
			}

			for (int i = 0; i < 3; i++)
			{
				int index = i * 3;
				unsigned int pointIndex = indices[index];
				unsigned int texCoordIndex = indices[index + 1];
				unsigned int normalIndex = indices[index + 2];
				VertexData vert;
				vert.point = tempPoints[pointIndex];
				vert.texCoord = tempTexCoords[texCoordIndex];
				vert.normal = tempNormals[normalIndex];
				groups.back().vertices.push_back(vert);
			}
		}
		else if (str == "s")
		{
			f.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if (str == "mtllib")
		{
			if (lastGroupIteration + 1 == iterationCount)
			{
				ignorePreviousGroup = false;
			}
			string s;
			f >> s;
			LoadMaterialData(dir + s, tempMaterialData);
		}
		else if (str == "usemtl")
		{
			if (lastGroupIteration + 1 == iterationCount)
			{
				ignorePreviousGroup = false;
			}
			string mtlname;
			f >> mtlname;
			for (auto it = tempMaterialData.begin(); it != tempMaterialData.end(); it++)
			{
				if (it->Name == mtlname)
				{
					if (groups.size() != 0) {
						groups.back().materialData = *it;
					}
					else {
						VertexGroup g;
						g.name = "default";
						g.materialData = *it;
						groups.push_back(g);
					}
					break;
				}
			}
		}
		else if (str == "g")
		{
			lastGroupIteration = iterationCount;
			if (ignorePreviousGroup)
			{
				f >> groups.back().name;
			}
			else
			{
				VertexGroup g;
				f >> g.name;
				groups.push_back(g);
				ignorePreviousGroup = true;
			}
		}
		else
		{
			string s;
			std::getline(f, s);
			cout << str << s << endl;
		}
		iterationCount++;
	}
}

void Mesh::LoadMaterialData(std::string filepath, vector<MeshMaterialData> & data)
{
	ifstream f(filepath);
	if (!f)
	{
		cout << "Can't open file: " << filepath << endl;
		return;
	}

	string str;
	while (!f.eof())
	{
		f >> str;
		if (str == "newmtl")
		{
			MeshMaterialData mtl;
			f >> mtl.Name;
			data.push_back(mtl);
		}
		else if (str == "illum")
		{
			f >> data.back().Illum;
		}
		else if (str == "Kd")
		{
			f >> data.back().Kd[0];
			f >> data.back().Kd[1];
			f >> data.back().Kd[2];
		}
		else if (str == "Ka")
		{
			f >> data.back().Ka[0];
			f >> data.back().Ka[1];
			f >> data.back().Ka[2];
		}
		else if (str == "Tf")
		{
			f >> data.back().Tf[0];
			f >> data.back().Tf[1];
			f >> data.back().Tf[2];
		}
		else if (str == "map_Kd")
		{
			f >> data.back().map_Kd;
		}
		else if (str == "Ni")
		{
			f >> data.back().Ni;
		}
		else
		{
			string s;
			std::getline(f, s);
			cout << str << s << endl;
		}
	}
}
*/
void LoadObjFile(std::string& dir, std::string& fileName, void* vertexData, std::size_t& vertexDataBytes, void* indexData, std::size_t& indexDataBytes)
{
	LoadObjFile(dir.c_str(), fileName.c_str(), vertexData, vertexDataBytes, indexData, indexDataBytes);
}

void LoadObjFile(const char* dir, const char* fileName, void* vertexData, std::size_t& vertexDataBytes, void * indexData, std::size_t& indexDataBytes)
{
	string filePath(dir);
	filePath += fileName;
	
	ifstream f(filePath);

	if (!f)
	{
		string s = "Can't open file: " + filePath;
		throw std::runtime_error(s);
	}

	char c;
	while (!f.eof())
	{
		f >> c;
		switch (c)
		{
		case '#':
			f.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case 'v':
			f >> c;

			switch (c)
			{
			case 't': // 'vt'
				break;
			case 'n': // 'vn'
				break;
			}			
			
			break;			
		case 'f':

			break;
		case 's':

			break;
		case 'g':

			break;

		/*case 'mtllib':

			break;
		case 'usemtl':

			break;*/
		}
	}
}
