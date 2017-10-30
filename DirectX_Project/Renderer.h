#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "GameObject.h"
#include "UserInput.h"
#include "TerrainRenderer.h"
#include "Camera.h"
#include "MessageManager.h"
#include "TerrainBrush.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct CUSTOMVERTEX {
	FLOAT X, Y, Z;
	D3DVECTOR NORMAL;
	FLOAT    tu, tv;
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

class RenderObject {
public:
	RenderObject():
		mesh(nullptr), rbody(nullptr)
	{
	}
	~RenderObject() {}
		
	Mesh* mesh;
	RigidBody* rbody;
};

class Renderer : public MessageReceiver
{
public:
	Renderer(Camera *cam);

	void Initialize(HWND hWnd);	
	void InitializeLightAndMaterials();

	void AddTerrainBrush(TerrainBrush *tb);
	void AddVertexes(CUSTOMVERTEX *vertexes, int vertexesNumb);
	void AddIndexes(int *indexes, int indexesNumb);
	void SetUserInput(UserInput *up);
		
	void AttachMesh(Mesh *m);
	void AllocateVideoMemory();	

	size_t CreateTexture(std::wstring fileName);
	void AttachCamera(Camera *cam);

	void AddGameObject(GameObject *go);

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
	
	std::vector<IDirect3DTexture9*> textures;

	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;
	
	LPDIRECT3DVERTEXBUFFER9 terrBrushVBuffer = NULL;

	std::vector <GameObject*> *graph_objects = nullptr;
	std::list <Mesh*> meshes;
	std::list <RenderObject*> robjects;
		
	Camera *camera;
	int oldX = 0;
	int oldY = 0;
	TerrainBrush *terrainBrush = nullptr;
	float index = 0.0f;
};

