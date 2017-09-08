#pragma once

#include "Vector3D.h"
#include <vector>

struct VertexData {
	Vector3Df position;
	Vector3Df normal;
	Vector2Df uv;
};

class GameObject
{
public:
	Vector3Dd position;
	Vector3Dd rotation;
	
	std::vector<VertexData> vertexes;	
	std::vector<int> indexes;
	std::wstring texture = L"default.png";
	unsigned int textureId = 0;

public:
	GameObject();
	~GameObject();

	void AddVertex(VertexData vd);
	void AddIndex(int i);

	int* GetIndexes() {
		return &indexes[0];
	}

	VertexData* GetVertexes() {
		return &vertexes[0];
	}

	unsigned int GetIndexNum() {
		return indexes.size();
	}
	unsigned int GetVertexNum() {
		return vertexes.size();
	}

		
};
