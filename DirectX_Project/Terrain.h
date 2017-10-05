#pragma once

#include <vector>

#include "TerrainRenderer.h"
#include "Camera.h"

class Terrain
{
private:
	std::vector <Triangle> triangles;
	TerrainRenderer *terrainRenderer = nullptr;

	int width;
	int height;
	float tile;
public:
	Terrain(int w, int h, float t);
	~Terrain();

	void CreateGraphics(TerrainRenderer *tr);
	TerrainRenderer* GetTerrainRenderer();

	void Clear();
};

