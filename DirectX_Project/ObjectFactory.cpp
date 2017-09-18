#include "ObjectFactory.h"

#include "Vector3D.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ObjectFactory::ObjectFactory(Renderer *rnd)
{
	if (rnd == nullptr) {
		throw - 1;
	}
	renderer = rnd;
}

ObjectFactory::~ObjectFactory()
{
}

GameObject * ObjectFactory::CreateCube()
{
	GameObject *go = new GameObject();

	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, },    // side 1
		{ 3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, },
		{ -3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, },    // side 2
		{ -3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, },
		{ 3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, },

		{ -3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, },    // side 3
		{ -3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, },    // side 4
		{ 3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, },
		{ -3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, },

		{ 3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, },    // side 5
		{ 3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, },
		{ 3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, },    // side 6
		{ -3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, },
		{ -3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
		{ -3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },
	};

	for (int i = 0; i < 24; i++) {
		VertexData vd;
		vd.position.x = vertices[i].X;
		vd.position.y = vertices[i].Y;
		vd.position.z = vertices[i].Z;
		vd.normal.x = vertices[i].NORMAL.x;
		vd.normal.y = vertices[i].NORMAL.y;
		vd.normal.z = vertices[i].NORMAL.z;
		vd.uv.x = vertices[i].tu;
		vd.uv.y = vertices[i].tv;

		go->AddVertex(vd);
	}
	
	//renderer->AddVertexes(vertices, 24);

	//renderer->AddVertexes((CUSTOMVERTEX*)go->GetVertexes(), 24);
		
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};
	
	for (int i = 0; i < 24; i++) {		
		go->AddIndex(indices[i]);
	}

	renderer->AddIndexes(go->GetIndexes(), 36);	

	return go;
}

GameObject * ObjectFactory::LoadObjFile(std::string fileName)
{
	GameObject *go = new GameObject();

	std::vector <std::string> fileLines;

	std::string line;
	std::ifstream objFile(fileName);
	if (objFile.is_open())
	{
		while (getline(objFile, line))
		{
			fileLines.push_back(line);			
		}
		objFile.close();
	}

	std::string objectName;
	std::vector<Vector3Df> positions;
	std::vector<Vector3Df> normals;
	std::vector<Vector2Df> uvs;

	int vertexCounter = 0;

	//std::vector < std::vector <std::string> > objData;
	for (int i = 0; i < fileLines.size(); i++) {
		std::stringstream ss;
		ss.str(fileLines[i]);
		std::string word;
		std::vector<std::string> words;
		while (std::getline(ss, word, ' ')) {
			words.push_back(word);			
		}
		if (words[0] == "o") {
			objectName = words[1];
		}
		if (words[0] == "v" && words.size() == 4) {			
			Vector3Df pos(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
			positions.push_back(pos);
		}
		if (words[0] == "vn" && words.size() == 4) {
			Vector3Df nor(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
			normals.push_back(nor);
		}
		if (words[0] == "vt" && words.size() == 3) {
			Vector2Df uv(std::stof(words[1]), 1 - std::stof(words[2]));
			uvs.push_back(uv);
		}
		if (words[0] == "f" && words.size() == 4) {
			VertexData vd;

			std::vector<int> numbers;

			for (int i = 1; i < 4; i++) {
				std::stringstream ss1;
				ss1.str(words[i]);
				std::string number;				
				while (std::getline(ss1, number, '/')) {	
					if (number == "") {
						numbers.push_back(1);
					} else {
						numbers.push_back(std::stoi(number) - 1);
					}					
				}
				if (numbers.size() == 3) {
					if (numbers[0] < positions.size()) {
						vd.position = positions[numbers[0]];
					}
					if (numbers[1] < uvs.size()) {
						vd.uv = uvs[numbers[1]];
					}
					if (numbers[2] < normals.size()) {
						vd.normal = normals[numbers[2]];
					}
					go->AddVertex(vd);
					go->AddIndex(vertexCounter);
					vertexCounter++;
				}
				numbers.clear();
			}			
		}
	}
		
	return go;
}

GameObject * ObjectFactory::LoadSmaFile(std::string fileName)
{
	GameObject *go = new GameObject();

	std::ifstream inputFile(fileName, std::ios::in | std::ios::binary);
	if (inputFile.fail()) {
		//error("Open file error\n");
	}

	// узнаем размер файла
	inputFile.seekg(0, std::ios::end);
	long fileSize = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);
	// читаем данные из файла
	unsigned char *data = new unsigned char[fileSize];
	inputFile.read((char*)data, fileSize);
	inputFile.close();
	unsigned char *data_iterator = data;

	char meshName[32] = " ";
	memcpy(meshName, data_iterator, sizeof(char) * 32);
	data_iterator += sizeof(char) * 32;
		
	// VERTISES
	unsigned short vertexTotal = *(unsigned short*)data_iterator;	
	data_iterator += sizeof(unsigned short);	
	std::vector<Vector3Df> vertexes;	
	if (vertexTotal) {	
		for (int i = 0; i < (vertexTotal / 3); i++) {

			float *x = (float*)data_iterator; data_iterator += sizeof(float);
			float *y = (float*)data_iterator; data_iterator += sizeof(float);
			float *z = (float*)data_iterator; data_iterator += sizeof(float);

			Vector3Df v(*x, *y, *z);
			
			vertexes.push_back(v);
		}
	}

	// NORMALS
	unsigned short normalsTotal = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	std::vector<Vector3Df> normals;
	if (normalsTotal) {
		for (int i = 0; i < (normalsTotal / 3); i++) {

			float *x = (float*)data_iterator; data_iterator += sizeof(float);
			float *y = (float*)data_iterator; data_iterator += sizeof(float);
			float *z = (float*)data_iterator; data_iterator += sizeof(float);

			Vector3Df n(*x, *y, *z);

			normals.push_back(n);
		}
	}

	unsigned short uvTotal = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	std::vector<Vector2Df> texcoords;
	if (uvTotal > 0 && uvTotal == (vertexTotal / 3) * 2) {
		for (int i = 0; i < (uvTotal / 2); i++) {

			float *s = (float*)data_iterator; data_iterator += sizeof(float);
			float *t = (float*)data_iterator; data_iterator += sizeof(float);			

			Vector2Df uv(*s, *t);
			texcoords.push_back(uv);
		}
	}

	data_iterator = nullptr;

	if (vertexTotal != normalsTotal) {
		delete go;
		return nullptr;
	}
	
	for (int i = 0; i < vertexes.size(); i++) {
		VertexData vd;

		vd.position.x = vertexes[i].x;
		vd.position.y = vertexes[i].z;
		vd.position.z = -vertexes[i].y;
		vd.normal.x = normals[i].x;
		vd.normal.y = normals[i].y;
		vd.normal.z = -normals[i].z;
		vd.uv = texcoords[i];

		go->AddVertex(vd);
		go->AddIndex(i);
	}

	return go;
}

GameObject * ObjectFactory::LoadSmaFile(std::string fileName)
{

}