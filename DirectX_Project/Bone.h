#pragma once

#include "Vector3D.h"

#include <d3dx9.h>

class Bone
{
public:
	Bone();
	~Bone();

	D3DXMATRIX GetLocalToWorldMatrix();

	int parentIndex;
	Bone *parent;
	Vector3Df position;
	Vector3Df rotation;
	int index;
};

class Weight
{
public:
	Weight() 
	{
		bone = nullptr;
		weight = 0;
	}
	~Weight() {}

	Bone *bone;
	float weight;
};
