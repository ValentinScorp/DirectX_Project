#include "SmaLoader.h"
#include <codecvt>

SmaLoader::SmaLoader()
{
}

GameObject* SmaLoader::load(std::string file)
{
	GameObject *go = new GameObject();
	std::ifstream inputFile(file, std::ios::in | std::ios::binary);
	if (inputFile.fail()) {
		delete go;
		return nullptr;
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

	// file id
	char fileFormat[8] = "";
	char fileFormatSma[8] = "SMA";
	memcpy(fileFormat, data_iterator, sizeof(char) * 8);
	if (strcmp(fileFormat, fileFormatSma) != 0) {
		delete[] data;
		delete go;
		return nullptr;
	}
	data_iterator += sizeof(char) * 8;
	
	// version
	unsigned short version = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);

	// object name
	char meshName[32] = " ";
	memcpy(meshName, data_iterator, sizeof(char) * 32);
	data_iterator += sizeof(char) * 32;

	// vertexes
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

	// normals
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
	// texture coordinates
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

	// textures
	unsigned short numTextures = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);

	for (int i = 0; i < numTextures; i++) {
		char textureName[64] = "default.png";
		memcpy(textureName, data_iterator, sizeof(char) * 64);
		data_iterator += sizeof(char) * 64;
		
		std::string texName(textureName);
		//setup converter
		typedef std::codecvt_utf8<wchar_t> convert_type;
		std::wstring_convert<convert_type, wchar_t> converter;

		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		go->texture = converter.from_bytes(texName);
				
		unsigned short numTriangleIndexes = *(unsigned short*)data_iterator;
		data_iterator += sizeof(unsigned short);

		for (int j = 0; j < numTriangleIndexes; j++) {
			unsigned short index = *(unsigned short*)data_iterator;
			// todo save indexes
			data_iterator += sizeof(unsigned short);
		}		
	}
	

	delete[] data;
	data_iterator = nullptr;

	if (vertexTotal != normalsTotal) {
		delete go;
		return nullptr;
	}

	for (int i = 0; i < vertexes.size(); i++) {
		VertexData vd;

		vd.position.x = vertexes[i].x;
		vd.position.y = vertexes[i].y;
		vd.position.z = vertexes[i].z;
		vd.normal.x = normals[i].x;
		vd.normal.y = normals[i].y;
		vd.normal.z = normals[i].z;
		vd.uv = texcoords[i];

		go->AddVertex(vd);
		go->AddIndex(i);
	}

	return go;
}


SmaLoader::~SmaLoader()
{
}
