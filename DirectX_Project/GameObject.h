#pragma once

#include "Vector3D.h"
#include "Bone.h"
#include "Skeleton.h"
#include "Animation.h"

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
	
	std::vector<Vector3Df> vertPositionsInit;
	std::vector<VertexData> vertexes;	
	std::vector<int> indexes;
	std::wstring texture = L"default.png";
	unsigned int textureId = 0;

	Skeleton skeleton;
	std::vector<Animation*> animations;
	std::vector<std::vector<Weight>> vertexWeights;

	int animationFrame;
	float animationSpeed;

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

	void animate();
};
