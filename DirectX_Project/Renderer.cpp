#include "Renderer.h"


Renderer::Renderer()
{
}

void Renderer::Initialize(HWND hWnd)
{
	IDirect3D9*	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	
	pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &pDevice);

	// creating font
	D3DXFONT_DESC fontDesc;
	fontDesc.Height = 24;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(fontDesc.FaceName, L"Times New Roman");
	D3DXCreateFontIndirect(pDevice, &fontDesc, &font);

	terrainRenderer = new TerrainRenderer(pDevice);
	

	camPosition.x = 0.0f;
	camPosition.y = -25.0f;
	camPosition.z = 8.0f;
}

void Renderer::InitializeLightAndMaterials()
{
	D3DLIGHT9 light;    // create the light struct
	D3DMATERIAL9 material;

	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse.r = 0.5f;
	light.Diffuse.g = 0.5f;
	light.Diffuse.b = 0.5f;
	light.Diffuse.a = 1.0f;
	light.Direction.x = 10.0f;
	light.Direction.y = 1.3f;
	light.Direction.z = -1.0f;

	light.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);


	pDevice->SetLight(0, &light);    // send the light struct properties to light #0
	pDevice->LightEnable(0, TRUE);    // turn on light #0

	ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);    // set ambient color to black
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white
	material.Power = 5.0f;
	pDevice->SetMaterial(&material);    // set the globably-used material to &material

	//pDevice->SetTexture(0, g_texture);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Renderer::AddVertexes(CUSTOMVERTEX * vertexes, int vertexesNumb)
{
	pDevice->CreateVertexBuffer(vertexesNumb * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &v_buffer, NULL);

	//VOID* pVoid;

//	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
//	memcpy(pVoid, vertexes, sizeof(CUSTOMVERTEX) * vertexesNumb);
//	v_buffer->Unlock();
}

void Renderer::AddIndexes(int * indexes, int indexesNumb)
{
	pDevice->CreateIndexBuffer(indexesNumb * sizeof(int), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &i_buffer, NULL);
	
	VOID* pVoid;
	
	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indexes, sizeof(int) * indexesNumb);
	i_buffer->Unlock();	
}

void Renderer::SetUserInput(UserInput * up)
{
	userInput = up;
}

void Renderer::SendData(std::vector<GameObject*> &objects)
{
	graph_objects = &objects;

	int vertexNum = 0;
	int indexNum = 0;
	for (int i = 0; i < objects.size(); i++) {
		GameObject *obj = objects[i];
		vertexNum = vertexNum + obj->GetVertexNum();
		indexNum = indexNum + obj->GetIndexNum();		
	}

	std::vector<VertexData> vert;
	std::vector<int> ind;
	for (int i = 0; i < objects.size(); i++) {
		GameObject *obj = objects[i];
		for (int j = 0; j < obj->GetVertexNum(); j++) {
			vert.push_back(obj->vertexes[j]);
		}		
		for (int j = 0; j < obj->GetIndexNum(); j++) {
			ind.push_back(obj->indexes[j]);
		}
		IDirect3DTexture9* tex = nullptr;
		D3DXCreateTextureFromFile(pDevice, obj->texture.c_str(), &tex);
		obj->textureId = textures.size();
		textures.push_back(tex);
	}

	pDevice->CreateVertexBuffer(vertexNum * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &v_buffer, NULL);
		
	//AddVertexes((CUSTOMVERTEX*)&vert[0], vertexNum);
	//AddIndexes((int*)&ind[0], indexNum);	
}

void Renderer::CreateTexture(wchar_t * fileName)
{
	//HRESULT hr = D3D_OK;

	//hr = D3DXCreateTextureFromFile(pDevice, fileName, &g_texture);	
}

void Renderer::BeginScene()
{
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pDevice->BeginScene();
}

void Renderer::Draw()
{
	pDevice->SetFVF(CUSTOMFVF);

	// draw text
	std::wstring wheelDelta = L"no text";
	if (userInput != nullptr) {
		userInput->BeginSearch();
		UserMessage *um;
		while (um = userInput->GetNextMessage()) {
			if (um->delta != 0) {
				wheelDelta = std::to_wstring(um->delta);
				camPosition.z += (um->delta / 60);
			}
		}
	}
	RECT R = { 0, 0, 0, 0 };
	font->DrawText(0, wheelDelta.c_str(), -1, &R, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&camPosition,    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),     // the look-at position
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f));    // the up direction
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProjection);
	// set the world transform
	terrainRenderer->Render();

	pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	pDevice->SetIndices(i_buffer);

	index += 0.01f;
	D3DXMATRIX matRotateZ;
	D3DXMATRIX matTransl;
	D3DXMATRIX matTransform;
	D3DXMatrixIdentity(&matTransform);	
	D3DXMatrixRotationZ(&matRotateZ, index);
	D3DXMatrixTranslation(&matTransl, 3.0f, 0.0f, 0.0f);
	int vertexNum = 0;	
	
	

	if (graph_objects != nullptr) {
		for (int i = 0; i < (*graph_objects).size(); i++) {
			GameObject *go = (*graph_objects)[i];
			go->animate();
		}

		for (int i = 0; i < (*graph_objects).size(); i++) {
			GameObject *go = (*graph_objects)[i];
			D3DXMatrixTranslation(&matTransl, go->position.x, go->position.y, go->position.z);

			matTransform = matRotateZ * matTransl;

			pDevice->SetTransform(D3DTS_WORLD, &(matTransform));
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

			pDevice->SetTexture(0, textures[go->textureId]);
			pDevice->SetFVF(CUSTOMFVF);

			VOID *pVoid;

			v_buffer->Lock(vertexNum * sizeof(VertexData), go->GetVertexNum() * sizeof(VertexData), (void**)&pVoid, D3DLOCK_DISCARD);

			memcpy(pVoid, go->GetVertexes(), go->GetVertexNum() * sizeof(VertexData));

			v_buffer->Unlock();

			pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, vertexNum, go->GetVertexNum() / 3);

			vertexNum = vertexNum + go->GetVertexNum();
		}
	}
}

void Renderer::EndScene()
{
	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void Renderer::Destroy()
{
	if (font != NULL)
	{
		font->Release();
		font = NULL;
	}

	if (v_buffer) {
		v_buffer->Release();
		v_buffer = NULL;
	}
	if (i_buffer) {
		i_buffer->Release();
		i_buffer = NULL;
	}

	for (int i = 0; i < textures.size(); i++) {
		if (textures[i]) {
			textures[i]->Release();
			textures[i] = NULL;
		}
	}	

	if (terrainRenderer != nullptr) {
		terrainRenderer->Destroy();
		delete terrainRenderer;
	}
	if (pDevice) {
		pDevice->Release();
		pDevice = NULL;
	}
	if (pDirect3D) {
		pDirect3D->Release();
		pDirect3D = NULL;
	}
}

Renderer::~Renderer()
{
}
