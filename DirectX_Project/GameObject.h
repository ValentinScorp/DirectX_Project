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
	
	std::vector<VertexData> vertexes;	
	std::vector<int> indexes;

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
