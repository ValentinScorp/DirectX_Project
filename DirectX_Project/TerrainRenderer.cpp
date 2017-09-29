#include "TerrainRenderer.h"
#include "UserInput.h"
#include <DxErr.h>

TerrainRenderer::TerrainRenderer(IDirect3DDevice9* dev)
{
	dxDevice = dev;
}


TerrainRenderer::~TerrainRenderer()
{
}

void TerrainRenderer::Create(int w, int h, int tl)
{
	int numTiles = w * h;
	int numTri = numTiles * 2;
	numVertexes = numTri * 3;

	std::vector <TerrainVertexData> vertexes;

	/*
		^	B---C
		|	|   |
		y	A---D
			x-->
	*/

	TerrainVertexData tvdA, tvdB, tvdC, tvdD;
	tvdA.normal.x = 0;	tvdA.normal.y = 0; tvdA.normal.z = 1;
	tvdB.normal.x = 0;	tvdB.normal.y = 0; tvdB.normal.z = 1;
	tvdC.normal.x = 0;	tvdC.normal.y = 0; tvdC.normal.z = 1;
	tvdD.normal.x = 0;	tvdD.normal.y = 0; tvdD.normal.z = 1;

	tvdA.uv.x = 0; tvdA.uv.y = 1;			tvdB.uv.x = 1; tvdB.uv.y = 1;	
	tvdD.uv.x = 0; tvdD.uv.y = 0;			tvdC.uv.x = 1; tvdC.uv.y = 0;

	tvdA.position.z = 0;
	tvdB.position.z = 0;
	tvdC.position.z = 0;
	tvdD.position.z = 0;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			tvdA.position.x = j * tl;	
			tvdA.position.y = i * tl;

			tvdB.position.x = j * tl;	
			tvdB.position.y = i * tl + tl;

			tvdC.position.x = j * tl + tl;
			tvdC.position.y = i * tl + tl;

			tvdD.position.x = j * tl + tl;
			tvdD.position.y = i * tl;

			vertexes.push_back(tvdA);
			vertexes.push_back(tvdC);
			vertexes.push_back(tvdB);
			vertexes.push_back(tvdA);
			vertexes.push_back(tvdD);
			vertexes.push_back(tvdC);
		}		
	}

	dxDevice->CreateVertexBuffer(numVertexes * sizeof(TERRAINVERTEX), NULL, TERRAINFVF, D3DPOOL_MANAGED, &dxVertexBuffer, NULL);
	VOID* pVoid;
	dxVertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, &vertexes[0], sizeof(TERRAINVERTEX) * numVertexes);
	dxVertexBuffer->Unlock();

	D3DXCreateTextureFromFile(dxDevice, L"default.png", &texture);
}

void TerrainRenderer::Render()
{
	D3DXMATRIX matTransform;
	D3DXMatrixIdentity(&matTransform);
	dxDevice->SetTransform(D3DTS_WORLD, &matTransform);
	dxDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	dxDevice->SetTexture(0, texture);
	dxDevice->SetFVF(TERRAINFVF);
	dxDevice->SetStreamSource(0, dxVertexBuffer, 0, sizeof(TERRAINVERTEX));
	HRESULT hr = dxDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numVertexes / 3);

	if (hr != D3D_OK) {	
		//std::wstring errorString = DXGetErrorString(hr);
		//std::wstring errorDescr = DXGetErrorDescription(hr);
		int i = 0;
	}
}

void TerrainRenderer::Destroy()
{	
	if (texture) {
		texture->Release();
		texture = nullptr;
	}
	
	if (dxVertexBuffer) {
		dxVertexBuffer->Release();
		dxVertexBuffer = NULL;
	}
}
