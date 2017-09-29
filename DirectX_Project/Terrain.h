#pragma once

#include <vector>

#include "Vector3D.h"
#include "TerrainRenderer.h"

class Terrain
{
private:
	std::vector <TerrainVertexData> vertexes;
	TerrainRenderer *terrainRenderer = nullptr;

	int width;
	int height;
	float tile;
public:
	Terrain(int w, int h, float t);
	~Terrain();

	void CreateGraphics(TerrainRenderer *tr);

	void Clear();
};

