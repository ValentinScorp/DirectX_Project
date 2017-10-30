#pragma once
#include <d3dx9.h>

struct TERRBRUSHVERTEX {
	FLOAT x, y, z;
	D3DCOLOR diffuse;
	
};
#define TERRBRUSHFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class TerrainBrush
{
private:
	float posX = 0;
	float posY = 0;
	float width = 4.0f;

public:
	TerrainBrush();
	~TerrainBrush();

	float GetWidth();
	float GetX();
	float GetY();

	void SetX(float x);
	void SetY(float y);
};

