#pragma once

#include <vector>
#include <d3dx9.h>

#include "Vector3D.h"

class DxVertex {
public:
	DxVertex() {}
	~DxVertex() {}

	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

class Mesh
{
private:
	std::vector<DxVertex> vertexes;
	
public:
	Mesh();
	~Mesh();

	void addVertex(D3DXVECTOR3 pos, D3DXVECTOR3 nor, D3DXVECTOR2 uv);
	size_t GetVertexesNum();
	size_t GetVertexSize();
	DxVertex* GetVertexes();
	//void addPosition(D3DXVECTOR3 pos);
	//void addNormal(D3DXVECTOR3 nor);
	//void addUv(D3DXVECTOR2 uv);
};

