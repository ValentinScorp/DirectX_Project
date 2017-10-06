#include "Mesh.h"

Mesh::Mesh():
	IObjectComponent("mesh")
{	
}


Mesh::~Mesh()
{
}

void Mesh::addVertex(D3DXVECTOR3 pos, D3DXVECTOR3 nor, D3DXVECTOR2 uv)
{
	DxVertex v;
	v.position = pos;
	v.normal = nor;
	v.uv = uv;
	vertexes.push_back(v);
}

void Mesh::addVertex(DxVertex ver)
{
	vertexes.push_back(ver);
}

void Mesh::UpdateAnimation(float dt)
{
}

void Mesh::SetTexture(std::string fileName)
{
	// "x.sma"
	if (fileName.length() > 4) {
		textureFileName = fileName;
	}
}

void Mesh::SetTextureId(size_t id)
{
	textureId = id;
}

std::string Mesh::GetTexture()
{
	return textureFileName;
}

size_t Mesh::GetTextureId()
{
	return textureId;
}

size_t Mesh::GetVertexesNum()
{
	return vertexes.size();
}

size_t Mesh::GetVertexSize()
{
	return sizeof(DxVertex);
}

DxVertex* Mesh::GetVertexes()
{
	return &vertexes[0];
}

/*
void Mesh::addPosition(D3DXVECTOR3 pos)
{
	positions.push_back(pos);
}

void Mesh::addNormal(D3DXVECTOR3 nor)
{
	normals.push_back(nor);
}

void Mesh::addUv(D3DXVECTOR2 uv)
{
	uvs.push_back(uv);
}
*/