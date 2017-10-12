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
	/*tvdA.normal.x = 0;	tvdA.normal.y = 0; tvdA.normal.z = 1;
	tvdB.normal.x = 0;	tvdB.normal.y = 0; tvdB.normal.z = 1;
	tvdC.normal.x = 0;	tvdC.normal.y = 0; tvdC.normal.z = 1;
	tvdD.normal.x = 0;	tvdD.normal.y = 0; tvdD.normal.z = 1;
	*/
	tvdA.uv0.x = 0; tvdA.uv0.y = 0;			
	tvdB.uv0.x = 0; tvdB.uv0.y = 1;	
	tvdC.uv0.x = 1; tvdC.uv0.y = 1;
	tvdD.uv0.x = 1; tvdD.uv0.y = 0;			

	//tvdA.uv1.x = 0; tvdA.uv1.y = 1;			tvdB.uv1.x = 1; tvdB.uv1.y = 1;
	//tvdD.uv1.x = 0; tvdD.uv1.y = 0;			tvdC.uv1.x = 1; tvdC.uv1.y = 0;

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

			Triangle triangle1, triangle2;
			D3DXVECTOR3 posA, posB, posC, posD;
			posA.x = tvdA.position.x;
			posA.y = tvdA.position.y;			
			posA.z = 0;

			posB.x = tvdB.position.x;
			posB.y = tvdB.position.y;
			posB.z = 0;

			posC.x = tvdC.position.x;
			posC.y = tvdC.position.y;
			posC.z = 0;

			posD.x = tvdD.position.x;
			posD.y = tvdD.position.y;
			posD.z = 0;

			triangle1.A = posA;
			triangle1.B = posB;
			triangle1.C = posC;

			triangle2.A = posA;
			triangle2.B = posC;
			triangle2.C = posD;

			triangles.push_back(triangle1);
			triangles.push_back(triangle2);
		}		
	}

	dxDevice->CreateVertexBuffer(numVertexes * sizeof(TERRAINVERTEX), NULL, TERRAINFVF, D3DPOOL_MANAGED, &dxVertexBuffer, NULL);
	VOID* pVoid;
	dxVertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, &vertexes[0], sizeof(TERRAINVERTEX) * numVertexes);
	dxVertexBuffer->Unlock();
	D3DXCreateTextureFromFile(dxDevice, L"sand.png", &sandTex);
	D3DXCreateTextureFromFile(dxDevice, L"grass.png", &grassTex);
	D3DXCreateTextureFromFile(dxDevice, L"rock.png", &rockTex);
	D3DXCreateTextureFromFile(dxDevice, L"alphaSide.png", &alphaSide);
	D3DXCreateTextureFromFile(dxDevice, L"alphaCorner.png", &alphaCorner);

	HRESULT hr;
	ID3DXBuffer *pErrors = nullptr;
	hr = D3DXCreateBuffer(1024, &pErrors);

	hr = D3DXCreateEffectFromFile(dxDevice, L"TerrainShader.fx", NULL, NULL, D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG, NULL, &terrainShader, &pErrors);

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
	terrainShader->SetTexture("g_Texture1", sandTex);
	terrainShader->SetTexture("g_Texture2", grassTex);
	terrainShader->SetTexture("g_Texture3", rockTex);
	terrainShader->SetTexture("g_AlphaCorner", alphaCorner);
	terrainShader->SetTexture("g_AlphaSide", alphaSide);

	texBIndex = 0;
	texFIndex = 1;
	texAIndex = 1;
	texArIndex = 3;

	terrainShader->SetInt("g_TexBackIndex", texBIndex);
	terrainShader->SetInt("g_TexFrontIndex", texFIndex);
	terrainShader->SetInt("g_AlghaIndex", texAIndex);
	terrainShader->SetInt("g_AlghaRotationIndex", texArIndex);


	dxDevice->SetFVF(TERRAINFVF);
	dxDevice->SetStreamSource(0, dxVertexBuffer, 0, sizeof(TERRAINVERTEX));

	terrainShader->SetTechnique("RenderTerrain");

	UINT passesNum = 0;
	terrainShader->Begin(&passesNum, 0);
	for (UINT i = 0; i < passesNum; i++) {
		terrainShader->BeginPass(i);
		HRESULT hr = dxDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numVertexes / 3);
		terrainShader->EndPass();
	}
	terrainShader->End();	
}

D3DXVECTOR3 TerrainRenderer::GetTerraneIntersection(RayVector rv)
{
	D3DXVECTOR3 intersection = {0.0f, 0.0f, 0.0f};

	for (int i = 0; i < triangles.size(); i++) {
		if (IntersectRayTriangle(rv, triangles[i], intersection)) {
			return intersection;
		}
	}

	intersection.x = 0;
	intersection.y = 0;
	intersection.z = 0;
	return intersection;
}

bool TerrainRenderer::IntersectRayTriangle(RayVector ray, Triangle triangle, D3DXVECTOR3 &intersectionVertex)
{
	// находим вектора сторон треугольника
	D3DXVECTOR3 u = triangle.B - triangle.A;
	D3DXVECTOR3 v = triangle.C - triangle.A;

	// находим нормаль к треугольнику
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);

	if (n.x == 0 && n.y == 0 && n.z == 0) {
		return 0;                       // неверные параметры треугольника (либо точки на одной прямой, либо все в одной точке)
	}

	// вектор направления луча
	D3DXVECTOR3 dir = ray.end - ray.begin;
	// векор от точки на треугольнике до начала луча
	D3DXVECTOR3 w0 = ray.begin - triangle.A;

	float a = -D3DXVec3Dot(&n, &w0);
	float b = D3DXVec3Dot(&n, &dir);

	if (fabs(b) < 0.0001) {       // луч паралельный плоскости треугольника
		if (a == 0) { return 0; }   // луч лежит на плоскости треугольника
		else { return 0; }   // луч не на плоскости треугольника

	}
	// найдем точку пересечения луча с треугольником
	float r = a / b;
	if (r < 0.0) {
		return 0;                    // луч идет мимо треугольника
	}
	// для сегмента проверить также (r > 1.0) => нет пересечения

	intersectionVertex = ray.begin + dir * r;           // точка пересечения луча и плоскости
														// лежит ли точка в треугольнике
	float    uu, uv, vv, wu, wv, D;
	uu = D3DXVec3Dot(&u, &u);
	uv = D3DXVec3Dot(&u, &v);
	vv = D3DXVec3Dot(&v, &v);
	D3DXVECTOR3 w = intersectionVertex - triangle.A;
	wu = D3DXVec3Dot(&w, &u);
	wv = D3DXVec3Dot(&w, &v);
	D = uv * uv - uu * vv;

	// найдем и проверим параметрические координаты
	float s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0) {
		return 0;                   // точка вне треугольника
	}
	float t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0) {
		return 0;                   // точка вне треугольника
	}

	return 1;
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
