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

	pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &pDevice);
}

void Renderer::InitializeGeometry()
{
	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, },    // side 1
		{ 3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, },
		{ -3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, },    // side 2
		{ -3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, },
		{ 3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, },

		{ -3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, },    // side 3
		{ -3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, },    // side 4
		{ 3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, },
		{ -3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, },

		{ 3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, },    // side 5
		{ 3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, },
		{ 3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, },    // side 6
		{ -3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, },
		{ -3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
		{ -3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },
	};

	// create a vertex buffer interface called v_buffer
	pDevice->CreateVertexBuffer(24 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	// create the indices using an int array
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	// create an index buffer interface called i_buffer
	pDevice->CreateIndexBuffer(36 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer,
		NULL);

	// lock i_buffer and load the indices into it
	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer->Unlock();


	HRESULT hr = D3D_OK;

	hr = D3DXCreateTextureFromFile(pDevice, L"brown-paper.bmp", &g_texture);
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

	pDevice->SetTexture(0, g_texture);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));
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

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 8.0f, 25.0f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),     // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProjection);
	// set the world transform

	index += 0.03f;
	D3DXMATRIX matRotateY;
	D3DXMatrixRotationY(&matRotateY, index);
	pDevice->SetTransform(D3DTS_WORLD, &(matRotateY));
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	pDevice->SetIndices(i_buffer);

	// draw the cube
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
	
	D3DXMATRIX matrixMoveX;
	D3DXMatrixTranslation(&matrixMoveX, 0.0f, 5.0f, 0.0f);
	pDevice->SetTransform(D3DTS_WORLD, &(matrixMoveX));
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}

void Renderer::EndScene()
{
	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void Renderer::Destroy()
{
	if (v_buffer) {
		v_buffer->Release();
		v_buffer = NULL;
	}
	if (i_buffer) {
		i_buffer->Release();
		i_buffer = NULL;
	}
	if (g_texture) {
		g_texture->Release();
		g_texture = NULL;
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
