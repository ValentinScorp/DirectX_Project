#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "ObjectFactory.h"
#include "Renderer.h"
#include "Scene.h"
#include "UserInput.h"
#include "Clock.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

Renderer *renderer = nullptr;
Scene *scene = nullptr;
UserInput *userInput = nullptr;

ObjectFactory *objectFactory = nullptr;

Camera camera(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
Clock clock;

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

	HWND hWnd = CreateWindowW(L"My DirectX Project", L"DirectX Project", WS_OVERLAPPEDWINDOW, 400, 200, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == NULL) {
		return -1;
	}

	SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	userInput = new UserInput();

	renderer = new Renderer(&camera);
	renderer->Initialize(hWnd);
	renderer->SetUserInput(userInput);
	
	objectFactory = new ObjectFactory(renderer);

	scene = new Scene();
	scene->Initialize(renderer, objectFactory);

	renderer->SetCamera(scene->GetActiveCamera());

	userInput->RegisterListener(scene->GetActiveCamera());
	userInput->RegisterListener(scene);

	renderer->InitializeLightAndMaterials();
		
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			clock.Update();
			scene->UpdateUnits(clock.GetDeltaTime());
			renderer->BeginScene();
			renderer->Draw();
			renderer->EndScene();
			userInput->ClearMessages();						
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

	if (userInput != nullptr) {
		userInput->ClearMessages();
		delete userInput;
	}
	UnregisterClass(L"My DirectX Project", hInstance);

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	short delta = 0;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	POINT mMousePos;
	GetCursorPos(&mMousePos);
	ScreenToClient(hWnd, &mMousePos);	
	RECT windowRect;
	if (1/*!gWindow->IsFullscreen()*/) {
		
		GetWindowRect(hWnd, &windowRect);
		
		//mMousePos.x = mMousePos.x - windowRect.left;
		//mMousePos.y = mMousePos.y - windowRect.top;
	}
	//mMousePos.y = 600 - mMousePos.y;

	//xPos = mMousePos.x;
	//yPos = mMousePos.y;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_MOUSEWHEEL:
		delta = GET_WHEEL_DELTA_WPARAM(wParam);		
		if (delta != 0) {			
			userInput->SendMessage(Message("user_input", "mouse_wheel", xPos, yPos, delta));
		}
		break;
	case WM_LBUTTONDOWN:	
		userInput->SendMessage(Message("user_input", "left_mouse_button_down", xPos, yPos, 0));
		break;
	case WM_LBUTTONUP:
		userInput->SendMessage(Message("user_input", "left_mouse_button_up", xPos, yPos, 0));		
		break;
	case WM_RBUTTONDOWN:
		userInput->SendMessage(Message("user_input", "right_mouse_button_down", xPos, yPos, 0));
		break;
	case WM_RBUTTONUP:		
		userInput->SendMessage(Message("user_input", "right_mouse_button_up", xPos, yPos, 0));
		break;
	case WM_MOUSEMOVE:
		//userInput->PlaceMessage(UserMessage(xPos, yPos, MouseMove, MouseMove, delta));
		userInput->SendMessage(Message("user_input", "mouse_move", xPos, yPos, 0));
		break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
