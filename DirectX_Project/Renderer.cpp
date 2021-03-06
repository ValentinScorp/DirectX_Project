#include "Renderer.h"

struct line_vertex
{	
	float x, y, z;
	DWORD color;	
};
const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;


Renderer::Renderer(Camera *cam)	
{
	camera = cam;
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
	

	//camPosition.x = 0.0f;
	//camPosition.y = -25.0f;
	//camPosition.z = 8.0f;
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

void Renderer::AddTerrainBrush(TerrainBrush * tb)
{
	pDevice->CreateVertexBuffer(6 * sizeof(TERRBRUSHVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, TERRBRUSHFVF, D3DPOOL_DEFAULT, &terrBrushVBuffer, NULL);

	

	terrainBrush = tb;
}

void Renderer::AddVertexes(CUSTOMVERTEX * vertexes, int vertexesNumb)
{
	pDevice->CreateVertexBuffer(vertexesNumb * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &v_buffer, NULL);
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

void Renderer::AttachMesh(Mesh * m)
{
	meshes.push_back(m);
}

void Renderer::AllocateVideoMemory()
{
	int vertexNum = 0;
	for (auto ro : robjects) {		
		std::string tn(ro->mesh->GetTexture());
		std::wstring texname(tn.begin(), tn.end());
		size_t id = CreateTexture(texname);
		ro->mesh->SetTextureId(id);
		
		vertexNum = vertexNum + ro->mesh->GetVertexesNum();
	}
		
	pDevice->CreateVertexBuffer(vertexNum * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, CUSTOMFVF, D3DPOOL_DEFAULT, &v_buffer, NULL);
}

size_t Renderer::CreateTexture(std::wstring fileName)
{
	IDirect3DTexture9* tex = nullptr;
	D3DXCreateTextureFromFile(pDevice, fileName.c_str(), &tex);
	textures.push_back(tex);
	return (textures.size() - 1);
}

void Renderer::AttachCamera(Camera * cam)
{
	if (cam != 0) {
		camera = cam;
	}
}

void Renderer::AddGameObject(GameObject * go)
{
	auto robj = new RenderObject();	
	
	robj->mesh = go->GetAnimatedMesh();
	robj->rbody = go->GetRigidBody();
	
	robjects.push_back(robj);	
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

	D3DXVECTOR3 endPos;
	
	// draw text
	std::wstring text = L"no text";
	RECT R = { 0, 0, 0, 0 };
	font->DrawText(0, text.c_str(), -1, &R, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matView1;
	D3DXMatrixLookAtLH(&matView1,
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f),     // the look-at position
		&D3DXVECTOR3(0.0f, -1.0f, 0.0f));    // the up direction

	D3DXMATRIX matView = camera->GetTransformMatrix();

	pDevice->SetTransform(D3DTS_VIEW, &matView);
	
	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(camera->GetFovy()), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, camera->GetNearPlane(), camera->GetFarPlane());	
	pDevice->SetTransform(D3DTS_PROJECTION, &matProjection);
	// set the world transform
	terrainRenderer->Render();

	// cam line
	line_vertex line_vertices[] = {
		{ 0.0f, 0.0f, 0.0f , D3DCOLOR_XRGB(255, 255, 255) },
		{ 0.0f, 0.0f, 10.0f, D3DCOLOR_XRGB(255, 255, 255) },
	};
	line_vertices[0].x = camera->GetPosition().x;
	line_vertices[0].y = camera->GetPosition().y;
	line_vertices[0].z = camera->GetPosition().z;

	line_vertices[1].x = camera->GetPosition().x;
	line_vertices[1].y = camera->GetPosition().y;
	line_vertices[1].z = camera->GetPosition().z;

	pDevice->SetRenderState(D3DRS_COLORVERTEX, true);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	pDevice->SetFVF(line_fvf);
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, line_vertices, sizeof(line_vertex));

	// -----------
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);	
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);	
	
	pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	pDevice->SetIndices(i_buffer);

	int vertexNum = 0;	
	
	for (auto ro : robjects) {
		D3DXMATRIX matTransform = ro->rbody->GetTransformationMatrix();;
		pDevice->SetTransform(D3DTS_WORLD, &matTransform);
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
				
		pDevice->SetFVF(CUSTOMFVF);		

		VOID *pVoid;				
		if (ro != nullptr) {
			Mesh *m = ro->mesh;
			if (m != nullptr) {
				size_t texid = m->GetTextureId();
				pDevice->SetTexture(0, textures[texid]);

				size_t vertexSize = m->GetVertexSize();
				size_t meshVertexesNum = m->GetVertexesNum();

				v_buffer->Lock(vertexNum * vertexSize, meshVertexesNum * vertexSize, (void**)&pVoid, D3DLOCK_DISCARD);
				memcpy(pVoid, m->GetVertexes(), meshVertexesNum * vertexSize);
				v_buffer->Unlock();
				pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, vertexNum, meshVertexesNum / 3);
				vertexNum = vertexNum + meshVertexesNum;
			}
		}		
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);    // turn on the color blending
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // set source factor
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // set dest factor
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);    // set the operation

	pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DBLENDOP_ADD);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	D3DXMATRIX matTransform;
	D3DXMatrixIdentity(&matTransform);
	D3DXMatrixTranslation(&matTransform, terrainBrush->GetX(), terrainBrush->GetY(), 0.2f);
	pDevice->SetTransform(D3DTS_WORLD, &matTransform);

	pDevice->SetFVF(TERRBRUSHFVF);
	pDevice->SetStreamSource(0, terrBrushVBuffer, 0, sizeof(TERRBRUSHVERTEX));

	float hw = terrainBrush->GetWidth() / 2.0f;

	TERRBRUSHVERTEX tbVerts[] = {
		{ -hw, -hw, 0.01f, D3DCOLOR_ARGB(100, 84, 230, 230) },
		{ hw, hw, 0.01f, D3DCOLOR_ARGB(100, 84, 230, 230) },
		{ -hw, hw, 0.01f, D3DCOLOR_ARGB(100, 84, 230, 230) },

		{ -hw, -hw, 0.01f, D3DCOLOR_ARGB(100, 84, 230, 230) },
		{ hw, -hw, 0.01f, D3DCOLOR_ARGB(100, 84, 230, 230) },
		{ hw, hw, 0.01f, D3DCOLOR_ARGB(100, 84, 230, 230) },
	};

	VOID *pVoid = nullptr;
	terrBrushVBuffer->Lock(0, 6 * sizeof(TERRBRUSHVERTEX), (void**)&pVoid, D3DLOCK_DISCARD);
	memcpy(pVoid, tbVerts, 6 * sizeof(TERRBRUSHVERTEX));
	terrBrushVBuffer->Unlock();

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

void Renderer::EndScene()
{
	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void Renderer::SetCamera(Camera * cam)
{
	camera = cam;
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
	if (terrBrushVBuffer) {
		terrBrushVBuffer->Release();
		terrBrushVBuffer = NULL;
	}	
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i]) {
			textures[i]->Release();
			textures[i] = NULL;
		}
	}	

	for (auto ro : robjects) {
		if (ro != nullptr) {
			delete ro;
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

void Renderer::OnMessage(Message mess)
{
}

Renderer::~Renderer()
{
}
