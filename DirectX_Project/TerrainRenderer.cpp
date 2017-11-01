#include "TerrainRenderer.h"
#include "UserInput.h"
#include <DxErr.h>
#include <time.h>

TerrainRenderer::TerrainRenderer(IDirect3DDevice9* dev)
{
	dxDevice = dev;
	srand(time(NULL));
}


TerrainRenderer::~TerrainRenderer()
{
}

void TerrainRenderer::Create(std::vector<TerrainPoint>& tpoints)
{
	for (int i = 0; i < (tpoints.size() / 6); i++) {
		randTiles[i] = rand() % 2;
	}
	std::vector<TerrainVertexData> terrainVertexes;
	for (auto tp : tpoints) {
		TerrainVertexData tv;
		tv.position = tp.position;
		tv.normal = tp.normal;
		tv.tcoord0 = tp.textureCo;
		tv.tcoord1 = tp.alphaCo;
		terrainVertexes.push_back(tv);
	}
	numVertexes = terrainVertexes.size();
	dxDevice->CreateVertexBuffer(numVertexes * sizeof(TerrainVertexData), NULL, TERRAINFVF, D3DPOOL_MANAGED, &dxVertexBuffer, NULL);
	VOID* pVoid;
	dxVertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, &terrainVertexes[0], sizeof(TerrainVertexData) * numVertexes);
	dxVertexBuffer->Unlock();
	D3DXCreateTextureFromFile(dxDevice, L"sand.png", &sandTex);
	D3DXCreateTextureFromFile(dxDevice, L"grass.png", &grassTex);
	D3DXCreateTextureFromFile(dxDevice, L"rock.png", &rockTex);
	D3DXCreateTextureFromFile(dxDevice, L"alphaSide.png", &alphaSide);
	D3DXCreateTextureFromFile(dxDevice, L"alphaCorner.png", &alphaCorner);
	D3DXCreateTextureFromFile(dxDevice, L"alphaCornerNew.png", &alphaCornerNew);
	D3DXCreateTextureFromFile(dxDevice, L"alphaDiag.png", &alphaDiag);

	HRESULT hr;
	ID3DXBuffer *pErrors = nullptr;
	hr = D3DXCreateBuffer(1024, &pErrors);

	hr = D3DXCreateEffectFromFile(dxDevice, L"TerrainShader2.fx", NULL, NULL, D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG, NULL, &terrainShader, &pErrors);	

	if (FAILED(hr)) {
		// Output shader compilation errors to the shell:
		CHAR* pErrorStr = (CHAR*)pErrors->GetBufferPointer();
		printf("%s\n", pErrorStr);
	}
	if (pErrors) {
		pErrors->Release();
		pErrors = nullptr;
	}
}

void TerrainRenderer::Render()
{
	D3DXMATRIX matTransform;
	D3DXMatrixIdentity(&matTransform);
	dxDevice->SetTransform(D3DTS_WORLD, &matTransform);
	dxDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	D3DXMatrixIdentity(&cameraMatrix);

	D3DXMATRIX matView = camera->GetTransformMatrix();
	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(camera->GetFovy()), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, camera->GetNearPlane(), camera->GetFarPlane());
	
	cameraMatrix = matView * matProjection;
	
	terrainShader->SetMatrix("g_mWorldViewProjection", &cameraMatrix);

	baseTexHandle = terrainShader->GetParameterByName(NULL, "g_TexBase");

		
	alphaHandle = terrainShader->GetParameterByName(NULL, "g_TexAlpha");
	layer1TexHandle = terrainShader->GetParameterByName(NULL, "g_TexLayer1");
	layer2TexHandle = terrainShader->GetParameterByName(NULL, "g_TexLayer2");
	layer3TexHandle = terrainShader->GetParameterByName(NULL, "g_TexLayer3");
	layer4TexHandle = terrainShader->GetParameterByName(NULL, "g_TexLayer4");
		
	dxDevice->SetFVF(TERRAINFVF);
	dxDevice->SetStreamSource(0, dxVertexBuffer, 0, sizeof(TerrainVertexData));

	terrainShader->SetTechnique("RenderTerrain");

	UINT passesNum = 0;
	terrainShader->Begin(&passesNum, 0);
		
	for (size_t i = 0; i < terrain->tiles.size(); i++) {		
		
		terrainShader->SetTexture(baseTexHandle, rockTex);
		terrainShader->SetTexture(layer1TexHandle, grassTex);
		terrainShader->SetTexture(layer2TexHandle, sandTex);
		terrainShader->SetTexture(layer3TexHandle, rockTex);
		terrainShader->SetTexture(layer4TexHandle, rockTex);

		if (randTiles[i] == 1)
			terrainShader->SetTexture(alphaHandle, alphaCorner);
		else
			terrainShader->SetTexture(alphaHandle, alphaCornerNew);
		
		for (UINT j = 0; j < passesNum; j++) {
			terrainShader->BeginPass(j);
			HRESULT hr = dxDevice->DrawPrimitive(D3DPT_TRIANGLELIST, i * 6, 2);	
			terrainShader->EndPass();
		}
	}

	terrainShader->End();
}

void TerrainRenderer::Destroy()
{	
	if (sandTex) {
		sandTex->Release();
		sandTex = nullptr;
	}
	if (grassTex) {
		grassTex->Release();
		grassTex = nullptr;
	}
	if (rockTex) {
		rockTex->Release();
		rockTex = nullptr;
	}
	if (alphaSide) {
		alphaSide->Release();
		alphaSide = nullptr;
	}
	if (alphaCorner) {
		alphaCorner->Release();
		alphaCorner = nullptr;
	}
	if (alphaFull) {
		alphaCorner->Release();
		alphaCorner = nullptr;
	}
	if (alphaDiag) {
		alphaDiag->Release();
		alphaDiag = nullptr;
	}
	for (auto t : textures) {
		if (t) {
			t->Release();
			t = nullptr;
		}
	}

	if (dxVertexBuffer) {
		dxVertexBuffer->Release();
		dxVertexBuffer = NULL;
	}

	if (terrainShader) {
		terrainShader->Release();
		terrainShader = nullptr;
	}
}

void TerrainRenderer::SetCamera(Camera * cam)
{
	camera = cam;
}

void TerrainRenderer::SetTerrain(Terrain * t)
{
	terrain = t;
}

size_t TerrainRenderer::CreateTexture(std::string textureFileName)
{
	size_t textureNum = textures.size();

	std::wstring textureFileNameW(textureFileName.begin(), textureFileName.end());

	IDirect3DTexture9* texturePtr = nullptr;
	D3DXCreateTextureFromFile(dxDevice, textureFileNameW.c_str(), &texturePtr);

	textures.push_back(texturePtr);

	return textureNum;
}
