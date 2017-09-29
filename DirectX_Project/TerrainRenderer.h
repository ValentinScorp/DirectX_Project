#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

struct TERRAINVERTEX {
	FLOAT X, Y, Z;
	D3DVECTOR NORMAL;
	FLOAT    tu, tv;
};
#define TERRAINFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct TerrainVertexData {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

class TerrainRenderer
{
private:
	IDirect3DDevice9*	dxDevice = nullptr;
	LPDIRECT3DVERTEXBUFFER9 dxVertexBuffer = nullptr;

	int numVertexes = 0;
	IDirect3DTexture9* texture = nullptr;

public:
	TerrainRenderer(IDirect3DDevice9* dev);
	~TerrainRenderer();

	void Create(int w, int h, int tl);
	void Render();
	void Destroy();
};

