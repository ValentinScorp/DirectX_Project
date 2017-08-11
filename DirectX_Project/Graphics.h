#pragma once
#include <d3d9.h>
#include <d3dx9.h>

void createDevice(HWND hWnd);
void initializeGraphics();
void initializeLight();
void attachGraphicsToDevice();

void initializeOther();
void loadTexture();

void drawAll();

void clearAll();

