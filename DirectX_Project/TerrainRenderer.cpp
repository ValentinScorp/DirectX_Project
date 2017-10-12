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

void TerrainRenderer::Create(std::vector<TerrainPoint>& tpoints)
{
	std::vector<TerrainVertexData> terrainVertexes;
	for (auto tp : tpoints) {
		TerrainVertexData tv;
		tv.position = tp.position;
		tv.normal = tp.normal;
		tv.tcoord0 = tp.textureCo;
		tv.tcoord1 = tp.textureCo;
		tv.tcoord2 = tp.alphaCo;
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
	dxDevice->SetStreamSource(0, dxVertexBuffer, 0, sizeof(TerrainVertexData));

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
