#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "GameObject.h"
#include "UserInput.h"
#include "TerrainRenderer.h"
#include "Camera.h"
#include "MessageManager.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct CUSTOMVERTEX {
	FLOAT X, Y, Z;
	D3DVECTOR NORMAL;
	FLOAT    tu, tv;
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

class Renderer : public MessageReceiver
{
public:
	Renderer(Camera *cam);

	void Initialize(HWND hWnd);	
	void InitializeLightAndMaterials();

	void AddVertexes(CUSTOMVERTEX *vertexes, int vertexesNumb);
	void AddIndexes(int *indexes, int indexesNumb);
	void SetUserInput(UserInput *up);

	void SendData(std::vector<GameObject*> &objects);

	void CreateTexture(wchar_t *fileName);
	void AttachCamera(Camera *cam);

	void BeginScene();
	void Draw();
	void EndScene();

	TerrainRenderer* GetTerrainRenderer() {
		return terrainRenderer;
	}
	void SetCamera(Camera *cam);

	void Destroy();
	void OnMessage(Message mess);

	~Renderer();

private:
	ID3DXFont*			font;
	IDirect3D9*			pDirect3D = nullptr;
	IDirect3DDevice9*	pDevice = nullptr;

	UserInput*			userInput = nullptr;
	TerrainRenderer*	terrainRenderer = nullptr;
	//IDirect3DTexture9*	g_texture = nullptr;

	std::vector<IDirect3DTexture9*> textures;

	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;
	
	std::vector <GameObject*> *graph_objects = nullptr;
		
	Camera *camera;
	int oldX = 0;
	int oldY = 0;

	float index = 0.0f;
};

