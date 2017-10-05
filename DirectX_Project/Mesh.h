#pragma once

#include <vector>
#include <d3dx9.h>

#include "IObjectComponent.h"

class DxVertex {
public:
	DxVertex() {}
	~DxVertex() {}

	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

class Mesh : public IObjectComponent
{
private:
	std::vector<DxVertex> vertexes;
	std::string textureFileName = "default.png";
	size_t textureId;
	
public:
	Mesh();
	~Mesh();

	void addVertex(D3DXVECTOR3 pos, D3DXVECTOR3 nor, D3DXVECTOR2 uv);
	void addVertex(DxVertex ver);
	void SetTexture(std::string fileName);
	void SetTextureId(size_t id);
	std::string GetTexture();
	size_t GetTextureId();
	
	size_t GetVertexesNum();
	size_t GetVertexSize();
	DxVertex* GetVertexes();
	std::vector<DxVertex>& GetVertexesRef();
};

