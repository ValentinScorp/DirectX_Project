#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "ObjectFactory.h"
#include "Renderer.h"
#include "Scene.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

Renderer *renderer = nullptr;
Scene *scene = nullptr;

ObjectFactory *objectFactory = nullptr;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"My DirectX Project";
	wcex.hIconSm = NULL;
	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowW(L"My DirectX Project", L"DirectX Project", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == NULL) {
		return -1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	renderer = new Renderer();
	renderer->Initialize(hWnd);

	objectFactory = new ObjectFactory(renderer);

	scene = new Scene();
	scene->Initialize(renderer, objectFactory);

	renderer->InitializeLightAndMaterials();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			renderer->BeginScene();
			renderer->Draw();
			renderer->EndScene();
		}
	}

	if (scene != nullptr) {
		scene->Destroy();
		delete scene;
		scene = nullptr;
	}

	if (objectFactory != nullptr) {
		delete objectFactory;
	}

	if (renderer != nullptr) {
		renderer->Destroy();
		delete renderer;
		renderer = nullptr;
	}

	UnregisterClass(L"My DirectX Project", hInstance);

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
