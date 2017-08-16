#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct CUSTOMVERTEX {
	FLOAT X, Y, Z;
	D3DVECTOR NORMAL;
	FLOAT    tu, tv;
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)	

class Renderer
{
public:
	Renderer();

	void Initialize(HWND hWnd);	
	void InitializeLightAndMaterials();

	void AddVertexes(CUSTOMVERTEX *vertexes, int vertexesNumb);
	void AddIndexes(short *indexes, int indexesNumb);

	void CreateTexture(wchar_t *fileName);

	void BeginScene();
	void Draw();
	void EndScene();

	void Destroy();

	~Renderer();

private:
	IDirect3D9*			pDirect3D = nullptr;
	IDirect3DDevice9*	pDevice = nullptr;

	IDirect3DTexture9*	g_texture = nullptr;

	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;

	float index = 0.0f;
};

