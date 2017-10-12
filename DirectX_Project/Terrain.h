#pragma once

#include <vector>

#include "TerrainRenderer.h"
#include "TerrainPatch.h"
#include "Camera.h"

class Terrain
{
private:
	std::vector <TerrainPatch> patches;
	TerrainRenderer *terrainRenderer = nullptr;
	
	int width;
	int height;
	int patchDimention;
	float tile;
public:
	Terrain(int w, int h, float t);
	~Terrain();

	void CreateGraphics(TerrainRenderer *tr);

	D3DXVECTOR3 GetTerraneIntersection(RayVector rv);
};

