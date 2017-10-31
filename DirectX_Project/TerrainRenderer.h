#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "Camera.h"
#include "TerrainTile.h"
#include "Terrain.h"

#define TERRAINFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3)

struct TerrainVertexData {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tcoord0;	
	D3DXVECTOR2 tcoord1;
	D3DXVECTOR2 tcoord2;
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
	IDirect3DTexture9* sandTex = nullptr;
	IDirect3DTexture9* grassTex = nullptr;
	IDirect3DTexture9* rockTex = nullptr;
	
	IDirect3DTexture9* alphaSide = nullptr;
	IDirect3DTexture9* alphaCorner = nullptr;
	IDirect3DTexture9* alphaFull = nullptr;
	IDirect3DTexture9* alphaDiag = nullptr;

	std::vector<IDirect3DTexture9*> textures;

	std::vector<IDirect3DTexture9*> terrainTextures;
	std::vector<IDirect3DTexture9*> alphaTextures;

	std::vector <Triangle> triangles;
	Terrain *terrain = nullptr;

	ID3DXEffect* terrainShader = nullptr;
	Camera *camera = nullptr;
	D3DXMATRIX cameraMatrix;
	
	unsigned int texBIndex;
	unsigned int texFIndex;
	unsigned int texAIndex;
	unsigned int texArIndex;
public:
	TerrainRenderer(IDirect3DDevice9* dev);
	~TerrainRenderer();
		
	void Create(std::vector<TerrainPoint> &tpoints);
	void Render();
	void Destroy();
	void SetCamera(Camera *cam);	
	void SetTerrain(Terrain *t);

	size_t CreateTexture(std::string textureFileName);
};

