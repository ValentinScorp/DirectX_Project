#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "Camera.h"

struct TERRAINVERTEX {
	FLOAT X, Y, Z;
	FLOAT    tu0, tv0;	
};
#define TERRAINFVF (D3DFVF_XYZ | D3DFVF_TEX1)

struct TerrainVertexData {
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv0;	
};

class Triangle {
public:
	D3DXVECTOR3 A;
	D3DXVECTOR3 B;
	D3DXVECTOR3 C;
};

class TerrainRenderer
{
private:
	IDirect3DDevice9*	dxDevice = nullptr;
	LPDIRECT3DVERTEXBUFFER9 dxVertexBuffer = nullptr;

	int numVertexes = 0;
	IDirect3DTexture9* mudTex = nullptr;
	IDirect3DTexture9* grassTex = nullptr;

	std::vector <Triangle> triangles;

	ID3DXEffect* terrainShader = nullptr;
	Camera *camera = nullptr;
	D3DXMATRIX cameraMatrix;

public:
	TerrainRenderer(IDirect3DDevice9* dev);
	~TerrainRenderer();

	void Create(int w, int h, int tl);
	void Render();
	void Destroy();
	void SetCamera(Camera *cam);

	D3DXVECTOR3 GetTerraneIntersection(RayVector rv);
	bool IntersectRayTriangle(RayVector ray, Triangle triangle, D3DXVECTOR3 &intersectionVertex);
};

